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
 * Revision 1.3  2000/01/19 00:56:59  roddey
 * Changes to get rid of dependence on old utils standard streams and to
 * get rid of the fgLibLocation stuff.
 *
 * Revision 1.2  2000/01/17 23:38:06  abagchi
 * Changed string "localhost" to XMLUni::fgLocalHostString
 *
 * Revision 1.1  2000/01/15 01:26:17  rahulj
 * Added support for HTTP to the parser using libWWW 5.2.8.
 * Renamed URL.[ch]pp to XMLURL.[ch]pp and like wise for the class name.
 * Only tested under NT 4.0 SP 5.
 * Removed URL.hpp from files where it was not used.
 *
 * Revision 1.2  2000/01/12 00:16:22  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.1.1.1  1999/11/09 01:04:20  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:45:16  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/BinFileInputStream.hpp>
#include <util/Janitor.hpp>
#include <util/PlatformUtils.hpp>
#include <util/RuntimeException.hpp>
#include <util/TransService.hpp>
#include <util/XMLURL.hpp>
#include <util/XMLNetAccessor.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUni.hpp>


// ---------------------------------------------------------------------------
//  Local types
//
//  TypeEntry
//      This structure defines a single entry in the list of URL types. Each
//      entry indicates the prefix for that type of URL, and the SourceTypes
//      value it maps to.
// ---------------------------------------------------------------------------
struct TypeEntry
{
    XMLURL::Protocols  protocol;
    const XMLCh*       prefix;
};


// ---------------------------------------------------------------------------
//  Local data
//
//  gXXXString
//      These are the strings for our prefix types. They all have to be
//      Unicode strings all the time, so we can't just do regular strings.
//
//  gTypeList
//      The list of URL types that we support
//
//  gMaxProtoLen
//  gMaxColonPos
//      The length of the longest protocol string and from that the maximum
//      index at which we must see the colon.
//
//      NOTE:!!! Be sure to keep this up to date!
// ---------------------------------------------------------------------------
static const XMLCh  gFileString[] =
{
        chLatin_f, chLatin_i, chLatin_l, chLatin_e, chNull
};

static const XMLCh gFTPString[]  =
{
        chLatin_f, chLatin_t, chLatin_p, chNull
};

static const XMLCh gHTTPString[] =
{
        chLatin_h, chLatin_t, chLatin_t, chLatin_p, chNull
};

static TypeEntry gTypeList[XMLURL::Protocols_Count] = 
{
        { XMLURL::File     , gFileString }
    ,   { XMLURL::HTTP     , gHTTPString }
    ,   { XMLURL::FTP      , gFTPString  }
};
static const unsigned int gTypeListSize = sizeof(gTypeList) / sizeof(gTypeList[0]);

// !!! Keep these up to date with list above!
static const unsigned int gMaxProtoLen = 4;



// ---------------------------------------------------------------------------
//  Local methods
// ---------------------------------------------------------------------------
static bool isHexDigit(const XMLCh toCheck)
{
    if ((toCheck >= chDigit_0) && (toCheck <= chDigit_9)
    ||  (toCheck >= chLatin_A) && (toCheck <= chLatin_Z)
    ||  (toCheck >= chLatin_a) && (toCheck <= chLatin_z))
    {
        return true;
    }
    return false;
}

static unsigned int xlatHexDigit(const XMLCh toXlat)
{
    if ((toXlat >= chDigit_0) && (toXlat <= chDigit_9))
        return (unsigned int)(toXlat - chDigit_0);

    if ((toXlat >= chLatin_A) && (toXlat <= chLatin_Z))
        return (unsigned int)(toXlat - chLatin_A) + 10;

    return (unsigned int)(toXlat - chLatin_a) + 10;
}



// ---------------------------------------------------------------------------
//  XMLURL: Public, static methods
// ---------------------------------------------------------------------------
XMLURL::Protocols XMLURL::lookupByName(const XMLCh* const protoName)
{
    for (unsigned int index = 0; index < gTypeListSize; index++)
    {
        if (!XMLString::compareIString(gTypeList[index].prefix, protoName))
            return gTypeList[index].protocol;
    }
    return XMLURL::Unknown;
}



