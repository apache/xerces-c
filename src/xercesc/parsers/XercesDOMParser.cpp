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
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/util/IOException.hpp>
#include <xercesc/internal/XMLScanner.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/validators/common/GrammarResolver.hpp>
#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/framework/XMLSchemaDescription.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/XMLEntityResolver.hpp>

XERCES_CPP_NAMESPACE_BEGIN



// ---------------------------------------------------------------------------
//  XercesDOMParser: Constructors and Destructor
// ---------------------------------------------------------------------------
XercesDOMParser::XercesDOMParser( XMLValidator* const   valToAdopt
                                , MemoryManager* const  manager
                                , XMLGrammarPool* const gramPool):

AbstractDOMParser(valToAdopt, manager, gramPool)
, fEntityResolver(0)
, fXMLEntityResolver(0)
, fErrorHandler(0)
{
}


XercesDOMParser::~XercesDOMParser()
{
}


// ---------------------------------------------------------------------------
//  XercesDOMParser: Getter methods
// ---------------------------------------------------------------------------
bool XercesDOMParser::isCachingGrammarFromParse() const
{
    return getScanner()->isCachingGrammarFromParse();
}

bool XercesDOMParser::isUsingCachedGrammarInParse() const
{
    return getScanner()->isUsingCachedGrammarInParse();
}

Grammar* XercesDOMParser::getGrammar(const XMLCh* const nameSpaceKey)
{
    return getGrammarResolver()->getGrammar(nameSpaceKey);
}

Grammar* XercesDOMParser::getRootGrammar()
{
    return getScanner()->getRootGrammar();
}

const XMLCh* XercesDOMParser::getURIText(unsigned int uriId) const
{
    return getScanner()->getURIText(uriId);
}

unsigned int XercesDOMParser::getSrcOffset() const
{
    return getScanner()->getSrcOffset();
}

// ---------------------------------------------------------------------------
//  XercesDOMParser: Setter methods
// ---------------------------------------------------------------------------
void XercesDOMParser::setErrorHandler(ErrorHandler* const handler)
{
    fErrorHandler = handler;
    XMLScanner* scanner = getScanner();
    if (fErrorHandler) {
        scanner->setErrorReporter(this);
        scanner->setErrorHandler(fErrorHandler);
    }
    else {
        scanner->setErrorReporter(0);
        scanner->setErrorHandler(0);
    }
}

void XercesDOMParser::setEntityResolver(EntityResolver* const handler)
{
    fEntityResolver = handler;
    if (fEntityResolver) {
        getScanner()->setEntityHandler(this);
        fXMLEntityResolver = 0;
    }
    else {
        getScanner()->setEntityHandler(0);
    }
}

void XercesDOMParser::setXMLEntityResolver(XMLEntityResolver* const handler)
{
    fXMLEntityResolver = handler;
    if (fXMLEntityResolver) {
        getScanner()->setEntityHandler(this);
        fEntityResolver = 0;
    }
    else {
        getScanner()->setEntityHandler(0);
    }
}

void XercesDOMParser::cacheGrammarFromParse(const bool newState)
{
    getScanner()->cacheGrammarFromParse(newState);

    if (newState)
        getScanner()->useCachedGrammarInParse(newState);
}

void XercesDOMParser::useCachedGrammarInParse(const bool newState)
{
    if (newState || !getScanner()->isCachingGrammarFromParse())
        getScanner()->useCachedGrammarInParse(newState);
}


// ---------------------------------------------------------------------------
//  XercesDOMParser: Utilities
// ---------------------------------------------------------------------------
void XercesDOMParser::resetDocumentPool()
{
    resetPool();
}


