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
 *
 * $Log$
 * Revision 1.3  2003/11/12 20:29:05  peiyongz
 * removal of fIDRefList
 *
 * Revision 1.2  2003/10/31 22:15:42  peiyongz
 * dumpContent
 *
 * Revision 1.1  2003/10/29 16:14:15  peiyongz
 * XObjectComparator/XTemplateComparator
 *
 * $Id$
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/internal/XObjectComparator.hpp>
#include <xercesc/internal/XTemplateComparator.hpp>

#include <stdio.h>

XERCES_CPP_NAMESPACE_BEGIN

/**********************************************************
 *
 * XMLGrammarPool
 *
 * Grammar
 *
 *   SchemaGrammar
 *   DTDGrammar
 *
 ***********************************************************/   
void XObjectComparator::dumpContent(XMLGrammarPoolImpl* const gramPool)
                                   
{
    RefHashTableOf<Grammar>*  gramReg = gramPool->fGrammarRegistry;
    RefHashTableOfEnumerator<Grammar> eNum(gramReg);
    int itemNumber = 0;        
    while (eNum.hasMoreElements())
    {
        eNum.nextElement();
        itemNumber++;
    }

    printf("itemNumber = <%d>\n", itemNumber);

    //Any thing in the lValue shall be found in the rValue
    eNum.Reset();
    while (eNum.hasMoreElements())
    {
        XMLCh*   key   = (XMLCh*) eNum.nextElementKey();
        char*    keyChar = XMLString::transcode(key);
        printf("key=<%s>\n", keyChar);
        XMLString::release(&keyChar);

        Grammar* data = (Grammar*) gramReg->get(key);
        printf("grammarType = <%d>\n", data->getGrammarType());
    }
}

bool XObjectComparator::isEquivalent(XMLGrammarPoolImpl* const lValue
                                   , XMLGrammarPoolImpl* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

#ifndef _DEBUG

    return (
             XTemplateComparator::isEquivalent(lValue->fGrammarRegistry
                                             , rValue->fGrammarRegistry)  &&
             isEquivalent(lValue->fStringPool, rValue->fStringPool)
           );
#else

    bool v1 = XTemplateComparator::isEquivalent(lValue->fGrammarRegistry
                                              , rValue->fGrammarRegistry);
    bool v2 = isEquivalent(lValue->fStringPool, rValue->fStringPool);

    return v1&&v2;

#endif

}

bool XObjectComparator::isEquivalent(Grammar* const lValue
                                   , Grammar* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    if (lValue->getGrammarType() != rValue->getGrammarType())
        return false;

    if (lValue->getGrammarType() == Grammar::SchemaGrammarType)
    {
        return isEquivalent((SchemaGrammar*)lValue, (SchemaGrammar*)rValue);
    }
    else
    {
        return isEquivalent((DTDGrammar*)lValue, (DTDGrammar*)rValue);
    }

}

bool XObjectComparator::isBaseEquivalent(Grammar* const lValue
                                       , Grammar* const rValue)
{
    return true;
}


