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
 * $Id$
 */

#if !defined(TRAVERSESCHEMA_HPP)
#define TRAVERSESCHEMA_HPP

/**
  * Instances of this class get delegated to Traverse the Schema and
  * to populate the SchemaGrammar internal representation.
  */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XMLUniDefs.hpp>
#include <dom/DOM_Element.hpp>
#include <framework/XMLBuffer.hpp>
#include <validators/schema/SchemaSymbols.hpp>
#include <util/ValueVectorOf.hpp>
#include <util/RefHash2KeysTableOf.hpp>
#include <validators/schema/SchemaGrammar.hpp>

// ---------------------------------------------------------------------------
//  Forward Declarations
// ---------------------------------------------------------------------------
class GrammarResolver;
class EntityResolver;
class XMLValidator;
class XMLScanner;
class DatatypeValidator;
class DatatypeValidatorFactory;
class QName;
class ComplexTypeInfo;
class XMLAttDef;
class ContentSpecNode;
class NamespaceScope;
class SchemaAttDef;
class SchemaInfo;
class InputSource;
class ErrorHandler;
class GeneralAttributeCheck;
class XercesGroupInfo;
class XercesAttGroupInfo;


class VALIDATORS_EXPORT TraverseSchema
{
public:
    // -----------------------------------------------------------------------
    //  Public Constructors/Destructor
    // -----------------------------------------------------------------------
    TraverseSchema
    (
          const DOM_Element&                 schemaRoot
        , XMLStringPool* const               uriStringPool
        , SchemaGrammar* const               schemaGrammar
        , GrammarResolver* const             grammarResolver
        , XMLScanner* const                  xmlScanner
        , XMLValidator* const                xmlValidator
        , const XMLCh* const                 schemaURL
        , EntityResolver* const              entityResolver
        , ErrorHandler* const                errorHandler
        , ValueVectorOf<unsigned int>* const importLocations = 0
    );

    ~TraverseSchema();

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setCurrentSchemaURL(const XMLCh* const urlStr);

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    TraverseSchema(const TraverseSchema&);
    void operator=(const TraverseSchema&);

    // -----------------------------------------------------------------------
    //  CleanUp methods
    // -----------------------------------------------------------------------
    void cleanUp();

    // -----------------------------------------------------------------------
    //  Traversal methods
    // -----------------------------------------------------------------------
    /**
      * Traverse the Schema DOM tree
      */
    void                doTraverseSchema();
    void                traverseSchemaHeader();
    void                traverseAnnotationDecl(const DOM_Element& childElem);
    void                traverseInclude(const DOM_Element& childElem);
    void                traverseImport(const DOM_Element& childElem);
    void                traverseRedefine(const DOM_Element& childElem);
    void                traverseAttributeDecl(const DOM_Element& childElem,
                                              ComplexTypeInfo* const typeInfo);
    void                traverseSimpleContentDecl(const XMLCh* const typeName,
                                                  const DOM_Element& contentDecl,
                                                  ComplexTypeInfo* const typeInfo);
    void                traverseComplexContentDecl(const XMLCh* const typeName,
                                                  const DOM_Element& contentDecl,
                                                  ComplexTypeInfo* const typeInfo,
                                                  const bool isMixed);
    int                 traverseSimpleTypeDecl(const DOM_Element& childElem,
                                               int baseRefContext = SchemaSymbols::EMPTY_SET);
    int                 traverseComplexTypeDecl(const DOM_Element& childElem);
    int                 traverseByList(const DOM_Element& rootElem,
                                       const DOM_Element& contentElem,
                                       const int typeNameIndex,
                                       const int finalSet);
    int                 traverseByRestriction(const DOM_Element& rootElem,
                                              const DOM_Element& contentElem,
                                              const int typeNameIndex,
                                              const int finalSet);
    int                 traverseByUnion(const DOM_Element& rootElem,
                                        const DOM_Element& contentElem,
                                        const int typeNameIndex,
                                        const int finalSet,
                                        int baseRefContext);
    QName*              traverseElementDecl(const DOM_Element& childElem);
    XMLCh*              traverseNotationDecl(const DOM_Element& childElem);
    ContentSpecNode*    traverseChoiceSequence(const DOM_Element& elemDecl,
                                               const int modelGroupType,
                                               bool& toAdoptSpecNode);
    ContentSpecNode*    traverseAny(const DOM_Element& anyDecl);
    ContentSpecNode*    traverseAll(const DOM_Element& allElem);
    XercesGroupInfo*    traverseGroupDecl(const DOM_Element& childElem,
                                       bool& toAdoptSpecNode);
    XercesGroupInfo*    traverseGroupDeclNS(const XMLCh* const uriStr,
                                            const XMLCh* const groupName,
                                            bool& toAdoptSpecNode);
    XercesAttGroupInfo* traverseAttributeGroupDecl(const DOM_Element& elem,
                                                    ComplexTypeInfo* const typeInfo);
    XercesAttGroupInfo* traverseAttributeGroupDeclNS(const XMLCh* const uriStr,
                                                      const XMLCh* const attGroupName);
    SchemaAttDef*       traverseAnyAttribute(const DOM_Element& elem);

