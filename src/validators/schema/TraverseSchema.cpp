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

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/schema/TraverseSchema.hpp>
#include <validators/datatype/DatatypeValidatorFactory.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#include <util/RefVectorOf.hpp>
#include <util/RefHashTableOf.hpp>
#include <util/Janitor.hpp>
#include <util/KVStringPair.hpp>
#include <util/XMLStringTokenizer.hpp>
#include <util/QName.hpp>
#include <validators/schema/XUtil.hpp>
#include <validators/common/GrammarResolver.hpp>
#include <validators/schema/SchemaGrammar.hpp>
#include <validators/schema/SchemaAttDef.hpp>
#include <internal/XMLReader.hpp>
#include <validators/common/ContentSpecNode.hpp>
#include <validators/schema/ComplexTypeInfo.hpp>
#include <validators/schema/NamespaceScope.hpp>
#include <validators/schema/SchemaAttDefList.hpp>
#include <framework/XMLValidityCodes.hpp>
#include <framework/XMLErrorCodes.hpp>
#include <internal/XMLScanner.hpp>
#include <internal/XMLInternalErrorHandler.hpp>
#include <framework/XMLValidator.hpp>
#include <sax/EntityResolver.hpp>
#include <util/XMLURL.hpp>
#include <sax/InputSource.hpp>
#include <framework/LocalFileInputSource.hpp>
#include <framework/URLInputSource.hpp>
#include <parsers/DOMParser.hpp>
#include <dom/DOM_DOMException.hpp>
#include <validators/schema/SchemaInfo.hpp>
#include <validators/datatype/InvalidDatatypeValueException.hpp>
#include <validators/datatype/InvalidDatatypeFacetException.hpp>
#include <validators/schema/GeneralAttributeCheck.hpp>
#include <validators/schema/XercesGroupInfo.hpp>
#include <validators/schema/XercesAttGroupInfo.hpp>

// ---------------------------------------------------------------------------
//  TraverseSchema: Local declaration
// ---------------------------------------------------------------------------
typedef RefVectorOf<DatatypeValidator> DVRefVector;


// ---------------------------------------------------------------------------
//  TraverseSchema: Static member data
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//  TraverseSchema: Local const data
// ---------------------------------------------------------------------------
const XMLCh fgXMLNS_Str[] =
{
    chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chColon, chNull
};

const XMLCh fgAnonSNamePrefix[] =
{
    chLatin_S, chNull
};

const XMLCh fgAnonCNamePrefix[] =
{
    chLatin_C, chNull
};

const XMLCh fgUnbounded[] =
{
    chLatin_u, chLatin_n, chLatin_b, chLatin_o, chLatin_u, chLatin_n, chLatin_d,
    chLatin_e, chLatin_d, chNull
};

const XMLCh fgSkip[] =
{
    chLatin_s, chLatin_k, chLatin_i, chLatin_p, chNull
};

const XMLCh fgLax[] =
{
    chLatin_l, chLatin_a, chLatin_x, chNull
};

const XMLCh fgStrict[] =
{
    chLatin_s, chLatin_t, chLatin_r, chLatin_i, chLatin_c, chLatin_t, chNull
};

const XMLCh fgValueOne[] =
{
    chDigit_1, chNull
};

const XMLCh fgValueZero[] =
{
    chDigit_0, chNull
};

// ---------------------------------------------------------------------------
//  TraverseSchema: Constructors and Destructor
// ---------------------------------------------------------------------------
TraverseSchema::TraverseSchema( const DOM_Element&                 schemaRoot
                              , XMLStringPool* const               uriStringPool
                              , SchemaGrammar* const               schemaGrammar
                              , GrammarResolver* const             grammarResolver
                              , XMLScanner* const                  xmlScanner
                              , XMLValidator* const                xmlValidator
                              , const XMLCh* const                 schemaURL
                              , EntityResolver* const              entityResolver
                              , ErrorHandler* const                errorHandler
                              , ValueVectorOf<unsigned int>* const importLocations)
    : fElementDefaultQualified(false)
    , fAttributeDefaultQualified(false)
    , fAdoptImportLocations(false)
    , fTargetNSURI(-1)
    , fEmptyNamespaceURI(-1)
    , fCurrentScope(Grammar::TOP_LEVEL_SCOPE)
    , fSimpleTypeAnonCount(0)
    , fComplexTypeAnonCount(0)
    , fFinalDefault(0)
    , fBlockDefault(0)
    , fScopeCount(0)
    , fCurrentNamespaceLevel(0)
    , fSchemaRootElement(schemaRoot)
    , fTargetNSURIString(0)
    , fCurrentSchemaURL(XMLString::replicate(schemaURL))
    , fDatatypeRegistry(0)
    , fGrammarResolver(grammarResolver)
    , fSchemaGrammar(schemaGrammar)
    , fEntityResolver(entityResolver)
    , fErrorHandler(errorHandler)
    , fURIStringPool(uriStringPool)
    , fStringPool(0)
    , fValidator(xmlValidator)
    , fScanner(xmlScanner)
    , fNamespaceScope(0)
    , fAttributeDeclRegistry(0)
    , fComplexTypeRegistry(0)
    , fGroupRegistry(0)
    , fAttGroupRegistry(0)
    , fSchemaInfoRoot(0)
    , fCurrentSchemaInfo(0)
    , fCurrentGroupInfo(0)
    , fCurrentAttGroupInfo(0)
    , fCurrentComplexType(0)
    , fImportLocations(importLocations)
    , fIncludeLocations(0)
    , fCurrentTypeNameStack(0)
    , fCurrentGroupStack(0)
    , fAttributeCheck(0)
    , fGlobalTypes(0)
    , fGlobalGroups(0)
    , fGlobalAttGroups(0)
    , fSubstitutionGroups(0)
    , fValidSubstitutionGroups(0)
    , fRefElements(0)
    , fRefElemScope(0)
{

    try {
        doTraverseSchema();
    }
    catch(...) {

        cleanUp();
        throw;
    }
}


TraverseSchema::~TraverseSchema()
{
   cleanUp();
}


// ---------------------------------------------------------------------------
//  TraverseSchema: Traversal methods
// ---------------------------------------------------------------------------
void TraverseSchema::doTraverseSchema() {

    fDatatypeRegistry = fGrammarResolver->getDatatypeRegistry();
    fDatatypeRegistry->expandRegistryToFullSchemaSet();

    if (!fImportLocations) {
        fAdoptImportLocations = true;
        fImportLocations = new ValueVectorOf<unsigned int>(8);
    }

    unsigned int schemaURLId = fURIStringPool->addOrFind(fCurrentSchemaURL);
    fImportLocations->addElement(schemaURLId);

    if (fSchemaRootElement.isNull()) {
        // REVISIT: Anything to do?
        return;
    }

    fStringPool = fGrammarResolver->getStringPool();
    fEmptyNamespaceURI = fURIStringPool->addOrFind(XMLUni::fgZeroLenString);
    fIncludeLocations = new ValueVectorOf<unsigned int>(8);
    fIncludeLocations->addElement(schemaURLId);
    fAttributeCheck = GeneralAttributeCheck::instance();
    fCurrentTypeNameStack = new ValueVectorOf<unsigned int>(8);
    fCurrentGroupStack = new ValueVectorOf<unsigned int>(8);
    fGlobalTypes = new RefHash2KeysTableOf<XMLCh>(29, false);
    fGlobalGroups = new RefHash2KeysTableOf<XMLCh>(13, false);
    fGlobalAttGroups = new RefHash2KeysTableOf<XMLCh>(13, false);
    fSubstitutionGroups = new RefHash2KeysTableOf<SchemaElementDecl>(29, false);
    fRefElements = new RefVectorOf<SchemaElementDecl>(32, false);
    fRefElemScope = new ValueVectorOf<int>(32);

    //Make sure namespace binding is defaulted
    DOMString rootPrefix = fSchemaRootElement.getPrefix();

    if (rootPrefix == 0 || rootPrefix.length() == 0) {

        DOMString xmlns = fSchemaRootElement.getAttribute(XMLUni::fgXMLNSString);

        if (xmlns.length() == 0) {
            fSchemaRootElement.setAttribute(
                XMLUni::fgXMLNSString, SchemaSymbols::fgURI_SCHEMAFORSCHEMA);
        }
    }

    //Retrieve the targetnamespace URI information
    DOMString targetNSURIStr = fSchemaRootElement.getAttribute(
                                        SchemaSymbols::fgATT_TARGETNAMESPACE);

    if (targetNSURIStr == 0) {
        fTargetNSURIString = XMLString::replicate(XMLUni::fgZeroLenString);
    }
    else {

        fBuffer.set(targetNSURIStr.rawBuffer(), targetNSURIStr.length());
        fTargetNSURIString = XMLString::replicate(fBuffer.getRawBuffer());
    }

    fTargetNSURI = fURIStringPool->addOrFind(fTargetNSURIString);

    // Set schemaGrammar data and add it to GrammarResolver
    if (fGrammarResolver == 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoGrammarResolver);
        return;
    }
    else{

        // for complex type registry, attribute decl registry , group/attGroup
        // and namespace mapping, needs to check whether the passed in
        // Grammar was a newly instantiated one.
        fComplexTypeRegistry = fSchemaGrammar->getComplexTypeRegistry();

        if (fComplexTypeRegistry == 0 ) {

            fComplexTypeRegistry = new RefHashTableOf<ComplexTypeInfo>(109);
            fSchemaGrammar->setComplexTypeRegistry(fComplexTypeRegistry);
        }

        fGroupRegistry = fSchemaGrammar->getGroupInfoRegistry();

        if (fGroupRegistry == 0 ) {

            fGroupRegistry = new RefHashTableOf<XercesGroupInfo>(13);
            fSchemaGrammar->setGroupInfoRegistry(fGroupRegistry);
        }

        fAttGroupRegistry = fSchemaGrammar->getAttGroupInfoRegistry();

        if (fAttGroupRegistry == 0 ) {

            fAttGroupRegistry = new RefHashTableOf<XercesAttGroupInfo>(13);
            fSchemaGrammar->setAttGroupInfoRegistry(fAttGroupRegistry);
        }

        fAttributeDeclRegistry = fSchemaGrammar->getAttributeDeclRegistry();

        if (fAttributeDeclRegistry == 0) {

            fAttributeDeclRegistry = new RefHashTableOf<XMLAttDef>(29, true);
            fSchemaGrammar->setAttributeDeclRegistry(fAttributeDeclRegistry);
        }

        fNamespaceScope = fSchemaGrammar->getNamespaceScope();

        if (fNamespaceScope == 0) {

            fNamespaceScope = new NamespaceScope();
            fNamespaceScope->reset(fEmptyNamespaceURI);
            fCurrentNamespaceLevel = fNamespaceScope->increaseDepth();
            fSchemaGrammar->setNamespaceScope(fNamespaceScope);
        }

        fValidSubstitutionGroups = fSchemaGrammar->getValidSubstitutionGroups();

        if (!fValidSubstitutionGroups) {

            fValidSubstitutionGroups = new RefHash2KeysTableOf<ElemVector>(29);
            fSchemaGrammar->setValidSubstitutionGroups(fValidSubstitutionGroups);
        }

        fSchemaGrammar->setDatatypeRegistry(fDatatypeRegistry);
        fSchemaGrammar->setTargetNamespace(fTargetNSURIString);
        fGrammarResolver->putGrammar(fSchemaGrammar->getTargetNamespace(),
                                     fSchemaGrammar);
    } // end else

    traverseSchemaHeader();

    // process children nodes
    processChildren(fSchemaRootElement);

    // Handle identity constraints
    // TO DO

    // Element consistency checks - substitution groups
    checkRefElementConsistency();
}


void TraverseSchema::traverseSchemaHeader() {

    // -----------------------------------------------------------------------
    // Check Attributes
    // -----------------------------------------------------------------------
    unsigned short scope = GeneralAttributeCheck::GlobalContext;
    fAttributeCheck->checkAttributes(fSchemaRootElement, scope, this);

    retrieveNamespaceMapping();

    fElementDefaultQualified =
        fSchemaRootElement.getAttribute(SchemaSymbols::fgATT_ELEMENTFORMDEFAULT).equals(SchemaSymbols::fgATTVAL_QUALIFIED);
    fAttributeDefaultQualified =
        fSchemaRootElement.getAttribute(SchemaSymbols::fgATT_ATTRIBUTEFORMDEFAULT).equals(SchemaSymbols::fgATTVAL_QUALIFIED);

    // Get finalDefault/blockDefault values
    const XMLCh* defaultVal = getElementAttValue(fSchemaRootElement,
                                          SchemaSymbols::fgATT_BLOCKDEFAULT);
    const XMLCh* finalVal = getElementAttValue(fSchemaRootElement,
                                          SchemaSymbols::fgATT_FINALDEFAULT);
    fBlockDefault = parseBlockSet(defaultVal, ES_Block);
    fFinalDefault = parseFinalSet(finalVal, ECS_Final);
}


void TraverseSchema::traverseAnnotationDecl(const DOM_Element& childElem) {

    //TO DO
}


/**
  * Traverse include
  *
  *    <include
  *        id = ID
  *        schemaLocation = anyURI
  *        {any attributes with non-schema namespace . . .}>
  *        Content: (annotation?)
  *    </include>
  */
void TraverseSchema::traverseInclude(const DOM_Element& elem) {

    // ------------------------------------------------------------------
    // Check attributes
    // ------------------------------------------------------------------
    unsigned short scope = GeneralAttributeCheck::GlobalContext;
    fAttributeCheck->checkAttributes(elem, scope, this);

    // ------------------------------------------------------------------
    // First, handle any ANNOTATION declaration
    // ------------------------------------------------------------------
    if (checkContent(elem, XUtil::getFirstChildElement(elem), true) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::OnlyAnnotationExpected);
    }

    // ------------------------------------------------------------------
    // Get 'schemaLocation' attribute
    // ------------------------------------------------------------------
    const XMLCh* schemaLocation =
            getElementAttValue(elem, SchemaSymbols::fgATT_SCHEMALOCATION);

    if (XMLString::stringLen(schemaLocation) == 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::IncludeNoSchemaLocation);
        return;
    }

    // ------------------------------------------------------------------
    // Resolve schema location
    // ------------------------------------------------------------------
    InputSource*         srcToFill = resolveSchemaLocation(schemaLocation);
    Janitor<InputSource> janSrc(srcToFill);

    // Nothing to do
    if (!srcToFill) {
        return;
    }

    const XMLCh* includeURL = srcToFill->getSystemId();
    unsigned int locationId = fURIStringPool->addOrFind(includeURL);

    if (fIncludeLocations->containsElement(locationId)) {
        return;
    }

    fIncludeLocations->addElement(locationId);

    // ------------------------------------------------------------------
    // Parse input source
    // ------------------------------------------------------------------
    DOMParser parser;
    XMLInternalErrorHandler internalErrorHandler(fErrorHandler);

    parser.setValidationScheme(DOMParser::Val_Never);
    parser.setDoNamespaces(true);
    parser.setErrorHandler((ErrorHandler*) &internalErrorHandler);
    parser.setEntityResolver(fEntityResolver);

    parser.parse(*srcToFill);

    if (internalErrorHandler.getSawFatal() && fScanner->getExitOnFirstFatal())
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SchemaScanFatalError);

    // ------------------------------------------------------------------
    // Get root element
    // ------------------------------------------------------------------
    DOM_Document document = parser.getDocument();

    if (!document.isNull()) {

        DOM_Element root = document.getDocumentElement();

        if (!root.isNull()) {

            const XMLCh* targetNSURIString =
                getElementAttValue(root,SchemaSymbols::fgATT_TARGETNAMESPACE);

            if (XMLString::stringLen(targetNSURIString) != 0
                && XMLString::compareString(targetNSURIString,
                                            fTargetNSURIString) != 0){
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::IncludeNamespaceDifference,
                                  schemaLocation, targetNSURIString);
            }
            else {

                // --------------------------------------------------------
                // Save current schema information
                // --------------------------------------------------------
                if (fSchemaInfoRoot == 0) {

                    fSchemaInfoRoot =
                        new SchemaInfo(fElementDefaultQualified,
                                       fAttributeDefaultQualified,
                                       fBlockDefault,
                                       fFinalDefault,
                                       0,
                                       fCurrentSchemaURL,
                                       fSchemaRootElement,
                                       0, 0);
                    fCurrentSchemaInfo = fSchemaInfoRoot;
                }

                // --------------------------------------------------------
                // Update schema information with included schema
                // --------------------------------------------------------
                unsigned int namespaceLevel = fNamespaceScope->increaseDepth();
                fSchemaRootElement = root;
                setCurrentSchemaURL(includeURL);
                traverseSchemaHeader();

                // and now we'd better save this stuff!
                fCurrentSchemaInfo =
                    new SchemaInfo(fElementDefaultQualified,
                                   fAttributeDefaultQualified,
                                   fBlockDefault,
                                   fFinalDefault,
                                   namespaceLevel,
                                   fCurrentSchemaURL,
                                   fSchemaRootElement,
                                   fCurrentSchemaInfo->getNext(),
                                   fCurrentSchemaInfo);

                (fCurrentSchemaInfo->getPrev())->setNext(fCurrentSchemaInfo);
                processChildren(fSchemaRootElement);

                // --------------------------------------------------------
                // Restore old schema information
                // --------------------------------------------------------
                fCurrentSchemaInfo = fCurrentSchemaInfo->getPrev();
                restoreSchemaInfo();
            }
        }
    }
}

/**
  * Traverse import
  *
  *    <import
  *        id = ID
  *        namespace = anyURI
  *        schemaLocation = anyURI
  *        {any attributes with non-schema namespace . . .}>
  *        Content: (annotation?)
  *    </import>
  */
void TraverseSchema::traverseImport(const DOM_Element& elem) {

    // ------------------------------------------------------------------
    // Check attributes
    // ------------------------------------------------------------------
    unsigned short scope = GeneralAttributeCheck::GlobalContext;
    fAttributeCheck->checkAttributes(elem, scope, this);

    // ------------------------------------------------------------------
    // First, handle any ANNOTATION declaration
    // ------------------------------------------------------------------
    if (checkContent(elem, XUtil::getFirstChildElement(elem), true) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::OnlyAnnotationExpected);
    }

    // ------------------------------------------------------------------
    // Handle 'namespace' attribute
    // ------------------------------------------------------------------
    const XMLCh* nameSpace = getElementAttValue(elem, SchemaSymbols::fgATT_NAMESPACE);

    if (!XMLString::compareString(nameSpace, fTargetNSURIString)) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::Import_1_1);
        return;
    }

    if (!XMLString::stringLen(nameSpace)
        && fTargetNSURI == fEmptyNamespaceURI) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::Import_1_2);
        return;
    }

    SchemaGrammar* importedGrammar = 0;
	
	if (nameSpace) {

		importedGrammar = (SchemaGrammar*) fGrammarResolver->getGrammar(nameSpace);

        if (importedGrammar) {
           return;
        }
    }

    // ------------------------------------------------------------------
    // Get 'schemaLocation' attribute
    // ------------------------------------------------------------------
    const XMLCh* schemaLocation =
            getElementAttValue(elem, SchemaSymbols::fgATT_SCHEMALOCATION);

    if (XMLString::stringLen(schemaLocation) == 0) {
//        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ImportNoSchemaLocation);
        return;
    }

    // ------------------------------------------------------------------
    // Resolve schema location
    // ------------------------------------------------------------------
    InputSource*         srcToFill = resolveSchemaLocation(schemaLocation);
    Janitor<InputSource> janSrc(srcToFill);

    // Nothing to do
    if (!srcToFill) {
        return;
    }

    const XMLCh* importURL = srcToFill->getSystemId();
    unsigned int locationId = fURIStringPool->addOrFind(importURL);

    if (fImportLocations->containsElement(locationId)) {
        return;
    }

    // ------------------------------------------------------------------
    // Parse input source
    // ------------------------------------------------------------------
    DOMParser parser;
    XMLInternalErrorHandler internalErrorHandler(fErrorHandler);

    parser.setValidationScheme(DOMParser::Val_Never);
    parser.setDoNamespaces(true);
    parser.setErrorHandler((ErrorHandler*) &internalErrorHandler);
    parser.setEntityResolver(fEntityResolver);

    parser.parse(*srcToFill);

    if (internalErrorHandler.getSawFatal() && fScanner->getExitOnFirstFatal())
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SchemaScanFatalError);


    // ------------------------------------------------------------------
    // Get root element
    // ------------------------------------------------------------------
    DOM_Document document = parser.getDocument();

    if (!document.isNull()) {

        DOM_Element root = document.getDocumentElement();

        if (!root.isNull()) {

            const XMLCh* targetNSURIString =
                getElementAttValue(root,SchemaSymbols::fgATT_TARGETNAMESPACE);

            if (XMLString::compareString(targetNSURIString, nameSpace) != 0) {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ImportNamespaceDifference,
                                  schemaLocation, targetNSURIString, nameSpace);
            }
            else {

                importedGrammar = new SchemaGrammar();

                TraverseSchema traverseSchema(root, fURIStringPool, importedGrammar,
                                              fGrammarResolver, fScanner, fValidator,
                                              importURL, fEntityResolver, fErrorHandler,
                                              fImportLocations);
            }
         }
         else {
             reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ImportRootError, schemaLocation);
         }
    }
}


void TraverseSchema::traverseRedefine(const DOM_Element& childElem) {

    //TO DO
}


/**
  * Traverse the Choice, Sequence declaration
  *
  *    <choice-sequqnce
  *        id = ID
  *        maxOccurs = (nonNegativeInteger | unbounded)  : 1
  *        minOccurs = nonNegativeInteger : 1
  *        Content: (annotation?, (element | group | choice | sequence | any)*)
  *    </choice-sequence>
  */
ContentSpecNode*
TraverseSchema::traverseChoiceSequence(const DOM_Element& elem,
                                       const int modelGroupType,
                                       bool& toAdoptSpecNode)
{

    // ------------------------------------------------------------------
    // Check attributes
    // ------------------------------------------------------------------
    unsigned short scope = GeneralAttributeCheck::LocalContext;
    fAttributeCheck->checkAttributes(elem, scope, this);

    // ------------------------------------------------------------------
    // Process contents
    // ------------------------------------------------------------------
    DOM_Element child = checkContent(elem, XUtil::getFirstChildElement(elem), true);
    ContentSpecNode* left = 0;
    ContentSpecNode* right = 0;
    bool hadContent = false;
    bool toAdoptLeft = true;
    bool toAdoptRight = true;

    for (; child != 0; child = XUtil::getNextSiblingElement(child)) {

        ContentSpecNode* contentSpecNode = 0;
        ContentSpecNode* expandedSpecNode = 0;
        bool seeParticle = false;
        bool adoptSpecNode = true;
        DOMString childName = child.getLocalName();

        hadContent = true;

        if (childName.equals(SchemaSymbols::fgELT_ELEMENT)) {

            QName* eltQName = traverseElementDecl(child);
            Janitor<QName> janQName(eltQName);

            if (eltQName == 0) {
                continue;
            }

            contentSpecNode = new ContentSpecNode(eltQName);
            seeParticle = true;
        }
        else if (childName.equals(SchemaSymbols::fgELT_GROUP)) {

            XercesGroupInfo* grpInfo = traverseGroupDecl(child, adoptSpecNode);

            if (grpInfo) {

                contentSpecNode = grpInfo->getContentSpec();

                if (contentSpecNode == 0)
                    continue;

                if (hasAllContent(contentSpecNode)) {

                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::AllContentLimited);
                    continue;
                }
            }

            seeParticle = true;
        }
        else if (childName.equals(SchemaSymbols::fgELT_CHOICE)) {

            contentSpecNode =
                traverseChoiceSequence(child,ContentSpecNode::Choice, adoptSpecNode);
            seeParticle = true;
        }
        else if (childName.equals(SchemaSymbols::fgELT_SEQUENCE)) {
            contentSpecNode =
                traverseChoiceSequence(child,ContentSpecNode::Sequence, adoptSpecNode);
            seeParticle = true;
        }
        else if (childName.equals(SchemaSymbols::fgELT_ANY)) {

            contentSpecNode = traverseAny(child);
            seeParticle = true;
        }
        else {
            fBuffer.set(childName.rawBuffer(), childName.length());
            reportSchemaError(XMLUni::fgValidityDomain, XMLValid::GroupContentRestricted,
                              fBuffer.getRawBuffer());
        }

        if (seeParticle) {
            checkMinMax(contentSpecNode, child, Not_All_Context);
        }

        expandedSpecNode = contentSpecNode;

        if (left == 0) {
            left = expandedSpecNode;
            toAdoptLeft = adoptSpecNode;
        }
        else if (right == 0) {
            right = expandedSpecNode;
            toAdoptRight = adoptSpecNode;
        }
        else {
            left = new ContentSpecNode((ContentSpecNode::NodeTypes) modelGroupType,
                                       left, right, toAdoptLeft, toAdoptRight);
            right = expandedSpecNode;
            toAdoptLeft = true;
            toAdoptRight = adoptSpecNode;
        }
    }

    if (hadContent && right != 0) {
        left = new ContentSpecNode((ContentSpecNode::NodeTypes) modelGroupType,
                                   left, right, toAdoptLeft, toAdoptRight);
        toAdoptLeft = true;
    }

    toAdoptSpecNode = toAdoptLeft;
    return left;
}

