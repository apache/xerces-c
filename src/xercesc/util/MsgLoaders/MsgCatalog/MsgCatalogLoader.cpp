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
 * Revision 1.2  2002/09/23 21:03:06  peiyongz
 * Build MsgCatalog on Solaris
 *
 * Revision 1.1.1.1  2002/02/01 22:22:21  peiyongz
 * sane_include
 *
 * Revision 1.7  2001/10/09 12:19:44  tng
 * Leak fix: can call transcode directly instead of using copyString.
 *
 * Revision 1.6  2000/07/25 22:28:40  aruna1
 * Char definitions in XMLUni moved to XMLUniDefs
 *
 * Revision 1.5  2000/03/02 19:55:16  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.4  2000/02/06 07:48:22  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.3  2000/01/05 22:00:22  aruna1
 * Modified message 'set' attribute reading sequence. Removed dependencies on  hard coded constants
 *
 * Revision 1.2  1999/12/23 01:43:37  aruna1
 * MsgCatalog support added for solaris
 *
 * Revision 1.1.1.1  1999/11/09 01:07:16  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:27  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLMsgLoader.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include "MsgCatalogLoader.hpp"
#include "XMLMsgCat_Ids.hpp"

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ---------------------------------------------------------------------------
//  Public Constructors and Destructor
// ---------------------------------------------------------------------------
MsgCatalogLoader::MsgCatalogLoader(const XMLCh* const msgDomain)
:fCatalogHandle(0)
,fMsgDomain(0)
,fMsgSet(0)
{
    if (XMLString::compareString(msgDomain, XMLUni::fgXMLErrDomain)
    &&  XMLString::compareString(msgDomain, XMLUni::fgExceptDomain)
    &&  XMLString::compareString(msgDomain, XMLUni::fgValidityDomain))
    {
        char* msgDom = XMLString::transcode(msgDomain);
        XMLPlatformUtils::panic(XMLPlatformUtils::Panic_UnknownMsgDomain);
    }

    // Try to get the module handle
    char* tempLoc = setlocale(LC_ALL, "");
    char catfile[256];

    strcpy(catfile, getenv("XERCESCROOT"));
    strcat(catfile, "/lib/msg/XMLMessages.cat");

    fCatalogHandle = catopen(catfile , 0);
    if ((int)fCatalogHandle == -1)
    {
        // Probably have to call panic here
        printf("Could not open catalog XMLMessages\n");
        // TBD: Tell user what the locale is
        exit(1);
    }

    fMsgDomain = XMLString::replicate(msgDomain);

    if (!XMLString::compareString(fMsgDomain, XMLUni::fgXMLErrDomain))
        fMsgSet = CatId_XMLErrs;
    else if (!XMLString::compareString(fMsgDomain, XMLUni::fgExceptDomain))
        fMsgSet = CatId_XMLExcepts;
    else if (!XMLString::compareString(fMsgDomain, XMLUni::fgValidityDomain))
        fMsgSet = CatId_XMLValid;
}

MsgCatalogLoader::~MsgCatalogLoader()
{
    catclose(fCatalogHandle);	
    delete [] fMsgDomain;
}


// ---------------------------------------------------------------------------
//  Implementation of the virtual message loader API
// ---------------------------------------------------------------------------
bool MsgCatalogLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                              ,       XMLCh*   const          toFill
                              , const unsigned int            maxChars)
{
    char msgString[100];
    sprintf(msgString, "Could not find message ID %d from message set %d\n", msgToLoad, fMsgSet);
    char* catMessage = catgets( fCatalogHandle, fMsgSet, (int)msgToLoad, msgString);

    // catgets returns a pointer to msgString if it fails to locate the message
    // from the message catalog
    if (XMLString::compareString(catMessage, msgString) == 0)
        return false;   
    else
    {
        XMLString::transcode(catMessage, toFill, maxChars);
        return true;
    }
	
}

bool MsgCatalogLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned int            maxChars
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


bool MsgCatalogLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned int            maxChars
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
