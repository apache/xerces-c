/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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
 * Revision 1.4  2003/05/15 18:47:05  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2003/03/07 18:15:57  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.2  2002/11/04 15:14:34  tng
 * C++ Namespace Support.
 *
 * Revision 1.1  2002/08/19 19:38:18  tng
 * [Bug 11771] Linux specific IconvGNU transcoder.  Patch from Vasily Tchekalkin.
 *
 */

#ifndef ICONVGNUTRANSSERVICE_HPP
#define ICONVGNUTRANSSERVICE_HPP

#include <xercesc/util/TransService.hpp>


#include <iconv.h>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Libiconv wrapper (low-level conversion utilities collection)
// ---------------------------------------------------------------------------

class XMLUTIL_EXPORT IconvGNUWrapper
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    IconvGNUWrapper
    (
  iconv_t		cd_from,
  iconv_t		cd_to,
  size_t		uchsize,
  unsigned int	ubo
    );
    virtual ~IconvGNUWrapper();

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
    IconvGNUWrapper();

    // Private data accessors
    inline void	setCDTo (iconv_t cd) { fCDTo = cd; }
    inline void	setCDFrom (iconv_t cd) { fCDFrom = cd; }
    inline void	setUChSize (size_t sz) { fUChSize = sz; }
    inline void	setUBO (unsigned int u) { fUBO = u; }

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    IconvGNUWrapper(const IconvGNUWrapper&);
    IconvGNUWrapper& operator=(const IconvGNUWrapper&);

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



// ---------------------------------------------------------------------------
//  FreeBSD-specific Transcoding Service implementation
// ---------------------------------------------------------------------------

class XMLUTIL_EXPORT IconvGNUTransService : public XMLTransService, IconvGNUWrapper
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    IconvGNUTransService();
    ~IconvGNUTransService();


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
    IconvGNUTransService(const IconvGNUTransService&);
    IconvGNUTransService& operator=(const IconvGNUTransService&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fUnicodeCP
    //      Unicode encoding schema name
    // -----------------------------------------------------------------------
    const char*	fUnicodeCP;

};


//----------------------------------------------------------------------------
// Implementation of the transcoders for arbitrary input characterset is
// supported ONLY through libiconv interface
//----------------------------------------------------------------------------

class XMLUTIL_EXPORT IconvGNUTranscoder : public XMLTranscoder, IconvGNUWrapper
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    IconvGNUTranscoder(const	XMLCh* const	encodingName
  		, const unsigned int	blockSize
  		,	iconv_t		cd_from
  		,	iconv_t		cd_to
  		,	size_t		uchsize
  		,	unsigned int	ubo
    );
    ~IconvGNUTranscoder();


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
    IconvGNUTranscoder();
    IconvGNUTranscoder(const IconvGNUTranscoder&);
    IconvGNUTranscoder& operator=(const IconvGNUTranscoder&);
};


// ---------------------------------------------------------------------------
//  GNU-specific XMLCh <-> local (host) characterset transcoder
// ---------------------------------------------------------------------------

class XMLUTIL_EXPORT IconvGNULCPTranscoder : public XMLLCPTranscoder, IconvGNUWrapper
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------

    IconvGNULCPTranscoder
    (
  iconv_t		from,
  iconv_t		to,
  size_t		uchsize,
  unsigned int	ubo
    );

protected:
    IconvGNULCPTranscoder();	// Unimplemented

public:

    ~IconvGNULCPTranscoder();


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
    IconvGNULCPTranscoder(const IconvGNULCPTranscoder&);
    IconvGNULCPTranscoder& operator=(const IconvGNULCPTranscoder&);
};

XERCES_CPP_NAMESPACE_END

#endif /* ICONVGNUTRANSSERVICE */

