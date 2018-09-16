
#! perl -w

#use the package xsldbgmatch
require "xsldbgmatch.pl";

      my $template ="",  $fileName ="", $identifier ="", $state ="", $line ="";
      my $testCount = 0, $failedTestCount = 0, $optionalTestCount = 0;

      my $result = 1, $testName ="test10";
      printf "Running test $testName\n";
        


    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "test10.xsl",  
               "13",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "test10.xsl",  
               "13",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "test10.xsl",  
               "13",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "test10.xsl",  
               "13",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test1.xml",  
                   "8",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xml",  
                   "9",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test3.xml",  
                   "9",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test4.xml",  
                   "9",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  



    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Starting stylesheet", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "test2.xml",  
               "9",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  



    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Starting stylesheet", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "test10.xsl",  
               "13",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  



    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "test10.xsl",  
               "24",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "1234", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "5678", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  



    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "<h1>test1</h1>", 
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

  