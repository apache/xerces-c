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
 * Revision 1.3  2003/11/10 21:56:54  neilg
 * make internal code use the new, stateless, method of traversing attribute lists
 *
 * Revision 1.2  2003/11/06 15:30:04  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSComplexTypeDefinition.hpp>
#include <xercesc/validators/schema/ComplexTypeInfo.hpp>
#include <xercesc/validators/schema/SchemaElementDecl.hpp>
#include <xercesc/validators/schema/SchemaAttDefList.hpp>
#include <xercesc/framework/psvi/XSWildcard.hpp>
#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSAttributeUse.hpp>

XERCES_CPP_NAMESPACE_BEGIN

XSComplexTypeDefinition::XSComplexTypeDefinition(ComplexTypeInfo*       complexTypeInfo,
                                                 XMLStringPool*         uriStringPool,
                                                 MemoryManager* const   manager ):  
        fComplexTypeInfo(complexTypeInfo),
        fXSWildcard(0),
        fXSAttributeUseList(0),
        fXSSimpleTypeDefinition(0),
        fProhibitedSubstitution(0),
        XSTypeDefinition(COMPLEX_TYPE, manager)
{
    int blockset;
    if (blockset = fComplexTypeInfo->getBlockSet()) 
    {
        if (blockset & SchemaSymbols::XSD_EXTENSION)
        {
            fProhibitedSubstitution |= XSConstants::DERIVATION_EXTENSION;
        }
        if (blockset & SchemaSymbols::XSD_RESTRICTION)
        {
            fProhibitedSubstitution |= XSConstants::DERIVATION_RESTRICTION;
        }
    }

    if (fComplexTypeInfo->getAttWildCard()) 
    {
        fXSWildcard = new (manager) XSWildcard(fComplexTypeInfo->getAttWildCard(), uriStringPool, manager);
    }

    if ((fComplexTypeInfo->getContentType() == SchemaElementDecl::Simple) &&
        (fComplexTypeInfo->getDatatypeValidator()))
    {
        fXSSimpleTypeDefinition = new (manager) XSSimpleTypeDefinition(fComplexTypeInfo->getDatatypeValidator(), manager);
    }

    if (fComplexTypeInfo->hasAttDefs())
    {
        SchemaAttDefList& schemaAttDefList = (SchemaAttDefList&) fComplexTypeInfo->getAttDefList();
        // REVISIT: size of vector...
        fXSAttributeUseList = new (manager) RefVectorOf <XSAttributeUse> (10, true, manager);
            
        for(unsigned int i=0; i<schemaAttDefList.getAttDefCount(); i++)
        {
            SchemaAttDef& attDef = (SchemaAttDef&) schemaAttDefList.getAttDef(i);
            fXSAttributeUseList->addElement(new (manager) XSAttributeUse(&attDef, uriStringPool, manager));
        }
    }
}

XSComplexTypeDefinition::~XSComplexTypeDefinition() 
{
    if (fXSWildcard) 
    {
        delete fXSWildcard;
    }
    if (fXSSimpleTypeDefinition)
    {
        delete fXSSimpleTypeDefinition;
    }
    if (fXSAttributeUseList)
    {
        delete fXSAttributeUseList;
    }
}

// XSComplexTypeDefinition methods

/**
 * [derivation method]: either <code>DERIVATION_EXTENSION</code>, 
 * <code>DERIVATION_RESTRICTION</code>, or <code>DERIVATION_NONE</code> 
 * (see <code>XSObject</code>). 
 */
XSConstants::DERIVATION_TYPE XSComplexTypeDefinition::getDerivationMethod() const
{
    switch(fComplexTypeInfo->getDerivedBy()) {
        case SchemaSymbols::XSD_RESTRICTION:
            return XSConstants::DERIVATION_RESTRICTION;
        case SchemaSymbols::XSD_EXTENSION:
            return XSConstants::DERIVATION_EXTENSION;
        default:
            return XSConstants::DERIVATION_NONE;
    }
}

/**
 * [abstract]: a boolean. Complex types for which <code>abstract</code> is 
 * true must not be used as the type definition for the validation of 
 * element information items. 
 */
bool XSComplexTypeDefinition::getAbstract() const
{
    return fComplexTypeInfo->getAbstract();
}

/**
 *  A set of attribute uses. 
 */
XSAttributeUseList *XSComplexTypeDefinition::getAttributeUses()
{
    return fXSAttributeUseList;
}

/**
 * Optional.An attribute wildcard. 
 */
XSWildcard *XSComplexTypeDefinition::getAttributeWildcard()
{
    return fXSWildcard;
}

/**
 * [content type]: one of empty (<code>CONTENTTYPE_EMPTY</code>), a simple 
 * type definition (<code>CONTENTTYPE_SIMPLE</code>), mixed (
 * <code>CONTENTTYPE_MIXED</code>), or element-only (
 * <code>CONTENTTYPE_ELEMENT</code>). 
 */
XSComplexTypeDefinition::CONTENT_TYPE XSComplexTypeDefinition::getContentType() const
{
    switch(fComplexTypeInfo->getContentType()) {
        case SchemaElementDecl::Simple:
            return CONTENTTYPE_SIMPLE;
        case SchemaElementDecl::Empty:
            return CONTENTTYPE_EMPTY;
        case SchemaElementDecl::Children:
            return CONTENTTYPE_ELEMENT;
        default:
            //case SchemaElementDecl::Mixed_Complex:
            //case SchemaElementDecl::Mixed_Simple:
            //case SchemaElementDecl::Any:
            return CONTENTTYPE_MIXED;
    }
}

/**
 * A simple type definition corresponding to simple content model, 
 * otherwise <code>null</code> 
 */
XSSimpleTypeDefinition *XSComplexTypeDefinition::getSimpleType()
{
    return fXSSimpleTypeDefinition;
}

/**
 * A particle for mixed or element-only content model, otherwise 
 * <code>null</code> 
 */
XSParticle *XSComplexTypeDefinition::getParticle()
{
    // REVISIT
    return 0;
}

/**
 * [prohibited substitutions]: a subset of {extension, restriction}
 * @param restriction  Extention or restriction constants (see 
 *   <code>XSObject</code>). 
 * @return True if toTest is a prohibited substitution, otherwise 
 *   false.
 */
bool XSComplexTypeDefinition::isProhibitedSubstitution(XSConstants::DERIVATION_TYPE toTest)                                                     
{
    if (fProhibitedSubstitution & toTest)
    {
        return true;
    }
    return false;
}

/**
 *  [prohibited substitutions]: A subset of {extension, restriction} or 
 * <code>DERIVATION_NONE</code> represented as a bit flag (see 
 * <code>XSObject</code>). 
 */
short XSComplexTypeDefinition::getProhibitedSubstitutions()
{
    return fProhibitedSubstitution;
}

/**
 * A set of [annotations]. 
 */
XSAnnotationList *XSComplexTypeDefinition::getAnnotations()
{
    // REVISIT
    return 0;
}

XERCES_CPP_NAMESPACE_END
