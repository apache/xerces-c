/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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
 * $Id$
 */

// ---------------------------------------------------------------------------
//  This sample program invokes the XercesDOMParser to build a DOM tree for
//  the specified input file. It then invokes DOMWriter::writeToString() to 
//  serialize the resultant DOM tree in to an XMLCh stream, if no error occurs 
//  during the parsing. 
//
//  If "-wverify" is specified, the parser will parse the resultant string
//  for the second time to verify the serialized result from the first parse,
//  and invokes DOMWriter::writeNode() to serialize the resultant DOM tree.
//  
//  Note: since any combination of characters can be the end of line sequence,
//        the resultant XML stream may NOT be well formed any more.
//
//        In case a filter is specified, and which does filter out some part 
//        of the original XML document, thus the resultant XML stream may not
//        valid.
//
//   Limitations:
//      1.  The encoding="xxx" clause in the XML header should reflect
//          the system local code page, but does not.
//      2.  Cases where the XML data contains characters that can not
//          be represented in the system local code page are not handled.
//
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/impl/DOMWriterImpl.hpp>
#include <xercesc/dom/impl/DOMDocumentTypeImpl.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>

#include "DOMTreeErrorReporter.hpp"
#include "DOMPrintFilter.hpp"

#include <string.h>
#include <stdlib.h>

// ---------------------------------------------------------------------------
//  Local data
//
//  gXmlFile
//      The path to the file to parser. Set via command line.
//
//  gDoNamespaces
//      Indicates whether namespace processing should be done.
//
//  gDoSchema
//      Indicates whether schema processing should be done.
//
//  gSchemaFullChecking
//      Indicates whether full schema constraint checking should be done.
//
//  gDoCreate
//      Indicates whether entity reference nodes needs to be created or not
//      Defaults to false
//
//  gEncodingName
//      The encoding we are to output in. If not set on the command line,
//      then it is defaults to the encoding of the input XML file.
//
//  gEndOfLineSequence
//      The end of line sequence we are to output. 
//
//  gValScheme
//      Indicates what validation scheme to use. It defaults to 'auto', but
//      can be set via the -v= command.
//
// ---------------------------------------------------------------------------
static char*                    gXmlFile               = 0;
static bool                     gDoNamespaces          = false;
static bool                     gDoSchema              = false;
static bool                     gSchemaFullChecking    = false;
static bool                     gDoCreate              = true;

// options for DOMWriter's features
static const XMLCh*             gEncodingName          = 0;
static const XMLCh*             gEndOfLineSequence     = 0;

static bool                     gCanonicalForm               = false;
static bool                     gFormatPrettyPrint           = false;
static bool                     gNormalizeCharacters         = false;
static bool                     gValidation                  = false;
static bool                     gSplitCdataSections          = true;
static bool                     gDiscardDefaultContent       = true;
static bool                     gEntityReferenceExpansion    = true;
static bool                     gWhitespaceInElementContent  = true;

static bool                     gVerifyResult                = false;
static bool                     gUseFilter                   = false;
static DOMPrintFilter          *gFilter = 0;

static XercesDOMParser::ValSchemes    gValScheme       = XercesDOMParser::Val_Auto;

// ---------------------------------------------------------------------------
//
//  Usage()
//
// ---------------------------------------------------------------------------
void usage()
{
    cout << "\nUsage:\n"
            "    DOMPrint [options] <XML file>\n\n"
            "This program invokes the DOM parser, and builds the DOM tree.\n"
            "It then asks the DOMWriter to serialize the DOM tree \n"
            "Options:\n"
            "    -e          create entity reference nodes. Default is no expansion.\n"
            "    -v=xxx      Validation scheme [always | never | auto*].\n"
            "    -n          Enable namespace processing. Default is off.\n"
            "    -s          Enable schema processing. Default is off.\n"
            "    -f          Enable full schema constraint checking. Defaults is off.\n"
            "    -weol=xxx   Set the character sequence as end of line.\n"
            "    -wenc=XXX   Use a particular encoding for output. Default is\n"
            "                the same encoding as the input XML file. UTF-8 if\n"
            "                input XML file has not XML declaration.\n"
            "    -wcnf       canonical-form.                default off \n"
            "    -wfpp       format-pretty-print.           default off \n"
            "    -wnch       normalize-characters.          default off \n"
            "    -wval       validation.                    default off \n"
            "    -wscs       split-cdata-sections.          default on  \n"
			"    -wddc       discard-default-content.       default on  \n"
            "    -went       entity-reference-expansion.    default on  \n"
            "    -wwec       whitespace-in-element-content. default on  \n"
			"    -wfilter    use DOMPrintFilter.            default off \n"
			"    -wverify    verify the result from writeToString(). default off \n"
            "    -?          Show this help.\n\n"
            "  * = Default if not provided explicitly.\n\n"
            "The parser has intrinsic support for the following encodings:\n"
            "    UTF-8, USASCII, ISO8859-1, UTF-16[BL]E, UCS-4[BL]E,\n"
            "    WINDOWS-1252, IBM1140, IBM037.\n"
          <<  endl;
}

