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

#ifndef BITTEST_H
#define BITTEST_H

#include <QtCore/QObject>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <BitQuick/BitQuickGlobals>

class QWindow;
class QSignalSpy;

namespace BitQuick { namespace Test {

class BITQUICK_EXPORT BitTest : public QQuickView
{
    Q_OBJECT
public:
    BitTest(const QString& file, ResizeMode resize = SizeViewToRootObject, bool assertOnFailure = true, QWindow* parent = nullptr);
    int warnings() const;

    template<class T>
    inline T findItem(const QString& objectName) const {
        T item = rootObject()->findChild<T>(objectName);
        if (item)
            return item;
        if (rootObject()->findChild<QObject*>(objectName))
            qFatal("Item '%s' found with unexpected type", qPrintable(objectName));
        qFatal("No item '%s' found", qPrintable(objectName));
    }

    static void ignoreWarning(const QString& fileName, uint line, uint column, const QString& message, uint occurences = 1);
    static void waitForSignal(QSignalSpy *spy, int timeout = 5000);

private:
    QSignalSpy* m_spy;
};

}} // namespace BitQuick::Test

#endif // BITTEST_H
