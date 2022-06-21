#ifndef LANGUAGEEXCEPTION_H
#define LANGUAGEEXCEPTION_H

#include <stdexcept>

namespace Tucuxi {
namespace Xpert {

/// \brief Custom exception used for language problems.
/// \date 21/04/2022
/// \author Herzig Melvyn
class LanguageException : public std::runtime_error
{
public:
    /// \brief String constructor.
    /// \param _msg Message to embeed.
    explicit LanguageException(const std::string& _msg);

    /// \brief Char array constructor.
    /// \param _msg Message to embeed.
    explicit LanguageException(const char* _msg);
};

} // namespace Xpert
} // namespace Tucuxi

#endif // LANGUAGEEXCEPTION_H
