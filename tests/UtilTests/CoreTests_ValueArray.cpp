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
 * Revision 1.4  2000/03/02 19:55:49  roddey
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
 * Revision 1.1.1.1  1999/11/09 01:02:07  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:42:28  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

// ---------------------------------------------------------------------------
//  XML4C2 includes
// ---------------------------------------------------------------------------
#include "CoreTests.hpp"
#include <util/ValueArrayOf.hpp>
#include <util/ArrayIndexOutOfBoundsException.hpp>


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
