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
 * Revision 1.10  2001/08/01 19:11:01  tng
 * Add full schema constraint checking flag to the samples and the parser.
 *
 * Revision 1.9  2001/05/11 13:24:55  tng
 * Copyright update.
 *
 * Revision 1.8  2001/05/03 15:59:48  tng
 * Schema: samples update with schema
 *
 * Revision 1.7  2000/06/20 02:23:08  rahulj
 * Help message added by Joe Polastre.
 *
 * Revision 1.6  2000/03/03 01:29:31  roddey
 * Added a scanReset()/parseReset() method to the scanner and
 * parsers, to allow for reset after early exit from a progressive parse.
 * Added calls to new Terminate() call to all of the samples. Improved
 * documentation in SAX and DOM parsers.
 *
 * Revision 1.5  2000/03/02 19:53:44  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.4  2000/02/11 02:37:48  abagchi
 * Removed StrX::transcode
 *
 * Revision 1.3  2000/02/06 07:47:20  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/01/12 00:27:00  roddey
 * Updates to work with the new URL and input source scheme.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:45  twl
 * Initial checkin
 *
 * Revision 1.5  1999/11/08 20:43:38  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  This sample program demonstrates the progressive parse capabilities of
//  the parser system. It allows you to do a scanFirst() call followed by
//  a loop which calls scanNext(). You can drop out when you've found what
//  ever it is you want. In our little test, our event handler looks for
//  16 new elements then sets a flag to indicate its found what it wants.
//  At that point, our progressive parse loop below exits.
//
//  The parameters are:
//
//      [-?]            - Show usage and exit
//      [-v=xxx]        - Validation scheme [always | never | auto*]
//      [-n]            - Enable namespace processing
//      [-s]            - Enable schema processing
//      [-f]            - Enable full schema constraint checking
//      filename        - The path to the XML file to parse
//
//  * = Default if not provided explicitly
//  These are non-case sensitive
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/PlatformUtils.hpp>
#include <framework/XMLPScanToken.hpp>
#include <parsers/SAXParser.hpp>
#include "PParse.hpp"


// ---------------------------------------------------------------------------
//  Local data
//
//  xmlFile
//      The path to the file to parser. Set via command line.
//
//  doNamespaces
//      Indicates whether namespace processing should be done.
//
//  doSchema
//      Indicates whether schema processing should be done.
//
//  schemaFullChecking
//      Indicates whether full schema constraint checking should be done.
//
//  valScheme
//      Indicates what validation scheme to use. It defaults to 'auto', but
//      can be set via the -v= command.
// ---------------------------------------------------------------------------
static char*	 xmlFile         = 0;
static bool     doNamespaces       = false;
static bool     doSchema           = false;
static bool     schemaFullChecking = false;
static SAXParser::ValSchemes    valScheme       = SAXParser::Val_Auto;



// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
static void usage()
{
    cout <<  "\nUsage: PParse [options] <file>\n\n"
         <<  "This sample program demonstrates the progressive parse capabilities of\n"
	     <<  "the parser system. It allows you to do a scanFirst() call followed by\n"
         <<  "a loop which calls scanNext(). You can drop out when you've found what\n"
         <<  "ever it is you want. In our little test, our event handler looks for\n"
         <<  "16 new elements then sets a flag to indicate its found what it wants.\n"
         <<  "At that point, our progressive parse loop exits.\n\n"
         <<  "Options:\n"
         <<  "      -v=xxx        - Validation scheme [always | never | auto*]\n"
         <<  "      -n            - Enable namespace processing [default is off]\n"
         <<  "      -s            - Enable schema processing [default is off]\n"
         <<  "      -f            - Enable full schema constraint checking [default is off]\n"
         <<  "      -?            - Show this help (must be the only parameter)\n\n"
         <<  "  * = Default if not provided explicitly\n"
         <<  endl;
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
         cerr << "Error during initialization! :\n"
              << StrX(toCatch.getMessage()) << endl;
         return 1;
    }


    // Check command line and extract arguments.
    if (argC < 2)
    {
        usage();
        return 1;
    }

    // Watch for special case help request
    if (!strcmp(argV[1], "-?"))
    {
        usage();
        return 2;
    }

    // See if non validating dom parser configuration is requested.
    int parmInd;
    for (parmInd = 1; parmInd < argC; parmInd++)
    {
        // Break out on first parm not starting with a dash
        if (argV[parmInd][0] != '-')
            break;

        if (!strncmp(argV[parmInd], "-v=", 3)
        ||  !strncmp(argV[parmInd], "-V=", 3))
        {
            const char* const parm = &argV[parmInd][3];

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
         else if (!strcmp(argV[parmInd], "-n")
              ||  !strcmp(argV[parmInd], "-N"))
        {
            doNamespaces = true;
        }
         else if (!strcmp(argV[parmInd], "-s")
              ||  !strcmp(argV[parmInd], "-S"))
        {
            doSchema = true;
        }
         else if (!strcmp(argV[parmInd], "-f")
              ||  !strcmp(argV[parmInd], "-F"))
        {
            schemaFullChecking = true;
        }
         else
        {
            usage();
            return 1;
        }
    }

    //
    //  And now we have to have only one parameter left and it must be
    //  the file name.
    //
    if (parmInd + 1 != argC)
    {
        usage();
        return 1;
    }
    xmlFile = argV[parmInd];


    //
    //  Create a SAX parser object to use and create our SAX event handlers
    //  and plug them in.
    //
    SAXParser parser;
    PParseHandlers handler;
    parser.setDocumentHandler(&handler);
    parser.setErrorHandler(&handler);
    parser.setValidationScheme(valScheme);
    parser.setDoNamespaces(doNamespaces);
    parser.setDoSchema(doSchema);
    parser.setValidationSchemaFullChecking(schemaFullChecking);

    //
    //  Ok, lets do the progressive parse loop. On each time around the
    //  loop, we look and see if the handler has found what its looking
    //  for. When it does, we fall out then.
    //
    try
    {
        // Create a progressive scan token
        XMLPScanToken token;

        if (!parser.parseFirst(xmlFile, token))
        {
            cerr << "scanFirst() failed\n" << endl;
            return 1;
        }

        //
        //  We started ok, so lets call scanNext() until we find what we want
        //  or hit the end.
        //
        bool gotMore = true;
        while (gotMore && !handler.getDone())
            gotMore = parser.parseNext(token);

        //
        //  Reset the parser. In this simple progrma, since we just exit
        //  now, its not technically required. But, in programs which
        //  would remain open, you should reset after a progressive parse
        //  in case you broke out before the end of the file. This insures
        //  that all opened files, sockets, etc... are closed.
        //
        parser.parseReset(token);
    }

    catch (const XMLException& toCatch)
    {
        cerr << "\nAn error occured: '" << xmlFile << "'\n"
             << "Exception message is: \n"
             << StrX(toCatch.getMessage())
             << "\n" << endl;
        return -1;
    }

    if (handler.getDone())
        cout << "Got the required 16 elements\n" << endl;
    else
        cout << "Did not get the required 16 elements\n" << endl;

    // And call the termination method
    XMLPlatformUtils::Terminate();

    return 0;
}

