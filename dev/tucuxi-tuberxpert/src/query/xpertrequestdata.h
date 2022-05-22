#ifndef XPERTREQUESTDATA_H
#define XPERTREQUESTDATA_H

#include "tucucommon/datetime.h"
#include "tucucore/computingservice/computingtrait.h"

#include "tucucore/definitions.h"

#include <string>

namespace Tucuxi {
namespace XpertQuery {

/// \brief List of the supported languages
/// It can be:
/// 0: English
/// \date 24/04/2022
/// \author Herzig Melvyn
enum class OutputLang
{
    ENGLISH = 0,
    FRENCH
};

/// \brief List of the supported languages
/// It can be:
/// 0: xml
/// 1: html
/// 2: pdf
/// \date 24/04/2022
/// \author Herzig Melvyn
enum class OutputFormat
{
    XML = 0,
    HTML,
    PDF
};

/// \brief The LoadingOption enum.
/// This option allows to authorize or not a loading dose to rapidly attain steady state.
/// This is the same we can find computingTrait.h with an added value that means "unspecified".
enum class LoadingOption
{
    /// No loading dose is allowed
    NoLoadingDose = 0,

    /// A loading dose shall be proposed if relevant
    LoadingDoseAllowed,

    /// The user will use the drug model recommendation.
    Unspecified
};

///
/// \brief The RestPeriodOption enum.
/// This option allows to authorize or not a resting period to rapidly attain steady state.
/// This is the same we can find computingTrait.h with an added value that means "unspecified".
enum class RestPeriodOption
{
    /// No resting period is allowed
    NoRestPeriod = 0,

    /// A resting period shall be proposed if relevant
    RestPeriodAllowed,

    /// The user will use the drug model recommendation.
    Unspecified
};


/// \brief Class of custom request for tuberXpert
/// \date 24/04/2022
/// \author Herzig Melvyn
class XpertRequestData
{
public:
    // Constructors
    /// \brief Default constructor is not supported.
    XpertRequestData() = delete;


    /// \brief XpertRequestData constructor.
    /// \param _drugID Drug targeted.
    /// \param _localComputation Should the computation be executed locally ?
    /// \param _outputFormat Output format.
    /// \param _outputLang Output language.
    /// \param _adjustmentTime Time at which the adjustment has to be calculated
    /// \param _candidatesOption Selection of best candidates options
    /// \param _loadingOption Selects if a loading dose can be proposed or not
    /// \param _restPeriodOption Selects if a rest period can be proposed or not
    /// \param _targetExtractionOption Target extraction options
    /// \param _formulationAndRouteSelectionOption Selection of the formulation and route options
    XpertRequestData(
            const std::string& _drugID,
            bool _localComputation,
            OutputFormat _outputFormat,
            OutputLang _outputLang,
            Common::DateTime _adjustmentTime,
            Core::BestCandidatesOption _candidatesOption,
            LoadingOption _loadingOption,
            RestPeriodOption _restPeriodOption,
            Core::TargetExtractionOption _targetExtractionOption,
            Core::FormulationAndRouteSelectionOption _formulationAndRouteSelectionOption);

    /// \brief Copy constructor is not supported.
    XpertRequestData(const XpertRequestData& _other) = delete;

    // Getters

    /// \brief Gets the drug id to adjust.
    /// \return Drug identifier.
    std::string getDrugID() const;

    /// \brief Gets if computation should be launch locally.
    /// \return True if computation is local otherwise false.
    bool getLocalComputation() const;

    /// \brief Gets the output format.
    /// \return Output format.
    OutputFormat getOutputFormat() const;

    /// \brief Gets the output language.
    /// \return Output language.
    OutputLang getOutputLang() const;

    /// \brief Gets the time of adjustment
    /// \return Time of the adjustment
    Common::DateTime getAdjustmentTime() const;

    /// \brief Gets the best candidate option
    /// \return The best candidate option
    Core::BestCandidatesOption getBestCandidatesOption() const;

    /// \brief Gets the option about a potential loading dose
    /// \return The option about a potential loading dose
    LoadingOption getLoadingOption() const;

    /// \brief Gets the option about a potential rest period
    /// \return The option about a potential rest period
    RestPeriodOption getRestPeriodOption() const;

    /// \brief Gets the target extraction option
    /// \return The target extraction option
    Core::TargetExtractionOption getTargetExtractionOption() const;

    /// \brief Gets the formulation and route selection option
    /// \return The formulation and route selection option
    Core::FormulationAndRouteSelectionOption getFormulationAndRouteSelectionOption() const;

protected:
    /// Targeted drug to use tuberXpert.
    std::string m_drugID;

    /// Should we execute the computation locally?
    bool m_localComputation;

    /// Output format.
    OutputFormat m_outputFormat;

    /// Output language.
    OutputLang m_outputLang;

    /// Date of the adjustment
    Common::DateTime m_adjustmentTime;

    /// Adjustment options : only the best, or all possible ones
    Core::BestCandidatesOption m_bestCandidatesOption;

    /// Shall we propose a loading dose if applicable?
    LoadingOption m_loadingOption;

    /// Shall we propose a rest period if applicable?
    RestPeriodOption m_restPeriodOption;

    /// Target extraction options
    Core::TargetExtractionOption m_targetExtractionOption;

    /// What formulation and route have to be used for generating candidates
    Core::FormulationAndRouteSelectionOption m_formulationAndRouteSelectionOption;
};

} // namespace XpertQuery
} // namespace Tucuxi

#endif // XPERTREQUESTDATA_H
