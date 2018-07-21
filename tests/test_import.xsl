<?xml version="1.0" ?>
<!--
     File : test_import.xsl
     Author: Keith Isdale <keithisdale@gmail.com>
     Description: stylesheet for import testing by test "test2"
     Copyright Reserved Under GPL
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:import href="test_import_sublevel.xsl"/>

  <xsl:template match="head">
        <importedHead>
        <xsl:apply-templates/>
        </importedHead>
  </xsl:template>

</xsl:stylesheet>
