/*
 * Copyright 2003,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Log$
 * Revision 1.3  2004/09/08 13:56:14  peiyongz
 * Apache License Version 2.0
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

#if !defined(XOBJECT_COMPARATOR_HPP)
#define XOBJECT_COMPARATOR_HPP

#include <xercesc/framework/XMLNotationDecl.hpp>
#include <xercesc/framework/XMLRefInfo.hpp>
#include <xercesc/framework/XMLDTDDescription.hpp>
#include <xercesc/framework/XMLSchemaDescription.hpp>

#include <xercesc/internal/XMLGrammarPoolImpl.hpp>

#include <xercesc/util/XMLNumber.hpp>
#include <xercesc/util/KVStringPair.hpp>

#include <xercesc/validators/common/ContentSpecNode.hpp>

#include <xercesc/validators/DTD/DTDAttDef.hpp>
#include <xercesc/validators/DTD/DTDAttDefList.hpp>
#include <xercesc/validators/DTD/DTDElementDecl.hpp>
#include <xercesc/validators/DTD/DTDEntityDecl.hpp>
#include <xercesc/validators/DTD/DTDGrammar.hpp>

#include <xercesc/validators/schema/SchemaAttDef.hpp>
#include <xercesc/validators/schema/SchemaAttDefList.hpp>
#include <xercesc/validators/schema/SchemaElementDecl.hpp>
#include <xercesc/validators/schema/XercesGroupInfo.hpp>
#include <xercesc/validators/schema/XercesAttGroupInfo.hpp>
#include <xercesc/validators/schema/SchemaGrammar.hpp>

#include <xercesc/validators/schema/identity/IC_Field.hpp>
#include <xercesc/validators/schema/identity/IC_Selector.hpp>
#include <xercesc/validators/schema/identity/IC_Key.hpp>
#include <xercesc/validators/schema/identity/IC_KeyRef.hpp>
#include <xercesc/validators/schema/identity/IC_Unique.hpp>
#include <xercesc/validators/schema/identity/IdentityConstraint.hpp>
#include <xercesc/validators/schema/identity/XercesXPath.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLUTIL_EXPORT XObjectComparator
{
public:

/**********************************************************
 *
 * XMLGrammarPoolImpl
 *
 * Grammar
 *
 *   SchemaGrammar
 *   DTDGrammar
 *
 ***********************************************************/   
    static void dumpContent
                (
                    XMLGrammarPoolImpl* const
                );

    static bool isEquivalent
                (
                    XMLGrammarPoolImpl* const
                  , XMLGrammarPoolImpl* const
                );

    static bool isEquivalent
                (
                    Grammar* const
                  , Grammar* const
                );

    static bool isBaseEquivalent
                (
                    Grammar* const
                  , Grammar* const
                );

    static bool isEquivalent
                (
                   SchemaGrammar* const
                 , SchemaGrammar* const
                );

/**********************************************************
 *
 * XMLGrammarDescription
 *
 *   XMLSchemaDescription
 *   XMLDTDDescription
 *
 ***********************************************************/   
    static bool isEquivalent
                (
                    XMLSchemaDescription* const
                  , XMLSchemaDescription* const
                );

    static bool isEquivalent
                (
                   XMLDTDDescription* const
                 , XMLDTDDescription* const
                );

/**********************************************************
 *
 * XMLElementDecl
 *   SchemaElementDecl
 *   DTDElementDecl
 *
 ***********************************************************/    
    static bool isBaseEquivalent
                (
                    XMLElementDecl* const
                  , XMLElementDecl* const
                );

    static bool isEquivalent
                (
                   SchemaElementDecl* const
                 , SchemaElementDecl* const
                );

    static bool isEquivalent
                (
                   DTDElementDecl* const
                 , DTDElementDecl* const
                );

/**********************************************************
 * XMLAttDef
 *   SchemaAttDef
 *   DTDAttDef
 *
***********************************************************/    
    static bool isBaseEquivalent
                (
                   XMLAttDef* const
                 , XMLAttDef* const
                );

    static bool isEquivalent
                (
                    SchemaAttDef* const
                  , SchemaAttDef* const
                );

    static bool isEquivalent
                (
                    DTDAttDef* const
                  , DTDAttDef* const
                );

/**********************************************************
 * XMLAttDefList
 *   SchemaAttDefList
 *   DTDAttDefList
 *
***********************************************************/   
    static bool isBaseEquivalent
                (
                   XMLAttDefList* const
                 , XMLAttDefList* const
                );
    
    static bool isEquivalent
                (
                    SchemaAttDefList* const
                  , SchemaAttDefList* const
                );

    static bool isEquivalent
                (
                    DTDAttDefList* const
                  , DTDAttDefList* const
                );

