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

TEMPLATE = app
QT += qml quick qmltest
CONFIG += no_keywords

# Do not use CONFIG += testcase that would add a 'make check' because it also
# adds a 'make install' that installs the test cases, which we do not want.
# Instead add a 'make check' manually.

message(Tests: $$TESTS)

check.target = check
check.commands = "set -e;"
check.commands += "export QML2_IMPORT_PATH=$${ROOT_BUILD_DIR}/qml;"
for(TEST, TESTS) {
  macx {
    _test_command = $$shadowed($$_PRO_FILE_PWD_)/$${TARGET}.app/Contents/MacOS/$${TARGET}
  } else: windows {
    _test_command = $$shadowed($$_PRO_FILE_PWD_)/$${TARGET}.exe
  } else: unix {
    _test_command = $$shadowed($$_PRO_FILE_PWD_)/$${TARGET}
  }

  _check_command = $$_test_command -import $${ROOT_BUILD_DIR}/qml -input $${_PRO_FILE_PWD_}/$${TEST} -maxwarnings 5000 -o $${ROOT_BUILD_DIR}/$${TEST}.xml,xunitxml -o -,txt

  check.commands += $${_check_command} ";"

  #add a convenience target per TEST file
  check_name = "check_$${TEST}"
  check_name = $$replace(check_name, "\.qml", "")
  check_name = $$replace(check_name, "\.", "_")
  $${check_name}.target   = $${check_name}
  $${check_name}.commands += $${_check_command}
  QMAKE_EXTRA_TARGETS+=$${check_name}
}
