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
#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/framework/XMLNotationDecl.hpp>
#include <xercesc/util/IOException.hpp>
#include <xercesc/internal/XMLScanner.hpp>
#include <xercesc/validators/DTD/DTDValidator.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/impl/DOMAttrImpl.hpp>
#include <xercesc/dom/DOMCDATASection.hpp>
#include <xercesc/dom/DOMComment.hpp>
#include <xercesc/dom/impl/DOMTextImpl.hpp>
#include <xercesc/dom/impl/DOMDocumentImpl.hpp>
#include <xercesc/dom/impl/DOMDocumentTypeImpl.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/impl/DOMElementImpl.hpp>
#include <xercesc/dom/impl/DOMEntityImpl.hpp>
#include <xercesc/dom/impl/DOMEntityReferenceImpl.hpp>
#include <xercesc/dom/impl/DOMNotationImpl.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMProcessingInstruction.hpp>
#include <xercesc/dom/impl/DOMNodeIDMap.hpp>


#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/DTD/DTDAttDefList.hpp>

// ---------------------------------------------------------------------------
//  XercesDOMParser: Constructors and Destructor
// ---------------------------------------------------------------------------
XercesDOMParser::XercesDOMParser(XMLValidator* const valToAdopt) :

fErrorHandler(0)
, fEntityResolver(0)
, fCreateEntityReferenceNodes(true)
, fIncludeIgnorableWhitespace(true)
, fNodeStack(0)
, fScanner(0)
, fDocument(0)
, fDocumentVector(0)
{
    //
    //  Create a scanner and tell it what validator to use. Then set us
    //  as the document event handler so we can fill the DOM document.
    //
    fScanner = new XMLScanner(valToAdopt);
    fScanner->setDocHandler(this);
    fScanner->setDocTypeHandler(this);

    fNodeStack = new ValueStackOf<DOMNode*>(64);
    this->reset();


}


XercesDOMParser::~XercesDOMParser()
{
    if (fDocumentVector)
        delete fDocumentVector;

    delete fDocument;
    delete fNodeStack;
    delete fScanner;
}


void XercesDOMParser::reset()
{
    // if fDocument exists already, store the old pointer in the vector for deletion later
    if (fDocument) {
        if (!fDocumentVector) {
            // allocate the vector if not exists yet
            fDocumentVector  = new RefVectorOf<DOMDocumentImpl>(10, true) ;
        }
        fDocumentVector->addElement(fDocument);
    }

    fDocument = 0;
    resetDocType();

    fCurrentParent   = 0;
    fCurrentNode     = 0;
    fParseInProgress = false;
    fWithinElement   = false;
    fNodeStack->removeAllElements();
};



// ---------------------------------------------------------------------------
//  XercesDOMParser: Getter methods
// ---------------------------------------------------------------------------
DOMDocument* XercesDOMParser::getDocument()
{
    return fDocument;
}

const XMLValidator& XercesDOMParser::getValidator() const
{
    return *fScanner->getValidator();
}

bool XercesDOMParser::getDoNamespaces() const
{
    return fScanner->getDoNamespaces();
}

bool XercesDOMParser::getExitOnFirstFatalError() const
{
    return fScanner->getExitOnFirstFatal();
}

bool XercesDOMParser::getValidationConstraintFatal() const
{
    return fScanner->getValidationConstraintFatal();
}

XercesDOMParser::ValSchemes XercesDOMParser::getValidationScheme() const
{
    const XMLScanner::ValSchemes scheme = fScanner->getValidationScheme();

    if (scheme == XMLScanner::Val_Always)
        return Val_Always;
    else if (scheme == XMLScanner::Val_Never)
        return Val_Never;

    return Val_Auto;
}

bool XercesDOMParser::getDoSchema() const
{
    return fScanner->getDoSchema();
}

bool XercesDOMParser::getValidationSchemaFullChecking() const
{
    return fScanner->getValidationSchemaFullChecking();
}

int XercesDOMParser::getErrorCount() const
{
    return fScanner->getErrorCount();
}

XMLCh* XercesDOMParser::getExternalSchemaLocation() const
{
    return fScanner->getExternalSchemaLocation();
}

