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
#include "backends/statesaverbackend_p.h"

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlInfo>
#include <QtQml/QQmlProperty>
#include <QtQml/private/qqmlcomponentattached_p.h>
#include <QtQml/private/qqmlcontext_p.h>
#include <QtQml/private/qqmldata_p.h>

#include <QtQml/private/qqmlproperty_p.h>

namespace BitQuick { namespace Tools {

/*!
 * \qmltype StateSaver
 * \ingroup bitquick_tools
 * \inqmlmodule BitQuick.Tools 1.0
 * \brief Saves the values of the attachee object properties
 *
 * TBD
 * \note Application name and organization must be set before the first state saving happens.
 */
StateSaver::StateSaver(QObject *parent)
    : QObject(parent)
{
}

/*!
 * \qmlproperty string StateSaver::applicationName
 * The property holds the application name. The application name should either be
 * set in c++ code or as a simple value in QML as it affects the place the storage
 * file will be stored, which is highly dependent on the application and organization
 * name. Any further changes on teh application name will be omitted, and an error
 * will be displayed.
 */
QString StateSaver::applicationName() const
{
    return QCoreApplication::applicationName();
}
void StateSaver::setApplicationName(const QString &name)
{
    QCoreApplication::setApplicationName(name);
    Q_EMIT applicationNameChanged();
    // make sure we create the statesaver backend instance!
    StateSaverFactory::instance(qmlEngine(this));
    qDebug() << "APPNAME=" << name;
}

/*!
 * \qmlproperty enumeration StateSaver::lastSaveStatus
 * \readonly
 * The property holds the application's last saved saving status, which can be one
 * of the following:
 * \list
 * \li Undefined
 * \li Normal
 * \li Deactivated
 * \li Interrupted
 * \li Terminated
 * \endlist
 */
StateSaver::SaveStatus StateSaver::lastSaveStatus() const
{
    return StateSaverFactory::instance(qmlEngine(this))->SaveStatus();
}

/*!
 * \qmlmethod void StateSaver::reset()
 * The method resets the state saver storage cleaning the saved property statuses
 * and deleting the storage file.
 */
void StateSaver::reset()
{
    StateSaverFactory::instance(qmlEngine(this))->reset();
}

StateSaverAttachedPrivate::StateSaverAttachedPrivate()
    : enabled(false)
    , ready(false)
{
}

StateSaverAttachedPrivate::~StateSaverAttachedPrivate()
{
    // clean the connections, if in an unlikely situation we reach here
    if (completedConnection) {
        QObject::disconnect(*completedConnection);
        delete completedConnection;
        completedConnection = nullptr;
    }
    if (destroyedConnection) {
        QObject::disconnect(*destroyedConnection);
        delete destroyedConnection;
        destroyedConnection = nullptr;
    }
}

void StateSaverAttachedPrivate::init()
{
    // make sure the attachee (parent) is linked to a qml context
    Q_ASSERT(qmlContext(parent));

    // make sure the current QML engine has a StateSaver backend
    backend = StateSaverFactory::instance(qmlEngine(parent));

    // watch for component completion
    QQmlComponentAttached *attached = QQmlComponent::qmlAttachedProperties(parent);
    completedConnection = new QMetaObject::Connection;
    *completedConnection = QObject::connect(attached, &QQmlComponentAttached::completed,
                                            std::bind(&StateSaverAttachedPrivate::onCompleted, this));

    // also for component destruction, as at that phase we may still have the QML context available
    destroyedConnection = new QMetaObject::Connection;
    *destroyedConnection = QObject::connect(attached, &QQmlComponentAttached::destruction,
                                            std::bind(&StateSaverAttachedPrivate::onDestroyed, this));

    // finally enable it
    setEnabled(true);
}

static QString className(QObject *object)
{
    QString result = QString::fromLatin1(object->metaObject()->className());
    return result.left(result.indexOf(QStringLiteral("_QML")));
}

static QString objectPath(QObject *object)
{
    QQmlContext *context = qmlContext(object);
    Q_ASSERT(context);

    QQmlContextData *cdata = QQmlContextData::get(context);
    return cdata->url().toString(QUrl::NormalizePathSegments | QUrl::PreferLocalFile);
}

// the uuid is of id(className, line, column)[index] format
static QString makeUuid(QObject *object)
{
    QQmlContext *context = qmlContext(object);
    Q_ASSERT(context);

    QString uuid = context->nameForObject(object);
    if (uuid.isEmpty()) {
        return QString();
    }
    QQmlData *ddata = QQmlData::get(object, false);
    uuid += QStringLiteral("(%1:%2:%3)").arg(className(object)).arg(ddata->lineNumber).arg(ddata->columnNumber);

    // The component may be a delegate in a view. Therefore we need to take the "index"
    // context property into account
    QVariant indexValue = context->contextProperty(QStringLiteral("index"));
    if (indexValue.isValid() && (indexValue.type() == QVariant::Int)) {
        uuid += QStringLiteral("[%1]").arg(indexValue.toString());
    }

    return uuid.prepend(QLatin1Char('_'));
}
bool StateSaverAttachedPrivate::buildUUId()
{
    QQmlContext *context = qmlContext(parent);
    if (!context) {
        qWarning() << QStringLiteral("Warning: cannot save properties of a context-less object");
        return false;
    }

    // Use the component's full document path, the line and the column number in junction with the ID
    // We need all these, as the component id is only unique within the document used
    // and even the QML document can be the same in two different folders
    // In case we don't need the full path (i.e. the object is a parent of the
    // targeted attachee) we use the className
    QString path(objectPath(parent));
    if (path.isEmpty()) {
        qmlWarning(parent) << QStringLiteral("Error retrienving object path");
        return false;
    }
    // replace separators with underscore
    path.replace(QDir::separator(), QLatin1Char('_'));

    // the first uuid has the full path of the object
    uuid = makeUuid(parent);
    if (uuid.isEmpty()) {
        qmlWarning(parent) << QStringLiteral("Warning: attachee must have an ID in order to save property states.");
        return false;
    }
    // follow the parents
    for (QObject *pl = parent->parent(); pl; pl = pl->parent()) {
        QString plid = makeUuid(pl);
        if (plid.isEmpty()) {
            qmlWarning(pl) << QStringLiteral("Warning: all parents must have an id defined to save the state.");
            uuid.clear();
            return false;
        }
        uuid += plid;
    }

    uuid = uuid.prepend(QLatin1Char(':')).prepend(path);

    return true;
}

void StateSaverAttachedPrivate::onCompleted()
{
    ready = true;
    // we need to remember the QML component ID path
    if (!buildUUId()) {
        // path error already displayed
        setEnabled(false);
    } else {
        // and then restore the property values
        restore();
    }

    // finally clean connection; no need to listen on completion anymore
    QObject::disconnect(*completedConnection);
    delete completedConnection;
    completedConnection = nullptr;
}

void StateSaverAttachedPrivate::onDestroyed()
{
    save();
    ready = false;

    // clean connection
    QObject::disconnect(*destroyedConnection);
    delete destroyedConnection;
    destroyedConnection = nullptr;
}

void StateSaverAttachedPrivate::save()
{
    if (ready && enabled && !propertyList.isEmpty() && !uuid.isEmpty() && backend) {

        auto getter = [this](const QString &property) -> QPair<QVariant, QMetaType::Type> {
            QQmlProperty qmlProperty(parent, property, qmlContext(parent));
            if (!qmlProperty.isValid()) {
                qmlWarning(parent) << QStringLiteral("Warning: invalid property \"%1\"").arg(property);
                return StateSaverBackend::PropertyValue();
            }
            if (!qmlProperty.isWritable()) {
                qmlWarning(parent) << QStringLiteral("Warning: cannot serialize read-only property \"%1\"").arg(property);
                return StateSaverBackend::PropertyValue();
            }
            QVariant value = qmlProperty.read();
            QMetaType::Type type = static_cast<QMetaType::Type>(value.type());
            switch (type) {
            case QMetaType::QObjectStar:
                qmlWarning(parent) << QStringLiteral("Warning: cannot save property \"%1\" of QObject type.").arg(property);
                return StateSaverBackend::PropertyValue();
            case QMetaType::User:
            {
                // lists declared as variants are user typed, those must be converted to variant arrays
                QVariantList list = value.toList();
                if (!list.isEmpty()) {
                    type = QMetaType::QVariantList;
                }
                break;
            }
            default:
                break;
            }
            return StateSaverBackend::PropertyValue(value, type);
        };

        backend->saveProperties(parent, propertyList, uuid, getter);
    }
}

void StateSaverAttachedPrivate::restore()
{
    if (ready && enabled && !propertyList.isEmpty() && !uuid.isEmpty() && backend) {
        auto setter = [this](const QString &property, const QVariant &value, int type) {
            QQmlProperty qmlProperty(parent, property, qmlContext(parent));
            if (qmlProperty.isValid() && qmlProperty.isWritable()) {
                if (!value.canConvert(type)) {
                    qmlWarning(parent) << QStringLiteral("Different type or type mismatch for property \"%1\": %2")
                                          .arg(property)
                                          .arg(QLatin1String(QVariant::typeToName(type)));
                    return;
                }

                // write to the property without breaking the bindings!
                if (!QQmlPropertyPrivate::write(qmlProperty, value, QQmlPropertyData::DontRemoveBinding)) {
                    qmlWarning(parent) << QStringLiteral("Restoring property \"%1\" error").arg(property);
                }
            }
        };
        backend->restoreProperties(parent, propertyList, uuid, setter);
    }
}

void StateSaverAttachedPrivate::toggleAutoSave()
{
    if (enabled) {
        Q_ASSERT(!autoSaveConnection);
        autoSaveConnection = new QMetaObject::Connection;
        auto autoSaveSlot = [this](StateSaver::SaveStatus) {
            save();
            QObject::disconnect(*autoSaveConnection);
            delete autoSaveConnection;
            autoSaveConnection = nullptr;
        };
        *autoSaveConnection = QObject::connect(backend, &StateSaverBackend::saveAndExit,
                                               autoSaveSlot);
    } else if (autoSaveConnection) {
        QObject::disconnect(*autoSaveConnection);
        delete autoSaveConnection;
        autoSaveConnection = nullptr;
    }
}

/******************************************************************************
 * Atached properties
 */
StateSaverAttached::StateSaverAttached(QObject *parent)
    : QObject(*(new StateSaverAttachedPrivate), parent)
{
    d_func()->init();
}

StateSaverAttached::~StateSaverAttached()
{
}

/*!
 * \qmlattachedproperty bool StateSaver::enabled
 * Indicates whether saving the properties of the attachee listed in \l properties
 * should be made or not. Developers can programatically disable the state saving
 * to skip certain property state savings when not needed.
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
 * \qmlattachedproperty string StateSaver::properties
 * The property holds the attachee properties which should be serialized. Properties
 * are listed with commas, and their type can be of any basic type. It supports
 * attached properties and group properties.
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
