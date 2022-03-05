/*
* Copyright (C) 2017 Tucuxi SA
*
* Bachelor's thesis
* Made by: Herzig Melvyn
*/

#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>

#include <QDir>
#include <QProcess>

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

    // Preparing tucucore
    QObject *parent = nullptr;
    QString program = "../tucuxi-core/make/qtcreator/tucucli/debug/tucucli";
    QStringList arguments;

    // Starting
    QProcess *myProcess = new QProcess(parent);
    myProcess->start(program, arguments);

    // Waiting execution end
    myProcess->waitForFinished();
    QString output( myProcess->readAllStandardOutput());

    // Printing result
    std::cout << "Hello from TuberXpert. App under building" <<  std::endl;
    std::cout << "Core output: " << std::endl;
    std::cout << output.toStdString() << std::endl;

    return EXIT_SUCCESS; //a.exec();
}
