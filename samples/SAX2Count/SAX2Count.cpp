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
* Revision 1.1  2000/08/08 17:17:20  jpolast
* initial checkin of SAX2Count
*
*
*/


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "SAX2Count.hpp"
#include <util/PlatformUtils.hpp>
#include <sax2/SAX2XMLReader.hpp>
#include <sax2/XMLReaderFactory.hpp>

// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
void usage()
{
    cout << "\nUsage:\n"
            "    SAXCount [options] <XML file>\n\n"
            "Options:\n"
            "    -v=xxx      Validation scheme [always | never | auto*]\n"
            "    -n          Disable namespace processing. Defaults to on.\n\n"
            "This program prints the number of elements, attributes,\n"
            "white spaces and other non-white space characters in the "
            "input file.\n\n"
            "  * = Default if not provided explicitly\n"
         << endl;
}


// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{
    // Initialize the XML4C2 system
    try
    {
        XMLPlatformUtils::Initialize();
    }
    
    catch (const XMLException& toCatch)
    {
        cerr << "Error during initialization! Message:\n"
            << StrX(toCatch.getMessage()) << endl;
        return 1;
    }
    
    // Check command line and extract arguments.
    if (argC < 2)
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 1;
    }
    
    const char*              xmlFile = 0;
    SAXParser::ValSchemes    valScheme = SAXParser::Val_Auto;
    bool                     doNamespaces = true;
    
    // See if non validating dom parser configuration is requested.
    if ((argC == 2) && !strcmp(argV[1], "-?"))
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 2;
    }
    
    int argInd;
    for (argInd = 1; argInd < argC; argInd++)
    {
        // Break out on first non-dash parameter
        if (argV[argInd][0] != '-')
            break;
        
        if (!strncmp(argV[argInd], "-v=", 3)
        ||  !strncmp(argV[argInd], "-V=", 3))
        {
            const char* const parm = &argV[argInd][3];

            if (!strcmp(parm, "never"))
                valScheme = SAXParser::Val_Never;
            else if (!strcmp(parm, "auto"))
                valScheme = SAXParser::Val_Auto;
            else if (!strcmp(parm, "always"))
                valScheme = SAXParser::Val_Always;
            else
            {
                cerr << "Unknown -v= value: " << parm << endl;
                return 2;
            }
        }
         else if (!strcmp(argV[argInd], "-n")
              ||  !strcmp(argV[argInd], "-N"))
        {
            doNamespaces = false;
        }
        else
        {
            cerr << "Unknown option '" << argV[argInd]
                << "', ignoring it\n" << endl;
        }
    }
    
    //
    //  There should be only one and only one parameter left, and that
    //  should be the file name.
    //
    if (argInd != argC - 1)
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 1;
    }
    xmlFile = argV[argInd];
    
    //
    //  Create a SAX parser object. Then, according to what we were told on
    //  the command line, set it to validate or not.
    //
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
	parser->setFeature(XMLString::transcode("http://xml.org/sax/features/namespaces"), doNamespaces);
	if (valScheme == SAXParser::Val_Auto)
	{
	  parser->setFeature(XMLString::transcode("http://xml.org/sax/features/validation"), true);
	  parser->setFeature(XMLString::transcode("http://apache.org/xml/features/validation/dynamic"), true);
	}
    if (valScheme == SAXParser::Val_Never)
	{
	  parser->setFeature(XMLString::transcode("http://xml.org/sax/features/validation"), false);
	}
	if (valScheme == SAXParser::Val_Always)
	{
	  parser->setFeature(XMLString::transcode("http://xml.org/sax/features/validation"), true);
	  parser->setFeature(XMLString::transcode("http://apache.org/xml/features/validation/dynamic"), false);
	}
    
    //
    //  Create our SAX handler object and install it on the parser, as the
    //  document and error handler.
    //
    SAX2CountHandlers handler;
    parser->setContentHandler(&handler);
    parser->setErrorHandler(&handler);
  
    //
    //  Get the starting time and kick off the parse of the indicated
    //  file. Catch any exceptions that might propogate out of it.
    //
    unsigned long duration;
    try
    {
        const unsigned long startMillis = XMLPlatformUtils::getCurrentMillis();
        parser->parse(xmlFile);
        const unsigned long endMillis = XMLPlatformUtils::getCurrentMillis();
        duration = endMillis - startMillis;
    }
    
    catch (const XMLException& e)
    {
        cerr << "\nError during parsing: '" << xmlFile << "'\n"
            << "Exception message is:  \n"
            << StrX(e.getMessage()) << "\n" << endl;
        XMLPlatformUtils::Terminate();
        return 4;
    }

    catch (...)
    {
        cerr << "\nUnexpected exception during parsing: '" << xmlFile << "'\n";
        XMLPlatformUtils::Terminate();
        return 4;
    }

    
    // Print out the stats that we collected and time taken
    if (!handler.getSawErrors())
    {
        cout << xmlFile << ": " << duration << " ms ("
            << handler.getElementCount() << " elems, "
            << handler.getAttrCount() << " attrs, "
            << handler.getSpaceCount() << " spaces, "
            << handler.getCharacterCount() << " chars)" << endl;
    }
    
    // And call the termination method
    XMLPlatformUtils::Terminate();
    
    return 0;
}

