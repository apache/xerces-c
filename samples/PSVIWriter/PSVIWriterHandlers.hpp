/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2003 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and Stringinary forms, with or without
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

#ifndef PSVIWRITERHANDLER_HPP
#define PSVIWRITERHANDLER_HPP

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/framework/psvi/XSConstants.hpp>
#include <xercesc/framework/psvi/PSVIHandler.hpp>
#include <xercesc/framework/psvi/PSVIAttribute.hpp>
#include <xercesc/framework/psvi/PSVIAttributeList.hpp>
#include <xercesc/framework/psvi/PSVIElement.hpp>
#include <xercesc/framework/psvi/PSVIItem.hpp>
#include <xercesc/framework/psvi/XSAnnotation.hpp>
#include <xercesc/framework/psvi/XSAttributeDeclaration.hpp>
#include <xercesc/framework/psvi/XSAttributeGroupDefinition.hpp>
#include <xercesc/framework/psvi/XSAttributeUse.hpp>
#include <xercesc/framework/psvi/XSComplexTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSElementDeclaration.hpp>
#include <xercesc/framework/psvi/XSFacet.hpp>
#include <xercesc/framework/psvi/XSIDCDefinition.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/framework/psvi/XSModelGroup.hpp>
#include <xercesc/framework/psvi/XSModelGroupDefinition.hpp>
#include <xercesc/framework/psvi/XSMultiValueFacet.hpp>
#include <xercesc/framework/psvi/XSNamedMap.hpp>
#include <xercesc/framework/psvi/XSNamespaceItem.hpp>
#include <xercesc/framework/psvi/XSNotationDeclaration.hpp>
#include <xercesc/framework/psvi/XSParticle.hpp>
#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSWildcard.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/util/ValueStackOf.hpp>
#include <xercesc/util/ValueVectorOf.hpp>
#include <xercesc/util/XMLEntityResolver.hpp>
#include <xercesc/util/XMLResourceIdentifier.hpp>
#include <stdlib.h>
#include <string.h>

XERCES_CPP_NAMESPACE_USE


class AttrInfo {
public:
	AttrInfo(const XMLCh* pUri, const XMLCh* pName, const XMLCh* pType, const XMLCh* pValue) {
		uri = XMLString::replicate(pUri);
		name = XMLString::replicate(pName);
		type = XMLString::replicate(pType);
		value = XMLString::replicate(pValue);
	}
	
	~AttrInfo() {
		XMLString::release((XMLCh**)&uri);
		XMLString::release((XMLCh**)&name);
		XMLString::release((XMLCh**)&type);
		XMLString::release((XMLCh**)&value);
	}
	
	const XMLCh* getUri() const {
		return uri;	
	}
	
	const XMLCh* getLocalName() const {
		return name;
	}
	
	const XMLCh* getType() const {
		return type;
	}
		
	const XMLCh* getValue() const {
		return value;
	}

private:
	const XMLCh* uri;
	const XMLCh* name;
	const XMLCh* type;
	const XMLCh* value;
};

class PSVIWriterHandlers : public PSVIHandler, public DefaultHandler, public XMLEntityResolver {
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    PSVIWriterHandlers(XMLFormatter* outputFormatter, XMLFormatter* errorFormatter = NULL);
    ~PSVIWriterHandlers();
    
    // -----------------------------------------------------------------------
    //  Convenience Utility
    // -----------------------------------------------------------------------
	void resetPSVIFormatter(XMLFormatter* outputFormatter);
    void resetDocument();
	
