/*
 * BitQuick - a QtQuick utility library
 *
 * Copyright (C) 2017, bitWelder Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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
 * <http://www.gnu.org/licenses/>
 *
 * Author: Zsombor Egri <zsombor.egri@bitwelder.fi>
 */

import QtQuick 2.9
import BitQuick.Tools 1.0

Item {
    property var stringArray: ["false", "false"]
    property int runCount: 0
    id: testItem
    objectName: "testItem"
    StateSaver.properties: "stringArray, runCount"
    StateSaver.enabled: runCount < 2

    Timer {
        id: autoClose
        interval: 1000
        running: false
        onTriggered: Qt.quit();
    }
    StateSaver {
        id: saver
        applicationName: "NormalClose"
        Component.onDestruction: {
            if (testItem.runCount > 1) {
                console.log("RESET");
                saver.reset();
            }
        }
    }

    Component.onCompleted: {
        autoClose.running = true
        runCount++;
    }
}
