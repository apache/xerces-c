/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2003 The Apache Software Foundation.  All rights
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
 * $Id$
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/XMLUri.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLUri: static data
// ---------------------------------------------------------------------------

//      Amended by RFC2732
//      reserved      = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+" |
//                      "$" | "," | "[" | "]"
//
const XMLCh XMLUri::RESERVED_CHARACTERS[] =
{
    chSemiColon, chForwardSlash, chQuestion, chColon, chAt,
    chAmpersand, chEqual, chPlus, chDollarSign, chComma, chOpenSquare,
    chCloseSquare, chNull
};

//
//      mark          = "-" | "_" | "." | "!" | "~" | "*" | "'" |
//                      "(" | ")"
//
const XMLCh XMLUri::MARK_CHARACTERS[] =
{
    chDash, chUnderscore, chPeriod, chBang, chTilde,
    chAsterisk, chSingleQuote, chOpenParen, chCloseParen, chNull
};

//
//      scheme        = alpha *( alpha | digit | "+" | "-" | "." )
//
const XMLCh XMLUri::SCHEME_CHARACTERS[] =
{
    chPlus, chDash, chPeriod, chNull
};

//
//      userinfo      = *( unreserved | escaped |
//                         ";" | ":" | "&" | "=" | "+" | "$" | "," )
//
const XMLCh XMLUri::USERINFO_CHARACTERS[] =
{
    chSemiColon, chColon, chAmpersand, chEqual, chPlus,
    chDollarSign, chPeriod, chNull
};

//      pchar plus ';' and '/'.
//      pchar         = unreserved | escaped |
//                      ":" | "@" | "&" | "=" | "+" | "$" | ","
const XMLCh XMLUri::PATH_CHARACTERS[] =
{
    chSemiColon, chForwardSlash, chColon, chAt, chAmpersand, 
    chEqual, chPlus, chDollarSign, chComma, chNull
};


// ---------------------------------------------------------------------------
//  Local methods and data
// ---------------------------------------------------------------------------
static const int BUF_LEN = 64;
static XMLCh value1[BUF_LEN+1];

//
// "Scheme"
// "SchemeSpecificPart"
// "Parameters"
// "UserInfo"
// "Host"
// "Port"
// "Path"
// "Query"
// "Fragment"
//
static const XMLCh errMsg_SCHEME[] =
{
    chLatin_S, chLatin_c, chLatin_h, chLatin_e,
    chLatin_m, chLatin_e, chNull
};

static const XMLCh errMsg_SCHEMESPART[] =
{
    chLatin_S, chLatin_c, chLatin_h, chLatin_e, chLatin_m, chLatin_e,
    chLatin_S, chLatin_p, chLatin_e, chLatin_c, chLatin_i, chLatin_f,
    chLatin_i, chLatin_c, chLatin_P, chLatin_a, chLatin_r, chLatin_t,
    chNull
};

static const XMLCh errMsg_PARAMS[] =
{
    chLatin_P, chLatin_a, chLatin_r, chLatin_a, chLatin_m,
    chLatin_e, chLatin_t, chLatin_e, chLatin_r, chLatin_s, chNull
};

static const XMLCh errMsg_USERINFO[] =
{
    chLatin_U, chLatin_s, chLatin_e, chLatin_r,
    chLatin_i, chLatin_n, chLatin_f, chLatin_o, chNull
};

static const XMLCh errMsg_HOST[] =
{
    chLatin_H, chLatin_o, chLatin_s, chLatin_t, chNull
};

static const XMLCh errMsg_PORT[] =
{
    chLatin_P, chLatin_o, chLatin_r, chLatin_t, chNull
};

static const XMLCh errMsg_PATH[] =
{
    chLatin_P, chLatin_a, chLatin_t, chLatin_h, chNull
};

static const XMLCh errMsg_QUERY[] =
{
    chLatin_Q, chLatin_u, chLatin_e, chLatin_r, chLatin_y, chNull
};

static const XMLCh errMsg_FRAGMENT[] =
{
    chLatin_F, chLatin_r, chLatin_a, chLatin_g,
    chLatin_m, chLatin_e, chLatin_n, chLatin_t, chNull
};

//
//  "//"
//  "/"
//  "./"
//  "/."
//  "/../"
//  "/.."
//
static const XMLCh DOUBLE_SLASH[] =
{
    chForwardSlash, chForwardSlash, chNull
};

static const XMLCh SINGLE_SLASH[] =
{
    chForwardSlash, chNull
};

static const XMLCh SLASH_DOT_SLASH[] =
{
    chForwardSlash, chPeriod, chForwardSlash, chNull
};

static const XMLCh SLASH_DOT[] =
{
    chForwardSlash, chPeriod, chNull
};

static const XMLCh SLASH_DOTDOT_SLASH[] =
{
    chForwardSlash, chPeriod, chPeriod, chForwardSlash, chNull
};

static const XMLCh SLASH_DOTDOT[] =
{
    chForwardSlash, chPeriod, chPeriod, chNull
};

//
//  ":/?#"
//
// REVISIT: why?
static const XMLCh SCHEME_SEPARATORS[] =
{
    chColon, chForwardSlash, chQuestion, chPound, chNull
};

//
//  "?#"
//
static const XMLCh PATH_SEPARATORS[] =
{
    chQuestion, chPound, chNull
};

// ---------------------------------------------------------------------------
//  XMLUri: Constructors and Helper methods
// ---------------------------------------------------------------------------
// ctor# 2
XMLUri::XMLUri(const XMLCh* const uriSpec,
               MemoryManager* const manager)
: fScheme(0)
, fUserInfo(0)
, fHost(0)
, fPort(-1)
, fPath(0)
, fQueryString(0)
, fFragment(0)
, fURIText(0)
, fMemoryManager(manager)
{
    try {
        initialize((XMLUri *)0, uriSpec);
    }
    catch (...)
    {
        cleanUp();
        throw;
    }
}

// ctor# 7 relative ctor
XMLUri::XMLUri(const XMLUri* const      baseURI
              , const XMLCh* const   uriSpec
              , MemoryManager* const manager)
: fScheme(0)
, fUserInfo(0)
, fHost(0)
, fPort(-1)
, fPath(0)
, fQueryString(0)
, fFragment(0)
, fURIText(0)
, fMemoryManager(manager)
{
    try {
        initialize(baseURI, uriSpec);
    }
    catch (...)
    {
        cleanUp();
        throw;
    }
}

