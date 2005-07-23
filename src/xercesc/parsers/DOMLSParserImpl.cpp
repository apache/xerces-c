/*
 * Copyright 2002, 2003,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
#include <xercesc/parsers/DOMLSParserImpl.hpp>
#include <xercesc/util/IOException.hpp>
#include <xercesc/dom/DOMEntityResolver.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/impl/DOMErrorImpl.hpp>
#include <xercesc/dom/impl/DOMLocatorImpl.hpp>
#include <xercesc/dom/impl/DOMConfigurationImpl.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/internal/XMLScanner.hpp>
#include <xercesc/framework/Wrapper4DOMLSInput.hpp>
#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/framework/XMLSchemaDescription.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/validators/common/GrammarResolver.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/XMLEntityResolver.hpp>

XERCES_CPP_NAMESPACE_BEGIN



// ---------------------------------------------------------------------------
//  DOMLSParserImpl: Constructors and Destructor
// ---------------------------------------------------------------------------
DOMLSParserImpl::DOMLSParserImpl( XMLValidator* const   valToAdopt
                              , MemoryManager* const  manager
                              , XMLGrammarPool* const gramPool) :

AbstractDOMParser(valToAdopt, manager, gramPool)
, fAutoValidation(false)
, fValidation(false)
, fEntityResolver(0)
, fXMLEntityResolver(0)
, fErrorHandler(0)
, fFilter(0)
, fCharsetOverridesXMLEncoding(true)
, fUserAdoptsDocument(false)
{
    // dom spec has different default from scanner's default, so set explicitly
    getScanner()->setNormalizeData(false);
}


DOMLSParserImpl::~DOMLSParserImpl()
{
}


// ---------------------------------------------------------------------------
//  DOMLSParserImpl: Setter methods
// ---------------------------------------------------------------------------
bool DOMLSParserImpl::getBusy() const
{
    return fParseInProgress;
}

// ---------------------------------------------------------------------------
//  DOMLSParserImpl: Setter methods
// ---------------------------------------------------------------------------
void DOMLSParserImpl::setFilter(DOMLSParserFilter* const)
{
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0, getMemoryManager());
}

// ---------------------------------------------------------------------------
//  DOMLSParserImpl: DOMConfiguration methods
// ---------------------------------------------------------------------------
void DOMLSParserImpl::setParameter(const XMLCh* name, const void* value)
{
    if (XMLString::compareIStringASCII(name, XMLUni::fgDOMResourceResolver) == 0) 
    {
        fEntityResolver = (DOMEntityResolver*)value;
        if (fEntityResolver) {
            getScanner()->setEntityHandler(this);
            fXMLEntityResolver = 0;
        }
        else {
            getScanner()->setEntityHandler(0);
        }
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMErrorHandler) == 0) 
    {
        fErrorHandler = (DOMErrorHandler*)value;
        if (fErrorHandler) {
            getScanner()->setErrorReporter(this);
        }
        else {
            getScanner()->setErrorReporter(0);
        }
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesEntityResolver) == 0) 
    {
        fXMLEntityResolver = (XMLEntityResolver*)value;
        if (fXMLEntityResolver) {
            getScanner()->setEntityHandler(this);
            fEntityResolver = 0;
        }
        else {
            getScanner()->setEntityHandler(0);
        }
    }
	else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaExternalSchemaLocation) == 0)
	{
		setExternalSchemaLocation((XMLCh*)value);
	}
	else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation) == 0)
	{
		setExternalNoNamespaceSchemaLocation((XMLCh*)value);
	}
	else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSecurityManager) == 0)
	{
		setSecurityManager((SecurityManager*)value);
	}
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesScannerName))
    {
        AbstractDOMParser::useScanner((const XMLCh*) value);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesParserUseDocumentFromImplementation))
    {
        useImplementation((const XMLCh*) value);
    }
    else
        throw DOMException(DOMException::NOT_FOUND_ERR, 0, getMemoryManager());
}

void DOMLSParserImpl::setParameter(const XMLCh* name, bool state)
{
    if (XMLString::compareIStringASCII(name, XMLUni::fgDOMCharsetOverridesXMLEncoding) == 0) 
    {
        // in fact, setting this has no effect to the parser
        fCharsetOverridesXMLEncoding = state;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMDisallowDoctype) == 0) 
    {
        // TODO
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMIgnoreUnknownCharacterDenormalization) == 0) 
    {
        // TODO
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMNamespaces) == 0) 
    {
        setDoNamespaces(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSupportedMediatypesOnly) == 0) 
    {
        if (state)
            throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0, getMemoryManager());
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMValidate) == 0) 
    {
        fValidation = state;

        if (fValidation) {
            if (getValidationScheme() == AbstractDOMParser::Val_Never)
                setValidationScheme(AbstractDOMParser::Val_Always);
        }
        else {
            setValidationScheme(AbstractDOMParser::Val_Never);
        }
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMValidateIfSchema) == 0) 
    {
        fAutoValidation = state;

        if (fAutoValidation) {
            setValidationScheme(AbstractDOMParser::Val_Auto);
        }
        else {
            setValidationScheme(AbstractDOMParser::Val_Never);
        }
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMWellFormed) == 0) 
    {
        if(state==false)
            throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0, getMemoryManager());
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMCanonicalForm) == 0 ) 
    {
        // TODO
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMCDATASections) == 0 ) 
    {
        // TODO
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMCheckCharacterNormalization) == 0 ) 
    {
        // TODO
    }    
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMComments) == 0) 
    {
        setCreateCommentNodes(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMDatatypeNormalization) == 0) 
    {
        getScanner()->setNormalizeData(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMElementContentWhitespace) == 0) 
    {
        setIncludeIgnorableWhitespace(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMEntities) == 0) 
    {
        setCreateEntityReferenceNodes(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMNamespaceDeclarations) == 0) 
    {
        if (state==false)
            throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0, getMemoryManager());
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMNormalizeCharacters) == 0) 
    {
        // TODO
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSchemaLocation) == 0) 
    {
        // TODO
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSchemaType) == 0) 
    {
        // TODO
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSplitCDATASections) == 0) 
    {
        // TODO
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMInfoset) == 0) 
    {
        if (!state)
            throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0, getMemoryManager());
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchema) == 0)
    {
        setDoSchema(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaFullChecking) == 0)
    {
        setValidationSchemaFullChecking(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesUserAdoptsDOMDocument) == 0)
    {
        if(state)
            fUserAdoptsDocument = true;
        else
            fUserAdoptsDocument = false;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesLoadExternalDTD) == 0)
    {
        setLoadExternalDTD(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesContinueAfterFatalError) == 0)
    {
        setExitOnFirstFatalError(!state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesValidationErrorAsFatal) == 0)
    {
        setValidationConstraintFatal(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesCacheGrammarFromParse) == 0)
    {
        getScanner()->cacheGrammarFromParse(state);

        if (state)
            getScanner()->useCachedGrammarInParse(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesUseCachedGrammarInParse) == 0)
    {
        if (state || !getScanner()->isCachingGrammarFromParse())
            getScanner()->useCachedGrammarInParse(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesCalculateSrcOfs) == 0)
    {
        getScanner()->setCalculateSrcOfs(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesStandardUriConformant) == 0)
    {
        getScanner()->setStandardUriConformant(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesDOMHasPSVIInfo) == 0)
    {
        setCreateSchemaInfo(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesGenerateSyntheticAnnotations) == 0)
    {
        getScanner()->setGenerateSyntheticAnnotations(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesValidateAnnotations) == 0)
    {
        getScanner()->setValidateAnnotations(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIdentityConstraintChecking) == 0)
    {
        getScanner()->setIdentityConstraintChecking(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIgnoreCachedDTD) == 0)
    {
        getScanner()->setIgnoredCachedDTD(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIgnoreAnnotations) == 0)
    {
        getScanner()->setIgnoreAnnotations(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesDisableDefaultEntityResolution) == 0)
    {
        getScanner()->setDisableDefaultEntityResolution(state);
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSkipDTDValidation) == 0)
    {
        getScanner()->setSkipDTDValidation(state);
    }
    else
        throw DOMException(DOMException::NOT_FOUND_ERR, 0, getMemoryManager());
}

const void* DOMLSParserImpl::getParameter(const XMLCh* name) const
{
    if (XMLString::compareIStringASCII(name, XMLUni::fgDOMCharsetOverridesXMLEncoding) == 0) 
    {
        return (void*)fCharsetOverridesXMLEncoding;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMDisallowDoctype) == 0) 
    {
        // TODO
        return 0;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMIgnoreUnknownCharacterDenormalization) == 0) 
    {
        // TODO
        return 0;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMNamespaces) == 0) 
    {
        return (void*)getDoNamespaces();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMResourceResolver) == 0) 
    {
        return fEntityResolver;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSupportedMediatypesOnly) == 0) 
    {
        return (void*)false;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMValidate) == 0) 
    {
        return (void*)fValidation;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMValidateIfSchema) == 0) 
    {
        return (void*)fAutoValidation;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMWellFormed) == 0) 
    {
        return (void*)true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMCanonicalForm) == 0 ) 
    {
        // TODO
        return 0;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMCDATASections) == 0 ) 
    {
        // TODO
        return 0;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMCheckCharacterNormalization) == 0 ) 
    {
        // TODO
        return 0;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMComments) == 0) 
    {
        return (void*)getCreateCommentNodes();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMDatatypeNormalization) == 0) 
    {
        return (void*)getScanner()->getNormalizeData();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMElementContentWhitespace) == 0) 
    {
        return (void*)getIncludeIgnorableWhitespace();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMEntities) == 0) 
    {
        return (void*)getCreateEntityReferenceNodes();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMErrorHandler) == 0) 
    {
        return fErrorHandler;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMNamespaceDeclarations) == 0) 
    {
        return (void*)true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMNormalizeCharacters) == 0) 
    {
        return 0;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSchemaLocation) == 0) 
    {
        return 0;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSchemaType) == 0) 
    {
        return 0;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMSplitCDATASections) == 0) 
    {
        return 0;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgDOMInfoset) == 0) 
    {
        return (void*)true;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchema) == 0)
    {
        return (void*)getDoSchema();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaFullChecking) == 0)
    {
        return (void*)getValidationSchemaFullChecking();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIdentityConstraintChecking) == 0)
    {
        return (void*)getIdentityConstraintChecking();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesLoadExternalDTD) == 0)
    {
        return (void*)getLoadExternalDTD();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesContinueAfterFatalError) == 0)
    {
        return (void*)!getExitOnFirstFatalError();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesValidationErrorAsFatal) == 0)
    {
        return (void*)getValidationConstraintFatal();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesCacheGrammarFromParse) == 0)
    {
        return (void*)getScanner()->isCachingGrammarFromParse();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesUseCachedGrammarInParse) == 0)
    {
        return (void*)getScanner()->isUsingCachedGrammarInParse();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesCalculateSrcOfs) == 0)
    {
        return (void*)getScanner()->getCalculateSrcOfs();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesStandardUriConformant) == 0)
    {
        return (void*)getScanner()->getStandardUriConformant();
    }
    else if(XMLString::compareIStringASCII(name, XMLUni::fgXercesUserAdoptsDOMDocument) == 0) 
    {
        return (void*)fUserAdoptsDocument;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesDOMHasPSVIInfo) == 0) 
    {
        return (void*)getCreateSchemaInfo();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesGenerateSyntheticAnnotations) == 0)
    {
        return (void*)getScanner()->getGenerateSyntheticAnnotations();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesValidateAnnotations) == 0)
    {
        return (void*)getScanner()->getValidateAnnotations();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIgnoreCachedDTD) == 0)
    {
        return (void*)getScanner()->getIgnoreCachedDTD();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesIgnoreAnnotations) == 0)
    {
        return (void*)getScanner()->getIgnoreAnnotations();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesDisableDefaultEntityResolution) == 0)
    {
        return (void*)getScanner()->getDisableDefaultEntityResolution();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSkipDTDValidation) == 0)
    {
        return (void*)getScanner()->getSkipDTDValidation();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesEntityResolver) == 0) 
    {
        return fXMLEntityResolver;
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaExternalSchemaLocation) == 0) 
    {
        return getExternalSchemaLocation();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation) == 0) 
    {
        return getExternalNoNamespaceSchemaLocation();
    }
    else if (XMLString::compareIStringASCII(name, XMLUni::fgXercesSecurityManager) == 0) 
    {
        return getSecurityManager();
    }
    else
        throw DOMException(DOMException::NOT_FOUND_ERR, 0, getMemoryManager());
}

bool DOMLSParserImpl::canSetParameter(const XMLCh* name, const void* value) const
{
    if((XMLString::compareIStringASCII(name, XMLUni::fgDOMCharsetOverridesXMLEncoding) == 0)
      )
        return true;
    
    return false;
}

bool DOMLSParserImpl::canSetParameter(const XMLCh* name, bool value) const
{
    return false;
}

const RefVectorOf<const XMLCh*>* DOMLSParserImpl::getParameterNames() const
{
    return 0;
}

// ---------------------------------------------------------------------------
//  DOMLSParserImpl: Feature methods
// ---------------------------------------------------------------------------
void DOMLSParserImpl::release()
{
    DOMLSParserImpl* builder = (DOMLSParserImpl*) this;
    delete builder;
}

void DOMLSParserImpl::resetDocumentPool()
{
    resetPool();
}


// ---------------------------------------------------------------------------
//  DOMLSParserImpl: Parsing methods
// ---------------------------------------------------------------------------
DOMDocument* DOMLSParserImpl::parse(const DOMLSInput& source)
{
    Wrapper4DOMLSInput isWrapper((DOMLSInput*) &source, false, getMemoryManager());

    AbstractDOMParser::parse(isWrapper);
    if (fUserAdoptsDocument)
        return adoptDocument();
    else
        return getDocument();
}

DOMDocument* DOMLSParserImpl::parseURI(const XMLCh* const systemId)
{
    AbstractDOMParser::parse(systemId);
    if (fUserAdoptsDocument)
        return adoptDocument();
    else
        return getDocument();
}

DOMDocument* DOMLSParserImpl::parseURI(const char* const systemId)
{
    AbstractDOMParser::parse(systemId);
    if (fUserAdoptsDocument)
        return adoptDocument();
    else
        return getDocument();
}

void DOMLSParserImpl::parseWithContext(const DOMLSInput&,
                                      DOMNode* ,
                                      const unsigned short)
{
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0, getMemoryManager());
}

void DOMLSParserImpl::abort()
{
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0, getMemoryManager());
}

// ---------------------------------------------------------------------------
//  DOMLSParserImpl: Implementation of the XMLErrorReporter interface
// ---------------------------------------------------------------------------
void DOMLSParserImpl::error( const   unsigned int                code
                            , const XMLCh* const
                            , const XMLErrorReporter::ErrTypes  errType
                            , const XMLCh* const                errorText
                            , const XMLCh* const                systemId
                            , const XMLCh* const
                            , const XMLSSize_t                  lineNum
                            , const XMLSSize_t                  colNum)
{
    if (fErrorHandler) {

        short severity = DOMError::DOM_SEVERITY_ERROR;

        if (errType == XMLErrorReporter::ErrType_Warning)
            severity = DOMError::DOM_SEVERITY_WARNING;
        else if (errType == XMLErrorReporter::ErrType_Fatal)
            severity = DOMError::DOM_SEVERITY_FATAL_ERROR;

        DOMLocatorImpl location((int)lineNum, (int) colNum, getCurrentNode(), systemId);
        DOMErrorImpl domError(severity, errorText, &location);

        // if user return false, we should stop the process, so throw an error
        if (!fErrorHandler->handleError(domError) && !getScanner()->getInException())
            throw (XMLErrs::Codes) code;
    }
}

void DOMLSParserImpl::resetErrors()
{
}


// ---------------------------------------------------------------------------
//  DOMLSParserImpl: Implementation of XMLEntityHandler interface
// ---------------------------------------------------------------------------
InputSource*
DOMLSParserImpl::resolveEntity(const XMLCh* const publicId,
                              const XMLCh* const systemId,
                              const XMLCh* const baseURI)
{
    //
    //  Just map it to the SAX entity resolver. If there is not one installed,
    //  return a null pointer to cause the default resolution.
    //
    if (fEntityResolver) {

        DOMLSInput* is = fEntityResolver->resolveEntity(0, 0, publicId, systemId, baseURI);

        if (is)
            return new (getMemoryManager()) Wrapper4DOMLSInput(is, true, getMemoryManager());
    }

    return 0;
}

InputSource*
DOMLSParserImpl::resolveEntity( XMLResourceIdentifier* resourceIdentifier )
{
    //
    //  Just map it to the SAX entity resolver. If there is not one installed,
    //  return a null pointer to cause the default resolution.
    //
    if (fEntityResolver) {
        DOMLSInput* is = fEntityResolver->resolveEntity(resourceIdentifier->getResourceIdentifierType()==XMLResourceIdentifier::ExternalEntity?XMLUni::fgDOMDTDType:XMLUni::fgDOMXMLSchemaType,
                                                        resourceIdentifier->getNameSpace(),
                                                        resourceIdentifier->getPublicId(),
                                                        resourceIdentifier->getSystemId(), 
                                                        resourceIdentifier->getBaseURI());
        if (is)
            return new (getMemoryManager()) Wrapper4DOMLSInput(is, true, getMemoryManager());    
    }
    if (fXMLEntityResolver) {
        return(fXMLEntityResolver->resolveEntity(resourceIdentifier));    
    }
    
    return 0;
}

typedef JanitorMemFunCall<DOMLSParserImpl>    ResetParseType;

// ---------------------------------------------------------------------------
//  DOMLSParserImpl: Grammar preparsing methods
// ---------------------------------------------------------------------------
Grammar* DOMLSParserImpl::loadGrammar(const char* const systemId,
                                     const short grammarType,
                                     const bool toCache)
{
    // Avoid multiple entrance
    if (getParseInProgress())
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    ResetParseType  resetParse(this, &DOMLSParserImpl::resetParse);

	Grammar* grammar = 0;

    try
    {
        setParseInProgress(true);
        if (grammarType == Grammar::DTDGrammarType) 
            getScanner()->setDocTypeHandler(0);
        grammar = getScanner()->loadGrammar(systemId, grammarType, toCache);

        // Release DOM tree - DTD
        DOMDocument* doc = adoptDocument();
        if (doc)
            doc->release();
    }
    catch(const OutOfMemoryException&)
    {
        resetParse.release();

        throw;
    }

    return grammar;
}

Grammar* DOMLSParserImpl::loadGrammar(const XMLCh* const systemId,
                                     const short grammarType,
                                     const bool toCache)
{
    // Avoid multiple entrance
    if (getParseInProgress())
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    ResetParseType  resetParse(this, &DOMLSParserImpl::resetParse);

	Grammar* grammar = 0;

    try
    {
        setParseInProgress(true);
        if (grammarType == Grammar::DTDGrammarType) 
            getScanner()->setDocTypeHandler(0);
        grammar = getScanner()->loadGrammar(systemId, grammarType, toCache);

        // Release DOM tree - DTD
        DOMDocument* doc = adoptDocument();
        if (doc)
            doc->release();
    }
    catch(const OutOfMemoryException&)
    {
        resetParse.release();

        throw;
    }

    return grammar;
}

Grammar* DOMLSParserImpl::loadGrammar(const DOMLSInput& source,
                                     const short grammarType,
                                     const bool toCache)
{
    // Avoid multiple entrance
    if (getParseInProgress())
        ThrowXMLwithMemMgr(IOException, XMLExcepts::Gen_ParseInProgress, fMemoryManager);

    ResetParseType  resetParse(this, &DOMLSParserImpl::resetParse);

    Grammar* grammar = 0;

    try
    {
        Wrapper4DOMLSInput isWrapper((DOMLSInput*) &source, false, getMemoryManager());

        setParseInProgress(true);
        if (grammarType == Grammar::DTDGrammarType) 
            getScanner()->setDocTypeHandler(0);
        grammar = getScanner()->loadGrammar(isWrapper, grammarType, toCache);

        // Release DOM tree - DTD
        DOMDocument* doc = adoptDocument();
        if (doc)
            doc->release();
    }
    catch(const OutOfMemoryException&)
    {
        resetParse.release();

        throw;
    }

    return grammar;
}

void DOMLSParserImpl::resetCachedGrammarPool()
{
    getGrammarResolver()->resetCachedGrammar();
}

void DOMLSParserImpl::resetParse()
{
    if (getScanner()->getDocTypeHandler() == 0)
    {
        getScanner()->setDocTypeHandler(this);
    }

    setParseInProgress(false);
}

Grammar* DOMLSParserImpl::getGrammar(const XMLCh* const nameSpaceKey) const
{
    return getGrammarResolver()->getGrammar(nameSpaceKey);
}

Grammar* DOMLSParserImpl::getRootGrammar() const
{
    return getScanner()->getRootGrammar();
}

const XMLCh* DOMLSParserImpl::getURIText(unsigned int uriId) const
{
    return getScanner()->getURIText(uriId);
}

unsigned int DOMLSParserImpl::getSrcOffset() const
{
    return getScanner()->getSrcOffset();
}

XERCES_CPP_NAMESPACE_END

