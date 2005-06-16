/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Log$
 * Revision 1.8  2004/09/08 13:56:45  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.7  2003/12/24 15:24:15  cargilld
 * More updates to memory management so that the static memory manager.
 *
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
    virtual char* transcode(const XMLCh* const toTranscode,
                            MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    virtual XMLCh* transcode(const char* const toTranscode,
                             MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);


    // -----------------------------------------------------------------------
    //  DEPRECATED old transcode interface
    // -----------------------------------------------------------------------
    virtual unsigned int calcRequiredSize(const char* const srcText
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

    virtual unsigned int calcRequiredSize(const XMLCh* const srcText
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);

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
    Iconv390LCPTranscoder(const Iconv390LCPTranscoder&);
    Iconv390LCPTranscoder& operator=(const Iconv390LCPTranscoder&);
    iconvconverter_t *converter;
};

XERCES_CPP_NAMESPACE_END

#endif
