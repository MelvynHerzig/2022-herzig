#include "requestexecutor.h"

#include <memory>

#include "tuberxpert/utils/xpertutils.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

RequestExecutor::RequestExecutor()
{}

void RequestExecutor::perform(XpertRequestResult& _xpertRequestResult)
{
    // Create a copy of the adjustment trait and prepare the response holder.
    unique_ptr<Core::ComputingTraitAdjustment> baseAdjustmentTrait = make_unique<Core::ComputingTraitAdjustment>(*_xpertRequestResult.getAdjustmentTrait());
    unique_ptr<Core::AdjustmentData> adjustmentResult = nullptr;

    // Execute request
    executeRequestAndGetResult<Core::ComputingTraitAdjustment, Core::AdjustmentData>(move(baseAdjustmentTrait), _xpertRequestResult, adjustmentResult);

    // Starting computation.

    // If request handling failed, return false.
    if (adjustmentResult == nullptr){

        _xpertRequestResult.setErrorMessage("Adjustment request execution failed.");
        return;

    // If it went well, set the response in the request result and return true.
    } else {

        if (adjustmentResult->getAdjustments().empty()) {
            _xpertRequestResult.setErrorMessage("No adjustment found.");
            return;
        }

        // Saving parameters
        _xpertRequestResult.addParameters(adjustmentResult->getAdjustments().front().getData().front().m_parameters);

        // Saving the adjustment data into the request result
        _xpertRequestResult.setAdjustmentData(move(adjustmentResult));

        // Get the statistics at steady state and the parameters
        gatherAdditionalData(_xpertRequestResult);

        return;
    }
}

void RequestExecutor::gatherAdditionalData(XpertRequestResult& _xpertRequestResult) const
{
    unique_ptr<Core::ComputingTraitAdjustment> baseAdjustmentTrait = make_unique<Core::ComputingTraitAdjustment>(*_xpertRequestResult.getAdjustmentTrait());

    // ------- statistics at steady state ---------

    // We update the end time to approximate a steady state in order to extract staistics at steady state.
    // It would have been ideal to do it with in one unique request but it is not possible to remove
    // the additional cycleData in the current state of the core.
    const Core::HalfLife& halfLife = _xpertRequestResult.getDrugModel()->getTimeConsiderations().getHalfLife();
    double hoursToAdd = Common::UnitManager::convertToUnit(halfLife.getValue(), halfLife.getUnit(), Common::TucuUnit("h"));
    Common::DateTime staeadyEndTime = baseAdjustmentTrait->getAdjustmentTime() + Duration(chrono::hours(int(halfLife.getMultiplier() * hoursToAdd)));

    // Create a computing trait that goes to steady state.
    unique_ptr<Core::ComputingTraitAdjustment> steadyAdustmentTrait = nullptr;
    tweakComputingTraitAdjustment(baseAdjustmentTrait,
                                  staeadyEndTime,
                                  20,
                                  baseAdjustmentTrait->getComputingOption().getParametersType(),
                                  steadyAdustmentTrait);
    unique_ptr<Core::AdjustmentData> steadyAdjustmentResult = nullptr;

    // Execute request
    executeRequestAndGetResult<Core::ComputingTraitAdjustment, Core::AdjustmentData>(move(steadyAdustmentTrait), _xpertRequestResult, steadyAdjustmentResult);

    if (steadyAdjustmentResult == nullptr){
        _xpertRequestResult.setErrorMessage("Failed to extract statistics at steady state.");
        return;
    } else {

        // Extracting the statistics at steady state for the best adjustment.
        _xpertRequestResult.setCycleStats(steadyAdjustmentResult->getAdjustments().front().getData().back().m_statistics);
    }

    // ------- Parameters "A priori" ---------

    // We don't want to extract them once again, so we check the base prediction type.
    if (baseAdjustmentTrait->getComputingOption().getParametersType() == Core::PredictionParameterType::Aposteriori) {

        // Create a computing trait a priori
        unique_ptr<Core::ComputingTraitAdjustment> aprioriAdustmentTrait = nullptr;
        tweakComputingTraitAdjustment(baseAdjustmentTrait,
                                      baseAdjustmentTrait->getEnd(),
                                      1,
                                      Core::PredictionParameterType::Apriori,
                                      aprioriAdustmentTrait);
        unique_ptr<Core::AdjustmentData> aprioriAdjustmentResult = nullptr;

        // Execute request
        executeRequestAndGetResult<Core::ComputingTraitAdjustment, Core::AdjustmentData>(move(aprioriAdustmentTrait), _xpertRequestResult, aprioriAdjustmentResult);

        if (aprioriAdjustmentResult == nullptr){
            _xpertRequestResult.setErrorMessage("Failed to extract apriori parameters.");
            return;
        } else {
            // Saving parameters
            _xpertRequestResult.addParameters(aprioriAdjustmentResult->getAdjustments().front().getData().front().m_parameters);
        }
    }

    // ------- Parameters "Typical patient" ---------

    // Create a computing trait for population
    unique_ptr<Core::ComputingTraitAdjustment> populationAdustmentTrait = nullptr;
    tweakComputingTraitAdjustment(baseAdjustmentTrait,
                                  baseAdjustmentTrait->getEnd(),
                                  1,
                                  Core::PredictionParameterType::Population,
                                  populationAdustmentTrait);
    unique_ptr<Core::AdjustmentData> populationAdjustmentResult = nullptr;

    // Execute request
    executeRequestAndGetResult<Core::ComputingTraitAdjustment, Core::AdjustmentData>(move(populationAdustmentTrait), _xpertRequestResult, populationAdjustmentResult);

    if (populationAdjustmentResult == nullptr){
        _xpertRequestResult.setErrorMessage("Failed to extract population parameters.");
        return;
    } else {
        // Saving parameters
        _xpertRequestResult.addParameters(populationAdjustmentResult->getAdjustments().front().getData().front().m_parameters);
    }
}

void RequestExecutor::tweakComputingTraitAdjustment(const unique_ptr<Core::ComputingTraitAdjustment>& _toCopy,
                                                          const Common::DateTime& _newEnd,
                                                          double _newNbPointsPerHour,
                                                          Core::PredictionParameterType _newPredictionType,
                                                          std::unique_ptr<Core::ComputingTraitAdjustment>& _resultingAdjustment) const
{

    // Make new computing option
    Core::ComputingOption newOptions{
        _newPredictionType,
        _toCopy->getComputingOption().getCompartmentsOption(),
        _toCopy->getComputingOption().retrieveStatistics(),
        _toCopy->getComputingOption().retrieveParameters(),
        _toCopy->getComputingOption().retrieveCovariates()
    };

    // Make new resulting adjustment trait
    _resultingAdjustment = make_unique<Core::ComputingTraitAdjustment>(
                "",
                _toCopy->getStart(),
                _newEnd,
                _newNbPointsPerHour,
                newOptions,
                _toCopy->getAdjustmentTime(),
                Core::BestCandidatesOption::BestDosage,
                _toCopy->getLoadingOption(),
                _toCopy->getRestPeriodOption(),
                _toCopy->getSteadyStateTargetOption(),
                _toCopy->getTargetExtractionOption(),
                _toCopy->getFormulationAndRouteSelectionOption()
                );
}

} // namespace Xpert
} // namespace Tucuxi
