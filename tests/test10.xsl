<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template match="/">
    <output>
        <xsl:call-template name="test_set_variable">
          <xsl:with-param name="item" select="'1234'"/>
        </xsl:call-template>
    </output>
  </xsl:template>

  <xsl:template name="test_set_variable">
    <xsl:param name="item" select="'default-value'"/>
    <xsl:value-of select="$item"/>
  </xsl:template>

</xsl:stylesheet>
