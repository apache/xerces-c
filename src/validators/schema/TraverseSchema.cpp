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
 * Revision 1.4  2001/05/11 13:27:38  tng
 * Copyright update.
 *
 * Revision 1.3  2001/05/10 17:49:44  tng
 * Schema: SchemaValidator fixes
 *
 * Revision 1.2  2001/05/10 16:33:17  knoaman
 * Traverse Schema Part III + error messages.
 *
 * Revision 1.1  2001/05/03 19:18:06  knoaman
 * TraverseSchema Part II.
 *
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
#include <util/StringTokenizer.hpp>
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
#include <framework/XMLValidator.hpp>
#include <sax/EntityResolver.hpp>
#include <util/XMLURL.hpp>
#include <sax/InputSource.hpp>
#include <framework/LocalFileInputSource.hpp>
#include <framework/URLInputSource.hpp>
#include <parsers/DOMParser.hpp>
#include <dom/DOM_DOMException.hpp>
#include <validators/schema/SchemaInfo.hpp>

// ---------------------------------------------------------------------------
//  TraverseSchema: Local declaration
// ---------------------------------------------------------------------------
typedef RefVectorOf<DatatypeValidator> DVRefVector;


// ---------------------------------------------------------------------------
//  TraverseSchema: Static member data
// ---------------------------------------------------------------------------
XMLStringPool TraverseSchema::fStringPool;

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

// ---------------------------------------------------------------------------
//  TraverseSchema: Constructors and Destructor
// ---------------------------------------------------------------------------
TraverseSchema::TraverseSchema( const DOM_Element&      schemaRoot
                              , XMLStringPool* const    uriStringPool
                              , SchemaGrammar* const    schemaGrammar
                              , GrammarResolver* const  grammarResolver
                              , XMLScanner* const       xmlScanner
                              , XMLValidator* const     xmlValidator
                              , const XMLCh* const      schemaURL
                              , EntityResolver* const   entityResolver
                              , ErrorHandler* const     errorHandler)
    : fElementDefaultQualified(false)
    , fAttributeDefaultQualified(false)
    , fTargetNSURI(-1)
    , fCurrentScope(Grammar::TOP_LEVEL_SCOPE)
    , fSimpleTypeAnonCount(0)
    , fComplexTypeAnonCount(0)
    , fFinalDefault(0)
    , fBlockDefault(0)
    , fScopeCount(0)
    , fSchemaRootElement(schemaRoot)
    , fTargetNSURIString(0)
    , fCurrentSchemaURL(XMLString::replicate(schemaURL))
    , fDatatypeRegistry(0)
    , fGrammarResolver(grammarResolver)
    , fSchemaGrammar(schemaGrammar)
    , fEntityResolver(entityResolver)
    , fErrorHandler(errorHandler)
    , fURIStringPool(uriStringPool)
    , fValidator(xmlValidator)
    , fScanner(xmlScanner)
    , fNamespaceScope(0)
    , fAttributeDeclRegistry(0)
    , fComplexTypeRegistry(0)
    , fSchemaInfoRoot(0)
    , fCurrentSchemaInfo(0)
    , fImportLocations(0)
    , fIncludeLocations(0)
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

	if (fSchemaRootElement.isNull()) {
        // REVISIT: Anything to do?
        return;
    }

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
    }
    else{

        // for complex type registry, attribute decl registry and
        // namespace mapping, needs to check whether the passed in
        // Grammar was a newly instantiated one.
        fComplexTypeRegistry = fSchemaGrammar->getComplexTypeRegistry();

        if (fComplexTypeRegistry == 0 ) {

            fComplexTypeRegistry = new RefHashTableOf<ComplexTypeInfo>(109);
            fSchemaGrammar->setComplexTypeRegistry(fComplexTypeRegistry);
        }

        fAttributeDeclRegistry = fSchemaGrammar->getAttributeDeclRegistry();

        if (fAttributeDeclRegistry == 0) {

            fAttributeDeclRegistry = new RefHashTableOf<XMLAttDef>(29, true);
            fSchemaGrammar->setAttributeDeclRegistry(fAttributeDeclRegistry);
        }

        fNamespaceScope = fSchemaGrammar->getNamespaceScope();

        if (fNamespaceScope == 0) {

            fNamespaceScope = new NamespaceScope();
            fNamespaceScope->reset(fURIStringPool->addOrFind(XMLUni::fgZeroLenString));
            fNamespaceScope->increaseDepth();
            fSchemaGrammar->setNamespaceScope(fNamespaceScope);
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
}


void TraverseSchema::traverseSchemaHeader() {

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
    fBlockDefault = defaultVal != 0 ? parseBlockSet(defaultVal) : 0;
    fFinalDefault = finalVal != 0 ? parseFinalSet(finalVal) : 0;

    fCurrentScope = Grammar::TOP_LEVEL_SCOPE;
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

    if (fIncludeLocations == 0) {
        fIncludeLocations = new ValueVectorOf<unsigned int>(8);
    }

    if (locationsContain(fIncludeLocations, locationId)) {
        return;
    }

    fIncludeLocations->addElement(locationId);

    // ------------------------------------------------------------------
	// Parse input source
    // ------------------------------------------------------------------
    DOMParser parser;

    parser.setValidationScheme(DOMParser::Val_Never);
    parser.setDoNamespaces(true);
    parser.setErrorHandler(fErrorHandler);
    parser.setEntityResolver(fEntityResolver);

    try {
        parser.parse(*srcToFill);
    }
    catch (const XMLException& e) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::XMLException,
                          e.getType(), e.getMessage());
    }
    catch (const DOM_DOMException& e) {
        throw e;
    }
    catch (...) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::UnexpectedError);
        throw;
    }

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
                                       fCurrentScope,
                                       fCurrentSchemaURL,
                                       fSchemaRootElement,
									   0, 0);
                    fCurrentSchemaInfo = fSchemaInfoRoot;
                }

                // --------------------------------------------------------
                // Update schema information with included schema
                // --------------------------------------------------------
                fSchemaRootElement = root;
                setCurrentSchemaURL(includeURL);
                traverseSchemaHeader();

                // and now we'd better save this stuff!
                fCurrentSchemaInfo =
					new SchemaInfo(fElementDefaultQualified,
                                   fAttributeDefaultQualified,
                                   fBlockDefault,
                                   fFinalDefault,
                                   fCurrentScope,
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
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ImportNoSchemaLocation);
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

    if (fImportLocations == 0) {
        fImportLocations = new ValueVectorOf<unsigned int>(8);
    }

    if (locationsContain(fImportLocations, locationId)) {
        return;
    }

    fImportLocations->addElement(locationId);

    const XMLCh* nameSpace = getElementAttValue(elem, SchemaSymbols::fgATT_NAMESPACE);
    SchemaGrammar* importedGrammar = (SchemaGrammar*) fGrammarResolver->getGrammar(nameSpace);

    if (importedGrammar == 0) {
        importedGrammar = new SchemaGrammar();
    }

    // ------------------------------------------------------------------
	// Parse input source
    // ------------------------------------------------------------------
    DOMParser parser;

    parser.setValidationScheme(DOMParser::Val_Never);
    parser.setDoNamespaces(true);
    parser.setErrorHandler(fErrorHandler);
    parser.setEntityResolver(fEntityResolver);

    try {
        parser.parse(*srcToFill);
    }
    catch (const XMLException& e) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::XMLException,
                          e.getType(), e.getMessage());
    }
    catch (const DOM_DOMException& e) {
        throw e;
    }
    catch (...) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::UnexpectedError);
        throw;
    }

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
                TraverseSchema traverseSchema(root, fURIStringPool, importedGrammar,
                                              fGrammarResolver, fScanner, fValidator,
											  importURL, fEntityResolver, fErrorHandler);
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
                                       const int modelGroupType)
{
    DOM_Element child = checkContent(elem, XUtil::getFirstChildElement(elem), true);
    ContentSpecNode* left = 0;
    ContentSpecNode* right = 0;
    bool hadContent = false;

    for (; child != 0; child = XUtil::getNextSiblingElement(child)) {

        ContentSpecNode* contentSpecNode = 0;
	    bool seeParticle = false;
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

            contentSpecNode = 0/*traverseGroupDecl(child)*/;

            if (contentSpecNode == 0)
                continue;

            seeParticle = true;
        }
        else if (childName.equals(SchemaSymbols::fgELT_CHOICE)) {

            contentSpecNode =
                traverseChoiceSequence(child,ContentSpecNode::Choice);
            seeParticle = true;
        }
        else if (childName.equals(SchemaSymbols::fgELT_SEQUENCE)) {
			contentSpecNode =
                traverseChoiceSequence(child,ContentSpecNode::Sequence);
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
            contentSpecNode = expandContentModel(contentSpecNode, child);
        }

        if (left == 0) {
            left = contentSpecNode;
        }
        else if (right == 0) {
            right = contentSpecNode;
        }
        else {
            left = new ContentSpecNode((ContentSpecNode::NodeTypes) modelGroupType,
                                       left, right);
            right = contentSpecNode;
        }
    }

    if (hadContent && right != 0) {
            left = new ContentSpecNode((ContentSpecNode::NodeTypes) modelGroupType,
                                       left, right);
    }

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
int TraverseSchema::traverseSimpleTypeDecl(const DOM_Element& childElem)
{
    const XMLCh* name = getElementAttValue(childElem,SchemaSymbols::fgATT_NAME);

    if (XMLString::stringLen(fTargetNSURIString) != 0) {

		fBuffer.set(fTargetNSURIString);
        fBuffer.append(chComma);
        fBuffer.append(name);
    }
	else {
        fBuffer.set(name);
    }

    //check if we have already traversed the same simpleType decl
    if (fDatatypeRegistry->getDatatypeValidator(fBuffer.getRawBuffer())!= 0) {
        return fStringPool.addOrFind(fBuffer.getRawBuffer());
    }

    int newSimpleTypeName = -1;

    if (XMLString::stringLen(name) == 0) { // anonymous simpleType
        name = genAnonTypeName(fgAnonSNamePrefix, fSimpleTypeAnonCount);
    }

    newSimpleTypeName = fStringPool.addOrFind(name);

    // Get 'final' values
    const XMLCh* finalVal = getElementAttValue(childElem, SchemaSymbols::fgATT_FINAL);
    int finalSet = parseFinalSet(finalVal);

	// annotation?,(list|restriction|union)
    DOM_Element content= checkContent(childElem,
                                      XUtil::getFirstChildElement(childElem),
                                      false);

    if (content == 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::EmptySimpleTypeContent);
        return -1;
    }

    DOMString varietyName = content.getLocalName();

    // Remark: some code will be repeated in list|restriction| union but it
    //         is cleaner that way
    if (varietyName.equals(SchemaSymbols::fgELT_LIST)) { //traverse List
		return traverseByList(childElem, content, newSimpleTypeName, finalSet);
    }
    else if (varietyName.equals(SchemaSymbols::fgELT_RESTRICTION)) { //traverse Restriction
        return traverseByRestriction(childElem, content, newSimpleTypeName, finalSet);
    }
    else if (varietyName.equals(SchemaSymbols::fgELT_UNION)) { //traverse union
		return traverseByUnion(childElem, content, newSimpleTypeName, finalSet);
    }
    else {

        fBuffer.set(varietyName.rawBuffer(), varietyName.length());
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::FeatureUnsupported,fBuffer.getRawBuffer());
    }

    return -1;
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
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidComplexTypeName, name);
        return -1;
    }

