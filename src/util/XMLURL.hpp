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
 * Revision 1.3  2000/02/24 20:05:26  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/06 07:48:06  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1  2000/01/15 01:26:17  rahulj
 * Added support for HTTP to the parser using libWWW 5.2.8.
 * Renamed URL.[ch]pp to XMLURL.[ch]pp and like wise for the class name.
 * Only tested under NT 4.0 SP 5.
 * Removed URL.hpp from files where it was not used.
 *
 * Revision 1.3  2000/01/12 00:16:22  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.2  1999/11/23 01:49:56  rahulj
 * Cannot use class qualifier in class defn. CC under HPUX is happy.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:22  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:16  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(XMLURL_HPP)
#define XMLURL_HPP

#include <util/XML4CDefs.hpp>
#include <util/XMLException.hpp>
#include <util/XMLUni.hpp>

class BinInputStream;


// Hard coded Unicode text for "MalformedURLException"
static const XMLCh gMalformedURLException_Name[] =
{
        chLatin_M, chLatin_a, chLatin_l, chLatin_f, chLatin_o, chLatin_r
    ,   chLatin_m, chLatin_e, chLatin_d, chLatin_U, chLatin_R, chLatin_L
    ,   chLatin_E, chLatin_x, chLatin_c, chLatin_e, chLatin_p, chLatin_t
    ,   chLatin_i, chLatin_o, chLatin_n, chNull
};

MakeXML4CException(MalformedURLException, XMLUTIL_EXPORT)


//
//  This class supports file, http, and ftp style URLs. All others are
//  rejected
//
class XMLUTIL_EXPORT XMLURL
{
public:
    // -----------------------------------------------------------------------
    //  Class types
    //
    //  And they must remain in this order because they are indexes into an
    //  array internally!
    // -----------------------------------------------------------------------
    enum Protocols
    {
        File
        , HTTP
        , FTP

        , Protocols_Count
        , Unknown
    };


    // -----------------------------------------------------------------------
    //  Public static methods
    // -----------------------------------------------------------------------
    Protocols lookupByName(const XMLCh* const protoName);


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XMLURL();
    XMLURL
    (
        const   XMLCh* const    baseURL
        , const XMLCh* const    relativeURL
    );
    XMLURL
    (
        const   XMLCh* const    baseURL
        , const char* const     relativeURL
    );
    XMLURL
    (
        const   XMLURL&         baseURL
        , const XMLCh* const    relativeURL
    );
    XMLURL
    (
        const   XMLURL&         baseURL
        , const char* const     relativeURL
    );
    XMLURL
    (
        const   XMLCh* const    urlText
    );
    XMLURL
    (
        const   char* const     urlText
    );
    XMLURL(const XMLURL& toCopy);
    virtual ~XMLURL();


    // -----------------------------------------------------------------------
    //  Operators
    // -----------------------------------------------------------------------
    XMLURL& operator=(const XMLURL& toAssign);
    bool operator==(const XMLURL& toCompare) const;
    bool operator!=(const XMLURL& toCompare) const;


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* getFragment() const;
    const XMLCh* getHost() const;
    const XMLCh* getPassword() const;
    const XMLCh* getPath() const;
    Protocols getProtocol() const;
    const XMLCh* getProtocolName() const;
    const XMLCh* getQuery() const;
    const XMLCh* getURLText() const;
    const XMLCh* getUser() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setURL(const XMLCh* const urlText);
    void setURL
    (
        const   XMLCh* const    baseURL
        , const XMLCh* const    relativeURL
    );
    void setURL
    (
        const   XMLURL&         baseURL
        , const XMLCh* const    relativeURL
    );


    // -----------------------------------------------------------------------
    //  Miscellaneous methods
    // -----------------------------------------------------------------------
    bool isRelative() const;
    BinInputStream* makeNewStream() const;
    void makeRelativeTo(const XMLCh* const baseURLText);
    void makeRelativeTo(const XMLURL& baseURL);


private:
    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void buildFullText();
    void cleanup();
    void conglomerateWithBase(const XMLURL& baseURL);
    void parse
    (
        const   XMLCh* const    urlText
    );
    void weavePaths(const XMLCh* const basePart);


    // -----------------------------------------------------------------------
    //  Data members
    //
    //  fFragment
    //      The fragment part of the URL, if any. If none, its a null.
    //
    //  fHost
    //      The host part of the URL that was parsed out. This one will often
    //      be null (or "localhost", which also means the current machine.)
    //
    //  fPassword
    //      The password found, if any. If none then its a null.
    //
    //  fPath
    //      The path part of the URL that was parsed out, if any. If none,
    //      then its a null.
    //
    //  fProtocol
    //      Indicates the type of the URL's source. The text of the prefix
    //      can be gotten from this.
    //
    //  fQuery
    //      The query part of the URL, if any. If none, then its a null.
    //
    //  fUser
    //      The username found, if any. If none, then its a null.
    //
    //  fURLText
    //      This is a copy of the URL text, after it has been taken apart,
    //      made relative if needed, canonicalized, and then put back
    //      together. Its only created upon demand.
    // -----------------------------------------------------------------------
    XMLCh*      fFragment;
    XMLCh*      fHost;
    XMLCh*      fPassword;
    XMLCh*      fPath;
    Protocols   fProtocol;
    XMLCh*      fQuery;
    XMLCh*      fUser;
    XMLCh*      fURLText;
};


// ---------------------------------------------------------------------------
//  XMLURL: Public operators
// ---------------------------------------------------------------------------
inline bool XMLURL::operator!=(const XMLURL& toCompare) const
{
    return !operator==(toCompare);
}


// ---------------------------------------------------------------------------
//  XMLURL: Getter methods
// ---------------------------------------------------------------------------
inline const XMLCh* XMLURL::getFragment() const
{
    return fFragment;
}

inline const XMLCh* XMLURL::getHost() const
{
    return fHost;
}

inline const XMLCh* XMLURL::getPassword() const
{
    return fPassword;
}

inline const XMLCh* XMLURL::getPath() const
{
    return fPath;
}

inline XMLURL::Protocols XMLURL::getProtocol() const
{
    return fProtocol;
}

inline const XMLCh* XMLURL::getQuery() const
{
    return fQuery;
}

inline const XMLCh* XMLURL::getUser() const
{
    return fUser;
}

inline const XMLCh* XMLURL::getURLText() const
{
    //
    //  Fault it in if not already. Since this is a const method and we
    //  can't use mutable members due the compilers we have to support,
    //  we have to cast off the constness.
    //
    if (!fURLText)
        ((XMLURL*)this)->buildFullText();

    return fURLText;
}

#endif
