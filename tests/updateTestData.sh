#!/bin/sh
# udate all test data to suite the test*.data files 
for a in 1 2 3 4 5 6 7 8 9 10 11 12 13; do echo $a; ../src/xsldbg  --param baseFileName:"'test$a'" testgenerator.xsl test$a.data --noshell; cp -v test.pl ../results/test$a.pl; cp -v test.script ../results/test$a.script; done
