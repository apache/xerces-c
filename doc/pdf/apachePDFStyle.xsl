<!-- Stylebook XML to XSL:FO -->
<!-- Author: Jordan Naftolin -->

<xsl:stylesheet 
	version="1.0" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    	xmlns="http://www.w3.org/TR/xhtml1/strict"
    	xmlns:fo="http://www.w3.org/1999/XSL/Format">               
    	
    	<xsl:param name="textColor">black</xsl:param>
    	<xsl:param name="titleColor">black</xsl:param>	
    	<xsl:param name="lineHeight">16</xsl:param>
    	

<!-- ************************************************ -->   	
<!-- 			Page Margins 		      --> 
<!-- ************************************************ --> 		
			
	<xsl:template match="book">
	        <fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format">		                
	                <fo:layout-master-set>
	
<!-- title page margins-->
	                        <fo:simple-page-master 
	                        	master-name="page-titlePage"
	                                page-height="29.7cm" 
	                                page-width="21cm"
	                                margin-top="10cm" 
	                                margin-bottom="1cm" 
	                                margin-left="2cm" 
	                                margin-right="2cm">
	                                	<fo:region-before extent="1cm"/>
	                                	<fo:region-body margin-top="1cm" margin-bottom="2cm"/>
	                                	<fo:region-after extent="1.5cm"/>
	                        </fo:simple-page-master>
	                
<!-- first page of chapter margins-->	                  
	                        <fo:simple-page-master 
	                        	master-name="page-chapterStartPage"
	                                page-height="29.7cm" 
	                                page-width="21cm"
	                                margin-top="1cm" 
	                                margin-bottom="1cm" 
	                                margin-left="1.5cm" 
	                                margin-right="1.5cm">
	                                	<fo:region-before extent="0cm"/>
	                                	<fo:region-body margin-top="3.2cm" margin-bottom="1.2cm"/>
	                                	<fo:region-after extent="1cm"/>
	                        </fo:simple-page-master>

<!-- basic page margins -->	                        	                   
	                        <fo:simple-page-master 
	                        	master-name="page-basicPage"
	                                page-height="29.7cm" 
	                                page-width="21cm"
	                                margin-top="1cm" 
	                                margin-bottom="1cm" 
	                                margin-left="1.5cm" 
	                                margin-right="1.5cm">
	                                	<fo:region-before extent="1cm"/>
	                                	<fo:region-body margin-top="1.2cm" margin-bottom="1.2cm"/>
	                                	<fo:region-after extent="1cm"/>
	                        </fo:simple-page-master>
	                        
	                        <fo:page-sequence-master master-name="sequence-titlePage">
	                        	<fo:single-page-master-reference master-name="page-titlePage"/>
	                        </fo:page-sequence-master>
	                        
	                        <fo:page-sequence-master master-name="sequence-TOC">
		                        <fo:repeatable-page-master-reference		                        			                        		
		                        		master-name="page-basicPage"/>		                        
	                        </fo:page-sequence-master>
	                        
	                        <fo:page-sequence-master master-name="sequence-chapter">
		                        <fo:repeatable-page-master-alternatives>
		                        	<fo:conditional-page-master-reference 
		                        		page-position="first" 
		                        		master-name="page-chapterStartPage"/>
		                        	<fo:conditional-page-master-reference 
		                        		page-position="rest" 
		                        		master-name="page-basicPage"/>
		                        </fo:repeatable-page-master-alternatives>	                        
	                        </fo:page-sequence-master>
	                        	                       	                                                	                        
	        
	                </fo:layout-master-set>
	                
	        
<!-- create title page -->
		        <xsl:call-template name="createTitlePage"> 
		        	<xsl:with-param name="documentTitle" select="@title"/>			        
		        </xsl:call-template>
		        
<!-- create table of contents -->
		        <xsl:apply-templates select="toc">
		        	<xsl:with-param name="documentTitle" select="@title"/>			        
		        </xsl:apply-templates>		        
		        		        		        
<!-- create chapters -->	
		        <xsl:apply-templates select="chapter">
		        	<xsl:with-param name="documentTitle" select="@title"/>			        
		        </xsl:apply-templates>		        		        
		        
<!-- create appendices -->	
		        <xsl:apply-templates select="appendix">
		        	<xsl:with-param name="documentTitle" select="@title"/>			        
		        </xsl:apply-templates>		        		        		        
	        
	        </fo:root>                      
	</xsl:template>			


