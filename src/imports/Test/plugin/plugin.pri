# BitQuick - a QtQuick utility library
#
# Copyright (C) 2017  bitWelder Ltd
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
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
# <http://www.gnu.org/licenses/>.requires(qtHaveModule(quick))
#
# Author: Zsombor Egri <zsombor.egri@bitwelder.fi>

QT += testlib qml quick quick-private

DEFINES += BITQUICK_QML_IMPORT_PATH='\\"$${ROOT_BUILD_DIR}/qml\\"'

TARGET=$$qtLibraryTarget($$TARGET)
URI=BitQuick.Test

HEADERS += \
    $$PWD/testplugin.h \
    $$PWD/bitquicktest.h

SOURCES += \
    $$PWD/testplugin.cpp \
    $$PWD/bitquicktest.cpp
