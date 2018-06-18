#!/bin/sh

# copy test data for use by http and ftp tests
cp -u *.xsl *.xml /home/xsldbg/xsldbg_tests/
if [ ! -d "/home/xsldbg/xsldbg_tests/subdir" ]; then
    mkdir -p /home/xsldbg/xsldbg_tests/subdir
fi
cp -u test2.xml subdir/*  /home/xsldbg/xsldbg_tests/subdir/

if [ ! -d ~/temp/xsldbg_tests/subdir ]; then
    mkdir -p ~/temp/xsldbg_tests/subdir
fi
cp -u *.xsl *.xml ~/temp/xsldbg_tests/
cp -u test2.xml subdir/*  ~/temp/xsldbg_tests/subdir/

cp -u *.xsl *.xml /srv/www/htdocs/xsldbg/tests/
if [ ! -d /srv/www/htdocs/xsldbg/tests/subdir ]; then
    mkdir -p /srv/www/htdocs/xsldbg/tests/subdir
fi
cp -u test2.xml subdir/*  /srv/www/htdocs/xsldbg/tests/subdir/



# update all test data to suite the test*.data files
for a in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20; do
    echo $a;
    xsldbg --cd $PWD --output test.pl  --param baseFileName:"'test$a'" testgenerator.xsl test$a.data --noshell
    if [ $? -ne 0 ]; then
        echo "Failed to gernerate test script for test$a"
        break
    fi
    cp -v test.pl ../results/test$a.pl
    cp -v test.script ../results/test$a.script
done

# remove temporary files
rm test.pl test.script
