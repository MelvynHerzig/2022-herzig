#include <iostream>
#include <memory>

#include "language/languagemanager.h"
#include "tucucommon/utils.h"

using namespace std;

int main()
{
    unique_ptr<Tucuxi::Language::LanguageManager>& upLangMgr = Tucuxi::Language::LanguageManager::getInstance("fr");

    cout << upLangMgr->translate("hello") << " " << upLangMgr->translate("world") << endl;
    return 0;
}
