/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
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
 * Revision 1.11  2004/09/08 13:56:33  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.10  2003/12/24 15:24:13  cargilld
 * More updates to memory management so that the static memory manager.
 *
 * Revision 1.9  2003/12/17 03:56:15  neilg
 * add default memory manager parameter to loadMsg method that uses char * parameters
 *
 * Revision 1.8  2003/05/15 18:29:49  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.7  2003/03/09 16:41:40  peiyongz
 * PanicHandler
 *
 * Revision 1.6  2002/12/12 19:14:00  peiyongz
 * InMem Message file name changed.
 *
 * Revision 1.5  2002/11/12 17:26:53  tng
 * DOM Message: add new domain for DOM Messages.
 *
 * Revision 1.4  2002/11/04 15:10:40  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/09/24 19:57:59  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.2  2002/09/23 22:14:37  peiyongz
 * Code sharing and Array boundary checking added
 *
 * Revision 1.1.1.1  2002/02/01 22:22:21  peiyongz
 * sane_include
 *
 * Revision 1.5  2000/03/28 19:43:21  roddey
 * Fixes for signed/unsigned warnings. New work for two way transcoding
 * stuff.
 *
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
#include <xercesc/util/BitOps.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLMsgLoader.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUni.hpp>
#include "InMemMsgLoader.hpp"
#include "XercesMessages_en_US.hpp"

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Public Constructors and Destructor
// ---------------------------------------------------------------------------
InMemMsgLoader::InMemMsgLoader(const XMLCh* const msgDomain)
:fMsgDomain(0)
{
    if (!XMLString::equals(msgDomain, XMLUni::fgXMLErrDomain)
    &&  !XMLString::equals(msgDomain, XMLUni::fgExceptDomain)
    &&  !XMLString::equals(msgDomain, XMLUni::fgXMLDOMMsgDomain)
    &&  !XMLString::equals(msgDomain, XMLUni::fgValidityDomain))
    {
        XMLPlatformUtils::panic(PanicHandler::Panic_UnknownMsgDomain);
    }

    fMsgDomain = XMLString::replicate(msgDomain, XMLPlatformUtils::fgMemoryManager);
}

InMemMsgLoader::~InMemMsgLoader()
{
    XMLPlatformUtils::fgMemoryManager->deallocate(fMsgDomain);//delete [] fMsgDomain;
}


// ---------------------------------------------------------------------------
//  Implementation of the virtual message loader API
// ---------------------------------------------------------------------------
bool InMemMsgLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned int            maxChars)
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
    const XMLCh* srcPtr = 0;

    if (XMLString::equals(fMsgDomain, XMLUni::fgXMLErrDomain))
    {
        if ( msgToLoad > gXMLErrArraySize)
            return false;
        else
            srcPtr = gXMLErrArray[msgToLoad - 1];
    }
     else if (XMLString::equals(fMsgDomain, XMLUni::fgExceptDomain))
    {
         if ( msgToLoad > gXMLExceptArraySize)
            return false;
         else
             srcPtr = gXMLExceptArray[msgToLoad - 1];
    }
     else if (XMLString::equals(fMsgDomain, XMLUni::fgValidityDomain))
    {
         if ( msgToLoad > gXMLValidityArraySize)
            return false;
         else
             srcPtr = gXMLValidityArray[msgToLoad - 1];
    }
     else if (XMLString::equals(fMsgDomain, XMLUni::fgXMLDOMMsgDomain))
    {
         if ( msgToLoad > gXMLDOMMsgArraySize)
            return false;
         else
             srcPtr = gXMLDOMMsgArray[msgToLoad - 1];
    }

     while (*srcPtr && (outPtr < endPtr))
     {
         *outPtr++ = *srcPtr++;
     }
     *outPtr = 0;

     return true;
}


bool InMemMsgLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
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


bool InMemMsgLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned int            maxChars
                            , const char* const             repText1
                            , const char* const             repText2
                            , const char* const             repText3
                            , const char* const             repText4
                            , MemoryManager * const         manager) 
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
