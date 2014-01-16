TARGET = sailfish-polkit-agent

CONFIG += link_pkgconfig
PKGCONFIG += polkit-qt-agent-1 polkit-qt-core-1

SOURCES += main.cpp listener.cpp
HEADERS += listener.h

INCLUDEPATH += /usr/include/polkit-qt-1

INCLUDEPATH += .
DEPENDPATH += .

target.path = /usr/bin/
INSTALLS += target
