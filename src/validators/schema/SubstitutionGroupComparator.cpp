/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.6  2001/08/21 15:57:51  tng
 * Schema: Add isAllowedByWildcard.  Help from James Murphy.
 *
 * Revision 1.5  2001/05/29 19:47:22  knoaman
 * Fix bug -  memory was not allocated before call to XMLString::subString
 *
 * Revision 1.4  2001/05/28 20:55:42  tng
 * Schema: Null pointer checking in SubsitutionGropuComparator
 *
 * Revision 1.3  2001/05/11 13:27:37  tng
 * Copyright update.
 *
 * Revision 1.2  2001/05/04 14:50:28  tng
 * Fixed the cvs symbols.
 *
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/schema/SubstitutionGroupComparator.hpp>
#include <validators/common/Grammar.hpp>
#include <validators/schema/SchemaGrammar.hpp>
#include <validators/schema/ComplexTypeInfo.hpp>
#include <validators/schema/SchemaSymbols.hpp>

bool SubstitutionGroupComparator::isEquivalentTo(QName* const anElement
                                               , QName* const exemplar)
{

    if ((XMLString::compareString(anElement->getLocalPart(), exemplar->getLocalPart()) == 0) &&
        (anElement->getURI() == exemplar->getURI()))
        return true; // they're the same!

    if (!fGrammarResolver || !fStringPool )
    {
        ThrowXML(RuntimeException, XMLExcepts::SubGrpComparator_NGR);
    }

    unsigned int uriId = anElement->getURI();
    if (uriId == XMLContentModel::gEOCFakeId || 
        uriId == XMLContentModel::gEpsilonFakeId ||
        uriId == XMLElementDecl::fgPCDataElemId)
        return false;

    const XMLCh* uri = fStringPool->getValueForId(uriId);
    const XMLCh* localpart = anElement->getLocalPart();

    // In addition to simply trying to find a chain between anElement and exemplar,
    // we need to make sure that no steps in the chain are blocked.
    // That is, at every step, we need to make sure that the element
    // being substituted for will permit being substituted
    // for, and whether the type of the element will permit derivations in
    // instance documents of this sort.

    if (!uri)
        return false;

    SchemaGrammar *sGrammar = (SchemaGrammar*) fGrammarResolver->getGrammar(uri);
    if (!sGrammar || sGrammar->getGrammarType() == Grammar::DTDGrammarType)
        return false;

    SchemaElementDecl* pElemDecl = (SchemaElementDecl*) sGrammar->getElemDecl(uriId, localpart, 0, Grammar::TOP_LEVEL_SCOPE);
    if (!pElemDecl)
        return false;

    SchemaElementDecl* anElementDecl = pElemDecl;     // to preserve the ElementDecl for anElement
    XMLCh* substitutionGroupFullName = pElemDecl->getSubstitutionGroupName();
    bool foundIt = false;

    while (substitutionGroupFullName)
    {
        int commaAt = XMLString::indexOf(substitutionGroupFullName, chComma);
        XMLCh tmpURI[256];
        XMLCh tmpLocalpart[256];

        if (commaAt >= 0)
        {
            if (commaAt > 0)
                 XMLString::subString(tmpURI, substitutionGroupFullName, 0, commaAt);

            XMLString::subString(tmpLocalpart, substitutionGroupFullName, commaAt+1, XMLString::stringLen(substitutionGroupFullName));
        }
        else {
            XMLString::subString(tmpLocalpart, substitutionGroupFullName, 0, XMLString::stringLen(substitutionGroupFullName));
        }

        if (!tmpURI)
            return false;

        sGrammar = (SchemaGrammar*) fGrammarResolver->getGrammar(tmpURI);
        if (!sGrammar || sGrammar->getGrammarType() == Grammar::DTDGrammarType)
            return false;

        uriId = fStringPool->addOrFind(tmpURI);
        pElemDecl = (SchemaElementDecl*) sGrammar->getElemDecl(uriId, tmpLocalpart, 0, Grammar::TOP_LEVEL_SCOPE);

        if (!pElemDecl)
            return false;

        if ((XMLString::compareString(tmpLocalpart, exemplar->getLocalPart()) == 0) &&
            (uriId == exemplar->getURI()))
        {
            // time to check for block value on element
            if((pElemDecl->getBlockSet() & SchemaSymbols::SUBSTITUTION) != 0)
                return false;

            foundIt = true;
            break;
        }

        substitutionGroupFullName = pElemDecl->getSubstitutionGroupName();
    }//while

    if (!foundIt)
        return false;

    // this will contain anElement's complexType information.
    ComplexTypeInfo *aComplexType = anElementDecl->getComplexTypeInfo();
    int exemplarBlockSet = pElemDecl->getBlockSet();

    if(!aComplexType)
    {
        // check on simpleType case
        DatatypeValidator *anElementDV = anElementDecl->getDatatypeValidator();
        DatatypeValidator *exemplarDV = pElemDecl->getDatatypeValidator();

        return((anElementDV == 0) ||
            ((anElementDV == exemplarDV) ||
            ((exemplarBlockSet & SchemaSymbols::RESTRICTION) == 0)));
    }

    // now we have to make sure there are no blocks on the complexTypes that this is based upon
    int anElementDerivationMethod = aComplexType->getDerivedBy();
    if((anElementDerivationMethod & exemplarBlockSet) != 0)
        return false;

    // this will contain exemplar's complexType information.
    ComplexTypeInfo *exemplarComplexType = pElemDecl->getComplexTypeInfo();

    for(ComplexTypeInfo *tempType = aComplexType;
        tempType != 0 && tempType != exemplarComplexType;
        tempType = tempType->getBaseComplexTypeInfo())
    {
        if((tempType->getBlockSet() & anElementDerivationMethod) != 0)
            return false;
    }//for

    return true;
}


