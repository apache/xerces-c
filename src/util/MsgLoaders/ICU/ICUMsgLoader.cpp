/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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

/**
 * $Log$
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
#include <util/XML4CDefs.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLMsgLoader.hpp>
#include <util/XMLString.hpp>
#include "ICUMsgLoader.hpp"

#include "resbund.h"
#include "string.h"


// ---------------------------------------------------------------------------
//  Local static methods
// ---------------------------------------------------------------------------
static const XMLCh* mapId(  const   XMLMsgLoader::XMLMsgId   msgToLoad
                            , const XMLCh*                   bundleType)
{
    static const XMLCh test[] = { 0x65, 0x66, 0 };
    return test;
}


// ---------------------------------------------------------------------------
//  Public Constructors and Destructor
// ---------------------------------------------------------------------------
ICUMsgLoader::ICUMsgLoader(const XMLCh* const  toLoad) :
    fBundle(0)
    , fBundleType((XMLCh*)toLoad)
{
    //
    //  Use the platform util's path to figure out where the resource bundles
    //  are. We keep them in an NLS directory underneath the DLL/Lib path.
    //
    const unsigned int maxChars = 4095;
    char tmpPath[maxChars + 1];
    strncpy(tmpPath, XMLPlatformUtils::fgLibLocation, maxChars);

    // Now copy the relative path part onto it
    strncat(tmpPath, "NLS//ICU//", maxChars);

    if (toLoad == 0)
        strncat(tmpPath, "XMLExceptMsgs", maxChars);
    else if (toLoad == 0)
        strncat(tmpPath, "XMLErrors", maxChars);
    else
    {
        // <TBD> Need to do something here. Panic call?
    }

    // Ok, lets try to create the bundle now
    UErrorCode err = U_ZERO_ERROR;
    fBundle = new ResourceBundle(tmpPath, err);
    if (!U_SUCCESS(err))
    {
        // <TBD> Need to panic again here?
    }
}

ICUMsgLoader::~ICUMsgLoader()
{
    delete fBundle;
}


// ---------------------------------------------------------------------------
//  Implementation of the virtual message loader API
// ---------------------------------------------------------------------------
bool ICUMsgLoader::loadMsg( const   XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned long           maxChars)
{
    //
    //  Map the passed id to the required message bundle key name. We use
    //  a local array to map the message id to the correct string. This array
    //  is generated from a little utility program that can kick out the
    //  message text in a couple of different formats.
    //
    const XMLCh* const keyStr = mapId(msgToLoad, fBundleType);
    if (!keyStr)
        return false;

    // And now try to load that key's related message
    UErrorCode err;
    UnicodeString keyVal(keyStr);
    const UnicodeString* msgString = fBundle->getString(keyVal, err);
    if (!msgString)
        return false;

    // Extract out from the UnicodeString to the passed buffer
    const unsigned int len = msgString->length();
    const unsigned int lesserLen = (len < maxChars) ? len : maxChars;
    msgString->extract
    (
        0
        , lesserLen
        , toFill
    );

    // Cap it off and return success
    toFill[lesserLen] = 0;
    return true;
}


bool ICUMsgLoader::loadMsg( const   XMLMsgLoader::XMLMsgId  msgToLoad
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


bool ICUMsgLoader::loadMsg( const   XMLMsgLoader::XMLMsgId  msgToLoad
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