// ---------------------------------------------------------------------------
//  XMLURL: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLURL::XMLURL() :

    fFragment(0)
    , fHost(0)
    , fPassword(0)
    , fPath(0)
    , fProtocol(XMLURL::Unknown)
    , fQuery(0)
    , fUser(0)
    , fURLText(0)
{
}

XMLURL::XMLURL(const XMLCh* const    baseURL
             , const XMLCh* const    relativeURL) :

    fFragment(0)
    , fHost(0)
    , fPassword(0)
    , fPath(0)
    , fProtocol(XMLURL::Unknown)
    , fQuery(0)
    , fUser(0)
    , fURLText(0)
{
    setURL(baseURL, relativeURL);
}

XMLURL::XMLURL(const XMLCh* const    baseURL
             , const char* const     relativeURL) :

    fFragment(0)
    , fHost(0)
    , fPassword(0)
    , fPath(0)
    , fProtocol(XMLURL::Unknown)
    , fQuery(0)
    , fUser(0)
    , fURLText(0)
{
    XMLCh* tmpRel = XMLString::transcode(relativeURL);
    ArrayJanitor<XMLCh> janRel(tmpRel);
    setURL(baseURL, tmpRel);
}

XMLURL::XMLURL(const XMLURL&         baseURL
             , const XMLCh* const    relativeURL) :

    fFragment(0)
    , fHost(0)
    , fPassword(0)
    , fPath(0)
    , fProtocol(XMLURL::Unknown)
    , fQuery(0)
    , fUser(0)
    , fURLText(0)
{
    setURL(baseURL, relativeURL);
}

XMLURL::XMLURL(const  XMLURL&        baseURL
             , const char* const     relativeURL) :

    fFragment(0)
    , fHost(0)
    , fPassword(0)
    , fPath(0)
    , fProtocol(XMLURL::Unknown)
    , fQuery(0)
    , fUser(0)
    , fURLText(0)
{
    XMLCh* tmpRel = XMLString::transcode(relativeURL);
    ArrayJanitor<XMLCh> janRel(tmpRel);
    setURL(baseURL, tmpRel);
}

XMLURL::XMLURL(const XMLCh* const urlText) :

    fFragment(0)
    , fHost(0)
    , fPassword(0)
    , fPath(0)
    , fProtocol(XMLURL::Unknown)
    , fQuery(0)
    , fUser(0)
    , fURLText(0)
{
    setURL(urlText);
}

XMLURL::XMLURL(const char* const urlText) :

    fFragment(0)
    , fHost(0)
    , fPassword(0)
    , fPath(0)
    , fProtocol(XMLURL::Unknown)
    , fQuery(0)
    , fUser(0)
    , fURLText(0)
{
    XMLCh* tmpText = XMLString::transcode(urlText);
    ArrayJanitor<XMLCh> janRel(tmpText);
    setURL(tmpText);
}

XMLURL::XMLURL(const XMLURL& toCopy) :

    fFragment(XMLString::replicate(toCopy.fFragment))
    , fHost(XMLString::replicate(toCopy.fHost))
    , fPassword(XMLString::replicate(toCopy.fPassword))
    , fPath(XMLString::replicate(toCopy.fPath))
    , fProtocol(toCopy.fProtocol)
    , fQuery(XMLString::replicate(toCopy.fQuery))
    , fUser(XMLString::replicate(toCopy.fUser))
    , fURLText(XMLString::replicate(toCopy.fURLText))
{
}

XMLURL::~XMLURL()
{
    cleanup();
}


