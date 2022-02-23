TEMPLATE = app
CONFIG += QT
CONFIG += console
CONFIG -= app_bundle
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = tucuxpert

LIBS += -lpthread


include($$(TUCUXI_ROOT)/make/qtcreator/general.pri)
include($$(TUCUXI_ROOT)/make/qtcreator/tucucommon.pri)
include($$(TUCUXI_ROOT)/make/qtcreator/tucucore.pri)
include($$(TUCUXI_ROOT)/make/qtcreator/tucuquery.pri)
include($$(TUCUXI_ROOT)/make/qtcreator/tinyjs.pri)

#HEADERS +=
#    tucucommon/utils.h

SOURCES += \
    src/tucuxpert.cpp \
    src/tucuxperthandler.cpp \
    src/querycreator.cpp \
    src/requestcreator.cpp \
    src/drugmodelselector.cpp \
    src/xpertdata.cpp \
    libs/qcustomplot.cpp \
    src/htmlexporter.cpp \
    src/enlanguage.cpp \
    src/ilanguage.cpp \
    src/graphcreator.cpp \
    src/tools.cpp

# Because of Eigen:
QMAKE_CXXFLAGS += -Wno-int-in-bool-context -Wno-extra-semi-stmt

HEADERS += \
    src/tucuxperthandler.h \
    src/querycreator.h \
    src/requestcreator.h \
    src/drugmodelselector.h \
    src/xpertdata.h \
    libs/qcustomplot.h \
    src/ilanguage.h \
    src/htmlexporter.h \
    src/enlanguage.h \
    src/graphcreator.h \
    src/tools.h
