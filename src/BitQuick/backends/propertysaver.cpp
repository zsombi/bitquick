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

QString PropertySaver::makeUuid(QObject *object, bool fullPath)
{
    QQmlContext *context = qmlContext(object);
    if (!context) {
        qWarning() << QStringLiteral("Warning: cannot save properties of a context-less object");
        return QString();
    }

    QString id = context->nameForObject(object);
    if (id.isEmpty()) {
        return QString();
    }

    QQmlContextData *cdata = QQmlContextData::get(context);
    QUrl url(cdata->url());
    QQmlData *ddata = QQmlData::get(object, false);

    // Use the component's full document path, the line and the column number in junction with the ID
    // We need all these, as the component id is only unique within the document used
    // and even the QML document can be the same in two different folders
    // In case we don't need the full path (i.e. the object is a parent of the
    // targeted attachee) we use the className
    QString uuid = fullPath
            ? url.path().replace(QLatin1Char('/'), QLatin1Char('_'))
            : className(object);

    uuid += QLatin1Char(':')
            + QString::number(ddata->lineNumber) + QLatin1Char(':')
            + QString::number(ddata->columnNumber) + QLatin1Char(':') + id;

    // The component may be a delegate in a view. Therefore we need to take the "index"
    // context property into account
    QVariant indexValue = context->contextProperty(QStringLiteral("index"));
    if (indexValue.isValid() && (indexValue.type() == QVariant::Int)) {
        uuid += indexValue.toString();
    }

    return uuid;
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

