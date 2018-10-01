#! perl -w

use English;

# NB: we skip test 11 and 12 as they to be run by hand
my @tests = qw (test1 test2 test3 test4 test5 test6 test7 test8
                test9 test10 test13 test14 test15);

my $xsldbg = "xsldbg";
my $test = "";
my $testResult = 0;

print "Note:Test 'test7' may fail due differences in text node proccessing in libxml2 versions\n";


for ($index = 0; $index <= $#tests; $index++)
  {
    $test = $tests[$index];
    print "Test $test ";
    system "$xsldbg --gdb --shell --noautoloadconfig --stdout --output ../results/$test.output $test.xsl $test.xml < ../results/$test.script > ../results/$test.log";
    if ($? == 0) {
        system ("perl ../results/$test.pl > ../results/$test.results < ../results/$test.log");
        if ($? == 0){
            print "succeeded\n";
        }else{
            print "failed\n";
            $testResult = 1;
        }
    }
  }


$test = "test16"; # change directory tests
print "Test $test ";
system "$xsldbg --shell --noautoloadconfig --stdout --cd .. --output results/$test.output tests/$test.xsl tests/$test.xml < ../results/$test.script > ../results/$test.log";
if ($? == 0) {
    # system ("perl ../results/$test.pl > ../results/$test.results < ../results/$test.log");
    if ($? == 0){
        print "succeeded\n";
    }else{
        print "failed\n";
        $testResult = 1;
    }
}

print "Note: The following 'http' tests will fail if the http server is not setup to suit xsldbg testing\n";
# http source debugging test
@tests = qw (test17 test18);
for ($index = 0; $index <= $#tests; $index++)
  {
    $test = $tests[$index];
    print "Test $test ";
    system "$xsldbg --gdb --shell --noautoloadconfig --stdout --output results/$test.output http://127.0.0.1/xsldbg/tests/$test.xsl http://127.0.0.1/xsldbg/tests/$test.xml < ../results/$test.script > ../results/$test.log";
    if ($? == 0) {
        system ("perl ../results/$test.pl > ../results/$test.results < ../results/$test.log");
        if ($? == 0){
            print "succeeded\n";
        }else{
            print "failed\n";
            $testResult = 1;
        }
    }
}

print "Note: The following 'ftp' tests will fail if the ftp server is not setup to suit xsldbg testing\n";
# ftp source debugging test
@tests = qw (test19);
for ($index = 0; $index <= $#tests; $index++)
  {
    $test = $tests[$index];
    print "Test $test ";
    system "$xsldbg --gdb --shell --noautoloadconfig --stdout --output results/$test.output ftp://xsldbg:xsldbg\@127.0.0.1/xsldbg/tests/$test.xsl ftp://xsldbg:xsldbg\@127.0.0.1/xsldbg/tests/$test.xml < ../results/$test.script > ../results/$test.log";
    if ($? == 0) {
        system ("perl ../results/$test.pl > ../results/$test.results < ../results/$test.log");
        if ($? == 0){
            print "succeeded\n";
        }else{
            print "failed\n";
            $testResult = 1;
        }
    }
}

if ($testResult == 0) {
    print "\n ======= All tests passed! =========== \n";
} else {
    print "\n ======= Some tests failed! =========== \n";
}

exit($testResult);
