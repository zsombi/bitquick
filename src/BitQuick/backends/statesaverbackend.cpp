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

#include "statesaverbackend_p.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlInfo>

#include "backends/settingsstorage_p.h"

namespace BitQuick { namespace Tools {

StateSaverBackend::StateSaverBackend(QObject *parent)
    : QObject(parent)
{
    // connect application's save
    connect(QGuiApplication::instance(), &QGuiApplication::aboutToQuit,
            this, &StateSaverBackend::onAboutToQuit);

    // handle SIGINT and SIGTERM
    connect(StateSaverFactory::termination(), &AbnormalTerminationHandler::terminated,
            this, &StateSaverBackend::onSignalTriggered);
    StateSaverFactory::termination()->watchTerminationSignal(StateSaver::SaveStatus::Interrupted);
    StateSaverFactory::termination()->watchTerminationSignal(StateSaver::SaveStatus::Terminated);
}

StateSaverBackend::~StateSaverBackend()
{
}

void StateSaverBackend::onAboutToQuit()
{
    m_saveStatus = StateSaver::SaveStatus::Normal;
    saveStatus();
    Q_EMIT saveAndExit(m_saveStatus);
}

void StateSaverBackend::onSignalTriggered(StateSaver::SaveStatus reason)
{
    // disconnect aboutToQuit first as we'll quit after we save the state
    disconnect(QGuiApplication::instance(), &QGuiApplication::aboutToQuit,
               this, &StateSaverBackend::onAboutToQuit);

    m_saveStatus = reason;
    saveStatus();
    Q_EMIT saveAndExit(m_saveStatus);
    QGuiApplication::quit();
}

/******************************************************************************
 * Factory
 */
// a system can be launched with multiple QML engines
// therefore each singleton backend must be linked to the QML engine!
StateSaverBackend *StateSaverFactory::instance(QQmlEngine *engine)
{
    Q_ASSERT(engine);
    // check if the owner has a SettingsStorage child
    SettingsStorage *inst = engine->findChild<SettingsStorage*>();
    if (!inst) {
        inst = new SettingsStorage(engine);
    }
    return inst;
}

}} // namespace BitQuick::Tools
