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
 * $Log$
 * Revision 1.1  1999/11/09 01:05:16  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:45:16  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#ifndef TRANSSERVICE_HPP
#define TRANSSERVICE_HPP

#include <util/XML4CDefs.hpp>

class XMLUTIL_EXPORT XMLTranscoder;

//
//  This class is an abstract base class which are used to abstract the
//  transcoding services that XML4C uses. XML4C's actual transcoding needs are
//  small (this does not count demo programs, just the parser system itself)
//  so it is desirable to allow different implementations to be provided.
//
//  Also defined here is the abstract class, XMLTranscoder, which represents
//  a particular instance of a transcoder for a particular encoding. The
//  transcoding service must be able to create new transcoders upon demand
//  for a particular encoding. It is assumed that the service will pool
//  transcoders for efficiency where possible (since their use will commonly
//  occur repeatedly and for short periods of time.)
//
class XMLUTIL_EXPORT XMLTransService
{
public :
    // -----------------------------------------------------------------------
    //  Class specific types
    // -----------------------------------------------------------------------
    enum Codes
    {
        Ok
        , UnsupportedEncoding
        , InternalFailure
        , SupportFilesNotFound
    };


    // -----------------------------------------------------------------------
    //  Public constructors and destructor
    // -----------------------------------------------------------------------
    virtual ~XMLTransService();


    // -----------------------------------------------------------------------
    //  The virtual transcoding service API
    // -----------------------------------------------------------------------
    virtual int compareIString
    (
        const   XMLCh* const    comp1
        , const XMLCh* const    comp2
    ) = 0;

    virtual int compareNIString
    (
        const   XMLCh* const    comp1
        , const XMLCh* const    comp2
        , const unsigned int    maxChars
    ) = 0;

    virtual bool isSpace(const XMLCh toCheck) const = 0;

    virtual XMLTranscoder* makeNewDefTranscoder() = 0;

    virtual XMLTranscoder* makeNewTranscoderFor
    (
        const   XMLCh* const            encodingName
        ,       XMLTransService::Codes& resValue
        , const unsigned int            blockSize
    ) = 0;


protected :
    // -----------------------------------------------------------------------
    //  Hidden constructors
    // -----------------------------------------------------------------------
    XMLTransService();


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLTransService(const XMLTransService&);
    void operator=(const XMLTransService&);
};



class XMLTranscoder
{
public :
    // -----------------------------------------------------------------------
    //  Public constructors and destructor
    // -----------------------------------------------------------------------
    virtual ~XMLTranscoder();


    // -----------------------------------------------------------------------
    //  The virtual transcoder API
    //
    //  NOTE:   All these APIs don't include null terminator characters in
    //          their parameters. So calcRequiredSize() returns the number
    //          of actual chars, not including the null. maxBytes and maxChars
    //          parameters refer to actual chars, not including the null so
    //          its assumed that the buffer is physically one char or byte
    //          larger.
    // -----------------------------------------------------------------------
    virtual unsigned int calcRequiredSize(const char* const srcText) = 0;

    virtual unsigned int calcRequiredSize(const XMLCh* const srcText) = 0;

    virtual XMLCh transcodeOne
    (
        const   char* const     srcData
        , const unsigned int    srcBytes
        ,       unsigned int&   bytesEaten
    ) = 0;

    virtual char* transcode(const XMLCh* const toTranscode) = 0;

    virtual bool transcode
    (
        const   XMLCh* const    toTranscode
        ,       char* const     toFill
        , const unsigned int    maxBytes
    ) = 0;

    virtual XMLCh* transcode(const char* const toTranscode) = 0;

    virtual bool transcode
    (
        const   char* const     toTranscode
        ,       XMLCh* const    toFill
        , const unsigned int    maxChars
    ) = 0;

    virtual unsigned int transcodeXML
    (
        const   char* const             srcData
        , const unsigned int            srcCount
        ,       XMLCh* const            toFill
        , const unsigned int            maxChars
        ,       unsigned int&           bytesEaten
    ) = 0;


protected :
    // -----------------------------------------------------------------------
    //  Hidden constructors
    // -----------------------------------------------------------------------
    XMLTranscoder();


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLTranscoder(const XMLTranscoder&);
    void operator=(const XMLTranscoder&);
};


// ---------------------------------------------------------------------------
//  XLMTransService: Hidden constructors and destructor
// ---------------------------------------------------------------------------
inline XMLTransService::XMLTransService()
{
}

inline XMLTransService::~XMLTransService()
{
}


// ---------------------------------------------------------------------------
//  XLMTranscoder: Hidden constructors and destructor
// ---------------------------------------------------------------------------
inline XMLTranscoder::XMLTranscoder()
{
}

inline XMLTranscoder::~XMLTranscoder()
{
}

#endif