void setFeature(DOMWriter* ptr, const XMLCh* const featName, bool toState)
{
	try
	{
		ptr->setFeature(featName, toState);
	}
	catch(const DOMException&)
	{
		//absorb it here
	}

}
// ---------------------------------------------------------------------------
//
//  main
//
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{
    int retval = 0;

    // Initialize the XML4C2 system
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch(const XMLException &toCatch)
    {
        cerr << "Error during Xerces-c Initialization.\n"
             << "  Exception message:"
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

    // See if non validating dom parser configuration is requested.
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
                gValScheme = XercesDOMParser::Val_Never;
            else if (!strcmp(parm, "auto"))
                gValScheme = XercesDOMParser::Val_Auto;
            else if (!strcmp(parm, "always"))
                gValScheme = XercesDOMParser::Val_Always;
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
            gDoNamespaces = true;
        }
         else if (!strcmp(argV[parmInd], "-s")
              ||  !strcmp(argV[parmInd], "-S"))
        {
            gDoSchema = true;
        }
         else if (!strcmp(argV[parmInd], "-f")
              ||  !strcmp(argV[parmInd], "-F"))
        {
            gSchemaFullChecking = true;
        }
         else if (!strcmp(argV[parmInd], "-e")
              ||  !strcmp(argV[parmInd], "-E"))
        {
            gDoCreate = true;
        }
         else if (!strncmp(argV[parmInd], "-wenc=", 6))
        {
             // Get out the encoding name
             gEncodingName = XMLString::transcode( &(argV[parmInd][6]) );
        }			
         else if (!strncmp(argV[parmInd], "-weol=", 6))
        {
             // Get out the end of line
             gEndOfLineSequence = XMLString::transcode( &(argV[parmInd][6]) );
        }			
		 else if (!strcmp(argV[parmInd], "-wcnf"))
        {
            gCanonicalForm = !gCanonicalForm;
        }
         else if (!strcmp(argV[parmInd], "-wddc"))
        {
            gDiscardDefaultContent = !gDiscardDefaultContent;
        }
         else if (!strcmp(argV[parmInd], "-went"))
        {
            gEntityReferenceExpansion = !gEntityReferenceExpansion;
        }
         else if (!strcmp(argV[parmInd], "-wfpp"))
        {
            gFormatPrettyPrint = !gFormatPrettyPrint;
        }
         else if (!strcmp(argV[parmInd], "-wnch"))
        {
            gNormalizeCharacters = !gNormalizeCharacters;
        }
         else if (!strcmp(argV[parmInd], "-wscs"))
        {
            gSplitCdataSections = !gSplitCdataSections;
        }
         else if (!strcmp(argV[parmInd], "-wval"))
        {
            gValidation = !gValidation;
        }
         else if (!strcmp(argV[parmInd], "-wwec"))
        {
            gWhitespaceInElementContent = !gWhitespaceInElementContent;
        }
         else if (!strcmp(argV[parmInd], "-wverify"))
        {
            gVerifyResult = !gVerifyResult;
        }
         else if (!strcmp(argV[parmInd], "-wfilter"))
        {
            gUseFilter = !gUseFilter;
        }
         else
        {
            cerr << "Unknown option '" << argV[parmInd]
                 << "', ignoring it.\n" << endl;
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
    gXmlFile = argV[parmInd];

    //
    //  Create our parser, then attach an error handler to the parser.
    //  The parser will call back to methods of the ErrorHandler if it
    //  discovers errors during the course of parsing the XML document.
    //
    XercesDOMParser *parser = new XercesDOMParser;
    parser->setValidationScheme(gValScheme);
    parser->setDoNamespaces(gDoNamespaces);
    parser->setDoSchema(gDoSchema);
    parser->setValidationSchemaFullChecking(gSchemaFullChecking);
    parser->setCreateEntityReferenceNodes(gDoCreate);

    DOMTreeErrorReporter *errReporter = new DOMTreeErrorReporter();
    parser->setErrorHandler(errReporter);

    //
    //  Parse the XML file, catching any XML exceptions that might propogate
    //  out of it.
    //
    bool errorsOccured = false;
    try
    {
        parser->parse(gXmlFile);
    }

    catch (const XMLException& e)
    {
        cerr << "An error occurred during parsing\n   Message: "
             << StrX(e.getMessage()) << endl;
        errorsOccured = true;
    }

    catch (const DOMException& e)
    {
       cerr << "A DOM error occurred during parsing\n   DOMException code: "
             << e.code << endl;
        errorsOccured = true;
    }

    catch (...)
    {
        cerr << "An error occurred during parsing\n " << endl;
        errorsOccured = true;
    }

    // If the parse was successful, output the document data from the DOM tree
    if (!errorsOccured && !errReporter->getSawErrors())
    {
        DOMNode        *doc = parser->getDocument();
		DOMWriter      *theSerializer = new DOMWriterImpl();

        try
        {
			theSerializer->setNewLine(gEndOfLineSequence);
			theSerializer->setEncoding(gEncodingName);

			setFeature(theSerializer, DOMWriter::CanonicalForm,              gCanonicalForm);
			setFeature(theSerializer, DOMWriter::FormatPrettyPrint,          gFormatPrettyPrint);
			setFeature(theSerializer, DOMWriter::NormalizeCharacters,        gNormalizeCharacters);
			setFeature(theSerializer, DOMWriter::Validation,                 gValidation);
			setFeature(theSerializer, DOMWriter::SplitCdataSections,         gSplitCdataSections);
			setFeature(theSerializer, DOMWriter::DiscardDefaultContent,      gDiscardDefaultContent);
			setFeature(theSerializer, DOMWriter::Entities,                   gEntityReferenceExpansion);
			setFeature(theSerializer, DOMWriter::WhitespaceInElementContent, gWhitespaceInElementContent);

			if (gUseFilter)
			{
				gFilter = new DOMPrintFilter(); 
				theSerializer->setFilter(gFilter);
			}

			XMLCh* retString = theSerializer->writeToString(*doc);
			char *memString = XMLString::transcode(retString);		
			delete [] retString;      // release the memory allocated by writeToString()

            cout<<memString;
			cout<<flush;

			if (gVerifyResult)
			{
               /***
                  verify the output stream
				***/
				//MemBufInputSource* memBufIS = MemBufInputSource
				MemBufInputSource memBufIS((const XMLByte*)memString
                                         , strlen(memString)
                                         , "verifyResult"
										 , false);

				parser->reset();
				parser->parse(memBufIS);
				DOMNode *doc2 = parser->getDocument();
				StdOutFormatTarget formatTarget;
				theSerializer->writeNode(&formatTarget, *doc2);
			}
			
			delete [] memString; // release the memory from the transcoder

			if (gUseFilter)
				delete gFilter;

        }
        catch (XMLException& e)
        {
            cerr << "An error occurred during creation of output transcoder. Msg is:"
                << endl
                << StrX(e.getMessage()) << endl;
            retval = 4;
        }

		delete theSerializer;
    }
    else
        retval = 4;

    //
    //  Clean up the error handler. The parser does not adopt handlers
    //  since they could be many objects or one object installed for multiple
    //  handlers.
    //
    delete errReporter;

    //
    //  Delete the parser itself.  Must be done prior to calling Terminate, below.
    //
    delete parser;

    // And call the termination method
    XMLPlatformUtils::Terminate();

	delete (void *)gEncodingName;        // const problems.
	delete (void *)gEndOfLineSequence;   // const problems.

    return retval;
}









