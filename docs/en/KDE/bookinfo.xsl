<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template name="book_info">
    <bookinfo>
      <title>Xsldbg <xsl:value-of select="$xsldbg_version"/></title>
      <subtitle>Command and usage documentation</subtitle>
      <authorgroup>
        <author>   
          <firstname>Keith</firstname>
          <surname>Isdale</surname>
          <affiliation>
            <address><email>k_isdale@tpg.com.au</email></address>
          </affiliation>
        </author>
      </authorgroup>

      <!-- TRANS:ROLES_OF_TRANSLATORS -->

      <copyright>
        <year>2001</year>
        <holder>Keith Isdale</holder>
      </copyright>

    <!-- Translators: put here the copyright notice of the translation -->
    <!-- Put here the FDL notice.  Read the explanation in fdl-notice.docbook 
         and in the FDL itself on how to use it. -->

    <xsl:element name="legalnotice"><xsl:text disable-output-escaping="yes">&amp;FDLNotice;</xsl:text>
    </xsl:element>

    <!-- Date and version information of the documentation
         Don't forget to include this last date and this last revision number, we
         need them for translation coordination !
         Please respect the format of the date (DD/MM/YYYY) and of the version
         (V.MM.LL), it could be used by automation scripts.
         Do NOT change these in the translation. -->
    
      <date>23/09/2001</date>
      <releaseinfo>0.2.0</releaseinfo>
      <abstract>
        <para>Blagh</para>
      </abstract>
    </bookinfo>
  </xsl:template>


</xsl:stylesheet>