/**
  * Traverse SimpleType declaration:
  * <simpleType
  *     id = ID
  *     name = NCName>
  *     Content: (annotation? , ((list | restriction | union)))
  * </simpleType>
  *
  * traverse <list>|<restriction>|<union>
  */
int TraverseSchema::traverseSimpleTypeDecl(const DOM_Element& childElem,
                                           int baseRefContext)
{
    // ------------------------------------------------------------------
    // Check attributes
    // ------------------------------------------------------------------
    bool topLevel = isTopLevelComponent(childElem);
    unsigned short scope = (topLevel) ? GeneralAttributeCheck::GlobalContext
                                      : GeneralAttributeCheck::LocalContext;

    fAttributeCheck->checkAttributes(childElem, scope, this);

    // ------------------------------------------------------------------
    // Process contents
    // ------------------------------------------------------------------
    const XMLCh* name = getElementAttValue(childElem,SchemaSymbols::fgATT_NAME);

    if (topLevel && XMLString::stringLen(name) == 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoNameGlobalElement,
                          SchemaSymbols::fgELT_SIMPLETYPE);
        return -1;
    }

    if (XMLString::stringLen(name) == 0) { // anonymous simpleType
        name = genAnonTypeName(fgAnonSNamePrefix, fSimpleTypeAnonCount);
    }

    fBuffer.set(fTargetNSURIString);
    fBuffer.append(chComma);
    fBuffer.append(name);

    XMLCh* fullName = fBuffer.getRawBuffer();
    int    fullTypeName = fStringPool->addOrFind(fullName);

    //check if we have already traversed the same simpleType decl
    if (fDatatypeRegistry->getDatatypeValidator(fullName)!= 0) {
        return fStringPool->addOrFind(fullName);
    }

    // Circular constraint checking
    if (fCurrentTypeNameStack->containsElement(fullTypeName)){

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoCircularDefinition, name);
        return -1;
    }

    fCurrentTypeNameStack->addElement(fullTypeName);

    // Get 'final' values
    const XMLCh* finalVal = getElementAttValue(childElem, SchemaSymbols::fgATT_FINAL);
    int finalSet = parseFinalSet(finalVal, S_Final);

    // annotation?,(list|restriction|union)
    DOM_Element content= checkContent(childElem,
                                      XUtil::getFirstChildElement(childElem),
                                      false);

    if (content == 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::EmptySimpleTypeContent);
        return resetCurrentTypeNameStack(-1);
    }

    DOMString varietyName = content.getLocalName();
    int newSimpleTypeName = fStringPool->addOrFind(name);

    // -----------------------------------------------------------------------
    // Check Attributes
    // -----------------------------------------------------------------------
    fAttributeCheck->checkAttributes(content, GeneralAttributeCheck::LocalContext, this);

    // Remark: some code will be repeated in list|restriction| union but it
    //         is cleaner that way
    if (varietyName.equals(SchemaSymbols::fgELT_LIST)) { //traverse List
        if (baseRefContext & SchemaSymbols::LIST != 0) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::AtomicItemType);
            return resetCurrentTypeNameStack(-1);
        }

        return traverseByList(childElem, content, newSimpleTypeName, finalSet);
    }
    else if (varietyName.equals(SchemaSymbols::fgELT_RESTRICTION)) { //traverse Restriction
        return traverseByRestriction(childElem, content, newSimpleTypeName, finalSet);
    }
    else if (varietyName.equals(SchemaSymbols::fgELT_UNION)) { //traverse union
        if (baseRefContext & SchemaSymbols::UNION != 0) {

           reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::MemberTypeNoUnion);
           return resetCurrentTypeNameStack(-1);
        }

        return traverseByUnion(childElem, content, newSimpleTypeName, finalSet, baseRefContext);
    }
    else {

        fBuffer.set(varietyName.rawBuffer(), varietyName.length());
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::FeatureUnsupported,fBuffer.getRawBuffer());
    }

    return resetCurrentTypeNameStack(-1);
}

/**
  * Traverse ComplexType Declaration - CR Implementation.
  *
  *     <complexType
  *        abstract = boolean
  *        block = #all or (possibly empty) subset of {extension, restriction}
  *        final = #all or (possibly empty) subset of {extension, restriction}
  *        id = ID
  *        mixed = boolean : false
  *        name = NCName>
  *        Content: (annotation? , (simpleContent | complexContent |
  *                   ( (group | all | choice | sequence)? ,
  *                   ( (attribute | attributeGroup)* , anyAttribute?))))
  *     </complexType>
  */
int TraverseSchema::traverseComplexTypeDecl(const DOM_Element& elem) {

    // Get the attributes of the complexType
    const XMLCh* name = getElementAttValue(elem, SchemaSymbols::fgATT_NAME);
    bool  topLevel = isTopLevelComponent(elem);

    if (XMLString::stringLen(name) == 0) {

        if (topLevel) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::TopLevelNoNameComplexType);
            return -1;
        }

        name = genAnonTypeName(fgAnonCNamePrefix, fComplexTypeAnonCount);
    }

    if (!XMLString::isValidNCName(name)) {

        //REVISIT - Should we return or continue and save type with wrong name?
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidDeclarationName,
                          SchemaSymbols::fgELT_COMPLEXTYPE, name);
        return -1;
    }

    // -----------------------------------------------------------------------
    // Check Attributes
    // -----------------------------------------------------------------------
    unsigned short scope = (topLevel) ? GeneralAttributeCheck::GlobalContext
                                      : GeneralAttributeCheck::LocalContext;
    fAttributeCheck->checkAttributes(elem, scope, this);

    // ------------------------------------------------------------------
    // Check if the type has already been registered
    // ------------------------------------------------------------------
    fBuffer.set(fTargetNSURIString);
    fBuffer.append(chComma);
    fBuffer.append(name);

    XMLCh* fullName = fBuffer.getRawBuffer();
    if (topLevel) {

        ComplexTypeInfo* temp = fComplexTypeRegistry->get(fullName);

        if (temp != 0 ) {
            return fStringPool->addOrFind(fullName);
        }
    }

    ComplexTypeInfo* typeInfo = new ComplexTypeInfo();
    int typeNameIndex = fStringPool->addOrFind(fullName);
    int previousScope = fCurrentScope;
    fCurrentScope = fScopeCount++;

    // ------------------------------------------------------------------
    // First, handle any ANNOTATION declaration and get next child
    // ------------------------------------------------------------------
    DOM_Element child = checkContent(elem, XUtil::getFirstChildElement(elem),
                                     true);

    // ------------------------------------------------------------------
    // Register the type
    // ------------------------------------------------------------------
    // Register the type first, so that in case of a recursive element type
    // declaration, we can retrieve the complexType info (though the rest of
    fComplexTypeRegistry->put((void*) fStringPool->getValueForId(typeNameIndex),
                              typeInfo);
    typeInfo->setTypeName(fullName);
    typeInfo->setScopeDefined(fCurrentScope);
    fCurrentTypeNameStack->addElement(typeNameIndex);
    ComplexTypeInfo* saveTypeInfo = fCurrentComplexType;
    fCurrentComplexType = typeInfo;

    // ------------------------------------------------------------------
    // Process the content of the complex type declaration
    // ------------------------------------------------------------------
    try {
        if (child == 0) {
            // EMPTY complexType with complexContent
            processComplexContent(name, child, typeInfo, 0,0,0, false);
        }
        else {

            DOMString childName = child.getLocalName();
            const XMLCh* mixedVal = getElementAttValue(elem,SchemaSymbols::fgATT_MIXED);
            bool isMixed = false;

            if (XMLString::stringLen(mixedVal)
                && (!XMLString::compareString(SchemaSymbols::fgATTVAL_TRUE, mixedVal)
                    || !XMLString::compareString(fgValueOne, mixedVal))) {
                isMixed = true;
            }

            if (childName.equals(SchemaSymbols::fgELT_SIMPLECONTENT)) {

                // SIMPLE CONTENT element
                traverseSimpleContentDecl(name, child, typeInfo);

                if (XUtil::getNextSiblingElement(child) != 0) {
                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidChildFollowingSimpleContent);
                }
            }
            else if (childName.equals(SchemaSymbols::fgELT_COMPLEXCONTENT)) {

                // COMPLEX CONTENT element
                traverseComplexContentDecl(name, child, typeInfo, isMixed);

                if (XUtil::getNextSiblingElement(child) != 0) {
                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidChildFollowingConplexContent);
                }
            }
            else {
                // We must have ....
                // GROUP, ALL, SEQUENCE or CHOICE, followed by optional attributes
                // Note that it's possible that only attributes are specified.
                processComplexContent(name, child, typeInfo, 0, 0, 0, isMixed);
            }
        }
    }
    catch(TraverseSchema::ExceptionCodes) {
        defaultComplexTypeInfo(typeInfo);
    }

    // ------------------------------------------------------------------
    // Finish the setup of the typeInfo
    // ------------------------------------------------------------------
    const XMLCh* blockAttVal = getElementAttValue(elem, SchemaSymbols::fgATT_BLOCK);
    const XMLCh* finalAttVal = getElementAttValue(elem, SchemaSymbols::fgATT_FINAL);
    const XMLCh* abstractAttVal = getElementAttValue(elem, SchemaSymbols::fgATT_ABSTRACT);
    int blockSet = parseBlockSet(blockAttVal, C_Block);
    int finalSet = parseFinalSet(finalAttVal, ECS_Final);

    typeInfo->setBlockSet(blockSet);
    typeInfo->setFinalSet(finalSet);

    if (XMLString::stringLen(abstractAttVal)
        && (!XMLString::compareString(abstractAttVal, SchemaSymbols::fgATTVAL_TRUE)
            || !XMLString::compareString(abstractAttVal, fgValueOne))) {
        typeInfo->setAbstract(true);
    }
    else {
        typeInfo->setAbstract(false);
    }

    // ------------------------------------------------------------------
    // Before exiting, restore the scope, mainly for nested anonymous types
    // ------------------------------------------------------------------
    fCurrentScope = previousScope;
    fCurrentComplexType = saveTypeInfo;
    resetCurrentTypeNameStack(0);


    return typeNameIndex;
}

/**
  * Traverse Group Declaration.
  *
  * <group
  *         id = ID
  *         maxOccurs = string
  *         minOccurs = nonNegativeInteger
  *         name = NCName
  *         ref = QName>
  *   Content: (annotation? , (all | choice | sequence)?)
  * <group/>
  *
  */
XercesGroupInfo*
TraverseSchema::traverseGroupDecl(const DOM_Element& elem,
                                  bool& toAdoptSpecNode) {

    bool         topLevel = isTopLevelComponent(elem);
    const XMLCh* name = getElementAttValue(elem, SchemaSymbols::fgATT_NAME);
    const XMLCh* ref = getElementAttValue(elem, SchemaSymbols::fgATT_REF);
    bool         nameEmpty = (XMLString::stringLen(name) == 0) ? true : false;
    bool         refEmpty = (XMLString::stringLen(ref) == 0) ? true : false;

    if (nameEmpty && topLevel) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoNameGlobalElement,
                          SchemaSymbols::fgELT_GROUP);
        return 0;
    }

    if (nameEmpty && refEmpty) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoNameRefGroup);
        return 0;
    }

    // ------------------------------------------------------------------
    // Check attributes
    // ------------------------------------------------------------------
    unsigned short scope = (topLevel) ? GeneralAttributeCheck::GlobalContext
                                      : GeneralAttributeCheck::LocalContext;
    fAttributeCheck->checkAttributes(elem, scope, this);

    // ------------------------------------------------------------------
    // Check for annotations
    // ------------------------------------------------------------------
    DOM_Element content = checkContent(elem, XUtil::getFirstChildElement(elem),
                                       true);

    // ------------------------------------------------------------------
    // Handle "ref="
    // ------------------------------------------------------------------
    if (nameEmpty || (!refEmpty && !topLevel)) {

        if (!nameEmpty) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DeclarationWithNameRef,
                              SchemaSymbols::fgELT_GROUP, name);
        }

        return processGroupRef(elem, ref, toAdoptSpecNode);
    }

    // ------------------------------------------------------------------
    // Process contents of global groups
    // ------------------------------------------------------------------
    // name must be a valid NCName
    if (!XMLString::isValidNCName(name)) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidDeclarationName,
                          SchemaSymbols::fgELT_GROUP, name);
        return 0;
    }

    fBuffer.set(fTargetNSURIString);
    fBuffer.append(chComma);
    fBuffer.append(name);

    int saveScope = fCurrentScope;
    unsigned int fullNameIndex = fStringPool->addOrFind(fBuffer.getRawBuffer());
    const XMLCh* fullName = fStringPool->getValueForId(fullNameIndex);
    ContentSpecNode* specNode = 0;
    XercesGroupInfo* saveGroupInfo = fCurrentGroupInfo;
    XercesGroupInfo* groupInfo = new XercesGroupInfo();

    fCurrentGroupStack->addElement(fullNameIndex);
    fCurrentGroupInfo = groupInfo;

    if (!saveGroupInfo && !fCurrentComplexType) {
        fCurrentScope = fScopeCount++;
    }

    fCurrentGroupInfo->setScope(fCurrentScope);

    if (content == 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::GroupContentError, name);
    }
    else {

        if (elem.getAttributeNode(SchemaSymbols::fgATT_MINOCCURS) != 0
            || elem.getAttributeNode(SchemaSymbols::fgATT_MAXOCCURS) != 0) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::MinMaxOnGroupChild);
        }

        bool illegalChild = false;
        bool adoptSpecNode = true;
        DOMString childName = content.getLocalName();

        if (childName.equals(SchemaSymbols::fgELT_SEQUENCE)) {
            specNode = traverseChoiceSequence(content, ContentSpecNode::Sequence, adoptSpecNode);
        }
        else if (childName.equals(SchemaSymbols::fgELT_CHOICE)) {
            specNode = traverseChoiceSequence(content, ContentSpecNode::Choice, adoptSpecNode);
        }
        else if (childName.equals(SchemaSymbols::fgELT_ALL)) {
            // TO DO
        }
        else {
            illegalChild = true;
        }

        if (illegalChild || XUtil::getNextSiblingElement(content) != 0) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::GroupContentError, name);
        }

        toAdoptSpecNode = adoptSpecNode;
    }

    // ------------------------------------------------------------------
    // Set groupInfo and pop group name from stack
    // ------------------------------------------------------------------
    unsigned int stackSize = fCurrentGroupStack->size();

    if (stackSize != 0) {
        fCurrentGroupStack->removeElementAt(stackSize - 1);
    }

    fCurrentGroupInfo->setContentSpec(specNode);
    fGroupRegistry->put((void*) fullName, fCurrentGroupInfo);
    fCurrentGroupInfo = saveGroupInfo;
    fCurrentScope = saveScope;

    return groupInfo;
}

XercesGroupInfo*
TraverseSchema::traverseGroupDeclNS(const XMLCh* const uriStr,
                                    const XMLCh* const name,
                                    bool& toAdoptSpecNode) {

    // ------------------------------------------------------------------
    // Get grammar information
    // ------------------------------------------------------------------
    SchemaGrammar* aGrammar = (SchemaGrammar*) fGrammarResolver->getGrammar(uriStr);

    if (!aGrammar || aGrammar->getGrammarType() != Grammar::SchemaGrammarType) {

        reportSchemaError(XMLUni::fgValidityDomain, XMLValid::GrammarNotFound, uriStr);
        return 0;
    }

    fBuffer.set(uriStr);
    fBuffer.append(chComma);
    fBuffer.append(name);

    XercesGroupInfo* groupInfo = aGrammar->getGroupInfoRegistry()->get(fBuffer.getRawBuffer());

    if (!groupInfo) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DeclarationNotFound,
                          SchemaSymbols::fgELT_GROUP, uriStr, name);
    }
    else {
        toAdoptSpecNode = false;
    }

    return groupInfo;
}


/**
  * Traverse attributeGroup Declaration.
  *
  * <attributeGroup
  *         id = ID
  *         name = NCName
  *         ref = QName>
  *   Content: (annotation? , (attribute|attributeGroup)*, anyAttribute?)
  * <attributeGroup/>
  *
  */
XercesAttGroupInfo*
TraverseSchema::traverseAttributeGroupDecl(const DOM_Element& elem,
                                           ComplexTypeInfo* const typeInfo) {

    bool         topLevel = isTopLevelComponent(elem);
    const XMLCh* name = getElementAttValue(elem, SchemaSymbols::fgATT_NAME);
    const XMLCh* ref = getElementAttValue(elem, SchemaSymbols::fgATT_REF);
    bool         nameEmpty = (XMLString::stringLen(name) == 0) ? true : false;
    bool         refEmpty = (XMLString::stringLen(ref) == 0) ? true : false;

    if (nameEmpty && topLevel) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoNameGlobalElement,
			SchemaSymbols::fgELT_ATTRIBUTEGROUP);
        return 0;
    }

    if (nameEmpty && refEmpty) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoNameRefAttGroup);
        return 0;
    }

    // ------------------------------------------------------------------
    // Check attributes
    // ------------------------------------------------------------------
    unsigned short scope = (topLevel) ? GeneralAttributeCheck::GlobalContext
                                      : GeneralAttributeCheck::LocalContext;
    fAttributeCheck->checkAttributes(elem, scope, this);

    // ------------------------------------------------------------------
    // Handle "ref="
    // ------------------------------------------------------------------
    if (nameEmpty || (!refEmpty && !topLevel)) {

        if (!nameEmpty) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DeclarationWithNameRef,
                              SchemaSymbols::fgELT_ATTRIBUTEGROUP, name);
        }

        return processAttributeGroupRef(elem, ref, typeInfo);
    }


    // ------------------------------------------------------------------
    // Handle "name="
    // ------------------------------------------------------------------
    // name must be a valid NCName
    if (!XMLString::isValidNCName(name)) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidDeclarationName,
                          SchemaSymbols::fgELT_ATTRIBUTEGROUP, name);
        return 0;
    }

    // ------------------------------------------------------------------
    // Check for annotations
    // ------------------------------------------------------------------
    DOM_Element content = checkContent(elem, XUtil::getFirstChildElement(elem),
                                       true);

    // ------------------------------------------------------------------
    // Process contents of global attributeGroups
    // ------------------------------------------------------------------
    XercesAttGroupInfo* saveAttGroupInfo = fCurrentAttGroupInfo;
    XercesAttGroupInfo* attGroupInfo = new XercesAttGroupInfo();

    fAttGroupRegistry->put((void*) name, attGroupInfo);
    fCurrentAttGroupInfo = attGroupInfo;

    for (; content !=0; content = XUtil::getNextSiblingElement(content)) {

        if (content.getLocalName().equals(SchemaSymbols::fgELT_ATTRIBUTE)) {
            traverseAttributeDecl(content, typeInfo);
        }
        else if (content.getLocalName().equals(SchemaSymbols::fgELT_ATTRIBUTEGROUP)) {
            traverseAttributeGroupDecl(content, typeInfo);
        }
        else {
            break;
        }
    }

    if (content != 0) {

        if (content.getLocalName().equals(SchemaSymbols::fgELT_ANYATTRIBUTE)) {

            SchemaAttDef* anyAtt = traverseAnyAttribute(content);

            if (anyAtt) {
                fCurrentAttGroupInfo->addAnyAttDef(anyAtt);
            }

            if (XUtil::getNextSiblingElement(content) != 0) {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::AttGroupContentError, name);
            }
        }
        else {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::AttGroupContentError, name);
        }
    }

    // ------------------------------------------------------------------
    // Restore old attGroupInfo
    // ------------------------------------------------------------------
    fCurrentAttGroupInfo = saveAttGroupInfo;

    return attGroupInfo;
}


XercesAttGroupInfo*
TraverseSchema::traverseAttributeGroupDeclNS(const XMLCh* const uriStr,
                                             const XMLCh* const name) {

    // ------------------------------------------------------------------
    // Get grammar information
    // ------------------------------------------------------------------
    SchemaGrammar* aGrammar = (SchemaGrammar*) fGrammarResolver->getGrammar(uriStr);

    if (!aGrammar || aGrammar->getGrammarType() != Grammar::SchemaGrammarType) {

        reportSchemaError(XMLUni::fgValidityDomain, XMLValid::GrammarNotFound, uriStr);
        return 0;
    }

    XercesAttGroupInfo* attGroupInfo = aGrammar->getAttGroupInfoRegistry()->get(name);

    if (!attGroupInfo) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DeclarationNotFound,
                          SchemaSymbols::fgELT_ATTRIBUTEGROUP, uriStr, name);
    }

    return attGroupInfo;
}


/**
  * Traverse Any declaration
  *
  *     <any
  *        id = ID
  *        maxOccurs = (nonNegativeInteger | unbounded)  : 1
  *        minOccurs = nonNegativeInteger : 1
  *        namespace = ((##any | ##other) | List of (anyURI |
  *                     (##targetNamespace | ##local)) )  : ##any
  *        processContents = (lax | skip | strict) : strict
  *        {any attributes with non-schema namespace . . .}>
  *        Content: (annotation?)
  *     </any>
  */
ContentSpecNode*
TraverseSchema::traverseAny(const DOM_Element& elem) {

    // -----------------------------------------------------------------------
    // Check Attributes
    // -----------------------------------------------------------------------
    unsigned short scope = GeneralAttributeCheck::LocalContext;
    fAttributeCheck->checkAttributes(elem, scope, this);

    // ------------------------------------------------------------------
    // First, handle any ANNOTATION declaration
    // ------------------------------------------------------------------
    if (checkContent(elem, XUtil::getFirstChildElement(elem), true) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::OnlyAnnotationExpected);
    }

    // ------------------------------------------------------------------
    // Get attributes
    // ------------------------------------------------------------------
    const XMLCh* const processContents =
            getElementAttValue(elem, SchemaSymbols::fgATT_PROCESSCONTENTS);
    const XMLCh* const nameSpace =
            getElementAttValue(elem, SchemaSymbols::fgATT_NAMESPACE);

    // ------------------------------------------------------------------
    // Set default node type based on 'processContents' value
    // ------------------------------------------------------------------
    ContentSpecNode::NodeTypes anyType = ContentSpecNode::Any;
    ContentSpecNode::NodeTypes anyLocalType = ContentSpecNode::Any_NS;
    ContentSpecNode::NodeTypes anyOtherType = ContentSpecNode::Any_Other;

    if (XMLString::stringLen(processContents) > 0
        && XMLString::compareString(processContents, fgStrict)) {

        if (!XMLString::compareString(processContents, fgLax)) {

            anyType = ContentSpecNode::Any_Lax;
            anyOtherType = ContentSpecNode::Any_Other_Lax;
            anyLocalType = ContentSpecNode::Any_NS_Lax;
        }
        else if (!XMLString::compareString(processContents, fgSkip)) {

            anyType = ContentSpecNode::Any_Skip;
            anyOtherType = ContentSpecNode::Any_Other_Skip;
            anyLocalType = ContentSpecNode::Any_NS_Skip;
        }
    }

    // ------------------------------------------------------------------
    // Process 'namespace' attribute
    // ------------------------------------------------------------------
    ContentSpecNode* retSpecNode = 0;
    QName elemName(XMLUni::fgZeroLenString, XMLUni::fgZeroLenString,
                   fEmptyNamespaceURI);

    if (XMLString::stringLen(nameSpace) == 0
        || !XMLString::compareString(nameSpace, SchemaSymbols::fgATTVAL_TWOPOUNDANY)) {

        retSpecNode = new ContentSpecNode(&elemName);
        retSpecNode->setType(anyType);
    }
    else if (!XMLString::compareString(nameSpace, SchemaSymbols::fgATTVAL_TWOPOUNDOTHER)) {

        elemName.setURI(fTargetNSURI);
        retSpecNode = new ContentSpecNode(&elemName);
        retSpecNode->setType(anyOtherType);
    }
    else {

        RefVectorOf<XMLCh>* nameSpaceTokens = XMLString::tokenizeString(nameSpace);
        ValueVectorOf<unsigned int> uriList(8);
        ContentSpecNode* firstNode = 0;
        ContentSpecNode* secondNode = 0;
        unsigned int tokensSize = nameSpaceTokens->size();

        for (unsigned int i=0; i < tokensSize; i++) {

            const XMLCh* tokenElem = nameSpaceTokens->elementAt(i);
            int uriIndex = fEmptyNamespaceURI;

            if (XMLString::compareString(tokenElem,SchemaSymbols::fgATTVAL_TWOPOUNDLOCAL)) { // not ##local

                if (!XMLString::compareString(tokenElem,SchemaSymbols::fgATTVAL_TWOPOUNDTRAGETNAMESPACE)) {
                    uriIndex = fTargetNSURI;
                }
                else {
                    uriIndex = fURIStringPool->addOrFind(tokenElem);
                }
            }

            if (uriList.containsElement(uriIndex)) {
                continue;
            }

            uriList.addElement(uriIndex);
            elemName.setURI(uriIndex);
            firstNode = new ContentSpecNode(&elemName);
            firstNode->setType(anyLocalType);

            if (secondNode == 0) {
                secondNode = firstNode;
            }
            else {
                secondNode = new ContentSpecNode(ContentSpecNode::Choice, secondNode, firstNode);
            }
        }

        retSpecNode = secondNode;
        delete nameSpaceTokens;
    }

    return retSpecNode;
}


