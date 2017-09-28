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
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlInfo>
#include <QtCore/QDebug>
#include <QtQml/private/qqmlcomponentattached_p.h>
#include <QtQml/private/qqmlcontext_p.h>
#include <QtQml/private/qqmldata_p.h>

namespace BitQuick { namespace Tools {

PropertySaver::PropertySaver(QObject *parent)
    : QObject(parent)
{
    // get the application path
    qDebug() << "STORAGE=" << states.fileName();
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

static QString className(QObject *object)
{
    QString result = QString::fromLatin1(object->metaObject()->className());
    return result.left(result.indexOf(QStringLiteral("_QML")));
}

QString PropertySaver::path(QObject *object)
{
    QQmlContext *context = qmlContext(object);
    Q_ASSERT(context);

    QQmlContextData *cdata = QQmlContextData::get(context);
    return cdata->url().toString(QUrl::NormalizePathSegments | QUrl::PreferLocalFile);
}

// the uuid is of id(className, line, column)[index] format
QString PropertySaver::makeUuid(QObject *object)
{
    QQmlContext *context = qmlContext(object);
    Q_ASSERT(context);

    QString uuid = context->nameForObject(object);
    if (uuid.isEmpty()) {
        return QString();
    }
    QQmlData *ddata = QQmlData::get(object, false);
    uuid += QStringLiteral("(%1,%2,%3)").arg(className(object)).arg(ddata->lineNumber).arg(ddata->columnNumber);

    // The component may be a delegate in a view. Therefore we need to take the "index"
    // context property into account
    QVariant indexValue = context->contextProperty(QStringLiteral("index"));
    if (indexValue.isValid() && (indexValue.type() == QVariant::Int)) {
        uuid += QStringLiteral("[%1]").arg(indexValue.toString());
    }

    return uuid.prepend(QLatin1Char('/'));
}


bool PropertySaver::registerUuid(const QString &uuid)
{
    Q_UNUSED(uuid);
    return false;
}

void PropertySaver::removeUuid(const QString &uuid)
{
    Q_UNUSED(uuid);
}

void PropertySaver::savePropertiesState(QObject *object, const QStringList &properties, const QString &path)
{
    Q_UNUSED(object);
    Q_UNUSED(properties);
    Q_UNUSED(path);
}

void PropertySaver::restorePropertiesState(QObject *object, const QStringList &properties, const QString &path)
{
    Q_UNUSED(object);
    Q_UNUSED(properties);
    Q_UNUSED(path);
}

}} // namespace BitQuick::Tools

