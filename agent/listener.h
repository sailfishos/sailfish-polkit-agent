/**
 * Sailfish polkit Agent: GUI Agent
 * Copyright (C) 2014-2016 Jolla Ltd.
 * Contact: Thomas Perl <thomas.perl@jolla.com>
 *
 * Partially based on polkit-qt-1 example code:
 * Copyright (C) 2009 Jaroslav Reznik
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

#ifndef SAILFISH_POLKIT_AGENT_LISTENER_H
#define SAILFISH_POLKIT_AGENT_LISTENER_H

#include <PolkitQt1/Agent/Listener>
#include "qobjectptr.h"
#include "dialog.h"

class SailfishPolKitAgentListener : public PolkitQt1::Agent::Listener
{
    Q_OBJECT
public:
    explicit SailfishPolKitAgentListener(QObject *parent = 0);
public Q_SLOTS:
    void initiateAuthentication(const QString &actionId,
                                const QString &message,
                                const QString &iconName,
                                const PolkitQt1::Details &details,
                                const QString &cookie,
                                const PolkitQt1::Identity::List &identities,
                                PolkitQt1::Agent::AsyncResult *result) override;

    bool initiateAuthenticationFinish() override;
    void cancelAuthentication() override;
private:
    static QString userFromIdentity(ConfirmationDialog::Identity identity);
    ConfirmationDialog::Identity getIdentity(const PolkitQt1::Identity::List &identities);
    QObjectPtr<ConfirmationDialog> m_dialog;
};

#endif /* SAILFISH_POLKIT_AGENT_LISTENER_H */
