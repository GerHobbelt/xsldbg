
#! perl -w

#use the package xsldbgmatch
require "xsldbgmatch.pl";

      my $template ="",  $fileName ="", $identifier ="", $state ="", $line ="";
      my $testCount = 0, $failedTestCount = 0, $optionalTestCount = 0;

      my $result = 1, $testName ="../results/test2";
      printf "Running test $testName\n";
        

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "18",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "18",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "21",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "104",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "105",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "106",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "110",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "110",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "25",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "67",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "67",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "67",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "27",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "80",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "81",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "31",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "32",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "33",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "67",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "67",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "67",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "39",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "40",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "41",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "84",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "87",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "87",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "88",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "88",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "46",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "72",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "73",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "74",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "49",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "93",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "94",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "95",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "98",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "98",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "100",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "100",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "101",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "52",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "53",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "67",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "67",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "67",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointMatch( 
             "",  
               "56",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  




    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: break command arguments not in format", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: break command arguments not in format", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: Unable to read line number", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: Unable to find template", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: Breakpoint to template", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: Breakpoint at file", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  



    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",  
                   "18",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  





    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: enable/disable command arguments not in format", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: enable/disable command arguments not in format", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: Unable to read line number", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: Breakpoint at template", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  



    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "disabled" , 
                 "test2.xsl",  
                   "18",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  



    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "enabled" , 
                 "test2.xsl",  
                   "18",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  



    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "disabled" , 
                 "test2.xsl",  
                   "18",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  


    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: Breakpoint at file", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  



    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: Breakpoint at file", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  




    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",  
                   "18",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  



    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: delete command arguments not in format", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: delete command arguments not in format", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: Unable to read line number", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "Error: Breakpoint at template", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "No file breakpoints set", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "No file breakpoints set", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "No file breakpoints set", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::textMatch(
             "No file breakpoints set", 
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

  