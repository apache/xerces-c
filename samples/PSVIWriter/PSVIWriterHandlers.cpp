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

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "PSVIWriterHandlers.hpp"
#include "PSVIUni.hpp"
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMTypeInfo.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <string.h>

XERCES_CPP_NAMESPACE_USE

static const XMLCh fgSpace[] = { chSpace, chNull };
static const XMLCh fgRoot[] = { chLatin_r, chLatin_o, chLatin_o, chLatin_t, chNull };
static const XMLCh fgChar[] = { chSpace, chLatin_C, chLatin_h, chLatin_a, chLatin_r, chNull }; // char
static const XMLCh fgLine[] = { chSpace, chLatin_L, chLatin_i, chLatin_n, chLatin_e, chNull }; // line
static const XMLCh fgError[] = { chLatin_E, chLatin_r, chLatin_r, chLatin_o, chLatin_r, chNull }; //Error
static const XMLCh fgAtFile[] = { // at file
	chSpace, chLatin_a, chLatin_t, chSpace, chLatin_f, chLatin_i, chLatin_l, chLatin_e, chNull 
};
static const XMLCh fgFatalError[] = { //Fatal Error
	chLatin_F, chLatin_a, chLatin_t, chLatin_a, chLatin_l, chSpace, 
	chLatin_E, chLatin_r, chLatin_r, chLatin_o, chLatin_r, chNull 
};
static const XMLCh fgMessage[] = { //Message
	chLatin_M, chLatin_e, chLatin_s, chLatin_s, chLatin_a, chLatin_g, chLatin_e, chNull 
};
static const XMLCh fgXsiNil[] = { //xsi:nil
	chLatin_x, chLatin_s, chLatin_i, chColon, chLatin_n, chLatin_i, chLatin_l, chNull 
};
static const XMLCh fgWarning[] = { //Warning
	chLatin_W, chLatin_a, chLatin_r, chLatin_n, chLatin_i, chLatin_n, chLatin_g, chNull 
};
static const XMLCh gXmlnsColonXsi[] = { //xmlns:xsi
	chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chColon, chLatin_x, chLatin_s, 
	chLatin_i, chNull
};
static const XMLCh gXmlnsColonPsv[] = { //xmlns:psv
	chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chColon, chLatin_p, chLatin_s, 
	chLatin_v, chNull
};

static const XMLCh gRef[] = { chLatin_r, chLatin_e, chLatin_f, chNull }; // ref
static const XMLCh gId[] = { chLatin_i, chLatin_d, chNull }; // id

static const XMLCh gEqualsQuote[] = { chEqual, chDoubleQuote, chNull };
static const XMLCh gAngleSlash[] = { chOpenAngle, chForwardSlash, chNull };
static const XMLCh gAngleFeed[] = { chCloseAngle, chLF, chNull };
static const XMLCh gSlashAngleFeed[] = { chForwardSlash, chCloseAngle, chLF, chNull };
static const XMLCh gQuoteAngleFeed[] = { chDoubleQuote, chCloseAngle, chLF, chNull };


// ---------------------------------------------------------------------------
//  PSVIWriterHandlers: Constructors and Destructor
// ---------------------------------------------------------------------------
PSVIWriterHandlers::PSVIWriterHandlers(XMLFormatter* outputFormatter, XMLFormatter* errorFormatter) :
		PSVIHandler(), DefaultHandler() {
	fFormatter = outputFormatter;
	fErrorFormatter = (errorFormatter != NULL) ? errorFormatter : outputFormatter;
	
	fAttrList = new StringList(8, false);
	fTempResult = new XMLCh[50];
	fIndentChars = new XMLCh[100];
	fBaseUri = 0;
	
	XMLString::copyString(fIndentChars, XMLUni::fgZeroLenString);
	
	fIndent = 0;
	fIndentCap = 100;
	fAnonNum = 1000;
	
	fIdMap = new RefHashTableOf<XMLCh>(101, false);
	fDefinedIds = new RefVectorOf<XSObject>(25, false);
	fIdNames = new RefArrayVectorOf<XMLCh>(25, true);
	fObjectLocations = new RefArrayVectorOf<XMLCh>(25, true);
	
	fPrefixMap = new RefHashTableOf<XMLCh>(5, false);
	fNamespaces = new RefArrayVectorOf<XMLCh>(5, false);
	
	fNSAttributes = new ValueVectorOf<unsigned int>(15);
	fElementChildren = new ValueStackOf<bool>(20);
	

	fAttributesInfo = new RefVectorOf<AttrInfo>(8, true);
}

PSVIWriterHandlers::~PSVIWriterHandlers() {
	if (fBaseUri != NULL)
		XMLString::release(&fBaseUri);
	delete fAttrList;
	delete[] fTempResult;
	delete[] fIndentChars;
	
	delete fIdMap;
	delete fDefinedIds;
	delete fIdNames;
	delete fObjectLocations;
	
	delete fPrefixMap;
	delete fNamespaces;
	
	delete fNSAttributes;
	delete fElementChildren;
	
	delete fAttributesInfo;

}

// -----------------------------------------------------------------------
//  Convenience Utility
// -----------------------------------------------------------------------

void PSVIWriterHandlers::resetPSVIFormatter(XMLFormatter* outputFormatter) {
	fFormatter = outputFormatter;	
}

void PSVIWriterHandlers::resetDocument() {

	fIndent = 0;
	fAnonNum = 1000;
	XMLString::copyString(fIndentChars, XMLUni::fgZeroLenString);
	
	if (fBaseUri != NULL)
		XMLString::release(&fBaseUri);
	
	fIdMap->removeAll();
	fDefinedIds->removeAllElements();
	fIdNames->removeAllElements();
	fObjectLocations->removeAllElements();
	
	fPrefixMap->removeAll();
	fNamespaces->removeAllElements();

	fElementChildren->removeAllElements();
}

// ---------------------------------------------------------------------------
//  PSVIWriterHandlers: Implementation of the SAX DocumentHandler interface
// ---------------------------------------------------------------------------
void PSVIWriterHandlers::startElement(	const XMLCh* const uri,
										const XMLCh* const localname,
										const XMLCh* const qname,
										const Attributes& attrs) {
	fAttributesInfo->removeAllElements();
	for (unsigned int i=0; i<attrs.getLength(); i++) {
		fAttributesInfo->addElement(
			new AttrInfo(
				attrs.getURI(i),
				attrs.getLocalName(i),
				attrs.getType(i),
				attrs.getValue(i)
			)
		);
	}
}

void PSVIWriterHandlers::endElement(	const XMLCh* const uri, 
										const XMLCh* const localname, 
										const XMLCh* const qname) {
}

void PSVIWriterHandlers::startDocument() {
	fAttrList->removeAllElements();
	fAttrList->addElement((XMLCh*)gXmlnsColonXsi);
	fAttrList->addElement((XMLCh*)PSVIUni::fgNamespaceInstance);
	fAttrList->addElement((XMLCh*)gXmlnsColonPsv);
	fAttrList->addElement((XMLCh*)PSVIUni::fgNamespacePsvi);
	fAttrList->addElement((XMLCh*)XMLUni::fgXMLNSString);
	fAttrList->addElement((XMLCh*)PSVIUni::fgNamespaceInfoset);
	
	writeOpen(PSVIUni::fgDocument, fAttrList);	
	incIndent();
	sendElementValue(PSVIUni::fgCharacterEncodingScheme, fFormatter->getEncodingName());
	sendElementEmpty(PSVIUni::fgStandalone);
	sendElementValue(PSVIUni::fgVersion, PSVIUni::fgOnePointZero);
	
	fElementChildren->push(false);
}

void PSVIWriterHandlers::endDocument() {
	processChildrenEnd();
	sendElementEmpty(PSVIUni::fgDocumentElement);
	writeEmpty(PSVIUni::fgNotations);
	writeEmpty(PSVIUni::fgUnparsedEntities);
	sendElementValue(PSVIUni::fgBaseURI, fBaseUri);
	sendElementValue(PSVIUni::fgAllDeclarationsProcessed, PSVIUni::fgTrue);
	sendUnindentedElement(PSVIUni::fgDocument);
	resetDocument();
}

void PSVIWriterHandlers::characters(	const XMLCh* const chars,
									const unsigned int length) {
	processChildren();
	sendIndentedElement(PSVIUni::fgCharacter);
	sendElementValue(PSVIUni::fgTextContent, chars);
	sendUnindentedElement(PSVIUni::fgCharacter);
}

void PSVIWriterHandlers::ignorableWhitespace(	const XMLCh* const chars,
												const unsigned int length) {
	//ignore it  
}

void PSVIWriterHandlers::comment(const XMLCh* const chars, const unsigned int length) {
	processChildren();
	sendIndentedElement(PSVIUni::fgComment);
	sendElementValue(PSVIUni::fgContent, chars);
	sendUnindentedElement(PSVIUni::fgComment);
}

void PSVIWriterHandlers::processingInstruction(	const XMLCh* const target, 
												const XMLCh* const data) {
	processChildren();
	sendIndentedElement(PSVIUni::fgProcessingInstruction);
	sendElementValue(PSVIUni::fgTarget, target);
	sendElementValue(PSVIUni::fgContent, data);
	sendUnindentedElement(PSVIUni::fgProcessingInstruction);
}

