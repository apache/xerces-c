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
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/framework/XMLGrammarPoolImpl.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/framework/psvi/XSElementDeclaration.hpp>
#include <xercesc/framework/psvi/XSTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSComplexTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSParticle.hpp>
#include <xercesc/framework/psvi/XSModelGroup.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

using namespace XERCES_CPP_NAMESPACE;

// ---------------------------------------------------------------------------
//  Forward references
// ---------------------------------------------------------------------------
static void usage();

void processElements(XSNamedMap<XSObject> *xsElements);
void processTypeDefinitions(XSNamedMap<XSObject> *xsTypeDefs);
void printBasic(XSObject *xsObject, const char *type);
void printCompositorTypeConnector(XSModelGroup::COMPOSITOR_TYPE type);
void processSimpleTypeDefinition(XSSimpleTypeDefinition * xsSimpleTypeDef);
void processComplexTypeDefinition(XSComplexTypeDefinition *xsComplexTypeDef);
void processParticle(XSParticle *xsParticle);

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of XMLCh data to local code page for display.
// ---------------------------------------------------------------------------
class StrX
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    StrX(const XMLCh* const toTranscode)
{
        // Call the private transcoding method
        fLocalForm = XMLString::transcode(toTranscode);
}

~StrX()
{
    XMLString::release(&fLocalForm);
}


// -----------------------------------------------------------------------
//  Getter methods
// -----------------------------------------------------------------------
const char* localForm() const
{
    return fLocalForm;
}

private :
// -----------------------------------------------------------------------
//  Private data members
//
//  fLocalForm
//      This is the local code page form of the string.
// -----------------------------------------------------------------------
char*   fLocalForm;
};

inline std::ostream& operator<<(std::ostream& target, const StrX& toDump)
{
    target << toDump.localForm();
    return target;
}

class SCMPrintHandler : public DefaultHandler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    SCMPrintHandler();
    ~SCMPrintHandler();

    bool getSawErrors() const
    {
        return fSawErrors;
    }

	void warning(const SAXParseException& exc);
    void error(const SAXParseException& exc);
    void fatalError(const SAXParseException& exc);
    void resetErrors();


private:
    bool            fSawErrors;
};

SCMPrintHandler::SCMPrintHandler() :
    fSawErrors(false)
{
}

SCMPrintHandler::~SCMPrintHandler()
{
}

// ---------------------------------------------------------------------------
//  SCMPrintHandler: Overrides of the SAX ErrorHandler interface
// ---------------------------------------------------------------------------
void SCMPrintHandler::error(const SAXParseException& e)
{
    fSawErrors = true;
    std::cerr << "\nError at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << std::endl;
}

void SCMPrintHandler::fatalError(const SAXParseException& e)
{
    fSawErrors = true;
    std::cerr << "\nFatal Error at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << std::endl;
}

