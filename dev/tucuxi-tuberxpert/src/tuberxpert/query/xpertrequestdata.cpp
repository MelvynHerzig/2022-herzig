#include "xpertrequestdata.h"

namespace Tucuxi {
namespace Xpert {

XpertRequestData::XpertRequestData(
        const std::string& _drugID,
        OutputFormat _outputFormat,
        OutputLang _outputLang,
        Common::DateTime _adjustmentTime,
        LoadingOption _loadingOption,
        RestPeriodOption _restPeriodOption,
        Core::TargetExtractionOption _targetExtractionOption,
        Core::FormulationAndRouteSelectionOption _formulationAndRouteSelectionOption) :
        m_drugID(_drugID),
        m_outputFormat(_outputFormat),
        m_outputLang(_outputLang),
        m_adjustmentTime(_adjustmentTime),
        m_loadingOption(_loadingOption),
        m_restPeriodOption(_restPeriodOption),
        m_targetExtractionOption(_targetExtractionOption),
        m_formulationAndRouteSelectionOption(_formulationAndRouteSelectionOption)
{}

std::string XpertRequestData::getDrugID() const
{
    return m_drugID;
}

OutputFormat XpertRequestData::getOutputFormat() const
{
    return m_outputFormat;
}

OutputLang XpertRequestData::getOutputLang() const
{
    return m_outputLang;
}

DateTime XpertRequestData::getAdjustmentTime() const
{
    return m_adjustmentTime;
}

LoadingOption XpertRequestData::getLoadingOption() const
{
    return m_loadingOption;
}

RestPeriodOption XpertRequestData::getRestPeriodOption() const
{
    return m_restPeriodOption;
}

Core::TargetExtractionOption XpertRequestData::getTargetExtractionOption() const
{
    return m_targetExtractionOption;
}

Core::FormulationAndRouteSelectionOption XpertRequestData::getFormulationAndRouteSelectionOption() const
{
    return m_formulationAndRouteSelectionOption;
}

} // namespace Xpert
} // namespace Tucuxi
