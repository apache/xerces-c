/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002,2004 The Apache Software Foundation.  All rights
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
#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/internal/XMLScannerResolver.hpp>
#include <xercesc/internal/ElemStack.hpp>
//#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/framework/XMLNotationDecl.hpp>
#include <xercesc/framework/XMLValidator.hpp>
#include <xercesc/util/IOException.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
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
#include <xercesc/dom/impl/DOMProcessingInstructionImpl.hpp>
#include <xercesc/dom/impl/DOMNodeIDMap.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/common/GrammarResolver.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN


// ---------------------------------------------------------------------------
//  AbstractDOMParser: Constructors and Destructor
// ---------------------------------------------------------------------------
AbstractDOMParser::AbstractDOMParser( XMLValidator* const   valToAdopt
                                    , MemoryManager* const  manager
                                    , XMLGrammarPool* const gramPool) :

  fCreateEntityReferenceNodes(true)
, fIncludeIgnorableWhitespace(true)
, fWithinElement(false)
, fParseInProgress(false)
, fCreateCommentNodes(true)
, fDocumentAdoptedByUser(false)
, fScanner(0)
, fImplementationFeatures(0)
, fCurrentParent(0)
, fCurrentNode(0)
, fCurrentEntity(0)
, fDocument(0)
, fNodeStack(0)
, fDocumentType(0)
, fDocumentVector(0)
, fGrammarResolver(0)
, fURIStringPool(0)
, fValidator(valToAdopt)
, fMemoryManager(manager)
, fGrammarPool(gramPool)
, fBufMgr(manager)
, fInternalSubset(fBufMgr.bidOnBuffer())
, fPSVIHandler(0)
{
    try
    {
        initialize();
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
       cleanUp();
       throw;
    }
}


AbstractDOMParser::~AbstractDOMParser()
{
    cleanUp();
}

// ---------------------------------------------------------------------------
//  AbstractDOMParser: Initialize/CleanUp methods
// ---------------------------------------------------------------------------
void AbstractDOMParser::initialize()
{
    //  Create grammar resolver and string pool to pass to the scanner
    fGrammarResolver = new (fMemoryManager) GrammarResolver(fGrammarPool, fMemoryManager);
    fURIStringPool = fGrammarResolver->getStringPool();

    //  Create a scanner and tell it what validator to use. Then set us
    //  as the document event handler so we can fill the DOM document.
    fScanner = XMLScannerResolver::getDefaultScanner(fValidator, fGrammarResolver, fMemoryManager);
    fScanner->setDocHandler(this);
    fScanner->setDocTypeHandler(this);
    fScanner->setURIStringPool(fURIStringPool);

    fNodeStack = new (fMemoryManager) ValueStackOf<DOMNode*>(64, fMemoryManager);
    this->reset();
}

void AbstractDOMParser::cleanUp()
{
    if (fDocumentVector)
        delete fDocumentVector;

    if (!fDocumentAdoptedByUser && fDocument)
        fDocument->release();

    delete fNodeStack;
    delete fScanner;
    delete fGrammarResolver;
    // grammar pool *always* owns this
    //delete fURIStringPool;
    fMemoryManager->deallocate(fImplementationFeatures);

    if (fValidator)
        delete fValidator;
}

// ---------------------------------------------------------------------------
//  AbstractDOMParser: Utilities
// ---------------------------------------------------------------------------
void AbstractDOMParser::reset()
{
    // if fDocument exists already, store the old pointer in the vector for deletion later
    if (fDocument && !fDocumentAdoptedByUser) {
        if (!fDocumentVector) {
            // allocate the vector if not exists yet
            fDocumentVector  = new (fMemoryManager) RefVectorOf<DOMDocumentImpl>(10, true, fMemoryManager) ;
        }
        fDocumentVector->addElement(fDocument);
    }

    fDocument = 0;
    resetDocType();
    fCurrentParent   = 0;
    fCurrentNode     = 0;
    fCurrentEntity   = 0;
    fParseInProgress = false;
    fWithinElement   = false;
    fDocumentAdoptedByUser = false;
    fNodeStack->removeAllElements();
    fInternalSubset.reset();
}


void AbstractDOMParser::resetPool()
{
    //  We cannot enter here while a regular parse is in progress.
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    if (fDocumentVector)
        fDocumentVector->removeAllElements();

    if (!fDocumentAdoptedByUser && fDocument)
        fDocument->release();

    fDocument = 0;
}

bool AbstractDOMParser::isDocumentAdopted() const
{
    return fDocumentAdoptedByUser;
}

DOMDocument* AbstractDOMParser::adoptDocument()
{
    fDocumentAdoptedByUser = true;
    return fDocument;
}


// ---------------------------------------------------------------------------
//  AbstractDOMParser: Getter methods
// ---------------------------------------------------------------------------
DOMDocument* AbstractDOMParser::getDocument()
{
    return fDocument;
}

const XMLValidator& AbstractDOMParser::getValidator() const
{
    return *fScanner->getValidator();
}

bool AbstractDOMParser::getDoNamespaces() const
{
    return fScanner->getDoNamespaces();
}

