<?xml version="1.0" ?>
<!-- 
     File : test_import.xsl     
     Author: Keith Isdale <k_isdale@tpg.com.au>
     Description: stylesheet for import testing by test "test2"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template match="fred">
        <xsl:apply-templates/>
  </xsl:template>

</xsl:stylesheet>
