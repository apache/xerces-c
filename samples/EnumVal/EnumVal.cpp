/*
 * (C) Copyright IBM Corp. 1999  All rights reserved.
 *
 * US Government Users Restricted Rights Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *
 * The program is provided "as is" without any warranty express or
 * implied, including the warranty of non-infringement and the implied
 * warranties of merchantibility and fitness for a particular purpose.
 * IBM will not be liable for any damages suffered by you as a result
 * of using the Program. In no event will IBM be liable for any
 * special, indirect or consequential damages or lost profits even if
 * IBM has been advised of the possibility of their occurrence. IBM
 * will not be liable for any third party claims against you.
 */

/**
 * @version Revision: %M% %I% %W% %Q%
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
	StrX(const XMLCh* const toTranscode, const unsigned int len = 0) :

        fLocalForm(0)
    {
        // Call the private transcoding method
        transcode(toTranscode, len);
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
    //  Private helper methods
    // -----------------------------------------------------------------------
	void transcode (const XMLCh* const toTranscode, const unsigned int len);


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
//  Program entry point
// ---------------------------------------------------------------------------
int main(int argc, char* args[])
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
         return 1;
    }

    // We only have one required parameter, which is the file to process
    if (argc < 2)
    {
        usage();
        return -1;
    }
    const char* xmlFile = args[1];
    bool  doValidation = false;

    // Check for some special cases values of the parameter
    if (!strncmp(xmlFile, "-?", 2))
    {
        usage();
        return 0;
    }
     else if (!strncmp(xmlFile, "-v", 2))
    {
        doValidation = true;
        if (argc < 3)
        {
            usage();
            return -1;
        }
        xmlFile = args[2];
    }
     else if (xmlFile[0] == '-')
    {
        usage();
        return -1;
    }

    //
    //  Create a DTD validator to be used for our validation work. Then create
    //  a SAX parser object and pass it our validator. Then, according to what
    //  we were told on the command line, set it to validate or not. He owns
    //  the validator, so we have to allocate it.
    //
    DTDValidator* valToUse = new DTDValidator;
    SAXParser parser(valToUse);
    parser.setDoValidation(doValidation);

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
        return -1;
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
    return 0;
}


// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
static void usage()
{
    cout << "\nUsage:\n"
         "    EnumVal [-v] <XML file>\n"
         "    -v  Do a validating parse. Defaults to non-validating.\n"
         "This program parses a file, then shows how to enumerate the"
         "contents of the validator pools\n" << endl;
}


// ---------------------------------------------------------------------------
//  StrX: Private helper methods
// ---------------------------------------------------------------------------
void StrX::transcode(const XMLCh* const toTranscode, const unsigned int len)
{
    // Short circuit if its a null pointer
    if (!toTranscode)
    {
        fLocalForm = new char[1];
        fLocalForm[0] = 0;
        return;
	}

    // See if our XMLCh and wchar_t as the same on this platform
    const bool isSameSize = (sizeof(XMLCh) == sizeof(wchar_t));

    //
    //  Get the actual number of chars. If the passed len is zero, its null
    //  terminated. Else we have to use the len.
    //
    wchar_t realLen = (wchar_t)len;
    if (!realLen)
    {
        //
        //  We cannot just assume we can use wcslen() because we don't know
        //  if our XMLCh is the same as wchar_t on this platform.
        //
        const XMLCh* tmpPtr = toTranscode;
        while (*(tmpPtr++))
            realLen++;
    }

    //
    //  If either the passed length was non-zero or our char sizes are not 
    //  same, we have to use a temp buffer. Since this is common in these
    //  samples, we just do it anyway.
    //
    wchar_t* tmpSource = new wchar_t[realLen + 1];
    if (isSameSize)
    {
        memcpy(tmpSource, toTranscode, realLen * sizeof(wchar_t));
    }
     else
    {
        for (unsigned int index = 0; index < realLen; index++)
            tmpSource[index] = (wchar_t)toTranscode[index];
    }
    tmpSource[realLen] = 0;

    // See now many chars we need to transcode this guy
    const unsigned int targetLen = ::wcstombs(0, tmpSource, 0);

    // Allocate out storage member
    fLocalForm = new char[targetLen + 1];

    //
    //  And transcode our temp source buffer to the local buffer. Cap it
    //  off since the converter won't do it (because the null is beyond
    //  where the target will fill up.)
    //
    ::wcstombs(fLocalForm, tmpSource, targetLen);
    fLocalForm[targetLen] = 0;

    // Don't forget to delete our temp buffer
    delete [] tmpSource;
}
