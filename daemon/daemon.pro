TEMPLATE=app
TARGET=sailfish-polkit-daemon

QT += dbus
QT -= gui

CONFIG += link_pkgconfig
PKGCONFIG += polkit-qt-core-1

INCLUDEPATH += /usr/include/polkit-qt-1

DEPENDPATH += .
INCLUDEPATH += .

SOURCES += main.cpp

SOURCES += polkitdaemon.cpp
HEADERS += polkitdaemon.h


# Generated at qmake time
system(qdbusxml2cpp \
    -c PolkitDaemonAdaptor \
    -a polkitdaemonadaptor.h:polkitdaemonadaptor.cpp \
    org.sailfishos.polkit.daemon.xml)

SOURCES += polkitdaemonadaptor.cpp
HEADERS += polkitdaemonadaptor.h


# Well-known name for our D-Bus service
DBUS_SERVICE_NAME=org.sailfishos.polkit.daemon


# Installation
service.files = $${DBUS_SERVICE_NAME}.service
service.path = /usr/share/dbus-1/system-services/

conf.files = $${DBUS_SERVICE_NAME}.conf
conf.path = /etc/dbus-1/system.d/

target.path = /usr/libexec/

INSTALLS += target service conf
