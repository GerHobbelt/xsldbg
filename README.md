Overview
========
xsldbg is debugger for XSLT scripts using API hooks kindly added libxslt libraries.

Features include
* run to a location in XSLT/XML file
    * step
    * stepup
    * stepdown
    * next
    * continue
    * run
* set breakpoints on XSLT/XML files
* view XSLT/XML data
    * view and possible set XSLT variables
    * view and set XSLT parameter variables feed to XSLT tranformation
    * view result of XPath expressions
    * watch result of XPath expressions
    * list templates in use
    * list stylesheets in use
    * list general parsed entities in use


Install dependancies
====================
This package requiries
* Qt5.5 Core module development libraries/headers to be present
* libexslt, libxslt and libxml2 libraries/headers to be present.

As example install prerequisites on
* OpenSuse Linux:

    sudo zypper install rpmbuild patterns-openSUSE-devel_qt5
    patterns-openSUSE-devel_basis patterns-openSUSE-devel_C_C++ libxslt-devel
    libxml2-devel libxslt libxml2
* Centos:

     sudo yum install gcc gcc-c++ make rpm-build libxslt-devel
     libxml2-devel libxslt libxml2 qt5-qtbase-devel
* Ubuntu based distro:

     sudo apt-get install g++ make qt5-qmake libxml2-dev libxslt1-dev
     qtbase5-dev qtbase5-dev-tools
* PLD Linux:

     sudo poldek -u gcc make Qt5Core-devel docbook-dtd412-xml libstdc++-devel libxml2-devel libxslt-devel perl-base qt5-qmake readline-devel  desktop-file-utils gtk-update-icon-cache  hicolor-icon-theme


Documentation
=============
* xsldbg man page (for *nix users)
* docs/en/html/index.html for the xsldbg manual
* docs/en/website/index.html for latest infomation and a quick tutorial about using xsldbg
* to add docbook DTD support to catalog on Linux use command like:
> sudo  xmlcatalog -v --add nextCatalog '' "file:///usr/share/xml/docbook/schema/dtd/4.5/catalog.xml" /etc/xml/catalog
* to regenerate docbook documentation saxon version 6 must be in path. See docs/en/en.pro



Note that the libxml2 library used supports the following path types for XSLT/XML documents

    <SomePath>
    file://<SomePath>
    http://<SomePath>
    ftp://[user[:password]@]host[:port]/path

When specifing 'http://' or 'ftp://' document paths ensure that host maps to IPv4 or IPv6 address used by related server. For
  example instead of localhost you may need to use the IPv4 address 127.0.0.1


License
=======
xsldbg is under the GPL2+ see 'COPYING' file for license text or https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html
main author of xsldbg is Keith Isdale <keithisdale@gmail.com>

The following libraries are used by xsldbg
* Qt5
    * Author of Qt5, see: https://www.qt.io/
    * License used is 'LGPL', see https://www.gnu.org/licenses/lgpl-3.0.en.html
* libxml2, libxslt and libexslt
    * Author for libxml related libraries, see: http://xmlsoft.org/index.html
    * License of libxml related libraries is 'MIT', see: https://opensource.org/licenses/mit-license.html
* iconv
    * Author of lib iconv, see: https://www.gnu.org/software/libiconv/
    * License of lib iconv is 'LGPL', see https://www.gnu.org/licenses/lgpl-3.0.en.html

Prebuilt win32 application uses MinGW32
* Author of MinGW32, see: http://www.mingw.org/
* License of MinGW32 runtime is public domain, see: http://www.mingw.org/license


Create rpm using rpmbuild
=========================
A very simple RPM spec file is provided in package, possibly this will be sufficient
if development dependacies are aready installed

    rpmbuild -tb xsldbg-*.tar.gz

To make used of custom .spec file for supported platforms

    mkdir -p ~/rpmbuild/SOURCES/
    cp xsldbg-*.tar.gz  ~/rpmbuild/SOURCES/
    # download the required .spec from sourceforge or extract from package like
    tar xzf ~/rpmbuild/SOURCES/xsldbg-*.tar.gz --wildcards "*/*.custom_spec"
    rpmbuild -bb */xsldbg_*distro*.custom_spec
where *distro* currently can be
* suse
* centos
* PLD_Linux


Note: THe PLD Linux team recommend using the RPM builds and SPEC they provide.

Compilation
===========
This package can be built on *nix and Windows.

If found tools xslt-config and xml2-config will be used to set
library and compliler flags, otherwise:
* $LIBXML_PREFIX must be set to the location of libxml2
* $LIBXSLT_PREFIX must be set to the location of libxslt
* $ICONV_PREFIX must be set to the location of iconv

For *nix when Qt5s qmake is present:

    qmake-qt5 -r qxsldbg.pro
    make
    sudo make install # may require sudo and it will update the System's XML catalog to include xsldbg's DTDs.

Note that on some *nix distributions qmake for Qt5 is not in $PATH,use qmake command like

    /usr/lib64/qt5/bin/qmake -r xsldbg.pro
or for Debian use

    qtchooser -qt=qt5 -run-tool=qmake -r xsldbg.pro

By default on *nix xsldbg installation prefix is "/usr" or "\\xsldbg" under Windows
documentation is installed to /usr/share/doc/packages/xsldbg or ${INSTALL_PREFIX}/doc/packages/xsldbg
application is installed to /usr/bin or ${INSTALL_PREFIX}/bin

To change the installation prefix run qmake like

    qmake-qt5 INSTALL_PREFIX="/some/path"

If full X11 environment is installed then to install xsldbg desktop menu links run qmake with addition of
	"CONFIG+=xsldbg_shortcut"

Currenty no testing has been done using version 4.7.0 on Windows(MSVC) though the following should work if Qt5 and other dependencies are setup correctly:

    qmake-qt5
    nmake
    nmake install
then if wished install xsldbg's catalog.xml via running

    createcatalog.bat

For WIN32 you must ensure that %LIBXML_PREFIX%\lib, %LIBXSLT_PREFIX%\lib, %ICONV_PREFIX%\lib has been added to your %PATH% before starting xsldbg.exe. It is recommended that you install xsldbg into c:\xsldbg

If on *nix and xsldbg is built in same directory as source tests can be run. For details see tests/README. Note currently 'test10' is known to fail at moment.
If you want to run test then:

    cd tests
    perl runtests.pl

For WIN32 user you will need to set several environment variables
 XSLDBG_PREFIX must to set to the location of xsldbg installation
*  LIBXML_PREFIX must be set to the location of libxml
*  LIBXSLT_PREFIX  must be set to the location of libxslt
*  ICONV_PREFIX must be set to the location of iconv

  You must ensure that %LIBXML_PREFIX%\lib, %LIBXSLT_PREFIX%\lib, %ICONV_PREFIX%\lib has been added to your %PATH% before starting qxsldbg.exe. Unless you yave copied the dll's found in these directories into you qxsldbg the \qxsldbg
  It is recommended that you install qxsldbg into c:\qxsldbg

Creating distribution package
=============================
To create dist packages in *nix the following must be present, see the makedist.sh script
* tar
* bzip2
* zip
