#include "languagemanager.h"

#include <fstream>

#include "tucucommon/xmlattribute.h"
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlimporter.h"
#include "tucucommon/xmliterator.h"
#include "tucucommon/xmlnode.h"

#include "tuberxpert/language/languageexception.h"

using namespace std;

namespace Tucuxi {
namespace Xpert {

unique_ptr<LanguageManager> LanguageManager::s_upInstance{nullptr};

const string LanguageManager::s_defaultTranslation = "unknown translation";

mutex LanguageManager::s_mutex;

LanguageManager::LanguageManager(){};

LanguageManager& LanguageManager::getInstance()
{
    lock_guard<mutex> lock(s_mutex);
    if (s_upInstance == nullptr)
    {
        s_upInstance = make_unique<LanguageManager>();
    }
    return *s_upInstance;
}

string LanguageManager::translate(const string &key) const
{
    // Try to find the key.
    auto it = m_keyToTranslation.find(key);

    // Key found!
    if (it != m_keyToTranslation.end()) {
        return it->second;
    }
    // Key not found!
    else {
        return s_defaultTranslation;
    }
}

void LanguageManager::loadTranslations(const string& _xmlString)
{
    m_keyToTranslation.clear();
    Common::XmlDocument document;

    if (_xmlString == "" || !document.fromString(_xmlString)) {
        throw LanguageException("Error importing language file. It may be badly formatted.");
    }

    Common::XmlNode root = document.getRoot();
    Common::XmlNodeIterator childrenIt = root.getChildren();


    // Iterating over child elements of root.
    while (childrenIt != Common::XmlNodeIterator::none()) {

        // Check if the name of the element is correct.
        if (childrenIt->getName() != "translation") {
            throw LanguageException("Element name must be \"translation\".");
        }

        // Check if only attribute is named "key".
        Common::XmlAttributeIterator attributeIt = childrenIt->getAttributes();

        if (attributeIt == Common::XmlAttributeIterator::none()) {
            throw LanguageException("Translation element needs one attribute.");
        }

        while (attributeIt != Common::XmlAttributeIterator::none()) {
            if (attributeIt->getName() != "key") {
                throw LanguageException("Only one attribute named \"key\" expected.");
            }
            attributeIt++;
        }

        m_keyToTranslation[childrenIt->getAttribute("key").getValue()] = childrenIt->getValue();
        childrenIt++;
    }
}


} // namespace Xpert
} // namespace Tucuxi
