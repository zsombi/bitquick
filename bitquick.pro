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
TEMPLATE = subdirs

requires(qtHaveModule(quick))
load(qt_parts)

SUBDIRS += doc

sub_tests.CONFIG -= no_default_target
sub_tests.CONFIG -= no_default_install
sub_tests.depends = sub_src

OTHER_FILES += \
    .qmake.conf \
    features/bitquick_do_testing.prf \
    features/bitquick_common.prf \
    features/bitquick_qt_module.prf \
    features/bitquick_qml_plugin.prf \
    features/bitquick_qml_module.prf \
    features/bitquick_docs.prf \
    sync.profile
