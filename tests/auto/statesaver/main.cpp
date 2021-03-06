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

#include <signal.h>

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtCore/QProcessEnvironment>
#include <QtTest/QTest>
#include <QtCore/QLibraryInfo>
#include <BitTestLib/BitTest>
#include <BitQuick/private/statesaver_p.h>

using namespace BitQuick::Test;

class TestProcess : public QProcess
{
    Q_OBJECT
public:
    explicit TestProcess(const char *qmlFile)
    {
        setProcessChannelMode(QProcess::SeparateChannels);
        QStringList env = QProcessEnvironment::systemEnvironment().toStringList();
        env.append(QStringLiteral("QT_LOGGING_RULES=BitQuick.StateSaver.Storage.debug=true"));
        setEnvironment(env);
        scene = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QStringLiteral("/qmlscene");
        args << QStringLiteral("-I") << qgetenv("QML2_IMPORT_PATH");
        args << QStringLiteral("%1/%2").arg(TEST_DIR).arg(qmlFile);
    }
    ~TestProcess()
    {
        // delete state file if wasn't deleted
        if (!stateFile.isEmpty()) {
            QFile::remove(stateFile);
        }
    }

    void run()
    {
        logs.clear();
        start(scene, args);
        QVERIFY(waitForStarted(5000));
    }

    bool waitForLogs(int msec = 30000)
    {
        bool result = waitForFinished(msec);
        // prepare logs
        QString slogs = QLatin1String(readAllStandardError());
        logs = slogs.remove(QStringLiteral("qml: ")).split(QLatin1Char('\n'), QString::SkipEmptyParts);
        // extract storage file
        for (QString log : qAsConst<QStringList>(logs)) {
            int idx = log.indexOf(QStringLiteral("STORAGE="));
            if (idx >= 0) {
                stateFile = log.remove(idx, QStringLiteral("STORAGE=").length());
                stateFile.remove(QStringLiteral("BitQuick.StateSaver.Storage:  "));
                stateFile.remove(QLatin1Char('"'));
            }
        }
//        qDebug() << "LOGS:" << logs;
        return result;
    }

    int intLog(const QString &id)
    {
        for (QString log : qAsConst<QStringList>(logs)) {
            int idx = log.indexOf(id + QStringLiteral("="));
            if (idx >= 0) {
                log.remove(idx, id.length() + QStringLiteral("= ").length());
                return log.toInt(nullptr, 10);
            }
        }
        return -1;
    }

    bool hasLog(const QString &log)
    {
        return logs.contains(log);
    }

private:
    QString scene;
    QStringList args;
    QStringList logs;
    QString stateFile;
};

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

        TestProcess test("NormalClose.qml");
        test.run();
        test.waitForLogs();
        QVERIFY(!test.hasLog(QStringLiteral("RESET")));

        // run it again!
        test.run();
        test.waitForLogs();
        QVERIFY(test.hasLog(QStringLiteral("RESET")));
    }

    void test_terminated()
    {
        TestProcess test("AbnormalInterrupt.qml");
        test.run();
        QVERIFY(test.waitForStarted());
        // wait a bit
        QTest::qWait(500);
        // kill
        test.terminate();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("runCount")), 1);
        QCOMPARE(test.intLog(QStringLiteral("lastSaveStatus")), int(BitQuick::Tools::StateSaver::Undefined));

        // re-launch
        test.run();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("runCount")), 2);
        QCOMPARE(test.intLog(QStringLiteral("lastSaveStatus")), int(BitQuick::Tools::StateSaver::Terminated));
    }

    void test_interrupted()
    {
        // skip this test if the test app PID is 0, as child PIDs will be same
        if (!QCoreApplication::applicationPid()) {
            QSKIP("This test requires a valid PID");
        }

        TestProcess test("AbnormalInterrupt.qml");
        test.run();
        QVERIFY(test.waitForStarted());
        // wait a bit
        QTest::qWait(500);

        // interrupt
        // FIXME: we must use platform specific termination signaling, this works on OSX and Linux
        ::kill(test.pid(), SIGINT);
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("runCount")), 1);
        QCOMPARE(test.intLog(QStringLiteral("lastSaveStatus")), int(BitQuick::Tools::StateSaver::Undefined));

        // re-launch
        test.run();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("runCount")), 2);
        QCOMPARE(test.intLog(QStringLiteral("lastSaveStatus")), int(BitQuick::Tools::StateSaver::Interrupted));
    }

    void test_save_only_when_quit()
    {
        TestProcess test("SaveWhenQuit.qml");
        test.run();
        QVERIFY(test.waitForStarted());
        QTest::qWait(500);

        test.terminate();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog("intProp"), 10);

        // rerun
        test.run();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog("intProp"), 10);

        // rerun again, this ttime the prop should be 20
        test.run();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog("intProp"), 20);

        // and once again, but kill (SIGINT)
        test.run();
        QVERIFY(test.waitForStarted());
        // wait a bit
        QTest::qWait(500);

        // interrupt
        // FIXME: we must use platform specific termination signaling, this works on OSX and Linux
        ::kill(test.pid(), SIGINT);
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 30);
    }

    void test_save_only_on_terminate()
    {
        TestProcess test("SaveOnTerm.qml");
        test.run();
        QVERIFY(test.waitForFinished());

        // reload but kill
        test.run();
        QVERIFY(test.waitForStarted());
        QTest::qWait(500);
        kill(test.pid(), SIGINT);
        test.waitForFinished();

        // reload
        test.run();
        test.waitForStarted();
        QTest::qWait(500);
        test.terminate();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 10);
    }

    void test_save_only_on_interrupted()
    {
        TestProcess test("SaveOnIntr.qml");
        test.run();
        QVERIFY(test.waitForFinished());

        // reload but terminate
        test.run();
        test.waitForStarted();
        QTest::qWait(500);
        test.terminate();
        test.waitForFinished();

        // reload
        test.run();
        QVERIFY(test.waitForStarted());
        QTest::qWait(500);
        kill(test.pid(), SIGINT);
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 10);
    }

    void test_save_on_quit_and_term()
    {
        TestProcess test("SaveOnQuitAndTerm.qml");
        test.run();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 10);

        // reload but terminate
        test.run();
        test.waitForStarted();
        QTest::qWait(500);
        test.terminate();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 20);

        // reload
        test.run();
        QVERIFY(test.waitForStarted());
        QTest::qWait(500);
        kill(test.pid(), SIGINT);
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 30);

        // load again to see if the value changed
        test.run();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 30);
    }

    void test_save_on_quit_and_interrupted()
    {
        TestProcess test("SaveOnQuitAndIntr.qml");
        test.run();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 10);

        // reload but kill
        test.run();
        QVERIFY(test.waitForStarted());
        QTest::qWait(500);
        kill(test.pid(), SIGINT);
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 20);

        // reload
        test.run();
        test.waitForStarted();
        QTest::qWait(500);
        test.terminate();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 30);

        // load again to see if the value changed
        test.run();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 30);
    }

    void test_save_on_term_and_interrupted()
    {
        TestProcess test("SaveOnTermAndIntr.qml");
        test.run();
        QVERIFY(test.waitForStarted());
        QTest::qWait(500);
        kill(test.pid(), SIGINT);
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 10);

        // reload
        test.run();
        test.waitForStarted();
        QTest::qWait(500);
        test.terminate();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 20);

        test.run();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 30);

        // load again to see if the value changed
        test.run();
        QVERIFY(test.waitForLogs());
        QCOMPARE(test.intLog(QStringLiteral("intProp")), 30);
    }
};

QTEST_MAIN(StateSaverTest)

#include "main.moc"