/**
  *  Traverse all
  *
  *     <all
  *        id = ID
  *        maxOccurs = 1 : 1
  *        minOccurs = (0 | 1) : 1
  *        {any attributes with non-schema namespace . . .}>
  *        Content: (annotation?, element*)
  *     </all>
  */
ContentSpecNode*
TraverseSchema::traverseAll(const DOM_Element& elem) {

    return 0;

    // Work in progress
    // ------------------------------------------------------------------
    // Check attributes
    // ------------------------------------------------------------------
    unsigned short scope = GeneralAttributeCheck::LocalContext;
    fAttributeCheck->checkAttributes(elem, scope, this);

    // ------------------------------------------------------------------
    // Process contents
    // ------------------------------------------------------------------
    DOM_Element child = checkContent(elem, XUtil::getFirstChildElement(elem), true);

    if (child == 0) {
        return 0;
    }

    ContentSpecNode* left = 0;
    RefVectorOf<ContentSpecNode>* allChildren = 0;

    for (; child != 0; child = XUtil::getNextSiblingElement(child)) {

        ContentSpecNode* contentSpecNode = 0;
        bool seeParticle = false;
        DOMString childName = child.getLocalName();

        if (childName.equals(SchemaSymbols::fgELT_ELEMENT)) {

            QName* eltQName = traverseElementDecl(child);
            Janitor<QName> janQName(eltQName);

            if (eltQName == 0) {
                continue;
            }

            contentSpecNode = new ContentSpecNode(eltQName);
            seeParticle = true;
        }
        else {
            fBuffer.set(childName.rawBuffer(), childName.length());
            reportSchemaError(0, 0, fBuffer.getRawBuffer()); //"Content of all group is restricted to elements only. '{0}' encountered and ignored."
        }

        if (seeParticle) {
            checkMinMax(contentSpecNode, child);
        }

        if (contentSpecNode != 0) {

            if (allChildren == 0) {
                allChildren = new RefVectorOf<ContentSpecNode>(32, false);
            }

            allChildren->addElement(contentSpecNode);
        }
    }

    if (!allChildren || allChildren->size() == 0) {
        return 0;
    }

    try {
//        left = allCalcWrapper(allChildren);
    }
    catch(...) {

        delete allChildren;
        reportSchemaError(0, 0); //"The size of the &lt;all&gt; declaration is too large for this parser and elements using it will not validate correctly"
    }

    return left;
}

/**
  * Traverses Schema attribute declaration.
  *
  *       <attribute
  *         fixed = string
  *         default = string
  *         form = qualified | unqualified
  *         id = ID
  *         name = NCName
  *         ref = QName
  *         type = QName
  *         use = optional | prohibited | required : optional
  >
  *         Content: (annotation? , simpleType?)
  *       <attribute/>
  *
  * @param elem:        the declaration of the attribute under consideration
  *
  * @param typeInfo:    Contains the complex type info of the element to which
  *                     the attribute declaration is attached.
  *
  */
void TraverseSchema::traverseAttributeDecl(const DOM_Element& elem,
                                           ComplexTypeInfo* const typeInfo) {

    bool           topLevel = isTopLevelComponent(elem);
    const XMLCh*   name = getElementAttValue(elem, SchemaSymbols::fgATT_NAME);
    const XMLCh*   ref = getElementAttValue(elem, SchemaSymbols::fgATT_REF);
    bool           nameEmpty = (XMLString::stringLen(name) == 0) ? true : false;
    bool           refEmpty = (XMLString::stringLen(ref) == 0) ? true : false;

    if (nameEmpty && refEmpty) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoNameRefAttribute);
        return;
    }

    if (topLevel) {

        if (nameEmpty) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::TopLevelNoNameAttribute);
            return;
        }
        else {
            if (!refEmpty) {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::GlobalAttributeWithNameRef, name);
            }
        }
    }
    else {
        if (!typeInfo && !fCurrentAttGroupInfo) {
            // Nothing to do - where to save that attribute info?
            return;
        }
    }

    // ------------------------------------------------------------------
    // Check attributes
    // ------------------------------------------------------------------
    unsigned short scope = (topLevel) ? GeneralAttributeCheck::GlobalContext
                                      : GeneralAttributeCheck::LocalContext;
    fAttributeCheck->checkAttributes(elem, scope, this);

    const XMLCh* defaultVal = getElementAttValue(elem, SchemaSymbols::fgATT_DEFAULT);
    const XMLCh* fixedVal = getElementAttValue(elem, SchemaSymbols::fgATT_FIXED);
    const XMLCh* useVal = getElementAttValue(elem, SchemaSymbols::fgATT_USE);
    DOM_Element  simpleType = checkContent(elem, XUtil::getFirstChildElement(elem), true);

    if(XMLString::stringLen(defaultVal) != 0) {

        if (XMLString::stringLen(fixedVal) != 0) {

            fixedVal = 0;
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::AttributeDefaultFixedValue);
        }

        if (XMLString::stringLen(useVal) != 0
            && XMLString::compareString(useVal, SchemaSymbols::fgATTVAL_OPTIONAL) != 0) {

            useVal = 0;
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NotOptionalDefaultAttValue);
        }
    }

    // processing ref
    if (nameEmpty || (!refEmpty && !topLevel)) {

        if (!nameEmpty) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::LocalAttributeWithNameRef, name);
        }

        processAttributeDeclRef(elem, simpleType, typeInfo, ref, useVal);
        return;
    }

    // processing 'name'
    if (!XMLString::isValidNCName(name)
        || XMLString::compareString(name, XMLUni::fgXMLNSString) == 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidDeclarationName, SchemaSymbols::fgELT_ATTRIBUTE, name);
        return;
    }

    // Check for duplicate declaration
    // REVISIT - need to add attributeGroup check
    const XMLCh* attForm = getElementAttValue(elem, SchemaSymbols::fgATT_FORM);
    const XMLCh* qualified = SchemaSymbols::fgATTVAL_QUALIFIED;
    int uriIndex = fEmptyNamespaceURI;

    if (XMLString::stringLen(fTargetNSURIString) != 0
        && (topLevel || XMLString::compareString(attForm, qualified) == 0
            || (fAttributeDefaultQualified
                && XMLString::stringLen(attForm) == 0))) {
        uriIndex = fTargetNSURI;
    }

    if (topLevel) {
        if (fAttributeDeclRegistry->containsKey(name)) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateAttribute, name);
            return;
        }
    }
    else if (typeInfo && typeInfo->getAttDef(name, uriIndex) != 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateAttribute, name);
        return;
    }
    else if (fCurrentAttGroupInfo && fCurrentAttGroupInfo->containsAttribute(name, uriIndex)) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateAttribute, name);
        return;
    }

    const XMLCh* typeAttr = getElementAttValue(elem, SchemaSymbols::fgATT_TYPE);
    DatatypeValidator*  dv = 0;
    XMLAttDef::AttTypes attType;
    DOMString contentName;

    while (simpleType != 0) {

        contentName = simpleType.getLocalName();
        fBuffer.set(contentName.rawBuffer(), contentName.length());

        if (XMLString::compareString(SchemaSymbols::fgELT_SIMPLETYPE,
                                     fBuffer.getRawBuffer()) == 0) {
            break;
        }

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidAttributeContent, fBuffer.getRawBuffer());
        simpleType = XUtil::getNextSiblingElement(simpleType);
    }

    if (simpleType != 0) {

        if (XMLString::stringLen(typeAttr) != 0) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::AttributeWithTypeAndSimpleType);
        }

        int datatypeSymbol = traverseSimpleTypeDecl(simpleType);

        attType = XMLAttDef::Simple;

        if (datatypeSymbol != -1) {
            dv = fDatatypeRegistry->getDatatypeValidator(fStringPool->getValueForId(datatypeSymbol));
        }

    }
    else if (XMLString::stringLen(typeAttr) == 0) {

        attType = XMLAttDef::Simple;
        dv = fDatatypeRegistry->getDatatypeValidator(SchemaSymbols::fgDT_STRING);
    }
    else {

        const XMLCh* localPart = getLocalPart(typeAttr);
        const XMLCh* prefix = getPrefix(typeAttr);
        const XMLCh* typeURI = resolvePrefixToURI(prefix);
        //temporary flag to bypass error message in case (ID/IDREF/ENTITY(s)) validators not found
        //REVISIT - remove flag once actual validators are available
        bool  bypassErrorMsg = true;

        if (XMLString::compareString(typeURI, SchemaSymbols::fgURI_SCHEMAFORSCHEMA)  == 0) {

            dv = getDatatypeValidator(SchemaSymbols::fgURI_SCHEMAFORSCHEMA, localPart);

            if (XMLString::compareString(localPart,XMLUni::fgIDString) == 0) {
                attType = XMLAttDef::ID;
            }
            else if (XMLString::compareString(localPart,XMLUni::fgIDRefString) == 0) {
                attType = XMLAttDef::IDRef;
            }
            else if (XMLString::compareString(localPart,XMLUni::fgIDRefsString) == 0) {
                attType = XMLAttDef::IDRefs;
            }
            else if (XMLString::compareString(localPart,XMLUni::fgEntityString) == 0) {
                attType = XMLAttDef::Entity;
            }
            else if (XMLString::compareString(localPart,XMLUni::fgEntitiesString) == 0) {
                attType = XMLAttDef::Entities;
            }
            else if (XMLString::compareString(localPart,XMLUni::fgNmTokenString) == 0) {
                bypassErrorMsg = false;
                attType = XMLAttDef::NmToken;
            }
            else if (XMLString::compareString(localPart,XMLUni::fgNmTokensString) == 0) {
                bypassErrorMsg = false;
                attType = XMLAttDef::NmTokens;
            }
            else if (XMLString::compareString(localPart,XMLUni::fgNotationString) == 0) {
                bypassErrorMsg = false;
                attType = XMLAttDef::Notation;
            }
            else {

                bypassErrorMsg = false;
                attType = XMLAttDef::Simple;

                if (dv == 0 && XMLString::stringLen(typeURI) == 0) {

                    DOM_Element topLevelType =
                        getTopLevelComponentByName(SchemaSymbols::fgELT_SIMPLETYPE, localPart);

                    if (topLevelType != 0) {

                        traverseSimpleTypeDecl(topLevelType);
                        dv = getDatatypeValidator(typeURI, localPart);
                    }
                    else {
                        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::AttributeSimpleTypeNotFound,
                                          typeURI, localPart, name);
                    }
                }
            }
        }
        else { //isn't of the schema for schemas namespace...

            bypassErrorMsg = false;
            // check if the type is from the same Schema
            dv = getDatatypeValidator(typeURI, localPart);

            if (dv == 0
                && XMLString::compareString(typeURI, fTargetNSURIString) == 0) {

                DOM_Element topLevelType =
                    getTopLevelComponentByName(SchemaSymbols::fgELT_SIMPLETYPE,
                                               localPart);

                if (topLevelType != 0) {

                    traverseSimpleTypeDecl(topLevelType);
                    dv = getDatatypeValidator(typeURI, localPart);
                }
                else {
                        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::AttributeSimpleTypeNotFound,
                                          typeURI, localPart, name);
                }
            }

            attType = XMLAttDef::Simple;
        }

        if (!dv && !bypassErrorMsg) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::TypeNotFound, typeURI, localPart);
        }
    }

    bool required = false;
    bool prohibited = false;

    if (XMLString::stringLen(useVal) != 0) {

        if (XMLString::compareString(useVal,
                            SchemaSymbols::fgATTVAL_REQUIRED) == 0) {
            required = true;
        }
        else if (XMLString::compareString(useVal,
                            SchemaSymbols::fgATTVAL_PROHIBITED) == 0) {
            prohibited = true;
        }
    }

    // validate fixed/default values
    const XMLCh* valueToCheck = XMLString::stringLen(defaultVal) != 0
                                    ? defaultVal : fixedVal;

    if (attType == XMLAttDef::Simple && dv != 0
        && XMLString::stringLen(valueToCheck) != 0) {

        try {
            dv->validate(valueToCheck);
        }
        catch(const InvalidDatatypeValueException& idve) {
            reportSchemaError(XMLUni::fgValidityDomain,
                              XMLValid::DisplayErrorMessage, idve.getMessage());
        }
        catch (const InvalidDatatypeFacetException& idfe) {
            reportSchemaError(XMLUni::fgValidityDomain,
                              XMLValid::DisplayErrorMessage, idfe.getMessage());
        }
        catch(...) {
            reportSchemaError(XMLUni::fgValidityDomain,
                              XMLValid::DatatypeValidationFailure, valueToCheck);
        }
    }

    // create SchemaAttDef
    SchemaAttDef* attDef =
        new SchemaAttDef(XMLUni::fgZeroLenString, name, uriIndex, attType);

    attDef->setDatatypeValidator(dv);

    if (prohibited) {
        attDef->setDefaultType(XMLAttDef::Prohibited);
    }
    else if (required) {

        if (XMLString::stringLen(fixedVal) == 0) {
            attDef->setDefaultType(XMLAttDef::Required);
        }
        else {
            attDef->setDefaultType(XMLAttDef::Required_And_Fixed);
        }
    }
    else {

        if (XMLString::stringLen(fixedVal) != 0) {
            attDef->setDefaultType(XMLAttDef::Fixed);
        }
        else if (XMLString::stringLen(defaultVal) != 0) {
            attDef->setDefaultType(XMLAttDef::Default);
        }
    }

    if (XMLString::stringLen(valueToCheck) != 0) {
        attDef->setValue(valueToCheck);
    }

    if (topLevel) {
        fAttributeDeclRegistry->put((void*) name, attDef);
    }
    else {
	
        bool toClone = false;		

        if (typeInfo) {

            toClone = true;
            typeInfo->addAttDef(attDef);
        }

        if (fCurrentAttGroupInfo) {
            fCurrentAttGroupInfo->addAttDef(attDef, toClone);
        }
    }
}


/**
  * Traverses Schema element declaration.
  *
  *       <element
  *            abstract = boolean : false
  *            block = (#all | List of (substitution | extension | restriction
  *                                     | list | union))
  *            default = string
  *            final = (#all | List of (extension | restriction))
  *            fixed = string
  *            form = (qualified | unqualified)
  *            id = ID
  *            maxOccurs = (nonNegativeInteger | unbounded)  : 1
  *            minOccurs = nonNegativeInteger : 1
  *            name = NCName
  *            nillable = boolean : false
  *            ref = QName
  *            substitutionGroup = QName
  *            type = QName
  *            Content: (annotation?, ((simpleType | complexType)?, (unique | key | keyref)*))
  *       </element>
  *
  * @param elem:  the declaration of the element under consideration
  */
QName* TraverseSchema::traverseElementDecl(const DOM_Element& elem) {

    bool         topLevel = isTopLevelComponent(elem);
    const XMLCh* name = getElementAttValue(elem, SchemaSymbols::fgATT_NAME);
    const XMLCh* ref = getElementAttValue(elem, SchemaSymbols::fgATT_REF);
    const XMLCh* fixed = getElementAttValue(elem, SchemaSymbols::fgATT_FIXED);
    const XMLCh* deflt = getElementAttValue(elem, SchemaSymbols::fgATT_DEFAULT);
    bool         nameEmpty = (XMLString::stringLen(name) == 0) ? true : false;
    bool         refEmpty = (XMLString::stringLen(ref) == 0) ? true : false;

    if (nameEmpty && topLevel) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::GlobalNoNameElement);
        return 0;
    }

    if (nameEmpty && refEmpty) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoNameRefElement);
        return 0;
    }

    // ------------------------------------------------------------------
    // Check attributes
    // ------------------------------------------------------------------
    unsigned short scope = (topLevel) ? GeneralAttributeCheck::GlobalContext
                                      : GeneralAttributeCheck::LocalContext;
    fAttributeCheck->checkAttributes(elem, scope, this);

    // ------------------------------------------------------------------
    // Process contents
    // ------------------------------------------------------------------
    if(XMLString::stringLen(fixed) != 0 && XMLString::stringLen(deflt) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ElementWithFixedAndDefault);
    }

    if (nameEmpty || (!refEmpty && !topLevel)) {

        if (!nameEmpty) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DeclarationWithNameRef,
                              SchemaSymbols::fgELT_ELEMENT, name);
        }

        if (!isValidRefDeclaration(elem)) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::BadAttWithRef);
        }

        return processElementDeclRef(elem, ref);
    }

    // Name is notEmpty
    if (!XMLString::isValidNCName(name)) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidDeclarationName,
                          SchemaSymbols::fgELT_ELEMENT, name);
        return 0;
    }

    const XMLCh*                  anotherSchemaURI = 0;
    int                           scopeDefined = Grammar::UNKNOWN_SCOPE;
    bool                          noErrorFound = true;
    bool                          anonymousType = false;
    DatatypeValidator*            validator = 0;
    DatatypeValidator*            subGroupValidator = 0;
    ComplexTypeInfo*              typeInfo = 0;
    ComplexTypeInfo*              subGroupTypeInfo = 0;
    ContentSpecNode*              contentSpecNode = 0;
    SchemaElementDecl::ModelTypes contentSpecType = SchemaElementDecl::Any;

    if (topLevel) {

        if (!refEmpty) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::GlobalElementWithRef, name);
        }

        if (fSchemaGrammar->getElemDecl(fTargetNSURI, name, 0, Grammar::TOP_LEVEL_SCOPE) != 0) {
            return new QName(name, fTargetNSURI);
        }
    }

    // Create element decl
    bool isDuplicate = false;
    SchemaElementDecl* elemDecl =
       createSchemaElementDecl(elem, topLevel, contentSpecType, isDuplicate, (fixed != 0));

    if (elemDecl == 0) {
        return 0;
    }

    if (!isDuplicate) {

        fSchemaGrammar->putElemDecl(elemDecl);

        if (fCurrentGroupInfo &&
            elemDecl->getEnclosingScope() == fCurrentGroupInfo->getScope()) {
            fCurrentGroupInfo->addElement(elemDecl);
        }

        if (fCurrentComplexType &&
            elemDecl->getEnclosingScope() == fCurrentComplexType->getScopeDefined()) {
            fCurrentComplexType->addElement(elemDecl);
        }
    }

    // Resolve the type for the element
    DOM_Element  content = checkContent(elem, XUtil::getFirstChildElement(elem), true);

    if (content != 0) {

        DOMString contentName = content.getLocalName();

        if (contentName.equals(SchemaSymbols::fgELT_COMPLEXTYPE)) {

            if (content.getAttribute(SchemaSymbols::fgATT_NAME).length() > 0) {
                // REVISIT - we are bypassing the complex type declaration.
                // Is this the right way to go?
                noErrorFound = false;
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::AnonComplexTypeWithName, name);
            }
            else {
                typeInfo = checkForComplexTypeInfo(content);
            }

            if (typeInfo == 0) {
                noErrorFound = false;
            }
            else if (!isDuplicate) {
                typeInfo->setElementId(elemDecl->getId());
            }

            anonymousType = true;
            content = XUtil::getNextSiblingElement(content);
        }
        else if (contentName.equals(SchemaSymbols::fgELT_SIMPLETYPE)) {

            if (content.getAttribute(SchemaSymbols::fgATT_NAME).length() > 0) {
                // REVISIT - we are bypassing the simple type declaration.
                // Is this the right way to go?
                noErrorFound = false;
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::AnonSimpleTypeWithName, name);
            }
            else {
                validator = checkForSimpleTypeValidator(content);
            }

            if (validator == 0) {
                noErrorFound = false;
            }

            contentSpecType = SchemaElementDecl::Simple;
            anonymousType = true;
            content = XUtil::getNextSiblingElement(content);
        }

        // Check for identity constraints
        if (content != 0) {

            content = checkIdentityConstraintContent(content);
            if (content != 0) {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidElementContent);
            }
        }
    }

    // Handle 'type' attribute
    const XMLCh* typeStr = getElementAttValue(elem, SchemaSymbols::fgATT_TYPE);
    if (XMLString::stringLen(typeStr) > 0) {

        if (anonymousType) {

            noErrorFound = false;
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ElementWithTypeAndAnonType, name);
        }
        else {

            if (!isAnyType(typeStr)) {

                checkEnumerationRequiredNotation(name, typeStr);

                anotherSchemaURI = checkTypeFromAnotherSchema(typeStr);

                // get complex type info
                typeInfo = getElementComplexTypeInfo(typeStr, noErrorFound, anotherSchemaURI);

                // get simple type validtor - if not a complex type
                if (typeInfo == 0) {
                    validator = getElementTypeValidator(typeStr, noErrorFound,
                                                    anotherSchemaURI,true);
				}
            }
        }
    }

    // Set element declararion type information - handle case when we have
    // circular substitution group (a subs b and b subs a)
    if (!isDuplicate) {
        elemDecl->setDatatypeValidator(validator);
        elemDecl->setComplexTypeInfo(typeInfo);
    }

    // Handle the substitutionGroup
    const XMLCh* subsGroupName =
            getElementAttValue(elem, SchemaSymbols::fgATT_SUBSTITUTIONGROUP);

    if (XMLString::stringLen(subsGroupName) != 0) {

        SchemaElementDecl* subsElemDecl =
                    getSubstituteGroupElemDecl(subsGroupName, noErrorFound);

        if (subsElemDecl != 0) {

            // An element cannot substitute itself
            if (subsElemDecl == elemDecl) {
                // REVISIT - add proper error message

            }
            else {

                // Check for substitution validity constraint
                // Substitution allowed (block and blockDefault) && same type
                if (isSubstitutionGroupValid(subsElemDecl,typeInfo,validator,name)) {

                    if (typeInfo == 0 && validator == 0 && noErrorFound) {

                        typeInfo = subsElemDecl->getComplexTypeInfo();
                        validator = subsElemDecl->getDatatypeValidator();
                    }

                    // set element substitutionGroup full name
                    const XMLCh* uri = resolvePrefixToURI(getPrefix(subsGroupName));
                    const XMLCh* localPart = getLocalPart(subsGroupName);

                    fBuffer.set(uri);
                    fBuffer.append(chComma);
                    fBuffer.append(localPart);

                    if (!isDuplicate) {

                        XMLCh* elemBaseName = elemDecl->getBaseName();
                        XMLCh* subsElemBaseName = subsElemDecl->getBaseName();
                        int    elemURI = elemDecl->getURI();
                        int    subsElemURI = subsElemDecl->getURI();

                        elemDecl->setSubstitutionGroupName(fBuffer.getRawBuffer());
                        fSubstitutionGroups->put((void*) elemBaseName, elemURI, subsElemDecl);
                        RefVectorOf<SchemaElementDecl>* subsElements =
                           fValidSubstitutionGroups->get(subsElemBaseName, subsElemURI);

                        if (!subsElements) {

                            subsElements = new RefVectorOf<SchemaElementDecl>(8, false);
                            fValidSubstitutionGroups->put(subsElemBaseName, subsElemURI, subsElements);
                        }

                        subsElements->addElement(elemDecl);
                        buildValidSubstitutionListB(elemDecl, subsElemDecl);
                        buildValidSubstitutionListF(elemDecl, subsElemDecl);
                    }
                }
                else {
                    noErrorFound = false;
                }
            }
        }
    }

    if (typeInfo == 0 && validator == 0) {

        if (noErrorFound) { // ur type
            contentSpecType = SchemaElementDecl::Any;
        }
        else {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::UntypedElement, name);
        }
    }

    // if element belongs to a compelx type
    if (typeInfo != 0) {

        contentSpecNode = typeInfo->getContentSpec();
        contentSpecType = (SchemaElementDecl::ModelTypes) typeInfo->getContentType();
        scopeDefined = typeInfo->getScopeDefined();
        validator = typeInfo->getDatatypeValidator();
    }

    // if element belongs to a simple type
    if (validator != 0) {

        contentSpecType = SchemaElementDecl::Simple;

        if (typeInfo == 0) {
            anotherSchemaURI = 0; // not to switch schema in this case
        }
    }

    // Now we can handle validation etc. of default and fixed attributes,
    // since we finally have all the type information.
    if(XMLString::stringLen(fixed) != 0) {
        deflt = fixed;
    }

    if(XMLString::stringLen(deflt) != 0) {

        try {
            if(validator == 0) { // in this case validate according to xs:string
                fDatatypeRegistry->getDatatypeValidator(
                    SchemaSymbols::fgDT_STRING)->validate(deflt);
            } else {
                validator->validate(deflt);
            }
        }
        catch(const InvalidDatatypeValueException& idve) {
            reportSchemaError(XMLUni::fgValidityDomain,
                              XMLValid::DisplayErrorMessage, idve.getMessage());
        }
        catch (const InvalidDatatypeFacetException& idfe) {
            reportSchemaError(XMLUni::fgValidityDomain,
                              XMLValid::DisplayErrorMessage, idfe.getMessage());
        }
        catch(...) {
            reportSchemaError(XMLUni::fgValidityDomain, XMLValid::DatatypeValidationFailure, deflt);
        }

        if(typeInfo != 0 &&
           typeInfo->getContentType() != SchemaElementDecl::Simple &&
           typeInfo->getContentType() != SchemaElementDecl::Mixed) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NotSimpleOrMixedElement, name);
        }

        if(typeInfo != 0 &&
           (typeInfo->getContentType() == SchemaElementDecl::Mixed
            && !emptiableParticle(typeInfo->getContentSpec()))) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::EmptiableMixedContent, name);
        }

        if (validator && (validator->getType() == DatatypeValidator::ID)) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ElemIDValueConstraint, name, deflt);
        }
    }

    // key/keyref/unique processing
    // TO DO



    // set element information, but first check for duplicate elements with
    // different types.
    if (isDuplicate) {

        DatatypeValidator* eltDV = elemDecl->getDatatypeValidator();
        ComplexTypeInfo*   eltTypeInfo = elemDecl->getComplexTypeInfo();

        if ( (eltTypeInfo != typeInfo) || (eltDV != validator))  {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateElementDeclaration, name);
        }
    }
    else {

        elemDecl->setDatatypeValidator(validator);
        elemDecl->setComplexTypeInfo(typeInfo);
        elemDecl->setDefaultValue(deflt);
        elemDecl->setDefinedScope(scopeDefined);
        elemDecl->setModelType(contentSpecType);
        elemDecl->setContentSpec(contentSpecNode);
        elemDecl->setTypeFromAnotherSchemaURI(anotherSchemaURI);
    }

    return new QName(elemDecl->getElementName());
}

