/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2004 The Apache Software Foundation.  All rights
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
 * Revision 1.4  2004/06/30 19:04:17  peiyongz
 * XML1.0-3rd Edition: UTF_8
 *
 * Revision 1.3  2004/04/22 22:46:46  neilg
 * not all 390 processors support the new transcoding instructions; this patch makes Xerces work there as well.  Thanks to Steve Dulin
 *
 * Revision 1.2  2004/02/06 18:18:09  cargilld
 * Misc 390 changes.
 *
 */

#ifndef XMLUTF8TRANSCODER390_HPP
#define XMLUTF8TRANSCODER390_HPP

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/UTFDataFormatException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
//  This class provides an implementation of the XMLTranscoder interface
//  for a simple UTF8 transcoder. The parser does some encodings
//  intrinsically without depending upon external transcoding services.
//  To make everything more orthagonal, we implement these internal
//  transcoders using the same transcoder abstraction as the pluggable
//  transcoding services do.
//
class XMLUTIL_EXPORT XMLUTF8Transcoder390 : public XMLTranscoder
{
public :
    // -----------------------------------------------------------------------
    //  Public constructors and destructor
    // -----------------------------------------------------------------------
    XMLUTF8Transcoder390
    (
        const   XMLCh* const    encodingName
        , const unsigned int    blockSize
        , MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager
    );

    virtual ~XMLUTF8Transcoder390();


    // -----------------------------------------------------------------------
    //  Implementation of the XMLTranscoder interface
    // -----------------------------------------------------------------------
    virtual unsigned int transcodeFrom
    (
        const   XMLByte* const          srcData
        , const unsigned int            srcCount
        ,       XMLCh* const            toFill
        , const unsigned int            maxChars
        ,       unsigned int&           bytesEaten
        ,       unsigned char* const    charSizes
    );

    virtual unsigned int transcodeTo
    (
        const   XMLCh* const    srcData
        , const unsigned int    srcCount
        ,       XMLByte* const  toFill
        , const unsigned int    maxBytes
        ,       unsigned int&   charsEaten
        , const UnRepOpts       options
    );

    virtual bool canTranscodeTo
    (
        const   unsigned int    toCheck
    )   const;


private :

    inline void checkTrailingBytes(
                                    const XMLByte      toCheck
                                  , const unsigned int trailingBytes
                                  , const unsigned int position       
                                  ) const;

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLUTF8Transcoder390(const XMLUTF8Transcoder390&);
    XMLUTF8Transcoder390& operator=(const XMLUTF8Transcoder390&);
};

inline 
void XMLUTF8Transcoder390::checkTrailingBytes(const XMLByte      toCheck
                                            , const unsigned int trailingBytes
                                            , const unsigned int position) const
{

    if((toCheck & 0xC0) != 0x80) 
    {
        char len[2]  = {(char)(trailingBytes+0x31), 0};
        char pos[2]  = {(char)(position+0x31), 0};
        char byte[2] = {toCheck,0};
        ThrowXMLwithMemMgr3(UTFDataFormatException, XMLExcepts::UTF8_FormatError, pos, byte, len, getMemoryManager());
    }

}

XERCES_CPP_NAMESPACE_END

#endif
