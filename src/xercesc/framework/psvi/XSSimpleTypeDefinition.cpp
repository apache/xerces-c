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
 * Revision 1.3  2003/11/14 22:33:30  neilg
 * ./src/xercesc/framework/psvi/XSAnnotation.cpp
 * ./src/xercesc/framework/psvi/XSAnnotation.hpp
 * ./src/xercesc/framework/psvi/XSAttributeDeclaration.cpp
 * ./src/xercesc/framework/psvi/XSAttributeDeclaration.hpp
 * ./src/xercesc/framework/psvi/XSAttributeGroupDefinition.cpp
 * ./src/xercesc/framework/psvi/XSAttributeGroupDefinition.hpp
 * ./src/xercesc/framework/psvi/XSAttributeUse.cpp
 * ./src/xercesc/framework/psvi/XSAttributeUse.hpp
 * ./src/xercesc/framework/psvi/XSComplexTypeDefinition.cpp
 * ./src/xercesc/framework/psvi/XSComplexTypeDefinition.hpp
 * ./src/xercesc/framework/psvi/XSElementDeclaration.cpp
 * ./src/xercesc/framework/psvi/XSElementDeclaration.hpp
 * ./src/xercesc/framework/psvi/XSFacet.cpp
 * ./src/xercesc/framework/psvi/XSFacet.hpp
 * ./src/xercesc/framework/psvi/XSIDCDefinition.cpp
 * ./src/xercesc/framework/psvi/XSIDCDefinition.hpp
 * ./src/xercesc/framework/psvi/XSModel.cpp
 * ./src/xercesc/framework/psvi/XSModel.hpp
 * ./src/xercesc/framework/psvi/XSModelGroup.cpp
 * ./src/xercesc/framework/psvi/XSModelGroup.hpp
 * ./src/xercesc/framework/psvi/XSModelGroupDefinition.cpp
 * ./src/xercesc/framework/psvi/XSModelGroupDefinition.hpp
 * ./src/xercesc/framework/psvi/XSMultiValueFacet.cpp
 * ./src/xercesc/framework/psvi/XSMultiValueFacet.hpp
 * ./src/xercesc/framework/psvi/XSNamespaceItem.cpp
 * ./src/xercesc/framework/psvi/XSNamespaceItem.hpp
 * ./src/xercesc/framework/psvi/XSNotationDeclaration.cpp
 * ./src/xercesc/framework/psvi/XSNotationDeclaration.hpp
 * ./src/xercesc/framework/psvi/XSObject.cpp
 * ./src/xercesc/framework/psvi/XSObject.hpp
 * ./src/xercesc/framework/psvi/XSParticle.cpp
 * ./src/xercesc/framework/psvi/XSParticle.hpp
 * ./src/xercesc/framework/psvi/XSSimpleTypeDefinition.cpp
 * ./src/xercesc/framework/psvi/XSSimpleTypeDefinition.hpp
 * ./src/xercesc/framework/psvi/XSTypeDefinition.cpp
 * ./src/xercesc/framework/psvi/XSTypeDefinition.hpp
 * ./src/xercesc/framework/psvi/XSWildcard.cpp
 * ./src/xercesc/framework/psvi/XSWildcard.hpp
 * ./src/xercesc/internal/XMLGrammarPoolImpl.cpp
 * ./src/xercesc/internal/XMLGrammarPoolImpl.hpp
 * ./src/xercesc/validators/schema/identity/IdentityConstraint.cpp
 * ./src/xercesc/validators/schema/identity/IdentityConstraint.hpp
 * ./src/xercesc/validators/schema/SchemaGrammar.hpp
 * ./src/xercesc/validators/schema/TraverseSchema.cpp
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
#include <xercesc/validators/datatype/DatatypeValidator.hpp>
#include <xercesc/validators/datatype/UnionDatatypeValidator.hpp>
#include <xercesc/util/XMLStringTokenizer.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN

static bool XSSimpleTypeDefinitionTestFlag(int flag) 
{
    if (flag)
        return true;
    return false;
}