void SCMPrintHandler::warning(const SAXParseException& e)
{
    std::cerr << "\nWarning at file " << StrX(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "\n  Message: " << StrX(e.getMessage()) << std::endl;
}

void SCMPrintHandler::resetErrors()
{
    fSawErrors = false;
}

// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
static void usage()
{
    std::cout << "\nUsage:\n"
    "    SCMPrint [options] <XSD file | List file>\n\n"
    "This program parses XML Schema file(s), to show how one can\n"
    "access the Schema Content Model information.\n\n"
    "Options:\n"
	"    -f     Enable full schema constraint checking processing. Defaults to off.\n"
    "    -l     Indicate the input file is a List File that has a list of XSD files.\n"
    "           Default to off (Input file is a XSD file).\n"
	"    -?     Show this help.\n\n"
    << std::endl;
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

    // cannot return out of catch-blocks lest exception-destruction
    // result in calls to destroyed memory handler!
    int errorCode = 0;
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& toCatch)
    {
        std::cerr   << "Error during initialization! Message:\n"
        << StrX(toCatch.getMessage()) << std::endl;
        errorCode = 2;
    }
    if(errorCode) {
        XMLPlatformUtils::Terminate();
        return errorCode;
    }

    bool							doList				= false;
    bool							schemaFullChecking	= false;
    const char*                     xsdFile             = 0;
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
            return 1;
        }
        else if (!strcmp(argV[argInd], "-l")
              ||  !strcmp(argV[argInd], "-L"))
        {
            doList = true;
        }
        else if (!strcmp(argV[argInd], "-f")
              ||  !strcmp(argV[argInd], "-F"))
        {
            schemaFullChecking = true;
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

    XMLGrammarPool *grammarPool = 0;
    SAX2XMLReader* parser = 0;
    try
    {
        grammarPool = new XMLGrammarPoolImpl(XMLPlatformUtils::fgMemoryManager);

        parser = XMLReaderFactory::createXMLReader(XMLPlatformUtils::fgMemoryManager, grammarPool);
        parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
        parser->setFeature(XMLUni::fgXercesSchema, true);
        parser->setFeature(XMLUni::fgXercesHandleMultipleImports, true);
        parser->setFeature(XMLUni::fgXercesSchemaFullChecking, schemaFullChecking);
        parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, false);
        parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
        parser->setFeature(XMLUni::fgXercesDynamic, true);
        parser->setProperty(XMLUni::fgXercesScannerName, (void *)XMLUni::fgSGXMLScanner);

        SCMPrintHandler handler;
        parser->setErrorHandler(&handler);

        bool more = true;
        bool parsedOneSchemaOkay = false;
        std::ifstream fin;

        // the input is a list file
        if (doList)
            fin.open(argV[argInd]);

        if (fin.fail()) {
            std::cerr <<"Cannot open the list file: " << argV[argInd] << std::endl;
            return 3;
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
                        xsdFile = fURI;
                        std::cerr << "==Parsing== " << xsdFile << std::endl;
                    }
                }
                else
                    break;
            }
            else {
                xsdFile = argV[argInd];
                more = false;
            }

            parser->loadGrammar(xsdFile, Grammar::SchemaGrammarType, true);
            if (handler.getSawErrors())
            {
                handler.resetErrors();
            }
            else
            {
                parsedOneSchemaOkay = true;
            }
        }

        if (parsedOneSchemaOkay)
        {
            std::cout << "********** Printing out information from Schema **********" << "\n\n";
            bool updatedXSModel;
            XSModel *xsModel = grammarPool->getXSModel(updatedXSModel);
            if (xsModel)
            {
                StringList *namespaces = xsModel->getNamespaces();
                for (unsigned i = 0; i < namespaces->size(); i++) {

                    std::cout << "Processing Namespace:   ";
                    const XMLCh *nameSpace = namespaces->elementAt(i);
                    if (nameSpace && *nameSpace)
                        std::cout << StrX(nameSpace);
                    std::cout << "\n============================================" << std::endl << std::endl;

                    processElements(xsModel->getComponentsByNamespace(XSConstants::ELEMENT_DECLARATION,
                                                                  nameSpace));
                    processTypeDefinitions(xsModel->getComponentsByNamespace(XSConstants::TYPE_DEFINITION,
                                                                         nameSpace));
                }
            }
            else
            {
                std::cout << "No XSModel to print" << "\n\n";
            }
        }
        else
        {
            std::cout << "Did not parse a schema document cleanly so not printing Schema for Schema XSModel information";
        }

        std::cout << std::endl;
    }
    catch (const OutOfMemoryException&)
    {
        std::cerr << "OutOfMemoryException during parsing: '" << xsdFile << "'\n" << std::endl;
        errorCode = 6;
    }
    catch (const XMLException& e)
    {
        std::cerr << "\nError during parsing: '" << xsdFile << "'\n"
        << "Exception message is:  \n"
        << StrX(e.getMessage()) << std::endl;
        errorCode = 4;
    }
    catch (...)
    {
        std::cerr << "\nUnexpected exception during parsing: '" << xsdFile << "'\n" << std::endl;
        errorCode = 5;
    }

    delete parser;
    delete grammarPool;
    XMLPlatformUtils::Terminate();

    return errorCode;
}

