<?xml version="1.0" ?>
<!--
     File : test_include.xsl
     Author: Keith Isdale <keithisdale@gmail.com>
     Description: stylesheet for include testing by test "test2"
     Copyright Reserved Under GPL
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:include href="test_include_bot_sublevel.xsl"/>

  <xsl:template name="include_bot">
        <xsl:apply-templates select="result/bottom"/>
        <xsl:call-template name="include_bot_sublevel" />
  </xsl:template>

  <xsl:template match="bottom">
    <bottom>
    <xsl:value-of select="@name" />
    </bottom>
  </xsl:template>


</xsl:stylesheet>
