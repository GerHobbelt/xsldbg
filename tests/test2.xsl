<?xml version="1.0" ?>
<!-- 
     File : test2.xsl     
     Author: Keith Isdale <k_isdale@tpg.com.au>
     Description: stylesheet for test "test2"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:strip-space elements="text()"/>
  <xsl:output method="text"/> 

  <!-- Test apply templates and call template -->
  <xsl:template match="/">
        <!-- Basic xsl:apply-templates, xsl:call-template usage -->
        <!-- Test basic usage of xsl:apply-templates -->
        <xsl:apply-templates select="result/data"/>
        <!-- Test basic usage of xsl:call-template -->
        <xsl:call-template name="call-template1"/>

        <!-- Test xsl:apply-templates with parameter value. 
           Test the ability to step into a xsl:with-param child -->
        <xsl:apply-templates select="result/data">
          <xsl:with-param name="item">
            <item/>
          </xsl:with-param>
        </xsl:apply-templates>

        <!-- Test xsl:call-template with parameter value
             Test the ability to step into a xsl:with-param child -->
        <xsl:call-template name="call-template2">
          <xsl:with-param name="item">
            <item />
          </xsl:with-param>
        </xsl:call-template>

        <!-- Test ability to step into xsl:param from xsl:apply-templates -->
        <xsl:apply-templates select="result/extra" />

        <!-- Test ability to step into xsl:param from xsl:call-template -->
        <xsl:call-template name="call-template3" />        

        <!-- Test ability to step into xsl:sort from xsl:apply-templates -->
        <xsl:apply-templates select="result/data">
          <xsl:sort select="."/>
        </xsl:apply-templates>

  </xsl:template>

  <xsl:template match="result">
    <xsl:param name="item" select="'default'"/>
    <!-- ignore node content -->
  </xsl:template>


  <xsl:template match="data">
    <!-- ignore node content -->
  </xsl:template>


  <xsl:template match="extra">
    <xsl:param name="item">
      <item/>
      </xsl:param>
    <!-- ignore node content -->
  </xsl:template>


  <xsl:template name="call-template1">
    <!-- ignore this node content -->
  </xsl:template>

  <xsl:template name="call-template2">
    <!-- ignore any param provided -->
    <!-- ignore node content -->
  </xsl:template>

  <xsl:template name="call-template3">
    <xsl:param name="item">
      <item/>
    </xsl:param>
    <!-- ignore node content -->
  </xsl:template>

</xsl:stylesheet>
