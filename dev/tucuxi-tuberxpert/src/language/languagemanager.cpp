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

std::unique_ptr<LanguageManager> LanguageManager::s_upInstance{nullptr};

const std::string LanguageManager::s_defaultTranslation = "unknown translation";

std::mutex LanguageManager::s_mutex;

LanguageManager::LanguageManager(){};

LanguageManager& LanguageManager::getInstance()
{
    std::lock_guard<std::mutex> lock(s_mutex);
    if (s_upInstance == nullptr)
    {
        s_upInstance = std::make_unique<LanguageManager>();
    }
    return *s_upInstance;
}

std::string LanguageManager::translate(const std::string &key) const
{
    auto it = m_keyToEntry.find(key);
    if (it != m_keyToEntry.end()) {
        return it->second;
    }
    else {
        return s_defaultTranslation;
    }
}

void LanguageManager::loadDictionary(const std::string& _xmlString)
{
    m_keyToEntry.clear();
    Tucuxi::Common::XmlDocument document;

    if (_xmlString == "" || !document.fromString(_xmlString)) {
        throw LanguageException("Error importing language file. It may be missing or bad formatted.");
    }

    Common::XmlNode root = document.getRoot();
    Common::XmlNodeIterator it = root.getChildren();


    // Iterating over child elements of root.
    while (it != Common::XmlNodeIterator::none()) {

        // Check if named correctly.
        if(it->getName() != "entry") {
            throw LanguageException("element name must be entry.");
        }

        // Check if only attribute is "key"
        Common::XmlAttributeIterator ait = it->getAttributes();

        if (ait == Common::XmlAttributeIterator::none()) {
            throw LanguageException("entry need one attribute.");
        }

        while (ait != Common::XmlAttributeIterator::none()) {
            if(ait->getName() != "key") {
                throw LanguageException("attribute expected must be key.");
            }
            ait++;
        }

        m_keyToEntry[it->getAttribute("key").getValue()] = it->getValue();
        it++;
    }
}


} // namespace Language
} // namespace Tucuxi
