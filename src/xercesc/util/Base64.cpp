/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.10  2003/05/20 22:10:02  peiyongz
 * To differentiate external/internal memory
 *
 * Revision 1.8  2003/01/27 21:15:56  peiyongz
 * only zero or one space allowed in between B64 character.
 *
 * Revision 1.7  2002/12/20 22:10:20  tng
 * XML 1.1
 *
 * Revision 1.6  2002/11/25 18:14:35  peiyongz
 * Schema Errata: E2-9 Base64
 *
 * Revision 1.5  2002/11/04 15:22:03  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/03/19 17:01:20  peiyongz
 * Fix to Bug#7243 Base64 encoding is not working.
 *
 * Revision 1.3  2002/02/18 16:07:38  peiyongz
 * fix: "i" redefined on line 428 reported by compilers on some UNIX platforms
 *
 * Revision 1.2  2002/02/15 21:36:56  peiyongz
 * Interface redefined for conversion in XMLByte
 *
 * Revision 1.1.1.1  2002/02/01 22:22:09  peiyongz
 * sane_include
 *
 * Revision 1.6  2001/10/15 19:42:16  knoaman
 * Null-terminate base64Alphabet.
 *
 * Revision 1.5  2001/10/10 19:14:08  peiyongz
 * Patch from Petr Gotthard : encode() provided and some other changes
 *
 * Revision 1.4  2001/06/07 20:55:20  tng
 * Fix no newline at the end warning.  By Pei Yong Zhang.
 *
 * Revision 1.3  2001/05/28 21:11:16  tng
 * Schema: Various DatatypeValidator fix.  By Pei Yong Zhang
 *
 * Revision 1.2  2001/05/16 19:01:04  tng
 * Schema: Typo fix in Base64
 *
 * Revision 1.1  2001/05/16 15:25:36  tng
 * Schema: Add Base64 and HexBin.  By Pei Yong Zhang.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/Base64.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/internal/XMLReader.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  constants
// ---------------------------------------------------------------------------
static const int BASELENGTH = 255;
static const int FOURBYTE   = 4;

// ---------------------------------------------------------------------------
//  class data member
// ---------------------------------------------------------------------------

// the base64 alphabet according to definition in RFC 2045
const XMLByte Base64::base64Alphabet[] = {
    chLatin_A, chLatin_B, chLatin_C, chLatin_D, chLatin_E,
    chLatin_F, chLatin_G, chLatin_H, chLatin_I, chLatin_J,
    chLatin_K, chLatin_L, chLatin_M, chLatin_N, chLatin_O,
    chLatin_P, chLatin_Q, chLatin_R, chLatin_S, chLatin_T,
    chLatin_U, chLatin_V, chLatin_W, chLatin_X, chLatin_Y, chLatin_Z,
	chLatin_a, chLatin_b, chLatin_c, chLatin_d, chLatin_e,
	chLatin_f, chLatin_g, chLatin_h, chLatin_i, chLatin_j,
	chLatin_k, chLatin_l, chLatin_m, chLatin_n, chLatin_o,
	chLatin_p, chLatin_q, chLatin_r, chLatin_s, chLatin_t,
	chLatin_u, chLatin_v, chLatin_w, chLatin_x, chLatin_y, chLatin_z,
	chDigit_0, chDigit_1, chDigit_2, chDigit_3, chDigit_4,
	chDigit_5, chDigit_6, chDigit_7, chDigit_8, chDigit_9,
	chPlus, chForwardSlash, chNull
};

XMLByte Base64::base64Inverse[BASELENGTH];

const XMLByte Base64::base64Padding = chEqual;

bool Base64::isInitialized = false;

/***
 *
 * Memory Management Issue:
 *
 * . For memory allocated for result returned to caller (external memory), 
 *   the plugged memory manager is used if it is provided, otherwise global 
 *   new used to retain the pre-memory-manager behaviour.
 *
 * . For memory allocated for temperary buffer (internal memory), 
 *   XMLPlatformUtils::fgMemoryManager is used.
 *
 */

static inline void* getExternalMemory(MemoryManager* const allocator
                                    , unsigned int const   sizeToAllocate)
{
   return allocator ? allocator->allocate(sizeToAllocate)
       : ::operator new(sizeToAllocate);
}

static inline void* getInternalMemory(unsigned int const   sizeToAllocate)
{
    return XMLPlatformUtils::fgMemoryManager->allocate(sizeToAllocate);
}

/***
 * internal memory is deallocated by janitorArray
 */ 
static inline void returnExternalMemory(MemoryManager* const allocator
                                      , void*                buffer)
{
    allocator ? allocator->deallocate(buffer)
        : ::operator delete(buffer);
}

