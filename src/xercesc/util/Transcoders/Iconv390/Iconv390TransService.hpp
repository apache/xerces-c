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
 * Revision 1.6  2003/05/15 18:47:03  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.5  2003/03/07 18:15:57  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.4  2003/01/06 21:46:08  tng
 * Since 390 uses ICU or Uniconv390 TransService, change Iconv390 to use intrinsic transcoders only so as to bypass Bug 2393.
 *
 * Revision 1.3  2002/11/04 15:14:33  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/04/09 15:44:00  knoaman
 * Add lower case string support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:36  peiyongz
 * sane_include
 *
 * Revision 1.3  2000/03/02 19:55:36  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/14 17:59:49  abagchi
 * Reused iconv descriptors
 *
 * Revision 1.1  2000/02/08 02:14:11  abagchi
 * Initial checkin
 *
 *
 */

#ifndef ICONV390TRANSSERVICE_HPP
#define ICONV390TRANSSERVICE_HPP

#include <xercesc/util/TransService.hpp>
#include <xercesc/util/Mutexes.hpp>
#include <iconv.h>

XERCES_CPP_NAMESPACE_BEGIN

/* Max encoding name in characters. */
#define UCNV_MAX_CONVERTER_NAME_LENGTH 60
typedef struct iconvconverter {
   struct iconvconverter *nextconverter;
   char                   name [UCNV_MAX_CONVERTER_NAME_LENGTH];
   XMLMutex               fMutex;
   iconv_t                fIconv390Descriptor;
   int                    usecount;
} iconvconverter_t;

class XMLUTIL_EXPORT Iconv390TransService : public XMLTransService
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    Iconv390TransService();
    ~Iconv390TransService();


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
    Iconv390TransService(const Iconv390TransService&);
    Iconv390TransService& operator=(const Iconv390TransService&);
};

class XMLUTIL_EXPORT Iconv390LCPTranscoder : public XMLLCPTranscoder
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    Iconv390LCPTranscoder();
    Iconv390LCPTranscoder(iconvconverter_t* const toAdopt);
    ~Iconv390LCPTranscoder();

    // -----------------------------------------------------------------------
    //  Implementation of the virtual transcoder interface
    // -----------------------------------------------------------------------
    virtual unsigned int calcRequiredSize(const char* const srcText);

    virtual unsigned int calcRequiredSize(const XMLCh* const srcText);

    virtual char* transcode(const XMLCh* const toTranscode);
    virtual char* transcode(const XMLCh* const toTranscode,
                            MemoryManager* const manager);

    virtual bool transcode
    (
        const   XMLCh* const    toTranscode
        ,       char* const     toFill
        , const unsigned int    maxBytes
    );

    virtual XMLCh* transcode(const char* const toTranscode);
    virtual XMLCh* transcode(const char* const toTranscode,
                             MemoryManager* const manager);

    virtual bool transcode
    (
        const   char* const     toTranscode
        ,       XMLCh* const    toFill
        , const unsigned int    maxChars
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    Iconv390LCPTranscoder(const Iconv390LCPTranscoder&);
    Iconv390LCPTranscoder& operator=(const Iconv390LCPTranscoder&);
    iconvconverter_t *converter;
};

XERCES_CPP_NAMESPACE_END

#endif
