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

#include "statesaver_p_p.h"
#include "backends/propertysaver_p.h"

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlInfo>
#include <QtQml/QQmlProperty>
#include <QtQml/private/qqmlcomponentattached_p.h>
#include <QtQml/private/qqmlcontext_p.h>
#include <QtQml/private/qqmldata_p.h>

namespace BitQuick { namespace Tools {

StateSaverAttachedPrivate::StateSaverAttachedPrivate()
    : enabled(false)
    , ready(false)
{
}

void StateSaverAttachedPrivate::init()
{
    // make sure the attachee (parent) is linked to a qml context
    Q_ASSERT(qmlContext(parent));

    // make sure the current QML engine has a PropertySaver
    backend = PropertySaver::instance(qmlEngine(parent));

    // watch for component completion
    QMetaObject::Connection *connection = new QMetaObject::Connection;
    auto completed = [connection, this]() {
        ready = true;

        // initialize path and properties
        onCompleted();

        // and disconnect; no need to listen on completion anymore
        QObject::disconnect(*connection);
        delete connection;
    };
    QQmlComponentAttached *attached = QQmlComponent::qmlAttachedProperties(parent);
    *connection = QObject::connect(attached, &QQmlComponentAttached::completed,
                                   completed);

    // enable it
    setEnabled(true);
}

bool StateSaverAttachedPrivate::buildUUId()
{
    // the first uuid has the full path of the object
    uuid = PropertySaver::makeUuid(parent, true);
    if (uuid.isEmpty()) {
        qmlWarning(parent) << QStringLiteral("Warning: attachee must have an ID in order to save property states.");
        return false;
    }
    // follow the parents
    for (QObject *pl = parent->parent(); pl; pl = pl->parent()) {
        QString plid = PropertySaver::makeUuid(pl);
        if (plid.isEmpty()) {
            qmlWarning(pl) << QStringLiteral("Warning: all parents must have an id defined to save the state.");
            uuid.clear();
            return false;
        }
        uuid += plid;
    }

    return true;
}

void StateSaverAttachedPrivate::onCompleted()
{
    // we need to remember the QML component ID path
    if (!buildUUId()) {
        // path error already displayed
        setEnabled(false);
    } else if (!backend->registerUuid(uuid)) {
        qmlWarning(parent) << QStringLiteral("Warning: uuid \"%1\" already registered").arg(uuid);
        setEnabled(false);
    } else {
        restore();
    }
}

void StateSaverAttachedPrivate::save()
{
    if (ready && enabled && !propertyList.isEmpty() && !uuid.isEmpty()) {
        backend->savePropertiesState(parent, propertyList, uuid);
    }
}

void StateSaverAttachedPrivate::restore()
{
    if (ready && enabled && !propertyList.isEmpty() && !uuid.isEmpty()) {
        backend->restorePropertiesState(parent, propertyList, uuid);
    }
}

void StateSaverAttachedPrivate::toggleAutoSave()
{
    if (enabled) {
        Q_ASSERT(!autoSaveConnection);
        autoSaveConnection = new QMetaObject::Connection;
        auto autoSaveSlot = [this]() {
            save();
            QObject::disconnect(*autoSaveConnection);
            delete autoSaveConnection;
            autoSaveConnection = nullptr;
        };
        *autoSaveConnection = QObject::connect(backend, &PropertySaver::saveAndExit,
                                               autoSaveSlot);
    } else if (autoSaveConnection) {
        QObject::disconnect(*autoSaveConnection);
        delete autoSaveConnection;
        autoSaveConnection = nullptr;
    }
}

/*!
 * \qmltype StateSaver
 * \ingroup bitquick_tools
 * \inqmlmodule BitQuick.Tools 1.0
 * \brief Saves the values of the attachee object properties
 */
StateSaverAttached::StateSaverAttached(QObject *parent)
    : QObject(*(new StateSaverAttachedPrivate), parent)
{
    d_func()->init();
}

StateSaverAttached::~StateSaverAttached()
{
    // remove uuid from the property saver
    Q_D(StateSaverAttached);
    d->backend->removeUuid(d->uuid);
}

/*!
 * \qmlproperty bool StateSaver::enabled
 */
bool StateSaverAttachedPrivate::isEnabled() const
{
    return enabled;
}
void StateSaverAttachedPrivate::setEnabled(bool enabled)
{
    if (this->enabled == enabled) {
        return;
    }
    this->enabled = enabled;
    toggleAutoSave();
    Q_EMIT q_func()->enabledChanged();
}

/*!
 * \qmlproperty string StateSaver::properties
 */
QString StateSaverAttachedPrivate::properties() const
{
    return propertyList.join(QLatin1Char(','));
}
void StateSaverAttachedPrivate::setProperties(const QString &properties)
{
    QStringList list = properties.split(QLatin1Char(','), QString::SkipEmptyParts);
    for (QString &prop : list) {
        prop = prop.trimmed();
    }
    if (propertyList == list) {
        return;
    }
    propertyList = list;
    Q_EMIT q_func()->propertiesChanged();
    // restore property values if there were any saved
    restore();
}

}} // namespace BitQuick::Tools

#include "moc_statesaver_p.cpp"
