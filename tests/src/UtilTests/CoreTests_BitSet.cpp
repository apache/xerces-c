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
//  XML4C2 Includes
// ---------------------------------------------------------------------------
#include "CoreTests.hpp"
#include <xercesc/util/BitSet.hpp>



// ---------------------------------------------------------------------------
//  Local testing methods
// ---------------------------------------------------------------------------
static bool basicTests()
{
    //
    //  Create a bitset with 32 bits. We just happen to know that this is
    //  the unit of expansion, so it should come back with exactly that
    //  number of bits of size.
    //
    BitSet setTest(32);

    if (setTest.size() != 32)
    {
        std::wcout  << L"    Ctor did not create set of correct size"
                    << std::endl;
        return false;
    }

    //
    //  Check the value of all of the bits and make sure that they all come
    //  back zero.
    //
    const unsigned int count = setTest.size();
    unsigned int index;
    for (index = 0; index < count; index++)
    {
        if (setTest.get(index))
        {
            std::wcout << L"    A bit's initial value was not zero"
                       << std::endl;
            return false;
        }
    }

    // Make sure that allAreCleared() agrees
    if (!setTest.allAreCleared())
    {
        std::wcout  << L"    allAreCleared() disagrees with individual bit gets"
                    << std::endl;
        return false;
    }

    // Set and clear each bit and make sure that they come back right
    for (index = 0; index < count; index++)
    {
        setTest.set(index);
        if (!setTest.get(index))
        {
            std::wcout << L"    Bit was set but get returned false"
                       << std::endl;
            return false;
        }

        setTest.clear(index);
        if (setTest.get(index))
        {
            std::wcout  << L"    Bit was cleared but get returned true"
                        << std::endl;
            return false;
        }
    }

    // And once more make sure they are all zero
    for (index = 0; index < count; index++)
    {
        if (setTest.get(index))
        {
            std::wcout << L"    A bit remained set after clearing"
                       << std::endl;
            return false;
        }
    }

    //
    //  Set some bits, then copy construct another bitset from this one. Then
    //  see if they come out equal.
    //
    setTest.set(1);
    setTest.set(16);
    setTest.set(20);
    setTest.set(24);

    BitSet setTest2(setTest);
    if (!setTest.equals(setTest2))
    {
        std::wcout  << L"    Copy ctor did not create equal sets"
                    << std::endl;
        return false;
    }

    // Clear all bits of the copy and make sure they are all cleared
    setTest2.clearAll();
    for (index = 0; index < count; index++)
    {
        if (setTest2.get(index))
        {
            std::wcout  << L"    clearAll() did not clear all bits"
                        << std::endl;
            return false;
        }
    }

    // Set a bit beyond the current size
    setTest2.set(32);

    // Make sure it expanded
    if (setTest2.size() != 64)
    {
        std::wcout  << L"    Set of bit beyond size did not expand"
                    << std::endl;
        return false;
    }

    // Set all the bits
    for (index = 0; index < count; index++)
        setTest.set(index);

    // Make sure that allAreSet() sees them all set
    if (!setTest.allAreSet())
    {
        std::wcout  << L"    After setting all bits, allAreSet() returned false"
                    << std::endl;
        return false;
    }

    return true;
}

static bool bitopsTests()
{
    // Create a bit set to test
    BitSet setTest(48);

    // Set some bits
    setTest.set(1);
    setTest.set(10);
    setTest.set(16);
    setTest.set(21);
    setTest.set(33);
    setTest.set(41);

    // Create another set to do ops on
    BitSet setTest2(48);

    // Or with the new set
    setTest2.orWith(setTest);

    // They should be equal now
    if (!setTest.equals(setTest2))
    {
        std::wcout  << L"    OR of set with empty set did not create equal sets"
                    << std::endl;
        return false;
    }

    // Xor with original which should get back an empty set
    setTest2.xorWith(setTest);
    if (!setTest2.allAreCleared())
    {
        std::wcout  << L"    XOR against original set did not get back original"
                    << std::endl;
        return false;
    }

    // And them, which should have no effect
    setTest2.andWith(setTest);
    if (!setTest2.allAreCleared())
    {
        std::wcout << L"    AND against empty set changed bits" << std::endl;
        return false;
    }

    return true;
}


// ---------------------------------------------------------------------------
//  Test entry point
// ---------------------------------------------------------------------------
bool testBitSet()
{
    std::wcout  << L"----------------------------------\n"
                << L"Testing BitSet class\n"
                << L"----------------------------------" << std::endl;

    bool retVal = true;

    try
    {
        std::wcout << L"Testing basic BitSet methods" << std::endl;
        if (!basicTests())
        {
            std::wcout << L"Bitset basic test methods failed" << std::endl;
            retVal = false;
        }
         else
        {
            std::wcout << L"Bitset basic tests passed" << std::endl;
        }
        std::wcout << std::endl;

        std::wcout << L"Testing BitSet logical bit ops methods" << std::endl;
        if (!bitopsTests())
        {
            std::wcout << L"Bitset logical bit ops failed" << std::endl;
            retVal = false;
        }
         else
        {
            std::wcout << L"Bitset logical bit ops passed" << std::endl;
        }
        std::wcout << std::endl;
    }

    catch(const XMLException& toCatch)
    {
        std::wcout << L"  ERROR: Unexpected exception!\n   Msg: "
                << toCatch.getMessage() << std::endl;
        return false;
    }
    return retVal;
}
