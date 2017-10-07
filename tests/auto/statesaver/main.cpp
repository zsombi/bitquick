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

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QProcessEnvironment>
#include <QtTest/QTest>
#include <BitTestLib/BitTest>

using namespace BitQuick::Test;

class StateSaverTest : public QObject
{
    Q_OBJECT

public:
    StateSaverTest(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

private Q_SLOTS:

    void test_save_on_quit()
    {
        QString app;
        app = qgetenv("QTDIR") + QStringLiteral("/bin/qmlscene");
        QStringList args;
        args << QStringLiteral("-I") << qgetenv("QML2_IMPORT_PATH");
        args << QStringLiteral("%1/%2").arg(TEST_DIR).arg("NormalClose.qml");
        qDebug() << "RUN" << app << args;

        QProcess test;
        test.start(app, args);
        test.waitForFinished();
        qDebug() << "ERROR" << test.readAllStandardError();
        qDebug() << "OUTPUT" << test.readAllStandardOutput();
    }
};

QTEST_MAIN(StateSaverTest)

#include "main.moc"
