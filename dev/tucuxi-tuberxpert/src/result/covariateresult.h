#ifndef COVARIATERESULT_H
#define COVARIATERESULT_H

#include <string>
#include <optional>

#include "tucucommon/unit.h"

namespace Tucuxi {
namespace XpertResult {


/// \brief Enum used to specify the source of a covariate
enum class CovariateSource {
    Patient,
    Model
};

class AbsractCovariateResult {
public:
    virtual ~AbsractCovariateResult(){};

    virtual std::string getValue() const = 0;

    //virtual Common::TucuUnit getUnit() const = 0;

    virtual std::string getSource() const = 0;

    virtual std::optional<std::string> getWarning() const = 0;

};

/// \brief This is a wrapper class that stores the information about the
///        covariates of drug considering a drug model.
///
///        Basically this class is used in a DrugResult which is associated to a drug model.
///        A DrugResult contains a list of covariate that corresponds to the definitions located
///        in the associated drug model. Consequently, it contains a map [CovariateDefinition, CovariateResult]
///        that tells for each definitions what is the value used, the unit and the source that are stored in
///        this class.
///
///        A patient coavriate might be invalid (not respecting the domain of the definition), so it could
///        contain an optional error message.
///
///        The generic value is expected to be a PatientCovariate or a CovariateDefinition.
///        They must implement the methods getValue() and getUnit()
///
/// \date 20/05/2022
/// \author Herzig Melvyn
template<typename T>
class CovariateResult : public AbsractCovariateResult
{
public:

    /// \brief Create a covariate result for a given covariate definition.
    /// \param _source Define from where comes the _covariate element.
    /// \param _covariate Reference to a PatientCovariate or a CovariateDefinition with getValue() and getUnit()
    /// \param _warning Optionnal warning message, if PatientCovariate does not match DefinitionCovariate domain.
    CovariateResult(CovariateSource _source, T _covariate, const std::optional<std::string>& _warning):
        m_source(_source), m_covariate(_covariate), m_warning(_warning)
    {};

    /// \brief Get the value (as string) of the covariate.
    /// \return Returns the value.
    std::string getValue() const {
        return "m_covariate.getValue();";
    };

    /// \brief Get the data's unit of measure.
    /// \return Data's unit of measure.
//    Common::TucuUnit getUnit() const{
//        return m_covariate.getUnit();
//    }

    /// \brief Get the source of the covariate as a string.
    /// \return Source of the covariate
    std::string getSource() const{
        switch(m_source) {
            case CovariateSource::Patient : return "patient";
            case CovariateSource::Model   : return "model";
            default : return "unknown";
        }
    }

    /// \brief Get the optional warning message.
    /// \return Return a copy of the optional warning.
    std::optional<std::string> getWarning() const{
        return m_warning;
    }

protected:

    /// Covariate source indication.
    CovariateSource m_source;

    /// Covariates reference to use getValue and getUnit.
    T m_covariate;

    /// Error to print when generating the covariate in report.
    std::optional<std::string> m_warning;
};

} // namespace XpertResult
} // namespace Tucuxi

#endif // COVARIATERESULT_H