    // -----------------------------------------------------------------------
    //  Error Reporting methods
    // -----------------------------------------------------------------------
    void reportSchemaError(const XMLCh* const msgDomain, const int errorCode);
    void reportSchemaError(const XMLCh* const msgDomain,
                           const int errorCode,
                           const XMLCh* const text1,
                           const XMLCh* const text2 = 0,
                           const XMLCh* const text3 = 0,
                           const XMLCh* const text4 = 0);

    // -----------------------------------------------------------------------
    //  Private Helper methods
    // -----------------------------------------------------------------------
    /**
      * Retrived the Namespace mapping from the schema element
      */
    void retrieveNamespaceMapping();

    /**
      * Loop through the children, and traverse the corresponding schema type
      * type declaration (simpleType, complexType, import, ....)
      */
    void processChildren(const DOM_Element& root);

    /**
      * Parameters:
      *   rootElem - top element for a given type declaration
      *   contentElem - content must be annotation? or some other simple content
      *   isEmpty: - true if (annotation?, smth_else), false if (annotation?)
      *
      * Check for Annotation if it is present, traverse it. If a sibling is
      * found and it is not an annotation return it, otherwise return 0.
      * Used by traverseSimpleTypeDecl.
      */
    DOM_Element checkContent(const DOM_Element& rootElem,
                             const DOM_Element& contentElem,
                             const bool isEmpty);

    /**
      * Parameters:
      *   contentElem - content element to check
      *
      * Check for identity constraints content.
      */
    DOM_Element checkIdentityConstraintContent(const DOM_Element& contentElem);

    DatatypeValidator* getDatatypeValidator(const XMLCh* const uriStr,
                                            const XMLCh* const localPartStr);

    /**
      * Return qualified name of a given type name index from the string pool
      * If a target name space exist, it is returned as part of the name
      *
      * The allocation will be handled internally, and the caller should not
      * delete the returned pointer.
      */
    XMLCh* getQualifiedName(const int typeNameIndex);

    /**
      * Process simpleType content of a list|restriction|union
      * Return a dataype validator if valid type, otherwise 0.
      */
    DatatypeValidator* checkForSimpleTypeValidator(const DOM_Element& content,
                                                   int baseRefContext = SchemaSymbols::EMPTY_SET);

    /**
      * Process complexType content of an element
      * Return a ComplexTypeInfo if valid type, otherwise 0.
      */
    ComplexTypeInfo* checkForComplexTypeInfo(const DOM_Element& content);

    /**
      * Return DatatypeValidator available for the baseTypeStr.
      */
    DatatypeValidator* findDTValidator(const DOM_Element& rootElem,
                                       const XMLCh* const baseTypeStr,
                                       const int baseRefContext);

