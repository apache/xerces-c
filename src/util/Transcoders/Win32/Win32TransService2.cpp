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
 * Revision 1.1  2000/03/08 23:40:37  roddey
 * Oops, I think I forgot to commit this new file
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/PlatformUtils.hpp>
#include <util/RefHashTableOf.hpp>
#include <util/XMLUni.hpp>
#include "Win32TransService.hpp"
#include <windows.h>


// ---------------------------------------------------------------------------
//  This is the simple CPMapEntry class. It just contains an encoding name
//  and a code page for that encoding.
// ---------------------------------------------------------------------------
class CPMapEntry
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    CPMapEntry
    (
        const   XMLCh* const    encodingName
        , const unsigned int    cpId
        , const unsigned int    ieId
    );

    CPMapEntry
    (
        const   char* const     encodingName
        , const unsigned int    cpId
        , const unsigned int    ieId
    );

    ~CPMapEntry();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* getEncodingName() const;
    const XMLCh* getKey() const;
    unsigned int getWinCP() const;
    unsigned int getIEEncoding() const;


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    CPMapEntry();
    CPMapEntry(const CPMapEntry&);
    void operator=(const CPMapEntry&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fEncodingName
    //      This is the encoding name for the code page that this instance
    //      represents.
    //
    //  fCPId
    //      This is the Windows specific code page for the encoding that this
    //      instance represents.
    //
    //  fIEId
    //      This is the IE encoding id. Its not used at this time, but we
    //      go ahead and get it and store it just in case for later.
    // -----------------------------------------------------------------------
    XMLCh*          fEncodingName;
    unsigned int    fCPId;
    unsigned int    fIEId;
};



// ---------------------------------------------------------------------------
//  Local data
//
//  gCPMap
//      This is hash table of entries which map encoding names to their
//      Windows specific code pages. The code page allows us to create
//      transcoders for those encodings. The encoding names come from XML
//      files.
//
//      This map is shared unsynchronized among all threads of the process,
//      which is cool since it will be read only once its initialized.
// ---------------------------------------------------------------------------
RefHashTableOf<CPMapEntry>   gCPMap(109);


// ---------------------------------------------------------------------------
//  CPMapEntry: Constructors and Destructor
// ---------------------------------------------------------------------------
CPMapEntry::CPMapEntry( const   char* const     encodingName
                        , const unsigned int    cpId
                        , const unsigned int    ieId) :
    fEncodingName(0)
    , fCPId(cpId)
    , fIEId(ieId)
{
    // Transcode the name to Unicode and store that copy
    const unsigned int srcLen = strlen(encodingName);
    const unsigned int targetLen = ::mbstowcs(0, encodingName, srcLen);
    fEncodingName = new XMLCh[targetLen + 1];
    ::mbstowcs(fEncodingName, encodingName, srcLen);
    fEncodingName[targetLen] = 0;

    //
    //  Upper case it because we are using a hash table and need to be
    //  sure that we find all case combinations.
    //
    ::wcsupr(fEncodingName);
}

CPMapEntry::CPMapEntry( const   XMLCh* const    encodingName
                        , const unsigned int    cpId
                        , const unsigned int    ieId) :

    fEncodingName(0)
    , fCPId(cpId)
    , fIEId(ieId)
{
    fEncodingName = XMLString::replicate(encodingName);

    //
    //  Upper case it because we are using a hash table and need to be
    //  sure that we find all case combinations.
    //
    ::wcsupr(fEncodingName);
}

CPMapEntry::~CPMapEntry()
{
    delete [] fEncodingName;
}


// ---------------------------------------------------------------------------
//  CPMapEntry: Getter methods
// ---------------------------------------------------------------------------
const XMLCh* CPMapEntry::getEncodingName() const
{
    return fEncodingName;
}

const XMLCh* CPMapEntry::getKey() const
{
    return fEncodingName;
}

unsigned int CPMapEntry::getWinCP() const
{
    return fCPId;
}

unsigned int CPMapEntry::getIEEncoding() const
{
    return fIEId;
}



// ---------------------------------------------------------------------------
//  Local methods
// ---------------------------------------------------------------------------
static bool isAlias(const   HKEY            encodingKey
                    ,       char* const     aliasBuf = 0
                    , const unsigned int    nameBufSz = 0)
{
    unsigned long theType;
    unsigned long theSize = nameBufSz;
    return (::RegQueryValueExA
    (
        encodingKey
        , "AliasForCharset"
        , 0
        , &theType
        , (unsigned char*)aliasBuf
        , &theSize
    ) == ERROR_SUCCESS);
}


// ---------------------------------------------------------------------------
//  Win32TransService: The protected virtual transcoding service API
// ---------------------------------------------------------------------------
XMLTranscoder*
Win32TransService::makeNewXMLTranscoder(const   XMLCh* const            encodingName
                                        ,       XMLTransService::Codes& resValue
                                        , const unsigned int            blockSize)
{
    const unsigned int upLen = 1024;
    XMLCh upEncoding[upLen + 1];

    //
    //  Get an upper cased copy of the encoding name, since we use a hash
    //  table and we store them all in upper case.
    //
    ::wcsncpy(upEncoding, encodingName, upLen);
    upEncoding[upLen] = 0;
    ::wcsupr(upEncoding);

    // Now to try to find this guy in the CP map
    CPMapEntry* theEntry = gCPMap.get(upEncoding);

    // If not found, then return a null pointer
    if (!theEntry)
    {
        resValue = XMLTransService::UnsupportedEncoding;
        return 0;
    }

    // We found it, so return a Win32 transcoder for this encoding
    return new Win32Transcoder
    (
        encodingName
        , theEntry->getWinCP()
        , theEntry->getIEEncoding()
        , blockSize
    );
}



