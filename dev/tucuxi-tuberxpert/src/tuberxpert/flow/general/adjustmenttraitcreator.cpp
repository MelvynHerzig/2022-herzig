#include "adjustmenttraitcreator.h"

#include <chrono>

#include "tucucommon/unit.h"
#include "tucucore/intakeevent.h"
#include "tucucore/intakeextractor.h"
#include "tucucore/computingservice/computingresult.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

AdjustmentTraitCreator::AdjustmentTraitCreator(Common::DateTime _computationTime) : m_computationTime(_computationTime)
{}

void AdjustmentTraitCreator::perform(XpertRequestResult& _xpertRequestResult) const
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

    // Points per hour
    double nbPointsPerHour = 20;

    // Computing options
    Core::ComputingOption computingOption{
        getPredictionParameterType(_xpertRequestResult.getTreatment()),
        Core::CompartmentsOption::AllActiveMoieties,
        Core::RetrieveStatisticsOption::RetrieveStatistics,
        Core::RetrieveParametersOption::RetrieveParameters,
        Core::RetrieveCovariatesOption::RetrieveCovariates
    };

    // Adjustment date
    Common::DateTime adjustmentTime = getAdjustmentTime(_xpertRequestResult.getXpertRequest(), _xpertRequestResult.getTreatment(), _xpertRequestResult.getDrugModel());

    // Period

    Common::DateTime start;
    Common::DateTime end;

    try {

        getPeriod(fullFormulationAndRoute, _xpertRequestResult, adjustmentTime, start, end);

    } catch (const invalid_argument& e) {
        // We catch the fact the the treatment may already be over
        _xpertRequestResult.setErrorMessage(string(e.what()));
        return;
    }

    // Best candidate option
    Core::BestCandidatesOption candidatesOption = Core::BestCandidatesOption::BestDosagePerInterval;

    // Loading option
    Core::LoadingOption loadingOption = getLoadingOption(_xpertRequestResult.getXpertRequest(), fullFormulationAndRoute);

    // Rest period option
    Core::RestPeriodOption restPeriodOption = getRestPeriodOption(_xpertRequestResult.getXpertRequest(), fullFormulationAndRoute);

    // Steady state target option
    Core::SteadyStateTargetOption steadyStateTargetOption = getSteadyStateTargetOption(fullFormulationAndRoute);

    // Target extraction option
    Core::TargetExtractionOption targetExtractionOption = _xpertRequestResult.getXpertRequest().getTargetExtractionOption();

    // Formulation and route selection option
    Core::FormulationAndRouteSelectionOption formulationAndRouteSelectionOption = _xpertRequestResult.getXpertRequest().getFormulationAndRouteSelectionOption();


    // Creating and setting trait.
    Core::ComputingTraitAdjustment computingTraitAdjustment = Core::ComputingTraitAdjustment(
                responseId,
                start,
                end,
                nbPointsPerHour,
                computingOption,
                adjustmentTime,
                candidatesOption,
                loadingOption,
                restPeriodOption,
                steadyStateTargetOption,
                targetExtractionOption,
                formulationAndRouteSelectionOption);

    _xpertRequestResult.setAdjustmentTrait(computingTraitAdjustment);

}

Core::PredictionParameterType AdjustmentTraitCreator::getPredictionParameterType(const std::unique_ptr<Core::DrugTreatment>& _drugTreatment) const
{
    if (!_drugTreatment->getDosageHistory().isEmpty() && !_drugTreatment->getSamples().empty()){
        return Core::PredictionParameterType::Aposteriori;
    } else {
        return Core::PredictionParameterType::Apriori;
    }
}

Common::DateTime AdjustmentTraitCreator::getOldestDosageTimeRangeStart(const Core::DosageHistory& _dosageHistory) const
{
    // In case the dosage history is empty. The olest treatment start date is present.
    Common::DateTime oldestDateKnown = m_computationTime;

    // Iterate on the time ranges and find the one that is the oldest.
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
        if (timeRange->getStartDate() < m_computationTime && (latestDateKnown.isUndefined() || timeRange->getStartDate() < latestDateKnown)){
            latestDateKnown = timeRange->getStartDate();
        }
    }

    return latestDateKnown;
}

