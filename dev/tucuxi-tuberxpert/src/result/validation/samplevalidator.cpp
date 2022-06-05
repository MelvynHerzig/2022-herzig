#include "samplevalidator.h"

#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>
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
namespace XpertResult {

SampleValidator::SampleValidator(Common::DateTime _computationDate) : m_computationDate(_computationDate)
{}

void SampleValidator::getSampleValidations(XpertRequestResult& _xpertRequestResult) const
{
    // if no sample skip this validation
    if (_xpertRequestResult.getTreatment()->getSamples().size() == 0) {
        return;
    }

    map<const Core::Sample*, SampleResult> sampleResults;

    // Getting percentile for each sample
    for(const unique_ptr<Core::Sample>& sample : _xpertRequestResult.getTreatment()->getSamples()) {

        // Preparing comuting request for percentile computation.
        string responseId = "";
        Common::DateTime start = sample->getDate() - chrono::hours(1);
        Common::DateTime end = sample->getDate() + chrono::hours(1);
        Core::PercentileRanks ranks(99);
        iota(ranks.begin(), ranks.end(),1);
        double nbPointsPerHour = 20;
        Core::ComputingOption co{
            Core::PredictionParameterType::Apriori,
                    Core::CompartmentsOption::AllActiveMoieties}; // In a future version where all analytes are in separated
                                                                  // cycleData, use AllAnalytes.

        unique_ptr<Core::ComputingTraitPercentiles> ctp = make_unique<Core::ComputingTraitPercentiles>(responseId, start, end, ranks, nbPointsPerHour, co);
        Core::ComputingRequest cReq { "", *_xpertRequestResult.getDrugModel(), *_xpertRequestResult.getTreatment(), move(ctp)};
        std::unique_ptr<Core::ComputingResponse> cRes = std::make_unique<Core::ComputingResponse>("");

        // Starting percentiles computation
        Core::IComputingService* computingComponent =
                dynamic_cast<Core::IComputingService*>(Core::ComputingComponent::createComponent());

        Core::ComputingStatus result = computingComponent->compute(cReq, cRes);


        const Core::PercentilesData* pData = dynamic_cast<const Core::PercentilesData*>(cRes->getData());

        unsigned posOver100Percentile = 0;
        try {
            posOver100Percentile = findPosOver100Percentile(pData, sample);
        } catch (const invalid_argument& e) {
            // We catch unit conversion error
            stringstream ss;
            ss << sample->getDate();
            _xpertRequestResult.setErrorMessage("Error handling samples, sample mesure date: " + ss.str());
            return;
        }

        // If computation failed, abort xpert request handling.
        if (result != Core::ComputingStatus::Ok) {
            _xpertRequestResult.setErrorMessage("Percentile computation failed.");
        }

        sampleResults.emplace(make_pair(sample.get(), SampleResult(sample.get(), posOver100Percentile)));
    }

     _xpertRequestResult.setSampleResults(move(sampleResults));
}

unsigned SampleValidator::findPosOver100Percentile(const Core::PercentilesData* _percentilesData, const std::unique_ptr<Core::Sample>& _sample) const
{
    // Iterate over percentiles (1-99) to find the first percentile that contains
    // a bigger value than the corresponding sample.
    for (size_t p = 0; p < 99; ++p) {

        const vector<Core::CycleData> percentileData = _percentilesData->getPercentileData(p);

        // Search the cycledata that contains the sample date.
        for (size_t cd = 0; cd < percentileData.size(); ++cd){

            // If the sample date is between the cycledata start and end.
            if (_sample->getDate() >= percentileData[cd].m_start && _sample->getDate() <= percentileData[cd].m_end) {

                const vector<double>& times = percentileData[cd].m_times[0];
                const vector<double>& concentrations = percentileData[cd].m_concentrations[0];

                // Iterates through pairs of time offsets and get the closest to the sample date and time.
                for (size_t i = 0; i < times.size() - 1; ++i) {

                    // Get pair of date and times
                    Common::DateTime t1 = percentileData[cd].m_start + chrono::minutes(int(times[i] * 60));
                    Common::DateTime t2 = percentileData[cd].m_start + chrono::minutes(int(times[i + 1] * 60));

                    // If the sample time is not bounded, jump to next pair
                    if (not(t1 <= _sample->getDate() && _sample->getDate() <= t2)) {
                        continue;
                    }

                    // Convert sample value into cycledata unit
                    double convertedMesure = Common::UnitManager::convertToUnit(
                                _sample->getValue(),
                                _sample->getUnit(),
                                percentileData[cd].m_unit);

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
                    }
                }
            }
        }
    }

    // If the mesure is not in the groups before the 99th percentil, it belongs
    // to the last group.
    return 100;
}

} // namespace XpertResult
} // namespace Tucuxi
