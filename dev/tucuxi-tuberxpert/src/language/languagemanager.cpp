#include "languagemanager.h"

#include "tucucommon/xmlattribute.h"
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlimporter.h"
#include "tucucommon/xmliterator.h"
#include "tucucommon/xmlnode.h"

#include "languageexception.h"

#include <fstream>

using namespace std;

namespace Tucuxi {
namespace XpertLanguage {

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
    auto it = m_keyToEntry.find(key);
    if (it != m_keyToEntry.end()) {
        return it->second;
    }
    else {
        return s_defaultTranslation;
    }
}

void LanguageManager::loadDictionary(const string& _xmlString)
{
    m_keyToEntry.clear();
    Tucuxi::Common::XmlDocument document;

    if (_xmlString == "" || !document.fromString(_xmlString)) {
        throw LanguageException("Error importing language file. It may be badly formatted.");
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

string LanguageManager::computeLanguageFileName(XpertQuery::OutputLang _lang)
{
    string fileName;

    switch(_lang) {
    case Tucuxi::XpertQuery::OutputLang::ENGLISH : fileName = "en.xml"; break;
    case Tucuxi::XpertQuery::OutputLang::FRENCH  : fileName = "fr.xml"; break;
    default : throw LanguageException("Unknown language");
    }

    return fileName;
}


} // namespace Language
} // namespace Tucuxi