<!-- ************************************************ -->   	
<!-- 			Title Page 		      --> 
<!-- ************************************************ --> 

	<xsl:template name="createTitlePage">
		<xsl:param name="documentTitle"/>
		<fo:page-sequence master-name="sequence-titlePage">                      		        
		                                                        
<!-- TitlePage header -->
		        <!--
		        <fo:static-content flow-name="xsl-before">		                
		        </fo:static-content> 
		        -->

<!-- TitlePage footer -->
		        <fo:static-content flow-name="xsl-after">

<!-- Copyright info -->
		                <fo:block 
		                        text-align="centered" 
		                        font-size="14pt" 
		                        font-family="serif" 
		                        line-height="14pt" 
		                        color="black">                                                                         
		                        	Copyright <xsl:value-of select="@copyright"/>. All Rights Reserved.                                                 
		                </fo:block>
		        </fo:static-content> 
		

<!-- TitlePage body -->
		        <fo:flow flow-name="xsl-body">
		                          
<!-- Title -->                                                    
		                <fo:block
		                        text-align="centered" 
		                        font-size="26pt"
		                        color="{$titleColor}" 
		                        font-family="serif" 
		                        line-height="28pt">                                        
		                        	<xsl:value-of select="$documentTitle"/>                                                                                
			        </fo:block>                                        
		        </fo:flow>        	
	        </fo:page-sequence>
	</xsl:template>    
	
<!-- ************************************************ -->   	
<!-- 			Table of Contents   	      --> 
<!-- ************************************************ --> 		  	
 
    	<!-- toc -->    	
	<xsl:template match="toc">	
		<xsl:param name="documentTitle"/>				
		<fo:page-sequence master-name="sequence-TOC" initial-page-number="auto">                                                                
        
<!-- TOC header -->
                        <fo:static-content flow-name="xsl-before">
                                <fo:block 
                                        text-align="end" 
                                        font-size="12pt" 
                                        font-family="serif" 
                                        line-height="14pt" >
                                                <xsl:value-of select="$documentTitle"/>
                                                <fo:display-rule/>                                                                                             
                                </fo:block>
                        </fo:static-content> 
                        
<!-- TOC footer -->     
			<!--                   
			<fo:static-content flow-name="xsl-after">                        
                        </fo:static-content> 
                        -->
                        
<!-- TOC body -->                                
                        <fo:flow flow-name="xsl-body">                               
	
<!-- TOC Title -->
			        <fo:block 
                                        text-align="start" 
                                        font-size="16pt" 
                                        font-family="serif"                                        
                                        color="{$titleColor}"
                                        padding-top="2pt"
                                        space-before.optimum="4pt" 
                                        space-after.optimum="5pt"
                                        line-height="18pt" >
                                                Table of Contents
                               </fo:block>                                

<!-- create tocChapter entries -->                               
                               <xsl:apply-templates select="tocChapter"/>

<!-- create tocAppendix entries -->                               		
                               <xsl:apply-templates select="tocAppendix"/>                                                           		
                                                              
                        </fo:flow>       
                </fo:page-sequence>                                                            
	</xsl:template>


<!-- ************************************************ -->

	<!-- tocChapter -->
	<xsl:template match="tocChapter">        
		<xsl:variable name = "chapterNumber" select="@chapterNumber"/>          
		<xsl:variable name = "chapterTitle" select="@chapterTitle"/>
		
<!-- chapter number and title -->		               
	        <fo:block
	                font-size="14pt"                
	                line-height="18pt"
	                font-weight="bold"
	                space-before.optimum="4pt">
	                	        	          
	        <xsl:value-of select="$chapterNumber"/>. 
	        <fo:inline-sequence color="blue">
	        	<fo:simple-link internal-destination="Chapter{$chapterNumber}">        
	        		<xsl:value-of select="$chapterTitle"/>                
	        	</fo:simple-link>
	        </fo:inline-sequence>
	        
<!-- space / dots -->	        
	        <fo:inline-sequence font-size="10pt" white-space-treatment="preserve">        	     
	        	<xsl:text>   </xsl:text>        	        	
<!-- page number -->	        	
	        	<fo:page-number-citation ref-id="Chapter{$chapterNumber}"/>
	        </fo:inline-sequence>        
	        