void PSVIWriterHandlers::startPrefixMapping(const XMLCh* const prefix, const XMLCh* const uri) {
	if (!fPrefixMap->containsKey(uri)) {
		XMLCh* permaUri = XMLString::replicate((XMLCh*)uri);
		XMLCh* permaPrefix = XMLString::replicate((XMLCh*)prefix);
		fNamespaces->addElement(permaUri);
		fPrefixMap->put(permaUri, permaPrefix);
	}
}

void PSVIWriterHandlers::endPrefixMapping(const XMLCh* const prefix) {
	for (unsigned int i=0; i < fNamespaces->size(); i++) {
		if (XMLString::equals(fPrefixMap->get(fNamespaces->elementAt(i)), prefix)) {
			XMLCh* uri = fNamespaces->elementAt(i);
			XMLCh* pre = fPrefixMap->get(uri);
			fPrefixMap->removeKey(uri);
			fNamespaces->removeElementAt(i);
			XMLString::release(&uri);
			XMLString::release(&pre);
			break;
		}
	}
}

InputSource* PSVIWriterHandlers::resolveEntity(XMLResourceIdentifier* resourceIdentifier) {
	if (fBaseUri != NULL)
		XMLString::release(&fBaseUri);
	fBaseUri = XMLString::replicate(resourceIdentifier->getBaseURI());
	return 0;
}

// ---------------------------------------------------------------------------
//  PSVIWriterHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void PSVIWriterHandlers::error(const SAXParseException& e) {
	XMLCh* temp1 = new XMLCh[10];
	XMLCh* temp2 = new XMLCh[10];
	XMLString::binToText(e.getLineNumber(), temp1, 9, 10);
	XMLString::binToText(e.getColumnNumber(), temp2, 9, 10);
	*fErrorFormatter << fgError << fgAtFile << chSpace << e.getSystemId()
		<< chComma << fgLine << chSpace << temp1
		<< chComma << fgChar << chSpace << temp2
		<< chLF << fgMessage << chColon << e.getMessage() << chLF;
	delete[] temp1;
	delete[] temp2;
}

void PSVIWriterHandlers::fatalError(const SAXParseException& e) {
	XMLCh* temp1 = new XMLCh[10];
	XMLCh* temp2 = new XMLCh[10];
	XMLString::binToText(e.getLineNumber(), temp1, 9, 10);
	XMLString::binToText(e.getColumnNumber(), temp2, 9, 10);
	*fErrorFormatter << fgFatalError << fgAtFile << chSpace << e.getSystemId()
		<< chComma << fgLine << chSpace << temp1
		<< chComma << fgChar << chSpace << temp2
		<< chLF	<< fgMessage << chColon << e.getMessage() << chLF;
	delete[] temp1;
	delete[] temp2;
	resetDocument();
}

void PSVIWriterHandlers::warning(const SAXParseException& e) {
	XMLCh* temp1 = new XMLCh[10];
	XMLCh* temp2 = new XMLCh[10];
	XMLString::binToText(e.getLineNumber(), temp1, 9, 10);
	XMLString::binToText(e.getColumnNumber(), temp2, 9, 10);
	*fErrorFormatter << fgWarning << fgAtFile << chSpace << e.getSystemId()
		<< chComma << fgLine << chSpace << temp1
		<< chComma << fgChar << chSpace << temp2
		<< chLF	<< fgMessage << chColon << e.getMessage() << chLF;
	delete[] temp1;
	delete[] temp2;
}

void PSVIWriterHandlers::resetErrors()
{   
}

// ---------------------------------------------------------------------------
//  PSVIWriterHandlers: Overrides of the PSVIHandler interface
// ---------------------------------------------------------------------------

void PSVIWriterHandlers::handleAttributesPSVI(	const XMLCh* const localName, 
												const XMLCh* const uri, 
												PSVIAttributeList* psviAttributes ) {

	processChildren();
	fElementChildren->push(false);
	sendIndentedElement(PSVIUni::fgElement);
	sendElementValue(PSVIUni::fgNamespaceName, uri);
	sendElementValue(PSVIUni::fgLocalName, localName);
	sendElementValue(PSVIUni::fgPrefix, fPrefixMap->get(uri));
	processAttributes(psviAttributes, fAttributesInfo);
	processInScopeNamespaces();
	sendElementValue(PSVIUni::fgBaseURI, fBaseUri);
}

void PSVIWriterHandlers::handleElementPSVI(	const XMLCh* const localName, 
											const XMLCh* const uri,
											PSVIElement* elementInfo ) {
	processChildrenEnd();
	processSchemaInformation(elementInfo->getSchemaInformation());
	sendElementValue( PSVIUni::fgValidationAttempted,
		translateValidationAttempted(elementInfo->getValidationAttempted()));
	sendElementValue(PSVIUni::fgValidationContext, elementInfo->getValidationContext());
	sendElementValue(PSVIUni::fgValidity, translateValidity(elementInfo->getValidity()));
	//REVISIT errorCodes not supported
	//processSchemaErrorCode(elementInfo->getErrorCodes());
	sendElementEmpty(PSVIUni::fgSchemaErrorCode);
	sendElementValue(PSVIUni::fgSchemaNormalizedValue, elementInfo->getSchemaNormalizedValue());
	sendElementValue(PSVIUni::fgSchemaSpecified,
		(elementInfo->getIsSchemaSpecified() ? PSVIUni::fgSchema : PSVIUni::fgInfoset));
	sendElementValue(PSVIUni::fgSchemaDefault, elementInfo->getSchemaDefault());
	processTypeDefinitionRef(PSVIUni::fgTypeDefinition, elementInfo->getTypeDefinition());
	processTypeDefinitionRef(PSVIUni::fgMemberTypeDefinition, elementInfo->getMemberTypeDefinition());
	sendElementEmpty(PSVIUni::fgNil);
	processElementDeclarationRef(PSVIUni::fgDeclaration, elementInfo->getElementDeclaration());
	sendReference(PSVIUni::fgNotation, elementInfo->getNotationDeclaration());
	sendElementEmpty(PSVIUni::fgIdIdrefTable);
	sendElementEmpty(PSVIUni::fgIdentityConstraintTable);
	sendUnindentedElement(PSVIUni::fgElement);
}

// ---------------------------------------------------------------------------
//  Private methods
// ---------------------------------------------------------------------------

void PSVIWriterHandlers::processAttributes(PSVIAttributeList* psviAttributes, const RefVectorOf<AttrInfo>* attributesInfo) {
	fNSAttributes->removeAllElements();  //will store the indecies of namespace attributes
	bool firstFlag = true;
	for (unsigned int i = 0; i < attributesInfo->size(); i++) {
		if (attributesInfo->elementAt(i)->getUri() == XMLUni::fgXMLNSURIName) {
			fNSAttributes->addElement(i);
		} else {
			if (firstFlag) {
				sendIndentedElement(PSVIUni::fgAttributes);
				firstFlag = false;
			}
			const XMLCh* localName = attributesInfo->elementAt(i)->getLocalName();
			const XMLCh* namespaceUri = attributesInfo->elementAt(i)->getUri();
			sendIndentedElement(PSVIUni::fgAttribute);
			sendElementValue(PSVIUni::fgNamespaceName, namespaceUri);
			sendElementValue(PSVIUni::fgLocalName, localName);
			sendElementValue(PSVIUni::fgPrefix, fPrefixMap->get(attributesInfo->elementAt(i)->getUri()));
			sendElementValue(PSVIUni::fgNormalizedValue, attributesInfo->elementAt(i)->getValue());
			if (psviAttributes!=NULL && psviAttributes->getAttributePSVIAtIndex(i)!=NULL) {
				sendElementValue(PSVIUni::fgSpecified, 
					translateBool(!(psviAttributes->getAttributePSVIAtIndex(i)->getIsSchemaSpecified())));
			} else //the desired value is !schemaSpecified
				sendElementValue(PSVIUni::fgSpecified, PSVIUni::fgUnknown);
			sendElementValue(PSVIUni::fgAttributeType, attributesInfo->elementAt(i)->getType());
			sendElementEmpty(PSVIUni::fgReferences);
			PSVIAttribute* psviAttr = psviAttributes->getAttributePSVIByName(localName, namespaceUri);
			processAttributePSVI(psviAttr);
			sendUnindentedElement(PSVIUni::fgAttribute);
		}
	}
	if (firstFlag)
		writeEmpty(PSVIUni::fgAttributes);
	else
		sendUnindentedElement(PSVIUni::fgAttributes);
	processNamespaceAttributes(psviAttributes, attributesInfo);
}

void PSVIWriterHandlers::processNamespaceAttributes(PSVIAttributeList* psviAttributes, const RefVectorOf<AttrInfo>* attributes) {
	if (fNSAttributes->size()==0) {
		writeEmpty(PSVIUni::fgNamespaceAttributes);
	} else {
		sendIndentedElement(PSVIUni::fgNamespaceAttributes);
		int ind;
		for (unsigned int count = 0; count < fNSAttributes->size(); count++) {
			ind = fNSAttributes->elementAt(count);
			sendIndentedElement(PSVIUni::fgAttribute);
			sendElementValue(PSVIUni::fgNamespaceName, XMLUni::fgXMLNSURIName);
			sendElementValue(PSVIUni::fgLocalName, attributes->elementAt(ind)->getLocalName());
			sendElementValue(PSVIUni::fgPrefix, XMLUni::fgXMLNSString);
			sendElementValue(PSVIUni::fgNormalizedValue, attributes->elementAt(ind)->getValue());
			if (psviAttributes!=NULL) {
				sendElementValue(PSVIUni::fgSpecified, 
					translateBool(
						psviAttributes->getAttributePSVIByName(
							attributes->elementAt(ind)->getLocalName(),
							attributes->elementAt(ind)->getUri()
						)->getIsSchemaSpecified()
					)
				);
			} else
				sendElementValue(PSVIUni::fgSpecified, PSVIUni::fgUnknown);
			sendElementValue(PSVIUni::fgAttributeType, attributes->elementAt(ind)->getType());
			// this property isn't relevent to PSVI
			sendElementEmpty(PSVIUni::fgReferences);
			sendUnindentedElement(PSVIUni::fgAttribute);
		}
		sendUnindentedElement(PSVIUni::fgNamespaceAttributes);
	}
}

