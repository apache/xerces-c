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
 * $Id$
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XercesDefs.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLMsgLoader.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/XMLUni.hpp>
#include "MsgLoader.hpp"
#include "XMLMessages.h"
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <qmhrtvm.h>
#include <qusec.h>


// ---------------------------------------------------------------------------
//  Public Constructors and Destructor
// ---------------------------------------------------------------------------
MsgCatalogLoader::MsgCatalogLoader(const XMLCh* const msgDomain) :

    fCatalogHandle(0)
    , fMsgDomain(0)
{

    fMsgDomain = XMLString::replicate(msgDomain);
}

MsgCatalogLoader::~MsgCatalogLoader()
{

    delete fMsgDomain;
}


char* PackingRepText(const char * const repText1,
		     const char * const repText2,
		     const char * const repText3,
		     const char * const repText4,
		     int& totalsize)
{
    short sizes[4];
    char* reps[4];
    char *buffer;		// Allocate just what we need
    char* anchor;
    char temp[2];


    reps[0] = (char*)repText1;
    reps[1] = (char*)repText2;
    reps[2] = (char*)repText3;
    reps[3] = (char*)repText4;
    int count = 0;

    for(int i = 0; i < 4; i++)
	if(reps[i] != NULL)
	{				// Collect all the sizes and count how many we've got
	    sizes[i] = strlen(reps[i]);
            totalsize += sizes[i]+2;
	    count++;
	}

    buffer = (char*)malloc(totalsize);		// Allocate just what we need
    anchor = buffer;

    for(i = 0; i < count; i++)		// Stuff everything into the buffer
    {
	
	memcpy(buffer, (void *)&sizes[i], sizeof(short));
	buffer += sizeof(short);
	memcpy(buffer, reps[i], sizes[i]);
	buffer += sizes[i];
    }

    return anchor;		// This guy is still holding the beginning of the buffer

}

// ---------------------------------------------------------------------------
//  Implementation of the local retrieve message
// ---------------------------------------------------------------------------


bool localMsg(char * msgId
                            ,       XMLCh* const            toFill
                            , const unsigned int            maxChars
			    ,		char*		    repTexts
			    ,		int		    size)
{

    typedef _Packed struct {
	Qmh_Rtvm_RTVM0100_t mdata;        /* Basic Message Data                  */
	char                mtext[1024];  /* Kind of arbitrary, enough?      */
    } MyMessage_t;

    /* Information about the message that we're going to retrieve               */
    char         msgformat[9]  = "RTVM0100";
    char         msgfile[21]   = "QXMLMSG   *LIBL     ";
    char	 *msgsubstdta  = repTexts;		// ??
    int	  msgsubstlen = 0;
    if(repTexts)
	msgsubstlen = size;  //strlen(repTexts);	// ??
    char         msgsubst[11]  = "*YES      ";
    char         msgfmtctl[11] = "*NO       ";
    Qus_EC_t     errorcode;
    MyMessage_t  message;                 /* message we'll retrieve        */


  memset(&errorcode, 0, sizeof(errorcode));    /* Clear the error area   */
  errorcode.Bytes_Provided = sizeof(errorcode);/* Init the error code    */

  memset(&message, 0, sizeof(message.mdata));  /* Clear the message area */
  message.mtext[0]='\0';                       /* Null Text String       */

  int len = sizeof(message);
  for(int i = 0; i < 1024; i++)
      message.mtext[i] = '\0';

  QMHRTVM( &message, len, msgformat, msgId,
           msgfile, msgsubstdta, msgsubstlen, msgsubst, msgfmtctl, &errorcode);  /*,
	   "*MSGID    ", 1200, 1200);  */

                                         /* The error message should now be stored in message.mtext */
					 /* with the CCSID 1200					    */

//    char msgString[100];
//    char* catMessage = catgets( fCatalogHandle, msgSet, (int)msgToLoad, msgString);
    XMLString::transcode(message.mtext, toFill, maxChars);
	
    return true;
}

// ---------------------------------------------------------------------------
//  Implementation of the virtual message loader API
// ---------------------------------------------------------------------------

bool MsgCatalogLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned int            maxChars)
{
    char * msgId;
    char * repTexts = NULL;
    int size = 0;
    if (!XMLString::compareString(fMsgDomain, XMLUni::fgXMLErrDomain))
    {
	if(msgToLoad < 7)	// Warning messages
	    msgId = Warnings[msgToLoad - 2];
	else			// Fatal Errors
	    msgId = Errors[msgToLoad - 9];
    }
    else if (!XMLString::compareString(fMsgDomain, XMLUni::fgExceptDomain))
        msgId = Exceptions[msgToLoad - 2];
    else if (!XMLString::compareString(fMsgDomain, XMLUni::fgValidityDomain))
	msgId = Invalid[msgToLoad - 2];

    if (!localMsg(msgId, toFill, maxChars, repTexts, size))
        return false;
	
    return true;
}


bool MsgCatalogLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned int            maxChars
                            , const char* const            repText1
                            , const char* const            repText2
                            , const char* const            repText3
                            , const char* const            repText4)
{
    char * msgId;
    int size=0;
    char* repTexts = PackingRepText(repText1, repText2, repText3, repText4, size);
    if (!XMLString::compareString(fMsgDomain, XMLUni::fgXMLErrDomain))
    {
	if(msgToLoad < 7)
	    msgId = Warnings[msgToLoad - 2];
	else
	    msgId = Errors[msgToLoad - 9];
    }
    else if (!XMLString::compareString(fMsgDomain, XMLUni::fgExceptDomain))
        msgId = Exceptions[msgToLoad - 2];
    else if (!XMLString::compareString(fMsgDomain, XMLUni::fgValidityDomain))
	msgId = Invalid[msgToLoad - 2];

    // Call the other version to load up the message
    if (!localMsg(msgId, toFill, maxChars, repTexts, size))
        return false;


    return true;
}


bool MsgCatalogLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned int           maxChars
                            , const XMLCh* const             repText1
                            , const XMLCh* const             repText2
                            , const XMLCh* const             repText3
                            , const XMLCh* const             repText4)
{
    //
    //  Transcode the provided parameters and call the other version,
    //  which will do the replacement work.
    //
    char* tmp1 = 0;
    char* tmp2 = 0;
    char* tmp3 = 0;
    char* tmp4 = 0;

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