<!-- create tocSections -->	        
	        <fo:list-block provisional-distance-between-starts="10pt">        
	            <xsl:apply-templates select="tocSection1"/>	            	
	        </fo:list-block>
	        </fo:block>  	        
	</xsl:template>


<!-- ************************************************ -->
                    
	<!-- tocAppendix -->	
	<xsl:template match="tocAppendix">		
	        <xsl:variable name = "appendixLetter" select="@appendixLetter"/>
	        <xsl:variable name = "appendixTitle" select="@appendixTitle"/>
	        
<!-- Appendix title -->	          
	        <fo:block
	        	font-size="14pt"                
	                line-height="18pt"
	                font-weight="bold"
	                space-before.optimum="4pt">
	        	
	        	<fo:inline-sequence color="blue">
	        		<fo:simple-link internal-destination="Appendix#{$appendixLetter}">        
	        			Appendix <xsl:value-of select="$appendixLetter"/>: <xsl:value-of select="$appendixTitle"/>
	        		</fo:simple-link>
	        	</fo:inline-sequence> 
	        	
<!-- page number-->	        
		        <fo:inline-sequence font-size="10pt" white-space-treatment="preserve">        	     
				<xsl:text>   </xsl:text>        	        	
				<fo:page-number-citation ref-id="Appendix#{$appendixLetter}"/>
			</fo:inline-sequence>                         
	        	
<!-- create tocSections -->	        	
	        	<fo:list-block provisional-distance-between-starts="10pt">
	            		<xsl:apply-templates select="tocSection1"/>
	        	</fo:list-block>
	        </fo:block>                        
	</xsl:template>


<!-- ************************************************ -->

	<!-- tocSection1 -->	
	<xsl:template match="tocSection1">			       	
	        <fo:list-item>
	        	<fo:list-item-label>
	        		<fo:block/>
	        	</fo:list-item-label>
	        	<fo:list-item-body>
<!-- toc Section -->	        	
	            		<fo:block
	                		font-size="12pt"                
	                		line-height="14pt"
	                		font-weight="normal"
	                		space-before.optimum="2pt"
	                		start-indent="20pt">     
	            			
	            			<fo:inline-sequence color="blue"> 
	            				<fo:simple-link internal-destination="{@link}">
	            					<xsl:value-of select="@sectionTitle"/>
	            				</fo:simple-link>
	            			</fo:inline-sequence>  
	            			
<!-- page number-->	        
				        <fo:inline-sequence font-size="10pt" white-space-treatment="preserve">        	     
	        				<xsl:text>   </xsl:text>        	        	
	        				<fo:page-number-citation ref-id="{@link}"/>
	        			</fo:inline-sequence>                  
	            		</fo:block>                       
	        	</fo:list-item-body>
	        </fo:list-item>        
	</xsl:template>

<!-- ************************************************ -->   	
<!-- 			Chapter 		      --> 
<!-- ************************************************ --> 		

	<xsl:template match="chapter">
		<xsl:param name="documentTitle"/>
		<fo:page-sequence master-name="sequence-chapter">                                  
                        
                        <!-- page-master-repeating="basicPage"/  -->
                        
        
<!-- chapter header -->
                        <fo:static-content flow-name="xsl-before">
                        	<fo:block>
                                	<fo:table>
                                        	<fo:table-column column-width="9cm" />                
                                        	<fo:table-column column-width="9cm" />
	                                        <fo:table-body>                            
		                                        <fo:table-row>                                                       
		                                        	<fo:table-cell>
<!-- chapter title -->		                                        	
		                                        		<fo:block
		                                        			text-align="start"
		                                        			font-size="12pt" 
		                                        			font-family="serif" 
		                                        			line-height="14pt"
		                                        			white-space-treatment="preserve">
		                                        		<fo:inline-sequence font-weight="bold">Chapter <xsl:value-of select="@chapterNumber"/></fo:inline-sequence><xsl:text> - </xsl:text><xsl:value-of select="@chapterTitle"/></fo:block>   
		                                        	</fo:table-cell>
		                                        	<fo:table-cell>