// ---------------------------------------------------------------------------
//  XMLURL: Public operators
// ---------------------------------------------------------------------------
XMLURL& XMLURL::operator=(const XMLURL& toAssign)
{
    if (this == &toAssign)
        return *this;

    // Clean up our stuff
    cleanup();

    // And copy his stuff
    fFragment = XMLString::replicate(toAssign.fFragment);
    fHost = XMLString::replicate(toAssign.fHost);
    fPassword = XMLString::replicate(toAssign.fPassword);
    fPath = XMLString::replicate(toAssign.fPath);
    fProtocol = toAssign.fProtocol;
    fQuery = XMLString::replicate(toAssign.fQuery);
    fUser = XMLString::replicate(toAssign.fUser);
    fURLText = XMLString::replicate(toAssign.fURLText);

    return *this;
}

bool XMLURL::operator==(const XMLURL& toCompare) const
{
    //
    //  Compare the two complete URLs (which have been processed the same
    //  way so they should now be the same even if they came in via different
    //  relative parts.
    //
    if (XMLString::compareString(getURLText(), toCompare.getURLText()))
        return false;

    return true;
}



// ---------------------------------------------------------------------------
//  XMLURL: Getter methods
// ---------------------------------------------------------------------------
const XMLCh* XMLURL::getProtocolName() const
{
    // Check to see if its ever been set
    if (fProtocol == Unknown)
        ThrowXML(MalformedURLException, XML4CExcepts::URL_NoProtocolPresent);

    return gTypeList[fProtocol].prefix;
}


// ---------------------------------------------------------------------------
//  XMLURL: Setter methods
// ---------------------------------------------------------------------------
void XMLURL::setURL(const XMLCh* const urlText)
{
    //
    //  Try to parse the URL. If this fails, we just give up, cleanup and
    //  rethrow out of here.
    //
    cleanup();
    try
    {
        parse(urlText);
    }

    catch(...)
    {
        cleanup();
        throw;
    }
}

void XMLURL::setURL(const XMLCh* const    baseURL
                  , const XMLCh* const    relativeURL)
{
    cleanup();
    try
    {
        // Parse our URL string
        parse(relativeURL);

        //
        //  If its relative and the base is non-null and non-empty, then
        //  parse the base URL string and conglomerate them.
        //
        if (isRelative() && baseURL)
        {
            if (*baseURL)
            {
                XMLURL basePart(baseURL);
                conglomerateWithBase(basePart);
            }
        }
    }

    catch(...)
    {
        cleanup();
        throw;
    }
}

void XMLURL::setURL(const XMLURL&         baseURL
                  , const XMLCh* const    relativeURL)
{
    cleanup();
    try
    {
        // Parse our URL string
        parse(relativeURL);

        // If its relative, then conglomerate with the base URL
        if (isRelative())
            conglomerateWithBase(baseURL);
    }

    catch(...)
    {
        cleanup();
        throw;
    }
}


// ---------------------------------------------------------------------------
//  XMLURL: Miscellaneous methods
// ---------------------------------------------------------------------------
bool XMLURL::isRelative() const
{
    // If no protocol then relative
    if (fProtocol == Unknown)
        return true;

    // If no path, or the path is not absolute, then relative
    if (!fPath)
        return true;

    if (*fPath != chForwardSlash)
        return true;

    return false;
}


BinInputStream* XMLURL::makeNewStream() const
{
    //
    //  If its a local host, then we short circuit it and use our own file
    //  stream support. Otherwise, we just let it fall through and let the
    //  installed network access object provide a stream.
    //
    if (fProtocol == XMLURL::File)
    {
        if (!fHost || !XMLString::compareIString(fHost, XMLUni::fgLocalHostString))
        {
            //
            //  We have to play a little trick here. If its really a Windows
            //  style fully qualified path, we have to toss the leading /
            //  character.
            //
            const XMLCh* realPath = fPath;
            if (*fPath == chForwardSlash)
            {
                if (XMLString::stringLen(fPath) > 3)
                {
                    if (*(fPath + 2) == chColon)
                    {
                        const XMLCh chDrive = *(fPath + 1);
                        if (((chDrive >= chLatin_A) && (chDrive <= chLatin_Z))
                        ||  ((chDrive >= chLatin_a) && (chDrive <= chLatin_z)))
                        {
                            realPath = fPath + 3;
                        }
                    }
                }
            }

            BinFileInputStream* retStrm = new BinFileInputStream(realPath);
            if (!retStrm->getIsOpen())
            {
                delete retStrm;
                return 0;
            }
            return retStrm;
        }
    }

    //
    //  If we don't have have an installed net accessor object, then we
    //  have to just throw here.
    //
    if (!XMLPlatformUtils::fgNetAccessor)
        ThrowXML(MalformedURLException, XML4CExcepts::URL_UnsupportedProto);

    // Else ask the net accessor to create the stream
    return XMLPlatformUtils::fgNetAccessor->makeNew(*this);
}

