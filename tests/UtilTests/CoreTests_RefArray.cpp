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
 * Revision 1.1.1.1  1999/11/09 01:01:52  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:42:27  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

// ---------------------------------------------------------------------------
//  XML4C2 includes
// ---------------------------------------------------------------------------
#include "CoreTests.hpp"
#include <xercesc/util/RefArrayOf.hpp>
#include <xercesc/util/ArrayIndexOutOfBoundsException.hpp>



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
        XERCES_STD_QUALIFIER wcout  << L"    The ctor created wrong length() value"
                    << XERCES_STD_QUALIFIER endl;
        return false;
    }

    // Copy construct another array from it and test the length
    RefArrayOf<double> testArray2(testArray1);

    if (testArray2.length() != 255)
    {
        XERCES_STD_QUALIFIER wcout  << L"    The copy ctor created wrong length() value"
                    << XERCES_STD_QUALIFIER endl;
        return false;
    }

    // Test the equality of the two arrays
    if (testArray1 != testArray2)
    {
        XERCES_STD_QUALIFIER wcout  << L"    The copy ctor created unequal arrays"
                    << XERCES_STD_QUALIFIER endl;
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
        XERCES_STD_QUALIFIER wcout  << L"    The init values ctor created wrong length() value"
                    << XERCES_STD_QUALIFIER endl;
        return false;
    }

    // Make sure the initial values are correct
    for (index = 0; index < 16; index++)
    {
        if (*testArray3[index] != (double)index)
        {
            XERCES_STD_QUALIFIER wcout  << L"    The init values ctor did not init contents correctly"
                        << XERCES_STD_QUALIFIER endl;
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
        XERCES_STD_QUALIFIER wcout  << L"    Assignment did not create equal arrays"
                    << XERCES_STD_QUALIFIER endl;
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
            XERCES_STD_QUALIFIER wcout  << L"    Failed to read back values just set"
                        << XERCES_STD_QUALIFIER endl;
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
        XERCES_STD_QUALIFIER wcout << L"    Failed to catch index error" << XERCES_STD_QUALIFIER endl;
        return false;
    }

    return true;
}


// ---------------------------------------------------------------------------
//  Test entry point
// ---------------------------------------------------------------------------
bool testRefArray()
{
    XERCES_STD_QUALIFIER wcout  << L"----------------------------------\n"
                << L"Testing RefArrayOf template class\n"
                << L"----------------------------------" << XERCES_STD_QUALIFIER endl;

    bool retVal = true;

    try
    {
        // Call other local methods to do specific tests
        XERCES_STD_QUALIFIER wcout << L"Testing RefArrayOf contructors" << XERCES_STD_QUALIFIER endl;
        if (!constructorTests())
        {
            XERCES_STD_QUALIFIER wcout  << L"RefArrayOf constructor tests failed"
                        << XERCES_STD_QUALIFIER endl;
            retVal = false;
        }
         else
        {
            XERCES_STD_QUALIFIER wcout  << L"RefArrayOf constructor tests passed"
                        << XERCES_STD_QUALIFIER endl;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;

        XERCES_STD_QUALIFIER wcout << L"Testing RefArrayOf element access" << XERCES_STD_QUALIFIER endl;
        if (!accessTests())
        {
            XERCES_STD_QUALIFIER wcout  << L"RefArrayOf element access tests failed"
                        << XERCES_STD_QUALIFIER endl;
            retVal = false;
        }
         else
        {
            XERCES_STD_QUALIFIER wcout  << L"RefArrayOf element access tests passed"
                        << XERCES_STD_QUALIFIER endl;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;
    }

    catch(const XMLException& toCatch)
    {
        XERCES_STD_QUALIFIER wcout  << L"  ERROR: Unexpected exception!\n   Msg: "
                    << toCatch.getMessage() << XERCES_STD_QUALIFIER endl;
        return false;
    }
    return retVal;
}
