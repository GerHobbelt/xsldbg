<?xml version="1.0" ?>
<!-- 
     File : test_include.xsl     
     Author: Keith Isdale <keith@kdewebdev.org>
     Description: stylesheet for include testing by test "test2"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template match="include_bot">
        <xsl:apply-templates/>
  </xsl:template>

</xsl:stylesheet>
