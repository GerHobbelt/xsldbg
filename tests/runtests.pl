#! perl -w

use English;

# NB: we skip test 11 and 12 as they to be run by hand
my @tests = qw (test1 test2 test3 test4 test5 test6 test7 test8 
		test9 test10 test13);

my $xsldbg = "/Users/keithisdale/Documents/lang/cpp/build-xsldbg-Desktop_Qt_5_6_0_clang_64bit-Release/src/xsldbg";
my $test = "";
my $testResult = 0;

for ($index = 0; $index <= $#tests; $index++)
  {
    $test = $tests[$index];
    system "$xsldbg --shell --stdout $test.xsl $test.xml < ../results/$test.script > ../results/$test.log";
    print "Test $test ";
    if (system ("perl ../results/$test.pl > ../results/$test.results < ../results/$test.log") == 0){
      print "succeeded\n";
    }else{
      print "failed\n";
      $testResult = 1;
    }
  }

exit($testResult);