void XMLURL::makeRelativeTo(const XMLCh* const baseURLText)
{
    // If this one is not relative, don't bother
    if (!isRelative())
        return;

    XMLURL baseURL(baseURLText);
    conglomerateWithBase(baseURL);
}

void XMLURL::makeRelativeTo(const XMLURL& baseURL)
{
    // If this one is not relative, don't bother
    if (!isRelative())
        return;
    conglomerateWithBase(baseURL);
}




// ---------------------------------------------------------------------------
//  XMLURL: Private helper methods
// ---------------------------------------------------------------------------

//
//  This method will take the broken out parts of the URL and build up the
//  full text. We don't do this unless someone asks us to, since its often
//  never required.
//
void XMLURL::buildFullText()
{
    // Calculate the worst case size of the buffer required
    unsigned int bufSize = gMaxProtoLen + 1
                           + XMLString::stringLen(fFragment) + 1
                           + XMLString::stringLen(fHost) + 2
                           + XMLString::stringLen(fPassword) + 1
                           + XMLString::stringLen(fPath)
                           + XMLString::stringLen(fQuery) + 1
                           + XMLString::stringLen(fUser) + 1
                           + 16;

    // Clean up the existing buffer and allocate another
    delete [] fURLText;
    fURLText = new XMLCh[bufSize];
    *fURLText = 0;

    XMLCh* outPtr = fURLText;
    if (fProtocol != Unknown)
    {
        XMLString::catString(fURLText, getProtocolName());
        outPtr += XMLString::stringLen(fURLText);
        *outPtr++ = chColon;
        *outPtr++ = chForwardSlash;
        *outPtr++ = chForwardSlash;
    }

    if (fHost)
    {
        XMLString::copyString(outPtr, fHost);
        outPtr += XMLString::stringLen(fHost);

        if (fUser)
        {
            *outPtr++ = chAt;
            XMLString::copyString(outPtr, fUser);
            outPtr += XMLString::stringLen(fUser);

            if (fPassword)
            {
                *outPtr++ = chColon;
                XMLString::copyString(outPtr, fPassword);
                outPtr += XMLString::stringLen(fPassword);
            }
        }
    }

    if (fPath)
    {
        XMLString::copyString(outPtr, fPath);
        outPtr += XMLString::stringLen(fPath);
    }

    if (fQuery)
    {
        *outPtr++ = chQuestion;
        XMLString::copyString(outPtr, fQuery);
        outPtr += XMLString::stringLen(fQuery);
    }

    if (fFragment)
    {
        *outPtr++ = chPound;
        XMLString::copyString(outPtr, fFragment);
        outPtr += XMLString::stringLen(fFragment);
    }
}


//
//  Just a central place to handle cleanup, since its done from a number
//  of different spots.
//
void XMLURL::cleanup()
{
    delete [] fFragment;
    delete [] fHost;
    delete [] fPassword;
    delete [] fPath;
    delete [] fQuery;
    delete [] fUser;
    delete [] fURLText;

    fFragment = 0;
    fHost = 0;
    fPassword = 0;
    fPath = 0;
    fQuery = 0;
    fUser = 0;
    fURLText = 0;

    fProtocol = Unknown;
}


