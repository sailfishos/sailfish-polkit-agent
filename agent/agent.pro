TARGET = sailfish-polkit-agent
CONFIG += sailfishapp
CONFIG += c++11
QT += quick qml dbus

PKGCONFIG += polkit-qt-agent-1 polkit-qt-core-1 libsystemd-login

INCLUDEPATH += /usr/include/polkit-qt-1
INCLUDEPATH += . ../include
DEPENDPATH += . ../include

HEADERS += listener.h \
    dialog.h \
    sessionhelper.h

SOURCES += main.cpp \
    listener.cpp \
    dialog.cpp \
    sessionhelper.cpp

unit.path = /usr/lib/systemd/user/
unit.files = sailfish-polkit-agent.service
INSTALLS += unit

OTHER_FILES += qml/confirmation.qml \
    qml/ProcessInfo.qml