XMLCh* TraverseSchema::traverseNotationDecl(const DOM_Element& childElem) {

    //REVISIT
    return 0;
}

int TraverseSchema::traverseByList(const DOM_Element& rootElem,
                                   const DOM_Element& contentElem,
                                   const int typeNameIndex,
                                   const int finalSet) {

    DatatypeValidator* baseValidator = 0;
    DOM_Element        content = contentElem;
    const XMLCh*       typeName = fStringPool->getValueForId(typeNameIndex);
    const XMLCh*       baseTypeName = getElementAttValue(content,
                                              SchemaSymbols::fgATT_ITEMTYPE);

    if (XUtil::getNextSiblingElement(content) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SimpleTypeContentError);
    }

    if (XMLString::stringLen(baseTypeName) == 0) { // must 'see' <simpleType>

        content = checkContent(rootElem, XUtil::getFirstChildElement(content), false);

        if (content == 0) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ExpectedSimpleTypeInList, typeName);
            return resetCurrentTypeNameStack(-1);
        }

        if (content.getLocalName().equals(SchemaSymbols::fgELT_SIMPLETYPE)) {
            baseValidator = checkForSimpleTypeValidator(content, SchemaSymbols::LIST);
        }
        else {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ListUnionRestrictionError, typeName);
            return resetCurrentTypeNameStack(-1);
        }

        content = XUtil::getNextSiblingElement(content);
    }
    else { // base was provided - get proper validator

        baseValidator = findDTValidator(rootElem, baseTypeName,
                                        SchemaSymbols::LIST);

        content = checkContent(rootElem, XUtil::getFirstChildElement(content), true);
    }

    if (baseValidator == 0) {
        return resetCurrentTypeNameStack(-1);
    }

    if (!baseValidator->isAtomic()) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::AtomicItemType, baseTypeName);
        return resetCurrentTypeNameStack(-1);
    }

    // 'content' should be empty
    // If an annotation was encountered we have already traversed it in
    // checkContent in the case of a base provided (only allowed child is
    // an annotation).
    if (content != 0) { // report an error and continue
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SimpleTypeDerivationByListError, typeName);
    }

    // create & register validator for "generated" type if it doesn't exist
    XMLCh* qualifiedName = getQualifiedName(typeNameIndex);

    try {

        DatatypeValidator* newValidator =
               fDatatypeRegistry->getDatatypeValidator(qualifiedName);

        if (newValidator == 0) {

            int strId = fStringPool->addOrFind(qualifiedName);
            fDatatypeRegistry->createDatatypeValidator(
                  fStringPool->getValueForId(strId), baseValidator, 0, 0, true, finalSet);
        }
    }
    catch(const InvalidDatatypeValueException& idve) {
        reportSchemaError(XMLUni::fgValidityDomain,
                          XMLValid::DisplayErrorMessage, idve.getMessage());
    }
    catch (const InvalidDatatypeFacetException& idfe) {
        reportSchemaError(XMLUni::fgValidityDomain,
                          XMLValid::DisplayErrorMessage, idfe.getMessage());
    }
    catch(...) {
        reportSchemaError(XMLUni::fgXMLErrDomain,
                          XMLErrs::DatatypeValidatorCreationError, typeName);
    }

    return resetCurrentTypeNameStack(fStringPool->addOrFind(qualifiedName));
}

int TraverseSchema::traverseByRestriction(const DOM_Element& rootElem,
                                          const DOM_Element& contentElem,
                                          const int typeNameIndex,
                                          const int finalSet) {

    DatatypeValidator* baseValidator = 0;
    DOM_Element        content = contentElem;
    const XMLCh*       typeName = fStringPool->getValueForId(typeNameIndex);
    const XMLCh*       baseTypeName = getElementAttValue(content,
                                              SchemaSymbols::fgATT_BASE);

    if (XUtil::getNextSiblingElement(content) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SimpleTypeContentError);
    }

    if (XMLString::stringLen(baseTypeName) == 0) { // must 'see' <simpleType>

        content = checkContent(rootElem, XUtil::getFirstChildElement(content), false);

        if (content == 0) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ExpectedSimpleTypeInRestriction);
            return resetCurrentTypeNameStack(-1);
        }

        if (content.getLocalName().equals(SchemaSymbols::fgELT_SIMPLETYPE)) {
            baseValidator = checkForSimpleTypeValidator(content);
        }
        else {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ListUnionRestrictionError, typeName);
            return resetCurrentTypeNameStack(-1);
        }

        // Check for facets
        content = XUtil::getNextSiblingElement(content);
    }
    else { // base was provided - get proper validator

        baseValidator = findDTValidator(rootElem, baseTypeName,
                                        SchemaSymbols::RESTRICTION);
        content = checkContent(rootElem, XUtil::getFirstChildElement(content), true);
    }

    if (baseValidator == 0) {
        return resetCurrentTypeNameStack(-1);
    }

    // Get facets if any existing
    RefHashTableOf<KVStringPair>*  facets = 0;
    RefVectorOf<XMLCh>*            enums = 0;
    XMLBuffer                      pattern;
    DOMString                      facetName;
    XMLCh                          fixedFlagStr[16];
    unsigned int                   fixedFlag = 0;
    unsigned int                   scope = GeneralAttributeCheck::LocalContext;
    bool                           isFirstPattern = true;

    while (content != 0) {

        if (content.getNodeType() == DOM_Node::ELEMENT_NODE) {

            fAttributeCheck->checkAttributes(content, scope, this);
            facetName = content.getLocalName();
            fBuffer.set(facetName.rawBuffer(), facetName.length());

            int facetId = fStringPool->addOrFind(fBuffer.getRawBuffer());
            const XMLCh* facetStr = fStringPool->getValueForId(facetId);
            DOMString    attValue = content.getAttribute(SchemaSymbols::fgATT_VALUE);
            int          attValueLen = attValue.length();

            if (facets == 0) {
                facets = new RefHashTableOf<KVStringPair>(29, true);
            }

            if (XMLString::compareString(facetStr,
                                     SchemaSymbols::fgELT_ENUMERATION) == 0) {

                // REVISIT
                // if validator is a notation datatype validator, we need
                // to get the qualified name first before adding it to the
                // enum buffer
                if (!enums) {
                    enums = new RefVectorOf<XMLCh>(8, true);
                }

                fBuffer.set(attValue.rawBuffer(), attValueLen);
                enums->addElement(XMLString::replicate(fBuffer.getRawBuffer()));
            }
            else if (XMLString::compareString(facetStr,
                                     SchemaSymbols::fgELT_PATTERN) == 0) {

                if (isFirstPattern) { // fBuffer.isEmpty() - overhead call

                    isFirstPattern = false;
                    pattern.set(attValue.rawBuffer(), attValueLen);
                }
                else { //datatypes: 5.2.4 pattern

                    pattern.append(chPipe);
                    pattern.append(attValue.rawBuffer(), attValueLen);
                }
            }
            else if (XMLString::compareString(facetStr,
                                     SchemaSymbols::fgELT_ANNOTATION) == 0
                     || XMLString::compareString(facetStr,
                                     SchemaSymbols::fgELT_SIMPLETYPE) == 0) {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ListUnionRestrictionError, typeName);
            }
            else {

                if (facets->containsKey(facetStr)) {
                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateFacet, facetStr);
                }
                else {

                    fBuffer.set(attValue.rawBuffer(), attValueLen);

                    const XMLCh* attVal = fBuffer.getRawBuffer();

                    if (!XMLString::compareString(facetStr, SchemaSymbols::fgELT_WHITESPACE)
                        && baseValidator->getType() != DatatypeValidator::String
                        && XMLString::compareString(attVal, SchemaSymbols::fgWS_COLLAPSE) != 0) {
                        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::WS_CollapseExpected,
                                          attVal);
                    }
                    else {
                        facets->put((void*) facetStr, new KVStringPair(facetStr, attVal));
                        checkFixedFacet(content, facetStr, baseValidator, fixedFlag);
                    }
                }
            }

            // REVISIT
            // check for annotation content - we are not checking whether the
            // return is empty or not. If not empty we should report an error
            checkContent(rootElem, XUtil::getFirstChildElement(content), true);
        }

        content = XUtil::getNextSiblingElement(content);
    } // end while

    if (!pattern.isEmpty()) {
        facets->put((void*) SchemaSymbols::fgELT_PATTERN,
                    new KVStringPair(SchemaSymbols::fgELT_PATTERN, pattern.getRawBuffer()));
    }

    if (fixedFlag) {

        XMLString::binToText(fixedFlag, fixedFlagStr, 15, 10);
        facets->put((void*) SchemaSymbols::fgATT_FIXED,
                    new KVStringPair(SchemaSymbols::fgATT_FIXED, fixedFlagStr));
    }

    XMLCh* qualifiedName = getQualifiedName(typeNameIndex);

    try {

        DatatypeValidator* newValidator =
               fDatatypeRegistry->getDatatypeValidator(qualifiedName);

        if (newValidator == 0) {

            int strId = fStringPool->addOrFind(qualifiedName);
            fDatatypeRegistry->createDatatypeValidator
                   (fStringPool->getValueForId(strId), baseValidator, facets,
                    enums, false, finalSet);
        }
    }
    catch(const InvalidDatatypeValueException& idve) {
        reportSchemaError(XMLUni::fgValidityDomain,
                          XMLValid::DisplayErrorMessage, idve.getMessage());
    }
    catch (const InvalidDatatypeFacetException& idfe) {
        reportSchemaError(XMLUni::fgValidityDomain,
                          XMLValid::DisplayErrorMessage, idfe.getMessage());
    }
    catch(...) {

        reportSchemaError(XMLUni::fgXMLErrDomain,
                          XMLErrs::DatatypeValidatorCreationError, typeName);
    }

    return resetCurrentTypeNameStack(fStringPool->addOrFind(qualifiedName));
}


int TraverseSchema::traverseByUnion(const DOM_Element& rootElem,
                                    const DOM_Element& contentElem,
                                    const int typeNameIndex,
                                    const int finalSet,
                                    int baseRefContext) {

    int                             size = 1;
    DOM_Element                     content = contentElem;
    const XMLCh* const              typeName =
                                      fStringPool->getValueForId(typeNameIndex);
    const XMLCh*                    baseTypeName = getElementAttValue(content,
                                              SchemaSymbols::fgATT_MEMBERTYPES);
    DatatypeValidator*              baseValidator = 0;
    RefVectorOf<DatatypeValidator>* validators =
                                      new RefVectorOf<DatatypeValidator>(4, false);
    Janitor<DVRefVector>            janValidators(validators);

    if (XUtil::getNextSiblingElement(content) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SimpleTypeContentError);
    }

    if (XMLString::stringLen(baseTypeName)) { //base was provided - get proper validator.

        XMLStringTokenizer unionMembers(baseTypeName);
        int             tokCount = unionMembers.countTokens();

        for (int i = 0; i < tokCount; i++) {

            const XMLCh* typeName = unionMembers.nextToken();

            baseValidator = findDTValidator(rootElem, typeName,
                                            SchemaSymbols::UNION);

            if (baseValidator == 0) {
                return resetCurrentTypeNameStack(-1);
            }

            if (baseValidator->getType() == DatatypeValidator::Union) {

                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::MemberTypeNoUnion, baseTypeName);
                return resetCurrentTypeNameStack(-1);
            }

            validators->addElement(baseValidator);
        }

        content = checkContent(rootElem, XUtil::getFirstChildElement(content), true);
    }
    else { // must 'see' <simpleType>

        content = checkContent(rootElem, XUtil::getFirstChildElement(content), false);

        if (content == 0) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ExpectedSimpleTypeInUnion, typeName);
            return resetCurrentTypeNameStack(-1);
        }

        if (!content.getLocalName().equals(SchemaSymbols::fgELT_SIMPLETYPE)) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ListUnionRestrictionError, typeName);
            return resetCurrentTypeNameStack(-1);
        }
    }

    // process union content of simpleType children if any
    while (content != 0) {

        if (content.getLocalName().equals(SchemaSymbols::fgELT_SIMPLETYPE)) {

            baseValidator = checkForSimpleTypeValidator(content, baseRefContext | SchemaSymbols::UNION);

            if (baseValidator == 0) {
                return resetCurrentTypeNameStack(-1);
            }

            if (baseValidator->getType() == DatatypeValidator::Union) {

                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::MemberTypeNoUnion, baseTypeName);
                return resetCurrentTypeNameStack(-1);
            }

            validators->addElement(baseValidator);
        }
        else {
            // REVISIT - should we break. For now, we will continue and move to
            // the next sibling
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ListUnionRestrictionError, typeName);
        }

        content   = XUtil::getNextSiblingElement(content);
    } // end while

    XMLCh* qualifiedName = getQualifiedName(typeNameIndex);

    try {

        DatatypeValidator* newValidator =
               fDatatypeRegistry->getDatatypeValidator(qualifiedName);

        if (newValidator == 0) {

            int strId = fStringPool->addOrFind(qualifiedName);
            if (fDatatypeRegistry->createDatatypeValidator(
                           fStringPool->getValueForId(strId), validators, finalSet) != 0) {
                janValidators.orphan();
            }
        }
    }
    catch(const InvalidDatatypeValueException& idve) {
        janValidators.orphan();
        reportSchemaError(XMLUni::fgValidityDomain,
                          XMLValid::DisplayErrorMessage, idve.getMessage());
    }
    catch (const InvalidDatatypeFacetException& idfe) {
        janValidators.orphan();
        reportSchemaError(XMLUni::fgValidityDomain,
                          XMLValid::DisplayErrorMessage, idfe.getMessage());
    }
    catch(...) {
        janValidators.orphan();
        reportSchemaError(XMLUni::fgXMLErrDomain,
                          XMLErrs::DatatypeValidatorCreationError, typeName);
    }

    return resetCurrentTypeNameStack(fStringPool->addOrFind(qualifiedName));
}


/**
  * Traverse SimpleContent Declaration
  *
  *   <simpleContent
  *     id = ID
  *     {any attributes with non-schema namespace...}>
  *
  *     Content: (annotation? , (restriction | extension))
  *   </simpleContent>
  *
  *   <restriction
  *     base = QNAME
  *     id = ID
  *     {any attributes with non-schema namespace...}>
  *
  *     Content: (annotation?, (simpleType?, (minExclusive | minInclusive
  *               | maxExclusive | maxInclusive | totalDigits | fractionDigits
  *               | length | minLength | maxLength | enumeration | pattern
  *               | whiteSpace)*)?, ((attribute | attributeGroup)* , anyAttribute?))
  *   </restriction>
  *
  *   <extension
  *     base = QNAME
  *     id = ID
  *     {any attributes with non-schema namespace...}>
  *     Content: (annotation? , ((attribute | attributeGroup)* , anyAttribute?))
  *   </extension>
  *
  */
void TraverseSchema::traverseSimpleContentDecl(const XMLCh* const typeName,
                                               const DOM_Element& contentDecl,
                                               ComplexTypeInfo* const typeInfo)
{
    // -----------------------------------------------------------------------
    // Check Attributes
    // -----------------------------------------------------------------------
    unsigned short scope = GeneralAttributeCheck::LocalContext;
    fAttributeCheck->checkAttributes(contentDecl, scope, this);

    // -----------------------------------------------------------------------
    // Set the content type to be simple, and initialize content spec handle
    // -----------------------------------------------------------------------
    typeInfo->setContentType(SchemaElementDecl::Simple);

    DOM_Element simpleContent =
        checkContent(contentDecl, XUtil::getFirstChildElement(contentDecl),false);

    // If there are no children, return
    if (simpleContent == 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::EmptySimpleTypeContent);
        throw TraverseSchema::InvalidComplexTypeInfo;
    }

    fAttributeCheck->checkAttributes(simpleContent, scope, this);

    // -----------------------------------------------------------------------
    // The content should be either "restriction" or "extension"
    // -----------------------------------------------------------------------
    DOMString contentName = simpleContent.getLocalName();

    if (contentName.equals(SchemaSymbols::fgATTVAL_RESTRICTION)) {
        typeInfo->setDerivedBy(SchemaSymbols::RESTRICTION);
    }
    else if (contentName.equals(SchemaSymbols::fgATTVAL_EXTENSION)) {
        typeInfo->setDerivedBy(SchemaSymbols::EXTENSION);
    }
    else {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidSimpleContent);
        throw TraverseSchema::InvalidComplexTypeInfo;
    }

    // -----------------------------------------------------------------------
    // Handle the base type name
    // -----------------------------------------------------------------------
    const XMLCh* baseName =
            getElementAttValue(simpleContent, SchemaSymbols::fgATT_BASE);

    if (XMLString::stringLen(baseName) == 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::UnspecifiedBase);
        throw TraverseSchema::InvalidComplexTypeInfo;
    }

    const XMLCh* prefix = getPrefix(baseName);
    const XMLCh* localPart = getLocalPart(baseName);
    const XMLCh* uri = resolvePrefixToURI(prefix);
    DatatypeValidator* baseValidator = getDatatypeValidator(uri, localPart);

    if (baseValidator != 0) {

        // check that the simpleType does not preclude derivation by extension
        if ((baseValidator->getFinalSet() & SchemaSymbols::EXTENSION) == typeInfo->getDerivedBy()) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DisallowedSimpleTypeExtension,
                              baseName, typeName);
            throw TraverseSchema::InvalidComplexTypeInfo;
        }

        typeInfo->setBaseComplexTypeInfo(0);
        typeInfo->setBaseDatatypeValidator(baseValidator);
    }
    else {

        // check for 'anyType'
        if (!XMLString::compareString(uri, SchemaSymbols::fgURI_SCHEMAFORSCHEMA)
            && !XMLString::compareString(localPart, SchemaSymbols::fgATTVAL_ANYTYPE)) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidSimpleContentBase, baseName);
            throw TraverseSchema::InvalidComplexTypeInfo;
        }

        processBaseTypeInfo(baseName, localPart, uri, typeInfo);
    }

    // check that the base isn't a complex type with complex content
    // and that derivation method is not included in 'final'
    ComplexTypeInfo* baseTypeInfo = typeInfo->getBaseComplexTypeInfo();

    if (baseTypeInfo) {

        if (baseTypeInfo->getContentType() != SchemaElementDecl::Simple) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidSimpleContentBase, baseName);
            throw TraverseSchema::InvalidComplexTypeInfo;
        }

        if ((baseTypeInfo->getFinalSet() & typeInfo->getDerivedBy()) != 0) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ForbiddenDerivation, baseName);
            throw TraverseSchema::InvalidComplexTypeInfo;
        }
    }

    // -----------------------------------------------------------------------
    // Process the content of the derivation
    // -----------------------------------------------------------------------
    //Skip over any annotations in the restriction or extension elements
    DOM_Element content = checkContent(simpleContent,
                            XUtil::getFirstChildElement(simpleContent), true);

    if (typeInfo->getDerivedBy() == SchemaSymbols::RESTRICTION) {

        //Schema Spec: 5.11: Complex Type Definition Properties Correct: 2
        if (typeInfo->getBaseDatatypeValidator() != 0) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidComplexTypeBase, baseName);
            throw TraverseSchema::InvalidComplexTypeInfo;
        }
        else {
           typeInfo->setBaseDatatypeValidator(baseTypeInfo->getDatatypeValidator());
        }

        if (content != 0) {

            // ---------------------------------------------------------------
            // There may be a simple type definition in the restriction
            // element. The data type validator will be based on it, if
            // specified
            // ---------------------------------------------------------------
            if (content.getLocalName().equals(SchemaSymbols::fgELT_SIMPLETYPE)) {

                int simpleTypeNameIndex = traverseSimpleTypeDecl(content);

                if (simpleTypeNameIndex !=-1) {

                    DatatypeValidator* simpleTypeDV =
                        fDatatypeRegistry->getDatatypeValidator(fStringPool->getValueForId(simpleTypeNameIndex));

                    // Check that the simpleType validator is validly derived
                    // from base
                    DatatypeValidator* baseDV = typeInfo->getBaseDatatypeValidator();

                    if (baseDV  && !baseDV->isSubstitutableBy(simpleTypeDV)) {

                        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidContentRestriction);
                        throw TraverseSchema::InvalidComplexTypeInfo;
                    }

                    typeInfo->setBaseDatatypeValidator(simpleTypeDV);
                    content = XUtil::getNextSiblingElement(content);
                }
                else {
                    throw TraverseSchema::InvalidComplexTypeInfo;
                }
            }

            // ---------------------------------------------------------------
            // Build up the facet info
            // ---------------------------------------------------------------
            RefHashTableOf<KVStringPair>*  facets = 0;
            RefVectorOf<XMLCh>*            enums = 0;
            XMLBuffer                      pattern;
            const XMLCh*                   facetName;
            XMLCh                          fixedFlagStr[16];
            unsigned int                   fixedFlag = 0;
            int                            facetId;
            bool                           isFirstPattern = true;

            while (content != 0) {

                fBuffer.set(content.getLocalName().rawBuffer(), content.getLocalName().length());
                facetId = fStringPool->addOrFind(fBuffer.getRawBuffer());
                facetName = fStringPool->getValueForId(facetId);

                // if not a valid facet, break from the loop
                if (!isValidFacet(SchemaSymbols::fgELT_SIMPLECONTENT, facetName)) {
                    break;
                }

                if (content.getNodeType() == DOM_Node::ELEMENT_NODE) {

                    fAttributeCheck->checkAttributes(content, scope, this);

                    DOMString attValue =
                        content.getAttribute(SchemaSymbols::fgATT_VALUE);

                    if (facets == 0) {
                        facets = new RefHashTableOf<KVStringPair>(29, true);
                    }

                    fBuffer.set(attValue.rawBuffer(), attValue.length());

                    if (XMLString::compareString(facetName,
                                     SchemaSymbols::fgELT_ENUMERATION) == 0) {

                        if (!enums) {
                            enums = new RefVectorOf<XMLCh>(8, true);
                        }

                        enums->addElement(XMLString::replicate(fBuffer.getRawBuffer()));
                    }
                    else if (XMLString::compareString(facetName,
                                     SchemaSymbols::fgELT_PATTERN) == 0) {

                        if (isFirstPattern) { // fBuffer.isEmpty() - overhead call

                            isFirstPattern = false;
                            pattern.set(fBuffer.getRawBuffer());
                        }
                        else { //datatypes: 5.2.4 pattern

                            pattern.append(chPipe);
                            pattern.append(fBuffer.getRawBuffer());
                        }
                    }
                    else {

                        if (facets->containsKey(facetName)) {
                            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateFacet, facetName);
                        }
                        else {
                            facets->put((void*) facetName,
                                new KVStringPair(facetName,fBuffer.getRawBuffer()));
                            checkFixedFacet(content, facetName, typeInfo->getBaseDatatypeValidator(), fixedFlag);
                        }
                    }
                }

                content = XUtil::getNextSiblingElement(content);
            }

            if (facets) {

                if (!pattern.isEmpty()) {
                    facets->put
                    (
                        (void*) SchemaSymbols::fgELT_PATTERN,
                        new KVStringPair
                            (
                                SchemaSymbols::fgELT_PATTERN,
                                pattern.getRawBuffer()
                            )
                    );
                }

                if (fixedFlag) {

                    XMLString::binToText(fixedFlag, fixedFlagStr, 15, 10);
                    facets->put((void*) SchemaSymbols::fgATT_FIXED,
                        new KVStringPair(SchemaSymbols::fgATT_FIXED, fixedFlagStr));
                }

                XMLCh* qualifiedName =
                    getQualifiedName(fStringPool->addOrFind(typeName));

                try {

                    int nameId = fStringPool->addOrFind(qualifiedName);

                    typeInfo->setDatatypeValidator
                    (
                        fDatatypeRegistry->createDatatypeValidator
                        (
                            fStringPool->getValueForId(nameId),
                            typeInfo->getBaseDatatypeValidator(),
                            facets, enums, false, 0
                        )
                    );
                }
                catch(const InvalidDatatypeValueException& idve) {
                    reportSchemaError(XMLUni::fgValidityDomain,
                          XMLValid::DisplayErrorMessage, idve.getMessage());
                }
                catch (const InvalidDatatypeFacetException& idfe) {
                    reportSchemaError(XMLUni::fgValidityDomain,
                          XMLValid::DisplayErrorMessage, idfe.getMessage());
                }
                catch(...) {
                    reportSchemaError(XMLUni::fgXMLErrDomain,
                                      XMLErrs::DatatypeValidatorCreationError, typeName);
                }
            }
            else {
                typeInfo->setDatatypeValidator(
                                        typeInfo->getBaseDatatypeValidator());
            }
        }
        else {
            typeInfo->setDatatypeValidator(typeInfo->getBaseDatatypeValidator());
        }
    } // end RESTRICTION
    else { // EXTENSION

        ComplexTypeInfo* baseTypeInfo = typeInfo->getBaseComplexTypeInfo();

        if (baseTypeInfo!= 0) {

            typeInfo->setBaseDatatypeValidator(baseTypeInfo->getDatatypeValidator());
        }

        typeInfo->setDatatypeValidator(typeInfo->getBaseDatatypeValidator());
    }

    // -----------------------------------------------------------------------
    // Process attributes if any
    // -----------------------------------------------------------------------
    if (content != 0 && isAttrOrAttrGroup(content)) {
        processAttributes(content, baseName, localPart, uri, typeInfo);
    }

    if (XUtil::getNextSiblingElement(simpleContent) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidChildInSimpleContent);
    }

} // End of function traverseSimpleContentDecl

