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

using namespace std;

namespace Tucuxi {
namespace Xpert {

SampleValidator::SampleValidator()
{}

void SampleValidator::perform(XpertRequestResult& _xpertRequestResult)
{
    // Checks treatment
    if (_xpertRequestResult.getTreatment() == nullptr) {
        _xpertRequestResult.setErrorMessage("No treatment set.");
        return;
    }

    // Checks that there is a dosage history
    if (!_xpertRequestResult.getTreatment()->getSamples().empty() &&
         _xpertRequestResult.getTreatment()->getDosageHistory().isEmpty()) {
         _xpertRequestResult.setErrorMessage("Samples found but dosage history is empty.");
        return;
    }

    // Checks drug model
    if (_xpertRequestResult.getDrugModel() == nullptr) {
        _xpertRequestResult.setErrorMessage("No drug model set.");
        return;
    }

    map<const Core::Sample*, SampleValidationResult> sampleResults;

    // Getting percentiles for each sample
    for(const unique_ptr<Core::Sample>& sample : _xpertRequestResult.getTreatment()->getSamples()) {

        // Preparing comuting request for percentile computation.
        string responseId = "";
        Common::DateTime start = sample->getDate() - chrono::hours(1);  // Minus/plus 1 hour are just here
        Common::DateTime end = sample->getDate() + chrono::hours(1);    // "effectless", the core computes other start and end dates.
        Core::PercentileRanks ranks(99);
        iota(ranks.begin(), ranks.end(),1);
        double nbPointsPerHour = 20;

        // In a future version where all analytes are in separated
        // cycleData, use AllAnalytes.
        Core::ComputingOption computingOption{Core::PredictionParameterType::Apriori, Core::CompartmentsOption::AllActiveMoieties};

        // Percentile trait.
        unique_ptr<Core::ComputingTraitPercentiles> ctp = make_unique<Core::ComputingTraitPercentiles>(responseId, start, end, ranks, nbPointsPerHour, computingOption);

        // Request and response.
        Core::ComputingRequest cReq { "", *_xpertRequestResult.getDrugModel(), *_xpertRequestResult.getTreatment(), move(ctp)};
        std::unique_ptr<Core::ComputingResponse> cRes = std::make_unique<Core::ComputingResponse>("");

        // Starting percentiles computation.
        Core::IComputingService* computingComponent = dynamic_cast<Core::IComputingService*>(Core::ComputingComponent::createComponent());
        Core::ComputingStatus result = computingComponent->compute(cReq, cRes);


        // Extracting sample position.
        const Core::PercentilesData* pData = dynamic_cast<const Core::PercentilesData*>(cRes->getData());

        unsigned groupOver99Percentiles = 0;
        try {
            groupOver99Percentiles = findGroupPositionOver99Percentiles(pData, sample);
        } catch (const invalid_argument& e) {
            // We catch unit error or if the sample is not bound by cycledata
            stringstream ss;
            ss << sample->getDate();
            _xpertRequestResult.setErrorMessage("Error handling sample of " + ss.str() + ", details: " + e.what());
            return;
        }

        // If computation failed, abort xpert request handling.
        if (result != Core::ComputingStatus::Ok) {
            _xpertRequestResult.setErrorMessage("Percentile computation failed.");
        }

        sampleResults.emplace(make_pair(sample.get(), SampleValidationResult(sample.get(), groupOver99Percentiles)));
    }

    _xpertRequestResult.setSampleResults(move(sampleResults));
}

unsigned SampleValidator::findGroupPositionOver99Percentiles(const Core::PercentilesData* _percentilesData, const unique_ptr<Core::Sample>& _sample) const
{
    // First find the cycleData that holds the values that surround the sample date
    // We simple use the first percentile to do that.

    int cycleDataIndex = -1;

    const vector<Core::CycleData> percentileData = _percentilesData->getPercentileData(0);
    for (size_t cdi = 0; cdi < percentileData.size(); ++cdi){

        // If the sample date is between the cycledata start and end.
        if (_sample->getDate() >= percentileData[cdi].m_start && _sample->getDate() <= percentileData[cdi].m_end) {
            cycleDataIndex = cdi;
            break;
        }
    }

    if (cycleDataIndex == -1) {
        throw invalid_argument("No cycle data contains the sample date.");
    }


    // Iterate over percentiles (1-99) to find the first percentile that contains
    // a bigger value than the corresponding sample.
    size_t saveIndexOfPair = 0;
    for (size_t p = 0; p < 99; ++p) {

        const vector<Core::CycleData> percentileData = _percentilesData->getPercentileData(p);

        const vector<double>& times = percentileData[cycleDataIndex].m_times[0];
        const vector<double>& concentrations = percentileData[cycleDataIndex].m_concentrations[0];

        // Iterates through pairs of time offsets and get the closest to the sample date and time.
        for (size_t i = saveIndexOfPair; i < times.size() - 1; ++i) {

            // Get bounded date and times
            Common::DateTime t1 = percentileData[cycleDataIndex].m_start + chrono::minutes(int(times[i] * 60));
            Common::DateTime t2 = percentileData[cycleDataIndex].m_start + chrono::minutes(int(times[i + 1] * 60));

            // If the sample time is not bounded, jump to the next pair.
            if (not(t1 <= _sample->getDate() && _sample->getDate() <= t2)) {
                continue;
            }

            // Convert sample value into cycledata unit
            double convertedMesure = Common::UnitManager::convertToUnit(
                        _sample->getValue(),
                        _sample->getUnit(),
                        percentileData[cycleDataIndex].m_unit);

            // If the mesure is below the interpolated mesure, it belongs to the current percentile
            // (+1 because we iterate from 0 to 98)
            // The interpolation goes as follows (assuming that the concentration interpolation is linear):
            // 1) Compute rp, the "relative" position of the sample date between the pair. 0 being t1 and 1 begin t1.
            // 2) Compute the the interpolated concentration relatively to p

            // 1) Relative position rp using date as seconds. Admitting t1 <= sample date <= t2
            //    the interpolation rate is (sample date - t1) / (t2 - t1)
            double rp = (_sample->getDate().toSeconds() - t1.toSeconds()) / (t2.toSeconds() - t1.toSeconds());

            // 2) Retrieving the interpolated concentration
            double interpolatedConcentration = concentrations[i] + rp * (concentrations[i + 1] - concentrations[i]);

            if (convertedMesure <= interpolatedConcentration) {
                return p + 1;
            } else {
                saveIndexOfPair = i; // Next percentile goes directly to the the good pair.
                break; // Try the next percentile.
            }
        }
    }

    // If the mesure is not in the groups before the 99th percentile, it belongs
    // to the last group, the 100th.
    return 100;
}

} // namespace Xpert
} // namespace Tucuxi
