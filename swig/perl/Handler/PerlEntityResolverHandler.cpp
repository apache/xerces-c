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
#include "xerces-swig-perl.hpp"

XERCES_CPP_NAMESPACE_USE

PerlEntityResolverHandler::PerlEntityResolverHandler()
{
    callbackObj = NULL;
}

PerlEntityResolverHandler::~PerlEntityResolverHandler()
{}

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
    char *isName = "XML::Xerces::InputSource";

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the publicId
    SV *string1 = UTF8_TRANSCODER->XMLString2Local(publicId);
    XPUSHs(string1);

        // the next argument is the systemId
    SV *string2 = UTF8_TRANSCODER->XMLString2Local(systemId);
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

    if (count == 1 
	&& SvOK(source_sv)
	&& !sv_derived_from(source_sv, isName)) {
	croak("EntityResolver did not return an InputSource\n") ;
    }

    // god bless John Lenz's new type system in SWIG 1.3.25!!!
    swig_type_info *isType = SWIG_TypeQuery(isName);
    if (SWIG_ConvertPtr(source_sv,(void **) &source, isType, 0) < 0) {
        croak("EntityResolver did not return an InputSource. Expected %s", isName);
    }
    PUTBACK ;
    FREETMPS;
    LEAVE;
    return source;
}

InputSource *
PerlEntityResolverHandler::resolveEntity (XMLResourceIdentifier* id)
{
    if (!callbackObj) {
        croak("\nresolveEntity: no EntityResolver set\n");
	return NULL;
    }

    dSP;
    InputSource *source;
    char *isName = "XML::Xerces::InputSource";
    char *resIdName = "XML::Xerces::XMLResourceIdentifier";

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

    // the only argument is the ResourceIdentifier
    // god bless John Lenz's new type system in SWIG 1.3.25!!!
    swig_type_info *ty = SWIG_TypeQuery(resIdName);
    SV* resId_sv = sv_newmortal();
    SWIG_MakePtr(resId_sv, (void *) id, ty,0);
    XPUSHs(resId_sv);

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
	croak("EntityResolver did not return an InputSource\n") ;
    }

    // god bless John Lenz's new type system in SWIG 1.3.25!!!
    swig_type_info *isType = SWIG_TypeQuery(isName);
    if (SWIG_ConvertPtr(source_sv,(void **) &source, isType, 0) < 0) {
        croak("EntityResolver did not return an InputSource. Expected %s", isName);
    }
    PUTBACK ;
    FREETMPS;
    LEAVE;
    return source;
}

