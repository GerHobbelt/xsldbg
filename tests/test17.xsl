<?xml version="1.0" ?>
<!-- 
     File : test17.xsl
     Author: Keith Isdale <keithisdale@gmail.com>
     Description: stylesheet for test "test17"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <!-- add some instructions so that stepping is required to reach / template -->
  <xsl:strip-space elements="text()"/>
  <xsl:decimal-format name="test" decimal-separator="."/>
  <xsl:output method="text"/>

  <xsl:template match="/">
    <!-- ignore node content -->
  </xsl:template>

</xsl:stylesheet>
