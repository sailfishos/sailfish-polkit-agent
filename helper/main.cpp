#include <QCoreApplication>
#include <PolkitQt1/Authority>
#include <PolkitQt1/Identity>

#include <stdio.h>
#include <QTimer>

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <cookie> <username>\n", argv[0]);
        return 1;
    }

    QCoreApplication app(argc, argv);
    QString cookie = argv[1];
    QString username = argv[2];

    // TODO: Use "/usr/lib/qt5/plugins/devicelock/encpartition --check-code"

    PolkitQt1::Authority *authority = PolkitQt1::Authority::instance();

    PolkitQt1::Identity identity = PolkitQt1::UnixUserIdentity(username);
    if (!authority->authenticationAgentResponseSync(cookie, identity)) {
        printf("Error: %s\n", qPrintable(authority->errorDetails()));
        return 1;
    }

    return 0;
}
