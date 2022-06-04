#include "samplevalidator.h"

#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>

#include "tucucore/computingcomponent.h"
#include "tucucore/computingservice/computingrequest.h"
#include "tucucore/computingservice/computingresponse.h"
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

    // Preparing comuting request for percentile computation.
    Core::RequestResponseId  responseId = "";
    Common::DateTime start = getOldestSampleDateTime(_xpertRequestResult.getTreatment()->getSamples());
    Common::DateTime end = start;
    end.addDays(1);
    Core::PercentileRanks ranks(10);
    iota(ranks.begin(), ranks.end(), 10);
    double nbPointsPerHour = 1;
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

    // If computation failed, abort xpert request handling.
    if (result != Core::ComputingStatus::Ok) {
        _xpertRequestResult.setErrorMessage("Percentile computation failed.");
    }

}

Common::DateTime SampleValidator::getOldestSampleDateTime(const Core::Samples& _samples) const
{
    Common::DateTime oldestDateTime = m_computationDate;
    for (const unique_ptr<Core::Sample>& sample: _samples) {
        if (sample->getDate() < oldestDateTime) {
            oldestDateTime = sample->getDate();
        }
    }

    return oldestDateTime;
}

} // namespace XpertResult
} // namespace Tucuxi
