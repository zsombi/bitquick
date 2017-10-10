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

TEMPLATE = app
QT += core gui qml quick testlib BitTestLib BitQuick_private
CONFIG += no_keywords

# add check target by hand, same way as in qmltest-include.pri
check.target = check

win32 {
    check.commands = "set QML2_IMPORT_PATH=$${ROOT_BUILD_DIR}/qml;"
    check.commands += "set QT_INSTALL_BINS=$$[QT_INSTALL_BINS];"
    check.commands += $$shadowed($$_PRO_FILE_PWD_)/$${TARGET}.exe
} else {
    check.commands = "set -e;"
    check.commands += "export QML2_IMPORT_PATH=$${ROOT_BUILD_DIR}/qml;"
    check.commands += "export QT_INSTALL_BINS=$$[QT_INSTALL_BINS];"
    check.commands += "export QT_QPA_PLATFORM=offscreen;"
    check.commands += "export QMLSCENE_DEVICE=softwarecontext;"
    check.commands += "export QML_DISABLE_DISK_CACHE=true;"
    macx {
        check.commands += "export DYLD_LIBRARY_PATH=$${ROOT_BUILD_DIR}/lib;"
        check.commands += "export DYLD_FRAMEWORK_PATH=$${ROOT_BUILD_DIR}/lib;"
        check.commands += $$shadowed($$_PRO_FILE_PWD_)/$${TARGET}.app/Contents/MacOS/$${TARGET}
    } else: unix {
        check.commands += $$shadowed($$_PRO_FILE_PWD_)/$${TARGET}
    }
}

check.commands += -maxwarnings 5000 -o $${ROOT_BUILD_DIR}/$${TEST}.xml,xunitxml -o -,txt
