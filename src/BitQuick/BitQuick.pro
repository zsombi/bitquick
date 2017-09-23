TARGET=BitQuick
TEMPLATE=lib
VERSION = 0.1.0

QT -= gui
QT += core-private

CONFIG += hide_symbols

DESTDIR = $${ROOT_LIBS_PATH}

DEFINES += BITQUICK_LIBRARY

HEADERS += \
    bitquickglobals.h \
    bitquicktoolsmodule.h

SOURCES += \
    bitquicktoolsmodule.cpp

load(bitquick_qt_module)
