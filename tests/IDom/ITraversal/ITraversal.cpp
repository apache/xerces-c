/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

//
//  Various IDOM tests.
//     Contents include
//       1.  NodeIterator tests
//       2.  Tree Walker tests
//     All individual are wrapped in a memory leak checker.
//
//     This is NOT a complete test of DOM functionality.
//

/**
 * $Log$
 * Revision 1.1  2001/06/04 20:11:55  tng
 * IDOM: Complete IDNodeIterator, IDTreeWalker, IDNodeFilter.
 *
 *

 */

#include <stdio.h>
#include <string.h>
#include <idom/IDOM.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLException.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>


#define TASSERT(c) tassert((c), __FILE__, __LINE__)

void tassert(bool c, char *file, int line)
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
    catch (IDOM_DOMException &e) {                                       \
    if (e.code != expected_exception)                       \
    printf(" Wrong exception code: %d at line %d\n", e.code, __LINE__); \
}                                                                 \
    catch (...)   {                                                 \
    printf(" Wrong exception thrown at line %d\n", __LINE__);       \
}                                                                   \
}

class  MyFilter : public IDOM_NodeFilter {
public:

  MyFilter(short nodeType, bool reject=false) : IDOM_NodeFilter(), fNodeType(nodeType), fReject(reject) {};
  virtual short acceptNode(const IDOM_Node* node) const;
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
short  MyFilter::acceptNode(const IDOM_Node* node) const {
    if (fNodeType == 0)
        return  IDOM_NodeFilter::FILTER_ACCEPT;
	if (node->getNodeType() ==  fNodeType) {
       	return  IDOM_NodeFilter::FILTER_ACCEPT;
	} else {
	    return  fReject ? IDOM_NodeFilter::FILTER_REJECT : IDOM_NodeFilter::FILTER_SKIP;
	}
}



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

    // Create a XMLCh buffer for string manipulation
    XMLCh tempStr[4000];


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

        IDOM_DOMImplementation* impl = IDOM_DOMImplementation::getImplementation();
        IDOM_Document* doc = impl->createDocument();

        //Creating a root element
        XMLString::transcode("RootElement", tempStr, 3999);
        IDOM_Element*     root = doc->createElement(tempStr);
        doc->appendChild(root);

        //Creating the siblings of root
        XMLString::transcode("FirstSibling", tempStr, 3999);
        IDOM_Element*     E11 = doc->createElement(tempStr);
        root->appendChild(E11);

        XMLString::transcode("SecondSibling", tempStr, 3999);
        IDOM_Element*     E12 = doc->createElement(tempStr);
        root->appendChild(E12);

        XMLString::transcode("ThirdSibling", tempStr, 3999);
        IDOM_Element*     E13 = doc->createElement(tempStr);
        root->appendChild(E13);

        //Attaching texts to few siblings
        XMLString::transcode("Text1", tempStr, 3999);
        IDOM_Text*        textNode1 = doc->createTextNode(tempStr);
        E11->appendChild(textNode1);

        XMLString::transcode("Text2", tempStr, 3999);
        IDOM_Text*        textNode2 = doc->createTextNode(tempStr);
        E12->appendChild(textNode2);

        //creating child of siblings
        XMLString::transcode("FirstSiblingChild1", tempStr, 3999);
        IDOM_Element*     E111 = doc->createElement(tempStr);
        E11->appendChild(E111);

        XMLString::transcode("Attr01", tempStr, 3999);
        IDOM_Attr*        attr01  = doc->createAttribute(tempStr);
        E11->setAttributeNode(attr01);

        XMLString::transcode("FirstSiblingChild2", tempStr, 3999);
        IDOM_Element*     E112 = doc->createElement(tempStr);
        E11->appendChild(E112);

        XMLString::transcode("SecondSiblingChild1", tempStr, 3999);
        IDOM_Element*     E121 = doc->createElement(tempStr);
        E12->appendChild(E121);

        XMLString::transcode("Attr01", tempStr, 3999);
        IDOM_Attr* attr02 = doc->createAttribute(tempStr);
        E12->setAttributeNode(attr02);

        XMLString::transcode("SecondSiblingChild2", tempStr, 3999);
        IDOM_Element*     E122 = doc->createElement(tempStr);
        E12->appendChild(E122);

        XMLString::transcode("ThirdSiblingChild1", tempStr, 3999);
        IDOM_Element*     E131 = doc->createElement(tempStr);
        E13->appendChild(E131);

        XMLString::transcode("DocComment", tempStr, 3999);
        IDOM_Comment* comment = doc->createComment(tempStr);
        root->appendChild(comment);

        XMLString::transcode("DocCDataSection", tempStr, 3999);
        IDOM_CDATASection*  cdataSec = doc->createCDATASection(tempStr);
        E11->appendChild(cdataSec);

        XMLString::transcode("DocPI", tempStr, 3999);
        XMLCh piStr[] = {chLatin_D, chLatin_o, chLatin_c, chLatin_P, chLatin_I, chNull};
        IDOM_ProcessingInstruction*  docPI = doc->createProcessingInstruction(piStr, tempStr);
        E13->appendChild(docPI);


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


