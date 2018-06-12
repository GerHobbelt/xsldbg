#! perl -w

use English;

# NB: we skip test 11 and 12 as they to be run by hand
my @tests = qw (test1 test2 test3 test4 test5 test6 test7 test8 
                test9 test10 test13 test14);

my $xsldbg = "xsldbg";
my $test = "";
my $testResult = 0;

for ($index = 0; $index <= $#tests; $index++)
  {
    $test = $tests[$index];
    print "Test $test ";    
    system "$xsldbg --shell --stdout $test.xsl $test.xml < ../results/$test.script > ../results/$test.log";
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

exit($testResult);
