/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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
//       1.  NodeIterator tests
//       2.  Tree Walker tests
//     All individual are wrapped in a memory leak checker.
//
//     This is NOT a complete test of DOM functionality.
//

/**
 * $Log$
 * Revision 1.6  2002/03/07 21:42:14  peiyongz
 * Call Terminate() to avoid memory tools reporting memory leak
 *
 * Revision 1.5  2002/02/04 20:12:43  tng
 * Test DOM Level missing functions:
 * 1. NodeIterator::getRoot
 * 2. TreeWalker::getRoot
 *
 * Revision 1.4  2002/02/01 22:43:48  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/11/23 16:18:54  tng
 * Elimiate compiler warning: Warning: String literal converted to char* in formal argument file in call to tassert(bool, char*, int).
 *
 * Revision 1.2  2000/03/11 03:19:57  chchou
 * Fix bug # 19, add const keyword to API.
 * As a result, update test case.
 *
 * Revision 1.1  2000/02/18 23:01:01  abagchi
 * Initial checkin
 *
 * Revision 1.2  2000/02/15 00:59:58  aruna1
 * Previous node iterator related changes in.
 *
 * Revision 1.1  2000/02/08 02:08:58  aruna1
 * DOM NodeIterator and TreeWalker tests initial check in
 *

 */

#include <stdio.h>
#include <string.h>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DomMemDebug.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLString.hpp>


#define TASSERT(c) tassert((c), __FILE__, __LINE__)

void tassert(bool c, const char *file, int line)
{
    if (!c)
        printf("Failure.  Line %d,   file %s\n", line, file);
};


#define TESTPROLOG entryMemState = DomMemDebug();

#define TESTEPILOG    exitMemState = DomMemDebug(); \
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

class  MyFilter : public DOM_NodeFilter {
public:

