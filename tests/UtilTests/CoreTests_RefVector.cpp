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
 * Revision 1.1  1999/11/09 01:01:59  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:42:27  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

// ---------------------------------------------------------------------------
//  XML4C2 Includes
// ---------------------------------------------------------------------------
#include "CoreTests.hpp"
#include <util/RefVectorOf.hpp>
#include <util/ArrayIndexOutOfBoundsException.hpp>



// ---------------------------------------------------------------------------
//  Force a full instantiation of the vector and its enumerator just to insure
//  that all methods get instantiated and compiled.
// ---------------------------------------------------------------------------
template RefVectorOf<int>;
template RefVectorEnumerator<int>;



// ---------------------------------------------------------------------------
//  Templatized testing code. These allow the exact same tests to be run
//  for any number of instantiation types over the by value vectors.
// ---------------------------------------------------------------------------
template <class T> bool commonRefTests()
{
    const unsigned int  testMax = 3;
    bool                caughtIt;

    //
    //  Create a vector of testMax of the instantiation type. Tell it that
    //  we adopt elements.
    //
    RefVectorOf<T> testVec(testMax, true);

    // Make sure the initial capacity is what we set
    if (testVec.curCapacity() != testMax)
    {
        outStrm << "   Init capacity was incorrect" << EndLn;
        return false;
    }

    // Make sure the initial size is zero
    if (testVec.size() != 0)
    {
        outStrm << "   Init size was not correct" << EndLn;
        return false;
    }

    // Add a value and check the count is 1
    testVec.addElement(new T);
    if (testVec.size() != 1)
    {
        outStrm << "   Adding one element caused bad size" << EndLn;
        return false;
    }

    // Add another value and check the count is 2
    testVec.addElement(new T);
    if (testVec.size() != 2)
    {
        outStrm << "   Adding another element caused bad size" << EndLn;
        return false;
    }

    // Add two more, which should cause an expansion of the vector
    testVec.addElement(new T);
    testVec.addElement(new T);

    if (testVec.curCapacity() == testMax)
    {
        outStrm << "   Adding another element failed to cause an expansion"
                << EndLn;
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
        outStrm << "   Failed to catch array bounds error at element 4"
                << EndLn;
        return false;
    }

    // Remove an item and see if the count went down by one
    testVec.removeElementAt(0);

    if (testVec.size() != 3)
    {
        outStrm << "   Removing an element did not adjust size correctly"
                << EndLn;
        return false;
    }

    // Remove the rest of them and make sure we hit zero
    testVec.removeElementAt(0);
    testVec.removeElementAt(0);
    testVec.removeElementAt(0);

    if (testVec.size() != 0)
    {
        outStrm << "   Removing all elements did not zero the size"
                << EndLn;
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
        outStrm << "   Failed to catch array bounds error at element 0"
                << EndLn;
        return false;
    }

    // Add a few more elements back in, via insertion
    testVec.insertElementAt(new T, 0);
    testVec.insertElementAt(new T, 0);
    testVec.insertElementAt(new T, 0);
    if (testVec.size() != 3)
    {
        outStrm << "   Inserting elements caused bad size" << EndLn;
        return false;
    }

    // Now do a remove all elements
    testVec.removeAllElements();

    if (testVec.size() != 0)
    {
        outStrm << "   removeAllElements caused bad size" << EndLn;
        return false;
    }

    return true;
}


template <class T> bool extendedRefTests()
{
    const unsigned int testMax = 8;

    // Create a test vector and put in ascending test values
    RefVectorOf<T> testVec(testMax, true);
    testVec.addElement(new T(0));
    testVec.addElement(new T(1));
    testVec.addElement(new T(2));
    testVec.addElement(new T(3));
    testVec.addElement(new T(4));
    testVec.addElement(new T(5));
    testVec.addElement(new T(6));
    testVec.addElement(new T(7));

    // Now check that they went in that way
    unsigned int index;
    for (index = 0; index < testMax; index++)
    {
        if (*testVec.elementAt(index) != T(index))
        {
            outStrm << "   addElement put elements in wrong order"
                    << EndLn;
            return false;
        }
    }

    // Remove the zero'th element and test again
    testVec.removeElementAt(0);

    for (index = 0; index < testMax-1; index++)
    {
        if (*testVec.elementAt(index) != T(index+1))
        {
            outStrm << "   removeElement at head removed wrong element"
                    << EndLn;
            return false;
        }
    }

    // Test edge case by removing last element and test again
    testVec.removeElementAt(6);

    for (index = 0; index < testMax-2; index++)
    {
        if (*testVec.elementAt(index) != T(index+1))
        {
            outStrm << "   removeElement at end removed wrong element"
                    << EndLn;
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
    // Do the common reference vector tests for ints, bools and strings.
    //
    outStrm << "Testing RefVectorOf<int>, common tests" << EndLn;
    if (!commonRefTests<int>())
    {
        outStrm << "RefVectorOf<int> failed" << EndLn;
        retVal = false;
    }
     else
    {
        outStrm << "RefVectorOf<int> passed" << EndLn;
    }
    outStrm << EndLn;

    outStrm << "Testing RefVectorOf<bool>, common tests" << EndLn;
    if (!commonRefTests<bool>())
    {
        outStrm << "RefVectorOf<bool> failed" << EndLn;
        retVal = false;
    }
     else
    {
        outStrm << "RefVectorOf<bool> passed" << EndLn;
    }
    outStrm << EndLn;


    //
    //  And now do the second round of extended tests. These require that
    //  the instantiation type be of a fundamental value, because its going
    //  to test element ordering issues.
    //
    outStrm << "Testing RefVectorOf<int>, extended tests" << EndLn;
    if (!extendedRefTests<int>())
    {
        outStrm << "Extended RefVectorOf<int> failed" << EndLn;
        retVal = false;
    }
     else
    {
        outStrm << "Extended RefVectorOf<int> passed" << EndLn;
    }
    outStrm << EndLn;

    return retVal;
}


static bool enumTests()
{
    // Create a vector and fill it in with some known values
    RefVectorOf<unsigned int> testVec(32, true);

    unsigned int index;
    for (index = 0; index < 32; index++)
        testVec.addElement(new unsigned int(index));

    // Create an enumeration for it
    RefVectorEnumerator<unsigned int> enumTest(&testVec);
    index = 0;
    while (enumTest.hasMoreElements())
    {
        if (enumTest.nextElement() != index++)
        {
            outStrm << "    Enumerator sequence was incorrect" << EndLn;
            return false;
        }
    }

    if (index != 32)
    {
        outStrm << "    Enumerator did not enum enough elements"
                << EndLn;
        return false;
    }
    return true;
}



// ---------------------------------------------------------------------------
//  Test entry point
// ---------------------------------------------------------------------------
bool testRefVector()
{
    outStrm << "----------------------------------\n"
            << "Testing RefVectorOf template class\n"
            << "----------------------------------" << EndLn;

    bool retVal = true;

    try
    {
        // Do the basic suite of tests
        if (!doBasicTests())
            retVal = false;

        // Test the enumerator
        outStrm << "Testing RefVectorEnumerator" << EndLn;
        if (!enumTests())
        {
            outStrm << "RefVectorEnumeration failed" << EndLn;
            retVal = false;
        }
         else
        {
            outStrm << "RefVectorEnumeration passed" << EndLn;
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