bool XObjectComparator::isEquivalent(SchemaGrammar* const lValue
                                   , SchemaGrammar* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    if (!isBaseEquivalent(lValue, rValue))
        return false;

#ifndef _DEBUG
    return (
            (lValue->fValidated == rValue->fValidated)                             &&
            XMLString::equals(lValue->fTargetNamespace, rValue->fTargetNamespace)  &&
            isEquivalent(lValue->fGramDesc, rValue->fGramDesc)                     &&
            isEquivalent(&(lValue->fDatatypeRegistry), 
                         &(rValue->fDatatypeRegistry))                             &&
            XTemplateComparator::isEquivalent(lValue->fElemDeclPool
                                            , rValue->fElemDeclPool)               &&
            XTemplateComparator::isEquivalent(lValue->fElemNonDeclPool
                                            , rValue->fElemNonDeclPool)            &&
            XTemplateComparator::isEquivalent(lValue->fGroupElemDeclPool
                                            , rValue->fGroupElemDeclPool)          &&
            XTemplateComparator::isEquivalent(lValue->fNotationDeclPool
                                            , rValue->fNotationDeclPool)           &&
            XTemplateComparator::isEquivalent(lValue->fAttributeDeclRegistry
                                            , rValue->fAttributeDeclRegistry)      &&
            XTemplateComparator::isEquivalent(lValue->fComplexTypeRegistry
                                            , rValue->fComplexTypeRegistry)        &&
            XTemplateComparator::isEquivalent(lValue->fGroupInfoRegistry
                                            , rValue->fGroupInfoRegistry)          &&
/***
            XTemplateComparator::isEquivalent(lValue->fIDRefList
                                            , rValue->fIDRefList)                  &&
***/
            XTemplateComparator::isEquivalent(lValue->fValidSubstitutionGroups
                                            , rValue->fValidSubstitutionGroups)
          );
#else

    bool v1 = lValue->fValidated == rValue->fValidated;
    bool v2 = XMLString::equals(lValue->fTargetNamespace, rValue->fTargetNamespace);
    bool v3 = isEquivalent(lValue->fGramDesc, rValue->fGramDesc);
    bool v4 = isEquivalent(&(lValue->fDatatypeRegistry), &(rValue->fDatatypeRegistry));
    bool v5 = XTemplateComparator::isEquivalent(lValue->fElemDeclPool
                                              , rValue->fElemDeclPool);
    bool v6 = XTemplateComparator::isEquivalent(lValue->fElemNonDeclPool
                                              , rValue->fElemNonDeclPool);
    bool v7 = XTemplateComparator::isEquivalent(lValue->fGroupElemDeclPool
                                              , rValue->fGroupElemDeclPool) ;
    bool v8 = XTemplateComparator::isEquivalent(lValue->fNotationDeclPool
                                              , rValue->fNotationDeclPool);
    bool v9 = XTemplateComparator::isEquivalent(lValue->fAttributeDeclRegistry
                                              , rValue->fAttributeDeclRegistry);
    bool v10 = XTemplateComparator::isEquivalent(lValue->fComplexTypeRegistry
                                              , rValue->fComplexTypeRegistry);
    bool v11 = XTemplateComparator::isEquivalent(lValue->fGroupInfoRegistry
                                              , rValue->fGroupInfoRegistry);
/***
    bool v12 = XTemplateComparator::isEquivalent(lValue->fIDRefList
                                              , rValue->fIDRefList);
***/
    bool v12 = true;
    bool v13 = XTemplateComparator::isEquivalent(lValue->fValidSubstitutionGroups
                                               , rValue->fValidSubstitutionGroups);

    return v1&&v2&&v3&&v4&&v5&&v6&&v7&&v8&&v9&&v10&&v11&&v12&&v13;

#endif

}

/**********************************************************
 *
 * XMLGrammarDescription
 *
 *   XMLSchemaDescription
 *   XMLDTDDescription
 *
 ***********************************************************/   
bool XObjectComparator::isEquivalent(XMLSchemaDescription* const lValue
                                   , XMLSchemaDescription* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return true;
}

bool XObjectComparator::isEquivalent(XMLDTDDescription* const lValue
                                   , XMLDTDDescription* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return true;
}

/**********************************************************
 *
 * XMLElementDecl
 *   SchemaElementDecl
 *   DTDElementDecl
 *
 ***********************************************************/    
bool XObjectComparator::isBaseEquivalent(XMLElementDecl* const lValue
                                       , XMLElementDecl* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return (
             (lValue->fCreateReason    == rValue->fCreateReason)                     &&
             (lValue->fId              == rValue->fId)                               &&
             (lValue->fExternalElement == rValue->fExternalElement)                  &&
             isEquivalent(lValue->getElementName(), rValue->getElementName())              
           );
}