    /**
      * Return a compenent defined as a top level in a schema grammar
      *
      * In redefine we've not only got to look at the space of the thing we
      * are redefining but at the original schema too. The idea is to start
      * from the top, then go down through our list of schemas until we find
      * what we want. This should not often be necessary, because we've
      * processed all redefined schemas, but there are conditions in which not
      * all elements so redefined may have been promoted to the topmost level.
      */
    DOM_Element getTopLevelComponentByName(const XMLCh* const compCategory,
                                           const XMLCh* const name);

    const XMLCh* resolvePrefixToURI(const XMLCh* const prefix);

    /**
      * Return whether an element is defined as a top level schema component
      * or not.
      */
    bool isTopLevelComponent(const DOM_Element& elem);

    /**
      * Return the prefix for a given rawname string
      *
      * Function allocated, caller managed (facm) - pointer to be deleted by
      * caller.
      */
    const XMLCh* getPrefix(const XMLCh* const rawName);

    /**
      * Return the local for a given rawname string
      *
      * caller allocated, caller managed (cacm)
      */
    const XMLCh* getLocalPart(const XMLCh* const rawName);

    /**
      * Process a 'ref' of an Element declaration
      */
    QName* processElementDeclRef(const DOM_Element& elem,
                                const XMLCh* const refName);

    /**
      * Process a 'ref' of an Attribute declaration
      */
    void processAttributeDeclRef(const DOM_Element& elem,
                                 const DOM_Element& simpleTypeContent,
                                 ComplexTypeInfo* const typeInfo,
                                 const XMLCh* const refName,
                                 const XMLCh* const useVal);

    /**
      * Process a 'ref' on a group
      */
    XercesGroupInfo* processGroupRef(const DOM_Element& elem,
                                     const XMLCh* const refName,
                                     bool& toAdoptScpecNode);

    /**
      * Process a 'ref' on a attributeGroup
      */
    XercesAttGroupInfo* processAttributeGroupRef(const DOM_Element& elem,
                                                 const XMLCh* const refName,
                                                 ComplexTypeInfo* const typeInfo);

    /**
      * Parse block & final items
      */
    int parseBlockSet(const XMLCh* const blockStr, const int blockType);
    int parseFinalSet(const XMLCh* const finalStr, const int finalType);

    /**
      * Return true if a name is an identity constraint, otherwise false
      */
    bool isIdentityConstraintName(const XMLCh* const constraintName);

    /**
      * Check a 'ref' declaration representation constraint
      */
    bool isValidRefDeclaration(const DOM_Element& elem);

    /**
      * If 'typeStr' belongs to a different schema, return that schema URI,
      * otherwise return 0;
      */
    const XMLCh* checkTypeFromAnotherSchema(const XMLCh* const typeStr);

    /**
      * Return the datatype validator for a given element type attribute if
      * the type is a simple type
      */
    DatatypeValidator* getElementTypeValidator(const XMLCh* const typeStr,
                                               bool& noErrorDetected,
                                               const XMLCh* const otherSchemaURI,
                                               bool errorCheck = false);

    /**
      * Return the complexType info for a given element type attribute if
      * the type is a complex type
      */
    ComplexTypeInfo* getElementComplexTypeInfo(const XMLCh* const typeStr,
                                               bool& noErrorDetected,
                                               const XMLCh* const otherSchemaURI);

    /**
      * Return schema element declaration for a given substituteGroup element
      * name
      */
    SchemaElementDecl* getSubstituteGroupElemDecl(const XMLCh* const name,
                                                  bool& noErrorDetected);

    /**
      * Return a Schema element declared in another schema
      */
    SchemaElementDecl* getElementDeclFromNS(const XMLCh* const nameUri,
                                            const XMLCh* const localPart);


    /**
      * Check validity constraint of a substitutionGroup attribute in
      * an element declaration
      */
    bool isSubstitutionGroupValid(const SchemaElementDecl* const elemDecl,
                                  const ComplexTypeInfo* const typeInfo,
                                  const DatatypeValidator* const validator,
                                  const XMLCh* const elemName,
                                  const bool toEmit = true);

