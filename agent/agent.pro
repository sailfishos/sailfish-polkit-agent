TARGET = sailfish-polkit-agent

QT += quick qml

CONFIG += link_pkgconfig
PKGCONFIG += polkit-qt-agent-1 polkit-qt-core-1

SOURCES += main.cpp listener.cpp dialog.cpp
HEADERS += listener.h dialog.h

INCLUDEPATH += /usr/include/polkit-qt-1

INCLUDEPATH += .
DEPENDPATH += .

target.path = /usr/bin/
INSTALLS += target

RESOURCES += agent.qrc