bool XObjectComparator::isEquivalent(SchemaElementDecl* const lValue
                                   , SchemaElementDecl* const rValue)
{
     IS_EQUIVALENT(lValue, rValue)

    if (!isBaseEquivalent((XMLElementDecl*)lValue, (XMLElementDecl*)rValue))
        return false;

#ifndef _DEBUG
    return (            
            (lValue->fModelType        == rValue->fModelType)             &&
            (lValue->fEnclosingScope   == rValue->fEnclosingScope)        &&
            (lValue->fFinalSet         == rValue->fFinalSet)              &&
            (lValue->fBlockSet         == rValue->fBlockSet)              &&
            (lValue->fMiscFlags        == rValue->fMiscFlags)             &&
            (lValue->fValidity         == rValue->fValidity)              &&
            (lValue->fValidation       == rValue->fValidation)            &&
            (lValue->fSeenValidation   == rValue->fSeenValidation)        &&
            (lValue->fSeenNoValidation == rValue->fSeenNoValidation)      &&
            (lValue->fHadContent       == rValue->fHadContent)            &&
            XMLString::equals(lValue->fDefaultValue
                             , rValue->fDefaultValue)                     &&
            isEquivalent(lValue->fDatatypeValidator
                       , rValue->fDatatypeValidator)                      &&
            isEquivalent((DatatypeValidator*) lValue->fXsiSimpleTypeInfo
                       , (DatatypeValidator*) rValue->fXsiSimpleTypeInfo) &&
            isEquivalent(lValue->fComplexTypeInfo
                       , rValue->fComplexTypeInfo)                        &&
            isEquivalent(lValue->fXsiComplexTypeInfo
                       , rValue->fXsiComplexTypeInfo)                     &&
            isEquivalent(lValue->fAttWildCard
                       , rValue->fAttWildCard)                            &&
            isEquivalent(lValue->fSubstitutionGroupElem
                       , rValue->fSubstitutionGroupElem)                  &&
            XTemplateComparator::isEquivalent(lValue->fAttDefs
                                            , rValue->fAttDefs)           &&
            XTemplateComparator::isEquivalent(lValue->fIdentityConstraints
                                            , rValue->fIdentityConstraints) 
          );                                            
#else

    bool v1 = lValue->fModelType         == rValue->fModelType;
    bool v2 = lValue->fEnclosingScope    == rValue->fEnclosingScope;
    bool v3 = lValue->fFinalSet          == rValue->fFinalSet;
    bool v4 = lValue->fBlockSet          == rValue->fBlockSet;
    bool v5 = lValue->fMiscFlags         == rValue->fMiscFlags;
    bool v6 =  lValue->fValidity         == rValue->fValidity;
    bool v7 =  lValue->fValidation       == rValue->fValidation;
    bool v8 =  lValue->fSeenValidation   == rValue->fSeenValidation;
    bool v9 =  lValue->fSeenNoValidation == rValue->fSeenNoValidation;
    bool v10 = lValue->fHadContent       == rValue->fHadContent;      
    bool v11 = XMLString::equals(lValue->fDefaultValue
                               , rValue->fDefaultValue);
    bool v12 = isEquivalent(lValue->fDatatypeValidator
                          , rValue->fDatatypeValidator);
    bool v13 = isEquivalent((DatatypeValidator*) lValue->fXsiSimpleTypeInfo
                          , (DatatypeValidator*) rValue->fXsiSimpleTypeInfo);
    bool v14 = isEquivalent(lValue->fComplexTypeInfo
                          , rValue->fComplexTypeInfo);
    bool v15 = isEquivalent(lValue->fXsiComplexTypeInfo
                          , rValue->fXsiComplexTypeInfo);
    bool v16 = isEquivalent(lValue->fAttWildCard
                          , rValue->fAttWildCard);
    bool v17 = isEquivalent(lValue->fSubstitutionGroupElem
                          , rValue->fSubstitutionGroupElem);
    bool v18 = XTemplateComparator::isEquivalent(lValue->fAttDefs
                                               , rValue->fAttDefs);
    bool v19 = XTemplateComparator::isEquivalent(lValue->fIdentityConstraints
                                               , rValue->fIdentityConstraints);

    return v1&&v2&&v3&&v4&&v5&&v6&&v7&&v8&&v9&&v10&&v11&&v12&&v13&&v14&&v15&&v16&&v17&&v18&&v19;

#endif

}

