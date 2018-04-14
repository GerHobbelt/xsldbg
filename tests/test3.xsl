<?xml version="1.0" ?>
<!-- 
     File : test3.xsl     
     Author: Keith Isdale <keithisdale@gmail.com>
     Description: stylesheet for test "test3"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:strip-space elements="text()"/>
  <xsl:output method="text"/> 

  <!-- Test xsl:choose and xsl:if -->
  <xsl:template match="/">

    <!-- Test xsl:if -->
    <xsl:if test="1=1">
      <item/>
    </xsl:if>

    <!-- Test basic usage of xsl:choose -->    
    <xsl:choose>
      <xsl:when test="1=1">
        <item/>        
      </xsl:when>
    </xsl:choose>
    
    <!-- Test xsl:otherwise child in xsl:choose -->  
    <xsl:choose>
      <xsl:when test="1=2">
        <item/>        
      </xsl:when>

      <xsl:otherwise>
        <item/>
      </xsl:otherwise>
    </xsl:choose>

  <!-- Test for-each -->
  <xsl:for-each select="result/data">
    <xsl:sort select="."/>
    <item/>
  </xsl:for-each>

  </xsl:template>


</xsl:stylesheet>
