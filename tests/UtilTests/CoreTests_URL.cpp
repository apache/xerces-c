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
 * Revision 1.4  2000/01/19 00:59:06  roddey
 * Get rid of dependence on old utils output streams.
 *
 * Revision 1.3  2000/01/12 00:17:48  roddey
 * Removed validator tests temporarily, since they have changed and the tests need
 * to be rewritten. Added new tests for the new URL class.
 *
 * Revision 1.2  1999/12/07 23:11:01  roddey
 * Add in some new tests for transcoders and update the URL tests
 * a bit.
 *
 * Revision 1.1.1.1  1999/11/09 01:02:05  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:42:28  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

// ---------------------------------------------------------------------------
//  XML4C2 includes
// ---------------------------------------------------------------------------
#include <util/RuntimeException.hpp>
#include <util/XMLURL.hpp>
#include "CoreTests.hpp"



// ---------------------------------------------------------------------------
//  Local testing functions
// ---------------------------------------------------------------------------

//
//  This test just sets up a list of variations on URLs and parses them. The
//  URL list includes the expected text for each of the parts that should be
//  parsed out of the URL.
//
struct BasicTestEntry
{
    const XMLCh*        orgURL;
    const XMLCh*        fullText;
    XMLURL::Protocols   Protocol;
    const XMLCh*        Fragment;
    const XMLCh*        Host;
    const XMLCh*        Path;
    const XMLCh*        Password;
    const XMLCh*        Query;
    const XMLCh*        User;
};

static bool checkAField(const XMLCh* const test, const XMLCh* const expected)
{
    if (!test && !expected)
        return true;

    if ((!test || !expected) || XMLString::compareString(test, expected))
    {
        std::wcout  << L"Expected: " << expected << L", but got: " << test
                    << std::endl;
        return false;
    }
    return true;
}

static bool checkBasicResult(const  XMLURL&         testURL
                            , const BasicTestEntry& testInfo)
{
    //
    //  Check each part to insure that its what its supposed to be. Since
    //  any of them can be a null pointer, we have a little helper function
    //  that spits out the actual testing code for each one.
    //
    if (!checkAField(testURL.getURLText(), testInfo.fullText))
        return false;

    if (!checkAField(testURL.getFragment(), testInfo.Fragment))
        return false;

    if (!checkAField(testURL.getHost(), testInfo.Host))
        return false;

    if (!checkAField(testURL.getPath(), testInfo.Path))
        return false;

    if (!checkAField(testURL.getPassword(), testInfo.Password))
        return false;

    if (!checkAField(testURL.getQuery(), testInfo.Query))
        return false;

    if (!checkAField(testURL.getUser(), testInfo.User))
        return false;

    return true;
}

static bool basicURLTest()
{
    static BasicTestEntry testList[] =
    {
        {
            L"file://host@user:password/path1/path2/file.txt?query#fragment"
            , L"file://host@user:password/path1/path2/file.txt?query#fragment"
            , XMLURL::File
            , L"fragment"
            , L"host"
            , L"/path1/path2/file.txt"
            , L"password"
            , L"query"
            , L"user"
        }
      , {
            L"file:///path2/file.txt?query#fragment"
            , L"file:///path2/file.txt?query#fragment"
            , XMLURL::File
            , L"fragment"
            , 0
            , L"/path2/file.txt"
            , 0
            , L"query"
            , 0
        }
      , {
            L"#fragment"
            , L"#fragment"
            , XMLURL::Unknown
            , L"fragment"
            , 0
            , 0
            , 0
            , 0
            , 0
        }
      , {
            L"file://host@user/path1/path2/file.txt#fragment"
            , L"file://host@user/path1/path2/file.txt#fragment"
            , XMLURL::File
            , L"fragment"
            , L"host"
            , L"/path1/path2/file.txt"
            , 0
            , 0
            , L"user"
        }
    };
    const unsigned int testCount = sizeof(testList) / sizeof(testList[0]);

    bool retVal = true;

    //
    //  Do a run where we construct the URL over and over for each
    //  test.
    //
    unsigned int index;
    for (index = 0; index < testCount; index++)
    {
        // Force full destruction each time
        {
            XMLURL testURL(testList[index].orgURL);

            // Call the comparison function
            if (!checkBasicResult(testURL, testList[index]))
                retVal = false;
        }
    }

    //
    //  Do a run where we use a single URL object and just reset it over
    //  and over again.
    //
    XMLURL testURL;
    for (index = 0; index < testCount; index++)
    {
        testURL.setURL(testList[index].orgURL);

        // Call the comparison function
        if (!checkBasicResult(testURL, testList[index]))
            retVal = false;
    }

    return retVal;
}


