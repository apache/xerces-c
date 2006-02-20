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

/*
 * Perl/transcode.i - maps Xerces-C UTF16 chars to Perl's UTF8 chars
 *
 */ 

%{

SV*
XMLString2Perl(const XMLCh* input) {
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
Perl2XMLString(SV* input){
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

%}