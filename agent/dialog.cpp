#include "dialog.h"

#include <QDebug>
#include <QQmlContext>
#include <QProcess>
#include <QFileInfo>


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


ConfirmationDialog::ConfirmationDialog(const QString &action,
        const QString &message, const QVariantMap &details,
        const QString &cookie, const QString &identity,
        PolkitQt1::Agent::AsyncResult *result)
    : m_action(action)
    , m_message(message)
    , m_details(details)
    , m_cookie(cookie)
    , m_identity(identity)
    , m_result(result)
    , m_got_confirmation(false)
    , m_subject()
    , m_caller()
    , m_view(new QQuickView())
{
    qDebug() << "Creating ConfirmationDialog";

    if (details.contains("polkit.subject-pid")) {
        long pid = details["polkit.subject-pid"].toString().toLong();
        m_subject = getProcessDetails(pid);
    } else {
        // TODO
    }

    if (details.contains("polkit.caller-pid")) {
        // TODO: Parse caller PID
        long pid = details["polkit.caller-pid"].toString().toLong();
        m_caller = getProcessDetails(pid);
    } else {
        // TODO
    }

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
