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

#ifndef SettingsStorage_P_H
#define SettingsStorage_P_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtQuick/QQuickItem>
#include <QtCore/QSettings>
#include "statesaverbackend_p.h"

class QQmlEngine;

namespace BitQuick { namespace Tools {

class SettingsStorage : public StateSaverBackend
{
    Q_OBJECT

public:
    SettingsStorage(QObject *owner = nullptr);
    ~SettingsStorage();

    void saveProperties(QObject *object, const QStringList &properties, const QString &path, ValueGetter getter) override;
    void restoreProperties(QObject *object, const QStringList &properties, const QString &path, ValueSetter setter) override;
    void reset() override;
    StateSaver::SaveStatus SaveStatus() override;

private Q_SLOTS:
    void misuse();

private:
    QSettings storage;
};

}} // namespace BitQuick::Tools

#endif // SettingsStorage_P_H