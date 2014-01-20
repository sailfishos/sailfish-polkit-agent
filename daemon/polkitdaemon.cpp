/**
 * Sailfish polkit Agent: D-Bus Helper Daemon
 * Copyright (C) 2014 Jolla Ltd.
 * Contact: Thomas Perl <thomas.perl@jolla.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB. If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 **/


#include "polkitdaemon.h"
#include "polkitdaemonadaptor.h"

#include "sailfish-polkit-agent.h"

#include <QDBusConnection>
#include <PolkitQt1/Authority>
#include <PolkitQt1/Identity>


namespace Sailfish {

PolkitDaemon::PolkitDaemon(QObject *parent)
    : QObject(parent)
    , QDBusContext()
    , autoclose()
{
    // Inactivity timeout after which the service quits to save resources
    const int AUTOCLOSE_TIMEOUT_MS = 60 * 1000;

    // Configure auto-close timer to quit the service after inactivity
    autoclose.setSingleShot(true);
    autoclose.setInterval(AUTOCLOSE_TIMEOUT_MS);
    QObject::connect(&autoclose, SIGNAL(timeout()),
                     this, SLOT(quit()));

    QDBusConnection connection = QDBusConnection::systemBus();
    if (!connection.registerService(SAILFISH_POLKIT_DAEMON_NAME)) {
        qFatal("Cannot register D-Bus service at %s",
                SAILFISH_POLKIT_DAEMON_NAME);
    }

    if (!connection.registerObject(SAILFISH_POLKIT_DAEMON_PATH, this)) {
        qFatal("Cannot register object at %s",
                SAILFISH_POLKIT_DAEMON_PATH);
    }

    new PolkitDaemonAdaptor(this);

    // Every time a client action is carried out, we call autoclose.start() to
    // reset the timer. Do it here for the first time.
    autoclose.start();
}

PolkitDaemon::~PolkitDaemon()
{
}

void
PolkitDaemon::sendResponse(const QString &cookie, const QString &identity)
{
    if (!isPrivileged()) return;

    PolkitQt1::Authority *authority = PolkitQt1::Authority::instance();

    // TODO: parse identity string instead of using user root
    PolkitQt1::Identity pkidentity = PolkitQt1::UnixUserIdentity("root");
    if (!authority->authenticationAgentResponseSync(cookie, pkidentity)) {
        sendErrorReply(QDBusError::AccessDenied, authority->errorDetails());
    }

    autoclose.start();
}

void
PolkitDaemon::quit()
{
    if (!isPrivileged()) return;

    QCoreApplication::quit();
}

bool
PolkitDaemon::isPrivileged()
{
    if (!calledFromDBus()) {
        // Local function calls are always privileged
        return true;
    }

    // Get the PID of the calling process
    pid_t pid = connection().interface()->servicePid(message().service());

    // The /proc/<pid> directory is owned by EUID:EGID of the process
    QFileInfo info(QString("/proc/%1").arg(pid));
    if (info.group() != "privileged" && info.owner() != "root") {
        sendErrorReply(QDBusError::AccessDenied,
                QString("PID %1 is not in privileged group").arg(pid));
        return false;
    }

    return true;
}

}; /* namespace Sailfish */
