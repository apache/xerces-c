/*
* The Apache Software License, Version 1.1
*
* Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
* Revision 1.14  2001/08/03 15:08:17  tng
* close the list file.
*
* Revision 1.13  2001/08/02 17:10:29  tng
* Allow DOMCount/SAXCount/IDOMCount/SAX2Count to take a file that has a list of xml file as input.
*
* Revision 1.12  2001/08/01 19:11:01  tng
* Add full schema constraint checking flag to the samples and the parser.
*
* Revision 1.11  2001/05/11 13:24:57  tng
* Copyright update.
*
* Revision 1.10  2001/05/03 16:00:12  tng
* Schema: samples update with schema
*
* Revision 1.9  2000/10/19 23:52:41  andyh
* SAXCount: Allow multiple files on command line
*
* Revision 1.8  2000/06/16 20:25:38  rahulj
* Add the -v=always option to force validation checking. Need this
* option for running the conformance tests.
*
* Revision 1.7  2000/05/31 18:39:59  rahulj
* 'Auto' validation is the default processing mode.
*
* Revision 1.6  2000/05/09 00:22:29  andyh
* Memory Cleanup.  XMLPlatformUtils::Terminate() deletes all lazily
* allocated memory; memory leak checking tools will no longer report
* that leaks exist.  (DOM GetElementsByTagID temporarily removed
* as part of this.)
*
* Revision 1.5  2000/03/03 01:29:31  roddey
* Added a scanReset()/parseReset() method to the scanner and
* parsers, to allow for reset after early exit from a progressive parse.
* Added calls to new Terminate() call to all of the samples. Improved
* documentation in SAX and DOM parsers.
*
* Revision 1.4  2000/03/02 19:53:47  roddey
* This checkin includes many changes done while waiting for the
* 1.1.0 code to be finished. I can't list them all here, but a list is
* available elsewhere.
*
* Revision 1.3  2000/02/11 02:39:10  abagchi
* Removed StrX::transcode
*
* Revision 1.2  2000/02/06 07:47:23  rahulj
* Year 2K copyright swat.
*
* Revision 1.1.1.1  1999/11/09 01:09:30  twl
* Initial checkin
*
* Revision 1.7  1999/11/08 20:43:40  rahul
* Swat for adding in Product name and CVS comment log variable.
*
*/


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "SAXCount.hpp"
#include <fstream.h>

// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
void usage()
{
    cout << "\nUsage:\n"
            "    SAXCount [options] <XML file> | List file>\n\n"
            "Options:\n"
            "    -l          Indicate the input file is a file that has a list of xml files.  Default: Input file is an XML file\n"
            "    -v=xxx      Validation scheme [always | never | auto*]\n"
            "    -n          Enable namespace processing. Defaults to off.\n"
            "    -s          Enable schema processing. Defaults to off.\n"
            "    -f          Enable full schema constraint checking. Defaults to off.\n\n"
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
    bool                     doNamespaces       = false;
    bool                     doSchema           = false;
    bool                     schemaFullChecking = false;
    bool                     doList = false;
    bool                     errorOccurred = false;


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
            doNamespaces = true;
        }
         else if (!strcmp(argV[argInd], "-s")
              ||  !strcmp(argV[argInd], "-S"))
        {
            doSchema = true;
        }
         else if (!strcmp(argV[argInd], "-f")
              ||  !strcmp(argV[argInd], "-F"))
        {
            schemaFullChecking = true;
        }
         else if (!strcmp(argV[argInd], "-l")
              ||  !strcmp(argV[argInd], "-L"))
        {
            doList = true;
        }
        else
        {
            cerr << "Unknown option '" << argV[argInd]
                << "', ignoring it\n" << endl;
        }
    }

    //
    //  There should at least one parameter left, and that
    //  should be the file name(s).
    //
    if (argInd == argC)
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 1;
    }

    //
    //  Create a SAX parser object. Then, according to what we were told on
    //  the command line, set it to validate or not.
    //
    SAXParser parser;

    parser.setValidationScheme(valScheme);
    parser.setDoNamespaces(doNamespaces);
    parser.setDoSchema(doSchema);
    parser.setValidationSchemaFullChecking(schemaFullChecking);

    //
    //  Create our SAX handler object and install it on the parser, as the
    //  document and error handler.
    //
    SAXCountHandlers handler;
    parser.setDocumentHandler(&handler);
    parser.setErrorHandler(&handler);


    //
    //  Get the starting time and kick off the parse of the indicated
    //  file. Catch any exceptions that might propogate out of it.
    //
    unsigned long duration;

    ifstream fin;

    // the input is a list file
    if (doList)
        fin.open(argV[argInd]);

    while (true)
    {
        char fURI[1000];
        if (doList) {
            if (! fin.eof() ) {
                fin.getline (fURI, sizeof(fURI));
                if (!*fURI)
                    continue;
                else
                    xmlFile = fURI;
            }
            else
                break;
        }
        else {
            if (argInd < argC)
            {
                 xmlFile = argV[argInd];
                 argInd++;
            }
            else
                break;
        }

        cerr << "==Parsing== " << xmlFile << endl;

        //reset error count first
        handler.resetErrors();

        try
        {
            const unsigned long startMillis = XMLPlatformUtils::getCurrentMillis();
            parser.parse(xmlFile);
            const unsigned long endMillis = XMLPlatformUtils::getCurrentMillis();
            duration = endMillis - startMillis;
        }

        catch (const XMLException& e)
        {
            cerr << "\nError during parsing: '" << xmlFile << "'\n"
                << "Exception message is:  \n"
                << StrX(e.getMessage()) << "\n" << endl;
            errorOccurred = true;
            continue;
        }

        catch (...)
        {
            cerr << "\nUnexpected exception during parsing: '" << xmlFile << "'\n";
            errorOccurred = true;
            continue;
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
    }

    if (doList)
        fin.close();

    // And call the termination method
    XMLPlatformUtils::Terminate();

    if (errorOccurred)
        return 4;
    else
        return 0;

}

