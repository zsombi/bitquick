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

#include "propertysaver_p.h"
#include <QtCore/QVariant>
#include <QtCore/QMetaType>
#include <QtCore/QDebug>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlInfo>

#include <QtQml/private/qqmlproperty_p.h>

namespace BitQuick { namespace Tools {

PropertySaver::PropertySaver(QObject *parent)
    : QObject(parent)
{
    storage.setFallbacksEnabled(false);
    // get the application path
    qDebug() << "STORAGE=" << storage.fileName();
    connect(QGuiApplication::instance(), &QGuiApplication::aboutToQuit,
            this, std::bind(&PropertySaver::triggerSave, this, ExitReason::NormalShutdown));
    // TODO: connect activation and deactivation

    // watch for application name changes to warn about misuse
    connect(QGuiApplication::instance(), &QGuiApplication::organizationNameChanged,
            this, &PropertySaver::misuse);
    connect(QGuiApplication::instance(), &QGuiApplication::applicationNameChanged,
            this, &PropertySaver::misuse);

    // TODO: watch forced application terminations
}

PropertySaver::~PropertySaver()
{
}

// a system can be launched with multiple QML engines
// therefore each singleton backend must be linked to the QML engine!
PropertySaver *PropertySaver::instance(QQmlEngine *owner)
{
    Q_ASSERT(owner);
    // check if the owner has a PropertySaver child
    PropertySaver *inst = owner->findChild<PropertySaver*>();
    if (!inst) {
        inst = new PropertySaver(owner);
    }
    return inst;
}


void PropertySaver::savePropertiesState(QObject *object, const QStringList &properties, const QString &path)
{
    Q_UNUSED(object);
    Q_UNUSED(properties);
    Q_UNUSED(path);

    // each property is saved under a group, where teh group ID is the object path
    // each property will have a type pair which will contain teh tuype of the property saved
    storage.beginGroup(path);
    for (const QString &property : properties) {
        QQmlProperty qmlProperty(object, property, qmlContext(object));
        if (!qmlProperty.isValid()) {
            qmlWarning(object) << QStringLiteral("Warning: invalid property \"%1\"").arg(property);
            continue;
        }
        if (!qmlProperty.isWritable()) {
            qmlWarning(object) << QStringLiteral("Warning: cannot serialize read-only property \"%1\"").arg(property);
        }
        QVariant value = qmlProperty.read();
        QMetaType::Type type = static_cast<QMetaType::Type>(value.type());
        if (type == QMetaType::QObjectStar) {
            qmlWarning(object) << QStringLiteral("Warning: cannot save property \"%1\" of QObject type.").arg(property);
            continue;
        }
        storage.setValue(property, value);
        storage.setValue(property + QStringLiteral("_TYPE"), QVariant::fromValue(int(type)));
    }
    storage.endGroup();
    storage.sync();
}

void PropertySaver::restorePropertiesState(QObject *object, const QStringList &properties, const QString &path)
{
    Q_UNUSED(object);
    Q_UNUSED(properties);
    Q_UNUSED(path);

    storage.beginGroup(path);
    QStringList storedProperties = storage.childKeys();
    for (const QString &property : storedProperties) {
        if (!properties.contains(property)) {
            continue;
        }

        QQmlProperty qmlProperty(object, property, qmlContext(object));
        if (qmlProperty.isValid() && qmlProperty.isWritable()) {
            QVariant type = storage.value(property + QStringLiteral("_TYPE"));
            QVariant value = storage.value(property);
            if (!value.convert(type.toInt())) {
                qmlWarning(object) << QStringLiteral("Different type or type mismatch for property \"%1\"").arg(property);
                continue;
            }

            // write to the property without breaking the bindings!
            if (!QQmlPropertyPrivate::write(qmlProperty, value, QQmlPropertyData::DontRemoveBinding)) {
                qmlWarning(object) << QStringLiteral("Restoring property \"%1\" error").arg(property);
            }
        }
    }
    storage.endGroup();
}

void PropertySaver::reset()
{
    storage.clear();
    storage.sync();
    QFile::remove(storage.fileName());
}

/******************************************************************************
 * private stuff
 */

void PropertySaver::misuse()
{
    qWarning() << QStringLiteral("Warning: Application or organization name change detected after the state saving was initiated. This will cause previously saved states to be omitted!");
}
void PropertySaver::triggerSave(ExitReason reason)
{
    Q_UNUSED(reason);
    qDebug() << "EXITING, reason" << reason;
}

}} // namespace BitQuick::Tools

