/*
 * Copyright 2001,2004 The Apache Software Foundation.
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
 * Revision 1.10  2004/09/08 13:56:45  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.9  2003/12/24 15:24:15  cargilld
 * More updates to memory management so that the static memory manager.
 *
 * Revision 1.8  2003/05/17 16:32:18  knoaman
 * Memory manager implementation : transcoder update.
 *
 * Revision 1.7  2003/05/15 18:47:04  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.6  2003/03/07 18:15:57  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.5  2003/03/07 15:08:57  tng
 * [Bug 17571] fix building IconvFBSD (namespaces) .  Patch from Bjoern A. Zeeb.
 *
 * Revision 1.4  2002/11/04 15:14:34  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/04/11 15:38:05  knoaman
 * String lower case support for FreeBSD by Max Gotlib
 *
 * Revision 1.2  2002/04/09 15:44:00  knoaman
 * Add lower case string support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:36  peiyongz
 * sane_include
 *
 * Revision 1.3  2002/01/14 19:45:15  tng
 * Support IconvFBSD in multi-threading environment with all the possible combinations of threading and transcoding options.  By Max Gotlib.
 *
 * Revision 1.2  2001/12/11 15:10:14  tng
 * More changes to IconvFBSDTransService.  Allow using "old" TransServece implementation (via '-t native' option to runConfigure) or
 * to employ libiconv (it is a part of FreeBSD ports-collection) services.  By Max Gotlib.
 *
 * Revision 1.1  2001/12/03 14:45:11  tng
 * FreeBSD native transcoder (IconvFBSD) added by Max Gotlib.
 *
 */

#ifndef ICONVFBSDTRANSSERVICE_HPP
#define ICONVFBSDTRANSSERVICE_HPP

#include <xercesc/util/TransService.hpp>

#ifdef XML_USE_LIBICONV
#  include <iconv.h>
#endif

XERCES_CPP_NAMESPACE_BEGIN

#ifdef XML_USE_LIBICONV

// ---------------------------------------------------------------------------
//  Libiconv wrapper (low-level conversion utilities collection)
// ---------------------------------------------------------------------------

class XMLUTIL_EXPORT IconvFBSDCD
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    IconvFBSDCD
    (
	iconv_t		cd_from,
	iconv_t		cd_to,
	size_t		uchsize,
	unsigned int	ubo
    );
    virtual ~IconvFBSDCD();

    // Convert "native unicode" character into XMLCh
    void	mbcToXMLCh (const char *mbc, XMLCh *toRet) const;

    // Convert XMLCh into "native unicode" character
    void	xmlChToMbc (XMLCh xch, char *mbc) const;

    // Return uppercase equivalent for XMLCh
    XMLCh 	toUpper (const XMLCh ch) const;

    // Return uppercase equivalent for XMLCh
    XMLCh 	toLower (const XMLCh ch) const;

    // Check if passed characters belongs to the :space: class
    virtual bool isSpace(const XMLCh toCheck) const;

    // Fill array of XMLCh characters with data, supplyed in the array
    // of "native unicode" characters.
    XMLCh*	mbsToXML (
	const char*	mbs_str,
	size_t		mbs_cnt,
	XMLCh*		xml_str,
	size_t		xml_cnt
    ) const;


    // Fill array of "native unicode" characters with data, supplyed
    // in the array of XMLCh characters.
    char*	xmlToMbs
    (
	const XMLCh*	xml_str,
	size_t		xml_cnt,
	char*		mbs_str,
	size_t		mbs_cnt
    ) const;

    // Wrapper aroung the iconv() for transcoding from the local charset
    size_t	iconvFrom
    (
	const char	*fromPtr,
	size_t		*fromLen,
	char		**toPtr,
	size_t		toLen
    ) const;

    // Wrapper aroung the iconv() for transcoding to the local charset
    size_t	iconvTo
    (
	const char	*fromPtr,
	size_t		*fromLen,
	char		**toPtr,
	size_t		toLen
    ) const;

    // Private data accessors
    inline iconv_t	cdTo () const { return fCDTo; }
    inline iconv_t	cdFrom () const { return fCDFrom; }
    inline size_t	uChSize () const { return fUChSize; }
    inline unsigned int	UBO () const { return fUBO; }

