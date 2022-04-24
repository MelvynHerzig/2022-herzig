#include "languageexception.h"

namespace Tucuxi {
namespace Language {

LanguageException::LanguageException(const std::string& _msg) : std::runtime_error(_msg) {};

LanguageException::LanguageException(const char* _msg) : std::runtime_error(_msg) {};

} // namespace Language
} // namespace Tucuxi