void XMLURL::conglomerateWithBase(const XMLURL& baseURL)
{
    // The base URL cannot be relative
    if (baseURL.isRelative())
    {
        // <TBD> Add an error for this
        // ThrowXML(MalformedURLException, XML4CExcepts::URL_BaseWasRelative);
    }

    //
    //  Check a special case. If all we have is a fragment, then we want
    //  to just take the base host and path, plus our fragment.
    //
    if ((fProtocol == Unknown)
    &&  !fHost
    &&  !fPath
    &&  fFragment)
    {
        fProtocol = baseURL.fProtocol;
        fHost = XMLString::replicate(baseURL.fHost);
        fUser = XMLString::replicate(baseURL.fUser);
        fPassword = XMLString::replicate(baseURL.fPassword);
        fPath = XMLString::replicate(baseURL.fPath);
        return;
    }

    //
    //  All we have to do is run up through our fields and, for each one
    //  that we don't have, use the based URL's. Once we hit one field
    //  that we have, we stop.
    //
    if (fProtocol != Unknown)
        return;
    fProtocol = baseURL.fProtocol;

    if (fHost || !baseURL.fHost)
        return;
    fHost = XMLString::replicate(baseURL.fHost);
    if (baseURL.fUser)
        fUser = XMLString::replicate(baseURL.fUser);
    if (baseURL.fPassword)
        fPassword = XMLString::replicate(baseURL.fPassword);

    // If we have a path and its absolute, then we are done
    const bool hadPath = (fPath != 0);
    if (hadPath)
    {
        if (*fPath == chForwardSlash)
            return;
    }

    // Its a relative path, so weave them together.
    if (baseURL.fPath)
        weavePaths(baseURL.fPath);

    // If we had any original path, then we are done
    if (hadPath)
        return;

    if (fQuery || !baseURL.fQuery)
        return;
    fQuery = XMLString::replicate(baseURL.fQuery);

    if (fFragment || !baseURL.fFragment)
        return;
    fFragment = XMLString::replicate(baseURL.fFragment);
}


