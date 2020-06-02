/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
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
 * $Id$
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "MemoryMonitor.hpp"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/dom/DOM.hpp>

MemoryManager* MemoryMonitor::getExceptionMemoryManager()
{
  return this;
}

void* MemoryMonitor::allocate(XMLSize_t size)
{
    void *key = ::operator new(size);
    fHashTable->put(key, (unsigned int)size);
    return key;
}

void MemoryMonitor::deallocate(void* p)
{
    // if fHashTable doesn't contain p, then this memory manager
    // didn't allocate that memory--a segfault waiting to happen...
    assert(p == 0 || fHashTable->containsKey(p) != 0);
    if (p != 0)
    {
        fHashTable->removeKey(p);
        ::operator delete(p);
    }
}

unsigned int MemoryMonitor::getTotalMemory()
{
    unsigned int total = 0;
    ValueHashTableOfEnumerator<unsigned int> *memEnum =
            new ValueHashTableOfEnumerator<unsigned int>(fHashTable);
    while(memEnum->hasMoreElements()) {
        total += memEnum->nextElement();
    }
    delete memEnum;
    return total;
}

static void usage()
{
    std::cout << "\nUsage:\n"
            "    MemHandlerTest [options] <XML file | List file>\n\n"
            "This program invokes the XercesDOMParser, DOMLSParser, SAXParser ,\n"
            "and the SAX2XMLReader, and ensures that MemoryManagers set on these\n"
            "domBuilders are called to delete just as many bytes as they allocate.\n"
            "This is done for each XML file, and each file is processed\n"
            "as many times as indicated.\n"
            "Options:\n"
            "    -l          Indicate the input file is a List File that has a list of xml files.\n"
            "                Default to off (Input file is an XML file).\n"
            "    -v=xxx      Validation scheme [always | never | auto*].\n"
            "    -n          Enable namespace processing. Defaults to off.\n"
            "    -s          Enable schema processing. Defaults to off.\n"
            "    -f          Enable full schema constraint checking. Defaults to off.\n"
            "    -r=n        Run file through domBuilders n times.\n"
		    "    -?          Show this help.\n\n"
            "  * = Default if not provided explicitly.\n"
         << std::endl;
}

class DOMLSParserHandler : public DOMErrorHandler
{
public:
    DOMLSParserHandler() {};
    ~DOMLSParserHandler() {};
    bool handleError(const DOMError &error)
    {
        char *message = 0;
        std::cerr << "Error occurred in DOMBuilder!  Message:  " <<
            (message = XMLString::transcode(error.getMessage())) << " of severity " << error.getSeverity() << "." << std::endl;
        XMLString::release(&message);
        return true;
    }
};

class SAXErrorHandler : public ErrorHandler
{
public:
    SAXErrorHandler() {};
    ~SAXErrorHandler() {};
    void warning(const SAXParseException &exc )
    {
        char *message = 0;
        std::cerr << "SAX warning received!  Text:  " <<
            (message = XMLString::transcode(exc.getMessage())) << "." << std::endl;
        XMLString::release(&message);
    }
    void error(const SAXParseException &exc )
    {
        char *message = 0;
        std::cerr << "SAX error received!  Text:  " <<
            (message = XMLString::transcode(exc.getMessage())) << "." << std::endl;
        XMLString::release(&message);
    }
    void fatalError(const SAXParseException &exc )
    {
        char *message = 0;
        std::cerr << "SAX fatalError received!  Text:  " <<
            (message = XMLString::transcode(exc.getMessage())) << "." << std::endl;
        XMLString::release(&message);
    }

    // no state so no body
    void resetErrors() {};
};

/**
 * This utility takes similar parameters as DOMCount,
 * with similar meanings.  The only difference is that it runs
 * the file(s) in question through a DOMParser, a DOMBuilder, a SAXParser and
 * a SAX2XMLReader, setting options as appropriate.  It does this
 * sequentially, n times per file with a single domBuilder
 * object, and reports what it finds in terms of memory
 * allocations/deallocations.
 */

