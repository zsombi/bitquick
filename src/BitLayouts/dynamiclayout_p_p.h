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

#ifndef DYNAMICLAYOUT_P_P_H
#define DYNAMICLAYOUT_P_P_H

#include "dynamiclayout_p.h"
#include <QtQuick/private/qquickitem_p.h>

namespace BitQuick {

class DynamicLayoutPrivate : public QQuickItemPrivate
{
    Q_DECLARE_PUBLIC(DynamicLayout)

public:
    explicit DynamicLayoutPrivate();
    static DynamicLayoutPrivate *get(DynamicLayout *layout)
    {
        return layout->d_func();
    }
    static void layoutAppend(QQmlListProperty<LayoutContainer> *list, LayoutContainer* container);

    void activateContainer(LayoutContainer* container);
    void setDefault(LayoutContainer *container);

    QList<LayoutContainer*> layouts;
    int currentContainer = -1;
    int defaultContainer = -1;

    QQuickItem *currentLayout = nullptr;
};

} // namespace BitQuick

#endif // DYNAMICLAYOUT_P_P_H
