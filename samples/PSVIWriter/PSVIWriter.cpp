/*
* The Apache Software License, Version 1.1
*
* Copyright (c) 2003 The Apache Software Foundation.  All rights
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

//REVISIT
/*
 * $Log$
 * Revision 1.1  2004/02/04 20:11:54  peiyongz
 * PSVIWriter
 *
 *
 * $Id: PSVIWriter
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "PSVIWriter.hpp"
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <fstream.h>

// ---------------------------------------------------------------------------
//  Local helper methods
// ---------------------------------------------------------------------------
void usage()
{
	XERCES_STD_QUALIFIER cout << "\nUsage:\n"
			"    PSVIWriter [options] <XML file | List file>\n\n"
			"This program invokes the SAX2XMLReaderImpl, and then exposes the\n"
			"underlying PSVI of each parsed XML file, using SAX2 API.\n\n"
			"Options:\n"
			"    -f          Enable full schema constraint checking processing. Defaults to off.\n"
			"    -o=xxx      Output PSVI to file xxx (default is stdout)\n"
			"    -e=xxx      Output errors to file xxx (default is stdout)\n"
			"    -u=xxx      Handle unrepresentable chars [fail | rep | ref*].\n"
			"    -x=XXX      Use a particular encoding for output (UTF8*).\n"
			"    -?          Show this help.\n\n"
			"  * = Default if not provided explicitly.\n"
		<< endl;
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

	static const char*				encodingName		= "UTF8";
	static XMLFormatter::UnRepFlags unRepFlags			= XMLFormatter::UnRep_CharRef;
    const char*						xmlFile				= 0;
    SAX2XMLReader::ValSchemes		valScheme			= SAX2XMLReader::Val_Auto;
    bool							doList				= false; //REVISIT
    bool							schemaFullChecking	= false;
    bool							errorOccurred		= false;
    const char*						psviOut				= 0;
    const char*						errorOut			= 0;
    const char*						output				= 0;
    XMLFormatTarget* 				psviTarget 			= 0;
    XMLFormatTarget* 				errorTarget 		= 0;
    XMLFormatter* 					psviFormatter 		= 0;
    XMLFormatter* 					errorFormatter 		= 0;
	char							fileName[80]		="";
    

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
            return 2;
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
         else if (!strncmp(argV[argInd], "-o=", 3)
              ||  !strncmp(argV[argInd], "-O=", 3))
        {
            psviOut = &argV[argInd][3];
        }
         else if (!strncmp(argV[argInd], "-e=", 3)
              ||  !strncmp(argV[argInd], "-E=", 3))
        {
            errorOut = &argV[argInd][3];
        }
         else if (!strncmp(argV[argInd], "-x=", 3)
              ||  !strncmp(argV[argInd], "-X=", 3))
        {
            // Get out the encoding name
            encodingName = &argV[argInd][3];
        }
         else if (!strncmp(argV[argInd], "-u=", 3)
              ||  !strncmp(argV[argInd], "-U=", 3))
        {
            const char* const parm = &argV[argInd][3];

            if (!strcmp(parm, "fail"))
                unRepFlags = XMLFormatter::UnRep_Fail;
            else if (!strcmp(parm, "rep"))
                unRepFlags = XMLFormatter::UnRep_Replace;
            else if (!strcmp(parm, "ref"))
                unRepFlags = XMLFormatter::UnRep_CharRef;
            else
            {
                XERCES_STD_QUALIFIER cerr << "Unknown -u= value: " << parm << XERCES_STD_QUALIFIER endl;
                XMLPlatformUtils::Terminate();
                return 2;
            }
        }
        else
        {
            XERCES_STD_QUALIFIER cerr << "Unknown option '" << argV[argInd]
                << "', ignoring it\n" << endl;
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

    // Initialize the XML4C2 system
    try
    {   
        XMLPlatformUtils::Initialize();
    } catch (const XMLException& toCatch) {
        XERCES_STD_QUALIFIER cerr << "Error during initialization! Message:\n"
            << StrX(toCatch.getMessage()) << endl;
        return 1;
    }

    //
    //  Create a SAX parser object, then set it to validate or not.
    //
    SAX2XMLReaderImpl* parser = new SAX2XMLReaderImpl();
    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
    parser->setFeature(XMLUni::fgXercesSchema, true);
    parser->setFeature(XMLUni::fgXercesSchemaFullChecking, schemaFullChecking);
    parser->setFeature(XMLUni::fgSAX2CoreNameSpacePrefixes, false);
	parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
	parser->setFeature(XMLUni::fgXercesDynamic, true);
	
	//
	//	Based on commandline arguments, create XMLFormatters for PSVI output and errors
	//
	if (!doList) {
	    if (psviOut==0) {
	    	psviTarget = new StdOutFormatTarget();
	    } else {
	    	psviTarget = new LocalFileFormatTarget(psviOut);
	    }
		psviFormatter = new XMLFormatter(encodingName, psviTarget, XMLFormatter::NoEscapes, unRepFlags);
    }
    
    if (errorOut==0) {
    	errorTarget = new StdOutFormatTarget();
    } else {
    	errorTarget = new LocalFileFormatTarget(errorOut);
    }
	errorFormatter = new XMLFormatter(encodingName, errorTarget, XMLFormatter::NoEscapes, unRepFlags);

  	//
    //  Create our SAX handler object and install it as the handlers
    //

   	PSVIWriterHandlers* handler;
   	if (doList)
   		handler = new PSVIWriterHandlers(0, errorFormatter);
   	else
   		handler = new PSVIWriterHandlers(psviFormatter, errorFormatter);

	parser->setPSVIHandler(handler);
    parser->setContentHandler(handler);
    parser->setLexicalHandler(handler);
	parser->setXMLEntityResolver(handler);
    parser->setErrorHandler(handler);

    //
    //  Get the starting time and kick off the parse of the indicated
    //  file. Catch any exceptions that might propogate out of it.
    //
    unsigned long duration;

    bool more = true;
    ifstream fin;

    // the input is a list file
    if (doList) //REVISIT
        fin.open(argV[argInd]);

    if (fin.fail()) {
        XERCES_STD_QUALIFIER cerr <<"Cannot open the list file: " << argV[argInd] << endl;
        return 2;
    }

    while (more) //REVISIT
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
                    xmlFile =fURI;
                    XMLString::trim((char*)xmlFile);
                    XERCES_STD_QUALIFIER cerr << "==Parsing== \"" << xmlFile << "\"" << XERCES_STD_QUALIFIER endl;
                }
                
				if (psviOut==0) {
					if (psviTarget==0 && psviFormatter==0) {
			    		psviTarget = new StdOutFormatTarget();
						psviFormatter = new XMLFormatter(encodingName, psviTarget, XMLFormatter::NoEscapes, unRepFlags);
						handler->resetPSVIFormatter(psviFormatter);
			    	}
			    } else {
					strcpy(fileName, psviOut);
					if (strrchr(xmlFile, '\\')>strrchr(xmlFile, '/')) {
						strcat(fileName, strrchr(xmlFile, '\\'));
					} else {
						strcat(fileName, strrchr(xmlFile, '/'));
					}
	                if (psviFormatter)
	                	delete psviFormatter;
	                if (psviTarget)
	                	delete psviTarget;
			    	psviTarget = new LocalFileFormatTarget(fileName);
					psviFormatter = new XMLFormatter(encodingName, psviTarget, XMLFormatter::NoEscapes, unRepFlags);
					handler->resetPSVIFormatter(psviFormatter);
			    }
            }
            else
                break;
        }
        else {
            xmlFile = argV[argInd];
            more = false; //REVISIT
        }

        //reset error count first
        handler->resetErrors();

        try
        {
            const unsigned long startMillis = XMLPlatformUtils::getCurrentMillis();
            parser->parse(xmlFile);
            const unsigned long endMillis = XMLPlatformUtils::getCurrentMillis();
            duration = endMillis - startMillis;
        }

        catch (const XMLException& e)
        {
            XERCES_STD_QUALIFIER cerr << "\nError during parsing: '" << xmlFile << "'\n"
                << "Exception message is:  \n"
                << StrX(e.getMessage()) << "\n" << endl;
            errorOccurred = true;
            continue;
        }

        catch (...)
        {
            XERCES_STD_QUALIFIER cerr << "\nUnexpected exception during parsing: '" << xmlFile << "'\n";
            errorOccurred = true;
            continue;
        }
    }

    if (doList) //REVISIT
        fin.close();

    //
    //  Delete the parser itself.  Must be done prior to calling Terminate, below.
    //
    delete parser;
    delete handler;
    delete psviFormatter;
    delete errorFormatter;
    delete psviTarget;
    delete errorTarget;

    // And call the termination method
    XMLPlatformUtils::Terminate();

    if (errorOccurred)
        return 4;
    else
        return 0;

}

