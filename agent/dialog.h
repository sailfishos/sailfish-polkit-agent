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

#ifndef SAILFISH_POLKIT_AGENT_DIALOG_H
#define SAILFISH_POLKIT_AGENT_DIALOG_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QQuickView>

#include <PolkitQt1/Agent/Listener>

class ConfirmationDialog : public QObject
{
    Q_OBJECT
    Q_ENUMS(Identity)
    Q_PROPERTY(Identity identity READ identity CONSTANT)
    Q_PROPERTY(QString message READ message NOTIFY changed)
    Q_PROPERTY(QString action READ action NOTIFY changed)
    Q_PROPERTY(QVariant details READ details NOTIFY changed)
    Q_PROPERTY(QVariantMap subject READ subject NOTIFY changed)
    Q_PROPERTY(QVariantMap caller READ caller NOTIFY changed)
public:
    enum Identity
    {
        Invalid,
        User,
        Admin
    };
    ConfirmationDialog(Identity identity, const QString &action, const QString &message,
                       const QVariantMap &details, PolkitQt1::Agent::AsyncResult *result);
    ~ConfirmationDialog();
    Identity identity() const;
    QString message() { return m_message; }
    QString action() { return m_action; }
    QVariant details() { return m_details; }
    QVariantMap subject() { return m_subject; }
    QVariantMap caller() { return m_caller; }

    PolkitQt1::Agent::AsyncResult *result() { return m_result; }
    bool approved() { return m_approved; }

    Q_INVOKABLE void setConfirmationResult(bool approved);
signals:
    // This signal is never emitted at the moment, as we don't change the
    // properties of this window dynamically for now
    void changed();

    // Send approval to the D-Bus helper daemon
    void finished();

private slots:
    void onViewClosing();

private:
    Identity m_identity {Invalid};
    QString m_action;
    QString m_message;
    QVariantMap m_details;
    PolkitQt1::Agent::AsyncResult *m_result;
    bool m_got_confirmation;
    bool m_approved;

    QVariantMap m_subject;
    QVariantMap m_caller;

    QQuickView *m_view;
};

#endif /* SAILFISH_POLKIT_AGENT_DIALOG_H */
