CONFIG += link_pkgconfig

PKGCONFIG += polkit-qt-agent-1 polkit-qt-core-1

SOURCES += main.cpp pkagentexample.cpp klistener.cpp
HEADERS += klistener.h pkagentexample.h

INCLUDEPATH += /usr/include/polkit-qt-1

INCLUDEPATH += .
DEPENDPATH += .

target.path = /usr/bin
INSTALLS += target
