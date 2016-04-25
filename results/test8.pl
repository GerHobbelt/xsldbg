
#! perl -w

#use the package xsldbgmatch
require "xsldbgmatch.pl";

      my $template ="",  $fileName ="", $identifier ="", $state ="", $line ="";
      my $testCount = 0, $failedTestCount = 0, $optionalTestCount = 0;

      my $result = 1, $testName ="test8";
      printf "Running test $testName\n";
        
 

    $testCount = $testCount + 1;
    $optionalTestCount = $optionalTestCount + 1;
    if (xsldbgmatch::textMatch(
             "Change to directory \/usr\/share\/xml\/docbook\/xsl\/html", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       
    }
  




    $testCount = $testCount + 1;
    $optionalTestCount = $optionalTestCount + 1;
    if (xsldbgmatch::textMatch(
             "Load of source deferred use run command", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       
    }
  




    $testCount = $testCount + 1;
    $optionalTestCount = $optionalTestCount + 1;
    if (xsldbgmatch::textMatch(
             "Load of xml data deferred use run command", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       
    }
  





    $testCount = $testCount + 1;
    $optionalTestCount = $optionalTestCount + 1;
    if (xsldbgmatch::textMatch(
             "Total of 1382 breakPoints present", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       
    }
  


    if ($result == 1){
      if ($failedTestCount != 0){
       printf "Success but some optional tests failed : $failedTestCount of $testCount tests\n";
      }
      printf "\n\n";
      exit(0);
    }else{
      printf "Failure : $failedTestCount of $testCount tests failed\n";
      printf "\n\n";
      exit(1);
    }

  