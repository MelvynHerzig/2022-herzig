#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <mutex>
#include <string>
#include <memory>
#include <map>
#include <vector>

namespace Tucuxi {
namespace XpertLanguage {

/// \brief The language manager class is a singleton that loads an xml language
/// file and allows to get the translation strings with common identifiers.
/// \date 20/04/2022
/// \author Herzig Melvyn
class LanguageManager
{
public:

    /// \brief Gets the unique instance of dictionary.
    /// \return The language manager itself.
    static LanguageManager& getInstance();

    /// \brief Gets a translation for a given key.
    /// \param _key Key to look for.
    /// \return Translated string.
    std::string translate(const std::string& _key) const;

    /// \brief Loads an xml dictionary from  xml string.
    ///  Each call erases the last.
    /// \param _xmlString Xml string to use.
    void loadDictionary(const std::string& _xmlString);

private:

    /// \brief Constructor. Used internal to create the singleton instance.
    LanguageManager();

    // Singleton should not be cloneable.
    LanguageManager( LanguageManager& _other) = delete;

    // Singleton should not be assignable.
    void operator=(const  LanguageManager& _other) = delete;

    /// \brief Language manager single instance.
    static std::unique_ptr<LanguageManager> s_upInstance;

    /// \brief Mutex to handle multi threading creation.
    static std::mutex s_mutex;

    /// \brief Map containing key to entry mapping.
    std::map<std::string, std::string> m_keyToEntry;

public:

    /// \brief String returned when key is not found when usgin translate method.
    static const std::string s_defaultTranslation;

    friend std::unique_ptr<LanguageManager> std::make_unique<LanguageManager>();
};

} // namespace Language
} // namespace Tucuxi

#endif // LANGUAGEMANAGER_H
