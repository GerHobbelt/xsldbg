<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- 
     File : test12.xsl     
     Author: Keith Isdale <k_isdale@tpg.com.au>
     Description: stylesheet for test "test12"
     This is uncertain as to whether it will be testable under WIN32
       since the DOS prompt, in my opinion, only supports ASCI properly
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <!-- A bit corny but test out the ability to use encodings to
       standard output -->

  <xsl:output method="xml" encoding="ISO-8859-1" />
    <xsl:variable name="lèglobal" select="'lèglobal'" />

      <!-- First test
           Run stylesheet and see if correct characters are printed
           Use "templates" command and check the template names
           Set a breakpoint at line 39 and use the globlals and locals commands
           -->
  <xsl:template match="/">
    <xsl:apply-templates select="root">
          <xsl:with-param name="lèlocal" select="'lèlocal#2'" />
          <xsl:with-param name="lelocal" select="'lelocal#2'" />
        </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="très">
    <xsl:param name="lèlocal" select="'lèlocal#1'" />
    <xsl:apply-templates />
  </xsl:template>

  <xsl:template match="tres">
    <xsl:param name="lèlocal" select="'lèlocal#1'" />
      <xsl:param name="lelocal" select="'lelocal#1'" />
    <xsl:apply-templates />
  </xsl:template>

</xsl:stylesheet>