//    fCurrentTypeNameStack->push(XMLString::replicate(name));

    // ------------------------------------------------------------------
    // Check if the type has already been registered
    // ------------------------------------------------------------------
    fBuffer.set(fTargetNSURIString);
    fBuffer.append(chComma);
    fBuffer.append(name);

    if (topLevel) {

        ComplexTypeInfo* temp = fComplexTypeRegistry->get(fBuffer.getRawBuffer());

        if (temp != 0 ) {
            return fStringPool.addOrFind(fBuffer.getRawBuffer());
        }
    }

	ComplexTypeInfo* typeInfo = new ComplexTypeInfo();
    int typeNameIndex = fStringPool.addOrFind(fBuffer.getRawBuffer());
    int scopeDefined = fScopeCount++;
    int previousScope = fCurrentScope;
    fCurrentScope = scopeDefined;

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
    // complex type information has not been added).
    fComplexTypeRegistry->put((void*) fStringPool.getValueForId(typeNameIndex),
                              typeInfo);
    typeInfo->setTypeName(fBuffer.getRawBuffer());

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
			
			if (XMLString::stringLen(mixedVal) != 0
				&& XMLString::compareString(SchemaSymbols::fgATTVAL_TRUE, mixedVal) == 0) {
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
    catch(...) {
        defaultComplexTypeInfo(typeInfo);
    }

    // ------------------------------------------------------------------
    // Finish the setup of the typeInfo
    // ------------------------------------------------------------------

    const XMLCh* lBlock = getElementAttValue(elem, SchemaSymbols::fgATT_BLOCK);
    const XMLCh* lFinal = getElementAttValue(elem, SchemaSymbols::fgATT_FINAL);
    const XMLCh* lAbstract = getElementAttValue(elem, SchemaSymbols::fgATT_ABSTRACT);
    int blockSet = lBlock != 0 ? parseBlockSet(lBlock) : fBlockDefault;
    int finalSet = lFinal != 0 ? parseFinalSet(lFinal) : fFinalDefault;
    int finalBlockValid = SchemaSymbols::RESTRICTION + SchemaSymbols::EXTENSION;

    typeInfo->setBlockSet(blockSet);
    typeInfo->setFinalSet(finalSet);
    typeInfo->setScopeDefined(scopeDefined);

    if (XMLString::stringLen(lBlock) != 0
        && XMLString::compareString(lBlock,SchemaSymbols::fgATTVAL_POUNDALL) != 0
        && ((blockSet & finalBlockValid) == 0)) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidComplexTypeBlockValue, lBlock);
    }

    if (XMLString::stringLen(lFinal) != 0
        && XMLString::compareString(lFinal,SchemaSymbols::fgATTVAL_POUNDALL) != 0
        && ((finalSet & finalBlockValid) == 0)) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidComplexTypeFinalValue, lFinal);
    }

	if (XMLString::stringLen(lAbstract) != 0
        && XMLString::compareString(lAbstract, SchemaSymbols::fgATTVAL_TRUE) == 0) {
        typeInfo->setAbstract(true);
    }
    else {
        typeInfo->setAbstract(false);
    }

    // ------------------------------------------------------------------
    // Before exiting, restore the scope, mainly for nested anonymous types
    // ------------------------------------------------------------------
    fCurrentScope = previousScope;
    //ArrayJanitor<XMLCh> janTypeName(fCurrentTypeNameStack->pop());

    return typeNameIndex;
}

