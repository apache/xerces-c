/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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
 * @02A           V5R2M0    200419   jrhansen : support lowercase function
 * $Id$
 */

#ifndef ICONV400TRANSSERVICE_HPP
#define ICONV400TRANSSERVICE_HPP

#include <xercesc/util/Mutexes.hpp>
#include <xercesc/util/TransService.hpp>
#include <qlg.h>

XERCES_CPP_NAMESPACE_BEGIN

struct UConverter;

class XMLUTIL_EXPORT Iconv400TransService : public XMLTransService
{
public :
    // -----------------------------------------------------------------------
    //  Public, static methods
    // -----------------------------------------------------------------------


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    Iconv400TransService();
    ~Iconv400TransService();


    // -----------------------------------------------------------------------
    //  Implementation of the virtual transcoding service API
    // -----------------------------------------------------------------------
    virtual int compareIString
    (
        const   XMLCh* const    comp1
        , const XMLCh* const    comp2
    );

    virtual int compareNIString
    (
        const   XMLCh* const    comp1
        , const XMLCh* const    comp2
        , const unsigned int    maxChars
    );

    virtual const XMLCh* getId() const;

    virtual bool isSpace(const XMLCh toCheck) const;

    virtual XMLLCPTranscoder* makeNewLCPTranscoder();

    virtual bool supportsSrcOfs() const;

    virtual void upperCase(XMLCh* const toUpperCase) const;
    virtual void lowerCase(XMLCh* const toLowerCase) const;


protected :
    // -----------------------------------------------------------------------
    //  Protected virtual methods
    // -----------------------------------------------------------------------
    virtual XMLTranscoder* makeNewXMLTranscoder
    (
        const   XMLCh* const            encodingName
        ,       XMLTransService::Codes& resValue
        , const unsigned int            blockSize
        ,       MemoryManager* const    manager
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    Iconv400TransService(const Iconv400TransService&);
    Iconv400TransService& operator=(const Iconv400TransService&);

    XMLCh toUnicodeUpper(XMLCh toupper) const;
    XMLCh toUnicodeLower(XMLCh tolower) const;

    Qlg_CCSID_ReqCtlBlk_T convertCtlblkUpper;
    Qlg_CCSID_ReqCtlBlk_T convertCtlblkLower;

};



class XMLUTIL_EXPORT Iconv400Transcoder : public XMLTranscoder
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    Iconv400Transcoder
    (
        const   XMLCh* const      encodingName
        ,       UConverter* const toAdopt
        , const unsigned int      blockSize
        , MemoryManager* const    manager = XMLPlatformUtils::fgMemoryManager
    );
    ~Iconv400Transcoder();

    // -----------------------------------------------------------------------
    //  Implementation of the virtual transcoder interface
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
        const   XMLCh* const	srcData
        , const unsigned int	srcCount
        ,       XMLByte* const	toFill
        , const unsigned int	maxBytes
        ,       unsigned int&	charsEaten
        , const UnRepOpts	options
    );

    virtual bool canTranscodeTo
    (
        const   unsigned int	toCheck
    )   const;



private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    Iconv400Transcoder();
    Iconv400Transcoder(const Iconv400Transcoder&);
    Iconv400Transcoder& operator=(const Iconv400Transcoder&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fConverter
    //      This is a pointer to the Iconv400 converter that this transcoder
    //      uses.
    //
    //  fFixed
    //      This is set to true if the encoding is a fixed size one. This
    //      can be used to optimize some operations.
    //
    //  fSrcOffsets
    //      This is an array of longs, which are allocated to the size of
    //      the trancoding block (if any) indicated in the ctor. It is used
    //      to get the character offsets from Iconv400, which are then translated
    //      into an array of char sizes for return.
    // -----------------------------------------------------------------------
    UConverter*     fConverter;
    bool            fFixed;
    long*           fSrcOffsets;

};


class XMLUTIL_EXPORT Iconv400LCPTranscoder : public XMLLCPTranscoder
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    Iconv400LCPTranscoder(UConverter* const toAdopt);
    ~Iconv400LCPTranscoder();


    // -----------------------------------------------------------------------
    //  Implementation of the virtual transcoder interface
    // -----------------------------------------------------------------------
    virtual unsigned int calcRequiredSize(const char* const srcText
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    virtual unsigned int calcRequiredSize(const XMLCh* const srcText
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    virtual char* transcode(const XMLCh* const toTranscode);
    virtual char* transcode(const XMLCh* const toTranscode,
                            MemoryManager* const manager);

    virtual XMLCh* transcode(const char* const toTranscode);
    virtual XMLCh* transcode(const char* const toTranscode,
                             MemoryManager* const manager);

    virtual bool transcode
    (
        const   char* const     toTranscode
        ,       XMLCh* const    toFill
        , const unsigned int    maxChars
        , MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager
    );

    virtual bool transcode
    (
        const   XMLCh* const    toTranscode
        ,       char* const     toFill
        , const unsigned int    maxChars
        , MemoryManager* const  manager = XMLPlatformUtils::fgMemoryManager
    );



private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    Iconv400LCPTranscoder();
    Iconv400LCPTranscoder(const Iconv400LCPTranscoder&);
    Iconv400LCPTranscoder& operator=(const Iconv400LCPTranscoder&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fConverter
    //      This is a pointer to the Iconv converter that this transcoder
    //      uses.
    //
    //  fMutex
    //      We have to synchronize threaded calls to the converter.
    // -----------------------------------------------------------------------
    UConverter*     fConverter;
    XMLMutex        fMutex;
};

XERCES_CPP_NAMESPACE_END

#endif