XSSimpleTypeDefinition::XSSimpleTypeDefinition(DatatypeValidator*   datatypeValidator,
                                               XSModel*             xsModel,
                                               MemoryManager* const manager):
        fDatatypeValidator(datatypeValidator),
        fXSFacetList(0),
        fXSMultiValueFacetList(0),
        fPatternList(0),
        fDefinedFacets(0),
        fFixedFacets(0),
        fPrimitiveOrItemType(0),
        fMemberTypes(0),
        fXSAnnotationList(0),
        XSTypeDefinition(SIMPLE_TYPE, xsModel, manager) 
{
    if (int finalset = fDatatypeValidator->getFinalSet()) 
    {
        if (finalset & SchemaSymbols::XSD_EXTENSION)
        {
            fFinal |= XSConstants::DERIVATION_EXTENSION;
        }
        if (finalset & SchemaSymbols::XSD_RESTRICTION)
        {
            fFinal |= XSConstants::DERIVATION_RESTRICTION;
        }
        if (finalset & SchemaSymbols::XSD_LIST)
        {
            fFinal |= XSConstants::DERIVATION_LIST;
        }
        if (finalset & SchemaSymbols::XSD_UNION)
        {
            fFinal |= XSConstants::DERIVATION_UNION;
        }
    }

    // compute fBaseType
    if (fDatatypeValidator->getBaseValidator())
    {
        fBaseType = (XSTypeDefinition*) getObjectFromMap(fDatatypeValidator->getBaseValidator());
        if (!fBaseType)
        {
            fBaseType = new (fMemoryManager) XSSimpleTypeDefinition(fDatatypeValidator->getBaseValidator(), fXSModel, fMemoryManager);
            putObjectInMap((void*)fDatatypeValidator->getBaseValidator(), fBaseType);
        }
    }    
    
    
    //REVISIT: the getFixed method is protected so added friend XSSimpleTypeDefinition
    //         to DatatypeValidator class... 

    if (fDatatypeValidator->getType() == DatatypeValidator::Union)
    {
        fVariety = VARIETY_UNION;
        RefVectorOf<DatatypeValidator>* memberTypeValidators = 
                ((UnionDatatypeValidator*)fDatatypeValidator)->getMemberTypeValidators();

        unsigned int size = memberTypeValidators->size();
        if (size) {
            fMemberTypes = new (manager) RefVectorOf <XSSimpleTypeDefinition>(size, false, manager);

            for (unsigned int i=0; i<size; i++) 
            {
                XSSimpleTypeDefinition* simpleType = (XSSimpleTypeDefinition*) getObjectFromMap((void*)memberTypeValidators->elementAt(i));
                if (!simpleType)
                {
                    simpleType = new (manager) XSSimpleTypeDefinition(memberTypeValidators->elementAt(i), fXSModel, manager);
                    putObjectInMap((void*)memberTypeValidators->elementAt(i), simpleType);
                }
                fMemberTypes->addElement(simpleType);
            }
        }

    } 
    else if (fDatatypeValidator->getType() == DatatypeValidator::List)
    {
        fVariety = VARIETY_LIST;

        DatatypeValidator* dv = fDatatypeValidator->getBaseValidator();

        while (dv->getType() == DatatypeValidator::List)
        {
            fPrimitiveOrItemType = (XSSimpleTypeDefinition*) getObjectFromMap(dv);
            if (!fPrimitiveOrItemType)
            {
                fPrimitiveOrItemType = new (manager) XSSimpleTypeDefinition(dv, fXSModel, manager);
                putObjectInMap((void*)dv, fPrimitiveOrItemType);
            }
            dv = dv->getBaseValidator();
        }
        fPrimitiveOrItemType = (XSSimpleTypeDefinition*) getObjectFromMap(dv);
        if (!fPrimitiveOrItemType)
        {
            fPrimitiveOrItemType = new (manager) XSSimpleTypeDefinition(dv, fXSModel, manager);
            putObjectInMap((void*)dv, fPrimitiveOrItemType);
        }
    }
    else
    {
        // REVISIT: assume ATOMIC but what about VARIETY_ABSENT?
        fVariety = VARIETY_ATOMIC;
        
        DatatypeValidator* dv = fDatatypeValidator->getBaseValidator();

        while (dv != 0)
        {
            fPrimitiveOrItemType = (XSSimpleTypeDefinition*) getObjectFromMap(dv);
            if (!fPrimitiveOrItemType)
            {
                fPrimitiveOrItemType = new (manager) XSSimpleTypeDefinition(dv, fXSModel, manager);
                putObjectInMap((void*)dv, fPrimitiveOrItemType);
            }
            dv = dv->getBaseValidator();
        }
    } 
    
    fFixedFacets = fDatatypeValidator->getFixed();
    bool isFixed;

    // NOTE: XSMultiValueFacetList is not owned by XSModel!
    fXSMultiValueFacetList = new (manager) RefVectorOf <XSMultiValueFacet> (2, true, manager);
    if (fDatatypeValidator->getFacetsDefined() & DatatypeValidator::FACET_PATTERN)
    {
        const XMLCh* pattern = fDatatypeValidator->getPattern();
        XMLStringTokenizer tokenizer(pattern, &chPipe, manager);
        fPatternList = new (manager) RefArrayVectorOf <XMLCh>(tokenizer.countTokens(), true, manager);                   
                
        while (tokenizer.hasMoreTokens()) {
            XMLCh* pattern = XMLString::replicate(tokenizer.nextToken(), manager);
            fPatternList->addElement(pattern);
        }
        isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_PATTERN);
        // NOTE: Don't need to add multivaluefacet to "ObjectMap -> getObjectFromMap/putObjectInMap);
        fXSMultiValueFacetList->addElement(new XSMultiValueFacet(FACET_PATTERN, fPatternList, isFixed, fXSModel, manager));
        fDefinedFacets |= FACET_PATTERN;
        if (isFixed) 
        {
            fFixedFacets |= FACET_PATTERN;
        }
    }

    if (fDatatypeValidator->getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION)
    {
        isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_ENUMERATION);        
        // NOTE: Don't need to add multivaluefacet to "ObjectMap -> getObjectFromMap/putObjectInMap);
        fXSMultiValueFacetList->addElement(new XSMultiValueFacet(FACET_ENUMERATION, (RefArrayVectorOf<XMLCh>*) fDatatypeValidator->getEnumString(), isFixed, fXSModel, manager));
        fDefinedFacets |= FACET_ENUMERATION;
        if (isFixed) 
        {
            fFixedFacets |= FACET_ENUMERATION;
        }
    }


    RefHashTableOf<KVStringPair>* facets = fDatatypeValidator->getFacets();

    if (!facets)
        return;

    // NOTE: XSFacetList is not owned by XSModel!
    fXSFacetList = new (manager) RefVectorOf <XSFacet> (10, true, manager);

    XMLCh* key;
    XMLCh* value;
    RefHashTableOfEnumerator<KVStringPair> e(facets);
    // NOTE: Don't need to add facet to "ObjectMap -> getObjectFromMap/putObjectInMap);

    while (e.hasMoreElements())
    {
        KVStringPair pair = e.nextElement();
        key = pair.getKey();
        value = pair.getValue();

        if (XMLString::equals(key, SchemaSymbols::fgELT_MAXINCLUSIVE))
        {
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_MAXINCLUSIVE);        
            fXSFacetList->addElement(new XSFacet(FACET_MAXINCLUSIVE, value, isFixed, fXSModel, manager));
            fDefinedFacets |= FACET_MAXINCLUSIVE;
            if (isFixed) 
            {
                fFixedFacets |= FACET_MAXINCLUSIVE;
            }
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MAXEXCLUSIVE))
        {
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_MAXEXCLUSIVE);        
            fXSFacetList->addElement(new XSFacet(FACET_MAXEXCLUSIVE, value, isFixed, fXSModel, manager));
            fDefinedFacets |= FACET_MAXEXCLUSIVE;
            if (isFixed) 
            {
                fFixedFacets |= FACET_MAXEXCLUSIVE;
            }
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MININCLUSIVE))
        {            
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_MININCLUSIVE);        
            fXSFacetList->addElement(new XSFacet(FACET_MININCLUSIVE, value, isFixed, fXSModel, manager));
            fDefinedFacets |= FACET_MININCLUSIVE;
            if (isFixed) 
            {
                fFixedFacets |= FACET_MININCLUSIVE;
            }
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MINEXCLUSIVE))
        {
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_MINEXCLUSIVE);        
            fXSFacetList->addElement(new XSFacet(FACET_MINEXCLUSIVE, value, isFixed, fXSModel, manager));
            fDefinedFacets |= FACET_MINEXCLUSIVE;
            if (isFixed) 
            {
                fFixedFacets |= FACET_MINEXCLUSIVE;
            }
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_LENGTH))
        {
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_LENGTH);        
            fXSFacetList->addElement(new XSFacet(FACET_LENGTH, value, isFixed, fXSModel, manager));
            fDefinedFacets |= FACET_LENGTH;
            if (isFixed) 
            {
                fFixedFacets |= FACET_LENGTH;
            }
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MINLENGTH))
        {
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_MINLENGTH);        
            fXSFacetList->addElement(new XSFacet(FACET_MINLENGTH, value, isFixed, fXSModel, manager));
            fDefinedFacets |= FACET_MINLENGTH;
            if (isFixed) 
            {
                fFixedFacets |= FACET_MINLENGTH;
            }
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MAXLENGTH))
        {
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_MAXLENGTH);        
            fXSFacetList->addElement(new XSFacet(FACET_MAXLENGTH, value, isFixed, fXSModel, manager));
            fDefinedFacets |= FACET_MAXLENGTH;
            if (isFixed) 
            {
                fFixedFacets |= FACET_MAXLENGTH;
            }
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_TOTALDIGITS))
        {
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_TOTALDIGITS);        
            fXSFacetList->addElement(new XSFacet(FACET_TOTALDIGITS, value, isFixed, fXSModel, manager));
            fDefinedFacets |= FACET_TOTALDIGITS;
            if (isFixed) 
            {
                fFixedFacets |= FACET_TOTALDIGITS;
            }
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_FRACTIONDIGITS))
        {
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_FRACTIONDIGITS);        
            fXSFacetList->addElement(new XSFacet(FACET_FRACTIONDIGITS, value, isFixed, fXSModel, manager));
            fDefinedFacets |= FACET_FRACTIONDIGITS;
            if (isFixed) 
            {
                fFixedFacets |= FACET_FRACTIONDIGITS;
            }
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_WHITESPACE))
        {
            isFixed = XSSimpleTypeDefinitionTestFlag(fFixedFacets & DatatypeValidator::FACET_WHITESPACE);        
            fXSFacetList->addElement(new XSFacet(FACET_WHITESPACE, value, isFixed, fXSModel, manager));
            fDefinedFacets |= FACET_WHITESPACE;
            if (isFixed) 
            {
                fFixedFacets |= FACET_WHITESPACE;
            }
        }
  
        // REVISIT: hmm... what about XSSimpleTypeDefinition::FACET_NONE don't think I need
        // to create an empty Facet?
    }
}

