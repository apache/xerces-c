/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLMsgLoader.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <windows.h>
#include "Win32MsgLoader.hpp"


//	Function prototypes
BOOL APIENTRY DllMain(HINSTANCE hModule,
                             DWORD  ul_reason_for_call,
                             LPVOID lpReserved);


// ---------------------------------------------------------------------------
//  Public Constructors and Destructor
// ---------------------------------------------------------------------------
HINSTANCE globalModuleHandle;

BOOL APIENTRY DllMain(HINSTANCE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    globalModuleHandle = hModule;
    break;
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Global module handle
// ---------------------------------------------------------------------------
Win32MsgLoader::Win32MsgLoader(const XMLCh* const msgDomain) :

    fDomainOfs(0)
    , fModHandle(0)
    , fMsgDomain(0)
{
    // Try to get the module handle
    fModHandle = globalModuleHandle;
    if (!fModHandle)
    {
        //
        //  If we didn't find it, its probably because its a development
        //  build which is built as separate DLLs, so lets look for the DLL
        //  that we are part of.
        //
        static const char* const privDLLName = "IXUTIL";
        fModHandle = ::GetModuleHandleA(privDLLName);

        // If neither exists, then we give up
        if (!fModHandle)
        {
            // Probably have to call panic here
        }
    }

    // Store the domain name
    fMsgDomain = XMLString::replicate(msgDomain, XMLPlatformUtils::fgMemoryManager);

    // And precalc the id offset we use for this domain
    if (XMLString::equals(fMsgDomain, XMLUni::fgXMLErrDomain))
        fDomainOfs = 0;
    else if (XMLString::equals(fMsgDomain, XMLUni::fgExceptDomain))
        fDomainOfs = 0x2000;
    else if (XMLString::equals(fMsgDomain, XMLUni::fgValidityDomain))
        fDomainOfs = 0x4000;
    else if (XMLString::equals(fMsgDomain, XMLUni::fgXMLDOMMsgDomain))
        fDomainOfs = 0x6000;
    else
        XMLPlatformUtils::panic(PanicHandler::Panic_UnknownMsgDomain);
}

Win32MsgLoader::~Win32MsgLoader()
{
    XMLPlatformUtils::fgMemoryManager->deallocate(fMsgDomain);//delete [] fMsgDomain;
}


// ---------------------------------------------------------------------------
//  Implementation of the virtual message loader API
// ---------------------------------------------------------------------------

//
//  This is the method that actually does the work of loading a message from
//  the attached resources. Note that we don't use LoadStringW here, since it
//  won't work on Win98. So we go the next level down and do what LoadStringW
//  would have done, since this will work on either platform.
//
bool Win32MsgLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned int            maxChars)
{
    // In case we error return, and they don't check it...
    toFill[0] = 0;

    // Adjust the message id by the domain offset
    const unsigned int theMsgId = msgToLoad + fDomainOfs;

    //
    //  Figure out the actual id the id, adjusting it by the domain offset.
    //  Then first we calculate the particular 16 string block that this id
    //  is in, and the offset within that block of the string in question.
    //
    const unsigned int theBlock = (theMsgId >> 4) + 1;
    const unsigned int theOfs   = theMsgId & 0x000F;

    // Try to find this resource. If we fail to find it, return false
    HRSRC hMsgRsc = ::FindResourceEx
    (
        fModHandle
        , RT_STRING
        , MAKEINTRESOURCE(theBlock)
        , MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)
    );
    if (!hMsgRsc)
        return false;

    // We found it, so load the block. If this fails, also return a false
    HGLOBAL hGbl = ::LoadResource(fModHandle, hMsgRsc);
    if (!hGbl)
        return false;

    // Lock this resource into memory. Again, if it fails, just return false
    const XMLCh* pBlock = (const XMLCh*)::LockResource(hGbl);
    if (!pBlock)
        return false;

    //
    //  Look through the block for our desired message. Its stored such that
    //  the zeroth entry has the length minus the separator null.
    //
    for (unsigned int index = 0; index < theOfs; index++)
        pBlock += *pBlock + 1;

    // Calculate how many actual chars we will end up with
    const unsigned int actualChars = ((maxChars < *pBlock) ? maxChars : *pBlock);

    // Ok, finally now copy as much as we can into the caller's buffer
    wcsncpy(toFill, pBlock + 1, actualChars);
    toFill[actualChars] = 0;

    return true;
}


bool Win32MsgLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned int            maxChars
                            , const XMLCh* const            repText1
                            , const XMLCh* const            repText2
                            , const XMLCh* const            repText3
                            , const XMLCh* const            repText4
                            , MemoryManager* const          manager)
{
    // Call the other version to load up the message
    if (!loadMsg(msgToLoad, toFill, maxChars))
        return false;

    // And do the token replacement
    XMLString::replaceTokens(toFill, maxChars, repText1, repText2, repText3, repText4, manager);
    return true;
}


bool Win32MsgLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned int            maxChars
                            , const char* const             repText1
                            , const char* const             repText2
                            , const char* const             repText3
                            , const char* const             repText4
                            , MemoryManager* const          manager)
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
        tmp1 = XMLString::transcode(repText1, manager);
    if (repText2)
        tmp2 = XMLString::transcode(repText2, manager);
    if (repText3)
        tmp3 = XMLString::transcode(repText3, manager);
    if (repText4)
        tmp4 = XMLString::transcode(repText4, manager);

    bRet = loadMsg(msgToLoad, toFill, maxChars, tmp1, tmp2, tmp3, tmp4, manager);

    if (tmp1)
        manager->deallocate(tmp1);//delete [] tmp1;
    if (tmp2)
        manager->deallocate(tmp2);//delete [] tmp2;
    if (tmp3)
        manager->deallocate(tmp3);//delete [] tmp3;
    if (tmp4)
        manager->deallocate(tmp4);//delete [] tmp4;

    return bRet;
}

XERCES_CPP_NAMESPACE_END