  MyFilter(short nodeType, bool reject=false) : DOM_NodeFilter(), fNodeType(nodeType), fReject(reject) {};
  virtual short acceptNode(const DOM_Node &node) const;
private:
    short fNodeType;
    bool fReject;
};
/*
        Node Types can be of the following:
        ELEMENT_NODE         = 1,
        ATTRIBUTE_NODE       = 2,
        TEXT_NODE            = 3,
        CDATA_SECTION_NODE   = 4,
        ENTITY_REFERENCE_NODE = 5,
        ENTITY_NODE          = 6,
        PROCESSING_INSTRUCTION_NODE = 7,
        COMMENT_NODE         = 8,
        DOCUMENT_NODE        = 9,
        DOCUMENT_TYPE_NODE   = 10,
        DOCUMENT_FRAGMENT_NODE = 11,
        NOTATION_NODE        = 12
*/
short  MyFilter::acceptNode(const DOM_Node &node) const {
    if (fNodeType == 0)
        return  DOM_NodeFilter::FILTER_ACCEPT;
	if (node.getNodeType() ==  fNodeType) {
       	return  DOM_NodeFilter::FILTER_ACCEPT;
	} else {
	    return  fReject ? DOM_NodeFilter::FILTER_REJECT : DOM_NodeFilter::FILTER_SKIP;
	}
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
        delete [] pMessage;
        return -1;
    }


    //
    //  Doc - Create a small document tree
    //

    {
        //creating a DOM Tree
         /* Tests are based on the tree structure below
           doc - root - E11 (attr01) - textNode1
                                     - E111
                                     - E112
                                     - cdataSec
                      - E12 (attr02) - textNode2
                                     - E121
                                     - E122
                      - E13          - E131
                                     - docPI
                      - comment
         */

        DOM_Document    doc = DOM_Document::createDocument();
        //Creating a root element
        DOM_Element     root = doc.createElement("RootElement");
        doc.appendChild(root);
        //Creating the siblings of root
        DOM_Element     E11 = doc.createElement("FirstSibling");
        root.appendChild(E11);
        DOM_Element     E12 = doc.createElement("SecondSibling");
        root.appendChild(E12);
        DOM_Element     E13 = doc.createElement("ThirdSibling");
        root.appendChild(E13);
        //Attaching texts to few siblings
        DOM_Text        textNode1 = doc.createTextNode("Text1");
        E11.appendChild(textNode1);
        DOM_Text        textNode2 = doc.createTextNode("Text2");
        E12.appendChild(textNode2);
        //creating child of siblings
        DOM_Element     E111 = doc.createElement("FirstSiblingChild1");
        E11.appendChild(E111);
        DOM_Attr        attr01  = doc.createAttribute("Attr01");
        E11.setAttributeNode(attr01);

        DOM_Element     E112 = doc.createElement("FirstSiblingChild2");
        E11.appendChild(E112);

        DOM_Element     E121 = doc.createElement("SecondSiblingChild1");
        E12.appendChild(E121);
        DOM_Attr attr02 = doc.createAttribute("Attr01");
        E12.setAttributeNode(attr02);

        DOM_Element     E122 = doc.createElement("SecondSiblingChild2");
        E12.appendChild(E122);

        DOM_Element     E131 = doc.createElement("ThirdSiblingChild1");
        E13.appendChild(E131);

        DOM_Comment comment = doc.createComment("DocComment");
        root.appendChild(comment);

        DOM_CDATASection  cdataSec = doc.createCDATASection("DocCDataSection");
        E11.appendChild(cdataSec);

        DOM_ProcessingInstruction  docPI = doc.createProcessingInstruction("DocPI", "DocTarget");
        E13.appendChild(docPI);


        /*
        following are whatToShow types:
            SHOW_ALL                       = 0x0000FFFF,
            SHOW_ELEMENT                   = 0x00000001,
            SHOW_ATTRIBUTE                 = 0x00000002,
            SHOW_TEXT                      = 0x00000004,
            SHOW_CDATA_SECTION             = 0x00000008,
            SHOW_ENTITY_REFERENCE          = 0x00000010,
            SHOW_ENTITY                    = 0x00000020,
            SHOW_PROCESSING_INSTRUCTION    = 0x00000040,
            SHOW_COMMENT                   = 0x00000080,
            SHOW_DOCUMENT                  = 0x00000100,
            SHOW_DOCUMENT_TYPE             = 0x00000200,
            SHOW_DOCUMENT_FRAGMENT         = 0x00000400,
            SHOW_NOTATION                  = 0x00000800
        */

        ////////// NodeIterator Test Cases ////////////////

        TESTPROLOG;
        {
            // all node iterating test

            DOM_Node    node = doc.getFirstChild();
            unsigned long       whatToShow = DOM_NodeFilter::SHOW_ALL;
            MyFilter* filter = new MyFilter(0);

            DOM_NodeIterator  iter = doc.createNodeIterator(root, whatToShow,  filter, true);
            TASSERT(iter.getWhatToShow() == 65535);
            TASSERT(iter.getExpandEntityReferences() == 1);

            DOM_Node  nd;
            nd = iter.nextNode();
            TASSERT (nd ==root);
            nd = iter.nextNode();
            TASSERT (nd ==E11);
            nd = iter.nextNode();
            TASSERT(nd == textNode1);
            nd = iter.nextNode();
            TASSERT(nd == E111);
            nd = iter.nextNode();
            TASSERT(nd == E112);
            nd = iter.nextNode();
            TASSERT(nd == cdataSec);
            nd = iter.nextNode();
            TASSERT(nd == E12);
            nd = iter.nextNode();
            TASSERT(nd == textNode2);
            nd = iter.nextNode();
            TASSERT(nd == E121);
            nd = iter.nextNode();
            TASSERT(nd == E122);
            nd = iter.nextNode();
            TASSERT(nd == E13);
            nd = iter.nextNode();
            TASSERT(nd == E131);
            nd = iter.nextNode();
            TASSERT(nd == docPI);
            nd = iter.nextNode();
            TASSERT(nd == comment);
            nd = iter.previousNode();
            TASSERT(nd == comment);
            nd = iter.previousNode();
            TASSERT(nd == docPI);
            nd = iter.previousNode();
            TASSERT(nd == E131);

            //test getRoot
            TASSERT(iter.getRoot() == root);
            TASSERT(iter.getRoot() != doc);

			delete filter;
        }
        TESTEPILOG;

        TESTPROLOG;
        {
            //element node iterating test

            DOM_Node    node = doc.getFirstChild();
            unsigned long       whatToShow = DOM_NodeFilter::SHOW_ELEMENT;
            MyFilter* filter = new MyFilter(DOM_Node::ELEMENT_NODE);

            DOM_NodeIterator  iter = doc.createNodeIterator(root, whatToShow,  filter, true);
            TASSERT(iter.getWhatToShow() == 1);
            TASSERT(iter.getExpandEntityReferences() == 1);

            DOM_Node  nd;
            nd = iter.nextNode();
            TASSERT (nd ==root);
            nd = iter.nextNode();
            TASSERT (nd ==E11);
            nd = iter.nextNode();
            TASSERT(nd == E111);
            nd = iter.nextNode();
            TASSERT(nd == E112);
            nd = iter.nextNode();
            TASSERT(nd == E12);
            nd = iter.nextNode();
            TASSERT(nd == E121);
            nd = iter.nextNode();
            TASSERT(nd == E122);
            nd = iter.nextNode();
            TASSERT(nd == E13);
            nd = iter.nextNode();
            TASSERT(nd == E131);
            nd = iter.previousNode();
            TASSERT(nd == E131);
            nd = iter.previousNode();
            TASSERT(nd == E13);
            nd = iter.previousNode();
            TASSERT(nd == E122);

			delete filter;
        }
        TESTEPILOG;



        TESTPROLOG;
        {
            // Text node iterating test

            DOM_Node    node = doc.getFirstChild();
            unsigned long       whatToShow = DOM_NodeFilter::SHOW_TEXT;
            MyFilter* filter = new MyFilter(DOM_Node::TEXT_NODE);

            DOM_NodeIterator  iter = doc.createNodeIterator(root, whatToShow,  filter, true);

            TASSERT(iter.getWhatToShow() == 4);
            TASSERT(iter.getExpandEntityReferences() == 1);

            DOM_Node  nd;
            nd = iter.nextNode();
            TASSERT (nd ==textNode1);
            nd = iter.nextNode();
            TASSERT (nd ==textNode2);
            nd = iter.previousNode();
            TASSERT(nd == textNode2);

			delete filter;
        }
        TESTEPILOG;
        TESTPROLOG;
        {
            //CDataSection node itearating test

            DOM_Node    node = doc.getFirstChild();
            unsigned long       whatToShow = DOM_NodeFilter::SHOW_CDATA_SECTION;
            MyFilter* filter = new MyFilter(DOM_Node::CDATA_SECTION_NODE);

            DOM_NodeIterator  iter = doc.createNodeIterator(root, whatToShow,  filter, true);
            TASSERT(iter.getWhatToShow() == 8);
            TASSERT(iter.getExpandEntityReferences() == 1);

            DOM_Node  nd;
            nd = iter.nextNode();
            TASSERT(nd == cdataSec);
            nd = iter.nextNode();
            TASSERT(nd == 0);

			delete filter;
        }
        TESTEPILOG;
        TESTPROLOG;
        {
            // PI nodes iterating test

            DOM_Node    node = doc.getFirstChild();
            unsigned long       whatToShow = DOM_NodeFilter::SHOW_PROCESSING_INSTRUCTION;
            MyFilter* filter = new MyFilter(DOM_Node::PROCESSING_INSTRUCTION_NODE);

            DOM_NodeIterator  iter = doc.createNodeIterator(root, whatToShow,  filter, true);
            TASSERT(iter.getWhatToShow() == 64);
            TASSERT(iter.getExpandEntityReferences() == 1);

            DOM_Node  nd;
            nd = iter.nextNode();
            TASSERT(nd == docPI);
            nd = iter.nextNode();
            TASSERT(nd == 0);

			delete filter;

        }
        TESTEPILOG;

        TESTPROLOG;
        {
            DOM_Node    node = doc.getFirstChild();
            unsigned long       whatToShow = DOM_NodeFilter::SHOW_COMMENT;
            MyFilter* filter = new MyFilter(DOM_Node::COMMENT_NODE);

            DOM_NodeIterator  iter = doc.createNodeIterator(root, whatToShow,  filter, true);
            TASSERT(iter.getWhatToShow() == 128);
            TASSERT(iter.getExpandEntityReferences() == 1);

            DOM_Node  nd;
            nd = iter.nextNode();
            TASSERT(nd == comment);
            nd = iter.nextNode();
            TASSERT(nd == 0);

			delete filter;
        }
        TESTEPILOG;



        ////////// TreeWalker Test Cases ////////////////


        TESTPROLOG;
        {
            unsigned long whatToShow = DOM_NodeFilter::SHOW_ALL;
            DOM_TreeWalker tw = doc.createTreeWalker(doc, whatToShow, 0, true);

            TASSERT(tw.getCurrentNode() == doc);
            TASSERT(tw.firstChild() == root);
            TASSERT(tw.nextSibling() == 0);
            TASSERT(tw.lastChild() == comment);
            TASSERT(tw.firstChild() == 0);
            TASSERT(tw.lastChild() == 0);
            TASSERT(tw.nextSibling() == 0);
            TASSERT(tw.nextNode() == 0);
            TASSERT(tw.previousSibling() == E13);
            TASSERT(tw.previousNode() == E122);
            TASSERT(tw.parentNode() == E12);
            TASSERT(tw.firstChild() == textNode2);
            TASSERT(tw.previousSibling() == 0);
            TASSERT(tw.nextSibling() == E121);
            TASSERT(tw.nextNode() == E122);
            TASSERT(tw.parentNode() == E12);
            TASSERT(tw.previousSibling() == E11);
            TASSERT(tw.previousNode() == root);
            TASSERT(tw.previousNode() == doc);
            TASSERT(tw.previousNode() == 0);
            TASSERT(tw.parentNode() == 0);
            TASSERT(tw.getCurrentNode() == doc);
        }
        TESTEPILOG;

        TESTPROLOG;
        {
            MyFilter mf(DOM_Node::ELEMENT_NODE);
            unsigned long whatToShow = DOM_NodeFilter::SHOW_ALL;
            DOM_TreeWalker tw = doc.createTreeWalker(root, whatToShow, &mf, true);

            TASSERT(tw.getCurrentNode() == root);
            TASSERT(tw.parentNode() == 0);  //should not change currentNode
            TASSERT(tw.getCurrentNode() == root);
            TASSERT(tw.nextNode() == E11);
            TASSERT(tw.nextNode() == E111);
            tw.setCurrentNode(E12);
            //when first is not visible, should it go to its sibling?
            TASSERT(tw.firstChild() == E121);   //first visible child
            TASSERT(tw.previousSibling() == 0);
        }
        TESTEPILOG;

        TESTPROLOG;
        {
            MyFilter mf(DOM_Node::ELEMENT_NODE, true);
            unsigned long whatToShow = DOM_NodeFilter::SHOW_ELEMENT;
            DOM_TreeWalker tw = doc.createTreeWalker(root, whatToShow, &mf, true);

            tw.setCurrentNode(E12);
            TASSERT(tw.firstChild() == E121);   //still first visible child
        }
        TESTEPILOG;

        TESTPROLOG;
        {
            MyFilter mf(DOM_Node::TEXT_NODE);
            unsigned long whatToShow = DOM_NodeFilter::SHOW_TEXT;
            DOM_TreeWalker tw = doc.createTreeWalker(root, whatToShow, &mf, true);

            //when first is not visible, should it go to its descendent?
            TASSERT(tw.firstChild() == textNode1);   //E11 skipped
            TASSERT(tw.firstChild() == 0);
            TASSERT(tw.nextNode() == textNode2);
            TASSERT(tw.nextSibling() == 0);
            TASSERT(tw.parentNode() == 0);  //no visible ancestor
            TASSERT(tw.getCurrentNode() == textNode2);
            tw.setCurrentNode(root);
            //when last is not visible, should it go to its sibling & descendent?
            TASSERT(tw.lastChild() == textNode2);   //last visible child
            tw.setCurrentNode(E12);
            //when next sibling is not visible, should it go to its descendent?
            TASSERT(tw.nextSibling() == 0);
        }
        TESTEPILOG;

        TESTPROLOG;
        {
            MyFilter mf(DOM_Node::TEXT_NODE, true);
            unsigned long whatToShow = DOM_NodeFilter::SHOW_TEXT;
            DOM_TreeWalker tw = doc.createTreeWalker(root, whatToShow, &mf, true);

            TASSERT(tw.firstChild() == 0);   //E11 rejected and no children is TEXT
            TASSERT(tw.getCurrentNode() == root);
            TASSERT(tw.nextNode() == 0);    //E11 rejected so can't get to textNode1

            //test getRoot
            TASSERT(tw.getRoot() == root);
            TASSERT(tw.getRoot() != doc);
        }
        TESTEPILOG;

    };


    //
    //  Print Final allocation stats for full test
    //
    DomMemDebug().print();

    // And call the termination method
    XMLPlatformUtils::Terminate();

    return 0;
    };

