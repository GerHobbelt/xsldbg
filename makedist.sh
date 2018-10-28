#!/bin/sh
# This script will create the xsldbg dist on most unix based systems
VERSION=$(grep -re "^XSLDBG_VERSION" xsldbg.pri | sed 's/\(^XSLDBG_VERSION="\)\([0-9,.]\+\)\(.*\".*$\)/\2/')

# Apply current qxsldbg version to spec file and build test script
sed -i "s/\(define version [0-9].[0-9].[0-9]\)/define version ${VERSION}/" xsldbg.spec
sed -i "s/VERSION=[0-9].[0-9].[0-9]/VERSION=${VERSION}/" build_test.sh

# update the SUSE spec file
sed -i "s/\(define version [0-9].[0-9].[0-9]\)/define version ${VERSION}/" xsldbg_suse.custom_spec

# update the Centos spec file
sed -i "s/\(define version [0-9].[0-9].[0-9]\)/define version ${VERSION}/" xsldbg_centos.custom_spec

# update the PLD_Linux spec file 
sed -i "s/\(define version [0-9].[0-9].[0-9]\)/define version ${VERSION}/" xsldbg_PLD_Linux.custom_spec

TMPDIR=~/tmp-xsldbg
SOURCEDIR=`dirname $(readlink -f $0)`

echo Temp dir is $TMPDIR
echo xsldbg source dir is $SOURCEDIR
if [ -d $TMPDIR ]; then rm -rf $TMPDIR; fi
mkdir $TMPDIR

cd $SOURCEDIR
cp -r $SOURCEDIR $TMPDIR/xsldbg-$VERSION

cd $TMPDIR

tar czf xsldbg-$VERSION.tar.gz --exclude .git --exclude xsldbg_submodule.pro --exclude debian --exclude-from=$SOURCEDIR/.gitignore xsldbg-$VERSION
echo created $PWD/xsldbg-$VERSION.tar.gz

zip -q -r -l --exclude .git --exclude .gitignore --exclude .gitmodules --exclude xsldbg_submodule.pro --exclude debian --exclude \*.git/\* -x@$SOURCEDIR/.gitignore xsldbg-$VERSION.zip xsldbg-$VERSION
echo created $PWD/xsldbg-$VERSION.zip