bool AbstractDOMParser::getExitOnFirstFatalError() const
{
    return fScanner->getExitOnFirstFatal();
}

bool AbstractDOMParser::getValidationConstraintFatal() const
{
    return fScanner->getValidationConstraintFatal();
}

AbstractDOMParser::ValSchemes AbstractDOMParser::getValidationScheme() const
{
    const XMLScanner::ValSchemes scheme = fScanner->getValidationScheme();

    if (scheme == XMLScanner::Val_Always)
        return Val_Always;
    else if (scheme == XMLScanner::Val_Never)
        return Val_Never;

    return Val_Auto;
}

bool AbstractDOMParser::getDoSchema() const
{
    return fScanner->getDoSchema();
}

bool AbstractDOMParser::getValidationSchemaFullChecking() const
{
    return fScanner->getValidationSchemaFullChecking();
}

int AbstractDOMParser::getErrorCount() const
{
    return fScanner->getErrorCount();
}

XMLCh* AbstractDOMParser::getExternalSchemaLocation() const
{
    return fScanner->getExternalSchemaLocation();
}

XMLCh* AbstractDOMParser::getExternalNoNamespaceSchemaLocation() const
{
    return fScanner->getExternalNoNamespaceSchemaLocation();
}

SecurityManager* AbstractDOMParser::getSecurityManager() const
{
    return fScanner->getSecurityManager();
}

bool AbstractDOMParser::getLoadExternalDTD() const
{
    return fScanner->getLoadExternalDTD();
}

bool AbstractDOMParser::getCalculateSrcOfs() const
{
    return fScanner->getCalculateSrcOfs();
}

bool AbstractDOMParser::getStandardUriConformant() const
{
    return fScanner->getStandardUriConformant();
}


// ---------------------------------------------------------------------------
//  AbstractDOMParser: Setter methods
// ---------------------------------------------------------------------------
void AbstractDOMParser::setPSVIHandler(PSVIHandler* const handler)
{
    fPSVIHandler = handler;
    if (fPSVIHandler) {
        fScanner->setPSVIHandler(fPSVIHandler);        
    }
    else {
        fScanner->setPSVIHandler(0);        
    }
}


void AbstractDOMParser::setDoNamespaces(const bool newState)
{
    fScanner->setDoNamespaces(newState);
}

void AbstractDOMParser::setExitOnFirstFatalError(const bool newState)
{
    fScanner->setExitOnFirstFatal(newState);
}

void AbstractDOMParser::setValidationConstraintFatal(const bool newState)
{
    fScanner->setValidationConstraintFatal(newState);
}

void AbstractDOMParser::setValidationScheme(const ValSchemes newScheme)
{
    if (newScheme == Val_Never)
        fScanner->setValidationScheme(XMLScanner::Val_Never);
    else if (newScheme == Val_Always)
        fScanner->setValidationScheme(XMLScanner::Val_Always);
    else
        fScanner->setValidationScheme(XMLScanner::Val_Auto);
}

void AbstractDOMParser::setDoSchema(const bool newState)
{
    fScanner->setDoSchema(newState);
}

void AbstractDOMParser::setValidationSchemaFullChecking(const bool schemaFullChecking)
{
    fScanner->setValidationSchemaFullChecking(schemaFullChecking);
}

void AbstractDOMParser::setExternalSchemaLocation(const XMLCh* const schemaLocation)
{
    fScanner->setExternalSchemaLocation(schemaLocation);
}
void AbstractDOMParser::setExternalNoNamespaceSchemaLocation(const XMLCh* const noNamespaceSchemaLocation)
{
    fScanner->setExternalNoNamespaceSchemaLocation(noNamespaceSchemaLocation);
}

void AbstractDOMParser::setExternalSchemaLocation(const char* const schemaLocation)
{
    fScanner->setExternalSchemaLocation(schemaLocation);
}
void AbstractDOMParser::setExternalNoNamespaceSchemaLocation(const char* const noNamespaceSchemaLocation)
{
    fScanner->setExternalNoNamespaceSchemaLocation(noNamespaceSchemaLocation);
}

void AbstractDOMParser::setSecurityManager(SecurityManager* const securityManager)
{
    // since this could impact various components, don't permit it to change
    // during a parse
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    fScanner->setSecurityManager(securityManager);
}

void AbstractDOMParser::setLoadExternalDTD(const bool newState)
{
    fScanner->setLoadExternalDTD(newState);
}

void AbstractDOMParser::setCalculateSrcOfs(const bool newState)
{
    fScanner->setCalculateSrcOfs(newState);
}

void AbstractDOMParser::setStandardUriConformant(const bool newState)
{
    fScanner->setStandardUriConformant(newState);
}

void AbstractDOMParser::useScanner(const XMLCh* const scannerName)
{
    XMLScanner* tempScanner = XMLScannerResolver::resolveScanner
    (
        scannerName
        , fValidator
        , fGrammarResolver
        , fMemoryManager
    );

    if (tempScanner) {

        tempScanner->setParseSettings(fScanner);
        tempScanner->setURIStringPool(fURIStringPool);
        delete fScanner;
        fScanner = tempScanner;
    }
}