XMLCh* XercesDOMParser::getExternalNoNamespaceSchemaLocation() const
{
    return fScanner->getExternalNoNamespaceSchemaLocation();
}

// ---------------------------------------------------------------------------
//  XercesDOMParser: Setter methods
// ---------------------------------------------------------------------------
void XercesDOMParser::setDoNamespaces(const bool newState)
{
    fScanner->setDoNamespaces(newState);
}

void XercesDOMParser::setErrorHandler(ErrorHandler* const handler)
{
    fErrorHandler = handler;
    if (fErrorHandler) {
        fScanner->setErrorReporter(this);
        fScanner->setErrorHandler(fErrorHandler);
    }
    else {
        fScanner->setErrorReporter(0);
        fScanner->setErrorHandler(0);
    }
}

void XercesDOMParser::setEntityResolver(EntityResolver* const handler)
{
    fEntityResolver = handler;
    if (fEntityResolver) {
        fScanner->setEntityHandler(this);
        fScanner->setEntityResolver(fEntityResolver);
    }
    else {
        fScanner->setEntityHandler(0);
        fScanner->setEntityResolver(0);
    }
}

void XercesDOMParser::setExitOnFirstFatalError(const bool newState)
{
    fScanner->setExitOnFirstFatal(newState);
}

void XercesDOMParser::setValidationConstraintFatal(const bool newState)
{
    fScanner->setValidationConstraintFatal(newState);
}

void XercesDOMParser::setValidationScheme(const ValSchemes newScheme)
{
    if (newScheme == Val_Never)
        fScanner->setValidationScheme(XMLScanner::Val_Never);
    else if (newScheme == Val_Always)
        fScanner->setValidationScheme(XMLScanner::Val_Always);
    else
        fScanner->setValidationScheme(XMLScanner::Val_Auto);
}

void XercesDOMParser::setDoSchema(const bool newState)
{
    fScanner->setDoSchema(newState);
}

void XercesDOMParser::setValidationSchemaFullChecking(const bool schemaFullChecking)
{
    fScanner->setValidationSchemaFullChecking(schemaFullChecking);
}

void XercesDOMParser::setExternalSchemaLocation(const XMLCh* const schemaLocation)
{
    fScanner->setExternalSchemaLocation(schemaLocation);
}
void XercesDOMParser::setExternalNoNamespaceSchemaLocation(const XMLCh* const noNamespaceSchemaLocation)
{
    fScanner->setExternalNoNamespaceSchemaLocation(noNamespaceSchemaLocation);
}

void XercesDOMParser::setExternalSchemaLocation(const char* const schemaLocation)
{
    fScanner->setExternalSchemaLocation(schemaLocation);
}
void XercesDOMParser::setExternalNoNamespaceSchemaLocation(const char* const noNamespaceSchemaLocation)
{
    fScanner->setExternalNoNamespaceSchemaLocation(noNamespaceSchemaLocation);
}


// ---------------------------------------------------------------------------
//  XercesDOMParser: Parsing methods
// ---------------------------------------------------------------------------
void XercesDOMParser::parse(const InputSource& source, const bool reuseGrammar)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(source, reuseGrammar);
        fParseInProgress = false;
    }

    catch(...)
    {
        fParseInProgress = false;
        throw;
    }
}

void XercesDOMParser::parse(const XMLCh* const systemId, const bool reuseGrammar)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(systemId, reuseGrammar);
        fParseInProgress = false;
    }

    catch(...)
    {
        fParseInProgress = false;
        throw;
    }
}

void XercesDOMParser::parse(const char* const systemId, const bool reuseGrammar)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(systemId, reuseGrammar);
        fParseInProgress = false;
    }

    catch(...)
    {
        fParseInProgress = false;
        throw;
    }
}



// ---------------------------------------------------------------------------
//  XercesDOMParser: Progressive parse methods
// ---------------------------------------------------------------------------
bool XercesDOMParser::parseFirst( const   XMLCh* const    systemId
                           ,       XMLPScanToken&  toFill
                           , const bool            reuseGrammar)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    return fScanner->scanFirst(systemId, toFill, reuseGrammar);
}

