/*
 * Copyright 2002,2004 The Apache Software Foundation.
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

/* 
 * All DOM classes and their related SWIG alterations go in this interface file
 */

/*
 * Modulename - this *must* be in double quotes to properly handle
 *   the namespace
 */
#ifdef SWIGPERL
%module(package="XML::Xerces") "DOM"
// %module "XML::Xerces::DOM"
#endif

%include "includes.i"

%include "dom-includes.i"

/*
 * Import the language specific macros
 */

#ifdef SWIGPERL
%include "Perl/errors.i"
%include "Perl/defines.i"
%include "Perl/dom-shadow.i"
%include "Perl/typemaps.i"
%include "Perl/typemaps-xmlch.i"
%include "Perl/includes.i"
%include "Perl/dom-includes.i"
#endif

// Get the type information
%import "Xerces.i"

/*
 * After this we will be under the Xerces namespace
 *
 */

%{

XERCES_CPP_NAMESPACE_USE

// we initialize the static UTF-8 transcoding info
// these are used by the typemaps to convert between
// Xerces internal UTF-16 and Perl's internal UTF-8
static Transcoder* UTF8_TRANSCODER  = NULL;

static XMLExceptionHandler* XML_EXCEPTION_HANDLER  = NULL;

void
makeXMLException(const XMLException& e){
    SV *error = ERRSV;
    SWIG_MakePtr(error, (void *) XML_EXCEPTION_HANDLER->copyXMLException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__XMLException, SWIG_SHADOW|0);
}

void
makeDOMException(const DOMException& e){
    SV *error = ERRSV;
    SWIG_MakePtr(error, (void *) new DOMException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMException, SWIG_SHADOW|0);
}

%}

// %include "make-xml-exception.i"

%include "typemaps-dom.i"

%include "typemaps-general.i"

/*****************************/
/*                           */
/*  Platforms and Compilers  */
/*                           */
/*****************************/

// we seem to need these defs loaded before parsing XercesDefs.hpp
// as of Xerces-3.0
%import "xercesc/util/Xerces_autoconf_config.hpp" // for XMLSize_t and namespaces

%import "xercesc/util/XercesDefs.hpp"

/* 
 * All %ignore directives
 */

%include "ignore.i"

/*
 * Operator support
 */

// Operators we do want
// %rename(operator_assignment) operator=;
%rename(operator_equal_to) operator==;
%rename(operator_not_equal_to) operator!=;


/*
 * the DOM classes gets a special exception handler
 *    'goto fail' must be called - either explicitly, or via SWIG_croak()
 *    to ensure that any variable cleanup is done - to avoid memory leaks.
 *    By making these macros, it reduces the code *file* size dramatically
 *    (but doesn't reduce the compiled file size at all...)
 */

%{
#define CATCH_DOM_EXCEPTION         \
    catch (const XMLException& e)   \
    {                               \
        makeXMLException(e);        \
	goto fail;                  \
    }                               \
    catch (const DOMException& e)   \
    {                               \
	makeDOMException(e);        \
	goto fail;                  \
    }                               \
    catch (...)                     \
    {                               \
        SWIG_croak("Handling Unknown exception"); \
        goto fail;                  \
    }
%}

%exception {
    try 
    {
        $action
    } 
    CATCH_DOM_EXCEPTION
}


// Introduced in DOM Level 1
%include "dom/DOMException.i"
%include "dom/DOMNode.i"
%include "dom/DOMAttr.i"
%include "dom/DOMElement.i"
%include "dom/DOMEntity.i"
%include "dom/DOMDocumentType.i"
%include "dom/DOMCharacterData.i"
%include "dom/DOMComment.i"
%include "dom/DOMText.i"
%include "dom/DOMCDATASection.i"
%include "dom/DOMNodeList.i"
%include "dom/DOMNamedNodeMap.i"
%include "dom/DOMDocumentFragment.i"
%include "dom/DOMEntityReference.i"
%include "dom/DOMNotation.i"
%include "dom/DOMProcessingInstruction.i"

// type info
%include "dom/DOMTypeInfo.i"
%include "dom/DOMPSVITypeInfo.i"

// XPath
%include "dom/DOMXPathEvaluator.i"
%include "dom/DOMXPathException.i"
%include "dom/DOMXPathExpression.i"
%include "dom/DOMXPathNamespace.i"
%include "dom/DOMXPathNSResolver.i"
%include "dom/DOMXPathResult.i"

// Introduced in DOM Level 2
%include "dom/DOMDocumentRange.i"
%include "dom/DOMDocumentTraversal.i"
%include "dom/DOMNodeIterator.i"
%include "dom/DOMNodeFilter.i"
%include "dom/DOMRange.i"
%include "dom/DOMRangeException.i"
%include "dom/DOMTreeWalker.i"

/*
 * Introduced in DOM Level 3
 */
%include "dom/DOMUserDataHandler.i"
%include "dom/DOMConfiguration.i"
%include "dom/DOMStringList.i"
%include "dom/DOMImplementationLS.i"
%include "dom/DOMImplementation.i"
%include "dom/DOMImplementationList.i"
%include "dom/DOMImplementationSource.i"
%include "dom/DOMImplementationRegistry.i"

%include "dom/DOMError.i"
%include "dom/DOMErrorHandler.i"
%include "dom/DOMDocument.i"
%include "dom/DOMLocator.i"
%include "dom/DOMLSResourceResolver.i"

// DOMLSParser
%include "dom/DOMLSInput.i"
%include "framework/Wrapper4InputSource.i"
%include "framework/Wrapper4DOMLSInput.i"
%include "dom/DOMLSParserFilter.i"
%include "dom/DOMLSParser.i"

// DOMWriter
%include "dom/DOMLSOutput.i"
%include "framework/XMLFormatter.i"
%include "framework/StdOutFormatTarget.i"
%include "framework/LocalFileFormatTarget.i"
%include "framework/MemBufFormatTarget.i"

%include "dom/DOMLSException.i"
%include "dom/DOMLSSerializer.i"
%include "dom/DOMLSSerializerFilter.i"

%import "xercesc/framework/XMLPScanToken.hpp"
%import "xercesc/framework/psvi/PSVIHandler.hpp"

%include "parsers/AbstractDOMParser.i"
%include "parsers/XercesDOMParser.i"

/* 
 * Include extra verbatim C code in the initialization function
 */
%init {
    // we create the global transcoder for UTF-8 to UTF-16
    // must initialize the Xerces-C transcoding service
    XMLPlatformUtils::Initialize();
    UTF8_TRANSCODER = Transcoder::getInstance();
    if (! UTF8_TRANSCODER) {
	croak("ERROR: XML::Xerces: INIT: Could not create UTF-8 transcoder");
    }

    XML_EXCEPTION_HANDLER = XMLExceptionHandler::getInstance();
    if (! XML_EXCEPTION_HANDLER) {
	croak("ERROR: XML::Xerces: INIT: Could not create XMLExceptionHandler");
    }
}

#ifdef SWIGPERL

/* 
 * Callbacks - this needs to be at the very end
 *   so that SWIG can wrap the superclass methods properly
 */

%include "Perl/dom-callback.i"

/* 
 * Include extra verbatim Perl code
 */
%pragma(perl5) include="../../interfaces/Perl/Xerces_DOM-extra.pm"

/* 
 * Include extra verbatim Perl code immediately after Module header
 */

%pragma(perl5) code="package XML::Xerces::DOM; 
use XML::Xerces;
use vars qw($VERSION @EXPORT);"

#endif
