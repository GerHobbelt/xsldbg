<?xml version="1.0" ?>
<!-- 
     File : test6.xsl     
     Author: Keith Isdale <k_isdale@tpg.com.au>
     Description: stylesheet for test "test6"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <!-- Test xsl:element, xsl:attribute, xsl:attribute-set -->

    <xsl:attribute-set name="base">
      <xsl:attribute name="href">index.html</xsl:attribute>
    </xsl:attribute-set>

    <xsl:attribute-set name="empty"/>


  <xsl:template match="/">
    <xsl:element name="item" use-attribute-sets="base">
      <xsl:attribute name="id">
        <xsl:value-of select="1"/>
      </xsl:attribute>      
    </xsl:element>
    <xsl:element name="item" use-attribute-sets="empty">
      <xsl:attribute name="id">
        <xsl:value-of select="1"/>
      </xsl:attribute>      
    </xsl:element>
  </xsl:template>

</xsl:stylesheet>