void XMLURL::parse(const XMLCh* const urlText)
{
    // Simplify things by checking for the psycho scenarios first
    if (!*urlText)
        ThrowXML(MalformedURLException, XML4CExcepts::URL_NoProtocolPresent);

    //
    //  The first thing we will do is to check for a file name, so that
    //  we don't waste time thinking its a URL. If its in the form x:\
    //  or x:/ and x is an ASCII letter, then assume that's the deal.
    //
    if (((*urlText >= chLatin_A) && (*urlText <= chLatin_Z))
    ||  ((*urlText >= chLatin_a) && (*urlText <= chLatin_z)))
    {
        if (*(urlText + 1) == chColon)
        {
            if (((*urlText + 2) == chForwardSlash)
            ||  ((*urlText + 2) == chBackSlash))
            {
                ThrowXML(MalformedURLException, XML4CExcepts::URL_NoProtocolPresent);
            }
        }
    }

    // Get a copy of the URL that we can modify
    XMLCh* srcCpy = XMLString::replicate(urlText);
    ArrayJanitor<XMLCh> janSrcCopy(srcCpy);

    //
    //  Get a pointer now that we can run up thrown the source as we parse
    //  bits and pieces out of it.
    //
    XMLCh* srcPtr = srcCpy;

    // Run up past any spaces
    while (*srcPtr)
    {
        if (!XMLPlatformUtils::fgTransService->isSpace(*srcPtr))
            break;
    }

    // Make sure it wasn't all space
    if (!*srcPtr)
        ThrowXML(MalformedURLException, XML4CExcepts::URL_NoProtocolPresent);

    //
    //  Ok, the next thing we have to do is to find either a / or : character.
    //  If the : is first, we assume we have a protocol. If the / is first,
    //  then we skip to the host processing.
    //
    static const XMLCh listOne[]    = { chColon, chForwardSlash, chNull };
    static const XMLCh listTwo[]    = { chAt, chForwardSlash, chNull };
    static const XMLCh listThree[]  = { chColon, chForwardSlash, chNull };
    static const XMLCh listFour[]   = { chForwardSlash, chNull };
    static const XMLCh listFive[]   = { chPound, chQuestion, chNull };
    static const XMLCh listSix[]    = { chPound, chNull };
    XMLCh* ptr1 = XMLString::findAny(srcPtr, listOne);
    XMLCh* ptr2;

    // If we found a protocol, then deal with it
    if (ptr1)
    {
        if (*ptr1 == chColon)
        {
            // Cap the string at the colon
            *ptr1 = 0;

            // And try to find it in our list of protocols
            fProtocol = lookupByName(srcPtr);

            if (fProtocol == Unknown)
            {
                ThrowXML1
                (
                    MalformedURLException
                    , XML4CExcepts::URL_UnsupportedProto1
                    , srcPtr
                );
            }
        
            // And move our source pointer up past what we've processed
            srcPtr = (ptr1 + 1);
        }
    }

    //
    //  Ok, next we need to see if we have any host part. If the next
    //  two characters are //, then we need to check, else move on.
    //
    if ((*srcPtr == chForwardSlash) && (*(srcPtr + 1) == chForwardSlash))
    {
        // Move up past the slashes
        srcPtr += 2;

        //
        //  If we aren't at the end of the string, then there has to be a
        //  host name at this point.
        //
        if (*srcPtr)
        {
            // Search from here for either a @ or / character
            ptr1 = XMLString::findAny(srcPtr, listTwo);

            //
            //  If we found something, then the host is between where
            //  we are and what we found. Else the host is the rest of
            //  the content and we are done. If its empty, leave it null.
            //
            if (ptr1)
            {
                if (ptr1 != srcPtr)
                {
                    fHost = new XMLCh[(ptr1 - srcPtr) + 1];
                    ptr2 = fHost;
                    while (srcPtr < ptr1)
                        *ptr2++ = *srcPtr++;
                    *ptr2 = 0;
                }

                //
                //  If we found a @, then we have to parse out a user name
                //  and optional password.
                //
                if (*srcPtr == chAt)
                {
                    // Move up past the @ and look for a / or : character
                    srcPtr++;
                    ptr1 = XMLString::findAny(srcPtr, listThree);

                    //
                    //  If we found something, then the user name is between
                    //  where we are and what we found. Else the user name
                    //  is the rest of the string and we are done.
                    //
                    if (ptr1)
                    {
                        fUser = new XMLCh[(ptr1 - srcPtr) + 1];
                        ptr2 = fUser;
                        while (srcPtr < ptr1)
                            *ptr2++ = *srcPtr++;
                        *ptr2 = 0;

                        //
                        //  If we found a : character, then everything from
                        //  after that to the end or a / character is the
                        //  password.
                        //
                        if (*srcPtr == chColon)
                        {
                            srcPtr++;
                            ptr1 = XMLString::findAny(srcPtr, listFour);

                            //
                            //  If we found one, then the password is everything
                            //  from where we are to there. Else, the password
                            //  is the rest of the string.
                            //
                            if (ptr1)
                            {
                                fPassword = new XMLCh[(ptr1 - srcPtr) + 1];
                                ptr2 = fPassword;
                                while (srcPtr < ptr1)
                                    *ptr2++ = *srcPtr++;
                                *ptr2 = 0;
                            }
                             else
                            {
                                fPassword = XMLString::replicate(srcPtr);
                                return;
                            }
                        }
                    }
                     else
                    {
                        fUser = XMLString::replicate(srcPtr);
                        return;
                    }
                }
            }
             else
            {
                fHost = XMLString::replicate(srcPtr);
                return;
            }
        }
    }

    //
    //  Next is the path part. It can be absolute, i.e. starting with a
    //  forward slash character, or relative. Its basically everything up
    //  to the end of the string or to any trailing query or fragment.
    //
    ptr1 = XMLString::findAny(srcPtr, listFive);
    if (!ptr1)
    {
        fPath = XMLString::replicate(srcPtr);
        return;
    }

    // Everything from where we are to what we found is the path
    if (ptr1 > srcPtr)
    {
        fPath = new XMLCh[(ptr1 - srcPtr) + 1];
        ptr2 = fPath;
        while (srcPtr < ptr1)
            *ptr2++ = *srcPtr++;
        *ptr2 = 0;
    }

    //
    //  If we found a fragment, then it is the rest of the string and we
    //  are done.
    //
    if (*srcPtr == chPound)
    {
        srcPtr++;
        fFragment = XMLString::replicate(srcPtr);
        return;
    }

    //
    //  The query is either the rest of the string, or up to the fragment
    //  separator.
    //
    srcPtr++;
    ptr1 = XMLString::findAny(srcPtr, listSix);
    if (!ptr1)
    {
        fQuery = XMLString::replicate(srcPtr);
        return;
    }
     else
    {
        fQuery = new XMLCh[(ptr1 - srcPtr) + 1];
        ptr2 = fQuery;
        while (srcPtr < ptr1)
            *ptr2++ = *srcPtr++;
        *ptr2 = 0;
    }

    // If we are not at the end now, then everything else is the fragment
    if (*srcPtr == chPound)
    {
        srcPtr++;
        fFragment = XMLString::replicate(srcPtr);
    }
}


