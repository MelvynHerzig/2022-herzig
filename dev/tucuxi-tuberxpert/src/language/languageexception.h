#ifndef LANGUAGEEXCEPTION_H
#define LANGUAGEEXCEPTION_H

#include <stdexcept>

namespace Tucuxi {
namespace XpertLanguage {

/// \brief Custom exception used for language problems
/// \date 21/04/2022
/// \author Herzig Melvyn
class LanguageException : public std::runtime_error
{
public:

    /// \brief Constructor for string.

    /// \brief Constructor
    /// \param _msg Message to embeed.
    explicit LanguageException(const std::string& _msg);

    /// \brief Constructor
    /// \param _msg Message to embeed.
    explicit LanguageException(const char* _msg);
};

} // namespace Language
} // namespace Tucuxi

#endif // LANGUAGEEXCEPTION_H
