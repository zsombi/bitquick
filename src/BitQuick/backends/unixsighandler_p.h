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

#ifndef UNIXSIGHANDLER_P_H
#define UNIXSIGHANDLER_P_H

#include <signal.h>
#include <array>

#include <QtCore/QObject>
#include <QtCore/QHash>
#include "statesaver_p.h"
#include "backends/statesaverbackend_p.h"

class QSocketNotifier;

namespace BitQuick { namespace Tools {

class UnixSigHandler : public AbnormalTerminationHandler
{
    Q_OBJECT

public:
    typedef QPair<std::array<int, 2>, QSocketNotifier*> Handler;
    static UnixSigHandler &instance()
    {
        static UnixSigHandler instance;
        return instance;
    }

    void watchTerminationSignal(StateSaver::SaveStatus reason) override;

Q_SIGNALS:
    void signalTriggered(StateSaver::SaveStatus);

private:
    explicit UnixSigHandler(QObject *parent = nullptr);
    int reasonToSig(StateSaver::SaveStatus reason);
    void onActivated(int socket);

    static void signalHandler(int);

    Handler createHandler(int sig);

    // signal/handler pair
    QHash<int, Handler> handlers;
    // signal/socket pair
    QHash<int, int> sockets;
};

}} // namespace BitQuick::Tools

#endif // UNIXSIGHANDLER_P_H
