
#! perl -w

#use the package xsldbgmatch
require "xsldbgmatch.pl";

      my $template ="",  $fileName ="", $identifier ="", $state ="", $line ="";
      my $testCount = 0, $failedTestCount = 0, $optionalTestCount = 0;

      my $result = 1, $testName ="test15";
      printf "Running test $testName\n";
        

    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "19",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "19",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "22",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "124",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "124",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "125",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "126",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "130",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "131",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "26",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "28",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "95",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "95",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "96",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "32",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "33",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "34",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "40",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "41",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "42",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "99",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "99",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "102",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "102",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "103",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "105",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "47",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "87",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "89",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "87",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "50",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "110",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "116",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "53",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "54",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "54",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "57",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "60",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "12",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "13",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "63",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "66",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "110",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "116",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "69",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "15",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "15",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "16",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "17",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "72",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "12",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "13",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "19",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "19",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "22",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "124",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "124",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "125",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "126",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "130",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "131",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "26",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "28",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "95",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "95",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "96",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "32",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "33",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "34",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "40",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "41",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "42",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "99",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "99",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "102",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "102",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "103",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "105",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "47",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "87",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "89",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "87",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "50",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "110",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "116",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "53",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "54",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "54",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "57",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "60",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "12",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "13",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "63",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "66",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "110",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "116",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "69",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "15",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "15",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "16",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "17",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "72",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "12",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "13",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  


    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "19",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "19",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "22",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "124",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "124",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "125",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "126",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "130",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "131",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "26",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "28",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "95",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "95",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "96",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "32",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "33",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "34",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "40",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "41",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "42",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "99",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "99",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "102",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "102",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "103",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "105",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "47",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "87",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "89",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "87",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "50",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "110",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "116",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "53",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "54",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "54",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "57",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "60",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "12",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "13",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "63",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "66",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "110",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "116",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "69",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "15",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "15",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "16",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "17",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "72",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "12",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "13",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "19",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "19",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "22",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "124",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "124",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "125",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "126",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "130",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "131",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "26",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "28",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "95",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "95",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "96",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "32",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "33",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "34",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "40",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "41",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "42",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "99",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "99",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "102",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "102",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "103",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "105",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "47",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "87",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "89",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "87",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "50",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "110",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "116",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "53",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "54",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "54",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "82",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "57",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "60",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "12",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "13",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "63",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "66",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "110",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "108",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "113",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "115",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "116",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "69",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "15",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "15",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "16",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_include_top.xsl",  
                   "17",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test2.xsl",
                   "72",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "11",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "12",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       $result = 0;
    }
  
    $testCount = $testCount + 1;
    
    if (xsldbgmatch::breakpointListMatch(
             "",  
               "" , 
                 "test_import.xsl",  
                   "13",
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

  