ContentSpecNode*
TraverseSchema::traverseGroupDecl(const DOM_Element& childElem) {

    // TO DO
    return 0;
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
    ContentSpecNode::NodeTypes anyLocalType = ContentSpecNode::Any_Local;
    ContentSpecNode::NodeTypes anyOtherType = ContentSpecNode::Any_Other;

    if (XMLString::stringLen(processContents) != 0) {

        if (XMLString::compareString(processContents, fgStrict) != 0) {
            // Do nothing
        }
        else if (XMLString::compareString(processContents, fgLax) == 0) {

            anyType = ContentSpecNode::Any_Lax;
            anyOtherType = ContentSpecNode::Any_Other_Lax;
            anyLocalType = ContentSpecNode::Any_Local_Lax;
        }
        else if (XMLString::compareString(processContents, fgSkip) == 0) {

            anyType = ContentSpecNode::Any_Skip;
            anyOtherType = ContentSpecNode::Any_Other_Skip;
            anyLocalType = ContentSpecNode::Any_Local_Skip;
        }
        else {
            reportSchemaError(0, 0, processContents); //"Invalid 'processContents' value: '{0}'
        }
    }

    // ------------------------------------------------------------------
    // Process 'namespace' attribute
    // ------------------------------------------------------------------
    int emptyURI = fURIStringPool->addOrFind(XMLUni::fgZeroLenString);
    ContentSpecNode* retSpecNode = 0;
	QName elemName(XMLUni::fgZeroLenString, XMLUni::fgZeroLenString,
                   fTargetNSURI);

    if (XMLString::stringLen(nameSpace) == 0
		|| XMLString::compareString(nameSpace, SchemaSymbols::fgATTVAL_TWOPOUNDANY) == 0) {

		retSpecNode = new ContentSpecNode(&elemName);
        retSpecNode->setType(anyType);
    }
    else if (XMLString::compareString(nameSpace, SchemaSymbols::fgATTVAL_TWOPOUNDOTHER) == 0) {

		retSpecNode = new ContentSpecNode(&elemName);
        retSpecNode->setType(anyOtherType);
    }
    else {

        RefVectorOf<XMLCh>* nameSpaceTokens = XMLString::tokenizeString(nameSpace);
        ContentSpecNode* firstNode = 0;
        ContentSpecNode* secondNode = 0;
        unsigned int tokensSize = nameSpaceTokens->size();

        for (unsigned int i=0; i < tokensSize; i++) {

            const XMLCh* tokenElem = nameSpaceTokens->elementAt(i);
            int uriIndex = emptyURI;

            if (XMLString::compareString(tokenElem,
                        SchemaSymbols::fgATTVAL_TWOPOUNDLOCAL) == 0) {

                elemName.setURI(uriIndex);
                firstNode = new ContentSpecNode(&elemName);
                firstNode->setType(anyLocalType);
            }
            else {

                if (XMLString::compareString(tokenElem,
					    SchemaSymbols::fgATTVAL_TWOPOUNDTRAGETNAMESPACE) == 0) {
                    uriIndex = fTargetNSURI;
                }

                uriIndex = fURIStringPool->addOrFind(tokenElem);
                elemName.setURI(uriIndex);
                firstNode = new ContentSpecNode(&elemName);
                firstNode->setType(anyType);
            }

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
            reportSchemaError(0, 0, fBuffer.getRawBuffer()); //Content of all group is restricted to elements only. '{0}' encountered and ignored."
        }

        if (seeParticle) {
            contentSpecNode = expandContentModel(contentSpecNode, child);
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

    bool         topLevel = isTopLevelComponent(elem);
    const XMLCh* name = getElementAttValue(elem, SchemaSymbols::fgATT_NAME);
    const XMLCh* ref = getElementAttValue(elem, SchemaSymbols::fgATT_REF);
    const XMLCh* defaultVal = getElementAttValue(elem, SchemaSymbols::fgATT_DEFAULT);
    const XMLCh* fixedVal = getElementAttValue(elem, SchemaSymbols::fgATT_FIXED);
    const XMLCh* useVal = getElementAttValue(elem, SchemaSymbols::fgATT_USE);
    bool         nameEmpty = (XMLString::stringLen(name) == 0) ? true : false;
    bool         refEmpty = (XMLString::stringLen(ref) == 0) ? true : false;
    DOM_Element  simpleType = checkContent(elem, XUtil::getFirstChildElement(elem), true);

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

    if (nameEmpty && refEmpty) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NoNameRefAttribute);
        return;
    }

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

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidAttributeName, name);
        return;
    }

    // Check for duplicate declaration
    // REVISIT - need to add attributeGroup check
    const XMLCh* attForm = getElementAttValue(elem, SchemaSymbols::fgATT_FORM);
    const XMLCh* qualified = SchemaSymbols::fgATTVAL_QUALIFIED;
    int uriIndex = fURIStringPool->addOrFind(XMLUni::fgZeroLenString);

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
    else if (typeInfo != 0 && typeInfo->getAttDef(name, uriIndex) != 0) {

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
            dv = fDatatypeRegistry->getDatatypeValidator(
			                       fStringPool.getValueForId(datatypeSymbol));
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

        if (XMLString::stringLen(typeURI) == 0
            || XMLString::compareString(typeURI, SchemaSymbols::fgURI_SCHEMAFORSCHEMA)  == 0) {

            dv = getDatatypeValidator(XMLUni::fgZeroLenString, localPart);

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
                attType = XMLAttDef::NmToken;
            }
            else if (XMLString::compareString(localPart,XMLUni::fgNmTokensString) == 0) {
                attType = XMLAttDef::NmTokens;
            }
            else if (XMLString::compareString(localPart,XMLUni::fgNotationString) == 0) {
                attType = XMLAttDef::Notation;
            }
            else {

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
        catch(...) {
			reportSchemaError(XMLUni::fgXMLErrDomain,
                              XMLErrs::DatatypeValidationFailure, valueToCheck);
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
    else if (typeInfo != 0) {
        typeInfo->addAttDef(attDef);
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

    if(XMLString::stringLen(fixed) != 0 && XMLString::stringLen(deflt) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ElementWithFixedAndDefault);
    }

    if (nameEmpty || (!refEmpty && !topLevel)) {

		if (!nameEmpty) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ElementWithNameRef, name);
        }

        if (!isValidRefDeclaration(elem)) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::BadAttWithRef);
        }

        return processElementDeclRef(elem, ref);
    }

    // Name is notEmpty
	if (!XMLString::isValidNCName(name)) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidElementName, name);
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
	SchemaElementDecl* elemDecl =
       createSchemaElementDecl(elem, topLevel, contentSpecType);

    if (elemDecl == 0) {
        return 0;
    }

    fSchemaGrammar->putElemDecl(elemDecl);

    // Resolve the type for the element
    DOM_Element content = checkContent(elem, XUtil::getFirstChildElement(elem), true);

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
            else {
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

            anotherSchemaURI = checkTypeFromAnotherSchema(typeStr);

            // REVISIT- get complex type info
            typeInfo = getElementComplexTypeInfo(typeStr, noErrorFound,
                                                 anotherSchemaURI);

            // get simple type validtor - if not a complex type
            if (typeInfo == 0) {
                validator = getElementTypeValidator(typeStr, noErrorFound,
                                                    anotherSchemaURI,true);
            }
        }
    }

    // Handle the substitutionGroup
    const XMLCh* subsGroupName =
		    getElementAttValue(elem, SchemaSymbols::fgATT_SUBSTITUTIONGROUP);

    if (XMLString::stringLen(subsGroupName) != 0) {

        SchemaElementDecl* subsElemDecl =
                    getSubstituteGroupElemDecl(subsGroupName, noErrorFound);

        if (subsElemDecl != 0) {

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
                fBuffer.append(chColon);
                fBuffer.append(localPart);
                elemDecl->setSubstitutionGroupName(fBuffer.getRawBuffer());
            }
            else {
                noErrorFound = false;
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
        catch(...) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DatatypeValidationFailure, deflt);
        }

        if(typeInfo != 0 &&
           (typeInfo->getContentType() != SchemaElementDecl::Mixed &&
            typeInfo->getContentType() != SchemaElementDecl::Simple)) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::NotSimpleOrMixedElement, name);
        }
    }

    // key/keyref/unique processing
    // TO DO



    // set element information
    elemDecl->setDatatypeValidator(validator);
    elemDecl->setComplexTypeInfo(typeInfo);
    elemDecl->setDefaultValue(deflt);
    elemDecl->setDefinedScope(scopeDefined);
    elemDecl->setModelType(contentSpecType);
    elemDecl->setContentSpec(contentSpecNode);
    elemDecl->setTypeFromAnotherSchemaURI(anotherSchemaURI);

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
    const XMLCh*       typeName = fStringPool.getValueForId(typeNameIndex);
    const XMLCh*       baseTypeName = getElementAttValue(content,
		                                      SchemaSymbols::fgATT_ITEMTYPE);

    if (XUtil::getNextSiblingElement(content) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SimpleTypeContentError);
    }

    if (XMLString::stringLen(baseTypeName) == 0) { // must 'see' <simpleType>

        content = checkContent(rootElem, XUtil::getFirstChildElement(content), false);

        if (content == 0) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ExpectedSimpleTypeInList, typeName);
            return -1;
        }

        if (content.getLocalName().equals(SchemaSymbols::fgELT_SIMPLETYPE)) {
            baseValidator = checkForSimpleTypeValidator(content);
        }
        else {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ListUnionRestrictionError, typeName);
            return -1;
        }

        content = XUtil::getNextSiblingElement(content);
    }
    else { // base was provided - get proper validator

        baseValidator = findDTValidator(rootElem, baseTypeName,
                                        SchemaSymbols::LIST);
        content = checkContent(rootElem, XUtil::getFirstChildElement(content), true);
	}

    if (baseValidator == 0) {
        return -1;
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

            int strId = fStringPool.addOrFind(qualifiedName);
            fDatatypeRegistry->createDatatypeValidator(
				  fStringPool.getValueForId(strId), baseValidator,0, true, finalSet);
		}
    }
    catch(...) {
        reportSchemaError(XMLUni::fgXMLErrDomain,
                          XMLErrs::DatatypeValidatorCreationError, typeName);
    }

    return fStringPool.addOrFind(qualifiedName);
}