bool XObjectComparator::isEquivalent(DTDElementDecl* const lValue
                                   , DTDElementDecl* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    if (!isBaseEquivalent((XMLElementDecl*)lValue, (XMLElementDecl*)rValue))
        return false;

    //don't compare
    //XMLContentModel*            fContentModel;
    //XMLCh*                      fFormattedModel;

    return (            
            (lValue->fModelType == rValue->fModelType)         &&
            isEquivalent(lValue->fContentSpec
                       , rValue->fContentSpec)                 &&
            isEquivalent(lValue->fAttList
                       , rValue->fAttList)                     &&
            XTemplateComparator::isEquivalent(lValue->fAttDefs
                                            , rValue->fAttDefs) 
          );                                            

}

/**********************************************************
 * XMLAttDef
 *   SchemaAttDef
 *   DTDAttDef
 *
***********************************************************/   
bool XObjectComparator::isBaseEquivalent(XMLAttDef* const lValue
                                       , XMLAttDef* const rValue)
{
    return (
             (lValue->fDefaultType       == rValue->fDefaultType)         &&
             (lValue->fType              == rValue->fType)                &&
             (lValue->fCreateReason      == rValue->fCreateReason)        &&
             (lValue->fProvided          == rValue->fProvided)            &&
             (lValue->fExternalAttribute == rValue->fExternalAttribute)   &&
             (lValue->fId                == rValue->fId)                  &&
             XMLString::equals(lValue->fValue,       rValue->fValue)      &&
             XMLString::equals(lValue->fEnumeration, rValue->fEnumeration)     
            );
}

bool XObjectComparator::isEquivalent(SchemaAttDef* const lValue
                                   , SchemaAttDef* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    if (!isBaseEquivalent((XMLAttDef*)lValue, (XMLAttDef*)rValue))
        return false;

    return (
            (lValue->fValidity   == rValue->fValidity)                       &&
            (lValue->fValidation == rValue->fValidation)                     &&
            (lValue->fElemId     == rValue->fElemId)                         &&
            isEquivalent(lValue->fAttName, rValue->fAttName)                 &&
            isEquivalent(lValue->fDatatypeValidator
                                          , rValue->fDatatypeValidator)      &&
            isEquivalent(lValue->fAnyDatatypeValidator
                       , rValue->fAnyDatatypeValidator)                      &&
            isEquivalent((DatatypeValidator*) lValue->fAnyDatatypeValidator
                       , (DatatypeValidator*) rValue->fAnyDatatypeValidator) &&
            XTemplateComparator::isEquivalent(lValue->fNamespaceList
                                            , rValue->fNamespaceList)
           );

}

bool XObjectComparator::isEquivalent(DTDAttDef* const lValue
                                   , DTDAttDef* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    if (!isBaseEquivalent((XMLAttDef*)lValue, (XMLAttDef*)rValue))
        return false;

    return (
            (lValue->fElemId   == rValue->fElemId)          &&
            XMLString::equals(lValue->fName, rValue->fName)
           );

}

/**********************************************************
 * XMLAttDefList
 *   SchemaAttDefList
 *   DTDAttDefList
 *
***********************************************************/   
bool XObjectComparator::isBaseEquivalent(XMLAttDefList* const lValue
                                       , XMLAttDefList* const rValue)
{
     IS_EQUIVALENT(lValue, rValue)

     return true;
}
   
bool XObjectComparator::isEquivalent(SchemaAttDefList* const lValue
                                   , SchemaAttDefList* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    //no base comparision needed

    return XTemplateComparator::isEquivalent(lValue->fList, rValue->fList);    
}

bool XObjectComparator::isEquivalent(DTDAttDefList* const lValue
                                   , DTDAttDefList* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    //no base comparision needed

    return XTemplateComparator::isEquivalent(lValue->fList, rValue->fList);    
}

/**********************************************************
 * XMLEntityDecl
 *    DTDEntityDecl 
 *
 ***********************************************************/   
