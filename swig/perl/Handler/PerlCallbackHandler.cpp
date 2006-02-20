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

XMLTranscoder* UTF8_TRANSCODER  = NULL;

PerlCallbackHandler::PerlCallbackHandler() {
  if (UTF8_TRANSCODER == NULL)
  {
    XMLTransService::Codes failReason;
    XMLCh* UTF8_ENCODING = XMLString::transcode("UTF-8");
    UTF8_TRANSCODER =
      XMLPlatformUtils::fgTransService->makeNewTranscoderFor(UTF8_ENCODING,
							     failReason,
							     1024,
							     XMLPlatformUtils::fgMemoryManager);
    if (UTF8_TRANSCODER == NULL) {
      croak("ERROR: PerlCallbackHandler Could not create UTF-8 transcoder");
    }
  }
   callbackObj = NULL;
//    printf("PerlCallback: constructor");
}

PerlCallbackHandler::~PerlCallbackHandler() {
     if (callbackObj) {
 	SvREFCNT_dec(callbackObj); 
 	callbackObj = NULL;
     }
//    printf("PerlCallback: destructor");
}

PerlCallbackHandler::PerlCallbackHandler(SV* object) 
        : callbackObj(NULL)
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

SV*
PerlCallbackHandler::XMLString2Perl(const XMLCh* input) {
    SV *output;
  unsigned int charsEaten = 0;
  int length  = XMLString::stringLen(input);            // string length
  // use +1 to make room for the '\0' at the end of the string
  // in the pathological case when each character of the string 
  // is UTF8_MAXLEN bytes long
  XMLByte* res = new XMLByte[(length * UTF8_MAXLEN) + 1]; // output string

  unsigned int total_chars =
    UTF8_TRANSCODER->transcodeTo((const XMLCh*) input, 
				   (unsigned int) length,
				   (XMLByte*) res,
				   (unsigned int) (length*UTF8_MAXLEN),
				   charsEaten,
				   XMLTranscoder::UnRep_Throw
				   );
  res[total_chars] = '\0';

  output = sv_newmortal();
  sv_setpv((SV*)output, (char *)res );
  SvUTF8_on((SV*)output);
  delete[] res;
  return output;
}