<!-- document title -->		      
		                                        		<fo:block
		                                        			text-align="end"
		                                        			font-size="12pt" 
		                                        			font-family="serif" 
		                                        			line-height="14pt">
				                	                        <xsl:value-of select="$documentTitle"/>
		                		                        </fo:block>   
		                                        	</fo:table-cell>
		                                        </fo:table-row>                 
	                                        </fo:table-body>
                                       	</fo:table>  
<!-- dividing line -->                                       	                                              
                                        <fo:display-rule/>        
				</fo:block>                                                                                                                        
                        </fo:static-content> 
                        
<!-- chapter footer -->
                        <fo:static-content flow-name="xsl-after">
<!-- page number -->
                                <fo:block 
                                        text-align="centered" 
                                        font-size="10pt" 
                                        font-family="serif"
                                        line-height="10pt">                                                
                                                <fo:block space-before.optimum="3pt">
                                                 -<fo:page-number/>-
                                                 </fo:block>                                         
                                </fo:block>
                        </fo:static-content> 
        
<!-- chapter body -->
                        <fo:flow flow-name="xsl-body">
                        <xsl:variable name = "chapterNumber" select="@chapterNumber"/>                                  			
			<fo:block id="Chapter{$chapterNumber}"/>
			
<!-- chapter number and title -->			
                            <fo:block                             	
                                text-align="end"
                                font-family="serif"
                                space-before.optimum="50pt"
                                space-after.optimum="30pt"
                                start-indent="20pt"
                                color="{$titleColor}"                                
                                font-size="55pt">
                                    <xsl:value-of select="@chapterNumber"/> 
                                    <fo:block space-before.optimum="20pt" font-size="24pt">
                                        <xsl:value-of select="@chapterTitle"/>
                                    </fo:block>
                                </fo:block>                                                                
			        <xsl:apply-templates/>                                                                >
                        </fo:flow>                        
                </fo:page-sequence>		
	</xsl:template>    		

<!-- ************************************************ -->   	
<!-- 			Headers 		      --> 
<!-- ************************************************ --> 			

<!-- ************************************************ -->   	
    	
	<!-- s1 title -->
	<xsl:template match="s1">
	        <xsl:variable name="id" select="@id"/>          
	        
	        <fo:block 
	        	id="{$id}"
	                text-align="start" 
	                font-size="20pt" 
	                font-weight="bold"
	                font-family="serif"                
	                color="{$titleColor}"                
	                space-before.optimum="4pt" 
	                space-after.optimum="5pt"
	                break-before="page"
	                line-height="22pt" >
	                        <xsl:value-of select="@title"/>
	         </fo:block>      
	         <xsl:apply-templates/>                
	</xsl:template>

<!-- ************************************************ -->   	

	<!-- s2 title -->
	<xsl:template match="s2">
	        <xsl:variable name="id" select="@id"/>
		
	        <fo:block
	        	id="{$id}" 
	                text-align="start" 
	                font-size="14pt" 
	                font-family="sans-serif"
	                font-weight="bold"                
	                color="{$titleColor}"
	                padding-top="2pt"
	                space-before.optimum="10pt"                 
	                line-height="16pt" >
	                        <xsl:value-of select="@title"/>
	         </fo:block>      
	         <xsl:apply-templates/>                
	</xsl:template>

<!-- ************************************************ -->   	

	<!-- s3 title -->
	<xsl:template match="s3">
	        <fo:block 
	                text-align="start" 
	                font-size="12pt" 
	                font-family="serif"
	                font-weight="bold"                
	                color="{$titleColor}"
	                padding-top="2pt"
	                space-before.optimum="10pt"                 
	                line-height="14pt" >
	                        <xsl:value-of select="@title"/>
	         </fo:block>      
	         <xsl:apply-templates/>                
	</xsl:template>

<!-- ************************************************ -->   		
	
	<!-- s4 title -->	
	<xsl:template match="s4">
	        <fo:block 
	                text-align="start" 
	                font-size="12pt" 
	                font-family="serif" 
	                text-decoration="underline"                               
	                background-color="transparent"
	                color="{$titleColor}"
	                padding-top="2pt"
	                space-before.optimum="10pt"                 
	                line-height="14pt" >
	                        <xsl:value-of select="@title"/>
	         </fo:block>      
	         <xsl:apply-templates/>                
	</xsl:template>