void XMLURL::weavePaths(const XMLCh* const basePart)
{
    // Watch for stupid stuff
    if (!basePart)
        return;
    if (!*basePart)
        return;

    //
    //  Ok, lets start at the end of the base path and work backwards and
    //  our path part and work forwards. For each leading . we see, we just
    //  eat it. For each leading .. we see, we eat it and throw away one
    //  level in the source URL.
    //
    //  If the last character in the base part is a forward slash, back
    //  up one first before we look for the last slash.
    //
    const XMLCh* basePtr = basePart + (XMLString::stringLen(basePart) - 1);
    if (*basePtr == chForwardSlash)
        basePtr--;

    while ((basePtr >= basePart)
    &&     ((*basePtr != chForwardSlash) && (*basePtr != chBackSlash)))
    {
        basePtr--;
    }

    if (basePtr < basePart)
        return;

    // Create a buffer as large as both parts
    XMLCh* tmpBuf = new XMLCh[XMLString::stringLen(fPath)
                              + XMLString::stringLen(basePart)
                              + 2];
    //
    //  If we have no path part, then copy the base part up to the
    //  base pointer
    //
    if (!fPath)
    {
        XMLCh* bufPtr = tmpBuf;
        const XMLCh* tmpPtr = basePart;
        while (tmpPtr <= basePtr)
            *bufPtr++ = *tmpPtr++;
        *bufPtr = 0;

        fPath = tmpBuf;
        return;
    }

    // After this, make sure the buffer gets handled if we exit early
    ArrayJanitor<XMLCh> janBuf(tmpBuf);

    //
    //  We have some path part, so we need to check to see if we ahve to
    //  weave any of the parts together.
    //
    XMLCh* pathPtr = fPath;
    while (true)
    {
        // If it does not start with some period, then we are done
        if (*pathPtr != chPeriod)
            break;

        unsigned int periodCount = 1;
        pathPtr++;
        if (*pathPtr == chPeriod)
        {
            pathPtr++;
            periodCount++;
        }

        // Has to be followed by a / or \ or the null to mean anything
        if ((*pathPtr != chForwardSlash) && (*pathPtr != chBackSlash)
        &&  *pathPtr)
        {
            break;
        }
        if (*pathPtr)
            pathPtr++;

        // If its one period, just eat it, else move backwards in the base
        if (periodCount == 2)
        {
            basePtr--;
            while ((basePtr >= basePart)
            &&     ((*basePtr != chForwardSlash) && (*basePtr != chBackSlash)))
            {
                basePtr--;
            }

            if (basePtr < basePart)
            {
                // The base cannot provide enough levels, so its in error
                // <TBD>
            }
        }
    }

    // Copy the base part up to the base pointer
    XMLCh* bufPtr = tmpBuf;
    const XMLCh* tmpPtr = basePart;
    while (tmpPtr <= basePtr)
        *bufPtr++ = *tmpPtr++;

    // And then copy on the rest of our path
    XMLString::copyString(bufPtr, pathPtr);

    // Now delete our path and make the new buffer our path
    delete [] fPath;
    janBuf.orphan();
    fPath = tmpBuf;
}
