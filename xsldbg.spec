%define name xsldbg
%define version 4.5.0
%define release 1
%define prefix /usr
%define kdeprefix /opt/kde4
%define qtprefix /usr/lib/qt4
%define gnomeprefix /opt/gnome
%define builddir $RPM_BUILD_DIR/%{name}-%{version}

Summary: xsldbg XSLT debugging/execution 
Name: %{name}
Version: %{version}
Release: %{release}
Prefix: %{prefix}
Group: Development/Debuggers
License: GPL
Distribution: Any GCC 3.2 based 
Vendor: Keith Isdale <keithisdale@gmail.com>
Packager: Keith Isdale <keithisdale@gmail.com>
Source: %{name}-%{version}.tar.gz
URL: http://xsldbg.sourceforge.net/ 
Requires: kdelibs >= 4.0 libxml2 >= 2.4.3 libxslt >= 1.0.13
BuildRoot: /tmp/build-%{name}-%{version}

%description
xsldbg is a text based tool to debug stylesheets (the eXtensible Stylesheet Language) and has commands similar to the Unix/Linux debugger gdb. 
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
aclocal
autoheader
automake
autoconf
CXXFLAGS="$RPM_OPT_FLAGS" CFLAGS="$RPM_OPT_FLAGS" qmake
make


%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install-strip

cd $RPM_BUILD_ROOT
find . -type d | sed '1,2d;s,^\.,\%attr(-\,root\,root) \%dir ,' > \
	$RPM_BUILD_DIR/file.list.%{name}
find . -type f | sed -e 's,^\.,\%attr(-\,root\,root) ,' \
	-e '/\/config\//s|^|%config|' >> \
	$RPM_BUILD_DIR/file.list.%{name}
find . -type l | sed 's,^\.,\%attr(-\,root\,root) ,' >> \
	$RPM_BUILD_DIR/file.list.%{name}
echo "%docdir %{prefix}/share/packages/xsldbg" >> $RPM_BUILD_DIR/file.list.%{name}
echo "%docdir %{kdeprefix}/share/doc/HTML/en/xsldbg" >> $RPM_BUILD_DIR/file.list.%{name}
echo "%docdir %{prefix}/share/gnome/help/xsldbg" >> $RPM_BUILD_DIR/file.list.%{name}
echo "Sleeping for a minute to ensure that the timestamp in index.cashe.bz2 is correct"
sleep 60
touch ./%{kdeprefix}/share/doc/HTML/en/xsldbg/index.cache.bz2


%clean
rm -rf $RPM_BUILD_ROOT
rm -rf %{builddir}
rm -f $RPM_BUILD_DIR/file.list.%{name}

%files -f ../file.list.%{name}
