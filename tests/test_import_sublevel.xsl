<?xml version="1.0" ?>
<!--
     File : test_import_sublevel.xsl
     Author: Keith Isdale <keithisdale@gmail.com>
     Description: stylesheet for import testing by test "test2"
     Copyright Reserved Under GPL
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template match="head_sublevel">
        <importedHead_sublevel>
        <xsl:apply-templates/>
        </importedHead_sublevel>
  </xsl:template>

</xsl:stylesheet>
