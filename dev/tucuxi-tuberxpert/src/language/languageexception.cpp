#include "languageexception.h"

namespace Tucuxi {
namespace Language {

LanguageException::LanguageException(const std::string& msg) : std::runtime_error(msg) {};

LanguageException::LanguageException(const char* msg) : std::runtime_error(msg) {};

} // namespace Language
} // namespace Tucuxi
