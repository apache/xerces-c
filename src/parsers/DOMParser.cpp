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
* $Log$
* Revision 1.16  2000/04/18 18:44:51  roddey
* Change "const static" to "static const" Bug #120
*
* Revision 1.15  2000/04/12 22:58:29  roddey
* Added support for 'auto validate' mode.
*
* Revision 1.14  2000/03/24 21:25:28  abagchi
* Added getElementById() from patch submitted by Jeff Lewis
*
* Revision 1.13  2000/03/24 01:31:12  chchou
* Fix bug #8 to support ignorable whitespace text nodes
*
* Revision 1.12  2000/03/11 02:17:35  chchou
* Fix bug # 29 to have the spefified flag set correctly for AttrImpl.
*
* Revision 1.11  2000/03/03 01:29:33  roddey
* Added a scanReset()/parseReset() method to the scanner and
* parsers, to allow for reset after early exit from a progressive parse.
* Added calls to new Terminate() call to all of the samples. Improved
* documentation in SAX and DOM parsers.
*
* Revision 1.10  2000/03/02 19:54:33  roddey
* This checkin includes many changes done while waiting for the
* 1.1.0 code to be finished. I can't list them all here, but a list is
* available elsewhere.
*
* Revision 1.9  2000/02/17 17:47:26  andyh
* Update Doc++ API comments
* NameSpace update to track W3C
* Changes were made by Chih Hsiang Chou
*
* Revision 1.8  2000/02/17 03:53:50  rahulj
* Added some new getters to query the parser state and
* Finished documenting the public and protected methods.
*
* Revision 1.7  2000/02/06 07:47:56  rahulj
* Year 2K copyright swat.
*
* Revision 1.6  2000/02/04 01:49:50  aruna1
* TreeWalker and NodeIterator changes
*
* Revision 1.5  2000/02/03 18:43:24  roddey
* Incorrect use of 'inline' inside the cpp file. Its wrong, and it also prevents
* export on some platforms.
*
* Revision 1.4  2000/01/19 21:40:57  andyh
* Remove a few remaining dependencies on the (now defunct)
* XML StdOut stream.
*
* Revision 1.3  2000/01/12 00:15:22  roddey
* Changes to deal with multiply nested, relative pathed, entities and to deal
* with the new URL class changes.
*
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
#include <internal/XMLScanner.hpp>
#include <validators/DTD/DTDValidator.hpp>
#include <parsers/DOMParser.hpp>
#include <dom/ElementImpl.hpp>
#include <dom/AttrImpl.hpp>
#include <dom/TextImpl.hpp>


// ---------------------------------------------------------------------------
//  DOMParser: Constructors and Destructor
// ---------------------------------------------------------------------------
DOMParser::DOMParser(XMLValidator* const valToAdopt) :

fErrorHandler(0)
, fEntityResolver(0)
, fExpandEntityReferences(false)
, fIncludeIgnorableWhitespace(true)
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
    //
    //  Note: DOM Documents are reference counted. Doing this assignment
    //  will cause the old one to go away unless application code is also
    //  holding a reference to it.
    //
    fDocument = DOM_Document::createDocument();

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
            // Register identifiers
            if (oneAttrib->getType()==XMLAttDef::ID)
            {
               fDocument.putIdentifier(oneAttrib->getValue(), elem);
            }
            attr->setSpecified(oneAttrib->getSpecified());
        }
    } else {    //DOM Level 1
        elem = fDocument.createElement(elemDecl.getFullName());
        ElementImpl *elemImpl = (ElementImpl *) elem.fImpl;
        for (unsigned int index = 0; index < attrCount; ++index) {
           const XMLAttr* oneAttrib = attrList.elementAt(index);
           AttrImpl *attr = elemImpl->setAttribute(oneAttrib->getName(), oneAttrib->getValue());
           // Register identifiers
           if (oneAttrib->getType()==XMLAttDef::ID)
           {
              fDocument.putIdentifier(oneAttrib->getValue(), elem);
	   }
           attr->setSpecified(oneAttrib->getSpecified());
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
