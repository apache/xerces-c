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
#include "PerlCallbackHandler.hpp"

PerlCallbackHandler::PerlCallbackHandler() {
  // fprintf(stderr,"PerlCallback: constructor\n");
  UTF8_TRANSCODER = Transcoder::getInstance();
  callbackObj = NULL;
}

PerlCallbackHandler::~PerlCallbackHandler() {
  if (callbackObj) {
    SvREFCNT_dec(callbackObj); 
    callbackObj = NULL;
  }
  if (UTF8_TRANSCODER) {
    UTF8_TRANSCODER = NULL;
  }
  // fprintf(stderr,"PerlCallback: destructor\n");
}

PerlCallbackHandler::PerlCallbackHandler(SV* object) 
{
  set_callback_obj(object);
}

PerlCallbackHandler::PerlCallbackHandler(PerlCallbackHandler* handler) {
    SvREFCNT_inc(callbackObj);
    handler->callbackObj = callbackObj;
//     printf("<copy constructor for obj: 0x%.4X, new obj: 0x%.4X>\n", this, handler);
}

SV*
PerlCallbackHandler::set_callback_obj(SV* object) {
    SV *oldRef = &PL_sv_undef;	// default to 'undef'
//    printf("<setting callback object for this: 0x%.4X>\n", this);
    if (callbackObj != NULL) {
	oldRef = callbackObj;
//	printf("<old callback object 0x%.4X>\n", callbackObj);
//	SvREFCNT_dec(oldRef);
    }
    SvREFCNT_inc(object);
//    printf("<setting callback object 0x%.4X>\n", object);
    callbackObj = object;
//    printf("<new callback object 0x%.4X>\n", callbackObj);
    return oldRef;
}