void PSVIWriterHandlers::processAttributePSVI(PSVIAttribute* attrPSVI) {
	if (attrPSVI != NULL) {
		sendElementValue(PSVIUni::fgValidationAttempted,
			translateValidationAttempted(attrPSVI->getValidationAttempted()));
		sendElementValue(PSVIUni::fgValidationContext, attrPSVI->getValidationContext());
		sendElementValue(PSVIUni::fgValidity, translateValidity(attrPSVI->getValidity()));
		//REVISIT errorCodes not supported
		//processSchemaErrorCode(attrPSVI->getErrorCodes());
		sendElementEmpty(PSVIUni::fgSchemaErrorCode);
		sendElementValue(PSVIUni::fgSchemaNormalizedValue, attrPSVI->getSchemaNormalizedValue());
		sendElementValue(PSVIUni::fgSchemaSpecified,
			(attrPSVI->getIsSchemaSpecified() ? PSVIUni::fgSchema : PSVIUni::fgInfoset));
		sendElementValue(PSVIUni::fgSchemaDefault, attrPSVI->getSchemaDefault());
		processTypeDefinitionRef(PSVIUni::fgTypeDefinition, attrPSVI->getTypeDefinition());
		processTypeDefinitionOrRef(PSVIUni::fgMemberTypeDefinition, attrPSVI->getMemberTypeDefinition());
		processAttributeDeclarationRef(PSVIUni::fgDeclaration, attrPSVI->getAttributeDeclaration());
	}
}

void PSVIWriterHandlers::processInScopeNamespaces() {
	sendIndentedElement(PSVIUni::fgInScopeNamespaces);
	sendIndentedElement(PSVIUni::fgNamespace);
	sendElementValue(PSVIUni::fgPrefix, PSVIUni::fgXml);
	sendElementValue(PSVIUni::fgNamespaceName, XMLUni::fgXMLURIName);
	sendUnindentedElement(PSVIUni::fgNamespace);
	for (unsigned int i=0; i<fNamespaces->size(); i++) {
		sendIndentedElement(PSVIUni::fgNamespace);
		sendElementValue(PSVIUni::fgPrefix, fPrefixMap->get(fNamespaces->elementAt(i)));
		sendElementValue(PSVIUni::fgNamespaceName, fNamespaces->elementAt(i));
		sendUnindentedElement(PSVIUni::fgNamespace);
	}
	sendUnindentedElement(PSVIUni::fgInScopeNamespaces);
}

void PSVIWriterHandlers::processSchemaInformation(XSModel* model) {
	if (fElementChildren->size()!=1 || model==NULL) {
		sendElementEmpty(PSVIUni::fgSchemaInformation);
	} else {
		sendIndentedElement(PSVIUni::fgSchemaInformation);
		XSNamespaceItemList* namespaceItems = model->getNamespaceItems();
		for (unsigned int i=0; i < namespaceItems->size(); i++) {
			processNamespaceItem(namespaceItems->elementAt(i));	
		}
		sendUnindentedElement(PSVIUni::fgSchemaInformation);
	}
}

void PSVIWriterHandlers::processNamespaceItem(XSNamespaceItem* namespaceItem) {
	if (!XMLString::equals(namespaceItem->getSchemaNamespace(), PSVIUni::fgNamespaceXmlSchema)) {
		sendIndentedElement(PSVIUni::fgNamespaceSchemaInformation);
		sendElementValue(PSVIUni::fgSchemaNamespace, namespaceItem->getSchemaNamespace());
		processSchemaComponents(namespaceItem);
		processSchemaDocuments(namespaceItem);
		processSchemaAnnotations(namespaceItem->getAnnotations());
		sendUnindentedElement(PSVIUni::fgNamespaceSchemaInformation);
	}
}

void PSVIWriterHandlers::processSchemaComponents(XSNamespaceItem* namespaceItem) {
	sendIndentedElement(PSVIUni::fgSchemaComponents);
	XSNamedMap<XSTypeDefinition>* types = (XSNamedMap<XSTypeDefinition>*)(namespaceItem->getComponents(XSConstants::TYPE_DEFINITION));
	for (unsigned int typeCount = 0; typeCount < types->getLength(); typeCount++) {
		processTypeDefinition(types->item(typeCount));
	}
	XSNamedMap<XSAttributeDeclaration>* attributes = (XSNamedMap<XSAttributeDeclaration>*)namespaceItem->getComponents(XSConstants::ATTRIBUTE_DECLARATION);
	for (unsigned int attrCount = 0; attrCount < attributes->getLength(); attrCount++) {
		processAttributeDeclaration(attributes->item(attrCount));
	}
	XSNamedMap<XSElementDeclaration>* elements = (XSNamedMap<XSElementDeclaration>*)namespaceItem->getComponents(XSConstants::ELEMENT_DECLARATION);
	for (unsigned int elemCount = 0; elemCount < elements->getLength(); elemCount++) {
		processElementDeclaration(elements->item(elemCount));
	}
	XSNamedMap<XSAttributeGroupDefinition>* attrGroups = (XSNamedMap<XSAttributeGroupDefinition>*)namespaceItem->getComponents(XSConstants::ATTRIBUTE_GROUP_DEFINITION);
	for (unsigned int attrGroupCount = 0; attrGroupCount < attrGroups->getLength(); attrGroupCount++) {
		processAttributeGroupDefinition(attrGroups->item(attrGroupCount));
	}
	XSNamedMap<XSModelGroupDefinition>* modelGroups = (XSNamedMap<XSModelGroupDefinition>*)namespaceItem->getComponents(XSConstants::MODEL_GROUP_DEFINITION);
	for (unsigned int modelGroupCount = 0; modelGroupCount < modelGroups->getLength(); modelGroupCount++) {
		processModelGroupDefinition(modelGroups->item(modelGroupCount));
	}
	XSNamedMap<XSNotationDeclaration>* notations = (XSNamedMap<XSNotationDeclaration>*)namespaceItem->getComponents(XSConstants::NOTATION_DECLARATION);
	for (unsigned int notationCount = 0; notationCount < notations->getLength(); notationCount++) {
		processNotationDeclaration(notations->item(notationCount));
	}
	sendUnindentedElement(PSVIUni::fgSchemaComponents);
}

void PSVIWriterHandlers::processSchemaDocuments(XSNamespaceItem* namespaceItem) {
	StringList* locations = namespaceItem->getDocumentLocations();
	if (locations==NULL) {
		sendElementEmpty(PSVIUni::fgSchemaDocuments);
	}
	sendIndentedElement(PSVIUni::fgSchemaDocuments);
	for (unsigned int i = 0; i < locations->size(); i++) {
		sendIndentedElement(PSVIUni::fgSchemaDocument);
		sendElementValue(PSVIUni::fgDocumentLocation, locations->elementAt(i));
		sendElementEmpty(PSVIUni::fgPsvDocument); //supposed to point to a document element, but we dont deal with them
		sendUnindentedElement(PSVIUni::fgSchemaDocument);
	}
	sendUnindentedElement(PSVIUni::fgSchemaDocuments);
}

void PSVIWriterHandlers::processSchemaAnnotations(XSAnnotationList* annotations) {
	if (annotations == NULL || annotations->size()==0) {
		sendElementEmpty(PSVIUni::fgSchemaAnnotations);	
	} else {
		sendIndentedElement(PSVIUni::fgSchemaAnnotations);
		for (unsigned int i = 0; i < annotations->size(); i++) {
			processAnnotation(annotations->elementAt(i));
		}
		sendUnindentedElement(PSVIUni::fgSchemaAnnotations);
	}
}

void PSVIWriterHandlers::processSchemaErrorCode(StringList* errors) {
	//REVISIT
	//ErrorCodes not yet supported
}

void PSVIWriterHandlers::processTypeDefinition(XSTypeDefinition* type) {
	if (type->getTypeCategory() == XSTypeDefinition::COMPLEX_TYPE) {
		processComplexTypeDefinition((XSComplexTypeDefinition*)type);
	} else { //XSTypeDefinition::SIMPLE_TYPE
		processSimpleTypeDefinition((XSSimpleTypeDefinition*)type);
	}
}

