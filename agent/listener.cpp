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

#include "dialog.h"

#include "sailfish-polkit-agent.h"

#include <QDebug>
#include <QMap>
#include <QFileInfo>
#include <QDBusInterface>

static void
finishRequest(bool approved, PolkitQt1::Agent::AsyncResult *result,
        const QString &cookie=QString(), const QString &identity=QString())
{
    if (approved) {
        QDBusInterface daemon(SAILFISH_POLKIT_DAEMON_NAME,
                SAILFISH_POLKIT_DAEMON_PATH,
                SAILFISH_POLKIT_DAEMON_INTF,
                QDBusConnection::systemBus());

        daemon.callWithArgumentList(QDBus::Block,
                SAILFISH_POLKIT_DAEMON_RESPONSE_METHOD,
                QVariantList() << cookie << identity);
    }

    result->setCompleted();
}

static QVariantMap
getProcessDetails(long pid)
{
    QVariantMap result;

    QFileInfo procdir(QString("/proc/%1").arg(pid));
    QFile cmdline(QString("/proc/%1/cmdline").arg(pid));

    QString execpath = QString("/proc/%1/exe").arg(pid);
    QFileInfo execinfo(execpath);
    QString execfile = execinfo.canonicalFilePath();

    cmdline.open(QIODevice::ReadOnly);
    QList<QByteArray> bargs = cmdline.readAll().split('\0');
    cmdline.close();

    QStringList args;
    Q_FOREACH (const QByteArray &arg, bargs) {
        args << QString::fromUtf8(arg);
    }

    // Usually the last arg is "" (the cmdline is terminated by '\0')
    if (args.size() && args.last() == "") {
        args.pop_back();
    }

    // Fall back to using the first command line argument if we cannot
    // read the /proc/<pid>/exe file (e.g. because the owner is root)
    if (args.size() && execfile == execpath) {
        execfile = args.first();
    }

    result["pid"] = qlonglong(pid);
    result["user"] = procdir.owner();
    result["group"] = procdir.group();
    result["exec"] = execfile;
    result["cmdline"] = args;

    return result;
}


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

    Q_FOREACH (const QString &key, details.keys()) {
        qDebug() << "details[" << key << "] =" << details.lookup(key);
    }

    Q_FOREACH (const PolkitQt1::Identity &identity, identities) {
        qDebug() << "identity: " << identity.toString();
    }

    QString identity = identities.first().toString();

    QVariantMap subject = getProcessDetails(details.lookup("polkit.subject-pid").toLong());
    QVariantMap caller = getProcessDetails(details.lookup("polkit.caller-pid").toLong());

    qDebug() << "Subject:" << subject;
    qDebug() << "Caller:" << caller;

    // TODO: Determine whether or not sideloading is enabled (via SSU?)
    bool sideloading = false;
    bool developermode = QFile("/usr/bin/devel-su").exists();

    qDebug() << "Sideloading:" << sideloading;
    qDebug() << "Developer mode:" << developermode;

    // TODO: Move rules (where possible) to polkit JavaScript rules files
    if (subject["group"].toString() == "privileged") {
        // Allow setgid privileged apps to call everything
        qDebug() << "Allowing access to" << actionId << "for privileged app.";
        finishRequest(true, result, cookie, identity);
    } else if (actionId.startsWith("org.freedesktop.packagekit.") &&
            !(sideloading || developermode)) {
        // Disallow PackageKit APIs if we don't have sideloading or developer
        // mode (TODO: offer to enable sideloading in the UI/show notification)
        qDebug() << "Denying access to" << actionId << "(no sideloading/devmode)";
        finishRequest(false, result);
    } else {
        // In all other cases, we just ask the interactive user and
        // expect them to know what they are doing

        QVariantMap mdetails;
        mdetails["subject"] = subject;
        mdetails["caller"] = caller;

        ConfirmationDialog *dialog = new ConfirmationDialog(actionId, message,
                mdetails, cookie, identity, result);

        QObject::connect(dialog, SIGNAL(finished(ConfirmationDialog *)),
                         this, SLOT(onFinished(ConfirmationDialog *)));
    }
}

void
SailfishPolKitAgentListener::onFinished(ConfirmationDialog *dialog)
{
    finishRequest(dialog->approved(), dialog->result(),
            dialog->cookie(), dialog->identity());
    dialog->deleteLater();
}