protected:

    // Hiden defaull constructor
    IconvFBSDCD();

    // Private data accessors
    inline void	setCDTo (iconv_t cd) { fCDTo = cd; }
    inline void	setCDFrom (iconv_t cd) { fCDFrom = cd; }
    inline void	setUChSize (size_t sz) { fUChSize = sz; }
    inline void	setUBO (unsigned int u) { fUBO = u; }

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    IconvFBSDCD(const IconvFBSDCD&);
    IconvFBSDCD& operator=(const IconvFBSDCD&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fCDTo
    //	    Characterset conversion descriptor TO the local-host encoding
    //  fCDFrom
    //	    Characterset conversion descriptor FROM the local-host encoding
    //  fUChSize
    //      Sizeof the "native unicode" character in bytes
    //  fUBO
    //      "Native unicode" characters byte order
    // -----------------------------------------------------------------------
    size_t	fUChSize;
    unsigned int fUBO;
    iconv_t	fCDTo;
    iconv_t	fCDFrom;
};

#endif /* XML_USE_LIBICONV */



// ---------------------------------------------------------------------------
//  FreeBSD-specific Transcoding Service implementation
// ---------------------------------------------------------------------------

class XMLUTIL_EXPORT IconvFBSDTransService : public XMLTransService
#ifdef XML_USE_LIBICONV
, IconvFBSDCD
#endif
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    IconvFBSDTransService();
    ~IconvFBSDTransService();


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
    virtual void lowerCase(XMLCh* const toUpperCase) const;

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
    IconvFBSDTransService(const IconvFBSDTransService&);
    IconvFBSDTransService& operator=(const IconvFBSDTransService&);

#ifdef XML_USE_LIBICONV

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fUnicodeCP
    //      Unicode encoding schema name
    // -----------------------------------------------------------------------
    const char*	fUnicodeCP;

#endif /* XML_USE_LIBICONV */
};


#ifdef XML_USE_LIBICONV
//----------------------------------------------------------------------------
// Implementation of the transcoders for arbitrary input characterset is
// supported ONLY through libiconv interface
//----------------------------------------------------------------------------

class XMLUTIL_EXPORT IconvFBSDTranscoder : public XMLTranscoder, IconvFBSDCD
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    IconvFBSDTranscoder(const	XMLCh* const	encodingName
			, const unsigned int	blockSize
			,	iconv_t		cd_from
			,	iconv_t		cd_to
			,	size_t		uchsize
			,	unsigned int	ubo
			, MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    ~IconvFBSDTranscoder();


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
    IconvFBSDTranscoder();
    IconvFBSDTranscoder(const IconvFBSDTranscoder&);
    IconvFBSDTranscoder& operator=(const IconvFBSDTranscoder&);
};

#endif /* XML_USE_LIBICONV */


// ---------------------------------------------------------------------------
//  FreeBSD-specific XMLCh <-> local (host) characterset transcoder
// ---------------------------------------------------------------------------

class XMLUTIL_EXPORT IconvFBSDLCPTranscoder : public XMLLCPTranscoder
#ifdef XML_USE_LIBICONV
, IconvFBSDCD
#endif
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
#ifdef XML_USE_LIBICONV

    IconvFBSDLCPTranscoder
    (
	iconv_t		from,
	iconv_t		to,
	size_t		uchsize,
	unsigned int	ubo
    );

protected:
    IconvFBSDLCPTranscoder();	// Unimplemented

public:

#else /* !XML_USE_LIBICONV */

    IconvFBSDLCPTranscoder();

#endif /* XML_USE_LIBICONV */

    ~IconvFBSDLCPTranscoder();


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
    IconvFBSDLCPTranscoder(const IconvFBSDLCPTranscoder&);
    IconvFBSDLCPTranscoder& operator=(const IconvFBSDLCPTranscoder&);
};

XERCES_CPP_NAMESPACE_END

#endif /* ICONVFBSDTRANSSERVICE */
