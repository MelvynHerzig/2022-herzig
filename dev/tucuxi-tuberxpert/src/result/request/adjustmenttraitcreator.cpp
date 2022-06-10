#include "adjustmenttraitcreator.h"

#include <chrono>

#include "tucucommon/unit.h"
#include "tucucore/intakeevent.h"
#include "tucucore/intakeextractor.h"
#include "tucucore/computingservice/computingresult.h"

using namespace std;

namespace Tucuxi {
namespace XpertResult {

AdjustmentTraitCreator::AdjustmentTraitCreator(Common::DateTime _computationDate) : m_computationDate(_computationDate)
{}

void AdjustmentTraitCreator::createAdjustmentTrait(XpertRequestResult& _xpertRequestResult) const
{
    // Checks treatment
    if (_xpertRequestResult.getTreatment() == nullptr) {
        _xpertRequestResult.setErrorMessage("No treatment set.");
        return;
    }

    // Checks drug model
    if (_xpertRequestResult.getDrugModel() == nullptr) {
        _xpertRequestResult.setErrorMessage("No drug model set.");
        return;
    }

    // Get the corresponding formulation and route from the drug model.
    const Core::FormulationAndRoutes& modelFormulationAndRoutes = _xpertRequestResult.getDrugModel()->getFormulationAndRoutes();
       const Core::FullFormulationAndRoute* fullFormulationAndRoute = nullptr;

    if (_xpertRequestResult.getTreatment()->getDosageHistory().isEmpty()){
        fullFormulationAndRoute = modelFormulationAndRoutes.getDefault();
    } else {
        fullFormulationAndRoute = modelFormulationAndRoutes.get(_xpertRequestResult.getTreatment()->getDosageHistory().getLastFormulationAndRoute());
    }

    // ------ Preparing params ------

    // Response identifier
    string responseId = "";

    // Period
    Common::DateTime start;
    Common::DateTime end;
    getPeriod(fullFormulationAndRoute, _xpertRequestResult, start, end);

    // Points per hour
    double ptPerHour = 20;

    // Computing options
    Core::ComputingOption co{
        getPredictionParamterType(_xpertRequestResult.getTreatment()),
        Core::CompartmentsOption::AllActiveMoieties,
        Core::RetrieveStatisticsOption::RetrieveStatistics,
        Core::RetrieveParametersOption::RetrieveParameters,
        Core::RetrieveCovariatesOption::RetrieveCovariates
    };

    // Adjustment date
    Common::DateTime adjustmentDate = getAdjustmentDate(_xpertRequestResult.getXpertRequest(), _xpertRequestResult.getTreatment());
}

Core::PredictionParameterType AdjustmentTraitCreator::getPredictionParamterType(const std::unique_ptr<Core::DrugTreatment>& _drugTreatment) const
{
    if (!_drugTreatment->getDosageHistory().isEmpty() && !_drugTreatment->getSamples().empty()){
        return Core::PredictionParameterType::Aposteriori;
    } else {
        return Core::PredictionParameterType::Apriori;
    }
}

void AdjustmentTraitCreator::getPeriod(const Core::FullFormulationAndRoute* _fullFormulationAndRoute, const XpertRequestResult& _xpertRequestResult, Common::DateTime& _start, Common::DateTime& _end) const
{
    // If the full formulation and route defines a standard treatment with a fixed duration.
    if (_fullFormulationAndRoute->getStandardTreatment() != nullptr && _fullFormulationAndRoute->getStandardTreatment()->getIsFixedDuration()) {

        // Getting treatment start
        _start = getOldestDosageTimeRangeStart(_xpertRequestResult.getTreatment()->getDosageHistory());

        // Getting treatment end ( start + standard treatment duration )
        double treatmentDuration = _fullFormulationAndRoute->getStandardTreatment()->getDuration();
        treatmentDuration = Common::UnitManager::convertToUnit(treatmentDuration, _fullFormulationAndRoute->getStandardTreatment()->getUnit(), Common::TucuUnit("d"));
        _end = _start + Common::Duration(Common::days(int(treatmentDuration)));

        if (_end <= m_computationDate) {
            throw invalid_argument("Based on the standard treatment in the model:" +
                                   _xpertRequestResult.getDrugModel()->getDrugModelId() +
                                   ", considering the the oldest dosage is the treatment start, the treatment is already over.");
        }

        return;

    // If no standard treatment.
    } else {
        _start = m_computationDate;
        _end = _start + Common::Duration(Common::days(int(7)));
    }
}

Common::DateTime AdjustmentTraitCreator::getOldestDosageTimeRangeStart(const Core::DosageHistory& _dosageHistory) const
{
    // In case the dosage history is empty. The olest treatment start date is present.
    Common::DateTime oldestDateKnown = Common::DateTime::now();

    for (const unique_ptr<Core::DosageTimeRange>& timeRange : _dosageHistory.getDosageTimeRanges()) {
        if (timeRange->getStartDate() < oldestDateKnown){
            oldestDateKnown = timeRange->getStartDate();
        }
    }

    return oldestDateKnown;
}

Common::DateTime AdjustmentTraitCreator::getAdjustmentDate(const XpertQuery::XpertRequestData& _request, const std::unique_ptr<Core::DrugTreatment>& _drugTreatment) const
{

    string dateFormat = "%Y-%m-%dT%H:%M:%S";
    Core::IntakeExtractor intakeExtractor;
    Core::IntakeSeries intakes;
    Core::ComputingStatus cs = intakeExtractor.extract(_drugTreatment->getDosageHistory(), Tucuxi::Common::DateTime{"2018-07-06T06:00:00", dateFormat}, Tucuxi::Common::DateTime{"2018-07-06T16:00:00", dateFormat}, 1, Common::TucuUnit("mg"), intakes);
    for (const Core::IntakeEvent& intake : intakes) {
        cout << "intake time : " << intake.getEventTime() << " dose: " << intake.getDose() << " " << intake.getUnit().toString() << endl;
    }


    // If an adjustment date has been manually set in the request, the use it.
    if (!_request.getAdjustmentTime().isUndefined()){
        return _request.getAdjustmentTime();

    // Otherwise
    } else {

        // There are 3 possible scenarios:
        // 1) There is no dosage history -> The adjustment date can be anytime, arbitrarily execution time + 1 hour.
        // 2) There is an ongoing treatment -> The next intake is the adjustment date.
        // 3) There was a treatment (that is over by now) -> Take the last intake and add as many times as necessary 2*half-life to reach the execution time.
        //    The resulting time is the adjustment date.

        // 1) There is no dosage history -> The adjustment date can be anytime, arbitrarily execution time + 1 hour.
        if (_drugTreatment->getDosageHistory().isEmpty()) {
            return m_computationDate + Duration(chrono::hours(1));
        }
    }
}

} // namespace XpertResult
} // namespace Tucuxi
