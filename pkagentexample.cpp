#include "pkagentexample.h"

#include <PolkitQt1/Subject>

#include <QDebug>

PkAgentExample::PkAgentExample(int &argc, char **argv)
        : QCoreApplication(argc, argv)
{
    PolkitQt1::UnixSessionSubject session(getsid(0));

    m_listener.registerListener(session, "/org/sailfishos/PolicyKit1/AuthenticationAgent");
}
