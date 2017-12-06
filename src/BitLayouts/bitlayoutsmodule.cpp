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

#include <QtQml/qqml.h>
#include "bitlayoutsmodule.h"
#include "dynamiclayout_p.h"
#include "layoutcontainer_p.h"

namespace BitQuick {

BitLayoutsModule::BitLayoutsModule()
{
}

void BitLayoutsModule::defineModule(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(engine);
    Q_UNUSED(uri);
}

void BitLayoutsModule::registerTypes(const char *uri)
{
    Q_UNUSED(uri);
    qmlRegisterType<DynamicLayout>(uri, 1, 0, "DynamicLayout");
//    qmlRegisterType<DynamicLayoutAttached>();
    qmlRegisterType<LayoutContainer>(uri, 1, 0, "LayoutContainer");
}

void BitLayoutsModule::undefineModule()
{
}

} // namespace BitQuick
