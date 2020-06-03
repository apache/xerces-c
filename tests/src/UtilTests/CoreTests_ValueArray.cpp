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
//  XML4C2 includes
// ---------------------------------------------------------------------------
#include "CoreTests.hpp"
#include <xercesc/util/ValueArrayOf.hpp>
#include <xercesc/util/ArrayIndexOutOfBoundsException.hpp>


// ---------------------------------------------------------------------------
//  Force a full instantiation of our array and its enumerator, just to
//  insure that all methods get instantiated and compiled.
// ---------------------------------------------------------------------------
template ValueArrayOf<int>;
template ValueArrayEnumerator<int>;



// ---------------------------------------------------------------------------
//  Local functions
// ---------------------------------------------------------------------------
static bool constructorTests()
{
    // Do a basic constructor with just the count of elements
    ValueArrayOf<double> testArray1(255);

    // Make sure that it has the right initial size
    if (testArray1.length() != 255)
    {
        std::wcout  << L"    The ctor created wrong length() value"
                    << std::endl;
        return false;
    }

    // Copy construct another array from it and test the length
    ValueArrayOf<double> testArray2(testArray1);

    if (testArray2.length() != 255)
    {
        std::wcout << L"    The copy ctor created wrong length() value"
                << std::endl;
        return false;
    }

    // Test the equality of the two arrays
    if (testArray1 != testArray2)
    {
        std::wcout  << L"    The copy ctor created unequal arrays"
                    << std::endl;
        return false;
    }

    //
    //  Do another one where we provide the initial values.
    //
    double initValues[] = { 1.1, 2.2, 3.3, 4.4 };
    ValueArrayOf<double> testArray3(initValues, 4);

    if (testArray3.length() != 4)
    {
        std::wcout  << L"    The init values ctor created wrong length() value"
                    << std::endl;
        return false;
    }

    // Make sure the initial values are correct
    if ((testArray3[0] != 1.1)
    ||  (testArray3[1] != 2.2)
    ||  (testArray3[2] != 3.3)
    ||  (testArray3[3] != 4.4))
    {
        std::wcout  << L"    The init values ctor did not init contents correctly"
                    << std::endl;
        return false;
    }

    //
    //  Create another array of a different size and assign one of the
    //  existing ones to it and make sure that they are equal.
    //
    ValueArrayOf<double> testArray4(15);
    testArray4 = testArray3;

    if (testArray4 != testArray3)
    {
        std::wcout  << L"    Assignment did not create equal arrays"
                    << std::endl;
        return false;
    }
    return true;
}


static bool accessTests()
{
    ValueArrayOf<unsigned int> testArray1(16);

    // Fill in the array
    unsigned int index;
    for (index = 0; index < 16; index++)
        testArray1[index] = index;

    // Read them back again
    for (index = 0; index < 16; index++)
    {
        if (testArray1[index] != index)
        {
            std::wcout  << L"    Failed to read back values just set"
                        << std::endl;
            return false;
        }
    }

    // Make sure we get the expected array index error
    bool caughtIt = false;
    try
    {
        index = testArray1[16];
    }

    catch(const ArrayIndexOutOfBoundsException&)
    {
        caughtIt = true;
    }

    if (!caughtIt)
    {
        std::wcout << L"    Failed to catch index error" << std::endl;
        return false;
    }

    return true;
}


// ---------------------------------------------------------------------------
//  Test entry point
// ---------------------------------------------------------------------------
bool testValueArray()
{
    std::wcout  << L"----------------------------------\n"
                << L"Testing ValueArrayOf template class\n"
                << L"----------------------------------" << std::endl;

    bool retVal = true;

    try
    {
        // Call other local methods to do specific tests
        std::wcout << L"Testing ValueArrayOf contructors" << std::endl;
        if (!constructorTests())
        {
            std::wcout  << L"ValueArrayOf constructor tests failed"
                        << std::endl;
            retVal = false;
        }
         else
        {
            std::wcout  << L"ValueArrayOf constructor tests passed"
                        << std::endl;
        }
        std::wcout << std::endl;

        std::wcout << L"Testing ValueArrayOf element access" << std::endl;
        if (!accessTests())
        {
            std::wcout  << L"ValueArrayOf element access tests failed"
                        << std::endl;
            retVal = false;
        }
         else
        {
            std::wcout  << L"ValueArrayOf element access tests passed"
                        << std::endl;
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