<!-- ************************************************ -->   	
<!-- 			Tables  		      --> 
<!-- ************************************************ --> 		   	

	<!-- table -->
	<xsl:template match="table">
	        <fo:table space-before.optimum="6pt" space-after.optimum="6pt" text-align="start">                                                
	                        <xsl:apply-templates/>                
	        </fo:table>
	</xsl:template>


<!-- ************************************************ -->   	

	<!-- table column declaration -->	
	<xsl:template match="table/column">
		<xsl:variable name = "width" select="@width"/>	        	                  
	        <fo:table-column column-width="{$width}pt" />
	</xsl:template>


<!-- ************************************************ -->   	

	<!-- table content -->
	<xsl:template match="tableContent">
	        <fo:table-body>
	                <xsl:apply-templates/>
	        </fo:table-body>
	</xsl:template>

<!-- ************************************************ -->   	

	<!-- row -->
	<xsl:template match="tr">
	        <fo:table-row>
	                <xsl:apply-templates/>
	        </fo:table-row>
	</xsl:template>


<!-- ************************************************ -->   	

	<!-- cell -->
	<xsl:template match="td|th|tn">
	        <fo:table-cell padding-left="2pt" border-color="black" border-style="solid" border-width="1pt" >
	                <fo:block>
	                        <xsl:apply-templates/>
	                </fo:block>
	        </fo:table-cell>               
	</xsl:template>


<!-- ************************************************ -->   	
<!-- 			Lists   		      --> 
<!-- ************************************************ --> 		   	

	<!-- unordered list -->	
	<xsl:template match="ul">        
	        <fo:list-block provisional-distance-between-starts="10pt" start-indent="8pt">
	                <xsl:apply-templates/>
	        </fo:list-block>
	</xsl:template>

<!-- ************************************************ -->   		
	
	<!-- unordered list element-->	
	<xsl:template match="ul/li">
	        <fo:list-item>
	                <fo:list-item-label>
	                        <fo:block>&#x2022;</fo:block>
	                </fo:list-item-label>
	                <fo:list-item-body>
	                        <fo:block
	                                font-size="12pt"
	                                line-height="{$lineHeight}pt"
	                                font-family="serif">
	                                	<xsl:apply-templates/>
	                        </fo:block>
	                </fo:list-item-body>        
	        </fo:list-item>
	</xsl:template>
	
<!-- ************************************************ -->   	
	
	<!-- unordered list element (nested level 1) -->	
	<xsl:template match="nestedUL1/li">
	        <fo:list-item>
	                <fo:list-item-label>
	                        <fo:block></fo:block>
	                </fo:list-item-label>
	                <fo:list-item-body>
	                        <fo:block 
	                                text-indent="8pt"
	                                font-size="12pt"
	                                line-height="{$lineHeight}pt"
	                                font-family="serif">	                         
	                                	<xsl:text> &#x2022; </xsl:text><xsl:apply-templates/>
	                        </fo:block>
	                </fo:list-item-body>        
	        </fo:list-item>
	</xsl:template>
	

<!-- ************************************************ -->   	
	
	<!-- unordered list element (nested level 2) -->	
	<xsl:template match="nestedUL2/li">
	        <fo:list-item>
	                <fo:list-item-label>
	                        <fo:block></fo:block>
	                </fo:list-item-label>
	                <fo:list-item-body>
	                        <fo:block 
	                        text-indent="20pt"
	                        font-size="12pt"
	                        line-height="{$lineHeight}pt"
	                        font-family="serif">	                 
	                                <xsl:text> - </xsl:text> <xsl:apply-templates/>
	                        </fo:block>
	                </fo:list-item-body>        
	        </fo:list-item>
	</xsl:template>
	

<!-- ************************************************ -->   		
	
	<!-- ordered list -->	
	<xsl:template match="ol">        
	        <fo:list-block provisional-distance-between-starts="20pt" start-indent="8pt">
	                <xsl:apply-templates/>
	        </fo:list-block>
	</xsl:template>
	
<!-- ************************************************ -->   		
	
	<!-- ordered list element-->	
	<xsl:template match="ol/li">
	        <fo:list-item>
	                <fo:list-item-label>
	                        <fo:block><xsl:value-of select="@number"/>.</fo:block>
	                </fo:list-item-label>
	                <fo:list-item-body>
	                        <fo:block
	                                font-size="12pt"
	                                line-height="{$lineHeight}pt"
	                                font-family="serif">	                        
	                                	<xsl:apply-templates/>
	                        </fo:block>
	                </fo:list-item-body>        
	        </fo:list-item>
	</xsl:template>

