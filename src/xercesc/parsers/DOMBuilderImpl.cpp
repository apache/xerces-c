/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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
#include <xercesc/parsers/DOMBuilderImpl.hpp>
#include <xercesc/util/IOException.hpp>
#include <xercesc/dom/DOMEntityResolver.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/impl/DOMErrorImpl.hpp>
#include <xercesc/dom/impl/DOMLocatorImpl.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/internal/XMLScanner.hpp>
#include <xercesc/util/DOMInputSourceWrapper.hpp>


// ---------------------------------------------------------------------------
//  DOMBuilderImpl: Constructors and Destructor
// ---------------------------------------------------------------------------
DOMBuilderImpl::DOMBuilderImpl(XMLValidator* const valToAdopt) :

AbstractDOMParser(valToAdopt)
, fAutoValidation(false)
, fValidation(false)
, fErrorHandler(0)
, fEntityResolver(0)
, fFilter(0)
{
}


DOMBuilderImpl::~DOMBuilderImpl()
{
}


// ---------------------------------------------------------------------------
//  DOMBuilderImpl: Setter methods
// ---------------------------------------------------------------------------
void DOMBuilderImpl::setErrorHandler(DOMErrorHandler* const handler)
{
    fErrorHandler = handler;
    if (fErrorHandler) {
        getScanner()->setErrorReporter(this);
    }
    else {
        getScanner()->setErrorReporter(0);
    }
}

void DOMBuilderImpl::setEntityResolver(DOMEntityResolver* const handler)
{
    fEntityResolver = handler;
    if (fEntityResolver) {
        getScanner()->setEntityHandler(this);
    }
    else {
        getScanner()->setEntityHandler(0);
    }
}

void DOMBuilderImpl::setFilter(DOMBuilderFilter* const filter)
{
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
}


// ---------------------------------------------------------------------------
//  DOMBuilderImpl: Feature methods
// ---------------------------------------------------------------------------
void DOMBuilderImpl::setFeature(const XMLCh* const name, const bool state)
{
	if (getParseInProgress())
		throw SAXNotSupportedException("Feature modification is not supported during parse.");

    if (XMLString::compareString(name, XMLUni::fgDOMEntities) == 0) {
        setCreateEntityReferenceNodes(state);
    }
    else if (XMLString::compareString(name, XMLUni::fgDOMNamespaces) == 0) {
        setDoNamespaces(state);
    }
    else if (XMLString::compareString(name, XMLUni::fgDOMWhitespaceInElementContent) == 0) {
        setIncludeIgnorableWhitespace(state);
    }
    else if (XMLString::compareString(name, XMLUni::fgDOMValidation) == 0) {

        fValidation = state;

        if (state) {
            if (getValidationScheme() == AbstractDOMParser::Val_Never)
                setValidationScheme(AbstractDOMParser::Val_Always);
        }
        else {
            setValidationScheme(AbstractDOMParser::Val_Never);
        }
    }
    else if (XMLString::compareString(name, XMLUni::fgDOMValidateIfSchema) == 0) {

        fAutoValidation = state;

        if (state) {
            setValidationScheme(AbstractDOMParser::Val_Auto);
        }
        else {
            setValidationScheme(AbstractDOMParser::Val_Never);
        }
    }
    else {
        throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
    }
}

bool DOMBuilderImpl::getFeature(const XMLCh* const name)
{
    if (XMLString::compareString(name, XMLUni::fgDOMEntities) == 0) {
        return getCreateEntityReferenceNodes();
    }
    else if (XMLString::compareString(name, XMLUni::fgDOMNamespaces) == 0) {
        return getDoNamespaces();
    }
    else if (XMLString::compareString(name, XMLUni::fgDOMWhitespaceInElementContent) == 0) {
        return getIncludeIgnorableWhitespace();
    }
    else if (XMLString::compareString(name, XMLUni::fgDOMValidation) == 0) {
        return fValidation;
    }
    else if (XMLString::compareString(name, XMLUni::fgDOMValidateIfSchema) == 0) {
        return fAutoValidation;
    }
    else {
        throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
    }

    return false;
}

bool DOMBuilderImpl::canSetFeature(const XMLCh* const name, const bool state)
{
    if ((XMLString::compareString(name, XMLUni::fgDOMEntities) == 0) ||
        (XMLString::compareString(name, XMLUni::fgDOMNamespaces) == 0) ||
        (XMLString::compareString(name, XMLUni::fgDOMValidation) == 0) ||
        (XMLString::compareString(name, XMLUni::fgDOMValidateIfSchema) == 0) ||
        (XMLString::compareString(name, XMLUni::fgDOMWhitespaceInElementContent) == 0)) {
        return true;
    }

    return false;
}

// ---------------------------------------------------------------------------
//  DOMBuilderImpl: Parsing methods
// ---------------------------------------------------------------------------
DOMDocument* DOMBuilderImpl::parse(const DOMInputSource& source,
                                   const bool reuseGrammar)
{
    DOMInputSourceWrapper isWrapper((DOMInputSource*) &source);

    isWrapper.setAdoptInputSource(false);
    AbstractDOMParser::parse(isWrapper, reuseGrammar);
    return getDocument();
}

DOMDocument* DOMBuilderImpl::parseURI(const XMLCh* const systemId,
                                      const bool reuseGrammar)
{
    AbstractDOMParser::parse(systemId, reuseGrammar);
    return getDocument();
}

DOMDocument* DOMBuilderImpl::parseURI(const char* const systemId,
                                      const bool reuseGrammar)
{
    AbstractDOMParser::parse(systemId, reuseGrammar);
    return getDocument();
}

void DOMBuilderImpl::parseWithContext(const DOMInputSource& source,
                                      DOMNode* const contextNode,
                                      const short action)
{
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
}


// ---------------------------------------------------------------------------
//  DOMBuilderImpl: Implementation of the XMLErrorReporter interface
// ---------------------------------------------------------------------------
void DOMBuilderImpl::error( const   unsigned int                code
                            , const XMLCh* const                msgDomain
                            , const XMLErrorReporter::ErrTypes  errType
                            , const XMLCh* const                errorText
                            , const XMLCh* const                systemId
                            , const XMLCh* const                publicId
                            , const XMLSSize_t                  lineNum
                            , const XMLSSize_t                  colNum)
{
    if (fErrorHandler) {

        short severity = DOMError::SEVERITY_ERROR;

        if (errType == XMLErrorReporter::ErrType_Warning)
            severity = DOMError::SEVERITY_WARNING;
        else if (errType == XMLErrorReporter::ErrType_Fatal)
            severity = DOMError::SEVERITY_FATAL_ERROR;

        DOMLocatorImpl location((int)lineNum, (int) colNum, getCurrentNode(), systemId);
        DOMErrorImpl domError(severity, errorText, &location);

        fErrorHandler->handleError(domError);
    }
}

void DOMBuilderImpl::resetErrors()
{
}


// ---------------------------------------------------------------------------
//  DOMBuilderImpl: Implementation of XMLEntityHandler interface
// ---------------------------------------------------------------------------
InputSource*
DOMBuilderImpl::resolveEntity(const XMLCh* const publicId,
                              const XMLCh* const systemId,
                              const XMLCh* const baseURI)
{
    //
    //  Just map it to the SAX entity resolver. If there is not one installed,
    //  return a null pointer to cause the default resolution.
    //
    if (fEntityResolver) {

        DOMInputSource* is = fEntityResolver->resolveEntity(publicId, systemId, baseURI);

        if (is)
            return new DOMInputSourceWrapper(is);
    }

    return 0;
}

