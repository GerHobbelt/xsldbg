<?xml version="1.0" ?>
<!--
     File : test14.xsl
     Author: Keith Isdale <keithisdale@gmail.com>
     Description: stylesheet for test "test14"
     Copyright Reserved Under GPL
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template match="/">
     <xsl:apply-templates/>
  </xsl:template>

    <xsl:template match="note">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="to">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="from">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="body">
        <xsl:apply-templates/>
    </xsl:template>

</xsl:stylesheet>