<!-- ************************************************ -->   		
	
	<!-- ordered list element (nested level 1) -->	
	<xsl:template match="nestedOL1/li">
	        <fo:list-item>
	                <fo:list-item-label>
	                        <fo:block></fo:block>
	                </fo:list-item-label>
	                <fo:list-item-body>
	                        <fo:block                                 
	                                font-size="12pt"
	                                line-height="{$lineHeight}pt"
	                                font-family="serif"
	                                text-indent="4pt">	                         
	                                	<xsl:value-of select="@number"/>. <xsl:apply-templates/>
	                        </fo:block>                        
	                </fo:list-item-body>        
	        </fo:list-item>
	</xsl:template>
	
<!-- ************************************************ -->   	
<!-- 			Misc    		      --> 
<!-- ************************************************ --> 		   			
   	

	<!-- paragraph-->
	<xsl:template match="p">
	        <fo:block 
	                font-size="12pt"
	                color="{$textColor}"
	                line-height="{$lineHeight}pt"
	                font-family="Times"                
	                space-before.optimum="2pt"
	                space-after.optimum="2pt">               
	                <xsl:apply-templates/>        
	        </fo:block>
	</xsl:template>      		

<!-- ************************************************ --> 

	<!-- source -->	
	<xsl:template match="source">        
	        <fo:block background-color="#dddddd"                 
			space-before.optimum="5pt"
	                space-after.optimum="5pt">
	        	<fo:block
				font-size="10pt"
	                        line-height="{$lineHeight}pt" 
	                        color="{$textColor}"               
	                        font-family="monospace"                                            
	                        start-indent="20pt"	                            
	                        white-space-treatment="preserve">
	                        	<xsl:apply-templates/>
			</fo:block>                   
		</fo:block>
	</xsl:template>
	

<!-- ************************************************ --> 	           

	<!-- code -->	
	<xsl:template match="code">
	    <fo:inline-sequence font-family="monospace">
	                <xsl:apply-templates/>
	    </fo:inline-sequence>
	</xsl:template>           
	

<!-- ************************************************ --> 
	    
	<!-- note -->	
	<xsl:template match="note">                
	          <fo:block 
	                space-before.optimum="2pt" 
	                space-after.optimum="3pt"
	                font-style="italic"
	                start-indent="20pt"
	                color="{$textColor}"
	                line-height="{$lineHeight}pt"
	                end-indent="20pt">
	                	<fo:inline-sequence font-weight="bold">Note: </fo:inline-sequence>
	                	<xsl:apply-templates/>
	        </fo:block>
	</xsl:template>
	
<!-- ************************************************ --> 	
	
	<!-- faqs -->	
	<xsl:template match="faqs">
	        <xsl:variable name = "id" select="@id"/>
	          				
	        <fo:block 
	        	id="{$id}"
	                text-align="start" 
	                font-size="20pt" 
	                font-weight="bold"
	                font-family="serif"                
	                color="{$titleColor}"                
	                space-before.optimum="4pt" 
	                space-after.optimum="5pt"                
	                line-height="22pt">
	                        <xsl:value-of select="@title"/>
	         </fo:block>      
	        <xsl:apply-templates/>   
	</xsl:template>
	             
	             
<!-- ************************************************ --> 	             
	
	<!-- question -->	
	<xsl:template match="q">
	        <fo:block 
	                font-size="12pt"
	                line-height="{$lineHeight}pt"
	                font-family="serif"
	                color="{$textColor}"       
	                font-weight="bold"
	                font-style="italic"         
	                space-before.optimum="2pt">               
	                <xsl:apply-templates/>        
	        </fo:block>
	</xsl:template>    

<!-- ************************************************ --> 	
	
	<!-- answer -->	
	<xsl:template match="a">
	        <fo:block 
	                font-size="12pt"
	                line-height="{$lineHeight}pt"
	                color="{$textColor}"
	                font-family="serif"
	                space-after.optimum="2pt">               
	                <xsl:apply-templates/>        
	        </fo:block>
	</xsl:template>    