// ---------------------------------------------------------------------------
//  AbstractDOMParser: Parsing methods
// ---------------------------------------------------------------------------
void AbstractDOMParser::parse(const InputSource& source)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(source);
        fParseInProgress = false;
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }    
    catch(...)
    {
        fParseInProgress = false;
        throw;
    }
}

void AbstractDOMParser::parse(const XMLCh* const systemId)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(systemId);
        fParseInProgress = false;
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        fParseInProgress = false;
        throw;
    }
}

void AbstractDOMParser::parse(const char* const systemId)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(systemId);
        fParseInProgress = false;
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        fParseInProgress = false;
        throw;
    }
}



// ---------------------------------------------------------------------------
//  AbstractDOMParser: Progressive parse methods
// ---------------------------------------------------------------------------
bool AbstractDOMParser::parseFirst( const XMLCh* const    systemId
                                   ,       XMLPScanToken&  toFill)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    return fScanner->scanFirst(systemId, toFill);
}

bool AbstractDOMParser::parseFirst( const char* const         systemId
                                   ,       XMLPScanToken&      toFill)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    return fScanner->scanFirst(systemId, toFill);
}

bool AbstractDOMParser::parseFirst( const InputSource& source
                                   ,       XMLPScanToken&  toFill)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    return fScanner->scanFirst(source, toFill);
}

bool AbstractDOMParser::parseNext(XMLPScanToken& token)
{
    return fScanner->scanNext(token);
}

void AbstractDOMParser::parseReset(XMLPScanToken& token)
{
    // Reset the scanner, and then reset the parser
    fScanner->scanReset(token);
    reset();
}



// ---------------------------------------------------------------------------
//  AbstractDOMParser: Implementation of XMLDocumentHandler interface
// ---------------------------------------------------------------------------
void AbstractDOMParser::docCharacters(  const   XMLCh* const    chars
                              , const unsigned int    length
                              , const bool cdataSection)
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


void AbstractDOMParser::docComment(const XMLCh* const comment)
{
    if (fCreateCommentNodes) {
        DOMComment *dcom = fDocument->createComment(comment);
        fCurrentParent->appendChild(dcom);
        fCurrentNode = dcom;
    }
}