/**
 *     E2-9
 *
 *     Canonical-base64Binary ::= (B64line* B64lastline)?
 *
 *      B64line ::= B64x15 B64x15 B64x15 B64x15 B64x15 B64 #xA
 *                  76 Base64 characters followed by newline
 *
 *      B64x15  ::= B64 B64 B64 B64 B64
 *                  B64 B64 B64 B64 B64
 *                  B64 B64 B64 B64 B64
 *
 *      B64lastline ::= B64x4? B64x4? B64x4? B64x4?
 *                      B64x4? B64x4? B64x4? B64x4?
 *                      B64x4? B64x4? B64x4? B64x4?
 *                      B64x4? B64x4? B64x4? B64x4?
 *                      B64x4? B64x4?
 *                      (B64x4 | (B64 B64 B16 '=') | (B64 B04 '=='))
 *                      #xA
 *
 *      B64x4   ::= B64 B64 B64 B64
 *      B04     ::= [AQgw]
 *      B16     ::= [AEIMQUYcgkosw048]
*/

// number of quadruplets per one line ( must be >1 and <19 )
const unsigned int Base64::quadsPerLine = 15;

XMLByte* Base64::encode(const XMLByte* const inputData
                      , const unsigned int   inputLength
                      , unsigned int*        outputLength
                      , MemoryManager* const memMgr)
{
    if (!isInitialized)
        init();

    if (!inputData || !outputLength)
        return 0;

    int quadrupletCount = ( inputLength + 2 ) / 3;
    if (quadrupletCount == 0)
        return 0;

    // number of rows in encoded stream ( including the last one )
    int lineCount = ( quadrupletCount + quadsPerLine-1 ) / quadsPerLine;

    //
    // convert the triplet(s) to quadruplet(s)
    //
    XMLByte  b1, b2, b3, b4;  // base64 binary codes ( 0..63 )

    unsigned int inputIndex = 0;
    unsigned int outputIndex = 0;
    XMLByte *encodedData = (XMLByte*) getExternalMemory(memMgr, (quadrupletCount*FOURBYTE+lineCount+1) * sizeof(XMLByte));

    //
    // Process all quadruplet(s) except the last
    //
    int quad = 1;
    for (; quad <= quadrupletCount-1; quad++ )
    {
        // read triplet from the input stream
        split1stOctet( inputData[ inputIndex++ ], b1, b2 );
        split2ndOctet( inputData[ inputIndex++ ], b2, b3 );
        split3rdOctet( inputData[ inputIndex++ ], b3, b4 );

        // write quadruplet to the output stream
        encodedData[ outputIndex++ ] = base64Alphabet[ b1 ];
        encodedData[ outputIndex++ ] = base64Alphabet[ b2 ];
        encodedData[ outputIndex++ ] = base64Alphabet[ b3 ];
        encodedData[ outputIndex++ ] = base64Alphabet[ b4 ];

        if (( quad % quadsPerLine ) == 0 )
            encodedData[ outputIndex++ ] = chLF;
    }

    //
    // process the last Quadruplet
    //
    // first octet is present always, process it
    split1stOctet( inputData[ inputIndex++ ], b1, b2 );
    encodedData[ outputIndex++ ] = base64Alphabet[ b1 ];

    if( inputIndex < inputLength )
    {
        // second octet is present, process it
        split2ndOctet( inputData[ inputIndex++ ], b2, b3 );
        encodedData[ outputIndex++ ] = base64Alphabet[ b2 ];

        if( inputIndex < inputLength )
        {
            // third octet present, process it
            // no PAD e.g. 3cQl
            split3rdOctet( inputData[ inputIndex++ ], b3, b4 );
            encodedData[ outputIndex++ ] = base64Alphabet[ b3 ];
            encodedData[ outputIndex++ ] = base64Alphabet[ b4 ];
        }
        else
        {
            // third octet not present
            // one PAD e.g. 3cQ=
            encodedData[ outputIndex++ ] = base64Alphabet[ b3 ];
            encodedData[ outputIndex++ ] = base64Padding;
        }
    }
    else
    {
        // second octet not present
        // two PADs e.g. 3c==
        encodedData[ outputIndex++ ] = base64Alphabet[ b2 ];
        encodedData[ outputIndex++ ] = base64Padding;
        encodedData[ outputIndex++ ] = base64Padding;
    }

    // write out end of the last line
    encodedData[ outputIndex++ ] = chLF;
    // write out end of string
    encodedData[ outputIndex ] = 0;

    *outputLength = outputIndex;

    return encodedData;
}

