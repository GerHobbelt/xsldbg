<?xml version="1.0" ?>
<!-- 
     File : test5.xsl     
     Author: Keith Isdale <k_isdale@tpg.com.au>
     Description: stylesheet for test "test5"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <!-- Test xsl:value-of , xsl:variable, xsl:number 
       for test of xsl:param, xsl:with-param see "test2"
   -->

  <xsl:template match="/">
    <xsl:value-of select="1"/>
    
    <xsl:variable name="fred" select="man"/>

    <xsl:variable name="wilma">
      <woman/>
    </xsl:variable>
    
    <xsl:number value="1" format="1."/>

  </xsl:template>

</xsl:stylesheet>
