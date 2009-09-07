<?xml version="1.0" ?>
<!-- 
     File : testgenerator.xsl    
     Author: Keith Isdale <k_isdale@tpg.com.au>
     Description: generate tests
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">
  <xsl:param name="baseFileName" select="'test2'"/>
  <xsl:strip-space elements="text"/>

  <xsl:template match="/">
    <xsl:call-template name="perlgenerate"/>
    <xsl:call-template name="scriptgenerate"/>
  </xsl:template>

  <xsl:template name="perlgenerate">
   <xsl:document href="test.pl" method="text">

    <xsl:text disable-output-escaping="yes">
#! perl -w

#use the package xsldbgmatch
require "xsldbgmatch.pl";

      my $template ="",  $fileName ="", $identifier ="", $state ="", $line ="";
      my $testCount = 0, $failedTestCount = 0, $optionalTestCount = 0;

      my $result = 1, $testName ="</xsl:text><xsl:value-of select="$baseFileName"/>";
      printf "Running test $testName\n";
        <xsl:apply-templates select="*"/>
  <xsl:text>
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

  </xsl:text>
  </xsl:document>
</xsl:template>

 <xsl:template name="scriptgenerate">
   <xsl:document href="test.script" method="text">
     <xsl:for-each select="//test">
       <xsl:value-of select="."/>
       <xsl:text>
</xsl:text>
     </xsl:for-each>     
  </xsl:document>
</xsl:template>


  <xsl:template match="test">
    <!-- ignore value as it is handled elsewhere--> 
  </xsl:template>

  <xsl:template match="cmd">
        <xsl:apply-templates select="result"/>
  </xsl:template>

  <xsl:template match="result">
    <xsl:apply-templates select="row"/>
  </xsl:template>
 
  <xsl:template match="row">
    <xsl:apply-templates select="*"/>
  </xsl:template>
 
  <xsl:template match="template">
    $testCount = $testCount + 1;
    <xsl:if test="@optional">$optionalTestCount = $optionalTestCount + 1;</xsl:if>
    if (xsldbgmatch::templateListMatch( 
             "<xsl:value-of select="@name"/>", 
               "<xsl:value-of select="@file"/>",  
                 "<xsl:value-of select="@line"/>", 
                    $testName) == 0){
      $failedTestCount = $failedTestCount + 1;
      <xsl:if test="not(@optional)">$result = 0;</xsl:if>
    }
  </xsl:template> 

  <xsl:template match="breakpoint">
    $testCount = $testCount + 1;
    <xsl:if test="@optional">$optionalTestCount = $optionalTestCount + 1;</xsl:if>
    if (xsldbgmatch::breakpointMatch( 
             "<xsl:value-of select="@file"/>",  
               "<xsl:value-of select="@line"/>",
                 $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       <xsl:if test="not(@optional)">$result = 0;</xsl:if>
    }
  </xsl:template> 

  <xsl:template match="breakpointlist">
    $testCount = $testCount + 1;
    <xsl:if test="@optional">$optionalTestCount = $optionalTestCount + 1;</xsl:if>
    if (xsldbgmatch::breakpointListMatch(
             "<xsl:value-of select="@id"/>",  
               "<xsl:value-of select="@state"/>" , 
                 "<xsl:value-of select="@file"/>",  
                   "<xsl:value-of select="@line"/>",
                     $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       <xsl:if test="not(@optional)">$result = 0;</xsl:if>
    }
  </xsl:template>


  <xsl:template match="parameter">
    $testCount = $testCount + 1;
    <xsl:if test="@optional">$optionalTestCount = $optionalTestCount + 1;</xsl:if>
    if (xsldbgmatch::parameterMatch(
             "<xsl:value-of select="@id"/>",  
               "<xsl:value-of select="@name"/>" , 
                 "<xsl:value-of select="@value"/>",
                   $testName) == 0){
       $failedTestCount = $failedTestCount + 1;
       <xsl:if test="not(@optional)">$result = 0;</xsl:if>
    }
  </xsl:template>

  <xsl:template match="matchtext">
    $testCount = $testCount + 1;
    <xsl:if test="@optional">$optionalTestCount = $optionalTestCount + 1;</xsl:if>
    if (xsldbgmatch::textMatch(
             "<xsl:value-of select="."/>", 
               $testName) == 0 ){
       $failedTestCount = $failedTestCount + 1;
       <xsl:if test="not(@optional)">$result = 0;</xsl:if>
    }
  </xsl:template>

</xsl:stylesheet>
