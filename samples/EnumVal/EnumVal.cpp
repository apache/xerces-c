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
 * Revision 1.18  2002/11/05 21:46:19  tng
 * Explicit code using namespace in application.
 *
 * Revision 1.17  2002/04/17 20:18:08  tng
 * [Bug 7493] The word "occured" is misspelled and it is a global error.
 *
 * Revision 1.16  2002/02/01 22:35:51  peiyongz
 * sane_include
 *
 * Revision 1.15  2001/11/22 14:47:54  tng
 * Use the phrase "Grammar" instead of "Validator" in EnumVal and SEnumVal Description.
 *
 * Revision 1.14  2001/10/25 15:18:33  tng
 * delete the parser before XMLPlatformUtils::Terminate.
 *
 * Revision 1.13  2001/10/19 19:02:42  tng
 * [Bug 3909] return non-zero an exit code when error was encounted.
 * And other modification for consistent help display and return code across samples.
 *
 * Revision 1.12  2001/05/11 13:24:54  tng
 * Copyright update.
 *
 * Revision 1.11  2001/04/19 18:16:46  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.10  2001/03/21 21:55:57  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.9  2000/06/12 22:07:22  rahulj
 * Correctly rejects unsupported command line options.
 *
 * Revision 1.8  2000/05/31 18:50:42  rahulj
 * Removed extraneous command line arguments.
 *
 * Revision 1.7  2000/03/03 01:29:30  roddey
 * Added a scanReset()/parseReset() method to the scanner and
 * parsers, to allow for reset after early exit from a progressive parse.
 * Added calls to new Terminate() call to all of the samples. Improved
 * documentation in SAX and DOM parsers.
 *
 * Revision 1.6  2000/03/02 19:53:40  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.5  2000/02/11 02:49:47  abagchi
 * Removed StrX::transcode
 *
 * Revision 1.4  2000/02/11 02:26:40  abagchi
 * Removed StrX::transcode
 *
 * Revision 1.3  2000/02/06 07:47:19  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  1999/12/21 19:54:44  rahulj
 * Added Apache license and version tags.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/NameIdPool.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/XMLValidator.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/validators/DTD/DTDValidator.hpp>
#include <iostream.h>
#include <stdlib.h>
#include <string.h>


XERCES_CPP_NAMESPACE_USE

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
        delete [] fLocalForm;
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

inline ostream& operator<<(ostream& target, const StrX& toDump)
{
    target << toDump.localForm();
    return target;
}


// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
static void usage()
{
    cout << "\nUsage:\n"
            "    EnumVal <XML file>\n\n"
            "This program parses the specified XML file, then shows how to\n"
            "enumerate the contents of the DTD Grammar. Essentially,\n"
            "shows how one can access the DTD information stored in internal\n"
            "data structures.\n"
         << endl;
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
         cerr   << "Error during initialization! Message:\n"
                << StrX(toCatch.getMessage()) << endl;
         XMLPlatformUtils::Terminate();
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
    try
    {
        parser->parse(xmlFile);
        errorCount = parser->getErrorCount();
    }

    catch (const XMLException& e)
    {
        cerr << "\nError during parsing: '" << xmlFile << "'\n"
             << "Exception message is:  \n"
             << StrX(e.getMessage()) << "\n" << endl;
        XMLPlatformUtils::Terminate();
        return 4;
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
            cout << "\nELEMENTS:\n----------------------------\n";
            while(elemEnum.hasMoreElements())
            {
                const DTDElementDecl& curElem = elemEnum.nextElement();
                cout << "  Name: " << StrX(curElem.getFullName()) << "\n";

                cout << "  Content Model: "
                     << StrX(curElem.getFormattedContentModel())
                     << "\n";

                // Get an enumerator for this guy's attributes if any
                if (curElem.hasAttDefs())
                {
                    cout << "  Attributes:\n";
                    XMLAttDefList& attList = curElem.getAttDefList();
                    while (attList.hasMoreElements())
                    {
                        const XMLAttDef& curAttDef = attList.nextElement();
                        cout << "    Name:" << StrX(curAttDef.getFullName())
                             << ", Type: ";

                        // Get the type and display it
                        const XMLAttDef::AttTypes type = curAttDef.getType();
                        switch(type)
                        {
                            case XMLAttDef::CData :
                                cout << "CDATA";
                                break;

                            case XMLAttDef::ID :
                                cout << "ID";
                                break;

                            case XMLAttDef::IDRef :
                            case XMLAttDef::IDRefs :
                                cout << "IDREF(S)";
                                break;

                            case XMLAttDef::Entity :
                            case XMLAttDef::Entities :
                                cout << "ENTITY(IES)";
                                break;

                            case XMLAttDef::NmToken :
                            case XMLAttDef::NmTokens :
                                cout << "NMTOKEN(S)";
                                break;

                            case XMLAttDef::Notation :
                                cout << "NOTATION";
                                break;

                            case XMLAttDef::Enumeration :
                                cout << "ENUMERATION";
                                break;
                        }

                        cout << "\n";
                    }
                }
                cout << endl;
            }
        }
         else
        {
            cout << "The validator has no elements to display\n" << endl;
        }
    }
    else
        cout << "\nErrors occurred, no output available\n" << endl;

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