int TraverseSchema::traverseByRestriction(const DOM_Element& rootElem,
                                          const DOM_Element& contentElem,
								          const int typeNameIndex,
                                          const int finalSet) {

    DatatypeValidator* baseValidator = 0;
    DOM_Element        content = contentElem;
    const XMLCh*       typeName = fStringPool.getValueForId(typeNameIndex);
    const XMLCh*       baseTypeName = getElementAttValue(content,
		                                      SchemaSymbols::fgATT_BASE);

    if (XUtil::getNextSiblingElement(content) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SimpleTypeContentError);
    }

    if (XMLString::stringLen(baseTypeName) == 0) { // must 'see' <simpleType>

        content = checkContent(rootElem, XUtil::getFirstChildElement(content), false);

        if (content == 0) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ExpectedSimpleTypeInRestriction);
            return -1;
        }

        if (content.getLocalName().equals(SchemaSymbols::fgELT_SIMPLETYPE)) {
            baseValidator = checkForSimpleTypeValidator(content);
        }
        else {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ListUnionRestrictionError, typeName);
            return -1;
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
        return -1;
    }

	// Get facets if any existing
    RefHashTableOf<KVStringPair>*  facets = 0;
    XMLBuffer                      enumData;
    XMLBuffer                      pattern;
    DOMString                      facetName;
    bool                           isFirstPattern = true;

    while (content != 0) {

        if (content.getNodeType() == DOM_Node::ELEMENT_NODE) {

            facetName = content.getLocalName();
            fBuffer.set(facetName.rawBuffer(), facetName.length());

            int facetId = fStringPool.addOrFind(fBuffer.getRawBuffer());
			const XMLCh* facetStr = fStringPool.getValueForId(facetId);
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
                enumData.append(attValue.rawBuffer(), attValueLen);
                enumData.append(chSpace);
            }
            else if (XMLString::compareString(facetStr,
					                 SchemaSymbols::fgELT_PATTERN) == 0) {

                if (isFirstPattern) { // fBuffer.isEmpty() - overhead call
                    pattern.set(attValue.rawBuffer(), attValueLen);
                }
                else { //datatypes: 5.2.4 pattern

                    isFirstPattern = false;
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
                    facets->put((void*) facetStr,
                                new KVStringPair(facetStr,fBuffer.getRawBuffer()));
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

    if (!enumData.isEmpty()) {
        facets->put((void*) SchemaSymbols::fgELT_ENUMERATION,
                    new KVStringPair(SchemaSymbols::fgELT_ENUMERATION, enumData.getRawBuffer()));
    }

    XMLCh* qualifiedName = getQualifiedName(typeNameIndex);

    try {

        DatatypeValidator* newValidator =
		       fDatatypeRegistry->getDatatypeValidator(qualifiedName);

        if (newValidator == 0) {

            int strId = fStringPool.addOrFind(qualifiedName);
            fDatatypeRegistry->createDatatypeValidator
				   (fStringPool.getValueForId(strId), baseValidator, facets,
                    false, finalSet);
		}
    }
    catch(...) {

        reportSchemaError(XMLUni::fgXMLErrDomain,
                          XMLErrs::DatatypeValidatorCreationError, typeName);
        return -1;
    }

    return fStringPool.addOrFind(qualifiedName);
}


int TraverseSchema::traverseByUnion(const DOM_Element& rootElem,
                                    const DOM_Element& contentElem,
								    const int typeNameIndex,
                                    const int finalSet) {

    int                             size = 1;
    DOM_Element                     content = contentElem;
    const XMLCh* const              typeName =
		                              fStringPool.getValueForId(typeNameIndex);
    const XMLCh*                    baseTypeName = getElementAttValue(content,
		                                      SchemaSymbols::fgATT_MEMBERTYPES);
    DatatypeValidator*              baseValidator = 0;
    RefVectorOf<DatatypeValidator>* validators =
		                              new RefVectorOf<DatatypeValidator>(4, false);
    Janitor<DVRefVector>            janValidators(validators);

    if (XUtil::getNextSiblingElement(content) != 0) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SimpleTypeContentError);
    }

    if (XMLString::stringLen(baseTypeName) == 0) { // must 'see' <simpleType>

        content = checkContent(rootElem, XUtil::getFirstChildElement(content), false);

        if (content == 0) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ExpectedSimpleTypeInUnion, typeName);
            return -1;
        }

        if (content.getLocalName().equals(SchemaSymbols::fgELT_SIMPLETYPE)) {

            baseValidator = checkForSimpleTypeValidator(content);

            if (baseValidator == 0) {
                return -1;
            }

            validators->addElement(baseValidator);
        }
        else {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ListUnionRestrictionError, typeName);
            return -1;
        }

        content = XUtil::getNextSiblingElement(content);

    }
    else { //base was provided - get proper validator.

        StringTokenizer unionMembers(baseTypeName);
		int             tokCount = unionMembers.countTokens();
		
        for (int i = 0; i < tokCount; i++) {

            const XMLCh* typeName = unionMembers.nextToken();

            baseValidator = findDTValidator(rootElem, typeName,
                                            SchemaSymbols::UNION);

            if (baseValidator == 0) {
                return -1;
            }

            validators->addElement(baseValidator);
        }

        content = checkContent(rootElem, XUtil::getFirstChildElement(content), true);
    }

    // process union content of simpleType children if any
    while (content != 0) {

        if (content.getLocalName().equals(SchemaSymbols::fgELT_SIMPLETYPE)) {

            baseValidator = checkForSimpleTypeValidator(content);

            if (baseValidator == 0) {
                return -1;
            }

            validators->addElement(baseValidator);
        }
        else {
            // REVISIT - should we return. For now, we will continue and move to
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

            int strId = fStringPool.addOrFind(qualifiedName);
            if (fDatatypeRegistry->createDatatypeValidator(
				           fStringPool.getValueForId(strId), validators, finalSet) != 0) {
                janValidators.orphan();
			}
		}
    }
    catch(...) {
        reportSchemaError(XMLUni::fgXMLErrDomain,
                          XMLErrs::DatatypeValidatorCreationError, typeName);
    }

    return fStringPool.addOrFind(qualifiedName);
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
    // Set the content type to be simple, and initialize content spec handle
    // -----------------------------------------------------------------------
    typeInfo->setContentType(SchemaElementDecl::Simple);

    DOM_Element simpleContent =
        checkContent(contentDecl, XUtil::getFirstChildElement(contentDecl),false);

    // If there are no children, return
    if (simpleContent == 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::EmptySimpleTypeContent);
        throw;
    }

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
        throw;
    }

    // -----------------------------------------------------------------------
    // Handle the base type name
    // -----------------------------------------------------------------------
    const XMLCh* baseName =
            getElementAttValue(simpleContent, SchemaSymbols::fgATT_BASE);

    if (XMLString::stringLen(baseName) == 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::UnspecifiedBase);
        throw;
    }

    const XMLCh* prefix = getPrefix(baseName);
    const XMLCh* localPart = getLocalPart(baseName);
    const XMLCh* uri = resolvePrefixToURI(prefix);
    DatatypeValidator* baseValidator = getDatatypeValidator(uri, localPart);

    if (baseValidator != 0
		&& ((baseValidator->getFinalSet()
		     & SchemaSymbols::EXTENSION) == typeInfo->getDerivedBy())) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DisallowedSimpleTypeExtension,
                          baseName, typeName);
        throw;
    }

    processBaseTypeInfo(baseName, localPart, uri, typeInfo);

    // check that the base isn't a complex type with complex content
    ComplexTypeInfo* baseTypeInfo = typeInfo->getBaseComplexTypeInfo();

    if (baseTypeInfo != 0 && baseTypeInfo->getContentSpec() != 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidSimpleContentBase, baseName);
        throw;
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
            throw;
        }
        else {
           typeInfo->setBaseDatatypeValidator(
               typeInfo->getBaseComplexTypeInfo()->getDatatypeValidator());
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

                    typeInfo->setBaseDatatypeValidator(fDatatypeRegistry->getDatatypeValidator(
                       fStringPool.getValueForId(simpleTypeNameIndex)));

                    content = XUtil::getNextSiblingElement(content);
                }
                else {
                    throw;
                }
            }

            // ---------------------------------------------------------------
            // Build up the facet info
            // ---------------------------------------------------------------
            RefHashTableOf<KVStringPair>*  facets = 0;
            XMLBuffer                      enumData;
            XMLBuffer                      pattern;
            const XMLCh*                   facetName;
            int                            facetId;
            bool                           isFirstPattern = true;

            while (content != 0) {

                fBuffer.set(content.getLocalName().rawBuffer(), content.getLocalName().length());
                facetId = fStringPool.addOrFind(fBuffer.getRawBuffer());
                facetName = fStringPool.getValueForId(facetId);

                // if not a valid facet, break from the loop
                if (!isValidFacet(SchemaSymbols::fgELT_SIMPLECONTENT, facetName)) {
                    break;
                }

                if (content.getNodeType() == DOM_Node::ELEMENT_NODE) {

                    DOMString attValue =
                        content.getAttribute(SchemaSymbols::fgATT_VALUE);

                    if (facets == 0) {
                        facets = new RefHashTableOf<KVStringPair>(29, true);
                    }

                    fBuffer.set(attValue.rawBuffer(), attValue.length());

                    if (XMLString::compareString(facetName,
					                 SchemaSymbols::fgELT_ENUMERATION) == 0) {

                        enumData.append(fBuffer.getRawBuffer());
                        enumData.append(chSpace);
                    }
                    else if (XMLString::compareString(facetName,
					                 SchemaSymbols::fgELT_PATTERN) == 0) {

                        if (isFirstPattern) { // fBuffer.isEmpty() - overhead call
                            pattern.set(fBuffer.getRawBuffer());
                        }
                        else { //datatypes: 5.2.4 pattern

                            isFirstPattern = false;
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
                        }
                    }
                }

                content = XUtil::getNextSiblingElement(content);
            }

            if (facets != 0) {

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

                if (!enumData.isEmpty()) {
                    facets->put
                    (
                        (void*) SchemaSymbols::fgELT_ENUMERATION,
                        new KVStringPair
                            (
                                SchemaSymbols::fgELT_ENUMERATION,
                                enumData.getRawBuffer()
                            )
                    );
				}

                XMLCh* qualifiedName =
                    getQualifiedName(fStringPool.addOrFind(typeName));

                try {

                    int nameId = fStringPool.addOrFind(qualifiedName);

                    typeInfo->setDatatypeValidator
                    (
                        fDatatypeRegistry->createDatatypeValidator
                        (
				            fStringPool.getValueForId(nameId),
						    typeInfo->getBaseDatatypeValidator(),
                            facets, false, 0
                        )
                    );
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
    // -----------------------------------------------------------------------
    // Determine whether the content is mixed, or element-only
    // Setting here overrides any setting on the complex type decl
    // -----------------------------------------------------------------------
    const XMLCh* const mixed =
                getElementAttValue(contentDecl, SchemaSymbols::fgATT_MIXED);

    bool mixedContent = isMixed;

    if (XMLString::compareString(mixed, SchemaSymbols::fgATTVAL_TRUE) == 0) {
        mixedContent = true;
    }
    else if (XMLString::compareString(mixed, SchemaSymbols::fgATTVAL_FALSE) == 0) {
        mixedContent = false;
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
       throw;
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
        throw;
    }

    // -----------------------------------------------------------------------
    // Handle the base type name
    // -----------------------------------------------------------------------
    const XMLCh* baseName =
            getElementAttValue(contentDecl, SchemaSymbols::fgATT_BASE);

    if (XMLString::stringLen(baseName) == 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::UnspecifiedBase);
        throw;
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
            throw;
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
  *   namespace = ##any | ##other | ##local | list of {uri, ##targetNamespace}>
  *   processContents = (lax | skip | strict) : strict
  *   Content: (annotation?)
  * </anyAttribute>
  */
SchemaAttDef* TraverseSchema::traverseAnyAttribute(const DOM_Element& elem) {

    // ------------------------------------------------------------------
    // First, handle any ANNOTATION declaration
    // ------------------------------------------------------------------
    if (checkContent(elem, XUtil::getFirstChildElement(elem), true) != 0) {
//        reportSchemaError("anyAttribute elements can contain at most one 'annotation' element in their children");
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
    else {
        reportSchemaError(0, 0, processContents); //"Invalid 'processContents' value: '{0}'
    }

    // ------------------------------------------------------------------
    // Process 'namespace' attribute
    // ------------------------------------------------------------------
    int uriIndex = fURIStringPool->addOrFind(XMLUni::fgZeroLenString);
    XMLAttDef::AttTypes attType = XMLAttDef::Any_Any;

    fBuffer.reset();

    if (XMLString::stringLen(nameSpace) == 0
        || XMLString::compareString(nameSpace, SchemaSymbols::fgATTVAL_TWOPOUNDANY) == 0) {
        // Do nothing - defaulted already
    }
    else if (XMLString::compareString(nameSpace, SchemaSymbols::fgATTVAL_TWOPOUNDOTHER) == 0) {

        attType = XMLAttDef::Any_Other;
        uriIndex = fTargetNSURI;
    }
    else if (XMLString::compareString(nameSpace, SchemaSymbols::fgATTVAL_TWOPOUNDLOCAL) == 0) {
        attType = XMLAttDef::Any_Local;
    }
    else {

        StringTokenizer tokenizer(nameSpace);
        XMLCh separator = chNull;

        attType = XMLAttDef::Any_List;

        // Will be using '|' as a separator to construct enumeration string.
        // When the data is retrieved, a StringTokenizer construction with a delimeter
        // string of "|" sould be used.
        while (tokenizer.hasMoreTokens()) {

			const XMLCh* token = tokenizer.nextToken();

            if (separator != chNull) {
                fBuffer.append(separator);
            }

            if (XMLString::compareString(token,
                    SchemaSymbols::fgATTVAL_TWOPOUNDTRAGETNAMESPACE) == 0) {
                fBuffer.append(fTargetNSURIString);
            }
            else {
                fBuffer.append(token);
            }

            separator = chPipe;
        }
    }

    SchemaAttDef* attDef = new SchemaAttDef(XMLUni::fgZeroLenString,
                                            XMLUni::fgZeroLenString,
                                            uriIndex, attType, attDefType);

	if (!fBuffer.isEmpty()){
        attDef->setEnumeration(fBuffer.getRawBuffer());
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
        int nameId = fStringPool.addOrFind(fBuffer.getRawBuffer());
        const XMLCh* name = fStringPool.getValueForId(nameId);

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
                        fURIStringPool->addOrFind(XMLUni::fgZeroLenString));
    }
}



void TraverseSchema::extractTopLevel3Components(const DOM_Element& rootElem) {

    // REVISIT - if necessary build list
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

        if (name.equals(SchemaSymbols::fgELT_ANNOTATION)) {
            traverseAnnotationDecl(child);
        }
        else if (name.equals(SchemaSymbols::fgELT_SIMPLETYPE)) {
            traverseSimpleTypeDecl(child);
        }
        else if (name.equals(SchemaSymbols::fgELT_COMPLEXTYPE)) {
            traverseComplexTypeDecl(child);
        }
        else if (name.equals(SchemaSymbols::fgELT_ELEMENT)) {
            traverseElementDecl(child);
        }
        else if (name.equals(SchemaSymbols::fgELT_ATTRIBUTEGROUP)) {
//            traverseAttributeGroupDecl(child, 0, 0);
        }
        else if (name.equals(SchemaSymbols::fgELT_ATTRIBUTE ) ) {
            traverseAttributeDecl( child, 0);
        }
        else if (name.equals(SchemaSymbols::fgELT_GROUP)) {
            traverseGroupDecl(child);
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

    if (XMLString::stringLen(uriStr) == 0
		|| XMLString::compareString(uriStr, SchemaSymbols::fgURI_SCHEMAFORSCHEMA) == 0) {
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

    const XMLCh* typeName = fStringPool.getValueForId(typeNameIndex);

    if (XMLString::stringLen(fTargetNSURIString) != 0) {

		fBuffer.set(fTargetNSURIString);
        fBuffer.append(chComma);
        fBuffer.append(typeName);
    }
    else {
        fBuffer.set(typeName);
    }

    return fBuffer.getRawBuffer();
}


DatatypeValidator*
TraverseSchema::checkForSimpleTypeValidator(const DOM_Element& content) {

    int typeNameIndex = traverseSimpleTypeDecl(content);
    DatatypeValidator* baseValidator = 0;

    if (typeNameIndex != -1) {

        baseValidator = fDatatypeRegistry->getDatatypeValidator(
			                      fStringPool.getValueForId(typeNameIndex));
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
			                      fStringPool.getValueForId(typeNameIndex));
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

    int nameSpaceIndex = fNamespaceScope->getNamespaceForPrefix(prefix);
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
        reportSchemaError(XMLUni::fgValidityDomain, XMLValid::NoContentForRef);
    }

    const XMLCh* prefix = getPrefix(refName);
    const XMLCh* localPart = getLocalPart(refName);
    const XMLCh* uriStr = resolvePrefixToURI(prefix);
    QName*       eltName = new QName(prefix , localPart, uriStr != 0
									   ? fURIStringPool->addOrFind(uriStr)
                                       : fURIStringPool->addOrFind(XMLUni::fgZeroLenString)); // StringPool.EMPTY_STRING == 0

    //if from another schema, just return the element QName
    if (XMLString::compareString(uriStr, fTargetNSURIString) != 0) {
        return eltName;
    }

    unsigned int elemIndex = fSchemaGrammar->getElemId(eltName->getURI(),
                                                       localPart, 0,
                                                       Grammar::TOP_LEVEL_SCOPE);

    //if not found, traverse the top level element that if referenced
    if (elemIndex == XMLElementDecl::fgInvalidElemId) {

        DOM_Element targetElem =
		             getTopLevelComponentByName(SchemaSymbols::fgELT_ELEMENT,
                                                localPart);

        if (targetElem == 0)  {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::RefElementNotFound, localPart);
            // REVISIT do we return 0 or what? for now we will return QName created
            return eltName;
        }
    }

    return eltName;
}

int TraverseSchema::parseBlockSet(const XMLCh* const blockStr) {

    if (!blockStr) {
        return fBlockDefault;
    }

    if (XMLString::compareString(blockStr, SchemaSymbols::fgATTVAL_POUNDALL) == 0) {
        return SchemaSymbols::EXTENSION + SchemaSymbols::LIST +
               SchemaSymbols::RESTRICTION + SchemaSymbols::UNION +
               SchemaSymbols::SUBSTITUTION;
    }

    int             blockSet = 0;
    StringTokenizer tokenizer(blockStr);

    while (tokenizer.hasMoreTokens()) {

        XMLCh* token = tokenizer.nextToken();

        if (XMLString::compareString(token, SchemaSymbols::fgATTVAL_SUBSTITUTION) == 0) {

            if ((blockSet & SchemaSymbols::SUBSTITUTION) == 0 ) {
                blockSet += SchemaSymbols::SUBSTITUTION;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SubstitutionRepeated);
            }
        }
        else if (XMLString::compareString(token, SchemaSymbols::fgELT_UNION) == 0) {

            if ((blockSet & SchemaSymbols::UNION) == 0) {
                blockSet += SchemaSymbols::UNION;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::UnionRepeated);
            }
        }
        else if (XMLString::compareString(token, SchemaSymbols::fgATTVAL_EXTENSION) == 0) {

            if ((blockSet & SchemaSymbols::EXTENSION) == 0) {
                blockSet += SchemaSymbols::EXTENSION;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ExtensionRepeated);
            }
        }
        else if (XMLString::compareString(token, SchemaSymbols::fgELT_LIST) == 0) {

            if ((blockSet & SchemaSymbols::LIST) == 0 ) {
                blockSet += SchemaSymbols::LIST;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ListRepeated);
            }
        }
        else if (XMLString::compareString(token, SchemaSymbols::fgATTVAL_RESTRICTION) ) {

            if ((blockSet & SchemaSymbols::RESTRICTION) == 0 ) {
                blockSet += SchemaSymbols::RESTRICTION;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::RestrictionRepeated);
            }
        }
        else {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidBlockValue, blockStr); //
        }
    } //end while

    return (blockSet == 0 ? fBlockDefault : blockSet);
}

