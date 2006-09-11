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

#include "PerlNodeFilterCallbackHandler.hpp"
#include "xerces-swig-perl.hpp"

PerlNodeFilterCallbackHandler::PerlNodeFilterCallbackHandler()
{
    callbackObj = NULL;
}

PerlNodeFilterCallbackHandler::~PerlNodeFilterCallbackHandler()
{}

PerlNodeFilterCallbackHandler::PerlNodeFilterCallbackHandler(SV *obj)
{
    set_callback_obj(obj);
}

short
PerlNodeFilterCallbackHandler::acceptNode (const DOMNode* node) const
{
    if (!callbackObj) {
        croak("\nacceptNode: no NodeFilter set\n");
	return 0;
    }
    short accept = 0;
    char *domNodeName = "XML::Xerces::DOMNode";

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

    // the only argument is the node
    // god bless John Lenz's new type system in SWIG 1.3.25!!!
    swig_type_info *domNodeType = SWIG_TypeQuery(domNodeName);
    swig_type_info *ty = SWIG_TypeDynamicCast(domNodeType, (void **) &node);
    SV* node_sv = sv_newmortal();
    SWIG_MakePtr(node_sv, (void *) node, ty,0);
    XPUSHs(node_sv);

    PUTBACK;

    int count = perl_call_method("acceptNode", G_SCALAR);

    SPAGAIN ;

    if (count != 1)
	croak("NodeFilter did not return an answer\n") ;

    accept = POPi;

    PUTBACK ;
    FREETMPS;
    LEAVE;

    return accept;
}
