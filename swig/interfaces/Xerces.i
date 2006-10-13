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

/*
 * Import the common macros
 */
%include "Xerces_common.i"

/*
 *    after this everything will be in Xerces namespace
 */ 
%{

XERCES_CPP_NAMESPACE_USE

%}

// This prevents a million warnings
%ignore XSerializable;
class XSerializable {
private:
XSerializable::XSerializable();
};

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
 * Module specific classes
 *
 */

/*
 * Unicode contants
 */

%include "unicode.i"

/*
 * Utility Classes
 */

%include "util.i"

/*
 * XML Schema Grammar support (DTD and W3C XML Schema)
 */

%include "grammars.i"

/* 
 * Auxiliary classes need for both DOM and SAX parsers
 */

%include "parsers.i"

#ifdef SWIGPERL

/* 
 * Callbacks - this needs to be at the very end
 *   so that SWIG can wrap the superclass methods properly
 */

// must %include this or getErrorHandler() fails to return proper type
%include "xercesc/sax/ErrorHandler.hpp"
%include "Perl/callback.i"

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
