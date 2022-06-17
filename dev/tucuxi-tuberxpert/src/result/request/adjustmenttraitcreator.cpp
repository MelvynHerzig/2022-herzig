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
    Common::DateTime adjustmentDate = getAdjustmentDate(_xpertRequestResult.getXpertRequest(), _xpertRequestResult.getTreatment(), _xpertRequestResult.getDrugModel());

    // Adjust the start/end period regardin the adjustment date, but not if standardTreatment.
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

Common::DateTime AdjustmentTraitCreator::getLatestDosageTimeRangeStart(const Core::DosageHistory &_dosageHistory) const
{
    // In case the dosage history is empty. The latest start time is undefined.
    Common::DateTime latestDateKnown = Common::DateTime::undefinedDateTime();

    for (const unique_ptr<Core::DosageTimeRange>& timeRange : _dosageHistory.getDosageTimeRanges()) {
        if (timeRange->getStartDate() < m_computationDate && (latestDateKnown.isUndefined() || timeRange->getStartDate() < latestDateKnown)){
            latestDateKnown = timeRange->getStartDate();
        }
    }

    return latestDateKnown;
}

Common::DateTime AdjustmentTraitCreator::getAdjustmentDate(const XpertQuery::XpertRequestData& _request,
                                                           const std::unique_ptr<Core::DrugTreatment>& _drugTreatment,
                                                           const Core::DrugModel* _drugModel) const
{
    // If an adjustment date has been manually set in the request, the use it.
    if (!_request.getAdjustmentTime().isUndefined()){
        return _request.getAdjustmentTime();

    // Otherwise
    } else {

        // TODO refactor into functions to use break / return instead of the boolean fall back

        // There are 3 possible scenarios:
        // 1) There is no dosage history in the past -> The adjustment date can be anytime, arbitrarily computation time + 1 hour.
        // 2) There is an ongoing treatment -> The next intake after computation time is the adjustment date.
        // 3) There was a treatment (that is over by now) -> Take the latest intake and add as many times as necessary 2*half-life to reach _start.
        //    The resulting time is the adjustment date.

        // We handle 2 and 3 before 1 because there are some computations that could "fail", in that case we fallback into 1.

        bool fallbackInto1 = false;

        // If the dosage history is not empty we might be in 2 and 3
        if (!_drugTreatment->getDosageHistory().isEmpty()){

             Common::DateTime startTimeOfTheLatestDosage = getLatestDosageTimeRangeStart(_drugTreatment->getDosageHistory());

             // If time is not undefined. We check that because we look for the latest time previous of the computation time from the dosage history.
             // I imagine it is possible to have dosage history in the future which would make the function to enter this branch.
             if (!startTimeOfTheLatestDosage.isUndefined()) {

                 // In order to do 2 & 3, we extract intakes from the latest dosage time range until the _end date time.
                 Core::IntakeExtractor intakeExtractor;
                 Core::IntakeSeries intakes;
                 Core::ComputingStatus cs = intakeExtractor.extract(_drugTreatment->getDosageHistory(),
                                                                    startTimeOfTheLatestDosage,
                                                                    Common::DateTime::undefinedDateTime(),
                                                                    1,
                                                                    Common::TucuUnit("mg"),
                                                                    intakes);

                 // Time of the nearest future intake or latest in the past.
                 Common::DateTime timeOfNearestFutureOrLatestIntake = getTimeOfNearestFutureOrLatestIntake(intakes);

                 // For some reason the intakes series could be empty and the result is undefined datetime.
                 // In that case fallback.
                 if (cs == Core::ComputingStatus::Ok && !timeOfNearestFutureOrLatestIntake.isUndefined()) {

                     // If the intake is in the future, use it as it is for the adjustment time
                     if (timeOfNearestFutureOrLatestIntake > m_computationDate) {
                         return timeOfNearestFutureOrLatestIntake;

                     // The last intake is in the past.
                     } else {

                         const Core::HalfLife& halfLife = _drugModel->getTimeConsiderations().getHalfLife();
                         while (timeOfNearestFutureOrLatestIntake < m_computationDate) {
                             double timeValue = Common::UnitManager::convertToUnit(halfLife.getValue(), halfLife.getUnit(), Common::TucuUnit("h"));
                             timeOfNearestFutureOrLatestIntake += Common::Duration(chrono::hours(int(timeValue)));
                         }
                     }

                     return timeOfNearestFutureOrLatestIntake;
                 }

             }

             fallbackInto1 = true;
        }




        // 1) There is no dosage history in the past -> The adjustment date can be anytime, arbitrarily start datetime + 1 hour.
        // or fallback from 2 and 3
        if (_drugTreatment->getDosageHistory().isEmpty() || fallbackInto1) {
            return m_computationDate + Duration(chrono::hours(1));
        }  
    }
}

Common::DateTime AdjustmentTraitCreator::getTimeOfNearestFutureOrLatestIntake(Core::IntakeSeries _intakes) const
{
    // Saved time tu return
    Common::DateTime savedTime = Common::DateTime::undefinedDateTime();

    // We look from the end of the intakes for the nearest intake in the future
    // or for the latest in the path if there is none in the future.
    for (auto it = _intakes.rbegin(); it != _intakes.rend(); ++it) {

        // Since we iterate from the end, we remember of every future intake
        // that is closest to the execution time.
        if ( savedTime.isUndefined() || (*it).getEventTime() < savedTime) {
            savedTime = (*it).getEventTime();
        }

        // If we are in the past.
        if ((*it).getEventTime() < m_computationDate) {

            // There is a close intake in the future
            if (!savedTime.isUndefined()){
                return savedTime;
            // There is no future intake
            } else {
                return (*it).getEventTime();
            }
        }
    }

    // There is no intake in the series.
    return savedTime; // undefined date.
}

} // namespace XpertResult
} // namespace Tucuxi
