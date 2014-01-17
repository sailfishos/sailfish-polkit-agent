#include "dialog.h"

#include <QDebug>
#include <QQmlContext>
#include <QProcess>


ConfirmationDialog::ConfirmationDialog(const QString &action,
        const QString &message, const QString &cookie, const QString &identity,
        PolkitQt1::Agent::AsyncResult *result)
    : m_action(action)
    , m_message(message)
    , m_cookie(cookie)
    , m_identity(identity)
    , m_result(result)
    , m_got_confirmation(false)
    , m_view(new QQuickView())
{
    qDebug() << "Creating ConfirmationDialog";

    m_view->rootContext()->setContextProperty("confirmation", this);
    m_view->setSource(QUrl("qrc:/qml/confirmation.qml"));
    m_view->show();

    connect(m_view, SIGNAL(closing(QQuickCloseEvent*)),
            this, SLOT(onViewClosing()));
}

ConfirmationDialog::~ConfirmationDialog()
{
    qDebug() << "Deleting ConfirmationDialog";
    delete m_view;
}

void
ConfirmationDialog::setConfirmationResult(bool approved)
{
    if (m_got_confirmation) {
        // Confirmation already sent
        return;
    }

    m_got_confirmation = true;
    qDebug() << "confirm:" << approved;

    if (approved) {
        QProcess helper;
        helper.start("/usr/libexec/sailfish-polkit-agent-helper",
                QStringList() << m_cookie << "root" /* XXX: use identity */);
        qDebug() << "GOT:" << helper.readAll();
        helper.waitForFinished();
    }

    m_result->setCompleted();
    deleteLater();
}

void
ConfirmationDialog::onViewClosing()
{
    qDebug() << "View is closing, sending confirmation -> false";
    // When we close the view, we want to make sure we send a confirmation
    // result so that the polkit transaction can be closed down properly
    setConfirmationResult(false);
}
