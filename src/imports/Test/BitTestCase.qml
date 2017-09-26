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
import QtTest 1.2
import BitQuick.Test 1.0

/*!
    \qmltype BitTestCase
    \ingroup bitquick_test
    \inqmlmodule BitQuick.Test 1.0
    \brief Extends QtQuick TestCase with additional functions.

    BitTestCase is a QML element which expands the standard QtQuick.TestCase element
    with additional test functions. In order to benefit fo those functions, simply
    replace tour TestCase elemenst in your unit tests with BitTestCase.
 */
TestCase {

    BitQuickTest {
        id: bitUtil
    }
    TestUtil {
        id: util
    }

    /*!
        Locates a child of \a object with \a objectName. Looks for both visible and
        invisible children.
     */
    function findChild(object, objectName) {
        return bitUtil.findChild(object, objectName);
    }

    /*!
        Ignores a warning \a message on a given \a file, \a line and \a column.
     */
    function ignoreWarningMessage(file, line, column, message) {
        var f = util.callerFile(0);
        ignoreWarning(f.substring(0, f.lastIndexOf('/') + 1) + file + ':' + line + ':' + column + ': ' + message);
    }
}