void printBasic(XSObject *xsObject, const char *type)
{
    std::cout << "Name:\t\t\t";
    const XMLCh *nameSpace = xsObject->getNamespace();
    if (nameSpace && *nameSpace) {
        std::cout << StrX(nameSpace) << ", ";
    }
    std::cout << StrX(xsObject->getName()) << "\n";
    std::cout << "Component Type:\t" << type << std::endl;
}

void processElements(XSNamedMap<XSObject> *xsElements)
{
    if (!xsElements || xsElements->getLength() == 0) {
        std::cout << "no elements\n\n"  << std::endl;
        return;
    }
    for (XMLSize_t i=0; i < xsElements->getLength(); i++) {
        XSElementDeclaration *xsElement = (XSElementDeclaration *)xsElements->item(i);
        printBasic(xsElement, "Element");

        // Content Model
        XSTypeDefinition *xsTypeDef = xsElement->getTypeDefinition();
        std::cout << "Content Model" << "\n";
        std::cout << "\tType:\t";
        if (xsTypeDef->getTypeCategory() == XSTypeDefinition::SIMPLE_TYPE) {
            std::cout << "Simple\n";
        } else {
            std::cout << "Complex\n";
        }
        std::cout << "\tName:\t"
            << StrX(xsTypeDef->getName()) << "\n";

        std::cout << "\n--------------------------------------------" << std::endl;
    }
}

void processSimpleTypeDefinition(XSSimpleTypeDefinition * xsSimpleTypeDef)
{
    XSTypeDefinition *xsBaseTypeDef = xsSimpleTypeDef->getBaseType();
    std::cout << "Base:\t\t\t";
    std::cout << StrX(xsBaseTypeDef->getName()) << std::endl;

    int facets = xsSimpleTypeDef->getDefinedFacets();
    if (facets) {
        std::cout << "Facets:\n";

        if (facets & XSSimpleTypeDefinition::FACET_LENGTH)
                std::cout << "\tLength:\t\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_LENGTH)) << std::endl;
        if (facets & XSSimpleTypeDefinition::FACET_MINLENGTH)
                std::cout << "\tMinLength:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_MINLENGTH)) << std::endl;
        if (facets & XSSimpleTypeDefinition::FACET_MAXLENGTH)
                std::cout << "\tMaxLength:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_MAXLENGTH)) << std::endl;
        if (facets & XSSimpleTypeDefinition::FACET_PATTERN) {
            StringList *lexicalPatterns = xsSimpleTypeDef->getLexicalPattern();
            if (lexicalPatterns && lexicalPatterns->size()) {
                std::cout << "\tPattern:\t\t";
                for (unsigned i = 0; i < lexicalPatterns->size(); i++) {
                    std::cout << StrX(lexicalPatterns->elementAt(i));
                }
                std::cout << std::endl;
            }
        }
        if (facets & XSSimpleTypeDefinition::FACET_WHITESPACE)
                std::cout << "\tWhitespace:\t\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_WHITESPACE)) << std::endl;
        if (facets & XSSimpleTypeDefinition::FACET_MAXINCLUSIVE)
                std::cout << "\tMaxInclusive:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_MAXINCLUSIVE)) << std::endl;
        if (facets & XSSimpleTypeDefinition::FACET_MAXEXCLUSIVE)
                std::cout << "\tMaxExclusive:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_MAXEXCLUSIVE)) << std::endl;
        if (facets & XSSimpleTypeDefinition::FACET_MINEXCLUSIVE)
                std::cout << "\tMinExclusive:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_MINEXCLUSIVE)) << std::endl;
        if (facets & XSSimpleTypeDefinition::FACET_MININCLUSIVE)
                std::cout << "\tMinInclusive:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_MININCLUSIVE)) << std::endl;
        if (facets & XSSimpleTypeDefinition::FACET_TOTALDIGITS)
                std::cout << "\tTotalDigits:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_TOTALDIGITS)) << std::endl;
        if (facets & XSSimpleTypeDefinition::FACET_FRACTIONDIGITS)
                std::cout << "\tFractionDigits:\t" << StrX(xsSimpleTypeDef->getLexicalFacetValue(XSSimpleTypeDefinition::FACET_FRACTIONDIGITS)) << std::endl;
        if (facets & XSSimpleTypeDefinition::FACET_ENUMERATION) {
            StringList *lexicalEnums = xsSimpleTypeDef->getLexicalEnumeration();
            if (lexicalEnums && lexicalEnums->size()) {
                std::cout << "\tEnumeration:\n";
                for (unsigned i = 0; i < lexicalEnums->size(); i++) {
                    std::cout << "\t\t\t" << StrX(lexicalEnums->elementAt(i)) << "\n";
                }
                std::cout << std::endl;
            }
        }
    }
}

