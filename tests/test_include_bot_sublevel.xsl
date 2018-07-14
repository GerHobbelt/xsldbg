<?xml version="1.0" ?>
<!--
     File : test_include_bot_sublevel.xsl
     Author: Keith Isdale <keithisdale@gmail.com>
     Description: stylesheet for include testing by test "test2"
     Copyright Reserved Under GPL
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template name="include_bot_sublevel">
        <xsl:apply-templates select="result/bottom_sublevel"/>
  </xsl:template>

  <xsl:template match="bottom_sublevel">
    <bottom_sublevel>
    <xsl:value-of select="@name" />
    </bottom_sublevel>
  </xsl:template>


</xsl:stylesheet>
