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

#ifndef LAYOUTELEMENT_P_H
#define LAYOUTELEMENT_P_H

#include <QtCore/QObject>
#include <QtQml/qqml.h>

namespace BitQuick {

class LayoutElementPrivate;
class LayoutElementAttached;
class LayoutElement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)

public:
    explicit LayoutElement(QObject *parent = nullptr);

    static LayoutElementAttached* qmlAttachedProperties(QObject *owner);

    QString identifier() const;
    void setIdentifier(const QString& id);

Q_SIGNALS:
    void identifierChanged();

private:
    Q_DECLARE_PRIVATE(LayoutElement)
};

class LayoutElementAttachedPrivate;
class LayoutElementAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)

public:
    explicit LayoutElementAttached(QObject *parent = nullptr);

    QString identifier() const;
    void setIdentifier(const QString& id);

Q_SIGNALS:
    void identifierChanged();

private:
    Q_DECLARE_PRIVATE(LayoutElementAttached)
};

} // namespace BitQuick

QML_DECLARE_TYPEINFO(BitQuick::LayoutElement, QML_HAS_ATTACHED_PROPERTIES)

#endif // LAYOUTELEMENT_P_H
