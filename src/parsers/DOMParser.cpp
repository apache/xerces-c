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

/**
*  This file contains code to build the DOM tree. It registers a document
*  handler with the scanner. In these handler methods, appropriate DOM nodes
*  are created and added to the DOM tree.
*
* $Id$
*
*/



// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <sax/EntityResolver.hpp>
#include <util/XMLUniDefs.hpp>
#include <sax/ErrorHandler.hpp>
#include <sax/SAXParseException.hpp>
#include <framework/XMLNotationDecl.hpp>
#include <util/IOException.hpp>
#include <internal/XMLScanner.hpp>
#include <validators/DTD/DTDValidator.hpp>
#include <parsers/DOMParser.hpp>
#include <dom/ElementImpl.hpp>
#include <dom/AttrImpl.hpp>
#include <dom/AttrNSImpl.hpp>
#include <dom/TextImpl.hpp>
#include <dom/DocumentImpl.hpp>
#include <dom/DocumentTypeImpl.hpp>
#include <dom/EntityImpl.hpp>
#include <dom/NotationImpl.hpp>
#include <dom/NamedNodeMapImpl.hpp>
#include <dom/NodeIDMap.hpp>


#include <validators/common/ContentSpecNode.hpp>
#include <validators/DTD/DTDAttDefList.hpp>

// ---------------------------------------------------------------------------
//  DOMParser: Constructors and Destructor
// ---------------------------------------------------------------------------
DOMParser::DOMParser(XMLValidator* const valToAdopt) :

fErrorHandler(0)
, fEntityResolver(0)
, fCreateEntityReferenceNodes(false)
, fToCreateXMLDeclTypeNode(false)
, fIncludeIgnorableWhitespace(true)
, fNodeStack(0)
, fScanner(0)
, fOldDocTypeHandler(0)
, fValidator(valToAdopt)
{

    // Create the validator if one was not provided
    if (!fValidator)
        fValidator = new DTDValidator;

    //If the user already registered the doctypehandler then chain it
	fOldDocTypeHandler =  ((DTDValidator*)fValidator)->getDocTypeHandler();
	
	//register the new doctypehandler
	((DTDValidator*)fValidator)->setDocTypeHandler(this);
    //
    //  Create a scanner and tell it what validator to use. Then set us
    //  as the document event handler so we can fill the DOM document.
    //
    fScanner = new XMLScanner(fValidator);
    fScanner->setDocHandler(this);

    fNodeStack = new ValueStackOf<DOM_Node>(64);
    this->reset();


}


DOMParser::~DOMParser()
{
    delete fNodeStack;
    delete fScanner;
    delete fValidator;
}


void DOMParser::reset()
{
    //
    //  Note: DOM Documents are reference counted. Doing this assignment
    //  will cause the old one to go away unless application code is also
    //  holding a reference to it.
    //
    fDocument = DOM_Document::createDocument();
    resetDocType();

    fCurrentParent   = 0;
    fCurrentNode     = 0;
    fParseInProgress = false;
    fWithinElement   = false;
    fNodeStack->removeAllElements();
};



// ---------------------------------------------------------------------------
//  DOMParser: Getter methods
// ---------------------------------------------------------------------------
const XMLValidator& DOMParser::getValidator() const
{
    return *fValidator;
}

bool DOMParser::getDoNamespaces() const
{
    return fScanner->getDoNamespaces();
}

bool DOMParser::getExitOnFirstFatalError() const
{
    return fScanner->getExitOnFirstFatal();
}

DOMParser::ValSchemes DOMParser::getValidationScheme() const
{
    const XMLScanner::ValSchemes scheme = fScanner->getValidationScheme();

    if (scheme == XMLScanner::Val_Always)
        return Val_Always;
    else if (scheme == XMLScanner::Val_Never)
        return Val_Never;

    return Val_Auto;
}

bool DOMParser::getSchemaValidation() const
{
    return fScanner->getSchemaValidation();
}


// ---------------------------------------------------------------------------
//  DOMParser: Setter methods
// ---------------------------------------------------------------------------
void DOMParser::setDoNamespaces(const bool newState)
{
    fScanner->setDoNamespaces(newState);
}

