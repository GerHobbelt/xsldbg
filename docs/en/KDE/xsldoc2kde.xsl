<?xml version="1.0" ?>
<!--
  File : xsldoc2html.xsl
  Purpose :Convert xsldoc.xml to kde style xmlDocbook
  Author : Keith Isdale <keithisdale@gmail.com>
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="xml" doctype-public="-//KDE//DTD DocBook XML V4.5-Based Variant V1.1//EN"
    doctype-system="dtd/kdedbx45.dtd"/>
  <xsl:param name="xsldbg_version" select="'0.5.9'"/>
  <xsl:param name="use_cs2" select="0"/>
  <xsl:variable name="doc_version" select="'0.4'"/>
  <xsl:strip-space elements="title list li para entry"/>
  <xsl:include href="bookinfo.xsl"/>
  <xsl:include href="overview.xsl"/>
  <xsl:include href="credits.xsl"/>
  <xsl:include href="commands.xsl"/>

  <xsl:template match="/xsldoc">
  <book>
  <xsl:call-template name="book_info"/>
  <xsl:call-template name="overview_chapter"/>
  <xsl:call-template name="command_chapter"/>
  <xsl:call-template name="credit_chapter"/>
  </book>
  </xsl:template>

 <xsl:template match="body">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="para">
    <para>
    <xsl:apply-templates/>
    </para>
  </xsl:template>


  <xsl:template match="title">
    <title>
        <xsl:value-of select="@title"/>
    </title>
  </xsl:template>


  <xsl:template match="list">
    <itemizedlist>
    <xsl:for-each select="li">
        <listitem>
            <para>
                <xsl:apply-templates/>
            </para>
        </listitem>
    </xsl:for-each>
    </itemizedlist>
  </xsl:template>

  <xsl:template match="usage">
    <para>Usage</para>
    <itemizedlist>
    <xsl:for-each select ="li">
        <listitem>
                <para><xsl:apply-templates /></para>
        </listitem>
    </xsl:for-each>
    </itemizedlist>
  </xsl:template>


  <xsl:template match="comment">
        <emphasis>
            <xsl:value-of select="."/>
        </emphasis>

  </xsl:template>

  <xsl:template match="summary">
    <para>
    <xsl:apply-templates/>
    </para>
  </xsl:template>

</xsl:stylesheet>
