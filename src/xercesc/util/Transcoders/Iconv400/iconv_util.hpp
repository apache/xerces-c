/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/**
 * $Log$
 * Revision 1.2  2002/11/04 15:14:33  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:36  peiyongz
 * sane_include
 *
 * Revision 1.1  2001/06/25 16:20:08  tng
 * Rename iconv_util.h to iconv_util.hpp.  AS400 changes by Linda Swan.
 *
 * Revision 1.1  2000/02/10 18:08:28  abagchi
 * Initial checkin
 *
 */


const char* iconv_getDefaultCodepage(void);


#include "utypes.h"
#include <iconv.h>

extern "C" int QlgCvtTextDescToDesc (int, int, char *, int, char*, int, int);

#define MAX_CONVERTER_NAME_LENGTH 60
#define MAX_SUBCHAR_LEN 4
#define ERROR_BUFFER_LENGTH 20
typedef enum {UNKNOWN = -1, IBM = 0}
UCNV_PLATFORM;
typedef enum
{
  UNSUPPORTED_CONVERTER = -1,
  SBCS = 0,
  DBCS = 1,
  MBCS = 2,
  LATIN_1 = 3,
  UTF8 = 4,
  UTF16_BigEndian = 5,
  UTF16_LittleEndian = 6,
  EBCDIC_STATEFUL = 7,
  ISO_2022 = 8,
  JIS = 9,
  EUC = 10,
  GB = 11
}
UCNV_TYPE;
int32_t
u_strlen(const UChar *s);

/* note sure if needed -  think that this is needed in cnv.c */
char* u_austrcpy(char *s1,
         const UChar *ucs2 );

/*Defines the struct of a UConverterSharedData the immutable, shared part of
 *UConverter
 */
typedef struct
  {
    uint32_t referenceCounter;	/*used to count number of clients */
    char name[MAX_CONVERTER_NAME_LENGTH];	/*internal name of the converter */
    UCNV_PLATFORM platform;	/*platform of the converter (only IBM now */
    int32_t codepage;		/*codepage # (now IBM-$codepage) */
    UCNV_TYPE conversionType;	/*conversion type */
    int8_t minBytesPerChar;	/*Minimum # bytes per char in this codepage */
    int8_t maxBytesPerChar;	/*Maximum # bytes per char in this codepage */

    struct
      {				/*initial values of some members of the mutable part of object */

	int8_t subCharLen;
	unsigned char subChar[MAX_SUBCHAR_LEN];
      } defaultConverterValues ;

    iconv_t toiconv_handle ;     /* handle to convert to unicode*/
    iconv_t fromiconv_handle;   /* handle to convert from unicode*/
  }
UConverterSharedData;


/*Defines a UConverter, the lightweight mutable part the user sees */
struct UConverter
  {



    int8_t pad;
    int32_t mode;
    int8_t subCharLen;		/*length of the codepage specific character sequence */
    unsigned char subChar[MAX_SUBCHAR_LEN];	/*codepage specific character sequence */


    UConverterSharedData *sharedData;	/*Pointer to the shared immutable part of the
					 *converter object
					 */


  };

typedef struct UConverter UConverter;

UConverter* createNewConverter(const char *name, UErrorCode *err);

/*Initializes the mutable lightweight portion of the object
 *By copying data from UConverter->sharedData->defaultConverter
 */
static void   initializeDataConverter (UConverter * myUConverter);
UConverter *createConverter (const char *converterName, UErrorCode * err);

XERCES_CPP_NAMESPACE_END