bool XObjectComparator::isBaseEquivalent(XMLEntityDecl* const lValue
                                       , XMLEntityDecl* const rValue )
{

    return (
             (lValue->fId       == rValue->fId)         &&
             (lValue->fValueLen == rValue->fValueLen)   &&
             XMLString::equals(lValue->fValue
                             , rValue->fValue)          &&
             XMLString::equals(lValue->fName
                             , rValue->fName)           &&
             XMLString::equals(lValue->fNotationName
                             , rValue->fNotationName)   &&
             XMLString::equals(lValue->fPublicId
                             , rValue->fPublicId)       &&
             XMLString::equals(lValue->fSystemId
                             , rValue->fSystemId)       &&
             XMLString::equals(lValue->fBaseURI
                             , rValue->fBaseURI)   
            );

}

bool XObjectComparator::isEquivalent(DTDEntityDecl* const lValue
                                   , DTDEntityDecl* const rValue )
{

    IS_EQUIVALENT(lValue, rValue)

    if (!isBaseEquivalent((XMLEntityDecl*)lValue, (XMLEntityDecl*)rValue))
        return false;

    return (
             (lValue->fDeclaredInIntSubset == rValue->fDeclaredInIntSubset) &&
             (lValue->fIsParameter         == rValue->fIsParameter)         &&
             (lValue->fIsSpecialChar       == rValue->fIsSpecialChar)
           );

}

/**********************************************************
 * XMLNotationDecl
 *
 * XMLEntityDecl
 *
 * ComplexTypeInfo
 * XercesGroupInfo
 * XercesAttGroupInfo
 ***********************************************************/   
bool XObjectComparator::isEquivalent(XMLNotationDecl* const lValue
                                   , XMLNotationDecl* const rValue )
{

    IS_EQUIVALENT(lValue, rValue)

    return (
            (lValue->fId == rValue->fId)                             &&
            XMLString::equals(lValue->fName    , rValue->fName)      &&
            XMLString::equals(lValue->fPublicId, rValue->fPublicId)  &&
            XMLString::equals(lValue->fSystemId, rValue->fSystemId)  &&
            XMLString::equals(lValue->fBaseURI,  rValue->fBaseURI)  
           );

}