    // -----------------------------------------------------------------------
    //  Handlers for the SAX ContentHandler interface
    // -----------------------------------------------------------------------
    void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const Attributes& attrs);
    void endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname);
    void startDocument();
    void endDocument();
    void characters(const XMLCh* const chars, const unsigned int length);
    void ignorableWhitespace(const XMLCh* const chars, const unsigned int length);
    void comment(const XMLCh* const chars, const unsigned int length);
    void processingInstruction(const XMLCh* const target, const XMLCh* const data);
    void startPrefixMapping(const XMLCh* const prefix, const XMLCh* const uri);
    void endPrefixMapping(const XMLCh* const prefix);
    InputSource* resolveEntity(XMLResourceIdentifier* resourceIdentifier);

    // -----------------------------------------------------------------------
    //  Handlers for the SAX ErrorHandler interface
    // -----------------------------------------------------------------------
	void warning(const SAXParseException& exception);
    void error(const SAXParseException& exception);
    void fatalError(const SAXParseException& exception);
    void resetErrors();
    
    // -----------------------------------------------------------------------
    //  Handlers for the PSVIHandler interface
    // -----------------------------------------------------------------------
	
	void handleAttributesPSVI( const XMLCh* const localName, 
								const XMLCh* const uri, 
								PSVIAttributeList* psviAttributes );
	void handleElementPSVI(	const XMLCh* const localName, 
								const XMLCh* const uri,
								PSVIElement* elementInfo );

