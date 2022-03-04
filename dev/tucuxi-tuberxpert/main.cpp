#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "tucuxi-tuberxpert_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    std::cout << "Hello from TuberXpert" << std::endl;

    return EXIT_SUCCESS;
}
