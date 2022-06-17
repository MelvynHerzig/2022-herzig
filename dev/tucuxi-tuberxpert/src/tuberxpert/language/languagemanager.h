#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <mutex>
#include <string>
#include <memory>
#include <map>
#include <vector>

#include "tuberxpert/query/xpertrequestdata.h"

namespace Tucuxi {
namespace XpertLanguage {

/// \brief The language manager class is a singleton that loads an xml language
///        file and allows to get some translation strings with unique identifiers.
/// \date 20/04/2022
/// \author Herzig Melvyn
class LanguageManager
{
public:

    /// \brief Gets the unique instance of LanguageManager.
    /// \return The language manager itself.
    static LanguageManager& getInstance();

    /// \brief Gets a translation for a given key.
    /// \param _key Key to look for translation.
    /// \return Translated string.
    std::string translate(const std::string& _key) const;

    /// \brief Loads an xml dictionary from an xml string.
    ///        Each call erase and reload the map that stores the translations.
    /// \param _xmlString Xml string to use.
    /// \throw LanguageException If the xml string could not be imported.
    void loadDictionary(const std::string& _xmlString);

private:

    /// \brief Constructor. Used internal to create the singleton instance.
    LanguageManager();

    /// \brief Singleton should not be cloneable.
    LanguageManager( LanguageManager& _other) = delete;

    /// \brief Singleton should not be assignable.
    void operator=(const  LanguageManager& _other) = delete;

    /// \brief LanguageManager unique instance.
    static std::unique_ptr<LanguageManager> s_upInstance;

    /// \brief Mutex to handle multi threading creation (just in case).
    static std::mutex s_mutex;

    /// \brief Map containing key to translation mapping.
    std::map<std::string, std::string> m_keyToEntry;

public:

    /// \brief String returned when a key is not found when using "translate" method.
    static const std::string s_defaultTranslation;

    // Otherwise singleton does not work.
    friend std::unique_ptr<LanguageManager> std::make_unique<LanguageManager>();
};

} // namespace Language
} // namespace Tucuxi

#endif // LANGUAGEMANAGER_H