XSSimpleTypeDefinition::~XSSimpleTypeDefinition() {
    if (fXSFacetList) 
    {
        delete fXSFacetList;
    }
    if (fXSMultiValueFacetList) 
    {
        delete fXSMultiValueFacetList;
    }
    if (fPatternList) 
    {
        delete fPatternList;
    }
    // don't delete fPrimitiveOrItemType -> deleted by XSModel
    if (fMemberTypes)
    {        
        delete fMemberTypes;
    }
    if (fXSAnnotationList)
    {
        delete fXSAnnotationList;
    }
}

// XSSimpleTypeDefinition methods


/**
 * [variety]: one of {atomic, list, union} or absent 
 */
XSSimpleTypeDefinition::VARIETY XSSimpleTypeDefinition::getVariety() const
{
    return fVariety;
}

/**
 * If variety is <code>atomic</code> the primitive type definition (a 
 * built-in primitive datatype definition or the simple ur-type 
 * definition) is available, otherwise <code>null</code>. 
 */
XSSimpleTypeDefinition *XSSimpleTypeDefinition::getPrimitiveType()
{
    if (fVariety == VARIETY_ATOMIC)
    {
        return fPrimitiveOrItemType;
    }
    return 0;
}

/**
 * If variety is <code>list</code> the item type definition (an atomic or 
 * union simple type definition) is available, otherwise 
 * <code>null</code>. 
 */