void AbstractDOMParser::docPI(  const   XMLCh* const    target
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


void AbstractDOMParser::endEntityReference(const XMLEntityDecl&)
{
    DOMEntityReferenceImpl *erImpl = 0;
    DOMNode* firstChild = 0;

    if (fCurrentParent->getNodeType() == DOMNode::ENTITY_REFERENCE_NODE) {
        erImpl = (DOMEntityReferenceImpl *) fCurrentParent;
        firstChild = erImpl->getFirstChild();
    }

    fCurrentParent = fNodeStack->pop();

    if (!fCreateEntityReferenceNodes && erImpl && firstChild) {
        DOMNode *kid, *next;
        fCurrentNode   = fCurrentParent->getLastChild();

        for (kid = firstChild; kid != 0; kid = next)
        {
            // If kid and fCurrentNode are both Text nodes (but _not_ CDATASection,
            // which is a subclass of Text), they can be merged.
            if (kid->getNodeType() == DOMNode::TEXT_NODE   &&
                fCurrentNode &&
                fCurrentNode->getNodeType() == DOMNode::TEXT_NODE )
            {
                ((DOMTextImpl *) fCurrentNode)->appendData(((DOMTextImpl *) kid)->getData());
            }
            else {
                // append the child of erImpl to currentParent
                fCurrentNode = kid->cloneNode(true);
                fCurrentParent->appendChild(fCurrentNode);

                if (erImpl->getBaseURI()) {
                    /**
                     * Record baseURI information for the Element (by adding xml:base attribute)
                     * or for the ProcessingInstruction (by setting a baseURI field)
                     */
                    if (fCurrentNode->getNodeType() == DOMNode::ELEMENT_NODE) {
                        // if an element already has xml:base attribute
                        // do nothing
                        const XMLCh baseString[] =
                        {
                            chLatin_b, chLatin_a, chLatin_s, chLatin_e, chNull
                        };
                        const XMLCh xmlBaseString[] =
                        {
                            chLatin_x, chLatin_m, chLatin_l, chColon, chLatin_b, chLatin_a, chLatin_s, chLatin_e, chNull
                        };

                        if (fScanner -> getDoNamespaces() && (((DOMElement*)fCurrentNode)->getAttributeNodeNS(DOMNodeImpl::getXmlURIString(), baseString) != 0)) {
                            return;
                        } else if (((DOMElement*)fCurrentNode)->getAttributeNode(xmlBaseString) != 0) {
                            return;
                        }

                        // retrive the baseURI from the entity decl
                        const XMLCh* baseURI = erImpl->getBaseURI();
                        if (baseURI != 0 && !XMLString::equals(baseURI,fDocument->getDocumentURI())) {
                            if (fScanner -> getDoNamespaces()) {
                                ((DOMElement*)fCurrentNode)->setAttributeNS(DOMNodeImpl::getXmlURIString(), baseString, baseURI);
                            } else {
                                ((DOMElement*)fCurrentNode)->setAttribute(xmlBaseString, baseURI);
                            }
                        }
                    }
                    else if (fCurrentNode->getNodeType() == DOMNode::PROCESSING_INSTRUCTION_NODE) {
                        ((DOMProcessingInstructionImpl*)fCurrentNode)->setBaseURI(erImpl->getBaseURI());
                    }
                }
            }

            next = kid->getNextSibling();
        }
    }
    else
        fCurrentNode   = fCurrentParent;

    if (erImpl)
        erImpl->setReadOnly(true, true);
}


void AbstractDOMParser::endElement( const   XMLElementDecl&
                           , const unsigned int
                           , const bool
                           , const XMLCh* const)
{
    fCurrentNode   = fCurrentParent;
    fCurrentParent = fNodeStack->pop();

    // If we've hit the end of content, clear the flag
    if (fNodeStack->empty())
        fWithinElement = false;
}

void AbstractDOMParser::elementTypeInfo( const   XMLCh * const  typeName
                           , const XMLCh *const                 typeURI)
{
    // very tightly coupled to endElement().  The state of the
    // object must not have changed between these calls!

    //validation is performed after the startElement event so we have to associate the info here
    ((DOMElementImpl *)(fCurrentNode))->setTypeInfo(typeName, typeURI);

}


void AbstractDOMParser::ignorableWhitespace(const   XMLCh* const    chars
                                    , const unsigned int    length
                                    , const bool)
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


void AbstractDOMParser::resetDocument()
{
    //
    //  The reset methods are called before a new parse event occurs.
    //  Reset this parsers state to clear out anything that may be left
    //  from a previous use, in particular the DOM document itself.
    //
    this->reset();
}


void AbstractDOMParser::startDocument()
{
    if(fImplementationFeatures == 0)
        fDocument = (DOMDocumentImpl *)DOMImplementation::getImplementation()->createDocument(fMemoryManager);
    else 
        fDocument = (DOMDocumentImpl *)DOMImplementationRegistry::getDOMImplementation(fImplementationFeatures)->createDocument(fMemoryManager);

    // Just set the document as the current parent and current node
    fCurrentParent = fDocument;
    fCurrentNode   = fDocument;
    // set DOM error checking off
    fDocument->setErrorChecking(false);
    fDocument->setDocumentURI(fScanner->getLocator()->getSystemId());
    fDocument->setActualEncoding(fScanner->getReaderMgr()->getCurrentEncodingStr());
}


void AbstractDOMParser::endDocument()
{
    // set DOM error checking back on
    fDocument->setErrorChecking(true);

    // DOM L2 does not support editing DocumentType nodes
    if (fDocumentType && fScanner -> getDoNamespaces())
        fDocumentType->setReadOnly(true, true);
}


void AbstractDOMParser::startElement(const  XMLElementDecl&         elemDecl
                             , const unsigned int            urlId
                             , const XMLCh* const            elemPrefix
                             , const RefVectorOf<XMLAttr>&   attrList
                             , const unsigned int            attrCount
                             , const bool                    isEmpty
                             , const bool                    isRoot)
{
    DOMElement     *elem;
    DOMElementImpl *elemImpl;

    //get the list for use in the loop
    XMLAttDefList* defAttrs = 0;
    if(elemDecl.hasAttDefs()) {
        defAttrs = &elemDecl.getAttDefList();
    }

    if (fScanner -> getDoNamespaces()) {    //DOM Level 2, doNamespaces on

        const XMLCh* namespaceURI = 0;

        if (urlId != fScanner->getEmptyNamespaceId()) {  //TagName has a prefix

            namespaceURI = fScanner->getURIText(urlId); //get namespaceURI

            if (elemPrefix && *elemPrefix) {

                XMLBufBid elemQName(&fBufMgr);

                elemQName.set(elemPrefix);
                elemQName.append(chColon);
                elemQName.append(elemDecl.getBaseName());
                elem = createElementNSNode(namespaceURI, elemQName.getRawBuffer());
            }
            else { 
                elem = createElementNSNode(namespaceURI, elemDecl.getBaseName());
            }
        } 
        else {
            elem = createElementNSNode(namespaceURI, elemDecl.getBaseName());
        }

        elemImpl = (DOMElementImpl *) elem;

        for (unsigned int index = 0; index < attrCount; ++index) {
            const XMLAttr* oneAttrib = attrList.elementAt(index);
            unsigned int attrURIId = oneAttrib -> getURIId();
            namespaceURI = 0;            
            if (XMLString::equals(oneAttrib -> getName(), XMLUni::fgXMLNSString)) {   //for xmlns=...
                attrURIId = fScanner->getXMLNSNamespaceId();                
            }
            if (attrURIId != fScanner->getEmptyNamespaceId()) {  //TagName has a prefix
                namespaceURI = fScanner->getURIText(attrURIId);   //get namespaceURI
            }
            //  revisit.  Optimize to init the named node map to the
            //            right size up front.
            DOMAttrImpl *attr = (DOMAttrImpl *)
                fDocument->createAttributeNS(namespaceURI, oneAttrib->getQName());
            attr->setValue(oneAttrib -> getValue());
            DOMNode* remAttr = elemImpl->setAttributeNodeNS(attr);
            if (remAttr)
                remAttr->release();

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

            attr->setTypeInfo(oneAttrib->getValidatingTypeName(), oneAttrib->getValidatingTypeURI());
        }
    }
    else {    //DOM Level 1
        elem = fDocument->createElement(elemDecl.getFullName());
        elemImpl = (DOMElementImpl *) elem;
			for (unsigned int index = 0; index < attrCount; ++index) {
				const XMLAttr* oneAttrib = attrList.elementAt(index);
            //AttrImpl *attr = elemImpl->setAttribute(oneAttrib->getName(), oneAttrib->getValue());
            DOMAttrImpl *attr = (DOMAttrImpl *)
                fDocument->createAttribute(oneAttrib->getName());
            attr->setValue(oneAttrib -> getValue());
            DOMNode* rem = elemImpl->setAttributeNode(attr);
            if (rem)
                rem->release();
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

            attr->setTypeInfo(oneAttrib->getValidatingTypeName(), oneAttrib->getValidatingTypeURI());

        }
    }

    // set up the default attributes
	if (defAttrs != 0)
	{		
        XMLAttDef* attr = 0;

        DOMAttrImpl * insertAttr = 0;

        for(unsigned int i=0; i<defAttrs->getAttDefCount(); i++)
        {
            attr = &defAttrs->getAttDef(i);

            const XMLAttDef::DefAttTypes defType = attr->getDefaultType();
            if ((defType == XMLAttDef::Default)
            ||  (defType == XMLAttDef::Fixed))
            {

                if (fScanner->getDoNamespaces())
                {
                    // DOM Level 2 wants all namespace declaration attributes
                    // to be bound to "http://www.w3.org/2000/xmlns/"
                    // So as long as the XML parser doesn't do it, it needs to
                    // done here.
                    const XMLCh* qualifiedName = attr->getFullName();
                    XMLBufBid bbPrefixQName(&fBufMgr);
                    XMLBuffer& prefixBuf = bbPrefixQName.getBuffer();
                    int colonPos = -1;
                    unsigned int uriId = fScanner->resolveQName(qualifiedName, prefixBuf, ElemStack::Mode_Attribute, colonPos);

                    const XMLCh* namespaceURI = 0;
                    if (XMLString::equals(qualifiedName, XMLUni::fgXMLNSString))    //for xmlns=...
                        uriId = fScanner->getXMLNSNamespaceId();
                    if (uriId != fScanner->getEmptyNamespaceId()) {  //TagName has a prefix
                        namespaceURI = fScanner->getURIText(uriId);
                    }

                    insertAttr = (DOMAttrImpl *) fDocument->createAttributeNS(
                                                                              namespaceURI,     // NameSpaceURI
                                                                              qualifiedName);   // qualified name

                    DOMAttr* remAttr = elemImpl->setDefaultAttributeNodeNS(insertAttr);
                    if (remAttr)
                        remAttr->release();

                    if (attr->getValue() != 0)
                    {
                        insertAttr->setValue(attr->getValue());
                        insertAttr->setSpecified(false);
                    }

                }
                else
                {
                    // Namespaces is turned off...
                    insertAttr = (DOMAttrImpl *) fDocument->createAttribute(attr->getFullName());

                    DOMNode* remAttr = elemImpl->setDefaultAttributeNode(insertAttr);
                    if (remAttr)
                        remAttr->release();

                    //need to do this before the get as otherwise we overwrite any value in the attr
                    if (attr->getValue() != 0)
                    {
                        insertAttr->setValue(attr->getValue());
                        insertAttr->setSpecified(false);
                    }
                }

                // REVISIT:  this won't work in multithreaded code...
                insertAttr->setTypeInfo(attr->getDOMTypeInfoName(), attr->getDOMTypeInfoUri());

            }

            insertAttr = 0;
            attr->reset();
        }
    }


    fCurrentParent->appendChild(elem);

    fNodeStack->push(fCurrentParent);
    fCurrentParent = elem;
    fCurrentNode = elem;
    fWithinElement = true;

    // If an empty element, do end right now (no endElement() will be called)
    if (isEmpty)
        endElement(elemDecl, urlId, isRoot, elemPrefix);
}


void AbstractDOMParser::startEntityReference(const XMLEntityDecl& entDecl)
{
    const XMLCh * entName = entDecl.getName();
    DOMNamedNodeMap *entities = fDocumentType->getEntities();
    DOMEntityImpl* entity = (DOMEntityImpl*)entities->getNamedItem(entName);
    if (entity)
        entity->setActualEncoding(fScanner->getReaderMgr()->getCurrentEncodingStr());
    fCurrentEntity = entity;

    DOMEntityReference *er = fDocument->createEntityReferenceByParser(entName);

    //set the readOnly flag to false before appending node, will be reset in endEntityReference
    DOMEntityReferenceImpl *erImpl = (DOMEntityReferenceImpl *) er;
    erImpl->setReadOnly(false, true);

    if (fCreateEntityReferenceNodes == true)
    {
        fCurrentParent->appendChild(er);
    }

    fNodeStack->push(fCurrentParent);
    fCurrentParent = er;
    fCurrentNode = er;

    // this entityRef needs to be stored in Entity map too.
    // We'd decide later whether the entity nodes should be created by a
    // separated method in parser or not. For now just stick it in if
    // the ref nodes are created
    if (entity)
        entity->setEntityRef(er);
}


void AbstractDOMParser::XMLDecl(const   XMLCh* const version
                                , const XMLCh* const encoding
                                , const XMLCh* const standalone
                                , const XMLCh* const actualEncStr)
{
    fDocument->setStandalone(XMLString::equals(XMLUni::fgYesString, standalone));

    fDocument->setVersion(version);
    fDocument->setEncoding(encoding);
    fDocument->setActualEncoding(actualEncStr);
}

// ---------------------------------------------------------------------------
//  AbstractDOMParser: Helper methods
// ---------------------------------------------------------------------------
DOMElement* AbstractDOMParser::createElementNSNode(const XMLCh *namespaceURI,
                                                   const XMLCh *qualifiedName)
{
    return fDocument->createElementNS(namespaceURI, qualifiedName);
}
// ---------------------------------------------------------------------------
//  AbstractDOMParser: Deprecated methods
// ---------------------------------------------------------------------------
bool AbstractDOMParser::getDoValidation() const
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

void AbstractDOMParser::setDoValidation(const bool newState)
{
    fScanner->setDoValidation
    (
        newState ? XMLScanner::Val_Always : XMLScanner::Val_Never
    );
}

//doctypehandler interfaces
void AbstractDOMParser::attDef
(
    const   DTDElementDecl&     elemDecl
    , const DTDAttDef&          attDef
    , const bool
)
{	
    if (fDocumentType->isIntSubsetReading())
    {
        if (elemDecl.hasAttDefs())
        {
            fInternalSubset.append(attDef.getFullName());

            // Get the type and display it
            const XMLAttDef::AttTypes type = attDef.getType();
            switch(type)
            {
            case XMLAttDef::CData :
                fInternalSubset.append(chSpace);
                fInternalSubset.append(XMLUni::fgCDATAString);
                break;
            case XMLAttDef::ID :
                fInternalSubset.append(chSpace);
                fInternalSubset.append(XMLUni::fgIDString);
                break;
            case XMLAttDef::IDRef :
                fInternalSubset.append(chSpace);
                fInternalSubset.append(XMLUni::fgIDRefString);
                break;
            case XMLAttDef::IDRefs :
                fInternalSubset.append(chSpace);
                fInternalSubset.append(XMLUni::fgIDRefsString);
                break;
            case XMLAttDef::Entity :
                fInternalSubset.append(chSpace);
                fInternalSubset.append(XMLUni::fgEntityString);
                break;
            case XMLAttDef::Entities :
                fInternalSubset.append(chSpace);
                fInternalSubset.append(XMLUni::fgEntitiesString);
                break;
            case XMLAttDef::NmToken :
                fInternalSubset.append(chSpace);
                fInternalSubset.append(XMLUni::fgNmTokenString);
                break;
            case XMLAttDef::NmTokens :
                fInternalSubset.append(chSpace);
                fInternalSubset.append(XMLUni::fgNmTokensString);
                break;

            case XMLAttDef::Notation :
                fInternalSubset.append(chSpace);
                fInternalSubset.append(XMLUni::fgNotationString);
                break;

            case XMLAttDef::Enumeration :
                fInternalSubset.append(chSpace);
                const XMLCh* enumString = attDef.getEnumeration();
                int length = XMLString::stringLen(enumString);
                if (length > 0) {

                    fInternalSubset.append(chOpenParen );
                    for(int i=0; i<length; i++) {
                        if (enumString[i] == chSpace)
                            fInternalSubset.append(chPipe);
                        else
                            fInternalSubset.append(enumString[i]);
                    }
                    fInternalSubset.append(chCloseParen);
                }
                break;
            }
            //get te default types of the attlist
            const XMLAttDef::DefAttTypes def = attDef.getDefaultType();
            switch(def)
            {
            case XMLAttDef::Required :
                fInternalSubset.append(chSpace);
                fInternalSubset.append(XMLUni::fgRequiredString);
                break;
            case XMLAttDef::Implied :
                fInternalSubset.append(chSpace);
                fInternalSubset.append(XMLUni::fgImpliedString);
                break;
            case XMLAttDef::Fixed :
                fInternalSubset.append(chSpace);
                fInternalSubset.append(XMLUni::fgFixedString);
                break;
            }

            const XMLCh* defaultValue = attDef.getValue();
            if (defaultValue != 0) {
                fInternalSubset.append(chSpace);
                fInternalSubset.append(chDoubleQuote);
                fInternalSubset.append(defaultValue);
                fInternalSubset.append(chDoubleQuote);
            }
        }
    }
}

void AbstractDOMParser::doctypeComment
(
    const   XMLCh* const    comment
)
{
    if (fDocumentType->isIntSubsetReading())
    {
        if (comment != 0)
        {
            fInternalSubset.append(XMLUni::fgCommentString);
            fInternalSubset.append(chSpace);
            fInternalSubset.append(comment);
            fInternalSubset.append(chSpace);
            fInternalSubset.append(chDash);
            fInternalSubset.append(chDash);
            fInternalSubset.append(chCloseAngle);
        }
    }
}

void AbstractDOMParser::doctypeDecl
(
    const   DTDElementDecl& elemDecl
    , const XMLCh* const    publicId
    , const XMLCh* const    systemId
    , const bool
    , const bool
)
{
    fDocumentType = (DOMDocumentTypeImpl *) fDocument->createDocumentType(elemDecl.getFullName(), publicId, systemId);
    fDocument->setDocumentType(fDocumentType);

}

void AbstractDOMParser::doctypePI
(
    const   XMLCh* const    target
    , const XMLCh* const    data
)
{
    if (fDocumentType->isIntSubsetReading())
    {
        //add these chars to internalSubset variable
        fInternalSubset.append(chOpenAngle);
        fInternalSubset.append(chQuestion);
        fInternalSubset.append(target);
        fInternalSubset.append(chSpace);
        fInternalSubset.append(data);
        fInternalSubset.append(chQuestion);
        fInternalSubset.append(chCloseAngle);
    }	
}


void AbstractDOMParser::doctypeWhitespace
(
    const   XMLCh* const    chars
    , const unsigned int
)
{
    if (fDocumentType->isIntSubsetReading())
        fInternalSubset.append(chars);
}

void AbstractDOMParser::elementDecl
(
    const   DTDElementDecl& decl
    , const bool
)
{
    if (fDocumentType->isIntSubsetReading())
    {
        fInternalSubset.append(chOpenAngle);
        fInternalSubset.append(chBang);
        fInternalSubset.append(XMLUni::fgElemString);
        fInternalSubset.append(chSpace);
        fInternalSubset.append(decl.getFullName());

        //get the ContentSpec information
        const XMLCh* contentModel = decl.getFormattedContentModel();
        if (contentModel != 0) {
            fInternalSubset.append(chSpace);
            fInternalSubset.append(contentModel);
        }

        fInternalSubset.append(chCloseAngle);
    }
}

void AbstractDOMParser::endAttList
(
    const   DTDElementDecl& elemDecl
)
{
    if (fDocumentType->isIntSubsetReading())
    {
        //print the closing angle
        fInternalSubset.append(chCloseAngle);
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

        DOMAttrImpl * insertAttr = 0;
        DOMElement     *elem  = fDocument->createElement(elemDecl.getFullName());
        DOMElementImpl *elemImpl = (DOMElementImpl *) elem;

        for(unsigned int i=0; i<defAttrs->getAttDefCount(); i++)
        {
            attr = &defAttrs->getAttDef(i);
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

                    XMLBufBid bbQName(&fBufMgr);
                    XMLBuffer& buf = bbQName.getBuffer();
                    static const XMLCh XMLNS[] = {
                        chLatin_x, chLatin_m, chLatin_l, chLatin_n, chLatin_s, chNull};

                    if (index > 0) {
                        // there is prefix
                        // map to XML URI for all cases except when prefix == "xmlns"
                        XMLCh* prefix;
                        XMLCh temp[1000];

                        if (index > 999)
                            prefix = (XMLCh*) fMemoryManager->allocate
                            (
                                (index + 1) * sizeof(XMLCh)
                            );//new XMLCh[index+1];
                        else
                            prefix = temp;

                        XMLString::subString(prefix ,qualifiedName, 0, index, fMemoryManager);

                        if (XMLString::equals(prefix,XMLNS))
                            buf.append(XMLUni::fgXMLNSURIName);
                        else
                            buf.append(XMLUni::fgXMLURIName);

                        if (index > 999)
                            fMemoryManager->deallocate(prefix);//delete [] prefix;
                    }
                    else {
                        //   No prefix
                        if (XMLString::equals(qualifiedName,XMLNS))
                            buf.append(XMLUni::fgXMLNSURIName);
                    }

                    insertAttr = (DOMAttrImpl *) fDocument->createAttributeNS(
                       buf.getRawBuffer(),     // NameSpaceURI
                       qualifiedName);   // qualified name

                    DOMNode* remAttr = elemImpl->setAttributeNodeNS(insertAttr);
                    if (remAttr)
                        remAttr->release();
                }
                else
                {
                    // Namespaces is turned off...
                    insertAttr = (DOMAttrImpl *) fDocument->createAttribute(attr->getFullName());
                    DOMNode* remAttr = elemImpl->setAttributeNode(insertAttr);
                    if (remAttr)
                        remAttr->release();
                }

                insertAttr->setValue(attr->getValue());
                insertAttr->setSpecified(false);
            }
        }
        DOMNode* rem = fDocumentType->getElements()->setNamedItem(elemImpl);
        if (rem)
            rem->release();
    }
}

