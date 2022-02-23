TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += console
CONFIG += c++14

TUCUXIROOTDIR = $$(TUCUXI_ROOT)

include($${TUCUXIROOTDIR}/make/qtcreator/general.pri)
include($${TUCUXIROOTDIR}/make/qtcreator/tucucommon.pri)
include($${TUCUXIROOTDIR}/make/qtcreator/tucucore.pri)

HEADERS += \
    src/communication/query.h \
    src/communication/xmlreader.h \
    src/communication/administrativedata.h \
    src/communication/parametersdata.h \
    src/communication/requestdata.h \
    src/api/HelloApi.h \
    src/impl/HelloApiImpl.h \
    src/api/ComputationApi.h \
    src/api/DrugsApi.h \
    src/impl/ComputationApiImpl.h \
    src/impl/DrugsApiImpl.h \
    src/computer.h \
    src/predictioncomputer.h \
    src/adjustmentcomputer.h \
    src/firstdosagecomputer.h \
    src/configuration.h \
    src/configurationreader.h \
    src/communication/apiresponse.h \
    src/communication/apiresponsewriter.h

SOURCES += $$PWD/src/main.cpp \
    src/communication/query.cpp \
    src/communication/xmlreader.cpp \
    src/communication/administrativedata.cpp \
    src/communication/parametersdata.cpp \
    src/communication/requestdata.cpp \
    src/api/HelloApi.cpp \
    src/impl/HelloApiImpl.cpp \
    src/api/ComputationApi.cpp \
    src/api/DrugsApi.cpp \
    src/impl/ComputationApiImpl.cpp \
    src/impl/DrugsApiImpl.cpp \
    src/computer.cpp \
    src/predictioncomputer.cpp \
    src/adjustmentcomputer.cpp \
    src/firstdosagecomputer.cpp \
    src/configuration.cpp \
    src/configurationreader.cpp \
    src/communication/apiresponse.cpp \
    src/communication/apiresponsewriter.cpp

win32{
LIBS += Iphlpapi.lib
}


unix{

LIBS += -lpthread

LIBS += $${TUCUXIROOTDIR}/bin/tinyjs.a \
        /usr/lib/x86_64-linux-gnu/libdl.so
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/pistache/build/src/release/ -lpistache
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/pistache/build/src/debug/ -lpistache
else:unix: LIBS += -L$$PWD/libs/pistache/build/src/ -lpistache

unix {
    INCLUDEPATH += $$PWD/libs/pistache/include
}
else {
    INCLUDEPATH += $$PWD/libs/pistache/build/src
}

DEPENDPATH += $$PWD/libs/pistache/build/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/pistache/build/src/release/libpistache.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/pistache/build/src/debug/libpistache.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/pistache/build/src/release/pistache.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/pistache/build/src/debug/pistache.lib
else:unix: PRE_TARGETDEPS += $$PWD/libs/pistache/build/src/libpistache.a
