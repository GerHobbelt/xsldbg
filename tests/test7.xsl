<?xml version="1.0" ?>
<!-- 
     File : test7.xsl     
     Author: Keith Isdale <keithisdale@gmail.com>
     Description: stylesheet for test "test7"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <!-- Test xsl:text, xsl:message -->

  <xsl:template match="/">
    <xsl:text>Hi there
</xsl:text>

    <xsl:message>Low there
</xsl:message>
  </xsl:template>

</xsl:stylesheet>
