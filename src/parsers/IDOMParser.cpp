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
*  handler with the scanner. In these handler methods, appropriate IDOM nodes
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
#include <parsers/IDOMParser.hpp>
#include <idom/IDOM_DOMImplementation.hpp>
#include <idom/IDOM_Element.hpp>
#include <idom/IDAttrImpl.hpp>
#include <idom/IDOM_CDATASection.hpp>
#include <idom/IDOM_Comment.hpp>
#include <idom/IDTextImpl.hpp>
#include <idom/IDDocumentImpl.hpp>
#include <idom/IDDocumentTypeImpl.hpp>
#include <idom/IDOM_DocumentType.hpp>
#include <idom/IDElementImpl.hpp>
#include <idom/IDEntityImpl.hpp>
#include <idom/IDEntityReferenceImpl.hpp>
#include <idom/IDNotationImpl.hpp>
#include <idom/IDOM_NamedNodeMap.hpp>
#include <idom/IDOM_ProcessingInstruction.hpp>
#include <idom/IDNodeIDMap.hpp>


#include <validators/common/ContentSpecNode.hpp>
#include <validators/DTD/DTDAttDefList.hpp>

// ---------------------------------------------------------------------------
//  IDOMParser: Constructors and Destructor
// ---------------------------------------------------------------------------
IDOMParser::IDOMParser(XMLValidator* const valToAdopt) :

fErrorHandler(0)
, fEntityResolver(0)
, fCreateEntityReferenceNodes(false)
, fIncludeIgnorableWhitespace(true)
, fNodeStack(0)
, fScanner(0)
, fDocument(0)
, fDocumentVector(0)
{
    //
    //  Create a scanner and tell it what validator to use. Then set us
    //  as the document event handler so we can fill the IDOM document.
    //
    fScanner = new XMLScanner(valToAdopt);
    fScanner->setDocHandler(this);
    fScanner->setDocTypeHandler(this);

    fNodeStack = new ValueStackOf<IDOM_Node*>(64);
    this->reset();


}


IDOMParser::~IDOMParser()
{
    if (fDocumentVector)
        delete fDocumentVector;

    delete fDocument;
    delete fNodeStack;
    delete fScanner;
}


