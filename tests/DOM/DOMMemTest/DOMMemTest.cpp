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

//
//  Various DOM tests.
//     Contents include
//       1.  Basic functionality for DOMString
//       2.  Regression tests for bugs fixed.
//     All individual are wrapped in a memory leak checker.
//
//     This is NOT a complete test of DOM functionality.
//

/**
 * $Log$
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


#define EXCEPTION_TEST(operation, expected_exception) \
    try {                                \
    operation;                           \
    printf(" Error: no exception thrown at line %d\n", __LINE__);                      \
}                                        \
catch (DOM_DOMException &e) {            \
    if (e.ExceptionCode != expectedException) \
        printf(" Wrong exception code: %d at line %d\n", e.ExceptionCode, __LINE__); \
 };   \
        \
    catch (...)   {                      \
        printf(" Wrong exception thrown at line %d\n", __LINE__); \
}




int  main()
{
    DomMemDebug     entryMemState, exitMemState;

	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) {
        char *pMessage = XMLString::transcode(toCatch.getMessage());
        fprintf(stderr, "Error during XMLPlatformUtils::Initialize(). \n"
                        "  Message is: %s\n", pMessage);
        delete pMessage;
        return -1;
    }


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

        XMLCh a[] = {'H', 'e', 'l', 'l', 'o', 0};
        DOMString x(a);
        DOMString y = "Hello";

        TASSERT(x.equals(y));
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
    //
    TESTPROLOG;
    {
        DOMString a = "Test 07";
        DOMString b = "append";

        a.appendData(b);
        TASSERT(a.equals("Test 07append"));
        TASSERT(b.equals("append"));
    }
    TESTEPILOG


    //
    //  Test 8 Append Data, with plenty of extra space in buffer.
    //
    TESTPROLOG;
    {
        DOMString a(100);
        DOMString b("Test 08");
        DOMString c("append");

        // TASSERT(a == 0);  // (should it?)
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
        DOMString a(100);
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
    TESTEPILOG


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
        nt1.setPublicId("Notation01PublicId");
        notationMap.setNamedItem (nt1);
        DOM_Notation    nt2 = (DOM_Notation &)notationMap.getNamedItem("Notation01");
        TASSERT(nt1==nt2);
        nt2 = DOM_Notation();
        nt1 = 0;
        nt2 = (DOM_Notation &)notationMap.getNamedItem("Notation01");
      

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

        DOM_Element     cloned = (DOM_Element &)root.cloneNode(true);
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
	// Minimal test of DOMString::transcode()
	//
	TESTPROLOG;
	{
		static char testStr[] = "This is our test string.";

		DOMString DOMTestStr = testStr;
		char *roundTripString = DOMTestStr.transcode();
		TASSERT(strcmp(testStr, roundTripString) == 0);
		delete roundTripString;
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
    //  DOM Level 2 tests.  These should be split out as a separate test.
    //


    //
    // hasFeature.  The set of supported options tested here is for Xerces 1.1
    //
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
        DOMString intSubSet = "Internal subsets are not parsed by this call!";
        
        DOM_DocumentType dt = impl.createDocumentType(qName, pubId, sysId, intSubSet);
        
        TASSERT(dt != 0);
        TASSERT(dt.getNodeType() == DOM_Node::DOCUMENT_TYPE_NODE);
        TASSERT(dt.getNodeName().equals(qName));
        TASSERT(dt.getPublicID().equals(pubId));
        TASSERT(dt.getSystemID().equals(sysId));
        TASSERT(dt.getInternalSubset().equals(intSubSet));
        
        DOM_NamedNodeMap nnm = dt.getEntities();
        TASSERT(nnm.getLength() == 0);
        nnm = dt.getNotations();
        TASSERT(nnm.getLength() == 0);
    }
    TESTEPILOG;
    

    //
    //  DOMImplementation::CreateDocument
    //
    TESTPROLOG;
    {
        
        DOM_DOMImplementation impl;
        
        DOMString qName = "foo:docName";
        DOMString pubId = "pubId";
        DOMString sysId = "http://sysId";
        DOMString intSubSet = "Internal subsets are not parsed by this call!";
        
        DOM_DocumentType dt = impl.createDocumentType(qName, pubId, sysId, intSubSet);
        
        DOMString docNSURI = "http://document.namespace";
        DOM_Document doc = impl.createDocument(docNSURI, qName, dt);

        TASSERT(doc.getNodeType() == DOM_Node::DOCUMENT_NODE);
        TASSERT(doc.getDoctype() == dt);
        TASSERT(doc.getNodeName().equals("#document"));
        TASSERT(doc.getNodeValue() == 0);

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
        DOMString intSubSet = "Internal subsets are not parsed by this call!";
        DOM_DocumentType dt = impl.createDocumentType(qName, pubId, sysId, intSubSet);
        
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
        //EXCEPTION_TEST(doc.createElementNS("http://nsa", "a:a:a"), DOM_DOMException::NAMESPACE_ERR);     

        // Prefix == xml, namespace != http://www.w3.org/XML/1998/namespace
        //EXCEPTION_TEST(doc.createElementNS("http://nsa", "xml:a", DOM_DOMException::NAMESPACE_ERR));     

        // A couple of corner cases that should not fail.
        TASSERT(doc.createElementNS("http://www.w3.org/XML/1998/namespace", "xml:a") != 0);
        TASSERT(doc.createElementNS("http://www.w3.org/XML/1998/namespace", "")      != 0);
        TASSERT(doc.createElementNS("http://www.w3.org/XML/1998/namespace", 0)    != 0);




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
        DOMString intSubSet = "Internal subsets are not parsed by this call!";
        DOM_DocumentType dt = impl.createDocumentType(qName, pubId, sysId, intSubSet);
        
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

        TASSERT(attrb.getNodeName().equals("attrb"));
        TASSERT(attrb.getNamespaceURI().equals("http://nsb"));
        TASSERT(attrb.getPrefix().equals(""));
        TASSERT(attrb.getLocalName().equals("attrb"));
        TASSERT(attrb.getName().equals("attrb"));

        TASSERT(attrc.getNodeName().equals("attrc"));
        TASSERT(attrc.getNamespaceURI().equals(""));
        TASSERT(attrc.getPrefix().equals(""));
        TASSERT(attrc.getLocalName().equals("attrc"));
        TASSERT(attrc.getName().equals("attrc"));

    }
    TESTEPILOG;


    //
    //  getElementsByTagNameNS
    //
    TESTPROLOG;
    {
        
        // Set up an initial (root element only) document.
        // 
        DOM_DOMImplementation impl;
        
        DOMString qName = "foo:docName";
        DOMString pubId = "pubId";
        DOMString sysId = "http://sysId";
        DOMString intSubSet = "Internal subsets are not parsed by this call!";
        DOM_DocumentType dt = impl.createDocumentType(qName, pubId, sysId, intSubSet);
        
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


    }
    TESTEPILOG;




    //
    //  Print Final allocation stats for full test
    //
    DomMemDebug().print();
    
    return 0;
    };
    
