/*
* The Apache Software License, Version 1.1
*
* Copyright (c) 1999-2003 The Apache Software Foundation.  All rights
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
* $Log$
* Revision 1.2  2003/12/13 20:17:18  neilg
* fix compilation errors under gcc
*
* Revision 1.1  2003/12/12 18:17:25  peiyongz
* XSerializerTest
*
*
*/

// ---------------------------------------------------------------------------
//  Includes
 // ---------------------------------------------------------------------------
#include <xercesc/framework/XMLGrammarPool.hpp>
#include <xercesc/internal/XMLGrammarPoolImpl.hpp>

#include <xercesc/util/BinFileInputStream.hpp>
#include <xercesc/internal/BinFileOutputStream.hpp>

#include <xercesc/internal/MemoryManagerImpl.hpp>
#include <xercesc/internal/XSerializationException.hpp>
#include <xercesc/internal/XObjectComparator.hpp>


#include "XSerializerTest.hpp"

#if defined(XERCES_NEW_IOSTREAMS)
#include <fstream>
#else
#include <fstream.h>
#endif

static const char*              xmlFile = 0;
static SAXParser::ValSchemes    valScheme = SAXParser::Val_Auto;
static bool                     doNamespaces       = false;
static bool                     doSchema           = false;
static bool                     schemaFullChecking = false;
static bool                     doList             = false;
static bool                     errorOccurred      = false;
static bool                     recognizeNEL       = false;
static bool                     showSerializationError = false;
static char                     localeStr[64];

static SAXParser*               parser      = 0;
static MemoryManager*           myMemMgr    = 0;
static XMLGrammarPoolImpl*      myGramPool  = 0;
static SAXCountHandlers*        handler     = 0;

static bool                     serializeGrammarOK = true;
static const char binDataFile[]="d:/XSerializerTest.txt";
static const char binLog[] = "d:/XseriailzerTest.log";

/***
*
*   This program is a variation of SAXCount.
*
*   Whenever a file is served, it parses the file and catch the grammar without
*   issue any error message with regards to the parsing, and serialize the grammar
*   and deserialize the grammar, and parse the instance document a second time 
*   and validate the instance against he serialized grammar if validation is on.
*
***/

// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
void usage()
{
    XERCES_STD_QUALIFIER cout << "\nUsage:\n"
            "    XSerializerTest [options] <XML file | List file>\n\n"
            "This program invokes the SAX Parser, and then prints the\n"
            "number of elements, attributes, spaces and characters found\n"
            "in each XML file, using SAX API.\n\n"
            "Options:\n"
            "    -l          Indicate the input file is a List File that has a list of xml files.\n"
            "                Default to off (Input file is an XML file).\n"
            "    -v=xxx      Validation scheme [always | never | auto*].\n"
            "    -n          Enable namespace processing. Defaults to off.\n"
            "    -s          Enable schema processing. Defaults to off.\n"
            "    -f          Enable full schema constraint checking. Defaults to off.\n"
            "    -locale=ll_CC specify the locale, default: en_US.\n"
            "    -e          Show serialization error. Defaults to off.\n"
		    "    -?          Show this help.\n\n"
            "  * = Default if not provided explicitly.\n"
         << XERCES_STD_QUALIFIER endl;
}

static void init()
{
    //
    //  Create our SAX handler object and install it on the parser, as the
    //  document and error handler.
    //
    if (!handler)
        handler = new SAXCountHandlers();

}

static void cleanUp()
{
    if (handler)
        delete handler;

}

static void getParser(bool setHandler)
{
    //
    //  Create a SAX parser object. Then, according to what we were told on
    //  the command line, set it to validate or not.
    //
    myMemMgr       = new MemoryManagerImpl();
    myGramPool     = new XMLGrammarPoolImpl(myMemMgr);
    parser         = new SAXParser(0, myMemMgr, myGramPool);

    parser->setValidationScheme(valScheme);
    parser->setDoNamespaces(doNamespaces);
    parser->setDoSchema(doSchema);
    parser->setValidationSchemaFullChecking(schemaFullChecking);

    if (setHandler)
    {
        parser->setDocumentHandler(handler);
        parser->setErrorHandler(handler);
    }
    else
    {
        parser->setDocumentHandler(0);
        parser->setErrorHandler(0);
    }
}

static void destroyParser()
{
    //the order is important
    delete parser;
    delete myGramPool;
    delete myMemMgr;
}