//
//  This test makes sure that parsing one URL relative to another works
//  correctly. The tests used here come from one of the internet RFCs on
//  generic URI syntax. A single base URL is created, then a number of
//  relative URLs are parsed against it and the results compared to the
//  expected result.
//
static bool relativeURLTest()
{
    static struct TestEntry
    {
        const XMLCh*    relative;
        const XMLCh*    result;
    } testList[] =
    {
        { L"g"      , L"http://a/b/c/g" }
      , { L"./g"    , L"http://a/b/c/g" }
      , { L"g/"     , L"http://a/b/c/g/" }
      , { L"/g"     , L"http://a/g" }
      , { L"?y"     , L"http://a/b/c/?y" }
      , { L"g?y"    , L"http://a/b/c/g?y" }
      , { L"#s"     , L"http://a/b/c/d;p#s" }
      , { L"g#s"    , L"http://a/b/c/g#s" }
      , { L"g?y#s"  , L"http://a/b/c/g?y#s" }
      , { L";x"     , L"http://a/b/c/;x" }
      , { L"g;x"    , L"http://a/b/c/g;x" }
      , { L"g;x?y#s", L"http://a/b/c/g;x?y#s" }
      , { L"."      , L"http://a/b/c/" }
      , { L"./"     , L"http://a/b/c/" }
      , { L".."     , L"http://a/b/" }
      , { L"../"    , L"http://a/b/" }
      , { L"../g"   , L"http://a/b/g" }
      , { L"../.."  , L"http://a/" }
      , { L"../../" , L"http://a/" }
      , { L"../../g", L"http://a/g" }
    };
    const unsigned int testCount = sizeof(testList) / sizeof(testList[0]);

    // This is the base URL against which the tests are run
    XMLURL baseURL(L"http://a/b/c/d;p?q");

    bool retVal = true;
    for (unsigned int index = 0; index < testCount; index++)
    {
        XMLURL testURL(baseURL, testList[index].relative);

        if (XMLString::compareString(testURL.getURLText(), testList[index].result))
        {
            std::wcout  << L"Expected URL: " << testList[index].result
                        << L" but got: " << testURL.getURLText() << std::endl;
            retVal = false;
        }
    }
    return retVal;
};


// ---------------------------------------------------------------------------
//  Test entry point
// ---------------------------------------------------------------------------
bool testURL()
{
    std::wcout  << L"----------------------------------\n"
                << L"Testing URL class \n"
                << L"----------------------------------"
                << std::endl;

    bool retVal = true;
    try
    {
        // Call other local methods to do specific tests
        std::wcout << L"Testing basic URL parsing" << std::endl;
        if (!basicURLTest())
        {
            std::wcout << L"Basic URL parsing tests failed" << std::endl;
            retVal = false;
        }
         else
        {
            std::wcout << L"Basic URL parsing tests passed" << std::endl;
        }
        std::wcout << std::endl;

        std::wcout << L"Testing relative URL parsing" << std::endl;
        if (!relativeURLTest())
        {
            std::wcout << L"Relative URL parsing tests failed" << std::endl;
            retVal = false;
        }
         else
        {
            std::wcout << L"Relative URL parsing tests passed" << std::endl;
        }
        std::wcout << std::endl;
    }

    catch(const XMLException& toCatch)
    {
        std::wcout  << L"  ERROR: Unexpected exception!\n   Msg: "
                    << toCatch.getMessage() << std::endl;
        return false;
    }
    return retVal;
}
