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
 *
 * Written by James D. Berry 7/20/00
 * Critical Path Software, Inc.
 */

/*
 * $Id$
 */


#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/NetAccessors/MacOSURLAccess/MacOSURLAccess.hpp>
#include <xercesc/util/NetAccessors/MacOSURLAccess/URLAccessBinInputStream.hpp>

XERCES_CPP_NAMESPACE_BEGIN

const XMLCh
MacOSURLAccess::sMyID[] =
{
    chLatin_M, chLatin_a, chLatin_c, chLatin_O, chLatin_S, chLatin_U,
    chLatin_R, chLatin_L, chLatin_A, chLatin_c, chLatin_c, chLatin_e,
    chLatin_s, chLatin_s,
    chNull
};


MacOSURLAccess::MacOSURLAccess()
{
	//	Ensure that we've got URLAccess
	if (!URLAccessAvailable())
        ThrowXML(NetAccessorException, XMLExcepts::NetAcc_InitFailed);
}


MacOSURLAccess::~MacOSURLAccess()
{
}


BinInputStream*
MacOSURLAccess::makeNew(const XMLURL&  urlSource)
{
	//	We just go ahead and try to create a URLAccess stream
	//	from this source. That's the correct place to verify
	//	whether or not we can really handle this URL type...
	//	if it throws, well, it throws ;)
	BinInputStream* result = new (urlSource.getMemoryManager()) URLAccessBinInputStream(urlSource);
	return result;
}

XERCES_CPP_NAMESPACE_END

