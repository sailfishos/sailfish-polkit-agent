#ifndef SAILFISH_POLKIT_AGENT_DIALOG_H
#define SAILFISH_POLKIT_AGENT_DIALOG_H

#include <QObject>
#include <QString>
#include <QQuickView>

#include <PolkitQt1/Agent/Listener>

class ConfirmationDialog : public QObject {
    Q_OBJECT

public:
    ConfirmationDialog(const QString &action, const QString &message,
            const QString &cookie, const QString &identity,
            PolkitQt1::Agent::AsyncResult *result);
    ~ConfirmationDialog();

    Q_INVOKABLE void setConfirmationResult(bool approved);

    Q_PROPERTY(QString message READ getMessage NOTIFY changed)
    Q_PROPERTY(QString action READ getAction NOTIFY changed)

    QString getMessage() { return m_message; }
    QString getAction() { return m_action; }

signals:
    // This signal is never emitted at the moment, as we don't change the
    // properties of this window dynamically for now
    void changed();

private slots:
    void onViewClosing();

private:
    QString m_action;
    QString m_message;
    QString m_cookie;
    QString m_identity;
    PolkitQt1::Agent::AsyncResult *m_result;
    bool m_got_confirmation;

    QQuickView *m_view;
};

#endif /* SAILFISH_POLKIT_AGENT_DIALOG_H */