<!-- ************************************************ -->   	

	<!-- em -->	
	<xsl:template match="em">
		<fo:inline-sequence font-weight="bold">
	        	<xsl:apply-templates/>
	        </fo:inline-sequence>  
	</xsl:template>

<!-- ************************************************ -->   		
	
	<!-- ref -->	
	<xsl:template match="ref">
		<fo:inline-sequence font-style="italic">
	        	<xsl:apply-templates/>
		</fo:inline-sequence>  
	</xsl:template>
	
<!-- ************************************************ -->   		
	
	<!-- change -->	
	<xsl:template match="changes">
		<fo:block 
	        	text-align="start" 
	                font-size="16pt" 
	                font-family="serif"	                
	                color="{$titleColor}"
	                padding-top="2pt"
	                space-before.optimum="5pt" 
	                space-after.optimum="5pt"
	                line-height="18pt" >
	                        <xsl:value-of select="@title"/>
		</fo:block> 
	        <xsl:apply-templates/>
	</xsl:template>
	
<!-- ************************************************ -->   		
	
	<!-- release -->	
	<xsl:template match="release">
	        <fo:block 
	                text-align="start" 
	                font-size="14pt" 
	                font-family="serif"
	                font-weight="bold"
	                background-color="transparent"
	                color="{$titleColor}"
	                padding-top="2pt"
	                space-before.optimum="10pt"                 
	                line-height="16pt" >
	                        <xsl:value-of select="@version"/> : <xsl:value-of select="@date"/> 
	         </fo:block>      
	         <xsl:apply-templates/>                
	</xsl:template>
	
<!-- ************************************************ -->   	
	
	<!-- features -->	
	<xsl:template match="features">
	        <fo:block 
	                text-align="start" 
	                font-size="12pt" 
	                line-height="{$lineHeight}pt"
	                color="{$titleColor}"
	                font-family="serif"
	                font-weight="bold"
	                background-color="transparent"                
	                padding-top="2pt"
	                space-before.optimum="4pt"                 
	                >
	                        Features
	         </fo:block>
	         <fo:list-block provisional-distance-between-starts="8pt" start-indent="8pt"> <xsl:apply-templates/> </fo:list-block>
	</xsl:template>
	
<!-- ************************************************ -->   	
	
	<!-- fixes -->	
	<xsl:template match="fixes">
	        <fo:block 
	                text-align="start" 
	                font-size="12pt"
	                color="{$titleColor}" 
	                line-height="{$lineHeight}pt"
	                font-family="serif"
	                font-weight="bold"
	                background-color="transparent"                
	                padding-top="2pt"
	                space-before.optimum="4pt">
	                        Fixes
	         </fo:block>      
	         <fo:list-block provisional-distance-between-starts="8pt" start-indent="8pt"> <xsl:apply-templates/> </fo:list-block>                
	</xsl:template>


<!-- ************************************************ -->   		
	
	<!-- feat / fix -->	
	<xsl:template match="feat|fix">
	        <fo:list-item>
	                <fo:list-item-label>
	                        <fo:block>&#x2022;</fo:block>
	                </fo:list-item-label>
	                <fo:list-item-body>
	                        <fo:block
	                                font-size="12pt"
	                                line-height="{$lineHeight}pt"
	                                font-family="serif">
	                                	<xsl:apply-templates/>
	                        </fo:block>
	                </fo:list-item-body>        
	        </fo:list-item>                                      
	</xsl:template>         
	

<!-- ************************************************ -->   	
	
	<!-- jump -->	
	<xsl:template match="jump">     
		<xsl:variable name = "href" select="@href"/>	                  
	     	<fo:inline-sequence color="blue">
	    		<fo:simple-link external-destination="{$href}">
	    			<xsl:apply-templates/>
	    		</fo:simple-link>
	    		<fo:inline-sequence font-size="10pt">
	    			[<xsl:value-of select="@referenceNumber"/>]
			</fo:inline-sequence>            
		</fo:inline-sequence>	    
	</xsl:template>


<!-- ************************************************ -->   		    
	        
	<!-- br -->
	<xsl:template match="br">
		<fo:block space-before.optimum="2pt"/>
	</xsl:template>

<!-- ************************************************ -->   		
	             
	<!-- link -->
	<xsl:template match="link">
	<xsl:variable name = "anchor" select="@anchor"/>	                      
		<fo:inline-sequence color="blue">
	    		<fo:simple-link internal-destination="{$anchor}">
	    			<xsl:apply-templates/>
	    		</fo:simple-link>
	    	</fo:inline-sequence>
	</xsl:template>