bool XercesDOMParser::parseFirst( const   char* const         systemId
                           ,       XMLPScanToken&      toFill
                           , const bool                reuseGrammar)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    return fScanner->scanFirst(systemId, toFill, reuseGrammar);
}

bool XercesDOMParser::parseFirst( const   InputSource&    source
                           ,       XMLPScanToken&  toFill
                           , const bool            reuseGrammar)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    return fScanner->scanFirst(source, toFill, reuseGrammar);
}

bool XercesDOMParser::parseNext(XMLPScanToken& token)
{
    return fScanner->scanNext(token);
}

void XercesDOMParser::parseReset(XMLPScanToken& token)
{
    // Reset the scanner, and then reset the parser
    fScanner->scanReset(token);
    reset();
}

// ---------------------------------------------------------------------------
//  XercesDOMParser: Utilities
// ---------------------------------------------------------------------------
void XercesDOMParser::resetDocumentPool()
{
    //  We cannot enter here while a regular parse is in progress.
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    if (fDocumentVector)
        fDocumentVector->removeAllElements();

    delete fDocument;
    fDocument = 0;
}


// ---------------------------------------------------------------------------
//  XercesDOMParser: Implementation of the XMLErrorReporter interface
// ---------------------------------------------------------------------------
void XercesDOMParser::error(  const   unsigned int                code
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

void XercesDOMParser::resetErrors()
{
}


// ---------------------------------------------------------------------------
//  XercesDOMParser: Implementation of XMLEntityHandler interface
// ---------------------------------------------------------------------------
InputSource*
XercesDOMParser::resolveEntity(const XMLCh* const publicId, const XMLCh* const systemId)
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
//  XercesDOMParser: Implementation of XMLDocumentHandler interface
// ---------------------------------------------------------------------------
void XercesDOMParser::docCharacters(  const   XMLCh* const    chars
                              , const unsigned int    length
                              , const bool            cdataSection)
{
    // Ignore chars outside of content
    if (!fWithinElement)
        return;

    // revisit.  Is it really safe to null-terminate here?
    //                Does the scanner do it already?
    //                If scanner goes up to the very end of an unterminated
    //                buffer, we may be stepping on something bad here.
    //           Probably best to modify the scanner to null terminate.
    XMLCh savedChar = chars[length];
    XMLCh *ncChars  = (XMLCh *)chars;   // cast off const
    ncChars[length] = 0;
    if (cdataSection == true)
    {
        DOMCDATASection *node = fDocument->createCDATASection(chars);
        fCurrentParent->appendChild(node);
        fCurrentNode = node;
    }
    else
    {
        if (fCurrentNode->getNodeType() == DOMNode::TEXT_NODE)
        {
            DOMText *node = (DOMText *)fCurrentNode;
            node->appendData(chars);
        }
        else
        {
            DOMText *node = fDocument->createTextNode(chars);
            fCurrentParent->appendChild(node);
            fCurrentNode = node;
        }
    }
    ncChars[length] = savedChar;
    return;
}


void XercesDOMParser::docComment(const XMLCh* const comment)
{
    DOMComment *dcom = fDocument->createComment(comment);
    fCurrentParent->appendChild(dcom);
    fCurrentNode = dcom;
}


void XercesDOMParser::docPI(  const   XMLCh* const    target
                      , const XMLCh* const    data)
{
    DOMProcessingInstruction *pi = fDocument->createProcessingInstruction
        (
        target
        , data
        );
    fCurrentParent->appendChild(pi);
    fCurrentNode = pi;
}


void XercesDOMParser::endEntityReference(const XMLEntityDecl& entDecl)
{
    if (fCreateEntityReferenceNodes == true)
    {
        if (fCurrentParent->getNodeType() == DOMNode::ENTITY_REFERENCE_NODE) {
            DOMEntityReferenceImpl *erImpl = (DOMEntityReferenceImpl *) fCurrentParent;
            erImpl->setReadOnly(true, true);
        }
        fCurrentParent = fNodeStack->pop();
        fCurrentNode   = fCurrentParent;
    }
}


void XercesDOMParser::endElement( const   XMLElementDecl&     elemDecl
                           , const unsigned int        urlId
                           , const bool                isRoot)
{
    fCurrentNode   = fCurrentParent;
    fCurrentParent = fNodeStack->pop();

    // If we've hit the end of content, clear the flag
    if (fNodeStack->empty())
        fWithinElement = false;
}


void XercesDOMParser::ignorableWhitespace(const   XMLCh* const    chars
                                    , const unsigned int    length
                                    , const bool            cdataSection)
{
    // Ignore chars before the root element
    if (!fWithinElement || !fIncludeIgnorableWhitespace)
        return;

    // revisit.  Not safe to slam in a null like this.
    XMLCh savedChar = chars[length];
    XMLCh *ncChars  = (XMLCh *)chars;   // cast off const
    ncChars[length] = chNull;

    if (fCurrentNode->getNodeType() == DOMNode::TEXT_NODE)
    {
        DOMText *node = (DOMText *)fCurrentNode;
        node->appendData(chars);
    }
    else
    {
        DOMTextImpl *node = (DOMTextImpl *)fDocument->createTextNode(chars);
        node->setIgnorableWhitespace(true);
        fCurrentParent->appendChild(node);

        fCurrentNode = node;
    }
    ncChars[length] = savedChar;
}


void XercesDOMParser::resetDocument()
{
    //
    //  The reset methods are called before a new parse event occurs.
    //  Reset this parsers state to clear out anything that may be left
    //  from a previous use, in particular the DOM document itself.
    //
    this->reset();
    fDocument = (DOMDocumentImpl *)DOMImplementation::getImplementation()->createDocument();
}


void XercesDOMParser::startDocument()
{
    // Just set the document as the current parent and current node
    fCurrentParent = fDocument;
    fCurrentNode   = fDocument;
    // set DOM error checking off
    fDocument->setErrorChecking(false);
}


void XercesDOMParser::endDocument()
{
    // set DOM error checking back on
    fDocument->setErrorChecking(true);
}


void XercesDOMParser::startElement(const  XMLElementDecl&         elemDecl
                             , const unsigned int            urlId
                             , const XMLCh* const            elemPrefix
                             , const RefVectorOf<XMLAttr>&   attrList
                             , const unsigned int            attrCount
                             , const bool                    isEmpty
                             , const bool                    isRoot)
{
    DOMElement     *elem;

    if (fScanner -> getDoNamespaces()) {    //DOM Level 2, doNamespaces on
        XMLBuffer buf;
        XMLCh* namespaceURI = 0;
        if (urlId != fScanner->getEmptyNamespaceId()) {  //TagName has a prefix
            fScanner->getURIText(urlId, buf);   //get namespaceURI
            namespaceURI = buf.getRawBuffer();
        }
        elem = createElementNSNode(namespaceURI, elemDecl.getFullName());
        DOMElementImpl *elemImpl = (DOMElementImpl *) elem;
        for (unsigned int index = 0; index < attrCount; ++index) {
            static const XMLCh XMLNS[] = {
            chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chNull
            };
            const XMLAttr* oneAttrib = attrList.elementAt(index);
            unsigned int attrURIId = oneAttrib -> getURIId();
            namespaceURI = 0;
            if (!XMLString::compareString(oneAttrib -> getName(), XMLNS))    //for xmlns=...
                attrURIId = fScanner->getXMLNSNamespaceId();
            if (attrURIId != fScanner->getEmptyNamespaceId()) {  //TagName has a prefix
                fScanner->getURIText(attrURIId, buf);   //get namespaceURI
                namespaceURI = buf.getRawBuffer();
            }
            //  revisit.  Optimize to init the named node map to the
            //                 right size up front.
            DOMAttrImpl *attr = (DOMAttrImpl *)
                fDocument->createAttributeNS(namespaceURI, oneAttrib->getQName());
            attr->setValue(oneAttrib -> getValue());
            elemImpl->setAttributeNode(attr);

            //DOMAttrImpl *attr = elemImpl->setAttributeNS(namespaceURI, oneAttrib -> getQName(),
            //    oneAttrib -> getValue());

            // Attributes of type ID.  If this is one, add it to the hashtable of IDs
            //   that is constructed for use by GetElementByID().
            //
            if (oneAttrib->getType()==XMLAttDef::ID)
            {
                if (fDocument->fNodeIDMap == 0)
                    fDocument->fNodeIDMap = new (fDocument) DOMNodeIDMap(500, fDocument);
                fDocument->fNodeIDMap->add(attr);
                attr->fNode.isIdAttr(true);
            }

            attr->setSpecified(oneAttrib->getSpecified());
        }
    }
    else {    //DOM Level 1
        elem = fDocument->createElement(elemDecl.getFullName());
        DOMElementImpl *elemImpl = (DOMElementImpl *) elem;
			for (unsigned int index = 0; index < attrCount; ++index) {
				const XMLAttr* oneAttrib = attrList.elementAt(index);
            //AttrImpl *attr = elemImpl->setAttribute(oneAttrib->getName(), oneAttrib->getValue());
            DOMAttrImpl *attr = (DOMAttrImpl *)
                fDocument->createAttribute(oneAttrib->getName());
            attr->setValue(oneAttrib -> getValue());
            elemImpl->setAttributeNode(attr);
				attr->setSpecified(oneAttrib->getSpecified());

				// Attributes of type ID.  If this is one, add it to the hashtable of IDs
				//   that is constructed for use by GetElementByID().
				//
				if (oneAttrib->getType()==XMLAttDef::ID)
				{
                if (fDocument->fNodeIDMap == 0)
                    fDocument->fNodeIDMap = new (fDocument) DOMNodeIDMap(500, fDocument);
                fDocument->fNodeIDMap->add(attr);
                attr->fNode.isIdAttr(true);
            }
        }
    }

    fCurrentParent->appendChild(elem);

    fNodeStack->push(fCurrentParent);
    fCurrentParent = elem;
    fCurrentNode = elem;
    fWithinElement = true;

    // If an empty element, do end right now (no endElement() will be called)
    if (isEmpty)
        endElement(elemDecl, urlId, isRoot);
}


void XercesDOMParser::startEntityReference(const XMLEntityDecl& entDecl)
{
    if (fCreateEntityReferenceNodes == true)
    {
        const XMLCh * entName = entDecl.getName();
        DOMEntityReference *er = fDocument->createEntityReference(entName);

        //set the readOnly flag to false before appending node, will be reset in endEntityReference
        DOMEntityReferenceImpl *erImpl = (DOMEntityReferenceImpl *) er;
        erImpl->setReadOnly(false, true);

        fCurrentParent->appendChild(er);
        fNodeStack->push(fCurrentParent);
        fCurrentParent = er;
        fCurrentNode = er;

        // this entityRef needs to be stored in Entity map too.
        // We'd decide later whether the entity nodes should be created by a
        // separated method in parser or not. For now just stick it in if
        // the ref nodes are created
        DOMNamedNodeMap *entities = fDocumentType->getEntities();
        DOMEntityImpl* entity = (DOMEntityImpl*)entities->getNamedItem(entName);
        entity->setEntityRef(er);

    }
}


void XercesDOMParser::XMLDecl(const   XMLCh* const version
                        , const XMLCh* const encoding
                        , const XMLCh* const standalone
                        , const XMLCh* const actualEncStr)
{
    // placehold for DOM Level 3
}

// ---------------------------------------------------------------------------
//  XercesDOMParser: Helper methods
// ---------------------------------------------------------------------------
DOMElement* XercesDOMParser::createElementNSNode(const XMLCh *namespaceURI,
                                              const XMLCh *qualifiedName)
{
    return fDocument->createElementNS(namespaceURI, qualifiedName);
}
// ---------------------------------------------------------------------------
//  XercesDOMParser: Deprecated methods
// ---------------------------------------------------------------------------
bool XercesDOMParser::getDoValidation() const
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

void XercesDOMParser::setDoValidation(const bool newState)
{
    fScanner->setDoValidation
    (
        newState ? XMLScanner::Val_Always : XMLScanner::Val_Never
    );
}

//doctypehandler interfaces
void XercesDOMParser::attDef
(
    const   DTDElementDecl&     elemDecl
    , const DTDAttDef&          attDef
    , const bool                ignoring
	)
{	
    if (fDocumentType->isIntSubsetReading())
    {
        XMLBuffer attString;
        if (elemDecl.hasAttDefs())
        {
            attString.append(chOpenAngle);
            attString.append(chBang);
            attString.append(XMLUni::fgAttListString);
            attString.append(chSpace);
            attString.append(elemDecl.getFullName());

            attString.append(chSpace);
            attString.append(attDef.getFullName());

            // Get the type and display it
            const XMLAttDef::AttTypes type = attDef.getType();
            switch(type)
            {
            case XMLAttDef::CData :
                attString.append(chSpace);
                attString.append(XMLUni::fgCDATAString);
                break;
            case XMLAttDef::ID :
                attString.append(chSpace);
                attString.append(XMLUni::fgIDString);
                break;
            case XMLAttDef::IDRef :
                attString.append(chSpace);
                attString.append(XMLUni::fgIDRefString);
                break;
            case XMLAttDef::IDRefs :
                attString.append(chSpace);
                attString.append(XMLUni::fgIDRefsString);
                break;
            case XMLAttDef::Entity :
                attString.append(chSpace);
                attString.append(XMLUni::fgEntityString);
                break;
            case XMLAttDef::Entities :
                attString.append(chSpace);
                attString.append(XMLUni::fgEntitiesString);
                break;
            case XMLAttDef::NmToken :
                attString.append(chSpace);
                attString.append(XMLUni::fgNmTokenString);
                break;
            case XMLAttDef::NmTokens :
                attString.append(chSpace);
                attString.append(XMLUni::fgNmTokensString);
                break;

            case XMLAttDef::Notation :
                attString.append(chSpace);
                attString.append(XMLUni::fgNotationString);
                break;

            case XMLAttDef::Enumeration :
                attString.append(chSpace);
                const XMLCh* enumString = attDef.getEnumeration();
                int length = XMLString::stringLen(enumString);
                if (length > 0) {

                    XMLBuffer anotherEnumString;

                    anotherEnumString.append(chOpenParen );
                    for(int i=0; i<length; i++) {
                        if (enumString[i] == chSpace)
                            anotherEnumString.append(chPipe);
                        else
                            anotherEnumString.append(enumString[i]);
                    }
                    anotherEnumString.append(chCloseParen);
                    attString.append(anotherEnumString.getRawBuffer());
                }
                break;
            }
            //get te default types of the attlist
            const XMLAttDef::DefAttTypes def = attDef.getDefaultType();
            switch(def)
            {
            case XMLAttDef::Required :
                attString.append(chSpace);
                attString.append(XMLUni::fgRequiredString);
                break;
            case XMLAttDef::Implied :
                attString.append(chSpace);
                attString.append(XMLUni::fgImpliedString);
                break;
            case XMLAttDef::Fixed :
                attString.append(chSpace);
                attString.append(XMLUni::fgFixedString);
                break;
            }

            const XMLCh* defaultValue = attDef.getValue();
            if (defaultValue != 0) {
                attString.append(chSpace);
                attString.append(chDoubleQuote);
                attString.append(defaultValue);
                attString.append(chDoubleQuote);
            }

            attString.append(chCloseAngle);
            fDocumentType->setInternalSubset(attString.getRawBuffer());
        }
    }
}

void XercesDOMParser::doctypeComment
(
    const   XMLCh* const    comment
)
{
    if (fDocumentType->isIntSubsetReading())
    {
        if (comment != 0)
        {
            XMLBuffer comString;
            comString.append(XMLUni::fgCommentString);
            comString.append(chSpace);
            comString.append(comment);
            comString.append(chSpace);
            comString.append(chDash);
            comString.append(chDash);
            comString.append(chCloseAngle);
            fDocumentType->setInternalSubset(comString.getRawBuffer());
        }
    }
}

void XercesDOMParser::doctypeDecl
(
    const   DTDElementDecl& elemDecl
    , const XMLCh* const    publicId
    , const XMLCh* const    systemId
    , const bool            hasIntSubset
)
{
    fDocumentType = (DOMDocumentTypeImpl *) fDocument->createDocumentType(elemDecl.getFullName(), publicId, systemId);
    fDocument->setDocumentType(fDocumentType);

}

void XercesDOMParser::doctypePI
(
    const   XMLCh* const    target
    , const XMLCh* const    data
)
{
    if (fDocumentType->isIntSubsetReading())
	{
		//add these chars to internalSubset variable
        XMLBuffer pi;
        pi.append(chOpenAngle);
        pi.append(chQuestion);
        pi.append(target);
        pi.append(chSpace);
        pi.append(data);
        pi.append(chQuestion);
        pi.append(chCloseAngle);

		fDocumentType->setInternalSubset(pi.getRawBuffer());
	}
	
}


void XercesDOMParser::doctypeWhitespace
(
    const   XMLCh* const    chars
    , const unsigned int    length
)
{
    if (fDocumentType->isIntSubsetReading())
		fDocumentType->setInternalSubset(chars);
}

void XercesDOMParser::elementDecl
(
    const   DTDElementDecl& decl
    , const bool            isIgnored
)
{
    if (fDocumentType->isIntSubsetReading())
	{
        XMLBuffer elemDecl;

        elemDecl.append(chOpenAngle);
        elemDecl.append(chBang);
        elemDecl.append(XMLUni::fgElemString);
        elemDecl.append(chSpace);
        elemDecl.append(decl.getFullName());

        //get the ContentSpec information
        const XMLCh* contentModel = decl.getFormattedContentModel();
        if (contentModel != 0) {
            elemDecl.append(chSpace);
            elemDecl.append(contentModel);
        }

        elemDecl.append(chCloseAngle);
		fDocumentType->setInternalSubset(elemDecl.getRawBuffer());
	}
}

void XercesDOMParser::endAttList
(
    const   DTDElementDecl& elemDecl
)
{
	// this section sets up default attributes.
	// default attribute nodes are stored in a NamedNodeMap DocumentTypeImpl::elements
	// default attribute data attached to the document is used to conform to the
	// DOM spec regarding creating element nodes & removing attributes with default values
	// see DocumentTypeImpl
	if (elemDecl.hasAttDefs())
	{		
		XMLAttDefList* defAttrs = &elemDecl.getAttDefList();
		XMLAttDef* attr = 0;

		DOMAttrImpl * insertAttr = 0;
      DOMElement     *elem  = fDocument->createElement(elemDecl.getFullName());
      DOMElementImpl *elemImpl = (DOMElementImpl *) elem;

		while (defAttrs->hasMoreElements())
        {
            attr = &defAttrs->nextElement();
            if (attr->getValue() != 0)
            {
                if (fScanner->getDoNamespaces())
                {
                    // DOM Level 2 wants all namespace declaration attributes
                    // to be bound to "http://www.w3.org/2000/xmlns/"
                    // So as long as the XML parser doesn't do it, it needs to
                    // done here.
                    const XMLCh* qualifiedName = attr->getFullName();
                    int index = DOMDocumentImpl::indexofQualifiedName(qualifiedName);

                    XMLBuffer buf;
                    static const XMLCh XMLNS[] = {
                        chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chNull};

                    if (index > 0) {
                        // there is prefix
                        // map to XML URI for all cases except when prefix == "xmlns"
                        XMLCh* prefix;
                        XMLCh temp[1000];

                        if (index > 999)
                            prefix = new XMLCh[index+1];
                        else
                            prefix = temp;

                        XMLString::subString(prefix ,qualifiedName, 0, index);

                        if (!XMLString::compareString(prefix,XMLNS))
                            buf.append(XMLUni::fgXMLNSURIName);
                        else
                            buf.append(XMLUni::fgXMLURIName);

                        if (index > 999)
                            delete prefix;
                    }
                    else {
                        //   No prefix
                        if (!XMLString::compareString(qualifiedName,XMLNS))
                            buf.append(XMLUni::fgXMLNSURIName);
                    }

                    insertAttr = (DOMAttrImpl *) fDocument->createAttributeNS(
                       buf.getRawBuffer(),     // NameSpaceURI
                       qualifiedName);   // qualified name

                }
                else
                {
                    // Namespaces is turned off...
                    insertAttr = (DOMAttrImpl *) fDocument->createAttribute(attr->getFullName());
                }
                insertAttr->setValue(attr->getValue());
                elemImpl->setAttributeNode(insertAttr);
                insertAttr->setSpecified(false);
            }
        }
        fDocumentType->getElements()->setNamedItem(elemImpl);
    }
}

void XercesDOMParser::endIntSubset()
{
	fDocumentType->intSubsetReading = false;
}

void XercesDOMParser::endExtSubset()
{
}

void XercesDOMParser::entityDecl
(
    const   DTDEntityDecl&  entityDecl
    , const bool            isPEDecl
    , const bool            isIgnored
)
{
    DOMEntityImpl* entity = (DOMEntityImpl *) fDocument->createEntity(entityDecl.getName());

    entity->setPublicId(entityDecl.getPublicId());
    entity->setSystemId(entityDecl.getSystemId());
    entity->setNotationName(entityDecl.getNotationName());

    DOMEntityImpl *previousDef = (DOMEntityImpl *)
	    fDocumentType->getEntities()->setNamedItem( entity );

    #ifdef _revisit
    //
    //  If this new entity node is replacing an entity node that was already
    //    in the entities named node map (happens if documents redefine the
    //    predefined entited such as lt), we need to delete the original
    //    entitiy node, assuming no-one else was referencing it.
    //
    if (previousDef != 0 && previousDef->nodeRefCount == 0)
    	        NodeImpl::deleteIf(previousDef);
    #endif

    if (fDocumentType->isIntSubsetReading())
    {
        //add thes chars to internalSubset variable
        XMLBuffer entityName;
        entityName.append(chOpenAngle);
        entityName.append(chBang);
        entityName.append(XMLUni::fgEntityString);
        entityName.append(chSpace);

        entityName.append(entityDecl.getName());

        const XMLCh* id = entity->getPublicId();
        if (id != 0) {
            entityName.append(chSpace);
            entityName.append(XMLUni::fgPubIDString);
            entityName.append(chSpace);
            entityName.append(chDoubleQuote);
            entityName.append(id);
            entityName.append(chDoubleQuote);
        }
        id = entity->getSystemId();
        if (id != 0) {
            entityName.append(chSpace);
            entityName.append(XMLUni::fgSysIDString);
            entityName.append(chSpace);
            entityName.append(chDoubleQuote);
            entityName.append(id);
            entityName.append(chDoubleQuote);

        }
        id = entity->getNotationName();
        if (id != 0) {
            entityName.append(chSpace);
            entityName.append(XMLUni::fgNDATAString);
            entityName.append(chSpace);
            entityName.append(chDoubleQuote);
            entityName.append(id);
            entityName.append(chDoubleQuote);
        }
        id = entityDecl.getValue();
        if (id !=0) {
            entityName.append(chSpace);
            entityName.append(chDoubleQuote);
            entityName.append(id);
            entityName.append(chDoubleQuote);
        }

        entityName.append(chCloseAngle);
        fDocumentType->setInternalSubset(entityName.getRawBuffer());
    }

}

void XercesDOMParser::resetDocType()
{
	fDocumentType = 0;
}

void XercesDOMParser::notationDecl
(
    const   XMLNotationDecl&    notDecl
    , const bool                isIgnored
)
{
	DOMNotationImpl* notation = (DOMNotationImpl *)fDocument->createNotation(notDecl.getName());
	notation->setPublicId(notDecl.getPublicId());
	notation->setSystemId(notDecl.getSystemId());

	fDocumentType->getNotations()->setNamedItem( notation );

}

void XercesDOMParser::startAttList
(
    const   DTDElementDecl& elemDecl
)
{
}

void XercesDOMParser::startIntSubset()
{
	fDocumentType->intSubsetReading = true;
}

void XercesDOMParser::startExtSubset()
{
}

void XercesDOMParser::TextDecl
(
    const   XMLCh* const    versionStr
    , const XMLCh* const    encodingStr
)
{
}
