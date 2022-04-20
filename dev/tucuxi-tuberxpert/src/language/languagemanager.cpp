#include "languagemanager.h"

#include "tucucommon/xmlattribute.h"
#include "tucucommon/xmldocument.h"
#include "tucucommon/xmlimporter.h"
#include "tucucommon/xmliterator.h"
#include "tucucommon/xmlnode.h"

#include <fstream>

namespace Tucuxi {
namespace Language {

std::string LanguageManager::dictionariesFolder = "../language/";

LanguageManager* LanguageManager::pInstance{nullptr};

std::mutex LanguageManager::mutex;

LanguageManager::LanguageManager()
{
    std::ifstream ifs(dictionariesFolder + "en.xml");

    // TODO check opening success

    std::string xmlString((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    Tucuxi::Common::XmlDocument document;

    if (!document.fromString(xmlString)) {
       // TODO check if could be formed
    }

     Common::XmlNode root = document.getRoot();
     Common::XmlNodeIterator it = root.getChildren();

     while (it != Common::XmlNodeIterator::none()) {
         std::cout<< "Attribute: " << it->getAttribute("key").getValue() << " Value: " << it->getValue() << std::endl;
         it++;
     }

}

LanguageManager* LanguageManager::getInstance()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (pInstance == nullptr)
    {
        pInstance = new LanguageManager();
    }
    return pInstance;
}


} // namespace Language
} // namespace Tucuxi