int TraverseSchema::parseFinalSet(const XMLCh* const finalStr) {

    if (!finalStr) {
        return fFinalDefault;
    }

    if (XMLString::compareString(finalStr, SchemaSymbols::fgATTVAL_POUNDALL) == 0) {
        return SchemaSymbols::EXTENSION + SchemaSymbols::LIST +
               SchemaSymbols::RESTRICTION + SchemaSymbols::UNION;
    }

    int             finalSet = 0;
    StringTokenizer tokenizer(finalStr);

    while (tokenizer.hasMoreTokens()) {

        XMLCh* token = tokenizer.nextToken();

        if (XMLString::compareString(token, SchemaSymbols::fgELT_UNION) == 0) {

            if ((finalSet & SchemaSymbols::UNION) == 0) {
                finalSet += SchemaSymbols::UNION;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::UnionRepeated);
            }
        }
        else if (XMLString::compareString(token, SchemaSymbols::fgATTVAL_EXTENSION) == 0) {

            if ((finalSet & SchemaSymbols::EXTENSION) == 0) {
                finalSet += SchemaSymbols::EXTENSION;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ExtensionRepeated);
            }
        }
        else if (XMLString::compareString(token, SchemaSymbols::fgELT_LIST) == 0) {

            if ((finalSet & SchemaSymbols::LIST) == 0 ) {
                finalSet += SchemaSymbols::LIST;
            }
            else {
                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ListRepeated);
            }
        }
        else if (XMLString::compareString(token, SchemaSymbols::fgATTVAL_RESTRICTION) ) {

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
					                    fStringPool.getValueForId(typeIndex));
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
                                         const XMLCh* const elemName) {

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

    // Check for type relationship;
    // that is, make sure that the type we're deriving has some relatoinship
    // to substitutionGroupElt's type.
    if (typeInfo != 0) { // do complexType case ...need testing

        int derivationMethod = typeInfo->getDerivedBy();

        if (typeInfo->getContentType() == SchemaElementDecl::Simple) {  // take care of complexType based on simpleType case...

            DatatypeValidator* elemDV = typeInfo->getDatatypeValidator();
            DatatypeValidator* subsValidator = subsElemDecl->getDatatypeValidator();

            if (subsValidator != 0 && subsValidator->isSubstitutableBy(elemDV)) {

                if ((subsElemDecl->getFinalSet() & derivationMethod) != 0) {

                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidSubstitutionGroupElement,
                                      elemName, subsElemDecl->getBaseName());
                    return false;
                }
			}
            else {

                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SubstitutionGroupTypeMismatch, elemName);
                return false;
            }
        }
        else { // complex content

            ComplexTypeInfo* subsTypeInfo = subsElemDecl->getComplexTypeInfo();
            const ComplexTypeInfo* elemTypeInfo = typeInfo;

            for (; elemTypeInfo != subsTypeInfo;
                 elemTypeInfo = elemTypeInfo->getBaseComplexTypeInfo()) {
            }

            if (elemTypeInfo != 0) {

                if ((subsElemDecl->getFinalSet() & derivationMethod) != 0) {

                    reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidSubstitutionGroupElement,
                                      elemName, subsElemDecl->getBaseName());
                    return false;
                }
			}
            else {

                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SubstitutionGroupTypeMismatch, elemName);
                return false;
            }
        }
    }
	else if (validator != 0) { // do simpleType case...

        // first, check for type relation.
        DatatypeValidator* subsValidator = subsElemDecl->getDatatypeValidator();

        if (subsValidator != 0  && subsValidator->isSubstitutableBy(validator)) {

            if ((subsElemDecl->getFinalSet() & SchemaSymbols::RESTRICTION) != 0) {

                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidSubstitutionGroupElement,
                                  elemName, subsElemDecl->getBaseName());
                return false;
            }
        }
        else {

            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::SubstitutionGroupTypeMismatch, elemName);
            return false;
        }
    }

    return true;
}


