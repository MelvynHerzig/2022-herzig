#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <mutex>
#include <string>

namespace Tucuxi {
namespace Language {


/// \brief The language manager class is a singleton that loads an xml language
/// file and allows to get the translation strings with common identifiers.
/// \date 20/04/2022
/// \author Herzig Melvyn
class LanguageManager
{
public:

    /// \brief The only way to get a hold on the language manager.
    /// \return The language manager itself.
    static LanguageManager* getInstance();

private:
    /// \brief Constructor. Used internal to create the singleton instance.
    LanguageManager();

    // Singletons should not be cloneable.
    LanguageManager( LanguageManager& other) = delete;

    // Singletons should not be assignable.
    void operator=(const  LanguageManager&) = delete;

    /// \brief Path to translation fiiles.
    static std::string dictionariesFolder;

    /// \brief Language manager single instance.
    static LanguageManager* pInstance;

    /// \brief Mutex to handle multi threading creation.
    static std::mutex mutex;
};

} // namespace Language
} // namespace Tucuxi

#endif // LANGUAGEMANAGER_H
