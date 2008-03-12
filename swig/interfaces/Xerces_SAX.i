/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
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
 * All SAX/SAX2 classes and their related SWIG alterations go in this interface file
 */

/*
 * Modulename - this *must* be in double quotes to properly handle
 *   the namespace
 */
#ifdef SWIGPERL
%module(package="XML::Xerces") "XML::Xerces::SAX"
#endif

/*
 * All the module-specific includes
 *
 */
%include "io-includes.i"
%include "sax-includes.i"

/*
 * After this we will be under the Xerces namespace
 *
 */

%{

XERCES_CPP_NAMESPACE_USE

static void
makeSAXNotRecognizedException(const SAXNotRecognizedException& e){
    SV *error = ERRSV;
    SWIG_MakePtr(error, (void *) new SAXNotRecognizedException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__SAXNotRecognizedException, SWIG_SHADOW|0);
}

static void
makeSAXNotSupportedException(const SAXNotSupportedException& e){
    SV *error = ERRSV;
    SWIG_MakePtr(error, (void *) new SAXNotSupportedException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__SAXNotSupportedException, SWIG_SHADOW|0);
}

%}

/*
 * Import the common macros
 */
%include "Xerces_common.i"

/*
 * Import the language specific macros
 */

#ifdef SWIGPERL
%include "Perl/shadow.i"
%include "Perl/sax-shadow.i"
%include "Perl/sax-includes.i"
#endif

/*
 * Import the module-specific typemaps
 */

%include "typemaps-sax.i"

/*
 * Import the type information from other modules
 */

%import "Xerces.i"
%import "Xerces_IO.i"

/*
 * Have the scripting language manage the memory for objects created
 * in factory methods SWIG will automatically handle objects created
 * in constructors but it must be told what methods are factory
 * methods
 */
%newobject createXMLReader;

/*
 * Define exception handlers
 *
 */

%exception {
    try 
    {
        $action
    } 
    CATCH_XML_EXCEPTION
}

/*
 * SAX1
 */

%include "SAX.i"

/*
 * SAX2
 */

%include "SAX2.i"

%include "xercesc/sax/Parser.hpp"
%include "xercesc/parsers/SAXParser.hpp"

/*
 * the SAX2XMLReader methods gets a special exception handler
 *    'goto fail' must be called - either explicitly, or via SWIG_croak()
 *    to ensure that any variable cleanup is done - to avoid memory leaks.
 *    We make this a macro to be similar to the other exception handlers.
 */
%{
#define CATCH_SAX_EXCEPTION                           \
    catch (const XMLException& e)                     \
        {                                             \
	    makeXMLException(e);                      \
            goto fail;                                \
        }                                             \
    catch (const SAXNotSupportedException& e)         \
	{                                             \
	    makeSAXNotSupportedException(e);          \
            goto fail;                                \
	}                                             \
    catch (const SAXNotRecognizedException& e)        \
	{                                             \
	    makeSAXNotRecognizedException(e);         \
            goto fail;                                \
	}                                             \
    catch (...)                                       \
        {                                             \
            SWIG_croak("Handling Unknown exception"); \
            goto fail;                                \
        }
%}

%define SAXEXCEPTION(method)
%exception method {
    try {
        $action
    } 
    CATCH_SAX_EXCEPTION
}
%enddef

SAXEXCEPTION(XERCES_CPP_NAMESPACE::SAX2XMLReader::getFeature)
SAXEXCEPTION(XERCES_CPP_NAMESPACE::SAX2XMLReader::setFeature)
SAXEXCEPTION(XERCES_CPP_NAMESPACE::SAX2XMLReader::setProperty)
SAXEXCEPTION(XERCES_CPP_NAMESPACE::SAX2XMLReader::getProperty)
SAXEXCEPTION(XERCES_CPP_NAMESPACE::SAX2XMLReader::parse)

%include "xercesc/sax2/SAX2XMLReader.hpp"
%include "xercesc/sax2/XMLReaderFactory.hpp"

#ifdef SWIGPERL

/* 
 * Callbacks - this needs to be at the very end
 *   so that SWIG can wrap the superclass methods properly
 */

%include "Perl/sax-callback.i"

/* 
 * Include extra verbatim Perl code
 */
%pragma(perl5) include="../../interfaces/Perl/Xerces_SAX-extra.pm"

#endif
