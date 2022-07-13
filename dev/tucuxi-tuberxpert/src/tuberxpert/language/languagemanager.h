#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <mutex>
#include <string>
#include <memory>
#include <map>
#include <vector>

#include "tuberxpert/query/xpertrequestdata.h"

namespace Tucuxi {
namespace Xpert {

/// \brief The language manager class is a singleton that loads an xml translations
///        file and allows to get some translation strings with unique identifiers.
/// \date 20/04/2022
/// \author Herzig Melvyn
class LanguageManager
{
public:

    /// \brief Get the unique instance of LanguageManager.
    /// \return The language manager unique instance.
    static LanguageManager& getInstance();

    /// \brief Get the translation of a given key.
    /// \param _key Key to look for translation.
    /// \return Translated string.
    std::string translate(const std::string& _key) const;

    /// \brief Load an xml translations file from an xml string.
    ///        Each call clears and reloads the map that stores the translations.
    /// \param _xmlString Translations xml string.
    /// \throw LanguageException if the xml string could not be imported.
    void loadTranslations(const std::string& _xmlString);

private:

    /// \brief Constructor. Used internally to create the singleton instance.
    LanguageManager();

    /// \brief Singleton should not be clonable.
    LanguageManager( LanguageManager& _other) = delete;

    /// \brief Singleton should not be assignable.
    void operator=(const  LanguageManager& _other) = delete;

public:

    /// \brief String returned when a key is not found when using "translate" method.
    static const std::string s_defaultTranslation;

private:

    /// \brief LanguageManager unique instance.
    static std::unique_ptr<LanguageManager> s_upInstance;

    /// \brief Mutex to manage multi threading creation (just in case).
    static std::mutex s_mutex;

    /// \brief Map containing the key to translation mapping.
    std::map<std::string, std::string> m_keyToTranslation;

    // Otherwise singleton does not work.
    friend std::unique_ptr<LanguageManager> std::make_unique<LanguageManager>();
};

} // namespace Xpert
} // namespace Tucuxi

#endif // LANGUAGEMANAGER_H
