/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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
 * Revision 1.1  2002/07/18 20:15:32  knoaman
 * Initial checkin: feature to control strict IANA encoding name.
 *
 */

// ---------------------------------------------------------------------------
//  This program is designed to parse an XML file containing the valid IANA
//  encodings. It will build a DOM tree from that source file and and spit out
//  a C++ code fragment that represents the table required by the TransService
//  class to check for valid IANA encodings before creating the corresponding
//  transcoder
//
//  The file format is pretty simple and this program is not intended to be
//  industrial strength by any means. Its use by anyone but the author is
//  at the user's own risk.
//
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <stdio.h>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include "IANACharset_ErrHandler.hpp"


// ---------------------------------------------------------------------------
//  Const data
// ---------------------------------------------------------------------------
enum ErrReturns
{
    ErrReturn_Success               = 0
    , ErrReturn_BadParameters       = 1
    , ErrReturn_OutFileOpenFailed   = 4
    , ErrReturn_ParserInit          = 5
    , ErrReturn_ParseErr            = 6
    , ErrReturn_SrcFmtError         = 7
};


// ---------------------------------------------------------------------------
//  Local data
//
//  gOutPath
//      This is the path to the output path, which is given on the command
//      line as /OutPath=. Its just the path, not a name.
//
//  gSrcFile
//      This the IANA encodings input file.
//
// ---------------------------------------------------------------------------
const XMLCh*    gOutPath = 0;
const XMLCh*    gSrcFile = 0;
static FILE*    gOutFile;
static bool     gFirst = false;


// ---------------------------------------------------------------------------
//  Local functions
// ---------------------------------------------------------------------------
//
//  This method is called to parse the parameters. They must be in this
//  order and format, for simplicity:
//
//  /SrcFile=xxx /OutPath=xxx
//
static bool parseParms(const int argC, XMLCh** argV)
{
    if (argC < 3)
        return false;

    unsigned int curParm = 1;

    if (XMLString::startsWith(argV[curParm], L"/SrcFile="))
    {
        gSrcFile = &argV[curParm][9];
    }
    else
    {
        return false;
    }

    curParm++;
    if (XMLString::startsWith(argV[curParm], L"/OutPath="))
    {
        gOutPath = &argV[curParm][9];
    }
    else
    {
        return false;
    }

    return true;
}

static void parseError(const XMLException& toCatch)
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


static void parseError(const SAXParseException& toCatch)
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

static void startOutput(const XMLCh* const outPath)
{
    //
    //  Ok, lets try to open the the output file.
    //
    const unsigned int bufSize = 4095;
    XMLCh tmpBuf[bufSize + 1];

    swprintf(tmpBuf, L"%s/%s.hpp", outPath, L"IANAEncodings");
    gOutFile = _wfopen(tmpBuf, L"wt");
    if (!gOutFile)
    {
        wprintf(L"Could not open the output file: %s\n\n", tmpBuf);
        throw ErrReturn_OutFileOpenFailed;
    }

    //
    //  Ok, lets output the grunt data at the start of the file. We put out a
    //  comment that indicates its a generated file, and the title string.
    //
    fwprintf
    (
        gOutFile
        , L"// ----------------------------------------------------------------\n"
          L"//  This file was generated from the IANA charset source.\n"
          L"//  so do not edit this file directly!!\n"
          L"// ----------------------------------------------------------------\n\n"
    );

    //
    //  Output the leading part of the array declaration. Its just an
    //  array of pointers to Unicode chars.
    //
    fwprintf(gOutFile, L"const XMLCh gEncodingArray[][128] = \n{\n");

    //
    // Reset first element trigger
    gFirst = true;
}

static void nextEncoding(const XMLCh* const encodingName)
{
    // Store the straight Unicode format as numeric character
    // values.
    if (gFirst)
    {
        fwprintf(gOutFile, L"    { ");
        gFirst = false;
    }
     else
    {
        fwprintf(gOutFile, L"  , { ");
    }

    const XMLCh* rawData = encodingName;
    while (*rawData)
        fwprintf(gOutFile, L"0x%04lX,", *rawData++);

    fwprintf(gOutFile, L"0x00 }\n");
}

static void endOutput(const unsigned int encCount)
{
    // And close out the array declaration
    fwprintf(gOutFile, L"\n};\n");

    // Output the const size value
    fwprintf(gOutFile, L"const unsigned int gEncodingArraySize = %d;\n\n", encCount);

    // Close the output file
    fclose(gOutFile);
}

static void usage()
{
    wprintf(L"Usage:\n  IANACharset /SrcFile=xx /OutPath=xx\n\n");
}


// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
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
        usage();
        XMLPlatformUtils::Terminate();
        return ErrReturn_BadParameters;
    }

    DOMDocument* srcDoc = 0;
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
                , L"%s"
                , gSrcFile
            );

            //
            //  Ok, lets invoke the DOM parser on the input file and build
            //  a DOM tree. Turn on validation when we do this.
            //
            XercesDOMParser parser;
            parser.setDoValidation(true);
            IANACharsetErrHandler errHandler;
            parser.setErrorHandler(&errHandler);
            parser.parse(tmpFileBuf);
            srcDoc = parser.adoptDocument();
        }
        catch(const XMLException& toCatch)
        {
            parseError(toCatch);
        }

        //
        //  Get the root element.
        //
        DOMElement* rootElem = srcDoc->getDocumentElement();

        //
        //  Ok, its good enough to get started. So lets call the start output
        //  method.
        //
        startOutput(gOutPath);

        //
        //  Loop through the children of this node, which should take us
        //  through the optional Warning, Error, and Validity subsections.
        //
        DOMNode* encNode = rootElem->getFirstChild();
        unsigned int count = 0;

        while (encNode)
        {
            // Skip over text nodes or comment nodes ect...
            if (encNode->getNodeType() != DOMNode::ELEMENT_NODE)
            {
                encNode = encNode->getNextSibling();
                continue;
            }

            // Convert it to an element node
            const DOMElement* encElem = (const DOMElement*)encNode;

            // Now get its tag name
            const XMLCh* tagName = encElem->getTagName();

            if (XMLString::compareString(tagName, L"Encoding"))
            {
                wprintf(L"Expected an Encoding node\n\n");
                throw ErrReturn_SrcFmtError;
            }

            //
            //  Ok, lets pull out the encoding name and output it to the file
            //
            const XMLCh* encName = encElem->getAttribute(L"name");
            nextEncoding(encName);
            count++;

            // Move to the next child of the source element
            encNode = encNode->getNextSibling();
        }

        endOutput(count);
    }
    catch(const ErrReturns retVal)
    {
        // And call the termination method
        delete srcDoc;
        XMLPlatformUtils::Terminate();
        return retVal;
    }

    delete srcDoc;

    // And call the termination method
    XMLPlatformUtils::Terminate();

    // Went ok, so return success
    return ErrReturn_Success;
}

// -----------------------------------------------------------------------
//  IANACharsetErrHandler: Implementation of the error handler interface
// -----------------------------------------------------------------------
void IANACharsetErrHandler::warning(const SAXParseException& toCatch)
{
    parseError(toCatch);
}

void IANACharsetErrHandler::error(const SAXParseException& toCatch)
{
    parseError(toCatch);
}

void IANACharsetErrHandler::fatalError(const SAXParseException& toCatch)
{
    parseError(toCatch);
}

void IANACharsetErrHandler::resetErrors()
{
}

