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
 * Revision 1.4  2000/03/02 19:55:15  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/06 07:48:22  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  1999/12/03 00:48:47  rahulj
 * Removed byteswapping logic as its not needed for InMemory message loading.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:19  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:45:26  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XercesDefs.hpp>
#include <util/BitOps.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLMsgLoader.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUni.hpp>
#include "InMemMsgLoader.hpp"
#include "CppErrMsgs_EN_US.hpp"



// ---------------------------------------------------------------------------
//  Public Constructors and Destructor
// ---------------------------------------------------------------------------
InMemMsgLoader::InMemMsgLoader(const XMLCh* const msgDomain) :

    fMsgDomain(0)
{
    fMsgDomain = XMLString::replicate(msgDomain);

    if (XMLString::compareString(fMsgDomain, XMLUni::fgXMLErrDomain)
    &&  XMLString::compareString(fMsgDomain, XMLUni::fgExceptDomain)
    &&  XMLString::compareString(fMsgDomain, XMLUni::fgValidityDomain))
    {
        XMLPlatformUtils::panic(XMLPlatformUtils::Panic_UnknownMsgDomain);
    }
}

InMemMsgLoader::~InMemMsgLoader()
{
    delete [] fMsgDomain;
}


// ---------------------------------------------------------------------------
//  Implementation of the virtual message loader API
// ---------------------------------------------------------------------------
bool InMemMsgLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned long           maxChars)
{
    //
    //  Just use the id to map into the correct array of messages. Then
    //  copy that to the caller's buffer.
    //
    //  NOTE:   The source text is in little endian form. So, if we are a
    //          big endian machine, flip them in the process.
    //
    XMLCh* endPtr = toFill + maxChars;
    XMLCh* outPtr = toFill;
    if (!XMLString::compareString(fMsgDomain, XMLUni::fgXMLErrDomain))
    {
        const XMLCh* srcPtr = gXMLErrArray[msgToLoad - 1];
        while (*srcPtr && (outPtr < endPtr))
        {
            *outPtr++ = *srcPtr++;
        }
        *outPtr = 0;
    }
     else if (!XMLString::compareString(fMsgDomain, XMLUni::fgExceptDomain))
    {
        const XMLCh* srcPtr = gXMLExceptArray[msgToLoad - 1];
        while (*srcPtr && (outPtr < endPtr))
        {
            *outPtr++ = *srcPtr++;
        }
        *outPtr = 0;
    }
     else if (!XMLString::compareString(fMsgDomain, XMLUni::fgValidityDomain))
    {
        const XMLCh* srcPtr = gXMLValidityArray[msgToLoad - 1];
        while (*srcPtr && (outPtr < endPtr))
        {
            *outPtr++ = *srcPtr++;
        }
        *outPtr = 0;
    }
     else
    {
        return false;
    }
    return true;
}


bool InMemMsgLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned long           maxChars
                            , const XMLCh* const            repText1
                            , const XMLCh* const            repText2
                            , const XMLCh* const            repText3
                            , const XMLCh* const            repText4)
{
    // Call the other version to load up the message
    if (!loadMsg(msgToLoad, toFill, maxChars))
        return false;

    // And do the token replacement
    XMLString::replaceTokens(toFill, maxChars, repText1, repText2, repText3, repText4);
    return true;
}


bool InMemMsgLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned long           maxChars
                            , const char* const             repText1
                            , const char* const             repText2
                            , const char* const             repText3
                            , const char* const             repText4)
{
    //
    //  Transcode the provided parameters and call the other version,
    //  which will do the replacement work.
    //
    XMLCh* tmp1 = 0;
    XMLCh* tmp2 = 0;
    XMLCh* tmp3 = 0;
    XMLCh* tmp4 = 0;

    bool bRet = false;
    if (repText1)
        tmp1 = XMLString::transcode(repText1);
    if (repText2)
        tmp2 = XMLString::transcode(repText2);
    if (repText3)
        tmp3 = XMLString::transcode(repText3);
    if (repText4)
        tmp4 = XMLString::transcode(repText4);

    bRet = loadMsg(msgToLoad, toFill, maxChars, tmp1, tmp2, tmp3, tmp4);

    if (tmp1)
        delete [] tmp1;
    if (tmp2)
        delete [] tmp2;
    if (tmp3)
        delete [] tmp3;
    if (tmp4)
        delete [] tmp4;

    return bRet;
}
