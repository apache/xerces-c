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

//---------------------------------------------------------------------
//
//  This test program is used, in conjunction with a set of test data files,
//  to verify support for different character encodings in XML.
//
//---------------------------------------------------------------------


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLException.hpp>

#include <xercesc/sax/SAXException.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>


#include <xercesc/parsers/DOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <stdio.h>

static int gTestsFailed = 0;
static int gTestsRun    = 0;


//-----------------------------------------------------------------------
//
//  ErrorHandler.   The DOM Parser will report any parsing errors by means
//                  of call-backs to the methods of this class.
//                  This is just necessary boilerplate, as far as this
//                  program is concerned.
//
//-----------------------------------------------------------------------

class  ParseErrorHandler: public ErrorHandler
{
public:
    void warning(const SAXParseException& e);
    void error(const SAXParseException& e);
    void fatalError(const SAXParseException& e);
    void resetErrors() {};

};

void ParseErrorHandler::error(const SAXParseException& e)
{
    fprintf(stderr, "\nError at file \"%s\", line %d, char %d:  %s\n",
        XMLString::transcode(e.getSystemId()), e.getLineNumber(),
        e.getColumnNumber(), XMLString::transcode(e.getMessage()));
    throw e;

};

void ParseErrorHandler::fatalError(const SAXParseException& e)
{
    fprintf(stderr, "\nFatal Error at file \"%s\", line %d, char %d:  %s\n",
        XMLString::transcode(e.getSystemId()), e.getLineNumber(),
        e.getColumnNumber(), XMLString::transcode(e.getMessage()));
    throw e;
};

void ParseErrorHandler::warning(const SAXParseException& e)
{
    fprintf(stderr, "\nWarning at file \"%s\", line %d, char %d:  %s\n",
        XMLString::transcode(e.getSystemId()), e.getLineNumber(),
        e.getColumnNumber(), XMLString::transcode(e.getMessage()));
    throw e;

};


//------------------------------------------------------------------------
//
//   parseFile  - a simpler to use function for just parsing an XML file
//                and getting the DOM Document back.
//
//------------------------------------------------------------------------
static DOM_Document parseFile(char *fileName)
{
    ParseErrorHandler eh;
    DOMParser parser;
    parser.setDoValidation(false);
    parser.setErrorHandler(&eh);
    try
    {
        parser.parse(fileName);
    }

    catch (const XMLException& e )
    {
		fprintf(stderr, "Exception Occurred \"%s\".  \n",
			XMLString::transcode(e.getMessage()));
		fprintf(stderr, "File being parsed is \"%s\".\n", fileName);
        return DOM_Document();  // A null document.
    }

	catch (...)
	{
		fprintf(stderr, "Unexpected Exception thrown during parse of file \"%s\".\n",
		                 fileName);
		return DOM_Document();
	}
    return parser.getDocument();
}


//------------------------------------------------------------------------
//
//  writeUData - Write out a udata xml element for a DOMString contents.
//
//------------------------------------------------------------------------
static void writeUData(const DOMString s)
{
    unsigned int i;
    printf("<udata>\n");
    for (i=0; i<s.length(); i++)
    {
        if (i % 16 == 0)
            printf("\n");
        XMLCh c = s.charAt(i);
        printf("%4x ", c);
    }
    printf("\n</udata>\n");
};



//------------------------------------------------------------------------
//
//  eatWhiteSpace -  DOMStrings are kind of short on utility functions :-(
//
//------------------------------------------------------------------------
static void eatWhiteSpace(DOMString s, unsigned int &i)
{
    while (i < s.length())
    {
    XMLCh c = s.charAt(i);
    if (!(c == 0x20 ||           // These are the official XML space characters,
        c == 0x09 ||             //   expressed as Unicode constants.
        c == 0x0A))
        break;
    i++;
    }
}

//------------------------------------------------------------------------
//
//   convertHexValue     if the DOMString contains a hex number at position i,
//                       convert it and return it, and update i to index the
//                       first char not in the string.
//                       return 0 if string[i] didn't have a hex digit.
//                       0 return is ambiguous, but it doesn't matter for XML,
//                       where 0 is not a valid character.
//
//------------------------------------------------------------------------
static int convertHexValue(DOMString s, unsigned int &i)
{
    int value = 0;

                                   // For reference, the digits  0-9 are Unicode 0x30-39
                                   //                the letters A-F are Unicode 0x41-0x46
                                   //                the letters a-f are Unicode 0x61-66
                                   // We can't use character literals - we might be
                                   //  building on an EBCDIC machine.
    while (i < s.length())
    {
        XMLCh c = s.charAt(i);
        if (c >= 0x61 && c <= 0x66)     // Uppercase a-f to A-F.
            c -= 0x20;

        if (c < 0x30 || c >0x46)        // Stop if not a hex digit
            break;
        if (c > 0x39 && c <0x41)
            break;

        value = value << 4;             // Append this digit to accumulating value
        if (c <= 0x39)
            value += c-0x30;
        else
            value += 0xA + c - 0x41;

        i++;
    }
    return value;
}



