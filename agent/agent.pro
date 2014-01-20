TARGET = sailfish-polkit-agent

CONFIG += sailfishapp
QT += quick qml dbus

PKGCONFIG += polkit-qt-agent-1 polkit-qt-core-1

SOURCES += main.cpp listener.cpp dialog.cpp
HEADERS += listener.h dialog.h

INCLUDEPATH += /usr/include/polkit-qt-1

INCLUDEPATH += . ../include
DEPENDPATH += . ../include

unit.path = /usr/lib/systemd/user/
unit.files = sailfish-polkit-agent.service
INSTALLS += unit
