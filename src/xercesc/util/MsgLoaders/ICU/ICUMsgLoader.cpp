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
 * Revision 1.26  2004/11/11 04:16:03  peiyongz
 * Build with ICU3.2
 *
 * Revision 1.25  2004/11/08 03:10:40  peiyongz
 * Using different package name for windows/Unix
 *
 * Revision 1.24  2004/11/04 18:38:44  peiyongz
 * udata_setAppData and ures_open refer to the same pkgname
 * 
 * Revision 1.23  2004/09/30 18:52:59  peiyongz
 * XercesC2_6_0 updates
 *
 * Revision 1.22  2004/09/08 13:56:33  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.21  2004/02/12 20:44:34  peiyongz
 * XercesC2_5_0
 *
 * Revision 1.20  2003/12/24 15:24:13  cargilld
 * More updates to memory management so that the static memory manager.
 *
 * Revision 1.19  2003/12/17 03:56:15  neilg
 * add default memory manager parameter to loadMsg method that uses char * parameters
 *
 * Revision 1.18  2003/11/19 23:08:02  peiyongz
 * build xercesc2_4_0
 *
 * Revision 1.17  2003/05/15 18:29:48  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.16  2003/03/17 19:28:05  peiyongz
 * Build versioned XercesMessages on Windows
 *
 * Revision 1.15  2003/03/09 16:41:20  peiyongz
 * PanicHandler
 *
 * Revision 1.14  2003/02/20 18:07:46  peiyongz
 * Bug#7077: build error message shared library for ICUMsgLoader
 *
 * Revision 1.12  2003/02/17 19:56:03  peiyongz
 * Re-prioritize search order for error message files.
 *
 * Revision 1.11  2002/12/12 16:53:25  peiyongz
 * Message file name changed.
 *
 * Revision 1.10  2002/12/06 16:29:17  peiyongz
 * $XERCESCROOT/msg created as home directory for message files, and
 * set default locale.
 *
 * Revision 1.9  2002/12/04 18:11:23  peiyongz
 * use $XERCESCROOT to search for icu resource bundle if XERCESC_NLS_HOME
 * undefined
 *
 * Revision 1.8  2002/11/20 20:28:17  peiyongz
 * fix to warning C4018: '>' : signed/unsigned mismatch
 *
 * Revision 1.7  2002/11/12 17:27:49  tng
 * DOM Message: add new domain for DOM Messages.
 *
 * Revision 1.6  2002/11/04 22:24:43  peiyongz
 * Locale setting for message loader
 *
 * Revision 1.5  2002/11/04 15:10:40  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/10/10 21:07:55  peiyongz
 * load resource files using environement vars and base name
 *
 * Revision 1.3  2002/10/02 17:08:50  peiyongz
 * XMLString::equals() to replace XMLString::compareString()
 *
 * Revision 1.2  2002/09/30 22:20:40  peiyongz
 * Build with ICU MsgLoader
 *
 * Revision 1.1.1.1  2002/02/01 22:22:19  peiyongz
 * sane_include
 *
 * Revision 1.7  2002/01/21 14:52:25  tng
 * [Bug 5847] ICUMsgLoader can't be compiled with gcc 3.0.3 and ICU2.  And also fix the memory leak introduced by Bug 2730 fix.
 *
 * Revision 1.6  2001/11/01 23:39:18  jasons
 * 2001-11-01  Jason E. Stewart  <jason@openinformatics.com>
 *
 * 	* src/util/MsgLoaders/ICU/ICUMsgLoader.hpp (Repository):
 * 	* src/util/MsgLoaders/ICU/ICUMsgLoader.cpp (Repository):
 * 	Updated to compile with ICU-1.8.1
 *
 * Revision 1.5  2000/03/02 19:55:14  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.4  2000/02/06 07:48:21  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.3  2000/01/19 00:58:38  roddey
 * Update to support new ICU 1.4 release.
 *
 * Revision 1.2  1999/11/19 21:24:03  aruna1
 * incorporated ICU 1.3.1 related changes int he file
 *
 * Revision 1.1.1.1  1999/11/09 01:07:23  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:45:26  rahul
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
#include <xercesc/util/Janitor.hpp>
#include "ICUMsgLoader.hpp"
#include "unicode/putil.h"
#include "unicode/uloc.h"
#include "unicode/udata.h" 

