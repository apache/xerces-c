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
 * $Id$
 */
 
#pragma once

#include <util/XercesDefs.hpp>
#include <cstdlib>

#if TARGET_API_MAC_CARBON
	#include <Carbon.h>
#else
	#include <Files.h>
#endif


class XMLMacAbstractFile
{
    public:
        XMLMacAbstractFile() {}
        virtual ~XMLMacAbstractFile() {}
        
        virtual unsigned int currPos() = 0;
        virtual void close() = 0;
        virtual unsigned int size() = 0;
        virtual void open(const XMLCh* const) = 0;
        virtual unsigned int read(const unsigned int, XMLByte* const) = 0;
        virtual void reset() = 0;
};


class XMLMacFile : public XMLMacAbstractFile
{
    public:
        XMLMacFile() : mFileRefNum(0), mFileValid(false) {}
        virtual ~XMLMacFile();
        
        unsigned int currPos();
        void close();
        unsigned int size();
        void open(const XMLCh* const);
        unsigned int read(const unsigned int, XMLByte* const);
        void reset();
        
    protected:
        short	mFileRefNum;
        bool	mFileValid;
        unsigned char pStr[300];
};


//	Convert fom FSRef/FSSpec to a Unicode character string path.
//	Note that you'll need to delete [] that string after you're done with it!
XMLCh*	XMLCreateFullPathFromFSRef(const FSRef& startingRef);
XMLCh*	XMLCreateFullPathFromFSSpec(const FSSpec& startingSpec);

//	Convert from path to FSRef/FSSpec
bool	XMLParsePathToFSRef(const XMLCh* const pathName, FSRef& ref);
bool	XMLParsePathToFSSpec(const XMLCh* const pathName, FSSpec& spec);

//	These routines copy characters between their representation in the Unicode Converter
//	and the representation used in the C library for the compiler. These representations
//	are the same for Metrowerks and different for Project Builder.
//	Each of these routines is safe to call for the case where the src and dst buffers are
//	in the same location (a conversion in place). Other overlapping ranges are probably
//	not safe.
XMLCh*		CopyUniCharsToXMLChs(const UniChar* src, XMLCh* dst, std::size_t charCount, std::size_t maxChars);
UniChar*	CopyXMLChsToUniChars(const XMLCh* src, UniChar* dst, std::size_t charCount, std::size_t maxChars);
