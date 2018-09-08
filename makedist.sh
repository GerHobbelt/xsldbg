#!/bin/sh
# This script will create the xsldbg dist on most unix based systems
VERSION=$(grep -re "^XSLDBG_VERSION" xsldbg.pri | sed 's/\(^XSLDBG_VERSION="\)\([0-9,.]\+\)\(.*\".*$\)/\2/')

# the basic RPM spec file that is customized as needed
BASIC_SPEC_FILE=xsldbg.spec

# Apply current qxsldbg version to spec file and build test script
sed -i "s/\(define version [0-9].[0-9].[0-9]\)/define version ${VERSION}/" ${BASIC_SPEC_FILE}
sed -i "s/VERSION=[0-9].[0-9].[0-9]/VERSION=${VERSION}/" build_test.sh

# update the SUSE spec file
SUSE_SPEC_FILE=xsldbg_suse.custom_spec
cp $BASIC_SPEC_FILE $SUSE_SPEC_FILE
SUSE_REQUIRES="libQt5Core5 >= 5.5, libQt5Network5 >= 5.5, libxml2, libxslt"
SUSE_BUILD_REQUIRES="patterns-openSUSE-devel_qt5, patterns-openSUSE-devel_basis, patterns-openSUSE-devel_C_C++, libxslt-devel libxml2-devel, libxslt, libxml2"
sed -i "s/^\(Requires:.*$\)/Requires: ${SUSE_REQUIRES}/" ${SUSE_SPEC_FILE}
sed -i "s/^\(BuildRequires:.*$\)/BuildRequires: ${SUSE_BUILD_REQUIRES}/" ${SUSE_SPEC_FILE}

# update the Centos spec file
CENTOS_SPEC_FILE=xsldbg_centos.custom_spec
cp $BASIC_SPEC_FILE $CENTOS_SPEC_FILE
CENTOS_REQUIRES="qt5-qtbase >= 5.5, libxml2, libxslt"
CENTOS_BUILD_REQUIRES="gcc, gcc-c++, make, libxslt-devel, libxml2-devel, libxslt, libxml2, qt5-qtbase-devel"
sed -i "s/^\(Requires:.*$\)/Requires: ${CENTOS_REQUIRES}/" ${CENTOS_SPEC_FILE}
sed -i "s/^\(BuildRequires:.*$\)/BuildRequires: ${CENTOS_BUILD_REQUIRES}/" ${CENTOS_SPEC_FILE}


TMPDIR=~/tmp-xsldbg
SOURCEDIR=`dirname $(readlink -f $0)`

echo Temp dir is $TMPDIR
echo xsldbg source dir is $SOURCEDIR
if [ -d $TMPDIR ]; then rm -rf $TMPDIR; fi
mkdir $TMPDIR

cd $SOURCEDIR
git clone . $TMPDIR/xsldbg-$VERSION

cd $TMPDIR

tar czf xsldbg-$VERSION.tar.gz --exclude .git --exclude xsldbg_submodule.pro --exclude debian --exclude-from=$SOURCEDIR/.gitignore xsldbg-$VERSION
echo created $PWD/xsldbg-$VERSION.tar.gz

zip -q -r -l --exclude .git --exclude .gitignore --exclude .gitmodules --exclude xsldbg_submodule.pro --exclude debian --exclude \*.git/\* -x@$SOURCEDIR/.gitignore xsldbg-$VERSION.zip xsldbg-$VERSION
echo created $PWD/xsldbg-$VERSION.zip
