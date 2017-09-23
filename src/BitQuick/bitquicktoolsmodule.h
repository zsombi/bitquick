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

#ifndef BITQUICKMODULE_H
#define BITQUICKMODULE_H

#include <BitQuick/BitQuickGlobals>

class QQmlEngine;
namespace BitQuick {

class BITQUICK_EXPORT BitQuickToolsModule
{
private:
    BitQuickToolsModule();

public:
    static void defineModule(QQmlEngine *engine, const char *uri);
    static void registerTypes(const char *uri);
    static void undefineModule();

};

} // namespace BitQuick

#endif // BITQUICKMODULE_H
