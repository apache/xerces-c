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
 * Revision 1.22  2004/12/06 11:45:14  cargilld
 * Add check to Xlat to see if message text is greater than 128 characters.
 *
 * Revision 1.21  2004/09/08 13:57:07  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.20  2004/05/05 10:53:08  amassari
 * Make it compile again on Windows
 *
 * Revision 1.19  2004/04/20 15:30:49  peiyongz
 * to generate code for private default constructor
 *
 * Revision 1.18  2003/05/14 19:11:34  gareth
 * Added code creation for new method that returns error serverity the DOM way.
 *
 * Revision 1.17  2003/04/14 08:41:00  gareth
 * Xlat now works under linux - Big thanks to Neil Graham (I no longer have to find a windows box). Still slight problems working with glibc before 2.2.4 (If you mess up the parameters it seg faults due to handling of wprintf)
 *
 * Revision 1.16  2002/12/12 23:40:39  peiyongz
 * normlize locale string.
 *
 * Revision 1.15  2002/11/12 17:24:58  tng
 * DOM Message: add new domain for DOM Messages.
 *
 * Revision 1.14  2002/11/04 15:24:50  tng
 * C++ Namespace Support.
 *
 * Revision 1.13  2002/09/30 22:09:58  peiyongz
 * To generate icu resource file (in text) for error message.
 *
 * Revision 1.12  2002/07/04 17:40:07  tng
 * Use new DOM in Xlat.
 *
 * Revision 1.11  2002/03/01 16:18:38  tng
 * Nest entire code in an inner block so that reference counting should recover
 * all document and DOMString storage when this block exits.
 *
 * Revision 1.10  2002/02/07 16:19:40  knoaman
 * sane_include update.
 *
 * Revision 1.9  2002/02/01 23:48:37  peiyongz
 * sane_include
 *
 * Revision 1.8  2001/05/03 19:09:36  knoaman
 * Support Warning/Error/FatalError messaging.
 * Validity constraints errors are treated as errors, with the ability by user to set
 * validity constraints as fatal errors.
 *
 * Revision 1.7  2000/03/18 00:00:58  roddey
 * Improved error reporting
 *
 * Revision 1.6  2000/03/03 01:29:36  roddey
 * Added a scanReset()/parseReset() method to the scanner and
 * parsers, to allow for reset after early exit from a progressive parse.
 * Added calls to new Terminate() call to all of the samples. Improved
 * documentation in SAX and DOM parsers.
 *
 * Revision 1.5  2000/03/02 19:55:52  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.4  2000/02/14 19:25:44  roddey
 * Fixed some small bugs in how it used strings returned from DOM calls.
 *
 * Revision 1.3  2000/02/06 07:48:41  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/01/05 20:24:58  roddey
 * Some changes to simplify life for the Messge Catalog message loader. The formatter
 * for the message loader now spits out a simple header of ids that allows the loader to
 * be independent of hard coded set numbers.
 *
 * Revision 1.1.1.1  1999/11/09 01:01:12  twl
 * Initial checkin
 *
 * Revision 1.5  1999/11/08 20:42:04  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  This program is designed to parse an XML file which holds error text
//  data. It will build a DOM tree from that source file and can output it
//  a number of different formats.
//
//  In order to drastically simplify the program, it is designed only to run
//  on platforms/compilers that understand Unicode. It can output the data
//  in whatever format is required, so it can handle outputting for other
//  platforms. This also simplifies bootstrapping new releases up on other
//  platforms. Once the Win32 version is working, it can generate output for
//  the other platforms so that they can have loadable text from day one.
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "Xlat.hpp"


// ---------------------------------------------------------------------------
//  Static data
//
//  gRelativeInputPath
//      This is the path, relative to the given input source root, to the
//      input file. The given local suffix must also be added to it.
// ---------------------------------------------------------------------------
XMLCh*      gRelativeInputPath = 0;


// ---------------------------------------------------------------------------
//  Global data
// ---------------------------------------------------------------------------
XMLCh* typePrefixes[MsgTypes_Count];