XSSimpleTypeDefinition *XSSimpleTypeDefinition::getItemType()
{
    if (fVariety == VARIETY_LIST)
    {
        return fPrimitiveOrItemType;
    }
    return 0;
}

/**
 * If variety is <code>union</code> the list of member type definitions (a 
 * non-empty sequence of simple type definitions) is available, 
 * otherwise <code>null</code>. 
 */
XSSimpleTypeDefinitionList *XSSimpleTypeDefinition::getMemberTypes()
{
    return fMemberTypes;
}

/**
 * [facets]: get all facets defined on this type. The value is a bit 
 * combination of FACET_XXX constants of all defined facets. 
 */
short XSSimpleTypeDefinition::getDefinedFacets()
{
    return fDefinedFacets;
}

/**
 * Convenience method. [Facets]: check whether a facet is defined on this 
 * type.
 * @param facetName  The name of the facet. 
 * @return  True if the facet is defined, false otherwise.
 */
bool XSSimpleTypeDefinition::isDefinedFacet(FACET facetName)
{
    return XSSimpleTypeDefinitionTestFlag(fDefinedFacets & facetName);
}

/**
 * [facets]: get all facets defined and fixed on this type.
 */
short XSSimpleTypeDefinition::getFixedFacets()
{
    return fFixedFacets;
}

