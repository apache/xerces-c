/*
 * (C) Copyright IBM Corp. 1999  All rights reserved.
 *
 * US Government Users Restricted Rights Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *
 * The program is provided "as is" without any warranty express or
 * implied, including the warranty of non-infringement and the implied
 * warranties of merchantibility and fitness for a particular purpose.
 * IBM will not be liable for any damages suffered by you as a result
 * of using the Program. In no event will IBM be liable for any
 * special, indirect or consequential damages or lost profits even if
 * IBM has been advised of the possibility of their occurrence. IBM
 * will not be liable for any third party claims against you.
 */

/**
 * @version Revision:  01 1.2 src/com/ibm/xml/util/MsgLoaders/MsgCatalog/MsgCatalogLoader.cpp, xml4c3Utils, xml4c3 
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XML4CDefs.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLMsgLoader.hpp>
#include <util/XMLString.hpp>
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
                            , const unsigned long           maxChars
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
    XMLString::copyString((XMLCh*)toFill,  XMLString::transcode(message.mtext));
	
    return true;
}

// ---------------------------------------------------------------------------
//  Implementation of the virtual message loader API
// ---------------------------------------------------------------------------

bool MsgCatalogLoader::loadMsg(const  XMLMsgLoader::XMLMsgId  msgToLoad
                            ,       XMLCh* const            toFill
                            , const unsigned long           maxChars)
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
                            , const unsigned long           maxChars
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
                            , const unsigned long           maxChars
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