/**
  * Traverse complexContent Declaration
  *
  *   <complexContent
  *     id = ID
  *     mixed = boolean
  *     {any attributes with non-schema namespace...}>
  *
  *     Content: (annotation? , (restriction | extension))
  *   </complexContent>
  *
  *   <restriction
  *     base = QNAME
  *     id = ID
  *     {any attributes with non-schema namespace...}>
  *
  *     Content: (annotation? , (group | all | choice | sequence)?,
  *              ((attribute | attributeGroup)* , anyAttribute?))
  *   </restriction>
  *
  *   <extension
  *     base = QNAME
  *     id = ID
  *     {any attributes with non-schema namespace...}>
  *         Content: (annotation? , (group | all | choice | sequence)?,
  *                  ((attribute | attributeGroup)* , anyAttribute?))
  *   </extension>
  */
void TraverseSchema::traverseComplexContentDecl(const XMLCh* const typeName,
                                                const DOM_Element& contentDecl,
                                                ComplexTypeInfo* const typeInfo,
                                                const bool isMixed)
{
    // ------------------------------------------------------------------
    // Check attributes
    // ------------------------------------------------------------------
    unsigned short scope = GeneralAttributeCheck::LocalContext;
    fAttributeCheck->checkAttributes(contentDecl, scope, this);

    // -----------------------------------------------------------------------
    // Determine whether the content is mixed, or element-only
    // Setting here overrides any setting on the complex type decl
    // -----------------------------------------------------------------------
    const XMLCh* const mixed =
                getElementAttValue(contentDecl, SchemaSymbols::fgATT_MIXED);

    bool mixedContent = isMixed;

	if (mixed) {
        if (!XMLString::compareString(mixed, SchemaSymbols::fgATTVAL_TRUE)
            || !XMLString::compareString(mixed, fgValueOne)) {
            mixedContent = true;
        }
        else if (!XMLString::compareString(mixed, SchemaSymbols::fgATTVAL_FALSE)
                 || !XMLString::compareString(mixed, fgValueZero)) {
            mixedContent = false;
        }
    }

    // -----------------------------------------------------------------------
    // Since the type must have complex content, set the simple type validators
    // to null
    // -----------------------------------------------------------------------
    typeInfo->setDatatypeValidator(0);
    typeInfo->setBaseDatatypeValidator(0);

    DOM_Element complexContent =
        checkContent(contentDecl,XUtil::getFirstChildElement(contentDecl),false);

    // If there are no children, return
    if (complexContent == 0) {
       throw TraverseSchema::InvalidComplexTypeInfo;
    }

    // -----------------------------------------------------------------------
    // The content should be either "restriction" or "extension"
    // -----------------------------------------------------------------------
    DOMString complexContentName = complexContent.getLocalName();

    if (complexContentName.equals(SchemaSymbols::fgELT_RESTRICTION)) {
        typeInfo->setDerivedBy(SchemaSymbols::RESTRICTION);
    }
    else if (complexContentName.equals(SchemaSymbols::fgELT_EXTENSION)) {
        typeInfo->setDerivedBy(SchemaSymbols::EXTENSION);
    }
    else {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidComplexContent);
        throw TraverseSchema::InvalidComplexTypeInfo;
    }

    // -----------------------------------------------------------------------
    // Handle the base type name
    // -----------------------------------------------------------------------
    const XMLCh* baseName =
            getElementAttValue(complexContent, SchemaSymbols::fgATT_BASE);

    if (XMLString::stringLen(baseName) == 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::UnspecifiedBase);
        throw TraverseSchema::InvalidComplexTypeInfo;
    }

    const XMLCh* prefix = getPrefix(baseName);
    const XMLCh* localPart = getLocalPart(baseName);
    const XMLCh* uri = resolvePrefixToURI(prefix);

    // -------------------------------------------------------------
    // check if the base is "anyType"
    // -------------------------------------------------------------
    if (!(XMLString::compareString(uri, SchemaSymbols::fgURI_SCHEMAFORSCHEMA) == 0
          && XMLString::compareString(localPart, SchemaSymbols::fgATTVAL_ANYTYPE) == 0)) {

        processBaseTypeInfo(baseName, localPart, uri, typeInfo);

        //Check that the base is a complex type
        if (typeInfo->getBaseComplexTypeInfo() == 0)  {

            reportSchemaError(XMLUni::fgXMLErrDomain,
                              XMLErrs::BaseNotComplexType);
            throw TraverseSchema::InvalidComplexTypeInfo;
        }
    }

    // -----------------------------------------------------------------------
    // Process the content of the derivation
    // -----------------------------------------------------------------------
    //Skip over any annotations in the restriction or extension elements
    DOM_Element content = checkContent(complexContent,
                            XUtil::getFirstChildElement(complexContent), true);

    processComplexContent(typeName, content, typeInfo, baseName, localPart,
                          uri, mixedContent);

    if (XUtil::getNextSiblingElement(complexContent) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidChildInComplexContent);
    }
}


/**
  * <anyAttribute
  *   id = ID
  *   namespace = ((##any | ##other) | list of (anyURI | (##targetNamespace | ##local)))>
  *   processContents = (lax | skip | strict) : strict
  *   Content: (annotation?)
  * </anyAttribute>
  */
SchemaAttDef* TraverseSchema::traverseAnyAttribute(const DOM_Element& elem) {

    // -----------------------------------------------------------------------
    // Check Attributes
    // -----------------------------------------------------------------------
    unsigned short scope = GeneralAttributeCheck::LocalContext;
    fAttributeCheck->checkAttributes(elem, scope, this);

    // ------------------------------------------------------------------
    // First, handle any ANNOTATION declaration
    // ------------------------------------------------------------------
    if (checkContent(elem, XUtil::getFirstChildElement(elem), true) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain,
                          XMLErrs::AnyAttributeContentError);
    }

    // ------------------------------------------------------------------
    // Get attributes
    // ------------------------------------------------------------------
    const XMLCh* const processContents =
            getElementAttValue(elem, SchemaSymbols::fgATT_PROCESSCONTENTS);
    const XMLCh* const nameSpace =
            getElementAttValue(elem, SchemaSymbols::fgATT_NAMESPACE);

    // ------------------------------------------------------------------
    // Set default att type based on 'processContents' value
    // ------------------------------------------------------------------
    XMLAttDef::DefAttTypes attDefType = XMLAttDef::ProcessContents_Strict;

    if (XMLString::stringLen(processContents) == 0
        || XMLString::compareString(processContents, SchemaSymbols::fgATTVAL_STRICT) == 0) {
        // Do nothing - defaulted already
    }
    else if (XMLString::compareString(processContents,
                                      SchemaSymbols::fgATTVAL_SKIP) == 0) {
        attDefType = XMLAttDef::ProcessContents_Skip;
    }
    else if (XMLString::compareString(processContents,
                                      SchemaSymbols::fgATTVAL_LAX) == 0) {
        attDefType = XMLAttDef::ProcessContents_Lax;
    }

    // ------------------------------------------------------------------
    // Process 'namespace' attribute
    // ------------------------------------------------------------------
    int uriIndex = fEmptyNamespaceURI;
    XMLAttDef::AttTypes attType = XMLAttDef::Any_Any;
    ValueVectorOf<unsigned int> namespaceList(8);

    if (XMLString::stringLen(nameSpace) == 0
        || XMLString::compareString(nameSpace, SchemaSymbols::fgATTVAL_TWOPOUNDANY) == 0) {
        // Do nothing - defaulted already
    }
    else if (XMLString::compareString(nameSpace, SchemaSymbols::fgATTVAL_TWOPOUNDOTHER) == 0) {

        attType = XMLAttDef::Any_Other;
        uriIndex = fTargetNSURI;
    }
    else {

        ValueVectorOf<unsigned int>* namespaceList = new ValueVectorOf<unsigned int>(8);
        XMLStringTokenizer tokenizer(nameSpace);

        attType = XMLAttDef::Any_List;

        while (tokenizer.hasMoreTokens()) {

            const XMLCh* token = tokenizer.nextToken();

            if (!XMLString::compareString(token, SchemaSymbols::fgATTVAL_TWOPOUNDLOCAL)) {
                uriIndex = fEmptyNamespaceURI;
            }
            else if (!XMLString::compareString(token, SchemaSymbols::fgATTVAL_TWOPOUNDTRAGETNAMESPACE)) {
                uriIndex = fTargetNSURI;
            }
            else {
                uriIndex = fURIStringPool->addOrFind(token);
            }

            if (!namespaceList->containsElement(uriIndex)) {
                namespaceList->addElement(uriIndex);
            }
        }

        uriIndex = fEmptyNamespaceURI;
    }

    // ------------------------------------------------------------------
    // Create wildcard attribute
    // ------------------------------------------------------------------
    SchemaAttDef* attDef = new SchemaAttDef(XMLUni::fgZeroLenString,
                                            XMLUni::fgZeroLenString,
                                            uriIndex, attType, attDefType);


    if (namespaceList.size()) {
       attDef->setNamespaceList(&namespaceList);
    }

    return attDef;
}

// ---------------------------------------------------------------------------
//  TraverseSchema: Helper methods
// ---------------------------------------------------------------------------
void TraverseSchema::retrieveNamespaceMapping() {

    DOM_NamedNodeMap schemaEltAttrs = fSchemaRootElement.getAttributes();
    bool seenXMLNS = false;
    int attrCount = schemaEltAttrs.getLength();

    for (int i = 0; i < attrCount; i++) {

        DOM_Node  attribute = schemaEltAttrs.item(i);

        if (attribute.isNull()) {
            break;
        }

        DOMString attName = attribute.getNodeName();

        fBuffer.set(attName.rawBuffer(), attName.length());
        int nameId = fStringPool->addOrFind(fBuffer.getRawBuffer());
        const XMLCh* name = fStringPool->getValueForId(nameId);

        // starts with 'xmlns:'
        if (XMLString::startsWith(name, fgXMLNS_Str)) {

            XMLCh prefix[256];
            int offsetIndex = XMLString::indexOf(name, chColon);
            DOMString attValue = attribute.getNodeValue();

            XMLString::subString(prefix, name, offsetIndex + 1, XMLString::stringLen(name));
            fBuffer.set(attValue.rawBuffer(), attValue.length());
            fNamespaceScope->addPrefix(prefix,
                        fURIStringPool->addOrFind(fBuffer.getRawBuffer()));
        }
        else if (attName.equals(XMLUni::fgXMLNSString)) { // == 'xmlns'

            DOMString attValue = attribute.getNodeValue();

            fBuffer.set(attValue.rawBuffer(), attValue.length());
            fNamespaceScope->addPrefix( XMLUni::fgZeroLenString,
                        fURIStringPool->addOrFind(fBuffer.getRawBuffer()));

            seenXMLNS = true;
        }
   } // end for

    if (!seenXMLNS && XMLString::stringLen(fTargetNSURIString) == 0 ) {
        fNamespaceScope->addPrefix( XMLUni::fgZeroLenString,
                        fEmptyNamespaceURI);
    }
}

void TraverseSchema::processChildren(const DOM_Element& root) {

    // process <redefine>, <include> and <import> info items.
    DOM_Element child = XUtil::getFirstChildElement(root);

    for (; child != 0; child = XUtil::getNextSiblingElement(child)) {

        DOMString name = child.getLocalName();

        if (name.equals(SchemaSymbols::fgELT_ANNOTATION)) {
            traverseAnnotationDecl(child);
        }
        else if (name.equals(SchemaSymbols::fgELT_INCLUDE)) {
            traverseInclude(child);
        }
        else if (name.equals(SchemaSymbols::fgELT_IMPORT)) {
            traverseImport(child);
        }
        else if (name.equals(SchemaSymbols::fgELT_REDEFINE)) {

        }
        else
            break;
    }

    // child refers to the first info item which is not <annotation> or
    // one of the schema inclusion/importation declarations.
    for (; child != 0; child = XUtil::getNextSiblingElement(child)) {

        DOMString name = child.getLocalName();
        const XMLCh* typeName = getElementAttValue(child, SchemaSymbols::fgATT_NAME);

        if (name.equals(SchemaSymbols::fgELT_ANNOTATION)) {
            traverseAnnotationDecl(child);
        }
        else if (name.equals(SchemaSymbols::fgELT_SIMPLETYPE)) {

            if (XMLString::stringLen(typeName)) {
                if (fGlobalTypes->containsKey(typeName, fTargetNSURI)) {
                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateGlobalType,
                                      SchemaSymbols::fgELT_SIMPLETYPE, typeName, SchemaSymbols::fgELT_COMPLEXTYPE);
                    continue;
                }
                else {
                    fGlobalTypes->put((void*) typeName, fTargetNSURI, 0);
                }
            }

            traverseSimpleTypeDecl(child);
        }
        else if (name.equals(SchemaSymbols::fgELT_COMPLEXTYPE)) {

            if (XMLString::stringLen(typeName)) {
                if (fGlobalTypes->containsKey(typeName, fTargetNSURI)) {

                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateGlobalType,
                                      SchemaSymbols::fgELT_COMPLEXTYPE, typeName, SchemaSymbols::fgELT_SIMPLETYPE);
                    continue;
                }
                else {
                    fGlobalTypes->put((void*) typeName, fTargetNSURI, 0);
                }
            }

            traverseComplexTypeDecl(child);
        }
        else if (name.equals(SchemaSymbols::fgELT_ELEMENT)) {

            QName* elmQName = traverseElementDecl(child);
            Janitor<QName> janQName(elmQName);
        }
        else if (name.equals(SchemaSymbols::fgELT_ATTRIBUTEGROUP)) {

            if (XMLString::stringLen(typeName)) {
                if (fGlobalAttGroups->containsKey(typeName, fTargetNSURI)) {
                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateGlobalDeclaration,
                                      SchemaSymbols::fgELT_ATTRIBUTEGROUP, typeName);
                    continue;
                }
                else {
                    fGlobalAttGroups->put((void*) typeName, fTargetNSURI, 0);
                }
            }

            traverseAttributeGroupDecl(child, 0);
        }
        else if (name.equals(SchemaSymbols::fgELT_ATTRIBUTE ) ) {
            traverseAttributeDecl( child, 0);
        }
        else if (name.equals(SchemaSymbols::fgELT_GROUP)) {

            if (XMLString::stringLen(typeName)) {
                if (fGlobalGroups->containsKey(typeName, fTargetNSURI)) {
                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateGlobalDeclaration,
                                      SchemaSymbols::fgELT_GROUP, typeName);
                    continue;
                }
                else {
                    fGlobalGroups->put((void*) typeName, fTargetNSURI, 0);
                }
            }

            bool toAdoptSpecNode = true;
            traverseGroupDecl(child, toAdoptSpecNode);
        }
        else if (name.equals(SchemaSymbols::fgELT_NOTATION)) {
            traverseNotationDecl(child); //TO DO
        } else {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SchemaElementContentError);
        }
    } // for each child node
}


DOM_Element TraverseSchema::checkContent(const DOM_Element& rootElem,
                                         const DOM_Element& contentElem,
                                         const bool isEmpty) {

    DOM_Element content = contentElem;
    const XMLCh* name = getElementAttValue(rootElem,SchemaSymbols::fgATT_NAME);

    if (content == 0) {
       if (!isEmpty) {
           reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ContentError, name);
       }

       return 0;
    }

    if (content.getLocalName().equals(SchemaSymbols::fgELT_ANNOTATION)) {

        traverseAnnotationDecl(contentElem);
        content = XUtil::getNextSiblingElement(content);

        if (content == 0) { // must be followed by content

            if (!isEmpty) {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ContentError, name);
            }

            return 0;
        }

        if (content.getLocalName().equals(SchemaSymbols::fgELT_ANNOTATION)) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::AnnotationError, name);
            return 0;
        }
    }

    return content;
}


DatatypeValidator*
TraverseSchema::getDatatypeValidator(const XMLCh* const uriStr,
                                     const XMLCh* const localPartStr) {

    DatatypeValidator* dv = 0;

    if (XMLString::compareString(uriStr, SchemaSymbols::fgURI_SCHEMAFORSCHEMA) == 0) {
        dv = fDatatypeRegistry->getDatatypeValidator(localPartStr);
    }
    else {

        fBuffer.set(uriStr);
        fBuffer.append(chComma);
        fBuffer.append(localPartStr);
        dv = fDatatypeRegistry->getDatatypeValidator(fBuffer.getRawBuffer());
    }

    return dv;
}


XMLCh* TraverseSchema::getQualifiedName(const int typeNameIndex) {

    const XMLCh* typeName = fStringPool->getValueForId(typeNameIndex);

    fBuffer.set(fTargetNSURIString);
    fBuffer.append(chComma);
    fBuffer.append(typeName);

    return fBuffer.getRawBuffer();
}


DatatypeValidator*
TraverseSchema::checkForSimpleTypeValidator(const DOM_Element& content,
                                            int baseRefContext) {

    int typeNameIndex = traverseSimpleTypeDecl(content, baseRefContext);
    DatatypeValidator* baseValidator = 0;

    if (typeNameIndex != -1) {

        baseValidator = fDatatypeRegistry->getDatatypeValidator(
                                  fStringPool->getValueForId(typeNameIndex));
    }

    if (typeNameIndex == -1 || baseValidator == 0) {

        const XMLCh* name = getElementAttValue(content,SchemaSymbols::fgATT_NAME);
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::UnknownSimpleType, name);
    }

    return baseValidator;
}

ComplexTypeInfo*
TraverseSchema::checkForComplexTypeInfo(const DOM_Element& content) {

    int typeNameIndex = traverseComplexTypeDecl(content);
    ComplexTypeInfo* baseTypeInfo = 0;

    if (typeNameIndex != -1) {
        baseTypeInfo = fComplexTypeRegistry->get(
                                  fStringPool->getValueForId(typeNameIndex));
    }

    if (typeNameIndex == -1 || baseTypeInfo == 0) {

        const XMLCh* name = getElementAttValue(content,SchemaSymbols::fgATT_NAME);
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::UnknownComplexType, name);
    }

    return baseTypeInfo;
}

DatatypeValidator*
TraverseSchema::findDTValidator(const DOM_Element& rootElem,
                                const XMLCh* const baseTypeStr,
                                const int baseRefContext) {

    const XMLCh*       prefix = getPrefix(baseTypeStr);
    const XMLCh*       localPart = getLocalPart(baseTypeStr);
    const XMLCh*       uri = resolvePrefixToURI(prefix);
    DatatypeValidator* baseValidator = getDatatypeValidator(uri, localPart);

    if (baseValidator == 0) {

        DOM_Element baseTypeNode =
            getTopLevelComponentByName(SchemaSymbols::fgELT_SIMPLETYPE, localPart);

        if (baseTypeNode != 0) {

            traverseSimpleTypeDecl(baseTypeNode);
            baseValidator = getDatatypeValidator(uri, localPart);
        }
    }

    if (baseValidator == 0) {
        reportSchemaError(XMLUni::fgValidityDomain, XMLValid::UnknownBaseDatatype, baseTypeStr,
            getElementAttValue(rootElem, SchemaSymbols::fgATT_NAME));
    }
    else {

        int finalSet = baseValidator->getFinalSet();

        if (finalSet !=0  && ((finalSet & baseRefContext) != 0)) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DisallowedBaseDerivation, baseTypeStr);
            return 0;
        }
    }

    return baseValidator;
}


DOM_Element
TraverseSchema::getTopLevelComponentByName(const XMLCh* const compCategory,
                                           const XMLCh* const name) {

    // REVISIT - need to update to accomodate 'redefine'

    DOM_Element child = XUtil::getFirstChildElement(fSchemaRootElement);

    while (child != 0) {

        if (child.getLocalName().equals(DOMString(compCategory))) {

            if (child.getAttribute(SchemaSymbols::fgATT_NAME).equals(DOMString(name))) {
                break;
            }
        }
        else { // if redefine
            // REVISIT
        }

        child = XUtil::getNextSiblingElement(child);
    }

    return child;
}


const XMLCh* TraverseSchema::resolvePrefixToURI(const XMLCh* const prefix) {

    int nameSpaceIndex = fNamespaceScope->getNamespaceForPrefix(prefix, fCurrentNamespaceLevel);
    const XMLCh* uriStr = fURIStringPool->getValueForId(nameSpaceIndex);

    if (uriStr == 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::UnresolvedPrefix, prefix);
        return XMLUni::fgZeroLenString;
    }

    // REVISIT, !!!! a hack: needs to be updated later, cause now we only use
    // localpart to key build-in datatype.
    if (XMLString::stringLen(prefix) == 0 &&
        XMLString::compareString(uriStr, SchemaSymbols::fgURI_SCHEMAFORSCHEMA) == 0
        && XMLString::stringLen(fTargetNSURIString) == 0) {

        return XMLUni::fgZeroLenString;
    }

    return uriStr;
}


bool TraverseSchema::isTopLevelComponent(const DOM_Element& elem) {

    DOMString parentName = elem.getParentNode().getLocalName();

    fBuffer.set(parentName.rawBuffer(), parentName.length());
    XMLCh* nameStr = fBuffer.getRawBuffer();

    return (XMLString::endsWith(nameStr, SchemaSymbols::fgELT_SCHEMA))
            || (XMLString::endsWith(nameStr, SchemaSymbols::fgELT_REDEFINE));
}

QName* TraverseSchema::processElementDeclRef(const DOM_Element& elem,
                                             const XMLCh* const refName) {

    DOM_Element content = checkContent(elem, XUtil::getFirstChildElement(elem),
                                       true);

    if (content != 0) {
        reportSchemaError(XMLUni::fgValidityDomain, XMLValid::NoContentForRef, SchemaSymbols::fgELT_ELEMENT);
    }

    const XMLCh* prefix = getPrefix(refName);
    const XMLCh* localPart = getLocalPart(refName);
    const XMLCh* uriStr = resolvePrefixToURI(prefix);
    QName*       eltName = new QName(prefix , localPart, uriStr != 0
                                       ? fURIStringPool->addOrFind(uriStr)
                                       : fEmptyNamespaceURI); // StringPool.EMPTY_STRING == 0

    //if from another schema, just return the element QName
    if (XMLString::compareString(uriStr, fTargetNSURIString) != 0) {
        return eltName;
    }

    unsigned int uriID = eltName->getURI();
    SchemaElementDecl* refElemDecl = (SchemaElementDecl*)
        fSchemaGrammar->getElemDecl(uriID, localPart, 0, Grammar::TOP_LEVEL_SCOPE);

    //if not found, traverse the top level element that is referenced
    if (!refElemDecl) {

        DOM_Element targetElem =
                     getTopLevelComponentByName(SchemaSymbols::fgELT_ELEMENT,
                                                localPart);

        if (targetElem == 0)  {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::RefElementNotFound, localPart);
            // REVISIT do we return 0 or what? for now we will return QName created
            return eltName;
        }
        else {

            delete eltName;
            eltName = traverseElementDecl(targetElem);
            refElemDecl = (SchemaElementDecl*)
                fSchemaGrammar->getElemDecl(uriID, localPart, 0, Grammar::TOP_LEVEL_SCOPE);
        }
    }

    const SchemaElementDecl* other = (SchemaElementDecl*)
        fSchemaGrammar->getElemDecl(uriID, localPart, 0, fCurrentScope);

    if (refElemDecl && other
        && (refElemDecl->getComplexTypeInfo() != other->getComplexTypeInfo()
            || refElemDecl->getDatatypeValidator() != other->getDatatypeValidator())) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateElementDeclaration, localPart);
    }

    if (refElemDecl) {

        fRefElements->addElement(refElemDecl);
        fRefElemScope->addElement(fCurrentScope);
    }

    return eltName;
}

