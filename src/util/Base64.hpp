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
 * $Id$
 */

#ifndef BASE64_HPP
#define BASE64_HPP

#include <util/XercesDefs.hpp>
#include <util/XMLUniDefs.hpp>

//
// This class provides encode/decode for RFC 2045 Base64 as
// defined by RFC 2045, N. Freed and N. Borenstein.
// RFC 2045: Multipurpose Internet Mail Extensions (MIME)
// Part One: Format of Internet Message Bodies. Reference
// 1996 Available at: http://www.ietf.org/rfc/rfc2045.txt
// This class is used by XML Schema binary format validation
//  
//
class XMLUTIL_EXPORT Base64
{
public :

    //@{

    /**
     * Encodes octets into Base64 data
     * 
     * @param inputData Byte array containing binary data.
     * @param inputLength Length of the input array.
     * @param outputLength Pointer to variable, where will be
     *     stored length of returned data.
     * @return Byte array containing encoded Base64 data,
     *     or NULL if input data can not be encoded.
     */
    static XMLCh* encode (
        const XMLCh* const inputData,
        const int inputLength,
        int *outputLength = 0 );

    /**
     * Get data length
     * returns length of decoded data given an array 
     * containing encoded data.
     *
     * @param inputData Byte array containing Base64 data.
     * @return Length of decoded data, or -1 if input data
     *     can not be decoded.
     */
    static int getDataLength(
        const XMLCh* const inputData );

    /**
     * Decodes Base64 data into octets
     * 
     * @param inputData Byte array containing Base64 data.
     * @param outputLength Reference to variable, where will be
     *     stored length of returned data.
     * @return Byte array containing decoded binary data, or 
     *     NULL if input data can not be decoded.
     */
    static XMLCh* decode(
        const XMLCh* const inputData,
        int& outputLength );

    //@}

private :

    // -----------------------------------------------------------------------
    //  Helper methods
    // -----------------------------------------------------------------------

    static void init();

    static bool isData(const XMLCh& octet);
    static bool isPad(const XMLCh& octet);

    static XMLCh set1stOctet(const XMLCh&, const XMLCh&);
    static XMLCh set2ndOctet(const XMLCh&, const XMLCh&);
    static XMLCh set3rdOctet(const XMLCh&, const XMLCh&);

    static void split1stOctet(const XMLCh&, XMLCh&, XMLCh&);
    static void split2ndOctet(const XMLCh&, XMLCh&, XMLCh&);
    static void split3rdOctet(const XMLCh&, XMLCh&, XMLCh&);

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    Base64();
    Base64(const Base64&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  base64Alphabet
    //     The Base64 alphabet (see RFC 2045).
    //
    //  base64Padding
    //     Padding character (see RFC 2045).
    //
    //  base64Inverse
    //     Table used in decoding base64.
    //
    //  isInitialized
    //     Set once base64Inverse is initalized.
    //
    //  quadsPerLine
    //     Number of quadruplets per one line. The encoded output
    //     stream must be represented in lines of no more 
    //     than 19 quadruplets each.
    //
    // -----------------------------------------------------------------------

    static const XMLCh  base64Alphabet[];
    static const XMLCh  base64Padding;

    static XMLCh  base64Inverse[];
    static bool  isInitialized;

    static const unsigned int  quadsPerLine;
};

// -----------------------------------------------------------------------
//  Helper methods
// -----------------------------------------------------------------------
inline bool Base64::isPad(const XMLCh& octet) 
{
    return ( octet == base64Padding );
}

inline XMLCh Base64::set1stOctet(const XMLCh& b1, const XMLCh& b2)
{
    return (( b1 << 2 ) | ( b2 >> 4 ));
}

inline XMLCh Base64::set2ndOctet(const XMLCh& b2, const XMLCh& b3)
{
    return (( b2 << 4 ) | ( b3 >> 2 ));
}

inline XMLCh Base64::set3rdOctet(const XMLCh& b3, const XMLCh& b4)
{
    return (( b3 << 6 ) | b4 );
}

inline void Base64::split1stOctet(const XMLCh& ch, XMLCh& b1, XMLCh& b2) {
    b1 = ch >> 2;
    b2 = ( ch & 0x3 ) << 4;
}

inline void Base64::split2ndOctet(const XMLCh& ch, XMLCh& b2, XMLCh& b3) {
    b2 |= ch >> 4;  // combine with previous value
    b3 = ( ch & 0xf ) << 2;
}

inline void Base64::split3rdOctet(const XMLCh& ch, XMLCh& b3, XMLCh& b4) {
    b3 |= ch >> 6;  // combine with previous value 
    b4 = ( ch & 0x3f );
}

#endif