//Copy constructor
XMLUri::XMLUri(const XMLUri& toCopy)
: fScheme(0)
, fUserInfo(0)
, fHost(0)
, fPort(-1)
, fPath(0)
, fQueryString(0)
, fFragment(0)
, fURIText(0)
, fMemoryManager(toCopy.fMemoryManager)
{
    try {
        initialize(toCopy);
    }
    catch (...)
    {
        cleanUp();
        throw;
    }

}

XMLUri& XMLUri::operator=(const XMLUri& toAssign)
{
    try {
        initialize(toAssign);
    }
    catch (...)
    {
        cleanUp();
        throw;
    }
    return *this;
}

XMLUri::~XMLUri()
{
    cleanUp();
}

void XMLUri::cleanUp()
{
    if (fScheme)
        fMemoryManager->deallocate(fScheme);//delete[] fScheme;

    if (fUserInfo)
        fMemoryManager->deallocate(fUserInfo);//delete[] fUserInfo;

    if (fHost)
        fMemoryManager->deallocate(fHost);//delete[] fHost;

    if (fPath)
        fMemoryManager->deallocate(fPath);//delete[] fPath;

    if (fQueryString)
        fMemoryManager->deallocate(fQueryString);//delete[] fQueryString;

    if (fFragment)
        fMemoryManager->deallocate(fFragment);//delete[] fFragment;

    fMemoryManager->deallocate(fURIText);//delete[] fURIText;
}

void XMLUri::initialize(const XMLUri& toCopy)
{
    //
    // assuming that all fields from the toCopy are valid,
    // therefore need NOT to go through various setXXX() methods
    //
    fMemoryManager = toCopy.fMemoryManager;
    fScheme = XMLString::replicate(toCopy.fScheme, fMemoryManager);
	fUserInfo = XMLString::replicate(toCopy.fUserInfo, fMemoryManager);
	fHost = XMLString::replicate(toCopy.fHost, fMemoryManager);
	fPort = toCopy.fPort;
	fPath = XMLString::replicate(toCopy.fPath, fMemoryManager);
	fQueryString = XMLString::replicate(toCopy.fQueryString, fMemoryManager);
	fFragment = XMLString::replicate(toCopy.fFragment, fMemoryManager);
}

