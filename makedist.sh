#!/bin/sh
# This script will create the xsldbg dist on most unix based systems
VERSION=$(grep -re "^XSLDBG_VERSION" xsldbg.pri | sed 's/\(^XSLDBG_VERSION="\)\([0-9,.]\+\)\(.*\".*$\)/\2/')

TMPDIR=~/tmp-xsldbg
SOURCEDIR=`dirname $(readlink -f $0)`

echo Temp dir is $TMPDIR
echo xsldbg source dir is $SOURCEDIR
if [ -d $TMPDIR ]; then rm -rf $TMPDIR; fi 
mkdir $TMPDIR

cd $SOURCEDIR
git clone . $TMPDIR/xsldbg-$VERSION

cd $TMPDIR

tar czf xsldbg-$VERSION.tar.gz --exclude .git --exclude xsldbg_submodule.pro --exclude-from=$SOURCEDIR/.gitignore xsldbg-$VERSION
echo created $PWD/xsldbg-$VERSION.tar.gz

zip -r -l --exclude .git --exclude .gitignore --exclude .gitmodules --exclude xsldbg_submodule.pro --exclude \*.git/\* -x@$SOURCEDIR/.gitignore xsldbg-$VERSION.zip xsldbg-$VERSION
echo created $PWD/xsldbg-$VERSION.zip
