TEMPLATE = app
CONFIG += console c++17
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
include(../../tucuxi-core/make/qtcreator/tucuquery.pri)
include(../src/tuberxpert/tuberxpert.pri)

SOURCES += \
        main.cpp \
        testutils.cpp

!win32 {
    # Because of Eigen:
    QMAKE_CXXFLAGS += -Wno-int-in-bool-context

    # Because of macros and clang:
    QMAKE_CXXFLAGS += -Wno-extra-semi-stmt
}

DEFINES+= \
    test_adjustmenttraitcreator \
    test_covariatevalidatorandmodelselector \
    test_dosevalidator \
    test_xpertqueryresultcreation \
    test_languagemanager \
    test_requestexecutor \
    test_samplevalidator \
    test_targetvalidator \
    test_xpertqueryimport \
    test_xpertquerytocoreextractor \
    test_xpertutils \

HEADERS += \
    test_adjustmenttraitcreator.h \
    test_covariatevalidatorandmodelselector.h \
    test_dosevalidator.h \
    test_xpertqueryresultcreation.h \
    test_languagemanager.h \
    test_requestexecutor.h \
    test_samplevalidator.h \
    test_targetvalidator.h \
    test_xpertqueryimport.h \
    test_xpertquerytocoreextractor.h \
    test_xpertutils.h \
    testutils.h

