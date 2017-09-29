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

Text {
    id: test
    property int intProperty: -1
    property bool boolProperty: false
    property real realProperty: -1.2
    property double doubleProperty: 123.456
    property string stringProperty: "this is a string"
    property url urlProperty: Qt.resolvedUrl("NoID.qml")
    property date dateProperty: new Date();
    property point pointProperty: Qt.point(10, 20)
    property rect rectProperty: Qt.rect(0, 0, 20, 20)
    property size sizeProperty: Qt.size(20, 30)
    property color colorProperty: "red"
    property font fontProperty: Qt.font({family: "Courrier New"})
    property vector2d vector2dProperty: Qt.vector2d(1, 2)
    property vector3d vector3dProperty: Qt.vector3d(1, 2, 3)
    property vector4d vector4dProperty: Qt.vector4d(1, 2, 3, 4)
    property quaternion quaternionProperty: Qt.quaternion(0.5, 1, 2, 3)
    property matrix4x4 matrix4x4Property: Qt.matrix4x4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16)
    readonly property Rectangle groupProperty: Rectangle {
        id: grp
        width: 10
    }
    horizontalAlignment: Text.AlignHCenter

    StateSaver.properties: "intProperty, boolProperty, realProperty, doubleProperty, stringProperty, urlProperty,"+
                           "dateProperty, pointProperty, rectProperty, sizeProperty, colorProperty, fontProperty," +
                           "vector2dProperty, vector3dProperty, vector4dProperty, quaternionProperty," +
                           "matrix4x4Property, groupProperty.width," +
                           "horizontalAlignment"

    // make sure we have the attached signals so we can wait for its destruction
    Component.onCompleted: {}
}
