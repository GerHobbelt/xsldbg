<?xml version="1.0" ?>
<!-- 
     File : test13.xsl     
     Author: Keith Isdale <k_isdale@tpg.com.au>
     Description: stylesheet for test "test13"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:import href="subdir/test14.xsl" />

  <!-- Test the guesing stylesheet and XML file name -->
  <xsl:template match="/">
    <xsl:call-template name="fred" />
  </xsl:template>

</xsl:stylesheet>
