/*
 * BitQuick - a QtQuick utility library
 *
 * Copyright (C) 2017, bitWelder Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
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
 * <http://www.gnu.org/licenses/>.requires(qtHaveModule(quick))
 *
 * Author: Zsombor Egri <zsombor.egri@bitwelder.fi>
 */

#include "bitquicktest.h"
#include <QtCore/QList>
#include <QtQuick/private/qquickitem_p.h>
#include <QtQml/QQmlListProperty>

BitQuickTest::BitQuickTest(QObject *parent)
    : QObject(parent)
{
}

/*!
 * \brief BitQuickTest::findChild
 * Look after a child of a given \a object with an \a objectName. If the object is a QQuickItem,
 * it will look in between the child items first, and if none found, will continue to look for
 * the child in between the data. If the object is a QObject, it will look in between the children
 * of the object. Returns the child if found, nullptr if not.
 */
QObject *BitQuickTest::findChild(QObject *object, const QString &objectName)
{
    if (!object) {
        return nullptr;
    }

    QObject *result = nullptr;
    QQuickItem *item = qobject_cast<QQuickItem*>(object);

    if (item) {
        for (QQuickItem *child: item->childItems()) {
            if (child->objectName() == objectName) {
                result = static_cast<QObject*>(child);
                break;
            }
        }
        if (!result) {
            for (QQuickItem *child: item->childItems()) {
                result = findChild(static_cast<QObject*>(child), objectName);
                if (result) {
                    break;
                }
            }
        }
        if (!result) {
            // lookup in data
            QQmlListProperty<QObject> data(QQuickItemPrivate::get(item)->data());
            for (int i = 0; !result && i < QQuickItemPrivate::data_count(&data); i++) {
                result = findChild(QQuickItemPrivate::data_at(&data, i), objectName);
            }
        }
    } else {
        result = object->findChild<QObject*>(objectName, Qt::FindChildrenRecursively);
    }
    return result;
}
