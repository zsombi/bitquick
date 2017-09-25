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

class QQmlEngine;

namespace BitQuick { namespace Tools {

class PropertySaver : public QObject
{
    Q_OBJECT

private:
    PropertySaver(QObject *owner = nullptr);
    ~PropertySaver();

public:
    static PropertySaver *instance(QQmlEngine *owner);

    static QString makeUuid(QObject *object, bool fullPath = false);

    bool registerUuid(const QString &uuid);
    void removeUuid(const QString &uuid);
    void savePropertiesState(QObject *object, const QStringList &properties, const QString &path);
    void restorePropertiesState(QObject *object, const QStringList &properties, const QString &path);

Q_SIGNALS:
    void saveAndExit();
};

}} // namespace BitQuick::Tools

#endif // PROPERTYSAVER_P_H