<!-- ************************************************ --> 

	<!-- internalAnchor -->
	<xsl:template match="internalAnchor">
		<xsl:variable name = "anchorName" select="@name"/>	
		<fo:block id="{$anchorName}"/>
	</xsl:template>

<!-- ************************************************ --> 
	
	<!-- image -->
	<xsl:template match="img">
		<xsl:variable name = "src" select="@src"/>	
		<fo:inline-graphic href="file:///{$src}"/>
	</xsl:template>  	
	
<!-- ************************************************ -->   	
<!-- 			Apppendix 		      --> 
<!-- ************************************************ --> 		   

	<!-- appendix -->
	<xsl:template match="appendix">
	<xsl:param name="documentTitle"/>
		<fo:page-sequence master-name="sequence-chapter" initial-page-number="auto">  	                	                      			
		        
<!-- appendix header -->
			<fo:static-content flow-name="xsl-before">
			        <fo:block>
			        	<fo:table>
			                	<fo:table-column column-width="9cm" />                
			                        <fo:table-column column-width="9cm" />
		                        	<fo:table-body>
		                                	<fo:table-row>                                                       
		                                        	<fo:table-cell>
			                                        	<fo:block
			                                        		text-align="start"
			                                        		font-size="12pt" 
			                                        		font-family="serif" 
			                                        		line-height="14pt"
			                                        		white-space-treatment="preserve">
			                                        			<fo:inline-sequence font-weight="bold">
			                                        				Appendix <xsl:value-of select="@appendixLetter"/></fo:inline-sequence><xsl:text> - </xsl:text><xsl:value-of select="@appendixTitle"/>
									</fo:block>   
		                                        	</fo:table-cell>
		                                        	<fo:table-cell>
		                                        		<fo:block
		                                        			text-align="end"
		                                        			font-size="12pt" 
		                                        			font-family="serif" 
		                                        			line-height="14pt">	                                        
		                                        			<xsl:value-of select="$documentTitle"/>
		                                        		</fo:block>   
		                                        	</fo:table-cell>
		                                        </fo:table-row>                 
						</fo:table-body>
					</fo:table>
			                                                
					<fo:display-rule/>        
				</fo:block>
			</fo:static-content>  
		                        
<!-- appendix footer -->
			<fo:static-content flow-name="xsl-after">
		        	<fo:block 
		                	text-align="centered" 
		                        font-size="10pt" 
		                        font-family="serif"
		                        line-height="10pt">	                                                                                      
					<fo:block space-before.optimum="3pt">
						-<fo:page-number/>-
					</fo:block>                                         
				</fo:block>
			</fo:static-content>
		                         
		                         
<!-- appendix body -->
			<fo:flow flow-name="xsl-body">
		        <xsl:variable name = "appendixLetter" select="@appendixLetter"/>	                  		
				<fo:block 
		                	id="Appendix#{$appendixLetter}"
		                        text-align="end"
		                        font-family="serif"
		                        space-before.optimum="50pt"
		                        space-after.optimum="30pt"
		                        start-indent="20pt"
		                        color="{$titleColor}"                                
		                        font-size="35pt">
		                        	Appendix <xsl:value-of select="$appendixLetter"/> 
		                                <fo:block space-before.optimum="20pt" font-size="24pt">
		                                	<xsl:value-of select="@appendixTitle"/>
		                                </fo:block>
				</fo:block>                                                                
					<xsl:apply-templates/>                                                                >
			</fo:flow>                                                                                 
		</fo:page-sequence>                                            
	</xsl:template>
	

<!-- ************************************************ --> 	         
	         
	<!-- links_reference_entry -->	
	<xsl:template match="links_reference_entry">               
	        <fo:block                      
	        	font-size="10pt" 
	        	font-family="serif" 
	        	line-height="10pt"
	        	space-before.optimum="4pt"
	        	space-after.optimum="8pt">	        
	        		[<xsl:value-of select="@referenceNumber"/>] <xsl:value-of select="@href"/>
	        </fo:block>   	        
	</xsl:template>      

<!-- ************************************************ -->   	   		  	
	
</xsl:stylesheet>