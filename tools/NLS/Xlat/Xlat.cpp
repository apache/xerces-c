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


/**
 * $Log$
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
static XMLCh* const     gRelativeInputPath  = L"src/NLS/";


// ---------------------------------------------------------------------------
//  Global data
// ---------------------------------------------------------------------------
const XMLCh* typePrefixes[MsgTypes_Count] = 
{
    L"W_"
    , L"E_"
    , L"V_"
};


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

//
//  This method is called to parse the parameters. They must be in this
//  order and format, for simplicity:
//
//  /SrcRoot=xxx /OutPath=xxx /OutFmt=xxx /Locale=xxx
//
static bool parseParms(const int argC, XMLCh** argV)
{
    if (argC < 5)
        return false;

    unsigned int curParm = 1;

    if (XMLString::startsWith(argV[curParm], L"/SrcRoot="))
    {
        gSrcRoot = &argV[curParm][9];
    }
     else
    {
        wprintf(L"\nExpected /SrcRoot=xxx. Got: %s\n", argV[curParm]);
        return false;
    }

    curParm++;
    if (XMLString::startsWith(argV[curParm], L"/OutPath="))
    {
        gOutPath = &argV[curParm][9];
    }
     else
    {
        wprintf(L"\nExpected /OutPath=xxx. Got: %s\n", argV[curParm]);
        return false;
    }


    curParm++;
    if (XMLString::startsWith(argV[curParm], L"/OutFmt="))
    {
        const XMLCh* tmpFmt = &argV[curParm][8];
        if (!XMLString::compareIString(tmpFmt, L"ResBundle"))
            gOutFormat = OutFormat_ResBundle;
        else if (!XMLString::compareIString(tmpFmt, L"Win32RC"))
            gOutFormat = OutFormat_Win32RC;
        else if (!XMLString::compareIString(tmpFmt, L"CppSrc"))
            gOutFormat = OutFormat_CppSrc;
        else if (!XMLString::compareIString(tmpFmt, L"MsgCat"))
            gOutFormat = OutFormat_MsgCatalog;
        else
        {
            wprintf(L"\n'%s' is not a legal output format\n", tmpFmt);
            return false;
        }
    }
     else
    {
        wprintf(L"\nExpected /OutFmt=xxx. Got: %s\n", argV[curParm]);
        return false;
    }

    curParm++;
    if (XMLString::startsWith(argV[curParm], L"/Locale="))
    {
        gLocale = &argV[curParm][8];
    }
     else
    {
        wprintf(L"\nExpected /Locale=xxx. Got: %s\n", argV[curParm]);
        return false;
    }

    return true;
}


static void parseError(const XMLException& toCatch)
{
    wprintf(L"Parse Error:\n   ERROR: %s\n\n", toCatch.getMessage());
    throw ErrReturn_ParseErr;
}


static void parseError(const SAXParseException& toCatch)
{
    wprintf(L"Parse Error:\n   SAX ERROR: %s\n\n", toCatch.getMessage());
    throw ErrReturn_ParseErr;
}


static void
enumMessages(   const   DOM_Element             srcElem
                ,       XlatFormatter* const    toCall
                ,       FILE* const             headerFl
                , const MsgTypes                msgType
                ,       unsigned int&           count)
{
    fwprintf
    (
        headerFl
        , L"      , %s%-30s   = %d\n"
        , typePrefixes[msgType]
        , L"LowBounds"
        , count++
    );

    //
    //  We just run through each of the child elements, each of which is
    //  a Message element. Each one represents a message to output. We keep
    //  a count so that we can output a const value afterwards.
    //
    DOM_Node curNode = srcElem.getFirstChild();
    while (!curNode.isNull())
    {
        // Skip over text nodes or comment nodes ect...
        if (curNode.getNodeType() != DOM_Node::ELEMENT_NODE)
        {
            curNode = curNode.getNextSibling();
            continue;
        }

        // Convert it to an element node
        const DOM_Element& curElem = (const DOM_Element&)curNode;

        // Ok, this should be a Message node
        if (!curElem.getTagName().equals(L"Message"))
        {
            wprintf(L"Expected a Message node\n\n");
            throw ErrReturn_SrcFmtError;
        }

        //
        //  Ok, lets pull out the id, text value, and message type. These are
        //  to be passed to the formatter. We have to translate the message
        //  type into one of the offical enum values.
        //
        DOMString msgText = curElem.getAttribute(L"Text");
        DOMString msgId   = curElem.getAttribute(L"Id");

        //
        //  Write out an entry to the target header file. These are enums, so
        //  we use the id as the enum name.
        //
        fwprintf(headerFl, L"      , %-32s   = %d\n", msgId.rawBuffer(), count);

        // And tell the formatter about this one
        toCall->nextMessage
        (
            msgText.rawBuffer()
            , msgId.rawBuffer()
            , count
            , count
        );

        // Bump the counter, which is also the id assigner
        count++;

        // Move to the next child of the source element
        curNode = curNode.getNextSibling();
    }

    // Write out an upper range bracketing id for this type of error
    fwprintf
    (
        headerFl
        , L"      , %s%-30s   = %d\n"
        , typePrefixes[msgType]
        , L"HighBounds"
        , count++
    );
}



// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------

//
//  This is the program entry point. It checks the parms, parses the input
//  file to get a DOM tree, then passes the DOM tree to the appropriate
//  output method to output the info in a particular format.
//
extern "C" int wmain(int argC, XMLCh** argV)
{
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch(const XMLException& toCatch)
    {
        wprintf(L"Parser init error.\n  ERROR: %s\n\n", toCatch.getMessage());
        return ErrReturn_ParserInit;
    }

    //
    //  Lets check the parameters and save them away in globals for use by
    //  the processing code.
    //
    if (!parseParms(argC, argV))
    {
        wprintf(L"Usage:\n  NLSXlat /SrcRoot=xx /OutPath=xx /OutFmt=xx /Locale=xx\n\n");
        return ErrReturn_BadParameters;
    }

    DOM_Document srcDoc;
    const unsigned int bufSize = 4095;
    XMLCh tmpFileBuf[bufSize + 1];
    try
    {
        try
        {
            // Build the input file name
            swprintf
            (
                tmpFileBuf
                , L"%s%s%s/XMLErrList_%s.Xml"
                , gSrcRoot
                , gRelativeInputPath
                , gLocale
                , gLocale
            );

            //
            //  Ok, lets invoke the DOM parser on the input file and build
            //  a DOM tree. Turn on validation when we do this.
            //
            DOMParser parser;
            parser.setDoValidation(true);
            XlatErrHandler errHandler;
            parser.setErrorHandler(&errHandler);
            parser.parse(tmpFileBuf);
            srcDoc = parser.getDocument();
        }

        catch(const XMLException& toCatch)
        {
            parseError(toCatch);
        }

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

            default :
                wprintf(L"Uknown formatter type enum\n\n");
                throw ErrReturn_Internal;
        }

        //
        //  Lets handle the root element stuff first. This one holds any over
        //  all information.
        //
        DOM_Element rootElem = srcDoc.getDocumentElement();
        DOMString localeStr = rootElem.getAttribute(L"Locale");

        // Make sure that the locale matches what we were given
        if (XMLString::compareString(localeStr.rawBuffer(), gLocale))
        {
            wprintf(L"The file's locale does not match the target locale\n");
            throw ErrReturn_LocaleErr;
        }

        //
        //  Get a list of all the MsgDomain children. These each hold one of
        //  the sets of (potentially separately) loadable messages. More
        //  importantly they all have their own error id space.
        //
        DOM_NodeList msgSetList = rootElem.getElementsByTagName(L"MsgDomain");

        //
        //  Loop through them and look for the domains that we know are
        //  supposed to be there.
        //
        const unsigned int count = msgSetList.getLength();

        //
        //  Ok, its good enough to get started. So lets call the start output
        //  method on the formatter.
        //
        formatter->startOutput(localeStr.rawBuffer(), gOutPath);

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
            DOM_Node curNode = msgSetList.item(index);
            const DOM_Element& curElem = (const DOM_Element&)curNode;

            //
            //  Get some of  the attribute strings that we need, and transcode
            //  couple that need to be in local format.
            //
            DOMString domainStr = curElem.getAttribute(L"Domain");

            //
            //  Look at the domain and set up our application specific info
            //  that is on a per-domain basis. We need to indicate what the
            //  name of the header is and what the namespace is that they
            //  codes will go into
            //
            const XMLCh* headerName = 0;
            const XMLCh* errNameSpace = 0;
            if (!XMLString::compareString(domainStr.rawBuffer(), XMLUni::fgXMLErrDomain))
            {
                headerName = L"XMLErrorCodes.hpp";
                errNameSpace = L"XML4CErrs";
            }
             else if (!XMLString::compareString(domainStr.rawBuffer(), XMLUni::fgValidityDomain))
            {
                headerName = L"XMLValidityCodes.hpp";
                errNameSpace = L"XML4CValid";
            }
             else if (!XMLString::compareString(domainStr.rawBuffer(), XMLUni::fgExceptDomain))
            {
                headerName = L"XMLExceptMsgs.hpp";
                errNameSpace = L"XML4CExcepts";
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
            swprintf
            (
                tmpFileBuf
                , L"%s%s"
                , gOutPath
                , headerName
            );
            FILE* outHeader = _wfopen(tmpFileBuf, L"wt+");
            if (!outHeader)
            {
                wprintf(L"Could not open domain header file: %s\n\n", tmpFileBuf);
                throw ErrReturn_OutFileOpenFailed;
            }

            //
            //  Write out the opening of the class they are nested within, and
            //  the header protection define.
            //
            fwprintf(outHeader, L"// This file is generated, don't edit it!!\n\n");
            fwprintf(outHeader, L"#if !defined(ERRHEADER_%s)\n", errNameSpace);
            fwprintf(outHeader, L"#define ERRHEADER_%s\n\n", errNameSpace);

            // If its not the exception domain, then we need a header included
            if (XMLString::compareString(domainStr.rawBuffer(), XMLUni::fgExceptDomain))
                fwprintf(outHeader, L"#include <framework/XMLErrorReporter.hpp>\n\n");

            fwprintf(outHeader, L"class %s\n{\npublic :\n    enum Codes\n    {\n", errNameSpace);

            // Tell the formatter that a new domain is starting
            formatter->startDomain
            (
                domainStr.rawBuffer()
                , errNameSpace
            );

            //
            //  Force out the first message, which is always implicit and is
            //  the 'no error' entry for that domain.
            //
            unsigned int count = 0;
            fwprintf(outHeader, L"        %-32s   = %d\n", L"NoError", count++);

            //
            //  Loop through the children of this node, which should take us
            //  through the optional Warning, Error, and Validity subsections.
            //
            DOM_Node typeNode = curElem.getFirstChild();
            bool typeGotten[3] = { false, false, false };
            while (!typeNode.isNull())
            {
                // Skip over text nodes or comment nodes ect...
                if (typeNode.getNodeType() != DOM_Node::ELEMENT_NODE)
                {
                    typeNode = typeNode.getNextSibling();
                    continue;
                }

                // Convert it to an element node
                const DOM_Element& typeElem = (const DOM_Element&)typeNode;

                // Now get its tag name and convert that to a message type enum
                DOMString typeName = typeElem.getTagName();

                MsgTypes type;
                if (typeName.equals(L"Warning"))
                {
                    type = MsgType_Warning;
                    typeGotten[0] = true;
                }
                 else if (typeName.equals(L"Error"))
                {
                    if (!XMLString::compareString(domainStr.rawBuffer(), XMLUni::fgValidityDomain))
                    {
                        type = MsgType_Validity;
                        typeGotten[2] = true;
                    }
                     else
                    {
                        type = MsgType_Error;
                        typeGotten[1] = true;
                    }
                }
                 else
                {
                    wprintf(L"Expected a Warning or Errornode\n\n");
                    throw ErrReturn_SrcFmtError;
                }

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
                typeNode = typeNode.getNextSibling();
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
                        , typePrefixes[subIndex]
                        , L"LowBounds"
                        , count++
                    );
                    fwprintf
                    (
                        outHeader
                        , L"      , %s%-30s   = %d\n"
                        , typePrefixes[subIndex]
                        , L"HighBounds"
                        , count++
                    );
                }
            }

            // Tell the formatter that this domain is ending
            formatter->endDomain(domainStr.rawBuffer(), count);

            // Close out the enum declaration
            fwprintf(outHeader, L"    };\n\n");

            //
            //  Generate the code that creates the simple static methods
            //  for testing the error types. We don't do this for the
            //  exceptions header.
            //
            if (XMLString::compareString(domainStr.rawBuffer(), XMLUni::fgExceptDomain))
            {
                fwprintf
                (
                    outHeader
                    , L"    static bool isFatal(const %s::Codes toCheck)\n"
                      L"    {\n"
                      L"        return ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds));\n"
                      L"    }\n\n"
                    , errNameSpace
                );

                fwprintf
                (
                    outHeader
                    , L"    static bool isWarning(const %s::Codes toCheck)\n"
                      L"    {\n"
                      L"        return ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds));\n"
                      L"    }\n\n"
                    , errNameSpace
                );

                fwprintf
                (
                    outHeader
                    , L"    static bool isValid(const %s::Codes toCheck)\n"
                      L"    {\n"
                      L"        return ((toCheck >= V_LowBounds) && (toCheck <= V_HighBounds));\n"
                      L"    }\n\n"
                    , errNameSpace
                );

                fwprintf
                (
                    outHeader
                    , L"    static XMLErrorReporter::ErrTypes errorType(const %s::Codes toCheck)\n"
                      L"    {\n"
                      L"       if ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds))\n"
                      L"           return XMLErrorReporter::ErrType_Warning;\n"
                      L"       else if ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds))\n"
                      L"            return XMLErrorReporter::ErrType_Fatal;\n"
                      L"       else if ((toCheck >= V_LowBounds) && (toCheck <= V_HighBounds))\n"
                      L"            return XMLErrorReporter::ErrType_Invalid;\n"
                      L"       return XMLErrorReporter::ErrTypes_Unknown;\n"
                      L"    }\n"
                    , errNameSpace
                );
            }

            // And close out the class declaration and the header file
            fwprintf(outHeader, L"};\n#endif\n\n");
            fclose(outHeader);
        }

        // Ok, we are done so call the end output method
        formatter->endOutput();

        // And clean up the stuff we allocated
        delete formatter;
    }

    catch(const ErrReturns retVal)
    {
        return retVal;
    }

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
