# BitQuick - a QtQuick utility library
#
# Copyright (C) 2017  bitWelder Ltd
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see
# <http://www.gnu.org/licenses/>
#
# Author: Zsombor Egri <zsombor.egri@bitwelder.fi>

TARGET = BitLayouts
TEMPLATE = lib
VERSION = 1.0.0

QT += core-private qml quick qml-private quick-private BitQuick

CONFIG += hide_symbols

DESTDIR = $${ROOT_LIBS_PATH}

DEFINES += BITQUICK_LIBRARY

HEADERS += \
    bitlayoutsmodule.h \
    dynamiclayout_p.h \
    dynamiclayout_p_p.h \
    layoutcontainer_p.h \
    layoutcontainer_p_p.h

SOURCES += \
    bitlayoutsmodule.cpp \
    dynamiclayout.cpp \
    layoutcontainer.cpp

load(bitquick_qt_module)
