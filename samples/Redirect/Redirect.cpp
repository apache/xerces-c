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


/**
 *
 * This simplistic sample illustrates how an XML application can use
 * the SAX entityResolver handler to provide customized handling for
 * external entities.
 *
 * It registers an entity resolver with the parser. When ever the parser
 * comes across an external entity, like a reference to an external DTD
 * file, it calls the 'resolveEntity()' callback. This callback in this
 * sample checks to see if the external entity to be resolved is the file
 * 'personal.dtd'.
 *
 * If it is then, it redirects the input stream to the file 'redirect.dtd',
 * which is then read instead of 'personal.dtd'.
 *
 * If the external entity to be resolved was not the file 'personal.dtd', the
 * callback returns NULL indicating that do the default behaviour which is
 * to read the contents of 'personal.dtd'.
 *
 * $Log$
 * Revision 1.5  2001/10/19 19:02:42  tng
 * [Bug 3909] return non-zero an exit code when error was encounted.
 * And other modification for consistent help display and return code across samples.
 *
 * Revision 1.4  2000/05/31 18:53:15  rahulj
 * Removed extraneous command line arguments.
 *
 * Revision 1.3  2000/02/11 02:38:28  abagchi
 * Removed StrX::transcode
 *
 * Revision 1.2  2000/02/06 07:47:21  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:37  twl
 * Initial checkin
 *
 * Revision 1.6  1999/11/08 20:43:39  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <parsers/SAXParser.hpp>
#include "Redirect.hpp"


// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
void usage()
{
    cout << "\nUsage:\n"
            "    Redirect <XML file>\n\n"
            "This program installs an entity resolver, traps the call to\n"
            "the external DTD file and redirects it to another application\n"
            "specific file which contains the actual dtd.\n\n"
            "The program then counts and reports the number of elements and\n"
            "attributes in the given XML file.\n"
         << endl;
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
        cerr << "Error during initialization! Message:\n"
             << StrX(toCatch.getMessage()) << endl;
        return 1;
    }

    // We only have one parameter, which is the file to process
    // We only have one required parameter, which is the file to process
    if ((argc != 2) || (*(args[1]) == '-'))
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 1;
    }

    const char*              xmlFile = args[1];

    //
    //  Create a SAX parser object. Then, according to what we were told on
    //  the command line, set it to validate or not.
    //
    SAXParser parser;

    //
    //  Create our SAX handler object and install it on the parser, as the
    //  document, entity and error handlers.
    //
    RedirectHandlers handler;
    parser.setDocumentHandler(&handler);
    parser.setErrorHandler(&handler);
    parser.setEntityResolver(&handler);

    //
    //  Get the starting time and kick off the parse of the indicated file.
    //  Catch any exceptions that might propogate out of it.
    //
    unsigned long duration;
    int errorCount = 0;
    try
    {
        const unsigned long startMillis = XMLPlatformUtils::getCurrentMillis();
        parser.parse(xmlFile);
        const unsigned long endMillis = XMLPlatformUtils::getCurrentMillis();
        duration = endMillis - startMillis;
        errorCount = parser.getErrorCount();
    }

    catch (const XMLException& e)
    {
        cerr << "\nError during parsing: '" << xmlFile << "'\n"
                << "Exception message is:  \n"
                << StrX(e.getMessage()) << "\n" << endl;
        XMLPlatformUtils::Terminate();
        return 4;
    }

    // Print out the stats that we collected and time taken.
    if (!errorCount) {
        cout << xmlFile << ": " << duration << " ms ("
             << handler.getElementCount() << " elems, "
             << handler.getAttrCount() << " attrs, "
             << handler.getSpaceCount() << " spaces, "
             << handler.getCharacterCount() << " chars)" << endl;
    }

    XMLPlatformUtils::Terminate();

    if (errorCount > 0)
        return 4;
    else
        return 0;
}