// ---------------------------------------------------------------------------
//  Win32TransService: Private helper methods
// ---------------------------------------------------------------------------
void Win32TransService::initCPMap()
{
    // Open up the registry key that contains the info we want
    HKEY charsetKey;
    if (::RegOpenKeyExA
    (
        HKEY_CLASSES_ROOT
        , "MIME\\Database\\Charset"
        , 0
        , KEY_READ
        , &charsetKey))
    {
        XMLPlatformUtils::panic(XMLPlatformUtils::Panic_NoTransService);
    }

    //
    //  Read in the registry keys that hold the code page ids. Skip for now
    //  those entries which indicate that they are aliases for some other
    //  encodings. We'll come back and do a second round for those and look
    //  up the original name and get the code page id.
    //
    //  Note that we have to use A versions here so that this will run on
    //  98, and transcode the strings to Unicode.
    //
    const unsigned int nameBufSz = 1024;
    char nameBuf[nameBufSz + 1];
    unsigned int subIndex = 0;
    unsigned long theSize;
    while (true)
    {
        // Get the name of the next key
        theSize = nameBufSz;
        if (::RegEnumKeyExA
        (
            charsetKey
            , subIndex
            , nameBuf
            , &theSize
            , 0, 0, 0, 0) == ERROR_NO_MORE_ITEMS)
        {
            break;
        }

        // Open this subkey
        HKEY encodingKey;
        if (::RegOpenKeyExA
        (
            charsetKey
            , nameBuf
            , 0
            , KEY_READ
            , &encodingKey))
        {
            XMLPlatformUtils::panic(XMLPlatformUtils::Panic_NoTransService);
        }

        //
        //  Lts see if its an alias. If so, then ignore it in this first
        //  loop. Else, we'll add a new entry for this one.
        //
        if (!isAlias(encodingKey))
        {
            //
            //  Lets get the two values out of this key that we are
            //  interested in. There should be a code page entry and an
            //  IE entry.
            //
            unsigned long theType;
            unsigned int CPId;
            unsigned int IEId;

            theSize = sizeof(unsigned int);
            if (::RegQueryValueExA
            (
                encodingKey
                , "Codepage"
                , 0
                , &theType
                , (unsigned char*)&CPId
                , &theSize) != ERROR_SUCCESS)
            {
                XMLPlatformUtils::panic(XMLPlatformUtils::Panic_NoTransService);
            }

            //
            //  If this is not a valid Id, and it might not be because its
            //  not loaded on this system, then don't take it.
            //
            if (::IsValidCodePage(CPId))
            {
                theSize = sizeof(unsigned int);
                if (::RegQueryValueExA
                (
                    encodingKey
                    , "InternetEncoding"
                    , 0
                    , &theType
                    , (unsigned char*)&IEId
                    , &theSize) != ERROR_SUCCESS)
                {
                    XMLPlatformUtils::panic(XMLPlatformUtils::Panic_NoTransService);
                }

                CPMapEntry* newEntry = new CPMapEntry(nameBuf, CPId, IEId);
                gCPMap.put(newEntry);
            }
        }

        // And now close the subkey handle and bump the subkey index
        ::RegCloseKey(encodingKey);
        subIndex++;
    }

    //
    //  Now loop one more time and this time we do just the aliases. For
    //  each one we find, we look up that name in the map we've already
    //  built and add a new entry with this new name and the same id
    //  values we stored for the original.
    //
    subIndex = 0;
    char aliasBuf[nameBufSz + 1];
    while (true)
    {
        // Get the name of the next key
        theSize = nameBufSz;
        if (::RegEnumKeyExA
        (
            charsetKey
            , subIndex
            , nameBuf
            , &theSize
            , 0, 0, 0, 0) == ERROR_NO_MORE_ITEMS)
        {
            break;
        }

        // Open this subkey
        HKEY encodingKey;
        if (::RegOpenKeyExA
        (
            charsetKey
            , nameBuf
            , 0
            , KEY_READ
            , &encodingKey))
        {
            XMLPlatformUtils::panic(XMLPlatformUtils::Panic_NoTransService);
        }

        //
        //  If its an alias, look up the name in the map. If we find it,
        //  then construct a new one with the new name and the aliased
        //  ids.
        //
        if (isAlias(encodingKey, aliasBuf, nameBufSz))
        {
            const unsigned int srcLen = strlen(aliasBuf);
            const unsigned int targetLen = ::mbstowcs(0, aliasBuf, srcLen);
            XMLCh* uniAlias = new XMLCh[targetLen + 1];
            ::mbstowcs(uniAlias, aliasBuf, srcLen);
            uniAlias[targetLen] = 0;
            ::wcsupr(uniAlias);

            // Look up the alias name
            CPMapEntry* aliasedEntry = gCPMap.get(uniAlias);
            if (aliasedEntry)
            {
                //
                //  If the name is actually different, then take it.
                //  Otherwise, don't take it. They map aliases that are
                //  just different case.
                //
                if (::wcscmp(uniAlias, aliasedEntry->getEncodingName()))
                {
                    CPMapEntry* newEntry = new CPMapEntry
                    (
                        uniAlias
                        , aliasedEntry->getWinCP()
                        , aliasedEntry->getIEEncoding()
                    );
                    gCPMap.put(newEntry);
                }
            }
             else
            {
                delete [] uniAlias;
            }
        }

        // And now close the subkey handle and bump the subkey index
        ::RegCloseKey(encodingKey);
        subIndex++;
    }

    // And close the main key handle
    ::RegCloseKey(charsetKey);
}