    /**
      * Create a 'SchemaElementDecl' object and add it to SchemaGrammar
      */
    SchemaElementDecl* createSchemaElementDecl(const DOM_Element& elem,
                                               const bool topLevel,
                                               const unsigned short elemType,
                                               bool& isDuplicate,
                                               const bool isFixedValue);

    /**
      * Return the value of a given attribute name from an element node
      */
    const XMLCh* getElementAttValue(const DOM_Element& elem,
                                    const XMLCh* const attName,
                                    const bool toTrim = false);

    void checkMinMax(ContentSpecNode* const specNode,
                     const DOM_Element& elem,
                     const int allContext = Not_All_Context);

    /**
      * Process complex content for a complexType
      */
    void processComplexContent(const XMLCh* const typeName,
                               const DOM_Element& childElem,
                               ComplexTypeInfo* const typeInfo,
                               const XMLCh* const baseRawName,
                               const XMLCh* const baseLocalPart,
                               const XMLCh* const baseURI,
                               const bool isMixed);

    /**
      * Process "base" information for a complexType
      */
    void processBaseTypeInfo(const XMLCh* const baseName,
                             const XMLCh* const localPart,
                             const XMLCh* const uriStr,
                             ComplexTypeInfo* const typeInfo);

    /**
      * Check if base is from another schema
      */
    bool isBaseFromAnotherSchema(const XMLCh* const baseURI);

    /**
      * Get complexType infp from another schema
      */
    ComplexTypeInfo* getTypeInfoFromNS(const XMLCh* const uriStr,
                                       const XMLCh* const localPart);

    /**
      * Returns true if 'name' is a valid facet for a given 'component',
      * otherwise false
      */
    bool isValidFacet(const XMLCh* const component, const XMLCh* const name);

    bool isAttrOrAttrGroup(const DOM_Element& elem);

    /**
      * Process attributes of a complex type
      */
    void processAttributes(const DOM_Element& elem,
                           const XMLCh* const baseRawName,
                           const XMLCh* const baseLocalPart,
                           const XMLCh* const baseURI,
                           ComplexTypeInfo* const typeInfo);

    /**
      * Generate a name for an anonymous type
      */
    const XMLCh* genAnonTypeName(const XMLCh* const prefix,
                                 int& anonCount);

    int addAttributeDeclFromAnotherSchema(const XMLCh* const name,
                                          const XMLCh* const uri,
                                          ComplexTypeInfo* const typeInfo);

    void defaultComplexTypeInfo(ComplexTypeInfo* const typeInfo);

    /**
      * Resolve a schema location attribute value to an input source.
      * Caller to delete the returned object.
      */
    InputSource* resolveSchemaLocation(const XMLCh* const loc);

    void restoreSchemaInfo();
    int  resetCurrentTypeNameStack(const int);

    /**
      * Check whether a mixed content is emptiable or not.
      * Needed to validate element constraint values (defualt, fixed)
      */
    bool emptiableParticle(const ContentSpecNode* const specNode);

    /**
      * Used by emptiableMixedContent to get the 'particle'
      * minimum/maximum total range.
      */
    int getMinTotalRange(const ContentSpecNode* const specNode);
    int getMaxTotalRange(const ContentSpecNode* const specNode);


    void checkFixedFacet(const DOM_Element&, const XMLCh* const,
                         const DatatypeValidator* const, unsigned int&);
    void checkRefElementConsistency();
    void buildValidSubstitutionListF(SchemaElementDecl* const,
                                     SchemaElementDecl* const);
    void buildValidSubstitutionListB(SchemaElementDecl* const,
                                     SchemaElementDecl* const);

    void checkEnumerationRequiredNotation(const XMLCh* const name,
                                          const XMLCh* const typeStr);

    bool hasAllContent(const ContentSpecNode* const specNode);

    void processElements(ComplexTypeInfo* const baseTypeInfo,
                         ComplexTypeInfo* const newTypeInfo);

