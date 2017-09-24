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

#ifndef STATESAVER_P_P_H
#define STATESAVER_P_P_H

#include "statesaver_p.h"
#include <QtCore/private/qobject_p.h>

namespace BitQuick { namespace Tools {

class StateSaverAttachedPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(StateSaverAttached)

public:
    StateSaverAttachedPrivate();

    bool isEnabled() const;
    void setEnabled(bool enabled);
    QString properties() const;
    void setProperties(const QString &properties);

    // functions
    void init();
    bool buildUUId();
    void onCompleted();
    void save();
    void restore();
    void toggleAutoSave();

    // members
    QStringList propertyList;
    QString uuid;
    QMetaObject::Connection *autoSaveConnection{nullptr};
    bool enabled:1;
    bool ready:1;
};

}} // namespace BitQuick::Tools

#endif // STATESAVER_P_P_H
