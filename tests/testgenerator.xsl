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
   <xsl:document href="{$baseFileName}.pl" method="text">

    <xsl:text disable-output-escaping="yes">
#! perl -w

#use the package xsldbgmatch
require "xsldbgmatch.pl";

my $template ="",  $fileName ="", $identifier ="", $state ="", $line ="";
my $result = 1;
  </xsl:text>
        <xsl:apply-templates select="*"/>
  <xsl:text>
    if ($result == 1){
      exit(0);
    }else{
      exit(1);
    }
  </xsl:text>
  </xsl:document>
</xsl:template>

 <xsl:template name="scriptgenerate">
   <xsl:document href="{$baseFileName}.script" method="text">
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
    $result = $result &amp;&amp; xsldbgmatch::templateListMatch( 
      "<xsl:value-of select="@name"/>", 
      "<xsl:value-of select="@file"/>",  
      "<xsl:value-of select="@line"/>", 
      "<xsl:value-of select="$baseFileName"/>" );
  </xsl:template> 

  <xsl:template match="breakpoint">
    $result =  $result &amp;&amp; xsldbgmatch::breakpointMatch( 
      "<xsl:value-of select="@file"/>",  
       "<xsl:value-of select="@line"/>",
       "<xsl:value-of select="$baseFileName"/>");
  </xsl:template> 

  <xsl:template match="breakpointlist">
    $result =  $result &amp;&amp; xsldbgmatch::breakpointListMatch(
      "<xsl:value-of select="@id"/>",  
        "<xsl:value-of select="@state"/>" , 
         "<xsl:value-of select="@file"/>",  
          "<xsl:value-of select="@line"/>",
          "<xsl:value-of select="$baseFileName"/>");
  </xsl:template>

</xsl:stylesheet>
