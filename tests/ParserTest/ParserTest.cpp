/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
 * $Log$
 * Revision 1.1  1999/11/09 01:02:14  twl
 * Initial revision
 *
 * Revision 1.3  1999/11/08 20:42:25  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include    <util/PlatformUtils.hpp>
#include    <util/XMLString.hpp>
#include    <util/URL.hpp>
#include    <internal/URLInputSource.hpp>
#include    <internal/XMLScanner.hpp>
#include    <validators/DTD/DTDValidator.hpp>
#include    "ParserTest.hpp"


// ---------------------------------------------------------------------------
//  Global data
//
//  errStrm
//  outStrm
//      The streams we use to output our test data and error info. These are
//      simple classes used just for XML4C2 samples and debug. They are not
//      sufficient for real applications, nor are they supported for
//      production code use. They merely provide a simple and portable means
//      to output the Unicode data from the parser on the local host.
// ---------------------------------------------------------------------------
XMLStdErr   errStrm;
XMLStdOut   outStrm;


// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int main(int argC, char** argV)
{
    // Init the XML platform
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch(const XMLException& toCatch)
    {
        errStrm << "Error during platform init! Message:\n"
                << toCatch.getMessage() << EndLn;
        return 1;
    }

    //
    //  Create our test parser object. This guy implements the internal event
    //  APIs and is plugged into the scanner.
    //
    TestParser parserTest;

    // Figure out the parameters
    bool doValidation = false;
    bool doNamespaces = false;
    bool keepGoing = false;
    XMLCh*  urlPath = 0;
    for (int index = 1; index < argC; index++)
    {
        if (!XMLString::compareIString(argV[index], "/Debug"))
            parserTest.setOutputType(OutputType_Debug);
        else if (!XMLString::compareIString(argV[index], "/Validate"))
            doValidation = true;
        else if (!XMLString::compareIString(argV[index], "/Namespaces"))
        {
            doNamespaces = true;
            parserTest.setDoNamespaces(true);
        }
        else if (!XMLString::compareIString(argV[index], "/XML"))
            parserTest.setOutputType(OutputType_XML);
        else if (!XMLString::compareIString(argV[index], "/IntDTD"))
            parserTest.setShowIntDTD(true);
        else if (!XMLString::compareIString(argV[index], "/ShowWarnings"))
            parserTest.setShowWarnings(true);
        else if (!XMLString::compareIString(argV[index], "/ShowErrLoc"))
            parserTest.setShowErrLoc(true);
        else if (!XMLString::compareIString(argV[index], "/JCCanon"))
            parserTest.setOutputType(OutputType_JCCanon);
        else if (!XMLString::compareIString(argV[index], "/SunCanon"))
            parserTest.setOutputType(OutputType_SunCanon);
        else if (!XMLString::compareIString(argV[index], "/KeepGoing"))
            keepGoing = true;
        else if (!XMLString::compareNIString(argV[index], "/URL=", 5))
            urlPath = XMLString::transcode(&argV[index][5]);
        else
            errStrm << "Unknown parameter: " << argV[index] << EndLn;
    }

    // We have to have a URL to work on
    if (!urlPath)
    {
        errStrm << "A URL must be provided, /URL=xxxx" << EndLn;
        return 1;
    }

    //
    //  Create a validator of the correct type so that we can install it
    //  on the scanner.
    //
    //  <TBD> Later, when Schema validators exist, we'll have a parameter
    //  to select one or the other
    //
    XMLValidator* validator = 0;
    DTDValidator* dtdVal = new DTDValidator(&parserTest);
    dtdVal->setDocTypeHandler(&parserTest);
    validator = dtdVal;

    // And now create the scanner and give it all the handlers
    XMLScanner scanner
    (
        &parserTest
        , 0
        , &parserTest
        , validator
    );

    // Set the scanner flags that we were told to
    scanner.setDoValidation(doValidation);
    scanner.setDoNamespaces(doNamespaces);
    scanner.setExitOnFirstFatal(!keepGoing);

    // Tell the parser about the scanner
    parserTest.setScanner(&scanner);

    try
    {
        URLInputSource src(urlPath);
        scanner.scanDocument(src);
    }

    catch(const XMLException& toCatch)
    {
        outStrm << "Exception during scan:\n    "
                << toCatch.getMessage()
                << EndLn;
    }

    return 0;
}
