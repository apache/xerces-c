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
 * Revision 1.1  1999/11/09 01:02:05  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:42:28  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

// ---------------------------------------------------------------------------
//  XML4C2 includes
// ---------------------------------------------------------------------------
#include <util/RuntimeException.hpp>
#include <util/URL.hpp>
#include "CoreTests.hpp"


// ---------------------------------------------------------------------------
//  Local test methods
// ---------------------------------------------------------------------------
static bool parseTests()
{
    URL testURL;

    //
    //  First lets set some invalid URL's and make sure that we get the
    //  expected malformed URL exceptions. Use an outer generic catch to
    //  handle anything unexpected and report it.
    //
    try
    {
        static const char* badURLS[] = 
        {
            "myfile.txt"
            , "bubba:///myfile.txt"
            , "file://myfile.txt"
            , 0
        };

        unsigned int index = 0;
        while (badURLS[index])
        {
            bool gotIt = false;
            try
            {
                testURL.setURL(badURLS[index]);
            }

            catch(const MalformedURLException&)
            {
                gotIt = true;
            }

            if (!gotIt)
            {
                outStrm << "Failed to get bad URL exception for: "
                        << badURLS[index] << EndLn;
            }
            index++;
        }
    }

    catch(const XMLException& toCatch)
    {
        outStrm << "Got an unexpected exception.\n  Type:"
                << toCatch.getType() << ", Message:" << toCatch.getMessage()
                << EndLn;
    }

    catch(...)
    {
        outStrm << "Got an unexpected system exception." << EndLn;
    }


    //
    //  Next lets test that we get runtime exceptions for unsupported
    //  protocols.
    //
    try
    {
        static const char* unsupportedURLS[] = 
        {
            "http:///xyz"
            , "gopher:///myfile.txt"
            , "wais:///myfile.txt"
            , "ftp://host/filename.txt"
            , "mailto://goober.74/snobby"
            , 0
        };

        unsigned int index = 0;
        while (unsupportedURLS[index])
        {
            bool gotIt = false;
            try
            {
                testURL.setURL(unsupportedURLS[index]);
            }

            catch(const RuntimeException&)
            {
                gotIt = true;
            }

            if (!gotIt)
            {
                outStrm << "Failed to get runtime exception for: "
                        << unsupportedURLS[index] << EndLn;
            }
            index++;
        }
    }

    catch(const XMLException& toCatch)
    {
        outStrm << "Got an unexpected exception.\n  Type:"
                << toCatch.getType() << ", Message:" << toCatch.getMessage()
                << EndLn;
    }

    catch(...)
    {
        outStrm << "Got an unexpected system exception." << EndLn;
    }


    return true;
}


static bool basicTests()
{
    //
    //  Set up a list of URLs and what we expect to get out of them for
    //  the various components.
    //
    struct TestInfo
    {
        const char*     urlText;
        const char*     hostComp;
        const char*     pathComp;
        const char*     protocol;
    };

    static const TestInfo testList[] =
    {
            { "file:///A:\\abc.txt"         , ""            , "A:\\abc.txt"     , "file://" }
        ,   { "file://localhost/A:\\abc.txt", "localhost"   , "A:\\abc.txt"     , "file://" }
        ,   { "file:///abc.txt"             , ""            , "/abc.txt"        , "file://" }
        ,   { "file:///A:\\a%20c.txt"       , ""            , "A:\\a c.txt"     , "file://" }
        ,   { 0, 0, 0, 0 }
    };

    bool result = true;
    unsigned int index = 0;
    while (true)
    {
        // Break out at the end
        if (!testList[index].urlText)
            break;

        // Parse the next URL in the list
        URL testURL;
        testURL.setURL(testList[index].urlText);

        //
        //  And test the various components. We have to transcode them in order
        //  to do the the comparison.
        //
        XMLCh* tmpStr = XMLString::transcode(testList[index].hostComp);
        if (XMLString::compareString(testURL.getHost(), tmpStr))
        {
            outStrm << "Host incorrect on URL: " << testList[index].urlText << EndLn;
            result = false;
        }
        delete [] tmpStr;

        tmpStr = XMLString::transcode(testList[index].protocol);
        if (XMLString::compareString(testURL.getProtocol(), tmpStr))
        {
            outStrm << "Protocol incorrect on URL: " << testList[index].urlText << EndLn;
            result = false;
        }
        delete [] tmpStr;

        tmpStr = XMLString::transcode(testList[index].pathComp);
        if (XMLString::compareString(testURL.getPath(), tmpStr))
        {
            outStrm << "Path incorrect on URL: " << testList[index].urlText << EndLn;
            result = false;
        }
        delete [] tmpStr;

        // Move up to the next test
        index++;
    }
    return result;
}



// ---------------------------------------------------------------------------
//  Test entry point
// ---------------------------------------------------------------------------
bool testURL()
{
    outStrm << "----------------------------------\n"
            << "Testing URL class \n"
            << "----------------------------------"
            << EndLn;

    bool retVal = true;
    try
    {
        // Test the URL parsing code
        outStrm << "Testing URL Parsing" << EndLn;
        if (!parseTests())
        {
            outStrm << "URL parsing tests failed\n";
            retVal = false;
        }
         else
        {
            outStrm << "URL parsing tests passed\n";
        }
        outStrm << EndLn;

        // Now test basic functionality
        outStrm << "Testing URL Basics" << EndLn;
        if (!basicTests())
        {
            outStrm << "URL basic tests failed\n";
            retVal = false;
        }
         else
        {
            outStrm << "URL basic tests passed\n";
        }
        outStrm << EndLn;
    }

    catch(const XMLException& toCatch)
    {
        outStrm << "  ERROR: Unexpected exception!\n   Msg: "
                << toCatch.getMessage() << EndLn;
        return false;
    }
    return retVal;
}
