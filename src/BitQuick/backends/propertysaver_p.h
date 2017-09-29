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

#ifndef PROPERTYSAVER_P_H
#define PROPERTYSAVER_P_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtQuick/QQuickItem>
#include <QtCore/QSettings>

class QQmlEngine;

namespace BitQuick { namespace Tools {

class PropertySaver : public QObject
{
    Q_OBJECT

private:
    PropertySaver(QObject *owner = nullptr);
    ~PropertySaver();

public:
    enum ExitReason {
        NormalShutdown,
        Deactivated,
        Interrupted,
        Terminated
    };
    static PropertySaver *instance(QQmlEngine *owner);

    void savePropertiesState(QObject *object, const QStringList &properties, const QString &path);
    void restorePropertiesState(QObject *object, const QStringList &properties, const QString &path);
    void reset();

Q_SIGNALS:
    void saveAndExit();

private Q_SLOTS:
    void misuse();
    void triggerSave(ExitReason reason);

private:
    QSettings storage;
};

}} // namespace BitQuick::Tools

#endif // PROPERTYSAVER_P_H
