/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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

/*
 * $Log$
 * Revision 1.11  2000/03/18 00:00:04  roddey
 * Initial updates for two way transcoding support
 *
 * Revision 1.10  2000/03/07 23:45:36  roddey
 * First cut for additions to Win32 xcode. Based very loosely on a
 * prototype from Eric Ulevik.
 *
 * Revision 1.9  2000/03/02 19:55:36  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.8  2000/02/06 07:48:34  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.7  2000/01/25 23:14:19  roddey
 * Borland does not support wcsupr(), but does support _wcsupr(). Since VC++ does also,
 * _wcsupr() was used in order to have the both work with the same transcoding code.
 *
 * Revision 1.6  2000/01/25 22:49:58  roddey
 * Moved the supportsSrcOfs() method from the individual transcoder to the
 * transcoding service, where it should have been to begin with.
 *
 * Revision 1.5  2000/01/25 19:19:09  roddey
 * Simple addition of a getId() method to the xcode and netacess abstractions to
 * allow each impl to give back an id string.
 *
 * Revision 1.4  1999/12/18 00:22:33  roddey
 * Changes to support the new, completely orthagonal, transcoder architecture.
 *
 * Revision 1.3  1999/12/15 19:44:02  roddey
 * Now implements the new transcoding abstractions, with separate interface
 * classes for XML transcoders and local code page transcoders.
 *
 * Revision 1.2  1999/12/01 18:54:26  roddey
 * Small syntactical change to make it compile under Borland's compiler.
 * It does not make it worse under other compilers, so why not. Basically
 * it was just changing ::iswspace() to iswspace(), i.e. get rid of the ::
 * prefix, which freaked out Borland for some reason.
 *
 * Revision 1.1.1.1  1999/11/09 01:06:04  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:35  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/TranscodingException.hpp>
#include <util/XMLException.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUni.hpp>
#include "Win32TransService.hpp"
#include <windows.h>


// ---------------------------------------------------------------------------
//  Local, const data
// ---------------------------------------------------------------------------
static const XMLCh gMyServiceId[] =
{
    chLatin_W, chLatin_i, chLatin_n, chDigit_3, chDigit_2, chNull
};



// ---------------------------------------------------------------------------
//  Win32TransService: Constructors and Destructor
// ---------------------------------------------------------------------------
Win32TransService::Win32TransService()
{
    // Call the init method to set up our code page map
    initCPMap();
}

Win32TransService::~Win32TransService()
{
}


// ---------------------------------------------------------------------------
//  Win32TransService: The virtual transcoding service API
// ---------------------------------------------------------------------------
int Win32TransService::compareIString(  const   XMLCh* const    comp1
                                        , const XMLCh* const    comp2)
{
    return _wcsicmp(comp1, comp2);
}


int Win32TransService::compareNIString( const   XMLCh* const    comp1
                                        , const XMLCh* const    comp2
                                        , const unsigned int    maxChars)
{
    return _wcsnicmp(comp1, comp2, maxChars);
}


const XMLCh* Win32TransService::getId() const
{
    return gMyServiceId;
}


bool Win32TransService::isSpace(const XMLCh toCheck) const
{
    return (iswspace(toCheck) != 0);
}


XMLLCPTranscoder* Win32TransService::makeNewLCPTranscoder()
{
    // Just allocate a new LCP transcoder of our type
    return new Win32LCPTranscoder;
}


bool Win32TransService::supportsSrcOfs() const
{
    //
    //  Since the only mechanism we have to translate XML text in this
    //  transcoder basically require us to do work that allows us to support
    //  source offsets, we might as well do it.
    //
    return true;
}


void Win32TransService::upperCase(XMLCh* const toUpperCase) const
{
    _wcsupr(toUpperCase);
}


// ---------------------------------------------------------------------------
//  Win32Transcoder: Constructors and Destructor
// ---------------------------------------------------------------------------
Win32Transcoder::Win32Transcoder(const  XMLCh* const    encodingName
                                , const unsigned int    winCP
                                , const unsigned int    ieCP
                                , const unsigned int    blockSize) :

    XMLTranscoder(encodingName, blockSize)
    , fIECP(ieCP)
    , fWinCP(winCP)
{
}

Win32Transcoder::~Win32Transcoder()
{
}


// ---------------------------------------------------------------------------
//  Win32Transcoder: The virtual transcoder API
// ---------------------------------------------------------------------------
unsigned int
Win32Transcoder::transcodeFrom( const   XMLByte* const      srcData
                                , const unsigned int        srcCount
                                ,       XMLCh* const        toFill
                                , const unsigned int        maxChars
                                ,       unsigned int&       bytesEaten
                                ,       unsigned char* const charSizes)
{
    // Get temp pointers to the in and out buffers, and the chars sizes one
    XMLCh*          outPtr = toFill;
    const XMLByte*  inPtr  = srcData;
    unsigned char*  sizesPtr = charSizes;

    // Calc end pointers for each of them
    XMLCh*          outEnd = toFill + maxChars;
    const XMLByte*  inEnd  = srcData + srcCount;

    //
    //  Now loop until we either get our max chars, or cannot get a whole
    //  character from the input buffer.
    //
    bytesEaten = 0;
    while ((outPtr < outEnd) && (inPtr < inEnd))
    {
        //
        //  If we are looking at a leading byte of a multibyte sequence,
        //  then we are going to eat 2 bytes, else 1.
        //
        const unsigned int toEat = ::IsDBCSLeadByteEx(fWinCP, *inPtr) ?
                                    2 : 1;

        // Make sure a whol char is in the source
        if (inPtr + toEat > inEnd)
            break;

        // Try to translate this next char and check for an error
        const unsigned int converted = ::MultiByteToWideChar
        (
            fWinCP
            , MB_PRECOMPOSED | MB_ERR_INVALID_CHARS
            , (const char*)inPtr
            , toEat
            , outPtr
            , 1
        );

        if (converted != 1)
        {
            if (toEat == 1)
            {
                XMLCh tmpBuf[16];
                XMLString::binToText((unsigned int)(*inPtr), tmpBuf, 16, 16);
                ThrowXML
                (
                    TranscodingException
                    , XMLExcepts::Trans_BadSrcCP
                    , tmpBuf
                    , getEncodingName()
                );
            }
             else            
            {
                ThrowXML(TranscodingException, XMLExcepts::Trans_BadSrcSeq);
            }
        }

        // Update the char sizes array for this round
        *sizesPtr++ = toEat;

        // And update the bytes eaten count
        bytesEaten += toEat;

        // And update our in/out ptrs
        inPtr += toEat;
        outPtr++;
    }

    // Return the chars we output
    return (outPtr - toFill);
}


unsigned int
Win32Transcoder::transcodeTo(const  XMLCh* const    srcData
                            , const unsigned int    srcCount
                            ,       XMLByte* const  toFill
                            , const unsigned int    maxBytes
                            ,       unsigned int&   charsEaten
                            , const UnRepOpts       options)
{
    // Get pointers to the start and end of each buffer
    const XMLCh*    srcPtr = srcData;
    const XMLCh*    srcEnd = srcData + srcCount;
    XMLByte*        outPtr = toFill;
    XMLByte*        outEnd = toFill + maxBytes;

    //
    //  Now loop until we either get our max chars, or cannot get a whole
    //  character from the input buffer.
    //
    //  NOTE: We have to use a loop for this unfortunately because the
    //  conversion API is too dumb to tell us how many chars it converted if
    //  it couldn't do the whole source.
    //
    BOOL usedDef;
    while ((outPtr < outEnd) && (srcPtr < srcEnd))
    {
        //
        //  Do one char and see if it made it.
        const unsigned int bytesStored = ::WideCharToMultiByte
        (
            fWinCP
            , WC_COMPOSITECHECK | WC_SEPCHARS
            , srcPtr
            , 1
            , (char*)outPtr
            , outEnd - outPtr
            , 0
            , &usedDef
        );

        // If we didn't transcode anything, then we are done
        if (!bytesStored)
            break;

        //
        //  If the defaault char was used and the options indicate that
        //  this isn't allowed, then throw.
        //
        if (usedDef && (options == UnRep_Throw))
        {
            XMLCh tmpBuf[16];
            XMLString::binToText((unsigned int)*srcPtr, tmpBuf, 16, 16);
            ThrowXML2
            (
                TranscodingException
                , XMLExcepts::Trans_Unrepresentable
                , tmpBuf
                , getEncodingName()
            );
        }

        // Update our pointers
        outPtr += bytesStored;
        srcPtr++;
    }

    // Update the chars eaten
    charsEaten = srcPtr - srcData;

    // And return the bytes we stored
    return outPtr - toFill;
}


bool Win32Transcoder::canTranscodeTo(const unsigned int toCheck) const
{
    //
    //  If the passed value is really a surrogate embedded together, then
    //  we need to break it out into its two chars. Else just one.
    //
    XMLCh           srcBuf[2];
    unsigned int    srcCount = 1;
    if (toCheck & 0xFFFF0000)
    {
        srcBuf[0] = XMLCh((toCheck >> 10) + 0xD800);
        srcBuf[1] = XMLCh(toCheck & 0x3FF) + 0xDC00;
        srcCount++;
    }
     else
    {
        srcBuf[0] = XMLCh(toCheck);
    }

    //
    //  Use a local temp buffer that would hold any sane multi-byte char
    //  sequence and try to transcode this guy into it.
    //
    char tmpBuf[64];

    BOOL usedDef;
    const unsigned int bytesStored = ::WideCharToMultiByte
    (
        fWinCP
        , WC_COMPOSITECHECK | WC_SEPCHARS
        , srcBuf
        , srcCount
        , tmpBuf
        , 64
        , 0
        , &usedDef
    );

    if (!bytesStored || usedDef)
        return false;

    return true;
}


// ---------------------------------------------------------------------------
//  Win32LCPTranscoder: Constructors and Destructor
// ---------------------------------------------------------------------------
Win32LCPTranscoder::Win32LCPTranscoder()
{
}

Win32LCPTranscoder::~Win32LCPTranscoder()
{
}


// ---------------------------------------------------------------------------
//  Win32LCPTranscoder: Implementation of the virtual transcoder interface
// ---------------------------------------------------------------------------
unsigned int Win32LCPTranscoder::calcRequiredSize(const char* const srcText)
{
    if (!srcText)
        return 0;

    const unsigned int retVal = ::mbstowcs(0, srcText, 0);
    if (retVal == (unsigned int)-1)
        return 0;
    return retVal;
}


unsigned int Win32LCPTranscoder::calcRequiredSize(const XMLCh* const srcText)
{
    if (!srcText)
        return 0;

    const unsigned int retVal = ::wcstombs(0, srcText, 0);
    if (retVal == (unsigned int)-1)
        return 0;
    return retVal;
}


char* Win32LCPTranscoder::transcode(const XMLCh* const toTranscode)
{
    if (!toTranscode)
        return 0;

    char* retVal = 0;
    if (*toTranscode)
    {
        // Calc the needed size
        const unsigned int neededLen = ::wcstombs(0, toTranscode, 0);
        if (neededLen == (unsigned int)-1)
            return 0;

        // Allocate a buffer of that size plus one for the null and transcode
        retVal = new char[neededLen + 1];
        ::wcstombs(retVal, toTranscode, neededLen + 1);

        // And cap it off anyway just to make sure
        retVal[neededLen] = 0;
    }
     else
    {
        retVal = new char[1];
        retVal[0] = 0;
    }
    return retVal;
}


XMLCh* Win32LCPTranscoder::transcode(const char* const toTranscode)
{
    if (!toTranscode)
        return 0;

    XMLCh* retVal = 0;
    if (*toTranscode)
    {
        // Calculate the buffer size required
        const unsigned int neededLen = ::mbstowcs(0, toTranscode, 0);
        if (neededLen == (unsigned int)-1)
            return 0;

        // Allocate a buffer of that size plus one for the null and transcode
        retVal = new XMLCh[neededLen + 1];
        ::mbstowcs(retVal, toTranscode, neededLen + 1);

        // Cap it off just to make sure. We are so paranoid!
        retVal[neededLen] = 0;
    }
     else
    {
        retVal = new XMLCh[1];
        retVal[0] = 0;
    }
    return retVal;
}


bool Win32LCPTranscoder::transcode( const   char* const     toTranscode
                                    ,       XMLCh* const    toFill
                                    , const unsigned int    maxChars)
{
    // Check for a couple of psycho corner cases
    if (!toTranscode || !maxChars)
    {
        toFill[0] = 0;
        return true;
    }

    if (!*toTranscode)
    {
        toFill[0] = 0;
        return true;
    }

    // This one has a fixed size output, so try it and if it fails it fails
    if (::mbstowcs(toFill, toTranscode, maxChars + 1) == size_t(-1))
        return false;
    return true;
}


bool Win32LCPTranscoder::transcode( const   XMLCh* const    toTranscode
                                    ,       char* const     toFill
                                    , const unsigned int    maxBytes)
{
    // Watch for a couple of pyscho corner cases
    if (!toTranscode || !maxBytes)
    {
        toFill[0] = 0;
        return true;
    }

    if (!*toTranscode)
    {
        toFill[0] = 0;
        return true;
    }

    // This one has a fixed size output, so try it and if it fails it fails
    if (::wcstombs(toFill, toTranscode, maxBytes + 1) == size_t(-1))
        return false;

    // Cap it off just in case
    toFill[maxBytes] = 0;
    return true;
}
