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
* Revision 1.5  2003/12/16 22:59:39  peiyongz
* compilation error fix
*
* Revision 1.4  2003/12/16 21:21:20  peiyongz
* fix compilation error
*
* Revision 1.3  2003/12/16 17:16:08  peiyongz
* . Using BinMemInputStream/BinMemOutputStream
* . Using SAX2XMLReader
*
* Revision 1.1  2003/12/12 18:17:25  peiyongz
* XSerializerTest
*
*
*/

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/internal/XMLGrammarPoolImpl.hpp>
#include <xercesc/internal/MemoryManagerImpl.hpp>
#include <xercesc/internal/XSerializationException.hpp>
#include <xercesc/internal/BinMemOutputStream.hpp>
#include <xercesc/util/BinMemInputStream.hpp>

#include "XSerializerTest.hpp"

#if defined(XERCES_NEW_IOSTREAMS)
#include <fstream>
#else
#include <fstream.h>
#endif

// ---------------------------------------------------------------------------
//  command line option variables
// ---------------------------------------------------------------------------
static const char*                  xmlFile            = 0;
static SAX2XMLReader::ValSchemes    valScheme          = SAX2XMLReader::Val_Auto;
static bool                         doNamespaces       = true;
static bool                         doSchema           = true;
static bool                         schemaFullChecking = false;
static bool                         doList             = false;
static bool                         namespacePrefixes  = false;
static bool                         errorOccurred      = false;
static bool                         recognizeNEL       = false;

static bool                         showSerializationError = false;
static char                         localeStr[64];

// ---------------------------------------------------------------------------
//  parsing components
// ---------------------------------------------------------------------------
static SAX2XMLReader*           parser      = 0;
static MemoryManager*           myMemMgr    = 0;
static XMLGrammarPool*          myGramPool  = 0;
static XSerializerHandlers*     handler     = 0;
static BinInputStream*          myIn        = 0;
static BinOutputStream*         myOut       = 0;

static bool                     serializeGrammarOK = true;
static const int                BufSize     = 1024;

/***
*
*   This program is a variation of SAXCount.
*
*   Whenever a file is served, it parses the file and catch the grammar without
*   issue any error message with regards to the parsing, and serialize the grammar
*   and deserialize the grammar, and parse the instance document a second time 
*   and validate the instance against the serialized grammar if validation is on.
*
***/

// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
void usage()
{
    XERCES_STD_QUALIFIER cout << "\nUsage:\n"
            "    XSerializerTest [options] <XML file | List file>\n\n"
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
         << XERCES_STD_QUALIFIER endl;
}

static void init()
{
    //
    //  Create our SAX handler object and install it on the parser, as the
    //  document and error handler.
    //
    if (!handler)
        handler = new XSerializerHandlers();

}

static void cleanUp()
{
    if (handler)
        delete handler;

    if (myIn)
        delete myIn;

    if (myOut)
        delete myOut;

}

static BinOutputStream* getOutputStream()
{
    if (!myOut)
    {
        myOut = new BinMemOutputStream(BufSize);
    }

    ((BinMemOutputStream*)myOut)->reset();

    return myOut;
}

static BinInputStream* getInputStream()
{
    if (!myOut)
    {
        XERCES_STD_QUALIFIER cerr << "DEserialization has to be done after serialization\n";
        exit(-1);
    }

    //BinMemInputStream can not refer to a different data once
    //it is instantiated, so we delete it and have a new one.
    if (myIn)
    {
        delete myIn;
        myIn = 0;
    }

    //make it to refer to the binary data saved in the myOut
    //but the data still belong to myOut
    myIn = new BinMemInputStream( ((BinMemOutputStream*)myOut)->getRawBuffer()
                                , BufSize
                                , BinMemInputStream::BufOpt_Reference
                                );
    return myIn;
}

static void getParser(bool setHandler)
{

    myMemMgr       = new MemoryManagerImpl();
    myGramPool     = new XMLGrammarPoolImpl(myMemMgr);

    parser = XMLReaderFactory::createXMLReader(myMemMgr, myGramPool);

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

    if (setHandler)
    {
        parser->setContentHandler(handler);
        parser->setErrorHandler(handler);
    }
    else
    {
        parser->setContentHandler(0);
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

//    parser->cacheGrammarFromParse(true);
    parser->setFeature(XMLUni::fgXercesCacheGrammarFromParse, true);

    try
    {
        parser->parse(xmlFile);
    }
    catch (...)
    {
        //do nothing
        // it could be instance document is invalid
        // but the grammar is fine
    }

    try
    {
        myGramPool->serializeGrammars(getOutputStream());
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
        myGramPool->deserializeGrammars(getInputStream());
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
        parser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);
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
                valScheme = SAX2XMLReader::Val_Never;
            else if (!strcmp(parm, "auto"))
                valScheme = SAX2XMLReader::Val_Auto;
            else if (!strcmp(parm, "always"))
                valScheme = SAX2XMLReader::Val_Always;
            else
            {
                XERCES_STD_QUALIFIER cerr << "Unknown -v= value: " << parm << XERCES_STD_QUALIFIER endl;
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

