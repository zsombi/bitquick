/*
 * BitQuick - a QtQuick utility library
 *
 * Copyright (C) 2017, bitWelder Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see
 * <http://www.gnu.org/licenses/>.requires(qtHaveModule(quick))
 *
 * Author: Zsombor Egri <zsombor.egri@bitwelder.fi>
 */

import QtQuick 2.8
import BitQuick.Test 1.0
import BitQuick.Tools 1.0

Item {
    id: main
    width: 100
    height: 100

    Loader {
        id: loader
    }

    BitTestCase {
        name: "StateSaver/1.0"

        function cleanup() {
            loader.source = "";
            tryCompare(loader, "status", Loader.Null, 1000, "");
        }

        function test_noid() {
            ignoreFormattedWarning("NoID.qml", 26, 1, "Warning: attachee must have an ID in order to save property states.");
            loader.source = "NoID.qml";
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");
        }
    }
}
