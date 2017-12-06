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

#include "layoutcontainer_p_p.h"
#include "dynamiclayout_p_p.h"
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlInfo>

namespace BitQuick {

LayoutContainer::LayoutContainer(QObject *parent)
    : QObject(*(new LayoutContainerPrivate), parent)
{
}

void LayoutContainer::classBegin()
{
}
void LayoutContainer::componentComplete()
{
    d_func()->sealed = true;
}

bool LayoutContainer::isDefault() const
{
    Q_D(const LayoutContainer);
    return d->isDefault;
}
void LayoutContainer::setDefault(bool value)
{
    Q_D(LayoutContainer);
    if (d->isDefault == value) {
        return;
    }
    if (d->sealed) {
        qmlWarning(this) << "Default status cannot be changed once completed.";
    }
    d->isDefault = value;

    if (d->layout) {
        // inform layout about the default status
        DynamicLayoutPrivate::get(d->layout)->setDefault(this);
    }
}

bool LayoutContainer::when() const
{
    Q_D(const LayoutContainer);
    return d->when;
}
void LayoutContainer::setWhen(bool when)
{
    Q_D(LayoutContainer);
    if (d->when == when) {
        return;
    }
    d->when = when;
    Q_EMIT whenChanged();

    if (d->layout) {
        DynamicLayoutPrivate::get(d->layout)->activateContainer(this);
    }
}

QQmlComponent* LayoutContainer::layout() const
{
    Q_D(const LayoutContainer);
    return d->content;
}
void LayoutContainer::setLayout(QQmlComponent* layout)
{
    Q_D(LayoutContainer);
    if (d->content == layout) {
        return;
    }
    // The layout content cannot be changed while activated.
    if (d->layout && d->layout->currentLayout() == this) {
        qmlWarning(this) << QStringLiteral("Cannot change layout content while active.");
        return;
    }
    d->content = layout;
    Q_EMIT layoutChanged();
}

} // namespace BitQuick
