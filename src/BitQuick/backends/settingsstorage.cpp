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

#include "settingsstorage_p.h"
#include <QtCore/QVariant>
#include <QtCore/QMetaType>
#include <QtCore/QDebug>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlInfo>

#include <QtCore/QVariantList>

namespace BitQuick { namespace Tools {

SettingsStorage::SettingsStorage(QObject *parent)
    : StateSaverBackend(parent)
    , storage(QStringLiteral("%1/%2.states")
              .arg(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation))
              .arg(QCoreApplication::applicationName()),
              QSettings::IniFormat)
{
    storage.setFallbacksEnabled(false);
    // get the application path
    qDebug() << "STORAGE=" << storage.fileName();

    // watch for application name changes to warn about misuse
    connect(QGuiApplication::instance(), &QGuiApplication::organizationNameChanged,
            this, &SettingsStorage::misuse);
    connect(QGuiApplication::instance(), &QGuiApplication::applicationNameChanged,
            this, &SettingsStorage::misuse);

    // TODO: watch forced application terminations
}

SettingsStorage::~SettingsStorage()
{
}


void SettingsStorage::saveProperties(QObject *object, const QStringList &properties, const QString &path, ValueGetter getter)
{
    Q_UNUSED(object);
    Q_UNUSED(properties);
    Q_UNUSED(path);

    // each property is saved under a group, where the group ID is the object path
    // each property will have a type pair which will contain teh tuype of the property saved
    storage.beginGroup(path);
    for (const QString &property : properties) {
        PropertyValue value = getter(property);
        if (!value.first.isValid()) {
            continue;
        }

        // is the value an array?
        storage.setValue(property, value.first);
        storage.setValue(property + QStringLiteral("_TYPE"), QVariant::fromValue(int(value.second)));
    }
    storage.endGroup();
    storage.sync();
}

void SettingsStorage::restoreProperties(QObject *object, const QStringList &properties, const QString &path, ValueSetter setter)
{
    Q_UNUSED(object);
    Q_UNUSED(properties);
    Q_UNUSED(path);

    storage.beginGroup(path);
    QStringList storedProperties = storage.childKeys();

    for (const QString &property : properties) {
        if (!storedProperties.contains(property)) {
            continue;
        }
        setter(property, storage.value(property), storage.value(property + QStringLiteral("_TYPE")).toInt());
    }
    storage.endGroup();
}

void SettingsStorage::reset()
{
    storage.clear();
    storage.sync();
    QFile::remove(storage.fileName());
}

StateSaver::SaveStatus SettingsStorage::SaveStatus()
{
    return StateSaver::SaveStatus::Undefined;
}

/******************************************************************************
 * private stuff
 */

void SettingsStorage::misuse()
{
    qWarning() << QStringLiteral("Warning: Application or organization name change detected after the state saving was initiated. This will cause previously saved states to be omitted!");
}

}} // namespace BitQuick::Tools

