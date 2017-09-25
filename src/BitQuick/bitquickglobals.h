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

#ifndef BITQUICKGLOBALS_H
#define BITQUICKGLOBALS_H

#include <QtCore/QtGlobal>

#if defined(BITQUICK_LIBRARY)
#  define BITQUICK_EXPORT Q_DECL_EXPORT
#else
#  define BITQUICK_EXPORT Q_DECL_IMPORT
#endif

/*
 * Some Qt version-specific hacks
 */
#if (QT_VERSION < QT_VERSION_CHECK(5, 9, 0))
    #include <QtQml/QQmlInfo>
    #define qmlWarning(object)      qmlInfo(object)
#endif

#endif // BITQUICKGLOBALS_H
