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

# libraries
src_bitquick.subdir = BitQuick
SUBDIRS += src_bitquick

#modules
src_test_module.subdir = imports/Test
src_test_module.module = sub-test-module
src_test_module.depends = src_bitquick_tools
SUBDIRS += src_test_module

src_bitquick_tools.subdir = imports/BitQuickTools
src_bitquick_tools.module = sub-bitquicktools-module
src_bitquick_tools.depends = src_bitquick
SUBDIRS += src_bitquick_tools
