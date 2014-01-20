/***************************************************************************
 *   Copyright (C) 2009 Jaroslav Reznik                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "listener.h"

#include <sailfishapp.h>

#include <QGuiApplication>
#include <PolkitQt1/Subject>

int main(int argc, char *argv[])
{
    QGuiApplication *app = SailfishApp::application(argc, argv);
    app->setQuitOnLastWindowClosed(false);

    SailfishPolKitAgentListener listener;
    PolkitQt1::UnixSessionSubject session(getsid(0));
    listener.registerListener(session, "/org/sailfishos/polkit/AuthenticationAgent");

    return app->exec();
}
