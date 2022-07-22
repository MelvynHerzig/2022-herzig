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
        tests/test_adjustmenttraitcreator.cpp \
        tests/test_covariatevalidatorandmodelselector.cpp \
        tests/test_dosevalidator.cpp \
        tests/test_languagemanager.cpp \
        tests/test_requestexecutor.cpp \
        tests/test_samplevalidator.cpp \
        tests/test_targetvalidator.cpp \
        tests/test_xpertqueryimport.cpp \
        tests/test_xpertqueryresultcreation.cpp \
        tests/test_xpertquerytocoreextractor.cpp \
        tests/test_xpertutils.cpp \
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
    tests/test_adjustmenttraitcreator.h \
    tests/test_covariatevalidatorandmodelselector.h \
    tests/test_dosevalidator.h \
    tests/test_xpertqueryresultcreation.h \
    tests/test_languagemanager.h \
    tests/test_requestexecutor.h \
    tests/test_samplevalidator.h \
    tests/test_targetvalidator.h \
    tests/test_xpertqueryimport.h \
    tests/test_xpertquerytocoreextractor.h \
    tests/test_xpertutils.h \
    testutils.h

