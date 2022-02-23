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
    src/impl/HelloApiImpl.h

SOURCES += $$PWD/src/main.cpp \
    src/communication/query.cpp \
    src/communication/xmlreader.cpp \
    src/communication/administrativedata.cpp \
    src/communication/parametersdata.cpp \
    src/communication/requestdata.cpp \
    src/api/HelloApi.cpp \
    src/impl/HelloApiImpl.cpp

win32{
LIBS += Iphlpapi.lib
}


unix{

LIBS += -lpthread

LIBS += $${TUCUXIROOTDIR}/bin/tinyjs.a \
        /usr/lib/x86_64-linux-gnu/libdl.so
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libs/pistache/build/src/release/ -lpistache
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libs/pistache/build/src/debug/ -lpistache
else:unix: LIBS += -L$$PWD/../libs/pistache/build/src/ -lpistache

INCLUDEPATH += $$PWD/../libs/pistache/include
DEPENDPATH += $$PWD/../libs/pistache/build/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../libs/pistache/build/src/release/libpistache.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../libs/pistache/build/src/debug/libpistache.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../libs/pistache/build/src/release/pistache.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../libs/pistache/build/src/debug/pistache.lib
else:unix: PRE_TARGETDEPS += $$PWD/../libs/pistache/build/src/libpistache.a