private:
    // -----------------------------------------------------------------------
    //  Private methods
    // -----------------------------------------------------------------------

    void processAttributes(PSVIAttributeList* psviAttributes, const RefVectorOf<AttrInfo>* attributesInfo);
    void processNamespaceAttributes(PSVIAttributeList* psviAttributes, const RefVectorOf<AttrInfo>* attributes);
    void processAttributePSVI(PSVIAttribute* attribute);
    void processInScopeNamespaces();
   
    void processSchemaInformation(XSModel* model);
    void processNamespaceItem(XSNamespaceItem* namespaceItem);
    void processSchemaComponents(XSNamespaceItem* namespaceItem);
    void processSchemaDocuments(XSNamespaceItem* namespaceItem);
    void processSchemaAnnotations(XSAnnotationList* annotations);
    void processSchemaErrorCode(StringList* errors);
    
    void processTypeDefinition(XSTypeDefinition* type);
    void processComplexTypeDefinition(XSComplexTypeDefinition* complexType);
    void processSimpleTypeDefinition(XSSimpleTypeDefinition* simpleType);
    void processModelGroupDefinition(XSModelGroupDefinition* modelGroup);
    void processAttributeGroupDefinition(XSAttributeGroupDefinition* attributeGroup);
    
    void processElementDeclaration(XSElementDeclaration* element);
    void processAttributeDeclaration(XSAttributeDeclaration* attribute);
    void processNotationDeclaration(XSNotationDeclaration* notation);
    
    void processAnnotations(XSAnnotationList* annotations);
    void processAttributeUses(XSAttributeUseList* attributeUses);
    void processFacets(XSFacetList* facets, XSMultiValueFacetList* multiFacets);
    void processFundamentalFacets(XSSimpleTypeDefinition* facets);
    void processMemberTypeDefinitions(XSSimpleTypeDefinitionList* memberTypes);
    
    void processAnnotation(XSAnnotation* annotation);
    void processDOMElement(const XMLCh* const encloseName, DOMElement* rootElem, const XMLCh* const elementName);
    void processDOMAttributes(DOMNamedNodeMap* attrs);
    void processWildcard(XSWildcard* wildcard);
    void processModelGroup(XSModelGroup* modelGroup);
    void processParticle(XSParticle* particle);
    
    void processAttributeWildcard(XSWildcard* wildcard);
    void processScope(XSComplexTypeDefinition* enclosingCTD, short scope);
    void processValueConstraint(XSConstants::VALUE_CONSTRAINT ConstraintType, const XMLCh* constraintValue);
    
    void processIdentityConstraintDefinition(XSNamedMap<XSIDCDefinition>* identityConstraint);
    void processFields(StringList* fields);
    void processXPath(const XMLCh* xpath);
    
    void processChildren();
    void processChildrenEnd();
    
    void processTypeDefinitionOrRef(const XMLCh* enclose, XSTypeDefinition* type);
	void processSimpleTypeDefinitionOrRef(XSSimpleTypeDefinition* type);
    void processAttributeDeclarationOrRef(XSAttributeDeclaration* attrDecl);
    void processElementDeclarationOrRef(XSElementDeclaration* elemDecl);
	void processTypeDefinitionRef(const XMLCh* enclose, XSTypeDefinition* type);
    void processAttributeDeclarationRef(const XMLCh* enclose, XSAttributeDeclaration* attrDecl);
    void processElementDeclarationRef(const XMLCh* enclose, XSElementDeclaration* elemDecl);
    void sendReference(const XMLCh* elementName, XSObject* obj);
    
    void sendElementEmpty(const XMLCh* elementName);
	void sendElementValueInt(const XMLCh* elementName, const int value);
    void sendElementValue(const XMLCh* elementName, const XMLCh* const value);
    void sendElementValueList(const XMLCh* const elementName, const StringList* const values);

	void sendIndentedElement(const XMLCh* elementName);
    void sendIndentedElementWithID(const XMLCh* elementName, XSObject* obj);	//adds the ID to the attribute list before sending
    void sendUnindentedElement(const XMLCh* elementName);
    
    void writeOpen(const XMLCh* const elementName);
	void writeOpen(const XMLCh* const elementName, const StringList* const attrs);
	void writeClose(const XMLCh* const elementName);
	void writeValue(const XMLCh* const elementName, const XMLCh* const value);
	void writeValue(const XMLCh* const elementName, const StringList* const values);
	void writeEmpty(const XMLCh* const elementName, const StringList* const attrs);
	void writeEmpty(const XMLCh* const elementName);
    
    const XMLCh* translateScope(XSConstants::SCOPE scope);
    const XMLCh* translateValueConstraint(XSConstants::VALUE_CONSTRAINT constraintKind);
    const XMLCh* translateBlockOrFinal(short val);
    const XMLCh* translateDerivationMethod(XSConstants::DERIVATION_TYPE derivation);
    const XMLCh* translateProcessContents(XSWildcard::PROCESS_CONTENTS processContents);
    const XMLCh* translateCompositor(XSModelGroup::COMPOSITOR_TYPE compositor);
    const XMLCh* translateValidity(PSVIItem::VALIDITY_STATE validity);
    const XMLCh* translateValidationAttempted(PSVIItem::ASSESSMENT_TYPE validation);
    const XMLCh* translateIdConstraintCategory(XSIDCDefinition::IC_CATEGORY category);
    const XMLCh* translateComplexContentType(XSComplexTypeDefinition::CONTENT_TYPE contentType);
    const XMLCh* translateSimpleTypeVariety(XSSimpleTypeDefinition::VARIETY variety);
    const XMLCh* translateOrderedFacet(XSSimpleTypeDefinition::ORDERING ordered);
    const XMLCh* translateFacet(XSSimpleTypeDefinition::FACET facetKind);
    const XMLCh* translateComponentType(XSConstants::COMPONENT_TYPE type);
    const XMLCh* translateBool(bool flag);
    
    XMLCh* createID(XSObject* obj);
    const XMLCh* getIdName(XSObject* obj);
    void incIndent();
    void decIndent();
    void checkCapacity(unsigned int actual, unsigned int* capacity, XMLCh** buffer);
    void doubleCapacity(XMLCh** buffer, unsigned int* bufferCap);
    
protected:    
	XMLFormatter* fFormatter;
	XMLFormatter* fErrorFormatter;
	
	StringList* fAttrList;
    XMLCh* fTempResult;
    XMLCh* fIndentChars;
    XMLCh* fBaseUri;
    
    unsigned int fIndent;
    unsigned int fIndentCap;
    unsigned int fAnonNum;
    
	RefHashTableOf<XMLCh>* fIdMap;
    RefVectorOf<XSObject>* fDefinedIds;
    RefArrayVectorOf<XMLCh>* fIdNames;
    RefArrayVectorOf<XMLCh>* fObjectLocations;
    
	RefHashTableOf<XMLCh>* fPrefixMap;
    RefArrayVectorOf<XMLCh>* fNamespaces;
    
	ValueVectorOf<unsigned int>* fNSAttributes;  //REVISIT  dont need if NSAttrs in different object
	ValueStackOf<bool>* fElementChildren;
		
	RefVectorOf<AttrInfo>* fAttributesInfo;
};


#endif

