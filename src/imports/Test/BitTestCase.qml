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
        Keeps executing an argument-less \a lambda until the \a expected result or
        the \a timeout is reached. On failure the \a message will be printed, and
        an exception will be thrown.
     */
    function tryCompareLambda(lambda, expected, timeout, message) {
        var elapsed = 0;
        var success = false;
        if (timeout === undefined) {
            timeout = 5000;
        }

        var result;
        while (elapsed < timeout && !success) {
            result = lambda();
            success = qtest_compareInternal(result, expected);
            if (!success) {
                wait(50);
                elapsed += 50;
            }
        }

        if (!qtest_results.compare(success, message, qtest_results.stringify(result), qtest_results.stringify(expected), util.callerFile(), util.callerLine())) {
            throw new Error("QtQuickTest::fail")
        }
    }
}
