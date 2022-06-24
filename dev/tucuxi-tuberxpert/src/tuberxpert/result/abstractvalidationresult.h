#ifndef ABSTRACTVALIDATIONRESULT_H
#define ABSTRACTVALIDATIONRESULT_H

#include <string>

namespace Tucuxi {
namespace Xpert {

/// \brief Enums that sets the "level" of the warning
///        returned by an AbstractResult class.
///
///         It can be:
///             - NORMAL = 0
///             - CRITICAL = 1
/// \date 02/06/2022
/// \author Herzig Melvyn
enum class WarningLevel
{
    NORMAL = 0,
    CRITICAL
};

/// \brief This class is the base class for all result classes except Global and Request.
///        A result class is always linked to a "source" pointer and
///        associates to it a warning (message) with a WarningLevel.
/// \date 02/06/2022
/// \author Herzig Melvyn
template <typename T>
class AbstractValidationResult
{
public:

    /// \brief Constructor.
    /// \param _source Pointer to the source object concerned by the result.
    /// \param _warning Warning message associated.
    AbstractValidationResult(const T* _source, const std::string& _warning) :
    m_source(_source), m_warning(_warning){}

    /// \brief Abstract destructor.
    virtual ~AbstractValidationResult(){};

    /// \brief Gets the source object of the result.
    /// \return A constant pointer to the source object.
    const T* getSource() const {
        return m_source;
    }

    /// \brief Gets the warning message.
    /// \return The warning message. May be empty string if none.
    const std::string& getWarning() const {
        return m_warning;
    }

    /// \brief Gets the warning level associated to a warning. This
    ///        doesn't consider if the warning is set or not.
    /// \return Returns WarningLevel::NORMAL. May be overrided.
    WarningLevel getWarningLevel() const{
        return WarningLevel::NORMAL;
    }

protected:

    /// \brief Source object targeted by the result.
    const T* m_source;

    /// \brief The associated warning message if necessary.
    std::string m_warning;
};

} // namespace Xpert
} // namespace Tucuxi

#endif // ABSTRACTVALIDATIONRESULT_H