SchemaElementDecl*
TraverseSchema::createSchemaElementDecl(const DOM_Element& elem,
                                        const bool topLevel,
                                        const unsigned short elemType)
{
    const XMLCh* name = getElementAttValue(elem, SchemaSymbols::fgATT_NAME);
    const XMLCh* elemForm = getElementAttValue(elem, SchemaSymbols::fgATT_FORM);
    int enclosingScope = fCurrentScope;
    int uriIndex = fURIStringPool->addOrFind(XMLUni::fgZeroLenString);

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
    const SchemaElementDecl* other = (SchemaElementDecl*)
		fSchemaGrammar->getElemDecl(uriIndex, name, 0, enclosingScope);

    if (other != 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateElementDeclaration, name);
        return 0;
    }

    const XMLCh* block = getElementAttValue(elem,SchemaSymbols::fgATT_BLOCK);
    const XMLCh* final = getElementAttValue(elem,SchemaSymbols::fgATT_FINAL);
    int blockSet = block != 0 ? parseBlockSet(block) : fBlockDefault;
    int finalSet = final != 0 ? parseFinalSet(final) : fFinalDefault;
	int elementMiscFlags = 0;
    int finalValid = SchemaSymbols::RESTRICTION + SchemaSymbols::EXTENSION;
    int blockValid = finalValid + SchemaSymbols::SUBSTITUTION;

    if (XMLString::stringLen(block) != 0
		&& XMLString::compareString(block,SchemaSymbols::fgATTVAL_POUNDALL) != 0
        && ((blockSet & blockValid) == 0)) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidElementBlockValue, block);
    }

    if (XMLString::stringLen(final) != 0
		&& XMLString::compareString(final,SchemaSymbols::fgATTVAL_POUNDALL) != 0
        && ((finalSet & finalValid) == 0)) {
        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidElementFinalValue, final);
    }

    const XMLCh* nillable = getElementAttValue(elem, SchemaSymbols::fgATT_NILLABLE);
    const XMLCh* abstract = getElementAttValue(elem, SchemaSymbols::fgATT_ABSTRACT);

    if (nillable) {

        if (XMLString::compareString(nillable, SchemaSymbols::fgATTVAL_TRUE) == 0) {
            elementMiscFlags += SchemaSymbols::NILLABLE;
        }
        else if (XMLString::compareString(nillable, SchemaSymbols::fgATTVAL_FALSE) != 0) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidElementNillableValue, nillable);
        }
    }

    if (abstract) {

        if (XMLString::compareString(abstract, SchemaSymbols::fgATTVAL_TRUE) == 0) {
            elementMiscFlags += SchemaSymbols::ABSTRACT;
        }
        else if (XMLString::compareString(abstract, SchemaSymbols::fgATTVAL_FALSE) != 0) {
            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidElementAbstractValue, abstract);
        }
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

    // REVISIT - attributeGroup case
    if (typeInfo == 0) {
        return;
    }

    const XMLCh* prefix = getPrefix(refName);
    const XMLCh* localPart = getLocalPart(refName);
    const XMLCh* uriStr = resolvePrefixToURI(prefix);

    if (XMLString::compareString(uriStr, fTargetNSURIString) != 0) {

        addAttributeDeclFromAnotherSchema(localPart, uriStr, typeInfo);
        return;
    }

    // Check for duplicate references
    if (typeInfo->getAttDef(localPart,fURIStringPool->addOrFind(uriStr)) != 0) {

        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::DuplicateRefAttribute, uriStr, localPart);
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

    typeInfo->addAttDef(attDef);
}