void XMLUri::initialize(const XMLUri* const baseURI
                      , const XMLCh*  const uriSpec)
{

    // get a trimmed version of uriSpec
    // uriSpec will NO LONGER be used in this function.
    //
    XMLCh* trimedUriSpec = XMLString::replicate(uriSpec, fMemoryManager);
    XMLString::trim(trimedUriSpec);
    ArrayJanitor<XMLCh> janName(trimedUriSpec, fMemoryManager);
    int trimedUriSpecLen = XMLString::stringLen(trimedUriSpec);

    if ( !baseURI &&
        (!trimedUriSpec || trimedUriSpecLen == 0))
    {
        ThrowXML1(MalformedURLException
               , XMLExcepts::XMLNUM_URI_Component_Empty
               , errMsg_PARAMS);
    }

	// just make a copy of the base if spec is empty
	if (!trimedUriSpec || trimedUriSpecLen == 0)
    {
        initialize(*baseURI);
        return;
	}

	int index = 0;
	bool foundScheme = false;

	// Check for scheme, which must be before `/', '?' or '#'. 
	// Also handle names with DOS drive letters ('D:'), 
	// so 1-character schemes are not allowed.
        int colonIdx = XMLString::indexOf(trimedUriSpec, chColon);
        int slashIdx = XMLString::indexOf(trimedUriSpec, chForwardSlash);
        int queryIdx = XMLString::indexOf(trimedUriSpec, chQuestion);
        int fragmentIdx = XMLString::indexOf(trimedUriSpec, chPound);

        if ((colonIdx < 2) ||
            (colonIdx > slashIdx && slashIdx != -1) ||
            (colonIdx > queryIdx && queryIdx != -1) ||
            (colonIdx > fragmentIdx && fragmentIdx != -1))
        {
            // A standalone base is a valid URI according to spec
            if ( colonIdx == 0 || (!baseURI && fragmentIdx != 0) )
            {
                ThrowXML(MalformedURLException, XMLExcepts::XMLNUM_URI_No_Scheme);
            }
        }
        else
        {
            foundScheme = true;
            initializeScheme(trimedUriSpec);
            index = XMLString::stringLen(fScheme)+1;
        }

    // It's an error if we stop here
    if (index == trimedUriSpecLen || (foundScheme && (trimedUriSpec[index] == chPound)))
    {
        ThrowXML1(MalformedURLException
                , XMLExcepts::XMLNUM_URI_Component_Empty
                , errMsg_PATH);
    }

	// two slashes means generic URI syntax, so we get the authority
    XMLCh* authUriSpec = (XMLCh*) fMemoryManager->allocate
    (
        (trimedUriSpecLen+1) * sizeof(XMLCh)
    );//new XMLCh[trimedUriSpecLen+1];
    ArrayJanitor<XMLCh> authName(authUriSpec, fMemoryManager);
    XMLString::subString(authUriSpec, trimedUriSpec, index, trimedUriSpecLen);

    if (((index+1) < trimedUriSpecLen) &&
        XMLString::startsWith(authUriSpec, DOUBLE_SLASH))
    {
        index += 2;
        int startPos = index;

        // get authority - everything up to path, query or fragment
        XMLCh testChar;
        while (index < trimedUriSpecLen)
        {
            testChar = trimedUriSpec[index];
            if (testChar == chForwardSlash ||
                testChar == chQuestion     ||
                testChar == chPound         )
            {
                break;
            }

            index++;
        }

        // if we found authority, parse it out, otherwise we set the
        // host to empty string
        if (index > startPos)
        {
            XMLString::subString(authUriSpec, trimedUriSpec, startPos, index);
            initializeAuthority(authUriSpec);
        }
        else
        {
            //fHost = 0;
            setHost(0);
        }
    }

    // we need to check if index has exceed the lenght or not
    if (index >= trimedUriSpecLen)
        return;

    XMLCh* pathUriSpec = (XMLCh*) fMemoryManager->allocate
    (
        (trimedUriSpecLen+1) * sizeof(XMLCh)
    );//new XMLCh[trimedUriSpecLen+1];
    ArrayJanitor<XMLCh> pathUriSpecName(pathUriSpec, fMemoryManager);
    XMLString::subString(pathUriSpec, trimedUriSpec, index, trimedUriSpecLen);

	initializePath(pathUriSpec);

	// Resolve relative URI to base URI - see RFC 2396 Section 5.2
	// In some cases, it might make more sense to throw an exception
	// (when scheme is specified is the string spec and the base URI
	// is also specified, for example), but we're just following the
	// RFC specifications
	if ( baseURI )
    {
        // check to see if this is the current doc - RFC 2396 5.2 #2
        // note that this is slightly different from the RFC spec in that
        // we don't include the check for query string being null
        // - this handles cases where the urispec is just a query
        // string or a fragment (e.g. "?y" or "#s") -
        // see <http://www.ics.uci.edu/~fielding/url/test1.html> which
        // identified this as a bug in the RFC
        if ((!fPath || !*fPath) &&
            fScheme == 0 &&
            fHost == 0)
        {
            fScheme = XMLString::replicate(baseURI->getScheme(), fMemoryManager);
            fMemoryManager->deallocate(fUserInfo);//delete [] fUserInfo;
            fUserInfo = XMLString::replicate(baseURI->getUserInfo(), fMemoryManager);
            fHost = XMLString::replicate(baseURI->getHost(), fMemoryManager);
            fPort = baseURI->getPort();
            fMemoryManager->deallocate(fPath);//delete [] fPath;
            fPath = XMLString::replicate(baseURI->getPath(), fMemoryManager);

            if ( !fQueryString )
            {
                fQueryString = XMLString::replicate(baseURI->getQueryString(), fMemoryManager);
            }
            return;
        }

        // check for scheme - RFC 2396 5.2 #3
        // if we found a scheme, it means absolute URI, so we're done
        if (fScheme == 0)
        {
            fScheme = XMLString::replicate(baseURI->getScheme(), fMemoryManager);
        }
        else
        {
            return;
        }

        // check for authority - RFC 2396 5.2 #4
        // if we found a host, then we've got a network path, so we're done
        if (fHost == 0)
        {
            fMemoryManager->deallocate(fUserInfo);//delete [] fUserInfo;
            fUserInfo = XMLString::replicate(baseURI->getUserInfo(), fMemoryManager);
            fHost = XMLString::replicate(baseURI->getHost(), fMemoryManager);
            fPort = baseURI->getPort();
        }
        else
        {
            return;
        }

        // check for absolute path - RFC 2396 5.2 #5
        if ((fPath && *fPath) &&
            XMLString::startsWith(fPath, SINGLE_SLASH))
        {
            return;
        }

        // if we get to this point, we need to resolve relative path
        // RFC 2396 5.2 #6

        XMLCh* basePath = XMLString::replicate(baseURI->getPath(), fMemoryManager);
        ArrayJanitor<XMLCh> basePathName(basePath, fMemoryManager);

        int bufLen = trimedUriSpecLen+XMLString::stringLen(fPath)+XMLString::stringLen(basePath)+1;
        XMLCh* path = (XMLCh*) fMemoryManager->allocate(bufLen * sizeof(XMLCh));//new XMLCh[bufLen];
        ArrayJanitor<XMLCh> pathName(path, fMemoryManager);
        path[0] = 0;

        XMLCh* tmp1 = (XMLCh*) fMemoryManager->allocate(bufLen * sizeof(XMLCh));//new XMLCh[bufLen];
        ArrayJanitor<XMLCh> tmp1Name(tmp1, fMemoryManager);
        XMLCh* tmp2 = (XMLCh*) fMemoryManager->allocate(bufLen * sizeof(XMLCh));//new XMLCh[bufLen];
        ArrayJanitor<XMLCh> tmp2Name(tmp2, fMemoryManager);

        // 6a - get all but the last segment of the base URI path
        if (basePath)
        {
            int lastSlash = XMLString::lastIndexOf(basePath, chForwardSlash);
            if (lastSlash != -1)
            {
                XMLString::subString(path, basePath, 0, lastSlash+1);
            }
        }

        // 6b - append the relative URI path
        XMLString::catString(path, fPath);

        // 6c - remove all "./" where "." is a complete path segment
        index = -1;
        while ((index = XMLString::patternMatch(path, SLASH_DOT_SLASH)) != -1)
        {
            XMLString::subString(tmp1, path, 0, index);
            XMLString::subString(tmp2, path, index+2, XMLString::stringLen(path));

            path[0] = 0;
            XMLString::catString(path, tmp1);
            XMLString::catString(path, tmp2);
        }

        // 6d - remove "." if path ends with "." as a complete path segment
        if (XMLString::endsWith(path, SLASH_DOT))
        {
            path[XMLString::stringLen(path) - 1] = chNull;
        }

        // 6e - remove all "<segment>/../" where "<segment>" is a complete
        // path segment not equal to ".."
        index = -1;
        int segIndex = -1;
		int offset = 1;

        while ((index = XMLString::patternMatch(&(path[offset]), SLASH_DOTDOT_SLASH)) != -1)
        {
			// Undo offset
			index += offset;

			// Find start of <segment> within substring ending at found point.
			XMLString::subString(tmp1, path, 0, index-1);
			segIndex = XMLString::lastIndexOf(tmp1, chForwardSlash);

			// Ensure <segment> exists and != ".."
            if (segIndex != -1                &&
                (path[segIndex+1] != chPeriod ||
                 path[segIndex+2] != chPeriod ||
				 segIndex + 3 != index))
            {

                XMLString::subString(tmp1, path, 0, segIndex);
                XMLString::subString(tmp2, path, index+3, XMLString::stringLen(path));

                path[0] = 0;
                XMLString::catString(path, tmp1);
                XMLString::catString(path, tmp2);

                offset = (segIndex == 0 ? 1 : segIndex);
            }
            else
            {
                offset += 4;
            }
        }// while

        // 6f - remove ending "<segment>/.." where "<segment>" is a
        // complete path segment
        if (XMLString::endsWith(path, SLASH_DOTDOT))
        {
			// Find start of <segment> within substring ending at found point.
            index = XMLString::stringLen(path) - 3;
			XMLString::subString(tmp1, path, 0, index-1);
			segIndex = XMLString::lastIndexOf(tmp1, chForwardSlash);

            if (segIndex != -1                &&
                (path[segIndex+1] != chPeriod ||
                 path[segIndex+2] != chPeriod ||
				 segIndex + 3 != index))
            {
                path[segIndex+1] = chNull;
            }
        }

        if (getPath())
            fMemoryManager->deallocate(fPath);//delete [] fPath;

        fPath = XMLString::replicate(path, fMemoryManager);

    }
}

// ---------------------------------------------------------------------------
//  Components initialization
// ---------------------------------------------------------------------------

//
// authority     = server | reg_name
// server        = [ [ userinfo "@" ] hostport ]
// hostport      = host [ ":" port ]
//
// reg_name      = 1*( unreserved | escaped | "$" | "," |
//                    ";" | ":" | "@" | "&" | "=" | "+" )
//
// userinfo      = *( unreserved | escaped |
//                 ";" | ":" | "&" | "=" | "+" | "$" | "," )
//

