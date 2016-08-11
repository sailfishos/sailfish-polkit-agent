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


#include "dialog.h"

#include <QDebug>
#include <QQmlContext>

#include <sailfishapp.h>


ConfirmationDialog::ConfirmationDialog(Identity identity, const QString &action,
                                       const QString &message, const QVariantMap &details,
                                       PolkitQt1::Agent::AsyncResult *result)
    : m_identity(identity)
    , m_action(action)
    , m_message(message)
    , m_details(details)
    , m_result(result)
    , m_got_confirmation(false)
    , m_approved(false)
    , m_subject(details["subject"].toMap())
    , m_caller(details["caller"].toMap())
    , m_view(SailfishApp::createView())
{
    qDebug() << "Creating ConfirmationDialog";

    m_view->rootContext()->setContextProperty("confirmation", this);
    m_view->setSource(SailfishApp::pathTo("qml/confirmation.qml"));
    m_view->show();

    connect(m_view, SIGNAL(closing(QQuickCloseEvent*)),
            this, SLOT(onViewClosing()));
}

ConfirmationDialog::~ConfirmationDialog()
{
    qDebug() << "Deleting ConfirmationDialog";
    delete m_view;
}

ConfirmationDialog::Identity ConfirmationDialog::identity() const
{
    return m_identity;
}

void
ConfirmationDialog::setConfirmationResult(bool approved)
{
    if (m_got_confirmation) {
        // Confirmation already sent
        return;
    }

    m_got_confirmation = true;
    m_approved = approved;

    emit finished();
}

void
ConfirmationDialog::onViewClosing()
{
    qDebug() << "View is closing, sending confirmation -> false";
    // When we close the view, we want to make sure we send a confirmation
    // result so that the polkit transaction can be closed down properly
    setConfirmationResult(false);
}
