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
 * Revision 1.17  2001/10/25 15:18:33  tng
 * delete the parser before XMLPlatformUtils::Terminate.
 *
 * Revision 1.16  2001/10/19 19:02:43  tng
 * [Bug 3909] return non-zero an exit code when error was encounted.
 * And other modification for consistent help display and return code across samples.
 *
 * Revision 1.15  2001/08/01 19:11:01  tng
 * Add full schema constraint checking flag to the samples and the parser.
 *
 * Revision 1.14  2001/05/11 13:24:58  tng
 * Copyright update.
 *
 * Revision 1.13  2001/05/03 16:00:21  tng
 * Schema: samples update with schema
 *
 * Revision 1.12  2000/06/16 20:25:43  rahulj
 * Add the -v=always option to force validation checking. Need this
 * option for running the conformance tests.
 *
 * Revision 1.11  2000/05/31 18:36:26  rahulj
 * Matched the command line options supported by DOMPrint.
 *
 * Revision 1.10  2000/04/12 22:58:27  roddey
 * Added support for 'auto validate' mode.
 *
 * Revision 1.9  2000/04/06 19:09:51  roddey
 * Some more improvements to output formatting. Now it will correctly
 * handle doing the 'replacement char' style of dealing with chars
 * that are unrepresentable.
 *
 * Revision 1.8  2000/04/05 00:20:32  roddey
 * More updates for the low level formatted output support
 *
 * Revision 1.7  2000/03/28 19:43:11  roddey
 * Fixes for signed/unsigned warnings. New work for two way transcoding
 * stuff.
 *
 * Revision 1.6  2000/03/03 01:29:31  roddey
 * Added a scanReset()/parseReset() method to the scanner and
 * parsers, to allow for reset after early exit from a progressive parse.
 * Added calls to new Terminate() call to all of the samples. Improved
 * documentation in SAX and DOM parsers.
 *
 * Revision 1.5  2000/03/02 19:53:49  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.4  2000/02/11 02:39:43  abagchi
 * Removed StrX::transcode
 *
 * Revision 1.3  2000/02/06 07:47:24  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/01/12 00:27:01  roddey
 * Updates to work with the new URL and input source scheme.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:28  twl
 * Initial checkin
 *
 * Revision 1.7  1999/11/08 20:43:41  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/PlatformUtils.hpp>
#include <util/TransService.hpp>
#include <parsers/SAXParser.hpp>
#include "SAXPrint.hpp"


// ---------------------------------------------------------------------------
//  Local data
//
//  doNamespaces
//      Indicates whether namespace processing should be enabled or not.
//      Defaults to disabled.
//
//  doSchema
//      Indicates whether schema processing should be enabled or not.
//      Defaults to disabled.
//
//  schemaFullChecking
//      Indicates whether full schema constraint checking should be enabled or not.
//      Defaults to disabled.
//
//  encodingName
//      The encoding we are to output in. If not set on the command line,
//      then it is defaulted to LATIN1.
//
//  xmlFile
//      The path to the file to parser. Set via command line.
//
//  valScheme
//      Indicates what validation scheme to use. It defaults to 'auto', but
//      can be set via the -v= command.
// ---------------------------------------------------------------------------
static bool                     doNamespaces        = false;
static bool                     doSchema            = false;
static bool                     schemaFullChecking  = false;
static const char*              encodingName    = "LATIN1";
static XMLFormatter::UnRepFlags unRepFlags      = XMLFormatter::UnRep_CharRef;
static char*                    xmlFile         = 0;
static SAXParser::ValSchemes    valScheme       = SAXParser::Val_Auto;



// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
static void usage()
{
    cout << "\nUsage:\n"
            "    SAX2Print [options] <XML file>\n\n"
            "This program invokes the SAX Parser, and then prints the\n"
            "data returned by the various SAX handlers for the specified\n"
            "XML file.\n\n"
            "Options:\n"
             "    -u=xxx      Handle unrepresentable chars [fail | rep | ref*].\n"
             "    -v=xxx      Validation scheme [always | never | auto*].\n"
             "    -n          Enable namespace processing.\n"
             "    -s          Enable schema processing.\n"
             "    -f          Enable full schema constraint checking.\n"
             "    -x=XXX      Use a particular encoding for output (LATIN1*).\n"
             "    -?          Show this help.\n\n"
             "  * = Default if not provided explicitly.\n\n"
             "The parser has intrinsic support for the following encodings:\n"
             "    UTF-8, USASCII, ISO8859-1, UTF-16[BL]E, UCS-4[BL]E,\n"
             "    WINDOWS-1252, IBM1140, IBM037.\n"
         <<  endl;
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
         cerr << "Error during initialization! :\n"
              << StrX(toCatch.getMessage()) << endl;
         return 1;
    }

    // Check command line and extract arguments.
    if (argC < 2)
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 1;
    }

    int parmInd;
    for (parmInd = 1; parmInd < argC; parmInd++)
    {
        // Break out on first parm not starting with a dash
        if (argV[parmInd][0] != '-')
            break;

        // Watch for special case help request
        if (!strcmp(argV[parmInd], "-?"))
        {
            usage();
            XMLPlatformUtils::Terminate();
            return 2;
        }
         else if (!strncmp(argV[parmInd], "-v=", 3)
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
                XMLPlatformUtils::Terminate();
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
         else if (!strncmp(argV[parmInd], "-x=", 3)
              ||  !strncmp(argV[parmInd], "-X=", 3))
        {
            // Get out the encoding name
            encodingName = &argV[parmInd][3];
        }
         else if (!strncmp(argV[parmInd], "-u=", 3)
              ||  !strncmp(argV[parmInd], "-U=", 3))
        {
            const char* const parm = &argV[parmInd][3];

            if (!strcmp(parm, "fail"))
                unRepFlags = XMLFormatter::UnRep_Fail;
            else if (!strcmp(parm, "rep"))
                unRepFlags = XMLFormatter::UnRep_Replace;
            else if (!strcmp(parm, "ref"))
                unRepFlags = XMLFormatter::UnRep_CharRef;
            else
            {
                cerr << "Unknown -u= value: " << parm << endl;
                XMLPlatformUtils::Terminate();
                return 2;
            }
        }
         else
        {
            cerr << "Unknown option '" << argV[parmInd]
                 << "', ignoring it\n" << endl;
        }
    }

    //
    //  And now we have to have only one parameter left and it must be
    //  the file name.
    //
    if (parmInd + 1 != argC)
    {
        usage();
        XMLPlatformUtils::Terminate();
        return 1;
    }
    xmlFile = argV[parmInd];
    int errorCount = 0;

    //
    //  Create a SAX parser object. Then, according to what we were told on
    //  the command line, set it to validate or not.
    //
    SAXParser* parser = new SAXParser;
    parser->setValidationScheme(valScheme);
    parser->setDoNamespaces(doNamespaces);
    parser->setDoSchema(doSchema);
    parser->setValidationSchemaFullChecking(schemaFullChecking);

    //
    //  Create the handler object and install it as the document and error
    //  handler for the parser-> Then parse the file and catch any exceptions
    //  that propogate out
    //
    try
    {
        SAXPrintHandlers handler(encodingName, unRepFlags);
        parser->setDocumentHandler(&handler);
        parser->setErrorHandler(&handler);
        parser->parse(xmlFile);
        errorCount = parser->getErrorCount();
    }

    catch (const XMLException& toCatch)
    {
        cerr << "\nAn error occured\n  Error: "
             << StrX(toCatch.getMessage())
             << "\n" << endl;
        XMLPlatformUtils::Terminate();
        return -1;
    }

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