    void copyGroupElements(XercesGroupInfo* const fromGroup,
                           XercesGroupInfo* const toGroup,
                           ComplexTypeInfo* const typeInfo);

    void copyAttGroupAttributes(XercesAttGroupInfo* const fromAttGroup,
                                XercesAttGroupInfo* const toAttGroup,
                                ComplexTypeInfo* const typeInfo);

    /**
      * Attribute wild card intersection.
      *
      * Note:
      *    The first parameter will be the result of the intersection, so
      *    we need to make sure that first parameter is a copy of the
      *    actual attribute definition we need to intersect with.
      *
      *    What we need to wory about is: type, defaultType, namespace,
      *    and URI. All remaining data members should be the same.
      */
    void attWildCardIntersection(SchemaAttDef* const resultWildCart,
                                 const SchemaAttDef* const toCompareWildCard);

    /**
      * Attribute wild card union.
      *
      * Note:
      *    The first parameter will be the result of the union, so
      *    we need to make sure that first parameter is a copy of the
      *    actual attribute definition we need to intersect with.
      *
      *    What we need to wory about is: type, defaultType, namespace,
      *    and URI. All remaining data members should be the same.
      */
    void attWildCardUnion(SchemaAttDef* const resultWildCart,
                          const SchemaAttDef* const toCompareWildCard);

    void copyWildCardData(const SchemaAttDef* const srcWildCard,
                          SchemaAttDef* const destWildCard);

    /**
      * Check that the attributes of a type derived by restriction satisfy
      * the constraints of derivation valid restriction
      */
    void checkAttDerivationOK(const ComplexTypeInfo* const baseTypeInfo,
                              const ComplexTypeInfo* const childTypeInfo);

    /**
      * Check whether a namespace value is valid with respect to wildcard
      * constraint
      */
    bool wildcardAllowsNamespace(const SchemaAttDef* const baseAttWildCard,
                                 const unsigned int nameURI);

    /**
      * Check whether a namespace constraint is an intensional subset of
      * another namespace constraint
      */
    bool isWildCardSubset(const SchemaAttDef* const baseAttWildCard,
                          const SchemaAttDef* const childAttWildCard);

    bool isAnyType(const XMLCh* const typeName);

    // -----------------------------------------------------------------------
    //  Private constants
    // -----------------------------------------------------------------------
    enum
    {
        ES_Block
        , C_Block
        , S_Final
        , ECS_Final
    };

    enum ExceptionCodes
    {
        NoException = 0,
        InvalidComplexTypeInfo = 1
    };