//
// delete the buffer allocated by decode() if
// decoding is successfully done.
//
// In previous version, we use XMLString::strLen(decodedData)
// to get the length, this will fail for test case containing
// consequtive "A", such "AAFF", or "ab56AA56". Instead of
// returning 3/6, we have 0 and 3, indicating that "AA", after
// decoded, is interpreted as <null> by the strLen().
//
// Since decode() has track of length of the decoded data, we
// will get this length from decode(), instead of strLen().
//
int Base64::getDataLength(const XMLCh* const inputData)
{
    unsigned int    retLen = 0;
    XMLCh* decodedData = decode(inputData, &retLen);

    if ( !decodedData )
        return -1;
    else
    {
        returnExternalMemory(0, decodedData);
        return retLen;
    }
}

//
// return 0(null) if invalid data found.
// return the buffer containning decoded data otherwise
// the caller is responsible for the de-allocation of the
// buffer returned.
//
// temporary data, rawInputData, is ALWAYS released by this function.
//

/***
 *     E2-9
 *
 *     Base64Binary ::= S? B64quartet* B64final?
 *
 *     B64quartet   ::= B64 S? B64 S? B64 S? B64 S?
 *
 *     B64final     ::= B64 S? B04 S? '=' S? '=' S?
 *                    | B64 S? B64 S? B16 S? '=' S?
 *
 *     B04          ::= [AQgw]
 *     B16          ::= [AEIMQUYcgkosw048]
 *     B64          ::= [A-Za-z0-9+/]
*/

XMLByte* Base64::decode(const XMLByte* const inputData
                      , unsigned int*        decodedLength
                      , MemoryManager* const memMgr)
{
    if (!isInitialized)
        init();

    if ((!inputData) || (!*inputData))
        return 0;

    //
    // remove all XML whitespaces from the base64Data
    //
    int inputLength = XMLString::stringLen( (const char* const)inputData );
    XMLByte* rawInputData = (XMLByte*) getInternalMemory((inputLength+1) * sizeof(XMLByte));
    ArrayJanitor<XMLByte> jan(rawInputData, XMLPlatformUtils::fgMemoryManager);

    int inputIndex = 0;
    int rawInputLength = 0;
    bool inWhiteSpace = false;

    while ( inputIndex < inputLength )
    {
        if (!XMLChar1_0::isWhitespace(inputData[inputIndex]))
        {
            rawInputData[ rawInputLength++ ] = inputData[ inputIndex ];
            inWhiteSpace = false;
        }
        else
        {
            if (inWhiteSpace)
                return 0; // more than 1 whitespaces encountered
            else
                inWhiteSpace = true;
        }

        inputIndex++;
    }
    rawInputData[ rawInputLength ] = 0;

    // the length of raw data should be divisible by four
    if (( rawInputLength % FOURBYTE ) != 0 )
        return 0;

    int quadrupletCount = rawInputLength / FOURBYTE;
    if ( quadrupletCount == 0 )
        return 0;

    //
    // convert the quadruplet(s) to triplet(s)
    //
    XMLByte  d1, d2, d3, d4;  // base64 characters
    XMLByte  b1, b2, b3, b4;  // base64 binary codes ( 0..64 )

    int rawInputIndex  = 0;
    int outputIndex    = 0;
    XMLByte *decodedData = (XMLByte*) getExternalMemory(memMgr, (quadrupletCount*3+1) * sizeof(XMLByte));

    //
    // Process all quadruplet(s) except the last
    //
    int quad = 1;
    for (; quad <= quadrupletCount-1; quad++ )
    {
        // read quadruplet from the input stream
        if (!isData( (d1 = rawInputData[ rawInputIndex++ ]) ) ||
            !isData( (d2 = rawInputData[ rawInputIndex++ ]) ) ||
            !isData( (d3 = rawInputData[ rawInputIndex++ ]) ) ||
            !isData( (d4 = rawInputData[ rawInputIndex++ ]) ))
        {
            // if found "no data" just return NULL
            returnExternalMemory(memMgr, decodedData);
            return 0;
        }

        b1 = base64Inverse[ d1 ];
        b2 = base64Inverse[ d2 ];
        b3 = base64Inverse[ d3 ];
        b4 = base64Inverse[ d4 ];

        // write triplet to the output stream
        decodedData[ outputIndex++ ] = set1stOctet(b1, b2);
        decodedData[ outputIndex++ ] = set2ndOctet(b2, b3);
        decodedData[ outputIndex++ ] = set3rdOctet(b3, b4);
    }

    //
    // process the last Quadruplet
    //
    // first two octets are present always, process them
    if (!isData( (d1 = rawInputData[ rawInputIndex++ ]) ) ||
        !isData( (d2 = rawInputData[ rawInputIndex++ ]) ))
    {
        // if found "no data" just return NULL
        returnExternalMemory(memMgr, decodedData);
        return 0;
    }

    b1 = base64Inverse[ d1 ];
    b2 = base64Inverse[ d2 ];

    // try to process last two octets
    d3 = rawInputData[ rawInputIndex++ ];
    d4 = rawInputData[ rawInputIndex++ ];

    if (!isData( d3 ) || !isData( d4 ))
    {
        // check if last two are PAD characters
        if (isPad( d3 ) && isPad( d4 ))
        {
            // two PAD e.g. 3c==
            if ((b2 & 0xf) != 0) // last 4 bits should be zero
            {
                returnExternalMemory(memMgr, decodedData);
                return 0;
            }

            decodedData[ outputIndex++ ] = set1stOctet(b1, b2);
        }
        else if (!isPad( d3 ) && isPad( d4 ))
        {
            // one PAD e.g. 3cQ=
            b3 = base64Inverse[ d3 ];
            if (( b3 & 0x3 ) != 0 ) // last 2 bits should be zero
            {
                returnExternalMemory(memMgr, decodedData);
                return 0;
            }

            decodedData[ outputIndex++ ] = set1stOctet( b1, b2 );
            decodedData[ outputIndex++ ] = set2ndOctet( b2, b3 );
        }
        else
        {
            // an error like "3c[Pad]r", "3cdX", "3cXd", "3cXX" where X is non data
            returnExternalMemory(memMgr, decodedData);
            return 0;
        }
    }
    else
    {
        // no PAD e.g 3cQl
        b3 = base64Inverse[ d3 ];
        b4 = base64Inverse[ d4 ];
        decodedData[ outputIndex++ ] = set1stOctet( b1, b2 );
        decodedData[ outputIndex++ ] = set2ndOctet( b2, b3 );
        decodedData[ outputIndex++ ] = set3rdOctet( b3, b4 );
    }

    // write out the end of string
    decodedData[ outputIndex ] = 0;
    *decodedLength = outputIndex;

    return decodedData;
}