//------------------------------------------------------------------------
//
//  processTestFile   Given the file name of an encoding test xml file,
//                    run it.
//
//------------------------------------------------------------------------
static bool  processTestFile(DOMString fileName)
{
    //
    //  Send the input file through the parse, create a DOM document for it.
    //
    char *cFileName = fileName.transcode();
    DOM_Document testDoc = parseFile(fileName.transcode());
    if (testDoc == 0)
        return false;    // parse errors in the source xml.

    //
    //  Pull the "data" element out of the document.
    //
    DOM_NodeList nl = testDoc.getElementsByTagName("data");
    if (nl.getLength() != 1) {
        fprintf(stderr, "Test file \"%s\" must have exactly one \"data\" element.\n", cFileName);
        return false;
    };
    DOM_Node tmpNode = nl.item(0);
    DOM_Element data = (DOM_Element &) tmpNode;


    //
    //  Build up a string containing the character data contents of the data element.
    //
    DOM_Node child;
    DOMString elData;
    for (child=data.getFirstChild(); child != 0; child= child.getNextSibling())
    {
		if (child.getNodeType() == DOM_Node::COMMENT_NODE)
			continue;
        if (! (child.getNodeType() == DOM_Node::TEXT_NODE ||
               child.getNodeType() == DOM_Node::CDATA_SECTION_NODE ||
               child.getNodeType() == DOM_Node::ENTITY_REFERENCE_NODE))
        {
               fprintf(stderr, "Test file \"%s\": data element contains unexpected children.",
                    cFileName);
               return false;
        }
        elData += ((DOM_CharacterData &)child).getData();
    };

    //
    //  Pull the "udata" element out of the document
    //
    nl = testDoc.getElementsByTagName("udata");
    if (nl.getLength() != 1) {
        fprintf(stderr, "Test file \"%s\" must have exactly one \"udata\" element.\n", cFileName);
        return false;
    };
    DOM_Node tmpNode1 = nl.item(0);
    DOM_Element udata = (DOM_Element &) tmpNode1;

    //
    //  Build up a string containing the character data contents of the udata element.
    //  This will consist of a whole bunch hex numbers, still in string from
    //

    DOMString rawUData;
    for (child=udata.getFirstChild(); child != 0; child= child.getNextSibling())
    {
        if (child.getNodeType() == DOM_Node::COMMENT_NODE)
            continue;
        if (! (child.getNodeType() == DOM_Node::TEXT_NODE ||
            child.getNodeType() == DOM_Node::CDATA_SECTION_NODE ||
            child.getNodeType() == DOM_Node::ENTITY_REFERENCE_NODE))
        {
            fprintf(stderr, "Test file \"%s\": udata element contains unexpected children.",
                cFileName);
            return false;
        }
        rawUData += ((DOM_CharacterData &)child).getData();
    };


    //
    // Convert the raw (hex numbers)  form of the udata to the corresponding string.
    //
    DOMString uData;
    unsigned int rawIndex = 0;

    while (rawIndex < rawUData.length())
    {
        eatWhiteSpace(rawUData, rawIndex);
        XMLCh c = convertHexValue(rawUData, rawIndex);
        if (c > 0)
            uData += c;
        else
            if (rawIndex < rawUData.length())
            {
                fprintf(stderr, "Test file \"%s\": Bad hex number in udata element.  "
                    "Data character number %d\n", cFileName, uData.length());
                return false;
            }
    }


    //
    // Compare the two strings.
    //
    unsigned int i;
    for (i=0; i<elData.length(); i++)
    {
        if (i >= uData.length())
        {
            fprintf(stderr, "Test file \"%s\": udata element shorter than data at char number %d\n",
                cFileName, i);
            writeUData(elData);
            return false;
        }
        if (uData.charAt(i) != elData.charAt(i))
        {
            fprintf(stderr, "Test file \"%s\": comparison failure at character number %d\n",
                cFileName, i);
            writeUData(elData);
            return false;
        };
    }

    if (elData.length() != uData.length())
    {
        fprintf(stderr, "Test file \"%s\": udata element longer than data at char number %d\n",
            cFileName, i);
        writeUData(elData);
        return false;
    }


    return true;
}


int main(int argc, char ** argv) {

   //
    // Initialize the Xerces-c environment
    //
	try
    {
        XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& toCatch)
    {
        fprintf(stderr, "Error during initialization of xerces-c: %s\n",
            XMLString::transcode(toCatch.getMessage()));
         return 1;
    }

    //
    // Parse the command line, which should specify exactly one file, which is an
    //   xml file containing the list of test files to be processed.
    //
    if (argc != 2) {
        printf("usage: %s file_name \n"
               "   where file name is the xml file specifying the list of test files.", argv[0]);
        return 1;
    }
    DOM_Document fileListDoc = parseFile(argv[1]);
    if (fileListDoc == 0) return 1;


    //
    // Iterate over the list of files, running each as a test.
    //
    DOM_NodeList list = fileListDoc.getElementsByTagName("testFile");
    int i;
    int numFiles = list.getLength();
    for (i=0; i<numFiles; i++)
    {
        ++gTestsRun;
        DOM_Node tmpNode3 = list.item(i);
        DOMString fileName = ((DOM_Element &) tmpNode3).getAttribute("name");
        if (processTestFile(fileName) == false)
            ++gTestsFailed;
    };



    //
    // We are done.  Print out a summary of the results
    //
    printf("Encoding Tests Results Summary: \n"
           "   %d encoding tests run.\n"
           "   %d tests passed,\n"
           "   %d tests failed\n", gTestsRun, gTestsRun-gTestsFailed, gTestsFailed);

   return 0;
};
