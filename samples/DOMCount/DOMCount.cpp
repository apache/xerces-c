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
 * Revision 1.4  2000/02/11 02:32:46  abagchi
 * Removed StrX::transcode
 *
 * Revision 1.3  2000/02/06 07:47:17  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  1999/11/12 02:13:40  rahulj
 * It now validates when the -v option is specified.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:52  twl
 * Initial checkin
 *
 * Revision 1.8  1999/11/08 20:43:34  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/PlatformUtils.hpp>
#include <sax/SAXException.hpp>
#include <sax/SAXParseException.hpp>
#include <parsers/DOMParser.hpp>
#include "DOMCount.hpp"
#include <string.h>
#include <stdlib.h>


// ---------------------------------------------------------------------------
//  This is a simple program which invokes the DOMParser to build a DOM
//  tree for the specified input file. It then walks the tree and counts
//  the number of elements. The element count is then printed.
// ---------------------------------------------------------------------------
void usage()
{
    cout << "\nUsage:\n"
         << "    DOMCount [-v] {XML file}\n"
         << "This program invokes the XML4C DOM parser, builds "
         << "the DOM tree, and then prints the number of elements "
         << "found in the input XML file.\n\n"
         << "Options:\n"
         << "        -v  Do validation in this parse.\n\n"
         << endl;
}


int main(int argc, char* args[])
{
    // Initialize the XML4C system
	try
    {
        XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& toCatch)
    {
         cerr << "Error during initialization! :\n"
              << StrX(toCatch.getMessage()) << endl;
         return 1;
    }

    // Check command line and extract arguments.
    if (argc < 2)
    {
        usage();
        return -1;
    }

    const char* xmlFile         = args[1];
    bool        doValidation    = false;

    // See if non validating dom parser configuration is requested.
    if (!strncmp(xmlFile, "-?", 2))
    {
        usage();
        return 0;
    }
     else if (!strncmp(xmlFile, "-v", 2))
    {
        doValidation = true;
        if (argc < 3)
        {
            usage();
            return -1;
        }
        xmlFile = args[2];
    }
     else if (xmlFile[0] == '-')
    {
        usage();
        return -1;
    }

    // Instantiate the DOM parser.
    DOMParser parser;
    parser.setDoValidation(doValidation);

    // And create our error handler and install it
    DOMCount elementCounter;
    parser.setErrorHandler(&elementCounter);

    //
    //  Get the starting time and kick off the parse of the indicated
    //  file. Catch any exceptions that might propogate out of it.
    //
    unsigned long duration;
    try
    {
        const unsigned long startMillis = XMLPlatformUtils::getCurrentMillis();
        parser.parse(xmlFile);
        const unsigned long endMillis = XMLPlatformUtils::getCurrentMillis();
        duration = endMillis - startMillis;
    }

    catch (const XMLException& toCatch)
    {
        cerr << "\nError during parsing: '" << xmlFile << "'\n"
                << "Exception message is:  \n"
                << StrX(toCatch.getMessage()) << "\n" << endl;
        return -1;
    }

    //
    //  Extract the DOM tree, get the list of all the elements and report the
    //  length as the count of elements.
    //
    DOM_Document doc = parser.getDocument();
    unsigned int elementCount = doc.getElementsByTagName("*").getLength();

    // Print out the stats that we collected and time taken.
    cout << xmlFile << ": " << duration << " ms ("
            << elementCount << " elems)." << endl;

    return 0;
}


DOMCount::DOMCount()
{
}

DOMCount::~DOMCount()
{
}


// ---------------------------------------------------------------------------
//  DOMCountHandlers: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void DOMCount::error(const SAXParseException& e)
{
    cerr << "\nError at (file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "): " << StrX(e.getMessage()) << endl;
}

void DOMCount::fatalError(const SAXParseException& e)
{
    cerr << "\nFatal Error at (file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "): " << StrX(e.getMessage()) << endl;
}

void DOMCount::warning(const SAXParseException& e)
{
    cerr << "\nWarning at (file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "): " << StrX(e.getMessage()) << endl;
}

void DOMCount::resetErrors()
{
}