void printCompositorTypeConnector(XSModelGroup::COMPOSITOR_TYPE type)
{
    switch (type) {
        case XSModelGroup::COMPOSITOR_SEQUENCE :
            std::cout << ",";
            break;
        case XSModelGroup::COMPOSITOR_CHOICE :
            std::cout << "|";
            break;
        case XSModelGroup::COMPOSITOR_ALL :
            std::cout << "*";
            break;
    }
}

void processParticle(XSParticle *xsParticle)
{
    if (!xsParticle) {
        std::cout << "xsParticle is NULL";
        return;
    }
    XSParticle::TERM_TYPE termType = xsParticle->getTermType();
    if (termType == XSParticle::TERM_ELEMENT) {
        XSElementDeclaration *xsElement = xsParticle->getElementTerm();
        std::cout << StrX(xsElement->getName());
    } else if (termType == XSParticle::TERM_MODELGROUP) {
        std::cout << "(";

        XSModelGroup *xsModelGroup = xsParticle->getModelGroupTerm();
        XSModelGroup::COMPOSITOR_TYPE compositorType = xsModelGroup->getCompositor();
        XSParticleList *xsParticleList = xsModelGroup->getParticles();
        for (unsigned i = 0; i < xsParticleList->size()-1; i++) {
            processParticle(xsParticleList->elementAt(i));
            printCompositorTypeConnector(compositorType);
        }
        processParticle(xsParticleList->elementAt(xsParticleList->size()-1));

        std::cout << ")";
    } else if (termType == XSParticle::TERM_WILDCARD) {
        std::cout << "* (wildcard)";
    }
}

void processComplexTypeDefinition(XSComplexTypeDefinition *xsComplexTypeDef)
{
    XSTypeDefinition *xsBaseTypeDef = xsComplexTypeDef->getBaseType();
    if (xsBaseTypeDef) {
        std::cout << "Base:\t\t\t";
        std::cout << StrX(xsBaseTypeDef->getName()) << "\n";
    }

    std::cout << "Content Model:\t";
    XSComplexTypeDefinition::CONTENT_TYPE contentType = xsComplexTypeDef->getContentType();
    if (contentType == XSComplexTypeDefinition::CONTENTTYPE_ELEMENT ||
        contentType == XSComplexTypeDefinition::CONTENTTYPE_MIXED) {
        processParticle(xsComplexTypeDef->getParticle());
        std::cout << std::endl;
    }
}

void processTypeDefinitions(XSNamedMap<XSObject> *xsTypeDefs)
{
    if (!xsTypeDefs) return;

    for (XMLSize_t i=0; i < xsTypeDefs->getLength(); i++) {
        XSTypeDefinition *xsTypeDef = (XSTypeDefinition *)xsTypeDefs->item(i);

        printBasic(xsTypeDef, "Type Definition");

        // Content Model
        std::cout << "Category:\t";
        if (xsTypeDef->getTypeCategory() == XSTypeDefinition::SIMPLE_TYPE) {
            std::cout << "\tSimple\n";
            processSimpleTypeDefinition((XSSimpleTypeDefinition *)xsTypeDef);
        } else {
            std::cout << "\tComplex\n";
            processComplexTypeDefinition((XSComplexTypeDefinition *)xsTypeDef);
        }

        std::cout << "\n--------------------------------------------" << std::endl;
    }
}