void PSVIWriterHandlers::processComplexTypeDefinition(XSComplexTypeDefinition* complexType) {
	sendIndentedElementWithID(PSVIUni::fgComplexTypeDefinition, (XSObject*) complexType);
	if (complexType->getAnonymous())
		sendElementEmpty(PSVIUni::fgName);
	else sendElementValue(PSVIUni::fgName, complexType->getName());
	sendElementValue(PSVIUni::fgTargetNamespace, complexType->getNamespace());
	processTypeDefinitionOrRef(PSVIUni::fgBaseTypeDefinition, complexType->getBaseType());
	sendElementValue(PSVIUni::fgDerivationMethod,
		translateDerivationMethod(complexType->getDerivationMethod()));
	sendElementValue(PSVIUni::fgFinal, translateBlockOrFinal(complexType->getFinal()));
	sendElementValue(PSVIUni::fgAbstract, translateBool(complexType->getAbstract()));
	processAttributeUses(complexType->getAttributeUses());
	processAttributeWildcard(complexType->getAttributeWildcard());
	
	sendIndentedElement(PSVIUni::fgContentType);
	sendElementValue(PSVIUni::fgVariety, translateComplexContentType(complexType->getContentType()));
	if (complexType->getSimpleType()==NULL) {
		sendElementEmpty(PSVIUni::fgSimpleTypeDefinition);
	} else {
		processSimpleTypeDefinitionOrRef(complexType->getSimpleType());
	}
	processParticle(complexType->getParticle());
	sendUnindentedElement(PSVIUni::fgContentType);
	sendElementValue(PSVIUni::fgProhibitedSubstitutions, 
		translateBlockOrFinal(complexType->getProhibitedSubstitutions()));
	processAnnotations(complexType->getAnnotations());
	sendUnindentedElement(PSVIUni::fgComplexTypeDefinition);
}

void PSVIWriterHandlers::processSimpleTypeDefinition(XSSimpleTypeDefinition* simpleType) {
	sendIndentedElementWithID(PSVIUni::fgSimpleTypeDefinition, (XSObject*) simpleType);
	if (simpleType->getAnonymous())
		sendElementEmpty(PSVIUni::fgName);
	else sendElementValue(PSVIUni::fgName, simpleType->getName());
	sendElementValue(PSVIUni::fgTargetNamespace, simpleType->getNamespace());
	processTypeDefinitionOrRef(PSVIUni::fgBaseTypeDefinition, simpleType->getBaseType());
	processTypeDefinitionOrRef(PSVIUni::fgPrimitiveTypeDefinition, simpleType->getPrimitiveType());
	processFacets(simpleType->getFacets(), simpleType->getMultiValueFacets());
	processFundamentalFacets(simpleType);
	sendElementValue(PSVIUni::fgFinal, translateBlockOrFinal(simpleType->getFinal()));
	sendElementValue(PSVIUni::fgVariety, translateSimpleTypeVariety(simpleType->getVariety()));
	processTypeDefinitionOrRef(PSVIUni::fgItemTypeDefinition, simpleType->getItemType());
	processMemberTypeDefinitions(simpleType->getMemberTypes());
	processAnnotations(simpleType->getAnnotations());
	sendUnindentedElement(PSVIUni::fgSimpleTypeDefinition);
}

void PSVIWriterHandlers::processModelGroupDefinition(XSModelGroupDefinition* modelGroup) {
	if (modelGroup == NULL) {
		sendElementEmpty(PSVIUni::fgModelGroupDefinition);
	} else {
		sendIndentedElementWithID(PSVIUni::fgModelGroupDefinition, (XSObject*) modelGroup);
		sendElementValue(PSVIUni::fgName, modelGroup->getName());
		sendElementValue(PSVIUni::fgTargetNamespace, modelGroup->getNamespace());
		processModelGroup(modelGroup->getModelGroup());
		processAnnotation(modelGroup->getAnnotation());
		sendUnindentedElement(PSVIUni::fgModelGroupDefinition);
	}
}

void PSVIWriterHandlers::processAttributeGroupDefinition(XSAttributeGroupDefinition* attributeGroup) {
	if (attributeGroup == NULL) {
		sendElementEmpty(PSVIUni::fgAttributeGroupDefinition);
	} else {
		sendIndentedElementWithID(PSVIUni::fgAttributeGroupDefinition, (XSObject*) attributeGroup);
		sendElementValue(PSVIUni::fgName, attributeGroup->getName());
		sendElementValue(PSVIUni::fgTargetNamespace, attributeGroup->getNamespace());
		processAttributeUses(attributeGroup->getAttributeUses());
		processAttributeWildcard(attributeGroup->getAttributeWildcard());
		processAnnotation(attributeGroup->getAnnotation());
		sendUnindentedElement(PSVIUni::fgAttributeGroupDefinition);
	}
}

void PSVIWriterHandlers::processElementDeclaration(XSElementDeclaration* element) {
	if (element == NULL) {
		sendElementEmpty(PSVIUni::fgElementDeclaration);
	} else {
		sendIndentedElementWithID(PSVIUni::fgElementDeclaration, (XSObject*) element);
		sendElementValue(PSVIUni::fgName, element->getName());
		sendElementValue(PSVIUni::fgTargetNamespace, element->getNamespace());
		processTypeDefinitionOrRef(PSVIUni::fgTypeDefinition, element->getTypeDefinition());
		processScope(element->getEnclosingCTDefinition(), element->getScope());
		processValueConstraint(element->getConstraintType(), element->getConstraintValue());
		sendElementValue(PSVIUni::fgNillable, translateBool(element->getNillable()));
		processIdentityConstraintDefinition(element->getIdentityConstraints());
		processElementDeclarationRef(PSVIUni::fgSubstitutionGroupAffiliation, 
			element->getSubstitutionGroupAffiliation());
		sendElementValue(PSVIUni::fgSubstitutionGroupExclusions, 
			translateBlockOrFinal(element->getSubstitutionGroupExclusions()));
		sendElementValue(PSVIUni::fgDisallowedSubstitutions, 
			translateBlockOrFinal(element->getDisallowedSubstitutions()));
		sendElementValue(PSVIUni::fgAbstract, translateBool(element->getAbstract()));
		processAnnotation(element->getAnnotation());
		sendUnindentedElement(PSVIUni::fgElementDeclaration);
	}
}

void PSVIWriterHandlers::processAttributeDeclaration(XSAttributeDeclaration* attribute) {
	if (attribute == NULL) {
		sendElementEmpty(PSVIUni::fgAttributeDeclaration);
	} else {
		sendIndentedElementWithID(PSVIUni::fgAttributeDeclaration, (XSObject*) attribute);
		sendElementValue(PSVIUni::fgName, attribute->getName());
		sendElementValue(PSVIUni::fgTargetNamespace, attribute->getNamespace());
		sendIndentedElement(PSVIUni::fgTypeDefinition);
		processSimpleTypeDefinitionOrRef(attribute->getTypeDefinition());
		sendUnindentedElement(PSVIUni::fgTypeDefinition);
		processScope(attribute->getEnclosingCTDefinition(), attribute->getScope());
		processValueConstraint(attribute->getConstraintType(), attribute->getConstraintValue());
		processAnnotation(attribute->getAnnotation());
		sendUnindentedElement(PSVIUni::fgAttributeDeclaration);
	}
}

void PSVIWriterHandlers::processNotationDeclaration(XSNotationDeclaration* notation) {
	if (notation == NULL) {
		sendElementEmpty(PSVIUni::fgNotationDeclaration);
	} else {
		sendIndentedElementWithID(PSVIUni::fgNotationDeclaration, (XSObject*) notation);
		sendElementValue(PSVIUni::fgName, notation->getName());
		sendElementValue(PSVIUni::fgTargetNamespace, notation->getNamespace());
		sendElementValue(PSVIUni::fgSystemIdentifier, notation->getSystemId());
		sendElementValue(PSVIUni::fgPublicIdentifier, notation->getPublicId());
		processAnnotation(notation->getAnnotation());
		sendUnindentedElement(PSVIUni::fgNotationDeclaration);
	}
}

void PSVIWriterHandlers::processAnnotations(XSAnnotationList* annotations) {
	if (annotations == NULL) {
		sendElementEmpty(PSVIUni::fgAnnotations);
	} else {
		sendIndentedElement(PSVIUni::fgAnnotations);
		for (unsigned int i = 0; i < annotations->size(); i++) {
			processAnnotation(annotations->elementAt(i));
		}
		sendUnindentedElement(PSVIUni::fgAnnotations);
	}
}

void PSVIWriterHandlers::processAttributeUses(XSAttributeUseList* attributeUses) {
	if (attributeUses == NULL) {
		sendElementEmpty(PSVIUni::fgAttributeUses);
	} else {
		sendIndentedElement(PSVIUni::fgAttributeUses);
		XSAttributeUse* attrUse;
		for (unsigned int i=0; i < attributeUses->size(); i++) {
			attrUse = attributeUses->elementAt(i);
			sendIndentedElement(PSVIUni::fgAttributeUse);
			sendElementValue(PSVIUni::fgRequired, translateBool(attrUse->getRequired()));
			processAttributeDeclarationOrRef(attrUse->getAttrDeclaration()); 
			processValueConstraint(attrUse->getConstraintType(), attrUse->getConstraintValue());
			sendUnindentedElement(PSVIUni::fgAttributeUse);
		}
		sendUnindentedElement(PSVIUni::fgAttributeUses);
	}
}

