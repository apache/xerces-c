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
 * Revision 1.5  2002/02/01 22:46:28  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/03/02 19:55:49  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/06 07:48:40  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/01/19 00:59:07  roddey
 * Get rid of dependence on old utils output streams.
 *
 * Revision 1.1.1.1  1999/11/09 01:02:10  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:42:29  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

// ---------------------------------------------------------------------------
//  XML4C2 includes
// ---------------------------------------------------------------------------
#include "CoreTests.hpp"
#include <xercesc/util/ValueVectorOf.hpp>
#include <xercesc/util/ArrayIndexOutOfBoundsException.hpp>


// ---------------------------------------------------------------------------
//  Force a full instantiation of our vector and its enumerator, just to
//  insure that all methods get instantiated and compiled.
// ---------------------------------------------------------------------------
template ValueVectorOf<int>;
template ValueVectorEnumerator<int>;


// ---------------------------------------------------------------------------
//  Templatized testing code. These allow the exact same tests to be run
//  for any number of instantiation types over the by value vectors.
// ---------------------------------------------------------------------------
template <class T> bool commonValueTests()
{
    const unsigned int  testMax = 3;
    bool                caughtIt;

    // Create a vector of testMax of the instantiation type
    ValueVectorOf<T> testVec(testMax);

    // Make sure the initial capacity is what we set
    if (testVec.curCapacity() != testMax)
    {
        std::wcout << L"   Init capacity was bad" << std::endl;
        return false;
    }

    // Make sure the initial size is zero
    if (testVec.size() != 0)
    {
        std::wcout << L"   Init size was bad" << std::endl;
        return false;
    }

    // Test value for adding
    T testElem;

    // Add a value and check the count is 1
    testVec.addElement(testElem);
    if (testVec.size() != 1)
    {
        std::wcout << L"   Adding one element caused bad size" << std::endl;
        return false;
    }

    // Add another value and check the count is 2
    testVec.addElement(testElem);
    if (testVec.size() != 2)
    {
        std::wcout << L"   Adding another element caused bad size" << std::endl;
        return false;
    }

    // Test that the two of them are the same
    if (testVec.elementAt(0) != testVec.elementAt(1))
    {
        std::wcout << L"   First two elements did not match" << std::endl;
        return false;
    }

    // Add two more, which should cause an expansion of the vector
    testVec.addElement(testElem);
    testVec.addElement(testElem);

    if (testVec.curCapacity() == testMax)
    {
        std::wcout  << L"   Adding another element failed to cause an expansion"
                    << std::endl;
        return false;
    }

    // Check that we get an array bounds exception after an expansion
    caughtIt = false;
    try
    {
        testVec.elementAt(4);
    }

    catch(const ArrayIndexOutOfBoundsException&)
    {
        caughtIt = true;
    }

    if (!caughtIt)
    {
        std::wcout  << L"   Failed to catch array bounds error at element 4"
                    << std::endl;
        return false;
    }

    // Remove an item and see if the count went down by one
    testVec.removeElementAt(0);

    if (testVec.size() != 3)
    {
        std::wcout  << L"   Removing an element did not adjust size correctly"
                    << std::endl;
        return false;
    }

    // Remove the rest of them and make sure we hit zero
    testVec.removeElementAt(0);
    testVec.removeElementAt(0);
    testVec.removeElementAt(0);

    if (testVec.size() != 0)
    {
        std::wcout  << L"   Removing all elements did not zero the size"
                    << std::endl;
        return false;
    }

    // Check that we get an array bounds exception now still
    caughtIt = false;
    try
    {
        testVec.elementAt(0);
    }

    catch(const ArrayIndexOutOfBoundsException&)
    {
        caughtIt = true;
    }

    if (!caughtIt)
    {
        std::wcout  << L"   Failed to catch array bounds error at element 0"
                    << std::endl;
        return false;
    }

    // Add a few more elements back in, via insertion
    testVec.insertElementAt(testElem, 0);
    testVec.insertElementAt(testElem, 0);
    testVec.insertElementAt(testElem, 0);
    if (testVec.size() != 3)
    {
        std::wcout << L"   Inserting elements caused bad size" << std::endl;
        return false;
    }

    // Now do a remove all elements
    testVec.removeAllElements();

    if (testVec.size() != 0)
    {
        std::wcout << L"   removeAllElements caused bad size" << std::endl;
        return false;
    }
    return true;
}


