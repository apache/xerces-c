/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id$
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "CoreTests.hpp"
#include <xercesc/util/PlatformUtils.hpp>



// ---------------------------------------------------------------------------
//  Externs for our testing methods. There is just one per file so a header
//  for each one is a little overkill, so we just do it the old fashioned way.
// ---------------------------------------------------------------------------
extern bool testTranscoders();
extern bool testCountedPointer();
extern bool testBitSet();
extern bool testRefArray();
extern bool testRefHashTable();
extern bool testRefStack();
extern bool testRefVector();
extern bool testString();
extern bool testURL();
extern bool testValueArray();
extern bool testValueStack();
extern bool testValueVector();


int main()
{
    // Do the platform initialization
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch(const XMLException& toCatch)
    {
        std::wcout << L"Parser Init Failed!\n   INFO: ("
                   << toCatch.getSrcFile() << L"." << toCatch.getSrcLine()
                   << L") -" << toCatch.getMessage() << std::endl;
        return 0xFFFF;
    }

    std::wcout << L"\nXML4C2 Core Utilities Unit Tester\n" << std::endl;

    // This value will return the number of failed tests
    int retVal = 0;


    try
    {
        // -------------------------------------------------------------------
        // Test the basic transcoding services
        // -------------------------------------------------------------------
        if (!testTranscoders())
        {
            std::wcout << L"Transcoder tests failed" << std::endl;
            retVal++;
        }
        std::wcout << std::endl;


        // -------------------------------------------------------------------
        // Test the String class
        // -------------------------------------------------------------------
        if (!testString())
        {
            std::wcout << L"String tests failed" << std::endl;
            retVal++;
        }
        std::wcout << std::endl;


        // -------------------------------------------------------------------
        // Test the CountedPointerTo template class
        // -------------------------------------------------------------------
        if (!testCountedPointer())
        {
            std::wcout << L"CountedPointerTo tests failed" << std::endl;
            retVal++;
        }
        std::wcout << std::endl;


        // -------------------------------------------------------------------
        // Test the URL class
        // -------------------------------------------------------------------
        if (!testURL())
        {
            std::wcout << L"URL tests failed" << std::endl;
            retVal++;
        }
        std::wcout << std::endl;


        // -------------------------------------------------------------------
        // Test the ValueVectorOf template class
        // -------------------------------------------------------------------
        if (!testValueVector())
        {
            std::wcout << L"ValueVectorOf tests failed" << std::endl;
            retVal++;
        }
        std::wcout << std::endl;


        // -------------------------------------------------------------------
        // Test the ValueArrayOf template class
        // -------------------------------------------------------------------
        if (!testValueArray())
        {
            std::wcout << L"ValueArrayOf tests failed" << std::endl;
            retVal++;
        }
        std::wcout << std::endl;


        // -------------------------------------------------------------------
        // Test the ValueStackOf template class
        // -------------------------------------------------------------------
        if (!testValueStack())
        {
            std::wcout << L"ValueStackOf tests failed" << std::endl;
            retVal++;
        }
        std::wcout << std::endl;


        // -------------------------------------------------------------------
        // Test the RefArrayOf template class
        // -------------------------------------------------------------------
        if (!testRefArray())
        {
            std::wcout << L"RefArrayOf tests failed" << std::endl;
            retVal++;
        }
        std::wcout << std::endl;


        // -------------------------------------------------------------------
        // Test the RefStackOf template class
        // -------------------------------------------------------------------
        if (!testRefStack())
        {
            std::wcout << L"RefStackOf tests failed" << std::endl;
            retVal++;
        }
        std::wcout << std::endl;


        // -------------------------------------------------------------------
        // Test the RefVectorOf template class
        // -------------------------------------------------------------------
        if (!testRefVector())
        {
            std::wcout << L"RefVectorOf tests failed" << std::endl;
            retVal++;
        }
        std::wcout << std::endl;


        // -------------------------------------------------------------------
        // Test the RefHashtableOf template class
        // -------------------------------------------------------------------
        if (!testRefHashTable())
        {
            std::wcout << L"RefHashTableOf tests failed" << std::endl;
            retVal++;
        }
        std::wcout << std::endl;


        // -------------------------------------------------------------------
        // Test the BitSet class
        // -------------------------------------------------------------------
        if (!testBitSet())
        {
            std::wcout << L"BitSet tests failed" << std::endl;
            retVal++;
        }
        std::wcout << std::endl;
    }

    catch(const XMLException& toCatch)
    {
        std::wcout  << L"Exception During Test!\n   INFO: ("
                    << toCatch.getSrcFile() << L"."
                    << toCatch.getSrcLine() << L") -"
                    << toCatch.getMessage() << std::endl;
        return 0xFFFF;
    }

    // If we failed any tests, display a message
    std::wcout << L"--------------------------------\n";
    if (retVal == 0)
        std::wcout << L"<<PASSED>>: All tests passed\n";
    else
        std::wcout << L"<<FAILED>>: Some tests failed\n";
    std::wcout << L"--------------------------------\n" << std::endl;

    return retVal;
}
