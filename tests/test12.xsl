<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- 
     File : test12.xsl     
     Author: Keith Isdale <k_isdale@tpg.com.au>
     Description: stylesheet for test "test12"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <!-- A bit corny but test out the ability to use encodings to
       standard output -->

  <xsl:output method="xml" encoding="ISO-8859-1" />
    <xsl:variable name="l�global" select="'l�global'" />
 
  <xsl:template match="/">
    <xsl:apply-templates select="root">
          <xsl:with-param name="l�local" select="'l�local#2'" />
          <xsl:with-param name="lelocal" select="'lelocal#2'" />
        </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="tr�s">
    <xsl:param name="l�local" select="'l�local#1'" />
    <xsl:apply-templates />
  </xsl:template>

  <xsl:template match="tres">
    <xsl:param name="l�local" select="'l�local#1'" />
      <xsl:param name="lelocal" select="'lelocal#1'" />
    <xsl:apply-templates />
  </xsl:template>

</xsl:stylesheet>
