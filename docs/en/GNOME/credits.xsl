<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template match="credit_chapter" name="credit_chapter">
    <chapter id="credits_chapter">
      <title>Credits</title>

      <para></para>
      <xsl:text disable-output-escaping="yes">&amp;underFDL;</xsl:text> <!-- FDL: do not remove -->

      <section>
        <title>Thanks to </title>
        <para> The writers the libxml and libxsl.</para>
      </section>

    </chapter>
  </xsl:template>

</xsl:stylesheet>
