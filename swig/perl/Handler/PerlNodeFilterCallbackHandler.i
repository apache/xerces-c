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

PerlNodeFilterCallbackHandler::PerlNodeFilterCallbackHandler()
{
    callbackObj = NULL;
}

PerlNodeFilterCallbackHandler::~PerlNodeFilterCallbackHandler()
{
    if (callbackObj != NULL) {
	SvREFCNT_dec(callbackObj);
	callbackObj = NULL;
    }
}

PerlNodeFilterCallbackHandler::PerlNodeFilterCallbackHandler(SV *obj)
{
    set_callback_obj(obj);
}

// SV*
// PerlNodeFilterCallbackHandler::set_callback_obj(SV* object) {
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

short
PerlNodeFilterCallbackHandler::acceptNode (const DOMNode* node) const
{
    if (!callbackObj) {
        croak("\nacceptNode: no NodeFilter set\n");
	return 0;
    }
    short accept = 0;

    dSP;

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);
	// first put the callback object on the stack
    XPUSHs(callbackObj);

        // the only argument is the node
    swig_type_info *ty = SWIG_TypeDynamicCast(SWIGTYPE_p_XERCES_CPP_NAMESPACE__DOMNode, (void **) &node);
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
