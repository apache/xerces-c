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
 * Revision 1.5  2003/11/21 17:34:04  knoaman
 * PSVI update
 *
 * Revision 1.4  2003/11/14 22:47:53  neilg
 * fix bogus log message from previous commit...
 *
 * Revision 1.3  2003/11/14 22:33:30  neilg
 * Second phase of schema component model implementation.  
 * Implement XSModel, XSNamespaceItem, and the plumbing necessary
 * to connect them to the other components.
 * Thanks to David Cargill.
 *
 * Revision 1.2  2003/11/06 15:30:04  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSFacet.hpp>
#include <xercesc/framework/psvi/XSMultiValueFacet.hpp>
#include <xercesc/framework/psvi/XSAnnotation.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/validators/datatype/DatatypeValidator.hpp>
#include <xercesc/validators/datatype/UnionDatatypeValidator.hpp>
#include <xercesc/util/XMLStringTokenizer.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Local, static functions
// ---------------------------------------------------------------------------
static bool XSSimpleTypeDefinitionTestFlag(int flag) 
{
    if (flag)
        return true;
    return false;
}


// ---------------------------------------------------------------------------
//  XSSimpleTypeDefinition: Constructors and Destructors
// ---------------------------------------------------------------------------
XSSimpleTypeDefinition::XSSimpleTypeDefinition(DatatypeValidator* const datatypeValidator,
                                               XSAnnotation* const      headAnnot,
                                               XSModel* const           xsModel,
                                               MemoryManager* const     manager)
    : XSTypeDefinition(SIMPLE_TYPE, 0, xsModel, manager)
    , fDefinedFacets(0)
    , fFixedFacets(0)
    , fVariety(VARIETY_ABSENT)
    , fDatatypeValidator(datatypeValidator)
    , fXSFacetList(0)
    , fXSMultiValueFacetList(0)
    , fPatternList(0)
    , fPrimitiveOrItemType(0)
    , fMemberTypes(0)
    , fXSAnnotationList(0)
{
    if (int finalset = fDatatypeValidator->getFinalSet()) 
    {
        if (finalset & SchemaSymbols::XSD_EXTENSION)
            fFinal |= XSConstants::DERIVATION_EXTENSION;

        if (finalset & SchemaSymbols::XSD_RESTRICTION)
            fFinal |= XSConstants::DERIVATION_RESTRICTION;

        if (finalset & SchemaSymbols::XSD_LIST)
            fFinal |= XSConstants::DERIVATION_LIST;

        if (finalset & SchemaSymbols::XSD_UNION)
            fFinal |= XSConstants::DERIVATION_UNION;
    }

    if (headAnnot)
    {
        XSAnnotation* annot = headAnnot;

        fXSAnnotationList = new (manager) RefVectorOf<XSAnnotation>(3, false, manager);
        do
        {
            fXSAnnotationList->addElement(annot);
            annot = annot->getNext();
        } while (annot);
    }
}

XSSimpleTypeDefinition::~XSSimpleTypeDefinition()
{
    if (fXSFacetList) 
        delete fXSFacetList;

    if (fXSMultiValueFacetList) 
        delete fXSMultiValueFacetList;

    if (fPatternList) 
        delete fPatternList;

    // don't delete fPrimitiveOrItemType -> deleted by XSModel
    if (fMemberTypes)
        delete fMemberTypes;

    if (fXSAnnotationList)
        delete fXSAnnotationList;
}


// ---------------------------------------------------------------------------
//  XSSimpleTypeDefinition: access methods
// ---------------------------------------------------------------------------
bool XSSimpleTypeDefinition::isDefinedFacet(FACET facetName)
{
    return XSSimpleTypeDefinitionTestFlag(fDefinedFacets & facetName);
}

bool XSSimpleTypeDefinition::isFixedFacet(FACET facetName)
{
    return XSSimpleTypeDefinitionTestFlag(fFixedFacets & facetName);
}

const XMLCh *XSSimpleTypeDefinition::getLexicalFacetValue(FACET facetName)
{
    // REVISIT
    // We have a vector of Facets.  To do a search is a linear operation.  Would
    // be faster to store them in mutiple formats (vector & hashtable).  Will
    // initially process them in just a vector...
    unsigned int size = fXSFacetList->size();
    for (unsigned int i=0; i<size; i++) 
    {
        if (((fXSFacetList->elementAt(i))->getFacetKind()) == facetName)
            return (fXSFacetList->elementAt(i))->getLexicalFacetValue();
    }
    return 0;
}

StringList *XSSimpleTypeDefinition::getLexicalEnumeration()
{
    return (RefArrayVectorOf<XMLCh>*) fDatatypeValidator->getEnumString();
}

