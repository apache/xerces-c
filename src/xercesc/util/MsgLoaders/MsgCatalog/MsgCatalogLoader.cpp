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
 * Revision 1.17  2003/12/24 15:24:13  cargilld
 * More updates to memory management so that the static memory manager.
 *
 * Revision 1.16  2003/12/17 03:56:15  neilg
 * add default memory manager parameter to loadMsg method that uses char * parameters
 *
 * Revision 1.15  2003/08/21 16:36:08  peiyongz
 * use PlatformUtils::panic()
 *
 * Revision 1.14  2003/05/15 18:29:49  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.13  2003/03/09 16:42:05  peiyongz
 * PanicHandler
 *
 * Revision 1.12  2003/02/20 20:20:11  peiyongz
 * Allow set user specified error message file location in PlatformUtils::Initialize().
 *
 * Revision 1.11  2002/12/12 16:46:18  peiyongz
 * MsgCatalog file name changed.
 *
 * Revision 1.10  2002/12/06 16:49:47  peiyongz
 * $XERCESCROOT/msg created as home directory for message files.
 *
 * Revision 1.9  2002/12/04 18:03:13  peiyongz
 * use $XERCESCROOT to search for msg catalog file if XERCESC_NLS_HOME
 * undefined
 *
 * Revision 1.8  2002/12/02 21:58:43  peiyongz
 * nls support
 *
 * Revision 1.7  2002/11/12 17:27:12  tng
 * DOM Message: add new domain for DOM Messages.
 *
 * Revision 1.6  2002/11/05 16:54:46  peiyongz
 * Using XERCESC_NLS_HOME
 *
 * Revision 1.5  2002/11/04 15:10:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/09/24 19:58:33  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.3  2002/09/23 21:05:40  peiyongz
 * remove debugging code
 *
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

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Public Constructors and Destructor
// ---------------------------------------------------------------------------
MsgCatalogLoader::MsgCatalogLoader(const XMLCh* const msgDomain)
:fCatalogHandle(0)
,fMsgSet(0)
{
    if (!XMLString::equals(msgDomain, XMLUni::fgXMLErrDomain)
    &&  !XMLString::equals(msgDomain, XMLUni::fgExceptDomain)
    &&  !XMLString::equals(msgDomain, XMLUni::fgXMLDOMMsgDomain)
    &&  !XMLString::equals(msgDomain, XMLUni::fgValidityDomain))
    {
        XMLPlatformUtils::panic(PanicHandler::Panic_UnknownMsgDomain);
    }

    // Prepare the path info
    char locationBuf[1024];
    memset(locationBuf, 0, sizeof locationBuf);
    const char *nlsHome = XMLMsgLoader::getNLSHome();
    
    if (nlsHome)
    {
    	strcpy(locationBuf, nlsHome);
        strcat(locationBuf, "/");
    }
    else
    {
        nlsHome = getenv("XERCESC_NLS_HOME");
        if (nlsHome)
        {
            strcpy(locationBuf, nlsHome);
            strcat(locationBuf, "/");
        }
        else
        {
            nlsHome = getenv("XERCESCROOT");
            if (nlsHome)
            {                       	
                strcpy(locationBuf, nlsHome);
                strcat(locationBuf, "/msg/");
            }
        }    
    }
    
    // Prepare user-specified locale specific cat file
    char catuser[1024];
    memset(catuser, 0, sizeof catuser);
    strcpy(catuser, locationBuf);
    strcat(catuser, "XercesMessages_");
    strcat(catuser, XMLMsgLoader::getLocale());
    strcat(catuser, ".cat");
        
    char catdefault[1024];
    memset(catdefault, 0, sizeof catdefault);
    strcpy(catdefault, locationBuf);
    strcat(catdefault, "XercesMessages_en_US.cat");

   /**
    * To open user-specified locale specific cat file
    * and default cat file if necessary
    */
    if ( ((int)(fCatalogHandle=catopen(catuser, 0)) == -1) &&
         ((int)(fCatalogHandle=catopen(catdefault, 0)) == -1)   )
    {
        // Probably have to call panic here
        printf("Could not open catalog:\n %s\n or %s\n", catuser, catdefault);
        XMLPlatformUtils::panic(PanicHandler::Panic_CantLoadMsgDomain);
    }

    if (XMLString::equals(msgDomain, XMLUni::fgXMLErrDomain))
        fMsgSet = CatId_XMLErrs;
    else if (XMLString::equals(msgDomain, XMLUni::fgExceptDomain))
        fMsgSet = CatId_XMLExcepts;
    else if (XMLString::equals(msgDomain, XMLUni::fgValidityDomain))
        fMsgSet = CatId_XMLValid;
    else if (XMLString::equals(msgDomain, XMLUni::fgXMLDOMMsgDomain))
        fMsgSet = CatId_XMLDOMMsg;
}

MsgCatalogLoader::~MsgCatalogLoader()
{
    catclose(fCatalogHandle);	
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
    if (XMLString::equals(catMessage, msgString))
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


bool MsgCatalogLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
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
