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

#ifndef STATESAVERBACKEND_P_H
#define STATESAVERBACKEND_P_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QPair>
#include "statesaver_p.h"

class QQmlEngine;

namespace BitQuick { namespace Tools {

class StateSaverBackend : public QObject
{
    Q_OBJECT

public:
    typedef QPair<QVariant, QMetaType::Type> PropertyValue;
    typedef std::function<PropertyValue(const QString&)> ValueGetter;
    typedef std::function<void(const QString&, const QVariant&, int)> ValueSetter;

    explicit StateSaverBackend(QObject *parent = nullptr);
    virtual ~StateSaverBackend();

    virtual void saveProperties(const QStringList &properties, const QString &path, ValueGetter getter) = 0;
    virtual void restoreProperties(const QStringList &properties, const QString &path, ValueSetter setter) = 0;
    virtual void reset() = 0;
    inline StateSaver::SaveStatus lastSaveStatus()
    {
        loadStatus();
        return m_saveStatus;
    }

protected:
    StateSaver::SaveStatus m_saveStatus{StateSaver::SaveStatus::Undefined};

    virtual void saveStatus() {}
    virtual void loadStatus() {}

Q_SIGNALS:
    void saveAndExit(StateSaver::SaveStatus status = StateSaver::SaveStatus::Normal);

private:
    void onAboutToQuit();
    void onSignalTriggered(StateSaver::SaveStatus);
};

class AbnormalTerminationHandler : public QObject
{
    Q_OBJECT

public:
    explicit AbnormalTerminationHandler(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    virtual void watchTerminationSignal(StateSaver::SaveStatus reason) = 0;

Q_SIGNALS:
    void terminated(StateSaver::SaveStatus reason);
};

class StateSaverFactory
{
public:
    static StateSaverBackend *instance(QQmlEngine *engine);
    static AbnormalTerminationHandler *termination();
};

}} // namespace BitQuick::Tools

#endif // STATESAVERBACKEND_P_H
