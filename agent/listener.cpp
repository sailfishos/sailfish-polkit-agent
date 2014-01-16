/*
 * This file is part of the Polkit-qt project
 * Copyright (C) 2009 Jaroslav Reznik <jreznik@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
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
 */

#include "listener.h"

#include <QDebug>
#include <QStringList>
#include <QProcess>

SailfishPolKitAgentListener::SailfishPolKitAgentListener(QObject *parent)
    : PolkitQt1::Agent::Listener(parent)
{
}

SailfishPolKitAgentListener::~SailfishPolKitAgentListener()
{
}

void
SailfishPolKitAgentListener::initiateAuthentication(const QString &actionId,
        const QString &message, const QString &iconName,
        const PolkitQt1::Details &details, const QString &cookie,
        const PolkitQt1::Identity::List &identities,
        PolkitQt1::Agent::AsyncResult *result)
{
    qDebug() << "action: " << actionId;
    qDebug() << "message: " << message;
    qDebug() << "iconName: " << iconName;
    qDebug() << "details: " << details.keys();
    qDebug() << "cookie: " << cookie;

    Q_FOREACH (const PolkitQt1::Identity &identity, identities) {
        qDebug() << "identity: " << identity.toString();
        QProcess helper;
        QStringList args;
        args << cookie << "root" /* XXX: use identity */;
        helper.start("/usr/libexec/sailfish-polkit-agent-helper", args);
        qDebug() << "GOT:" << helper.readAll();
        helper.waitForFinished();
        result->setCompleted();
    }
}