void DOMParser::setErrorHandler(ErrorHandler* const handler)
{
    fErrorHandler = handler;
    if (fErrorHandler)
    {
        fScanner->setErrorReporter(this);
        fValidator->setErrorReporter(this);
    }
    else
    {
        fScanner->setErrorReporter(0);
        fValidator->setErrorReporter(0);
    }
}

void DOMParser::setEntityResolver(EntityResolver* const handler)
{
    fEntityResolver = handler;
    if (fEntityResolver)
        fScanner->setEntityHandler(this);
    else
        fScanner->setEntityHandler(0);
}

void DOMParser::setExitOnFirstFatalError(const bool newState)
{
    fScanner->setExitOnFirstFatal(newState);
}

void DOMParser::setValidationScheme(const ValSchemes newScheme)
{
    if (newScheme == Val_Never)
        fScanner->setValidationScheme(XMLScanner::Val_Never);
    else if (newScheme == Val_Always)
        fScanner->setValidationScheme(XMLScanner::Val_Always);
    else
        fScanner->setValidationScheme(XMLScanner::Val_Auto);
}

void DOMParser::setSchemaValidation(const bool newState)
{
    fScanner->setSchemaValidation(newState);
}



// ---------------------------------------------------------------------------
//  DOMParser: Parsing methods
// ---------------------------------------------------------------------------
void DOMParser::parse(const InputSource& source, const bool reuseValidator)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(source, reuseValidator);
        fParseInProgress = false;
    }

    catch(...)
    {
        fParseInProgress = false;
        throw;
    }
}

void DOMParser::parse(const XMLCh* const systemId, const bool reuseValidator)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(systemId, reuseValidator);
        fParseInProgress = false;
    }

    catch(...)
    {
        fParseInProgress = false;
        throw;
    }
}

void DOMParser::parse(const char* const systemId, const bool reuseValidator)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(systemId, reuseValidator);
        fParseInProgress = false;
    }

    catch(...)
    {
        fParseInProgress = false;
        throw;
    }
}



// ---------------------------------------------------------------------------
//  DOMParser: Progressive parse methods
// ---------------------------------------------------------------------------
bool DOMParser::parseFirst( const   XMLCh* const    systemId
                           ,       XMLPScanToken&  toFill
                           , const bool            reuseValidator)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    return fScanner->scanFirst(systemId, toFill, reuseValidator);
}

bool DOMParser::parseFirst( const   char* const         systemId
                           ,       XMLPScanToken&      toFill
                           , const bool                reuseValidator)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    return fScanner->scanFirst(systemId, toFill, reuseValidator);
}

bool DOMParser::parseFirst( const   InputSource&    source
                           ,       XMLPScanToken&  toFill
                           , const bool            reuseValidator)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    return fScanner->scanFirst(source, toFill, reuseValidator);
}

bool DOMParser::parseNext(XMLPScanToken& token)
{
    return fScanner->scanNext(token);
}

void DOMParser::parseReset(XMLPScanToken& token)
{
    // Reset the scanner, and then reset the parser
    fScanner->scanReset(token);
    reset();
}


// ---------------------------------------------------------------------------
//  DOMParser: Implementation of the XMLErrorReporter interface
// ---------------------------------------------------------------------------
void DOMParser::error(  const   unsigned int                code
                      , const XMLCh* const                msgDomain
                      , const XMLErrorReporter::ErrTypes  errType
                      , const XMLCh* const                errorText
                      , const XMLCh* const                systemId
                      , const XMLCh* const                publicId
                      , const unsigned int                lineNum
                      , const unsigned int                colNum)
{
    SAXParseException toThrow = SAXParseException
        (
        errorText
        , publicId
        , systemId
        , lineNum
        , colNum
        );

    //
    //  If there is an error handler registered, call it, otherwise ignore
    //  all but the fatal errors.
    //
    if (!fErrorHandler)
    {
        if (errType == XMLErrorReporter::ErrType_Fatal)
            throw toThrow;
        return;
    }

    if (errType == XMLErrorReporter::ErrType_Warning)
        fErrorHandler->warning(toThrow);
    else if (errType >= XMLErrorReporter::ErrType_Fatal)
        fErrorHandler->fatalError(toThrow);
    else
        fErrorHandler->error(toThrow);
}

void DOMParser::resetErrors()
{
}


