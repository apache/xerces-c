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
 * Revision 1.1  1999/11/09 01:04:20  twl
 * Initial revision
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
#include <util/URL.hpp>
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
    URL::Protocols  protocol;
    const XMLCh*    prefix;
    bool            supported;
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
        chLatin_f, chLatin_i, chLatin_l, chLatin_e, chColon
    ,   chForwardSlash, chForwardSlash, chNull
};

static const XMLCh gFTPString[]  =
{
        chLatin_f, chLatin_t, chLatin_p, chColon, chForwardSlash
    ,   chForwardSlash, chNull
};

static const XMLCh gGopherString[]  =
{
        chLatin_g, chLatin_o, chLatin_p, chLatin_h, chLatin_e
    ,   chLatin_r, chColon, chForwardSlash, chForwardSlash, chNull
};

static const XMLCh gHTTPString[] =
{
        chLatin_h, chLatin_t, chLatin_t, chLatin_p, chColon
    ,   chForwardSlash, chForwardSlash, chNull
};

static const XMLCh gLocalHostString[] =
{
        chLatin_l, chLatin_o, chLatin_c, chLatin_a, chLatin_l
    ,   chLatin_h, chLatin_o, chLatin_s, chLatin_t, chNull
};

static const XMLCh gMailToString[]  =
{
        chLatin_m, chLatin_a, chLatin_i, chLatin_l, chLatin_t
    ,   chLatin_o, chColon, chForwardSlash, chForwardSlash, chNull
};

static const XMLCh gNewsString[] =
{
        chLatin_n, chLatin_e, chLatin_w, chLatin_s, chColon
    ,   chForwardSlash, chForwardSlash, chNull
};

static const XMLCh gNNTPString[] =
{
        chLatin_n, chLatin_n, chLatin_t, chLatin_p, chColon
    ,   chForwardSlash, chForwardSlash, chNull
};

static const XMLCh gTelnetString[]  =
{
        chLatin_t, chLatin_e, chLatin_l, chLatin_n, chLatin_e
    ,   chLatin_t, chColon, chForwardSlash, chForwardSlash, chNull
};

static const XMLCh gWaisString[]  =
{
        chLatin_w, chLatin_a, chLatin_i, chLatin_s, chColon
    ,   chForwardSlash, chForwardSlash, chNull
};

static const XMLCh gProsperoString[]  =
{
        chLatin_p, chLatin_r, chLatin_o, chLatin_s, chLatin_p
    ,   chLatin_e, chLatin_r, chLatin_o, chColon, chForwardSlash
    ,   chForwardSlash, chNull
};

static TypeEntry gTypeList[URL::Protocols_Count] = 
{
        { URL::File     , gFileString       , true }
    ,   { URL::HTTP     , gHTTPString       , false }
    ,   { URL::FTP      , gFTPString        , false }
    ,   { URL::Gopher   , gGopherString     , false }
    ,   { URL::MailTo   , gMailToString     , false }
    ,   { URL::News     , gNewsString       , false }
    ,   { URL::NNTP     , gNNTPString       , false }
    ,   { URL::Telnet   , gTelnetString     , false }
    ,   { URL::Wais     , gWaisString       , false }
    ,   { URL::Prospero , gProsperoString   , false }
};

// !!! Keep these up to date with list above!
static const unsigned int gMaxProtoLen = 11;
static const unsigned int gMaxColonPos = gMaxProtoLen - 3;


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
//  URL: Constructors and Destructor
// ---------------------------------------------------------------------------
URL::URL() :

    fFullURL(0)
    , fHost(0)
    , fPath(0)
    , fProtocol(URL::File)
{
}

URL::URL(const URL& toCopy) :

    fFullURL(XMLString::replicate(toCopy.fFullURL))
    , fHost(XMLString::replicate(toCopy.fHost))
    , fPath(XMLString::replicate(toCopy.fPath))
    , fProtocol(toCopy.fProtocol)
{
}

URL::~URL()
{
    cleanup();
}


// ---------------------------------------------------------------------------
//  URL: Public operators
// ---------------------------------------------------------------------------
URL& URL::operator=(const URL& toAssign)
{
    if (this == &toAssign)
        return *this;

    // Clean up our stuff
    cleanup();

    // And copy his stuff
    fFullURL = XMLString::replicate(toAssign.fFullURL);
    fHost = XMLString::replicate(toAssign.fHost);
    fPath = XMLString::replicate(toAssign.fPath);
    fProtocol = toAssign.fProtocol;

    return *this;
}

bool URL::operator==(const URL& toCompare) const
{
    // Test the obvious one first
    if (fProtocol != toCompare.fProtocol)
        return false;

    //
    //  Oh well, we have to test the components. Don't test the original
    //  URLs, because normalization might have occured that would have made
    //  them equal even though actual text of the full URLs is not.
    //
    if (XMLString::compareString(fPath, toCompare.fPath))
        return false;

    if (XMLString::compareString(fHost, toCompare.fHost))
        return false;

    return true;
}



