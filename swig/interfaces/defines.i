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
 * defines.i - This file contains all directives included
 *   verbatim into the C++ output file
 */

%{

XERCES_CPP_NAMESPACE_USE

// we initialize the static UTF-8 transcoding info
// these are used by the typemaps to convert between
// Xerces internal UTF-16 and Perl's internal UTF-8
static Transcoder* UTF8_TRANSCODER  = NULL;

static XMLExceptionHandler* XML_EXCEPTION_HANDLER  = NULL;

static bool DEBUG_UTF8_OUT = 0;
static bool DEBUG_UTF8_IN = 0;

%}

// These get wrapped by SWIG so that we can modify them from the scripting language
bool DEBUG_UTF8_OUT;
bool DEBUG_UTF8_IN;


%{

void
makeXMLException(const XMLException& e){
    SV *error = ERRSV;
    SWIG_MakePtr(error, (void *) XML_EXCEPTION_HANDLER->copyXMLException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__XMLException, SWIG_SHADOW|0);
}

void
makeSAXNotRecognizedException(const SAXNotRecognizedException& e){
    SV *error = ERRSV;
    SWIG_MakePtr(error, (void *) new SAXNotRecognizedException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__SAXNotRecognizedException, SWIG_SHADOW|0);
}

void
makeSAXNotSupportedException(const SAXNotSupportedException& e){
    SV *error = ERRSV;
    SWIG_MakePtr(error, (void *) new SAXNotSupportedException(e), SWIGTYPE_p_XERCES_CPP_NAMESPACE__SAXNotSupportedException, SWIG_SHADOW|0);
}

%}

// %include "make-xml-exception.i"
