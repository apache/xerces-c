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
#include <xercesc/util/NameIdPool.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/XMLValidator.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/validators/DTD/DTDValidator.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>    
#include <iostream>
#include <cstdlib>
#include <cstring>


using namespace XERCES_CPP_NAMESPACE;

// ---------------------------------------------------------------------------
//  Forward references
// ---------------------------------------------------------------------------
static void usage();



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


// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
static void usage()
{
    std::cout << "\nUsage:\n"
            "    EnumVal <XML file>\n\n"
            "This program parses the specified XML file, then shows how to\n"
            "enumerate the contents of the DTD Grammar. Essentially,\n"
            "shows how one can access the DTD information stored in internal\n"
            "data structures.\n"
         << std::endl;
}


// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{
    // Initialize the XML4C system
    try
    {
         XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& toCatch)
    {
         std::cerr   << "Error during initialization! Message:\n"
                << StrX(toCatch.getMessage()) << std::endl;
         return 1;
    }

    // Check command line and extract arguments.
    if (argC < 2)
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 1;
    }

    // We only have one required parameter, which is the file to process
    if ((argC != 2) || (*(argV[1]) == '-'))
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 1;
    }

    const char*              xmlFile   = argV[1];
    SAXParser::ValSchemes    valScheme = SAXParser::Val_Auto;

    //
    //  Create a DTD validator to be used for our validation work. Then create
    //  a SAX parser object and pass it our validator. Then, according to what
    //  we were told on the command line, set it to validate or not. He owns
    //  the validator, so we have to allocate it.
    //
    int errorCount = 0;
    DTDValidator* valToUse = new DTDValidator;
    SAXParser* parser = new SAXParser(valToUse);
    parser->setValidationScheme(valScheme);

    //
    //  Get the starting time and kick off the parse of the indicated
    //  file. Catch any exceptions that might propogate out of it.
    //
    int errorCode = 0;
    try
    {
        parser->parse(xmlFile);
        errorCount = parser->getErrorCount();
    }
    catch (const OutOfMemoryException&)
    {
        std::cerr << "OutOfMemoryException" << std::endl;
        errorCode = 5;
    }
    catch (const XMLException& e)
    {
        std::cerr << "\nError during parsing: '" << xmlFile << "'\n"
             << "Exception message is:  \n"
             << StrX(e.getMessage()) << "\n" << std::endl;
        errorCode = 4;
    }
    if(errorCode) {
        XMLPlatformUtils::Terminate();
        return errorCode;
    }

    if (!errorCount) {
        //
        //  Now we will get an enumerator for the element pool from the validator
        //  and enumerate the elements, printing them as we go. For each element
        //  we get an enumerator for its attributes and print them also.
        //
        DTDGrammar* grammar = (DTDGrammar*) valToUse->getGrammar();
        NameIdPoolEnumerator<DTDElementDecl> elemEnum = grammar->getElemEnumerator();
        if (elemEnum.hasMoreElements())
        {
            std::cout << "\nELEMENTS:\n----------------------------\n";
            while(elemEnum.hasMoreElements())
            {
                const DTDElementDecl& curElem = elemEnum.nextElement();
                std::cout << "  Name: " << StrX(curElem.getFullName()) << "\n";

                std::cout << "  Content Model: "
                     << StrX(curElem.getFormattedContentModel())
                     << "\n";

                // Get an enumerator for this guy's attributes if any
                if (curElem.hasAttDefs())
                {
                    std::cout << "  Attributes:\n";
                    XMLAttDefList& attList = curElem.getAttDefList();
                    for (unsigned int i=0; i<attList.getAttDefCount(); i++)
                    {
                        const XMLAttDef& curAttDef = attList.getAttDef(i);
                        std::cout << "    Name:" << StrX(curAttDef.getFullName())
                             << ", Type: ";

                        // Get the type and display it
                        const XMLAttDef::AttTypes type = curAttDef.getType();
                        switch(type)
                        {
                            case XMLAttDef::CData :
                                std::cout << "CDATA";
                                break;

                            case XMLAttDef::ID :
                                std::cout << "ID";
                                break;

                            case XMLAttDef::IDRef :
                            case XMLAttDef::IDRefs :
                                std::cout << "IDREF(S)";
                                break;

                            case XMLAttDef::Entity :
                            case XMLAttDef::Entities :
                                std::cout << "ENTITY(IES)";
                                break;

                            case XMLAttDef::NmToken :
                            case XMLAttDef::NmTokens :
                                std::cout << "NMTOKEN(S)";
                                break;

                            case XMLAttDef::Notation :
                                std::cout << "NOTATION";
                                break;

                            case XMLAttDef::Enumeration :
                                std::cout << "ENUMERATION";
                                break;
                            default:
                                break;
                        }

                        std::cout << "\n";
                    }
                }
                std::cout << std::endl;
            }
        }
         else
        {
            std::cout << "The validator has no elements to display\n" << std::endl;
        }
    }
    else
        std::cout << "\nErrors occurred, no output available\n" << std::endl;

    //
    //  Delete the parser itself.  Must be done prior to calling Terminate, below.
    //
    delete parser;

    // And call the termination method
    XMLPlatformUtils::Terminate();

    if (errorCount > 0)
        return 4;
    else
        return 0;
}


