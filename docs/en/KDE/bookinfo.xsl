<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template name="book_info">
    <bookinfo>
      <title>Xsldbg <xsl:value-of select="$xsldbg_version"/> Command and usage documentation</title>
      <authorgroup>
        <author>
          <firstname>Keith</firstname>
          <surname>Isdale</surname>
          <affiliation>
            <address><email>keithisdale@gmail.com</email></address>
          </affiliation>
        </author>
      </authorgroup>

      <!-- TRANS:ROLES_OF_TRANSLATORS -->

      <copyright>
        <year>2002-</year>
        <holder>Keith Isdale</holder>
      </copyright>

    <!-- Translators: put here the copyright notice of the translation -->
    <!-- Put here the FDL notice.  Read the explanation in fdl-notice.docbook
         and in the FDL itself on how to use it. -->

    <!-- <xsl:element name="legalnotice"><xsl:text disable-output-escaping="yes">&amp;FDLNotice;</xsl:text>
    </xsl:element>
    -->

    <!-- Date and version information of the documentation
         Don't forget to include this last date and this last revision number, we
         need them for translation coordination !
         Please respect the format of the date (DD/MM/YYYY) and of the version
         (V.MM.LL), it could be used by automation scripts.
         Do NOT change these in the translation. -->

      <date>2018-09-10</date>
      <releaseinfo><xsl:value-of select="$xsldbg_version"/></releaseinfo>
      <abstract>
        <para>
xsldbg is a tool intended to help understand stylesheets.
What makes it different to other stylesheet debuggers is the ability
to search for items of interest and trace stylesheet execution.
	</para>
	<para>
A good deal of effort has been put into</para>
<itemizedlist>
    <listitem>creating a <ulink url="http://www.xemacs.org/">XEmacs</ulink> front end
    called <ulink url="https://sourceforge.net/projects/xsldbg/files/xsldbg-mode/">xsldbg-mode</ulink>.
    </listitem>
    <listitem>creating a Qt5 front end  called <ulink url="https://sourceforge.net/projects/xsldbg/files/qxsldbg/">qxsldbg</ulink>.
    </listitem>
    <listitem>creating a kate plugin(KF5/Qt5) called <ulink url="https://sourceforge.net/projects/xsldbg/files/kxsldbg/">kxsldbg</ulink>.
    </listitem>
</itemizedlist>
      </abstract>
      <keywordset>
        <keyword>debugger</keyword>
         <keyword>XSL</keyword>
      </keywordset>
    </bookinfo>
  </xsl:template>


</xsl:stylesheet>
