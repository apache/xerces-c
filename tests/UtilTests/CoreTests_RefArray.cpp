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
 * Revision 1.1  1999/11/09 01:01:52  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:42:27  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

// ---------------------------------------------------------------------------
//  XML4C2 includes
// ---------------------------------------------------------------------------
#include "CoreTests.hpp"
#include <util/RefArrayOf.hpp>
#include <util/ArrayIndexOutOfBoundsException.hpp>



// ---------------------------------------------------------------------------
//  Force a full instantiation of our array and its enumerator, just to
//  insure that all methods get instantiated and compiled.
// ---------------------------------------------------------------------------
template RefArrayOf<int>;
template RefArrayEnumerator<int>;


// ---------------------------------------------------------------------------
//  Local functions
// ---------------------------------------------------------------------------
static bool constructorTests()
{
    // Some values to test with
    double testVals[16];
    unsigned int index;
    for (index = 0; index < 16; index++)
        testVals[index] = index;

    // Do a basic constructor with just the count of elements
    RefArrayOf<double> testArray1(255);

    // Make sure that it has the right initial size
    if (testArray1.length() != 255)
    {
        outStrm << "    The ctor created wrong length() value" << EndLn;
        return false;
    }

    // Copy construct another array from it and test the length
    RefArrayOf<double> testArray2(testArray1);

    if (testArray2.length() != 255)
    {
        outStrm << "    The copy ctor created wrong length() value"
                << EndLn;
        return false;
    }

    // Test the equality of the two arrays
    if (testArray1 != testArray2)
    {
        outStrm << "    The copy ctor created unequal arrays"
                << EndLn;
        return false;
    }

    //
    //  Do another one where we provide the initial values.
    //
    double* initValues[16];
    for (index = 0; index < 16; index++)
        initValues[index ] = &testVals[index];
    RefArrayOf<double> testArray3(initValues, 16);

    if (testArray3.length() != 16)
    {
        outStrm << "    The init values ctor created wrong length() value"
                << EndLn;
        return false;
    }

    // Make sure the initial values are correct
    for (index = 0; index < 16; index++)
    {
        if (*testArray3[index] != (double)index)
        {
            outStrm << "    The init values ctor did not init contents correctly"
                    << EndLn;
            return false;
        }
    }

    //
    //  Create another array of a different size and assign one of the
    //  existing ones to it and make sure that they are equal.
    //
    RefArrayOf<double> testArray4(15);
    testArray4 = testArray3;

    if (testArray4 != testArray3)
    {
        outStrm << "    Assignment did not create equal arrays" << EndLn;
        return false;
    }

    return true;
}


static bool accessTests()
{
    // Some values to test with
    unsigned int testVals[16];
    unsigned int index;
    for (index = 0; index < 16; index++)
        testVals[index] = index;

    RefArrayOf<unsigned int> testArray1(16);

    // Fill in the array
    for (index = 0; index < 16; index++)
        testArray1[index] = &testVals[index];

    // Read them back again
    for (index = 0; index < 16; index++)
    {
        if (testArray1[index] != &testVals[index])
        {
            outStrm << "    Failed to read back values just set"
                    << EndLn;
            return false;
        }
    }

    // Make sure we get the expected array index error
    bool caughtIt = false;
    try
    {
        testArray1[16];
    }

    catch(const ArrayIndexOutOfBoundsException&)
    {
        caughtIt = true;
    }

    if (!caughtIt)
    {
        outStrm << "    Failed to catch index error" << EndLn;
        return false;
    }

    return true;
}


// ---------------------------------------------------------------------------
//  Test entry point
// ---------------------------------------------------------------------------
bool testRefArray()
{
    outStrm << "----------------------------------\n"
            << "Testing RefArrayOf template class\n"
            << "----------------------------------" << EndLn;

    bool retVal = true;

    try
    {
        // Call other local methods to do specific tests
        outStrm << "Testing RefArrayOf contructors" << EndLn;
        if (!constructorTests())
        {
            outStrm << "RefArrayOf constructor tests failed" << EndLn;
            retVal = false;
        }
         else
        {
            outStrm << "RefArrayOf constructor tests passed" << EndLn;
        }
        outStrm << EndLn;

        outStrm << "Testing RefArrayOf element access" << EndLn;
        if (!accessTests())
        {
            outStrm << "RefArrayOf element access tests failed" << EndLn;
            retVal = false;
        }
         else
        {
            outStrm << "RefArrayOf element access tests passed" << EndLn;
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