Common::DateTime AdjustmentTraitCreator::getAdjustmentTime(const XpertRequestData& _request,
                                                           const std::unique_ptr<Core::DrugTreatment>& _drugTreatment,
                                                           const Core::DrugModel* _drugModel) const
{
    // If an adjustment time has been manually set in the request, the use it.
    if (!_request.getAdjustmentTime().isUndefined()){
        return _request.getAdjustmentTime();

    // Otherwise
    } else {    

        // There are 3 possible scenarios left:
        // 1) There is an ongoing treatment -> The next intake after computation time is the adjustment time.
        // 2) There was a treatment (that is over by now) -> Take the latest intake and add as many times as necessary 2*half-life to reach _start.
        //    The resulting time is the adjustment time.
        // 3) There is no dosage history in the past -> The adjustment time can be anytime, arbitrarily computation time + 1 hour.

        // We handle 1 and 2 (in makeIntakeSeriesAndTryToExtractAdjustmentTime) before 3, because there are some computations that could "fail",
        // in that case we fallback into 3.

        // If the dosage history is not empty we might be in 1 and 2.
        if (!_drugTreatment->getDosageHistory().isEmpty()){
            Common::DateTime adjustmentBasedOnIntakes = makeIntakeSeriesAndTryToExtractAdjustmentTime(_drugTreatment, _drugModel);

            // We check 1 or 2 succeed.
            if(!adjustmentBasedOnIntakes.isUndefined()) {
                return adjustmentBasedOnIntakes;
            }
        }

        // 3) There is no dosage history in the past -> The adjustment time can be anytime, arbitrarily start datetime + 1 hour.
        // or fallback from 1 and 2
        return m_computationTime + Duration(chrono::hours(1));
    }
}

Common::DateTime AdjustmentTraitCreator::makeIntakeSeriesAndTryToExtractAdjustmentTime(const std::unique_ptr<Core::DrugTreatment>& _drugTreatment,
                                                                                       const Core::DrugModel* _drugModel) const
{
    // get the latest (in the past) dosage time range start as the extraction starting time.
    Common::DateTime startTimeOfTheLatestDosage = getLatestDosageTimeRangeStart(_drugTreatment->getDosageHistory());

    // If time is not undefined. We check that because we look for the latest time from the dosage history before the the computation time.
    // I imagine that it is possible to have dosage history only in the future which would lead to an undefined latest dosage time range start.
    // In that case, just return undefined date time because the next steps have no sense.
    if (startTimeOfTheLatestDosage.isUndefined()) {
        return Common::DateTime::undefinedDateTime();
    }

    // To define the adjustment time based on the intakes, we need to extract the intakes.
    Core::IntakeExtractor intakeExtractor;
    Core::IntakeSeries intakes;
    Core::ComputingStatus cs = intakeExtractor.extract(_drugTreatment->getDosageHistory(),
                                                       startTimeOfTheLatestDosage,
                                                       Common::DateTime::undefinedDateTime(),
                                                       1,
                                                       Common::TucuUnit("mg"),
                                                       intakes);

    // Now, we look for the closest intake in the future. If there is none, for the last in the past.
    Common::DateTime possibleAdjustmentTime = getTimeOfNearestFutureOrLatestIntake(intakes);

    // For some reasons, the intakes series could be empty. In that case just return undefined datetime.
    if (cs != Core::ComputingStatus::Ok || possibleAdjustmentTime.isUndefined()) {
        return Common::DateTime::undefinedDateTime();
    }

    // The extracted intake time is in the past.
    if (possibleAdjustmentTime < m_computationTime) {

        // Add 2 * helf-life until the computing time is reached, it defines the adjustment time.
        const Core::HalfLife& halfLife = _drugModel->getTimeConsiderations().getHalfLife();
        while (possibleAdjustmentTime < m_computationTime) {
            double timeValue = Common::UnitManager::convertToUnit(halfLife.getValue(), halfLife.getUnit(), Common::TucuUnit("h"));
            possibleAdjustmentTime += Common::Duration(chrono::hours(int(2 * timeValue)));
        }
    }

    // At that point, the possible adustment time is:
    // - The closest intake in the future -> use it as it ise
    // or
    // - The latest intake in the past on which we added the half life until it reached the computation time.
    return possibleAdjustmentTime;
}