void PSVIWriterHandlers::processFacets(XSFacetList* facets, XSMultiValueFacetList* multiFacets) {
	if (facets == NULL && multiFacets == NULL) {
		sendElementEmpty(PSVIUni::fgFacets);	
	} else {
		sendIndentedElement(PSVIUni::fgFacets);
		if (facets != NULL) {
			for (unsigned int facetCount = 0; facetCount < facets->size(); facetCount++) {
				XSFacet* facet = facets->elementAt(facetCount);
				sendIndentedElement(translateFacet(facet->getFacetKind()));
				sendElementValue(PSVIUni::fgValue, facet->getLexicalFacetValue());
				sendElementValue(PSVIUni::fgFixed, translateBool(facet->isFixed()));
				processAnnotation(facet->getAnnotation());
				sendUnindentedElement(translateFacet(facet->getFacetKind()));
			}	
		}
		if (multiFacets != NULL) {
			for (unsigned int multiFacetCount = 0; multiFacetCount < multiFacets->size(); multiFacetCount++) {
				XSMultiValueFacet* multiFacet = multiFacets->elementAt(multiFacetCount);
				sendIndentedElement(translateFacet(multiFacet->getFacetKind()));
				StringList* values = multiFacet->getLexicalFacetValues();
				for (unsigned int i=0; i < values->size(); i++) {
					sendElementValue(PSVIUni::fgValue, values->elementAt(i));
				}
				sendElementValue(PSVIUni::fgFixed, translateBool(multiFacet->isFixed()));
				processAnnotations(multiFacet->getAnnotations());
				sendUnindentedElement(translateFacet(multiFacet->getFacetKind()));
			}	
		}
		sendUnindentedElement(PSVIUni::fgFacets);
	}
}

void PSVIWriterHandlers::processFundamentalFacets(XSSimpleTypeDefinition* type) {
	sendIndentedElement(PSVIUni::fgFundamentalFacets);
	sendIndentedElement(PSVIUni::fgOrdered);
	sendElementValue(PSVIUni::fgValue, translateOrderedFacet(type->getOrdered()));
	sendUnindentedElement(PSVIUni::fgOrdered);
	sendIndentedElement(PSVIUni::fgBounded);
	sendElementValue(PSVIUni::fgValue, translateBool(type->getBounded()));
	sendUnindentedElement(PSVIUni::fgBounded);
	sendIndentedElement(PSVIUni::fgCardinality);
	sendElementValue(PSVIUni::fgValue, translateBool(type->getFinite()));
	sendUnindentedElement(PSVIUni::fgCardinality);
	sendIndentedElement(PSVIUni::fgNumeric);
	sendElementValue(PSVIUni::fgValue, translateBool(type->getNumeric()));
	sendUnindentedElement(PSVIUni::fgNumeric);
	sendUnindentedElement(PSVIUni::fgFundamentalFacets);
}

void PSVIWriterHandlers::processMemberTypeDefinitions(XSSimpleTypeDefinitionList* memberTypes) {
	if (memberTypes == NULL) {
		sendElementEmpty(PSVIUni::fgMemberTypeDefinitions);
	} else {
		sendIndentedElement(PSVIUni::fgMemberTypeDefinitions);
		for (unsigned int i = 0; i < memberTypes->size(); i++) {
			processTypeDefinitionOrRef(PSVIUni::fgMemberTypeDefinition, (XSTypeDefinition*)memberTypes->elementAt(i));
		}
		sendUnindentedElement(PSVIUni::fgMemberTypeDefinitions);
	}
}

void PSVIWriterHandlers::processAnnotation(XSAnnotation* annotation) {
	if (annotation == NULL) {
		sendElementEmpty(PSVIUni::fgAnnotation);
	} else {
		XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* document = DOMImplementationRegistry::getDOMImplementation(XMLUni::fgZeroLenString)->
                createDocument();
		annotation->writeAnnotation((DOMNode*)document, XSAnnotation::W3C_DOM_DOCUMENT);
		DOMElement* elem = document->getDocumentElement();
		sendIndentedElement(PSVIUni::fgAnnotation);
		processDOMElement(PSVIUni::fgApplicationInformation, elem, PSVIUni::fgAppinfo);
		processDOMElement(PSVIUni::fgUserInformation, elem, PSVIUni::fgDocumentation);
		processDOMAttributes(elem->getAttributes());
		sendUnindentedElement(PSVIUni::fgAnnotation);
	}
}

void PSVIWriterHandlers::processDOMElement(const XMLCh* const encloseName, DOMElement* rootElem, const XMLCh* const elementName) {
	DOMNodeList* elems = rootElem->getElementsByTagNameNS(SchemaSymbols::fgURI_SCHEMAFORSCHEMA, elementName);
	if (elems->getLength()==0) {
		sendElementEmpty(encloseName);
	} else {
		sendIndentedElement(encloseName);
		for (unsigned int i=0; i < elems->getLength(); i++) {
			DOMElement* elem = (DOMElement*)elems->item(i);
			sendIndentedElement(PSVIUni::fgElement);
			sendElementValue(PSVIUni::fgNamespaceName, elem->getNamespaceURI());
			sendElementValue(PSVIUni::fgLocalName, elem->getLocalName());
			sendElementValue(PSVIUni::fgPrefix, elem->getPrefix());
			sendIndentedElement(PSVIUni::fgChildren);
			sendIndentedElement(PSVIUni::fgCharacter);
			sendElementValue(PSVIUni::fgTextContent, elem->getTextContent());
			sendUnindentedElement(PSVIUni::fgCharacter);
			sendUnindentedElement(PSVIUni::fgChildren);
			processDOMAttributes(elem->getAttributes());
			sendUnindentedElement(PSVIUni::fgElement);
		}
		sendUnindentedElement(encloseName);
	}
}

void PSVIWriterHandlers::processDOMAttributes(DOMNamedNodeMap* attrs) {
	fNSAttributes->removeAllElements();
	bool firstFlag = true;
	for (unsigned int count=0; count < attrs->getLength(); count++) {
		DOMAttr* attr = (DOMAttr*)attrs->item(count);
		if (XMLString::equals(attr->getPrefix(), XMLUni::fgXMLNSString)) {
			fNSAttributes->addElement(count);
		} else {
			if (firstFlag) {
				sendIndentedElement(PSVIUni::fgAttributes);
				firstFlag = false;
			}
			sendIndentedElement(PSVIUni::fgAttribute);
			sendElementValue(PSVIUni::fgNamespaceName, attr->getNamespaceURI());
			sendElementValue(PSVIUni::fgLocalName, attr->getLocalName());
			sendElementValue(PSVIUni::fgPrefix, attr->getPrefix());
			sendElementValue(PSVIUni::fgNormalizedValue, attr->getValue());
			sendElementValue(PSVIUni::fgSpecified, translateBool(attr->getSpecified()));
			sendElementValue(PSVIUni::fgAttributeType, attr->getTypeInfo()->getName());
			sendElementEmpty(PSVIUni::fgReferences);
			sendUnindentedElement(PSVIUni::fgAttribute);
		}
	}
	if (firstFlag)
		writeEmpty(PSVIUni::fgAttributes);
	else 
		sendUnindentedElement(PSVIUni::fgAttributes);
	
	//now for namespace attributes
	if (fNSAttributes->size()==0) {
		writeEmpty(PSVIUni::fgNamespaceAttributes);
	} else {
		sendIndentedElement(PSVIUni::fgNamespaceAttributes);
		for (unsigned int NScount = 0; NScount < fNSAttributes->size(); NScount++) {	
			DOMAttr* attr = (DOMAttr*)attrs->item(fNSAttributes->elementAt(NScount));
			sendIndentedElement(PSVIUni::fgAttribute);
			sendElementValue(PSVIUni::fgNamespaceName, XMLUni::fgXMLNSURIName);
			sendElementValue(PSVIUni::fgLocalName, attr->getLocalName());
			sendElementValue(PSVIUni::fgPrefix, XMLUni::fgXMLNSString);
			sendElementValue(PSVIUni::fgNormalizedValue, attr->getValue());
			sendElementValue(PSVIUni::fgSpecified, translateBool(attr->getSpecified()));
			sendElementValue(PSVIUni::fgAttributeType, attr->getTypeInfo()->getName());
			sendElementEmpty(PSVIUni::fgReferences);
			sendUnindentedElement(PSVIUni::fgAttribute);
		}
		sendUnindentedElement(PSVIUni::fgNamespaceAttributes);
	}
}

void PSVIWriterHandlers::processWildcard(XSWildcard* wildcard) {
	if (wildcard == NULL) {
		sendElementEmpty(PSVIUni::fgWildcard);
	} else {
		sendIndentedElement(PSVIUni::fgWildcard);
		sendIndentedElement(PSVIUni::fgNamespaceConstraint);
		if (wildcard->getConstraintType()==XSWildcard::NSCONSTRAINT_ANY) {
			sendElementValue(PSVIUni::fgVariety, PSVIUni::fgAny);
			sendElementEmpty(PSVIUni::fgNamespaces);
		} else {
			if (wildcard->getConstraintType()==XSWildcard::NSCONSTRAINT_DERIVATION_LIST) {
				sendElementEmpty(PSVIUni::fgVariety);
				sendElementValueList(PSVIUni::fgNamespaces, wildcard->getNsConstraintList());
			} else {  //NSCONSTRAINT_NOT
				sendElementValue(PSVIUni::fgVariety, PSVIUni::fgNot);
				sendElementValueList(PSVIUni::fgNamespaces, wildcard->getNsConstraintList());	
			}
		}
		sendUnindentedElement(PSVIUni::fgNamespaceConstraint);
		sendElementValue(PSVIUni::fgProcessContents, 
			translateProcessContents(wildcard->getProcessContents()));
		processAnnotation(wildcard->getAnnotation());
		sendUnindentedElement(PSVIUni::fgWildcard);
	}
}

