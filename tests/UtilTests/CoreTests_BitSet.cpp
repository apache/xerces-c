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
 * Revision 1.6  2003/05/30 13:08:26  gareth
 * move over to macros for std:: and iostream/iostream.h issues.
 *
 * Revision 1.5  2002/02/01 22:46:28  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/03/02 19:55:48  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/06 07:48:39  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/01/19 00:59:06  roddey
 * Get rid of dependence on old utils output streams.
 *
 * Revision 1.1.1.1  1999/11/09 01:01:47  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:42:26  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
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
        XERCES_STD_QUALIFIER wcout  << L"    Ctor did not create set of correct size"
                    << XERCES_STD_QUALIFIER endl;
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
            XERCES_STD_QUALIFIER wcout << L"    A bit's initial value was not zero"
                       << XERCES_STD_QUALIFIER endl;
            return false;
        }
    }

    // Make sure that allAreCleared() agrees
    if (!setTest.allAreCleared())
    {
        XERCES_STD_QUALIFIER wcout  << L"    allAreCleared() disagrees with individual bit gets"
                    << XERCES_STD_QUALIFIER endl;
        return false;
    }

    // Set and clear each bit and make sure that they come back right
    for (index = 0; index < count; index++)
    {
        setTest.set(index);
        if (!setTest.get(index))
        {
            XERCES_STD_QUALIFIER wcout << L"    Bit was set but get returned false"
                       << XERCES_STD_QUALIFIER endl;
            return false;
        }

        setTest.clear(index);
        if (setTest.get(index))
        {
            XERCES_STD_QUALIFIER wcout  << L"    Bit was cleared but get returned true"
                        << XERCES_STD_QUALIFIER endl;
            return false;
        }
    }

    // And once more make sure they are all zero
    for (index = 0; index < count; index++)
    {
        if (setTest.get(index))
        {
            XERCES_STD_QUALIFIER wcout << L"    A bit remained set after clearing"
                       << XERCES_STD_QUALIFIER endl;
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
        XERCES_STD_QUALIFIER wcout  << L"    Copy ctor did not create equal sets"
                    << XERCES_STD_QUALIFIER endl;
        return false;
    }

    // Clear all bits of the copy and make sure they are all cleared
    setTest2.clearAll();
    for (index = 0; index < count; index++)
    {
        if (setTest2.get(index))
        {
            XERCES_STD_QUALIFIER wcout  << L"    clearAll() did not clear all bits"
                        << XERCES_STD_QUALIFIER endl;
            return false;
        }
    }

    // Set a bit beyond the current size
    setTest2.set(32);

    // Make sure it expanded
    if (setTest2.size() != 64)
    {
        XERCES_STD_QUALIFIER wcout  << L"    Set of bit beyond size did not expand"
                    << XERCES_STD_QUALIFIER endl;
        return false;
    }

    // Set all the bits
    for (index = 0; index < count; index++)
        setTest.set(index);

    // Make sure that allAreSet() sees them all set
    if (!setTest.allAreSet())
    {
        XERCES_STD_QUALIFIER wcout  << L"    After setting all bits, allAreSet() returned false"
                    << XERCES_STD_QUALIFIER endl;
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
        XERCES_STD_QUALIFIER wcout  << L"    OR of set with empty set did not create equal sets"
                    << XERCES_STD_QUALIFIER endl;
        return false;
    }

    // Xor with original which should get back an empty set
    setTest2.xorWith(setTest);
    if (!setTest2.allAreCleared())
    {
        XERCES_STD_QUALIFIER wcout  << L"    XOR against original set did not get back original"
                    << XERCES_STD_QUALIFIER endl;
        return false;
    }

    // And them, which should have no effect
    setTest2.andWith(setTest);
    if (!setTest2.allAreCleared())
    {
        XERCES_STD_QUALIFIER wcout << L"    AND against empty set changed bits" << XERCES_STD_QUALIFIER endl;
        return false;
    }

    return true;
}


// ---------------------------------------------------------------------------
//  Test entry point
// ---------------------------------------------------------------------------
bool testBitSet()
{
    XERCES_STD_QUALIFIER wcout  << L"----------------------------------\n"
                << L"Testing BitSet class\n"
                << L"----------------------------------" << XERCES_STD_QUALIFIER endl;

    bool retVal = true;

    try
    {
        XERCES_STD_QUALIFIER wcout << L"Testing basic BitSet methods" << XERCES_STD_QUALIFIER endl;
        if (!basicTests())
        {
            XERCES_STD_QUALIFIER wcout << L"Bitset basic test methods failed" << XERCES_STD_QUALIFIER endl;
            retVal = false;
        }
         else
        {
            XERCES_STD_QUALIFIER wcout << L"Bitset basic tests passed" << XERCES_STD_QUALIFIER endl;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;

        XERCES_STD_QUALIFIER wcout << L"Testing BitSet logical bit ops methods" << XERCES_STD_QUALIFIER endl;
        if (!bitopsTests())
        {
            XERCES_STD_QUALIFIER wcout << L"Bitset logical bit ops failed" << XERCES_STD_QUALIFIER endl;
            retVal = false;
        }
         else
        {
            XERCES_STD_QUALIFIER wcout << L"Bitset logical bit ops passed" << XERCES_STD_QUALIFIER endl;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;
    }

    catch(const XMLException& toCatch)
    {
        XERCES_STD_QUALIFIER wcout << L"  ERROR: Unexpected exception!\n   Msg: "
                << toCatch.getMessage() << XERCES_STD_QUALIFIER endl;
        return false;
    }
    return retVal;
}
