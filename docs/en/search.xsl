<?xml version="1.0" ?>
<!-- 
     File : search.xsl     
     Author: Keith Isdale <k_isdale@tpg.com.au>
     Description: Stylesheet to process search.data and generate list of 
                 breakPoints, templates, callStack items
     Copyright Reserved Under GPL     
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:param name="query" select="/search/*"/>
  <xsl:param name="dosort" select="0"/>

  <xsl:strip-space elements="text() text"/>
  <xsl:output method="text"/>


  <xsl:template match="/">
    <xsl:choose>
      <xsl:when test="boolean($dosort)">
        <xsl:apply-templates select="$query">
          <xsl:sort select="@id"/>
        </xsl:apply-templates>
       </xsl:when>
       <xsl:otherwise>
         <!-- disable sorting it does't always work anyhow 
             need to fix section attriute for xsl:sort
             -->
         <xsl:apply-templates select="$query">
         </xsl:apply-templates>
       </xsl:otherwise>
  </xsl:choose>
  </xsl:template>

  <xsl:template match="breakpoint">
    <xsl:text> Breakpoint </xsl:text>
    <xsl:value-of select="@id"/>
    <xsl:choose>
      <xsl:when test="@enabled='1'">        
        <xsl:text> enabled</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text> disabled</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:if test="@template">
       <xsl:text> for template </xsl:text>
         <xsl:text>&quot;</xsl:text>
         <xsl:value-of select="@template"/>
         <xsl:text>&quot;</xsl:text>
    </xsl:if>
    <xsl:text> in file </xsl:text>
    <xsl:value-of select="@url"/>
     <xsl:text> : line </xsl:text>   
    <xsl:value-of select="@line"/>
     <xsl:text>
</xsl:text>  
  </xsl:template>

  <xsl:template match="template">
    <xsl:text> Template </xsl:text>
    <xsl:choose>
    <xsl:when test="@name">
       <xsl:text>&quot;</xsl:text>
       <xsl:value-of select="@name"/>
       <xsl:text>&quot;</xsl:text>
    </xsl:when>
    <xsl:otherwise>
       <xsl:text>&quot;</xsl:text>
       <xsl:value-of select="@match"/>
       <xsl:text>&quot;</xsl:text>
    </xsl:otherwise>
    </xsl:choose>
    <xsl:text> in file </xsl:text>
      <xsl:value-of select="@url"/>
      <xsl:text> : line </xsl:text>   
      <xsl:value-of select="@line"/>
    <xsl:text>
</xsl:text>
  </xsl:template>

  <xsl:template match="source">
      <xsl:text> Source </xsl:text>
      <xsl:value-of select="@href"/>
      <xsl:if test="@url"> imported from 
        <xsl:value-of select="@url"/>
        <xsl:text> line : </xsl:text><xsl:value-of select="@line"/>
      </xsl:if>
      <xsl:text>
</xsl:text>
  </xsl:template>

  <xsl:template match="callstack">
      <xsl:text> Callstack entry</xsl:text>
      <xsl:value-of select="@href"/>
      <xsl:text> at file </xsl:text>
         <xsl:value-of select="@url"/>
         <xsl:text> line : </xsl:text><xsl:value-of select="@line"/>
      <xsl:text> from template </xsl:text>
           <xsl:text>&quot;</xsl:text>
           <xsl:value-of select="@template"/>
           <xsl:text>&quot;</xsl:text>
      <xsl:text>
</xsl:text>
  </xsl:template>


</xsl:stylesheet>