void PSVIWriterHandlers::processModelGroup(XSModelGroup* modelGroup) {
	if (modelGroup == NULL) {
		sendElementEmpty(PSVIUni::fgModelGroup);
	} else {
		sendIndentedElement(PSVIUni::fgModelGroup);
		sendElementValue(PSVIUni::fgCompositor, translateCompositor(modelGroup->getCompositor()));
		sendIndentedElement(PSVIUni::fgParticles);
		for (unsigned int i=0; i < modelGroup->getParticles()->size(); i++) {
			processParticle(modelGroup->getParticles()->elementAt(i));	
		}
		sendUnindentedElement(PSVIUni::fgParticles);
		processAnnotation(modelGroup->getAnnotation());
		sendUnindentedElement(PSVIUni::fgModelGroup);
	}
}

void PSVIWriterHandlers::processParticle(XSParticle* particle) {
	if (particle == NULL) {
		sendElementEmpty(PSVIUni::fgParticle);
	} else {
		sendIndentedElement(PSVIUni::fgParticle);
		sendElementValueInt(PSVIUni::fgMinOccurs, particle->getMinOccurs());
		if (particle->getMaxOccursUnbounded()) {
			sendElementValue(PSVIUni::fgMaxOccurs, PSVIUni::fgUnbounded);
		} else { 
			sendElementValueInt(PSVIUni::fgMaxOccurs,particle->getMaxOccurs());
		}
		sendIndentedElement(PSVIUni::fgTerm);
		switch (particle->getTermType()) {
                                                case XSParticle::TERM_ELEMENT:
				processElementDeclarationOrRef(particle->getElementTerm());
				break;
                                               case XSParticle::TERM_MODELGROUP:
				processModelGroup(particle->getModelGroupTerm());
				break;
                                               case XSParticle::TERM_WILDCARD:
				processWildcard(particle->getWildcardTerm());
		}
		sendUnindentedElement(PSVIUni::fgTerm);
		sendUnindentedElement(PSVIUni::fgParticle);
	}
}

void PSVIWriterHandlers::processAttributeWildcard(XSWildcard* wildcard) {
	if (wildcard == NULL) {
		sendElementEmpty(PSVIUni::fgAttributeWildcard);
	} else {
		sendIndentedElement(PSVIUni::fgAttributeWildcard);
		processWildcard(wildcard);
		sendUnindentedElement(PSVIUni::fgAttributeWildcard);
	}
}

void PSVIWriterHandlers::processScope(XSComplexTypeDefinition* enclosingCTD, short scope) {
	switch (scope) {
		case XSConstants::SCOPE_ABSENT:
			sendElementEmpty(PSVIUni::fgScope);
			break;
		case XSConstants::SCOPE_LOCAL:
			sendIndentedElement(PSVIUni::fgScope);
			sendReference(PSVIUni::fgComplexTypeDefinition, enclosingCTD);
			sendUnindentedElement(PSVIUni::fgScope);
			break;
		case XSConstants::SCOPE_GLOBAL:
			sendElementValue(PSVIUni::fgScope, PSVIUni::fgGlobal);
	}
}

void PSVIWriterHandlers::processValueConstraint(XSConstants::VALUE_CONSTRAINT valueConstraintType, const XMLCh* constraintValue) {
	if (valueConstraintType == XSConstants::VC_NONE) {
		sendElementEmpty(PSVIUni::fgValueConstraint);
	} else {
		sendIndentedElement(PSVIUni::fgValueConstraint);
		sendElementValue(PSVIUni::fgVariety, translateValueConstraint(valueConstraintType));
		sendElementValue(PSVIUni::fgValue, constraintValue);
		sendUnindentedElement(PSVIUni::fgValueConstraint);
	}
}

void PSVIWriterHandlers::processIdentityConstraintDefinition(XSNamedMap<XSIDCDefinition>* idConstraint) {
	if (idConstraint == NULL) {
		sendElementEmpty(PSVIUni::fgIdentityConstraintDefinitions);
	} else {
		sendIndentedElement(PSVIUni::fgIdentityConstraintDefinitions);
		for (unsigned int i=0; i < idConstraint->getLength(); i++) {
			XSIDCDefinition* constraint = idConstraint->item(i);
			sendIndentedElementWithID(PSVIUni::fgIdentityConstraintDefinition, (XSObject*) constraint);
			sendElementValue(PSVIUni::fgName, constraint->getName());
			sendElementValue(PSVIUni::fgTargetNamespace, constraint->getNamespace());
			sendElementValue(PSVIUni::fgIdentityConstraintCategory, translateIdConstraintCategory(constraint->getCategory()));
			sendIndentedElement(PSVIUni::fgSelector);
			processXPath(constraint->getSelectorStr());
			sendUnindentedElement(PSVIUni::fgSelector);
			processFields(constraint->getFieldStrs());
			sendReference(PSVIUni::fgReferencedKey, constraint->getRefKey());
			processAnnotations(constraint->getAnnotations());
			sendUnindentedElement(PSVIUni::fgIdentityConstraintDefinition);
		}
		sendUnindentedElement(PSVIUni::fgIdentityConstraintDefinitions);
	}
}

void PSVIWriterHandlers::processFields(StringList* fields) {
	sendIndentedElement(PSVIUni::fgFields);
	for (unsigned int i=0; i < fields->size(); i++) {
		processXPath(fields->elementAt(i));
	}
	sendUnindentedElement(PSVIUni::fgFields);
}

void PSVIWriterHandlers::processXPath(const XMLCh* xpath) {
	sendIndentedElement(PSVIUni::fgXpath);
	sendElementValue(PSVIUni::fgXpath, xpath);
	sendUnindentedElement(PSVIUni::fgXpath);
}

void PSVIWriterHandlers::processChildren() {
	if (!fElementChildren->empty() && !fElementChildren->peek()) {
		fElementChildren->pop();
		sendIndentedElement(PSVIUni::fgChildren);
		fElementChildren->push(true);
	}
}

void PSVIWriterHandlers::processChildrenEnd() {
	if (fElementChildren->pop()) {
		sendUnindentedElement(PSVIUni::fgChildren);	
	} else {
		writeEmpty(PSVIUni::fgChildren);	
	}
}

void PSVIWriterHandlers::processTypeDefinitionOrRef(const XMLCh* enclose, XSTypeDefinition* type) {
	if (type==NULL) {
		sendElementEmpty(enclose);
	} else {
		sendIndentedElement(enclose);
		if (type->getAnonymous() && !(fDefinedIds->containsElement(type))) {
			processTypeDefinition(type);
		} else {
			if (type->getTypeCategory() == XSTypeDefinition::SIMPLE_TYPE) {
				sendReference(PSVIUni::fgSimpleTypeDefinition, type);
			} else {
				sendReference(PSVIUni::fgComplexTypeDefinition, type);
			}
		}
		sendUnindentedElement(enclose);
	}
}

void PSVIWriterHandlers::processSimpleTypeDefinitionOrRef(XSSimpleTypeDefinition* type) {
	if (type==NULL) {
		sendElementEmpty(PSVIUni::fgSimpleTypeDefinition);
	} else {
		if (type->getAnonymous() && !(fDefinedIds->containsElement(type))) {
			processSimpleTypeDefinition(type);
		} else {
			sendReference(PSVIUni::fgSimpleTypeDefinition, type);
		}
	}
}

void PSVIWriterHandlers::processAttributeDeclarationOrRef(XSAttributeDeclaration* attrDecl) {
	if (attrDecl==NULL) {
		sendElementEmpty(PSVIUni::fgAttributeDeclaration);
	} else {
		if (fDefinedIds->containsElement(attrDecl) ||
				(attrDecl->getScope() == XSConstants::SCOPE_GLOBAL)) {
			sendReference(PSVIUni::fgAttributeDeclaration, attrDecl);
		} else {
			processAttributeDeclaration(attrDecl);
		}
	}
}

void PSVIWriterHandlers::processElementDeclarationOrRef(XSElementDeclaration* elemDecl) {
	if (elemDecl==NULL) {
		sendElementEmpty(PSVIUni::fgElementDeclaration);
	} else {
		if (fDefinedIds->containsElement(elemDecl) ||
				(elemDecl->getScope() == XSConstants::SCOPE_GLOBAL)) {
			sendReference(PSVIUni::fgElementDeclaration, elemDecl);
		} else {
			processElementDeclaration(elemDecl);
		}
	}
}

void PSVIWriterHandlers::processTypeDefinitionRef(const XMLCh* enclose, XSTypeDefinition* type) {
	if (type==NULL) {
		sendElementEmpty(enclose);
	} else {
		sendIndentedElement(enclose);
		if (type->getTypeCategory() == XSTypeDefinition::SIMPLE_TYPE) {
			sendReference(PSVIUni::fgSimpleTypeDefinition, type);
		} else {
			sendReference(PSVIUni::fgComplexTypeDefinition, type);
		}
		sendUnindentedElement(enclose);
	}
}

void PSVIWriterHandlers::processAttributeDeclarationRef(const XMLCh* enclose, XSAttributeDeclaration* attrDecl) {
	if (attrDecl == NULL) {
		sendElementEmpty(PSVIUni::fgDeclaration);
	} else {
		sendIndentedElement(PSVIUni::fgDeclaration);
		sendReference(PSVIUni::fgAttributeDeclaration, attrDecl);
		sendUnindentedElement(PSVIUni::fgDeclaration);
	}
}

