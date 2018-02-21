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
#include <xercesc/util/RefStackOf.hpp>


// ---------------------------------------------------------------------------
//  Force a full instantiation of our stack and its enumerator, just to
//  insure that all methods get instantiated and compiled.
// ---------------------------------------------------------------------------
template RefStackOf<int>;
template RefStackEnumerator<int>;



// ---------------------------------------------------------------------------
//  Test entry point
// ---------------------------------------------------------------------------
static bool basicTests()
{
    RefStackOf<double> testStack(500);

    return true;
}


// ---------------------------------------------------------------------------
//  Test entry point
// ---------------------------------------------------------------------------
bool testRefStack()
{
    std::wcout  << L"----------------------------------\n"
                << L"Testing RefStackOf template class\n"
                << L"----------------------------------" << std::endl;

    bool retVal = true;

    try
    {
        // Call other local methods to do specific tests
        std::wcout << L"Testing RefStackOf basics" << std::endl;
        if (!basicTests())
        {
            std::wcout << L"RefStackOf basic tests failed" << std::endl;
            retVal = false;
        }
         else
        {
            std::wcout  << L"RefArrayOf constructor tests passed"
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
