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
 * Revision 1.4  2000/01/12 00:16:23  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.3  1999/12/18 00:18:10  roddey
 * More changes to support the new, completely orthagonal support for
 * intrinsic encodings.
 *
 * Revision 1.2  1999/12/15 19:41:28  roddey
 * Support for the new transcoder system, where even intrinsic encodings are
 * done via the same transcoder abstraction as external ones.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:52  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:21  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(XMLSTRING_HPP)
#define XMLSTRING_HPP

#include <util/XML4CDefs.hpp>

class XMLLCPTranscoder;

class XMLUTIL_EXPORT XMLString
{
public:
    // -----------------------------------------------------------------------
    //  Static methods for native character mode string manipulation
    // -----------------------------------------------------------------------
    static void binToText
    (
        const   unsigned int    toFormat
        ,       char* const     toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    static void binToText
    (
        const   unsigned long   toFormat
        ,       char* const     toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    static void binToText
    (
        const   long            toFormat
        ,       char* const     toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    static void binToText
    (
        const   int             toFormat
        ,       char* const     toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    static void catString
    (
                char* const     target
        , const char* const     src
    );

    static int compareIString
    (
        const   char* const     str1
        , const char* const     str2
    );

    static int compareNString
    (
        const   char* const     str1
        , const char* const     str2
        , const unsigned int    count
    );

    static int compareNIString
    (
        const   char* const     str1
        , const char* const     str2
        , const unsigned int    count
    );

    static int compareString
    (
        const   char* const     str1
        , const char* const     str2
    );

    static void copyString
    (
                char* const     target
        , const char* const     src
    );

    static unsigned int hash
    (
        const   char* const     tohash
        , const unsigned int    hashModulus
    );

    static int indexOf(const char* const toSearch, const char ch);

    static int lastIndexOf(const char* const toSearch, const char ch);

    static int lastIndexOf
    (
        const   char* const     toSearch
        , const char            chToFind
        , const unsigned int    fromIndex
    );

    static char* replicate(const char* const toRep);

    static bool startsWith
    (
        const   char* const     toTest
        , const char* const     prefix
    );

    static unsigned int stringLen(const char* const src);

    static bool startsWithI
    (
        const   char* const     toTest
        , const char* const     prefix
    );

    static char* transcode
    (
        const   XMLCh* const    toTranscode
    );

    static bool transcode
    (
        const   XMLCh* const    toTranscode
        ,       char* const     toFill
        , const unsigned int    maxChars
    );

    static XMLCh* transcode
    (
        const   char* const     toTranscode
    );

    static bool transcode
    (
        const   char* const     toTranscode
        ,       XMLCh* const    toFill
        , const unsigned int    maxChars
    );

    static void trim(char* const toTrim);


    //
    //  XMLCh versions of everything
    //
    static void binToText
    (
        const   unsigned int    toFormat
        ,       XMLCh* const    toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    static void binToText
    (
        const   unsigned long   toFormat
        ,       XMLCh* const    toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    static void binToText
    (
        const   long            toFormat
        ,       XMLCh* const    toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    static void binToText
    (
        const   int             toFormat
        ,       XMLCh* const    toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    static void catString
    (
                XMLCh* const    target
        , const XMLCh* const    src
    );

    static int compareIString
    (
        const   XMLCh* const    str1
        , const XMLCh* const    str2
    );

    static int compareNString
    (
        const   XMLCh* const    str1
        , const XMLCh* const    str2
        , const unsigned int    count
    );

    static int compareNIString
    (
        const   XMLCh* const    str1
        , const XMLCh* const    str2
        , const unsigned int    count
    );

    static int compareString
    (
        const   XMLCh* const    str1
        , const XMLCh* const    str2
    );

    static void copyString
    (
                XMLCh* const    target
        , const XMLCh* const    src
    );

    static bool copyNString
    (
                XMLCh* const    target
        , const XMLCh* const    src
        , const unsigned int    maxChars
    );

    static const XMLCh* findAny
    (
        const   XMLCh* const    toSearch
        , const XMLCh* const    searchList
    );

    static XMLCh* findAny
    (
                XMLCh* const    toSearch
        , const XMLCh* const    searchList
    );

    static unsigned int hash
    (
        const   XMLCh* const    toHash
        , const unsigned int    hashModulus
    );

    static unsigned int hashN
    (
        const   XMLCh* const    toHash
        , const unsigned int    numChars
        , const unsigned int    hashModulus
    );

    static int indexOf(const XMLCh* const toSearch, const XMLCh ch);

    static int lastIndexOf(const XMLCh* const toSearch, const XMLCh ch);

    static int lastIndexOf
    (
        const   XMLCh* const    toSearch
        , const XMLCh           ch
        , const unsigned int    fromIndex
    );

    static XMLCh* makeUName
    (
        const   XMLCh* const    pszURI
        , const XMLCh* const    pszName
    );

    static unsigned int replaceTokens
    (
                XMLCh* const    errText
        , const unsigned int    maxChars
        , const XMLCh* const    text1
        , const XMLCh* const    text2
        , const XMLCh* const    text3
        , const XMLCh* const    text4
    );

    static XMLCh* replicate(const XMLCh* const toRep);

    static bool startsWith
    (
        const   XMLCh* const    toTest
        , const XMLCh* const    prefix
    );

    static bool startsWithI
    (
        const   XMLCh* const    toTest
        , const XMLCh* const    prefix
    );

    static unsigned int stringLen(const XMLCh* const src);

    static void trim(XMLCh* const toTrim);

    static void upperCase(XMLCh* const toUpperCase);


private :
    // -----------------------------------------------------------------------
    //  Unimplemented Constructors and Destructor
    // -----------------------------------------------------------------------
    XMLString();
    ~XMLString();


    // -----------------------------------------------------------------------
    //  Initialization, called from XMLPlatformUtils class
    // -----------------------------------------------------------------------
    friend class XMLPlatformUtils;
    static void initString(XMLLCPTranscoder* const defToUse);
};

#endif