// ---------------------------------------------------------------------------
//  URL: Getter methods
// ---------------------------------------------------------------------------
const XMLCh* URL::getProtocol() const
{
    return gTypeList[fProtocol].prefix;
}


// ---------------------------------------------------------------------------
//  URL: Setter methods
// ---------------------------------------------------------------------------
void URL::setURL(const XMLCh* const urlText)
{
    fFullURL = XMLString::replicate(urlText);
    try
    {
        parse();
    }

    catch(...)
    {
        cleanup();
        throw;
    }
}

void URL::setURL(const char* const urlText)
{
    // Transcode the passed string to Unicode
    fFullURL = XMLString::transcode(urlText);
    try
    {
        parse();
    }

    catch(...)
    {
        cleanup();
        throw;
    }
}



// ---------------------------------------------------------------------------
//  URL: Miscellaneous methods
// ---------------------------------------------------------------------------
BinInputStream* URL::makeNewStream() const
{
    switch(fProtocol)
    {
        case URL::File  :
        {
            BinFileInputStream* retStrm = new BinFileInputStream(getPath());
            if (!retStrm->getIsOpen())
            {
                delete retStrm;
                return 0;
            }
            return retStrm;
            break;
        }

        default :
            ThrowXML(MalformedURLException, XML4CExcepts::URL_UnsupportedProto);
            break;
    }
    return 0;
}


// ---------------------------------------------------------------------------
//  URL: Private helper methods
// ---------------------------------------------------------------------------

//
//  Just a central place to handle cleanup, since its done from a number
//  of different spots.
//
void URL::cleanup()
{
    delete [] fFullURL;
    fFullURL = 0;
    delete [] fHost;
    fHost = 0;
    delete [] fPath;
    fPath = 0;
}


//
//  This method searches our list of protocols and sees if the passed text
//  starts with one of them. The prefix is the whole thing up to the second
//  forward slash. The length of the text is passed so that obvious failures
//  can be found quickly.
//
URL::Protocols URL::findType(unsigned int& curPos) const
{
    XMLCh tmpStr[gMaxProtoLen+1];

    //
    //  Remember the current position so we can do exploratory reads from
    //  the URL. Then look forward for a colon.
    //
    const unsigned int orgPos = curPos;
    unsigned int tmpPos = curPos;
    unsigned int tmpIndex = 0;
    while (true)
    {
        // Get another char from the source URL. Indicate end of text is ok
        const XMLCh nextCh = getNextCh(tmpPos, true);

        // If we hit the end, then no good
        if (!nextCh)
            ThrowXML(MalformedURLException, XML4CExcepts::URL_MalformedURL);

        // Store this new character
        tmpStr[tmpIndex++] = nextCh;

        // If we hit the colon, break out
        if (nextCh == chColon)
            break;

        // If we exceed the max colon pos without finding a colon, then no good
        if (tmpIndex > gMaxColonPos)
            ThrowXML(MalformedURLException, XML4CExcepts::URL_MalformedURL);
    }

    //
    //  See if the next two chars are forward slashes, If not, then undo
    //  our read and return local. Else store them and compare against
    //  the list. Indicate that end of input is ok here.
    //
    const bool gotSlashes = (getNextCh(tmpPos, true) == chForwardSlash)
                            && (getNextCh(tmpPos, true) == chForwardSlash);
    if (!gotSlashes)
        ThrowXML(MalformedURLException, XML4CExcepts::URL_MalformedURL);

    // Store the slashes in our temp string too
    tmpStr[tmpIndex++] = chForwardSlash;
    tmpStr[tmpIndex++] = chForwardSlash;

    // Update the caller's position and cap off our string
    curPos = tmpPos;
    tmpStr[tmpIndex] = chNull;

    //
    //  Ok, lets see if tmpStr matches any of the prefixes in our list of
    //  protocols.
    //
    for (unsigned int index = 0; index < Protocols_Count; index++)
    {
        if (!XMLString::compareString(tmpStr, gTypeList[index].prefix))
            return gTypeList[index].protocol;
    }

    // Cannot be a supported URL protocol
    ThrowXML(MalformedURLException, XML4CExcepts::URL_UnsupportedProto);
}


