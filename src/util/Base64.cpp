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
 * Revision 1.1  2001/05/16 15:25:36  tng
 * Schema: Add Base64 and HexBin.  By Pei Yong Zhang.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/Base64.hpp>
#include <util/XMLString.hpp>
#include <util/Janitor.hpp>
#include <util/PlatformUtils.hpp>
#include <util/TransService.hpp>

// ---------------------------------------------------------------------------
//  constants
// ---------------------------------------------------------------------------
static const int BASELENGTH = 255;       
static const int FOURBYTE   = 4;

// ---------------------------------------------------------------------------
//  class data member
// ---------------------------------------------------------------------------
bool Base64::isInitialized = false;
XMLCh Base64::base64Alphabet[BASELENGTH]; 

//
// delete the buffer allocated by decode() if
// decoding is successfully done.
//
int Base64::getDataLength(const XMLCh* const base64Data)
{
    XMLCh* decodedData = decode(base64Data);
    if ( !decodedData )
        return -1;

    int base64Len = XMLString::stringLen(decodedData);
    delete[] decodedData;
    return base64Len;
}

//
// return 0(null) if invalid data found.
// return the buffer containning decoded data otherwise
// the caller is responsible for the de-allocation of the
// buffer returned. 
//
// temporary data, normalizedBase64Data, is ALWAYS released by this function.
//
XMLCh* Base64::decode(const XMLCh* const base64Data)
{
    if (!isInitialized)
        init();

    if (( base64Data == 0 ) || ( *base64Data == 0 ))
        return 0;

    //
    // remove whitespaces from the base64Data
    //
    XMLCh* normalizedBase64Data = XMLString::replicate(base64Data);
    XMLString::trim(normalizedBase64Data);
    ArrayJanitor<XMLCh> jan(normalizedBase64Data);

    //
    // check the length: should be divisible by four
    //
    int strLen = XMLString::stringLen(normalizedBase64Data);
    if (strLen%FOURBYTE != 0) 
        return 0;  

    int numberQuadruple = strLen/FOURBYTE;
    if (numberQuadruple == 0)
        return 0;

    //
    // convert the quadruplet(s) to triplet(s)
    //
    XMLCh    d1, d2, d3, d4;
    XMLCh    b1, b2, b3, b4;

    int i = 0;
    int encodedIndex = 0;
    int dataIndex    = 0;
    XMLCh  *decodedData = new XMLCh[numberQuadruple*3+1];

    //
    // Prcess all quadruple(s) except for the last
    //
    for (; i < numberQuadruple-1; i++) 
    {
        if  (!isData( (d1 = normalizedBase64Data[dataIndex++]) )||  
             !isData( (d2 = normalizedBase64Data[dataIndex++]) )||
             !isData( (d3 = normalizedBase64Data[dataIndex++]) )||
             !isData( (d4 = normalizedBase64Data[dataIndex++]) ))
        {
            delete[] decodedData;
            return 0;//if found "no data" just return null
        }

        b1 = base64Alphabet[d1]; 
        b2 = base64Alphabet[d2];
        b3 = base64Alphabet[d3];
        b4 = base64Alphabet[d4];

        decodedData[encodedIndex++] = set1stOctect(b1, b2);
        decodedData[encodedIndex++] = set2ndOctect(b2, b3);
        decodedData[encodedIndex++] = set3rdOctect(b3, b4);
    }

    //
    // process the last Quadruple
    // the first two octets
    if (!isData( (d1 = normalizedBase64Data[dataIndex++]) ) ||  
        !isData( (d2 = normalizedBase64Data[dataIndex++]) )) 
    {
        delete[] decodedData;
        return 0;//if found "no data" just return null
    }

    b1 = base64Alphabet[d1]; 
    b2 = base64Alphabet[d2];

    // the last two octets
    d3 = normalizedBase64Data[dataIndex++];
    d4 = normalizedBase64Data[dataIndex++];

    if (!isData( d3 ) || !isData( d4 )) 
    {
        //Check if they are PAD characters
        if (isPad( d3 ) && isPad( d4 )) 
        {
            //Two PAD e.g. 3c[Pad][Pad]
            if ((b2 & 0xf) != 0)//last 4 bits should be zero
            {
                delete[] decodedData;
                return 0;
            }

            decodedData[encodedIndex++] = set1stOctect(b1, b2);
            decodedData[encodedIndex] = 0;            
        }
        else if (!isPad( d3) && isPad(d4)) 
        {               
            //One PAD  e.g. 3cQ[Pad]
            b3 = base64Alphabet[ d3 ];
            if ((b3 & 0x3) != 0)//last 2 bits should be zero
            {
                delete[] decodedData;
                return 0;
            }

            decodedData[encodedIndex++] = set1stOctect(b1, b2);
            decodedData[encodedIndex++] = set2ndOctect(b2, b3);
            decodedData[encodedIndex] = 0;            
        } 
        else 
        {
            delete[] decodedData;
            return 0;//an error  like "3c[Pad]r", "3cdX", "3cXd", "3cXX" where X is non data 
        }       
    } 
    else 
    { //No PAD e.g 3cQl
        b3 = base64Alphabet[d3];
        b4 = base64Alphabet[d4];
        decodedData[encodedIndex++] = set1stOctect(b1, b2);
        decodedData[encodedIndex++] = set2ndOctect(b2, b3);
        decodedData[encodedIndex++] = set3rdOctect(b3, b4); 
        decodedData[encodedIndex] = 0; 
    }

    return decodedData;
}

// -----------------------------------------------------------------------
//  Helper methods
// -----------------------------------------------------------------------
void Base64::init()
{
    if (isInitialized)
        return;

    int i;
    // [0] = -1, [255] = -1
    for ( i = 0; i < BASELENGTH; i++ ) 
        base64Alphabet[i] = (XMLCh)-1; 

    // [65] = 0, [90] = 25
    for ( i = chLatin_Z; i >= chLatin_A; i-- ) 
        base64Alphabet[i] = (XMLCh)( i - chLatin_A );
    
    // [97] = 0 + 26 = 26, [122] = 25 + 26 = 51 
    for ( i = chLatin_z; i >= chLatin_a; i-- )
        base64Alphabet[i] = (XMLCh)( i - chLatin_a + 26 );

    // [48] = 0 + 52, [57] = 9 + 52
    for ( i = chDigit_9; i >= chDigit_0; i-- ) 
        base64Alphabet[i] = (XMLCh)( i- chDigit_0 + 52 );

    // [43] = 62
    // [47] = 63
    base64Alphabet[chPlus] = (XMLCh) 62; 
    base64Alphabet[chForwardSlash] = (XMLCh) 63;

    isInitialized=true;

    //
    // index:  0       42 43         47  48       57     65          90       97         122       254
    // char:              '+'        '/' '0'      '9'    'A'         'Z'      'a'        'z'
    // value: -1 ...  -1  62 -1...   63  52       61      0          25  -1.. 26         51  -1...
    //
}

inline bool Base64::isData(const XMLCh& octect) 
{
    // sanity check to avoid out-of-bound index
    if (( octect >= BASELENGTH ) || ( octect < 0 ))
        return false;

    return( base64Alphabet[octect] != (XMLCh) -1);
}