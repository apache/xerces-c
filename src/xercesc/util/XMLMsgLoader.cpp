/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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
 *  $Id$
 * $Log$
 * Revision 1.3  2003/05/15 19:07:46  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2003/02/17 19:54:47  peiyongz
 * Allow set user specified error message file location in PlatformUtils::Initialize().
 *
 * Revision 1.1  2002/11/04 22:24:21  peiyongz
 * Locale setting for message loader
 *
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLMsgLoader.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/***
 *   The PlatformUtils::initialize() would set fLocale
 *   to either a user-privded string or 0
 *
 ***/
char* XMLMsgLoader::fLocale = 0;

char* XMLMsgLoader::fPath = 0;

XMLCh XMLMsgLoader::fLanguage[] = {chLatin_e, chLatin_n, chNull};

/***
 *  if localeToAdopt is 0, that is to release memory for
 *  the user defined locale string
 *
 ***/
void  XMLMsgLoader::setLocale(const char* const localeToAdopt)
{
    /***
     * Release the current setting's memory, if any
     ***/
	if (fLocale)
	{
        XMLPlatformUtils::fgMemoryManager->deallocate(fLocale);//delete [] fLocale;
		fLocale = 0;
	}

    /***
     *  
     *  REVISIT: later we may do locale format checking
     * 
     *           refer to phttp://oss.software.ibm.com/icu/userguide/locale.html
     *           for details.
     */
	if (localeToAdopt)
	{
		fLocale   = XMLString::replicate(localeToAdopt, XMLPlatformUtils::fgMemoryManager);
        XMLString::transcode(fLocale, fLanguage, 2);
        fLanguage[2] = 0;
    }

}

const char* XMLMsgLoader::getLocale()
{
	return fLocale;
}

/***
 *  if nlsHomeToAdopt is 0, that is to release memory for
 *  the user defined NLSHome string
 *
 ***/
void  XMLMsgLoader::setNLSHome(const char* const nlsHomeToAdopt)
{
    /***
     * Release the current setting's memory, if any
     ***/
    if (fPath)
    {
        XMLPlatformUtils::fgMemoryManager->deallocate(fPath);//delete [] fPath;
        fPath = 0;
    }

    if (nlsHomeToAdopt)
    {
        fPath = XMLString::replicate(nlsHomeToAdopt, XMLPlatformUtils::fgMemoryManager);
    }

}

const char* XMLMsgLoader::getNLSHome()
{
    return fPath;
}

// ---------------------------------------------------------------------------
//  Deprecated
//
//     These two methods are deprecated.
//  
//     The default implementations for these two methods are provided as is,
//     any specific derivative may change this as and when necessary.
//      
// ---------------------------------------------------------------------------
const XMLCh* XMLMsgLoader::getLanguageName() const
{
    return fLanguage;
}

void XMLMsgLoader::setLanguageName(XMLCh* const)
{
}

XERCES_CPP_NAMESPACE_END
