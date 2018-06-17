<?xml version="1.0" ?>
<!-- 
     File : test_include_top.xsl     
     Author: Keith Isdale <keithisdale@gmail.com>
     Description: stylesheet for include testing by test "test2"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template name="include_top">
        <xsl:apply-templates select="result/top"/>
  </xsl:template>

<xsl:template match="top">
  <top>
  <xsl:value-of select="@name" />
  </top>
</xsl:template>


</xsl:stylesheet>
