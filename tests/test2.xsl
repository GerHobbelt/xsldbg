<?xml version="1.0" ?>
<!-- 
     File : test2.xsl     
     Author: Keith Isdale <k_isdale@tpg.com.au>
     Description: stylesheet for test "test2"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template match="/">
        <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="html">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="head">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="body">
    <xsl:apply-templates/>
  </xsl:template> 

  <xsl:template match="h1">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="para">
    <xsl:apply-templates/>
  </xsl:template>



</xsl:stylesheet>