bool SubstitutionGroupComparator::isAllowedByWildcard(QName* const element, unsigned int wuri, bool wother)
{
    // whether the uri is allowed directly by the wildcard
    unsigned int uriId = element->getURI();

    if ((!wother && uriId == wuri) ||
        (wother && uriId != wuri && uriId != XMLContentModel::gEOCFakeId && uriId != XMLContentModel::gEpsilonFakeId))
    {
        return true;
    }

    if (fGrammarResolver == 0 || fStringPool == 0)
         ThrowXML(RuntimeException, XMLExcepts::SubGrpComparator_NGR);

    // get the corresponding grammar
    const XMLCh* uri = fStringPool->getValueForId(uriId);
    if(uri == 0)
        return false;

    SchemaGrammar *sGrammar = (SchemaGrammar*) fGrammarResolver->getGrammar(uri);
    if (!sGrammar || sGrammar->getGrammarType() == Grammar::DTDGrammarType)
        return false;

    if(sGrammar == 0)
        return false;

    // get all elements that can substitute the current element
    RefHash2KeysTableOf<ElemVector>* theValidSubstitutionGroups = sGrammar->getValidSubstitutionGroups();

    if (!theValidSubstitutionGroups)
        return false;

    RefVectorOf<SchemaElementDecl>* subsElements = theValidSubstitutionGroups->get(element->getLocalPart(), uriId);

    if (!subsElements)
        return false;

    // then check whether there exists one element that is allowed by the wildcard
    int size = subsElements->size();

    for (int i = 0; i < size; i++)
    {
        unsigned int subUriId = subsElements->elementAt(i)->getElementName()->getURI();

        if ((!wother && subUriId == wuri) ||
            (wother && subUriId != wuri && subUriId != XMLContentModel::gEOCFakeId && subUriId != XMLContentModel::gEpsilonFakeId))
        {
            return true;
        }
    }
    return false;
}

/**
  * End of file SubstitutionGroupComparator.cpp
  */

