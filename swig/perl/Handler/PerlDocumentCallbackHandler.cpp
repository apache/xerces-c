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
#include "PerlDocumentCallbackHandler.hpp"

PerlDocumentCallbackHandler::PerlDocumentCallbackHandler()
{
    callbackObj = NULL;
}

PerlDocumentCallbackHandler::~PerlDocumentCallbackHandler()
{
    if (callbackObj != NULL) {
	SvREFCNT_dec(callbackObj);
	callbackObj = NULL;
    }
}

PerlDocumentCallbackHandler::PerlDocumentCallbackHandler(SV *obj)
{
    set_callback_obj(obj);
}

// SV*
// PerlDocumentCallbackHandler::set_callback_obj(SV* object) {
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

void
PerlDocumentCallbackHandler::startElement(const XMLCh* const name, 
				  AttributeList& attributes) {
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the element name
    SV *string = XMLString2Perl(name);
    XPUSHs(string);

        // next is the attribute list
    char *class_name = "XML::Xerces::AttributeList";
    XPUSHs(sv_setref_pv(sv_newmortal(), 
			class_name, 
			(void *)&attributes));

    PUTBACK;

    perl_call_method("start_element", G_DISCARD);

    FREETMPS;
    LEAVE;
}

void
PerlDocumentCallbackHandler::endElement(const XMLCh* const name)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the element name
    SV *string = XMLString2Perl(name);
    XPUSHs(string);

    PUTBACK;

    perl_call_method("end_element", G_DISCARD);

    FREETMPS;
    LEAVE;
}

void
PerlDocumentCallbackHandler::characters(const XMLCh* const chars, 
				const unsigned int length)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the char data
    SV *string = XMLString2Perl(chars);
    XPUSHs(string);

        // next is the length
    XPUSHs(sv_2mortal(newSViv(length)));

    PUTBACK;

    perl_call_method("characters", G_DISCARD);

    FREETMPS;
    LEAVE;
}
void
PerlDocumentCallbackHandler::ignorableWhitespace(const XMLCh* const chars, 
						 const unsigned int length)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the element name
    SV *string = XMLString2Perl(chars);
    XPUSHs(string);

        // next is the length
    XPUSHs(sv_2mortal(newSViv(length)));

    PUTBACK;

    perl_call_method("ignorable_whitespace", G_DISCARD);

    FREETMPS;
    LEAVE;
}

void
PerlDocumentCallbackHandler::resetDocument(void)
{
    return;
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

    PUTBACK;

    perl_call_method("reset_document", G_DISCARD);

    FREETMPS;
    LEAVE;
}

void
PerlDocumentCallbackHandler::startDocument(void)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

    PUTBACK;

    perl_call_method("start_document", G_DISCARD);

    FREETMPS;
    LEAVE;
}

void
PerlDocumentCallbackHandler::endDocument(void)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

    PUTBACK;

    perl_call_method("end_document", G_DISCARD);

    FREETMPS;
    LEAVE;
}


void
PerlDocumentCallbackHandler::processingInstruction(const XMLCh* const target,
						   const XMLCh* const data)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the target
    SV *string1 = XMLString2Perl(target);
    XPUSHs(string1);

        // the next argument is the data
    SV *string2 = XMLString2Perl(data);
    XPUSHs(string2);

    PUTBACK;

    perl_call_method("processing_instruction", G_DISCARD);

    FREETMPS;
    LEAVE;
}

void
PerlDocumentCallbackHandler::setDocumentLocator(const Locator* const locator)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // next is the attribute list
    char *class_name = "XML::Xerces::Locator";
    XPUSHs(sv_setref_pv(sv_newmortal(), 
			class_name, 
			(void *)locator));

    PUTBACK;

    perl_call_method("set_document_locator", G_DISCARD);

    FREETMPS;
    LEAVE;
}

