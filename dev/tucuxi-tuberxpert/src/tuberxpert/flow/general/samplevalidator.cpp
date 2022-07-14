#include "samplevalidator.h"

#include <memory>
#include <vector>
#include <numeric>
#include <chrono>
#include <map>
#include <sstream>

#include <iostream>

#include "tucucore/computingcomponent.h"
#include "tucucore/computingservice/computingrequest.h"
#include "tucucore/computingservice/computingtrait.h"
#include "tucucore/definitions.h"

#include "tuberxpert/utils/xpertutils.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

void SampleValidator::perform(XpertRequestResult& _xpertRequestResult)
{
    // Check if there is a treatment.
    if (_xpertRequestResult.getTreatment() == nullptr) {
        _xpertRequestResult.setErrorMessage("No treatment set.");
        return;
    }

    // If there are samples, check if there is a dosage history.
    if (!_xpertRequestResult.getTreatment()->getSamples().empty() &&
         _xpertRequestResult.getTreatment()->getDosageHistory().isEmpty()) {
         _xpertRequestResult.setErrorMessage("Samples found but dosage history is empty.");
        return;
    }

    // Check if there is drug model
    if (_xpertRequestResult.getDrugModel() == nullptr) {
        _xpertRequestResult.setErrorMessage("No drug model set.");
        return;
    }

    vector<SampleValidationResult> results;

    // Getting percentiles for each sample.
    for(const unique_ptr<Core::Sample>& sample : _xpertRequestResult.getTreatment()->getSamples()) {

        // Prepare te computing request for percentiles computation.
        string responseId = "";
        Common::DateTime start = sample->getDate() - chrono::hours(1);  // Minus/plus 1 hour are just here "effectless", the core
        Common::DateTime end = sample->getDate() + chrono::hours(1);    // computes other start and end dates for a cycleData.
        Core::PercentileRanks ranks(99);
        iota(ranks.begin(), ranks.end(), 1);
        double nbPointsPerHour = 20;

        // In a future version where all analytes are in separate
        // cycleData, use AllAnalytes.
        Core::ComputingOption computingOption{Core::PredictionParameterType::Apriori, Core::CompartmentsOption::AllActiveMoieties};

        // Percentile trait and response.
        unique_ptr<Core::ComputingTraitPercentiles> percentileTrait =
                make_unique<Core::ComputingTraitPercentiles>(responseId, start, end, ranks, nbPointsPerHour, computingOption);
        unique_ptr<Core::PercentilesData> percentilesResult = nullptr;

        // Execute the request.
        executeRequestAndGetResult<Core::ComputingTraitPercentiles, Core::PercentilesData>(move(percentileTrait), _xpertRequestResult, percentilesResult);

        // If computation failed, abort xpertRequest processing.
        if (percentilesResult == nullptr) {
            _xpertRequestResult.setErrorMessage("Percentiles computation failed.");
            return;
        }

        // The sample position starts at 0.
        unsigned groupOver99Percentiles = 0;
        try {
            groupOver99Percentiles = findGroupPositionOver99Percentiles(percentilesResult.get(), sample);
        } catch (const invalid_argument& e) {

            // We catch unit error or if the sample is not bound by any cycleData.
            _xpertRequestResult.setErrorMessage("Error handling sample of " + dateTimeToString(sample->getDate()) + ", details: " + e.what());
            return;
        }

        results.emplace_back(SampleValidationResult(sample.get(), groupOver99Percentiles));
    }

    // Save the validation results.
    _xpertRequestResult.setSampleResults(move(results));
}

unsigned SampleValidator::findGroupPositionOver99Percentiles(const Core::PercentilesData* _percentilesData,
                                                             const unique_ptr<Core::Sample>& _sample) const
{
    // First, find the cycleData that holds the values that surround the sample date
    // We simply use the first percentile to do that.

    int cycleDataIndex = -1;

    const vector<Core::CycleData> firstPercentileData = _percentilesData->getPercentileData(0);
    for (size_t cycleIndex = 0; cycleIndex < firstPercentileData.size(); ++cycleIndex){

        // If the sample date is between the cycleData start and end date.
        if (_sample->getDate() >= firstPercentileData[cycleIndex].m_start && _sample->getDate() <= firstPercentileData[cycleIndex].m_end) {
            cycleDataIndex = cycleIndex;
            break;
        }
    }

    // If we didn't find a cycleData.
    if (cycleDataIndex == -1) {
        throw invalid_argument("No cycle data contains the sample date.");
    }


    // Iterate over the percentiles (1-99) to find the first percentile that contains
    // a bigger value than the corresponding sample.

    // This variable saves the position of the time offsets to not being forced
    // to parse everything again when switching percentile.
    size_t savedIndexOfPair = 0;

    for (size_t percentileIndex = 0; percentileIndex < 99; ++percentileIndex) {

        // Get the percentile.
        const vector<Core::CycleData> percentileData = _percentilesData->getPercentileData(percentileIndex);

        // Get its times and concentrations.
        const vector<double>& times = percentileData[cycleDataIndex].m_times[0];
        const vector<double>& concentrations = percentileData[cycleDataIndex].m_concentrations[0];

        // Iterates through pairs of time offsets and look for the pair that bounds the sample date.
        for (size_t i = savedIndexOfPair; i < times.size() - 1; ++i) {

            // Get the bounded dates and times.
            Common::DateTime t0 = percentileData[cycleDataIndex].m_start + chrono::minutes(int(times[i] * 60));
            Common::DateTime t1 = percentileData[cycleDataIndex].m_start + chrono::minutes(int(times[i + 1] * 60));

            // If the sample date time is not bounded, jump to the next pair.
            if (not(t0 <= _sample->getDate() && _sample->getDate() <= t1)) {
                continue;
            }

            // Convert the sample value to the cycleData unit.
            double convertedSampleConcentration = Common::UnitManager::convertToUnit(
                        _sample->getValue(),
                        _sample->getUnit(),
                        percentileData[cycleDataIndex].m_unit);

            // If the mesure is lower than the interpolated mesure, it belongs to the current percentile
            // (+1 because we iterate from 0 to 98)
            // The interpolation proceeds  as follows (assuming the concentration interpolation is linear):
            // 1) Compute rp, the "relative" position of the sample date between the pair. 0 being above t0 and 1 being above t1.
            // 2) Compute the interpolated concentration with respect to rp. concentration(t0) + rp * ( concentration(t1) - concentration(t0) )

            // 1) Relative position rp using date as seconds. Assuming t1 <= sample date <= t2,
            //    the interpolation rate is (sample date - t1) / (t2 - t1)
            double rp = (_sample->getDate().toSeconds() - t0.toSeconds()) / (t1.toSeconds() - t0.toSeconds());

            // 2) Retrieving the interpolated concentration
            double interpolatedConcentration = concentrations[i] + rp * (concentrations[i + 1] - concentrations[i]);

            // If the sample concentration is below the percentile concentration, it belongs to its group.
            if (convertedSampleConcentration <= interpolatedConcentration) {

                return percentileIndex + 1;

            // Jump to the next percentile.
            } else {
                savedIndexOfPair = i; // Next percentile goes directly to the the good times pair.
                break;                // Try the next percentile.
            }
        }
    }

    // If the mesure is not in the groups before the 99th percentile, it belongs
    // to the last group, the 100th.
    return 100;
}

} // namespace Xpert
} // namespace Tucuxi
