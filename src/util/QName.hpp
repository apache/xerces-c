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
 * Revision 1.1  2001/02/26 19:44:25  tng
 * Schema: add utility class QName, by Pei Yong Zhang.
 *
 */

#if !defined(QNAME_HPP)
#define QNAME_HPP

#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/XMLUni.hpp>

class XMLUTIL_EXPORT QName
{
public :
    // -----------------------------------------------------------------------
    //  Contructors and Destructor
    // -----------------------------------------------------------------------
    /** Default constructor. */
    QName();

    /** Constructs a specified qname. */
    QName
    (   const XMLCh* const        prefix
       ,const XMLCh* const        localPart
	   ,const int                 uriId = -1
    );

    /** Copy constructor. */
    QName(const QName& qname);

    ~QName();

    // -----------------------------------------------------------------------
    //  Getters
    // -----------------------------------------------------------------------
    const XMLCh* getPrefix() const;

	const XMLCh* getLocalPart() const;

	const int getURI() const;

	const XMLCh* getRawName() const;

    // -----------------------------------------------------------------------
    //  Setters
    // -----------------------------------------------------------------------
    void setName
    (
        const XMLCh* const        prefix
       ,const XMLCh* const        localPart
	   ,const int                 uriId = -1
    );

    void setPrefix(const XMLCh*) ;

	void setLocalPart(const XMLCh*) ;

	void setURI(const int) ;

    // -----------------------------------------------------------------------
    //  comparison
    // -----------------------------------------------------------------------
	bool operator==(const QName&);

    // -----------------------------------------------------------------------
    //  Misc
    // -----------------------------------------------------------------------
	void cleanUp();

private :

    // -----------------------------------------------------------------------
    //  Private instance variables
    //
    //  We copy the followings from XMLAttr.hpp, but stick to Java version's
	//  naming convention
    //
    //  fPrefix
    //  fPrefixBufSz
    //      The prefix that was applied to this attribute's name, and the
    //      current size of the buffer (minus one for the null.) Prefixes
    //      really don't matter technically but it might be required for
    //      pratical reasons, to recreate the original document for instance.
	//
    //  fLocalPart
    //  fLocalPartBufSz
    //      The base part of the name of the attribute, and the current size
    //      of the buffer (minus one, where the null is.)
    //
    //  fRawName
    //  fRawNameBufSz
    //      This is the QName form of the name, which is faulted in (from the
    //      prefix and name) upon request. The size field indicates the
    //      current size of the buffer (minus one for the null.) It will be
    //      zero until fauled in.
    //
    //  fURIId
    //      The id of the URI that this attribute belongs to.
    // -----------------------------------------------------------------------
    XMLCh*              fPrefix;
    unsigned int        fPrefixBufSz;
    XMLCh*              fLocalPart;
    unsigned int        fLocalPartBufSz;
    XMLCh*              fRawName;
    unsigned int        fRawNameBufSz;
    int                 fURIId;

};

#endif
