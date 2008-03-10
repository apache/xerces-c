<?xml version="1.0" encoding="ISO-8859-1" ?>

<!DOCTYPE xsl:stylesheet>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		xmlns:external="http://xml.apache.org/xalan-p/external_function"
		exclude-result-prefixes="external"
		version="1.0">

  <xsl:strip-space elements="*"/>
  <xsl:output method="text"/>

  <!-- only purpose is so we can die early if wrap() isn't available -->
<!--
  <xsl:template match="/">  
    <xsl:if test="not(function-available('external:wrap'))">
      <xsl:message terminate="yes">
	Function external:wrap() is not available!
      </xsl:message>
    </xsl:if>
    <xsl:apply-templates/>
  </xsl:template>
-->

  <xsl:template match="s1">
    <xsl:text>============================================================</xsl:text>
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:value-of select="@title"/>
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:text>============================================================</xsl:text>
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:text>        $Id: readme.xsl,v 1.1 2002/05/07 06:06:10 jasons Exp $</xsl:text>
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:text>
        LEGAL HOOP JUMPING:
        ===================
        This code is distributed under the terms of the Apache Software
        License, Version 2.0. See the file LICENSE for details</xsl:text>
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="s2">
    <xsl:number format="1) "/>
    <xsl:value-of select="@title"/>
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:text>========================</xsl:text>
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="s3">
    <xsl:text>    </xsl:text>
    <xsl:number level="multiple" format="1.1) " count="s2|s3"/>
    <xsl:value-of select="@title"/>
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:text>    </xsl:text>
    <xsl:text>------------------------</xsl:text>
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="note">
    <xsl:variable name="text">
      <xsl:apply-templates/>
    </xsl:variable>

    <xsl:text>&#09;</xsl:text><!-- a tab -->
    <xsl:text>#### NOTE: ####</xsl:text>
    <xsl:text>&#10;</xsl:text><!-- a newline -->

<!--    <xsl:value-of select="external:wrap(8,8,normalize-space($text))"/> -->
    <xsl:value-of select="normalize-space($text)"/>

    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:text>&#09;</xsl:text><!-- a tab -->
    <xsl:text>###############</xsl:text>
    <xsl:text>&#10;</xsl:text><!-- a newline -->
    <xsl:text>&#10;</xsl:text><!-- a newline -->
  </xsl:template>

  <!-- we include the external URL after the text -->
  <xsl:template match="jump">
    <xsl:value-of select="."/>
    <xsl:text> &lt;URL: </xsl:text>
    <xsl:value-of select="@href"/>
    <xsl:text>&gt; </xsl:text>
  </xsl:template>

  <xsl:template match="ul/li | ol/li">
    <xsl:variable name="text">
      <xsl:apply-templates/>
    </xsl:variable>

    <xsl:variable name="bullet">
      <xsl:choose>
	<xsl:when test="parent::ol">
	  <xsl:number format="A) "/>
	</xsl:when>
	<xsl:otherwise>
	  <xsl:text>* </xsl:text>
	</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="indent">
      <xsl:choose>
	<xsl:when test="ancestor::*[2][self::ol] or ancestor::*[2][self::ul]">
	  <xsl:number value="8"/>
	</xsl:when>
	<xsl:otherwise>
	  <xsl:number value="4"/>
	</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!-- create the indent -->
    <xsl:call-template name="x">
      <xsl:with-param name="count" select="$indent"/>
      <xsl:with-param name="string" select="string(' ')"/>
    </xsl:call-template>
    <xsl:value-of select="$bullet"/>

<!--    <xsl:value-of select="external:wrap(0,4+$indent,normalize-space($text))"/> -->
    <xsl:value-of select="normalize-space($text)"/>

    <xsl:text>&#10;</xsl:text><!-- a newline -->
  </xsl:template>

  <!--    only match paragraphs without anchors -->
  <xsl:template match="p[not(anchor)]">

    <xsl:variable name="text">
      <xsl:apply-templates/>
    </xsl:variable>

    <xsl:variable name="first">
      <xsl:choose>
	<xsl:when test="parent::s2">
	  <xsl:number value="0"/>
	</xsl:when>
	<xsl:otherwise>
	  <xsl:number value="4"/>
	</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="subsequent">
      <xsl:choose>
	<xsl:when test="parent::s2">
	  <xsl:number value="0"/>
	</xsl:when>
	<xsl:otherwise>
	  <xsl:number value="4"/>
	</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

<!--    <xsl:value-of select="external:wrap($first,
		                        $subsequent,
		                        normalize-space($text))"/> -->
    <xsl:value-of select="normalize-space($text)"/>

    <xsl:text >&#10;</xsl:text><!-- a newline -->
    <xsl:text >&#10;</xsl:text><!-- a newline -->
  </xsl:template>

  <xsl:template match="ul|ol">
    <xsl:apply-templates/>
    <xsl:text >&#10;</xsl:text><!-- a newline -->
  </xsl:template>

  <xsl:template match="source">
    <xsl:apply-templates/>
    <xsl:text >&#10;</xsl:text><!-- a newline -->
  </xsl:template>

  <xsl:template match="em">
    <xsl:text>*</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>*</xsl:text>
  </xsl:template>

  <!-- named after the Perl operator 'x' -->
  <xsl:template name="x">
    <xsl:param name="count"/>
    <xsl:param name="string"/>
    <xsl:choose>
      <xsl:when test="$count &gt; 0">
	<xsl:variable name="rest">
	  <xsl:call-template name="x">
	    <xsl:with-param name="count" select="$count - 1"/>
	    <xsl:with-param name="string" select="$string"/>
	  </xsl:call-template>
	</xsl:variable>
	<xsl:value-of select="concat($string, $rest)"/>
      </xsl:when>
      <xsl:otherwise>
	<xsl:text/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
