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
 * Revision 1.2  2004/09/08 13:56:14  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.1  2003/10/29 16:14:15  peiyongz
 * XObjectComparator/XTemplateComparator
 *
 * $Id$
 *
 */

#if !defined(XTEMPLATE_COMPARATOR_HPP)
#define XTEMPLATE_COMPARATOR_HPP

#include <xercesc/util/ValueVectorOf.hpp>
#include <xercesc/util/RefArrayVectorOf.hpp>
#include <xercesc/util/RefVectorOf.hpp>
#include <xercesc/util/RefHashTableOf.hpp>
#include <xercesc/util/RefHash2KeysTableOf.hpp>
#include <xercesc/util/RefHash3KeysIdPool.hpp>
#include <xercesc/util/NameIdPool.hpp>

#include <xercesc/framework/XMLNotationDecl.hpp>
#include <xercesc/framework/XMLRefInfo.hpp>
#include <xercesc/util/XMLNumber.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/DTD/DTDAttDef.hpp>
#include <xercesc/validators/DTD/DTDElementDecl.hpp>
#include <xercesc/validators/DTD/DTDEntityDecl.hpp>
#include <xercesc/validators/schema/SchemaAttDef.hpp>
#include <xercesc/validators/schema/SchemaElementDecl.hpp>
#include <xercesc/validators/schema/XercesGroupInfo.hpp>
#include <xercesc/validators/schema/XercesAttGroupInfo.hpp>
#include <xercesc/validators/schema/SchemaGrammar.hpp>
#include <xercesc/validators/schema/identity/IC_Field.hpp>
#include <xercesc/validators/schema/identity/IdentityConstraint.hpp>
#include <xercesc/validators/schema/identity/XercesXPath.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLUTIL_EXPORT XTemplateComparator
{
public:

    /**********************************************************
     *
     * ValueVectorOf
     *
     *   SchemaElementDecl*
     *   unsigned int
     *
     ***********************************************************/   
    static bool           isEquivalent(
                                          ValueVectorOf<SchemaElementDecl*>* const lValue
                                        , ValueVectorOf<SchemaElementDecl*>* const rValue
                                       );

    static bool           isEquivalent(
                                          ValueVectorOf<unsigned int>* const lValue
                                        , ValueVectorOf<unsigned int>* const rValue
                                       );

    /**********************************************************
     *
     * RefArrayVectorOf
     *
     *   XMLCh
     *
     ***********************************************************/   
    static bool           isEquivalent(
                                          RefArrayVectorOf<XMLCh>* const lValue
                                        , RefArrayVectorOf<XMLCh>* const rValue
                                       );

    /**********************************************************
     *
     * RefVectorOf
     *
     *   SchemaAttDef
     *   SchemaElementDecl
     *   ContentSpecNode
     *   IC_Field
     *   DatatypeValidator
     *   IdentityConstraint
     *   XMLNumber
     *   XercesLocationPath
     *   XercesStep
     *
     ***********************************************************/
    static bool           isEquivalent(
                                          RefVectorOf<SchemaAttDef>* const lValue
                                        , RefVectorOf<SchemaAttDef>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefVectorOf<SchemaElementDecl>* const lValue
                                        , RefVectorOf<SchemaElementDecl>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefVectorOf<ContentSpecNode>* const lValue
                                        , RefVectorOf<ContentSpecNode>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefVectorOf<IC_Field>* const lValue
                                        , RefVectorOf<IC_Field>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefVectorOf<DatatypeValidator>* const lValue
                                        , RefVectorOf<DatatypeValidator>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefVectorOf<IdentityConstraint>* const lValue
                                        , RefVectorOf<IdentityConstraint>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefVectorOf<XMLNumber>* const lValue
                                        , RefVectorOf<XMLNumber>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefVectorOf<XercesLocationPath>* const lValue
                                        , RefVectorOf<XercesLocationPath>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefVectorOf<XercesStep>* const lValue
                                        , RefVectorOf<XercesStep>* const rValue
                                       );

    /**********************************************************
     *
     * RefHashTableOf
     *
     *   KVStringPair
     *   XMLAttDef
     *   DTDAttDef
     *   ComplexTypeInfo
     *   XercesGroupInfo
     *   XercesAttGroupInfo
     *   XMLRefInfo
     *   DatatypeValidator
     *   Grammar
     *
     ***********************************************************/
    static bool           isEquivalent(
                                          RefHashTableOf<KVStringPair>* const lValue
                                        , RefHashTableOf<KVStringPair>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefHashTableOf<XMLAttDef>* const lValue
                                        , RefHashTableOf<XMLAttDef>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefHashTableOf<DTDAttDef>* const lValue
                                        , RefHashTableOf<DTDAttDef>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefHashTableOf<ComplexTypeInfo>* const lValue
                                        , RefHashTableOf<ComplexTypeInfo>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefHashTableOf<XercesGroupInfo>* const lValue
                                        , RefHashTableOf<XercesGroupInfo>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefHashTableOf<XercesAttGroupInfo>* const lValue
                                        , RefHashTableOf<XercesAttGroupInfo>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefHashTableOf<XMLRefInfo>* const lValue
                                        , RefHashTableOf<XMLRefInfo>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefHashTableOf<DatatypeValidator>* const lValue
                                        , RefHashTableOf<DatatypeValidator>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefHashTableOf<Grammar>* const lValue
                                        , RefHashTableOf<Grammar>* const rValue
                                       );

    /**********************************************************
     *
     * RefHash2KeysTableOf
     *
     *   SchemaAttDef
     *   ElemVector
     *
     ***********************************************************/
    static bool           isEquivalent(
                                          RefHash2KeysTableOf<SchemaAttDef>* const lValue
                                        , RefHash2KeysTableOf<SchemaAttDef>* const rValue
                                       );

    static bool           isEquivalent(
                                          RefHash2KeysTableOf<ElemVector>* const lValue
                                        , RefHash2KeysTableOf<ElemVector>* const rValue
                                       );

    /**********************************************************
     *
     * RefHash3KeysIdPool
     *
     *   SchemaElementDecl
     *
     ***********************************************************/
    static bool           isEquivalent(
                                         RefHash3KeysIdPool<SchemaElementDecl>* const lop
                                       , RefHash3KeysIdPool<SchemaElementDecl>* const rop
                                      );


    /**********************************************************
     *
     * NameIdPool
     *
     *   DTDElementDecl
     *   DTDEntityDecl
     *   XMLNotationDecl
     *
     ***********************************************************/
    static bool           isEquivalent(
                                          NameIdPool<DTDElementDecl>* const lValue
                                        , NameIdPool<DTDElementDecl>* const rValue
                                       );

    static bool           isEquivalent(
                                          NameIdPool<DTDEntityDecl>* const lValue
                                        , NameIdPool<DTDEntityDecl>* const rValue
                                       );

    static bool           isEquivalent(
                                          NameIdPool<XMLNotationDecl>* const lValue
                                        , NameIdPool<XMLNotationDecl>* const rValue
                                       );

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
	~XTemplateComparator();
    XTemplateComparator();
    XTemplateComparator(const XTemplateComparator&);
	XTemplateComparator& operator=(const XTemplateComparator&);

};

XERCES_CPP_NAMESPACE_END

#endif
