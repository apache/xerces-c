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
 * All DOM classes and their related SWIG alterations go in this interface file
 */

/*
 * Modulename - this *must* be in double quotes to properly handle
 *   the namespace
 */
#ifdef SWIGPERL
%module(package="XML::Xerces") "XML::Xerces::IO"
#endif
#ifdef SWIGXML
%module(package="XML::Xerces") "XML::Xerces::IO"
#endif

/*
 * All the module-specific includes
 *
 */
%include "io-includes.i"

/*
 * After this we will be under the Xerces namespace
 *
 */

%{

XERCES_CPP_NAMESPACE_USE

%}

/*
 * Import the common macros
 */
%include "Xerces_common.i"

/*
 * Import the module-specific typemaps
 */

%include "typemaps-io.i"

/*
 * Import the language specific macros
 */

#ifdef SWIGPERL
%include "Perl/typemaps-io.i"
#endif

/*
 * Import the type information from other modules
 */

%import "Xerces.i"

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
 * InputSource
 */

%include "input-source.i"


/*
 * OutputTarget
 */

%include "output-target.i"

#ifdef SWIGPERL

/* 
 * Include extra verbatim Perl code
 */
%pragma(perl5) include="../../interfaces/Perl/Xerces_IO-extra.pm"

#endif
