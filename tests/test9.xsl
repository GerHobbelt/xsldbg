<?xml version="1.0" ?>
<!-- 
     File : test9.xsl     
     Author: Keith Isdale <k_isdale@tpg.com.au>
     Description: stylesheet for test "test9" which show elements that are not
                  supported by xsldbg 
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:namespace-alias stylesheet-prefix="lxsl"
    result-prefix="xsl"/>
  <xsl:strip-space elements="text"/>
  <xsl:preserve-space elements="html"/>
  <xsl:output method="xml"/>
  <xsl:key name="key1" match="*" use="@name"/>

  <xsl:template match="/">
    <empty/>
  </xsl:template>

</xsl:stylesheet>