ContentSpecNode*
TraverseSchema::expandContentModel(ContentSpecNode* const specNode,
                                   const DOM_Element& elem) {

    unsigned int minOccurs = 0;
    unsigned int maxOccurs = 0;	
    DOMString    nOccurs = elem.getAttribute(SchemaSymbols::fgATT_MINOCCURS);

    if (nOccurs.length() > 0) {

        fBuffer.set(nOccurs.rawBuffer(), nOccurs.length());
	    XMLString::trim(fBuffer.getRawBuffer());
    }
    else {
        fBuffer.reset();
    }

    if (XMLString::stringLen(fBuffer.getRawBuffer()) == 0) {
        minOccurs = 1;
    }
    else {
        XMLString::textToBin(fBuffer.getRawBuffer(), minOccurs);
    }

    nOccurs = elem.getAttribute(SchemaSymbols::fgATT_MAXOCCURS);

    if (nOccurs.length() > 0) {
        fBuffer.set(nOccurs.rawBuffer(), nOccurs.length());
    }
    else {
        fBuffer.reset();
    }

    XMLCh* maxOccursStr = XMLString::replicate(fBuffer.getRawBuffer());
    ArrayJanitor<XMLCh> janMaxOccur(maxOccursStr);

    XMLString::trim(maxOccursStr);

    bool isMaxUnbounded =
            (XMLString::compareString(maxOccursStr, fgUnbounded) == 0);

    if (XMLString::stringLen(maxOccursStr) == 0) {
        maxOccurs = 1;
    }
    else {
        XMLString::textToBin(maxOccursStr, maxOccurs);
    }

    if (minOccurs == 0 && maxOccurs == 0 && !isMaxUnbounded) {
        return 0;
    }

    ContentSpecNode* saveNode = specNode;
    ContentSpecNode* retNode = specNode;

    if (minOccurs == 1 && maxOccurs == 1) {
    }
    else if (minOccurs == 0 && maxOccurs == 1) {

        retNode = new ContentSpecNode(ContentSpecNode::ZeroOrOne,
                                      retNode, 0);
    }
    else if (minOccurs == 0 && isMaxUnbounded) {
        retNode = new ContentSpecNode(ContentSpecNode::ZeroOrMore,
                                      retNode, 0);
    }
    else if (minOccurs == 1 && isMaxUnbounded) {
        retNode = new ContentSpecNode(ContentSpecNode::OneOrMore,
                                      retNode, 0);
    }
    else if (isMaxUnbounded) {

        retNode = new ContentSpecNode(ContentSpecNode::OneOrMore,
                                      retNode, 0);

        for (int i=0; i < (int)(minOccurs-1); i++) {
            retNode = new ContentSpecNode(ContentSpecNode::Sequence,
                                          saveNode, retNode, false);
        }
    }
    else {

        if (minOccurs == 0) {

            ContentSpecNode* optional =
                new ContentSpecNode(ContentSpecNode::ZeroOrOne, saveNode, 0);

            retNode = optional;

            for (int i=0; i < (int)(maxOccurs-minOccurs-1); i++) {
                retNode = new ContentSpecNode(ContentSpecNode::Sequence,
                                              retNode, optional, true, false);
            }
        }
        else {

            for (int i=0; i < (int)(minOccurs-1); i++) {
                retNode = new ContentSpecNode(ContentSpecNode::Sequence,
                                              retNode, saveNode, true, false);
            }

            ContentSpecNode* optional =
                new ContentSpecNode(ContentSpecNode::ZeroOrOne, saveNode, 0, false);

            for (int j=0; j < (int)(maxOccurs-minOccurs); j++) {

                bool toAdopt = (j == 0) ? true : false;
                retNode = new ContentSpecNode(ContentSpecNode::Sequence,
                                              retNode, optional, true, toAdopt);
            }
        }
    }

    return retNode;
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

    if (childElem != 0) {

        // --------------------------------------------------------------------
        // GROUP, ALL, SEQUENCE or CHOICE, followed by attributes, if specified.
        // Note that it's possible that only attributes are specified.
        // --------------------------------------------------------------------
        DOMString childName = childElem.getLocalName();

		if (childName.equals(SchemaSymbols::fgELT_GROUP)) {

//            specNode = expandContentModel(traverseGroupDecl(childElem),
//                                          childElem);
            attrNode = XUtil::getNextSiblingElement(childElem);
        }
        else if (childName.equals(SchemaSymbols::fgELT_SEQUENCE)) {

            specNode = expandContentModel(traverseChoiceSequence(childElem,
				                                  ContentSpecNode::Sequence),
                                          childElem);
            attrNode = XUtil::getNextSiblingElement(childElem);
        }
        else if (childName.equals(SchemaSymbols::fgELT_CHOICE)) {

            specNode = expandContentModel(traverseChoiceSequence(childElem,
                                                  ContentSpecNode::Choice),
                                          childElem);
            attrNode = XUtil::getNextSiblingElement(childElem);
        }
        else if (childName.equals(SchemaSymbols::fgELT_ALL)) {

            specNode = expandContentModel(traverseAll(childElem), childElem);
            attrNode = XUtil::getNextSiblingElement(childElem);
            //TO DO: REVISIT
            //check that minOccurs = 1 and maxOccurs = 1
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
    }

    if (isMixed) {

        // TODO - check to see if we MUST have an element.  What if only attributes
        // were specified??

        // add #PCDATA leaf
        QName* tmpName = new QName(XMLUni::fgZeroLenString, XMLUni::fgZeroLenString, XMLElementDecl::fgPCDataElemId);
        Janitor<QName> janQName(tmpName);
        ContentSpecNode* pcdataNode = new ContentSpecNode(tmpName);

        // If there was an element, the content spec becomes a choice of PCDATA and
        // the element
        if (specNode != 0) {
            specNode = new ContentSpecNode(ContentSpecNode::Choice,
			                               pcdataNode, specNode);
        }
        else {
            specNode = pcdataNode;
        }
    }

    typeInfo->setContentSpec(specNode);

    // -----------------------------------------------------------------------
    // Merge in information from base, if it exists
    // -----------------------------------------------------------------------
    ComplexTypeInfo* baseTypeInfo = typeInfo->getBaseComplexTypeInfo();
    if (baseTypeInfo != 0) {

        ContentSpecNode* baseSpecNode = baseTypeInfo->getContentSpec();

        if (typeInfo->getDerivedBy() == SchemaSymbols::RESTRICTION) {

            // check to see if the baseType permits derivation by restriction
            if((baseTypeInfo->getFinalSet() & SchemaSymbols::RESTRICTION) != 0) {

                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ForbiddenDerivationByRestriction,
                                  baseLocalPart);
                throw;
            }

            //REVISIT: !!!really hairy stuff to check the particle derivation OK in 5.10
        }
        else {

            // check to see if the baseType permits derivation by extension
            if((baseTypeInfo->getFinalSet() & SchemaSymbols::EXTENSION) != 0) {

                reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::ForbiddenDerivationByExtension, baseLocalPart);
                throw;
            }

            // Compose the final content model by concatenating the base and
            // the current in sequence
            if (typeInfo->getContentSpec() == 0) {

                typeInfo->setContentSpec(baseSpecNode);
                typeInfo->setAdoptContentSpec(false);
            }
            else if (baseSpecNode != 0) {

                typeInfo->setContentSpec(
                    new ContentSpecNode(ContentSpecNode::Sequence, baseSpecNode,
                                        typeInfo->getContentSpec(), false,
                                        typeInfo->getAdoptContentSpec()));
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
                throw;
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
        baseComplexTypeInfo = fComplexTypeRegistry->get(fBuffer.getRawBuffer());

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
                                fStringPool.getValueForId(baseTypeSymbol));
                }
                else {

                    baseTypeNode = getTopLevelComponentByName(
                                SchemaSymbols::fgELT_SIMPLETYPE, localPart);

                    if (baseTypeNode != 0) {

                        baseTypeSymbol = traverseSimpleTypeDecl(baseTypeNode);
                        baseDTValidator = getDatatypeValidator(uriStr, localPart);

                        if (baseDTValidator == 0)  {

                            reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::TypeNotFound, uriStr, localPart, uriStr);
                            throw;
                        }
                    }
                    else {

                        reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::BaseTypeNotFound, baseName);
                        throw;
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

    for (; child != 0; child = XUtil::getNextSiblingElement(child)) {

        DOMString childName = child.getLocalName();

        if (childName.equals(SchemaSymbols::fgELT_ATTRIBUTE)) {
            traverseAttributeDecl(child, typeInfo);
        }
        else if (childName.equals(SchemaSymbols::fgELT_ATTRIBUTEGROUP)) {
            // TO DO
        }
        else if (childName.equals(SchemaSymbols::fgELT_ANYATTRIBUTE) ) {
            attWildCard = traverseAnyAttribute(child);
            janAttWildCard.reset(attWildCard);
        }
        else {

            fBuffer.set(childName.rawBuffer(), childName.length());
            reportSchemaError(XMLUni::fgXMLErrDomain,
                              XMLErrs::InvalidChildInComplexType,
							  fBuffer.getRawBuffer());
        }
    }

    // -------------------------------------------------------------
    // Handle wild card/any attribute
    // -------------------------------------------------------------
    // TO DO - based on attributeGroup
    if (attWildCard != 0) {
    }
    else {
    }


    // -------------------------------------------------------------
    // merge in base type's attribute decls
    // -------------------------------------------------------------
    ComplexTypeInfo* baseTypeInfo = typeInfo->getBaseComplexTypeInfo();
    SchemaAttDef*    baseAttWildCard = 0;

    if (baseTypeInfo != 0 && baseTypeInfo->hasAttDefs()) {

        SchemaAttDefList& baseAttList = (SchemaAttDefList&)
                                        baseTypeInfo->getAttDefList();

        while (baseAttList.hasMoreElements()) {

            SchemaAttDef& attDef = (SchemaAttDef&) baseAttList.nextElement();
            XMLAttDef::AttTypes attType = attDef.getType();
            QName* attName = attDef.getAttName();

            if (attType == XMLAttDef::Any_Any
                || attType == XMLAttDef::Any_List
                || attType == XMLAttDef::Any_Local
                || attType == XMLAttDef::Any_Other) {

                // TO DO
                if (attWildCard == 0) {
                    baseAttWildCard = &attDef;
                }

                continue;
            }

            // if found a duplicate, if it is derived by restriction,
            // then skip the one from the base type
            if (typeInfo->contains(attName->getLocalPart())) {

                if (typeInfo->getDerivedBy() == SchemaSymbols::RESTRICTION) {
                    continue;
                }
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

    // -------------------------------------------------------------
    // insert wildcard attributes
    // -------------------------------------------------------------
    if (attWildCard != 0) {

        if (attWildCard->getType() != XMLAttDef::AttTypes_Unknown) {

            typeInfo->addAttDef(attWildCard);
            janAttWildCard.orphan();
        }
    }
    else if (baseAttWildCard != 0) {

        QName* attName = baseAttWildCard->getAttName();
        SchemaAttDef* newAttDef = new SchemaAttDef(attName->getPrefix(),
                                                   attName->getLocalPart(),
                                                   attName->getURI(),
                                                   baseAttWildCard->getValue(),
                                                   baseAttWildCard->getType(),
                                                   baseAttWildCard->getDefaultType(),
                                                   baseAttWildCard->getEnumeration());

        newAttDef->setDatatypeValidator(baseAttWildCard->getDatatypeValidator());
        typeInfo->addAttDef(newAttDef);
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

    if (typeInfo!= 0) {

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
        typeInfo->addAttDef(newAtt);
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

// ---------------------------------------------------------------------------
//  TraverseSchema: Error reporting methods
// ---------------------------------------------------------------------------
void TraverseSchema::reportSchemaError(const XMLCh* const msgDomain,
                                       const int errorCode) {

    if (fScanner && XMLString::compareString(msgDomain, XMLUni::fgXMLErrDomain) == 0) {
        fScanner->emitError((XMLErrs::Codes) errorCode);
    }
    else if (fValidator && fScanner->getDoValidation()
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
    else if (fValidator && fScanner->getDoValidation()
             && XMLString::compareString(msgDomain, XMLUni::fgValidityDomain) == 0) {
        fValidator->emitError((XMLValid::Codes) errorCode,text1,text2,text3,text4);
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

    return srcToFill;
}


void TraverseSchema::restoreSchemaInfo() {

    fCurrentSchemaURL = fCurrentSchemaInfo->getCurrentSchemaURL();
    fCurrentScope = fCurrentSchemaInfo->getCurrentScope();
    fElementDefaultQualified = fCurrentSchemaInfo->isElementDefaultQualified();
    fAttributeDefaultQualified = fCurrentSchemaInfo->isAttributeDefaultQualified();
    fBlockDefault = fCurrentSchemaInfo->getBlockDefault();
    fFinalDefault = fCurrentSchemaInfo->getFinalDefault();
    fSchemaRootElement = fCurrentSchemaInfo->getRoot();
}

// ---------------------------------------------------------------------------
//  TraverseSchema: CleanUp methods
// ---------------------------------------------------------------------------
void TraverseSchema::cleanUp() {

    delete [] fTargetNSURIString;
    delete [] fCurrentSchemaURL;
    delete fSchemaInfoRoot;
    delete fImportLocations;
    delete fIncludeLocations;
}
/**
  * End of file TraverseSchema.cpp
  */