Common::DateTime AdjustmentTraitCreator::getTimeOfNearestFutureOrLatestIntake(Core::IntakeSeries _intakes) const
{
    // Saved time to return
    Common::DateTime savedTime = Common::DateTime::undefinedDateTime();

    // We look from the end of the intakes for the nearest intake in the future
    // or for the latest in the path if there is none in the future.
    for (auto it = _intakes.rbegin(); it != _intakes.rend(); ++it) {

        // Since we iterate from the end, we remember of every future intake
        // that is closest to the execution time.
        if ( savedTime.isUndefined() ||
             ((*it).getEventTime() < savedTime && (*it).getEventTime() > m_computationTime)) {
            savedTime = (*it).getEventTime();
        }

        // If we are in the past.
        if ((*it).getEventTime() < m_computationTime) {

            // There is a close intake in the future
            if (!savedTime.isUndefined()){
                return savedTime;
            // There is no future intake.
            } else {
                return (*it).getEventTime();
            }
        }
    }

    // There is no intake in the series.
    return savedTime; // undefined date.
}

void AdjustmentTraitCreator::getPeriod(const Core::FullFormulationAndRoute* _fullFormulationAndRoute,
                                       const XpertRequestResult& _xpertRequestResult,
                                       const Common::DateTime& _adjustmentTime,
                                       Common::DateTime& _start,
                                       Common::DateTime& _end) const
{
    // If the full formulation and route defines a standard treatment with a fixed duration.
    if (_fullFormulationAndRoute->getStandardTreatment() != nullptr && _fullFormulationAndRoute->getStandardTreatment()->getIsFixedDuration()) {

        // Getting treatment start
        _start = getOldestDosageTimeRangeStart(_xpertRequestResult.getTreatment()->getDosageHistory());

        // Getting treatment end ( start + standard treatment duration )
        double treatmentDuration = _fullFormulationAndRoute->getStandardTreatment()->getDuration();
        treatmentDuration = Common::UnitManager::convertToUnit(treatmentDuration, _fullFormulationAndRoute->getStandardTreatment()->getUnit(), Common::TucuUnit("d"));
        _end = _start + Common::Duration(Common::days(int(treatmentDuration)));

        if (_end <= _adjustmentTime) {
            throw invalid_argument("Based on the standard treatment in the model:" +
                                   _xpertRequestResult.getDrugModel()->getDrugModelId() +
                                   ", considering that the oldest dosage is the treatment start, the treatment is already over at the time of the adjustment.");
        }

        return;

    // If no standard treatment.
    } else {
        _start = _adjustmentTime - Common::Duration(std::chrono::hours(int(1)));;
        _end = _start + Common::Duration(Common::days(int(7)));
    }
}

Core::LoadingOption AdjustmentTraitCreator::getLoadingOption(const XpertRequestData& _request,
                                                             const Core::FullFormulationAndRoute* _fullFormulationAndRoute) const
{
    switch (_request.getLoadingOption()) {
    case LoadingOption::LoadingDoseAllowed : return Core::LoadingOption::LoadingDoseAllowed;
    case LoadingOption::NoLoadingDose : return Core::LoadingOption::NoLoadingDose;
    case LoadingOption::Unspecified :
        return _fullFormulationAndRoute->isLoadingDoseRecommended() ? Core::LoadingOption::LoadingDoseAllowed : Core::LoadingOption::NoLoadingDose;
    }
}

Core::RestPeriodOption AdjustmentTraitCreator::getRestPeriodOption(const XpertRequestData& _request,
                                                                   const Core::FullFormulationAndRoute* _fullFormulationAndRoute) const
{
    switch (_request.getRestPeriodOption()) {
    case RestPeriodOption::RestPeriodAllowed : return Core::RestPeriodOption::RestPeriodAllowed;
    case RestPeriodOption::NoRestPeriod : return Core::RestPeriodOption::NoRestPeriod;
    case RestPeriodOption::Unspecified :
        return _fullFormulationAndRoute->isRestPeriodRecommended() ? Core::RestPeriodOption::RestPeriodAllowed : Core::RestPeriodOption::NoRestPeriod;
    }
}

Core::SteadyStateTargetOption AdjustmentTraitCreator::getSteadyStateTargetOption(const Core::FullFormulationAndRoute *_fullFormulationAndRoute) const
{
    // If the full formulation and route defines a standard treatment with a fixed duration.
    if (_fullFormulationAndRoute->getStandardTreatment() != nullptr) {
        return Core::SteadyStateTargetOption::WithinTreatmentTimeRange;
    } else {
        return Core::SteadyStateTargetOption::AtSteadyState;
    }
}

} // namespace Xpert
} // namespace Tucuxi
