/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2002, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

//
//  Various IDOM tests.
//     This is NOT a complete test of IDOM functionality.
//

/*
 * $Id
 */

#include <stdio.h>
#include <string.h>
#include <xercesc/idom/IDOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>


#define TASSERT(c) tassert((c), __FILE__, __LINE__)

void tassert(bool c, const char *file, int line)
{
    if (!c)
        printf("Failure.  Line %d,   file %s\n", line, file);
};


#define EXCEPTION_TEST(operation, expected_exception)               \
{                                                                   \
    try {                                                           \
    operation;                                                      \
    printf(" Error: no exception thrown at line %d\n", __LINE__);   \
}                                                                   \
    catch (IDOM_DOMException &e) {                                  \
    if (e.code != expected_exception)                       \
    printf(" Wrong exception code: %d at line %d\n", e.code, __LINE__); \
}                                                                 \
    catch (...)   {                                                 \
    printf(" Wrong exception thrown at line %d\n", __LINE__);       \
}                                                                   \
}


//temp XMLCh String Buffer
XMLCh tempStr[4000];
XMLCh tempStr2[4000];
XMLCh tempStr3[4000];
XMLCh tempStr4[4000];
XMLCh tempStr5[4000];

//---------------------------------------------------------------------------------------
//
//   IDOMBasicTests    Basic IDOM Level 1 tests
//
//---------------------------------------------------------------------------------------
void IDOMBasicTests()
{
    //
    //  Test Doc01      Create a new empty document
    //
    {
        //  First precondition, so that lazily created strings do not appear
        //  as memory leaks.
        IDOM_Document*   doc;
        doc = IDOM_DOMImplementation::getImplementation()->createDocument();
        delete doc;
    }

    //
    //  Test Doc02      Create one of each kind of node using the
    //                  document createXXX methods.
    //                  Watch for memory leaks.
    //
    {
        //  Do all operations in a preconditioning step, to force the
        //  creation of implementation objects that are set up on first use.
        //  Don't watch for leaks in this block (no  / )
        IDOM_Document* doc = IDOM_DOMImplementation::getImplementation()->createDocument();

        XMLString::transcode("Doc02Element", tempStr, 3999);
        IDOM_Element*   el = doc->createElement(tempStr);

        IDOM_DocumentFragment* frag = doc->createDocumentFragment ();

        XMLString::transcode("Doc02TextNode", tempStr, 3999);
        IDOM_Text*  text = doc->createTextNode(tempStr);

        XMLString::transcode("Doc02Comment", tempStr, 3999);
        IDOM_Comment* comment = doc->createComment(tempStr);

        XMLString::transcode("Doc02CDataSection", tempStr, 3999);
        IDOM_CDATASection*  cdataSec = doc->createCDATASection(tempStr);

        XMLString::transcode("Doc02DocumentType", tempStr, 3999);
        IDOM_DocumentType*  docType = doc->createDocumentType(tempStr);

        XMLString::transcode("Doc02Notation", tempStr, 3999);
        IDOM_Notation* notation = doc->createNotation(tempStr);

        XMLString::transcode("Doc02PITarget", tempStr, 3999);
        XMLString::transcode("Doc02PIData", tempStr2, 3999);
        IDOM_ProcessingInstruction* pi = doc->createProcessingInstruction(tempStr, tempStr2);

        XMLString::transcode("*", tempStr, 3999);
        IDOM_NodeList*    nodeList = doc->getElementsByTagName(tempStr);

        delete doc;
    }



    //
    //  Doc03 - Create a small document tree
    //

    {
        IDOM_Document*   doc = IDOM_DOMImplementation::getImplementation()->createDocument();

        XMLString::transcode("Doc03RootElement", tempStr, 3999);
        IDOM_Element*   rootEl = doc->createElement(tempStr);

        doc->appendChild(rootEl);

        XMLString::transcode("Doc03 text stuff", tempStr, 3999);
        IDOM_Text*       textNode = doc->createTextNode(tempStr);

        rootEl->appendChild(textNode);

        XMLString::transcode("*", tempStr, 3999);
        IDOM_NodeList*    nodeList = doc->getElementsByTagName(tempStr);
        delete doc;
    };



    //
    //  Attr01
    //
    {
        IDOM_Document*   doc = IDOM_DOMImplementation::getImplementation()->createDocument();

        XMLString::transcode("RootElement", tempStr, 3999);
        IDOM_Element*   rootEl  = doc->createElement(tempStr);

        doc->appendChild(rootEl);
        {
            XMLString::transcode("Attr01", tempStr, 3999);
            IDOM_Attr*        attr01  = doc->createAttribute(tempStr);
            rootEl->setAttributeNode(attr01);
        }


        {
            XMLString::transcode("Attr01", tempStr, 3999);
            IDOM_Attr* attr02 = doc->createAttribute(tempStr);
            rootEl->setAttributeNode(attr02);
        }
        delete doc;

    };

    //
    //  Attr02
    //

    {
        IDOM_Document*   doc = IDOM_DOMImplementation::getImplementation()->createDocument();

        XMLString::transcode("RootElement", tempStr, 3999);
        IDOM_Element*   rootEl  = doc->createElement(tempStr);

        doc->appendChild(rootEl);

        XMLString::transcode("Attr02", tempStr, 3999);
        IDOM_Attr*        attr01  = doc->createAttribute(tempStr);

        rootEl->setAttributeNode(attr01);

        IDOM_Attr*        attr02 = doc->createAttribute(tempStr);

        rootEl->setAttributeNode(attr02);
        delete doc;
    }



    //
    //  Attr03
    //

    {
        IDOM_Document*   doc = IDOM_DOMImplementation::getImplementation()->createDocument();

        XMLString::transcode("RootElement", tempStr, 3999);
        IDOM_Element*   rootEl  = doc->createElement(tempStr);

        doc->appendChild(rootEl);

        XMLString::transcode("Attr03", tempStr, 3999);
        IDOM_Attr*        attr01  = doc->createAttribute(tempStr);

        rootEl->setAttributeNode(attr01);

        XMLString::transcode("Attr03Value1", tempStr, 3999);
        attr01->setValue(tempStr);

        XMLString::transcode("Attr03Value2", tempStr, 3999);
        attr01->setValue(tempStr);
        delete doc;
    }



    //
    //  Attr04
    //

    {
        IDOM_Document*   doc = IDOM_DOMImplementation::getImplementation()->createDocument();

        XMLString::transcode("RootElement", tempStr, 3999);
        IDOM_Element*   rootEl  = doc->createElement(tempStr);

        doc->appendChild(rootEl);

        XMLString::transcode("Attr04", tempStr, 3999);
        IDOM_Attr*        attr01  = doc->createAttribute(tempStr);

        rootEl->setAttributeNode(attr01);

        XMLString::transcode("Attr04Value1", tempStr, 3999);
        attr01->setValue(tempStr);

        IDOM_Node* value = attr01->getFirstChild();
        delete doc;
    }




    //
    //  Text01
    //

    {
        IDOM_Document*   doc = IDOM_DOMImplementation::getImplementation()->createDocument();

        XMLString::transcode("RootElement", tempStr, 3999);
        IDOM_Element*   rootEl  = doc->createElement(tempStr);

        doc->appendChild(rootEl);

        XMLString::transcode("Hello Goodbye", tempStr, 3999);
        IDOM_Text*        txt1 = doc->createTextNode(tempStr);
        rootEl->appendChild(txt1);

        txt1->splitText(6);
        rootEl->normalize();
        delete doc;

    }



    //
    //  Notation01
    //

    {
        IDOM_Document*       doc = IDOM_DOMImplementation::getImplementation()->createDocument();

        XMLString::transcode("DocType_for_Notation01", tempStr, 3999);
        IDOM_DocumentType*   dt  = doc->createDocumentType(tempStr);

        doc->appendChild(dt);


        IDOM_NamedNodeMap* notationMap = dt->getNotations();

        XMLString::transcode("Notation01", tempStr, 3999);
        IDOM_Notation*    nt1 = doc->createNotation(tempStr);

        notationMap->setNamedItem (nt1);

        IDOM_Node*  abc1 = notationMap->getNamedItem(tempStr);

        IDOM_Notation*    nt2 = (IDOM_Notation*) abc1;
        TASSERT(nt1==nt2);
        nt2 = 0;
        nt1 = 0;

        IDOM_Node* abc6 = notationMap->getNamedItem(tempStr);

        nt2 = (IDOM_Notation*) abc6;
        delete doc;
    }



    //
    //  NamedNodeMap01 - comparison operators.
    //

    {
        IDOM_NamedNodeMap*    nnm = 0;
        TASSERT(nnm == 0);

        IDOM_Document*       doc = IDOM_DOMImplementation::getImplementation()->createDocument();
        nnm = doc->getAttributes();    // Should be null, because node type
                                      //   is not Element.
        TASSERT(nnm == 0);
        TASSERT(!(nnm != 0));

        XMLString::transcode("NamedNodeMap01", tempStr, 3999);
        IDOM_Element* el = doc->createElement(tempStr);

        IDOM_NamedNodeMap* nnm2 = el->getAttributes();    // Should be an empty, but non-null map.
        TASSERT(nnm2 != 0);
        TASSERT(nnm != nnm2);
        nnm = nnm2;
        TASSERT(nnm == nnm2);
        delete doc;
    }



    //
    //  importNode quick test
    //

    {
        IDOM_Document*   doc1 = IDOM_DOMImplementation::getImplementation()->createDocument();
        IDOM_Document*   doc2 = IDOM_DOMImplementation::getImplementation()->createDocument();

        XMLString::transcode("abc", tempStr, 3999);
        IDOM_Element*   el1  = doc1->createElement(tempStr);

        doc1->appendChild(el1);
        TASSERT(el1->getParentNode() != 0);
        IDOM_Node*        el2  = doc2->importNode(el1, true);
        TASSERT(el2->getParentNode() == 0);
        const XMLCh*       tagName = el2->getNodeName();

        TASSERT(!XMLString::compareString(tagName, tempStr));

        TASSERT(el2->getOwnerDocument() == doc2);
        TASSERT(doc1 != doc2);
        delete doc1;
        delete doc2;
    }


    //
    //  getLength() tests.  Both Node CharacterData and NodeList implement
    //                  getLength().  Early versions of the IDOM had a clash
    //                  between the two, originating in the implementation class
    //                  hirearchy, which has NodeList as a (distant) base class
    //                  of CharacterData.  This is a regression test to verify
    //                  that the problem stays fixed.
    //

    {
        IDOM_Document*    doc = IDOM_DOMImplementation::getImplementation()->createDocument();

        XMLString::transcode("Hello", tempStr, 3999);
        IDOM_Text*          tx = doc->createTextNode(tempStr);

        XMLString::transcode("abc", tempStr, 3999);
        IDOM_Element*     el = doc->createElement(tempStr);

        el->appendChild(tx);

        int     textLength = tx->getLength();
        TASSERT(textLength == 5);

        IDOM_NodeList*      nl = tx->getChildNodes();
        int      nodeListLen = nl->getLength();
        TASSERT(nodeListLen == 0);

        nl = el->getChildNodes();
        nodeListLen = nl->getLength();
        TASSERT(nodeListLen == 1);
        delete doc;
    }


    //
    //  NodeList - comparison operators, basic operation.
    //

    {
        IDOM_NodeList*    nl = 0;
        IDOM_NodeList*    nl2 = 0;
        TASSERT(nl == 0);
        TASSERT(!(nl != 0));
        TASSERT(nl == nl2);

        IDOM_Document*       doc = IDOM_DOMImplementation::getImplementation()->createDocument();
        nl = doc->getChildNodes();    // Should be non-null, but empty

        TASSERT(nl != 0);
        int len = nl->getLength();
        TASSERT(len == 0);

        XMLString::transcode("NodeList01", tempStr, 3999);
        IDOM_Element* el = doc->createElement(tempStr);

        doc->appendChild(el);
        len = nl->getLength();
        TASSERT(len == 1);
        TASSERT(nl != nl2);
        nl2 = nl;
        TASSERT(nl == nl2);
        delete doc;
    }




    //
    //  Name validity checking.
    //

    {
         IDOM_Document*       doc = IDOM_DOMImplementation::getImplementation()->createDocument();
         try
         {
             XMLString::transcode("!@@ bad element name", tempStr, 3999);
             IDOM_Element* el = doc->createElement(tempStr);
             TASSERT(false);  // Exception above should prevent us reaching here.
         }
         catch ( IDOM_DOMException e)
         {
             TASSERT(e.code == IDOM_DOMException::INVALID_CHARACTER_ERR);
         }
         catch (...)
         {
             TASSERT(false);  // Wrong exception thrown.
         }
         delete doc;
    }



    //
    //  Assignment ops return value
    //

    {
        IDOM_Document*       doc = IDOM_DOMImplementation::getImplementation()->createDocument();

        XMLString::transcode("NodeList01", tempStr, 3999);
        IDOM_Element* el = doc->createElement(tempStr);

        doc->appendChild(el);

        IDOM_Element* n1, *n2, *n3;

        n1 = n2 = n3 = el;
        TASSERT(n1 == n2);
        TASSERT(n1 == n3);
        TASSERT(n1 == el);
        TASSERT(n1 != 0);
        n1 = n2 = n3 = 0;
        TASSERT(n1 == 0);
        delete doc;
    }



    //
    //  Cloning of a node with attributes. Regression test for a ref counting
    //  bug in attributes of cloned nodes that occured when the "owned" flag
    //  was not set in the clone.
    //

    {
        IDOM_Document*   doc = IDOM_DOMImplementation::getImplementation()->createDocument();

        XMLString::transcode("CTestRoot", tempStr, 3999);
        IDOM_Element*   root = doc->createElement(tempStr);

        XMLString::transcode("CTestAttr", tempStr, 3999);
        XMLString::transcode("CTestAttrValue", tempStr2, 3999);
        root->setAttribute(tempStr, tempStr2);

        const XMLCh* s = root->getAttribute(tempStr);
        TASSERT(!XMLString::compareString(s, tempStr2));

        IDOM_Node* abc2 = root->cloneNode(true);
        IDOM_Element*   cloned = (IDOM_Element*) abc2;
        IDOM_Attr* a = cloned->getAttributeNode(tempStr);
        TASSERT(a != 0);
        s = a->getValue();
        TASSERT(!XMLString::compareString(s, tempStr2));
        a = 0;

        a = cloned->getAttributeNode(tempStr);
        TASSERT(a != 0);
        s = a->getValue();
        TASSERT(!XMLString::compareString(s, tempStr2));
        delete doc;

    }



    //
    //  splitText()
    //     Regression test for a bug from Tinny Ng
    //

    {
        IDOM_Document* doc;
        doc = IDOM_DOMImplementation::getImplementation()->createDocument();
        IDOM_Text* tn, *tn1, *tn2;

        XMLString::transcode("0123456789", tempStr, 3999);
        tn = doc->createTextNode (tempStr);

        tn1 = tn->splitText(5);

        XMLString::transcode("01234", tempStr, 3999);
        TASSERT(!XMLString::compareString(tn->getNodeValue(), tempStr));

        XMLString::transcode("56789", tempStr2, 3999);
        TASSERT(!XMLString::compareString(tn1->getNodeValue(), tempStr2));


        tn2 = tn->splitText(5);
        TASSERT(!XMLString::compareString(tn->getNodeValue(), tempStr));

        XMLString::transcode("", tempStr2, 3999);
        TASSERT(!XMLString::compareString(tn2->getNodeValue(), tempStr2));

        EXCEPTION_TEST(tn->splitText(6), IDOM_DOMException::INDEX_SIZE_ERR);
        delete doc;
    }



}