#include "string.h"
#include <stdio.h>
#include <stdlib.h>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Local static methods
// ---------------------------------------------------------------------------

/*
 *  Resource Data Reference.  
 * 
 *  The data is packaged as a dll (or .so or whatever, depending on the platform) that exports a data symbol.
 *  The application (thic *.cpp) references that symbol here, and will pass the data address to ICU, which 
 *  will then  be able to fetch resources from the data.
 */

#if defined(_WIN32) || defined(WIN32)
extern "C" void U_IMPORT *XercesMessages2_6_dat;
#else
extern "C" void U_IMPORT *XercesMessages2_6_0_dat;
#endif

/* 
 *  Tell ICU where our resource data is located in memory. The data lives in the XercesMessages dll, and we just
 *  pass the address of an exported symbol from that library to ICU.
 */
static bool setAppDataOK = false;

static void setAppData()
{
    static bool setAppDataDone = false;

    if (setAppDataDone)
    {
        return;
    }
    else
    {
        setAppDataDone = true;
        UErrorCode err = U_ZERO_ERROR;
#if defined(_WIN32) || defined(WIN32)
        udata_setAppData("XercesMessages2_6", &XercesMessages2_6_dat, &err);
#else
        udata_setAppData("XercesMessages2_6_0", &XercesMessages2_6_0_dat, &err);
#endif        
        if (U_SUCCESS(err))
        {
    	    setAppDataOK = true;
        }
    }

}

// ---------------------------------------------------------------------------
//  Public Constructors and Destructor
// ---------------------------------------------------------------------------
ICUMsgLoader::ICUMsgLoader(const XMLCh* const  msgDomain)
:fLocaleBundle(0)
,fDomainBundle(0)
{
    /***
	    Validate msgDomain
    ***/
    if (!XMLString::equals(msgDomain, XMLUni::fgXMLErrDomain)    &&
        !XMLString::equals(msgDomain, XMLUni::fgExceptDomain)    &&
        !XMLString::equals(msgDomain, XMLUni::fgXMLDOMMsgDomain) &&
        !XMLString::equals(msgDomain, XMLUni::fgValidityDomain)   )
    {
        XMLPlatformUtils::panic(PanicHandler::Panic_UnknownMsgDomain);
    }

    /***
	Resolve domainName
    ***/
    int     index = XMLString::lastIndexOf(msgDomain, chForwardSlash);
    char*   domainName = XMLString::transcode(&(msgDomain[index + 1]), XMLPlatformUtils::fgMemoryManager);
    ArrayJanitor<char> jan1(domainName, XMLPlatformUtils::fgMemoryManager);

    /***
        Location resolution priority
         
         1. XMLMsgLoader::getNLSHome(), set by user through
            XMLPlatformUtils::Initialize(), which provides user-specified
            location where the message loader shall retrieve error messages.

         2. envrionment var: XERCESC_NLS_HOME

         3. path $XERCESCROOT/msg
    ***/

    char locationBuf[1024];
    memset(locationBuf, 0, sizeof locationBuf);
    const char *nlsHome = XMLMsgLoader::getNLSHome();

    if (nlsHome)
    {
    	strcpy(locationBuf, nlsHome);
        strcat(locationBuf, U_FILE_SEP_STRING);
    }
    else
    {
        nlsHome = getenv("XERCESC_NLS_HOME");
        if (nlsHome)
        {
            strcpy(locationBuf, nlsHome);
            strcat(locationBuf, U_FILE_SEP_STRING);
        }
        else
        {
            nlsHome = getenv("XERCESCROOT");
            if (nlsHome)
            {
                strcpy(locationBuf, nlsHome);
                strcat(locationBuf, U_FILE_SEP_STRING);
                strcat(locationBuf, "msg");
                strcat(locationBuf, U_FILE_SEP_STRING);                    
            }
            else
            {
                /***
                 leave it to ICU to decide where to search
                 for the error message.
                 ***/
                 setAppData();
            }
        }    
    }

    /***
	Open the locale-specific resource bundle
    ***/
#if defined(_WIN32) || defined(WIN32)
    strcat(locationBuf, "XercesMessages2_6");
#else
    strcat(locationBuf, "XercesMessages2_6_0");
#endif
    UErrorCode err = U_ZERO_ERROR;
    uloc_setDefault("en_US", &err);   // in case user-specified locale unavailable
    err = U_ZERO_ERROR;
    fLocaleBundle = ures_open(locationBuf, XMLMsgLoader::getLocale(), &err);
    if (!U_SUCCESS(err) || fLocaleBundle == NULL)
    {
    	/***
    	   in case user specified location does not work
    	   try the dll
        ***/

#if defined(_WIN32) || defined(WIN32)
        if (strcmp(locationBuf, "XercesMessages2_6") !=0 )
#else
        if (strcmp(locationBuf, "XercesMessages2_6_0") !=0 )
#endif        
        {    	     	   
            setAppData();        	
            err = U_ZERO_ERROR;
#if defined(_WIN32) || defined(WIN32)            
            fLocaleBundle = ures_open("XercesMessages2_6", XMLMsgLoader::getLocale(), &err);
#else
            fLocaleBundle = ures_open("XercesMessages2_6_0", XMLMsgLoader::getLocale(), &err);
#endif            
            if (!U_SUCCESS(err) || fLocaleBundle == NULL)
            {
                 XMLPlatformUtils::panic(PanicHandler::Panic_CantLoadMsgDomain);
            }
        }
        else
        {    	     	   
            XMLPlatformUtils::panic(PanicHandler::Panic_CantLoadMsgDomain);
        }        
    }

    /***
	Open the domain specific resource bundle within
	the locale-specific resource bundle
    ***/
    err = U_ZERO_ERROR;
    fDomainBundle = ures_getByKey(fLocaleBundle, domainName, NULL, &err);
    if (!U_SUCCESS(err) || fDomainBundle == NULL)
    {
        XMLPlatformUtils::panic(PanicHandler::Panic_CantLoadMsgDomain);
    }

}