// ---------------------------------------------------------------------------
//  DOMParser: Implementation of XMLEntityHandler interface
// ---------------------------------------------------------------------------
InputSource*
DOMParser::resolveEntity(const XMLCh* const publicId, const XMLCh* const systemId)
{
    //
    //  Just map it to the SAX entity resolver. If there is not one installed,
    //  return a null pointer to cause the default resolution.
    //
    if (fEntityResolver)
        return fEntityResolver->resolveEntity(publicId, systemId);
    return 0;
}



// ---------------------------------------------------------------------------
//  DOMParser: Implementation of XMLDocumentHandler interface
// ---------------------------------------------------------------------------
void DOMParser::docCharacters(  const   XMLCh* const    chars
                              , const unsigned int    length
                              , const bool            cdataSection)
{
    // Ignore chars outside of content
    if (!fWithinElement)
        return;

    if (cdataSection == true)
    {
        DOM_CDATASection node = fDocument.createCDATASection
            (
            DOMString(chars, length)
            );
        fCurrentParent.appendChild(node);
        fCurrentNode = node;
    }
    else
    {
        if (fCurrentNode.getNodeType() == DOM_Node::TEXT_NODE)
        {
            DOM_Text node = (DOM_Text&)fCurrentNode;
            node.appendData(DOMString(chars, length));
        }
        else
        {
            DOM_Text node = fDocument.createTextNode(DOMString(chars, length));
			//If the node type is entityRef then set the readOnly flag to false before appending node
			bool oldReadFlag;
			if (fCurrentParent.getNodeType() == DOM_Node::ENTITY_REFERENCE_NODE) {
				oldReadFlag = fCurrentParent.fImpl->isReadOnly();
				fCurrentParent.fImpl->isReadOnly(false);
			}

            fCurrentParent.appendChild(node);
			if (fCurrentParent.getNodeType() == DOM_Node::ENTITY_REFERENCE_NODE) {
				fCurrentParent.fImpl->isReadOnly(oldReadFlag);
			}
            fCurrentNode = node;
        }
    }
}


void DOMParser::docComment(const XMLCh* const comment)
{
    DOM_Comment dcom = fDocument.createComment(comment);
    fCurrentParent.appendChild(dcom);
    fCurrentNode = dcom;
}


void DOMParser::docPI(  const   XMLCh* const    target
                      , const XMLCh* const    data)
{
    DOM_ProcessingInstruction pi = fDocument.createProcessingInstruction
        (
        target
        , data
        );
    fCurrentParent.appendChild(pi);
    fCurrentNode = pi;
}


void DOMParser::endEntityReference(const XMLEntityDecl& entDecl)
{
    if (fCreateEntityReferenceNodes == true)
    {
        fCurrentParent = fNodeStack->pop();
        fCurrentNode   = fCurrentParent;
    }
}


void DOMParser::endElement( const   XMLElementDecl&     elemDecl
                           , const unsigned int        urlId
                           , const bool                isRoot)
{
    fCurrentNode   = fCurrentParent;
    fCurrentParent = fNodeStack->pop();

    // If we've hit the end of content, clear the flag
    if (fNodeStack->empty())
        fWithinElement = false;
}


void DOMParser::ignorableWhitespace(const   XMLCh* const    chars
                                    , const unsigned int    length
                                    , const bool            cdataSection)
{
    // Ignore chars before the root element
    if (!fWithinElement || !fIncludeIgnorableWhitespace)
        return;

    if (fCurrentNode.getNodeType() == DOM_Node::TEXT_NODE)
    {
        DOM_Text node = (DOM_Text&)fCurrentNode;
        node.appendData(DOMString(chars, length));
    }
    else
    {
        DOM_Text node = fDocument.createTextNode(DOMString(chars, length));
        TextImpl *text = (TextImpl *) node.fImpl;
        text -> setIgnorableWhitespace(true);
		//If the node type is entityRef then set the readOnly flag to false before appending node
		bool oldReadFlag;
		if (fCurrentParent.getNodeType() == DOM_Node::ENTITY_REFERENCE_NODE) {
			oldReadFlag = fCurrentParent.fImpl->isReadOnly();
			fCurrentParent.fImpl->isReadOnly(false);
		}

        fCurrentParent.appendChild(node);
		if (fCurrentParent.getNodeType() == DOM_Node::ENTITY_REFERENCE_NODE) {
			fCurrentParent.fImpl->isReadOnly(oldReadFlag);
		}

        fCurrentNode = node;
    }
}