bool XObjectComparator::isEquivalent(ComplexTypeInfo* const lValue
                                   , ComplexTypeInfo* const rValue )
{
  /***
    don't compare
    XMLContentModel*                   fContentModel;
    XMLCh*                             fFormattedModel;
    RefVectorOf<ContentSpecNode>*      fSpecNodesToDelete;
    XSDLocator*                        fLocator;
    * 
    * fContentSpecOrgURI:     start of the array
    * fContentSpecOrgURISize: size of the array
    * fUniqueURI:             the current last element in the array
    ***/

    IS_EQUIVALENT(lValue, rValue)

#ifndef _DEBUG
return (
            (lValue->fAnonymous        == rValue->fAnonymous)        &&
            (lValue->fAbstract         == rValue->fAbstract)         &&
            (lValue->fAdoptContentSpec == rValue->fAdoptContentSpec) &&
            (lValue->fAttWithTypeId    == rValue->fAttWithTypeId)    &&
            (lValue->fPreprocessed     == rValue->fPreprocessed)     &&
            (lValue->fDerivedBy        == rValue->fDerivedBy)        &&
            (lValue->fBlockSet         == rValue->fBlockSet)         &&
            (lValue->fFinalSet         == rValue->fFinalSet)         &&
            (lValue->fScopeDefined     == rValue->fScopeDefined)     &&
            (lValue->fElementId        == rValue->fElementId)        &&
            (lValue->fContentType      == rValue->fContentType)      &&
            XMLString::equals(lValue->fTypeName,      rValue->fTypeName)      &&
            XMLString::equals(lValue->fTypeLocalName, rValue->fTypeLocalName) &&
            XMLString::equals(lValue->fTypeUri,       rValue->fTypeUri)       &&
            isEquivalent(lValue->fBaseDatatypeValidator
                       , rValue->fBaseDatatypeValidator)                      &&
            isEquivalent(lValue->fDatatypeValidator
                       , rValue->fDatatypeValidator)                          &&
            isEquivalent(lValue->fBaseComplexTypeInfo
                       , rValue->fBaseComplexTypeInfo)                        &&
            isEquivalent(lValue->fContentSpec
                       , rValue->fContentSpec)                                &&
            isEquivalent(lValue->fAttWildCard
                       , rValue->fAttWildCard)                                &&
            isEquivalent(lValue->fAttList
                       , rValue->fAttList)                                    &&
            XTemplateComparator::isEquivalent(lValue->fElements
                                            , rValue->fElements)              &&
            XTemplateComparator::isEquivalent(lValue->fAttDefs
                                            , rValue->fAttDefs)              
        );

#else

    bool v1 =  lValue->fAnonymous        == rValue->fAnonymous;
    bool v2 =  lValue->fAbstract         == rValue->fAbstract;         
    bool v3 =  lValue->fAdoptContentSpec == rValue->fAdoptContentSpec;
    bool v4 =  lValue->fAttWithTypeId    == rValue->fAttWithTypeId;  
    bool v5 =  lValue->fPreprocessed     == rValue->fPreprocessed;     
    bool v6 =  lValue->fDerivedBy        == rValue->fDerivedBy;      
    bool v7 =  lValue->fBlockSet         == rValue->fBlockSet;         
    bool v8 =  lValue->fFinalSet         == rValue->fFinalSet;          
    bool v9 =  lValue->fScopeDefined     == rValue->fScopeDefined;
    bool v10 = lValue->fElementId        == rValue->fElementId;      
    bool v11 = lValue->fContentType      == rValue->fContentType;
    bool v12 = XMLString::equals(lValue->fTypeName
                               , rValue->fTypeName);
    bool v13 = XMLString::equals(lValue->fTypeLocalName
                               , rValue->fTypeLocalName);
    bool v14 = XMLString::equals(lValue->fTypeUri
                               , rValue->fTypeUri);
    bool v15 = isEquivalent(lValue->fBaseDatatypeValidator
                          , rValue->fBaseDatatypeValidator);
    bool v16 = isEquivalent(lValue->fDatatypeValidator
                          , rValue->fDatatypeValidator);
    bool v17 = isEquivalent(lValue->fBaseComplexTypeInfo
                          , rValue->fBaseComplexTypeInfo);
    bool v18 = isEquivalent(lValue->fContentSpec
                          , rValue->fContentSpec);
    bool v19 = isEquivalent(lValue->fAttWildCard
                          , rValue->fAttWildCard);
    bool v20 = isEquivalent(lValue->fAttList
                          , rValue->fAttList);
    bool v21 = XTemplateComparator::isEquivalent(lValue->fElements
                                               , rValue->fElements);
    bool v22 = XTemplateComparator::isEquivalent(lValue->fAttDefs
                                               , rValue->fAttDefs);
                                                       
    return v1&&v2&&v3&&v4&&v5&&v6&&v7&&v8&&v9&&v10&&v11&&v12&&v13&&v14&&v15&&v16&&v17&&v18&&v19&&v20&&v21&&v22;

#endif

}

bool XObjectComparator::isEquivalent(XercesGroupInfo* const lValue
                                   , XercesGroupInfo* const rValue )
{

    IS_EQUIVALENT(lValue, rValue)

    //    don't compare
    //    XSDLocator*                     fLocator;

    return (
            (lValue->fCheckElementConsistency == rValue->fCheckElementConsistency)  &&
            (lValue->fScope                   == rValue->fScope)                    &&
            isEquivalent(lValue->fContentSpec
                       , rValue->fContentSpec)                                      &&
            isEquivalent(lValue->fBaseGroup
                       , rValue->fBaseGroup)                                        &&
            XTemplateComparator::isEquivalent(lValue->fElements
                                            , rValue->fElements)              
           );

}

