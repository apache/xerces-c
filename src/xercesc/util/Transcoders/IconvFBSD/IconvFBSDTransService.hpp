/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
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

XERCES_CPP_NAMESPACE_BEGIN

#ifdef XML_USE_LIBICONV

#  include <iconv.h>

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
    void operator=(const IconvFBSDCD&);

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
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    IconvFBSDTransService(const IconvFBSDTransService&);
    void operator=(const IconvFBSDTransService&);

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
    void operator=(const IconvFBSDTranscoder&);
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
    virtual unsigned int calcRequiredSize(const char* const srcText);

    virtual unsigned int calcRequiredSize(const XMLCh* const srcText);

    virtual char* transcode(const XMLCh* const toTranscode);

    virtual bool transcode
    (
        const   XMLCh* const    toTranscode
        ,       char* const     toFill
        , const unsigned int    maxBytes
    );

    virtual XMLCh* transcode(const char* const toTranscode);

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
    IconvFBSDLCPTranscoder(const IconvFBSDLCPTranscoder&);
    void operator=(const IconvFBSDLCPTranscoder&);
};

XERCES_CPP_NAMESPACE_END

#endif /* ICONVFBSDTRANSSERVICE */
