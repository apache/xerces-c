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
 * 4. The names "[PRODUCT NAME]" and "Apache Software Foundation" must
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
 * $Id$
 */

#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/NetAccessors/libWWW/BinURLInputStream.hpp>
#include <xercesc/util/NetAccessors/libWWW/LibWWWNetAccessor.hpp>

#include <WWWInit.h>

XERCES_CPP_NAMESPACE_BEGIN

const XMLCh LibWWWNetAccessor::fgMyName[] =
{
    chLatin_l, chLatin_i, chLatin_b, chLatin_W, chLatin_W, chLatin_W,
    chLatin_N, chLatin_e, chLatin_t, chLatin_A, chLatin_c, chLatin_c,
    chLatin_e, chLatin_s, chLatin_s, chLatin_o, chLatin_r, chNull
};


LibWWWNetAccessor::LibWWWNetAccessor()
{
    //
    // Initialize the libWWW library here.
    //
    HTProfile_newPreemptiveClient("XercesC", gXercesFullVersionStr);
#ifdef XML_DEBUG
	HTSetTraceMessageMask("sop");
#endif
    HTAlert_setInteractive(NO);
    HTHost_setEventTimeout(5000);
}


LibWWWNetAccessor::~LibWWWNetAccessor()
{
    // Cleanup the libWWW library here.

	/* Quote from http://www.w3.org/Library/src/HTProfil.html#Client:
	 *
	 * This call also supersedes the termination function for the
	 * Library core, HTLibTerminate() so that you don't have to call
	 * that after calling this function.
	 */
	HTProfile_delete();
}


BinInputStream* LibWWWNetAccessor::makeNew(const XMLURL&  urlSource)
{
    XMLURL::Protocols  protocol = urlSource.getProtocol();
    switch(protocol)
    {
        case XMLURL::HTTP:
        {
            BinURLInputStream* retStrm =
				new (urlSource.getMemoryManager()) BinURLInputStream(urlSource);
            return retStrm;
        }

        //
        // These are the only protocols we support now. So throw and
        // unsupported protocol exception for the others.
        //
        default :
            ThrowXMLwithMemMgr(MalformedURLException, XMLExcepts::URL_UnsupportedProto, urlSource.getMemoryManager());
    }
}

XERCES_CPP_NAMESPACE_END