bool XObjectComparator::isEquivalent(XercesAttGroupInfo* const lValue
                                   , XercesAttGroupInfo* const rValue )
{

    IS_EQUIVALENT(lValue, rValue)

    return (
            (lValue->fTypeWithId == rValue->fTypeWithId)              &&
            isEquivalent(lValue->fCompleteWildCard
                       , rValue->fCompleteWildCard)                   &&
            XTemplateComparator::isEquivalent(lValue->fAttributes
                                            , rValue->fAttributes)    &&
            XTemplateComparator::isEquivalent(lValue->fAnyAttributes
                                            , rValue->fAnyAttributes)
           );

}

/**********************************************************
 *
 * DatatypeValidator
 *
 *
 * DatatypeValidatorFactory
 *
 ***********************************************************/   
bool XObjectComparator::isEquivalent(DatatypeValidator* const lValue
                                   , DatatypeValidator* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    if (lValue->getType() != rValue->getType())
    {
        return false;
    }

    //todo
    //to call individual isEquivalent according to the actual type
    //
    return true;
}

bool XObjectComparator::isBaseEquivalent(DatatypeValidator* const lValue
                                       , DatatypeValidator* const rValue)
{
    //todo
    return true;
}

bool XObjectComparator::isEquivalent(DatatypeValidatorFactory* const lValue
                                   , DatatypeValidatorFactory* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return XTemplateComparator::isEquivalent(lValue->fUserDefinedRegistry
                                           , rValue->fUserDefinedRegistry);

}

/**********************************************************
 *
 * ContentSpecNode
 * QName
 * KVStringPair
 * XMLRefInfo
 * StringPool
 *
 ***********************************************************/   
bool XObjectComparator::isEquivalent(ContentSpecNode* const lValue
                                   , ContentSpecNode* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return (
            (lValue->fType        == rValue->fType)                        &&
            (lValue->fAdoptFirst  == rValue->fAdoptFirst)                  &&
            (lValue->fAdoptSecond == rValue->fAdoptSecond)                 &&
            (lValue->fMinOccurs   == rValue->fMinOccurs)                   &&
            (lValue->fMaxOccurs   == rValue->fMaxOccurs)                   &&
            isEquivalent(lValue->fElement, rValue->fElement)        &&
            isEquivalent(lValue->fFirst,  rValue->fFirst) &&
            isEquivalent(lValue->fSecond, rValue->fSecond) 
            );

}

bool XObjectComparator::isEquivalent(QName* const lValue
                                   , QName* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return (XMLString::equals(lValue->fPrefix, rValue->fPrefix)       &&
            XMLString::equals(lValue->fLocalPart, rValue->fLocalPart) &&
            (lValue->fURIId == rValue->fURIId)
           );
}

bool XObjectComparator::isEquivalent(KVStringPair* const lValue
                                   , KVStringPair* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return (XMLString::equals(lValue->fKey, rValue->fKey)     &&
            XMLString::equals(lValue->fValue, rValue->fValue)
           );
}

bool XObjectComparator::isEquivalent(XMLRefInfo* const lValue
                                   , XMLRefInfo* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return (
            (lValue->fDeclared == rValue->fDeclared)              &&
            (lValue->fUsed     == rValue->fUsed)                  &&
            XMLString::equals(lValue->fRefName, rValue->fRefName) 
           );
}

bool XObjectComparator::isEquivalent(XMLStringPool* const lValue
                                   , XMLStringPool* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    if (lValue->getStringCount() != rValue->getStringCount())
        return false;

    for (unsigned int i = 1; i < lValue->getStringCount(); i++)
    {
        if (!XMLString::equals(lValue->getValueForId(i), rValue->getValueForId(i)))
            return false;
    }

    return true;
}

/**********************************************************
 *
 * XercesNodeTest
 * XercesStep
 * XercesLocationPath
 * XercesXPath
 *
 ***********************************************************/ 
bool XObjectComparator::isEquivalent(XercesNodeTest* const lValue
                                   , XercesNodeTest* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return (
             (lValue->fType == rValue->fType)           &&
             isEquivalent(lValue->fName, rValue->fName) 
           );
   
}
  
bool XObjectComparator::isEquivalent(XercesStep* const lValue
                                   , XercesStep* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return (
             (lValue->fAxisType == rValue->fAxisType)  &&
             isEquivalent(lValue->fNodeTest, rValue->fNodeTest) 
           );
   
}

