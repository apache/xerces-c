/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
 *  $Log$
 *  Revision 1.1  1999/11/09 01:08:37  twl
 *  Initial revision
 *
 *  Revision 1.2  1999/11/08 20:44:40  rahul
 *  Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/RuntimeException.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUni.hpp>
#include <framework/XMLRecognizer.hpp>
#include <memory.h>
#include <string.h>


// ---------------------------------------------------------------------------
//  Local data
//
//  gEncodingNameMap
//      This array maps the Encodings enum values to their canonical names.
//      Be sure to keep this in sync with that enum!
//
//  gEBCDICPre
//      The byte sequence prefix for a legal EBCDIC encoded file. This tells
//      enough to let us read the XMLDecl in EBCDIC and get the real encoding
//      string out.
//
//  gUCS4XXX
//      The byte sequence prefixes for a legal UCS encoded file. If we get
//      one of these, we can read the decl line in UCS and get the actual
//      encoding.
//
//  gUTF16XXX
//      These are the byte sequences that a legal UTF-16 (without BOM) file
//      can start with. The BOM is checked for, but if not found we still
//      will try for this sequence. Once we hit one of these, we can read
//      the first line in UTF-6 and get the real encoding out.
// ---------------------------------------------------------------------------
static const XMLCh* gEncodingNameMap[XMLRecognizer::Encodings_Count] =
{
    XMLUni::fgEBCDICEncodingString
    , XMLUni::fgUCS4BEncodingString
    , XMLUni::fgUCS4LEncodingString
    , XMLUni::fgUSASCIIEncodingString
    , XMLUni::fgUTF8EncodingString
    , XMLUni::fgUTF16BEncodingString
    , XMLUni::fgUTF16LEncodingString
};
static const XMLByte    gEBCDICPre[]    = { 0x4C, 0x6F, 0xA7, 0x94, 0x93, 0x40 };
static const XMLByte    gUCS4BPre[]     = { 0x00, 0x00, 0x00, 0x3C };
static const XMLByte    gUCS4LPre[]     = { 0x3C, 0x00, 0x00, 0x00 };
static const XMLByte    gUTF16BPre[]    = { 0x00, 0x3C, 0x00, 0x3F };
static const XMLByte    gUTF16LPre[]    = { 0x3C, 0x00, 0x3F, 0x00 };
static const char       gXMLDecl_ASCII[]= { 0x3C, 0x3F, 0x78, 0x6D, 0x6C };




// ---------------------------------------------------------------------------
//  XMLRecognizer: Encoding recognition methods
// ---------------------------------------------------------------------------
XMLRecognizer::Encodings
XMLRecognizer::basicEncodingProbe(  const   XMLByte* const  rawBuffer
                                    , const unsigned int    rawByteCount)
{
    //
    //  As an optimization to check the 90% case, check first for the ASCII
    //  sequence '<?xml', which means its either US-ASCII, UTF-8, or some
    //  other encoding that we don't do manually but which happens to share
    //  the US-ASCII code points for these characters. So just return UTF-8
    //  to get us through the first line.
    //
    if (rawByteCount > 5)
    {
        if (!memcmp(rawBuffer, gXMLDecl_ASCII, 5))
            return UTF_8;
    }

    //
    //  If the count of raw bytes is less than 2, it cannot be anything
    //  we understand, so return UTF-8 as a fallback.
    //
    if (rawByteCount < 2)
        return UTF_8;

    //
    //  We know its at least two bytes, so lets check for a UTF-16 BOM. That
    //  is quick to check and enough to identify two major encodings.
    //
    if ((rawBuffer[0] == 0xFE) && (rawBuffer[1] == 0xFF))
        return UTF_16B;
    else if ((rawBuffer[0] == 0xFF) && (rawBuffer[1] == 0xFE))
        return UTF_16L;

    //
    //  Oh well, not one of those. So now lets see if we have at least 4
    //  bytes. If not, then we are out of ideas and can return UTF-8 as the
    //  fallback.
    //
    if (rawByteCount < 4)
        return UTF_8;

    //
    //  We have at least 4 bytes. So lets check the 4 byte sequences that
    //  indicate other UTF-16 and UCS encodings.
    //
    if ((rawBuffer[0] == 0x00) || (rawBuffer[0] == 0x3C))
    {
        if (!memcmp(rawBuffer, gUCS4BPre, 4))
            return UCS_4B;
        else if (!memcmp(rawBuffer, gUCS4LPre, 4))
            return UCS_4L;
        else if (!memcmp(rawBuffer, gUTF16BPre, 4))
            return UTF_16B;
        else if (!memcmp(rawBuffer, gUTF16LPre, 4))
            return UTF_16L;
    }

    //
    //  See if we have enough bytes to possibly match the EBCDIC prefix.
    //  If so, try it.
    //
    if (rawByteCount > 5)
    {
        if (!memcmp(rawBuffer, gEBCDICPre, 5))
            return EBCDIC;
    }

    //
    //  Does not seem to be anything we know, so go with UTF-8 to get at
    //  least through the first line and see what it really is.
    //
    return UTF_8;
}


XMLRecognizer::Encodings
XMLRecognizer::encodingForName(const XMLCh* const encName)
{
    //
    //  Compare the passed string, case insensitively, to the variations
    //  that we recognize.
    //
    //  !!NOTE: Note that we don't handle EBCDIC here because we don't handle
    //  that one ourselves. It is allowed to fall into 'other'.
    //
    if (!XMLString::compareIString(encName, XMLUni::fgUTF8EncodingString)
    ||  !XMLString::compareIString(encName, XMLUni::fgUTF8EncodingString2))
    {
        return XMLRecognizer::UTF_8;
    }
     else if (!XMLString::compareIString(encName, XMLUni::fgUSASCIIEncodingString)
          ||  !XMLString::compareIString(encName, XMLUni::fgUSASCIIEncodingString2)
          ||  !XMLString::compareIString(encName, XMLUni::fgUSASCIIEncodingString3)
          ||  !XMLString::compareIString(encName, XMLUni::fgUSASCIIEncodingString4))
    {
        return XMLRecognizer::US_ASCII;
    }
     else if (!XMLString::compareIString(encName, XMLUni::fgUTF16LEncodingString)
          ||  !XMLString::compareIString(encName, XMLUni::fgUTF16LEncodingString2))
    {
        return XMLRecognizer::UTF_16L;
    }
     else if (!XMLString::compareIString(encName, XMLUni::fgUTF16BEncodingString)
          ||  !XMLString::compareIString(encName, XMLUni::fgUTF16BEncodingString2))
    {
        return XMLRecognizer::UTF_16B;
    }
     else if (!XMLString::compareIString(encName, XMLUni::fgUCS4LEncodingString)
          ||  !XMLString::compareIString(encName, XMLUni::fgUCS4LEncodingString2))
    {
        return XMLRecognizer::UCS_4L;
    }
     else if (!XMLString::compareIString(encName, XMLUni::fgUCS4BEncodingString)
          ||  !XMLString::compareIString(encName, XMLUni::fgUCS4BEncodingString2))
    {
        return XMLRecognizer::UCS_4B;
    }

    // Return 'other' since we don't recognizer it
    return XMLRecognizer::OtherEncoding;
}


const XMLCh*
XMLRecognizer::nameForEncoding(const XMLRecognizer::Encodings theEncoding)
{
    if (theEncoding > Encodings_Count)
        ThrowXML(RuntimeException, XML4CExcepts::XMLRec_UnknownEncoding);

    return gEncodingNameMap[theEncoding];
}
