<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- 
     File : test12.xsl     
     Author: Keith Isdale <keithisdale@gmail.com>
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
    <xsl:variable name="lèglobal1" select="/root" />
    <xsl:variable name="lèglobal2" select="1 = 1" />
    <xsl:variable name="lèglobal3" select="12 * 2" />
   <xsl:variable name="xsl:myvar" select="'2'" />

      <!-- First test
           Run stylesheet and see if correct characters are printed
           Use "templates" command and check the template names
           Set a breakpoint at line 39 and use the globlals and locals commands
           -->
  <xsl:template match="/">
    <!-- We should not get values into tres the first times its called-->
    <xsl:apply-templates select="root">
          <xsl:with-param name="lèlocal1" select="'no_value'" />
          <xsl:with-param name="lelocal2" select="'no_value'" />
          <xsl:with-param name="lelocal3" select="'no_value'" />
    </xsl:apply-templates>


    <xsl:apply-templates select="root/tres">
          <xsl:with-param name="lèlocal1" select="'lèlocal#1'" />
          <xsl:with-param name="lelocal2" select="'lelocal#2'" />
          <xsl:with-param name="lelocal3" select="'lelocal#3'" />
          <xsl:variable name="xsl:myvar2" select="'2'" />
    </xsl:apply-templates>
    
  </xsl:template>


  <xsl:template match="très">
    <xsl:param name="lèlocal1" select="'lèlocal__1'" />
    <xsl:apply-templates />
  </xsl:template>


  <xsl:template match="tres">
    <xsl:param name="lèlocal" select="'lèlocal#1'" />
    <xsl:apply-templates />
  </xsl:template>

</xsl:stylesheet>