    // Flags indicate any special restrictions on minOccurs and maxOccurs
    // relating to "all".
    //    Not_All_Context    - not processing an <all>
    //    All_Element        - processing an <element> in an <all>
    //    Group_Ref_With_All - processing <group> reference that contained <all>
    //    All_Group          - processing an <all> group itself
    enum
	{
        Not_All_Context = 0
        , All_Element = 1
        , Group_Ref_With_All = 2
        , All_Group = 4
    };

    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    bool                                    fElementDefaultQualified;
    bool                                    fAttributeDefaultQualified;
    bool                                    fAdoptImportLocations;
    int                                     fTargetNSURI;
    int                                     fEmptyNamespaceURI;
    int                                     fCurrentScope;
    int                                     fSimpleTypeAnonCount;
    int                                     fComplexTypeAnonCount;
    int                                     fFinalDefault;
    int                                     fBlockDefault;
    int                                     fScopeCount;
    unsigned int                            fCurrentNamespaceLevel;
    DOM_Element                             fSchemaRootElement;
    XMLCh*                                  fTargetNSURIString;
    XMLCh*                                  fCurrentSchemaURL;
    DatatypeValidatorFactory*               fDatatypeRegistry;
    GrammarResolver*                        fGrammarResolver;
    SchemaGrammar*                          fSchemaGrammar;
    EntityResolver*                         fEntityResolver;
    ErrorHandler*                           fErrorHandler;
    XMLStringPool*                          fURIStringPool;
    XMLStringPool*                          fStringPool;
    XMLBuffer                               fBuffer;
    XMLValidator*                           fValidator;
    XMLScanner*                             fScanner;
    NamespaceScope*                         fNamespaceScope;
    RefHashTableOf<XMLAttDef>*              fAttributeDeclRegistry;
    RefHashTableOf<ComplexTypeInfo>*        fComplexTypeRegistry;
    RefHashTableOf<XercesGroupInfo>*        fGroupRegistry;
    RefHashTableOf<XercesAttGroupInfo>*     fAttGroupRegistry;
    SchemaInfo*                             fSchemaInfoRoot;
    SchemaInfo*                             fCurrentSchemaInfo;
    XercesGroupInfo*                        fCurrentGroupInfo;
    XercesAttGroupInfo*                     fCurrentAttGroupInfo;
    ComplexTypeInfo*                        fCurrentComplexType;
    ValueVectorOf<unsigned int>*            fImportLocations;
    ValueVectorOf<unsigned int>*            fIncludeLocations;
    ValueVectorOf<unsigned int>*            fCurrentTypeNameStack;
    ValueVectorOf<unsigned int>*            fCurrentGroupStack;
    GeneralAttributeCheck*                  fAttributeCheck;
    RefHash2KeysTableOf<XMLCh>*             fGlobalTypes;
    RefHash2KeysTableOf<XMLCh>*             fGlobalGroups;
    RefHash2KeysTableOf<XMLCh>*             fGlobalAttGroups;
    RefHash2KeysTableOf<SchemaElementDecl>* fSubstitutionGroups;
    RefHash2KeysTableOf<ElemVector>*        fValidSubstitutionGroups;
    RefVectorOf<SchemaElementDecl>*         fRefElements;
    ValueVectorOf<int>*                     fRefElemScope;

    friend class GeneralAttributeCheck;
};


// ---------------------------------------------------------------------------
//  TraverseSchema: Setter methods
// ---------------------------------------------------------------------------
inline void TraverseSchema::setCurrentSchemaURL(const XMLCh* const urlStr) {

    delete [] fCurrentSchemaURL;
    fCurrentSchemaURL = XMLString::replicate(urlStr);
}

// ---------------------------------------------------------------------------
//  TraverseSchema: Helper methods
// ---------------------------------------------------------------------------
inline const XMLCh* TraverseSchema::getPrefix(const XMLCh* const rawName) {

    int colonIndex = XMLString::indexOf(rawName, chColon);

    if (colonIndex == -1 || colonIndex == 0) {
        return XMLUni::fgZeroLenString;
    }

    fBuffer.set(rawName, colonIndex + 1);
    XMLString::subString(fBuffer.getRawBuffer(), rawName, 0, colonIndex);

    unsigned int nameId = fStringPool->addOrFind(fBuffer.getRawBuffer());

    return fStringPool->getValueForId(nameId);
}

inline const XMLCh* TraverseSchema::getLocalPart(const XMLCh* const rawName) {

    int    colonIndex = XMLString::indexOf(rawName, chColon);
    int    rawNameLen = XMLString::stringLen(rawName);

    if (colonIndex + 1 == rawNameLen) {
        return XMLUni::fgZeroLenString;
    }

    if (colonIndex == -1) {
        fBuffer.set(rawName, rawNameLen);
    }
    else {

        fBuffer.set(rawName, rawNameLen - colonIndex);
        XMLString::subString(fBuffer.getRawBuffer(), rawName,
                             colonIndex + 1, rawNameLen);
    }

    unsigned int nameId = fStringPool->addOrFind(fBuffer.getRawBuffer());

    return fStringPool->getValueForId(nameId);
}

