/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2003 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.3  2003/11/06 21:50:33  neilg
 * fix compilation errors under gcc 3.3.
 *
 * Revision 1.2  2003/11/06 15:30:04  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#if !defined(XSSIMPLETYPEDEFINITION_HPP)
#define XSSIMPLETYPEDEFINITION_HPP

#include <xercesc/framework/psvi/XSTypeDefinition.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/**
 * This class represents a simpleType definition
 * schema component.
 * This is *always* owned by the validator /parser object from which
 * it is obtained.  
 *
 */

// forward declarations
class XSAnnotation;
class XSFacet;
class XSMultiValueFacet;

class DatatypeValidator;

class XMLPARSER_EXPORT XSSimpleTypeDefinition : public XSTypeDefinition
{
public:

    // Variety definitions
    enum VARIETY {
	    /**
	     * The variety is absent for the anySimpleType definition.
	     */
	    VARIETY_ABSENT            = 0,
	    /**
	     * <code>Atomic</code> type.
	     */
	    VARIETY_ATOMIC            = 1,
	    /**
	     * <code>List</code> type.
	     */
	    VARIETY_LIST              = 2,
	    /**
	     * <code>Union</code> type.
	     */
	    VARIETY_UNION             = 3
    };

    // Facets
    enum FACET {
	    /**
	     * No facets defined.
	     */
	    FACET_NONE                = 0,
	    /**
	     * 4.3.1 Length
	     */
	    FACET_LENGTH              = 1,
	    /**
	     * 4.3.2 minLength. 
	     */
	    FACET_MINLENGTH           = 2,
	    /**
	     * 4.3.3 maxLength.
	     */
	    FACET_MAXLENGTH           = 4,
	    /**
	     * 4.3.4 pattern.
	     */
	    FACET_PATTERN             = 8,
	    /**
	     * 4.3.5 whitespace.
	     */
	    FACET_WHITESPACE          = 16,
	    /**
	     * 4.3.7 maxInclusive.
	     */
	    FACET_MAXINCLUSIVE        = 32,
	    /**
	     * 4.3.9 maxExclusive.
	     */
	    FACET_MAXEXCLUSIVE        = 64,
	    /**
	     * 4.3.9 minExclusive.
	     */
	    FACET_MINEXCLUSIVE        = 128,
	    /**
	     * 4.3.10 minInclusive.
	     */
	    FACET_MININCLUSIVE        = 256,
	    /**
	     * 4.3.11 totalDigits .
	     */
	    FACET_TOTALDIGITS         = 512,
	    /**
	     * 4.3.12 fractionDigits.
	     */
	    FACET_FRACTIONDIGITS      = 1024,
	    /**
	     * 4.3.5 enumeration.
	     */
	    FACET_ENUMERATION         = 2048
    };

    // possible order relations
    enum ORDERING {
	    /**
	     * A constant defined for the 'ordered' fundamental facet: Not ordered.
	     */
	    ORDERED_FALSE             = 0,
	    /**
	     * A constant defined for the 'ordered' fundamental facet: partially 
	     * ordered.
	     */
	    ORDERED_PARTIAL           = 1,
	    /**
	     * A constant defined for the 'ordered' fundamental facet: total ordered.
	     */
        ORDERED_TOTAL             = 2
    };

	//  Constructors and Destructor
    // -----------------------------------------------------------------------
    /** @name Constructors */
    //@{

