
#! perl -w

#use the package xsldbgmatch
require "xsldbgmatch.pl";

      my $template ="",  $fileName ="", $identifier ="", $state ="", $line ="";
      my $testCount = 0, $failedTestCount = 0, $optionalTestCount = 0;

      my $result = 1, $testName ="test14";
      printf "Running test $testName\n";
        
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Entity SystemID:\"notemessage1.xml\"", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Entity PublicID:\"-//xsldbg//tests notemessage2 XML V1.0//EN\" SystemID:\"notemessage2.xml\"
", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Entity SystemID:\"note3.xml\"", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Total of 3 entity found", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Entity SystemID:\"notemessage1.xml\" URI:\"file:///usr/share/doc/packages/xsldbg/en/notemessage1.xml\"", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Entity PublicID:\"-//xsldbg//tests notemessage2 XML V1.0//EN\" SystemID:\"notemessage2.xml\" URI:\"file:///usr/share/doc/packages/xsldbg/en/notemessage2.xml\"", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Entity SystemID:\"note3.xml\" URI:\"file:///usr/share/doc/packages/xsldbg/en/note3.xml\"", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Total of 3 entity found", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
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

  