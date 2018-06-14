#! perl -w

use English;

# NB: we skip test 11 and 12 as they to be run by hand
#my @tests = qw (test1 test2 test3 test4 test5 test6 test7 test8
#                test9 test10 test13 test14 test15);
my @tests = qw ();

my $xsldbg = "xsldbg";
my $test = "";
my $testResult = 0;

for ($index = 0; $index <= $#tests; $index++)
  {
    $test = $tests[$index];
    print "Test $test ";    
    system "$xsldbg --shell --stdout --output ../results/$test.output $test.xsl $test.xml < ../results/$test.script > ../results/$test.log";
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
system "$xsldbg --shell --stdout --cd .. --output results/$test.output tests/$test.xsl tests/$test.xml < ../results/$test.script > ../results/$test.log";
if ($? == 0) {
    # system ("perl ../results/$test.pl > ../results/$test.results < ../results/$test.log");
    if ($? == 0){
        print "succeeded\n";
    }else{
        print "failed\n";
        $testResult = 1;
    }
}


$test = "test17"; # http source debugging test
print "Test $test ";
system "$xsldbg --shell --stdout --output results/$test.output http://localhost/xsldbg/tests/$test.xsl http://localhost/xsldbg/tests/$test.xml < ../results/$test.script > ../results/$test.log";
if ($? == 0) {
    system ("perl ../results/$test.pl > ../results/$test.results < ../results/$test.log");
    if ($? == 0){
        print "succeeded\n";
    }else{
        print "failed\n";
        $testResult = 1;
    }
}


$test = "test18"; # invalid http source debugging test
print "Test $test ";
system "$xsldbg --shell --stdout --output results/$test.output http://localhost/xsldbg/tests/foo.xsl http://localhost/xsldbg/tests/bar.xml < ../results/$test.script > ../results/$test.log";
if ($? == 0) {
    system ("perl ../results/$test.pl > ../results/$test.results < ../results/$test.log");
    if ($? == 0){
        print "succeeded\n";
    }else{
        print "failed\n";
        $testResult = 1;
    }
}

$test = "test19"; # ftp source debugging test, note this require a 'xsldbg' user to be present with password of 'xsldbg' and tests director copied as /home/xsldbg/xsldbg_tests
print "Test $test ";
system "$xsldbg --shell --stdout --output results/$test.output ftp://xsldbg:xsldbg\@127.0.0.1/home/xsldbg/xsldbg_tests/$test.xsl ftp://xsldbg:xsldbg\@127.0.0.1/home/xsldbg/xsldbg_tests/$test.xml < ../results/$test.script > ../results/$test.log";
if ($? == 0) {
    system ("perl ../results/$test.pl > ../results/$test.results < ../results/$test.log");
    if ($? == 0){
        print "succeeded\n";
    }else{
        print "failed\n";
        $testResult = 1;
    }
}

exit($testResult);
