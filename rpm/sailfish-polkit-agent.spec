%define dbus_service_name org.sailfishos.polkit.daemon
%define dbus_service_path /org/sailfishos/polkit/daemon

Name: sailfish-polkit-agent
Version: 1.0.4
Release: 1
Summary: Sailfish polkit Agent
License: LGPL
Url: https://github.com/sailfishos/sailfish-polkit-agent
Source0: %{name}-%{version}.tar.bz2
Source1: %{name}.privileges
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: pkgconfig(Qt5Quick)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(polkit-qt-agent-1)
BuildRequires: pkgconfig(polkit-qt-core-1)
BuildRequires: pkgconfig(sailfishapp) >= 0.0.10
BuildRequires: pkgconfig(systemd)
Requires: dbus
Requires: procps

%description
This package contains the polkit authentication agent for Sailfish OS.

%prep
%setup -q

%build
%qmake5
make %{?_smp_mflags}

%install
make install INSTALL_ROOT=%{buildroot}

# Install agent into post-user-session
mkdir -p %{buildroot}%{_userunitdir}/post-user-session.target.wants
ln -s ../%{name}.service %{buildroot}%{_userunitdir}/post-user-session.target.wants/

mkdir -p %{buildroot}%{_datadir}/mapplauncherd/privileges.d
install -m 644 -p %{SOURCE1} %{buildroot}%{_datadir}/mapplauncherd/privileges.d/

%post
# Ask running daemon to quit
if pgrep -u root -f %{name} >/dev/null; then
    dbus-send --system --dest=%{dbus_service_name} \
        --print-reply %{dbus_service_path} \
        %{dbus_service_name}.quit >/dev/null 2>&1 || true
fi
# (Re-)start the agent
systemctl-user daemon-reload || :
systemctl-user stop %{name}.service || :
systemctl-user start %{name}.service || :

%preun
# Ask running daemon to quit
if pgrep -u root -f %{name} >/dev/null; then
    dbus-send --system --dest=%{dbus_service_name} \
        --print-reply %{dbus_service_path} \
        %{dbus_service_name}.quit >/dev/null 2>&1 || true
fi
# Stop the agent
systemctl-user stop %{name}.service || :
systemctl-user daemon-reload || :

%files
%defattr(-,root,root,-)
# Agent application and QML
%{_bindir}/%{name}
%{_datadir}/mapplauncherd/privileges.d/*
%{_datadir}/%{name}

# D-Bus Daemon (helper, system bus)
%{_libexecdir}/sailfish-polkit-daemon
%{_datadir}/dbus-1/system-services/%{dbus_service_name}.service
%{_sysconfdir}/dbus-1/system.d/%{dbus_service_name}.conf

# Agent application as systemd user session service
%{_userunitdir}/%{name}.service
%{_userunitdir}/post-user-session.target.wants/%{name}.service
