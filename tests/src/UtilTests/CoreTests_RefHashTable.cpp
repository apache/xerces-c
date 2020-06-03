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
#include <xercesc/util/RefHashTableOf.hpp>


// ---------------------------------------------------------------------------
//  A simple class to test the ref hash table with
// ---------------------------------------------------------------------------
class TestElem
{
    public :
        TestElem(   const   XMLCh* const    key
                    , const unsigned int    value1
                    , const double          value2) :
            fKey(XMLString::replicate(key))
            , fValue1(value1)
            , fValue2(value2)
        {
        }

        ~TestElem()
        {
            XMLString::release(&fKey);
        }

        const XMLCh* getKey() const
        {
            return fKey;
        }


    private :
        XMLCh*          fKey;
        unsigned int    fValue1;
        double          fValue2;
};


// ---------------------------------------------------------------------------
//  Force a full instantiation of our hashtable, and its enumerator, just to
//  insure that all methods get instantiated and compiled.
// ---------------------------------------------------------------------------
template RefHashTableOf<TestElem>;
template RefHashTableOfEnumerator<TestElem>;



// ---------------------------------------------------------------------------
//  Local testing methods
// ---------------------------------------------------------------------------
static bool basicTests()
{

    return true;
}


// ---------------------------------------------------------------------------
//  Test entry point
// ---------------------------------------------------------------------------
bool testRefHashTable()
{
    std::wcout  << L"----------------------------------\n"
                << L"Testing RefHashTableOf class\n"
                << L"----------------------------------" << std::endl;

    bool retVal = true;

    try
    {
        std::wcout << L"Testing basic RefHashtable methods" << std::endl;
        if (!basicTests())
        {
            std::wcout  << L"RefHashtable basic test methods failed"
                        << std::endl;
            retVal = false;
        }
         else
        {
            std::wcout << L"RefHashtable basic tests passed" << std::endl;
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
