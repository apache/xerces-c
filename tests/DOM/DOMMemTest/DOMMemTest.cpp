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

//
//  Various DOM tests.
//     Contents include
//       1.  Basic functionality for DOMString
//       2.  Regression tests for bugs fixed.
//     All individual are wrapped in a memory leak checker.
//
//     This is NOT a complete test of DOM functionality.
//

/*
 * $Log$
 * Revision 1.24  2000/11/30 19:46:07  andyh
 * DOM_Text::splitText(), fix off by one error in the test for index too big error.
 * Tinny Ng
 *
 * Revision 1.23  2000/10/13 22:47:37  andyh
 * Fix bug (failure to null-terminate result) in XMLString::trim().
 * Patch contributed by Nadav Aharoni
 *
 * Revision 1.22  2000/05/09 00:22:48  andyh
 * Memory Cleanup.  XMLPlatformUtils::Terminate() deletes all lazily
 * allocated memory; memory leak checking tools will no longer report
 * that leaks exist.  (DOM GetElementsByTagID temporarily removed
 * as part of this.)
 *
 * Revision 1.21  2000/04/18 01:07:28  aruna1
 * Rectified memory leak caused by doctype-getNodeName()
 *
 * Revision 1.20  2000/03/11 03:00:13  chchou
 * Fix bug # 18, remove set method of readonly attributes.
 * As a result, remove related test cases
 *
 * Revision 1.19  2000/03/10 02:15:15  chchou
 * add null DOM_DocumentType constructor
 *
 * Revision 1.18  2000/03/02 19:55:44  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.17  2000/02/29 02:19:12  rahulj
 * No more compilation errors under HPUX 11.0. We do not build
 * DOMMemTest as it crashes the aCC compiler.
 *
 * Revision 1.16  2000/02/17 21:56:02  andyh
 * Update to match W3C Dom Level 2 spec changes
 *
 * Revision 1.15  2000/02/15 23:15:28  andyh
 * Add additional tests to DOMMemTest
 *
 * Revision 1.13  2000/02/06 07:48:35  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.12  2000/02/05 01:19:19  andyh
 * Add more DOMString tests.  Fix limit test error in DOMString::insertData()
 * Andy Heninger  heninger@us.ibm.com
 *
 * Revision 1.11  2000/02/03 23:07:28  andyh
 * Add several new functions from Robert Weir to DOMString.
 *
 * Revision 1.10  2000/01/29 00:39:09  andyh
 * Redo synchronization in DOMStringHandle allocator.  There
 * was a bug in the use of Compare and Swap.  Switched to mutexes.
 *
 * Changed a few plain deletes to delete [].
 *
 * Revision 1.9  2000/01/22 01:38:32  andyh
 * Remove compiler warnings in DOM impl classes
 *
 * Revision 1.8  2000/01/20 20:37:25  andyh
 * Remove DEVENV_VCPP preprocessor variable everywhere.
 * It was obsolete, left over from an earlier configuration system.
 * And it was not set correctly in all projects.
 *
 * Should fix build problem reported by some with use of
 * InterlockedCompareExchange() on Windows with VC6.
 *
 * Revision 1.6  2000/01/19 21:40:58  andyh
 * Remove a few remaining dependencies on the (now defunct)
 * XML StdOut stream.
 *
 * Revision 1.5  2000/01/18 19:57:30  andyh
 * Add some DOM L2 tests
 *
 * Revision 1.4  1999/12/17 02:09:41  andyh
 * Fix bug in DOMString::insertData() that occured if the source
 * and destination strings were the same and the orginal buffer had
 * enough space to hold the result.
 *
 * Revision 1.3  1999/12/10 18:35:15  andyh
 * Removed spurious debug output from DOMMemTest.
 * Udated MSVC project files for Release build of DOMTest and DOMMemTest,
 *    these weren't set up before.
 *
 * Revision 1.2  1999/12/03 00:02:25  andyh
 * DOM tests updated to catch DOMException by ref.
 * Added tests of DOMString::transcode() and append()
 *
 * Revision 1.1.1.1  1999/11/09 01:02:43  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:42:24  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#include <stdio.h>
#include <string.h>
#include <dom/DOM.hpp>
#include <dom/DomMemDebug.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLException.hpp>
#include <util/XMLString.hpp>


#define TASSERT(c) tassert((c), __FILE__, __LINE__)

void tassert(bool c, char *file, int line)
{
    if (!c)
        printf("Failure.  Line %d,   file %s\n", line, file);
};


#define TESTPROLOG entryMemState = DomMemDebug();

#define TESTEPILOG \
    exitMemState = DomMemDebug(); \
    if (entryMemState != exitMemState) { \
        printf(" Memory leak at line %d, file %s:  ", __LINE__, __FILE__);  \
        exitMemState.printDifference(entryMemState); \
    }


#define EXCEPTION_TEST(operation, expected_exception)               \
{                                                                   \
    try {                                                           \
    operation;                                                      \
    printf(" Error: no exception thrown at line %d\n", __LINE__);   \
}                                                                   \
    catch (DOM_DOMException &e) {                                       \
    if (e.code != expected_exception)                       \
    printf(" Wrong exception code: %d at line %d\n", e.code, __LINE__); \
}                                                                 \
    catch (...)   {                                                 \
    printf(" Wrong exception thrown at line %d\n", __LINE__);       \
}                                                                   \
}


//---------------------------------------------------------------------------------------
//
//   DOMStringTests    tests of class DOMString
//
//---------------------------------------------------------------------------------------
void    DOMStringTests()
{
    DomMemDebug     entryMemState, exitMemState;
    
    //
    //  Test 1.  Basic operations on a simple string.
    //
    printf("DOM Memory Test.\n");
    TESTPROLOG;
    {
        DOMString foo = "fop";
        TASSERT(foo.length() == 3);
        TASSERT(foo.equals(foo));
        TASSERT(foo != 0);
        TASSERT(foo.charAt(0) == 'f');
        TASSERT(foo.charAt(2) == 'p');
        TASSERT(foo.charAt(3) == 0);
    }
    TESTEPILOG
        
        //
        //  Construct from XMLCh *
        //
        TESTPROLOG
    {
        //  ToDo - this test will fail on EBCDIC machines.  !!
        
        XMLCh a[] = {'H', 'e', 'l', 'l', 'o', 0};
        DOMString x(a);
        DOMString y = "Hello";
        TASSERT(x.equals(y));
        
        DOMString z(a+2, 3);
        TASSERT(z.equals("llo"));
    }
    TESTEPILOG
        
        //
        //  Test 2.  Empty strings shouldn't leave anything lying around
        //
        TESTPROLOG;
    {
        DOMString a;
        DOMString b;
        TASSERT (a==0);
        a = 0;
        TASSERT(a==0);
        TASSERT((a!=0) == false);
        
        DOMString c(0);
        TASSERT(c==0);
        TASSERT(c==a);
    }
    TESTEPILOG
        
        
        //
        //  Test 3.   Clones should be equal. 
        TESTPROLOG;
    {
        DOMString a = "hello";
        DOMString b = a.clone();
        TASSERT(a.equals(b));
        TASSERT(a != b);
        a = 0;
        TASSERT(b.equals("hello"));
        TASSERT(a == 0);
        TASSERT(a.equals(""));
    }
    TESTEPILOG
        
        
        //
        //  Test 4.  Copy construction and assignemnt
        //
        TESTPROLOG;
    {
        DOMString a = "Test 04";
        DOMString b(a);
        DOMString c = a;
        DOMString d;
        DOMString e;
        d = a;
        TASSERT(a==b);
        TASSERT(a==c);
        TASSERT(a==d);
        TASSERT(a!=e);
        // printf ("   test04 should have 1 handle, 1 buffer here.  "); 
    }
    TESTEPILOG
        
        
        //
        // Test 5  AppendData, degenerate cases.
        //
        TESTPROLOG;
    {
        DOMString a;
        DOMString b = "Test 05";
        
        a.appendData(b);
        TASSERT(a.equals(b));
        TASSERT(a!=b);
        TASSERT(a.equals("Test 05"));
    };
    TESTEPILOG
        
        
        //
        //  Test 6  Append data, degenerate case 2
        //
        TESTPROLOG;
    {
        DOMString a;
        DOMString b = "Test 06";
        
        b.appendData(a);
        TASSERT(!a.equals(b));
        TASSERT(a!=b);
        TASSERT(b.equals("Test 06"));
        TASSERT(a==0);
    }
    TESTEPILOG
        
        
        //
        //  Test 7  Append Data, Common case, no extra space in original buffer.
        //          Also, operator +=, which is a convenience alias for appendData.
        //
        TESTPROLOG;
    {
        DOMString a = "Test 07";
        DOMString b = "append";
        
        a.appendData(b);
        TASSERT(a.equals("Test 07append"));
        TASSERT(b.equals("append"));
        
        a.appendData((XMLCh)0x31);
        TASSERT(a.equals("Test 07append1"));
        
        XMLCh  s[] = {0x32, 0x33, 0x00};
        a.appendData(s);
        TASSERT(a.equals("Test 07append123"));
        
        a = "Test 07a ";
        a += b;
        TASSERT(a.equals("Test 07a append"));
        
        a += (XMLCh)0x31;
        TASSERT(a.equals("Test 07a append1"));
        
        a += s;
        TASSERT(a.equals("Test 07a append123"));
        
    }
    TESTEPILOG
        
        
        //
        //  Test 8 Append Data, with plenty of extra space in buffer.
        //
        TESTPROLOG;
    {
        DOMString a;
        a.reserve(100);
        DOMString b("Test 08");
        DOMString c("append");
        
        TASSERT(a != 0);  // (The String object has an identity, even if no contents)
        TASSERT(a.length() == 0);
        a.appendData(b);
        TASSERT(a.equals(b));
        TASSERT(a.equals("Test 08"));
        TASSERT(a != b);
        a.appendData(c);
        TASSERT(a.equals("Test 08append"));
        TASSERT(b.equals("Test 08"));
        TASSERT(c.equals("append"));
    };
    TESTEPILOG
        
        
        
        //
        //  Test 9 Append Data, with plenty of extra space in buffer, but with
        //                      a clone, so that the original buffer can not be modified.
        //
        TESTPROLOG;
    {
        DOMString a;
        a.reserve(100);
        DOMString b("Test 09");
        DOMString c("append");
        
        TASSERT(a.length() == 0);
        a.appendData(b);
        TASSERT(a.equals(b));
        TASSERT(a.equals("Test 09"));
        TASSERT(a != b);
        
        DOMString d = a.clone();
        TASSERT(a.equals("Test 09"));
        TASSERT(b.equals("Test 09"));
        TASSERT(d.equals("Test 09"));
        TASSERT(a != b);
        TASSERT(a != d);
        TASSERT(b != d);
        
        a.appendData(c);
        TASSERT(a.equals("Test 09append"));
        TASSERT(b.equals("Test 09"));
        TASSERT(c.equals("append"));
        TASSERT(d.equals("Test 09"));
    };
    TESTEPILOG;
    
    
    //
    // Test 10   DOMString Operator +
    //
    TESTPROLOG;
    {
        DOMString a;
        DOMString b("DOMString ");
        XMLCh     s[] = {0x58, 0x4d, 0x4c, 0x20, 0x00};   // Unicode "XML "
        XMLCh     Z   = 0x5A;                             // Unicode 'Z'
        
        a = b + b;
        TASSERT(a.equals("DOMString DOMString "));
        
        a = b + s;
        TASSERT(a.equals("DOMString XML "));
        
        a = s + b;
        TASSERT(a.equals("XML DOMString "));
        
        a = b + Z;
        TASSERT(a.equals("DOMString Z"));
        
        a = Z + b;
        TASSERT(a.equals("ZDOMString "));
    }
    TESTEPILOG;
    
    
    //
    // Test 11   DOMString::subStringData(unsigned int offset, unsigned int count)
    //
    TESTPROLOG;
    {
        DOMString srcString("This is the source string.");
        //                   01234567890123456789012345
        
        DOMString This = srcString.substringData(0,4);
        DOMString is   = srcString.substringData(5,2);
        DOMString dot  = srcString.substringData(25,1);
        DOMString offEnd = srcString.substringData(19, 1000);
        
        TASSERT(This.equals("This"));
        TASSERT(is  .equals("is"));
        TASSERT(dot .equals("."));
        TASSERT(offEnd.equals("string."));
        
        EXCEPTION_TEST(srcString.substringData(-1, 10), DOM_DOMException::INDEX_SIZE_ERR);
        EXCEPTION_TEST(srcString.substringData(26, 1), DOM_DOMException::INDEX_SIZE_ERR);
        
        srcString.insertData(0, "x");   // Changing the source should not alter previously
        //   extracted substrings.
        
        TASSERT(This.equals("This"));
        TASSERT(is  .equals("is"));
        TASSERT(dot .equals("."));
        TASSERT(offEnd.equals("string."));
        TASSERT(srcString.equals("xThis is the source string."));
        
    }
    TESTEPILOG;
    
    
    //
    // Test 12   DOMString::insertData(unsigned int offset, DOMString &src)
    //
    TESTPROLOG;
    {
        DOMString aString("This is a string.");
        //                 01234567890123456
        
        aString.insertData(17, " Added at end.");
        TASSERT(aString.equals("This is a string. Added at end."));
        
        aString = "This is a string.";
        EXCEPTION_TEST(aString.insertData(18, "x"), DOM_DOMException::INDEX_SIZE_ERR);
        TASSERT(aString.equals("This is a string."));
        
        aString = 0;
        aString.reserve(100);
        aString.appendData("This is a string.");
        aString.insertData(17, " Added at end.");
        TASSERT(aString.equals("This is a string. Added at end."));
        
        aString.insertData(0, "So ");
        TASSERT(aString.equals("So This is a string. Added at end."));
        
        aString.insertData(2, "x");
        TASSERT(aString.equals("Sox This is a string. Added at end."));
        
        EXCEPTION_TEST(aString.substringData(-1, 1), DOM_DOMException::INDEX_SIZE_ERR);
        
        
    }
    TESTEPILOG;
    

	//
	// Minimal test of DOMString::transcode()
	//
	TESTPROLOG;
	{
		static char testStr[] = "This is our test string.";

		DOMString DOMTestStr = testStr;
		char *roundTripString = DOMTestStr.transcode();
		TASSERT(strcmp(testStr, roundTripString) == 0);
		delete [] roundTripString;

        DOMString domstr2 = DOMString::transcode(testStr);
        TASSERT(domstr2.equals(DOMTestStr));
	}
    TESTEPILOG;


    //
    //  String bugs submitted by David Chung
    //
	TESTPROLOG;
	{
        DOMString greeting("hello");
        greeting.appendData(greeting);
        TASSERT(greeting.equals("hellohello"));

 
        // Test DOMString.insertData, when source string is the same as the destination.
        //   Implementation forces creation of a new buffer.
        //                            
        DOMString greeting2("Hello              ");
        //                   0123456789012345678
        greeting2.deleteData(5, 14);    // Leave unused space at end of buffer.
        TASSERT(greeting2.equals("Hello"));

        greeting2.insertData(2, greeting2); 
        TASSERT(greeting2.equals("HeHellollo"));


        // DOMString.insertData().  Original buffer has space, and is retained.
        DOMString greeting3("Hello              ");
        //                   0123456789012345678
        greeting3.deleteData(5, 14);    // Leave unused space at end of buffer.
        TASSERT(greeting3.equals("Hello"));  

        greeting3.insertData(2, "ByeBye"); 
        TASSERT(greeting3.equals("HeByeByello"));

    }
    TESTEPILOG;


    //
    //  String bug submitted by Nadav Aharoni
    //
    TESTPROLOG;
    {
        char testString[] = "            ";
        testString[4] = 0;
        testString[5] = 'x';
        char *origString = testString;
        XMLString::trim(testString);
        TASSERT(strlen(testString) == 0);
        TASSERT(testString[5] == 'x');

        strcpy(testString, "  Hello  ");
        XMLString::trim(testString);
        TASSERT(strcmp(testString, "Hello") == 0);
    }
    TESTEPILOG;
}



//---------------------------------------------------------------------------------------
//
//   DOMBasicTests    Basic DOM Level 1 tests
//
//---------------------------------------------------------------------------------------
void DOMBasicTests()
{
        DomMemDebug     entryMemState, exitMemState;
    //
    //  Test Doc01      Create a new empty document
    //
    {
        //  First precondition, so that lazily created strings do not appear
        //  as memory leaks.
        DOM_Document    doc;
        doc = DOM_Document::createDocument();
    }
    TESTPROLOG;
    {
        DOM_Document    doc;
        doc = DOM_Document::createDocument();
    }
    TESTEPILOG

    //
    //  Test Doc02      Create one of each kind of node using the
    //                  document createXXX methods.
    //                  Watch for memory leaks.
    //
    {
        //  Do all operations in a preconditioning step, to force the
        //  creation of implementation objects that are set up on first use.
        //  Don't watch for leaks in this block (no TESTPROLOG / TESTEPILOG)
        DOM_Document doc = DOM_Document::createDocument();
        DOM_Element     el = doc.createElement("Doc02Element");
        DOM_DocumentFragment frag = doc.createDocumentFragment ();
        DOM_Text  text = doc.createTextNode("Doc02TextNode");
        DOM_Comment comment = doc.createComment("Doc02Comment");
        DOM_CDATASection  cdataSec = doc.createCDATASection("Doc02CDataSection");
        DOM_DocumentType  docType = doc.createDocumentType("Doc02DocumentType");
        DOM_Notation notation = doc.createNotation("Doc02Notation");
        DOM_ProcessingInstruction pi = doc.createProcessingInstruction("Doc02PITarget",
                                    "Doc02PIData");
        DOM_NodeList    nodeList = doc.getElementsByTagName("*");
    }


    TESTPROLOG;
    {
        DOM_Document doc = DOM_Document::createDocument();
        DOM_Element     el = doc.createElement("Doc02Element");
    }
    TESTEPILOG;

    TESTPROLOG
    {
        DOM_Document    doc = DOM_Document::createDocument();
        DOM_DocumentFragment frag = doc.createDocumentFragment ();
    };
    TESTEPILOG


    TESTPROLOG;
    {
        DOM_Document doc = DOM_Document::createDocument();
        DOM_Element     el = doc.createElement("Doc02Element");
    }
    TESTEPILOG;

    TESTPROLOG;
    {
        DOM_Document doc = DOM_Document::createDocument();
        DOM_Text  text = doc.createTextNode("Doc02TextNode");
    }
    TESTEPILOG;

    TESTPROLOG;
    {
        DOM_Document doc = DOM_Document::createDocument();
        DOM_Comment comment = doc.createComment("Doc02Comment");
    }
    TESTEPILOG;

    TESTPROLOG;
    {
        DOM_Document doc = DOM_Document::createDocument();
        DOM_CDATASection  cdataSec = doc.createCDATASection("Doc02CDataSection");
    }
    TESTEPILOG;


    TESTPROLOG;
    {
        DOM_Document doc = DOM_Document::createDocument();
        DOM_DocumentType  docType = doc.createDocumentType("Doc02DocumentType");
    }
    TESTEPILOG;


    TESTPROLOG;
    {
        DOM_Document doc = DOM_Document::createDocument();
        DOM_Notation notation = doc.createNotation("Doc02Notation");
    }
    TESTEPILOG;


    TESTPROLOG;
    {
        DOM_Document doc = DOM_Document::createDocument();
        DOM_ProcessingInstruction pi = doc.createProcessingInstruction("Doc02PITarget",
                                    "Doc02PIData");
    }
    TESTEPILOG;

    TESTPROLOG;
    {
        DOM_Document doc = DOM_Document::createDocument();
        DOM_Attr  attribute = doc.createAttribute("Doc02Attribute");
    }
    TESTEPILOG;


    TESTPROLOG;
    {
        DOM_Document doc = DOM_Document::createDocument();
        DOM_EntityReference  er = doc.createEntityReference("Doc02EntityReference");
    }
    TESTEPILOG;

    TESTPROLOG;
    {
        DOM_Document doc = DOM_Document::createDocument();
        DOM_NodeList    nodeList = doc.getElementsByTagName("*");
    }
    TESTEPILOG;

    
    //
    //  Doc03 - Create a small document tree
    //
    TESTPROLOG;
    {
        DOM_Document    doc = DOM_Document::createDocument();
        DOM_Element     rootEl = doc.createElement("Doc03RootElement");
        doc.appendChild(rootEl);
        DOM_Text        textNode = doc.createTextNode("Doc03 text stuff");
        rootEl.appendChild(textNode);

        DOM_NodeList    nodeList = doc.getElementsByTagName("*");
    };
    TESTEPILOG;


    //
    //  Attr01
    //
    {
        DOM_Document    doc = DOM_Document::createDocument();
        DOM_Element     rootEl  = doc.createElement("RootElement");
        doc.appendChild(rootEl);
        {
            DOM_Attr        attr01  = doc.createAttribute("Attr01");
            rootEl.setAttributeNode(attr01);
        }
        
        TESTPROLOG;
        {
            DOM_Attr attr02 = doc.createAttribute("Attr01");
            rootEl.setAttributeNode(attr02);  
        }
        TESTEPILOG
    };

    //
    //  Attr02
    //
    TESTPROLOG;
    {
        DOM_Document    doc = DOM_Document::createDocument();
        DOM_Element     rootEl  = doc.createElement("RootElement");
        doc.appendChild(rootEl);
        DOM_Attr        attr01  = doc.createAttribute("Attr02");
        rootEl.setAttributeNode(attr01);
        DOM_Attr        attr02 = doc.createAttribute("Attr02");
        rootEl.setAttributeNode(attr02);  
    }
    TESTEPILOG;


    //
    //  Attr03
    //
    TESTPROLOG;
    {
        DOM_Document    doc = DOM_Document::createDocument();
        DOM_Element     rootEl  = doc.createElement("RootElement");
        doc.appendChild(rootEl);
        DOM_Attr        attr01  = doc.createAttribute("Attr03");
        rootEl.setAttributeNode(attr01);

        attr01.setValue("Attr03Value1");
        attr01.setValue("Attr03Value2");
    }
    TESTEPILOG;



    //
    //  Text01
    //
    TESTPROLOG;
    {
        DOM_Document    doc = DOM_Document::createDocument();
        DOM_Element     rootEl  = doc.createElement("RootElement");
        doc.appendChild(rootEl);


        DOM_Text        txt1 = doc.createTextNode("Hello Goodbye");
        rootEl.appendChild(txt1);

        txt1.splitText(6);
        rootEl.normalize();

    }
    TESTEPILOG;


    //
    //  Notation01
    //
    TESTPROLOG;
    { 
        DOM_Document        doc = DOM_Document::createDocument();
        DOM_DocumentType    dt  = doc.createDocumentType("DocType_for_Notation01");
        doc.appendChild(dt);


        DOM_NamedNodeMap notationMap = dt.getNotations();
        DOM_Notation    nt1 = doc.createNotation("Notation01");
        notationMap.setNamedItem (nt1);
		DOM_Node  abc1 = notationMap.getNamedItem("Notation01");
        DOM_Notation    nt2 = (DOM_Notation &) abc1;
        TASSERT(nt1==nt2);
        nt2 = DOM_Notation();
        nt1 = 0;
		DOM_Node abc6 = notationMap.getNamedItem("Notation01");
        nt2 = (DOM_Notation &) abc6;
      

    }
    TESTEPILOG;


    //
    //  NamedNodeMap01 - comparison operators.
    //
    TESTPROLOG;
    {
        DOM_NamedNodeMap    nnm;
        TASSERT(nnm == 0);

        DOM_Document        doc = DOM_Document::createDocument();
        nnm = doc.getAttributes();    // Should be null, because node type
                                      //   is not Element.
        TASSERT(nnm == 0);
        TASSERT(!(nnm != 0));

        DOM_Element el = doc.createElement("NamedNodeMap01");
        DOM_NamedNodeMap nnm2 = el.getAttributes();    // Should be an empty, but non-null map.
        TASSERT(nnm2 != 0);
        TASSERT(nnm != nnm2);
        nnm = nnm2;
        TASSERT(nnm == nnm2);
    }
    TESTEPILOG;


    //
    //  importNode quick test
    //
    TESTPROLOG
    {
        DOM_Document    doc1 = DOM_Document::createDocument();
        DOM_Document    doc2 = DOM_Document::createDocument();
        
        DOM_Element     el1  = doc1.createElement("abc");
        doc1.appendChild(el1);
        TASSERT(el1.getParentNode() != 0);
        DOM_Node        el2  = doc2.importNode(el1, true);
        TASSERT(el2.getParentNode() == 0);
        DOMString       tagName = el2.getNodeName();
        TASSERT(tagName.equals("abc"));
        TASSERT(el2.getOwnerDocument() == doc2);
        TASSERT(doc1 != doc2);
    }
    TESTEPILOG;

    //
    //  getLength() tests.  Both Node CharacterData and NodeList implement
    //                  getLength().  Early versions of the DOM had a clash
    //                  between the two, originating in the implementation class
    //                  hirearchy, which has NodeList as a (distant) base class
    //                  of CharacterData.  This is a regression test to verify
    //                  that the problem stays fixed.
    //
    TESTPROLOG
    {
        DOM_Document     doc = DOM_Document::createDocument();
        DOM_Text          tx = doc.createTextNode("Hello");
        DOM_Element       el = doc.createElement("abc");
        el.appendChild(tx);

        int     textLength = tx.getLength();
        TASSERT(textLength == 5);

        DOM_NodeList      nl = tx.getChildNodes();
        int      nodeListLen = nl.getLength();
        TASSERT(nodeListLen == 0);

        nl = el.getChildNodes();
        nodeListLen = nl.getLength();
        TASSERT(nodeListLen == 1);
    }


    //
    //  NodeList - comparison operators, basic operation.
    //
    TESTPROLOG;
    {
        DOM_NodeList    nl;
        DOM_NodeList    nl2;
        TASSERT(nl == 0);
        TASSERT(!(nl != 0));
        TASSERT(nl == nl2);

        DOM_Document        doc = DOM_Document::createDocument();
        nl = doc.getChildNodes();    // Should be non-null, but empty

        TASSERT(nl != 0);
        int len = nl.getLength();
        TASSERT(len == 0);

        DOM_Element el = doc.createElement("NodeList01");
        doc.appendChild(el);
        len = nl.getLength();
        TASSERT(len == 1);
        TASSERT(nl != nl2);
        nl2 = nl;
        TASSERT(nl == nl2);
    }
    TESTEPILOG;


 
    //
    //  Name validity checking.
    //
    TESTPROLOG;
    {
         DOM_Document        doc = DOM_Document::createDocument();
         try
         {
             DOM_Element el = doc.createElement("!@@ bad element name");
             TASSERT(false);  // Exception above should prevent us reaching here.
         }
         catch ( DOM_DOMException &e)
         {
             TASSERT(e.code == DOM_DOMException::INVALID_CHARACTER_ERR);
         }
         catch (...)
         {
             TASSERT(false);  // Wrong exception thrown.
         }
    }
    TESTEPILOG;


    //
    //  Assignment ops return value
    //
    TESTPROLOG;
    {
        DOM_Document        doc = DOM_Document::createDocument();
        DOM_Element el = doc.createElement("NodeList01");
        doc.appendChild(el);
        
        DOM_Element n1, n2, n3;
        
        n1 = n2 = n3 = el;
        TASSERT(n1 == n2);
        TASSERT(n1 == n3);
        TASSERT(n1 == el);
        TASSERT(n1 != 0);
        n1 = n2 = n3 = 0;
        TASSERT(n1 == 0);
    }
    TESTEPILOG;


    //
    //  Cloning of a node with attributes. Regression test for a ref counting 
    //  bug in attributes of cloned nodes that occured when the "owned" flag
    //  was not set in the clone.
    //
    TESTPROLOG;
    {
        DOM_Document    doc = DOM_Document::createDocument();
        DOM_Element     root = doc.createElement("CTestRoot");
        root.setAttribute("CTestAttr", "CTestAttrValue");

        DOMString s = root.getAttribute("CTestAttr");
        TASSERT(s.equals("CTestAttrValue"));

        DOM_Node abc2 = root.cloneNode(true);
        DOM_Element     cloned = (DOM_Element &) abc2;
        DOM_Attr a = cloned.getAttributeNode("CTestAttr");
        TASSERT(a != 0);
        s = a.getValue();
        TASSERT(s.equals("CTestAttrValue"));
        a = 0;

        a = cloned.getAttributeNode("CTestAttr");
        TASSERT(a != 0);
        s = a.getValue();
        TASSERT(s.equals("CTestAttrValue"));

    }
    TESTEPILOG;


    //
    //  splitText()
    //     Regression test for a bug from Tinny Ng
    //
    TESTPROLOG;
    {
        DOM_Document doc;
        doc = DOM_Document::createDocument();
        DOM_Text tn, tn1, tn2;
        tn = doc.createTextNode ("0123456789");

        tn1 = tn.splitText(5);
        TASSERT( tn.getNodeValue().equals("01234"));
        TASSERT(tn1.getNodeValue().equals("56789"));

        tn2 = tn.splitText(5);
        TASSERT( tn.getNodeValue().equals("01234"));
        TASSERT(tn2.getNodeValue().equals(""));

        EXCEPTION_TEST(tn.splitText(6), DOM_DOMException::INDEX_SIZE_ERR);
    }
    TESTEPILOG;


}


//---------------------------------------------------------------------------------------
//
//   DOMNSTests    DOM Name Space tests
//
//---------------------------------------------------------------------------------------
void DOMNSTests()
{
        DomMemDebug     entryMemState, exitMemState;



    //
    //  DOM Level 2 tests.  These should be split out as a separate test.
    //


    //
    // hasFeature.  The set of supported options tested here is for Xerces 1.1
    //              Note: because the implementation lazily creates some of the comprison
    //                    strings within the implementation, this test must be pre-flighted
    //                    outside of the TESPROLOG/TESTEPILOG macros to avoid spurious
    //                    reports of memory leaks.
    //
    {
        DOM_DOMImplementation  impl;
        TASSERT(impl.hasFeature("XML", "2.0")    == true);
        TASSERT(impl.hasFeature("XML", "")       == true);
        //  We also support 1.0
        TASSERT(impl.hasFeature("XML", "1.0")    == true);
        TASSERT(impl.hasFeature("XML", "3.0")    == false);
        TASSERT(impl.hasFeature("Traversal", "") == true);
    }

    
    TESTPROLOG;
    {
        DOM_DOMImplementation  impl;
        TASSERT(impl.hasFeature("XML", "2.0")    == true);
        TASSERT(impl.hasFeature("XML", "")       == true);
        //  We also support 1.0
        TASSERT(impl.hasFeature("XML", "1.0")    == true);
        TASSERT(impl.hasFeature("XML", "3.0")    == false);
        TASSERT(impl.hasFeature("Traversal", "") == true);


        TASSERT(impl.hasFeature("HTML", "")           == false);
        TASSERT(impl.hasFeature("Views", "")          == false);
        TASSERT(impl.hasFeature("StyleSheets", "")    == false);
        TASSERT(impl.hasFeature("CSS", "")            == false);
        TASSERT(impl.hasFeature("CSS2", "")           == false);
        TASSERT(impl.hasFeature("Events", "")         == false);
        TASSERT(impl.hasFeature("UIEvents", "")       == false);
        TASSERT(impl.hasFeature("MouseEvents", "")    == false);
        TASSERT(impl.hasFeature("MutationEvents", "") == false);
        TASSERT(impl.hasFeature("HTMLEvents", "")     == false);
        TASSERT(impl.hasFeature("Range", "")          == false);
    }
    TESTEPILOG;


    //
    // CreateDocumentType
    //
    TESTPROLOG;
    {
        DOM_DOMImplementation impl;
        
        DOMString qName = "foo:docName";
        DOMString pubId = "pubId";
        DOMString sysId = "http://sysId";
        
        DOM_DocumentType dt = impl.createDocumentType(qName, pubId, sysId);
        
        TASSERT(dt != 0);
        TASSERT(dt.getNodeType() == DOM_Node::DOCUMENT_TYPE_NODE);
        TASSERT(dt.getNodeName().equals(qName));
        TASSERT(dt.getNamespaceURI().equals(0));
        TASSERT(dt.getPrefix().equals(0));
        TASSERT(dt.getLocalName().equals(0));
        TASSERT(dt.getPublicId().equals(pubId));
        TASSERT(dt.getSystemId().equals(sysId));
        TASSERT(dt.getInternalSubset().equals(0));
        TASSERT(dt.getOwnerDocument() == 0);
        
        DOM_NamedNodeMap nnm = dt.getEntities();
        TASSERT(nnm.getLength() == 0);
        nnm = dt.getNotations();
        TASSERT(nnm.getLength() == 0);

        //
        // Qualified name without prefix should also work.
        //
        qName = "docName";
        dt = impl.createDocumentType(qName, pubId, sysId);

        TASSERT(dt != 0);
        TASSERT(dt.getNodeType() == DOM_Node::DOCUMENT_TYPE_NODE);
        TASSERT(dt.getNodeName().equals(qName));
        TASSERT(dt.getNamespaceURI().equals(0));
        TASSERT(dt.getPrefix().equals(0));
        TASSERT(dt.getLocalName().equals(0));
        TASSERT(dt.getPublicId().equals(pubId));
        TASSERT(dt.getSystemId().equals(sysId));
        TASSERT(dt.getInternalSubset().equals(0));
        TASSERT(dt.getOwnerDocument() == 0);

        // Creating a DocumentType with invalid or malformed qName should fail.
        EXCEPTION_TEST(impl.createDocumentType("<docName", pubId, sysId), DOM_DOMException::INVALID_CHARACTER_ERR);     
        EXCEPTION_TEST(impl.createDocumentType(":docName", pubId, sysId), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(impl.createDocumentType("docName:", pubId, sysId), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(impl.createDocumentType("doc::Name", pubId, sysId), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(impl.createDocumentType("doc:N:ame", pubId, sysId), DOM_DOMException::NAMESPACE_ERR);
    }
    TESTEPILOG;
    

    //
    //  DOMImplementation::CreateDocument
    {
        // Preflight the operations that will lazily create DOMStrings
        // in the implementation.  This prevents incorrect reports of
        // memory leaks in the real test.

        DOM_DOMImplementation   impl;
        DOM_DocumentType        dt; 
        DOM_Document            doc = impl.createDocument("", "a", dt);
        doc.getNodeName();
    }

    //
    TESTPROLOG;
    {
        
        DOM_DOMImplementation impl;
        
        DOMString qName = "foo:docName";
        DOMString pubId = "pubId";
        DOMString sysId = "http://sysId";
        
        DOM_DocumentType dt = impl.createDocumentType(qName, pubId, sysId);
        
        DOMString docNSURI = "http://document.namespace";

        DOM_Document doc = impl.createDocument(docNSURI, qName, dt);

        TASSERT(dt.getOwnerDocument() == doc);
        TASSERT(doc.getOwnerDocument() == 0);

        TASSERT(doc.getNodeType() == DOM_Node::DOCUMENT_NODE);
        TASSERT(doc.getDoctype() == dt);
        TASSERT(doc.getNodeName().equals("#document"));

        TASSERT(doc.getNodeValue() == 0);
        TASSERT(doc.getNamespaceURI().equals(0));
        TASSERT(doc.getPrefix().equals(0));
        TASSERT(doc.getLocalName().equals(0));

        DOM_Element el = doc.getDocumentElement();

        TASSERT(el.getLocalName().equals("docName"));
        TASSERT(el.getNamespaceURI().equals(docNSURI));
        TASSERT(el.getNodeName().equals(qName));
        TASSERT(el.getOwnerDocument() == doc);
        TASSERT(el.getParentNode() == doc);
        TASSERT(el.getPrefix().equals("foo"));
        TASSERT(el.getTagName().equals(qName));
        TASSERT(el.hasChildNodes() == false);


        //
        // Creating a second document with the same docType object should fail.
        //
        try
        {
            DOM_Document doc2 = impl.createDocument(docNSURI, qName, dt);
            TASSERT(false);  // should not reach here.
        }
        catch ( DOM_DOMException &e)
        {
            TASSERT(e.code == DOM_DOMException::WRONG_DOCUMENT_ERR);
        }
        catch (...)
        {
            TASSERT(false);  // Wrong exception thrown.
        }

        // Creating a document with null NamespaceURI and DocumentType
        doc = impl.createDocument(docNSURI, qName, 0);

        // Namespace tests of createDocument are covered by createElementNS below
    }
    TESTEPILOG;
    



    //
    //  CreateElementNS methods
    //
    TESTPROLOG;
    {
        
        // Set up an initial (root element only) document.
        // 
        DOM_DOMImplementation impl;
        
        DOMString qName = "foo:docName";
        DOMString pubId = "pubId";
        DOMString sysId = "http://sysId";
        DOM_DocumentType dt = impl.createDocumentType(qName, pubId, sysId);
        
        DOMString docNSURI = "http://document.namespace";
        DOM_Document doc = impl.createDocument(docNSURI, qName, dt);
        DOM_Element rootEl = doc.getDocumentElement();

        //
        // CreateElementNS
        //
        DOM_Element ela = doc.createElementNS("http://nsa", "a:ela");  // prefix and URI
        DOM_Element elb = doc.createElementNS("http://nsb", "elb");    //  URI, no prefix.
        DOM_Element elc = doc.createElementNS("", "elc");              // No URI, no prefix.

        rootEl.appendChild(ela);
        rootEl.appendChild(elb);
        rootEl.appendChild(elc);

        TASSERT(ela.getNodeName().equals("a:ela"));
        TASSERT(ela.getNamespaceURI().equals("http://nsa"));
        TASSERT(ela.getPrefix().equals("a"));
        TASSERT(ela.getLocalName().equals("ela"));
        TASSERT(ela.getTagName().equals("a:ela"));

        TASSERT(elb.getNodeName().equals("elb"));
        TASSERT(elb.getNamespaceURI().equals("http://nsb"));
        TASSERT(elb.getPrefix().equals(""));
        TASSERT(elb.getLocalName().equals("elb"));
        TASSERT(elb.getTagName().equals("elb"));

        TASSERT(elc.getNodeName().equals("elc"));
        TASSERT(elc.getNamespaceURI().equals(""));
        TASSERT(elc.getPrefix().equals(""));
        TASSERT(elc.getLocalName().equals("elc"));
        TASSERT(elc.getTagName().equals("elc"));

        // Badly formed qualified name
        EXCEPTION_TEST(doc.createElementNS("http://nsa", "<a"), DOM_DOMException::INVALID_CHARACTER_ERR);     
        EXCEPTION_TEST(doc.createElementNS("http://nsa", ":a"), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(doc.createElementNS("http://nsa", "a:"), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(doc.createElementNS("http://nsa", "a::a"), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(doc.createElementNS("http://nsa", "a:a:a"), DOM_DOMException::NAMESPACE_ERR);     

        // xml:a must have namespaceURI == "http://www.w3.org/XML/1998/namespace"
        DOMString xmlURI = "http://www.w3.org/XML/1998/namespace";
        TASSERT(doc.createElementNS(xmlURI, "xml:a").getNamespaceURI().equals(xmlURI));
        EXCEPTION_TEST(doc.createElementNS("http://nsa", "xml:a"), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(doc.createElementNS("", "xml:a"), DOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc.createElementNS(0,  "xml:a"), DOM_DOMException::NAMESPACE_ERR);

        //unlike Attribute, xmlns (no different from foo) can have any namespaceURI for Element
        TASSERT(doc.createElementNS("http://nsa", "xmlns").getNamespaceURI().equals("http://nsa"));
        TASSERT(doc.createElementNS(xmlURI, "xmlns").getNamespaceURI().equals(xmlURI));
        TASSERT(doc.createElementNS("", "xmlns").getNamespaceURI().equals(""));
        TASSERT(doc.createElementNS(0,  "xmlns").getNamespaceURI().equals(""));

        //unlike Attribute, xmlns:a (no different from foo:a) can have any namespaceURI for Element
        //except "" and null
        TASSERT(doc.createElementNS("http://nsa", "xmlns:a").getNamespaceURI().equals("http://nsa"));
        TASSERT(doc.createElementNS(xmlURI, "xmlns:a").getNamespaceURI().equals(xmlURI));
        EXCEPTION_TEST(doc.createElementNS("", "xmlns:a"), DOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc.createElementNS(0,  "xmlns:a"), DOM_DOMException::NAMESPACE_ERR);

        //In fact, any prefix != null should have a namespaceURI != 0 or != ""
        TASSERT(doc.createElementNS("http://nsa", "foo:a").getNamespaceURI().equals("http://nsa"));
        EXCEPTION_TEST(doc.createElementNS("", "foo:a"), DOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc.createElementNS(0,  "foo:a"), DOM_DOMException::NAMESPACE_ERR);

        //Change prefix
        DOM_Element elem = doc.createElementNS("http://nsa", "foo:a");
        elem.setPrefix("bar");
        TASSERT(elem.getNodeName().equals("bar:a"));
        TASSERT(elem.getNamespaceURI().equals("http://nsa"));
        TASSERT(elem.getPrefix().equals("bar"));
        TASSERT(elem.getLocalName().equals("a"));
        TASSERT(elem.getTagName().equals("bar:a"));
        //The spec does not prevent us from setting prefix to a node without prefix
        elem = doc.createElementNS("http://nsa", "a");
        TASSERT(elem.getPrefix().equals(""));
        elem.setPrefix("bar");
        TASSERT(elem.getNodeName().equals("bar:a"));
        TASSERT(elem.getNamespaceURI().equals("http://nsa"));
        TASSERT(elem.getPrefix().equals("bar"));
        TASSERT(elem.getLocalName().equals("a"));
        TASSERT(elem.getTagName().equals("bar:a"));
        //Special case for xml:a where namespaceURI must be xmlURI
        elem = doc.createElementNS(xmlURI, "foo:a");
        elem.setPrefix("xml");
        elem = doc.createElementNS("http://nsa", "foo:a");
        EXCEPTION_TEST(elem.setPrefix("xml"), DOM_DOMException::NAMESPACE_ERR);     
        //However, there is no restriction on prefix xmlns
        elem.setPrefix("xmlns");
        //Also an element can not have a prefix with namespaceURI == null or ""
        elem = doc.createElementNS(0, "a");
        EXCEPTION_TEST(elem.setPrefix("foo"), DOM_DOMException::NAMESPACE_ERR);     
        elem = doc.createElementNS("", "a");
        EXCEPTION_TEST(elem.setPrefix("foo"), DOM_DOMException::NAMESPACE_ERR);     

        //Only prefix of Element and Attribute can be changed
        EXCEPTION_TEST(doc.setPrefix("foo"), DOM_DOMException::NAMESPACE_ERR);

        //Prefix of readonly Element can not be changed.
        //However, there is no way to create such DOM_Element for testing yet.
    }
    TESTEPILOG;



    //
    //  CreateAttributeNS methods
    //
    TESTPROLOG;
    {
        
        // Set up an initial (root element only) document.
        // 
        DOM_DOMImplementation impl;
        
        DOMString qName = "foo:docName";
        DOMString pubId = "pubId";
        DOMString sysId = "http://sysId";
        DOM_DocumentType dt = impl.createDocumentType(qName, pubId, sysId);
        
        DOMString docNSURI = "http://document.namespace";
        DOM_Document doc = impl.createDocument(docNSURI, qName, dt);
        DOM_Element rootEl = doc.getDocumentElement();

        //
        // CreateAttributeNS
        //
        DOM_Attr attra = doc.createAttributeNS("http://nsa", "a:attra");       // prefix and URI
        DOM_Attr attrb = doc.createAttributeNS("http://nsb", "attrb");         //  URI, no prefix.
        DOM_Attr attrc = doc.createAttributeNS("", "attrc");    // No URI, no prefix.

        TASSERT(attra.getNodeName().equals("a:attra"));
        TASSERT(attra.getNamespaceURI().equals("http://nsa"));
        TASSERT(attra.getPrefix().equals("a"));
        TASSERT(attra.getLocalName().equals("attra"));
        TASSERT(attra.getName().equals("a:attra"));
        TASSERT(attra.getOwnerElement() == 0);

        TASSERT(attrb.getNodeName().equals("attrb"));
        TASSERT(attrb.getNamespaceURI().equals("http://nsb"));
        TASSERT(attrb.getPrefix().equals(""));
        TASSERT(attrb.getLocalName().equals("attrb"));
        TASSERT(attrb.getName().equals("attrb"));
        TASSERT(attrb.getOwnerElement() == 0);

        TASSERT(attrc.getNodeName().equals("attrc"));
        TASSERT(attrc.getNamespaceURI().equals(""));
        TASSERT(attrc.getPrefix().equals(""));
        TASSERT(attrc.getLocalName().equals("attrc"));
        TASSERT(attrc.getName().equals("attrc"));
        TASSERT(attrc.getOwnerElement() == 0);

        // Badly formed qualified name
        EXCEPTION_TEST(doc.createAttributeNS("http://nsa", "<a"), DOM_DOMException::INVALID_CHARACTER_ERR);     
        EXCEPTION_TEST(doc.createAttributeNS("http://nsa", ":a"), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(doc.createAttributeNS("http://nsa", "a:"), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(doc.createAttributeNS("http://nsa", "a::a"), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(doc.createAttributeNS("http://nsa", "a:a:a"), DOM_DOMException::NAMESPACE_ERR);     

        // xml:a must have namespaceURI == "http://www.w3.org/XML/1998/namespace"
        DOMString xmlURI = "http://www.w3.org/XML/1998/namespace";
        TASSERT(doc.createAttributeNS(xmlURI, "xml:a").getNamespaceURI().equals(xmlURI));
        EXCEPTION_TEST(doc.createAttributeNS("http://nsa", "xml:a"), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(doc.createAttributeNS("", "xml:a"), DOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc.createAttributeNS(0,  "xml:a"), DOM_DOMException::NAMESPACE_ERR);

        //unlike Element, xmlns must have namespaceURI == "http://www.w3.org/2000/xmlns/"
        DOMString xmlnsURI = "http://www.w3.org/2000/xmlns/";
        TASSERT(doc.createAttributeNS(xmlnsURI, "xmlns").getNamespaceURI().equals(xmlnsURI));
        EXCEPTION_TEST(doc.createAttributeNS("http://nsa", "xmlns"), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(doc.createAttributeNS(xmlURI, "xmlns"), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(doc.createAttributeNS("", "xmlns"), DOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc.createAttributeNS(0,  "xmlns"), DOM_DOMException::NAMESPACE_ERR);

        //unlike Element, xmlns:a must have namespaceURI == "http://www.w3.org/2000/xmlns/"
        TASSERT(doc.createAttributeNS(xmlnsURI, "xmlns:a").getNamespaceURI().equals(xmlnsURI));
        EXCEPTION_TEST(doc.createAttributeNS("http://nsa", "xmlns:a"), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(doc.createAttributeNS(xmlURI, "xmlns:a"), DOM_DOMException::NAMESPACE_ERR);     
        EXCEPTION_TEST(doc.createAttributeNS("", "xmlns:a"), DOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc.createAttributeNS(0,  "xmlns:a"), DOM_DOMException::NAMESPACE_ERR);

        //In fact, any prefix != null should have a namespaceURI != 0 or != ""
        TASSERT(doc.createAttributeNS("http://nsa", "foo:a").getNamespaceURI().equals("http://nsa"));
        EXCEPTION_TEST(doc.createAttributeNS("", "foo:a"), DOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc.createAttributeNS(0,  "foo:a"), DOM_DOMException::NAMESPACE_ERR);

        //Change prefix
        DOM_Attr attr = doc.createAttributeNS("http://nsa", "foo:a");
        attr.setPrefix("bar");
        TASSERT(attr.getNodeName().equals("bar:a"));
        TASSERT(attr.getNamespaceURI().equals("http://nsa"));
        TASSERT(attr.getPrefix().equals("bar"));
        TASSERT(attr.getLocalName().equals("a"));
        TASSERT(attr.getName().equals("bar:a"));
        //The spec does not prevent us from setting prefix to a node without prefix
        attr = doc.createAttributeNS("http://nsa", "a");
        TASSERT(attr.getPrefix().equals(""));
        attr.setPrefix("bar");
        TASSERT(attr.getNodeName().equals("bar:a"));
        TASSERT(attr.getNamespaceURI().equals("http://nsa"));
        TASSERT(attr.getPrefix().equals("bar"));
        TASSERT(attr.getLocalName().equals("a"));
        TASSERT(attr.getName().equals("bar:a"));
        //Special case for xml:a where namespaceURI must be xmlURI
        attr = doc.createAttributeNS(xmlURI, "foo:a");
        attr.setPrefix("xml");
        attr = doc.createAttributeNS("http://nsa", "foo:a");
        EXCEPTION_TEST(attr.setPrefix("xml"), DOM_DOMException::NAMESPACE_ERR);
        //Special case for xmlns:a where namespaceURI must be xmlURI
        attr = doc.createAttributeNS(xmlnsURI, "foo:a");
        attr.setPrefix("xmlns");
        attr = doc.createAttributeNS("http://nsa", "foo:a");
        EXCEPTION_TEST(attr.setPrefix("xmlns"), DOM_DOMException::NAMESPACE_ERR);
        //Special case for xmlns where no prefix can be set
        attr = doc.createAttributeNS(xmlnsURI, "xmlns");
        EXCEPTION_TEST(attr.setPrefix("foo"), DOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(attr.setPrefix("xmlns"), DOM_DOMException::NAMESPACE_ERR);
        //Also an attribute can not have a prefix with namespaceURI == null or ""
        attr = doc.createAttributeNS(0, "a");
        EXCEPTION_TEST(attr.setPrefix("foo"), DOM_DOMException::NAMESPACE_ERR);     
        attr = doc.createAttributeNS("", "a");
        EXCEPTION_TEST(attr.setPrefix("foo"), DOM_DOMException::NAMESPACE_ERR);     
        
        //Only prefix of Element and Attribute can be changed
        EXCEPTION_TEST(doc.setPrefix("foo"), DOM_DOMException::NAMESPACE_ERR);

        //Prefix of readonly Attribute can not be changed.
        //However, there is no way to create such DOM_Attribute for testing yet.
    }
    TESTEPILOG;


    //
    //  getElementsByTagName*
    //
    TESTPROLOG;
    {
        
        // Set up an initial (root element only) document.
        // 
        DOM_DOMImplementation impl;
        
        DOMString qName = "foo:docName";
        DOMString pubId = "pubId";
        DOMString sysId = "http://sysId";
        DOM_DocumentType dt = impl.createDocumentType(qName, pubId, sysId);
        
        DOMString docNSURI = "http://document.namespace";
        DOM_Document doc = impl.createDocument(docNSURI, qName, dt);
        DOM_Element rootEl = doc.getDocumentElement();

        //
        // Populate the document
        //
        DOM_Element ela = doc.createElementNS("http://nsa", "a:ela");  
        rootEl.appendChild(ela);
        DOM_Element elb = doc.createElementNS("http://nsb", "elb");   
        rootEl.appendChild(elb);
        DOM_Element elc = doc.createElementNS("",           "elc");  
        rootEl.appendChild(elc);
        DOM_Element eld = doc.createElementNS("http://nsa", "d:ela");
        rootEl.appendChild(eld);
        DOM_Element ele = doc.createElementNS("http://nse", "elb");   
        rootEl.appendChild(ele);


        // 
        // Access with DOM Level 1 getElementsByTagName
        //

        DOM_NodeList nl;

        nl = doc.getElementsByTagName("a:ela");
        TASSERT(nl.getLength() == 1);
        TASSERT(nl.item(0) == ela);

        nl = doc.getElementsByTagName("elb");
        TASSERT(nl.getLength() == 2);
        TASSERT(nl.item(0) == elb);
        TASSERT(nl.item(1) == ele);

        nl = doc.getElementsByTagName("d:ela");
        TASSERT(nl.getLength() == 1);
        TASSERT(nl.item(0) == eld);

        //
        //  Access with DOM Level 2 getElementsByTagNameNS
        //

        nl = doc.getElementsByTagNameNS("", "elc");
        TASSERT(nl.getLength() == 1);
        TASSERT(nl.item(0) == elc);

        nl = doc.getElementsByTagNameNS(0, "elc");
        TASSERT(nl.getLength() == 1);
        TASSERT(nl.item(0) == elc);
       
        nl = doc.getElementsByTagNameNS("http://nsa", "ela");
        TASSERT(nl.getLength() == 2);
        TASSERT(nl.item(0) == ela);
        TASSERT(nl.item(1) == eld);

        nl = doc.getElementsByTagNameNS("", "elb");
        TASSERT(nl.getLength() == 0);

        nl = doc.getElementsByTagNameNS("http://nsb", "elb");
        TASSERT(nl.getLength() == 1);
        TASSERT(nl.item(0) == elb);

        nl = doc.getElementsByTagNameNS("*", "elb");
        TASSERT(nl.getLength() == 2);
        TASSERT(nl.item(0) == elb);
        TASSERT(nl.item(1) == ele);

        nl = doc.getElementsByTagNameNS("http://nsa", "*");
        TASSERT(nl.getLength() == 2);
        TASSERT(nl.item(0) == ela);
        TASSERT(nl.item(1) == eld);

        nl = doc.getElementsByTagNameNS("*", "*");
        TASSERT(nl.getLength() == 6);     // Gets the document root element, plus 5 more

        TASSERT(nl.item(6) == 0);
        // TASSERT(nl.item(-1) == 0);

        nl = rootEl.getElementsByTagNameNS("*", "*");
        TASSERT(nl.getLength() == 5);


        nl = doc.getElementsByTagNameNS("http://nsa", "d:ela");
        TASSERT(nl.getLength() == 0);


        //
        // Node lists are Live
        //

        nl = doc.getElementsByTagNameNS("*", "*");
        DOM_NodeList nla = ela.getElementsByTagNameNS("*", "*");

        TASSERT(nl.getLength() == 6); 
        TASSERT(nla.getLength() == 0);

        rootEl.removeChild(elc);
        TASSERT(nl.getLength() == 5);
        TASSERT(nla.getLength() == 0);

        ela.appendChild(elc);
        TASSERT(nl.getLength() == 6);
        TASSERT(nla.getLength() == 1);
    }
    TESTEPILOG;


   //
    // Attributes and NamedNodeMaps.
    //
    TESTPROLOG;
    {

        // Set up an initial (root element only) document.
        // 
        DOM_DOMImplementation impl;
        
        DOMString qName = "foo:docName";
        DOMString pubId = "pubId";
        DOMString sysId = "http://sysId";
        DOM_DocumentType dt = impl.createDocumentType(qName, pubId, sysId);
        
        DOMString docNSURI = "http://document.namespace";
        DOM_Document doc = impl.createDocument(docNSURI, qName, dt);
        DOM_Element rootEl = doc.getDocumentElement();

        //
        // Create a set of attributes and hang them on the root element.
        //
        DOM_Attr attra = doc.createAttributeNS("http://nsa", "a:attra");  
        rootEl.setAttributeNodeNS(attra);
        DOM_Attr attrb = doc.createAttributeNS("http://nsb", "attrb");   
        rootEl.setAttributeNodeNS(attrb);
        DOM_Attr attrc = doc.createAttributeNS("",           "attrc");  
        rootEl.setAttributeNodeNS(attrc);
        DOM_Attr attrd = doc.createAttributeNS("http://nsa", "d:attra");
        rootEl.setAttributeNodeNS(attrd);
        DOM_Attr attre = doc.createAttributeNS("http://nse", "attrb");   
        rootEl.setAttributeNodeNS(attre);

        //
        // Check that the attribute nodes were created with the correct properties.
        //
        TASSERT(attra.getNodeName().equals("a:attra"));
        TASSERT(attra.getNamespaceURI().equals("http://nsa"));
        TASSERT(attra.getLocalName().equals("attra"));
        TASSERT(attra.getName().equals("a:attra"));
        TASSERT(attra.getNodeType() == DOM_Node::ATTRIBUTE_NODE);
        TASSERT(attra.getNodeValue().equals(""));
        TASSERT(attra.getPrefix().equals("a"));
        TASSERT(attra.getSpecified() == true);
        TASSERT(attra.getValue().equals(""));
        TASSERT(attra.getOwnerElement() == 0);

        // Test methods of NamedNodeMap
        DOM_NamedNodeMap nnm = rootEl.getAttributes();
        TASSERT(nnm.getLength() == 4);
        TASSERT(nnm.getNamedItemNS("http://nsa", "attra") == attrd);
        TASSERT(nnm.getNamedItemNS("http://nsb", "attrb") == attrb);
        TASSERT(nnm.getNamedItemNS("http://nse", "attrb") == attre);
        TASSERT(nnm.getNamedItemNS("", "attrc") == attrc);
        TASSERT(nnm.getNamedItemNS("", "attra") == 0);
        TASSERT(nnm.getNamedItemNS("http://nsa", "attrb") == 0);
    }
    TESTEPILOG;

    //
    // 
    //

}



//---------------------------------------------------------------------------------------
//
//   main
//
//---------------------------------------------------------------------------------------
int  main()
{
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        char *pMessage = XMLString::transcode(toCatch.getMessage());
        fprintf(stderr, "Error during XMLPlatformUtils::Initialize(). \n"
            "  Message is: %s\n", pMessage);
        delete [] pMessage;
        return -1;
    }
    
    
    DOMStringTests();
    DOMBasicTests();
    DOMNSTests();

    //
    //  Print Final allocation stats for full set of tests
    //
    XMLPlatformUtils::Terminate();
    DomMemDebug().print();
    return 0;

};