void XMLUri::initializeAuthority(const XMLCh* const uriSpec)
{

    int index = 0;
    int start = 0;
    const int end = XMLString::stringLen(uriSpec);

    //
    // server = [ [ userinfo "@" ] hostport ]
    // userinfo is everything up @,
    //
    XMLCh* userinfo = (XMLCh*) fMemoryManager->allocate
    (
        (end+1) * sizeof(XMLCh)
    );//new XMLCh[end+1];
    ArrayJanitor<XMLCh> userName(userinfo, fMemoryManager);
    index = XMLString::indexOf(&(uriSpec[start]), chAt);

    if ( index != -1)
    {
        XMLString::subString(userinfo, &(uriSpec[start]), 0, index);
        index++; // skip the @
        start += index;
    }
    else
    {
        XMLString::copyString(userinfo, XMLUni::fgZeroLenString);
    }

    //
    // hostport = host [ ":" port ]
    // host is everything up to ':', or up to 
    // and including ']' if followed by ':'.
    //
    XMLCh* host = (XMLCh*) fMemoryManager->allocate
    (
        (end+1) * sizeof(XMLCh)
    );//new XMLCh[end+1];
    ArrayJanitor<XMLCh> hostName(host, fMemoryManager);
    
    // Search for port boundary.
    if (start < end && uriSpec[start] == chOpenSquare)
    {
    	index = XMLString::indexOf(&(uriSpec[start]), chCloseSquare);
    	if (index != -1)
    	{
            // skip the ']'
            index = ((start + index + 1) < end 
              && uriSpec[start + index + 1] == chColon) ? index+1 : -1;
    	}
    }
    else
    {
        index = XMLString::indexOf(&(uriSpec[start]), chColon);
    }

    if ( index != -1 )
    {
        XMLString::subString(host, &(uriSpec[start]), 0, index);
        index++;  // skip the :
        start +=index;
    }
    else
    {
        XMLString::subString(host, &(uriSpec[start]), 0, end-start);
        start = end;
    }

    // port is everything after ":"

    XMLCh* portStr = (XMLCh*) fMemoryManager->allocate
    (
        (end+1) * sizeof(XMLCh)
    );//new XMLCh[end+1];
    ArrayJanitor<XMLCh> portName(portStr, fMemoryManager);
    int port = -1;

    if ((host && *host) &&   // non empty host
        (index != -1)                    &&   // ":" found
        (start < end)                     )   // ":" is not the last
    {
        XMLString::subString(portStr, &(uriSpec[start]), 0, end-start);

        if (portStr && *portStr)
        {
            try
            {
                port = XMLString::parseInt(portStr);
            }
            catch (...)
            {
                throw;
            }
        }
    } // if > 0

    // The order is important, do not change
    //
    setHost(host);
    setPort(port);
    setUserInfo(userinfo);
}

// scheme = alpha *( alpha | digit | "+" | "-" | "." )
void XMLUri::initializeScheme(const XMLCh* const uriSpec)
{
    const XMLCh* tmpPtr = XMLString::findAny(uriSpec, SCHEME_SEPARATORS);

    if ( !tmpPtr )
    {
        ThrowXML(MalformedURLException, XMLExcepts::XMLNUM_URI_No_Scheme);
    }
	else
    {
        XMLCh* scheme = (XMLCh*) fMemoryManager->allocate
        (
            (XMLString::stringLen(uriSpec) + 1) * sizeof(XMLCh)
        );//new XMLCh[XMLString::stringLen(uriSpec)+1];
        ArrayJanitor<XMLCh> tmpName(scheme, fMemoryManager);
        XMLString::subString(scheme, uriSpec, 0, (tmpPtr - uriSpec));
        setScheme(scheme);
	}

}