void PSVIWriterHandlers::processElementDeclarationRef(const XMLCh* enclose, XSElementDeclaration* elemDecl) {
	if (elemDecl==NULL) {
		sendElementEmpty(enclose);
	} else {
		sendIndentedElement(enclose);
		sendReference(PSVIUni::fgElementDeclaration, elemDecl);
		sendUnindentedElement(enclose);
	}
}

void PSVIWriterHandlers::sendReference(const XMLCh* elementName, XSObject* obj) {
	if (obj==NULL) {
		sendElementEmpty(elementName);
	} else {
		fAttrList->removeAllElements();
		fAttrList->addElement((XMLCh*)gRef);
		fAttrList->addElement((XMLCh*)getIdName(obj));
		fAttrList->addElement((XMLCh*)fgXsiNil);
		fAttrList->addElement((XMLCh*)PSVIUni::fgTrue);
		writeEmpty(elementName, fAttrList);
	}
}

void PSVIWriterHandlers::sendElementEmpty(const XMLCh* const elementName) {
	fAttrList->removeAllElements();
	fAttrList->addElement((XMLCh*)fgXsiNil);
	fAttrList->addElement((XMLCh*)PSVIUni::fgTrue);
	writeEmpty(elementName, fAttrList);
}

void PSVIWriterHandlers::sendElementValueInt(const XMLCh* elementName, int value) {
	XMLString::binToText(value, fTempResult, 50, 10);
	writeValue(elementName, fTempResult);
}

void PSVIWriterHandlers::sendElementValue(const XMLCh* const elementName, const XMLCh* const value) {
	if (value==NULL || XMLString::equals(value, XMLUni::fgZeroLenString)) {
		sendElementEmpty(elementName);
	} else {
		writeValue(elementName, value);
	}
}

void PSVIWriterHandlers::sendElementValueList(const XMLCh* const elementName, const StringList* values) {
	if (values==NULL) {
		sendElementEmpty(elementName);
	} else {
		writeValue(elementName, values);
	}
}

void PSVIWriterHandlers::sendIndentedElement(const XMLCh* const elementName) {
	writeOpen(elementName);
	incIndent();
}

void PSVIWriterHandlers::sendIndentedElementWithID(const XMLCh* elementName, XSObject* obj) {
	fDefinedIds->addElement(obj);
	fAttrList->removeAllElements();
	fAttrList->addElement((XMLCh*)gId);
	fAttrList->addElement((XMLCh*)getIdName(obj));
	writeOpen(elementName, fAttrList);
	incIndent();
}

void PSVIWriterHandlers::sendUnindentedElement(const XMLCh* elementName) {
	decIndent();
	writeClose(elementName);
}

void PSVIWriterHandlers::writeOpen(const XMLCh* const elementName) {
	*fFormatter 
		<< XMLFormatter::NoEscapes << fIndentChars << chOpenAngle << elementName << gAngleFeed;
}

void PSVIWriterHandlers::writeOpen(const XMLCh* const elementName, const StringList* const attrs) {
	*fFormatter 
		<< XMLFormatter::NoEscapes << fIndentChars << chOpenAngle << elementName ;
	for (unsigned int i=0; i < attrs->size(); i+=2 ) {
		*fFormatter
			<< XMLFormatter::NoEscapes << chSpace << attrs->elementAt(i) << gEqualsQuote
			<< XMLFormatter::AttrEscapes << attrs->elementAt(i+1)
			<< XMLFormatter::NoEscapes << chDoubleQuote ;
	}
	*fFormatter
		<< XMLFormatter::NoEscapes << gAngleFeed;
}

void PSVIWriterHandlers::writeClose(const XMLCh* const elementName) {
	*fFormatter 
		<< XMLFormatter::NoEscapes << fIndentChars << gAngleSlash << elementName << gAngleFeed;
}

void PSVIWriterHandlers::writeValue(const XMLCh* const elementName, const XMLCh* const value) {
	*fFormatter 
		<< XMLFormatter::NoEscapes << fIndentChars << chOpenAngle << elementName << chCloseAngle
		<< XMLFormatter::CharEscapes << value
		<< XMLFormatter::NoEscapes << gAngleSlash << elementName <<	gAngleFeed ;
}

void PSVIWriterHandlers::writeValue(const XMLCh* const elementName, const StringList* values) {
	*fFormatter 
		<< XMLFormatter::NoEscapes << fIndentChars << chOpenAngle << elementName << chCloseAngle;
	for (unsigned int i=0; i < values->size(); i++) {
		*fFormatter 
			<< XMLFormatter::CharEscapes << values->elementAt(i) << chSpace;
	}
	*fFormatter 
		<< XMLFormatter::NoEscapes << gAngleSlash << elementName <<	gAngleFeed ;
}

void PSVIWriterHandlers::writeEmpty(const XMLCh* const elementName, const StringList* const attrs) {
	*fFormatter 
		<< XMLFormatter::NoEscapes << fIndentChars << chOpenAngle << elementName ; 
	for (unsigned int i=0; i < attrs->size(); i+=2 ) {
		*fFormatter
			<< XMLFormatter::NoEscapes << chSpace << attrs->elementAt(i) << gEqualsQuote
			<< XMLFormatter::AttrEscapes << attrs->elementAt(i+1)
			<< XMLFormatter::NoEscapes << chDoubleQuote ;
	}
	*fFormatter
		<< XMLFormatter::NoEscapes << gSlashAngleFeed ;
}

void PSVIWriterHandlers::writeEmpty(const XMLCh* const elementName) {
	*fFormatter 
		<< XMLFormatter::NoEscapes << fIndentChars << chOpenAngle << elementName << gSlashAngleFeed ;
}

const XMLCh* PSVIWriterHandlers::translateScope(XSConstants::SCOPE scope) {
	switch (scope) {
		case XSConstants::SCOPE_ABSENT :
			return NULL;
		case XSConstants::SCOPE_GLOBAL :
			return PSVIUni::fgGlobal;
		case XSConstants::SCOPE_LOCAL :
			return PSVIUni::fgLocal;
		default :
			return PSVIUni::fgUnknown;
	}
}

const XMLCh* PSVIWriterHandlers::translateValueConstraint(XSConstants::VALUE_CONSTRAINT constraintKind) {
	switch (constraintKind) {
		case XSConstants::VC_DEFAULT :
			return PSVIUni::fgDefault;
		case XSConstants::VC_FIXED :
			return PSVIUni::fgFixed;
		default :
			return PSVIUni::fgUnknown;
	}
}

const XMLCh* PSVIWriterHandlers::translateBlockOrFinal(short val) {
	XMLString::copyString(fTempResult, XMLUni::fgZeroLenString); 
	if ((val & XSConstants::DERIVATION_EXTENSION) != 0) {
		XMLString::catString(fTempResult, PSVIUni::fgExtension);
	}
	if ((val & XSConstants::DERIVATION_RESTRICTION) != 0) {
		if (XMLString::stringLen(fTempResult) != 0)
			XMLString::catString(fTempResult, fgSpace);
		XMLString::catString(fTempResult, PSVIUni::fgRestriction);
	}
	if ((val & XSConstants::DERIVATION_LIST) != 0) {
		if (XMLString::stringLen(fTempResult) != 0)
			XMLString::catString(fTempResult, fgSpace);
		XMLString::catString(fTempResult, PSVIUni::fgList);
	}
	if ((val & XSConstants::DERIVATION_UNION) != 0) {
		if (XMLString::stringLen(fTempResult) != 0)
			XMLString::catString(fTempResult, fgSpace);
		XMLString::catString(fTempResult, PSVIUni::fgUnion);
	}
	if ((val & XSConstants::DERIVATION_SUBSTITUTION) != 0) {
		if (XMLString::stringLen(fTempResult) != 0)
			XMLString::catString(fTempResult, fgSpace);
		XMLString::catString(fTempResult, PSVIUni::fgSubstitution);
	}
	return fTempResult;
}

const XMLCh* PSVIWriterHandlers::translateDerivationMethod(XSConstants::DERIVATION_TYPE derivation) {
	switch (derivation) {
		case XSConstants::DERIVATION_EXTENSION :
			return PSVIUni::fgExtension;
		case XSConstants::DERIVATION_LIST :
			return PSVIUni::fgList;
		case XSConstants::DERIVATION_RESTRICTION :
			return PSVIUni::fgRestriction;
		case XSConstants::DERIVATION_SUBSTITUTION :
			return PSVIUni::fgSubstitution;
		case XSConstants::DERIVATION_UNION :
			return PSVIUni::fgUnion;
		case XSConstants::DERIVATION_NONE :
			return NULL;
		default :
			return PSVIUni::fgUnknown;
	}
}

const XMLCh* PSVIWriterHandlers::translateProcessContents(XSWildcard::PROCESS_CONTENTS processContents) {
	switch (processContents) {
		case XSWildcard::PC_LAX :
			return PSVIUni::fgLax;
		case XSWildcard::PC_SKIP :
			return PSVIUni::fgSkip;
		case XSWildcard::PC_STRICT :
			return PSVIUni::fgStrict;
		default :
			return PSVIUni::fgUnknown;
	}
}

const XMLCh* PSVIWriterHandlers::translateCompositor(XSModelGroup::COMPOSITOR_TYPE compositor) {
	switch (compositor) {
		case XSModelGroup::COMPOSITOR_SEQUENCE :
			return PSVIUni::fgSequence;
		case XSModelGroup::COMPOSITOR_CHOICE :
			return PSVIUni::fgChoice;
		case XSModelGroup::COMPOSITOR_ALL :
			return PSVIUni::fgAll;
		default :
			return PSVIUni::fgUnknown;
	}
}