void DOMParser::resetDocument()
{
    //
    //  The reset methods are called before a new parse event occurs.
    //  Reset this parsers state to clear out anything that may be left
    //  from a previous use, in particular the DOM document itself.
    //
    this->reset();
}


void DOMParser::startDocument()
{
    // Just set the document as the current parent and current node
    fCurrentParent = fDocument;
    fCurrentNode   = fDocument;
    // set DOM error checking off
    fDocument.setErrorChecking(false);
}


void DOMParser::endDocument()
{
    // set DOM error checking back on
    fDocument.setErrorChecking(true);
}


void DOMParser::startElement(const  XMLElementDecl&         elemDecl
                             , const unsigned int            urlId
                             , const XMLCh* const            elemPrefix
                             , const RefVectorOf<XMLAttr>&   attrList
                             , const unsigned int            attrCount
                             , const bool                    isEmpty
                             , const bool                    isRoot)
{
    DOM_Element     elem;
    DocumentImpl    *docImpl = (DocumentImpl *)fDocument.fImpl;

    if (fScanner -> getDoNamespaces()) {    //DOM Level 2, doNamespaces on
        unsigned int globalNSid = fValidator -> getGlobalNamespaceId();
        XMLBuffer buf;
        DOMString namespaceURI = 0;
        if (urlId != globalNSid) {  //TagName has a prefix
            fValidator -> getURIText(urlId, buf);   //get namespaceURI
            namespaceURI = DOMString(buf.getRawBuffer());
        }
        elem = fDocument.createElementNS(namespaceURI, elemDecl.getFullName());
        ElementImpl *elemImpl = (ElementImpl *) elem.fImpl;
        for (unsigned int index = 0; index < attrCount; ++index) {
            static const XMLCh XMLNS[] = {
            chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chNull
            };
            const XMLAttr* oneAttrib = attrList.elementAt(index);
            unsigned int attrURIId = oneAttrib -> getURIId();
            namespaceURI = 0;
            if (!XMLString::compareString(oneAttrib -> getName(), XMLNS))    //for xmlns=...
                attrURIId = fValidator -> getXMLNSNamespaceId();
            if (attrURIId != globalNSid) {	//TagName has a prefix
                fValidator -> getURIText(attrURIId, buf);   //get namespaceURI
                namespaceURI = DOMString(buf.getRawBuffer());
            }
            AttrImpl *attr = elemImpl->setAttributeNS(namespaceURI, oneAttrib -> getQName(),
            oneAttrib -> getValue());

            // Attributes of type ID.  If this is one, add it to the hashtable of IDs
            //   that is constructed for use by GetElementByID().
            //
            if (oneAttrib->getType()==XMLAttDef::ID)
            {
                if (docImpl->fNodeIDMap == 0)
                    docImpl->fNodeIDMap = new NodeIDMap(500);
                docImpl->fNodeIDMap->add(attr);
                attr->isIdAttr(true);
            }

            attr->setSpecified(oneAttrib->getSpecified());
        }
    }
	else {    //DOM Level 1
			elem = fDocument.createElement(elemDecl.getFullName());
			ElementImpl *elemImpl = (ElementImpl *) elem.fImpl;
			for (unsigned int index = 0; index < attrCount; ++index) {
				const XMLAttr* oneAttrib = attrList.elementAt(index);
				AttrImpl *attr = elemImpl->setAttribute(oneAttrib->getName(), oneAttrib->getValue());
				attr->setSpecified(oneAttrib->getSpecified());

				// Attributes of type ID.  If this is one, add it to the hashtable of IDs
				//   that is constructed for use by GetElementByID().
				//
				if (oneAttrib->getType()==XMLAttDef::ID)
				{
					if (docImpl->fNodeIDMap == 0)
						docImpl->fNodeIDMap = new NodeIDMap(500);
					docImpl->fNodeIDMap->add(attr);
					attr->isIdAttr(true);
				}

		}
    }

    //If the node type is entityRef then set the readOnly flag to false before appending node
	bool oldReadFlag;
	if (fCurrentParent.getNodeType() == DOM_Node::ENTITY_REFERENCE_NODE) {
		oldReadFlag = fCurrentParent.fImpl->isReadOnly();
		fCurrentParent.fImpl->isReadOnly(false);
	}

    fCurrentParent.appendChild(elem);
	if (fCurrentParent.getNodeType() == DOM_Node::ENTITY_REFERENCE_NODE) {
		fCurrentParent.fImpl->isReadOnly(oldReadFlag);
	}

    fNodeStack->push(fCurrentParent);
    fCurrentParent = elem;
    fCurrentNode = elem;
    fWithinElement = true;

    // If an empty element, do end right now (no endElement() will be called)
    if (isEmpty)
        endElement(elemDecl, urlId, isRoot);
}


