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
* Revision 1.24  2002/12/10 13:34:51  tng
* Samples minor update in usage information.
*
* Revision 1.23  2002/11/08 16:18:50  peiyongz
* no message
*
* Revision 1.22  2002/11/07 18:30:42  peiyongz
* command line option for "locale"
*
* Revision 1.21  2002/11/04 14:09:06  tng
* [Bug 14201] use of ios::nocreate breaks build.
*
* Revision 1.20  2002/11/01 22:05:57  tng
* Samples/Test update: Issue error if the list file failed to open.
*
* Revision 1.19  2002/09/27 19:24:57  tng
* Samples Fix: wrong length in memset
*
* Revision 1.18  2002/07/17 18:58:35  tng
* samples update: for testing special encoding purpose.
*
* Revision 1.17  2002/06/17 15:33:05  tng
* Name Xerces features as XMLUni::fgXercesXXXX instead of XMLUni::fgSAX2XercesXXXX so that they can be shared with DOM parser.
*
* Revision 1.16  2002/02/13 16:11:06  knoaman
* Update samples to use SAX2 features/properties constants from XMLUni.
*
* Revision 1.15  2002/02/06 16:36:51  knoaman
* Added a new flag '-p' to SAX2 samples to set the 'namespace-prefixes' feature.
*
* Revision 1.14  2002/02/01 22:38:52  peiyongz
* sane_include
*
* Revision 1.13  2001/10/29 17:02:57  tng
* Fix typo in samples.
*
* Revision 1.12  2001/10/25 15:18:33  tng
* delete the parser before XMLPlatformUtils::Terminate.
*
* Revision 1.11  2001/10/19 19:02:43  tng
* [Bug 3909] return non-zero an exit code when error was encounted.
* And other modification for consistent help display and return code across samples.
*
* Revision 1.10  2001/08/15 12:41:04  tng
* Initialize the fURI array to zeros, in case, some compilers like AIX xlC_r doesn't reset the memory.
*
* Revision 1.9  2001/08/08 12:12:32  tng
* Print the file name only if doList is on.
*
* Revision 1.8  2001/08/03 15:08:17  tng
* close the list file.
*
* Revision 1.7  2001/08/02 17:10:29  tng
* Allow DOMCount/SAXCount/IDOMCount/SAX2Count to take a file that has a list of xml file as input.
*
* Revision 1.6  2001/08/01 19:11:01  tng
* Add full schema constraint checking flag to the samples and the parser.
*
* Revision 1.5  2001/05/11 13:24:56  tng
* Copyright update.
*
* Revision 1.4  2001/05/03 15:59:55  tng
* Schema: samples update with schema
*
* Revision 1.3  2000/08/09 22:46:06  jpolast
* replace occurences of SAXCount with SAX2Count
*
* Revision 1.2  2000/08/09 22:40:15  jpolast
* updates for changes to sax2 core functionality.
*
* Revision 1.1  2000/08/08 17:17:20  jpolast
* initial checkin of SAX2Count
*
*
*/


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "SAX2Count.hpp"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <fstream.h>

// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
void usage()
{
    cout << "\nUsage:\n"
            "    SAX2Count [options] <XML file | List file>\n\n"
            "This program invokes the SAX2XMLReader, and then prints the\n"
            "number of elements, attributes, spaces and characters found\n"
            "in each XML file, using SAX2 API.\n\n"
            "Options:\n"
            "    -l          Indicate the input file is a List File that has a list of xml files.\n"
            "                Default to off (Input file is an XML file).\n"
            "    -v=xxx      Validation scheme [always | never | auto*].\n"
            "    -f          Enable full schema constraint checking processing. Defaults to off.\n"
            "    -p          Enable namespace-prefixes feature. Defaults to off.\n"
            "    -n          Disable namespace processing. Defaults to on.\n"
            "                NOTE: THIS IS OPPOSITE FROM OTHER SAMPLES.\n"
            "    -s          Disable schema processing. Defaults to on.\n"
            "                NOTE: THIS IS OPPOSITE FROM OTHER SAMPLES.\n"
            "    -locale=ll_CC specify the locale, default: en_US.\n"
            "    -?          Show this help.\n\n"
            "  * = Default if not provided explicitly.\n"
         << endl;
}


// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{

    // Check command line and extract arguments.
    if (argC < 2)
    {
        usage();
        return 1;
    }

    const char*                  xmlFile      = 0;
    SAX2XMLReader::ValSchemes    valScheme    = SAX2XMLReader::Val_Auto;
    bool                         doNamespaces = true;
    bool                         doSchema = true;
    bool                         schemaFullChecking = false;
    bool                         doList = false;
    bool                         errorOccurred = false;
    bool                         namespacePrefixes = false;
    bool                         recognizeNEL = false;
    char                         localeStr[64];
    memset(localeStr, 0, sizeof localeStr);

    int argInd;
    for (argInd = 1; argInd < argC; argInd++)
    {
        // Break out on first parm not starting with a dash
        if (argV[argInd][0] != '-')
            break;

        // Watch for special case help request
        if (!strcmp(argV[argInd], "-?"))
        {
            usage();
            return 2;
        }
         else if (!strncmp(argV[argInd], "-v=", 3)
              ||  !strncmp(argV[argInd], "-V=", 3))
        {
            const char* const parm = &argV[argInd][3];

            if (!strcmp(parm, "never"))
                valScheme = SAX2XMLReader::Val_Never;
            else if (!strcmp(parm, "auto"))
                valScheme = SAX2XMLReader::Val_Auto;
            else if (!strcmp(parm, "always"))
                valScheme = SAX2XMLReader::Val_Always;
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
         else if (!strcmp(argV[argInd], "-s")
              ||  !strcmp(argV[argInd], "-S"))
        {
            doSchema = false;
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
         else if (!strcmp(argV[argInd], "-p")
              ||  !strcmp(argV[argInd], "-P"))
        {
            namespacePrefixes = true;
        }
         else if (!strcmp(argV[argInd], "-special:nel"))
        {
            // turning this on will lead to non-standard compliance behaviour
            // it will recognize the unicode character 0x85 as new line character
            // instead of regular character as specified in XML 1.0
            // do not turn this on unless really necessary
             recognizeNEL = true;
        }
         else if (!strncmp(argV[argInd], "-locale=", 8))
        {
             // Get out the end of line
             strcpy(localeStr, &(argV[argInd][8]));
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
        return 1;
    }

    // Initialize the XML4C2 system
    try
    {
        if (strlen(localeStr))
        {
            XMLPlatformUtils::Initialize(localeStr);
        }
        else
        {
            XMLPlatformUtils::Initialize();
        }

        if (recognizeNEL)
        {
            XMLPlatformUtils::recognizeNEL(recognizeNEL);
        }
    }

    catch (const XMLException& toCatch)
    {
        cerr << "Error during initialization! Message:\n"
            << StrX(toCatch.getMessage()) << endl;
        return 1;
    }

    //
    //  Create a SAX parser object. Then, according to what we were told on
    //  the command line, set it to validate or not.
    //
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, doNamespaces);
    parser->setFeature(XMLUni::fgXercesSchema, doSchema);
    parser->setFeature(XMLUni::fgXercesSchemaFullChecking, schemaFullChecking);
    parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, namespacePrefixes);

    if (valScheme == SAX2XMLReader::Val_Auto)
    {
        parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
        parser->setFeature(XMLUni::fgXercesDynamic, true);
    }
    if (valScheme == SAX2XMLReader::Val_Never)
    {
        parser->setFeature(XMLUni::fgSAX2CoreValidation, false);
    }
    if (valScheme == SAX2XMLReader::Val_Always)
    {
        parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
        parser->setFeature(XMLUni::fgXercesDynamic, false);
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

    bool more = true;
    ifstream fin;

    // the input is a list file
    if (doList)
        fin.open(argV[argInd]);

    if (fin.fail()) {
        cerr <<"Cannot open the list file: " << argV[argInd] << endl;
        return 2;
    }

    while (more)
    {
        char fURI[1000];
        //initialize the array to zeros
        memset(fURI,0,sizeof(fURI));

        if (doList) {
            if (! fin.eof() ) {
                fin.getline (fURI, sizeof(fURI));
                if (!*fURI)
                    continue;
                else {
                    xmlFile = fURI;
                    cerr << "==Parsing== " << xmlFile << endl;
                }
            }
            else
                break;
        }
        else {
            xmlFile = argV[argInd];
            more = false;
        }

        //reset error count first
        handler.resetErrors();

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
        else
            errorOccurred = true;
    }

    if (doList)
        fin.close();

    //
    //  Delete the parser itself.  Must be done prior to calling Terminate, below.
    //
    delete parser;

    // And call the termination method
    XMLPlatformUtils::Terminate();

    if (errorOccurred)
        return 4;
    else
        return 0;

}

