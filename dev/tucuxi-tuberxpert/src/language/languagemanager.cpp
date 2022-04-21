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

const std::string LanguageManager::dictionariesFolder = "../language/";

std::unique_ptr<LanguageManager> LanguageManager::upInstance{nullptr};

const std::string LanguageManager::defaultTranslation = "unknown translation";

std::mutex LanguageManager::mutex;

LanguageManager::LanguageManager(const std::string& lang)
{
    std::ifstream ifs(dictionariesFolder + lang + ".xml");

    std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    Tucuxi::Common::XmlDocument document;

    if (xmlString == "" || !document.fromString(xmlString)) {
        throw LanguageException("Error importing language file. It may be missing or bad formatted.");
    }

    Common::XmlNode root = document.getRoot();
    Common::XmlNodeIterator it = root.getChildren();

    while (it != Common::XmlNodeIterator::none()) {
        keyToEntry[it->getAttribute("key").getValue()] = it->getValue();
        it++;
    }
}

std::unique_ptr<LanguageManager>& LanguageManager::getInstance(const std::string& filePath)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (upInstance == nullptr)
    {
        upInstance = std::make_unique<LanguageManager>(filePath);
    }
    return upInstance;
}

const std::string& LanguageManager::translate(const std::string &key) const
{
    auto it = keyToEntry.find(key);
    if (it != keyToEntry.end()) {
        return it->second;
    }
    else {
        return defaultTranslation;
    }
}


} // namespace Language
} // namespace Tucuxi