int main (int argC,  char *argV[])
{

    MemoryMonitor *staticMemMonitor = new MemoryMonitor();

    // Initialize the XML4C system
    try
    {
        XMLPlatformUtils::Initialize(XMLUni::fgXercescDefaultLocale, 0, 0, staticMemMonitor);
    }
    catch (const XMLException& toCatch)
    {
         char *msg = XMLString::transcode(toCatch.getMessage());
         std::cerr << "Error during initialization! :\n"
              << msg << std::endl;
         XMLString::release(&msg);
         return 1;
    }

    // Check command line and extract arguments.
    if (argC < 2)
    {
        usage();
        return 1;
    }

    const char*                xmlFile = 0;
    AbstractDOMParser::ValSchemes domBuilderValScheme = AbstractDOMParser::Val_Auto;
    bool                       doNamespaces       = false;
    bool                       doSchema           = false;
    bool                       schemaFullChecking = false;
    bool                       doList = false;
    int                        numReps =1;

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
                domBuilderValScheme = AbstractDOMParser::Val_Never;
            else if (!strcmp(parm, "auto"))
                domBuilderValScheme = AbstractDOMParser::Val_Auto;
            else if (!strcmp(parm, "always"))
                domBuilderValScheme = AbstractDOMParser::Val_Always;
            else
            {
                std::cerr << "Unknown -v= value: " << parm << std::endl;
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
         else if (!strncmp(argV[argInd], "-r=", 3)
              ||  !strncmp(argV[argInd], "-R=", 3))
        {
            const char* const numStr = &argV[argInd][3];
            XMLCh* numXStr = XMLString::transcode(numStr);
            numReps = XMLString::parseInt(numXStr);
            XMLString::release(&numXStr);
        }
         else
        {
            std::cerr << "Unknown option '" << argV[argInd]
                 << "', ignoring it\n" << std::endl;
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

    // Instantiate the DOM domBuilder with its memory manager.
    MemoryMonitor *domBuilderMemMonitor = new MemoryMonitor();
    static const XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
    DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(gLS);
    DOMLSParser       *domBuilder = ((DOMImplementationLS*)impl)->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0, domBuilderMemMonitor);
    DOMLSParserHandler domBuilderHandler;
    domBuilder->getDomConfig()->setParameter(XMLUni::fgDOMErrorHandler, &domBuilderHandler);

    // Instantiate the SAX2 parser with its memory manager.
    MemoryMonitor *sax2MemMonitor = new MemoryMonitor();
    SAX2XMLReader *sax2parser = XMLReaderFactory::createXMLReader(sax2MemMonitor);
    SAXErrorHandler saxErrorHandler;
    sax2parser->setErrorHandler(&saxErrorHandler);

    // Instantiate the SAX 1 parser with its memory manager.
    MemoryMonitor *sax1MemMonitor = new MemoryMonitor();
    SAXParser *saxParser = new (sax1MemMonitor) SAXParser(0, sax1MemMonitor);
    saxParser->setErrorHandler(&saxErrorHandler);

    // set features
    domBuilder->getDomConfig()->setParameter(XMLUni::fgDOMNamespaces, doNamespaces);
    sax2parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, doNamespaces);
    saxParser->setDoNamespaces(doNamespaces);

    domBuilder->getDomConfig()->setParameter(XMLUni::fgXercesSchema, doSchema);
    sax2parser->setFeature(XMLUni::fgXercesSchema, doSchema);
    saxParser->setDoSchema(doSchema);

    domBuilder->getDomConfig()->setParameter(XMLUni::fgXercesHandleMultipleImports, true);
    sax2parser->setFeature(XMLUni::fgXercesHandleMultipleImports, true);
    saxParser->setHandleMultipleImports (true);

    domBuilder->getDomConfig()->setParameter(XMLUni::fgXercesSchemaFullChecking, schemaFullChecking);
    sax2parser->setFeature(XMLUni::fgXercesSchemaFullChecking, schemaFullChecking);
    saxParser->setValidationSchemaFullChecking(schemaFullChecking);

    if (domBuilderValScheme == AbstractDOMParser::Val_Auto)
    {
        domBuilder->getDomConfig()->setParameter(XMLUni::fgDOMValidateIfSchema, true);
        sax2parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
        sax2parser->setFeature(XMLUni::fgXercesDynamic, true);
        saxParser->setValidationScheme(SAXParser::Val_Auto);
    }
    else if (domBuilderValScheme == AbstractDOMParser::Val_Never)
    {
        domBuilder->getDomConfig()->setParameter(XMLUni::fgDOMValidate, false);
        sax2parser->setFeature(XMLUni::fgSAX2CoreValidation, false);
        saxParser->setValidationScheme(SAXParser::Val_Never);
    }
    else if (domBuilderValScheme == AbstractDOMParser::Val_Always)
    {
        domBuilder->getDomConfig()->setParameter(XMLUni::fgDOMValidate, true);
        sax2parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
        sax2parser->setFeature(XMLUni::fgXercesDynamic, false);
        saxParser->setValidationScheme(SAXParser::Val_Always);
    }

    // enable datatype normalization - default is off
    domBuilder->getDomConfig()->setParameter(XMLUni::fgDOMDatatypeNormalization, true);

    std::ifstream fin;
    bool more = true;

    // the input is a list file
    if (doList)
        fin.open(argV[argInd]);

    if (fin.fail()) {
        std::cerr <<"Cannot open the list file: " << argV[argInd] << std::endl;
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
                    std::cerr << "==Parsing== " << xmlFile << std::endl;
                }
            }
            else
                break;
        }
        else {
            xmlFile = argV[argInd];
            more = false;
        }

        // parse numReps times (in case we need it for some reason)
        for (int i=0; i<numReps; i++)
        {

            xercesc::DOMDocument *doc = 0;

            try
            {
                // reset document pool
                domBuilder->resetDocumentPool();

                doc = domBuilder->parseURI(xmlFile);
                if(doc && doc->getDocumentElement())
                {
                    xercesc::DOMNodeList *list=NULL;
                    if(doNamespaces)
                        list=doc->getElementsByTagNameNS(doc->getDocumentElement()->getNamespaceURI(), doc->getDocumentElement()->getLocalName());
                    else
                        list=doc->getElementsByTagName(doc->getDocumentElement()->getNodeName());
                    if(list==NULL)
                        std::cout << "getElementsByTagName didn't return a valid DOMNodeList." << std::endl;
                    else if(list->item(0)!=doc->getDocumentElement())
                        std::cout << "getElementsByTagName didn't find the root element." << std::endl;

                    DOMTreeWalker* pWalker=doc->createTreeWalker(doc->getDocumentElement(), DOMNodeFilter::SHOW_TEXT, NULL, true);
	                DOMText* textNode=(DOMText*)pWalker->nextNode();
                    pWalker->release();
                    // this isn't stored or used, but might throw as part of the test?
                    if (textNode!=NULL)
                        textNode->getWholeText();
                }
                sax2parser->parse(xmlFile);
                saxParser->parse(xmlFile);
            }
            catch (const OutOfMemoryException&)
            {
                std::cerr << "OutOfMemoryException during parsing: '" << xmlFile << "'\n" << std::endl;;
                continue;
            }
            catch (const XMLException& toCatch)
            {
                char *msg = XMLString::transcode(toCatch.getMessage());
                std::cerr << "\nError during parsing: '" << xmlFile << "'\n"
                    << "Exception message is:  \n"
                    << msg << "\n" << std::endl;
                XMLString::release(&msg);
                continue;
            }
            catch (const DOMException& toCatch)
            {
                const unsigned int maxChars = 2047;
                XMLCh errText[maxChars + 1];

                std::cerr << "\nDOM Error during parsing: '" << xmlFile << "'\n"
                    << "DOMException code is:  " << toCatch.code << std::endl;

                if (DOMImplementation::loadDOMExceptionMsg(toCatch.code, errText, maxChars))
                {
                    char * msg = XMLString::transcode(errText);
                    std::cerr << "Message is: " << msg << std::endl;

                    continue;
                }
            }
            catch (...)
            {
                std::cerr << "\nUnexpected exception during parsing: '" << xmlFile << "'\n";
                continue;
            }

        }
    }

    //
    //  Delete the domBuilder itself.  Must be done prior to calling Terminate, below.
    //
    domBuilder->release();
    delete sax2parser;
    delete saxParser;

    std::cout << "At destruction, domBuilderMemMonitor has " << domBuilderMemMonitor->getTotalMemory() << " bytes." << std::endl;
    std::cout << "At destruction, sax2MemMonitor has " << sax2MemMonitor->getTotalMemory() << " bytes." << std::endl;
    std::cout << "At destruction, sax1MemMonitor has " << sax1MemMonitor->getTotalMemory() << " bytes." << std::endl;
    delete domBuilderMemMonitor;
    delete sax2MemMonitor;
    delete sax1MemMonitor;

    XMLPlatformUtils::Terminate();
    std::cout << "At destruction, staticMemMonitor has " << staticMemMonitor->getTotalMemory() << " bytes." << std::endl;
    delete staticMemMonitor;
    return 0;
}
