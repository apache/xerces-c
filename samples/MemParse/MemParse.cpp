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
 * Revision 1.8  2001/05/03 15:59:40  tng
 * Schema: samples update with schema
 *
 * Revision 1.7  2000/09/11 18:43:48  aruna1
 * OS390 related updates
 *
 * Revision 1.6  2000/03/02 19:53:42  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.5  2000/02/11 02:37:01  abagchi
 * Removed StrX::transcode
 *
 * Revision 1.4  2000/02/06 07:47:19  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.3  2000/01/12 00:27:00  roddey
 * Updates to work with the new URL and input source scheme.
 *
 * Revision 1.2  1999/11/20 01:09:55  rahulj
 * Fixed usage message.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:49  twl
 * Initial checkin
 *
 * Revision 1.7  1999/11/08 20:43:36  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


/**
 * This sample program illustrates how one can use a memory buffer as the
 * input to parser. The memory buffer contains raw bytes representing XML
 * statements.
 *
 * Look at the API documentation for 'MemBufInputSource' for more information
 * on parameters to the constructor.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <parsers/SAXParser.hpp>
#include <framework/MemBufInputSource.hpp>
#include "MemParse.hpp"


// ---------------------------------------------------------------------------
//  Local const data
//
//  gXMLInMemBuf
//      Defines the memory buffer contents here which parsed by the XML
//      parser. This is the cheap way to do it, instead of reading it from
//      somewhere. For this demo, its fine.
//
//      NOTE: This will NOT work if your compiler's default char type is not
//      ASCII, since we indicate in the encoding that its ascii.
//
//  gMemBufId
//      A simple name to give as the system id for the memory buffer. This
//      just for indentification purposes in case of errors. Its not a real
//      system id (and the parser knows that.)
// ---------------------------------------------------------------------------

#ifdef OS390
static const char*  gXMLInMemBuf =
"\
<?xml version='1.0' encoding='ibm-1047-s390'?>\n\
<!DOCTYPE company [\n\
<!ELEMENT company     (product,category,developedAt)>\n\
<!ELEMENT product     (#PCDATA)>\n\
<!ELEMENT category    (#PCDATA)>\n\
<!ATTLIST category idea CDATA #IMPLIED>\n\
<!ELEMENT developedAt (#PCDATA)>\n\
]>\n\n\
<company>\n\
    <product>XML4C</product>\n\
    <category idea='great'>XML Parsing Tools</category>\n\
    <developedAt>\n\
      IBM Center for Java Technology, Silicon Valley, Cupertino, CA\n\
    </developedAt>\n\
</company>\
";
#else
static const char*  gXMLInMemBuf =
"\
<?xml version='1.0' encoding='ascii'?>\n\
<!DOCTYPE company [\n\
<!ELEMENT company     (product,category,developedAt)>\n\
<!ELEMENT product     (#PCDATA)>\n\
<!ELEMENT category    (#PCDATA)>\n\
<!ATTLIST category idea CDATA #IMPLIED>\n\
<!ELEMENT developedAt (#PCDATA)>\n\
]>\n\n\
<company>\n\
    <product>XML4C</product>\n\
    <category idea='great'>XML Parsing Tools</category>\n\
    <developedAt>\n\
      IBM Center for Java Technology, Silicon Valley, Cupertino, CA\n\
    </developedAt>\n\
</company>\
";
#endif

static const char*  gMemBufId = "prodInfo";



// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
void usage()
{
    cout << "\nUsage:\n"
         << "    MemParse [-v]\n"
         << "This program uses the SAX Parser to parse a memory buffer\n"
         << "containing XML statements, and reports the number of\n"
         << "elements and attributes found.\n"
         << "\nOptions:\n"
         << "    -v=xxx      Validation scheme [always | never | auto*]\n"
         << "    -n          Enable namespace processing. Defaults to off.\n"
         << "    -s          Enable schema processing. Defaults to off.\n\n"
         << "  * = Default if not provided explicitly\n"
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

    SAXParser::ValSchemes    valScheme = SAXParser::Val_Auto;
    bool doNamespaces    = false;
    bool doSchema        = false;
    if (argC > 1)
    {
        // See if non validating dom parser configuration is requested.
        if ((argC == 2) && !strcmp(argV[1], "-?"))
        {
            usage();
            return 2;
        }

        int argInd;
        for (argInd = 1; argInd < argC; argInd++)
        {
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
             else
            {
                cerr << "Unknown option '" << argV[argInd]
                     << "', ignoring it\n" << endl;
            }
        }
    }

    //
    //  Create a SAX parser object. Then, according to what we were told on
    //  the command line, set it to validate or not.
    //
    SAXParser parser;
    parser.setValidationScheme(valScheme);
    parser.setDoNamespaces(doNamespaces);
    parser.setDoSchema(doSchema);

    //
    //  Create our SAX handler object and install it on the parser, as the
    //  document and error handlers.
    //
    MemParseHandlers handler;
    parser.setDocumentHandler(&handler);
    parser.setErrorHandler(&handler);

    //
    //  Create MemBufferInputSource from the buffer containing the XML
    //  statements.
    //
    //  NOTE: We are using strlen() here, since we know that the chars in
    //  our hard coded buffer are single byte chars!!! The parameter wants
    //  the number of BYTES, not chars, so when you create a memory buffer
    //  give it the byte size (which just happens to be the same here.)
    //
    MemBufInputSource* memBufIS = new MemBufInputSource
    (
        (const XMLByte*)gXMLInMemBuf
        , strlen(gXMLInMemBuf)
        , gMemBufId
        , false
    );

    //
    //  Get the starting time and kick off the parse of the indicated
    //  file. Catch any exceptions that might propogate out of it.
    //
    unsigned long duration;
    try
    {
        const unsigned long startMillis = XMLPlatformUtils::getCurrentMillis();
        parser.parse(*memBufIS);
        const unsigned long endMillis = XMLPlatformUtils::getCurrentMillis();
        duration = endMillis - startMillis;
    }

    catch (const XMLException& e)
    {
        cerr << "\nError during parsing memory stream:\n"
             << "Exception message is:  \n"
             << StrX(e.getMessage()) << "\n" << endl;
        return -1;
    }

    // Print out the stats that we collected and time taken.
    cout << "\nFinished parsing the memory buffer containing the following "
         << "XML statements:\n\n"
         << gXMLInMemBuf
         << "\n\n\n"
         << "Parsing took " << duration << " ms ("
         << handler.getElementCount() << " elements, "
         << handler.getAttrCount() << " attributes, "
         << handler.getSpaceCount() << " spaces, "
         << handler.getCharacterCount() << " characters).\n" << endl;

    // And call the termination method
    XMLPlatformUtils::Terminate();

    return 0;
}

