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
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>

XERCES_CPP_NAMESPACE_BEGIN

XSSimpleTypeDefinition::XSSimpleTypeDefinition( MemoryManager* const manager ):  
        XSTypeDefinition(SIMPLE_TYPE, manager)
{
}

// XSSimpleTypeDefinition methods


/**
 * [variety]: one of {atomic, list, union} or absent 
 */
XSSimpleTypeDefinition::VARIETY XSSimpleTypeDefinition::getVariety() const
{
    // REVISIT
    return VARIETY_ATOMIC;
}

/**
 * If variety is <code>atomic</code> the primitive type definition (a 
 * built-in primitive datatype definition or the simple ur-type 
 * definition) is available, otherwise <code>null</code>. 
 */
XSSimpleTypeDefinition *XSSimpleTypeDefinition::getPrimitiveType()
{
    // REVISIT
    return 0;
}

/**
 * If variety is <code>list</code> the item type definition (an atomic or 
 * union simple type definition) is available, otherwise 
 * <code>null</code>. 
 */
XSSimpleTypeDefinition *XSSimpleTypeDefinition::getItemType()
{
    // REVISIT
    return 0;
}

/**
 * If variety is <code>union</code> the list of member type definitions (a 
 * non-empty sequence of simple type definitions) is available, 
 * otherwise <code>null</code>. 
 */
StringList *XSSimpleTypeDefinition::getMemberTypes()
{
    // REVISIT
    return 0;
}

/**
 * [facets]: get all facets defined on this type. The value is a bit 
 * combination of FACET_XXX constants of all defined facets. 
 */
short XSSimpleTypeDefinition::getDefinedFacets()
{
    // REVISIT
    return 0;
}

/**
 * Convenience method. [Facets]: check whether a facet is defined on this 
 * type.
 * @param facetName  The name of the facet. 
 * @return  True if the facet is defined, false otherwise.
 */
bool XSSimpleTypeDefinition::isDefinedFacet(FACET facetName)
{
    // REVISIT 
    return 0;
}

/**
 * [facets]: get all facets defined and fixed on this type.
 */
short XSSimpleTypeDefinition::getFixedFacets()
{
    // REVISIT
    return 0;
}

/**
 * Convenience method. [Facets]: check whether a facet is defined and 
 * fixed on this type. 
 * @param facetName  The name of the facet. 
 * @return  True if the facet is fixed, false otherwise.
 */
bool XSSimpleTypeDefinition::isFixedFacet(FACET facetName)
{
    // REVISIT
    return false;
}

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
const XMLCh *XSSimpleTypeDefinition::getLexicalFacetValue(FACET facetName)
{
    // REVISIT
    return 0;
}

/**
 * Returns a list of enumeration values. 
 */
StringList *XSSimpleTypeDefinition::getLexicalEnumeration()
{
    // REVISIT
    return 0;
}

/**
 * Returns a list of pattern values. 
 */
StringList *XSSimpleTypeDefinition::getLexicalPattern()
{
    // REVISIT
    return 0;
}

/**
 *  Fundamental Facet: ordered 
 */
XSSimpleTypeDefinition::ORDERING XSSimpleTypeDefinition::getOrdered() const
{
    // REVISIT
    return ORDERED_TOTAL;
}

/**
 * Fundamental Facet: cardinality. 
 */
bool XSSimpleTypeDefinition::getFinite() const
{
    // REVISIT
    return false;
}

/**
 * Fundamental Facet: bounded. 
 */
bool XSSimpleTypeDefinition::getBounded() const
{
    // REVISIT
    return false;
}

/**
 * Fundamental Facet: numeric. 
 */
bool XSSimpleTypeDefinition::getNumeric() const
{
    // REVISIT
    return false;
}

/**
 * Optional. A set of [annotation]s. 
 */
XSAnnotationList *XSSimpleTypeDefinition::getAnnotations()
{
    // REVISIT 
    return 0;
}

/** 
 * @return list of constraining facets.
 * This method must not be used to retrieve 
 * values for <code>enumeration</code> and <code>pattern</code> facets.
 */
XSFacetList *XSSimpleTypeDefinition::getFacets()
{
    // REVISIT 
    return 0;
}

/** 
 * @return list of enumeration and pattern facets.
 */
XSFacetList *XSSimpleTypeDefinition::getMultiValueFacets()
{
    // REVISIT 
    return 0;
}

XERCES_CPP_NAMESPACE_END


