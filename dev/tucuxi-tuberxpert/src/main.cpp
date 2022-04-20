#include <iostream>

#include "language/languagemanager.h"
#include "tucucommon/utils.h"

using namespace std;

int main(int _argc, char** _argv)
{
    // Get application folder
    std::string appFolder = Tucuxi::Common::Utils::getAppFolder(_argv);

    Tucuxi::Language::LanguageManager* pLangMgr = Tucuxi::Language::LanguageManager::getInstance();

    cout << appFolder << endl;
    return 0;
}
