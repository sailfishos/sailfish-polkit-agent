/**
 * Sailfish polkit Agent: GUI Agent
 * Copyright (C) 2014 Jolla Ltd.
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

#include "listener.h"

#include "sailfish-polkit-agent.h"

#include <sailfishapp.h>

#include <QGuiApplication>
#include <PolkitQt1/Subject>


int main(int argc, char *argv[])
{
    QGuiApplication *app = SailfishApp::application(argc, argv);
    app->setQuitOnLastWindowClosed(false);

    SailfishPolKitAgentListener listener;
    PolkitQt1::UnixSessionSubject session(getsid(0));
    listener.registerListener(session, SAILFISH_POLKIT_AGENT_PATH);

    return app->exec();
}
