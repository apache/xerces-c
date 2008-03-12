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

#include <stdlib.h>
#include "PerlSAXCallbackHandler.hpp"

XERCES_CPP_NAMESPACE_USE

PerlSAXCallbackHandler::PerlSAXCallbackHandler()
{
    callbackObj = NULL;
}

PerlSAXCallbackHandler::~PerlSAXCallbackHandler()
{}

PerlSAXCallbackHandler::PerlSAXCallbackHandler(SV *obj)
{
  set_callback_obj(obj);
}

void
PerlSAXCallbackHandler::startElement(const   XMLCh* const    uri,
					 const   XMLCh* const    localname,
					 const   XMLCh* const    qname,
					 const   Attributes&     attrs) 
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the uri
    SV *string1 = UTF8_TRANSCODER->XMLString2Local(uri);
    XPUSHs(string1);

        // the next argument is the localname
    SV *string2 = UTF8_TRANSCODER->XMLString2Local(localname);
    XPUSHs(string2);

        // the next argument is the qname
    SV *string3 = UTF8_TRANSCODER->XMLString2Local(qname);
    XPUSHs(string3);

        // next is the attributes
    char *class_name = "XML::Xerces::Attributes";
    XPUSHs(sv_setref_pv(sv_newmortal(), 
			class_name, 
			(void *)&attrs));
    PUTBACK;

    perl_call_method("startElement", G_VOID);

    FREETMPS;
    LEAVE;
}

void
PerlSAXCallbackHandler::endElement(const   XMLCh* const    uri,
				       const   XMLCh* const    localname,
				       const   XMLCh* const    qname)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the uri
    SV *string1 = UTF8_TRANSCODER->XMLString2Local(uri);
    XPUSHs(string1);

        // the next argument is the localname
    SV *string2 = UTF8_TRANSCODER->XMLString2Local(localname);
    XPUSHs(string2);

        // the next argument is the qname
    SV *string3 = UTF8_TRANSCODER->XMLString2Local(qname);
    XPUSHs(string3);

    PUTBACK;

    perl_call_method("endElement", G_VOID);

    FREETMPS;
    LEAVE;
}

void
PerlSAXCallbackHandler::startElement(const   XMLCh* const    localname,
				     AttributeList&     attrs) 
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the localname
    SV *string2 = UTF8_TRANSCODER->XMLString2Local(localname);
    XPUSHs(string2);

        // next is the attributes
    char *class_name = "XML::Xerces::AttributeList";
    XPUSHs(sv_setref_pv(sv_newmortal(), 
			class_name, 
			(void *)&attrs));
    PUTBACK;

    perl_call_method("startElement", G_VOID);

    FREETMPS;
    LEAVE;
}

void
PerlSAXCallbackHandler::endElement(const   XMLCh* const    localname)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the localname
    SV *string2 = UTF8_TRANSCODER->XMLString2Local(localname);
    XPUSHs(string2);

    PUTBACK;

    perl_call_method("endElement", G_VOID);

    FREETMPS;
    LEAVE;
}

void
PerlSAXCallbackHandler::characters(const XMLCh* const chars, 
				const XMLSize_t length)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the char data
    SV *string = UTF8_TRANSCODER->XMLString2Local(chars);
    XPUSHs(string);

        // next is the length
    XPUSHs(sv_2mortal(newSViv(length)));

    PUTBACK;

    perl_call_method("characters", G_VOID);

    FREETMPS;
    LEAVE;
}
void
PerlSAXCallbackHandler::ignorableWhitespace(const XMLCh* const chars, 
						 const XMLSize_t length)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the char data
    SV *string = UTF8_TRANSCODER->XMLString2Local(chars);
    XPUSHs(string);

        // next is the length
    XPUSHs(sv_2mortal(newSViv(length)));

    PUTBACK;

    perl_call_method("ignorableWhitespace", G_VOID);

    FREETMPS;
    LEAVE;
}

void
PerlSAXCallbackHandler::resetDocument(void)
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

    perl_call_method("resetDocument", G_VOID);

    FREETMPS;
    LEAVE;
}

void
PerlSAXCallbackHandler::startDocument(void)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

    PUTBACK;

    perl_call_method("startDocument", G_VOID);

    FREETMPS;
    LEAVE;
}

void
PerlSAXCallbackHandler::endDocument(void)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

    PUTBACK;

    perl_call_method("endDocument", G_VOID);

    FREETMPS;
    LEAVE;
}


void
PerlSAXCallbackHandler::processingInstruction(const XMLCh* const target,
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
    SV *string1 = UTF8_TRANSCODER->XMLString2Local(target);
    XPUSHs(string1);

        // the next argument is the data
    SV *string2 = UTF8_TRANSCODER->XMLString2Local(data);
    XPUSHs(string2);

    PUTBACK;

    perl_call_method("processingInstruction", G_VOID);

    FREETMPS;
    LEAVE;
}

void
PerlSAXCallbackHandler::setDocumentLocator(const Locator* const locator)
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

    perl_call_method("setDocumentLocator", G_VOID);

    FREETMPS;
    LEAVE;
}

void
PerlSAXCallbackHandler::startPrefixMapping (const XMLCh* const prefix,
						const XMLCh* const uri)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the prefix
    SV *string1 = UTF8_TRANSCODER->XMLString2Local(prefix);
    XPUSHs(string1);

        // the next argument is the uri
    SV *string2 = UTF8_TRANSCODER->XMLString2Local(uri);
    XPUSHs(string2);

    PUTBACK;

    perl_call_method("startPrefixMapping", G_VOID);

    FREETMPS;
    LEAVE;
}

void
PerlSAXCallbackHandler::endPrefixMapping (const XMLCh* const prefix)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the prefix
    SV *string1 = UTF8_TRANSCODER->XMLString2Local(prefix);
    XPUSHs(string1);

    PUTBACK;

    perl_call_method("endPrefixMapping", G_VOID);

    FREETMPS;
    LEAVE;
}

void
PerlSAXCallbackHandler::skippedEntity (const XMLCh* const name)
{
    if (!callbackObj) return;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the next argument is the name
    SV *string1 = UTF8_TRANSCODER->XMLString2Local(name);
    XPUSHs(string1);

    PUTBACK;

    perl_call_method("skippedEntity", G_VOID);

    FREETMPS;
    LEAVE;
}
