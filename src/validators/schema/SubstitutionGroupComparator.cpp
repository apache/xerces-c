/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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
 * $Log: 
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

bool SubstitutionGroupComparator::isEquivalentTo(const QName& anElement
                                               , const QName& exemplar)
{

    if ((XMLString::compareString(anElement.getLocalPart(), exemplar.getLocalPart()) == 0) && 
        (anElement.getURI() == exemplar.getURI())) 
        return true; // they're the same!

    if (!fGrammarResolver || !fStringPool ) 
    {
        ThrowXML(RuntimeException, XMLExcepts::SubGrpComparator_NGR);
    }

    unsigned int uriId = anElement.getURI();
    const XMLCh* uri = fStringPool->getValueForId(uriId);
    const XMLCh* localpart = anElement.getLocalPart();

    // In addition to simply trying to find a chain between anElement and exemplar,
    // we need to make sure that no steps in the chain are blocked.  
    // That is, at every step, we need to make sure that the element
    // being substituted for will permit being substituted
    // for, and whether the type of the element will permit derivations in
    // instance documents of this sort.

    if (!uri) 
        return false;

    SchemaGrammar *sGrammar = (SchemaGrammar*) fGrammarResolver->getGrammar(uri);
    if (!sGrammar)
        return false;

    SchemaElementDecl* pElemDecl = (SchemaElementDecl*) sGrammar->getElemDecl(uriId, localpart, 0, Grammar::TOP_LEVEL_SCOPE);
    SchemaElementDecl* anElementDecl = pElemDecl;     // to preserve the ElementDecl for anElement
    XMLCh* substitutionGroupFullName = pElemDecl->getSubstitutionGroupName();
    bool foundIt = false;

    while (substitutionGroupFullName) 
    {
        int commaAt = XMLString::indexOf(substitutionGroupFullName, ','); 
        XMLCh* tmpURI = 0;
        XMLCh* tmpLocalpart = substitutionGroupFullName;
        if (commaAt >= 0) 
        {
            if (commaAt > 0) 
                 XMLString::subString(tmpURI, substitutionGroupFullName, 0, commaAt);
            
            XMLString::subString(tmpLocalpart, substitutionGroupFullName, commaAt+1, XMLString::stringLen(substitutionGroupFullName));           
        }

        if (!tmpURI) 
            return false;
        
        sGrammar = (SchemaGrammar*) fGrammarResolver->getGrammar(tmpURI);
        if (!sGrammar) 
            return false;       

        uriId = fStringPool->addOrFind(tmpURI);
        pElemDecl = (SchemaElementDecl*) sGrammar->getElemDecl(uriId, tmpLocalpart, 0, Grammar::TOP_LEVEL_SCOPE);
       
        if (!pElemDecl) 
            return false; 

        if ((XMLString::compareString(tmpLocalpart, exemplar.getLocalPart()) == 0) && 
            (uriId == exemplar.getURI())) 
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
/**
  * End of file SubstitutionGroupComparator.cpp
  */