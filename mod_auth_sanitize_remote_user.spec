Summary: Apache module that replaces problematic characters in (remote) user names
Name: mod_auth_sanitize_remote_user
Version: 1.0
Release: 2
Group: System
License: ASL 2.0
Source: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-buildroot
BuildRequires: httpd-devel >= 2.4.0
BuildRequires: gcc >= 4.8.0
Requires: httpd >= 2.4.0

%description
This Apache module allows the user name of the authenticated user to be sanitized by
replacing problematic characters (such as spaces) with a single unproblematic character
(such as an underscore).

%prep
%setup

%build
make

%install
rm -rf %{buildroot}
%make_install
install -d %{buildroot}/etc/httpd/conf.d
install -m 0644 auth_sanitize_remote_user.load %{buildroot}/etc/httpd/conf.d/auth_sanitize_remote_user.load

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root)
/usr/lib64/httpd/modules/mod_auth_sanitize_remote_user.so
/etc/httpd/conf.d/auth_sanitize_remote_user.load

%changelog
* Thu Sep 08 2016 Robert W Frank <robert.frank@manchester.ac.uk>
- fixed bug that caused a segfault when enabled in a http virtual host
* Thu Jun 23 2016 Robert W Frank <robert.frank@manchester.ac.uk>
- first version