//
//  This method is used during the parse. It gets the next character out of
//  the source URL (in fFullURL, which is a copy of the original text) and
//  returns it. It updates the passed position with the new position.
//
//  The primary job of this method is to handle escaped characters, by reading
//  them in and converting them to a Unicode char.
//
XMLCh URL::getNextCh(unsigned int& pos, const bool endOk) const
{
    //
    //  If we are at the end of the URL, then either return a zero or
    //  throw if end of URL is not legal here.
    //
    if (!fFullURL[pos])
    {
        if (!endOk)
            ThrowXML(MalformedURLException, XML4CExcepts::URL_MalformedURL);
        return chNull;
    }

    //
    //  See if the current character is a '%'. If so, then we need to
    //  deal with an escaped character.
    //
    if (fFullURL[pos] == chPercent)
    {
        XMLCh escapedChar = 0;

        // There must be at least two more characters
        if (!fFullURL[pos+1] || !fFullURL[pos+2])
            ThrowXML(MalformedURLException, XML4CExcepts::URL_MalformedURL);

        // Get them out and test them
        const XMLCh test1 = fFullURL[pos+1];
        const XMLCh test2 = fFullURL[pos+2];

        if (!isHexDigit(test1) || !isHexDigit(test2))
            ThrowXML(MalformedURLException, XML4CExcepts::URL_MalformedURL);

        // Convert these to a character
        escapedChar = XMLCh((xlatHexDigit(test1) << 4) + xlatHexDigit(test2));

        // Bump the position up
        pos += 3;

        return escapedChar;
    }

    // Else just return the current char and bump the position
    return fFullURL[pos++];
}


//
//  This method is called to parse the text into its components and validate
//  that the URL is legal. It uses getNextCh() to pull characters out of the
//  URL.
//
void URL::parse()
{
    // This is the current position that we track during the parse
    unsigned int curPos = 0;
    XMLCh nextCh;
    const unsigned int bufSize = 2047;
    XMLCh tmpBuf[bufSize + 1];
    unsigned int bufIndex;

    //
    //  Search the text for a prefix. We will get back the type of the prefix
    //  and the current position will be updated.
    //
    fProtocol = findType(curPos);

    //  In order to distinguish between a malformed URL and an unsupported
    //  URL, we watch here for the types that we support. If its not supported
    //  we throw a runtime exception.
    //
    if (!gTypeList[fProtocol].supported)
        ThrowXML(MalformedURLException, XML4CExcepts::URL_UnsupportedProto);

    //
    //  Check the next char. It must be either a forward slash or it will
    //  be the first char of the host name. We don't allow end of input
    //  here so it will cause an exception if we hit it.
    //
    nextCh = getNextCh(curPos);
    if (nextCh == chForwardSlash)
    {
        // There is no host so make an empty one
        fHost = new XMLCh[1];
        fHost[0] = chNull;
    }
     else
    {
        // Put in the lookahead char we did above before we enter the loop
        bufIndex = 0;
        tmpBuf[bufIndex++] = nextCh;

        // And now read up to the slash separator
        while (true)
        {
            // Get the next char, end of input is not valid here
            const XMLCh nextCh = getNextCh(curPos);

            // Break out on the forward slash
            if (nextCh == chForwardSlash)
                break;

            // Otherwise, save it
            tmpBuf[bufIndex++] = nextCh;

            // If we max out on the temp buffer, definitely bad
            if (bufIndex >= bufSize)
                ThrowXML(MalformedURLException, XML4CExcepts::URL_MalformedURL);
        }

        // Cap the temp buffer and replicate to our host member
        tmpBuf[bufIndex] = chNull;
        fHost = XMLString::replicate(tmpBuf);
    }

    //
    //  Now we need to get the path part of the URL. This should be the
    //  rest of the content. So we just go until we get a null char back
    //  from the character spooler. This gets rid of all escaped chars
    //  in the URL.
    //
    bufIndex = 0;
    while (true)
    {
        // Tell it that end of input is ok
        const XMLCh nextCh = getNextCh(curPos, true);
        tmpBuf[bufIndex++] = nextCh;

        // Break out at the end
        if (!nextCh)
            break;
    }

    //
    //  And get our own copy of the temp buffer as the path. If the path
    //  does not start with 'x:', then assume that its a Unix style path
    //  and put in a leading '/'.
    //
    tmpBuf[bufIndex] = chNull;
    if ((((tmpBuf[0] >= chLatin_A) && (tmpBuf[0] <= chLatin_Z))
    ||   ((tmpBuf[0] >= chLatin_a) && (tmpBuf[0] <= chLatin_z)))
    &&  (tmpBuf[1] == chColon))
    {
        fPath = XMLString::replicate(tmpBuf);
    }
     else
    {
        fPath = new XMLCh[XMLString::stringLen(tmpBuf) + 2];
        fPath[0] = chForwardSlash;
        XMLString::copyString(&fPath[1], tmpBuf);
    }

    //
    //  <TBD> When we have more complete support, get rid of this. But for
    //  now we only support file:// (which is checked above) and for files
    //  we only support an empty host or "localhost" which means the same
    //  thing.
    //
    if (fHost[0])
    {
        if (XMLString::compareString(fHost, gLocalHostString))
            ThrowXML(MalformedURLException, XML4CExcepts::URL_OnlyLocalHost);
    }
}