        {
            // all node iterating test

            IDOM_Node*    node = doc->getFirstChild();
            unsigned long       whatToShow = IDOM_NodeFilter::SHOW_ALL;
            MyFilter* filter = new MyFilter(0);

            IDOM_NodeIterator*  iter = doc->createNodeIterator(root, whatToShow,  filter, true);
            TASSERT(iter->getWhatToShow() == 65535);
            TASSERT(iter->getExpandEntityReferences() == 1);

            IDOM_Node*  nd;
            nd = iter->nextNode();
            TASSERT (nd ==root);
            nd = iter->nextNode();
            TASSERT (nd ==E11);
            nd = iter->nextNode();
            TASSERT(nd == textNode1);
            nd = iter->nextNode();
            TASSERT(nd == E111);
            nd = iter->nextNode();
            TASSERT(nd == E112);
            nd = iter->nextNode();
            TASSERT(nd == cdataSec);
            nd = iter->nextNode();
            TASSERT(nd == E12);
            nd = iter->nextNode();
            TASSERT(nd == textNode2);
            nd = iter->nextNode();
            TASSERT(nd == E121);
            nd = iter->nextNode();
            TASSERT(nd == E122);
            nd = iter->nextNode();
            TASSERT(nd == E13);
            nd = iter->nextNode();
            TASSERT(nd == E131);
            nd = iter->nextNode();
            TASSERT(nd == docPI);
            nd = iter->nextNode();
            TASSERT(nd == comment);
            nd = iter->previousNode();
            TASSERT(nd == comment);
            nd = iter->previousNode();
            TASSERT(nd == docPI);
            nd = iter->previousNode();
            TASSERT(nd == E131);

        }



        {
            //element node iterating test

            IDOM_Node*    node = doc->getFirstChild();
            unsigned long       whatToShow = IDOM_NodeFilter::SHOW_ELEMENT;
            MyFilter* filter = new MyFilter(IDOM_Node::ELEMENT_NODE);

            IDOM_NodeIterator*  iter = doc->createNodeIterator(root, whatToShow,  filter, true);
            TASSERT(iter->getWhatToShow() == 1);
            TASSERT(iter->getExpandEntityReferences() == 1);

            IDOM_Node*  nd;
            nd = iter->nextNode();
            TASSERT (nd ==root);
            nd = iter->nextNode();
            TASSERT (nd ==E11);
            nd = iter->nextNode();
            TASSERT(nd == E111);
            nd = iter->nextNode();
            TASSERT(nd == E112);
            nd = iter->nextNode();
            TASSERT(nd == E12);
            nd = iter->nextNode();
            TASSERT(nd == E121);
            nd = iter->nextNode();
            TASSERT(nd == E122);
            nd = iter->nextNode();
            TASSERT(nd == E13);
            nd = iter->nextNode();
            TASSERT(nd == E131);
            nd = iter->previousNode();
            TASSERT(nd == E131);
            nd = iter->previousNode();
            TASSERT(nd == E13);
            nd = iter->previousNode();
            TASSERT(nd == E122);
        }





        {
            // Text node iterating test

            IDOM_Node*    node = doc->getFirstChild();
            unsigned long       whatToShow = IDOM_NodeFilter::SHOW_TEXT;
            MyFilter* filter = new MyFilter(IDOM_Node::TEXT_NODE);

            IDOM_NodeIterator*  iter = doc->createNodeIterator(root, whatToShow,  filter, true);

            TASSERT(iter->getWhatToShow() == 4);
            TASSERT(iter->getExpandEntityReferences() == 1);

            IDOM_Node*  nd;
            nd = iter->nextNode();
            TASSERT (nd ==textNode1);
            nd = iter->nextNode();
            TASSERT (nd ==textNode2);
            nd = iter->previousNode();
            TASSERT(nd == textNode2);

        }


        {
            //CDataSection node itearating test

            IDOM_Node*    node = doc->getFirstChild();
            unsigned long       whatToShow = IDOM_NodeFilter::SHOW_CDATA_SECTION;
            MyFilter* filter = new MyFilter(IDOM_Node::CDATA_SECTION_NODE);

            IDOM_NodeIterator*  iter = doc->createNodeIterator(root, whatToShow,  filter, true);
            TASSERT(iter->getWhatToShow() == 8);
            TASSERT(iter->getExpandEntityReferences() == 1);

            IDOM_Node*  nd;
            nd = iter->nextNode();
            TASSERT(nd == cdataSec);
            nd = iter->nextNode();
            TASSERT(nd == 0);

        }


        {
            // PI nodes iterating test

            IDOM_Node*    node = doc->getFirstChild();
            unsigned long       whatToShow = IDOM_NodeFilter::SHOW_PROCESSING_INSTRUCTION;
            MyFilter* filter = new MyFilter(IDOM_Node::PROCESSING_INSTRUCTION_NODE);

            IDOM_NodeIterator*  iter = doc->createNodeIterator(root, whatToShow,  filter, true);
            TASSERT(iter->getWhatToShow() == 64);
            TASSERT(iter->getExpandEntityReferences() == 1);

            IDOM_Node*  nd;
            nd = iter->nextNode();
            TASSERT(nd == docPI);
            nd = iter->nextNode();
            TASSERT(nd == 0);


        }



