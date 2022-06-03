TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt   

unix {
    LIBS += -lpthread
}

win32 {
    include(../tucuxi-core/make/qtcreator/tinyjs.pri)
}

include(../tucuxi-core/make/qtcreator/general.pri)
include(../tucuxi-core/make/qtcreator/tucucommon.pri)
include(../tucuxi-core/make/qtcreator/tucucore.pri)
include(../tucuxi-core/make/qtcreator/tucuquery.pri)

include(./src/tuberxpert.pri)

HEADERS += \

SOURCES += \
    src/tuberxpert.cpp \

!win32 {
    # Because of Eigen:
    QMAKE_CXXFLAGS += -Wno-int-in-bool-context

    # Because of macros and clang:
    QMAKE_CXXFLAGS += -Wno-extra-semi-stmt
}

OTHER_FILES += \
    language/dictionary.xsd \
    language/en.xml \
    language/fr.xml \
    query/tuberxpert_computing_query.xsd