/**********************************************************
 * XMLEntityDecl
 *    DTDEntityDecl
 *
 ***********************************************************/   
    static bool isBaseEquivalent
                (
                    XMLEntityDecl* const
                  , XMLEntityDecl* const
                );

    static bool isEquivalent
                (
                    DTDEntityDecl* const
                  , DTDEntityDecl* const
                );

/**********************************************************
 * XMLNotationDecl
 *
 * DTDEntityDecl
 *
 * ComplexTypeInfo
 * XercesGroupInfo
 * XercesAttGroupInfo
 ***********************************************************/   
    static bool isEquivalent
                (
                    XMLNotationDecl* const
                  , XMLNotationDecl* const
                );
   
    static bool isEquivalent
                (
                    ComplexTypeInfo* const
                  , ComplexTypeInfo* const
                );

    static bool isEquivalent
                (
                    XercesGroupInfo* const
                  , XercesGroupInfo* const
                );

    static bool isEquivalent
                (
                    XercesAttGroupInfo* const
                  , XercesAttGroupInfo* const
                );

/**********************************************************
 *
 * DatatypeValidator
 *
 *
 * DatatypeValidatorFactory
 *
 ***********************************************************/   

    static bool isEquivalent
                (
                   DatatypeValidator* const
                 , DatatypeValidator* const
                );

    static bool isBaseEquivalent
                (
                   DatatypeValidator* const
                 , DatatypeValidator* const
                );

    static bool isEquivalent
                (
                   DatatypeValidatorFactory* const
                 , DatatypeValidatorFactory* const
                );

/**********************************************************
 *
 * ContentSpecNode
 * QName
 * KVStringPair
 * XMLRefInfo
 * XMLStringPool
 *
 ***********************************************************/   
    static bool isEquivalent
                (
                   ContentSpecNode* const
                 , ContentSpecNode* const
                );

    static bool isEquivalent
                (
                   QName* const
                 , QName* const
                );

    static bool isEquivalent
                (
                   KVStringPair* const
                 , KVStringPair* const
                );

    static bool isEquivalent
                (
                   XMLRefInfo* const
                 , XMLRefInfo* const
                );

    static bool isEquivalent
                (
                   XMLStringPool* const
                 , XMLStringPool* const
                );

/**********************************************************
 *
 * XercesNodeTest
 * XercesStep
 * XercesLocationPath
 * XercesXPath
 *
***********************************************************/   
    static bool isEquivalent
                (
                   XercesNodeTest* const
                 , XercesNodeTest* const
                );

    static bool isEquivalent
                (
                   XercesStep* const
                 , XercesStep* const
                );

    static bool isEquivalent
                (
                   XercesLocationPath* const
                 , XercesLocationPath* const
                );

    static bool isEquivalent
                (
                   XercesXPath* const
                 , XercesXPath* const
                );

/**********************************************************
 *
 * IC_Field
 * IC_Selector
 *
 * IdentityConstraint
 *   IC_Key
 *   IC_KeyRef
 *   IC_Unique
 *
 ***********************************************************/   
    static bool isEquivalent
                (
                   IC_Field* const
                 , IC_Field* const
                );

    static bool isEquivalent
                (
                   IC_Selector* const
                 , IC_Selector* const
                );

    static bool isEquivalent
                (
                   IdentityConstraint* const
                 , IdentityConstraint* const
                );

    static bool isBaseEquivalent
                (
                   IdentityConstraint* const
                 , IdentityConstraint* const
                );

    static bool isEquivalent
                (
                   IC_Key* const
                 , IC_Key* const
                );

    static bool isEquivalent
                (
                   IC_KeyRef* const
                 , IC_KeyRef* const
                );

    static bool isEquivalent
                (
                   IC_Unique* const
                 , IC_Unique* const
                );

/**********************************************************
 * XMLNumber
 *   XMLDouble
 *   XMLFloat
 *   XMLBigDecimal
 *   XMLDateTime
 *
 ***********************************************************/   
    static bool isEquivalent
                (
                   XMLNumber* const
                 , XMLNumber* const
                );

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
	~XObjectComparator();
    XObjectComparator();
    XObjectComparator(const XObjectComparator&);
	XObjectComparator& operator=(const XObjectComparator&);

};

XERCES_CPP_NAMESPACE_END

#endif
