#
# Step counter daemon
# (C) kimmoli 2016
#
Name:       stpcntrd

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

Summary:    Step counter daemon
Version:    0.0.1
Release:    1
Group:      Qt/Qt
License:    LICENSE
URL:        https://github.com/kimmoli/stpcntrd
Source0:    %{name}-%{version}.tar.bz2
Requires:   sensorfw-qt5 > 0.8.30
Requires:   hybris-libsensorfw-qt5 > 0.8.30

BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  pkgconfig(sensord-qt5)
BuildRequires:  sensorfw-qt5-devel

%description
%{summary}

%prep
%setup -q -n %{name}-%{version}

%build

%qtc_qmake5 SPECVERSION=%{version}

%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}

%qmake5_install

%preun
# in case of complete removal, stop and disable
if [ "$1" = "0" ]; then
  systemctl-user stop %{name}
  systemctl-user disable %{name}
fi

%post
systemctl-user daemon-reload
systemctl-user start %{name}
systemctl-user enable %{name}

%pre
# In case of update, stop first
if [ "$1" = "2" ]; then
  systemctl-user stop %{name}
  systemctl-user disable %{name}
fi

%files
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/%{name}
%{_datadir}/dbus-1/
%config %{_sysconfdir}/systemd/user/