XMLCh* Base64::decode(const XMLCh* const   inputData
                    , unsigned int*        decodedLen
                    , MemoryManager* const memMgr)
{
	if (!inputData)
		return 0;

    /***
     *  Move input data to a XMLByte buffer
     */
	unsigned int srcLen = XMLString::stringLen(inputData);
    XMLByte *dataInByte = (XMLByte*) getInternalMemory((srcLen+1) * sizeof(XMLByte));
    ArrayJanitor<XMLByte> janFill(dataInByte, XMLPlatformUtils::fgMemoryManager);

    for (unsigned int i = 0; i < srcLen; i++)
		dataInByte[i] = (XMLByte)inputData[i];

	dataInByte[srcLen] = 0;

    /***
     * Forward to the actual decoding method to do the decoding
     */
	*decodedLen = 0;
	XMLByte *DecodedBuf = decode(dataInByte, decodedLen, memMgr);

	if (!DecodedBuf)
		return 0;

    /***
     * Move decoded data to a XMLCh buffer to return
     */
    XMLCh *toRet = (XMLCh*) getExternalMemory(memMgr, (*decodedLen+1) * sizeof(XMLCh));
               
    for (unsigned int j = 0; j < *decodedLen; j++)
		toRet[j] = (XMLCh)DecodedBuf[j];

	toRet[*decodedLen] = 0;

    /***
     * Release the memory allocated in the actual decoding method
     */ 
    returnExternalMemory(memMgr, DecodedBuf);

    return toRet;

}

// -----------------------------------------------------------------------
//  Helper methods
// -----------------------------------------------------------------------
void Base64::init()
{
    if (isInitialized)
        return;

    isInitialized = true;

    // create inverse table for base64 decoding
    // if base64Alphabet[ 17 ] = 'R', then base64Inverse[ 'R' ] = 17
    // for characters not in base64Alphabet the base64Inverse[] = -1

    int i;
    // set all fields to -1
    for ( i = 0; i < BASELENGTH; i++ )
        base64Inverse[i] = (XMLByte)-1;

    // compute inverse table
    for ( i = 0; i < 64; i++ )
        base64Inverse[ base64Alphabet[i] ] = (XMLByte)i;
}

bool Base64::isData(const XMLByte& octet)
{
    return (base64Inverse[octet]!=(XMLByte)-1);
}

XERCES_CPP_NAMESPACE_END
