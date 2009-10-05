<?xml version="1.0" ?>
<!-- 
     File : test11.xsl     
     Author: Keith Isdale <keith@kdewebdev.org>
     Description: stylesheet for test "test11"
     This test is so tricky it will be done by hand
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="xml"/> 
  <xsl:variable name="globalVar" select="'1234'"/>
  <xsl:variable name="globalVar2" select="//item"/>

  <xsl:template match="/">
    <!-- Test globals and  set command -->
    <!-- Change value of globals variables and continue executing 
         stylesheet. And check that new values are adopted properly --> 
    <result>
        <xsl:apply-templates select="result/data">
          <xsl:with-param name="item">
            <item>Data</item>
          </xsl:with-param>
        </xsl:apply-templates>

        <!-- Test ability to step into xsl:param from xsl:apply-templates -->
        <xsl:call-template name="printExtra">
          <xsl:with-param name="item" select="'Extra'" />
          <!-- We can't change variables that don't use the select attribute
               so we should NOT be able to change the value of item2 -->
            <xsl:with-param name="item2">Extra</xsl:with-param>
          <xsl:with-param name="children" select="//extra[@type='child']" />
          <xsl:with-param name="man" select="//extra[@type='man']" />
        </xsl:call-template>

        <xsl:apply-templates select="$globalVar2"/>
      </result>
  </xsl:template>

  <xsl:template match="data">
    <xsl:param name="item" select="'default'"/>
    <xsl:value-of select="$item"/>
    <!-- ignore node content -->
  </xsl:template>

    <xsl:template match="item">
      <item>
        <xsl:apply-templates />
        </item><xsl:text>
</xsl:text>
    </xsl:template>

  <xsl:template name="printExtra">
    <xsl:param name="item"/>
    <xsl:param name="item2"/>
    <xsl:param name="man"/>
    <xsl:param name="children"/>

    <!-- Test locals and set command -->
    <!-- Change value of local variable and continue executing 
         stylesheet. And check that new values are adopted properly --> 
    <xsl:apply-templates select="$item" />
    <xsl:apply-templates select="$item2" />
    <xsl:apply-templates select="$man" />
    <xsl:apply-templates select="$children" />
    <!-- ignore node content -->
  </xsl:template>

  <xsl:template match="extra">
    <out type="{@type}" name="{@name}"/><xsl:text>
</xsl:text>
  </xsl:template>

</xsl:stylesheet>
