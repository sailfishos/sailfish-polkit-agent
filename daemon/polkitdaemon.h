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


#ifndef ORG_SAILFISHOS_POLKIT_DAEMON_H
#define ORG_SAILFISHOS_POLKIT_DAEMON_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QDBusContext>

namespace Sailfish {

class PolkitDaemon : public QObject, protected QDBusContext {
    Q_OBJECT

    public:
        PolkitDaemon(QObject *parent=NULL);
        virtual ~PolkitDaemon();

    public slots:
        void sendResponse(const QString &cookie, const QString &identity);
        void quit();

    private:
        static const char *SERVICE_NAME;
        static const char *OBJECT_PATH;

    private:
        bool isPrivileged();

    private:
        QTimer autoclose;
};

}; /* namespace Sailfish */

#endif /* ORG_SAILFISHOS_POLKIT_DAEMON_H */
