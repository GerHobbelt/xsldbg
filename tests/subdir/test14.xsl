<?xml version="1.0" ?>
<!-- 
     File : test14.xsl     
     Author: Keith Isdale <k_isdale@tpg.com.au>
     Description: stylesheet for test "test13"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <!-- Test the guesing stylesheet and XML file name, imported by test13.xsl-->
  <xsl:template name="fred">
    <xsl:text>Hi there from fred
</xsl:text>
  </xsl:template>

  <xsl:template match="data">
    <xsl:text>Hi there from data
</xsl:text>
	<xsl:apply-templates />
  </xsl:template>
</xsl:stylesheet>
