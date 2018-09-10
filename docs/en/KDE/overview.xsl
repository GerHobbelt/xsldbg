<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template name="overview_chapter">
    <chapter id="overview_chapter">
      <title>Overview</title>
      <xsl:for-each select="/xsldoc/overview/body">
          <xsl:element name="section">
            <xsl:attribute name="id" >
                <xsl:value-of select="@id"/>
            </xsl:attribute>
            <xsl:apply-templates/>
          </xsl:element>
      </xsl:for-each>
    </chapter>
  </xsl:template>

</xsl:stylesheet>
