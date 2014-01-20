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

#ifndef SAILFISH_POLKIT_AGENT_LISTENER_H
#define SAILFISH_POLKIT_AGENT_LISTENER_H

#include <QObject>
#include <QString>

#include <PolkitQt1/Identity>
#include <PolkitQt1/Details>
#include <PolkitQt1/Agent/Listener>
#include <PolkitQt1/Agent/Session>

#include "dialog.h"


class SailfishPolKitAgentListener : public PolkitQt1::Agent::Listener
{
    Q_OBJECT
    Q_DISABLE_COPY(SailfishPolKitAgentListener)

public:
    SailfishPolKitAgentListener(QObject *parent = 0);
    ~SailfishPolKitAgentListener();

public Q_SLOTS:
    void initiateAuthentication(const QString &actionId,
                                const QString &message,
                                const QString &iconName,
                                const PolkitQt1::Details &details,
                                const QString &cookie,
                                const PolkitQt1::Identity::List &identities,
                                PolkitQt1::Agent::AsyncResult *result);

    bool initiateAuthenticationFinish() { return true; }
    void cancelAuthentication() {}

private slots:
    void onFinished(ConfirmationDialog *dialog);
};

#endif /* SAILFISH_POLKIT_AGENT_LISTENER_H */
