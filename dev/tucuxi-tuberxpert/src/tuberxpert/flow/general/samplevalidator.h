#ifndef SAMPLEVALIDATOR_H
#define SAMPLEVALIDATOR_H

#include "tucucommon/datetime.h"
#include "tucucore/drugtreatment/sample.h"
#include "tucucore/computingservice/computingresponse.h"

#include "tuberxpert/flow/abstract/abstractxpertflowstep.h"
#include "tuberxpert/result/xpertrequestresult.h"

struct TestSampleValidator;

namespace Tucuxi {
namespace XpertFlow {


/// \brief This class evaluates patient's samples.
///
///        Per sample, it makes an "a priori" percentile request and submits it to Tucuxi computing core.
///        The request asks for the percentiles 1 - 99. Then, it locates the position of the sample in the result.
///
///        The group is a number from 1 to 100 that corresponds to the 100
///        separations produced by de 99 percentiles.
///
///        If the group number is 1, the sample is located before the first percentile,
///        if the group number is 2, the sample is located before the second percentile but after the first
///        and so on...
///
///        The 100th group is the one that is after the 99th percentile.
///
///        Each sample is evaluated. In futur version, maybe consider forgetting too old samples.
/// \date 08/06/2022
/// \author Herzig Melvyn
class SampleValidator : public XpertFlow::AbstractXpertFlowStep
{
public:

    /// \brief Constructor.
    SampleValidator();

    /// \brief Evaluates each sample in the treatment from the XpertRequestResult.
    /// \param _xpertRequestResult XpertRequestResult containing samples to evaluate.
    void perform(XpertResult::XpertRequestResult& _xpertRequestResult) const;

protected:

    /// \brief Given a percentiles data (response from Tucuxi core) find where is located the sample.
    ///        This methods only consider times[0] and concentrations[0] of the cycleData. Consider changing it
    ///        when the cycleData implements the analytes featue.
    /// \param _percentilesData Response from the core with 99 percentiles.
    /// \param _sample Patient's sample to position.
    /// \return Returns the position of sample relatively to the percentiles from 1 to 100.
    /// \throw invalida_argument exception of the consersion of the sample to match the percentiles unit fails or if the
    ///        sample date is not bound by any cycleData.
    unsigned findGroupPositionOver99Percentiles(const Core::PercentilesData* _percentilesData, const std::unique_ptr<Core::Sample>& _sample) const;

    // For testing purpose. the tests works with findGroupPositionOver99Percentiles and not with getSampleValidations. It is simpler
    // because it allows us to forge our own percentiles data and to be able to predict the location of some predetermined samples.
    friend TestSampleValidator;
};

} // namespace XpertFlow
} // namespace Tucuxi

#endif // SAMPLEVALIDATOR_H
