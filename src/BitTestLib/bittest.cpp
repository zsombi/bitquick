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

#include "bittest.h"
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtQml/QQmlEngine>

#include <stdlib.h>

Q_DECLARE_METATYPE(QList<QQmlError>)

namespace BitQuick { namespace Test {

/*!
 * \class BitQuick::Test::BitTest
 * \ingroup bitquick_test
 * \brief BitTest is the C++ pendant to the QML BitTest.
 */
BitTest::BitTest(const QString& file, ResizeMode resize, bool assertOnFailure, QWindow* parent) : QQuickView(parent)
{
    qRegisterMetaType<QList <QQmlError> >();
    m_spy = new QSignalSpy(engine(), SIGNAL(warnings(QList<QQmlError>)));
    m_spy->setParent(this);

    Q_ASSERT(!file.isEmpty());
    setResizeMode(resize);
    setSource(QUrl::fromLocalFile(file));
    if (assertOnFailure) {
        Q_ASSERT(status() == QQuickView::Ready);
        Q_ASSERT(rootObject());
    }
    if (rootObject()) {
        show();
        QTest::qWaitForWindowExposed(this);
    }
}

/*!
 * Returns the number of all warnings from the point of loading the first line of QML code.
 */
int BitTest::warnings() const
{
    return m_spy->count();
}

/*!
 * \brief Ignore an expected warning message created using qWarning() or qmlWarning().
 * The \a fileName, \a line, \a column and \a message are defining the warning message
 * itself, and the \a occurences specifies how many times the message is expected.
 */
void BitTest::ignoreWarning(const QString& fileName, uint line, uint column, const QString& message, uint occurences)
{
    for (uint i = 0; i < occurences; i++) {
        QString url(QUrl::fromLocalFile(QFileInfo(fileName).absoluteFilePath()).toString());
        QString warning(QStringLiteral("%1:%2:%3: %4").arg(url).arg(line).arg(column).arg(message));
        QTest::ignoreMessage(QtWarningMsg, warning.toUtf8());
    }
}

/*!
 * \brief Helper method to wait for a signal \a spy for a given \a timeout. Signals
 * watched by the spy may be emitted by the time the execution reaches the place
 * where the spy is about to be interrogated, time when the spy's wait() function
 * may fail.
 */
void BitTest::waitForSignal(QSignalSpy *spy, int timeout)
{
    QVERIFY(spy);
    if (!spy->count()) {
        QVERIFY(spy->wait(timeout));
    } else {
        QCOMPARE(spy->count(), 1);
    }
}

}} // namespace BitQuick::Test
