#!/bin/sh
# This script will create the xsldbg dist on most unix based systems
VERSION=4.5.0
TMPDIR=~/tmp-xsldbg
SOURCEDIR=`dirname $0`

echo Temp dir is $TMPDIR
echo xsldbg source dir is $SOURCEDIR
if [ ! -d $TMPDIR ]; then mkdir $TMPDIR; fi 
rm -rf $TMPDIR/xsldbg*
cp -r $SOURCEDIR $TMPDIR/xsldbg-$VERSION

cd $TMPDIR

tar czf xsldbg-$VERSION.tar.gz --exclude .git --exclude .cvsignore --exclude *.swp xsldbg-$VERSION 
echo created $PWD/xsldbg-$VERSION.tar.gz