template <class T> bool extendedValueTests()
{
    const unsigned int testMax = 8;

    // Create a test vector and put in ascending test values
    ValueVectorOf<T> testVec(testMax);
    testVec.addElement(T(0));
    testVec.addElement(T(1));
    testVec.addElement(T(2));
    testVec.addElement(T(3));
    testVec.addElement(T(4));
    testVec.addElement(T(5));
    testVec.addElement(T(6));
    testVec.addElement(T(7));

    // Now check that they went in that way
    unsigned int index;
    for (index = 0; index < testMax; index++)
    {
        if (testVec.elementAt(index) != T(index))
        {
            std::wcout  << L"   addElement put elements in wrong order"
                        << std::endl;
            return false;
        }
    }

    // Remove the zero'th element and test again
    testVec.removeElementAt(0);

    for (index = 0; index < testMax-1; index++)
    {
        if (testVec.elementAt(index) != T(index+1))
        {
            std::wcout  << L"   removeElement at head removed wrong element"
                        << std::endl;
            return false;
        }
    }

    // Test edge case by removing last element and test again
    testVec.removeElementAt(6);

    for (index = 0; index < testMax-2; index++)
    {
        if (testVec.elementAt(index) != T(index+1))
        {
            std::wcout  << L"   removeElement at end removed wrong element"
                        << std::endl;
            return false;
        }
    }
    return true;
}


// ---------------------------------------------------------------------------
//  Local functions
// ---------------------------------------------------------------------------
static bool doBasicTests()
{
    bool retVal = true;

    //
    // Do the common value vector tests for ints, bools and strings.
    //
    std::wcout << L"Testing ValueVectorOf<int>, common tests" << std::endl;
    if (!commonValueTests<int>())
    {
        std::wcout << L"ValueVectorOf<int> failed" << std::endl;
        retVal = false;
    }
     else
    {
        std::wcout << L"ValueVectorOf<int> passed" << std::endl;
    }
    std::wcout << std::endl;

    std::wcout << L"Testing ValueVectorOf<bool>, common tests" << std::endl;
    if (!commonValueTests<bool>())
    {
        std::wcout << L"ValueVectorOf<bool> failed" << std::endl;
        retVal = false;
    }
     else
    {
        std::wcout << L"ValueVectorOf<bool> passed" << std::endl;
    }
    std::wcout << std::endl;


    //
    //  And now do the second round of extended tests. These require that
    //  the instantiation type be of a fundamental value, because its going
    //  to test element ordering issues.
    //
    std::wcout << L"Testing ValueVectorOf<int>, extended tests" << std::endl;
    if (!extendedValueTests<int>())
    {
        std::wcout << L"Extended ValueVectorOf<int> failed" << std::endl;
        retVal = false;
    }
     else
    {
        std::wcout << L"Extended ValueVectorOf<int> passed" << std::endl;
    }
    std::wcout << std::endl;

    return retVal;
}


static bool enumTests()
{
    // Create a vector and fill it in with some known values
    ValueVectorOf<unsigned int> testVec(32);

    unsigned int index;
    for (index = 0; index < 32; index++)
        testVec.addElement(index);

    // Create an enumeration for it
    ValueVectorEnumerator<unsigned int> enumTest(&testVec);
    index = 0;
    while (enumTest.hasMoreElements())
    {
        if (enumTest.nextElement() != index++)
        {
            std::wcout  << L"    Enumerator sequence was incorrect"
                        << std::endl;
            return false;
        }
    }

    if (index != 32)
    {
        std::wcout  << L"    Enumerator did not enum enough elements"
                    << std::endl;
        return false;
    }

    return true;
}


// ---------------------------------------------------------------------------
//  Test entry point
// ---------------------------------------------------------------------------
bool testValueVector()
{
    std::wcout  << L"----------------------------------\n"
                << L"Testing ValueVectorOf template class\n"
                << L"----------------------------------" << std::endl;

    bool retVal = true;

    try
    {
        // Do the basic suite of tests, which is templatized
        if (!doBasicTests())
            retVal = false;

        // Test the enumerator
        std::wcout << L"Testing ValueVectorEnumerator" << std::endl;
        if (!enumTests())
        {
            std::wcout << L"ValueVectorEnumeration failed" << std::endl;
            retVal = false;
        }
         else
        {
            std::wcout << L"ValueVectorEnumeration passed" << std::endl;
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
