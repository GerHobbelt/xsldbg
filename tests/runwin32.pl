#! perl -w

my @tests = qw (test1 test2 test3 test4 test5 test6 test7 test8
                test9 test10 test12 test13);
my $xsldbg = "xsldbg";
my $test= "";
my $testResult = 0;

for ($index = 0; $index <= $#tests; $index++)
  {
    $test = $tests[$index];
    system "$xsldbg --shell --stdout $test.xsl $test.xml < ..\\results\\$test.script > ..\\results\\$test.log";
    print "Test $test ";
    if (system ("perl -I ..\\tests ..\\results\\$test.pl > ..\\results\\$test.results < ..\\results\\$test.log") == 0){
      print "succeeded\n";
    }else{
      print "failed\n";
      $testResult = 1;
    }
  }

exit($testResult);
