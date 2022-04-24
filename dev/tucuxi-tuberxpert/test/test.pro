TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

unix {
    LIBS += -lpthread
}

win32 {
    include(../../tucuxi-core/make/qtcreator/tinyjs.pri)
}

include(../../tucuxi-core/make/qtcreator/general.pri)
include(../../tucuxi-core/make/qtcreator/tucucommon.pri)
include(../../tucuxi-core/make/qtcreator/tucucore.pri)
include(../src/language/tuculanguage.pri)

SOURCES += \
        main.cpp

!win32 {
    # Because of Eigen:
    QMAKE_CXXFLAGS += -Wno-int-in-bool-context

    # Because of macros and clang:
    QMAKE_CXXFLAGS += -Wno-extra-semi-stmt
}

DEFINES+= \
    test_language

HEADERS += \
    test_languagemanager.h
