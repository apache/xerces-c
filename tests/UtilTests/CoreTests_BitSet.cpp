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
 * Revision 1.1  1999/11/09 01:01:47  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:42:26  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

// ---------------------------------------------------------------------------
//  XML4C2 Includes
// ---------------------------------------------------------------------------
#include "CoreTests.hpp"
#include <util/BitSet.hpp>



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
        outStrm << "    Ctor did not create set of correct size" << EndLn;
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
            outStrm << "    A bit's initial value was not zero" << EndLn;
            return false;
        }
    }

    // Make sure that allAreCleared() agrees
    if (!setTest.allAreCleared())
    {
        outStrm << "    allAreCleared() disagrees with individual bit gets"
                << EndLn;
        return false;
    }

    // Set and clear each bit and make sure that they come back right
    for (index = 0; index < count; index++)
    {
        setTest.set(index);
        if (!setTest.get(index))
        {
            outStrm << "    Bit was set but get returned false" << EndLn;
            return false;
        }

        setTest.clear(index);
        if (setTest.get(index))
        {
            outStrm << "    Bit was cleared but get returned true"
                    << EndLn;
            return false;
        }
    }

    // And once more make sure they are all zero
    for (index = 0; index < count; index++)
    {
        if (setTest.get(index))
        {
            outStrm << "    A bit remained set after clearing" << EndLn;
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
        outStrm << "    Copy ctor did not create equal sets" << EndLn;
        return false;
    }

    // Clear all bits of the copy and make sure they are all cleared
    setTest2.clearAll();
    for (index = 0; index < count; index++)
    {
        if (setTest2.get(index))
        {
            outStrm << "    clearAll() did not clear all bits"
                    << EndLn;
            return false;
        }
    }

    // Set a bit beyond the current size
    setTest2.set(32);

    // Make sure it expanded
    if (setTest2.size() != 64)
    {
        outStrm << "    Set of bit beyond size did not expand"
                << EndLn;
        return false;
    }

    // Set all the bits
    for (index = 0; index < count; index++)
        setTest.set(index);

    // Make sure that allAreSet() sees them all set
    if (!setTest.allAreSet())
    {
        outStrm << "    After setting all bits, allAreSet() returned false"
                << EndLn;
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
        outStrm << "    OR of set with empty set did not create equal sets"
                << EndLn;
        return false;
    }

    // Xor with original which should get back an empty set
    setTest2.xorWith(setTest);
    if (!setTest2.allAreCleared())
    {
        outStrm << "    XOR against original set did not get back original"
                << EndLn;
        return false;
    }

    // And them, which should have no effect
    setTest2.andWith(setTest);
    if (!setTest2.allAreCleared())
    {
        outStrm << "    AND against empty set changed bits" << EndLn;
        return false;
    }

    return true;
}


// ---------------------------------------------------------------------------
//  Test entry point
// ---------------------------------------------------------------------------
bool testBitSet()
{
    outStrm << "----------------------------------\n"
            << "Testing BitSet class\n"
            << "----------------------------------" << EndLn;

    bool retVal = true;

    try
    {
        outStrm << "Testing basic BitSet methods" << EndLn;
        if (!basicTests())
        {
            outStrm << "Bitset basic test methods failed" << EndLn;
            retVal = false;
        }
         else
        {
            outStrm << "Bitset basic tests passed" << EndLn;
        }
        outStrm << EndLn;

        outStrm << "Testing BitSet logical bit ops methods" << EndLn;
        if (!bitopsTests())
        {
            outStrm << "Bitset logical bit ops failed" << EndLn;
            retVal = false;
        }
         else
        {
            outStrm << "Bitset logical bit ops passed" << EndLn;
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
