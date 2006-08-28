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

#include "xercesc/sax/InputSource.hpp"
#include "PerlEntityResolverHandler.hpp"

PerlEntityResolverHandler::PerlEntityResolverHandler()
{
    callbackObj = NULL;
}

PerlEntityResolverHandler::~PerlEntityResolverHandler()
{
    if (callbackObj != NULL) {
	SvREFCNT_dec(callbackObj);
	callbackObj = NULL;
    }
}

PerlEntityResolverHandler::PerlEntityResolverHandler(SV *obj)
{
    if (!sv_isobject(obj)) {
	croak("expect object");
    }
    if (!sv_derived_from(obj,"XML::Xerces::PerlEntityResolver")) {
	croak("expect subclass of XML::Xerces::PerlEntityResolver");
    }
    set_callback_obj(obj);
}

// SV*
// PerlEntityResolverHandler::set_callback_obj(SV* object) {
//     SV *oldRef = &PL_sv_undef;	// default to 'undef'
//     if (callbackObj != NULL) {
// 	oldRef = callbackObj;
// #if defined(PERL_VERSION) && PERL_VERSION >= 8
// //	SvREFCNT_dec(oldRef);
// #endif
//     }
//     SvREFCNT_inc(object);
//     callbackObj = object;
//     return oldRef;
// }

InputSource *
PerlEntityResolverHandler::resolveEntity (const XMLCh* const publicId, 
					  const XMLCh* const systemId)
{
    if (!callbackObj) {
        croak("\nresolveEntity: no EntityResolver set\n");
	return NULL;
    }

    dSP;
    InputSource *source;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the publicId
    SV *string1 = XMLString2Perl(publicId);
    XPUSHs(string1);

        // the next argument is the systemId
    SV *string2 = XMLString2Perl(systemId);
    XPUSHs(string2);

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

    if (count == 1 && SvOK(source_sv) && !sv_derived_from(source_sv,"XML::Xerces::InputSource")) {
	croak("EntityResolver did not return an InputSource\n") ;
    }

    if (SWIG_ConvertPtr(source_sv,(void **) &source, SWIGTYPE_p_XERCES_CPP_NAMESPACE__InputSource,0) < 0) {

        croak("EntityResolver did not return an InputSource. Expected %s", SWIGTYPE_p_XERCES_CPP_NAMESPACE__InputSource->name);
    }
    PUTBACK ;
    FREETMPS;
    LEAVE;
    return source;
}
