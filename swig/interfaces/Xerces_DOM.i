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
%module(package="XML::Xerces") "XML::Xerces::DOM"
#endif
#ifdef SWIGXML
%module(package="XML::Xerces") "XML::Xerces::DOM"
#endif

%include "io-includes.i"
%include "dom-includes.i"

/*
 * After this we will be under the Xerces namespace
 *
 */

%{

XERCES_CPP_NAMESPACE_USE

void
makeDOMException(const DOMException& e){
    SV *error = ERRSV;
    SWIG_MakePtr(error, (void *) new DOMException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMException, SWIG_SHADOW|0);
}

// void
// makeDOMXPathException(const DOMXPathException& e){
//     SV *error = ERRSV;
//     SWIG_MakePtr(error, (void *) new DOMXPathException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMXPathException, SWIG_SHADOW|0);
// }

%}

/*
 * Import the common macros
 */
%include "Xerces_common.i"

/*
 * Import the language-specific macros
 */

#ifdef SWIGPERL
%include "Perl/dom-shadow.i"
%include "Perl/dom-includes.i"
#endif

/*
 * Import the module-specific typemaps
 */

%include "typemaps-dom.i"
%include "typemaps-domnode.i"

/*
 * Import the type information from other modules
 */

%import "Xerces.i"
%import "Xerces_IO.i"

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

#define CATCH_DOMXPATH_EXCEPTION    \
    catch (const XMLException& e)   \
    {                               \
        makeXMLException(e);        \
	goto fail;                  \
    }                               \
    catch (const DOMXPathException& e)   \
    {                               \
	makeDOMXPathException(e);    \
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

%define XPATH_METHOD(class,method)
%exception XERCES_CPP_NAMESPACE::class::method %{
    try 
    {
        $action
    } 
    CATCH_DOMXPATH_EXCEPTION
%}
%enddef

%define XPATH_CLASS(class)
%exception XERCES_CPP_NAMESPACE::class %{
    try 
    {
        $action
    } 
    CATCH_DOMXPATH_EXCEPTION
%}
%enddef

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

// Introduced in DOM Level 2
%include "dom/DOMDocumentRange.i"
%include "dom/DOMDocumentTraversal.i"
%include "dom/DOMNodeIterator.i"

%feature("notabstract", 1) DOMNodeFilter;
%include "dom/DOMNodeFilter.i"
%include "dom/DOMRange.i"
%include "dom/DOMRangeException.i"
%include "dom/DOMTreeWalker.i"

/*
 * Introduced in DOM Level 3
 */
%include "dom/DOMStringList.i"
%include "dom/DOMUserDataHandler.i"
%include "dom/DOMConfiguration.i"
%include "dom/DOMImplementationLS.i"
%include "dom/DOMImplementation.i"
%include "dom/DOMImplementationList.i"
%include "dom/DOMImplementationRegistry.i"

// XPath - currently unimplemented
%import "dom/DOMXPathEvaluator.i"
%import "dom/DOMXPathException.i"
%import "dom/DOMXPathExpression.i"
%import "dom/DOMXPathNamespace.i"
%import "dom/DOMXPathNSResolver.i"
%import "dom/DOMXPathResult.i"

// Interfaces not useful for scripting languages
// %include "dom/DOMImplementationSource.i"

%include "dom/DOMError.i"
%include "dom/DOMErrorHandler.i"

// from Level 1, but with extensions from Level 3
%include "dom/DOMDocument.i"

%include "dom/DOMLocator.i"
%include "dom/DOMLSResourceResolver.i"

// DOMLSSerializer
%include "dom/DOMLSOutput.i"

%include "dom/DOMLSException.i"

%feature("notabstract", 1) DOMLSSerializerFilter;
%include "dom/DOMLSSerializerFilter.i"
%include "dom/DOMLSSerializer.i"

// Base class for DOMLSParser and XercesDOMParser
%include "parsers/AbstractDOMParser.i"

// DOMLSParser
%include "dom/DOMLSInput.i"
%include "framework/Wrapper4InputSource.i"
%include "framework/Wrapper4DOMLSInput.i"
%include "dom/DOMLSParserFilter.i"
%include "dom/DOMLSParser.i"

// XercesDOMParser
%feature("notabstract", 1) XercesDOMParser;
%include "parsers/XercesDOMParser.i"

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

#endif
