#include "requestexecutor.h"

#include <memory>

#include "tuberxpert/utils/xpertutils.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

void RequestExecutor::perform(XpertRequestResult& _xpertRequestResult)
{
    // Check if there is an adjustment trait.
    if (_xpertRequestResult.getAdjustmentTrait() == nullptr) {
        _xpertRequestResult.setErrorMessage("No adjustment trait set.");
        return;
    }

    // Check if there is drug model.
    if (_xpertRequestResult.getDrugModel() == nullptr) {
        _xpertRequestResult.setErrorMessage("No drug model set.");
        return;
    }

    // Create a copy of the adjustment trait and prepare the response holder.
    unique_ptr<Core::ComputingTraitAdjustment> baseAdjustmentTrait = make_unique<Core::ComputingTraitAdjustment>(*_xpertRequestResult.getAdjustmentTrait());
    unique_ptr<Core::AdjustmentData> adjustmentResult = nullptr;

    // Execute the request
    executeRequestAndGetResult<Core::ComputingTraitAdjustment, Core::AdjustmentData>(move(baseAdjustmentTrait), _xpertRequestResult, adjustmentResult);

    // If the request execution failed
    if (adjustmentResult == nullptr){
        _xpertRequestResult.setErrorMessage("Adjustment request execution failed.");
        return;

    // If all went well.
    } else {

        if (adjustmentResult->getAdjustments().empty()) {
            _xpertRequestResult.setErrorMessage("No adjustment found.");
            return;
        }

        // Save the parameters for the current request type (A priori or A posteriori).
        _xpertRequestResult.addParameters(adjustmentResult->getAdjustments().front().getData().front().m_parameters);

        // Save the adjustment data into the XpertRequestResult
        _xpertRequestResult.setAdjustmentData(move(adjustmentResult));

        // Get the statistics at steady state and the parameters of previous type.
        gatherAdditionalData(_xpertRequestResult);

        return;
    }
}

