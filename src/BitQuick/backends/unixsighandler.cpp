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

#include "unixsighandler_p.h"

#include <sys/socket.h>
#include <unistd.h>

#include <QtCore/QSocketNotifier>

namespace BitQuick { namespace Tools {

UnixSigHandler::UnixSigHandler(QObject *parent)
    : AbnormalTerminationHandler(parent)
{
}

int UnixSigHandler::reasonToSig(StateSaver::SaveStatus reason)
{
    switch (reason) {
    case StateSaver::SaveStatus::Interrupted:
        return SIGINT;
    case StateSaver::SaveStatus::Terminated:
        return SIGTERM;
    default:
        return -1;
    }
}

void UnixSigHandler::watchTerminationSignal(StateSaver::SaveStatus reason)
{
    int sig = reasonToSig(reason);
    if (sig < 0) {
        return;
    }
    if (handlers.contains(sig)) {
        return;
    }
    Handler handler = createHandler(sig);
    handlers.insert(sig, handler);
    sockets.insert(handler.first[1], sig);
}

UnixSigHandler::Handler UnixSigHandler::createHandler(int sig)
{
    struct sigaction sigAction;
    sigAction.sa_handler = signalHandler;
    sigemptyset(&sigAction.sa_mask);
    sigAction.sa_flags = SA_RESTART;

    if (sigaction(sig, &sigAction, 0) > 0) {
        qFatal("UNIX signal registration error");
    }
    Handler handler;
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, handler.first.data())) {
        qFatal("Cannot create signal socketpair");
    }
    handler.second = new QSocketNotifier(handler.first[1], QSocketNotifier::Read, this);
    QObject::connect(handler.second, &QSocketNotifier::activated,
                     this, &UnixSigHandler::onActivated);

    return handler;
}

void UnixSigHandler::signalHandler(int sig)
{
    Handler handler = UnixSigHandler::instance().handlers.value(sig);
    char value = 1;
    ssize_t size = write(handler.first[0], &value, sizeof(value));
    // this is needed to suppress write return value warning
    Q_UNUSED(size)
}

void UnixSigHandler::onActivated(int socket)
{
    int signal = sockets.value(socket);
    Handler handler = handlers.value(signal);

    handler.second->setEnabled(false);
    char value;
    ssize_t size = read(handler.first[1], &value, sizeof(value));
    // this is needed to suppress read return value warning
    Q_UNUSED(size)

    switch (signal) {
    case SIGINT:
        Q_EMIT terminated(StateSaver::SaveStatus::Interrupted);
        break;
    case SIGTERM:
        Q_EMIT terminated(StateSaver::SaveStatus::Terminated);
        break;
    default:
        break;
    }

    handler.second->setEnabled(true);
}

/******************************************************************************
 * Factory function
 */
AbnormalTerminationHandler *StateSaverFactory::termination()
{
    return &UnixSigHandler::instance();
}

}} // namespace BitQuick::Tools