//---------------------------------------------------------------------------------------
//
//   IDOMNSTests    IDOM Name Space tests
//
//---------------------------------------------------------------------------------------
void IDOMNSTests()
{

    //
    //  IDOM Level 2 tests.  These should be split out as a separate test.
    //


    //
    // hasFeature.  The set of supported options tested here is for Xerces 1.1
    //              Note: because the implementation lazily creates some of the comprison
    //                    strings within the implementation, this test must be pre-flighted
    //                    outside of the TESPROLOG/ macros to avoid spurious
    //                    reports of memory leaks.
    //
    // Also test the case-insensitive
    //
    {

        IDOM_DOMImplementation*  impl = IDOM_DOMImplementation::getImplementation();
        XMLString::transcode("1.0", tempStr, 3999);
        XMLString::transcode("2.0", tempStr2, 3999);
        XMLString::transcode("3.0", tempStr3, 3999);
        XMLString::transcode("", tempStr4, 3999);

        XMLString::transcode("XmL", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr2) == true);

        XMLString::transcode("xML", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4) == true);

        XMLString::transcode("XMl", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr)  == true);

        XMLString::transcode("xMl", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr3)  == false);

        XMLString::transcode("TrAveRsal", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)  == true);
    }



    {
        IDOM_DOMImplementation*  impl = IDOM_DOMImplementation::getImplementation();
        XMLString::transcode("1.0", tempStr, 3999);
        XMLString::transcode("2.0", tempStr2, 3999);
        XMLString::transcode("3.0", tempStr3, 3999);
        XMLString::transcode("", tempStr4, 3999);

        XMLString::transcode("XmL", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == true);
        TASSERT(impl->hasFeature(tempStr5, tempStr)    == true);
        TASSERT(impl->hasFeature(tempStr5, tempStr2)   == true);
        TASSERT(impl->hasFeature(tempStr5, tempStr3)   == false);

        XMLString::transcode("Core", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == true);

        XMLString::transcode("coRe", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr)    == true);

        XMLString::transcode("core", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr2)   == true);

        XMLString::transcode("cORe", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr3)   == false);

        XMLString::transcode("Traversal", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == true);

        XMLString::transcode("traversal", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr)    == false);

        XMLString::transcode("TraVersal", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr2)   == true);

        XMLString::transcode("Range", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == true);

        XMLString::transcode("raNge", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr)    == false);

        XMLString::transcode("RaNge", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr2)   == true);


        XMLString::transcode("HTML", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == false);

        XMLString::transcode("Views", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == false);

        XMLString::transcode("StyleSheets", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == false);

        XMLString::transcode("CSS", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == false);

        XMLString::transcode("CSS2", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == false);

        XMLString::transcode("Events", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == false);

        XMLString::transcode("UIEvents", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == false);

        XMLString::transcode("MouseEvents", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == false);

        XMLString::transcode("MutationEvents", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == false);

        XMLString::transcode("HTMLEvents", tempStr5, 3999);
        TASSERT(impl->hasFeature(tempStr5, tempStr4)   == false);
    }



    //
    // isSupported test (similar to hasFeature)
    //

    {
        IDOM_Document* doc;
        doc = IDOM_DOMImplementation::getImplementation()->createDocument();

        XMLString::transcode("1.0", tempStr, 3999);
        XMLString::transcode("2.0", tempStr2, 3999);
        XMLString::transcode("3.0", tempStr3, 3999);
        XMLString::transcode("", tempStr4, 3999);

        XMLString::transcode("XmL", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == true);
        TASSERT(doc->isSupported(tempStr5, tempStr)    == true);
        TASSERT(doc->isSupported(tempStr5, tempStr2)   == true);
        TASSERT(doc->isSupported(tempStr5, tempStr3)   == false);

        XMLString::transcode("Core", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == true);

        XMLString::transcode("coRe", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr)    == true);

        XMLString::transcode("core", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr2)   == true);

        XMLString::transcode("cORe", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr3)   == false);

        XMLString::transcode("Traversal", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == true);

        XMLString::transcode("traversal", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr)    == false);

        XMLString::transcode("TraVersal", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr2)   == true);

        XMLString::transcode("Range", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == true);

        XMLString::transcode("raNge", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr)    == false);

        XMLString::transcode("RaNge", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr2)   == true);


        XMLString::transcode("HTML", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == false);

        XMLString::transcode("Views", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == false);

        XMLString::transcode("StyleSheets", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == false);

        XMLString::transcode("CSS", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == false);

        XMLString::transcode("CSS2", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == false);

        XMLString::transcode("Events", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == false);

        XMLString::transcode("UIEvents", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == false);

        XMLString::transcode("MouseEvents", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == false);

        XMLString::transcode("MutationEvents", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == false);

        XMLString::transcode("HTMLEvents", tempStr5, 3999);
        TASSERT(doc->isSupported(tempStr5, tempStr4)   == false);
        delete doc;
    }


    //
    // CreateDocumentType
    //

    {
        IDOM_DOMImplementation*  impl = IDOM_DOMImplementation::getImplementation();

        XMLString::transcode("foo:docName", tempStr, 3999);
        XMLString::transcode("pubId", tempStr2, 3999);
        XMLString::transcode( "http://sysId", tempStr3, 3999);

        IDOM_DocumentType* dt = impl->createDocumentType(tempStr, tempStr2, tempStr3);

        TASSERT(dt != 0);
        TASSERT(dt->getNodeType() == IDOM_Node::DOCUMENT_TYPE_NODE);
        TASSERT(!XMLString::compareString(dt->getNodeName(), tempStr));
        TASSERT(dt->getNamespaceURI() == 0);
        TASSERT(dt->getPrefix() == 0);
        TASSERT(dt->getLocalName() == 0);
        TASSERT(!XMLString::compareString(dt->getPublicId(), tempStr2));
        TASSERT(!XMLString::compareString(dt->getSystemId(), tempStr3));
        TASSERT(dt->getInternalSubset() == 0);
        TASSERT(dt->getOwnerDocument() == 0);

        IDOM_NamedNodeMap* nnm = dt->getEntities();
        TASSERT(nnm->getLength() == 0);
        nnm = dt->getNotations();
        TASSERT(nnm->getLength() == 0);
        delete dt;

        //
        // Qualified name without prefix should also work.
        //
        XMLString::transcode("docName", tempStr, 3999);
        dt = impl->createDocumentType(tempStr, tempStr2, tempStr3);

        TASSERT(dt != 0);
        TASSERT(dt->getNodeType() == IDOM_Node::DOCUMENT_TYPE_NODE);
        TASSERT(!XMLString::compareString(dt->getNodeName(), tempStr));
        TASSERT(dt->getNamespaceURI() == 0);
        TASSERT(dt->getPrefix() == 0);
        TASSERT(dt->getLocalName() == 0);
        TASSERT(!XMLString::compareString(dt->getPublicId(), tempStr2));
        TASSERT(!XMLString::compareString(dt->getSystemId(), tempStr3));
        TASSERT(dt->getInternalSubset() == 0);
        TASSERT(dt->getOwnerDocument() == 0);

        // Creating a DocumentType with invalid or malformed qName should fail.
        XMLString::transcode("<docName", tempStr, 3999);
        EXCEPTION_TEST(impl->createDocumentType(tempStr, tempStr2, tempStr3), IDOM_DOMException::INVALID_CHARACTER_ERR);

        XMLString::transcode(":docName", tempStr, 3999);
        EXCEPTION_TEST(impl->createDocumentType(tempStr, tempStr2, tempStr3), IDOM_DOMException::NAMESPACE_ERR);

        XMLString::transcode("docName:", tempStr, 3999);
        EXCEPTION_TEST(impl->createDocumentType(tempStr, tempStr2, tempStr3), IDOM_DOMException::NAMESPACE_ERR);

        XMLString::transcode("doc::Name", tempStr, 3999);
        EXCEPTION_TEST(impl->createDocumentType(tempStr, tempStr2, tempStr3), IDOM_DOMException::NAMESPACE_ERR);

        XMLString::transcode("doc:N:ame", tempStr, 3999);
        EXCEPTION_TEST(impl->createDocumentType(tempStr, tempStr2, tempStr3), IDOM_DOMException::NAMESPACE_ERR);
        delete dt;

    }



    //
    //  IDOMImplementation::CreateDocument
    {
        // Preflight the operations that will lazily create IDOMStrings
        // in the implementation.  This prevents incorrect reports of
        // memory leaks in the real test.

        IDOM_DOMImplementation*   impl = IDOM_DOMImplementation::getImplementation();
        IDOM_DocumentType*       dt = 0;

        XMLString::transcode("", tempStr2, 3999);
        XMLString::transcode("a", tempStr, 3999);
        IDOM_Document*           doc = impl->createDocument(tempStr2, tempStr, dt);

        doc->getNodeName();
        delete doc;
    }

    //

    {

        IDOM_DOMImplementation* impl = IDOM_DOMImplementation::getImplementation();

        XMLString::transcode("foo:docName", tempStr3, 3999);
        XMLString::transcode("pubId", tempStr2, 3999);
        XMLString::transcode( "http://sysId", tempStr, 3999);


        IDOM_DocumentType* dt = impl->createDocumentType(tempStr3, tempStr2, tempStr);

        XMLString::transcode("http://document.namespace", tempStr2, 3999);

        IDOM_Document* doc = impl->createDocument(tempStr2, tempStr3, dt);

        TASSERT(dt->getOwnerDocument() == doc);
        TASSERT(doc->getOwnerDocument() == 0);

        TASSERT(doc->getNodeType() == IDOM_Node::DOCUMENT_NODE);
        TASSERT(doc->getDoctype() == dt);

        XMLString::transcode("#document", tempStr, 3999);
        TASSERT(!XMLString::compareString(doc->getNodeName(), tempStr));

        TASSERT(doc->getNodeValue() == 0);
        TASSERT(doc->getNamespaceURI() == 0);
        TASSERT(doc->getPrefix() == 0);
        TASSERT(doc->getLocalName() == 0);

        IDOM_Element* el = doc->getDocumentElement();

        XMLString::transcode("docName", tempStr, 3999);
        TASSERT(!XMLString::compareString(el->getLocalName(), tempStr));

        TASSERT(!XMLString::compareString(el->getNamespaceURI(), tempStr2));
        TASSERT(!XMLString::compareString(el->getNodeName(), tempStr3));
        TASSERT(el->getOwnerDocument() == doc);
        TASSERT(el->getParentNode() == doc);

        XMLString::transcode("foo", tempStr, 3999);
        TASSERT(!XMLString::compareString(el->getPrefix(), tempStr));

        TASSERT(!XMLString::compareString(el->getTagName(), tempStr3));
        TASSERT(el->hasChildNodes() == false);
        delete doc;


        //
        // Creating a second document with the same docType object should fail.
        //
        try
        {
            IDOM_Document* doc2 = impl->createDocument(tempStr2, tempStr3, dt);
            TASSERT(false);  // should not reach here.
        }
        catch ( IDOM_DOMException &e)
        {
            TASSERT(e.code == IDOM_DOMException::WRONG_DOCUMENT_ERR);
        }
        catch (...)
        {
            TASSERT(false);  // Wrong exception thrown.
        }

        // Creating a document with null NamespaceURI and DocumentType
        doc = impl->createDocument(tempStr2, tempStr3, 0);

        // Namespace tests of createDocument are covered by createElementNS below
        delete doc;
        delete dt;
    }





    //
    //  CreateElementNS methods
    //

    {

        // Set up an initial (root element only) document.
        //
        IDOM_DOMImplementation* impl = IDOM_DOMImplementation::getImplementation();

        XMLString::transcode("foo:docName", tempStr3, 3999);
        XMLString::transcode("pubId", tempStr2, 3999);
        XMLString::transcode( "http://sysId", tempStr, 3999);


        IDOM_DocumentType* dt = impl->createDocumentType(tempStr3, tempStr2, tempStr);

        XMLString::transcode("http://document.namespace", tempStr2, 3999);

        IDOM_Document* doc = impl->createDocument(tempStr2, tempStr3, dt);
        IDOM_Element* rootEl = doc->getDocumentElement();

        //
        // CreateElementNS
        //
        XMLString::transcode("http://nsa", tempStr5, 3999);
        XMLString::transcode("a:ela", tempStr, 3999);
        IDOM_Element* ela = doc->createElementNS(tempStr5, tempStr);  // prefix and URI

        TASSERT(!XMLString::compareString(ela->getNodeName(), tempStr));
        TASSERT(!XMLString::compareString(ela->getNamespaceURI(), tempStr5));
        XMLString::transcode("a", tempStr3, 3999);
        TASSERT(!XMLString::compareString(ela->getPrefix(), tempStr3));
        XMLString::transcode("ela", tempStr3, 3999);
        TASSERT(!XMLString::compareString(ela->getLocalName(), tempStr3));
        TASSERT(!XMLString::compareString(ela->getTagName(), tempStr));


        XMLString::transcode("", tempStr3, 3999);
        XMLString::transcode("http://nsb", tempStr2, 3999);
        XMLString::transcode("elb", tempStr, 3999);
        IDOM_Element* elb = doc->createElementNS(tempStr2, tempStr);    //  URI, no prefix.

        TASSERT(!XMLString::compareString(elb->getNodeName(), tempStr));
        TASSERT(!XMLString::compareString(elb->getNamespaceURI(), tempStr2));
        TASSERT(!XMLString::compareString(elb->getPrefix(), tempStr3));
        TASSERT(!XMLString::compareString(elb->getLocalName(), tempStr));
        TASSERT(!XMLString::compareString(elb->getTagName(), tempStr));


        XMLString::transcode("elc", tempStr, 3999);
        IDOM_Element* elc = doc->createElementNS(tempStr3, tempStr);              // No URI, no prefix.

        TASSERT(!XMLString::compareString(elc->getNodeName(), tempStr));
        TASSERT(!XMLString::compareString(elc->getNamespaceURI(), tempStr3));
        TASSERT(!XMLString::compareString(elc->getPrefix(), tempStr3));
        TASSERT(!XMLString::compareString(elc->getLocalName(), tempStr));
        TASSERT(!XMLString::compareString(elc->getTagName(), tempStr));

        rootEl->appendChild(ela);
        rootEl->appendChild(elb);
        rootEl->appendChild(elc);

        // Badly formed qualified name
        XMLString::transcode("<a", tempStr4, 3999);
        EXCEPTION_TEST(doc->createElementNS(tempStr5, tempStr4), IDOM_DOMException::INVALID_CHARACTER_ERR);

        XMLString::transcode(":a", tempStr4, 3999);
        EXCEPTION_TEST(doc->createElementNS(tempStr5, tempStr4), IDOM_DOMException::NAMESPACE_ERR);

        XMLString::transcode("a:", tempStr4, 3999);
        EXCEPTION_TEST(doc->createElementNS(tempStr5, tempStr4), IDOM_DOMException::NAMESPACE_ERR);

        XMLString::transcode("a::a", tempStr4, 3999);
        EXCEPTION_TEST(doc->createElementNS(tempStr5, tempStr4), IDOM_DOMException::NAMESPACE_ERR);

        XMLString::transcode("a:a:a", tempStr4, 3999);
        EXCEPTION_TEST(doc->createElementNS(tempStr5, tempStr4), IDOM_DOMException::NAMESPACE_ERR);

        // xml:a must have namespaceURI == "http://www.w3.org/XML/1998/namespace"
        XMLString::transcode("http://www.w3.org/XML/1998/namespace", tempStr4, 3999);
        XMLString::transcode("xml:a",tempStr2, 3999);
        TASSERT(!XMLString::compareString(doc->createElementNS(tempStr4, tempStr2)->getNamespaceURI(), tempStr4));
        EXCEPTION_TEST(doc->createElementNS(tempStr5, tempStr2), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc->createElementNS(tempStr3, tempStr2), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc->createElementNS(0,  tempStr2), IDOM_DOMException::NAMESPACE_ERR);

        //unlike Attribute, xmlns (no different from foo) can have any namespaceURI for Element
        XMLString::transcode("xmlns", tempStr2, 3999);
        TASSERT(!XMLString::compareString(doc->createElementNS(tempStr5, tempStr2)->getNamespaceURI(), tempStr5));

        TASSERT(!XMLString::compareString(doc->createElementNS(tempStr4, tempStr2)->getNamespaceURI(), tempStr4));
        TASSERT(!XMLString::compareString(doc->createElementNS(tempStr3, tempStr2)->getNamespaceURI(), tempStr3));
        TASSERT(!XMLString::compareString(doc->createElementNS(0,  tempStr2)->getNamespaceURI(), tempStr3));

        //unlike Attribute, xmlns:a (no different from foo:a) can have any namespaceURI for Element
        //except "" and null
        XMLString::transcode("xmlns:a", tempStr2, 3999);
        TASSERT(!XMLString::compareString(doc->createElementNS(tempStr5, tempStr2)->getNamespaceURI(), tempStr5));
        TASSERT(!XMLString::compareString(doc->createElementNS(tempStr4, tempStr2)->getNamespaceURI(), tempStr4));
        EXCEPTION_TEST(doc->createElementNS(tempStr3, tempStr2), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc->createElementNS(0, tempStr2), IDOM_DOMException::NAMESPACE_ERR);

        //In fact, any prefix != null should have a namespaceURI != 0 or != ""
        XMLString::transcode("foo:a", tempStr2, 3999);
        TASSERT(!XMLString::compareString(doc->createElementNS(tempStr5, tempStr2)->getNamespaceURI(), tempStr5));
        EXCEPTION_TEST(doc->createElementNS(tempStr3, tempStr2), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc->createElementNS(0,  tempStr2), IDOM_DOMException::NAMESPACE_ERR);

        //Change prefix
        IDOM_Element* elem = doc->createElementNS(tempStr5, tempStr2);
        XMLString::transcode("bar", tempStr2, 3999);
        elem->setPrefix(tempStr2);
        XMLString::transcode("bar:a", tempStr4, 3999);
        TASSERT(!XMLString::compareString(elem->getNodeName(), tempStr4));
        TASSERT(!XMLString::compareString(elem->getNamespaceURI(), tempStr5));
        TASSERT(!XMLString::compareString(elem->getPrefix(), tempStr2));
        XMLString::transcode("a", tempStr, 3999);
        TASSERT(!XMLString::compareString(elem->getLocalName(), tempStr));
        TASSERT(!XMLString::compareString(elem->getTagName(), tempStr4));
        //The spec does not prevent us from setting prefix to a node without prefix
        elem = doc->createElementNS(tempStr5, tempStr);
        TASSERT(!XMLString::compareString(elem->getPrefix(), tempStr3));
        elem->setPrefix(tempStr2);
        TASSERT(!XMLString::compareString(elem->getNodeName(), tempStr4));
        TASSERT(!XMLString::compareString(elem->getNamespaceURI(), tempStr5));
        TASSERT(!XMLString::compareString(elem->getPrefix(), tempStr2));
        TASSERT(!XMLString::compareString(elem->getLocalName(), tempStr));
        TASSERT(!XMLString::compareString(elem->getTagName(), tempStr4));

        //Special case for xml:a where namespaceURI must be xmlURI
        XMLString::transcode("foo:a", tempStr, 3999);
        XMLString::transcode("http://www.w3.org/XML/1998/namespace", tempStr4, 3999);
        elem = doc->createElementNS(tempStr4, tempStr);
        XMLString::transcode("xml", tempStr2, 3999);
        elem->setPrefix(tempStr2);
        elem = doc->createElementNS(tempStr5, tempStr);
        EXCEPTION_TEST(elem->setPrefix(tempStr2), IDOM_DOMException::NAMESPACE_ERR);
        //However, there is no restriction on prefix xmlns
        XMLString::transcode("xmlns", tempStr4, 3999);
        elem->setPrefix(tempStr4);
        //Also an element can not have a prefix with namespaceURI == null or ""
        XMLString::transcode("a", tempStr, 3999);
        XMLString::transcode("foo", tempStr2, 3999);
        elem = doc->createElementNS(0, tempStr);
        EXCEPTION_TEST(elem->setPrefix(tempStr2), IDOM_DOMException::NAMESPACE_ERR);
        elem = doc->createElementNS(tempStr3, tempStr);
        EXCEPTION_TEST(elem->setPrefix(tempStr2), IDOM_DOMException::NAMESPACE_ERR);

        //Only prefix of Element and Attribute can be changed
        EXCEPTION_TEST(doc->setPrefix(tempStr2), IDOM_DOMException::NAMESPACE_ERR);

        //Prefix of readonly Element can not be changed.
        //However, there is no way to create such IDOM_Element* for testing yet.
        delete doc;
        delete dt;
    }




    //
    //  CreateAttributeNS methods
    //

    {

        // Set up an initial (root element only) document.
        //
        IDOM_DOMImplementation* impl = IDOM_DOMImplementation::getImplementation();

        XMLString::transcode("foo:docName", tempStr3, 3999);
        XMLString::transcode("pubId", tempStr2, 3999);
        XMLString::transcode( "http://sysId", tempStr, 3999);


        IDOM_DocumentType* dt = impl->createDocumentType(tempStr3, tempStr2, tempStr);

        XMLString::transcode("http://document.namespace", tempStr2, 3999);
        IDOM_Document* doc = impl->createDocument(tempStr2, tempStr3, dt);
        IDOM_Element* rootEl = doc->getDocumentElement();

        //
        // CreateAttributeNS
        //
        XMLString::transcode("http://nsa", tempStr5, 3999);
        XMLString::transcode("http://nsb", tempStr4, 3999);
        XMLString::transcode("", tempStr3, 3999);


        XMLString::transcode("a:attra", tempStr, 3999);
        IDOM_Attr* attra = doc->createAttributeNS(tempStr5, tempStr);       // prefix and URI
        TASSERT(!XMLString::compareString(attra->getNodeName(), tempStr));
        TASSERT(!XMLString::compareString(attra->getNamespaceURI(), tempStr5));

        XMLString::transcode("a", tempStr2, 3999);
        TASSERT(!XMLString::compareString(attra->getPrefix(), tempStr2));

        XMLString::transcode("attra", tempStr2, 3999);
        TASSERT(!XMLString::compareString(attra->getLocalName(), tempStr2));
        TASSERT(!XMLString::compareString(attra->getName(), tempStr));
        TASSERT(attra->getOwnerElement() == 0);

        XMLString::transcode("attrb", tempStr2, 3999);
        IDOM_Attr* attrb = doc->createAttributeNS(tempStr4, tempStr2);         //  URI, no prefix.
        TASSERT(!XMLString::compareString(attrb->getNodeName(), tempStr2));
        TASSERT(!XMLString::compareString(attrb->getNamespaceURI(), tempStr4));
        TASSERT(!XMLString::compareString(attrb->getPrefix(), tempStr3));
        TASSERT(!XMLString::compareString(attrb->getLocalName(), tempStr2));
        TASSERT(!XMLString::compareString(attrb->getName(), tempStr2));
        TASSERT(attrb->getOwnerElement() == 0);



        XMLString::transcode("attrc", tempStr2, 3999);
        IDOM_Attr* attrc = doc->createAttributeNS(tempStr3, tempStr2);
        TASSERT(!XMLString::compareString(attrc->getNodeName(), tempStr2));
        TASSERT(!XMLString::compareString(attrc->getNamespaceURI(), tempStr3));
        TASSERT(!XMLString::compareString(attrc->getPrefix(), tempStr3));
        TASSERT(!XMLString::compareString(attrc->getLocalName(), tempStr2));
        TASSERT(!XMLString::compareString(attrc->getName(), tempStr2));
        TASSERT(attrc->getOwnerElement() == 0);

        // Badly formed qualified name
        XMLString::transcode("<a", tempStr, 3999);
        EXCEPTION_TEST(doc->createAttributeNS(tempStr5, tempStr), IDOM_DOMException::INVALID_CHARACTER_ERR);

        XMLString::transcode(":a", tempStr, 3999);
        EXCEPTION_TEST(doc->createAttributeNS(tempStr5, tempStr), IDOM_DOMException::NAMESPACE_ERR);

        XMLString::transcode("a:", tempStr, 3999);
        EXCEPTION_TEST(doc->createAttributeNS(tempStr5, tempStr), IDOM_DOMException::NAMESPACE_ERR);

        XMLString::transcode("a::a", tempStr, 3999);
        EXCEPTION_TEST(doc->createAttributeNS(tempStr5, tempStr), IDOM_DOMException::NAMESPACE_ERR);

        XMLString::transcode("a:a:a", tempStr, 3999);
        EXCEPTION_TEST(doc->createAttributeNS(tempStr5, tempStr), IDOM_DOMException::NAMESPACE_ERR);

        // xml:a must have namespaceURI == "http://www.w3.org/XML/1998/namespace"
        XMLString::transcode("http://www.w3.org/XML/1998/namespace", tempStr2, 3999);
        XMLString::transcode("xml:a", tempStr, 3999);
        TASSERT(!XMLString::compareString(doc->createAttributeNS(tempStr2, tempStr)->getNamespaceURI(), tempStr2));
        EXCEPTION_TEST(doc->createAttributeNS(tempStr5, tempStr), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc->createAttributeNS(tempStr3, tempStr), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc->createAttributeNS(0,  tempStr), IDOM_DOMException::NAMESPACE_ERR);

        //unlike Element, xmlns must have namespaceURI == "http://www.w3.org/2000/xmlns/"
        XMLString::transcode("http://www.w3.org/2000/xmlns/", tempStr4, 3999);
        XMLString::transcode("xmlns", tempStr, 3999);
        TASSERT(!XMLString::compareString(doc->createAttributeNS(tempStr4, tempStr)->getNamespaceURI(), tempStr4));
        EXCEPTION_TEST(doc->createAttributeNS(tempStr5, tempStr), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc->createAttributeNS(tempStr2, tempStr), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc->createAttributeNS(tempStr3, tempStr), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc->createAttributeNS(0,  tempStr), IDOM_DOMException::NAMESPACE_ERR);

        //unlike Element, xmlns:a must have namespaceURI == "http://www.w3.org/2000/xmlns/"
        XMLString::transcode("xmlns:a", tempStr, 3999);
        TASSERT(!XMLString::compareString(doc->createAttributeNS(tempStr4, tempStr)->getNamespaceURI(), tempStr4));
        EXCEPTION_TEST(doc->createAttributeNS(tempStr5, tempStr), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc->createAttributeNS(tempStr2, tempStr), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc->createAttributeNS(tempStr3, tempStr), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc->createAttributeNS(0,  tempStr), IDOM_DOMException::NAMESPACE_ERR);

        //In fact, any prefix != null should have a namespaceURI != 0 or != ""
        XMLString::transcode("foo:a", tempStr, 3999);
        TASSERT(!XMLString::compareString(doc->createAttributeNS(tempStr5, tempStr)->getNamespaceURI(), tempStr5));
        EXCEPTION_TEST(doc->createAttributeNS(tempStr3, tempStr), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(doc->createAttributeNS(0,  tempStr), IDOM_DOMException::NAMESPACE_ERR);

        //Change prefix
        IDOM_Attr* attr = doc->createAttributeNS(tempStr5, tempStr);
        XMLString::transcode("bar", tempStr4, 3999);
        XMLString::transcode("bar:a", tempStr, 3999);
        XMLString::transcode("a", tempStr2, 3999);
        attr->setPrefix(tempStr4);

        TASSERT(!XMLString::compareString(attr->getNodeName(), tempStr));
        TASSERT(!XMLString::compareString(attr->getNamespaceURI(), tempStr5));
        TASSERT(!XMLString::compareString(attr->getPrefix(), tempStr4));
        TASSERT(!XMLString::compareString(attr->getName(), tempStr));
        //The spec does not prevent us from setting prefix to a node without prefix
        TASSERT(!XMLString::compareString(attr->getLocalName(), tempStr2));
        attr = doc->createAttributeNS(tempStr5, tempStr2);
        TASSERT(!XMLString::compareString(attr->getPrefix(), tempStr3));
        attr->setPrefix(tempStr4);
        TASSERT(!XMLString::compareString(attr->getNodeName(), tempStr));
        TASSERT(!XMLString::compareString(attr->getNamespaceURI(), tempStr5));
        TASSERT(!XMLString::compareString(attr->getPrefix(), tempStr4));
        TASSERT(!XMLString::compareString(attr->getLocalName(), tempStr2));
        TASSERT(!XMLString::compareString(attr->getName(), tempStr));


        //Special case for xml:a where namespaceURI must be xmlURI
        XMLString::transcode("foo:a", tempStr, 3999);
        XMLString::transcode("xml", tempStr4, 3999);
        XMLString::transcode("http://www.w3.org/XML/1998/namespace", tempStr2, 3999);

        attr = doc->createAttributeNS(tempStr2, tempStr);
        attr->setPrefix(tempStr4);
        attr = doc->createAttributeNS(tempStr5, tempStr);
        EXCEPTION_TEST(attr->setPrefix(tempStr4), IDOM_DOMException::NAMESPACE_ERR);
        //Special case for xmlns:a where namespaceURI must be xmlURI
        XMLString::transcode("http://www.w3.org/2000/xmlns/", tempStr2, 3999);
        attr = doc->createAttributeNS(tempStr2, tempStr);

        XMLString::transcode("xmlns", tempStr4, 3999);
        attr->setPrefix(tempStr4);
        attr = doc->createAttributeNS(tempStr5, tempStr);
        EXCEPTION_TEST(attr->setPrefix(tempStr4), IDOM_DOMException::NAMESPACE_ERR);
        //Special case for xmlns where no prefix can be set
        attr = doc->createAttributeNS(tempStr2, tempStr4);

        XMLString::transcode("foo", tempStr, 3999);
        EXCEPTION_TEST(attr->setPrefix(tempStr), IDOM_DOMException::NAMESPACE_ERR);
        EXCEPTION_TEST(attr->setPrefix(tempStr4), IDOM_DOMException::NAMESPACE_ERR);
        //Also an attribute can not have a prefix with namespaceURI == null or ""
        XMLString::transcode("a", tempStr4, 3999);
        attr = doc->createAttributeNS(0, tempStr4);
        EXCEPTION_TEST(attr->setPrefix(tempStr), IDOM_DOMException::NAMESPACE_ERR);
        attr = doc->createAttributeNS(tempStr3, tempStr4);
        EXCEPTION_TEST(attr->setPrefix(tempStr), IDOM_DOMException::NAMESPACE_ERR);

        //Only prefix of Element and Attribute can be changed
        EXCEPTION_TEST(doc->setPrefix(tempStr), IDOM_DOMException::NAMESPACE_ERR);

        //Prefix of readonly Attribute can not be changed.
        //However, there is no way to create such IDOM_Attribute for testing yet.
        delete doc;
        delete dt;
    }



    //
    //  getElementsByTagName*
    //

    {

        // Set up an initial (root element only) document.
        //
        IDOM_DOMImplementation* impl = IDOM_DOMImplementation::getImplementation();

        XMLString::transcode("foo:docName", tempStr3, 3999);
        XMLString::transcode("pubId", tempStr2, 3999);
        XMLString::transcode( "http://sysId", tempStr, 3999);


        IDOM_DocumentType* dt = impl->createDocumentType(tempStr3, tempStr2, tempStr);

        XMLString::transcode("http://document.namespace", tempStr2, 3999);
        IDOM_Document* doc = impl->createDocument(tempStr2, tempStr3, dt);

        IDOM_Element* rootEl = doc->getDocumentElement();

        //
        // Populate the document
        //
        XMLString::transcode("http://nsa", tempStr5, 3999);
        XMLString::transcode("http://nsb", tempStr4, 3999);
        XMLString::transcode("", tempStr3, 3999);

        XMLString::transcode("a:ela", tempStr, 3999);
        IDOM_Element* ela = doc->createElementNS(tempStr5, tempStr);
        rootEl->appendChild(ela);

        XMLString::transcode("elb", tempStr2, 3999);
        IDOM_Element* elb = doc->createElementNS(tempStr4, tempStr2);
        rootEl->appendChild(elb);

        XMLString::transcode("elc", tempStr, 3999);
        IDOM_Element* elc = doc->createElementNS(tempStr3, tempStr);
        rootEl->appendChild(elc);

        XMLString::transcode("d:ela", tempStr, 3999);
        IDOM_Element* eld = doc->createElementNS(tempStr5, tempStr);
        rootEl->appendChild(eld);

        XMLString::transcode("http://nse", tempStr, 3999);
        IDOM_Element* ele = doc->createElementNS(tempStr, tempStr2);
        rootEl->appendChild(ele);


        //
        // Access with IDOM Level 1 getElementsByTagName
        //

        IDOM_NodeList* nl;

        XMLString::transcode("a:ela", tempStr, 3999);
        nl = doc->getElementsByTagName(tempStr);
        TASSERT(nl->getLength() == 1);
        TASSERT(nl->item(0) == ela);

        nl = doc->getElementsByTagName(tempStr2);
        TASSERT(nl->getLength() == 2);
        TASSERT(nl->item(0) == elb);
        TASSERT(nl->item(1) == ele);

        XMLString::transcode("d:ela", tempStr, 3999);
        nl = doc->getElementsByTagName(tempStr);
        TASSERT(nl->getLength() == 1);
        TASSERT(nl->item(0) == eld);

        //
        //  Access with IDOM Level 2 getElementsByTagNameNS
        //

        XMLString::transcode("elc", tempStr, 3999);
        nl = doc->getElementsByTagNameNS(tempStr3, tempStr);
        TASSERT(nl->getLength() == 1);
        TASSERT(nl->item(0) == elc);

        nl = doc->getElementsByTagNameNS(0, tempStr);
        TASSERT(nl->getLength() == 1);
        TASSERT(nl->item(0) == elc);

        XMLString::transcode("ela", tempStr, 3999);
        nl = doc->getElementsByTagNameNS(tempStr5, tempStr);
        TASSERT(nl->getLength() == 2);
        TASSERT(nl->item(0) == ela);
        TASSERT(nl->item(1) == eld);

        nl = doc->getElementsByTagNameNS(tempStr3, tempStr2);
        TASSERT(nl->getLength() == 0);

        nl = doc->getElementsByTagNameNS(tempStr4, tempStr2);
        TASSERT(nl->getLength() == 1);
        TASSERT(nl->item(0) == elb);

        XMLString::transcode("*", tempStr, 3999);
        nl = doc->getElementsByTagNameNS(tempStr, tempStr2);
        TASSERT(nl->getLength() == 2);
        TASSERT(nl->item(0) == elb);
        TASSERT(nl->item(1) == ele);

        nl = doc->getElementsByTagNameNS(tempStr5, tempStr);
        TASSERT(nl->getLength() == 2);
        TASSERT(nl->item(0) == ela);
        TASSERT(nl->item(1) == eld);

        nl = doc->getElementsByTagNameNS(tempStr, tempStr);
        TASSERT(nl->getLength() == 6);     // Gets the document root element, plus 5 more

        TASSERT(nl->item(6) == 0);
        // TASSERT(nl->item(-1) == 0);

        nl = rootEl->getElementsByTagNameNS(tempStr, tempStr);
        TASSERT(nl->getLength() == 5);


        XMLString::transcode("d:ela", tempStr2, 3999);
        nl = doc->getElementsByTagNameNS(tempStr5, tempStr2);
        TASSERT(nl->getLength() == 0);


        //
        // Node lists are Live
        //

        nl = doc->getElementsByTagNameNS(tempStr, tempStr);
        IDOM_NodeList* nla = ela->getElementsByTagNameNS(tempStr, tempStr);

        TASSERT(nl->getLength() == 6);
        TASSERT(nla->getLength() == 0);

        rootEl->removeChild(elc);
        TASSERT(nl->getLength() == 5);
        TASSERT(nla->getLength() == 0);

        ela->appendChild(elc);
        TASSERT(nl->getLength() == 6);
        TASSERT(nla->getLength() == 1);
        delete doc;
        delete dt;
    }



   //
    // Attributes and NamedNodeMaps.
    //

    {

        // Set up an initial (root element only) document.
        //
        IDOM_DOMImplementation* impl = IDOM_DOMImplementation::getImplementation();

        XMLString::transcode("foo:docName", tempStr, 3999);
        XMLString::transcode("pubId", tempStr2, 3999);
        XMLString::transcode("http://sysId", tempStr3, 3999);
        IDOM_DocumentType* dt = impl->createDocumentType(tempStr, tempStr2, tempStr3);

        XMLString::transcode("http://document.namespace", tempStr2, 3999);
        IDOM_Document* doc = impl->createDocument(tempStr2, tempStr, dt);
        IDOM_Element* rootEl = doc->getDocumentElement();

        //
        // Create a set of attributes and hang them on the root element.
        //
        XMLString::transcode("http://nsa", tempStr5, 3999);
        XMLString::transcode("http://nsb", tempStr4, 3999);
        XMLString::transcode("", tempStr3, 3999);
        XMLString::transcode("a:attra", tempStr2, 3999);

        IDOM_Attr* attra = doc->createAttributeNS(tempStr5, tempStr2);
        rootEl->setAttributeNodeNS(attra);

        XMLString::transcode("attrb", tempStr, 3999);
        IDOM_Attr* attrb = doc->createAttributeNS(tempStr4, tempStr);
        rootEl->setAttributeNodeNS(attrb);

        XMLString::transcode("attrc", tempStr, 3999);
        IDOM_Attr* attrc = doc->createAttributeNS(tempStr3, tempStr);
        rootEl->setAttributeNodeNS(attrc);

        XMLString::transcode("d:attra", tempStr, 3999);
        IDOM_Attr* attrd = doc->createAttributeNS(tempStr5, tempStr);
        rootEl->setAttributeNodeNS(attrd);

        XMLString::transcode("http://nse", tempStr2, 3999);
        XMLString::transcode("attrb", tempStr, 3999);
        IDOM_Attr* attre = doc->createAttributeNS(tempStr2, tempStr);
        rootEl->setAttributeNodeNS(attre);

        //
        // Check that the attribute nodes were created with the correct properties.
        //
        XMLString::transcode("a:attra", tempStr2, 3999);
        TASSERT(!XMLString::compareString(attra->getNodeName(), tempStr2));
        TASSERT(!XMLString::compareString(attra->getNamespaceURI(), tempStr5));

        XMLString::transcode("attra", tempStr, 3999);
        TASSERT(!XMLString::compareString(attra->getLocalName(), tempStr));
        TASSERT(!XMLString::compareString(attra->getName(), tempStr2));
        TASSERT(attra->getNodeType() == IDOM_Node::ATTRIBUTE_NODE);
        TASSERT(!XMLString::compareString(attra->getNodeValue(), tempStr3));

        XMLString::transcode("a", tempStr, 3999);
        TASSERT(!XMLString::compareString(attra->getPrefix(), tempStr));
        TASSERT(attra->getSpecified() == true);
        TASSERT(!XMLString::compareString(attra->getValue(), tempStr3));
        TASSERT(attra->getOwnerElement() == 0);

        // Test methods of NamedNodeMap
        IDOM_NamedNodeMap* nnm = rootEl->getAttributes();
        TASSERT(nnm->getLength() == 4);

        XMLString::transcode("attra", tempStr2, 3999);
        XMLString::transcode("attrb", tempStr, 3999);

        TASSERT(nnm->getNamedItemNS(tempStr5, tempStr2) == attrd);
        TASSERT(nnm->getNamedItemNS(tempStr4, tempStr) == attrb);
        TASSERT(nnm->getNamedItemNS(tempStr3, tempStr2) == 0);
        TASSERT(nnm->getNamedItemNS(tempStr5, tempStr) == 0);

        XMLString::transcode("http://nse", tempStr2, 3999);
        TASSERT(nnm->getNamedItemNS(tempStr2, tempStr) == attre);

        XMLString::transcode("attrc", tempStr2, 3999);
        TASSERT(nnm->getNamedItemNS(tempStr3, tempStr2) == attrc);

        // Test hasAttributes, hasAttribute, hasAttributeNS
        TASSERT(doc->hasAttributes() ==  false);
        TASSERT(attrc->hasAttributes() == false);
        TASSERT(rootEl->hasAttributes() == true);
        TASSERT(rootEl->hasAttribute(tempStr2) == true);

        XMLString::transcode("wrong", tempStr, 3999);
        TASSERT(rootEl->hasAttribute(tempStr) == false);

        XMLString::transcode("attra", tempStr2, 3999);
        TASSERT(rootEl->hasAttributeNS(tempStr5, tempStr2) == true);
        TASSERT(rootEl->hasAttributeNS(tempStr5, tempStr) == false);
        delete doc;
        delete dt;
    }


    //
    //
    //

}



//---------------------------------------------------------------------------------------
//
//   main
//
//---------------------------------------------------------------------------------------
int  mymain()
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

    IDOMBasicTests();
    IDOMNSTests();

    //
    //  Print Final allocation stats for full set of tests
    //
    XMLPlatformUtils::Terminate();

    return 0;

};

int  main() {
    for (int i = 0; i<50; i++)
        mymain();

    printf("Test Run Successfully\n");
}

