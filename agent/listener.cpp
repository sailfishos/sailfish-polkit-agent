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

#include "listener.h"
#include "dialog.h"
#include "sailfish-polkit-agent.h"
#include <QFileInfo>
#include <QDBusInterface>
#include <QLoggingCategory>
#include <PolkitQt1/Details>

Q_DECLARE_LOGGING_CATEGORY(listenerLog)
Q_LOGGING_CATEGORY(listenerLog, "listener")

static QDebug operator<<(QDebug debug, const PolkitQt1::Details &details)
{
    QStringList out;
    foreach (const QString &key, details.keys()) {
        out.append(QString(QLatin1String("%1: %2")).arg(key, details.lookup(key)));
    }

    QDebugStateSaver saver(debug);
    debug.nospace() << "{" << out.join(QLatin1String(", ")).toLocal8Bit().data() << "}";
    return debug;
}

static QDebug operator<<(QDebug debug, const PolkitQt1::Identity::List &identities)
{
    QStringList out;
    foreach (const PolkitQt1::Identity &identity, identities) {
        out.append(identity.toString());
    }

    QDebugStateSaver saver(debug);
    debug.nospace() << "[" << out.join(QLatin1String(", ")).toLocal8Bit().data() << "]";
    return debug;
}

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

void
SailfishPolKitAgentListener::initiateAuthentication(const QString &actionId,
        const QString &message, const QString &iconName,
        const PolkitQt1::Details &details, const QString &cookie,
        const PolkitQt1::Identity::List &identities,
        PolkitQt1::Agent::AsyncResult *result)
{
    if (m_dialog) {
        qCWarning(listenerLog) << "Another authentication session is running";
        result->setError(("Another authentication session is running, please try again later."));
        result->setCompleted();
        return;
    }

    qCDebug(listenerLog) << "Received an initiate authentication";
    qCDebug(listenerLog) << "  action:" << actionId;
    qCDebug(listenerLog) << "  message:" << message;
    qCDebug(listenerLog) << "  iconName:" << iconName;
    qCDebug(listenerLog) << "  details:" << details;
    qCDebug(listenerLog) << "  cookie:" << cookie;
    qCDebug(listenerLog) << "  identities:" << identities;

    QVariantMap subject = getProcessDetails(details.lookup("polkit.subject-pid").toLong());
    QVariantMap caller = getProcessDetails(details.lookup("polkit.caller-pid").toLong());

    qCDebug(listenerLog) << "  subject:" << subject;
    qCDebug(listenerLog) << "  caller:" << caller;

    QVariantMap mdetails;
    mdetails["subject"] = subject;
    mdetails["caller"] = caller;

    ConfirmationDialog::Identity identity (getIdentity(identities));
    if (identity == ConfirmationDialog::Invalid) {
        qCWarning(listenerLog) << "No supported identity found. Provided identities" << identities;
        result->setError(("No supported identity found."));
        result->setCompleted();
        return;
    }

    m_dialog.reset(new ConfirmationDialog(getIdentity(identities), actionId, message, mdetails, result));

    connect(m_dialog.get(), &ConfirmationDialog::finished, [this, result, cookie, identity]() {
        finishRequest(m_dialog->approved(), result, cookie, userFromIdentity(identity));
        m_dialog.reset();
    });
}

bool SailfishPolKitAgentListener::initiateAuthenticationFinish()
{
    return true;
}

void SailfishPolKitAgentListener::cancelAuthentication()
{
}

QString SailfishPolKitAgentListener::userFromIdentity(ConfirmationDialog::Identity identity)
{
    switch (identity) {
    case ConfirmationDialog::User:
        return QLatin1String("nemo");
    case ConfirmationDialog::Admin:
        return QLatin1String("root");
    default:
        return QString();
    }
}

ConfirmationDialog::Identity SailfishPolKitAgentListener::getIdentity(const PolkitQt1::Identity::List &identities)
{
    // We get the lowest permission level.
    // If we are asked to authorize as root and as nemo, we choose nemo over root
    ConfirmationDialog::Identity returned (ConfirmationDialog::Invalid);

    foreach (const PolkitQt1::Identity &identity, identities) {
        const QString &name (identity.toString().remove("unix-user:"));
        if (name == QLatin1String("nemo")) {
            returned = ConfirmationDialog::User;
        } else if (name == QLatin1String("root")) {
            if (returned == ConfirmationDialog::Invalid) {
                returned = ConfirmationDialog::Admin;
            }
        }
    }
    return returned;
}
