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
#include <util/NameIdPool.hpp>
#include <util/PlatformUtils.hpp>
#include <framework/XMLValidator.hpp>
#include <parsers/SAXParser.hpp>
#include <validators/DTD/DTDValidator.hpp>
#include <iostream.h>
#include <stdlib.h>
#include <string.h>


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
            "This program parses a file, then shows how to enumerate the\n"
            "contents of the validator pools. Essentially, shows how one can\n"
            "access the DTD information stored in internal data structures.\n\n"
            "  * = Default if not provided explicitly\n"
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
    if ((argC == 2) && (*(argV[1]) == '-'))
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 2;
    }
    
    const char*              xmlFile   = argV[1];
    SAXParser::ValSchemes    valScheme = SAXParser::Val_Auto;

    //
    //  Create a DTD validator to be used for our validation work. Then create
    //  a SAX parser object and pass it our validator. Then, according to what
    //  we were told on the command line, set it to validate or not. He owns
    //  the validator, so we have to allocate it.
    //
    DTDValidator* valToUse = new DTDValidator;
    SAXParser parser(valToUse);
    parser.setValidationScheme(valScheme);

    //
    //  Get the starting time and kick off the parse of the indicated
    //  file. Catch any exceptions that might propogate out of it.
    //
    try
    {
        parser.parse(xmlFile);
    }

    catch (const XMLException& e)
    {
        cerr << "\nError during parsing: '" << xmlFile << "'\n"
             << "Exception message is:  \n"
             << StrX(e.getMessage()) << "\n" << endl;
        XMLPlatformUtils::Terminate();
        return 3;
    }

    //
    //  Now we will get an enumerator for the element pool from the validator
    //  and enumerate the elements, printing them as we go. For each element
    //  we get an enumerator for its attributes and print them also.
    //
    NameIdPoolEnumerator<DTDElementDecl> elemEnum = valToUse->getElemEnumerator();
    if (elemEnum.hasMoreElements())
    {
        cout << "\nELEMENTS:\n----------------------------\n";
        while(elemEnum.hasMoreElements())
        {
            const DTDElementDecl& curElem = elemEnum.nextElement();
            cout << "  Name: " << StrX(curElem.getFullName()) << "\n";

            cout << "  Content Model: "
                 << StrX(curElem.getFormattedContentModel(*valToUse))
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

    // And call the termination method
    XMLPlatformUtils::Terminate();

    return 0;
}


