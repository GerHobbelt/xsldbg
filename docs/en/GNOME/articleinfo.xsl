<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:template name="article_header">
    <xsl:comment>      
  This is a GNOME documentation template, designed by the GNOME
  Documentation Project Team. Please use it for writing GNOME
  documentation, making obvious changes. In particular, all the words
  written in UPPERCASE (with the exception of GNOME) should be
  replaced. As for "legalnotice", please leave the reference
  unchanged.

  Remember that this is a guide, rather than a perfect model to follow
  slavishly. Make your manual logical and readable.  And don't forget
  to remove these comments in your final documentation!  ;-)
  </xsl:comment><xsl:text>

</xsl:text><xsl:comment>
      (Do not remove this comment block.)
  Maintained by the GNOME Documentation Project
  http://developer.gnome.org/projects/gdp
  Template version: 1.0.4
  Template last modified April 20, 2001
  </xsl:comment><xsl:text>


</xsl:text><xsl:comment> =============Document Header ============================= </xsl:comment><xsl:text>

</xsl:text>
  </xsl:template>

  <xsl:template name="article_info">
  <articleinfo>
    <title>xsldbg Manual</title>
    <copyright>
      <year>2001</year>
      <holder>Keith Isdale</holder>
    </copyright>

   <xsl:comment> translators: uncomment this:

  <copyright>
   <year>2000</year>
   <holder>ME-THE-TRANSLATOR (Latin translation)</holder>
  </copyright>

</xsl:comment><xsl:text>

</xsl:text><xsl:comment> do not put authorname in the header except in copyright - use
  section "authors" below </xsl:comment><xsl:text>


</xsl:text><xsl:comment> Use this legal notice for online documents which depend on </xsl:comment><xsl:text>
</xsl:text><xsl:comment> core GNOME packages.                                       </xsl:comment><xsl:text>
</xsl:text>
  <legalnotice id="legalnotice">
   <para>
    Permission is granted to copy, distribute and/or modify this document
    under the terms of the <ulink type="help" 
    url="gnome-help:fdl"><citetitle>GNU Free Documentation
    License</citetitle></ulink>, Version 1.1 or any later version 
    published by the Free Software Foundation with no Invariant Sections, 
    no Front-Cover Texts, and no Back-Cover Texts.  A copy of the license
    can be found <ulink type="help" url="gnome-help:fdl">here</ulink>.
   </para>
   <para>
    Many of the names used by companies to distinguish their products and
    services are claimed as trademarks. Where those names appear in any
    GNOME documentation, and those trademarks are made aware to the members
    of the GNOME Documentation Project, the names have been printed in caps
    or initial caps.
   </para>
  </legalnotice>

  <xsl:comment> Use this legal notice for documents which are placed on  </xsl:comment><xsl:text>
</xsl:text><xsl:comment> the web, shipped in any way other than online documents  </xsl:comment><xsl:text>
</xsl:text><xsl:comment> (eg. PS, PDF, or RTF), or which do not depend on the     </xsl:comment><xsl:text>
</xsl:text><xsl:comment> core GNOME distribution.                                 </xsl:comment><xsl:text>
</xsl:text><xsl:comment>                                                          </xsl:comment><xsl:text>
</xsl:text><xsl:comment> If you use this version, you must place the following    </xsl:comment><xsl:text>
</xsl:text><xsl:comment> line in the document declaration at the top of your      </xsl:comment><xsl:text>
</xsl:text><xsl:comment> document:                                                </xsl:comment><xsl:text>
</xsl:text><xsl:comment>   &lt;!ENTITY FDL SYSTEM "fdl.sgml"&gt;                        </xsl:comment><xsl:text>
</xsl:text><xsl:comment> and the following line at the bottom of your document    </xsl:comment><xsl:text>
</xsl:text><xsl:comment> after the last &lt;/sect1&gt;.                                 </xsl:comment><xsl:text>
</xsl:text><xsl:comment>   &amp;FDL;                                                  </xsl:comment><xsl:text>
</xsl:text><xsl:comment>
  <legalnotice id="legalnotice">
   <para>
    Permission is granted to copy, distribute and/or modify this document
    under the terms of the <link linkend="fdl"><citetitle>GNU
    Free Documentation License</citetitle></link>, Version 1.1 or any later
    version published by the Free Software Foundation with no Invariant
    Sections, no Front-Cover Texts, and no Back-Cover Texts.  A copy of the
   license can be found in <xref linkend="fdl" />.
   </para>
   <para>
    Many of the names used by companies to distinguish their products and
    services are claimed as trademarks. Where those names appear in any
    GNOME documentation, and those trademarks are made aware to the members
    of the GNOME Documentation Project, the names have been printed in caps
    or initial caps.
   </para>
  </legalnotice>
  </xsl:comment>


    <releaseinfo>
      This manual describes version <xsl:value-of select="$xsldbg_version"/> of xsldbg. It was
       last updated on <xsl:value-of select="$xsldbg_date"/>.
    </releaseinfo>

  </articleinfo>
  </xsl:template>


</xsl:stylesheet>
