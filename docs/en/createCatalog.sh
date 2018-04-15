#!/bin/bash -x
if test -z "$XML_CATALOG_FILES"; then
    LOCALXML_CATALOG_FILES=/etc/xml/catalog
else
    LOCALXML_CATALOG_FILES=$XML_CATALOG_FILES
fi

DOCS_DIR=$1

if test -z "$DOCS_DIR";then
    echo Install prefix not passed as parameter to $0
    exit 1
fi

CATALOG_DIR=`dirname $LOCALXML_CATALOG_FILES`
if test -z "$CATALOG_DIR";then
    echo "Error unable to deterine catalog directory from '$LOCALXML_CATALOG_FILES'"
    exit 1
fi

echo Catalog file: $LOCALXML_CATALOG_FILES
echo Catalog dir: $CATALOG_DIR


if ! test -d $CATALOG_DIR; then
    echo "Warning $CATALOG_DIR does not exist, attempting to create it"
    mkdir $CATALOG_DIR
fi

if test -d $CATALOG_DIR; then
    echo "" > result.txt
    if ! test -e $LOCALXML_CATALOG_FILES; then 
            echo "Creating $LOCALXML_CATALOG_FILES" >> result.txt 2>&1
            xmlcatalog --noout --create $LOCALXML_CATALOG_FILES >> result.txt 2>&1
    fi
    if test "x`grep xsldbg.*catalog.xml $LOCALXML_CATALOG_FILES`" = "x" ;then
        xmlcatalog --noout --add nextCatalog '' "file://$DOCS_DIR/catalog.xml" $LOCALXML_CATALOG_FILES >> result.txt 2>&1
        echo "Adding xsldbg DTD to default xml catalog /etc/xml/catalog" >> result.txt 2>&1
        echo "Warning you may need to edit these by hand using xmlcatalog" >> result.txt 2>&1
    else
        echo "skipping addition of xsldbg catalog as it is already present" >> result.txt 2>&1
    fi
    echo "Results of seeking to add xsldbg catalog is"
    echo "----------------------------------------------"
    cat result.txt
    echo "----------------------------------------------"
else
    echo "Unable to create $LOCALXML_CATALOG_FILES as not such directory $CATALOG_DIR" 
fi
