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

import QtQuick 2.8
import BitQuick.Test 1.0

Item {
    id: main
    width: 100
    height: 100

    ListView {
        id: list
        anchors.fill: parent
        model: 10
        delegate: Rectangle {
            width: parent.width
            height: 10
            objectName: "ListItem" + modelData
            property int itemIndex: index
        }
    }


    BitTestCase {
        name: "BitTestCase"

        function test_findChild() {
            var item = findChild(main, "ListItem5");
            verify(item);
            compare(item.itemIndex, 5);
        }

        function test_tryCompare_pass() {
            tryCompareLambda(function() { return list.model; }, 10, 100, "unexpected failure");
        }
    }
}
