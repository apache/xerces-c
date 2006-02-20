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
 * This is the Main Xerces-* interface file. It is designed to be
 *   language agnostic so that it is useful for multiple scripting 
 *   languages. It is also modular - so all the component are in
 *   interface/ directory.
 */

/*
 * Modulename - this *must* be in double quotes to properly handle
 *   the namespace
 */
#ifdef SWIGPERL
%module "XML::Xerces"
#endif

%include "defines.i"

%include "typemaps.i"

/*
 * Import the language specific macros
 */

#ifdef SWIGPERL
%include "Perl/callback.i"
%include "Perl/shadow.i"
#endif

/*****************************/
/*                           */
/*  Platforms and Compilers  */
/*                           */
/*****************************/

// we seem to need these defs loaded before parsing XercesDefs.hpp
// as of Xerces-3.0
%import "xercesc/util/Xerces_autoconf_config.hpp" // for XMLSize_t and namespaces

%import "xercesc/util/XercesDefs.hpp"

/******************/
/*                */
/*  General code  */
/*                */
/******************/

/*
 * The generic exception handler
 *    'goto fail' must be called - either explicitly, or via SWIG_croak()
 *    to ensure that any variable cleanup is done - to avoid memory leaks.
 *    By making these macros, it reduces the code size dramatically
 */

%{
#define CATCH_XML_EXCEPTION       \
    catch (const XMLException& e) \
    {                             \
        makeXMLException(e);      \
	goto fail;                \
    }                             \
    catch (...)                   \
    {                             \
        SWIG_croak("Handling Unknown exception"); \
	goto fail;                \
    }
%}

%exception {
    try 
    {
        $action
    } 
    CATCH_XML_EXCEPTION
}

/*
 * Have the scripting language manage the memory for objects created
 * in factory methods SWIG will automatically handle objects created
 * in constructors but it must be told what methods are factory
 * methods
 */
// %newobject createDOMWriter;
// %newobject createDocumentType;
// %newobject createDocument;
// %newobject getDOMImplementation;
%newobject createXMLReader;

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
 * Utility Classes
 */

%include "util.i"

/*
 * XML Schema Grammar support (DTD and W3C XML Schema)
 */

%include "grammars.i"

/*
 * InputSource
 */

%include "input-source.i"

/*
 * DOM
 */

%include "DOM.i"

/*
 * SAX1
 */

%include "SAX.i"

/*
 * SAX2
 */

%include "SAX2.i"

/* 
 * Parsers
 */

%include "parsers.i"

/* 
 * Callbacks - this needs to be at the very end
 *   so that SWIG can wrap the superclass methods properly
 */

%include "callback.i"

/* 
 * Include extra verbatim C code in the initialization function
 */
%init {
    // we create the global transcoder for UTF-8 to UTF-16
    XMLTransService::Codes failReason;
    XMLPlatformUtils::Initialize(); // first we must create the transservice
    UTF8_ENCODING = XMLString::transcode("UTF-8");
    UTF8_TRANSCODER =
      XMLPlatformUtils::fgTransService->makeNewTranscoderFor(UTF8_ENCODING,
                                                             failReason,
                                                             1024,
							     XMLPlatformUtils::fgMemoryManager);
    if (! UTF8_TRANSCODER) {
	croak("ERROR: XML::Xerces: INIT: Could not create UTF-8 transcoder");
    }
}

#ifdef SWIGPERL

/* 
 * Include extra verbatim Perl code
 */
%pragma(perl5) include="../interfaces/Perl/Xerces-extra.pm"

/* 
 * Include extra verbatim Perl code immediately after Module header
 */

%pragma(perl5) code="package XML::Xerces; 
use vars qw($VERSION @EXPORT);"

#endif