void RequestExecutor::gatherAdditionalData(XpertRequestResult& _xpertRequestResult) const
{
    unique_ptr<Core::ComputingTraitAdjustment> baseAdjustmentTrait = make_unique<Core::ComputingTraitAdjustment>(*_xpertRequestResult.getAdjustmentTrait());

    // ------- statistics at steady state ---------

    // Update the end time to approximate a steady state to extract the statistics at steady state.
    // Ideally, this would have been done in a single query, but it is not possible to remove the extra cycle
    // data in the current core state. We should have included them with the xpert request result.
    // A steady state is approximated with adjustment time + multiplier * half life.
    const Core::HalfLife& halfLife = _xpertRequestResult.getDrugModel()->getTimeConsiderations().getHalfLife();
    double hoursToAdd = Common::UnitManager::convertToUnit(halfLife.getValue(), halfLife.getUnit(), Common::TucuUnit("h"));
    Common::DateTime staeadyEndTime = baseAdjustmentTrait->getAdjustmentTime() + Duration(chrono::hours(int(halfLife.getMultiplier() * hoursToAdd)));

    // Create the computing trait that goes to steady state.
    unique_ptr<Core::ComputingTraitAdjustment> steadyStateAdustmentTrait = nullptr;
    tweakComputingTraitAdjustment(baseAdjustmentTrait,
                                  staeadyEndTime,
                                  20,
                                  baseAdjustmentTrait->getComputingOption().getParametersType(),
                                  steadyStateAdustmentTrait);
    unique_ptr<Core::AdjustmentData> steadyAdjustmentResult = nullptr;

    // Execute the request.
    executeRequestAndGetResult<Core::ComputingTraitAdjustment, Core::AdjustmentData>(move(steadyStateAdustmentTrait), _xpertRequestResult, steadyAdjustmentResult);

    // If execution failed.
    if (steadyAdjustmentResult == nullptr){
        _xpertRequestResult.setErrorMessage("Failed to extract statistics at steady state.");
        return;
    } else {

        // Extracting the statistics at steady state for the best adjustment.
        _xpertRequestResult.setCycleStats(steadyAdjustmentResult->getAdjustments().front().getData().back().m_statistics);
    }

    // ------- Parameters type "A priori" ---------

    // We don't want to extract them again, so we check the base trait prediction parameter type.
    if (baseAdjustmentTrait->getComputingOption().getParametersType() == Core::PredictionParameterType::Aposteriori) {

        // Create a computing trait A priori.
        unique_ptr<Core::ComputingTraitAdjustment> aprioriAdustmentTrait = nullptr;
        tweakComputingTraitAdjustment(baseAdjustmentTrait,
                                      baseAdjustmentTrait->getEnd(),
                                      1,
                                      Core::PredictionParameterType::Apriori,
                                      aprioriAdustmentTrait);
        unique_ptr<Core::AdjustmentData> aprioriAdjustmentResult = nullptr;

        // Execute the request.
        executeRequestAndGetResult<Core::ComputingTraitAdjustment, Core::AdjustmentData>(move(aprioriAdustmentTrait), _xpertRequestResult, aprioriAdjustmentResult);

        // If execution failed.
        if (aprioriAdjustmentResult == nullptr){
            _xpertRequestResult.setErrorMessage("Failed to extract apriori parameters.");
            return;
        } else {

            // Saving the "A priori" parameters.
            _xpertRequestResult.addParameters(aprioriAdjustmentResult->getAdjustments().front().getData().front().m_parameters);
        }
    }

    // ------- Parameters "Typical patient" ---------

    // Create the computing trait for population.
    unique_ptr<Core::ComputingTraitAdjustment> populationAdustmentTrait = nullptr;
    tweakComputingTraitAdjustment(baseAdjustmentTrait,
                                  baseAdjustmentTrait->getEnd(),
                                  1,
                                  Core::PredictionParameterType::Population,
                                  populationAdustmentTrait);
    unique_ptr<Core::AdjustmentData> populationAdjustmentResult = nullptr;

    // Execute the request
    executeRequestAndGetResult<Core::ComputingTraitAdjustment, Core::AdjustmentData>(move(populationAdustmentTrait), _xpertRequestResult, populationAdjustmentResult);

    // If execution failed.
    if (populationAdjustmentResult == nullptr){
        _xpertRequestResult.setErrorMessage("Failed to extract population parameters.");
        return;
    } else {
        // Saving the "Typical patient" parameters.
        _xpertRequestResult.addParameters(populationAdjustmentResult->getAdjustments().front().getData().front().m_parameters);
    }
}

void RequestExecutor::tweakComputingTraitAdjustment(const unique_ptr<Core::ComputingTraitAdjustment>& _baseTrait,
                                                          const Common::DateTime& _newEnd,
                                                          double _newNbPointsPerHour,
                                                          Core::PredictionParameterType _newPredictionType,
                                                          std::unique_ptr<Core::ComputingTraitAdjustment>& _resultingAdjustment) const
{

    // Make new computing option with the new prediction parameter type.
    Core::ComputingOption newOptions{
        _newPredictionType,
        _baseTrait->getComputingOption().getCompartmentsOption(),
        _baseTrait->getComputingOption().retrieveStatistics(),
        _baseTrait->getComputingOption().retrieveParameters(),
        _baseTrait->getComputingOption().retrieveCovariates()
    };

    // Make a new adjustment trait with new number of points per hour, new end time and
    // for the best dosage.
    _resultingAdjustment = make_unique<Core::ComputingTraitAdjustment>(
                "",
                _baseTrait->getStart(),
                _newEnd,
                _newNbPointsPerHour,
                newOptions,
                _baseTrait->getAdjustmentTime(),
                Core::BestCandidatesOption::BestDosage,
                _baseTrait->getLoadingOption(),
                _baseTrait->getRestPeriodOption(),
                _baseTrait->getSteadyStateTargetOption(),
                _baseTrait->getTargetExtractionOption(),
                _baseTrait->getFormulationAndRouteSelectionOption()
                );
}

} // namespace Xpert
} // namespace Tucuxi
