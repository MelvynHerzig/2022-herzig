#include "languagemanager.h"

#include "tucucommon/xmlattribute.h"
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlimporter.h"
#include "tucucommon/xmliterator.h"
#include "tucucommon/xmlnode.h"

#include "languageexception.h"

#include <fstream>

namespace Tucuxi {
namespace Language {

const std::string LanguageManager::s_dictionariesFolder = "../language/";

std::unique_ptr<LanguageManager> LanguageManager::s_upInstance{nullptr};

const std::string LanguageManager::s_defaultTranslation = "unknown translation";

std::mutex LanguageManager::s_mutex;

LanguageManager::LanguageManager(const std::string& _lang)
{
    std::ifstream ifs(s_dictionariesFolder + _lang + ".xml");

    std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    Tucuxi::Common::XmlDocument document;

    if (xmlString == "" || !document.fromString(xmlString)) {
        throw LanguageException("Error importing language file. It may be missing or bad formatted.");
    }

    Common::XmlNode root = document.getRoot();
    Common::XmlNodeIterator it = root.getChildren();

    while (it != Common::XmlNodeIterator::none()) {
        m_keyToEntry[it->getAttribute("key").getValue()] = it->getValue();
        it++;
    }
}

std::unique_ptr<LanguageManager>& LanguageManager::getInstance(const std::string& filePath)
{
    std::lock_guard<std::mutex> lock(s_mutex);
    if (s_upInstance == nullptr)
    {
        s_upInstance = std::make_unique<LanguageManager>(filePath);
    }
    return s_upInstance;
}

const std::string& LanguageManager::translate(const std::string &key) const
{
    auto it = m_keyToEntry.find(key);
    if (it != m_keyToEntry.end()) {
        return it->second;
    }
    else {
        return s_defaultTranslation;
    }
}


} // namespace Language
} // namespace Tucuxi