void XMLUri::initializePath(const XMLCh* const uriSpec)
{
    if ( !uriSpec )
    {
        ThrowXML1(MalformedURLException
                , XMLExcepts::XMLNUM_URI_Component_Empty
                , errMsg_PATH);
    }

    int index = 0;
    int start = 0;
    int end = XMLString::stringLen(uriSpec);
    XMLCh testChar;

    // path - everything up to query string or fragment
    if (start < end)
    {
        // RFC 2732 only allows '[' and ']' to appear in the opaque part.
        if (!getScheme() || uriSpec[start] == chForwardSlash)
        {
            // Scan path.
            // abs_path = "/"  path_segments
            // rel_path = rel_segment [ abs_path ]
            while (index < end)
            {
                testChar = uriSpec[index];
                if (testChar == chQuestion || testChar == chPound)
                {
                    break;
                }

                // check for valid escape sequence
                if (testChar == chPercent)
                {
                    if (index+2 >= end ||
                        !XMLString::isHex(uriSpec[index+1]) ||
                        !XMLString::isHex(uriSpec[index+2]))
                    {
                        XMLString::moveChars(value1, &(uriSpec[index]), 3);
                        value1[3] = chNull;
                        ThrowXML2(MalformedURLException
                                , XMLExcepts::XMLNUM_URI_Component_Invalid_EscapeSequence
                                , errMsg_PATH
                                , value1);
                    }
                }
                else if (!isUnreservedCharacter(testChar) &&
                         !isPathCharacter(testChar))
                {
                    value1[0] = testChar;
                    value1[1] = chNull;
                    ThrowXML2(MalformedURLException
                            , XMLExcepts::XMLNUM_URI_Component_Invalid_Char
                            , errMsg_PATH
                            , value1);
                }

                index++;
            }//while (index < end)
        }
        else
        {
            // Scan opaque part.
            // opaque_part = uric_no_slash *uric
            while (index < end)
            {
                testChar = uriSpec[index];
                if (testChar == chQuestion || testChar == chPound)
                {
                    break;
                }

                // check for valid escape sequence
                if (testChar == chPercent)
                {
                    if (index+2 >= end ||
                        !XMLString::isHex(uriSpec[index+1]) ||
                        !XMLString::isHex(uriSpec[index+2]))
                    {
                        XMLString::moveChars(value1, &(uriSpec[index]), 3);
                        value1[3] = chNull;
                        ThrowXML2(MalformedURLException
                                , XMLExcepts::XMLNUM_URI_Component_Invalid_EscapeSequence
                                , errMsg_PATH
                                , value1);
                    }
                }
                // If the scheme specific part is opaque, it can contain '['
                // and ']'. uric_no_slash wasn't modified by RFC 2732, which
                // I've interpreted as an error in the spec, since the 
                // production should be equivalent to (uric - '/'), and uric
                // contains '[' and ']'.
                else if (!isUnreservedCharacter(testChar) &&
                         !isReservedCharacter(testChar))
                {
                    value1[0] = testChar;
                    value1[1] = chNull;
                    ThrowXML2(MalformedURLException
                            , XMLExcepts::XMLNUM_URI_Component_Invalid_Char
                            , errMsg_PATH
                            , value1);
                }

                index++;
            }//while (index < end)     	
        }
    } //if (start < end)

    if (getPath())
    {
        fMemoryManager->deallocate(fPath);//delete [] fPath;
    }

    fPath = (XMLCh*) fMemoryManager->allocate((index+1) * sizeof(XMLCh));//new XMLCh[index+1];
    XMLString::subString(fPath, uriSpec, start, index);

    // query - starts with ? and up to fragment or end
    if (testChar == chQuestion)
    {
        index++;
        start = index;
        while (index < end)
        {
            testChar = uriSpec[index];
            if (testChar == chPound)
            {
                break;
            }

            if (testChar == chPercent)
            {
                if (index+2 >= end ||
                    !XMLString::isHex(uriSpec[index+1]) ||
                    !XMLString::isHex(uriSpec[index+2]))
                {
                    XMLString::moveChars(value1, &(uriSpec[index]), 3);
                    value1[3] = chNull;
                    ThrowXML2(MalformedURLException
                            , XMLExcepts::XMLNUM_URI_Component_Invalid_EscapeSequence
                            , errMsg_QUERY
                            , value1);
                }
            }
            else if (!isUnreservedCharacter(testChar) &&
                     !isReservedCharacter(testChar))
            {
                value1[0] = testChar;
                value1[1] = chNull;
                ThrowXML2(MalformedURLException
                        , XMLExcepts::XMLNUM_URI_Component_Invalid_Char
                        , errMsg_QUERY
                        , value1);
            }
            index++;
        }

        if (getQueryString())
        {
            fMemoryManager->deallocate(fQueryString);//delete [] fQueryString;
        }

        fQueryString = (XMLCh*) fMemoryManager->allocate
        (
            (index - start + 1) * sizeof(XMLCh)
        );//new XMLCh[index - start + 1];
        XMLString::subString(fQueryString, uriSpec, start, index);
    }

    // fragment - starts with #
    if (testChar == chPound)
    {
        index++;
        start = index;
        while (index < end)
        {
            testChar = uriSpec[index];

            if (testChar == chPercent)
            {
                if (index+2 >= end ||
                    !XMLString::isHex(uriSpec[index+1]) ||
                    !XMLString::isHex(uriSpec[index+2]))
                {
                    XMLString::moveChars(value1, &(uriSpec[index]), 3);
                    value1[3] = chNull;
                    ThrowXML2(MalformedURLException
                            , XMLExcepts::XMLNUM_URI_Component_Invalid_EscapeSequence
                            , errMsg_FRAGMENT
                            , value1);
                }
            }
            else if (!isUnreservedCharacter(testChar) &&
                     !isReservedCharacter(testChar))
            {
                value1[0] = testChar;
                value1[1] = chNull;
                ThrowXML2(MalformedURLException
                        , XMLExcepts::XMLNUM_URI_Component_Invalid_Char
                        , errMsg_FRAGMENT
                        , value1);
            }

            index++;

        }

        if (getFragment())
            fMemoryManager->deallocate(fFragment);//delete [] fFragment;

        //make sure that there is something following the '#'
        if (index > start)
        {
            fFragment = (XMLCh*) fMemoryManager->allocate
            (
                (index - start + 1) * sizeof(XMLCh)
            );//new XMLCh[index - start + 1];
            XMLString::subString(fFragment, uriSpec, start, index);
        }
        else
        {
            // RFC 2396, 4.0. URI Reference
            // URI-reference = [absoulteURI | relativeURI] [# fragment]
            //
            // RFC 2396, 4.1. Fragment Identifier
            // fragment = *uric
            //
            // empty fragment is valid
            fFragment = 0;
        }
    }

}

// ---------------------------------------------------------------------------
//  Setter
// ---------------------------------------------------------------------------
void XMLUri::setScheme(const XMLCh* const newScheme)
{
    if ( !newScheme )
    {
        ThrowXML1(MalformedURLException
                , XMLExcepts::XMLNUM_URI_Component_Set_Null
                , errMsg_SCHEME);
    }

    if (!isConformantSchemeName(newScheme))
    {
        ThrowXML2(MalformedURLException
                , XMLExcepts::XMLNUM_URI_Component_Not_Conformant
                , errMsg_SCHEME
                , newScheme);
    }

    if (getScheme())
    {
        fMemoryManager->deallocate(fScheme);//delete [] fScheme;
    }

    fScheme = XMLString::replicate(newScheme, fMemoryManager);
    XMLString::lowerCase(fScheme);
}

//
// server = [ [ userinfo "@" ] hostport ]
// hostport = host [":" port]
//
// setUserInfo(), setHost() and setPort() are closely related
// three methods, in a word, userinfo and port has dependency
// on host.
//
// if host is not present, userinfo must be null and port = -1
//
void XMLUri::setUserInfo(const XMLCh* const newUserInfo)
{
    if ( newUserInfo &&
         !getHost()    )
    {
        ThrowXML2(MalformedURLException
                , XMLExcepts::XMLNUM_URI_NullHost
                , errMsg_USERINFO
                , newUserInfo);
    }

    try
    {
        isConformantUserInfo(newUserInfo);
    }
    catch (...)
    {
        throw;
    }

    if (getUserInfo())
    {
        fMemoryManager->deallocate(fUserInfo);//delete [] fUserInfo;
    }

    //sometimes we get passed a empty string rather than a null.
    //Other procedures rely on it being null
    if(newUserInfo && *newUserInfo) {
        fUserInfo = XMLString::replicate(newUserInfo, fMemoryManager);
    }

}

void XMLUri::setHost(const XMLCh* const newHost)
{
    if ( !newHost )
    {
        if (getHost())
            fMemoryManager->deallocate(fHost);//delete [] fHost;

        fHost = 0;
        setUserInfo(0);
        setPort(-1);

        return;
    }

    if (!isWellFormedAddress(newHost))
    {
        ThrowXML2(MalformedURLException
                , XMLExcepts::XMLNUM_URI_Component_Not_Conformant
                , errMsg_HOST
                , newHost);
    }

    if (getHost())
    {
        fMemoryManager->deallocate(fHost);//delete [] fHost;
    }

    fHost = XMLString::replicate(newHost, fMemoryManager);
}