int TraverseSchema::parseBlockSet(const XMLCh* const blockStr,
                                  const int blockType) {

    if (!XMLString::stringLen(blockStr)) {
        return fBlockDefault;
    }

    int blockSet = 0;

    if (!XMLString::compareString(blockStr, SchemaSymbols::fgATTVAL_POUNDALL)) {

        blockSet = SchemaSymbols::EXTENSION + SchemaSymbols::RESTRICTION;
		
		if (blockType == ES_Block) {
			blockSet += SchemaSymbols::SUBSTITUTION;
        }

        return blockSet;
    }

    XMLStringTokenizer tokenizer(blockStr);

    while (tokenizer.hasMoreTokens()) {

        XMLCh* token = tokenizer.nextToken();

        if (!XMLString::compareString(token, SchemaSymbols::fgATTVAL_SUBSTITUTION)
			&& blockType == ES_Block) {

            if ((blockSet & SchemaSymbols::SUBSTITUTION) == 0 ) {
                blockSet += SchemaSymbols::SUBSTITUTION;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SubstitutionRepeated);
            }
        }
        else if (!XMLString::compareString(token, SchemaSymbols::fgATTVAL_EXTENSION)) {

            if ((blockSet & SchemaSymbols::EXTENSION) == 0) {
                blockSet += SchemaSymbols::EXTENSION;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ExtensionRepeated);
            }
        }
        else if (!XMLString::compareString(token, SchemaSymbols::fgATTVAL_RESTRICTION)) {

            if ((blockSet & SchemaSymbols::RESTRICTION) == 0 ) {
                blockSet += SchemaSymbols::RESTRICTION;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::RestrictionRepeated);
            }
        }
        else {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidBlockValue, blockStr);
        }
    } //end while

    return (blockSet == 0 ? fBlockDefault : blockSet);
}

int TraverseSchema::parseFinalSet(const XMLCh* const finalStr,
                                  const int finalType) {

    if (!XMLString::stringLen(finalStr)) {
        return fFinalDefault;
    }

    int finalSet = 0;

    if (!XMLString::compareString(finalStr, SchemaSymbols::fgATTVAL_POUNDALL)) {

        finalSet = SchemaSymbols::RESTRICTION;

        if (finalType == S_Final) {
            finalSet += SchemaSymbols::LIST + SchemaSymbols::UNION;
        }
        else {
            finalSet += SchemaSymbols::EXTENSION;
        }

        return finalSet;
    }

    XMLStringTokenizer tokenizer(finalStr);

    while (tokenizer.hasMoreTokens()) {

        XMLCh* token = tokenizer.nextToken();

        if (!XMLString::compareString(token, SchemaSymbols::fgELT_UNION)
            && finalType == S_Final) {

            if ((finalSet & SchemaSymbols::UNION) == 0) {
                finalSet += SchemaSymbols::UNION;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::UnionRepeated);
            }
        }
        else if (!XMLString::compareString(token, SchemaSymbols::fgATTVAL_EXTENSION)
                 && finalType != S_Final) {

            if ((finalSet & SchemaSymbols::EXTENSION) == 0) {
                finalSet += SchemaSymbols::EXTENSION;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ExtensionRepeated);
            }
        }
        else if (!XMLString::compareString(token, SchemaSymbols::fgELT_LIST)
                 && finalType == S_Final) {

            if ((finalSet & SchemaSymbols::LIST) == 0 ) {
                finalSet += SchemaSymbols::LIST;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ListRepeated);
            }
        }
        else if (!XMLString::compareString(token, SchemaSymbols::fgATTVAL_RESTRICTION)) {

            if ((finalSet & SchemaSymbols::RESTRICTION) == 0 ) {
                finalSet += SchemaSymbols::RESTRICTION;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::RestrictionRepeated);
            }
        }
        else {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidFinalValue, finalStr);
        }
    } //end while

    return (finalSet == 0 ? fFinalDefault : finalSet);
}


DOM_Element
TraverseSchema::checkIdentityConstraintContent(const DOM_Element& content) {

    DOM_Element child = content;

    if (child != 0) {

        do {

            DOMString childName = child.getLocalName();
            fBuffer.set(childName.rawBuffer(), childName.length());

            if (!isIdentityConstraintName(fBuffer.getRawBuffer())) {
                break;
            }

            child = XUtil::getNextSiblingElement(child);

        } while (child != 0);
    }

    return child;
}

bool TraverseSchema::isIdentityConstraintName(const XMLCh* const name) {

    return (XMLString::compareString(name, SchemaSymbols::fgELT_KEY) == 0
            || XMLString::compareString(name, SchemaSymbols::fgELT_KEYREF) == 0
            || XMLString::compareString(name, SchemaSymbols::fgELT_UNIQUE) == 0);
}

const XMLCh*
TraverseSchema::checkTypeFromAnotherSchema(const XMLCh* const typeStr) {

    const XMLCh* prefix = getPrefix(typeStr);
    const XMLCh* typeURI = resolvePrefixToURI(prefix);

    if (XMLString::compareString(typeURI, fTargetNSURIString) != 0
        && XMLString::compareString(typeURI,
                                    SchemaSymbols::fgURI_SCHEMAFORSCHEMA) != 0
        && XMLString::stringLen(typeURI) != 0) {
        return typeURI;
    }

    return 0;
}

DatatypeValidator*
TraverseSchema::getElementTypeValidator(const XMLCh* const typeStr,
                                        bool& noErrorDetected,
                                        const XMLCh* const otherSchemaURI,
                                        bool errorCheck)
{
    const XMLCh*       localPart = getLocalPart(typeStr);
    const XMLCh*       typeURI = otherSchemaURI;
    DatatypeValidator* dv = 0;

    if (otherSchemaURI != 0) {
        dv = getDatatypeValidator(typeURI, localPart);
    }
    else {
        const XMLCh* prefix = getPrefix(typeStr);

        typeURI = resolvePrefixToURI(prefix);
        dv = getDatatypeValidator(typeURI, localPart);

        if (dv == 0) {

            if (XMLString::compareString(typeURI,
                       SchemaSymbols::fgURI_SCHEMAFORSCHEMA) != 0
                || XMLString::compareString(fTargetNSURIString,
                       SchemaSymbols::fgURI_SCHEMAFORSCHEMA) == 0) {

                DOM_Element elem = getTopLevelComponentByName(
                                    SchemaSymbols::fgELT_SIMPLETYPE, localPart);

                if (elem != 0 && traverseSimpleTypeDecl(elem) != -1) {
                    dv = getDatatypeValidator(typeURI, localPart);
                }
            }
        }
    }

    if (dv == 0 && errorCheck) {
        noErrorDetected = false;
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::TypeNotFound, typeURI, localPart);
    }

    return dv;
}


ComplexTypeInfo*
TraverseSchema::getElementComplexTypeInfo(const XMLCh* const typeStr,
                                          bool& noErrorDetected,
                                          const XMLCh* const otherSchemaURI)
{
    const XMLCh*       localPart = getLocalPart(typeStr);
    const XMLCh*       typeURI = otherSchemaURI;
    ComplexTypeInfo*   typeInfo = 0;

    if (otherSchemaURI != 0) {
        typeInfo = getTypeInfoFromNS(typeURI, localPart);
    }
    else {

        const XMLCh* prefix = getPrefix(typeStr);

        typeURI = resolvePrefixToURI(prefix);
        fBuffer.set(typeURI);
        fBuffer.append(chComma);
        fBuffer.append(localPart);
        typeInfo = fComplexTypeRegistry->get(fBuffer.getRawBuffer());

        if (typeInfo == 0) {

            if (XMLString::compareString(typeURI,
                       SchemaSymbols::fgURI_SCHEMAFORSCHEMA) != 0
                || XMLString::compareString(fTargetNSURIString,
                       SchemaSymbols::fgURI_SCHEMAFORSCHEMA) == 0) {

                DOM_Element elem = getTopLevelComponentByName(
                                    SchemaSymbols::fgELT_COMPLEXTYPE, localPart);

                if (elem != 0) {

                    int typeIndex = traverseComplexTypeDecl(elem);
                    typeInfo =  fComplexTypeRegistry->get(
                                        fStringPool->getValueForId(typeIndex));
                }
            }
        }
    }

    return typeInfo;
}


SchemaElementDecl*
TraverseSchema::getSubstituteGroupElemDecl(const XMLCh* const name,
                                           bool& noErrorDetected) {

    const XMLCh*       nameURI =  resolvePrefixToURI(getPrefix(name));
    const XMLCh*       localPart = getLocalPart(name);
    SchemaElementDecl* elemDecl = 0;

    if (XMLString::compareString(nameURI, fTargetNSURIString) != 0) {
        elemDecl = getElementDeclFromNS(nameURI, localPart);
    }
    else {

        elemDecl = (SchemaElementDecl*)
                fSchemaGrammar->getElemDecl(fTargetNSURI, localPart,
                                            0, Grammar::TOP_LEVEL_SCOPE);

        if (elemDecl == 0) {

            DOM_Element subsGroupElem =
                getTopLevelComponentByName(SchemaSymbols::fgELT_ELEMENT,
                                           localPart);

            if (subsGroupElem != 0) {

                QName* subsGroupQName = traverseElementDecl(subsGroupElem);
                Janitor<QName> janQName(subsGroupQName);

                if (subsGroupQName != 0) {
                    elemDecl = (SchemaElementDecl*)
                       fSchemaGrammar->getElemDecl(fTargetNSURI, localPart,
                                                      0, Grammar::TOP_LEVEL_SCOPE);
                }
            }
        }
    }

    if (elemDecl == 0
        || (elemDecl->getDatatypeValidator() == 0
            && elemDecl->getComplexTypeInfo() == 0)) {

        noErrorDetected = false;
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::TypeNotFound, nameURI, localPart);
    }

    return elemDecl;
}

SchemaElementDecl*
TraverseSchema::getElementDeclFromNS(const XMLCh* const nameUri,
                                     const XMLCh* const localPart) {

    // REVISIT:
    Grammar* grammar = fGrammarResolver->getGrammar(nameUri);
    unsigned int uriId = fURIStringPool->addOrFind(nameUri);

    if (grammar != 0 && grammar->getGrammarType() == Grammar::SchemaGrammarType) {
        return (SchemaElementDecl*)
            grammar->getElemDecl(uriId, localPart, 0, Grammar::TOP_LEVEL_SCOPE);
    }

    reportSchemaError(XMLUni::fgValidityDomain, XMLValid::GrammarNotFound, nameUri);

    return 0;
}

bool
TraverseSchema::isSubstitutionGroupValid(const SchemaElementDecl* const subsElemDecl,
                                         const ComplexTypeInfo* const typeInfo,
                                         const DatatypeValidator* const validator,
                                         const XMLCh* const elemName,
                                         const bool toEmit) {

    // here we must do two things:
    // 1.  Make sure there actually *is* a relation between the types of
    // the element being nominated and the element doing the nominating;
    // (see PR 3.3.6 point #3 in the first tableau, for instance; this
    // and the corresponding tableaux from 3.4.6 and 3.14.6 rule out the nominated
    // element having an anonymous type declaration.
    // 2.  Make sure the nominated element allows itself to be nominated by
    // an element with the given type-relation.
    // Note:  we assume that (complex|simple)Type processing checks
    // whether the type in question allows itself to
    // be modified as this element desires.

    // if substitution element has any as content model type, return true
    if (subsElemDecl->getModelType() == SchemaElementDecl::Any) {
        return true;
    }

    bool subsRestricted = false;

    // Check for type relationship;
    // that is, make sure that the type we're deriving has some relatoinship
    // to substitutionGroupElt's type.
    if (typeInfo) { // do complexType case ...need testing

        int derivationMethod = typeInfo->getDerivedBy();

        if (typeInfo->getContentType() == SchemaElementDecl::Simple) {  // take care of complexType based on simpleType case...

            DatatypeValidator* elemDV = typeInfo->getDatatypeValidator();
            DatatypeValidator* subsValidator = subsElemDecl->getDatatypeValidator();

            if (subsValidator && subsValidator->isSubstitutableBy(elemDV)) {
                if ((subsElemDecl->getFinalSet() & derivationMethod) == 0) {
                    return true;
                }
                else {
                    subsRestricted = true;
                }
            }
        }
        else { // complex content

            ComplexTypeInfo* subsTypeInfo = subsElemDecl->getComplexTypeInfo();
            const ComplexTypeInfo* elemTypeInfo = typeInfo;

            for (; elemTypeInfo && elemTypeInfo != subsTypeInfo;
                elemTypeInfo = elemTypeInfo->getBaseComplexTypeInfo()) {
            }

            if (elemTypeInfo) {
                if ((subsElemDecl->getFinalSet() & derivationMethod) == 0) {
                    return true;
                }
                else {
                    subsRestricted = true;
                }
            }
        }
    }
    else if (validator) { // do simpleType case...

        // first, check for type relation.
        DatatypeValidator* subsValidator = subsElemDecl->getDatatypeValidator();

        if (subsValidator && subsValidator->isSubstitutableBy(validator)
            && ((subsElemDecl->getFinalSet() & SchemaSymbols::RESTRICTION) == 0)) {
                return true;
        }
    }

    if (toEmit) {
		if (subsRestricted) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidSubstitutionGroupElement,
                              elemName, subsElemDecl->getBaseName());
        }
        else {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SubstitutionGroupTypeMismatch, elemName);
        }
    }

    return false;
}


SchemaElementDecl*
TraverseSchema::createSchemaElementDecl(const DOM_Element& elem,
                                        const bool topLevel,
                                        const unsigned short elemType,
                                        bool& isDuplicate,
                                        const bool isFixedVal)
{
    const XMLCh* name = getElementAttValue(elem, SchemaSymbols::fgATT_NAME);
    const XMLCh* elemForm = getElementAttValue(elem, SchemaSymbols::fgATT_FORM);
    int enclosingScope = fCurrentScope;
    int uriIndex = fEmptyNamespaceURI;

    //refer to 4.3.2 in "XML Schema Part 1: Structures"
    if (topLevel) {

        uriIndex = fTargetNSURI;
        enclosingScope = Grammar::TOP_LEVEL_SCOPE;
    }
    else if ((XMLString::stringLen(elemForm) == 0 && fElementDefaultQualified)
             || XMLString::compareString(elemForm,SchemaSymbols::fgATTVAL_QUALIFIED) == 0) {

        uriIndex = fTargetNSURI;
    }

    // Check for duplicate elements
    SchemaElementDecl* other = (SchemaElementDecl*)
        fSchemaGrammar->getElemDecl(uriIndex, name, 0, enclosingScope);

    if (other != 0) {

        isDuplicate = true;
        return other;
    }

    const XMLCh* block = getElementAttValue(elem,SchemaSymbols::fgATT_BLOCK);
    const XMLCh* final = getElementAttValue(elem,SchemaSymbols::fgATT_FINAL);
    int blockSet = parseBlockSet(block, ES_Block);
    int finalSet = parseFinalSet(final, ECS_Final);
    int elementMiscFlags = 0;
    const XMLCh* nillable = getElementAttValue(elem, SchemaSymbols::fgATT_NILLABLE);
    const XMLCh* abstract = getElementAttValue(elem, SchemaSymbols::fgATT_ABSTRACT);

    if (nillable) {

        if (!XMLString::compareString(nillable, SchemaSymbols::fgATTVAL_TRUE)
            || !XMLString::compareString(nillable, fgValueOne)) {
            elementMiscFlags += SchemaSymbols::NILLABLE;
        }
    }

    if (abstract) {

        if (!XMLString::compareString(abstract, SchemaSymbols::fgATTVAL_TRUE)
            || !XMLString::compareString(abstract, fgValueOne)) {
            elementMiscFlags += SchemaSymbols::ABSTRACT;
        }
    }

    if (isFixedVal) {
        elementMiscFlags += SchemaSymbols::FIXED;
    }

    const XMLCh* prefix = getPrefix(name);
    SchemaElementDecl* elemDecl =
        new SchemaElementDecl(prefix, name, uriIndex,
                              (SchemaElementDecl::ModelTypes) elemType,
                              enclosingScope);

    elemDecl->setFinalSet(finalSet);
    elemDecl->setBlockSet(blockSet);
    elemDecl->setMiscFlags(elementMiscFlags);
    elemDecl->setCreateReason(XMLElementDecl::Declared);

    return elemDecl;
}


void TraverseSchema::processAttributeDeclRef(const DOM_Element& elem,
                                             const DOM_Element& simpleContent,
                                             ComplexTypeInfo* const typeInfo,
                                             const XMLCh* const refName,
                                             const XMLCh* const useAttr) {

    // REVISIT - fixedValue & defaultValue - is it valid to overwrite the 'ref'
    // attribute. For now, we are keeping the actual attribute declaration info
    // as is.

    // Check ref representation OK - 3.2.3::3.2
    if (!elem.getAttribute(SchemaSymbols::fgATT_FORM).equals("")
        || !elem.getAttribute(SchemaSymbols::fgATT_TYPE).equals("")
        || simpleContent != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::AttributeRefContentError);
    }

    if (!typeInfo && !fCurrentAttGroupInfo) {
        return;
    }

    const XMLCh* prefix = getPrefix(refName);
    const XMLCh* localPart = getLocalPart(refName);
    const XMLCh* uriStr = resolvePrefixToURI(prefix);
    unsigned int attURI = fURIStringPool->addOrFind(uriStr);

    // Check for duplicate references
    if (typeInfo && typeInfo->getAttDef(localPart, attURI)) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateRefAttribute, uriStr, localPart);
        return;
    }
    else if (fCurrentAttGroupInfo && fCurrentAttGroupInfo->containsAttribute(localPart, attURI)) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateRefAttribute, uriStr, localPart);
        return;
    }

    // check for different namespace
    if (XMLString::compareString(uriStr, fTargetNSURIString) != 0) {

        addAttributeDeclFromAnotherSchema(localPart, uriStr, typeInfo);
        return;
    }

    // if Global attribute registry does not contain the ref attribute, get
    // the referred attribute declaration and traverse it.
    if (fAttributeDeclRegistry->containsKey(localPart) == false) {

        DOM_Element referredAttribute =
            getTopLevelComponentByName(SchemaSymbols::fgELT_ATTRIBUTE, localPart);

        if (referredAttribute == 0) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::TopLevelAttributeNotFound, refName);
            return;
        }

        traverseAttributeDecl(referredAttribute, 0);
    }

    SchemaAttDef* refAttDef = (SchemaAttDef*)
                              fAttributeDeclRegistry->get(localPart);

    if (refAttDef == 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::TopLevelAttributeNotFound, refName);
        return;
    }

    bool required        = (XMLString::compareString(useAttr,
                                    SchemaSymbols::fgATTVAL_REQUIRED) == 0);
    bool prohibited      = (XMLString::compareString(useAttr,
                                    SchemaSymbols::fgATTVAL_PROHIBITED) == 0);
    QName* attQName      = refAttDef->getAttName();
    SchemaAttDef* attDef = new SchemaAttDef(attQName->getPrefix(),
                                            attQName->getLocalPart(),
                                            attQName->getURI(),
                                            refAttDef->getType());

    attDef->setValue(refAttDef->getValue());
    attDef->setDatatypeValidator(refAttDef->getDatatypeValidator());

    if (prohibited) {
        attDef->setDefaultType(XMLAttDef::Prohibited);
    }
    else if (required){

        if (refAttDef->getDefaultType() == XMLAttDef::Fixed) {
            attDef->setDefaultType(XMLAttDef::Required_And_Fixed);
        }
        else {
            attDef->setDefaultType(XMLAttDef::Required);
        }
    }

    bool toClone = false;

    if (typeInfo) {

        toClone = true;
        typeInfo->addAttDef(attDef);
    }

    if (fCurrentAttGroupInfo) {
        fCurrentAttGroupInfo->addAttDef(attDef, toClone);
    }
}


void TraverseSchema::checkMinMax(ContentSpecNode* const specNode,
                            const DOM_Element& elem,
                            const int allContextFlag) {

    unsigned int minOccurs = 0;
    unsigned int maxOccurs = 0;
    const XMLCh* minOccursStr =
        getElementAttValue(elem, SchemaSymbols::fgATT_MINOCCURS, true);
    const XMLCh* maxOccursStr =
        getElementAttValue(elem, SchemaSymbols::fgATT_MAXOCCURS, true);

    if (XMLString::stringLen(minOccursStr) == 0) {
        if (specNode)
            minOccurs = specNode->getMinOccurs();
        else
            minOccurs = 1;
    }
    else {
        XMLString::textToBin(minOccursStr, minOccurs);
        if (specNode)
            specNode->setMinOccurs(minOccurs);
    }

    bool isMaxUnbounded =
            (XMLString::compareString(maxOccursStr, fgUnbounded) == 0);

    if (isMaxUnbounded) {
        maxOccurs = -1;
        if (specNode)
            specNode->setMaxOccurs(maxOccurs);
    }
    else {
        if (XMLString::stringLen(maxOccursStr) == 0) {
            if (specNode)
                maxOccurs = specNode->getMaxOccurs();
            else
                maxOccurs = 1;
        }
        else {
            XMLString::textToBin(maxOccursStr, maxOccurs);
            if (specNode)
                specNode->setMaxOccurs(maxOccurs);
        }
    }

    if (minOccurs == 0 && maxOccurs == 0 && !isMaxUnbounded) {
        return;
    }

    // Constraint checking for min/max value
    if (!isMaxUnbounded) {

        XMLCh tmpMinStr[128];
        XMLCh tmpMaxStr[128];

        XMLString::binToText(minOccurs, tmpMinStr, 127, 10);
        XMLString::binToText(maxOccurs, tmpMaxStr, 127, 10);

        if (maxOccurs < 1) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidAttValue,
                              tmpMaxStr, SchemaSymbols::fgATT_MAXOCCURS);
        }
        else if (maxOccurs < minOccurs) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidMin2MaxOccurs,
                              tmpMinStr, tmpMaxStr);
        }
    }

    // Constraint checking for 'all' content
    bool isAllElement = (allContextFlag == All_Element);
    bool isAllGroup = (allContextFlag == All_Group);
    bool isGroupRefAll = (allContextFlag == Group_Ref_With_All);

    if (isAllElement || isAllGroup || isGroupRefAll) {

        if (maxOccurs != 1
			|| ((isAllGroup || isGroupRefAll || minOccurs != 0)
			    && minOccurs != 1)) {

            if (isAllElement) {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::BadMinMaxAllElem);
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::BadMinMaxAllCT);
            }
        }
    }
}