void DOMParser::startEntityReference(const XMLEntityDecl& entDecl)
{
    if (fCreateEntityReferenceNodes == true)
    {
		DOMString entName(entDecl.getName());
        DOM_EntityReference er = fDocument.createEntityReference(entName);
        fCurrentParent.appendChild(er);
        fNodeStack->push(fCurrentParent);
        fCurrentParent = er;
        fCurrentNode = er;

		// this entityRef needs to be stored in Entity map too.
        // We'd decide later whether the entity nodes should be created by a
        // separated method in parser or not. For now just stick it in if
        // the ref nodes are created
		EntityImpl* entity = (EntityImpl*)fDocumentType->entities->getNamedItem(entName);
		entity->setEntityRef((EntityReferenceImpl*)er.fImpl);

    }
}


void DOMParser::XMLDecl(const   XMLCh* const version
                        , const XMLCh* const encoding
                        , const XMLCh* const standalone
                        , const XMLCh* const actualEncStr)
{
    //This is a non-standard extension to creating XMLDecl type nodes and attching to DOM Tree
    // currently this flag it set to false unless user explicitly asks for it
    // Needs to be revisited after W3C specs are laid out on this issue.

    if (fToCreateXMLDeclTypeNode) {

        DOMString ver(version);
        DOMString enc(encoding);
        DOMString std(standalone);
        DOM_XMLDecl xmlDecl = fDocument.createXMLDecl(ver, enc, std);

        fCurrentParent.appendChild(xmlDecl);
    }
}



// ---------------------------------------------------------------------------
//  DOMParser: Deprecated methods
// ---------------------------------------------------------------------------
bool DOMParser::getDoValidation() const
{
    //
    //  We don't want to tie the public parser classes to the enum used
    //  by the scanner, so we use a separate one and map.
    //
    //  DON'T mix the new and old methods!!
    //
    const XMLScanner::ValSchemes scheme = fScanner->getValidationScheme();
    if (scheme == XMLScanner::Val_Always)
        return true;
    return false;
}

void DOMParser::setDoValidation(const bool newState)
{
    fScanner->setDoValidation
    (
        newState ? XMLScanner::Val_Always : XMLScanner::Val_Never
    );
}