void XMLUri::setPort(int newPort)
{
    if (newPort >= 0 && newPort <= 65535)
    {
        if (!getHost())
        {
            XMLString::binToText(newPort, value1, BUF_LEN, 10);
            ThrowXML2(MalformedURLException
                    , XMLExcepts::XMLNUM_URI_NullHost
                    , errMsg_PORT
                    , value1);
        }
    }
    else if (newPort != -1)
    {
        XMLString::binToText(newPort, value1, BUF_LEN, 10);
        ThrowXML1(MalformedURLException
                , XMLExcepts::XMLNUM_URI_PortNo_Invalid
                , value1);
    }

    fPort = newPort;

}

//
// setPath(), setQueryString() and setFragment() are closely
// related three methods as well.
//
void XMLUri::setPath(const XMLCh* const newPath)
{
    if (!newPath)
    {
        if (getPath())
            fMemoryManager->deallocate(fPath);//delete [] fPath;

        fPath = 0;
        setQueryString(0);
        setFragment(0);
    }
    else
    {
      initializePath(newPath);
    }
  }

//
// fragment = *uric
//
void XMLUri::setFragment(const XMLCh* const newFragment)
{
	if ( !newFragment )
    {
        if (getFragment())
            fMemoryManager->deallocate(fFragment);//delete [] fFragment;

        fFragment = 0;
	}
	else if (!isGenericURI())
    {
        ThrowXML2(MalformedURLException
                , XMLExcepts::XMLNUM_URI_Component_for_GenURI_Only
                , errMsg_FRAGMENT
                , newFragment);
	}
	else if ( !getPath() )
    {
        ThrowXML2(MalformedURLException
               , XMLExcepts::XMLNUM_URI_NullPath
               , errMsg_FRAGMENT
               , newFragment);
	}
	else if (!isURIString(newFragment))
    {
        ThrowXML1(MalformedURLException
                , XMLExcepts::XMLNUM_URI_Component_Invalid_Char
                , errMsg_FRAGMENT);
	}
	else
    {
        if (getFragment())
        {
            fMemoryManager->deallocate(fFragment);//delete [] fFragment;
        }

        fFragment = XMLString::replicate(newFragment, fMemoryManager);
	}
}

//
// query = *uric
//
void XMLUri::setQueryString(const XMLCh* const newQueryString)
{
	if ( !newQueryString )
    {
        if (getQueryString())
            fMemoryManager->deallocate(fQueryString);//delete [] fQueryString;

        fQueryString = 0;
	}
	else if (!isGenericURI())
    {
        ThrowXML2(MalformedURLException
                , XMLExcepts::XMLNUM_URI_Component_for_GenURI_Only
                , errMsg_QUERY
                , newQueryString);
	}
	else if ( !getPath() )
    {
        ThrowXML2(MalformedURLException
                , XMLExcepts::XMLNUM_URI_NullPath
                , errMsg_QUERY
                , newQueryString);
	}
	else if (!isURIString(newQueryString))
    {
        ThrowXML2(MalformedURLException
               , XMLExcepts::XMLNUM_URI_Component_Invalid_Char
               , errMsg_QUERY
               , newQueryString);
	}
	else
    {
        if (getQueryString())
        {
            fMemoryManager->deallocate(fQueryString);//delete [] fQueryString;
        }

        fQueryString = XMLString::replicate(newQueryString, fMemoryManager);
	}
}

// ---------------------------------------------------------------------------
//  XMLUri: Public, static methods
// ---------------------------------------------------------------------------

//
//  scheme = alpha *( alpha | digit | "+" | "-" | "." )
//  alphanum = alpha | digit
//
bool XMLUri::isConformantSchemeName(const XMLCh* const scheme)
{
	if ( !scheme )
        return false;

    const XMLCh* tmpStr = scheme;
    if (!XMLString::isAlpha(*tmpStr))     // first: alpha
        return false;

    // second onwards: ( alpha | digit | "+" | "-" | "." )
    tmpStr++;
    while (*tmpStr)
    {
        if ( !XMLString::isAlphaNum(*tmpStr) &&
             (XMLString::indexOf(SCHEME_CHARACTERS, *tmpStr) == -1))
            return false;

        tmpStr++;
    }

    return true;
}

//
// userinfo = *( unreserved | escaped |
//              ";" | ":" | "&" | "=" | "+" | "$" | "," )
//
void XMLUri::isConformantUserInfo(const XMLCh* const userInfo)
{
	if ( !userInfo )
        return;

    const XMLCh* tmpStr = userInfo;
    while (*tmpStr)
    {
        if ( isUnreservedCharacter(*tmpStr) ||
            (XMLString::indexOf(USERINFO_CHARACTERS, *tmpStr) != -1))
        {
            tmpStr++;
        }
        else if (*tmpStr == chPercent)               // '%'
        {
            if (XMLString::isHex(*(tmpStr+1)) &&     // 1st hex
                XMLString::isHex(*(tmpStr+2))  )     // 2nd hex
            {
                tmpStr+=3;
            }
            else
            {
                value1[0] = chPercent;
                value1[1] = *(tmpStr+1);
                value1[2] = *(tmpStr+2);
                value1[3] = chNull;

                ThrowXML2(MalformedURLException
                        , XMLExcepts::XMLNUM_URI_Component_Invalid_EscapeSequence
                        , errMsg_USERINFO
                        , value1);
            }
        }
        else
        {	
            ThrowXML2(MalformedURLException
                    , XMLExcepts::XMLNUM_URI_Component_Invalid_Char
                    , errMsg_USERINFO
                    , userInfo);
        }
    } //while

    return;
}

//
// uric     = reserved | unreserved | escaped
// escaped  = "%" hex hex
// hex      = digit | "A" | "B" | "C" | "D" | "E" | "F" |
//                    "a" | "b" | "c" | "d" | "e" | "f"
//
bool XMLUri::isURIString(const XMLCh* const uricString)
{
	if (!uricString || !*uricString)
        return false;

    const XMLCh* tmpStr = uricString;
    while (*tmpStr)
    {
        if (isReservedCharacter(*tmpStr)    ||
            isUnreservedCharacter(*tmpStr)   )
        {
            tmpStr++;
        }
        else if (*tmpStr == chPercent)               // '%'
        {
            if (XMLString::isHex(*(tmpStr+1)) &&     // 1st hex
                XMLString::isHex(*(tmpStr+2))  )     // 2nd hex
            {
                tmpStr+=3;
            }
            else
            {
                return false;
            }
        }
        else
        {	
            return false;
        }
    }

    return true;
}