void TraverseSchema::processComplexContent(const XMLCh* const typeName,
                                           const DOM_Element& childElem,
                                           ComplexTypeInfo* const typeInfo,
                                           const XMLCh* const baseRawName,
                                           const XMLCh* const baseLocalPart,
                                           const XMLCh* const baseURI,
                                           const bool isMixed) {

    ContentSpecNode* specNode = 0;
    DOM_Element      attrNode;
    int              typeDerivedBy = SchemaSymbols::EMPTY_SET;
    bool             adoptSpecNode = true;
    ComplexTypeInfo* baseTypeInfo = typeInfo->getBaseComplexTypeInfo();

    if (baseTypeInfo) {

        if (typeInfo->getDerivedBy() == SchemaSymbols::RESTRICTION) {

            typeDerivedBy = SchemaSymbols::RESTRICTION;

            // check to see if the baseType permits derivation by restriction
            if((baseTypeInfo->getFinalSet() & typeDerivedBy) != 0) {

                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ForbiddenDerivationByRestriction,
                                  baseLocalPart);
                throw TraverseSchema::InvalidComplexTypeInfo;
            }
        }
        else {

            typeDerivedBy = SchemaSymbols::EXTENSION;

            // check to see if the baseType permits derivation by extension
            if((baseTypeInfo->getFinalSet() & typeDerivedBy) != 0) {

                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ForbiddenDerivationByExtension, baseLocalPart);
                throw TraverseSchema::InvalidComplexTypeInfo; // REVISIT - should we continue
            }

            // Check for derivation valid (extension) - 1.4.2.2
            int baseContentType = baseTypeInfo->getContentType();

            if (baseContentType != SchemaElementDecl::Empty) {
                if ((isMixed && baseContentType == SchemaElementDecl::Children)
                    || (!isMixed && baseContentType == SchemaElementDecl::Mixed)) {

                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::MixedOrElementOnly, baseLocalPart, typeName);
                    throw TraverseSchema::InvalidComplexTypeInfo; //REVISIT - should we continue
                }
            }

            processElements(baseTypeInfo, typeInfo);
        }
    }

    if (childElem != 0) {

        // --------------------------------------------------------------------
        // GROUP, ALL, SEQUENCE or CHOICE, followed by attributes, if specified.
        // Note that it's possible that only attributes are specified.
        // --------------------------------------------------------------------
        DOMString childName = childElem.getLocalName();
        ContentSpecNode* tmpSpecNode = 0;

        if (childName.equals(SchemaSymbols::fgELT_GROUP)) {

            bool             adoptSpecNode = true;
            XercesGroupInfo* grpInfo = traverseGroupDecl(childElem, adoptSpecNode);

            if (grpInfo) {

                specNode = grpInfo->getContentSpec();
                int contentContext = hasAllContent(specNode) ? Group_Ref_With_All : Not_All_Context;
                checkMinMax(specNode, childElem, contentContext);
            }

            attrNode = XUtil::getNextSiblingElement(childElem);

        }
        else if (childName.equals(SchemaSymbols::fgELT_SEQUENCE)) {

            specNode = traverseChoiceSequence(childElem, ContentSpecNode::Sequence, adoptSpecNode);
            checkMinMax(specNode, childElem, Not_All_Context);
            attrNode = XUtil::getNextSiblingElement(childElem);
        }
        else if (childName.equals(SchemaSymbols::fgELT_CHOICE)) {

            specNode = traverseChoiceSequence(childElem, ContentSpecNode::Choice, adoptSpecNode);
            checkMinMax(specNode, childElem, Not_All_Context);
            attrNode = XUtil::getNextSiblingElement(childElem);
        }
        else if (childName.equals(SchemaSymbols::fgELT_ALL)) {

            //specNode = checkMinMax(traverseAll(childElem), childElem);
            attrNode = XUtil::getNextSiblingElement(childElem);
        }
        else if (isAttrOrAttrGroup(childElem)) {
            // reset the contentType
            typeInfo->setContentType(SchemaElementDecl::Any);
            attrNode = childElem;
        }
        else {

            fBuffer.set(childName.rawBuffer(), childName.length());
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidChildInComplexType,
                              fBuffer.getRawBuffer());
        }

        if (tmpSpecNode != specNode) {
            adoptSpecNode = true;
        }
    }

    if (isMixed) {

        // add #PCDATA leaf
        QName* tmpName = new QName(XMLUni::fgZeroLenString, XMLUni::fgZeroLenString, XMLElementDecl::fgPCDataElemId);
        Janitor<QName> janQName(tmpName);
        ContentSpecNode* pcdataNode = new ContentSpecNode(tmpName);

        // If there was an element, the content spec becomes a choice of PCDATA and
        // the element
        if (specNode != 0) {
            specNode = new ContentSpecNode(ContentSpecNode::Choice,
                                           pcdataNode, specNode, true, adoptSpecNode);
        }
        else {
            specNode = pcdataNode;
        }

        adoptSpecNode = true;
    }

    typeInfo->setContentSpec(specNode);
    typeInfo->setAdoptContentSpec(adoptSpecNode);

    // -----------------------------------------------------------------------
    // Merge in information from base, if it exists
    // -----------------------------------------------------------------------
    if (baseTypeInfo != 0) {

        ContentSpecNode* baseSpecNode = baseTypeInfo->getContentSpec();

        if (typeDerivedBy == SchemaSymbols::RESTRICTION) {

            //check derivation valid - content type is empty (5.2)
            if (!typeInfo->getContentSpec()) {
                int baseContentType = baseTypeInfo->getContentType();

                if (baseContentType != SchemaElementDecl::Empty
                    && ((baseContentType != SchemaElementDecl::Children
                         && baseContentType != SchemaElementDecl::Mixed)
                        || !emptiableParticle(baseSpecNode))) {
                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::EmptyComplexRestrictionDerivation);
                }
            }

            // Delay particle constraint checking (5.3) until we have processed
            // the whole schema.
        }
        else {

            // Compose the final content model by concatenating the base and
            // the current in sequence
            if (!specNode) {

                typeInfo->setContentSpec(baseSpecNode);
                typeInfo->setAdoptContentSpec(false);
            }
            else if (baseSpecNode != 0) {

                bool toAdoptSpecNode = typeInfo->getAdoptContentSpec();

                typeInfo->setAdoptContentSpec(false);
                typeInfo->setContentSpec(
                    new ContentSpecNode(ContentSpecNode::Sequence, baseSpecNode,
                                        specNode, false,
                                        toAdoptSpecNode));
                typeInfo->setAdoptContentSpec(true);
            }
        }
    }
    else {
        typeInfo->setDerivedBy(0);
    }

    // -------------------------------------------------------------
    // Set the content type
    // -------------------------------------------------------------
    if (isMixed) {
        typeInfo->setContentType(SchemaElementDecl::Mixed);
    }
    else if (typeInfo->getContentSpec() == 0) {
        typeInfo->setContentType(SchemaElementDecl::Empty);
    }
    else {
        typeInfo->setContentType(SchemaElementDecl::Children);
    }

    // -------------------------------------------------------------
    // Now, check attributes and handle
    // -------------------------------------------------------------
    if (attrNode != 0) {

        if (!isAttrOrAttrGroup(attrNode)) {

            fBuffer.set(attrNode.getLocalName().rawBuffer(),
                        attrNode.getLocalName().length());
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidChildInComplexType,
                              fBuffer.getRawBuffer());
        }
        else {
              processAttributes(attrNode, baseRawName, baseLocalPart,
                                baseURI, typeInfo);
        }
    }
    else if (baseTypeInfo != 0) {
        processAttributes(0, baseRawName, baseLocalPart, baseURI, typeInfo);
    }
}


void TraverseSchema::processBaseTypeInfo(const XMLCh* const baseName,
                                         const XMLCh* const localPart,
                                         const XMLCh* const uriStr,
                                         ComplexTypeInfo* const typeInfo) {

    ComplexTypeInfo* baseComplexTypeInfo = 0;
    DatatypeValidator* baseDTValidator = 0;

    // -------------------------------------------------------------
    // check if the base type is from another schema
    // -------------------------------------------------------------
    if (isBaseFromAnotherSchema(uriStr)) {

        baseComplexTypeInfo = getTypeInfoFromNS(uriStr, localPart);

        if (baseComplexTypeInfo == 0) {

            baseDTValidator = getDatatypeValidator(uriStr, localPart);

            if (baseDTValidator == 0) {

                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::TypeNotFound, uriStr, localPart);
                throw TraverseSchema::InvalidComplexTypeInfo;
            }
        }
    }

    // -------------------------------------------------------------
    // type must be from same schema
    // -------------------------------------------------------------
    else {

        fBuffer.set(uriStr);
        fBuffer.append(chComma);
        fBuffer.append(localPart);

        // assume the base is a complexType and try to locate the base type first
        const XMLCh* fullBaseName = fBuffer.getRawBuffer();
        baseComplexTypeInfo = fComplexTypeRegistry->get(fullBaseName);

        // Circular check
        if (baseComplexTypeInfo &&
            fCurrentTypeNameStack->containsElement(fStringPool->addOrFind(fullBaseName))) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoCircularDefinition, fullBaseName);
            throw TraverseSchema::InvalidComplexTypeInfo;
        }

        // if not found, 2 possibilities:
        //           1: ComplexType in question has not been compiled yet;
        //           2: base is SimpleTYpe;
        if (baseComplexTypeInfo == 0) {

            baseDTValidator = getDatatypeValidator(uriStr, localPart);

            if (baseDTValidator == 0) {

                int baseTypeSymbol;
                DOM_Element baseTypeNode = getTopLevelComponentByName(
                                SchemaSymbols::fgELT_COMPLEXTYPE, localPart);

                if (baseTypeNode != 0) {

                    baseTypeSymbol = traverseComplexTypeDecl(baseTypeNode);
                    baseComplexTypeInfo = fComplexTypeRegistry->get(
                                fStringPool->getValueForId(baseTypeSymbol));
                }
                else {

                    baseTypeNode = getTopLevelComponentByName(
                                SchemaSymbols::fgELT_SIMPLETYPE, localPart);

                    if (baseTypeNode != 0) {

                        baseTypeSymbol = traverseSimpleTypeDecl(baseTypeNode);
                        baseDTValidator = getDatatypeValidator(uriStr, localPart);

                        if (baseDTValidator == 0)  {

                            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::TypeNotFound, uriStr, localPart, uriStr);
                            throw TraverseSchema::InvalidComplexTypeInfo;
                        }
                    }
                    else {

                        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::BaseTypeNotFound, baseName);
                        throw TraverseSchema::InvalidComplexTypeInfo;
                    }
                }
            }
        }
    } // end else (type must be from same schema)

    typeInfo->setBaseComplexTypeInfo(baseComplexTypeInfo);
    typeInfo->setBaseDatatypeValidator(baseDTValidator);
}


ComplexTypeInfo* TraverseSchema::getTypeInfoFromNS(const XMLCh* const uriStr,
                                                   const XMLCh* const localPart)
{
    Grammar* grammar = fGrammarResolver->getGrammar(uriStr);

    if (grammar != 0 && grammar->getGrammarType() == Grammar::SchemaGrammarType) {

        fBuffer.set(uriStr);
        fBuffer.append(chComma);
        fBuffer.append(localPart);

        ComplexTypeInfo* typeInfo =
            ((SchemaGrammar*)grammar)->getComplexTypeRegistry()->get(fBuffer.getRawBuffer());

        return typeInfo;
    }
    else {
        reportSchemaError(XMLUni::fgValidityDomain, XMLValid::GrammarNotFound, uriStr);
    }

    return 0;
}


bool TraverseSchema::isValidFacet(const XMLCh* const component,
                                  const XMLCh* const name) {

    // TO DO
    return true;
}


void TraverseSchema::processAttributes(const DOM_Element& attElem,
                                       const XMLCh* const baseRawName,
                                       const XMLCh* const baseLocalPart,
                                       const XMLCh* const baseURI,
                                       ComplexTypeInfo* const typeInfo) {

    // If we do not have a complexTypeInfo, then what is the point of
    // processing.
    if (typeInfo == 0) {
        return;
    }

    DOM_Element child = attElem;
    SchemaAttDef* attWildCard = 0;
    Janitor<SchemaAttDef> janAttWildCard(0);
    XercesAttGroupInfo* attGroupInfo = 0;

    for (; child != 0; child = XUtil::getNextSiblingElement(child)) {

        DOMString childName = child.getLocalName();

        if (childName.equals(SchemaSymbols::fgELT_ATTRIBUTE)) {
            traverseAttributeDecl(child, typeInfo);
        }
        else if (childName.equals(SchemaSymbols::fgELT_ATTRIBUTEGROUP)) {
            attGroupInfo = traverseAttributeGroupDecl(child, typeInfo);
        }
        else if (childName.equals(SchemaSymbols::fgELT_ANYATTRIBUTE) ) {
            attWildCard = traverseAnyAttribute(child);
            janAttWildCard.reset(attWildCard);
        }
        else {

            fBuffer.set(childName.rawBuffer(), childName.length());
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidChildInComplexType,
                              fBuffer.getRawBuffer());
        }
    }

    // -------------------------------------------------------------
    // Handle wild card/any attribute
    // -------------------------------------------------------------
    ComplexTypeInfo* baseTypeInfo = typeInfo->getBaseComplexTypeInfo();
    SchemaAttDef* baseAttWildCard = (baseTypeInfo) ? baseTypeInfo->getAttWildCard() : 0;
    int derivedBy = typeInfo->getDerivedBy();

    if (attGroupInfo) {

        unsigned int anyAttCount = attGroupInfo->anyAttributeCount();

        if (anyAttCount) {

            SchemaAttDef* attCompleteWildCard = attGroupInfo->getCompleteWildCard();
            XMLAttDef::DefAttTypes defAttType = (attWildCard)
                ? attWildCard->getDefaultType() : attGroupInfo->anyAttributeAt(0)->getDefaultType();

            if (!attCompleteWildCard) {

                attCompleteWildCard = new SchemaAttDef(attGroupInfo->anyAttributeAt(0));

                for (unsigned int i= 1; i < anyAttCount; i++) {
                    attWildCardIntersection(attCompleteWildCard, attGroupInfo->anyAttributeAt(i));
                }

                attGroupInfo->setCompleteWildCard(attCompleteWildCard);
            }

            if (attWildCard) {
                attWildCardIntersection(attWildCard, attCompleteWildCard);
            }
            else {

                attWildCard = new SchemaAttDef(attCompleteWildCard);
                janAttWildCard.reset(attWildCard);
            }

            attWildCard->setDefaultType(defAttType);
        }
    }

    if (derivedBy == SchemaSymbols::EXTENSION && baseAttWildCard && attWildCard) {

        XMLAttDef::DefAttTypes saveDefType = attWildCard->getDefaultType();
        attWildCardUnion(attWildCard, baseAttWildCard);
        attWildCard->setDefaultType(saveDefType);
    }

    // -------------------------------------------------------------
    // insert wildcard attribute
    // -------------------------------------------------------------
    if (attWildCard) {

        typeInfo->setAttWildCard(attWildCard);
        janAttWildCard.orphan();

        if (attWildCard->getType() == XMLAttDef::AttTypes_Unknown) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NotExpressibleWildCardIntersection);
        }
    }
    else if (baseAttWildCard && derivedBy == SchemaSymbols::EXTENSION) {

        SchemaAttDef* newWildCard = new SchemaAttDef(baseAttWildCard);
        typeInfo->setAttWildCard(newWildCard);
    }

    // -------------------------------------------------------------
    // Check attributes derivation OK
    // -------------------------------------------------------------
    bool baseWithAttributes = (baseTypeInfo && baseTypeInfo->hasAttDefs());
    bool childWithAttributes = typeInfo->hasAttDefs();

    if (derivedBy == SchemaSymbols::RESTRICTION && childWithAttributes) {

        if (!baseWithAttributes && !baseAttWildCard) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::BadAttDerivation_1);
        }
        else {
            checkAttDerivationOK(baseTypeInfo, typeInfo);
		}
    }

    // -------------------------------------------------------------
    // merge in base type's attribute decls
    // -------------------------------------------------------------
    if (baseTypeInfo && baseTypeInfo->hasAttDefs()) {

        SchemaAttDefList& baseAttList = (SchemaAttDefList&)
                                        baseTypeInfo->getAttDefList();

        while (baseAttList.hasMoreElements()) {

            SchemaAttDef& attDef = (SchemaAttDef&) baseAttList.nextElement();
            QName* attName = attDef.getAttName();
            const XMLCh* localPart = attName->getLocalPart();

            // if found a duplicate, then skip the one from the base type
            if (typeInfo->getAttDef(localPart, attName->getURI()) != 0) {

                if (derivedBy == SchemaSymbols::EXTENSION) {
                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateAttInDerivation, localPart);
                }

                continue;
            }

            SchemaAttDef* newAttDef = new SchemaAttDef(attName->getPrefix(),
                                                       attName->getLocalPart(),
                                                       attName->getURI(),
                                                       attDef.getValue(),
                                                       attDef.getType(),
                                                       attDef.getDefaultType(),
                                                       attDef.getEnumeration());

            newAttDef->setDatatypeValidator(attDef.getDatatypeValidator());
            typeInfo->addAttDef(newAttDef);
        }
    }
}


int
TraverseSchema::addAttributeDeclFromAnotherSchema(const XMLCh* const name,
                                                  const XMLCh* const uri,
                                                  ComplexTypeInfo* const typeInfo)
{
    SchemaGrammar* aGrammar = (SchemaGrammar*)
                              fGrammarResolver->getGrammar(uri);

    if (XMLString::stringLen(uri) == 0 || aGrammar == 0
        || aGrammar->getGrammarType() != Grammar::SchemaGrammarType) {

        reportSchemaError(XMLUni::fgValidityDomain, XMLValid::GrammarNotFound, uri);
        return -1;
    }

    if (aGrammar->getAttributeDeclRegistry() == 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoAttributeInSchema, name, uri);
        return -1;
    }

    SchemaAttDef* tempAtt = (SchemaAttDef*)
                            aGrammar->getAttributeDeclRegistry()->get(name);

    if (tempAtt == 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoAttributeInSchema, name, uri);
        return -1;
    }

    if (typeInfo || fCurrentAttGroupInfo) {

        QName* attName = tempAtt->getAttName();

        if (typeInfo->getAttDef(attName->getLocalPart(), attName->getURI()) != 0) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateRefAttribute, uri, name);
            return -1;
        }

        SchemaAttDef* newAtt = new SchemaAttDef(attName->getPrefix(),
                                                attName->getLocalPart(),
                                                attName->getURI(),
                                                tempAtt->getValue(),
                                                tempAtt->getType(),
                                                tempAtt->getDefaultType(),
                                                tempAtt->getEnumeration());

        newAtt->setDatatypeValidator(tempAtt->getDatatypeValidator());

        bool toClone = false;

        if (typeInfo) {

            toClone = true;
            typeInfo->addAttDef(newAtt);
        }

        if (fCurrentGroupInfo) {
            fCurrentAttGroupInfo->addAttDef(newAtt, toClone);
        }
    }

    return 0;
}


void TraverseSchema::defaultComplexTypeInfo(ComplexTypeInfo* const typeInfo) {

    if (typeInfo) {

        typeInfo->setDerivedBy(0);
        typeInfo->setContentType(SchemaElementDecl::Any);
        typeInfo->setDatatypeValidator(0);
        typeInfo->setContentSpec(0);
    }
}


InputSource* TraverseSchema::resolveSchemaLocation(const XMLCh* const loc) {

    // ------------------------------------------------------------------
    // Create an input source
    // ------------------------------------------------------------------
    InputSource* srcToFill = 0;

    if (fEntityResolver){
        srcToFill = fEntityResolver->resolveEntity(XMLUni::fgZeroLenString,
                                                   loc);
    }

    //  If they didn't create a source via the entity resolver, then we
    //  have to create one on our own.
    if (!srcToFill) {

        try {

            XMLURL urlTmp(fCurrentSchemaURL, loc);

            if (urlTmp.isRelative()) {
                ThrowXML(MalformedURLException,
                         XMLExcepts::URL_NoProtocolPresent);
            }

            srcToFill = new URLInputSource(urlTmp);
        }
        catch(const MalformedURLException&) {
            // Its not a URL, so lets assume its a local file name.
            srcToFill = new LocalFileInputSource(fCurrentSchemaURL,loc);
        }
    }

    return srcToFill;
}


void TraverseSchema::restoreSchemaInfo() {

    setCurrentSchemaURL(fCurrentSchemaInfo->getCurrentSchemaURL());
    fElementDefaultQualified = fCurrentSchemaInfo->isElementDefaultQualified();
    fAttributeDefaultQualified = fCurrentSchemaInfo->isAttributeDefaultQualified();
    fBlockDefault = fCurrentSchemaInfo->getBlockDefault();
    fFinalDefault = fCurrentSchemaInfo->getFinalDefault();
    fCurrentNamespaceLevel = fCurrentSchemaInfo->getNamespaceScopeLevel();
    fSchemaRootElement = fCurrentSchemaInfo->getRoot();
}


bool
TraverseSchema::emptiableParticle(const ContentSpecNode* const specNode) {

    if (getMinTotalRange(specNode) == 0) {
        return true;
    }

    return false;
}

int TraverseSchema::getMinTotalRange(const ContentSpecNode* const specNode) {

    if (!specNode) {
        return 0;
    }

    int min = ((ContentSpecNode*)specNode)->getMinOccurs();

    ContentSpecNode::NodeTypes nodeType = specNode->getType();
    const ContentSpecNode* first = 0;
    const ContentSpecNode* second = 0;

    if (nodeType == ContentSpecNode::Sequence
        || nodeType == ContentSpecNode::Choice) {

        first = specNode->getFirst();
        second = specNode->getSecond();
        int minFirst = getMinTotalRange(first);

        if (second) {

            int minSecond = getMinTotalRange(second);

            if (nodeType == ContentSpecNode::Choice)
                min = min * ((minFirst < minSecond)? minFirst : minSecond);
            else
                min = min * (minFirst + minSecond);
        }
        else
            min = min * minFirst;
    }

    return min;
}

int TraverseSchema::getMaxTotalRange(const ContentSpecNode* const specNode) {

    if (!specNode) {
        return 0;
    }

    ContentSpecNode::NodeTypes nodeType = specNode->getType();

    if (nodeType == ContentSpecNode::OneOrMore ||
        nodeType == ContentSpecNode::ZeroOrMore) {
        return SchemaSymbols::UNBOUNDED;
    }

    const ContentSpecNode* first = 0;
    const ContentSpecNode* second = 0;
    int max = 1;

    if (nodeType == ContentSpecNode::Sequence
        || nodeType == ContentSpecNode::Choice) {

        first = specNode->getFirst();
        second = specNode->getSecond();
        max = getMaxTotalRange(first);

        if (second && (max != SchemaSymbols::UNBOUNDED)) {

            int maxSecond = getMaxTotalRange(second);

            if (maxSecond == SchemaSymbols::UNBOUNDED) {
                max = maxSecond;
            }
            else {
                if (nodeType == ContentSpecNode::Choice) {
                    max = (max > maxSecond) ? max : maxSecond;
                }
                else {
                    max += maxSecond;
                }
            }
        }
    }

    return max;
}

void TraverseSchema::checkFixedFacet(const DOM_Element& elem,
                                     const XMLCh* const facetName,
                                     const DatatypeValidator* const baseDV,
                                     unsigned int& flags)
{
    const XMLCh* fixedFacet =
                    getElementAttValue(elem, SchemaSymbols::fgATT_FIXED);

    if (fixedFacet &&
        (!XMLString::compareString(fixedFacet, SchemaSymbols::fgATTVAL_TRUE)
         || !XMLString::compareString(fixedFacet, fgValueOne))) {

        if (!XMLString::compareString(SchemaSymbols::fgELT_MINLENGTH, facetName)) {
            flags |= DatatypeValidator::FACET_MINLENGTH;
        }
        else if (!XMLString::compareString(SchemaSymbols::fgELT_MAXLENGTH, facetName)) {
            flags |= DatatypeValidator::FACET_MAXLENGTH;
        }
        else if (!XMLString::compareString(SchemaSymbols::fgELT_MAXEXCLUSIVE, facetName)) {
            flags |= DatatypeValidator::FACET_MAXEXCLUSIVE;
        }
        else if (!XMLString::compareString(SchemaSymbols::fgELT_MAXINCLUSIVE, facetName)) {
            flags |= DatatypeValidator::FACET_MAXINCLUSIVE;
        }
        else if (!XMLString::compareString(SchemaSymbols::fgELT_MINEXCLUSIVE, facetName)) {
            flags |= DatatypeValidator::FACET_MINEXCLUSIVE;
        }
        else if (!XMLString::compareString(SchemaSymbols::fgELT_MININCLUSIVE, facetName)) {
            flags |= DatatypeValidator::FACET_MININCLUSIVE;
        }
        else if (!XMLString::compareString(SchemaSymbols::fgELT_TOTALDIGITS, facetName)) {
            flags |= DatatypeValidator::FACET_TOTALDIGITS;
        }
        else if (!XMLString::compareString(SchemaSymbols::fgELT_FRACTIONDIGITS, facetName)) {
            flags |= DatatypeValidator::FACET_FRACTIONDIGITS;
        }
        else if ((!XMLString::compareString(SchemaSymbols::fgELT_WHITESPACE, facetName)) &&
                 baseDV->getType() == DatatypeValidator::String) {
            flags |= DatatypeValidator::FACET_WHITESPACE;
        }
    }
}

void TraverseSchema::checkRefElementConsistency() {

    unsigned int refElemSize = fRefElements->size();

    for (unsigned int i=0; i < refElemSize; i++) {

        int elemScope = fRefElemScope->elementAt(i);
        SchemaElementDecl* elem = fRefElements->elementAt(i);
        RefVectorOf<SchemaElementDecl>* subsElements =
            fValidSubstitutionGroups->get(elem->getBaseName(), elem->getURI());

        if (subsElements) {
            unsigned subsElemSize = subsElements->size();

            for (unsigned int j=0; j < subsElemSize; j++) {

                SchemaElementDecl* subsElem = subsElements->elementAt(j);
                const XMLCh* subsElemName = subsElem->getBaseName();
                SchemaElementDecl* sameScopeElem = (SchemaElementDecl*)
                    fSchemaGrammar->getElemDecl(subsElem->getURI(), subsElemName, 0, elemScope);

                if (sameScopeElem
                    && (subsElem->getComplexTypeInfo() != sameScopeElem->getComplexTypeInfo()
                        || subsElem->getDatatypeValidator() != sameScopeElem->getDatatypeValidator())) {
                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateElementDeclaration, subsElemName);
                }
            }
        }
    }
}

void
TraverseSchema::buildValidSubstitutionListB(SchemaElementDecl* const elemDecl,
                                            SchemaElementDecl* const subsElemDecl) {

    int elemURI = subsElemDecl->getURI();
    XMLCh* elemName = subsElemDecl->getBaseName();

    SchemaElementDecl* chainElem = fSubstitutionGroups->get(elemName, elemURI);

    if (!chainElem || (chainElem == elemDecl)) {
        return;
    }

    RefVectorOf<SchemaElementDecl>* validSubsElements =
        fValidSubstitutionGroups->get(chainElem->getBaseName(), chainElem->getURI());

    if (validSubsElements->containsElement(elemDecl)) {
        return;
    }

    if (isSubstitutionGroupValid(chainElem, elemDecl->getComplexTypeInfo(),
                                 elemDecl->getDatatypeValidator(), 0, false)) {

        validSubsElements->addElement(elemDecl);
    }

    buildValidSubstitutionListB(elemDecl, chainElem);
}