// ---------------------------------------------------------------------------
//  temporary variables/conversion utility functions
//  We need different temps depending on treatment of wide characters
// ---------------------------------------------------------------------------
#ifdef longChars
    char* fTmpStr = 0;
#else 
    wchar_t fTmpWStr[256];
#endif 

// ---------------------------------------------------------------------------
//  Local data
//
//  gLocale
//      This is the locale suffix, e.g. US_EN, that is used to find the
//      correct file and can be used on output files as well. Its set via
//      the /Locale= parameter.
//
//  gOutFormat
//      This is the output format, which is given on the command line as
//      /OutFmt= Its mapped to the internal enum which is stored here.
//
//  gOutPath
//      This is the path to the output path, which is given on the command
//      line as /OutPath=. Its just the path, not a name, since the output
//      might consist of multiple output files. They will all be based on
//      the base part of the input name.
//
//  gSrcRoot
//      This the path to the root of the build tree. The input files needed
//      are found in known places relative to it.
// ---------------------------------------------------------------------------
const XMLCh*    gLocale = 0;
OutFormats      gOutFormat = OutFormat_Unknown;
const XMLCh*    gOutPath = 0;
const XMLCh*    gSrcRoot = 0;


// ---------------------------------------------------------------------------
//  Local utility methods
// ---------------------------------------------------------------------------
  
// Initialize the global "constants" (that really require use of the transcoder)
void init_Globals(void) 
{
    typePrefixes[0] = XMLString::transcode("W_");
    typePrefixes[1] = XMLString::transcode("E_");
    typePrefixes[2] = XMLString::transcode("F_");
    gRelativeInputPath  = XMLString::transcode("src/xercesc/NLS/");
}

// Release the global "constants" (that really require use of the transcoder)
void release_Globals(void) 
{
    for(int i=0; i<3; i++) 
    {
        XMLString::release(&typePrefixes[i]);
    }
    XMLString::release(&gRelativeInputPath);
}

//
//  This method is called to parse the parameters. They must be in this
//  order and format, for simplicity:
//
//  /SrcRoot=xxx /OutPath=xxx /OutFmt=xxx /Locale=xxx
//
//static bool parseParms(const int argC, XMLCh** argV)
bool parseParms(const int argC, XMLCh** argV)
{
    if (argC < 5)
        return false;

    unsigned int curParm = 1;
    XMLCh *tmpXMLStr = XMLString::transcode("/SrcRoot=");
    if (XMLString::startsWith(argV[curParm], tmpXMLStr))
    {
        gSrcRoot = &argV[curParm][9];
    }
     else
    {
        wprintf(L"\nExpected /SrcRoot=xxx. Got: %s\n", argV[curParm]);
        XMLString::release(&tmpXMLStr);
        return false;
    }
    XMLString::release(&tmpXMLStr);

    curParm++;
    tmpXMLStr = XMLString::transcode("/OutPath=");
    if (XMLString::startsWith(argV[curParm], tmpXMLStr ))
    {
        gOutPath = &argV[curParm][9];
    }
     else
    {
        wprintf(L"\nExpected /OutPath=xxx. Got: %s\n", argV[curParm]);
        XMLString::release(&tmpXMLStr);
        return false;
    }
    XMLString::release(&tmpXMLStr);


    curParm++;
    tmpXMLStr = XMLString::transcode("/OutFmt=");
    if (XMLString::startsWith(argV[curParm], tmpXMLStr ))
    {
        XMLString::release(&tmpXMLStr);
        const XMLCh* tmpFmt = &argV[curParm][8];
        tmpXMLStr = XMLString::transcode("ResBundle");
        XMLCh *tmpXMLStr2 = XMLString::transcode("Win32RC");
        XMLCh *tmpXMLStr3 = XMLString::transcode("CppSrc");
        XMLCh *tmpXMLStr4 = XMLString::transcode("MsgCat");
        if (!XMLString::compareIString(tmpFmt, tmpXMLStr ))
            gOutFormat = OutFormat_ResBundle;
        else if (!XMLString::compareIString(tmpFmt, tmpXMLStr2 ))
            gOutFormat = OutFormat_Win32RC;
        else if (!XMLString::compareIString(tmpFmt, tmpXMLStr3 ))
            gOutFormat = OutFormat_CppSrc;
        else if (!XMLString::compareIString(tmpFmt, tmpXMLStr4 ))
            gOutFormat = OutFormat_MsgCatalog;
        else
        {
            wprintf(L"\n'%s' is not a legal output format\n", tmpFmt);
            XMLString::release(&tmpXMLStr);
            XMLString::release(&tmpXMLStr2);
            XMLString::release(&tmpXMLStr3);
            XMLString::release(&tmpXMLStr4);
            return false;
        }
        XMLString::release(&tmpXMLStr);
        XMLString::release(&tmpXMLStr2);
        XMLString::release(&tmpXMLStr3);
        XMLString::release(&tmpXMLStr4);
    }
     else
    {
        wprintf(L"\nExpected /OutFmt=xxx. Got: %s\n", argV[curParm]);
        XMLString::release(&tmpXMLStr);
        return false;
    }

    curParm++;
    tmpXMLStr = XMLString::transcode("/Locale=");
    if (XMLString::startsWith(argV[curParm], tmpXMLStr ))
    {
        gLocale = &argV[curParm][8];
    }
     else
    {
        wprintf(L"\nExpected /Locale=xxx. Got: %s\n", argV[curParm]);
        XMLString::release(&tmpXMLStr);
        return false;
    }
    XMLString::release(&tmpXMLStr);

    return true;
}


