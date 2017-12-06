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

TEMPLATE = aux
CONFIG += bitquick_qml_module

uri = BitQuick.Layouts
installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

# qmldir
QMLDIR_FILE = qmldir

# define deployment for found deployables
qmldir_file.installPath = $$installPath
qmldir_file.files = $$QMLDIR_FILE

BITQUICK_QML_MODULE_FILES += qmldir_file

plugins_qmltypes.path = $$installPath
plugins_qmltypes.files = plugins.qmltypes
# Silence spam on stderr due to fonts
# https://bugs.launchpad.net/ubuntu-ui-toolkit/+bug/1256999
# https://bugreports.qt-project.org/browse/QTBUG-36243
plugins_qmltypes.extra = $$[QT_INSTALL_BINS]/qmlplugindump -notrelocatable BitQuick.Layouts 1.0 ../../ 2>/dev/null > $(INSTALL_ROOT)/$$installPath/plugins.qmltypes

INSTALLS += plugins_qmltypes

