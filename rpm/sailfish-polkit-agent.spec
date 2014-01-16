Name: sailfish-polkit-agent
Version: 1.0.0
Release: 1
Summary: Sailfish polkit Agent
Group: Utilities
License: LGPL
Url: http://sailfishos.org/
Source: %{name}-%{version}.tar.bz2
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(polkit-qt-agent-1)
BuildRequires: pkgconfig(polkit-qt-core-1)

%description
This package contains the polkit authentication agent for Sailfish OS.

%prep
%setup -q

%build
%qmake5
make %{?_smp_mflags}

%install
make install INSTALL_ROOT=%{buildroot}

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%attr(4755,root,root) %{_libexecdir}/%{name}-helper
