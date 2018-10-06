#!/bin/sh -ex
# This script will test the xsldbg/qxsldbg building on most *nix based systems
# 
# Usage: build_test.sh [norpm] [PRODUCT] [PACKAGE_URL] [VERSION] [MAKE_JOBS]
#    
# where 
#	norpm disables usage of rpm
# 	PRODUCT is defaults to "xsldbg"; POSSIBLE values "xsldbg", "qxsldbg"
#	PACKAGE_URL defaults to "http://localhost/public" or $PACKAGE_URL if set
#	VERSION default to "4.7.0" or $VERSION if set
# 	MAKE_JOBS defaults to "4"
#	

if [ "$1" == "norpm" ];then
   shift
   NO_RPM="true"
fi
if [ -z "$PRODUCT" ]; then
  PRODUCT=$1
fi
if [ -z "$PRODUCT" ]; then
  if [ -e "xsldbg.pro" ];then
     PRODUCT="xsldbg"
  fi
  if [ -e "qxsldbg.pro" ];then
     PRODUCT="qxsldbg"
  fi
  if [ -e "CMakeLists.txt" ];then
     PRODUCT="kxsldbg"
  fi
fi

TMPDIR=~/tmp-${PRODUCT}
if [ -d "$TMPDIR" ];then
    rm -rf $TMPDIR
fi
mkdir -p "$TMPDIR"

if [ -z "$PACKAGE_URL" ];then
  PACKAGE_URL=$2
fi
if [ -z "$PACKAGE_URL" ];then
  PACKAGE_URL="http://localhost/public"
fi

if [ -z "$VERSION" ]; then
  VERSION=$3
fi
if [ -z "$VERSION" ]; then
  VERSION=4.8.0
fi

if [ -z "$MAKE_JOBS" ];then
  MAKE_JOBS=4
fi
if [ -z "$RPM_BUILD_NCPUS" ];then
  export RPM_BUILD_NCPUS=4
fi


cd $TMPDIR
wget ${PACKAGE_URL}/${PRODUCT}-${VERSION}.tar.gz

if [ -z "$NO_RPM" -a -e /usr/bin/rpmbuild ]; then
    echo "Simple build test using rpmbuild asuming tar.gz copied to $TMPDIR"
    cp ${PRODUCT}-$VERSION.tar.gz ~/rpmbuild/SOURCES/
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
        rpmbuild -tb $PWD/${PRODUCT}-$VERSION.tar.gz
    else
        CUSTOM_SPEC_FILE=${PRODUCT}_${DISTRO}.custom_spec
        tar xzf ~/rpmbuild/SOURCES/${PRODUCT}-${VERSION}.tar.gz --wildcards  "${PRODUCT}-*/${CUSTOM_SPEC_FILE}"
        rpmbuild --clean -bb ${PRODUCT}-*/${CUSTOM_SPEC_FILE}
    fi
elif [ -e "/usr/bin/qtchooser" ]; then
    echo "build using qtchooser"
    tar xzf ${PRODUCT}-${VERSION}.tar.gz
    cd ${PRODUCT}-${VERSION}
    qtchooser -qt=qt5 -run-tool=qmake -r ${PRODUCT}.pro
    make -j${MAKE_JOBS}
    sudo make install
elif [ -e "/usr/bin/qmake-qt5" ]; then
    echo "build using qmake-qt5"
    tar xzf ${PRODUCT}-${VERSION}.tar.gz
    cd ${PRODUCT}-${VERSION}
    qmake-qt5 -r ${PRODUCT}.pro
    make -j${MAKE_JOBS}
    sudo make install
else
    # we should not get to here
    echo "build using qmake, usumes is Qt5's qmake is in path"
    tar xzf ${PRODUCT}-${VERSION}.tar.gz
    cd ${PRODUCT}-${VERSION}
    qmake -r ${PRODUCT}.pro
    make -j${MAKE_JOBS}
    sudo make install
fi