    /**
      * The default constructor 
      *
      * @param  manager     The configurable memory manager
      */
    XSSimpleTypeDefinition(DatatypeValidator*   datatypeValidator,
                           MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    //@};

    /** @name Destructor */
    //@{
    ~XSSimpleTypeDefinition();
    //@}

    //---------------------
    /** @name XSSimpleTypeDefinition methods */

    //@{

    /**
     * [variety]: one of {atomic, list, union} or absent 
     */
    VARIETY getVariety() const;

    /**
     * If variety is <code>atomic</code> the primitive type definition (a 
     * built-in primitive datatype definition or the simple ur-type 
     * definition) is available, otherwise <code>null</code>. 
     */
    XSSimpleTypeDefinition *getPrimitiveType();

    /**
     * If variety is <code>list</code> the item type definition (an atomic or 
     * union simple type definition) is available, otherwise 
     * <code>null</code>. 
     */
    XSSimpleTypeDefinition *getItemType();

    /**
     * If variety is <code>union</code> the list of member type definitions (a 
     * non-empty sequence of simple type definitions) is available, 
     * otherwise <code>null</code>. 
     */
    XSSimpleTypeDefinitionList *getMemberTypes();

    /**
     * [facets]: get all facets defined on this type. The value is a bit 
     * combination of FACET_XXX constants of all defined facets. 
     */
    short getDefinedFacets();

    /**
     * Convenience method. [Facets]: check whether a facet is defined on this 
     * type.
     * @param facetName  The name of the facet. 
     * @return  True if the facet is defined, false otherwise.
     */
    bool isDefinedFacet(FACET facetName);

    /**
     * [facets]: get all facets defined and fixed on this type.
     */
    short getFixedFacets();

    /**
     * Convenience method. [Facets]: check whether a facet is defined and 
     * fixed on this type. 
     * @param facetName  The name of the facet. 
     * @return  True if the facet is fixed, false otherwise.
     */
    bool isFixedFacet(FACET facetName);

    /**
     * Convenience method. Returns a value of a single constraining facet for 
     * this simple type definition. This method must not be used to retrieve 
     * values for <code>enumeration</code> and <code>pattern</code> facets. 
     * @param facetName The name of the facet, i.e. 
     *   <code>FACET_LENGTH, FACET_TOTALDIGITS </code> (see 
     *   <code>XSConstants</code>).To retrieve value for pattern or 
     *   enumeration, see <code>enumeration</code> and <code>pattern</code>.
     * @return A value of the facet specified in <code>facetName</code> for 
     *   this simple type definition or <code>null</code>. 
     */
    const XMLCh *getLexicalFacetValue(FACET facetName);

    /**
     * Returns a list of enumeration values. 
     */
    StringList *getLexicalEnumeration();

    /**
     * Returns a list of pattern values. 
     */
    StringList *getLexicalPattern();

    /**
     *  Fundamental Facet: ordered 
     */
    ORDERING getOrdered() const;

    /**
     * Fundamental Facet: cardinality. 
     */
    bool getFinite() const;

    /**
     * Fundamental Facet: bounded. 
     */
    bool getBounded() const;

    /**
     * Fundamental Facet: numeric. 
     */
    bool getNumeric() const;

    /**
     * Optional. A set of [annotation]s. 
     */
    XSAnnotationList *getAnnotations();
    /** 
     * @return list of constraining facets.
     * This method must not be used to retrieve 
     * values for <code>enumeration</code> and <code>pattern</code> facets.
     */
    XSFacetList *getFacets();
    
    /** 
     * @return list of enumeration and pattern facets.
     */
    XSMultiValueFacetList *getMultiValueFacets();

    //@}

    //----------------------------------
    /** methods needed by implementation */

    //@{

    //@}

private:

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XSSimpleTypeDefinition(const XSSimpleTypeDefinition&);
    XSSimpleTypeDefinition & operator=(const XSSimpleTypeDefinition &);

protected:

    // -----------------------------------------------------------------------
    //  data members
    // -----------------------------------------------------------------------
    DatatypeValidator*                  fDatatypeValidator;
    XSFacetList*                        fXSFacetList;
    XSMultiValueFacetList*              fXSMultiValueFacetList;
    StringList*                         fPatternList;
    short                               fDefinedFacets;
    short                               fFixedFacets;
    VARIETY                             fVariety;
    XSSimpleTypeDefinition*             fPrimitiveOrItemType;
    XSSimpleTypeDefinitionList*         fMemberTypes;
};

XERCES_CPP_NAMESPACE_END

#endif
