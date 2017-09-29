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

#ifndef STATESAVER_H
#define STATESAVER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtQml/QtQml>
#include <BitQuick/BitQuickGlobals>

namespace BitQuick { namespace Tools {

class StateSaverAttachedPrivate;
class BITQUICK_EXPORT StateSaverAttached : public QObject
{
    Q_OBJECT
    Q_PRIVATE_PROPERTY(d_func(), bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PRIVATE_PROPERTY(d_func(), QString properties READ properties WRITE setProperties NOTIFY propertiesChanged)

public:
    explicit StateSaverAttached(QObject *parent = nullptr);
    ~StateSaverAttached();

Q_SIGNALS:
    void enabledChanged();
    void propertiesChanged();

private:
    Q_DECLARE_PRIVATE(StateSaverAttached)
};

class BITQUICK_EXPORT StateSaver : public QObject
{
    Q_OBJECT

public:
    StateSaver(QObject *parent = nullptr);
    static StateSaverAttached *qmlAttachedProperties(QObject *owner)
    {
        return new StateSaverAttached(owner);
    }

public Q_SLOTS:
    void reset();
};

}} // namespace BitQuick::Tools

QML_DECLARE_TYPEINFO(BitQuick::Tools::StateSaver, QML_HAS_ATTACHED_PROPERTIES)

#endif // STATESAVER_H