void IDOMParser::reset()
{
    // if fDocument exists already, store the old pointer in the vector for deletion later
    if (fDocument) {
        if (!fDocumentVector) {
            // allocate the vector if not exists yet
            fDocumentVector  = new RefVectorOf<IDDocumentImpl>(10, true) ;
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
//  IDOMParser: Getter methods
// ---------------------------------------------------------------------------
IDOM_Document* IDOMParser::getDocument()
{
    return fDocument;
}

const XMLValidator& IDOMParser::getValidator() const
{
    return *fScanner->getValidator();
}

bool IDOMParser::getDoNamespaces() const
{
    return fScanner->getDoNamespaces();
}

bool IDOMParser::getExitOnFirstFatalError() const
{
    return fScanner->getExitOnFirstFatal();
}

bool IDOMParser::getValidationConstraintFatal() const
{
    return fScanner->getValidationConstraintFatal();
}

IDOMParser::ValSchemes IDOMParser::getValidationScheme() const
{
    const XMLScanner::ValSchemes scheme = fScanner->getValidationScheme();

    if (scheme == XMLScanner::Val_Always)
        return Val_Always;
    else if (scheme == XMLScanner::Val_Never)
        return Val_Never;

    return Val_Auto;
}

bool IDOMParser::getDoSchema() const
{
    return fScanner->getDoSchema();
}

bool IDOMParser::getValidationSchemaFullChecking() const
{
    return fScanner->getValidationSchemaFullChecking();
}

int IDOMParser::getErrorCount() const
{
    return fScanner->getErrorCount();
}



// ---------------------------------------------------------------------------
//  IDOMParser: Setter methods
// ---------------------------------------------------------------------------
void IDOMParser::setDoNamespaces(const bool newState)
{
    fScanner->setDoNamespaces(newState);
}

void IDOMParser::setErrorHandler(ErrorHandler* const handler)
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

void IDOMParser::setEntityResolver(EntityResolver* const handler)
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

void IDOMParser::setExitOnFirstFatalError(const bool newState)
{
    fScanner->setExitOnFirstFatal(newState);
}

void IDOMParser::setValidationConstraintFatal(const bool newState)
{
    fScanner->setValidationConstraintFatal(newState);
}

void IDOMParser::setValidationScheme(const ValSchemes newScheme)
{
    if (newScheme == Val_Never)
        fScanner->setValidationScheme(XMLScanner::Val_Never);
    else if (newScheme == Val_Always)
        fScanner->setValidationScheme(XMLScanner::Val_Always);
    else
        fScanner->setValidationScheme(XMLScanner::Val_Auto);
}

void IDOMParser::setDoSchema(const bool newState)
{
    fScanner->setDoSchema(newState);
}

void IDOMParser::setValidationSchemaFullChecking(const bool schemaFullChecking)
{
    fScanner->setValidationSchemaFullChecking(schemaFullChecking);
}

// ---------------------------------------------------------------------------
//  IDOMParser: Parsing methods
// ---------------------------------------------------------------------------
void IDOMParser::parse(const InputSource& source, const bool reuseGrammar)
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

void IDOMParser::parse(const XMLCh* const systemId, const bool reuseGrammar)
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

void IDOMParser::parse(const char* const systemId, const bool reuseGrammar)
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
//  IDOMParser: Progressive parse methods
// ---------------------------------------------------------------------------
bool IDOMParser::parseFirst( const   XMLCh* const    systemId
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

bool IDOMParser::parseFirst( const   char* const         systemId
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

bool IDOMParser::parseFirst( const   InputSource&    source
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

bool IDOMParser::parseNext(XMLPScanToken& token)
{
    return fScanner->scanNext(token);
}

void IDOMParser::parseReset(XMLPScanToken& token)
{
    // Reset the scanner, and then reset the parser
    fScanner->scanReset(token);
    reset();
}


// ---------------------------------------------------------------------------
//  IDOMParser: Implementation of the XMLErrorReporter interface
// ---------------------------------------------------------------------------
void IDOMParser::error(  const   unsigned int                code
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

void IDOMParser::resetErrors()
{
}


// ---------------------------------------------------------------------------
//  IDOMParser: Implementation of XMLEntityHandler interface
// ---------------------------------------------------------------------------
InputSource*
IDOMParser::resolveEntity(const XMLCh* const publicId, const XMLCh* const systemId)
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
//  IDOMParser: Implementation of XMLDocumentHandler interface
// ---------------------------------------------------------------------------
void IDOMParser::docCharacters(  const   XMLCh* const    chars
                              , const unsigned int    length
                              , const bool            cdataSection)
{
    // Ignore chars outside of content
    if (!fWithinElement)
        return;

    // idom_revisit.  Is it really safe to null-terminate here?
    //                Does the scanner do it already?
    //                If scanner goes up to the very end of an unterminated
    //                buffer, we may be stepping on something bad here.
    //           Probably best to modify the scanner to null terminate.
    XMLCh savedChar = chars[length];
    XMLCh *ncChars  = (XMLCh *)chars;   // cast off const
    ncChars[length] = 0;
    if (cdataSection == true)
    {
        IDOM_CDATASection *node = fDocument->createCDATASection(chars);
        fCurrentParent->appendChild(node);
        fCurrentNode = node;
    }
    else
    {
        if (fCurrentNode->getNodeType() == IDOM_Node::TEXT_NODE)
        {
            IDOM_Text *node = (IDOM_Text *)fCurrentNode;
            node->appendData(chars);
        }
        else
        {
            IDOM_Text *node = fDocument->createTextNode(chars);
            //If the node type is entityRef then set the readOnly flag to false before appending node
            if (fCurrentParent->getNodeType() == IDOM_Node::ENTITY_REFERENCE_NODE) {
                IDEntityReferenceImpl *erImpl = (IDEntityReferenceImpl *) fCurrentParent;
                bool oldReadFlag = erImpl->fNode.isReadOnly();
                erImpl->fNode.isReadOnly(false);
                fCurrentParent->appendChild(node);
                erImpl->fNode.isReadOnly(oldReadFlag);
            }
            else
            {
                fCurrentParent->appendChild(node);
			   }
            fCurrentNode = node;
        }
    }
    ncChars[length] = savedChar;
    return;
}


void IDOMParser::docComment(const XMLCh* const comment)
{
    IDOM_Comment *dcom = fDocument->createComment(comment);
    fCurrentParent->appendChild(dcom);
    fCurrentNode = dcom;
}


void IDOMParser::docPI(  const   XMLCh* const    target
                      , const XMLCh* const    data)
{
    IDOM_ProcessingInstruction *pi = fDocument->createProcessingInstruction
        (
        target
        , data
        );
    fCurrentParent->appendChild(pi);
    fCurrentNode = pi;
}


void IDOMParser::endEntityReference(const XMLEntityDecl& entDecl)
{
    if (fCreateEntityReferenceNodes == true)
    {
        fCurrentParent = fNodeStack->pop();
        fCurrentNode   = fCurrentParent;
    }
}


void IDOMParser::endElement( const   XMLElementDecl&     elemDecl
                           , const unsigned int        urlId
                           , const bool                isRoot)
{
    fCurrentNode   = fCurrentParent;
    fCurrentParent = fNodeStack->pop();

    // If we've hit the end of content, clear the flag
    if (fNodeStack->empty())
        fWithinElement = false;
}


void IDOMParser::ignorableWhitespace(const   XMLCh* const    chars
                                    , const unsigned int    length
                                    , const bool            cdataSection)
{
    // Ignore chars before the root element
    if (!fWithinElement || !fIncludeIgnorableWhitespace)
        return;

    // idom_revisit.  Not safe to slam in a null like this.
    XMLCh savedChar = chars[length];
    XMLCh *ncChars  = (XMLCh *)chars;   // cast off const
    ncChars[length] = chNull;

    if (fCurrentNode->getNodeType() == IDOM_Node::TEXT_NODE)
    {
        IDOM_Text *node = (IDOM_Text *)fCurrentNode;
        node->appendData(chars);
    }
    else
    {
        IDTextImpl *node = (IDTextImpl *)fDocument->createTextNode(chars);
        node->setIgnorableWhitespace(true);
        //If the node type is entityRef then set the readOnly flag to false before appending node
        if (fCurrentParent->getNodeType() == IDOM_Node::ENTITY_REFERENCE_NODE) {
            IDEntityReferenceImpl *erImpl = (IDEntityReferenceImpl *) fCurrentParent;
            bool oldReadFlag = erImpl->fNode.isReadOnly();
            erImpl->fNode.isReadOnly(false);
            fCurrentParent->appendChild(node);
            erImpl->fNode.isReadOnly(oldReadFlag);
        }

        else
        {
            fCurrentParent->appendChild(node);
        }

        fCurrentNode = node;
    }
    ncChars[length] = savedChar;
}


void IDOMParser::resetDocument()
{
    //
    //  The reset methods are called before a new parse event occurs.
    //  Reset this parsers state to clear out anything that may be left
    //  from a previous use, in particular the IDOM document itself.
    //
    this->reset();
    fDocument = (IDDocumentImpl *)IDOM_DOMImplementation::getImplementation()->createDocument();
}


void IDOMParser::startDocument()
{
    // Just set the document as the current parent and current node
    fCurrentParent = fDocument;
    fCurrentNode   = fDocument;
    // set IDOM error checking off
    fDocument->setErrorChecking(false);
}


void IDOMParser::endDocument()
{
    // set IDOM error checking back on
    fDocument->setErrorChecking(true);
}


void IDOMParser::startElement(const  XMLElementDecl&         elemDecl
                             , const unsigned int            urlId
                             , const XMLCh* const            elemPrefix
                             , const RefVectorOf<XMLAttr>&   attrList
                             , const unsigned int            attrCount
                             , const bool                    isEmpty
                             , const bool                    isRoot)
{
    IDOM_Element     *elem;

    if (fScanner -> getDoNamespaces()) {    //IDOM Level 2, doNamespaces on
        XMLBuffer buf;
        XMLCh* namespaceURI = 0;
        if (urlId != fScanner->getEmptyNamespaceId()) {  //TagName has a prefix
            fScanner->getURIText(urlId, buf);   //get namespaceURI
            namespaceURI = buf.getRawBuffer();
        }
        elem = fDocument->createElementNS(namespaceURI, elemDecl.getFullName());
        IDElementImpl *elemImpl = (IDElementImpl *) elem;
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
            //  idom_revisit.  Optimize to init the named node map to the
            //                 right size up front.
            IDAttrImpl *attr = (IDAttrImpl *)
                fDocument->createAttributeNS(namespaceURI, oneAttrib->getQName());
            attr->setValue(oneAttrib -> getValue());
            elemImpl->setAttributeNode(attr);

            //IDAttrImpl *attr = elemImpl->setAttributeNS(namespaceURI, oneAttrib -> getQName(),
            //    oneAttrib -> getValue());

            // Attributes of type ID.  If this is one, add it to the hashtable of IDs
            //   that is constructed for use by GetElementByID().
            //
            if (oneAttrib->getType()==XMLAttDef::ID)
            {
                if (fDocument->fNodeIDMap == 0)
                    fDocument->fNodeIDMap = new (fDocument) IDNodeIDMap(500, fDocument);
                fDocument->fNodeIDMap->add(attr);
                attr->fNode.isIdAttr(true);
            }

            attr->setSpecified(oneAttrib->getSpecified());
        }
    }
    else {    //DOM Level 1
        elem = fDocument->createElement(elemDecl.getFullName());
        IDElementImpl *elemImpl = (IDElementImpl *) elem;
			for (unsigned int index = 0; index < attrCount; ++index) {
				const XMLAttr* oneAttrib = attrList.elementAt(index);
            //AttrImpl *attr = elemImpl->setAttribute(oneAttrib->getName(), oneAttrib->getValue());
            IDAttrImpl *attr = (IDAttrImpl *)
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
                    fDocument->fNodeIDMap = new (fDocument) IDNodeIDMap(500, fDocument);
                fDocument->fNodeIDMap->add(attr);
                attr->fNode.isIdAttr(true);
            }
        }
    }

    //If the node type is entityRef then set the readOnly flag to false before appending node
    bool oldReadFlag;
    if (fCurrentParent->getNodeType() == IDOM_Node::ENTITY_REFERENCE_NODE) {
        IDEntityReferenceImpl *erNode = (IDEntityReferenceImpl *)fCurrentParent;
        oldReadFlag = erNode->fNode.isReadOnly();
        erNode->fNode.isReadOnly(false);
        fCurrentParent->appendChild(elem);
        erNode->fNode.isReadOnly(oldReadFlag);
    }

    else
    {
        fCurrentParent->appendChild(elem);
    }

    fNodeStack->push(fCurrentParent);
    fCurrentParent = elem;
    fCurrentNode = elem;
    fWithinElement = true;

    // If an empty element, do end right now (no endElement() will be called)
    if (isEmpty)
        endElement(elemDecl, urlId, isRoot);
}


void IDOMParser::startEntityReference(const XMLEntityDecl& entDecl)
{
    if (fCreateEntityReferenceNodes == true)
    {
        const XMLCh * entName = entDecl.getName();
        IDOM_EntityReference *er = fDocument->createEntityReference(entName);
        fCurrentParent->appendChild(er);
        fNodeStack->push(fCurrentParent);
        fCurrentParent = er;
        fCurrentNode = er;

        // this entityRef needs to be stored in Entity map too.
        // We'd decide later whether the entity nodes should be created by a
        // separated method in parser or not. For now just stick it in if
        // the ref nodes are created
        IDOM_NamedNodeMap *entities = fDocumentType->getEntities();
        IDEntityImpl* entity = (IDEntityImpl*)entities->getNamedItem(entName);
        entity->setEntityRef(er);

    }
}


void IDOMParser::XMLDecl(const   XMLCh* const version
                        , const XMLCh* const encoding
                        , const XMLCh* const standalone
                        , const XMLCh* const actualEncStr)
{
    // placehold for DOM Level 3
}



// ---------------------------------------------------------------------------
//  IDOMParser: Deprecated methods
// ---------------------------------------------------------------------------
bool IDOMParser::getDoValidation() const
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

void IDOMParser::setDoValidation(const bool newState)
{
    fScanner->setDoValidation
    (
        newState ? XMLScanner::Val_Always : XMLScanner::Val_Never
    );
}

//doctypehandler interfaces
void IDOMParser::attDef
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

void IDOMParser::doctypeComment
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

void IDOMParser::doctypeDecl
(
    const   DTDElementDecl& elemDecl
    , const XMLCh* const    publicId
    , const XMLCh* const    systemId
    , const bool            hasIntSubset
)
{
    fDocumentType = (IDDocumentTypeImpl *) fDocument->createDocumentType(elemDecl.getFullName(), publicId, systemId);
    fDocument->setDocumentType(fDocumentType);

}

void IDOMParser::doctypePI
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


void IDOMParser::doctypeWhitespace
(
    const   XMLCh* const    chars
    , const unsigned int    length
)
{
    if (fDocumentType->isIntSubsetReading())
		fDocumentType->setInternalSubset(chars);
}

void IDOMParser::elementDecl
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

void IDOMParser::endAttList
(
    const   DTDElementDecl& elemDecl
)
{
	// this section sets up default attributes.
	// default attribute nodes are stored in a NamedNodeMap DocumentTypeImpl::elements
	// default attribute data attached to the document is used to conform to the
	// IDOM spec regarding creating element nodes & removing attributes with default values
	// see DocumentTypeImpl
	if (elemDecl.hasAttDefs())
	{		
		XMLAttDefList* defAttrs = &elemDecl.getAttDefList();
		XMLAttDef* attr = 0;

		IDAttrImpl * insertAttr = 0;
      IDOM_Element     *elem  = fDocument->createElement(elemDecl.getFullName());
      IDElementImpl *elemImpl = (IDElementImpl *) elem;

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
                    int index = IDDocumentImpl::indexofQualifiedName(qualifiedName);

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

                    insertAttr = (IDAttrImpl *) fDocument->createAttributeNS(
                       buf.getRawBuffer(),     // NameSpaceURI
                       qualifiedName);   // qualified name

                }
                else
                {
                    // Namespaces is turned off...
                    insertAttr = (IDAttrImpl *) fDocument->createAttribute(attr->getFullName());
                }
                insertAttr->setValue(attr->getValue());
                elemImpl->setAttributeNode(insertAttr);
                insertAttr->setSpecified(false);
            }
        }
        fDocumentType->getElements()->setNamedItem(elemImpl);
    }
}

void IDOMParser::endIntSubset()
{
	fDocumentType->intSubsetReading = false;
}

void IDOMParser::endExtSubset()
{
}

void IDOMParser::entityDecl
(
    const   DTDEntityDecl&  entityDecl
    , const bool            isPEDecl
    , const bool            isIgnored
)
{
    IDEntityImpl* entity = (IDEntityImpl *) fDocument->createEntity(entityDecl.getName());

    entity->setPublicId(entityDecl.getPublicId());
    entity->setSystemId(entityDecl.getSystemId());
    entity->setNotationName(entityDecl.getNotationName());

    IDEntityImpl *previousDef = (IDEntityImpl *)
	    fDocumentType->getEntities()->setNamedItem( entity );

    #ifdef idom_revisit
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

void IDOMParser::resetDocType()
{
	fDocumentType = 0;
}

void IDOMParser::notationDecl
(
    const   XMLNotationDecl&    notDecl
    , const bool                isIgnored
)
{
	IDNotationImpl* notation = (IDNotationImpl *)fDocument->createNotation(notDecl.getName());
	notation->setPublicId(notDecl.getPublicId());
	notation->setSystemId(notDecl.getSystemId());

	fDocumentType->getNotations()->setNamedItem( notation );

}

void IDOMParser::startAttList
(
    const   DTDElementDecl& elemDecl
)
{
}

void IDOMParser::startIntSubset()
{
	fDocumentType->intSubsetReading = true;
}

void IDOMParser::startExtSubset()
{
}

void IDOMParser::TextDecl
(
    const   XMLCh* const    versionStr
    , const XMLCh* const    encodingStr
)
{
}
