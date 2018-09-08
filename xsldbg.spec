%define name xsldbg
%define version 4.7.0
%define release 1
%define prefix /usr
%define builddir $RPM_BUILD_DIR/%{name}-%{version}

Summary: XSLT debugging/execution at console
Name: %{name}
Version: %{version}
Release: %{release}
Prefix: %{prefix}
Group: Development/Debuggers
License: GPLv2+
Distribution: Any GCC 3.2 based
Vendor: Keith Isdale <keithisdale@gmail.com>
Packager: Keith Isdale <keithisdale@gmail.com>
Source: %{name}-%{version}.tar.gz
URL: http://xsldbg.sourceforge.net/
BuildRequires: libQt5Core-devel, libQt5Gui-devel libxslt-devel, libxml2-devel
Requires: libQt5Core5 >= 5.5 libQt5Gui5 >= 5.5
BuildRoot: /tmp/build-%{name}-%{version}

%description
A console tool to debug stylesheets(the eXtensible Stylesheet Language)
that has commands similar to the Unix/Linux debugger gdb.
It has three major modes of execution of stylesheets.
   o Run the whole stylesheet
   o Step to the next XSLT instruction
   o Continue until next break point is found, or the stylesheet has restarted

%prep
rm -rf $RPM_BUILD_ROOT
rm -rf %{builddir}

%setup
touch `find . -type f`

%build
qmake-qt5
make

%install
rm -rf $RPM_BUILD_ROOT
INSTALL_ROOT=$RPM_BUILD_ROOT make install

cd $RPM_BUILD_ROOT
# compress the xsldbg manpage before file list is generated
gzip usr/share/man/man1/xsldbg.1

find . -type f | sed -e 's,^\.,\%attr(-\,root\,root) ,' \
	-e '/\/config\//s|^|%config|' >> \
	$RPM_BUILD_DIR/file.list.%{name}
find . -type l | sed 's,^\.,\%attr(-\,root\,root) ,' >> \
	$RPM_BUILD_DIR/file.list.%{name}
#echo "Sleeping for a minute to ensure that the timestamp in index.cashe.bz2 is correct"
#sleep 60
#touch ./%{kdeprefix}/share/doc/HTML/en/xsldbg/index.cache.bz2


%clean
rm -rf $RPM_BUILD_ROOT
rm -rf %{builddir}
rm -f $RPM_BUILD_DIR/file.list.%{name}

%files -f ../file.list.%{name}
%license COPYING

%changelog
* Sat Sep 08 2018 Keith Isdale <keithisdale@gmail.com> - 4.7.0-1
- Initial RPM release