// ---------------------------------------------------------------------------
//  XercesDOMParser: Implementation of the XMLErrorReporter interface
// ---------------------------------------------------------------------------
void XercesDOMParser::error( const   unsigned int
                             , const XMLCh* const
                             , const XMLErrorReporter::ErrTypes  errType
                             , const XMLCh* const                errorText
                             , const XMLCh* const                systemId
                             , const XMLCh* const                publicId
                             , const XMLSSize_t                  lineNum
                             , const XMLSSize_t                  colNum)
{
    SAXParseException toThrow = SAXParseException
        (
        errorText
        , publicId
        , systemId
        , lineNum
        , colNum
        , getMemoryManager()
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
XercesDOMParser::resolveEntity(const XMLCh* const publicId,
                               const XMLCh* const systemId,
                               const XMLCh* const)
{
    //
    //  Just map it to the SAX entity resolver. If there is not one installed,
    //  return a null pointer to cause the default resolution.
    //
    if (fEntityResolver)
        return fEntityResolver->resolveEntity(publicId, systemId);
    return 0;
}

InputSource*
XercesDOMParser::resolveEntity(XMLResourceIdentifier* resourceIdentifier)
{
    //
    //  Just map it to the SAX entity resolver. If there is not one installed,
    //  return a null pointer to cause the default resolution.
    //
    if (fEntityResolver)
        return fEntityResolver->resolveEntity(resourceIdentifier->getPublicId(), 
                                                resourceIdentifier->getSystemId());
    if (fXMLEntityResolver)
        return fXMLEntityResolver->resolveEntity(resourceIdentifier);

    return 0;
}

// ---------------------------------------------------------------------------
//  XercesDOMParser: Grammar preparsing methods
// ---------------------------------------------------------------------------
Grammar* XercesDOMParser::loadGrammar(const char* const systemId,
                                      const short grammarType,
                                      const bool toCache)
{
    // Avoid multiple entrance
    if (getParseInProgress())
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    Grammar* grammar = 0;
    try
    {
        setParseInProgress(true);
        if (grammarType == Grammar::DTDGrammarType) 
            getScanner()->setDocTypeHandler(0);
        grammar = getScanner()->loadGrammar(systemId, grammarType, toCache);
        if (grammarType == Grammar::DTDGrammarType) 
            getScanner()->setDocTypeHandler(this);
        setParseInProgress(false);
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        if (grammarType == Grammar::DTDGrammarType) 
            getScanner()->setDocTypeHandler(this);        
        setParseInProgress(false);
        throw;
    }

    return grammar;
}

Grammar* XercesDOMParser::loadGrammar(const XMLCh* const systemId,
                                      const short grammarType,
                                      const bool toCache)
{
    // Avoid multiple entrance
    if (getParseInProgress())
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    Grammar* grammar = 0;
    try
    {
        setParseInProgress(true);
        if (grammarType == Grammar::DTDGrammarType) 
            getScanner()->setDocTypeHandler(0);
        grammar = getScanner()->loadGrammar(systemId, grammarType, toCache);
        if (grammarType == Grammar::DTDGrammarType) 
            getScanner()->setDocTypeHandler(this);
        setParseInProgress(false);
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        if (grammarType == Grammar::DTDGrammarType) 
            getScanner()->setDocTypeHandler(this);        
        setParseInProgress(false);
        throw;
    }

    return grammar;
}

Grammar* XercesDOMParser::loadGrammar(const InputSource& source,
                                      const short grammarType,
                                      const bool toCache)
{
    // Avoid multiple entrance
    if (getParseInProgress())
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

   Grammar* grammar = 0;
    try
    {
        setParseInProgress(true);
        if (grammarType == Grammar::DTDGrammarType) 
            getScanner()->setDocTypeHandler(0);
        grammar = getScanner()->loadGrammar(source, grammarType, toCache);
        if (grammarType == Grammar::DTDGrammarType) 
            getScanner()->setDocTypeHandler(this);
        setParseInProgress(false);
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        if (grammarType == Grammar::DTDGrammarType) 
            getScanner()->setDocTypeHandler(this);
        setParseInProgress(false);
        throw;
    }

    return grammar;
}

void XercesDOMParser::resetCachedGrammarPool()
{
    getGrammarResolver()->resetCachedGrammar();
}

XERCES_CPP_NAMESPACE_END

