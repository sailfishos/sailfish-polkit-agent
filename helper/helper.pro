TARGET = sailfish-polkit-agent-helper

CONFIG += link_pkgconfig
PKGCONFIG += polkit-qt-core-1

SOURCES += main.cpp

INCLUDEPATH += /usr/include/polkit-qt-1

INCLUDEPATH += .
DEPENDPATH += .

target.path = /usr/libexec/
INSTALLS += target
