#ifndef SAILFISH_POLKIT_AGENT_DIALOG_H
#define SAILFISH_POLKIT_AGENT_DIALOG_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QQuickView>

#include <PolkitQt1/Agent/Listener>

class ConfirmationDialog : public QObject {
    Q_OBJECT

public:
    ConfirmationDialog(const QString &action, const QString &message,
            const QVariantMap &details, const QString &cookie,
            const QString &identity, PolkitQt1::Agent::AsyncResult *result);
    ~ConfirmationDialog();

    Q_INVOKABLE void setConfirmationResult(bool approved);

    Q_PROPERTY(QString message READ message NOTIFY changed)
    Q_PROPERTY(QString action READ action NOTIFY changed)
    Q_PROPERTY(QVariant details READ details NOTIFY changed)
    Q_PROPERTY(QString cookie READ cookie NOTIFY changed)
    Q_PROPERTY(QString identity READ identity NOTIFY changed)

    QString message() { return m_message; }
    QString action() { return m_action; }
    QVariant details() { return m_details; }
    QString cookie() { return m_cookie; }
    QString identity() { return m_identity; }

    Q_PROPERTY(QVariantMap subject READ subject NOTIFY changed)
    Q_PROPERTY(QVariantMap caller READ caller NOTIFY changed)

    QVariantMap subject() { return m_subject; }
    QVariantMap caller() { return m_caller; }

    PolkitQt1::Agent::AsyncResult *result() { return m_result; }
    bool approved() { return m_approved; }

signals:
    // This signal is never emitted at the moment, as we don't change the
    // properties of this window dynamically for now
    void changed();

    // Send approval to the D-Bus helper daemon
    void finished(ConfirmationDialog *dialog);

private slots:
    void onViewClosing();

private:
    QString m_action;
    QString m_message;
    QVariantMap m_details;
    QString m_cookie;
    QString m_identity;
    PolkitQt1::Agent::AsyncResult *m_result;
    bool m_got_confirmation;
    bool m_approved;

    QVariantMap m_subject;
    QVariantMap m_caller;

    QQuickView *m_view;
};

#endif /* SAILFISH_POLKIT_AGENT_DIALOG_H */
