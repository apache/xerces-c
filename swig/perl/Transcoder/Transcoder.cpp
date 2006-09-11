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
#include "Transcoder.hpp"

Transcoder* Transcoder::_instance = NULL;

Transcoder*
Transcoder::getInstance() {
  // fprintf(stderr, "getInstance: finding instance\n");
  if (_instance == NULL) {
    // fprintf(stderr, "getInstance: making new transcoder\n");
    _instance = new Transcoder();
  }
  return _instance;
}

Transcoder::~Transcoder() {
  // fprintf(stderr, "Deleting transcoder\n");
}

Transcoder::Transcoder() {
  XMLTransService::Codes failReason;

  // we assume that the Xerces-C transcoding service is already initialized
  // via XMLPlatformUtils::Initialize()
  UTF8_TRANSCODER = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(
						XMLUni::fgUTF8EncodingString,
						failReason,
						1024);

  if (UTF8_TRANSCODER == NULL) {
    croak("ERROR: Transcoder Could not create UTF-8 transcoder");
  } else if (failReason == XMLTransService::UnsupportedEncoding) {
    croak("ERROR: Transcoder: unsupported encoding");
  } else if (failReason == XMLTransService::InternalFailure) {
    croak("ERROR: Transcoder: internal failure");
  } else if (failReason == XMLTransService::SupportFilesNotFound) {
    croak("ERROR: Transcoder: support files not found");
  } else if (failReason == XMLTransService::Ok) {
    // fprintf(stderr, "Created transcoder ok\n");
  }
}

SV*
Transcoder::XMLString2Perl(const XMLCh* input) {
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

#if (0) 
  if (DEBUG_UTF8_OUT) {
      printf("Xerces out length = %d: ",total_chars);
      for (int i=0;i<length;i++){
	  printf("<0x%.4X>",res[i]);
      }
      printf("\n");
  }
#endif

  output = sv_newmortal();
  sv_setpv((SV*)output, (char *)res );
  SvUTF8_on((SV*)output);
  delete[] res;
  return output;
}

XMLCh* 
Transcoder::Perl2XMLString(SV* input){
    XMLCh* output;

    STRLEN length;
    char *ptr = (char *)SvPVutf8(input,length);

#if (0) 
    if (DEBUG_UTF8_IN) {
	printf("Perl in length = %d: ",length);
	for (unsigned int i=0;i<length;i++){
	    printf("<0x%.4X>",ptr[i]);
	}
	printf("\n");
    }
#endif

    if (SvUTF8(input)) {
	unsigned int charsEaten = 0;
        unsigned char* sizes = new unsigned char[length+1];
        output = new XMLCh[length+1];
	unsigned int chars_stored = 
	    UTF8_TRANSCODER->transcodeFrom((const XMLByte*) ptr,
					   (unsigned int) length,
					   (XMLCh*) output, 
					   (unsigned int) length,
					   charsEaten,
					   (unsigned char*)sizes
					   );
	delete [] sizes;

#if (0) 
	if (DEBUG_UTF8_IN) {
	    printf("Xerces in length = %d: ",chars_stored);
	    for (unsigned int i=0;i<chars_stored;i++){
		printf("<0x%.4X>",output[i]);
	    }
	    printf("\n");
	}
#endif

	// indicate the end of the string
	output[chars_stored] = '\0';
    } else {
	output = XMLString::transcode(ptr);

#if (0) 
	if (DEBUG_UTF8_IN) {
	    printf("Xerces: ");
	    for (int i=0;output[i];i++){
		printf("<0x%.4X>",output[i]);
	    }
	    printf("\n");
	}
#endif

    }
    return(output);
}