void AbstractDOMParser::endIntSubset()
{
    fDocumentType->setInternalSubset(fInternalSubset.getRawBuffer());
    // the buffer shouldn't be released as it is reused in the next parse
    // fBufMgr.releaseBuffer(fInternalSubset);
    fDocumentType->fIntSubsetReading = false;
}

void AbstractDOMParser::endExtSubset()
{
}

void AbstractDOMParser::entityDecl
(
    const   DTDEntityDecl&  entityDecl
    , const bool
    , const bool
)
{
    DOMEntityImpl* entity = (DOMEntityImpl *) fDocument->createEntity(entityDecl.getName());

    entity->setPublicId(entityDecl.getPublicId());
    entity->setSystemId(entityDecl.getSystemId());
    entity->setNotationName(entityDecl.getNotationName());
    entity->setBaseURI(entityDecl.getBaseURI());

    DOMEntityImpl *previousDef = (DOMEntityImpl *)
	    fDocumentType->getEntities()->setNamedItem( entity );

    if (previousDef)
        previousDef->release();

    if (fDocumentType->isIntSubsetReading())
    {
        //add thes chars to internalSubset variable
        fInternalSubset.append(chOpenAngle);
        fInternalSubset.append(chBang);
        fInternalSubset.append(XMLUni::fgEntityString);
        fInternalSubset.append(chSpace);

        fInternalSubset.append(entityDecl.getName());

        const XMLCh* id = entity->getPublicId();
        if (id != 0) {
            fInternalSubset.append(chSpace);
            fInternalSubset.append(XMLUni::fgPubIDString);
            fInternalSubset.append(chSpace);
            fInternalSubset.append(chDoubleQuote);
            fInternalSubset.append(id);
            fInternalSubset.append(chDoubleQuote);
        }
        id = entity->getSystemId();
        if (id != 0) {
            fInternalSubset.append(chSpace);
            fInternalSubset.append(XMLUni::fgSysIDString);
            fInternalSubset.append(chSpace);
            fInternalSubset.append(chDoubleQuote);
            fInternalSubset.append(id);
            fInternalSubset.append(chDoubleQuote);

        }
        id = entity->getNotationName();
        if (id != 0) {
            fInternalSubset.append(chSpace);
            fInternalSubset.append(XMLUni::fgNDATAString);
            fInternalSubset.append(chSpace);
            fInternalSubset.append(id);
        }
        id = entityDecl.getValue();
        if (id !=0) {
            fInternalSubset.append(chSpace);
            fInternalSubset.append(chDoubleQuote);
            fInternalSubset.append(id);
            fInternalSubset.append(chDoubleQuote);
        }

        fInternalSubset.append(chCloseAngle);
    }

}