//doctypehandler interfaces
void DOMParser::attDef
(
    const   DTDElementDecl&     elemDecl
    , const DTDAttDef&          attDef
    , const bool                ignoring
	)
{	
    if(fOldDocTypeHandler)
    {
        fOldDocTypeHandler->attDef(elemDecl,attDef, ignoring );
    }

    if (fDocumentType->isIntSubsetReading())
    {
        DOMString attString;
        if (elemDecl.hasAttDefs())
        {
            attString.appendData(chOpenAngle);
            attString.appendData(chBang);
            attString.appendData(XMLUni::fgAttListString);
            attString.appendData(chSpace);
            attString.appendData(elemDecl.getFullName());

            attString.appendData(chSpace);
            attString.appendData(attDef.getFullName());

            // Get the type and display it
            const XMLAttDef::AttTypes type = attDef.getType();
            switch(type)
            {
            case XMLAttDef::CData :
                attString.appendData(chSpace);
                attString.appendData(XMLUni::fgCDATAString);
                break;
            case XMLAttDef::ID :
                attString.appendData(chSpace);
                attString.appendData(XMLUni::fgIDString);
                break;
            case XMLAttDef::IDRef :
                attString.appendData(chSpace);
                attString.appendData(XMLUni::fgIDRefString);
                break;
            case XMLAttDef::IDRefs :
                attString.appendData(chSpace);
                attString.appendData(XMLUni::fgIDRefsString);
                break;
            case XMLAttDef::Entity :
                attString.appendData(chSpace);
                attString.appendData(XMLUni::fgEntityString);
                break;
            case XMLAttDef::Entities :
                attString.appendData(chSpace);
                attString.appendData(XMLUni::fgEntitiesString);
                break;
            case XMLAttDef::NmToken :
                attString.appendData(chSpace);
                attString.appendData(XMLUni::fgNmTokenString);
                break;
            case XMLAttDef::NmTokens :
                attString.appendData(chSpace);
                attString.appendData(XMLUni::fgNmTokensString);
                break;

            case XMLAttDef::Notation :
                attString.appendData(chSpace);
                attString.appendData(XMLUni::fgNotationString);
                break;

            case XMLAttDef::Enumeration :
                attString.appendData(chSpace);
                //  attString.appendData(XMLUni::fgEnumerationString);
                const XMLCh* enumString = attDef.getEnumeration();
                int length = XMLString::stringLen(enumString);
                if (length > 0) {

                    DOMString anotherEnumString;

                    anotherEnumString.appendData(chOpenParen );
                    for(int i=0; i<length; i++) {
                        if (enumString[i] == chSpace)
                            anotherEnumString.appendData(chPipe);
                        else
                            anotherEnumString.appendData(enumString[i]);
                    }
                    anotherEnumString.appendData(chCloseParen);
                    attString.appendData(anotherEnumString);

                    enumString = attDef.getValue();
                    if ( enumString != 0) {
                        attString.appendData(chSpace);
                        attString.appendData(chDoubleQuote);
                        attString.appendData(XMLString::transcode(attDef.getValue()));
                        attString.appendData(chDoubleQuote);
                    }
                }
                break;
            }
            //get te default types of the attlist
            const XMLAttDef::DefAttTypes def = attDef.getDefaultType();
            switch(def)
            {
            case XMLAttDef::Required :
                attString.appendData(chSpace);
                attString.appendData(XMLUni::fgRequiredString);
                break;
            case XMLAttDef::Implied :
                attString.appendData(chSpace);
                attString.appendData(XMLUni::fgImpliedString);
                break;
            case XMLAttDef::Fixed :
                attString.appendData(chSpace);
                attString.appendData(XMLUni::fgFixedString);
                break;
            }
            attString.appendData(chCloseAngle);
            fDocumentType->internalSubset.appendData(attString);
        }
    }
}

void DOMParser::doctypeComment
(
    const   XMLCh* const    comment
)
{
	if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->doctypeComment(comment);
    }
    if (fDocumentType->isIntSubsetReading())
    {
        if (comment != 0)
        {
            DOMString comString;
            comString.appendData(XMLUni::fgCommentString);
            comString.appendData(chSpace);
            comString.appendData(comment);
            comString.appendData(chSpace);
            comString.appendData(chDash);
            comString.appendData(chDash);
            comString.appendData(chCloseAngle);
            fDocumentType->internalSubset.appendData(comString);
        }
    }
}

void DOMParser::doctypeDecl
(
    const   DTDElementDecl& elemDecl
    , const XMLCh* const    publicId
    , const XMLCh* const    systemId
    , const bool            hasIntSubset
)
{
	if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->doctypeDecl(elemDecl, publicId, systemId, hasIntSubset);
	}

	DOM_DocumentType dt;
	dt = fDocument.getImplementation().createDocumentType(elemDecl.getFullName(), publicId, systemId);
    fDocumentType = (DocumentTypeImpl*)dt.fImpl;
	((DocumentImpl*)fDocument.fImpl)->setDocumentType(fDocumentType);

	populateDocumentType();                  //  Add the entities and notations to this DocType.
	
}

void DOMParser::doctypePI
(
    const   XMLCh* const    target
    , const XMLCh* const    data
)
{
    if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->doctypePI(target, data);
	}
    if (fDocumentType->isIntSubsetReading())
	{
		//add these chars to internalSubset variable
        DOMString pi;
        pi.appendData(chOpenAngle);
        pi.appendData(chQuestion);
        pi.appendData(target);
        pi.appendData(chSpace);
        pi.appendData(data);
        pi.appendData(chQuestion);
        pi.appendData(chCloseAngle);

		fDocumentType->internalSubset.appendData(pi);
	}
	
}


