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

#ifndef LAYOUTCONTAINER_P_P_H
#define LAYOUTCONTAINER_P_P_H

#include <QtCore/private/qobject_p.h>
#include "layoutcontainer_p.h"

namespace BitQuick {

class DynamicLayout;
class LayoutContainerPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(LayoutContainer)

public:
    LayoutContainerPrivate() :
        QObjectPrivate()
    {
    }
    static LayoutContainerPrivate* get(LayoutContainer *container)
    {
        return container->d_func();
    }

    DynamicLayout* layout = nullptr;
    QQmlComponent* content = nullptr;
    int containerIndex = -1;
    bool sealed = false;
    bool isDefault = false;
    bool when = false;
};

} // namespace BitQuick

#endif // LAYOUTCONTAINER_P_P_H