void AbstractDOMParser::resetDocType()
{
	fDocumentType = 0;
}

void AbstractDOMParser::notationDecl
(
    const   XMLNotationDecl&    notDecl
    , const bool
)
{
    DOMNotationImpl* notation = (DOMNotationImpl *)fDocument->createNotation(notDecl.getName());
    notation->setPublicId(notDecl.getPublicId());
    notation->setSystemId(notDecl.getSystemId());
    notation->setBaseURI(notDecl.getBaseURI());

    DOMNode* rem = fDocumentType->getNotations()->setNamedItem( notation );
    if (rem)
        rem->release();

    if (fDocumentType->isIntSubsetReading())
    {
        //add thes chars to internalSubset variable
        fInternalSubset.append(chOpenAngle);
        fInternalSubset.append(chBang);
        fInternalSubset.append(XMLUni::fgNotationString);
        fInternalSubset.append(chSpace);

        fInternalSubset.append(notDecl.getName());

        const XMLCh* id = notation->getPublicId();
        if (id != 0) {
            fInternalSubset.append(chSpace);
            fInternalSubset.append(XMLUni::fgPubIDString);
            fInternalSubset.append(chSpace);
            fInternalSubset.append(chDoubleQuote);
            fInternalSubset.append(id);
            fInternalSubset.append(chDoubleQuote);
        }
        id = notation->getSystemId();
        if (id != 0) {
            fInternalSubset.append(chSpace);
            fInternalSubset.append(XMLUni::fgSysIDString);
            fInternalSubset.append(chSpace);
            fInternalSubset.append(chDoubleQuote);
            fInternalSubset.append(id);
            fInternalSubset.append(chDoubleQuote);

        }
        fInternalSubset.append(chCloseAngle);
    }
}

void AbstractDOMParser::startAttList
(
    const   DTDElementDecl& elemDecl
)
{
    if (fDocumentType->isIntSubsetReading())
    {
        fInternalSubset.append(chOpenAngle);
        fInternalSubset.append(chBang);
        fInternalSubset.append(XMLUni::fgAttListString);
        fInternalSubset.append(chSpace);
        fInternalSubset.append(elemDecl.getFullName());
    }
}

void AbstractDOMParser::startIntSubset()
{
	fDocumentType->fIntSubsetReading = true;
}

void AbstractDOMParser::startExtSubset()
{
}

void AbstractDOMParser::TextDecl
(
    const   XMLCh* const    versionStr
    , const XMLCh* const    encodingStr
)
{
    if (fCurrentEntity) {
        fCurrentEntity->setVersion(versionStr);
        fCurrentEntity->setEncoding(encodingStr);
    }
}

XERCES_CPP_NAMESPACE_END