void DOMParser::doctypeWhitespace
(
    const   XMLCh* const    chars
    , const unsigned int    length
)
{
	if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->doctypeWhitespace(chars, length);
	}

    if (fDocumentType->isIntSubsetReading())
		fDocumentType->internalSubset.appendData(chars);
}

void DOMParser::elementDecl
(
    const   DTDElementDecl& decl
    , const bool            isIgnored
)
{
    if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->elementDecl(decl, isIgnored);
	}
    if (fDocumentType->isIntSubsetReading())
	{
        DOMString elemDecl;

        elemDecl.appendData(chOpenAngle);
        elemDecl.appendData(chBang);
        elemDecl.appendData(XMLUni::fgElemString);
        elemDecl.appendData(chSpace);
        elemDecl.appendData(decl.getFullName());

        //get the ContentSpec information
        const XMLCh* contentModel = decl.getFormattedContentModel(*fValidator);
        if (contentModel != 0) {
            elemDecl.appendData(chSpace);
            elemDecl.appendData(contentModel);
        }

        elemDecl.appendData(chCloseAngle);
		fDocumentType->internalSubset.appendData(elemDecl);
	}
}

void DOMParser::endAttList
(
    const   DTDElementDecl& elemDecl
)
{
	if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->endAttList(elemDecl);
	}

	// this section sets up default attributes.
	// default attribute nodes are stored in a NamedNodeMap DocumentTypeImpl::elements
	// default attribute data attached to the document is used to conform to the
	// DOM spec regarding creating element nodes & removing attributes with default values
	// see DocumentTypeImpl
	if (elemDecl.hasAttDefs())
	{		
		XMLAttDefList* defAttrs = &elemDecl.getAttDefList();
		XMLAttDef* attr = 0;
		AttrImpl* insertAttr = 0;
		DOM_Element dom_elem = fDocument.createElement(elemDecl.getFullName());
		ElementImpl* elem = (ElementImpl*)(dom_elem.fImpl);

		static const XMLCh XMLNS[] = {chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chNull};

		while (defAttrs->hasMoreElements())
        {
            attr = &defAttrs->nextElement();
            if (attr->getValue() != null)
            {
                if (fScanner->getDoNamespaces())
                {
                    // Namespaces is turned on...
                    //   While scanning the DTD there is no binding defined from
                    //   name space prefixes to name space URIs.  Just set all the
                    //   URIs to be null.
                    insertAttr = new AttrNSImpl((DocumentImpl*)fDocument.fImpl,
                        DOMString(0),                     // NameSpaceURI
                        DOMString(attr->getFullName()));   // qualified name
                }
                else
                {
                    // Namespaces is turned off...
                    insertAttr = new AttrImpl((DocumentImpl*)fDocument.fImpl, attr->getFullName());
                }
                insertAttr->setValue(attr->getValue());
                elem->setAttributeNode(insertAttr);
                insertAttr->setSpecified(false);
            }
        }
        fDocumentType->getElements()->setNamedItem(elem);
    }
}

void DOMParser::endIntSubset()
{
	fDocumentType->intSubsetReading = false;
	if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->endIntSubset();
	}
}

void DOMParser::endExtSubset()
{
	if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->endExtSubset();
	}
}

