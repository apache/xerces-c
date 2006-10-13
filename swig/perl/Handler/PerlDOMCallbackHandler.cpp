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

#include <stdlib.h>
#include "PerlDOMCallbackHandler.hpp"
#include "xerces-swig-perl.hpp"

XERCES_CPP_NAMESPACE_USE

PerlDOMCallbackHandler::PerlDOMCallbackHandler()
{
    callbackObj = NULL;
}

PerlDOMCallbackHandler::~PerlDOMCallbackHandler()
{}

PerlDOMCallbackHandler::PerlDOMCallbackHandler(SV *obj)
{
    set_callback_obj(obj);
}

bool
PerlDOMCallbackHandler::handleError(const DOMError& error) {
    if (!callbackObj) {
	die("Received handleError and no ErrorHandler was set");
    }

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);

	// first put the callback object on the stack
    XPUSHs(callbackObj);

	// then put the exception on the stack
    char *class_name = "XML::Xerces::DOMError";
    XPUSHs(sv_setref_pv(sv_newmortal(), 
			class_name, 
			(void *)&error));
    PUTBACK;

    int count = perl_call_method("handle_error", G_SCALAR);

    SPAGAIN ;

    SV* source_sv;
    if (count != 1)
    {
	warn("ErroHandler did not return a value\n");
	source_sv = &PL_sv_undef;
    }
    else
    {
        source_sv = POPs;
    }

    FREETMPS;
    LEAVE;

    return SvTRUE(source_sv);
}

DOMLSInput*
PerlDOMCallbackHandler::resolveResource(const XMLCh* const    resourceType
					   , const XMLCh* const    namespaceUri
					   , const XMLCh* const    publicId
					   , const XMLCh* const    systemId
					   , const XMLCh* const    baseURI)
{
    if (!callbackObj) {
        croak("\nresolveResource: no ResourceResolver set\n");
	return NULL;
    }

    dSP;
    DOMLSInput *source;
    char *isName = "XML::Xerces::DOMLSInput";

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the first argument is the resourceType
    SV *string1 = UTF8_TRANSCODER->XMLString2Local(resourceType);
    XPUSHs(string1);

        // the next argument is the namespaceUri
    SV *string2 = UTF8_TRANSCODER->XMLString2Local(namespaceUri);
    XPUSHs(string2);

        // the next argument is the publicId
    SV *string3 = UTF8_TRANSCODER->XMLString2Local(publicId);
    XPUSHs(string3);

        // the next argument is the systemId
    SV *string4 = UTF8_TRANSCODER->XMLString2Local(systemId);
    XPUSHs(string4);

        // the next argument is the baseURI
    SV *string5 = UTF8_TRANSCODER->XMLString2Local(baseURI);
    XPUSHs(string5);

    PUTBACK;

    int count = perl_call_method("resolve_entity", G_SCALAR);

    SPAGAIN ;

    SV* source_sv;
    if (count != 1)
    {
	warn("EntityResolver did not retury any object\n");
	source_sv = &PL_sv_undef;
    }
    else
    {
        source_sv = POPs;
    }

    if (count == 1 
	&& SvOK(source_sv)
	&& !sv_derived_from(source_sv, isName)) {
	croak("ResourceResolver did not return an DOMLSInput\n") ;
    }

    // god bless John Lenz's new type system in SWIG 1.3.25!!!
    swig_type_info *isType = SWIG_TypeQuery(isName);
    if (SWIG_ConvertPtr(source_sv,(void **) &source, isType, 0) < 0) {
        croak("ResourceResolver did not return an DOMLSInput. Expected %s", isName);
    }
    PUTBACK ;
    FREETMPS;
    LEAVE;
    return source;
}