        {
            IDOM_Node*    node = doc->getFirstChild();
            unsigned long       whatToShow = IDOM_NodeFilter::SHOW_COMMENT;
            MyFilter* filter = new MyFilter(IDOM_Node::COMMENT_NODE);

            IDOM_NodeIterator*  iter = doc->createNodeIterator(root, whatToShow,  filter, true);
            TASSERT(iter->getWhatToShow() == 128);
            TASSERT(iter->getExpandEntityReferences() == 1);

            IDOM_Node*  nd;
            nd = iter->nextNode();
            TASSERT(nd == comment);
            nd = iter->nextNode();
            TASSERT(nd == 0);

        }




        ////////// TreeWalker Test Cases ////////////////



        {
            unsigned long whatToShow = IDOM_NodeFilter::SHOW_ALL;
            IDOM_TreeWalker* tw = doc->createTreeWalker(doc, whatToShow, 0, true);

            TASSERT(tw->getCurrentNode() == doc);
            TASSERT(tw->firstChild() == root);
            TASSERT(tw->nextSibling() == 0);
            TASSERT(tw->lastChild() == comment);
            TASSERT(tw->firstChild() == 0);
            TASSERT(tw->lastChild() == 0);
            TASSERT(tw->nextSibling() == 0);
            TASSERT(tw->nextNode() == 0);
            TASSERT(tw->previousSibling() == E13);
            TASSERT(tw->previousNode() == E122);
            TASSERT(tw->parentNode() == E12);
            TASSERT(tw->firstChild() == textNode2);
            TASSERT(tw->previousSibling() == 0);
            TASSERT(tw->nextSibling() == E121);
            TASSERT(tw->nextNode() == E122);
            TASSERT(tw->parentNode() == E12);
            TASSERT(tw->previousSibling() == E11);
            TASSERT(tw->previousNode() == root);
            TASSERT(tw->previousNode() == doc);
            TASSERT(tw->previousNode() == 0);
            TASSERT(tw->parentNode() == 0);
            TASSERT(tw->getCurrentNode() == doc);
        }



        {
            MyFilter mf(IDOM_Node::ELEMENT_NODE);
            unsigned long whatToShow = IDOM_NodeFilter::SHOW_ALL;
            IDOM_TreeWalker* tw = doc->createTreeWalker(root, whatToShow, &mf, true);

            TASSERT(tw->getCurrentNode() == root);
            TASSERT(tw->parentNode() == 0);  //should not change currentNode
            TASSERT(tw->getCurrentNode() == root);
            TASSERT(tw->nextNode() == E11);
            TASSERT(tw->nextNode() == E111);
            tw->setCurrentNode(E12);
            //when first is not visible, should it go to its sibling?
            TASSERT(tw->firstChild() == E121);   //first visible child
            TASSERT(tw->previousSibling() == 0);
        }



        {
            MyFilter mf(IDOM_Node::ELEMENT_NODE, true);
            unsigned long whatToShow = IDOM_NodeFilter::SHOW_ELEMENT;
            IDOM_TreeWalker* tw = doc->createTreeWalker(root, whatToShow, &mf, true);

            tw->setCurrentNode(E12);
            TASSERT(tw->firstChild() == E121);   //still first visible child
        }



        {
            MyFilter mf(IDOM_Node::TEXT_NODE);
            unsigned long whatToShow = IDOM_NodeFilter::SHOW_TEXT;
            IDOM_TreeWalker* tw = doc->createTreeWalker(root, whatToShow, &mf, true);

            //when first is not visible, should it go to its descendent?
            TASSERT(tw->firstChild() == textNode1);   //E11 skipped
            TASSERT(tw->firstChild() == 0);
            TASSERT(tw->nextNode() == textNode2);
            TASSERT(tw->nextSibling() == 0);
            TASSERT(tw->parentNode() == 0);  //no visible ancestor
            TASSERT(tw->getCurrentNode() == textNode2);
            tw->setCurrentNode(root);
            //when last is not visible, should it go to its sibling & descendent?
            TASSERT(tw->lastChild() == textNode2);   //last visible child
            tw->setCurrentNode(E12);
            //when next sibling is not visible, should it go to its descendent?
            TASSERT(tw->nextSibling() == 0);
        }



        {
            MyFilter mf(IDOM_Node::TEXT_NODE, true);
            unsigned long whatToShow = IDOM_NodeFilter::SHOW_TEXT;
            IDOM_TreeWalker* tw = doc->createTreeWalker(root, whatToShow, &mf, true);

            TASSERT(tw->firstChild() == 0);   //E11 rejected and no children is TEXT
            TASSERT(tw->getCurrentNode() == root);
            TASSERT(tw->nextNode() == 0);    //E11 rejected so can't get to textNode1
        }


    };


    return 0;
    };