//
//  host          = hostname | IPv4address
//
//  hostname      = *( domainlabel "." ) toplabel [ "." ]
//  domainlabel   = alphanum | alphanum *( alphanum | "-" ) alphanum
//  toplabel      = alpha | alpha *( alphanum | "-" ) alphanum
//
//  IPv4address   = 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT
//
bool XMLUri::isWellFormedAddress(const XMLCh* const addrString)
{
    // Check that we have a non-zero length string.
    if (!addrString || !*addrString)
        return false;
        
    // Get address length.
    int addrStrLen = XMLString::stringLen(addrString);
    
    // Check if the host is a valid IPv6reference.
    if (*addrString == chOpenSquare)
    {
        return isWellFormedIPv6Reference(addrString, addrStrLen);
    }

    //
    // Cannot start with a '.', '-', or end with a '-'.
    //
    if (*addrString == chPeriod ||
        *addrString == chDash ||
        addrString[addrStrLen-1] == chDash)
        return false;

    // rightmost domain label starting with digit indicates IP address
    // since top level domain label can only start with an alpha
    // see RFC 2396 Section 3.2.2
    
    int lastPeriodPos = XMLString::lastIndexOf(addrString, chPeriod);

    // if the string ends with "."
    // get the second last "."
    if (lastPeriodPos + 1 == addrStrLen)
    {
        XMLCh* tmp2 = (XMLCh*) XMLPlatformUtils::fgMemoryManager->allocate
        (
            addrStrLen * sizeof(XMLCh)
        );//new XMLCh[addrStrLen];
        XMLString::subString(tmp2, addrString, 0, lastPeriodPos);
        lastPeriodPos = XMLString::lastIndexOf(tmp2, chPeriod);
        XMLPlatformUtils::fgMemoryManager->deallocate(tmp2);//delete [] tmp2;

        if ( XMLString::isDigit(addrString[lastPeriodPos + 1]))
			return false;
    }

    if (XMLString::isDigit(addrString[lastPeriodPos + 1]))
    {
        return isWellFormedIPv4Address(addrString, addrStrLen);
    } // end of IPv4address
    else
    {
        //
        //  hostname      = *( domainlabel "." ) toplabel [ "." ]
        //  domainlabel   = alphanum | alphanum *( alphanum | "-" ) alphanum
        //  toplabel      = alpha | alpha *( alphanum | "-" ) alphanum
        
        // RFC 2396 states that hostnames take the form described in 
        // RFC 1034 (Section 3) and RFC 1123 (Section 2.1). According
        // to RFC 1034, hostnames are limited to 255 characters.
        if (addrStrLen > 255) {
            return false;
        }
        
        unsigned int labelCharCount = 0;
        
        // domain labels can contain alphanumerics and '-"
        // but must start and end with an alphanumeric
        for (int i = 0; i < addrStrLen; i++)
        {
            if (addrString[i] == chPeriod)
            {
              if (((i > 0)  &&
                   (!XMLString::isAlphaNum(addrString[i-1]))) ||
                  ((i + 1 < addrStrLen) &&
                   (!XMLString::isAlphaNum(addrString[i+1])))  )
                {
                    return false;
                }
                labelCharCount = 0;
            }
            else if (!XMLString::isAlphaNum(addrString[i]) &&
                      addrString[i] != chDash)
            {
                return false;
            }
            // RFC 1034: Labels must be 63 characters or less.
            else if (++labelCharCount > 63) {
                return false;
            }
        } //for
    }

    return true;
}

//
//  RFC 2732 amended RFC 2396 by replacing the definition 
//  of IPv4address with the one defined by RFC 2373.
//
//  IPv4address   = 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT
//
bool XMLUri::isWellFormedIPv4Address(const XMLCh* const addr, const int& length)
{
    int numDots = 0;
    int numDigits = 0;
    
    //
    // IPv4address = 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT
    //
    // make sure that
    // 1) we see only digits and dot separators,
    // 2) that any dot separator is preceded and followed by a digit
    // 3) that we find 3 dots
    // 4) that each segment contains 1 to 3 digits.
    // 5) that each segment is not greater than 255.
    
    for (int i = 0; i < length; ++i)
    {
        if (addr[i] == chPeriod)
        {
            if (((i > 0)  &&
                (!XMLString::isDigit(addr[i-1]))) ||
                ((i + 1 < length) &&
                (!XMLString::isDigit(addr[i+1])))  )
            {
               return false;
            }
            numDigits = 0;
            if (++numDots > 3)
            {
                return false;
            }
        }
        else if (!XMLString::isDigit(addr[i]))
        {
            return false;
        }
        // Check that that there are no more than three digits
        // in this segment.
        else if (++numDigits > 3)
        {
            return false;
        }
        // Check that this segment is not greater than 255.
        else if (numDigits == 3)
        {
            XMLCh first = addr[i-2];
            XMLCh second = addr[i-1];
            XMLCh last = addr[i];
            if (!(first < chDigit_2 || 
                 (first == chDigit_2 && 
                 (second < chDigit_5 || 
                 (second == chDigit_5 && last <= chDigit_5)))))
            {
                return false;
            }
        }
    } //for
    return (numDots == 3);
}

//
//  IPv6reference = "[" IPv6address "]"
//
bool XMLUri::isWellFormedIPv6Reference(const XMLCh* const addr, const int& length)
{
    int index = 1;
    int end = length-1;
    
    // Check if string is a potential match for IPv6reference.
    if (!(length > 2 && addr[0] == chOpenSquare && addr[end] == chCloseSquare))
    {
        return false;
    }
    
    // Counter for the number of 16-bit sections read in the address.
    int counter = 0;
      
    // Scan hex sequence before possible '::' or IPv4 address.
    index = scanHexSequence(addr, index, end, counter);
    if (index == -1) 
    {
        return false;
    }
    // Address must contain 128-bits of information.
    else if (index == end) 
    {
       return (counter == 8);
    }
      
    if (index+1 < end && addr[index] == chColon) 
    {
        if (addr[index+1] == chColon) 
        {
            // '::' represents at least one 16-bit group of zeros.
            if (++counter > 8) 
            {
                return false;
            }
            index += 2;
            // Trailing zeros will fill out the rest of the address.
            if (index == end)
            {
                return true;
            }
        }
        // If the second character wasn't ':', in order to be valid,
        // the remainder of the string must match IPv4Address, 
        // and we must have read exactly 6 16-bit groups.
        else 
        {
            if (counter == 6)
            {
                XMLCh* ipVfour = (XMLCh*) 
                  XMLPlatformUtils::fgMemoryManager->allocate((length+1) * sizeof(XMLCh));
                XMLString::subString(ipVfour, addr, index+1, end);
                ArrayJanitor<XMLCh> janitor(ipVfour, XMLPlatformUtils::fgMemoryManager);
                int newLength = XMLString::stringLen(ipVfour);
                return isWellFormedIPv4Address(ipVfour, newLength);
            }
            else
            {
                return false;
            }
        }
    }
    else 
    {
       return false;
    }
      
    // 3. Scan hex sequence after '::'.
    int prevCount = counter;
    index = scanHexSequence(addr, index, end, counter);
    if (index == -1) 
    {
        return false;
    }
    // If this is the end of the address then
    // we've got 128-bits of information.
    else if (index == end) 
    {
        return true;
    }

    // The address ends in an IPv4 address, or it is invalid. 
    // scanHexSequence has already made sure that we have the right number of bits. 
    XMLCh* ipVfour = 
      (XMLCh*) XMLPlatformUtils::fgMemoryManager->allocate((length+1) * sizeof(XMLCh));
    XMLString::subString(ipVfour, addr, (counter > prevCount) ? index+1 : index, end);
    ArrayJanitor<XMLCh> janitor(ipVfour, XMLPlatformUtils::fgMemoryManager);
    int newLength = XMLString::stringLen(ipVfour);
    return isWellFormedIPv4Address(ipVfour, newLength);
}

