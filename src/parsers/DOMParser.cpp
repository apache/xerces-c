/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
 * $Log$
 * Revision 1.2  2000/01/05 01:16:11  andyh
 * DOM Level 2 core, namespace support added.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:49  twl
 * Initial checkin
 *
 * Revision 1.7  1999/11/08 20:44:52  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */



// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <sax/EntityResolver.hpp>
#include <sax/ErrorHandler.hpp>
#include <sax/SAXParseException.hpp>
#include <framework/XMLNotationDecl.hpp>
#include <util/IOException.hpp>
#include <internal/URLInputSource.hpp>
#include <internal/XMLScanner.hpp>
#include <validators/DTD/DTDValidator.hpp>
#include <parsers/DOMParser.hpp>


// ---------------------------------------------------------------------------
//  DOMParser: Constructors and Destructor
// ---------------------------------------------------------------------------
DOMParser::DOMParser(XMLValidator* const valToAdopt) :

    fErrorHandler(0)
    , fEntityResolver(0)
    , fExpandEntityReferences(false)
    , fNodeStack(0)
    , fScanner(0)
    , fValidator(valToAdopt)
{

    // Create the validator if one was not provided
    if (!fValidator)
        fValidator = new DTDValidator;

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
    fDocument = DOM_Document::createDocument();
        //   Note:  DOM Documents are reference counted.  Doing this 
        //   assignment will cause the old one to go away unless 
        //   application code is also holding a reference to it.
    fCurrentParent   = 0;
    fCurrentNode     = 0;
    fParseInProgress = false;
    fWithinElement   = false;
    fNodeStack->removeAllElements();
};



// ---------------------------------------------------------------------------
//  DOMParser: Getter methods
// ---------------------------------------------------------------------------
inline const XMLValidator& DOMParser::getValidator() const
{
    return *fValidator;
}


// ---------------------------------------------------------------------------
//  DOMParser: Setter methods
// ---------------------------------------------------------------------------
void DOMParser::setDoNamespaces(const bool newState)
{
    fScanner->setDoNamespaces(newState);
}

void DOMParser::setDoValidation(const bool newState)
{
    fScanner->setDoValidation(newState);
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


// ---------------------------------------------------------------------------
//  DOMParser: Parsing methods
// ---------------------------------------------------------------------------
void DOMParser::parse(const InputSource& source, const bool reuseValidator)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXML(IOException, XML4CExcepts::Gen_ParseInProgress);

    try
    { 
        fParseInProgress = true;
        fScanner->scanDocument(source, reuseValidator);
        fParseInProgress = false;
    }

    catch(const SAXException&)
    {
        fParseInProgress = false;
        throw;
    }

    catch(const XMLException&)
    {
        fParseInProgress = false;
        throw;
    }
}

void DOMParser::parse(const XMLCh* const systemId, const bool reuseValidator)
{
    // Just call the URL input source version
    parse(URLInputSource(systemId), reuseValidator);
}

void DOMParser::parse(const char* const systemId, const bool reuseValidator)
{
    // Just call the URL input source version
    parse(URLInputSource(systemId), reuseValidator);
}



// ---------------------------------------------------------------------------
//  DOMParser: Progressive parse methods
// ---------------------------------------------------------------------------
bool DOMParser::parseFirst( const   XMLCh* const    systemId
                            ,       XMLPScanToken&  toFill
                            , const bool            reuseValidator)
{
    // Call the other version with a URL input source
    return parseFirst(URLInputSource(systemId), toFill, reuseValidator);
}

bool DOMParser::parseFirst( const   char* const         systemId
                            ,       XMLPScanToken&      toFill
                            , const bool                reuseValidator)
{
    // Call the other version with a URL input source
    return parseFirst(URLInputSource(systemId), toFill, reuseValidator);
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
        ThrowXML(IOException, XML4CExcepts::Gen_ParseInProgress);

    return fScanner->scanFirst(source, toFill, reuseValidator);
}

bool DOMParser::parseNext(XMLPScanToken& token)
{
    return fScanner->scanNext(token);
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
            fCurrentParent.appendChild(node);
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
    if (fExpandEntityReferences == true)
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
    if (!fWithinElement)
        return;

    if (fCurrentNode.getNodeType() == DOM_Node::TEXT_NODE)
    {
        DOM_Text node = (DOM_Text&)fCurrentNode;
        node.appendData(DOMString(chars, length));
    }
     else
    {
        DOM_Text node = fDocument.createTextNode(DOMString(chars, length));
        fCurrentParent.appendChild(node);
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
}


void DOMParser::startElement(const  XMLElementDecl&         elemDecl
                            , const unsigned int            urlId
                            , const XMLCh* const            elemPrefix
                            , const RefVectorOf<XMLAttr>&   attrList
                            , const unsigned int            attrCount
                            , const bool                    isEmpty
                            , const bool                    isRoot)
{
    DOM_Element elem;
    if (fScanner -> getDoNamespaces()) {    //DOM Level 2, doNamespaces on
        unsigned int globalNSid = fValidator -> getGlobalNamespaceId();
	unsigned int xmlnsNSid = fValidator -> getXMLNSNamespaceId();
	XMLBuffer buf;
	DOMString namespaceURI = 0;
        if (urlId != globalNSid) {	//TagName has a prefix
	    fValidator -> getURIText(urlId, buf);   //get namespaceURI
	    namespaceURI = DOMString(buf.getRawBuffer());
	}
	elem = fDocument.createElementNS(namespaceURI, elemDecl.getFullName());
	for (unsigned int index = 0; index < attrCount; ++index) {
	    const XMLAttr* oneAttrib = attrList.elementAt(index);
	    unsigned int attrURIId = oneAttrib -> getURIId();
	    namespaceURI = 0;
	    if (attrURIId != globalNSid && attrURIId != xmlnsNSid) {
		//Attribute Name has a prefix != "xmlns"
		fValidator -> getURIText(attrURIId, buf);   //get namespaceURI
		namespaceURI = DOMString(buf.getRawBuffer());
	    }
	    elem.setAttributeNS(namespaceURI, oneAttrib -> getQName(),
		oneAttrib -> getValue());
	}
    } else {	//DOM Level 1
	elem = fDocument.createElement(elemDecl.getFullName());
	for (unsigned int index = 0; index < attrCount; ++index) {
	    const XMLAttr* oneAttrib = attrList.elementAt(index);
	    elem.setAttribute(oneAttrib->getName(), oneAttrib->getValue());
	}
    }

    fCurrentParent.appendChild(elem);
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
    if (fExpandEntityReferences == true)
    {
        DOM_EntityReference er = fDocument.createEntityReference
        (
            DOMString(entDecl.getName())
        );
        fCurrentParent.appendChild(er);
        fNodeStack->push(fCurrentParent);
        fCurrentParent = er;
        fCurrentNode = er;
    }
}


void DOMParser::XMLDecl(const   XMLCh* const
                        , const XMLCh* const
                        , const XMLCh* const
                        , const XMLCh* const)
{
    // This is not used by DOM at this time
}
