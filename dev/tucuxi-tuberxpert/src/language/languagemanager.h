#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <mutex>
#include <string>
#include <memory>
#include <map>
#include <vector>

namespace Tucuxi {
namespace Language {


/// \brief The language manager class is a singleton that loads an xml language
/// file and allows to get the translation strings with common identifiers.
/// \date 20/04/2022
/// \author Herzig Melvyn
class LanguageManager
{
public:

    /// \brief Gets the unique instance of dictionary. If not already created, load it using lang argument.
    /// The argument is ignored when getInstance has already been called once.
    /// \param lang Lang to use. Default "en". May be "it", "de", "fr" if such xml files exist.
    /// \return The language manager itself.
    static std::unique_ptr<LanguageManager>& getInstance(const std::string& lang = "en");

    /// \brief Gets a translation for a given key.
    /// \param key Key to look for.
    /// \return Translated string.
    const std::string& translate(const std::string& key) const;

private:

    /// \brief Constructor. Used internal to create the singleton instance.
    /// \param lang Lang to use. Default "en". May be "it", "de", "fr" if such xml files exist.
    LanguageManager(const std::string& lang = "en");

    // Singletons should not be cloneable.
    LanguageManager( LanguageManager& other) = delete;

    // Singletons should not be assignable.
    void operator=(const  LanguageManager&) = delete;

    /// \brief Path to translation fiiles.
    const static std::string dictionariesFolder;

    /// \brief Language manager single instance.
    static std::unique_ptr<LanguageManager> upInstance;

    /// \brief Mutex to handle multi threading creation.
    static std::mutex mutex;

    /// \brief Map containing key to entry mapping.
    std::map<std::string, std::string> keyToEntry;

public:

    /// \brief String returned when key is not found when usgin translate method.
    static const std::string defaultTranslation;

    // To allow make_to create the instance.
    friend std::unique_ptr<LanguageManager> std::make_unique<LanguageManager>(const std::string&);
};

} // namespace Language
} // namespace Tucuxi

#endif // LANGUAGEMANAGER_H
