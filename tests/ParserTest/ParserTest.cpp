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
 * Revision 1.10  2001/11/28 21:15:08  tng
 * Fix broken ParserTest.
 *
 * Revision 1.9  2000/06/22 04:31:27  roddey
 * Another do nothing change to make sure I can still commit
 * from home after changing my password.
 *
 * Revision 1.7  2000/03/02 19:55:46  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.6  2000/02/06 07:48:36  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.5  2000/01/26 19:35:57  roddey
 * When the /Debug output format is used, it will spit out source offset
 * data as well now.
 *
 * Revision 1.4  2000/01/21 23:58:06  roddey
 * Initial move away from util streams was bad. Wide char APIs didn't allow enough
 * control to do canonical output, so changed to use std short char APIs.
 *
 * Revision 1.2  2000/01/12 00:29:49  roddey
 * Changes for the new URL and InputSource changes.
 *
 * Revision 1.1.1.1  1999/11/09 01:02:14  twl
 * Initial checkin
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
#include    <util/XMLURL.hpp>
#include    <internal/XMLScanner.hpp>
#include    <validators/DTD/DTDValidator.hpp>
#include    "ParserTest.hpp"


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
        cout << "Error during platform init! Message:\n"
             << StrX(toCatch.getMessage()) << endl;
        return 1;
    }

    //
    //  Create our test parser object. This object implements the internal
    //  event APIs and is plugged into the scanner.
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
            cout << "Unknown parameter: " << argV[index] << endl;
    }

    // We have to have a URL to work on
    if (!urlPath)
    {
        cout << "A URL must be provided, /URL=xxxx" << endl;
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
    validator = dtdVal;

    // And now create the scanner and give it all the handlers
    XMLScanner scanner
    (
        &parserTest
        , &parserTest
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
        scanner.scanDocument(urlPath);
    }

    catch(const XMLException& toCatch)
    {
        cout << "Exception during scan:\n    "
             << StrX(toCatch.getMessage())
             << endl;
    }

    // And call the termination method
    XMLPlatformUtils::Terminate();

    return 0;
}



// ---------------------------------------------------------------------------
//  StrX: Private helper methods
// ---------------------------------------------------------------------------
void StrX::transcode(const XMLCh* const toTranscode, const unsigned int len)
{
    // Short circuit if its a null pointer
    if (!toTranscode || (!toTranscode[0]))
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