void
TraverseSchema::buildValidSubstitutionListF(SchemaElementDecl* const elemDecl,
                                            SchemaElementDecl* const subsElemDecl) {

    int elemURI = elemDecl->getURI();
    XMLCh* elemName = elemDecl->getBaseName();
    RefVectorOf<SchemaElementDecl>* validSubsElements =
        fValidSubstitutionGroups->get(elemName, elemURI);

    if (validSubsElements) {

        unsigned int elemSize = validSubsElements->size();
        for (unsigned int i=0; i<elemSize; i++) {

            SchemaElementDecl* chainElem = validSubsElements->elementAt(i);

            if (chainElem == subsElemDecl) {
                continue;
            }

            RefVectorOf<SchemaElementDecl>* validSubs =
                fValidSubstitutionGroups->get(subsElemDecl->getBaseName(), subsElemDecl->getURI());

            if (validSubs->containsElement(chainElem)) {
                continue;
            }

            if (isSubstitutionGroupValid(subsElemDecl, chainElem->getComplexTypeInfo(),
                                         chainElem->getDatatypeValidator(), 0, false)) {
                validSubsElements->addElement(chainElem);
            }

            buildValidSubstitutionListF(chainElem, subsElemDecl);
        }
    }
}

void TraverseSchema::checkEnumerationRequiredNotation(const XMLCh* const name,
                                                      const XMLCh* const type) {

    const XMLCh* localPart = getLocalPart(type);

    if (!XMLString::compareString(localPart, SchemaSymbols::fgELT_NOTATION)) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoNotationType, name);
    }
}

XercesGroupInfo* TraverseSchema::processGroupRef(const DOM_Element& elem,
                                                 const XMLCh* const refName,
                                                 bool& toAdoptSpecNode) {

    if (XUtil::getFirstChildElement(elem) != 0) {
        reportSchemaError(XMLUni::fgValidityDomain, XMLValid::NoContentForRef, SchemaSymbols::fgELT_GROUP);
    }

    const XMLCh* prefix = getPrefix(refName);
    const XMLCh* localPart = getLocalPart(refName);
    const XMLCh* uriStr = resolvePrefixToURI(prefix);

    fBuffer.set(uriStr);
    fBuffer.append(chComma);
    fBuffer.append(localPart);

    unsigned int nameIndex = fStringPool->addOrFind(fBuffer.getRawBuffer());
	
    if (fCurrentGroupStack->containsElement(nameIndex)) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoCircularDefinition, localPart);
        return 0;
    }

    XercesGroupInfo* groupInfo = 0;

    //if from another schema
    if (XMLString::compareString(uriStr, fTargetNSURIString) != 0) {
        groupInfo = traverseGroupDeclNS(uriStr, localPart, toAdoptSpecNode);
    }
    else {

        groupInfo = fGroupRegistry->get(localPart);

        if (!groupInfo) {

            DOM_Element groupElem =
                getTopLevelComponentByName(SchemaSymbols::fgELT_GROUP, localPart);

            if (groupElem != 0) {

                groupInfo = traverseGroupDecl(groupElem, toAdoptSpecNode);

                if (groupInfo && fCurrentGroupInfo
                    && groupInfo->getScope() == fCurrentGroupInfo->getScope()) {
                    copyGroupElements(groupInfo, fCurrentGroupInfo, 0);
                }

                return groupInfo;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DeclarationNotFound,
                                  SchemaSymbols::fgELT_GROUP, uriStr, localPart);
            }
        }
        else {
            toAdoptSpecNode = false;
        }
    }

    if (groupInfo) {
        copyGroupElements(groupInfo, fCurrentGroupInfo, fCurrentComplexType);
    }

    return groupInfo;
}


XercesAttGroupInfo*
TraverseSchema::processAttributeGroupRef(const DOM_Element& elem,
                                         const XMLCh* const refName,
                                         ComplexTypeInfo* const typeInfo) {

    if (XUtil::getFirstChildElement(elem) != 0) {
        reportSchemaError(XMLUni::fgValidityDomain, XMLValid::NoContentForRef, SchemaSymbols::fgELT_ATTRIBUTEGROUP);
    }

    const               XMLCh* prefix = getPrefix(refName);
    const               XMLCh* localPart = getLocalPart(refName);
    const               XMLCh* uriStr = resolvePrefixToURI(prefix);
    XercesAttGroupInfo* attGroupInfo = 0;

    if (XMLString::compareString(uriStr, fTargetNSURIString) != 0) {
         attGroupInfo = traverseAttributeGroupDeclNS(uriStr, localPart);
    }
    else {

        // circular check
        DOM_Node parentElem = elem.getParentNode();

        if (parentElem.getLocalName().equals(SchemaSymbols::fgELT_ATTRIBUTEGROUP)
            && ((DOM_Element&) parentElem).getAttribute(SchemaSymbols::fgATT_NAME).equals(localPart)
            && !(parentElem.getParentNode().getLocalName().equals(SchemaSymbols::fgELT_REDEFINE))) {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoCircularAttGroup);
            return 0;
        }

        attGroupInfo = fAttGroupRegistry->get(localPart);

        if (!attGroupInfo) {

            // traverse top level attributeGroup - if found
            DOM_Element attGroupElem =
                getTopLevelComponentByName(SchemaSymbols::fgELT_ATTRIBUTEGROUP, localPart);

            if (attGroupElem != 0) {

                attGroupInfo = traverseAttributeGroupDecl(attGroupElem, typeInfo);

                if (attGroupInfo && fCurrentAttGroupInfo) {
                    copyAttGroupAttributes(attGroupInfo, fCurrentAttGroupInfo, 0);
                }

                return attGroupInfo;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DeclarationNotFound,
                                  SchemaSymbols::fgELT_ATTRIBUTEGROUP, uriStr, localPart);
            }
        }
    }

    if (attGroupInfo) {
        copyAttGroupAttributes(attGroupInfo, fCurrentAttGroupInfo, typeInfo);
    }

    return attGroupInfo;
}

bool TraverseSchema::hasAllContent(const ContentSpecNode* const specNode) {

    if (specNode) {

        const ContentSpecNode* tmpSpecNode = specNode;

        if (specNode->getType() == ContentSpecNode::ZeroOrOne) {
            tmpSpecNode = specNode->getFirst();
        }

        return (tmpSpecNode->getType() == ContentSpecNode::All);
    }

    return false;
}

void TraverseSchema::processElements(ComplexTypeInfo* const baseTypeInfo,
                                     ComplexTypeInfo* const newTypeInfo) {

    unsigned int elemCount = baseTypeInfo->elementCount();

    if (elemCount) {

        int newTypeScope = newTypeInfo->getScopeDefined();
        int schemaURI = fURIStringPool->addOrFind(SchemaSymbols::fgURI_SCHEMAFORSCHEMA);

        for (unsigned int i=0; i < elemCount; i++) {

            SchemaGrammar*     aGrammar = fSchemaGrammar;
            SchemaElementDecl* elemDecl = baseTypeInfo->elementAt(i);

            if (!elemDecl) {
                continue;
            }

            int elemURI = elemDecl->getURI();

            if (elemURI != fTargetNSURI && elemURI != schemaURI && elemURI != fEmptyNamespaceURI) {

                Grammar* aGrammar =
                    fGrammarResolver->getGrammar(fURIStringPool->getValueForId(elemURI));

                if (!aGrammar || aGrammar->getGrammarType() != Grammar::SchemaGrammarType) {
                    continue; // REVISIT - error message
                }
            }

            const XMLCh*             localPart = elemDecl->getBaseName();
            const SchemaElementDecl* other = (SchemaElementDecl*)
                aGrammar->getElemDecl(elemURI, localPart, 0, newTypeScope);

            if (other) {

                if (elemDecl->getComplexTypeInfo() != other->getComplexTypeInfo()
                    || elemDecl->getDatatypeValidator() != other->getDatatypeValidator()) {
                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateElementDeclaration, localPart);
                }
            }
            else {

                int elemScope = elemDecl->getEnclosingScope();

                newTypeInfo->addElement(elemDecl);
                elemDecl->setEnclosingScope(newTypeScope);
                ((SchemaGrammar*) aGrammar)->putGroupElemDecl(elemDecl);
                elemDecl->setEnclosingScope(elemScope);
            }
        }
    }
}


void TraverseSchema::copyGroupElements(XercesGroupInfo* const fromGroup,
                                       XercesGroupInfo* const toGroup,
                                       ComplexTypeInfo* const typeInfo) {

    unsigned int elemCount = fromGroup->elementCount();
    int newScope = (typeInfo) ? typeInfo->getScopeDefined() : 0;

    for (unsigned int i = 0; i < elemCount; i++) {

        SchemaElementDecl*       elemDecl = fromGroup->elementAt(i);

        if (typeInfo) {
            int                      elemURI = elemDecl->getURI();
            const XMLCh*             localPart = elemDecl->getBaseName();
            const SchemaElementDecl* other = (SchemaElementDecl*)
                    fSchemaGrammar->getElemDecl(elemURI, localPart, 0, fCurrentScope);

            if (other) {

                if (elemDecl->getComplexTypeInfo() != other->getComplexTypeInfo()
                    || elemDecl->getDatatypeValidator() != other->getDatatypeValidator()) {
                   reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateElementDeclaration, localPart);
                }

                continue;
            }

            int elemScope = elemDecl->getEnclosingScope();

            elemDecl->setEnclosingScope(newScope);
            typeInfo->addElement(elemDecl);
            fSchemaGrammar->putGroupElemDecl(elemDecl);
            elemDecl->setEnclosingScope(elemScope);

            if (toGroup) {
                toGroup->addElement(elemDecl);
            }
        }
        else {
            if (!toGroup->containsElement(elemDecl)) {
                toGroup->addElement(elemDecl);
            }
        }
    }
}

void TraverseSchema::copyAttGroupAttributes(XercesAttGroupInfo* const fromAttGroup,
                                            XercesAttGroupInfo* const toAttGroup,
                                            ComplexTypeInfo* const typeInfo) {

    unsigned int attCount = fromAttGroup->attributeCount();

    for (unsigned int i=0; i < attCount; i++) {

        SchemaAttDef* attDef = fromAttGroup->attributeAt(i);
        QName* attName = attDef->getAttName();
        const XMLCh* localPart = attName->getLocalPart();

        if (typeInfo) {

            if (typeInfo->getAttDef(localPart, attName->getURI())) {

                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateAttribute, localPart);
                continue;
            }

            typeInfo->addAttDef(new SchemaAttDef(attDef));

            if (toAttGroup) {
                toAttGroup->addAttDef(attDef, true);
            }
        }
        else {

            if (toAttGroup->containsAttribute(localPart, attName->getURI())) {

                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateAttribute, localPart);
                continue;
            }

            toAttGroup->addAttDef(attDef, true);
        }
    }

    if (toAttGroup) {
        unsigned int anyAttCount = fromAttGroup->anyAttributeCount();

        for (unsigned int j=0; j < anyAttCount; j++) {
            toAttGroup->addAnyAttDef(fromAttGroup->anyAttributeAt(j), true);
        }
    }
}

void
TraverseSchema::attWildCardIntersection(SchemaAttDef* const resultWildCard,
                                        const SchemaAttDef* const compareWildCard) {

    XMLAttDef::AttTypes typeR = resultWildCard->getType();
    XMLAttDef::AttTypes typeC = compareWildCard->getType();

    //If either O1 or O2 is any, then the other must be the value.
    if (typeC == XMLAttDef::Any_Any ||
        typeR == XMLAttDef::AttTypes_Unknown) {
        return;
    }

    if (typeR == XMLAttDef::Any_Any ||
        typeC == XMLAttDef::AttTypes_Unknown) {

        resultWildCard->resetNamespaceList();
        copyWildCardData(compareWildCard, resultWildCard);
        return;
    }

    // If either O1 or O2 is a pair of not and a namespace name and the other
    // is a set, then that set, minus the negated namespace name if it was in
    // is the value
    if ((typeC == XMLAttDef::Any_Other && typeR == XMLAttDef::Any_List) ||
        (typeR == XMLAttDef::Any_Other && typeC == XMLAttDef::Any_List)) {

		unsigned int compareURI = 0;
        ValueVectorOf<unsigned int>* nameURIList = 0;

        if (typeC == XMLAttDef::Any_List) {
            nameURIList = compareWildCard->getNamespaceList();
            compareURI = resultWildCard->getAttName()->getURI();
        }
        else {
            nameURIList = resultWildCard->getNamespaceList();
            compareURI = compareWildCard->getAttName()->getURI();
        }

        unsigned int listSize = (nameURIList) ? nameURIList->size() : 0;

        if (listSize) {

            bool                        found = false;
            ValueVectorOf<unsigned int> tmpURIList(listSize);

            for (unsigned int i=0; i < listSize; i++) {

                unsigned int nameURI = nameURIList->elementAt(i);

                if (nameURI != compareURI) {
                    tmpURIList.addElement(nameURI);
                }
                else {
                    found = true;
                }
            }

            if (found || typeC == XMLAttDef::Any_List) {
                resultWildCard->setNamespaceList(&tmpURIList);
            }
        }

        if (typeC == XMLAttDef::Any_List) {
            copyWildCardData(compareWildCard, resultWildCard);
        }

        return;
    }

    // If both O1 and O2 are sets, then the intersection of those sets must be
    // the value.
    if (typeR == XMLAttDef::Any_List && typeC == XMLAttDef::Any_List) {
		
        ValueVectorOf<unsigned int>* uriListR = resultWildCard->getNamespaceList();
        ValueVectorOf<unsigned int>* uriListC = compareWildCard->getNamespaceList();
        unsigned int listSize = (uriListC) ? uriListC->size() : 0;

        if (listSize) {

            ValueVectorOf<unsigned int> tmpURIList(listSize);

            for (unsigned int i=0; i < listSize; i++) {

                unsigned int uriName = uriListC->elementAt(i);

                if (uriListR && uriListR->containsElement(uriName)) {
                    tmpURIList.addElement(uriName);
                }
            }

            resultWildCard->setNamespaceList(&tmpURIList);
        }
        else {
            resultWildCard->resetNamespaceList();
        }

        return;
    }

    // If the two are negations of different namespace names, then the
    // intersection is not expressible.
    if (typeR == XMLAttDef::Any_Other && typeC == XMLAttDef::Any_Other) {

        QName* qnameR = resultWildCard->getAttName();

        if (qnameR->getURI() != compareWildCard->getAttName()->getURI()) {

            qnameR->setURI(fEmptyNamespaceURI);
            resultWildCard->setType(XMLAttDef::AttTypes_Unknown);
        }
    }
}


void
TraverseSchema::attWildCardUnion(SchemaAttDef* const resultWildCard,
                                 const SchemaAttDef* const compareWildCard) {

    XMLAttDef::AttTypes typeR = resultWildCard->getType();
    XMLAttDef::AttTypes typeC = compareWildCard->getType();

    //If either O1 or O2 is any, then the other must be the value.
    if (typeR == XMLAttDef::Any_Any ||
        typeR == XMLAttDef::AttTypes_Unknown) {
        return;
    }

    if (typeC == XMLAttDef::Any_Any ||
        typeC == XMLAttDef::AttTypes_Unknown) {

        resultWildCard->resetNamespaceList();
        copyWildCardData(compareWildCard, resultWildCard);
        return;
    }

    // If both O1 and O2 are sets, then the union of those sets must be
    // the value.
    if (typeR == XMLAttDef::Any_List && typeC == XMLAttDef::Any_List) {

        ValueVectorOf<unsigned int>* uriListR = resultWildCard->getNamespaceList();
        ValueVectorOf<unsigned int>* uriListC = compareWildCard->getNamespaceList();
        unsigned int listSizeC = (uriListC) ? uriListC->size() : 0;

        if (listSizeC) {

            if (!uriListR || !uriListR->size()) {

                resultWildCard->setNamespaceList(uriListC);
                return;
            }

            ValueVectorOf<unsigned int> tmpURIList(*uriListR);

            for (unsigned int i = 0; i < listSizeC; i++) {

                unsigned int uriName = uriListC->elementAt(i);

                if (!uriListR->containsElement(uriName)) {
                    tmpURIList.addElement(uriName);
                }
            }

            resultWildCard->setNamespaceList(&tmpURIList);
        }

        return;
    }

    // If the two are negations of different namespace names, then any must
    // be the value
    if (typeR == XMLAttDef::Any_Other && typeC == XMLAttDef::Any_Other) {

        QName* qnameR = resultWildCard->getAttName();

        if (qnameR->getURI() != compareWildCard->getAttName()->getURI()) {

            qnameR->setURI(fEmptyNamespaceURI);
            resultWildCard->setType(XMLAttDef::Any_Any);
        }
    }

    // If either O1 or O2 is a pair of not and a namespace name and the other
    // is a set, then:
    //   1. If the set includes the negated namespace name, then any must be the value.
    //   2. If the set does not include the negated namespace name, then whichever of O1 or O2 is a
    //      pair of not and a namespace name must be the value.
    if ((typeC == XMLAttDef::Any_Other && typeR == XMLAttDef::Any_List) ||
		(typeR == XMLAttDef::Any_Other && typeC == XMLAttDef::Any_List)) {

        ValueVectorOf<unsigned int>* nameURIList = 0;
        QName* attNameR = resultWildCard->getAttName();
        unsigned int compareURI = 0;

        if (typeC == XMLAttDef::Any_List) {
            nameURIList = compareWildCard->getNamespaceList();
            compareURI = attNameR->getURI();
        }
        else {
            nameURIList = resultWildCard->getNamespaceList();
            compareURI = compareWildCard->getAttName()->getURI();
        }


        if (nameURIList && nameURIList->containsElement(compareURI)) {

            resultWildCard->setType(XMLAttDef::Any_Any);
            attNameR->setURI(fEmptyNamespaceURI);
        }
        else if (typeR == XMLAttDef::Any_List) {

            resultWildCard->setType(XMLAttDef::Any_Other);
            attNameR->setURI(compareURI);
        }

        resultWildCard->resetNamespaceList();
    }
}


void TraverseSchema::checkAttDerivationOK(const ComplexTypeInfo* const baseTypeInfo,
                                          const ComplexTypeInfo* const childTypeInfo) {

    SchemaAttDefList& childAttList = (SchemaAttDefList&) childTypeInfo->getAttDefList();
    const SchemaAttDef* baseAttWildCard = baseTypeInfo->getAttWildCard();

    while (childAttList.hasMoreElements()) {

        SchemaAttDef& childAttDef = (SchemaAttDef&) childAttList.nextElement();
        QName* childAttName = childAttDef.getAttName();
        const XMLCh* childLocalPart = childAttName->getLocalPart();
        const SchemaAttDef* baseAttDef = baseTypeInfo->getAttDef(childLocalPart, childAttName->getURI());

        if (baseAttDef) {

            XMLAttDef::DefAttTypes baseAttDefType = baseAttDef->getDefaultType();
            XMLAttDef::DefAttTypes childAttDefType = childAttDef.getDefaultType();

            // Constraint 2.1.1 & 3
            if ((baseAttDefType & XMLAttDef::Required)
                && !(childAttDefType & XMLAttDef::Required)) {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::BadAttDerivation_2, childLocalPart);
            }

            // Constraint 2.1.2
            DatatypeValidator* baseDV = baseAttDef->getDatatypeValidator();
            DatatypeValidator* childDV = childAttDef.getDatatypeValidator();
            if (!baseDV || !baseDV->isSubstitutableBy(childDV)) {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::BadAttDerivation_3, childLocalPart);
            }

            // Constraint 2.1.3
            if ((baseAttDefType & XMLAttDef::Fixed) &&
                (!(childAttDefType & XMLAttDef::Fixed) ||
                 XMLString::compareString(baseAttDef->getValue(), childAttDef.getValue()))) {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::BadAttDerivation_4, childLocalPart);
            }
        }
        // Constraint 2.2
        else if (!baseAttWildCard ||
                 !wildcardAllowsNamespace(baseAttWildCard, childAttName->getURI())) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::BadAttDerivation_5, childLocalPart);
        }
    }

    // Constraint 4
    const SchemaAttDef* childAttWildCard = childTypeInfo->getAttWildCard();

    if (childAttWildCard) {

        if (!baseAttWildCard) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::BadAttDerivation_6);
        }
        else if (!isWildCardSubset(baseAttWildCard, childAttWildCard)) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::BadAttDerivation_7);
        }
    }
}

bool TraverseSchema::wildcardAllowsNamespace(const SchemaAttDef* const wildCard,
                                             const unsigned int nameURI) {

    XMLAttDef::AttTypes wildCardType = wildCard->getType();

    // The constraint must be any
    if (wildCardType == XMLAttDef::Any_Any) {
        return true;
    }

    // All of the following must be true:
    //    2.1 The constraint is a pair of not and a namespace name or ·absent
    //    2.2 The value must not be identical to the ·namespace test·.
    //    2.3 The value must not be ·absent·.
    if (wildCardType == XMLAttDef::Any_Other &&
		((int) nameURI) != fEmptyNamespaceURI &&
        wildCard->getAttName()->getURI() != nameURI) {
        return true;
    }

    // The constraint is a set, and the value is identical to one of the
    // members of the set
    if (wildCardType == XMLAttDef::Any_List) {

        ValueVectorOf<unsigned int>* nameURIList = wildCard->getNamespaceList();		

        if (nameURIList->containsElement(nameURI)) {
            return true;
        }
    }

    return false;
}

bool TraverseSchema::isWildCardSubset(const SchemaAttDef* const baseAttWildCard,
                                      const SchemaAttDef* const childAttWildCard) {

    XMLAttDef::AttTypes baseWildCardType = baseAttWildCard->getType();
    XMLAttDef::AttTypes childWildCardType = childAttWildCard->getType();

    if (baseWildCardType == XMLAttDef::AttTypes_Unknown ||
        childWildCardType == XMLAttDef::AttTypes_Unknown) {
        return false;
    }

    // 1 super must be any.
    if (baseWildCardType == XMLAttDef::Any_Any) {
        return true;
    }

    // 2 All of the following must be true:
    //     2.1 sub must be a pair of not and a namespace name or ·absent·.
    //     2.2 super must be a pair of not and the same value.
    if (childWildCardType == XMLAttDef::Any_Other && baseWildCardType == XMLAttDef::Any_Other &&
        childAttWildCard->getAttName()->getURI() == baseAttWildCard->getAttName()->getURI()) {
        return true;
    }

    // 3 All of the following must be true:
    //     3.1 sub must be a set whose members are either namespace names or ·absent·.
    //     3.2 One of the following must be true:
    //          3.2.1 super must be the same set or a superset thereof.
    //          3.2.2 super must be a pair of not and a namespace name or ·absent· and
    //                 that value must not be in sub's set.
    if (childWildCardType == XMLAttDef::Any_List) {

        ValueVectorOf<unsigned int>* childURIList = childAttWildCard->getNamespaceList();

		if (baseWildCardType == XMLAttDef::Any_List) {

            ValueVectorOf<unsigned int>* baseURIList = baseAttWildCard->getNamespaceList();
            unsigned int childListSize = (childURIList) ? childURIList->size() : 0;

            for (unsigned int i=0; i<childListSize; i++) {
                if (!baseURIList->containsElement(childURIList->elementAt(i))) {
                    return false;
                }
            }

            return true;
        }
        else if (baseWildCardType == XMLAttDef::Any_Other) {
            if (!childURIList->containsElement(baseAttWildCard->getAttName()->getURI())) {
                return true;
            }
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
//  TraverseSchema: Error reporting methods
// ---------------------------------------------------------------------------
void TraverseSchema::reportSchemaError(const XMLCh* const msgDomain,
                                       const int errorCode) {

    if (fScanner && XMLString::compareString(msgDomain, XMLUni::fgXMLErrDomain) == 0) {
        fScanner->emitError((XMLErrs::Codes) errorCode);
    }
    else if (fValidator && fScanner && fScanner->getDoValidation()
             && XMLString::compareString(msgDomain, XMLUni::fgValidityDomain) == 0) {
        fValidator->emitError((XMLValid::Codes) errorCode);
    }
}

void TraverseSchema::reportSchemaError(const XMLCh* const msgDomain,
                                       const int errorCode,
                                       const XMLCh* const text1,
                                       const XMLCh* const text2,
                                       const XMLCh* const text3,
                                       const XMLCh* const text4) {

    if (fScanner && XMLString::compareString(msgDomain, XMLUni::fgXMLErrDomain) == 0) {
        fScanner->emitError((XMLErrs::Codes) errorCode,text1,text2,text3,text4);
    }
    else if (fValidator && fScanner && fScanner->getDoValidation()
             && XMLString::compareString(msgDomain, XMLUni::fgValidityDomain) == 0) {
        fValidator->emitError((XMLValid::Codes) errorCode,text1,text2,text3,text4);
    }
}

// ---------------------------------------------------------------------------
//  TraverseSchema: CleanUp methods
// ---------------------------------------------------------------------------
void TraverseSchema::cleanUp() {

    delete [] fTargetNSURIString;
    delete [] fCurrentSchemaURL;
    delete fSchemaInfoRoot;
    delete fIncludeLocations;
    delete fCurrentTypeNameStack;
    delete fCurrentGroupStack;
    delete fGlobalTypes;
    delete fGlobalGroups;
    delete fGlobalAttGroups;
    delete fSubstitutionGroups;
    delete fRefElements;
    delete fRefElemScope;

    if (fAdoptImportLocations) {
        delete fImportLocations;
    }
}

/**
  * End of file TraverseSchema.cpp
  */