XSSimpleTypeDefinition::ORDERING XSSimpleTypeDefinition::getOrdered() const
{
    return fDatatypeValidator->getOrdered();
}

bool XSSimpleTypeDefinition::getFinite() const
{
    return fDatatypeValidator->getFinite();
}

bool XSSimpleTypeDefinition::getBounded() const
{
    return fDatatypeValidator->getBounded();
}

bool XSSimpleTypeDefinition::getNumeric() const
{
    return fDatatypeValidator->getNumeric();
}


// ---------------------------------------------------------------------------
//  XSSimpleTypeDefinition: virtual methods
// ---------------------------------------------------------------------------
const XMLCh *XSSimpleTypeDefinition::getName() 
{
    return fDatatypeValidator->getTypeLocalName();
}

const XMLCh *XSSimpleTypeDefinition::getNamespace() 
{
    return fDatatypeValidator->getTypeUri();
}

XSNamespaceItem *XSSimpleTypeDefinition::getNamespaceItem() 
{
    return fXSModel->getNamespaceItem(getNamespace());
}

bool XSSimpleTypeDefinition::getAnonymous() const
{
    return fDatatypeValidator->getAnonymous(); 
}

XSTypeDefinition *XSSimpleTypeDefinition::getBaseType() 
{
    return fBaseType;
}

bool XSSimpleTypeDefinition::derivedFromType(const XSTypeDefinition * const ancestorType, 
                               short derivationMethod)
{
    // REVISIT: review
    // REVISIT: how to check derivationMethod (note: Java doesn't check)...

    if (!ancestorType)
        return false;

    if (ancestorType->getTypeCategory() == XSTypeDefinition::COMPLEX_TYPE)
        return false;

    XSTypeDefinition* type = this;

    while (type && (type != ancestorType))
    {
        type = type->getBaseType();
    }

    return (type == ancestorType);
}

// ---------------------------------------------------------------------------
//  XSSimpleTypeDefinition: initialization methods
// ---------------------------------------------------------------------------
void XSSimpleTypeDefinition::construct()
{
    //REVISIT
/*    // compute fBaseType
    if (fDatatypeValidator->getBaseValidator())
        fBaseType = PSVIUtil::addOrFind(fDatatypeValidator->getBaseValidator(), fXSModel, fMemoryManager);
    
    //REVISIT: the getFixed method is protected so added friend XSSimpleTypeDefinition
    //         to DatatypeValidator class... 
    if (fDatatypeValidator->getType() == DatatypeValidator::Union)
    {
        fVariety = VARIETY_UNION;
        RefVectorOf<DatatypeValidator>* memberTypeValidators = ((UnionDatatypeValidator*)fDatatypeValidator)->getMemberTypeValidators();
        unsigned int size = memberTypeValidators->size();
        if (size)
        {
            fMemberTypes = new (fMemoryManager) RefVectorOf<XSSimpleTypeDefinition>(size, false, fMemoryManager);
            for (unsigned int i=0; i<size; i++)
            {
                fMemberTypes->addElement(
                    PSVIUtil::addOrFind(memberTypeValidators->elementAt(i), fXSModel, fMemoryManager)
                );
            }
        }
    } 
    else if (fDatatypeValidator->getType() == DatatypeValidator::List)
    {
        fVariety = VARIETY_LIST;

        DatatypeValidator* dv = fDatatypeValidator->getBaseValidator();
        while (dv->getType() == DatatypeValidator::List)
        {
            PSVIUtil::addOrFind(dv, fXSModel, fMemoryManager);
            dv = dv->getBaseValidator();
        }
        fPrimitiveOrItemType = PSVIUtil::addOrFind(dv, fXSModel, fMemoryManager);
    }
    else
    {
        // REVISIT: assume ATOMIC but what about VARIETY_ABSENT?
        fVariety = VARIETY_ATOMIC;
        
        DatatypeValidator* dv = fDatatypeValidator->getBaseValidator();
        while (dv)
        {
            fPrimitiveOrItemType = PSVIUtil::addOrFind(dv, fXSModel, fMemoryManager);
            dv = dv->getBaseValidator();
        }
    }

    if (fDatatypeValidator->getFacetsDefined())
        processFacets();*/
}