//static void parseError(const XMLException& toCatch)
void parseError(const XMLException& toCatch)
{
    wprintf
    (
        L"Exception\n   (Line.File):%d.%s\n   ERROR: %s\n\n"
        , toCatch.getSrcLine()
        , toCatch.getSrcFile()
        , toCatch.getMessage()
    );
    throw ErrReturn_ParseErr;
}


//static void parseError(const SAXParseException& toCatch)
void parseError(const SAXParseException& toCatch)
{
    wprintf
    (
        L"SAX Parse Error:\n   (Line.Col.SysId): %d.%d.%s\n   ERROR: %s\n\n"
        , toCatch.getLineNumber()
        , toCatch.getColumnNumber()
        , toCatch.getSystemId()
        , toCatch.getMessage()
    );
    throw ErrReturn_ParseErr;
}


//static void
void
enumMessages(   const   DOMElement*             srcElem
                ,       XlatFormatter* const    toCall
                ,       FILE* const             headerFl
                , const MsgTypes                msgType
                ,       unsigned int&           count)
{
    fwprintf
    (
        headerFl
        , L"      , %s%-30s   = %d\n"
        , xmlStrToPrintable(typePrefixes[msgType]) 
        , longChars("LowBounds")
        , count++
    );
    releasePrintableStr

    //
    //  We just run through each of the child elements, each of which is
    //  a Message element. Each one represents a message to output. We keep
    //  a count so that we can output a const value afterwards.
    //
    DOMNode* curNode = srcElem->getFirstChild();
    while (curNode)
    {
        // Skip over text nodes or comment nodes ect...
        if (curNode->getNodeType() != DOMNode::ELEMENT_NODE)
        {
            curNode = curNode->getNextSibling();
            continue;
        }

        // Convert it to an element node
        const DOMElement* curElem = (const DOMElement*)curNode;

        // Ok, this should be a Message node
        XMLCh *tmpXMLStr = XMLString::transcode("Message");
        if (XMLString::compareString(curElem->getTagName(), tmpXMLStr ))
        {
            wprintf(L"Expected a Message node\n\n");
            XMLString::release(&tmpXMLStr);
            throw ErrReturn_SrcFmtError;
        }
        XMLString::release(&tmpXMLStr);

        //
        //  Ok, lets pull out the id, text value, and message type. These are
        //  to be passed to the formatter. We have to translate the message
        //  type into one of the offical enum values.
        //
        tmpXMLStr = XMLString::transcode("Text");
        const XMLCh* msgText = curElem->getAttribute(tmpXMLStr );
        XMLString::release(&tmpXMLStr);
        tmpXMLStr = XMLString::transcode("Id");
        const XMLCh* msgId   = curElem->getAttribute(tmpXMLStr );
        XMLString::release(&tmpXMLStr);

        //
        //  Write out an entry to the target header file. These are enums, so
        //  we use the id as the enum name.
        //

        if (XMLString::stringLen(msgText) > 128) {
            wprintf(L"Message text %s is too long, 128 character limit\n\n", xmlStrToPrintable(msgText));            
            throw ErrReturn_SrcFmtError;
        }


        fwprintf(headerFl, L"      , %-32s   = %d\n", xmlStrToPrintable(msgId), count);
        releasePrintableStr

        // And tell the formatter about this one
        toCall->nextMessage
        (
            msgText
            , msgId
            , count
            , count
        );

        // Bump the counter, which is also the id assigner
        count++;

        // Move to the next child of the source element
        curNode = curNode->getNextSibling();
    }

    // Write out an upper range bracketing id for this type of error
    fwprintf
    (
        headerFl
        , L"      , %s%-30s   = %d\n"
        , xmlStrToPrintable(typePrefixes[msgType])
        , longChars("HighBounds")
        , count++
    );
    releasePrintableStr
}



// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------

//
//  This is the program entry point. It checks the parms, parses the input
//  file to get a DOM tree, then passes the DOM tree to the appropriate
//  output method to output the info in a particular format.
//
int Xlat_main(int argC, XMLCh** argV);
int main (int argC, char** argV) {
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch(const XMLException& toCatch)
    {
        wprintf(L"Parser init error.\n  ERROR: %s\n\n", toCatch.getMessage());
        return ErrReturn_ParserInit;
    }
    
    int i;
    XMLCh** newArgV = new XMLCh*[argC];
    for(i=0;i<argC; i++) 
    {
        newArgV[i] = XMLString::transcode(argV[i]);
    }
    int toReturn = (Xlat_main(argC,newArgV));
    for (i=0; i<argC; i++) 
    {
        delete [] newArgV[i];
    }
    delete [] newArgV;
    return toReturn;
}

int Xlat_main(int argC, XMLCh** argV)
{
    init_Globals();

    //
    //  Lets check the parameters and save them away in globals for use by
    //  the processing code.
    //
    if (!parseParms(argC, argV))
    {
        wprintf(L"Usage:\n  NLSXlat /SrcRoot=xx /OutPath=xx /OutFmt=xx /Locale=xx\n\n");
        return ErrReturn_BadParameters;
    }

    {
        //  Nest entire code in an inner block.

        DOMDocument* srcDoc;
        const unsigned int bufSize = 4095;
        XMLCh *tmpFileBuf = new XMLCh [bufSize + 1];
        tmpFileBuf[0] = 0;
        XMLCh *tmpXMLStr = XMLString::transcode("/XMLErrList_");
        XMLCh *tmpXMLStr2 = XMLString::transcode(".Xml");
        try
        {
            try
            {
                // Build the input file name
                XMLString::catString(tmpFileBuf, gSrcRoot);
                XMLString::catString(tmpFileBuf, gRelativeInputPath);
                XMLString::catString(tmpFileBuf, gLocale);
                XMLString::catString(tmpFileBuf, tmpXMLStr );
                XMLString::catString(tmpFileBuf, gLocale);
                XMLString::catString(tmpFileBuf, tmpXMLStr2 );
                XMLString::release(&tmpXMLStr);
                XMLString::release(&tmpXMLStr2);

                //
                //  Ok, lets invoke the DOM parser on the input file and build
                //  a DOM tree. Turn on validation when we do this.
                //
                XercesDOMParser parser;
                parser.setDoValidation(true);
                XlatErrHandler errHandler;
                parser.setErrorHandler(&errHandler);
                parser.parse(tmpFileBuf);
                srcDoc = parser.adoptDocument();
            }

            catch(const XMLException& toCatch)
            {
                parseError(toCatch);
            }
            XMLString::release(&tmpFileBuf);

            //
            //  Use the output format parm to create the correct kind of output
            //  formatter.
            //
            XlatFormatter* formatter = 0;
            switch(gOutFormat)
            {
                case OutFormat_CppSrc :
                    formatter = new CppSrcFormatter;
                    break;

                case OutFormat_Win32RC :
                    formatter = new Win32RCFormatter;
                    break;

                case OutFormat_MsgCatalog :
                    formatter = new MsgCatFormatter;
                    break;

                case OutFormat_ResBundle:
                    formatter = new ICUResBundFormatter;
                    break;

                default :
                    wprintf(L"Unknown formatter type enum\n\n");
                    throw ErrReturn_Internal;
            }

            //
            //  Lets handle the root element stuff first. This one holds any over
            //  all information.
            //
            DOMElement* rootElem = srcDoc->getDocumentElement();
            tmpXMLStr = XMLString::transcode("Locale");
            const XMLCh* localeStr = rootElem->getAttribute(tmpXMLStr);
            XMLString::release(&tmpXMLStr);

            // Make sure that the locale matches what we were given
            if (XMLString::compareString(localeStr, gLocale))
            {
                wprintf(L"The file's locale does not match the target locale\n");
                throw ErrReturn_LocaleErr;
            }

            //
            //  Get a list of all the MsgDomain children. These each hold one of
            //  the sets of (potentially separately) loadable messages. More
            //  importantly they all have their own error id space.
            //
            tmpXMLStr = XMLString::transcode("MsgDomain");
            DOMNodeList* msgSetList = rootElem->getElementsByTagName(tmpXMLStr);
            XMLString::release(&tmpXMLStr);

            //
            //  Loop through them and look for the domains that we know are
            //  supposed to be there.
            //
            const unsigned int count = msgSetList->getLength();

            //
            // Normalize locale string
            //
            // locale = ll[[_CC][_VARIANT]]
            // where ll          is language code
            //       CC          is country code
            //       VARIANT     is variant code
            //
            XMLCh normalizedLocale[256];

            normalizedLocale[0] = localeStr[0];
            normalizedLocale[1] = localeStr[1];
            normalizedLocale[2] = 0;
            XMLString::lowerCase(normalizedLocale);

            if (XMLString::stringLen(localeStr) > 2)
            {
                XMLString::catString(&(normalizedLocale[2]), &(localeStr[2]));
                XMLString::upperCase(&(normalizedLocale[2]));
            }

            //
            //  Ok, its good enough to get started. So lets call the start output
            //  method on the formatter.
            //
    
            formatter->startOutput(normalizedLocale, gOutPath);

            //
            //  For each message domain element, we call start and end domain
            //  events bracketed around the loop that sends out each message
            //  in that domain.
            //
            //  Within each domain, we check for the Warning, Error, and Validity
            //  subelements, and then iterate all the messages in each one.
            //
            for (unsigned int index = 0; index < count; index++)
            {
                // We know its a DOM Element, so go ahead and cast it
                DOMNode* curNode = msgSetList->item(index);
                const DOMElement* curElem = (const DOMElement*)curNode;

                //
                //  Get some of  the attribute strings that we need, and transcode
                //  couple that need to be in local format.
                //
                tmpXMLStr = XMLString::transcode("Domain");
                const XMLCh* domainStr = curElem->getAttribute(tmpXMLStr );
                XMLString::release(&tmpXMLStr);

                //
                //  Look at the domain and set up our application specific info
                //  that is on a per-domain basis. We need to indicate what the
                //  name of the header is and what the namespace is that they
                //  codes will go into
                //
                XMLCh* headerName = 0;
                XMLCh* errNameSpace = 0;
                if (!XMLString::compareString(domainStr, XMLUni::fgXMLErrDomain))
                {
                    headerName = XMLString::transcode("XMLErrorCodes.hpp");
                    errNameSpace = XMLString::transcode("XMLErrs");
                }
                 else if (!XMLString::compareString(domainStr, XMLUni::fgValidityDomain))
                {
                    headerName = XMLString::transcode("XMLValidityCodes.hpp");
                    errNameSpace = XMLString::transcode("XMLValid");
                }
                 else if (!XMLString::compareString(domainStr, XMLUni::fgExceptDomain))
                {
                    headerName = XMLString::transcode("XMLExceptMsgs.hpp");
                    errNameSpace = XMLString::transcode("XMLExcepts");
                }
                 else if (!XMLString::compareString(domainStr, XMLUni::fgXMLDOMMsgDomain))
                {
                    headerName = XMLString::transcode("XMLDOMMsg.hpp");
                    errNameSpace = XMLString::transcode("XMLDOMMsg");
                }
                 else
                {
                    // Not one of ours, so skip it
                    continue;
                }

                //
                //  Lets try to create the header file that was indicated for
                //  this domain.
                //
                tmpFileBuf = new XMLCh [bufSize + 1];
                tmpFileBuf[0] = 0;
                XMLString::catString(tmpFileBuf, gOutPath);
                XMLString::catString(tmpFileBuf, headerName);
                char *tmpFileBufCh = XMLString::transcode(tmpFileBuf);                
                FILE* outHeader = fopen(tmpFileBufCh, "wt+");
                XMLString::release(&tmpFileBufCh);
                if ((!outHeader) || (fwide(outHeader, 1) < 0)) 
                {
                    wprintf(L"Could not open domain header file: %s\n\n", xmlStrToPrintable(tmpFileBuf));
                    releasePrintableStr
                    XMLString::release(&tmpFileBuf);
                    XMLString::release(&headerName);
                    XMLString::release(&errNameSpace);
                    throw ErrReturn_OutFileOpenFailed;
                }
                XMLString::release(&tmpFileBuf);                

                //
                //  Write out the opening of the class they are nested within, and
                //  the header protection define.
                //                
                fwprintf(outHeader, L"// This file is generated, don't edit it!!\n\n");
                fwprintf(outHeader, L"#if !defined(ERRHEADER_%s)\n", xmlStrToPrintable(errNameSpace) );
                releasePrintableStr                
                fwprintf(outHeader, L"#define ERRHEADER_%s\n\n", xmlStrToPrintable(errNameSpace) );
                releasePrintableStr

                // If its not the exception domain, then we need a header included
                if (XMLString::compareString(domainStr, XMLUni::fgExceptDomain))
                    fwprintf(outHeader, L"#include <xercesc/framework/XMLErrorReporter.hpp>\n");

                //  Write out the namespace declaration
                fwprintf(outHeader, L"#include <xercesc/util/XercesDefs.hpp>\n");
                fwprintf(outHeader, L"#include <xercesc/dom/DOMError.hpp>\n\n");
                fwprintf(outHeader, L"XERCES_CPP_NAMESPACE_BEGIN\n\n");

                //  Now the message codes
                fwprintf(outHeader, L"class %s\n{\npublic :\n    enum Codes\n    {\n", xmlStrToPrintable(errNameSpace) );
                releasePrintableStr

                // Tell the formatter that a new domain is starting
                formatter->startDomain
                (
                    domainStr
                    , errNameSpace
                );

                //
                //  Force out the first message, which is always implicit and is
                //  the 'no error' entry for that domain.
                //
                unsigned int count = 0;
                fwprintf(outHeader, L"        %-32s   = %d\n", longChars("NoError"), count++);

                //
                //  Loop through the children of this node, which should take us
                //  through the optional Warning, Error, and Validity subsections.
                //
                DOMNode* typeNode = curElem->getFirstChild();
                bool typeGotten[3] = { false, false, false };
                while (typeNode)
                {
                    // Skip over text nodes or comment nodes ect...
                    if (typeNode->getNodeType() != DOMNode::ELEMENT_NODE)
                    {
                        typeNode = typeNode->getNextSibling();
                        continue;
                    }

                    // Convert it to an element node
                    const DOMElement* typeElem = (const DOMElement*)typeNode;

                    // Now get its tag name and convert that to a message type enum
                    const XMLCh* typeName = typeElem->getTagName();

                    MsgTypes type;
                    tmpXMLStr = XMLString::transcode("Warning");
                    XMLCh* tmpXMLStr2 = XMLString::transcode("Error");
                    XMLCh* tmpXMLStr3 =XMLString::transcode("FatalError");
                    if (!XMLString::compareString(typeName, tmpXMLStr ))
                    {
                        type = MsgType_Warning;
                        typeGotten[0] = true;
                    }
                     else if (!XMLString::compareString(typeName, tmpXMLStr2 ))
                    {
                        type = MsgType_Error;
                        typeGotten[1] = true;
                    }
                     else if (!XMLString::compareString(typeName, tmpXMLStr3 ))
                    {
                        type = MsgType_FatalError;
                        typeGotten[2] = true;
                    }
                     else
                    {
                        wprintf(L"Expected a Warning, Error, or FatalError node\n\n");
                        XMLString::release(&tmpXMLStr);
                        XMLString::release(&tmpXMLStr2);
                        XMLString::release(&tmpXMLStr3);
                        throw ErrReturn_SrcFmtError;
                    }
                    XMLString::release(&tmpXMLStr);
                    XMLString::release(&tmpXMLStr2);
                    XMLString::release(&tmpXMLStr3);

                    // Call the start message type event
                    formatter->startMsgType(type);

                    // Enumerate the messages under this subsection
                    enumMessages
                    (
                        typeElem
                        , formatter
                        , outHeader
                        , type
                        , count
                    );

                    // Call the end message type event
                    formatter->endMsgType(type);

                    // Move to the next child of the source element
                    typeNode = typeNode->getNextSibling();
                }

                //
                //  For any that we did not get, spit out faux boundary
                //  values for it.
                //
                for (unsigned int subIndex = 0; subIndex < 3; subIndex++)
                {
                    if (!typeGotten[subIndex])
                    {
                        fwprintf
                        (
                            outHeader
                            , L"      , %s%-30s   = %d\n"
                            , xmlStrToPrintable(typePrefixes[subIndex]) 
                            , longChars("LowBounds")
                            , count++
                        );
                        releasePrintableStr
                        fwprintf
                        (
                            outHeader
                            , L"      , %s%-30s   = %d\n"
                            , xmlStrToPrintable(typePrefixes[subIndex]) 
                            , longChars("HighBounds")
                            , count++
                        );
                        releasePrintableStr
                    }
                }

                // Tell the formatter that this domain is ending
                formatter->endDomain(domainStr, count);

                // Close out the enum declaration
                fwprintf(outHeader, L"    };\n\n");

                //
                //  Generate the code that creates the simple static methods
                //  for testing the error types. We don't do this for the
                //  exceptions header.
                //
                if (XMLString::compareString(domainStr, XMLUni::fgExceptDomain))
                {
                    fwprintf
                    (
                        outHeader
                        , L"    static bool isFatal(const %s::Codes toCheck)\n"
                          L"    {\n"
                          L"        return ((toCheck >= F_LowBounds) && (toCheck <= F_HighBounds));\n"
                          L"    }\n\n"
                        , xmlStrToPrintable(errNameSpace) 
                    );
                    releasePrintableStr

                    fwprintf
                    (
                        outHeader
                        , L"    static bool isWarning(const %s::Codes toCheck)\n"
                          L"    {\n"
                          L"        return ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds));\n"
                          L"    }\n\n"
                        , xmlStrToPrintable(errNameSpace) 
                    );
                    releasePrintableStr

                    fwprintf
                    (
                        outHeader
                        , L"    static bool isError(const %s::Codes toCheck)\n"
                          L"    {\n"
                          L"        return ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds));\n"
                          L"    }\n\n"
                        , xmlStrToPrintable(errNameSpace) 
                    );
                    releasePrintableStr

                    fwprintf
                    (
                        outHeader
                        , L"    static XMLErrorReporter::ErrTypes errorType(const %s::Codes toCheck)\n"
                          L"    {\n"
                          L"       if ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds))\n"
                          L"           return XMLErrorReporter::ErrType_Warning;\n"
                          L"       else if ((toCheck >= F_LowBounds) && (toCheck <= F_HighBounds))\n"
                          L"            return XMLErrorReporter::ErrType_Fatal;\n"
                          L"       else if ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds))\n"
                          L"            return XMLErrorReporter::ErrType_Error;\n"
                          L"       return XMLErrorReporter::ErrTypes_Unknown;\n"
                          L"    }\n"
                        , xmlStrToPrintable(errNameSpace)
                    );
                    releasePrintableStr

                    fwprintf
                    (
                        outHeader
                        , L"    static DOMError::ErrorSeverity  DOMErrorType(const %s::Codes toCheck)\n"
                          L"    {\n"
                          L"       if ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds))\n"
                          L"           return DOMError::DOM_SEVERITY_WARNING;\n"
                          L"       else if ((toCheck >= F_LowBounds) && (toCheck <= F_HighBounds))\n"
                          L"            return DOMError::DOM_SEVERITY_FATAL_ERROR;\n"
                          L"       else return DOMError::DOM_SEVERITY_ERROR;\n"
                          L"    }\n"
                        , xmlStrToPrintable(errNameSpace)
                    );
                    releasePrintableStr

                }

                // the private default ctor
                fwprintf(outHeader, L"\n");
                fwprintf(outHeader, L"private:\n");
                fwprintf(outHeader, L"    // -----------------------------------------------------------------------\n");
                fwprintf(outHeader, L"    //  Unimplemented constructors and operators\n");
                fwprintf(outHeader, L"    // -----------------------------------------------------------------------\n");
                fwprintf(outHeader, L"    %s();\n", xmlStrToPrintable(errNameSpace));
                releasePrintableStr

                // And close out the class declaration, the namespace declaration and the header file
                fwprintf(outHeader, L"};\n\n");
                fwprintf(outHeader, L"XERCES_CPP_NAMESPACE_END\n\n");
                fwprintf(outHeader, L"#endif\n\n");
                fclose(outHeader);
                XMLString::release(&headerName);
                XMLString::release(&errNameSpace);
            }

            // Ok, we are done so call the end output method
            formatter->endOutput();

            // And clean up the stuff we allocated
            delete formatter;
        }

        catch(const ErrReturns retVal)
        {
            // And call the termination method
            if(srcDoc)
                delete srcDoc;
            XMLPlatformUtils::Terminate();
            return retVal;
        }

        delete srcDoc;
    }

    // And call the termination method
    release_Globals();
    XMLPlatformUtils::Terminate();

    // Went ok, so return success
    return ErrReturn_Success;
}



// -----------------------------------------------------------------------
//  XlatErrHandler: Implementation of the error handler interface
// -----------------------------------------------------------------------
void XlatErrHandler::warning(const SAXParseException& toCatch)
{
    parseError(toCatch);
}

void XlatErrHandler::error(const SAXParseException& toCatch)
{
    parseError(toCatch);
}

void XlatErrHandler::fatalError(const SAXParseException& toCatch)
{
    parseError(toCatch);
}

void XlatErrHandler::resetErrors()
{
}

// if longChars is a macro, don't bother
#ifndef longChars
    wchar_t* longChars(const char *str) 
    {
        mbstowcs(fTmpWStr, str, 255);
        return (fTmpWStr);
    }
#endif

