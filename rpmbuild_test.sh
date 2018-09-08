#!/bin/sh -ex
# This script will test the qxsldbg building on most *nix based systems

VERSION=4.7.0

TMPDIR=~/tmp-qxsldbg
if [ ! -d "$TMPDIR" ];then
    mkdir -p "$TMPDIR"
fi

cd $TMPDIR
wget ${PACKAGE_URL}/qxsldbg-${VERSION}.tar.gz

if [ -e /usr/bin/rpmbuild ]; then
    echo "Simple build test using rpmbuild asuming tar.gz copied to $TMPDIR"
    cp qxsldbg-$VERSION.tar.gz ~/rpmbuild/SOURCES/
    DISTRO=""
    SUSEDISTRO=`uname -a | sed -n "/suse/p"`
    CENTOSDISTRO=`uname -a | sed -n "/centos/p"`
    if [ -n "$SUSEDISTRO" ];then
        DISTRO="suse"
    fi

    if [ -n "$CENTOSDISTRO" ];then
        DISTRO="centos"
    fi

    if [ -z "$DISTRO" ]; then
        rpmbuild -tb $PWD/qxsldbg-$VERSION.tar.gz
    else
        CUSTOM_SPEC_FILE=qxsldbg_${DISTRO}.custom_spec
        tar xzf ~/rpmbuild/SOURCES/qxsldbg-${VERSION}.tar.gz --wildcards  "qxsldbg-*/${CUSTOM_SPEC_FILE}"
        rpmbuild -bb qxsldbg-*/${CUSTOM_SPEC_FILE}
    fi
elif [ -e "/usr/bin/qtchooser" ]; then
    echo "build using qtchooser"
    tar xzf qxsldbg-${VERSION}.tar.gz
    cd qxsldbg-${VERSION}
    qtchooser -qt=qt5 -run-tool=qmake -r qxsldbg.pro
    make -j4
elif [ -e "/usr/bin/qmake-qt5" ]; then
    echo "build using qmake-qt5"
    tar xzf qxsldbg-${VERSION}.tar.gz
    cd qxsldbg-${VERSION}
    qmake-qt5 -r qxsldbg.pro
    make -j4
else
    # we should not get to here
    echo "build using qmake, usumes is Qt5's qmake is in path"
    tar xzf qxsldbg-${VERSION}.tar.gz
    cd qxsldbg-${VERSION}
    qmake -r qxsldbg.pro
    make -j4
fi
