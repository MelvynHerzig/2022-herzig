#include "xpertrequestdata.h"

namespace Tucuxi {
namespace XpertQuery {

XpertRequestData::XpertRequestData(
        std::string& _drugID,
        bool _localComputation,
        OutputFormat _outputFormat,
        OutputLang _outputLang,
        Common::DateTime _adjustmentTime,
        Core::BestCandidatesOption _candidatesOption,
        Core::LoadingOption _loadingOption,
        Core::RestPeriodOption _restPeriodOption,
        Core::SteadyStateTargetOption _steadyStateTargetOption,
        Core::TargetExtractionOption _targetExtractionOption,
        Core::FormulationAndRouteSelectionOption _formulationAndRouteSelectionOption) :
        m_drugID(_drugID), m_localComputation(_localComputation), m_outputFormat(_outputFormat), m_outputLang(_outputLang),
        m_adjustmentTime(_adjustmentTime), m_bestCandidatesOption(_candidatesOption), m_loadingOption(_loadingOption),
        m_restPeriodOption(_restPeriodOption), m_steadyStateTargetOption(_steadyStateTargetOption), m_targetExtractionOption(_targetExtractionOption),
        m_formulationAndRouteSelectionOption(_formulationAndRouteSelectionOption)
{
}

std::string Tucuxi::XpertQuery::XpertRequestData::getDrugID() const
{
    return m_drugID;
}

bool Tucuxi::XpertQuery::XpertRequestData::getLocalComputation() const
{
    return m_localComputation;
}

Tucuxi::XpertQuery::OutputFormat Tucuxi::XpertQuery::XpertRequestData::getOutputFormat() const
{
    return m_outputFormat;
}

Tucuxi::XpertQuery::OutputLang Tucuxi::XpertQuery::XpertRequestData::getOutputLang() const
{
    return m_outputLang;
}

DateTime Tucuxi::XpertQuery::XpertRequestData::getAdjustmentTime() const
{
    return m_adjustmentTime;
}

Tucuxi::Core::BestCandidatesOption Tucuxi::XpertQuery::XpertRequestData::getBestCandidatesOption() const
{
    return m_bestCandidatesOption;
}

Tucuxi::Core::LoadingOption Tucuxi::XpertQuery::XpertRequestData::getLoadingOption() const
{
    return m_loadingOption;
}

Tucuxi::Core::RestPeriodOption Tucuxi::XpertQuery::XpertRequestData::getRestPeriodOption() const
{
    return m_restPeriodOption;
}

Tucuxi::Core::SteadyStateTargetOption Tucuxi::XpertQuery::XpertRequestData::getSteadyStateTargetOption() const
{
    return m_steadyStateTargetOption;
}

Tucuxi::Core::TargetExtractionOption Tucuxi::XpertQuery::XpertRequestData::getTargetExtractionOption() const
{
    return m_targetExtractionOption;
}

Tucuxi::Core::FormulationAndRouteSelectionOption Tucuxi::XpertQuery::XpertRequestData::getFormulationAndRouteSelectionOption() const
{
    return m_formulationAndRouteSelectionOption;
}

} // namespace XpertQuery
} // namespace Tucuxi