ICUMsgLoader::~ICUMsgLoader()
{
    ures_close(fDomainBundle);
    ures_close(fLocaleBundle);
}


// ---------------------------------------------------------------------------
//  Implementation of the virtual message loader API
// ---------------------------------------------------------------------------
bool ICUMsgLoader::loadMsg( const   XMLMsgLoader::XMLMsgId  msgToLoad
                          ,         XMLCh* const            toFill
                          , const   unsigned int            maxChars)
{
    UErrorCode   err = U_ZERO_ERROR;
    int32_t      strLen = 0;

    // Assuming array format
    const UChar *name = ures_getStringByIndex(fDomainBundle, (int32_t)msgToLoad-1, &strLen, &err);

    if (!U_SUCCESS(err) || (name == NULL))
    {
        return false;
    }

    int retStrLen = strLen > (int32_t)maxChars ? maxChars : strLen;

    if (sizeof(UChar)==sizeof(XMLCh))
    {
        XMLString::moveChars(toFill, (XMLCh*)name, retStrLen);
        toFill[retStrLen] = (XMLCh) 0;
    }
    else
    {
        XMLCh* retStr = toFill;
        const UChar *srcPtr = name;

        while (retStrLen--)
           *retStr++ = *srcPtr++;

        *retStr = 0;
    }

    return true;
}


bool ICUMsgLoader::loadMsg( const   XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned int            maxChars
                            , const XMLCh* const            repText1
                            , const XMLCh* const            repText2
                            , const XMLCh* const            repText3
                            , const XMLCh* const            repText4
                            , MemoryManager* const          manager   )
{
    // Call the other version to load up the message
    if (!loadMsg(msgToLoad, toFill, maxChars))
        return false;

    // And do the token replacement
    XMLString::replaceTokens(toFill, maxChars, repText1, repText2, repText3, repText4, manager);
    return true;
}


bool ICUMsgLoader::loadMsg( const   XMLMsgLoader::XMLMsgId  msgToLoad
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
