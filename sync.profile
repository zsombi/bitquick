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

%modules = ( # path to module name map
    "BitQuick" => "$basedir/src/BitQuick",
);
%moduleheaders = ( # restrict the module headers to those found in relative path
);

%classnames = (
 #add classnames that are not automatically detected
 #e.g. "someheader.h" => "SomeType"
 "bitquickglobals.h" => "BitQuickGlobals",
 "bitquicktoolsmodule.h" => "BitQuickToolsModule",
);

# Module dependencies.
# Every module that is required to build this module should have one entry.
# Each of the module version specifiers can take one of the following values:
#   - A specific Git revision.
#   - any git symbolic ref resolvable from the module's repository (e.g. "refs/heads/master" to track master branch)
#
%dependencies = (

    "qtbase" => "",
    "qtdeclarative" => "",
);