// parse the instance document and
// build a grammar from parsing
//return false if no grammar built
static bool getAndSaveGrammar(const char* const xmlFile)
{
    bool    retVal = true;
    getParser(false);  //don't emit error
    parser->cacheGrammarFromParse(true);

    try
    {
        parser->parse(xmlFile);
    }
    catch (...)
    {

        int i = 1;
        //do nothing
        // it could be instance document is invalid
        // but the grammar is fine
    }

    XObjectComparator::dumpContent((XMLGrammarPoolImpl*)myGramPool);

    try
    {
        BinFileOutputStream myOut(binDataFile);
        myGramPool->serializeGrammars(&myOut);
    }
    catch(const XSerializationException& e)
    {
        //do emit error here so that we know serialization failure
        if (showSerializationError)
        {
            XERCES_STD_QUALIFIER cerr << "An error occurred during serialization\n   Message: "
             << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
        }
        retVal = false;
    }

    destroyParser();
    return retVal;
}

static bool restoreGrammar()
{
    bool    retVal = true;
    getParser(true);  //emit error

    try
    {
        BinFileInputStream myIn(binDataFile);
        myGramPool->deserializeGrammars(&myIn);
    }
    catch(const XSerializationException& e)
    {
        if (showSerializationError)
        {
            XERCES_STD_QUALIFIER cerr << "An error occurred during de-serialization\n   Message: "
                << StrX(e.getMessage()) << XERCES_STD_QUALIFIER endl;
        }

        destroyParser();
        retVal = false;
    }

    //parser to be used by parseing file
    return retVal;
}

static void parseFile(const char* const xmlFile)
{
    //
    //  Get the starting time and kick off the parse of the indicated
    //  file. Catch any exceptions that might propogate out of it.
    //
    unsigned long duration;

    //reset error count first
    handler->resetErrors();

    try
    {
        const unsigned long startMillis = XMLPlatformUtils::getCurrentMillis();
        parser->parse(xmlFile);
        const unsigned long endMillis = XMLPlatformUtils::getCurrentMillis();
        duration = endMillis - startMillis;
    }
    catch (const XMLException& e)
    {
        XERCES_STD_QUALIFIER cerr << "\nError during parsing: '" << xmlFile << "'\n"
            << "Exception message is:  \n"
            << StrX(e.getMessage()) << "\n" << XERCES_STD_QUALIFIER endl;
        errorOccurred = true;
    }
    catch (...)
    {
        XERCES_STD_QUALIFIER cerr << "\nUnexpected exception during parsing: '" << xmlFile << "'\n";
        errorOccurred = true;
    }

    // Print out the stats that we collected and time taken
    if (!handler->getSawErrors())
    {
        XERCES_STD_QUALIFIER cout << xmlFile << ": " << duration << " ms ("
            << handler->getElementCount() << " elems, "
            << handler->getAttrCount() << " attrs, "
            << handler->getSpaceCount() << " spaces, "
            << handler->getCharacterCount() << " chars)" << XERCES_STD_QUALIFIER endl;
    }
    else
        errorOccurred = true;

}

static void parseCase(const char* const xmlFile)
{

    //if we can successfully getAndSaveGrammar and
    // restoreGrammar, then parse using the cached Grammar
    if (getAndSaveGrammar(xmlFile) && restoreGrammar())
    {
        parser->useCachedGrammarInParse(true);
    }
    else //otherwise, do a normal parsing
    {
        getParser(true);
    }

    parseFile(xmlFile);
    destroyParser();

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
                valScheme = SAXParser::Val_Never;
            else if (!strcmp(parm, "auto"))
                valScheme = SAXParser::Val_Auto;
            else if (!strcmp(parm, "always"))
                valScheme = SAXParser::Val_Always;
            else
            {
                XERCES_STD_QUALIFIER cerr << "Unknown -v= value: " << parm << XERCES_STD_QUALIFIER endl;
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
         else if (!strcmp(argV[argInd], "-e")
              ||  !strcmp(argV[argInd], "-E"))
        {
            showSerializationError = true;
        }         
        else
        {
            XERCES_STD_QUALIFIER cerr << "Unknown option '" << argV[argInd]
                << "', ignoring it\n" << XERCES_STD_QUALIFIER endl;
        }
    }

    //
    //  There should at least one parameter left, and that
    //  should be the file name(s).
    //
    if (argInd == argC)
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

        init();
    }

    catch (const XMLException& toCatch)
    {
        XERCES_STD_QUALIFIER cerr << "Error during initialization! Message:\n"
            << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
        return 1;
    }

    XERCES_STD_QUALIFIER ifstream fin;

    // the input is a list file
    if (doList)
        fin.open(argV[argInd]);

    if (fin.fail()) {
        XERCES_STD_QUALIFIER cerr <<"Cannot open the list file: " << argV[argInd] << XERCES_STD_QUALIFIER endl;
        return 2;
    }

    while (true)
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
                    XERCES_STD_QUALIFIER cerr << "==Parsing== " << xmlFile << XERCES_STD_QUALIFIER endl;
                }
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

        parseCase(xmlFile);
    }

    if (doList)
        fin.close();

    cleanUp();

    // And call the termination method
    XMLPlatformUtils::Terminate();

    if (errorOccurred)
        return 4;
    else
        return 0;

}