bool XObjectComparator::isEquivalent(XercesLocationPath* const lValue
                                   , XercesLocationPath* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return (
             XTemplateComparator::isEquivalent(lValue->fSteps, rValue->fSteps) 
           );
   
}

bool XObjectComparator::isEquivalent(XercesXPath* const lValue
                                   , XercesXPath* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return (
             (lValue->fEmptyNamespaceId == rValue->fEmptyNamespaceId)  &&
             XMLString::equals(lValue->fExpression
                             , rValue->fExpression)                    &&
             XTemplateComparator::isEquivalent(lValue->fLocationPaths
                                             , rValue->fLocationPaths) 
            );
   
}

/**********************************************************
 *
 * IC_Field
 * IC_Select
 *
 * IdentityConstraint
 *   IC_Key
 *   IC_KeyRef
 *   IC_Unique
 *
 *
 ***********************************************************/   
bool XObjectComparator::isEquivalent(IC_Field* const lValue
                                   , IC_Field* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return (
             (lValue->fMayMatch == rValue->fMayMatch)  &&
             isEquivalent(lValue->fXPath
                        , rValue->fXPath)              &&
             isEquivalent(lValue->fIdentityConstraint
                        , rValue->fIdentityConstraint) 
            );
   
}

bool XObjectComparator::isEquivalent(IC_Selector* const lValue
                                   , IC_Selector* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    return (
             isEquivalent(lValue->fXPath
                        , rValue->fXPath)              &&
             isEquivalent(lValue->fIdentityConstraint
                        , rValue->fIdentityConstraint) 
            );
   
}

bool XObjectComparator::isEquivalent(IdentityConstraint* const lValue
                                   , IdentityConstraint* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    if (lValue->getType() != rValue->getType())
    {
        //throw exception here
        return false;
    }

    switch(lValue->getType())
    {
    case IdentityConstraint::UNIQUE: 
        return isEquivalent((IC_Unique*)lValue, (IC_Unique*)rValue);
        break;
    case IdentityConstraint::KEY:
        return isEquivalent((IC_Key*)lValue, (IC_Key*)rValue);
        break;
    case IdentityConstraint::KEYREF: 
        return isEquivalent((IC_KeyRef*)lValue, (IC_KeyRef*)rValue);
        break;
    default: 
        //throw exception here
        return false;
        break;
    }
  
}

bool XObjectComparator::isBaseEquivalent(IdentityConstraint* const lValue
                                       , IdentityConstraint* const rValue)
{

    return (
             XMLString::equals(lValue->fIdentityConstraintName
                             , rValue->fIdentityConstraintName)    &&
             XMLString::equals(lValue->fElemName
                             , rValue->fElemName)                  &&
             isEquivalent(lValue->fSelector, rValue->fSelector)    &&
             XTemplateComparator::isEquivalent(lValue->fFields
                                             , rValue->fFields)
            );         
                     
}

bool XObjectComparator::isEquivalent(IC_Key* const lValue
                                   , IC_Key* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    if (!isBaseEquivalent(lValue, rValue))
        return false;

    // no data
    return true;
}

bool XObjectComparator::isEquivalent(IC_KeyRef* const lValue
                                   , IC_KeyRef* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    if (!isBaseEquivalent(lValue, rValue))
        return false;

    return isEquivalent(lValue->fKey, rValue->fKey);

}

bool XObjectComparator::isEquivalent(IC_Unique* const lValue
                                   , IC_Unique* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    if (!isBaseEquivalent(lValue, rValue))
        return false;

    // no data
    return true;
}

/**********************************************************
 * XMLNumber
 *   XMLDouble
 *   XMLFloat
 *   XMLBigDecimal
 *   XMLDateTime
 *
 ***********************************************************/   
bool XObjectComparator::isEquivalent(XMLNumber* const lValue
                                   , XMLNumber* const rValue)
{
    IS_EQUIVALENT(lValue, rValue)

    //to do, to introduce numberType later
    return true;
}

XERCES_CPP_NAMESPACE_END

