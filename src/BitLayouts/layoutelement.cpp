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

#include "layoutelement_p_p.h"

namespace BitQuick {

/*!
 * \qmltype LayoutElement
 * \ingroup bitquick_layouts
 * \inqmlmodule BitQuick.Layouts 1.0
 * \brief Defines the properties of a laid out element in a given layout.
 */
LayoutElement::LayoutElement(QObject *parent)
    : QObject(*(new LayoutElementPrivate), parent)
{
}

LayoutElementAttached* LayoutElement::qmlAttachedProperties(QObject *owner)
{
    return new LayoutElementAttached(owner);
}

/*!
 * \qmlproperty string LayoutElement::identifier
 * Holds the string identifier of the laid out element.
 */
QString LayoutElement::identifier() const
{
    Q_D(const LayoutElement);
    return d->id;
}
void LayoutElement::setIdentifier(const QString& id)
{
    Q_D(LayoutElement);
    if (d->id == id) {
        return;
    }
    d->id = id;
    Q_EMIT identifierChanged();
}

/******************************************************************************
 *
 */
LayoutElementAttached::LayoutElementAttached(QObject *parent)
    : QObject(*(new LayoutElementAttachedPrivate), parent)
{
}

/*!
 * \qmlattachedproperty string LayoutElement::identifier
 * Holds the identifier of the element to be laid out. The elements the property
 * is attached to must reside in a LayoutElement.
 */
QString LayoutElementAttached::identifier() const
{
    Q_D(const LayoutElementAttached);
    return d->id;
}
void LayoutElementAttached::setIdentifier(const QString& id)
{
    Q_D(LayoutElementAttached);
    if (d->id == id) {
        return;
    }
    d->id = id;
    Q_EMIT identifierChanged();
}

} // namespace BitQuick
