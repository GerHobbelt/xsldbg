<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template name="command_chapter">
    <chapter id="command_chapter">
      <title>Command summary</title>
        <xsl:apply-templates select="cmd">
          <xsl:sort name="@name"/>
        </xsl:apply-templates>
    </chapter>
  </xsl:template>

  <xsl:template match="cmd">
    <xsl:element name="section">
      <xsl:attribute name="id" >
        <xsl:value-of select="concat(@name, '_cmd')"/>
      </xsl:attribute>
      <title><xsl:value-of select="@title"/></title>
      <xsl:apply-templates />
    </xsl:element>
  </xsl:template>

</xsl:stylesheet>