/**
 * Convenience method. [Facets]: check whether a facet is defined and 
 * fixed on this type. 
 * @param facetName  The name of the facet. 
 * @return  True if the facet is fixed, false otherwise.
 */
bool XSSimpleTypeDefinition::isFixedFacet(FACET facetName)
{
    return XSSimpleTypeDefinitionTestFlag(fFixedFacets & facetName);
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

/**
 * Returns a list of enumeration values. 
 */
StringList *XSSimpleTypeDefinition::getLexicalEnumeration()
{
    return (RefArrayVectorOf<XMLCh>*) fDatatypeValidator->getEnumString();
}

/**
 * Returns a list of pattern values. 
 */
StringList *XSSimpleTypeDefinition::getLexicalPattern()
{
    return fPatternList;
}

/**
 *  Fundamental Facet: ordered 
 */
XSSimpleTypeDefinition::ORDERING XSSimpleTypeDefinition::getOrdered() const
{
    return fDatatypeValidator->getOrdered();
}

/**
 * Fundamental Facet: cardinality. 
 */
bool XSSimpleTypeDefinition::getFinite() const
{
    return fDatatypeValidator->getFinite();
}

/**
 * Fundamental Facet: bounded. 
 */
bool XSSimpleTypeDefinition::getBounded() const
{
    return fDatatypeValidator->getBounded();
}

/**
 * Fundamental Facet: numeric. 
 */
bool XSSimpleTypeDefinition::getNumeric() const
{
    return fDatatypeValidator->getNumeric();
}

/**
 * Optional. A set of [annotation]s. 
 */
XSAnnotationList *XSSimpleTypeDefinition::getAnnotations()
{
    if (fXSAnnotationList)
    {
        return fXSAnnotationList;    
    }
    // REVISIT Size
    fXSAnnotationList = new (fMemoryManager) RefVectorOf <XSAnnotation> (3, false, fMemoryManager);
    XSAnnotation* annot = getAnnotationFromModel(fDatatypeValidator);
    while (annot)
    {
        fXSAnnotationList->addElement(annot);
        annot = annot->getNext();
    }
    return fXSAnnotationList;
}

/** 
 * @return list of constraining facets.
 * This method must not be used to retrieve 
 * values for <code>enumeration</code> and <code>pattern</code> facets.
 */
XSFacetList *XSSimpleTypeDefinition::getFacets()
{
    return fXSFacetList;
}

/** 
 * @return list of enumeration and pattern facets.
 */
XSMultiValueFacetList *XSSimpleTypeDefinition::getMultiValueFacets()
{
    return fXSMultiValueFacetList;
}

/**
 * virtual function's from XSTypeDefinition
 */
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
    return getNamespaceItemFromHash(getNamespace());
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


XERCES_CPP_NAMESPACE_END


