<?xml version="1.0" ?>
<!-- 
     File : test15.xsl
     Author: Keith Isdale <keithisdale@gmail.com>
     Description: stylesheet for test "test15"
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:import href="test_import.xsl"/>
  <xsl:include href="test_include_top.xsl"/>
  <xsl:strip-space elements="text()"/>
  <xsl:decimal-format name="test" decimal-separator="."/>
  <xsl:output method="text"/> 

  <!-- Test xsl:apply-templates, xsl:call-template, xsl:apply-imports -->
  <xsl:template match="/">

        <!-- test fallback -->
        <xsl:call-template name="fallback_test"/>

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

        <xsl:apply-imports/> <!-- useless but test that we can step to it -->


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
    <xsl:number value="position()" format="1."/>
  </xsl:template>

  <xsl:template name="call-template2">
    <!-- ignore any param provided -->
    <!-- test message -->
    <xsl:message terminate="no">Message here</xsl:message>
      <xsl:processing-instruction name="pitest">
        pi text
    </xsl:processing-instruction>
  </xsl:template>

  <xsl:template name="call-template3">
    <xsl:param name="item">
      <item/>
    </xsl:param>
    <!-- test comments -->
    <xsl:comment>A text comment.</xsl:comment>
    <!-- test copy and copy-of -->
    <xsl:copy>copy text</xsl:copy>
    <xsl:copy-of select="'copy-of Text'"/>
  </xsl:template>

<xsl:template name="fallback_test">
  <result xsl:version="6.1">
    <xsl:foo-of select="/Fuhrpark">
	<xsl:fallback>
          <!-- If we didn't have a xsl element here the text node would not be
               debuggable -->
          <xsl:value-of select="1"/>  
          <xsl:text>An error occoured, foo-of not known</xsl:text>
	</xsl:fallback>
      </xsl:foo-of>
    </result>
  </xsl:template>

</xsl:stylesheet>