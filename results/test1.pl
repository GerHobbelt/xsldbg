
#! perl -w

#use the package xsldbgmatch
require "xsldbgmatch.pl";

      my $template ="",  $fileName ="", $identifier ="", $state ="", $line ="";
      my $testCount = 0, $failedTestCount = 0, $optionalTestCount = 0;

      my $result = 1, $testName ="";
      printf "Running test $testName\n";
        
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test1.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::templateListMatch( 
             "/", 
               "test1.xsl",  
                 "11", 
                    $testName) == 0){
      $failedTestCount = $failedTestCount + 1;
      $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::templateListMatch( 
             "html", 
               "",  
                 "21", 
                    $testName) == 0){
      $failedTestCount = $failedTestCount + 1;
      $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::templateListMatch( 
             "head", 
               "",  
                 "25", 
                    $testName) == 0){
      $failedTestCount = $failedTestCount + 1;
      $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::templateListMatch( 
             "body", 
               "",  
                 "29", 
                    $testName) == 0){
      $failedTestCount = $failedTestCount + 1;
      $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::templateListMatch( 
             "h1", 
               "",  
                 "33", 
                    $testName) == 0){
      $failedTestCount = $failedTestCount + 1;
      $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::templateListMatch( 
             "para", 
               "",  
                 "37", 
                    $testName) == 0){
      $failedTestCount = $failedTestCount + 1;
      $result = 0;
    }
  
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "test1.xsl",  
               "12",
                 $testName) == 0){
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

  