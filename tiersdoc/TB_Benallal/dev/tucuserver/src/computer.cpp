#include "computer.h"

using namespace std;
using namespace Tucuxi::Core;

namespace Tucuxi {
namespace Server {

const Core::CycleSize Computer::m_sCycleSize = Configuration::getInstance()->getCycleSize();

Computer::Computer(const Query& _query)
    : m_query(_query)
{}

ApiResponse& Computer::getResult()
{
    if (m_result.get() == nullptr) {
        m_result = make_unique<ApiResponseAdjustment>(m_query.getQueryID(), "123");
        m_result->addError("ComputationError", m_query.getLanguage(), "Getting result when computing is not complete!");
    }

    return *m_result;
}

PatientVariates Computer::extractPatientVariates() const
{
    const vector< unique_ptr<CovariateData> >& covariateData = m_query.getpParameters()
                                                                            .getpPatient()
                                                                            .getCovariates();

    PatientVariates patientVariates;
    for (const unique_ptr<CovariateData>& covariate : covariateData) {
        patientVariates.push_back(
                        make_unique<PatientCovariate>(
                                          covariate->getName(),
                                          covariate->getValue(),
                                          covariate->getDatatype(),
                                          Core::Unit(covariate->getUnit()),
                                          covariate->getpDate()
                                      )
                    );
    }

    return patientVariates;
}

Targets Computer::extractTargets(size_t _drugPosition) const
{
    Targets targets;

    const vector< unique_ptr<TargetData> >& targetsData = m_query.getpParameters()
                                                                    .getDrugs().at(_drugPosition)
                                                                    ->getTargets();

    for (const unique_ptr<TargetData>& td : targetsData) {
        targets.push_back(
                    make_unique<Target>(
                        td->getActiveMoietyID(),
                        td->getTargetType(),
                        td->getMin(),
                        td->getBest(),
                        td->getMax()
                        )
                    );
    }

    return targets;
}

Samples Computer::extractSamples(size_t _drugPosition) const
{
    Samples samples;

    const vector< unique_ptr<SampleData> >& samplesData = m_query.getpParameters()
                                                                    .getDrugs().at(_drugPosition)
                                                                    ->getSamples();

    for (const unique_ptr<SampleData>& sd : samplesData) {
        for (const unique_ptr<ConcentrationData>& cd : sd->getConcentrations()) {
            samples.push_back(
                        make_unique<Sample>(
                            sd->getpSampleDate(),
                            cd->getAnalyteID(),
                            cd->getValue(),
                            Core::Unit(cd->getUnit())
                            )
                        );
        }
    }

    return samples;
}

} // namespace Server
} // namespace Tucuxi
