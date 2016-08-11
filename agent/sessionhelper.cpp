/**
 * Sailfish polkit Agent: GUI Agent
 * Copyright (C) 2016 Jolla Ltd.
 * Contact: Lucien Xu <sfietkonstantin@free.fr>
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

#include "sessionhelper.h"
#include <systemd/sd-login.h>
#include <unistd.h>
#include <string>

PolkitQt1::Subject SessionHelper::getSeatedSession()
{
    uid_t uid = 0;
    if (sd_pid_get_owner_uid(getpid(), &uid) < 0) {
        return PolkitQt1::Subject();
    }

    std::string session;
    char **sessions = nullptr;
    int count = sd_uid_get_sessions(uid, 0, &sessions);

    for (int i = 0; i < count; ++i) {
        if (session.empty() && sessions[i] != nullptr) {
            char *seat = nullptr;
            sd_session_get_seat(sessions[i], &seat);
            if (seat != nullptr) {
                session = sessions[i];
            }
            free(seat);
        }
        free(sessions[i]);
    }
    free(sessions);
    return PolkitQt1::UnixSessionSubject(QString::fromStdString(session));
}