inline bool
TraverseSchema::isValidRefDeclaration(const DOM_Element& elem) {

    return !(elem.getAttribute(SchemaSymbols::fgATT_ABSTRACT).length() != 0
             || elem.getAttribute(SchemaSymbols::fgATT_NILLABLE).length() != 0
             || elem.getAttribute(SchemaSymbols::fgATT_BLOCK).length() != 0
             || elem.getAttribute(SchemaSymbols::fgATT_FINAL).length() != 0
             || elem.getAttribute(SchemaSymbols::fgATT_TYPE).length() != 0
             || elem.getAttribute(SchemaSymbols::fgATT_DEFAULT).length() != 0
             || elem.getAttribute(SchemaSymbols::fgATT_FIXED).length() != 0
             || elem.getAttribute(SchemaSymbols::fgATT_SUBSTITUTIONGROUP).length() != 0);
}

inline
const XMLCh* TraverseSchema::getElementAttValue(const DOM_Element& elem,
                                                const XMLCh* const attName,
                                                const bool toTrim) {

    DOMString attValue = elem.getAttribute(attName);

    if (attValue.length() > 0) {

        fBuffer.set(attValue.rawBuffer(), attValue.length());
        XMLCh* bufValue = fBuffer.getRawBuffer();

        if (toTrim) {

			XMLString::trim(bufValue);

            if (!XMLString::stringLen(bufValue)) {
                return 0;
            }
        }

        unsigned int elemId = fStringPool->addOrFind(bufValue);
        return fStringPool->getValueForId(elemId);
    }

    return 0;
}

inline bool TraverseSchema::isBaseFromAnotherSchema(const XMLCh* const baseURI)
{
    if (XMLString::compareString(baseURI,fTargetNSURIString) != 0
        && XMLString::compareString(baseURI, SchemaSymbols::fgURI_SCHEMAFORSCHEMA) != 0
        && XMLString::stringLen(baseURI) != 0) {
        //REVISIT, !!!! a hack: for schema that has no
        //target namespace, e.g. personal-schema.xml
        return true;
    }

    return false;
}

inline bool TraverseSchema::isAttrOrAttrGroup(const DOM_Element& elem) {

    DOMString elementName = elem.getLocalName();

    if (elementName.equals(SchemaSymbols::fgELT_ATTRIBUTE) ||
        elementName.equals(SchemaSymbols::fgELT_ATTRIBUTEGROUP) ||
        elementName.equals(SchemaSymbols::fgELT_ANYATTRIBUTE)) {
        return true;
    }

    return false;
}

inline const XMLCh* TraverseSchema::genAnonTypeName(const XMLCh* const prefix,
                                                    int& anonCount) {

    XMLCh anonCountStr[16]; // a count of 15 digits should be enough

    XMLString::binToText(anonCount++, anonCountStr, 15, 10);
    fBuffer.set(prefix);
    fBuffer.append(anonCountStr);

    int anonTypeId = fStringPool->addOrFind(fBuffer.getRawBuffer());

    return fStringPool->getValueForId(anonTypeId);
}

inline int TraverseSchema::resetCurrentTypeNameStack(const int value) {

    unsigned int stackSize = fCurrentTypeNameStack->size();

    if (stackSize != 0) {
        fCurrentTypeNameStack->removeElementAt(stackSize - 1);
    }

    return value;
}

inline void
TraverseSchema::copyWildCardData(const SchemaAttDef* const srcWildCard,
                                 SchemaAttDef* const destWildCard) {

    destWildCard->getAttName()->setURI(srcWildCard->getAttName()->getURI());
    destWildCard->setType(srcWildCard->getType());
    destWildCard->setDefaultType(srcWildCard->getDefaultType());
}

inline bool
TraverseSchema::isAnyType(const XMLCh* const typeName) {

    const XMLCh* const localPart = getLocalPart(typeName);
    const XMLCh* const uri = resolvePrefixToURI(getPrefix(typeName));

    return (!XMLString::compareString(uri, SchemaSymbols::fgURI_SCHEMAFORSCHEMA)
            && !XMLString::compareString(localPart, SchemaSymbols::fgATTVAL_ANYTYPE));
}

#endif

/**
  * End of file TraverseSchema.hpp
  */

