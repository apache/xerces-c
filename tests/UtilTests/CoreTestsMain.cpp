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
 * Revision 1.4  2000/03/02 19:55:47  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/06 07:48:39  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/01/19 00:59:07  roddey
 * Get rid of dependence on old utils output streams.
 *
 * Revision 1.1.1.1  1999/11/09 01:01:44  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:42:26  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "CoreTests.hpp"
#include <xercesc/util/PlatformUtils.hpp>



// ---------------------------------------------------------------------------
//  Externs for our testing methods. There is just one per file so a header
//  for each one is a little overkill, so we just do it the old fashioned way.
// ---------------------------------------------------------------------------
extern bool testTranscoders();
extern bool testCountedPointer();
extern bool testBitSet();
extern bool testRefArray();
extern bool testRefHashTable();
extern bool testRefStack();
extern bool testRefVector();
extern bool testString();
extern bool testURL();
extern bool testValueArray();
extern bool testValueStack();
extern bool testValueVector();


int main()
{
    // Do the platform initialization
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch(const XMLException& toCatch)
    {
        XERCES_STD_QUALIFIER wcout << L"Parser Init Failed!\n   INFO: ("
                   << toCatch.getSrcFile() << L"." << toCatch.getSrcLine()
                   << L") -" << toCatch.getMessage() << XERCES_STD_QUALIFIER endl;
        return 0xFFFF;
    }

    XERCES_STD_QUALIFIER wcout << L"\nXML4C2 Core Utilities Unit Tester\n" << XERCES_STD_QUALIFIER endl;

    // This value will return the number of failed tests
    int retVal = 0;


    try
    {
        // -------------------------------------------------------------------
        // Test the basic transcoding services
        // -------------------------------------------------------------------
        if (!testTranscoders())
        {
            XERCES_STD_QUALIFIER wcout << L"Transcoder tests failed" << XERCES_STD_QUALIFIER endl;
            retVal++;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;


        // -------------------------------------------------------------------
        // Test the String class
        // -------------------------------------------------------------------
        if (!testString())
        {
            XERCES_STD_QUALIFIER wcout << L"String tests failed" << XERCES_STD_QUALIFIER endl;
            retVal++;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;


        // -------------------------------------------------------------------
        // Test the CountedPointerTo template class
        // -------------------------------------------------------------------
        if (!testCountedPointer())
        {
            XERCES_STD_QUALIFIER wcout << L"CountedPointerTo tests failed" << XERCES_STD_QUALIFIER endl;
            retVal++;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;


        // -------------------------------------------------------------------
        // Test the URL class
        // -------------------------------------------------------------------
        if (!testURL())
        {
            XERCES_STD_QUALIFIER wcout << L"URL tests failed" << XERCES_STD_QUALIFIER endl;
            retVal++;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;


        // -------------------------------------------------------------------
        // Test the ValueVectorOf template class
        // -------------------------------------------------------------------
        if (!testValueVector())
        {
            XERCES_STD_QUALIFIER wcout << L"ValueVectorOf tests failed" << XERCES_STD_QUALIFIER endl;
            retVal++;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;


        // -------------------------------------------------------------------
        // Test the ValueArrayOf template class
        // -------------------------------------------------------------------
        if (!testValueArray())
        {
            XERCES_STD_QUALIFIER wcout << L"ValueArrayOf tests failed" << XERCES_STD_QUALIFIER endl;
            retVal++;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;


        // -------------------------------------------------------------------
        // Test the ValueStackOf template class
        // -------------------------------------------------------------------
        if (!testValueStack())
        {
            XERCES_STD_QUALIFIER wcout << L"ValueStackOf tests failed" << XERCES_STD_QUALIFIER endl;
            retVal++;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;


        // -------------------------------------------------------------------
        // Test the RefArrayOf template class
        // -------------------------------------------------------------------
        if (!testRefArray())
        {
            XERCES_STD_QUALIFIER wcout << L"RefArrayOf tests failed" << XERCES_STD_QUALIFIER endl;
            retVal++;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;


        // -------------------------------------------------------------------
        // Test the RefStackOf template class
        // -------------------------------------------------------------------
        if (!testRefStack())
        {
            XERCES_STD_QUALIFIER wcout << L"RefStackOf tests failed" << XERCES_STD_QUALIFIER endl;
            retVal++;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;


        // -------------------------------------------------------------------
        // Test the RefVectorOf template class
        // -------------------------------------------------------------------
        if (!testRefVector())
        {
            XERCES_STD_QUALIFIER wcout << L"RefVectorOf tests failed" << XERCES_STD_QUALIFIER endl;
            retVal++;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;


        // -------------------------------------------------------------------
        // Test the RefHashtableOf template class
        // -------------------------------------------------------------------
        if (!testRefHashTable())
        {
            XERCES_STD_QUALIFIER wcout << L"RefHashTableOf tests failed" << XERCES_STD_QUALIFIER endl;
            retVal++;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;


        // -------------------------------------------------------------------
        // Test the BitSet class
        // -------------------------------------------------------------------
        if (!testBitSet())
        {
            XERCES_STD_QUALIFIER wcout << L"BitSet tests failed" << XERCES_STD_QUALIFIER endl;
            retVal++;
        }
        XERCES_STD_QUALIFIER wcout << XERCES_STD_QUALIFIER endl;
    }

    catch(const XMLException& toCatch)
    {
        XERCES_STD_QUALIFIER wcout  << L"Exception During Test!\n   INFO: ("
                    << toCatch.getSrcFile() << L"."
                    << toCatch.getSrcLine() << L") -"
                    << toCatch.getMessage() << XERCES_STD_QUALIFIER endl;
        return 0xFFFF;
    }

    // If we failed any tests, display a message
    XERCES_STD_QUALIFIER wcout << L"--------------------------------\n";
    if (retVal == 0)
        XERCES_STD_QUALIFIER wcout << L"<<PASSED>>: All tests passed\n";
    else
        XERCES_STD_QUALIFIER wcout << L"<<FAILED>>: Some tests failed\n";
    XERCES_STD_QUALIFIER wcout << L"--------------------------------\n" << XERCES_STD_QUALIFIER endl;

    return retVal;
}
