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
import BitQuick.Test 1.0
import QtTest 1.2
import BitQuick.Tools 1.0

Item {
    id: main
    width: 100
    height: 100

    Loader {
        id: loader
        asynchronous: false
    }

    Item {
        id: testItem
    }

    StateSaver {
        Component.onDestruction: reset();
    }

    BitTestCase {
        name: "StateSaver/1.0"

        SignalSpy {
            id: deadSpy
            signalName: "destruction"
        }

        function cleanup() {
            if (loader.item) {
                deadSpy.clear()
                deadSpy.target = loader.item.Component;
                loader.source = "";
                tryCompare(loader, "status", Loader.Null, 1000, "");
                deadSpy.wait(500);
            }
            deadSpy.clear();
        }

        function test_noid_data() {
            return [
                {tag: "simple property", file: "NoID.qml", line: 26, column: 1},
                {tag: "group property", file: "InvalidGroup.qml", line: 28, column: 43},
            ];
        }

        function test_noid(data) {
            ignoreWarningMessage(data.file, data.line, data.column, "QML Item: Warning: attachee must have an ID in order to save property states.");
            loader.source = data.file;
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");
        }

        function test_no_parent_id() {
            ignoreWarningMessage("NoParentId.qml", 26, 1, "QML Item: Warning: all parents must have an id defined to save the state.");
            loader.source = "NoParentId.qml";
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");
        }

        // test one property at a time
        function test_save_supported_values_data() {
            return [
                {tag: "int", property: "intProperty", value: 100},
                {tag: "bool", property: "boolProperty", value: true},
                {tag: "real", property: "realProperty", value: 123.45},
                {tag: "double", property: "doubleProperty", value: 98765.4321},
                {tag: "string", property: "stringProperty", value: "BitQuick is really quick"},
                {tag: "url", property: "urlProperty", value: Qt.resolvedUrl("NoParentId.qml")},
                {tag: "date", property: "dateProperty", value: new Date(2015, 10, 2)},
                {tag: "point", property: "pointProperty", value: Qt.point(-10, -20)},
                {tag: "rect", property: "rectProperty", value: Qt.rect(-10, -20, 50, 80)},
                {tag: "size", property: "sizeProperty", value: Qt.size(10, 40)},
                {tag: "color", property: "colorProperty", value: "#00ff00"},
                {tag: "font", property: "fontProperty", value: Qt.font({family: "Arial"})},
                {tag: "vector2d", property: "vector2dProperty", value: Qt.vector2d(3, 4)},
                {tag: "vector3d", property: "vector3dProperty", value: Qt.vector3d(4, 5, 6)},
                {tag: "vector4d", property: "vector4dProperty", value: Qt.vector4d(5, 6, 7, 8)},
                {tag: "quaternion", property: "quaternionProperty", value: Qt.quaternion(3, 2, 1, 0.5)},
                {tag: "matrix4x4", property: "matrix4x4Property", value: Qt.matrix4x4(16,15,14,13,12,11,10,9,8,7,6,5,7,3,2,1)},
                {tag: "group", group: "groupProperty", property: "width", value: 100},
                {tag: "enum", property: "horizontalAlignment", value: Text.AlignRight},
            ];
        }
        function test_save_supported_values(data) {
            loader.source = "SupportedTypes.qml";
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");

            verify(loader.item[data.property] != data.value);

            if (data.group) {
                var grp = loader.item[data.group];
                grp[data.property] = data.value;
            } else {
                loader.item[data.property] = data.value;
            }

            // unload document, and then reload
            deadSpy.target = loader.item.Component;
            loader.source = "";
            deadSpy.wait();
            loader.source = "SupportedTypes.qml";
            tryVerify(function () { return loader.item != null}, 1000);

            if (data.group) {
                var grp = loader.item[data.group];
                compare(grp[data.property], data.value);
            } else {
                compare(loader.item[data.property], data.value);
            }
        }

        function test_arrays_data() {
            return [
                        {tag: "integers", property: "intList", array: [10, 0]},
                        {tag: "to string", property: "intList", array: ["apple", "pear", "plum"]},
                    ];
        }
        function test_arrays(data) {
            loader.source = "Arrays.qml"
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");

            verify(loader.item[data.property].length != data.array.length);
            // change it
            loader.item[data.property] = data.array;

            // reload it
            deadSpy.target = loader.item.Component;
            loader.source = "";
            deadSpy.wait();
            loader.source = "Arrays.qml";
            tryVerify(function () { return loader.item != null}, 1000);

            compare(loader.item[data.property].length, data.array.length);
            for (var i in data.array.length) {
                compare(loader.item[data.property][i], data.array[i]);
            }
        }

        function test_unsupported_values_data() {
            return [
                {tag: "object", property: "object", value: testItem, message: "QML UnsupportedTypes: Warning: cannot save property \"object\" of QObject type."},
            ];
        }
        function test_unsupported_values(data) {
            loader.source = "UnsupportedTypes.qml";
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");

            verify(loader.item[data.property] != data.value);

            ignoreWarningMessage("UnsupportedTypes.qml", 26, 1, data.message);

            loader.item[data.property] = data.value;

            // unload document, and then reload
            deadSpy.target = loader.item.Component;
            loader.source = "";
            deadSpy.wait();
            loader.source = "UnsupportedTypes.qml";
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");

            expectFailContinue("", "the value is not set");
            compare(loader.item[data.property], data.value);
            // disable state saver so we don't get the warning message again
            loader.item.StateSaver.enabled = false;
        }

        function test_list_index() {
            loader.source = "ListIndex.qml";
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");

            compare(loader.item.currentIndex, 0);

            loader.item.currentIndex = 10;

            // unload document, and then reload
            deadSpy.target = loader.item.Component;
            loader.source = "";
            deadSpy.wait();
            loader.source = "ListIndex.qml";
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");

            compare(loader.item.currentIndex, 10);
        }

        function test_list_delegate_statesave() {
            loader.source = "ListIndex.qml";
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");

            verify(loader.item.currentItem);

            var label = findChild(loader.item, "label0");
            verify(label);
            label.text = "Altered string";

            // unload document, and then reload
            deadSpy.target = loader.item.Component;
            loader.source = "";
            deadSpy.wait();
            loader.source = "ListIndex.qml";
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");

            label = findChild(loader.item, "label0");
            compare(label.text, "Altered string");
        }

        function test_disabled() {
            loader.source = "Disabled.qml";
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");

            var initialValue = loader.item.currentIndex;
            loader.item.currentIndex = initialValue + 1;

            // unload document, and then reload
            deadSpy.target = loader.item.Component;
            loader.source = "";
            deadSpy.wait();
            loader.source = "Disabled.qml";
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");

            expectFailContinue("", "index must not be saved");
            compare(loader.item.currentIndex, initialValue + 1);
        }

        function test_readonly_property() {
            ignoreWarningMessage("Readonly.qml", 26, 1, "QML Readonly: Warning: cannot serialize read-only property \"readOnly\"");
            loader.source = "Readonly.qml";
            tryCompare(loader, "status", Loader.Ready, 1000, "file not found");

            // unload document, and then reload
            deadSpy.target = loader.item.Component;
            loader.source = "";
            deadSpy.wait();
        }
    }
}