const XMLCh* PSVIWriterHandlers::translateValidity(PSVIItem::VALIDITY_STATE validity) {
	switch (validity) {
		case PSVIItem::VALIDITY_NOTKNOWN :
			return PSVIUni::fgNotKnown;
		case PSVIItem::VALIDITY_VALID :
			return PSVIUni::fgValid;
		case PSVIItem::VALIDITY_INVALID :
			return PSVIUni::fgInvalid;
		default :
			return PSVIUni::fgUnknown;
	}
}

const XMLCh* PSVIWriterHandlers::translateValidationAttempted(PSVIItem::ASSESSMENT_TYPE validation) {
	switch (validation) {
		case PSVIItem::VALIDATION_NONE :
			return PSVIUni::fgNone;
		case PSVIItem::VALIDATION_PARTIAL :
			return PSVIUni::fgPartial;
		case PSVIItem::VALIDATION_FULL :
			return PSVIUni::fgFull;
		default :
			return PSVIUni::fgUnknown;
	}
}

const XMLCh* PSVIWriterHandlers::translateIdConstraintCategory(XSIDCDefinition::IC_CATEGORY category) {
	switch (category) {
		case XSIDCDefinition::IC_KEY :
			return PSVIUni::fgKey;
		case XSIDCDefinition::IC_KEYREF :
			return PSVIUni::fgKeyref;
		case XSIDCDefinition::IC_UNIQUE :
			return PSVIUni::fgUnique;
		default :
			return PSVIUni::fgUnknown;
	}
}

const XMLCh* PSVIWriterHandlers::translateComplexContentType(XSComplexTypeDefinition::CONTENT_TYPE contentType) {
	switch (contentType) {
		case XSComplexTypeDefinition::CONTENTTYPE_ELEMENT :
			return PSVIUni::fgElementOnly;
		case XSComplexTypeDefinition::CONTENTTYPE_EMPTY :
			return PSVIUni::fgEmpty;
		case XSComplexTypeDefinition::CONTENTTYPE_MIXED :
			return PSVIUni::fgMixed;
		case XSComplexTypeDefinition::CONTENTTYPE_SIMPLE :
			return PSVIUni::fgSimple;
		default :
			return PSVIUni::fgUnknown;
	}
}

const XMLCh* PSVIWriterHandlers::translateSimpleTypeVariety(XSSimpleTypeDefinition::VARIETY variety) {
	switch (variety) {
		case XSSimpleTypeDefinition::VARIETY_LIST :
			return PSVIUni::fgList;
		case XSSimpleTypeDefinition::VARIETY_UNION :
			return PSVIUni::fgUnion;
		case XSSimpleTypeDefinition::VARIETY_ATOMIC :
			return PSVIUni::fgAtomic;
		case XSSimpleTypeDefinition::VARIETY_ABSENT :
			return NULL;
		default :
			return PSVIUni::fgUnknown;
	}
}

const XMLCh* PSVIWriterHandlers::translateOrderedFacet(XSSimpleTypeDefinition::ORDERING ordered) {
	switch (ordered) {
		case XSSimpleTypeDefinition::ORDERED_FALSE:
			return PSVIUni::fgFalse;
		case XSSimpleTypeDefinition::ORDERED_PARTIAL:
			return PSVIUni::fgPartial;
		case XSSimpleTypeDefinition::ORDERED_TOTAL:
			return PSVIUni::fgTotal;
		default :
			return PSVIUni::fgUnknown;
	}
}

const XMLCh* PSVIWriterHandlers::translateFacet(XSSimpleTypeDefinition::FACET facetKind) {
	switch (facetKind) {
		case XSSimpleTypeDefinition::FACET_WHITESPACE :
			return PSVIUni::fgWhiteSpace;
		case XSSimpleTypeDefinition::FACET_LENGTH :
			return PSVIUni::fgLength;
		case XSSimpleTypeDefinition::FACET_MINLENGTH :
			return PSVIUni::fgMinLength;
		case XSSimpleTypeDefinition::FACET_MAXLENGTH :
			return PSVIUni::fgMaxLength;
		case XSSimpleTypeDefinition::FACET_TOTALDIGITS :
			return PSVIUni::fgTotalDigits;
		case XSSimpleTypeDefinition::FACET_FRACTIONDIGITS :
			return PSVIUni::fgFractionDigits;
		case XSSimpleTypeDefinition::FACET_PATTERN :
			return PSVIUni::fgPattern;
		case XSSimpleTypeDefinition::FACET_ENUMERATION :
			return PSVIUni::fgEnumeration;
		case XSSimpleTypeDefinition::FACET_MAXINCLUSIVE :
			return PSVIUni::fgMaxInclusive;
		case XSSimpleTypeDefinition::FACET_MAXEXCLUSIVE :
			return PSVIUni::fgMaxExclusive;
		case XSSimpleTypeDefinition::FACET_MINEXCLUSIVE :
			return PSVIUni::fgMinExclusive;
		case XSSimpleTypeDefinition::FACET_MININCLUSIVE :
			return PSVIUni::fgMinInclusive;
		default :
			return PSVIUni::fgUnknown;
	}
}

const XMLCh* PSVIWriterHandlers::translateComponentType(XSConstants::COMPONENT_TYPE type) {
	switch (type) {
		case XSConstants::TYPE_DEFINITION :
			return PSVIUni::fgType;
		case XSConstants::ANNOTATION :
			return PSVIUni::fgAnnot;
		case XSConstants::ATTRIBUTE_DECLARATION :
			return PSVIUni::fgAttr;
		case XSConstants::ATTRIBUTE_GROUP_DEFINITION :
			return PSVIUni::fgAg;
		case XSConstants::ATTRIBUTE_USE :
			return PSVIUni::fgAu;
		case XSConstants::ELEMENT_DECLARATION :
			return PSVIUni::fgElt;
		case XSConstants::MODEL_GROUP_DEFINITION :
			return PSVIUni::fgMg;
		case XSConstants::NOTATION_DECLARATION :
			return PSVIUni::fgNot;
		case XSConstants::IDENTITY_CONSTRAINT :
			return PSVIUni::fgIdc;
		default :
			return PSVIUni::fgUnknown;
	}
}

const XMLCh* PSVIWriterHandlers::translateBool(bool flag) {
	return (flag ? PSVIUni::fgTrue : PSVIUni::fgFalse);
}

XMLCh* PSVIWriterHandlers::createID(XSObject* obj) {
	const XMLCh* objPrefix = fPrefixMap->get(obj->getNamespace());
	XMLCh* result = new XMLCh[100];
	
	if (XMLString::equals(obj->getNamespace(), PSVIUni::fgNamespaceXmlSchema)) {
			XMLString::copyString(result, obj->getName());
	} else {
		const XMLCh period[] = { chPeriod, chNull };
		XMLCh anonNum[6];
		bool hasPrefix = objPrefix!=NULL && XMLString::stringLen(objPrefix)!=0;
		if (hasPrefix) {
			XMLString::copyString(result, objPrefix);
			XMLString::catString(result, period);
			XMLString::catString(result, translateComponentType(obj->getType()));
		} else {
			XMLString::copyString(result, translateComponentType(obj->getType()));
		}
		XMLString::catString(result, period);
		
		if (obj->getType()==XSConstants::TYPE_DEFINITION && ((XSTypeDefinition*)obj)->getAnonymous()) {
			const XMLCh anon[] = { chLatin_a, chLatin_n, chLatin_o, chLatin_n, chUnderscore, chNull };
			XMLString::catString(result, anon);
			XMLString::binToText(fAnonNum, anonNum, 5, 10);
			XMLString::catString(result, anonNum);
			fAnonNum++;
		} else {
			XMLString::catString(result, obj->getName());
			if (!hasPrefix) {
				XMLString::catString(result, period);
				XMLString::binToText(fAnonNum, anonNum, 5, 10);
				XMLString::catString(result, anonNum);
				fAnonNum++;
			}
		}
	}
	fIdNames->addElement(result);
	return result;
}

const XMLCh* PSVIWriterHandlers::getIdName(XSObject* obj) {
	XMLCh* objLoc = new XMLCh[9];
	XMLString::binToText((unsigned long)obj, objLoc, 8, 16);
	XMLCh* idName = fIdMap->get(objLoc);
	if (!idName) {
		idName = createID(obj);
		fIdMap->put(objLoc, idName);
		fObjectLocations->addElement(objLoc);
	} else {
		delete objLoc;
	}
	return idName;
}

void PSVIWriterHandlers::incIndent() {
	XMLCh tab[] = {chHTab, chNull};
	checkCapacity(fIndent, &fIndentCap, &fIndentChars);
	XMLString::catString(fIndentChars, tab);
	fIndent++;
}

void PSVIWriterHandlers::decIndent() {
	fIndentChars[XMLString::stringLen(fIndentChars)-1] = chNull;
	fIndent--;
}

void PSVIWriterHandlers::checkCapacity(unsigned int actual, unsigned int* capacity, XMLCh** buffer) {
	if (actual >= *capacity) {
		doubleCapacity(buffer, capacity);
	}
}

void PSVIWriterHandlers::doubleCapacity(XMLCh** buffer, unsigned int* bufferCap) {
	*bufferCap *= 2;
	XMLCh* temp = new XMLCh[*bufferCap];
	XMLString::copyString(temp, *buffer);
	delete[] *buffer;
	*buffer = temp;
}