//
//  For use with isWellFormedIPv6Reference only.
//
int XMLUri::scanHexSequence (const XMLCh* const addr, int index, int end, int& counter)
{
    XMLCh testChar = chNull;
    int numDigits = 0;
    int start = index;
      
    // Trying to match the following productions:
    // hexseq = hex4 *( ":" hex4)
    // hex4   = 1*4HEXDIG
    for (; index < end; ++index)
    {
      	testChar = addr[index];
      	if (testChar == chColon)
      	{
      	    // IPv6 addresses are 128-bit, so there can be at most eight sections.
      	    if (numDigits > 0 && ++counter > 8)
      	    {
      	        return -1;
      	    }
      	    // This could be '::'.
      	    if (numDigits == 0 || ((index+1 < end) && addr[index+1] == chColon))
      	    {
      	        return index;
      	    }
      	    numDigits = 0;
        }
        // This might be invalid or an IPv4address. If it's potentially an IPv4address,
        // backup to just after the last valid character that matches hexseq.
        else if (!XMLString::isHex(testChar))
        {
            if (testChar == chPeriod && numDigits < 4 && numDigits > 0 && counter <= 6)
            {
                int back = index - numDigits - 1;
                return (back >= start) ? back : start;
            }
            return -1;
        }
        // There can be at most 4 hex digits per group.
        else if (++numDigits > 4)
        {
            return -1;
        }
    }
    return (numDigits > 0 && ++counter <= 8) ? end : -1;
}

bool XMLUri::isGenericURI()
{
    return (getHost() != 0);
}


//
//  This method will take the broken out parts of the URI and build up the
//  full text. We don't do this unless someone asks us to, since its often
//  never required.
//
void XMLUri::buildFullText()
{
    // Calculate the worst case size of the buffer required
    unsigned int bufSize = XMLString::stringLen(fScheme) + 1
                           + XMLString::stringLen(fFragment) + 1
                           + XMLString::stringLen(fHost) + 2
                           + XMLString::stringLen(fPath)
                           + XMLString::stringLen(fQueryString) + 1
                           + XMLString::stringLen(fUserInfo) + 1
                           + 32;

    // Clean up the existing buffer and allocate another
    fMemoryManager->deallocate(fURIText);//delete [] fURIText;
    fURIText = (XMLCh*) fMemoryManager->allocate(bufSize * sizeof(XMLCh));//new XMLCh[bufSize];
    *fURIText = 0;

    XMLCh* outPtr = fURIText;
    if (fScheme != 0)
    {
        XMLString::catString(fURIText, getScheme());
        outPtr += XMLString::stringLen(fURIText);
        *outPtr++ = chColon;
        *outPtr++ = chForwardSlash;
        *outPtr++ = chForwardSlash;
    }

    if (fUserInfo)
    {
        XMLString::copyString(outPtr, fUserInfo);
        outPtr += XMLString::stringLen(fUserInfo);


        /*REVISIT dont have password field in uri - is this right??
        if (fPassword)
        {
            *outPtr++ = chColon;
            XMLString::copyString(outPtr, fPassword);
            outPtr += XMLString::stringLen(fPassword);
        }
        */
        *outPtr++ = chAt;
    }

    if (fHost)
    {
        XMLString::copyString(outPtr, fHost);
        outPtr += XMLString::stringLen(fHost);

        //
        //  If the port is -1, then we don't put it in. Else we need
        //  to because it was explicitly provided.
        //
        if (fPort != -1)
        {
            *outPtr++ = chColon;

            XMLCh tmpBuf[16];
            XMLString::binToText(fPort, tmpBuf, 16, 10);
            XMLString::copyString(outPtr, tmpBuf);
            outPtr += XMLString::stringLen(tmpBuf);
        }
    }

    if (fPath)
    {
        XMLString::copyString(outPtr, fPath);
        outPtr += XMLString::stringLen(fPath);
    }

    if (fQueryString)
    {
        *outPtr++ = chQuestion;
        XMLString::copyString(outPtr, fQueryString);
        outPtr += XMLString::stringLen(fQueryString);
    }

    if (fFragment)
    {
        *outPtr++ = chPound;
        XMLString::copyString(outPtr, fFragment);
        outPtr += XMLString::stringLen(fFragment);
    }

    // Cap it off in case the last op was not a string copy
    *outPtr = 0;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(XMLUri)

void XMLUri::serialize(XSerializeEngine& serEng)
{

    if (serEng.isStoring())
    {

        serEng.writeString(fScheme);
        serEng.writeString(fUserInfo);
        serEng.writeString(fHost);

        serEng<<fPort;

        serEng.writeString(fPath);
        serEng.writeString(fQueryString);
        serEng.writeString(fFragment);
        serEng.writeString(fURIText);
    }
    else
    {
        serEng.readString(fScheme);
        serEng.readString(fUserInfo);
        serEng.readString(fHost);

        serEng>>fPort;

        serEng.readString(fPath);
        serEng.readString(fQueryString);
        serEng.readString(fFragment);
        serEng.readString(fURIText);
    }

}

XMLUri::XMLUri(MemoryManager* const manager)
: fScheme(0)
, fUserInfo(0)
, fHost(0)
, fPort(-1)
, fPath(0)
, fQueryString(0)
, fFragment(0)
, fURIText(0)
, fMemoryManager(manager)
{
}

XERCES_CPP_NAMESPACE_END

