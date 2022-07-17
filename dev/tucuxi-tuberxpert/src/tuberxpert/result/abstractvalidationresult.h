#ifndef ABSTRACTVALIDATIONRESULT_H
#define ABSTRACTVALIDATIONRESULT_H

#include <string>

namespace Tucuxi {
namespace Xpert {

/// \brief Enum which defines the severity of a validation warning
///        returned by an AbstractResult class.
/// \date 02/06/2022
/// \author Herzig Melvyn
enum class WarningLevel
{
    NORMAL = 0, /**< It seems strange. May be a double check is needed */
    CRITICAL    /**< It is unusual. A serious control is necessary. */
};

/// \brief This class is the base class for all validation result classes (except Common and Request).
///        A validation result class is always linked to a "source" pointer and
///        associates a warning message with a WarningLevel.
/// \date 02/06/2022
/// \author Herzig Melvyn
template <typename T>
class AbstractValidationResult
{
public:

    /// \brief Constructor.
    /// \param _source Pointer to the source object concerned by the validation result.
    ///                The source pointer must have at least the same lifetime as this object.
    /// \param _warning Warning message associated with the validation result
    ///                 if something is suspicious.
    AbstractValidationResult(const T* _source, const std::string& _warning) :
        m_source(_source), m_warning(_warning){}

    /// \brief Destructor.
    virtual ~AbstractValidationResult(){};

    // Getters

    /// \brief Get the source object concerned by the validation result.
    /// \return A constant pointer to the source object. This pointer
    ///         must not be deleted.
    const T* getSource() const {
        return m_source;
    }

    /// \brief Get the warning message associated with the validation result.
    /// \return The warning message. May be an empty string if nothing is suspicious.
    std::string getWarning() const {
        return m_warning;
    }

    /// \brief Get the warning level associated to a warning. It
    ///        doesn't take into account whether the warning is set or not.
    /// \return WarningLevel::NORMAL. May be override.
    virtual WarningLevel getWarningLevel() const{
        return WarningLevel::NORMAL;
    }

protected:

    /// \brief Source object concerned by the validation result.
    const T* m_source;

    /// \brief The associated warning message if necessary.
    std::string m_warning;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // ABSTRACTVALIDATIONRESULT_H
