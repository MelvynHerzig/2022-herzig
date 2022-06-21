
INCLUDEPATH += $$PWD/../../src

HEADERS += \
    $$PWD/flow/abstract/abstractxpertflowstep.h \
    $$PWD/flow/abstract/abstractxpertflowstepprovider.h \
    $$PWD/flow/general/covariatevalidatorandmodelselector.h \
    $$PWD/flow/general/adjustmenttraitcreator.h \
    $$PWD/flow/general/dosevalidator.h \
    $$PWD/flow/general/generalxpertflowstepprovider.h \
    $$PWD/flow/general/samplevalidator.h \
    $$PWD/flow/general/targetvalidator.h \
    $$PWD/query/xpertadministrativedata.h \
    $$PWD/query/xpertquerydata.h \
    $$PWD/query/xpertqueryimport.h \
    $$PWD/query/xpertquerytocoreextractor.h \
    $$PWD/query/xpertrequestdata.h \
    $$PWD/language/languageexception.h \
    $$PWD/language/languagemanager.h \
    $$PWD/result/abstractresult.h \
    $$PWD/result/covariateresult.h \
    $$PWD/result/doseresult.h \
    $$PWD/result/sampleresult.h \
    $$PWD/result/xpertrequestresult.h \
    $$PWD/result/xpertresult.h \
    $$PWD/utils/xpertutils.h \
    $$PWD/tuberxpertcomputer.h

SOURCES += \
    $$PWD/flow/general/covariatevalidatorandmodelselector.cpp \
    $$PWD/flow/general/adjustmenttraitcreator.cpp \
    $$PWD/flow/general/dosevalidator.cpp \
    $$PWD/flow/general/generalxpertflowstepprovider.cpp \
    $$PWD/flow/general/samplevalidator.cpp \
    $$PWD/flow/general/targetvalidator.cpp \
    $$PWD/query/xpertadministrativedata.cpp \
    $$PWD/query/xpertquerydata.cpp \
    $$PWD/query/xpertqueryimport.cpp \
    $$PWD/language/languageexception.cpp \
    $$PWD/language/languagemanager.cpp \
    $$PWD/query/xpertquerytocoreextractor.cpp \
    $$PWD/query/xpertrequestdata.cpp \
    $$PWD/result/covariateresult.cpp \
    $$PWD/result/doseresult.cpp \
    $$PWD/result/sampleresult.cpp \
    $$PWD/result/xpertrequestresult.cpp \
    $$PWD/result/xpertresult.cpp \
    $$PWD/utils/xpertutils.cpp \
    $$PWD/tuberxpertcomputer.cpp
