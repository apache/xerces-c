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
#include "PerlErrorCallbackHandler.hpp"

PerlErrorCallbackHandler::PerlErrorCallbackHandler()
{
    callbackObj = NULL;
}

PerlErrorCallbackHandler::~PerlErrorCallbackHandler()
{
    if (callbackObj != NULL) {
	SvREFCNT_dec(callbackObj);
	callbackObj = NULL;
    }
}

PerlErrorCallbackHandler::PerlErrorCallbackHandler(SV *obj)
{
    set_callback_obj(obj);
}

// SV*
// PerlErrorCallbackHandler::set_callback_obj(SV* object) {
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
PerlErrorCallbackHandler::warning(const SAXParseException& exception) {

//    printf("in error: warning"); 
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);

	// first put the callback object on the stack
    XPUSHs(callbackObj);

	// then put the exception on the stack
    char *class_name = "XML::Xerces::SAXParseException";
    XPUSHs(sv_setref_pv(sv_newmortal(), 
			class_name, 
			(void *)&exception));

    PUTBACK;

    perl_call_method("warning", G_VOID);

    FREETMPS;
    LEAVE;
}

void
PerlErrorCallbackHandler::error(const SAXParseException& exception) {

//    printf("in error: error"); 
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

	// then put the exception on the stack
    char *class_name = "XML::Xerces::SAXParseException";
    XPUSHs(sv_setref_pv(sv_newmortal(), 
			class_name, 
			(void *)&exception));
    PUTBACK;

    perl_call_method("error", G_VOID);

    FREETMPS;
    LEAVE;
}

void
PerlErrorCallbackHandler::fatalError(const SAXParseException& exception) {
//    printf("in error: fatal_error"); 
    if (!callbackObj) {
	die("Received FatalError and no ErrorHandler was set");
    }
//    printf("<FatalError: for this: 0x%.4X>\n", this);
//    printf("<FatalError: using callback object 0x%.4X>\n", callbackObj);

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);

	// first put the callback object on the stack
    XPUSHs(callbackObj);

	// then put the exception on the stack
    char *class_name = "XML::Xerces::SAXParseException";
    XPUSHs(sv_setref_pv(sv_newmortal(), 
			class_name, 
			(void *)&exception));
    PUTBACK;

    perl_call_method("fatal_error", G_VOID);

    FREETMPS;
    LEAVE;
}


void
PerlErrorCallbackHandler::resetErrors(void) 
{
//    printf("in error: reset_errors"); 
    if (!callbackObj) return;

    dSP;

    PUSHMARK(SP);

	// first put the callback object on the stack
    XPUSHs(callbackObj);

    PUTBACK;

    perl_call_method("reset_errors", G_VOID);
}