void DOMParser::entityDecl
(
    const   DTDEntityDecl&  entityDecl
    , const bool            isPEDecl
    , const bool            isIgnored
)
{
	EntityImpl* entity = ((DocumentImpl*)fDocument.fImpl)->createEntity(entityDecl.getName());

	entity->setPublicId(entityDecl.getPublicId());
	entity->setSystemId(entityDecl.getSystemId());
	entity->setNotationName(entityDecl.getNotationName());

    EntityImpl *previousDef = (EntityImpl *)
	    fDocumentType->entities->setNamedItem( entity );

    //
    //  If this new entity node is replacing an entity node that was already
    //    in the entities named node map (happens if documents redefine the
    //    predefined entited such as lt), we need to delete the original
    //    entitiy node, assuming no-one else was referencing it.
    //
    if (previousDef != 0 && previousDef->nodeRefCount == 0)
    	        NodeImpl::deleteIf(previousDef);


	if (fDocumentType->isIntSubsetReading())
	{
		//add thes chars to internalSubset variable
		DOMString entityName;
		entityName.appendData(chOpenAngle);
        entityName.appendData(chBang);
		entityName.appendData(XMLUni::fgEntityString);
        entityName.appendData(chSpace);

        entityName.appendData(entityDecl.getName());
        DOMString id = entity->getPublicId();
        if (id != 0) {
            entityName.appendData(chSpace);
            entityName.appendData(XMLUni::fgPubIDString);
            entityName.appendData(chSpace);
            entityName.appendData(chDoubleQuote);
            entityName.appendData(id);
            entityName.appendData(chDoubleQuote);
        }
        id = entity->getSystemId();
        if (id != 0) {
            entityName.appendData(chSpace);
            entityName.appendData(XMLUni::fgSysIDString);
            entityName.appendData(chSpace);
            entityName.appendData(chDoubleQuote);
            entityName.appendData(id);
            entityName.appendData(chDoubleQuote);

        }
        id = entity->getNotationName();
        if (id != 0) {
            entityName.appendData(chSpace);
            entityName.appendData(XMLUni::fgNDATAString);
            entityName.appendData(chSpace);
            entityName.appendData(chDoubleQuote);
            entityName.appendData(id);
            entityName.appendData(chDoubleQuote);
        }
        id = entityDecl.getValue();
        if (id !=0) {
            entityName.appendData(chSpace);
            entityName.appendData(chDoubleQuote);
            entityName.appendData(id);
            entityName.appendData(chDoubleQuote);
        }

        entityName.appendData(chCloseAngle);
        fDocumentType->internalSubset.appendData(entityName);
    }

	if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->entityDecl(entityDecl, isPEDecl, isIgnored);
	}
}

void DOMParser::resetDocType()
{
	fDocumentType = null;
}

void DOMParser::notationDecl
(
    const   XMLNotationDecl&    notDecl
    , const bool                isIgnored
)
{
	NotationImpl* notation = ((DocumentImpl*)fDocument.fImpl)->createNotation(notDecl.getName());
	notation->setPublicId(notDecl.getPublicId());
	notation->setPublicId(notDecl.getPublicId());

	fDocumentType->notations->setNamedItem( notation );

	if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->notationDecl(notDecl, isIgnored);
	}
}

void DOMParser::startAttList
(
    const   DTDElementDecl& elemDecl
)
{
	if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->startAttList(elemDecl);
	}
}

void DOMParser::startIntSubset()
{
	fDocumentType->intSubsetReading = true;
	if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->startIntSubset();
	}
}

void DOMParser::startExtSubset()
{
	if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->startExtSubset();
	}
}

void DOMParser::TextDecl
(
    const   XMLCh* const    versionStr
    , const XMLCh* const    encodingStr
)
{
	if (fOldDocTypeHandler)
	{
		fOldDocTypeHandler->TextDecl(versionStr, encodingStr);
	}
}


// to populate the entities in the documentType
void DOMParser::populateDocumentType()
{
	if (fDocumentType == null)
		return;
	

	NameIdPoolEnumerator<DTDEntityDecl> entityPoolEnum = ((DTDValidator*)fValidator)->getEntityEnumerator();

	
	while(entityPoolEnum.hasMoreElements()) {
		
		DTDEntityDecl* entityDecl = &entityPoolEnum.nextElement();
		
		EntityImpl* entity = ((DocumentImpl*)fDocument.fImpl)->createEntity(entityDecl->getName());
		entity->setPublicId(entityDecl->getPublicId());
		entity->setSystemId(entityDecl->getSystemId());
		entity->setNotationName(entityDecl->getNotationName());

		fDocumentType->entities->setNamedItem( entity );
	}

	NameIdPoolEnumerator<XMLNotationDecl> notationPoolEnum = ((DTDValidator*)fValidator)->getNotationEnumerator();
	
	while(notationPoolEnum.hasMoreElements()) {
		XMLNotationDecl* notationDecl = &notationPoolEnum.nextElement();

		NotationImpl* notation = ((DocumentImpl*)fDocument.fImpl)->createNotation(notationDecl->getName());
		notation->setPublicId(notationDecl->getPublicId());
		notation->setPublicId(notationDecl->getPublicId());

		fDocumentType->notations->setNamedItem( notation );
	}

}