void XSSimpleTypeDefinition::processFacets()
{
    // REVISIT
/*    bool isFixed = false;
    int facetsDefined = fDatatypeValidator->getFacetsDefined();

    fFixedFacets = fDatatypeValidator->getFixed();

    // NOTE: XSMultiValueFacetList is not owned by XSModel!
    if ((facetsDefined & DatatypeValidator::FACET_PATTERN)
        || (facetsDefined & DatatypeValidator::FACET_ENUMERATION))
        fXSMultiValueFacetList = new (fMemoryManager) RefVectorOf<XSMultiValueFacet>(2, true, fMemoryManager);

    if (facetsDefined & DatatypeValidator::FACET_ENUMERATION)
    {
        RefArrayVectorOf<XMLCh>* enumList = (RefArrayVectorOf<XMLCh>*)
            fDatatypeValidator->getEnumString();
        isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_ENUMERATION);

        // NOTE: Don't need to add multivaluefacet to "ObjectMap -> getObjectFromMap/putObjectInMap);
        fXSMultiValueFacetList->addElement(
            new (fMemoryManager) XSMultiValueFacet(
                FACET_ENUMERATION , enumList , isFixed
                , PSVIUtil::getAnnotationFromModel(fXSModel, enumList)
                , fXSModel, fMemoryManager)
        );
        fDefinedFacets |= FACET_ENUMERATION;
        if (isFixed)
            fFixedFacets |= FACET_ENUMERATION;
    }

    RefHashTableOf<KVStringPair>* facets = fDatatypeValidator->getFacets();
    if (!facets)
        return;

    // NOTE: XSFacetList is not owned by XSModel!
    fXSFacetList = new (fMemoryManager) RefVectorOf<XSFacet>(10, true, fMemoryManager);

    // NOTE: Don't need to add facet to "ObjectMap -> getObjectFromMap/putObjectInMap);
    RefHashTableOfEnumerator<KVStringPair> e(facets);
    while (e.hasMoreElements())
    {
        KVStringPair& pair = e.nextElement();
        XMLCh* key = pair.getKey();
        FACET facetType = FACET_NONE;
        XSAnnotation* annot = PSVIUtil::getAnnotationFromModel(fXSModel, &pair);

        if (XMLString::equals(key, SchemaSymbols::fgELT_MAXINCLUSIVE))
        {
            facetType = FACET_MAXINCLUSIVE;
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_MAXINCLUSIVE);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MAXEXCLUSIVE))
        {
            facetType = FACET_MAXEXCLUSIVE;
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_MAXEXCLUSIVE);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MININCLUSIVE))
        {            
            facetType = FACET_MININCLUSIVE;
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_MININCLUSIVE);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MINEXCLUSIVE))
        {
            facetType = FACET_MINEXCLUSIVE;
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_MINEXCLUSIVE);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_LENGTH))
        {
            facetType = FACET_LENGTH;
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_LENGTH);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MINLENGTH))
        {
            facetType = FACET_MINLENGTH;
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_MINLENGTH);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MAXLENGTH))
        {
            facetType = FACET_MAXLENGTH;
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_MAXLENGTH);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_TOTALDIGITS))
        {
            facetType = FACET_TOTALDIGITS;
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_TOTALDIGITS);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_FRACTIONDIGITS))
        {
            facetType = FACET_FRACTIONDIGITS;
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_FRACTIONDIGITS);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_WHITESPACE))
        {
            facetType = FACET_WHITESPACE;
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_WHITESPACE);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_PATTERN))
        {
            XMLStringTokenizer tokenizer(fDatatypeValidator->getPattern(), &chPipe, fMemoryManager);
            fPatternList = new (fMemoryManager) RefArrayVectorOf<XMLCh>(tokenizer.countTokens(), true, fMemoryManager);
                
            while (tokenizer.hasMoreTokens())
                fPatternList->addElement(XMLString::replicate(tokenizer.nextToken(), fMemoryManager));

            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_PATTERN);
            // NOTE: Don't need to add multivaluefacet to "ObjectMap -> getObjectFromMap/putObjectInMap);
            fXSMultiValueFacetList->addElement(
                new (fMemoryManager) XSMultiValueFacet(
                    FACET_PATTERN, fPatternList, isFixed, annot, fXSModel, fMemoryManager)
	        );
            fDefinedFacets |= FACET_PATTERN;
            if (isFixed) 
                fFixedFacets |= FACET_PATTERN;
            continue;
        }
        else
        {
            // REVISIT: hmm... what about XSSimpleTypeDefinition::FACET_NONE
            // don't think I need to create an empty Facet?
            continue;
        }

        fXSFacetList->addElement(
            new (fMemoryManager) XSFacet(
                facetType, pair.getValue(), isFixed, annot, fXSModel, fMemoryManager)
        );

        fDefinedFacets |= facetType;
        if (isFixed) 
            fFixedFacets |= facetType;
    }*/
}


XERCES_CPP_NAMESPACE_END


