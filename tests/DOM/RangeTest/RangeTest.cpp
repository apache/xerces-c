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
 * $Id$
 */


/** This RangeTest tests all of the cases delineated as examples
 *  in the DOM Level 2 Range specification, and a few others.
 *  <p>These do not by any means completely test the API and
 *  corner cases.
 */

#include <stdio.h>
#include <string.h>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DomMemDebug.hpp>
#include <xercesc/parsers/DOMParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM_Range.hpp>
#include <xercesc/dom/DOM_XMLDecl.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>


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

    /*
    Range tests include testing of

    createRange

    setStart, setStartBefore. setStartAfter,
    setEnd, setEndBefore. setEndAfter
    getStartContainer, getStartOffset
    getEndContainer, getEndOffset
    getCommonAncestorContainer
    selectNode
    selectNodeContents
    insertNode
    deleteContents
    collapse
    getCollapsed
    surroundContents
    compareBoundaryPoints
    cloneRange
    cloneContents
    extractContents
    toString
    detach
    removeChild
    */
    {

        {
            DOM_Document    doc = DOM_Document::createDocument();
            //Creating a root element
            DOM_Element     root = doc.createElement("Body");
            doc.appendChild(root);

            //Creating the siblings of root
            DOM_Element     E11 = doc.createElement("H1");
            root.appendChild(E11);

            DOM_Element     E12 = doc.createElement("P");
            root.appendChild(E12);

            //Attaching texts to siblings
            DOM_Text        textNode1 = doc.createTextNode("Title");
            E11.appendChild(textNode1);

            DOM_Text        textNode11 = doc.createTextNode("AnotherText");
            E11.appendChild(textNode11);

            DOM_Text        textNode2 = doc.createTextNode("Blah xyz");
            E12.appendChild(textNode2);

            DOM_Text     E210 = doc.createTextNode("insertedText");


        }


        TESTPROLOG;
        {
            //DOM Tree and some usable node creation
            DOM_Document    doc = DOM_Document::createDocument();
            //Creating a root element
            DOM_Element     root = doc.createElement("Body");
            doc.appendChild(root);

            //Creating the siblings of root
            DOM_Element     E11 = doc.createElement("H1");
            root.appendChild(E11);

            DOM_Element     E12 = doc.createElement("P");
            root.appendChild(E12);

            //Attaching texts to siblings
            DOM_Text        textNode1 = doc.createTextNode("Title");
            E11.appendChild(textNode1);

            DOM_Text        textNode11 = doc.createTextNode("AnotherText");
            E11.appendChild(textNode11);

            DOM_Text        textNode2 = doc.createTextNode("Blah xyz");
            E12.appendChild(textNode2);

            //experimental nodes
            DOM_Element     E120 = doc.createElement("Element1");
            DOM_Element     E121 = doc.createElement("Element2");
            DOM_Element     E122 = doc.createElement("Element3");
            DOM_Element     E311 = doc.createElement("SurroundNode1");

            DOM_Text     E210 = doc.createTextNode("insertedText");

            DOM_Node rt = doc.getDocumentElement();
            DOM_Range range = doc.createRange();



            //Tests start here
            // Initial dom tree looks like :
            // <Body><H1>TitleAnother Text</H1><P>Blah xyz</P></Body>
            //i.e.,            Body(rt)
            //     _____________|________________
            //     |                           |
            //  ___H1(E11)___                    P(E12)
            //  |           |                    |
            //  "Title"  "Another Text"        "Blah xyz"


            //test for start and end settings of a range
            range.setStart(rt.getFirstChild(), 0);
            TASSERT(range.getStartContainer() == rt.getFirstChild() );
            TASSERT(range.getStartOffset() == 0);

            range.setEnd(rt.getFirstChild(), 1);
            TASSERT(range.getEndContainer() == rt.getFirstChild() );
            TASSERT(range.getEndOffset() == 1);


            DOM_Node node = range.getCommonAncestorContainer();
            TASSERT(range.getCommonAncestorContainer() == rt.getFirstChild());

            //selection related test
            range.selectNode(rt.getLastChild());
            TASSERT(range.getStartContainer() == rt);
            TASSERT(range.getStartOffset() == 1);
            TASSERT(range.getEndContainer() == rt);
            TASSERT(range.getEndOffset() == 2);

            //insertion related tests
            range.insertNode(E120);

            //only end offset moves and new node gets into range as being inserted at boundary point
            TASSERT(range.getStartContainer() == rt);
            TASSERT(range.getStartOffset() == 1);
            TASSERT(range.getEndContainer() == rt);
            TASSERT(range.getEndOffset() == 3);

            range.insertNode(E121);
            //only end offset moves and new node gets into range as being inserted at boundary point
            TASSERT(range.getStartContainer() == rt);
            TASSERT(range.getStartOffset() == 1);
            TASSERT(range.getEndContainer() == rt);
            TASSERT(range.getEndOffset() == 4);

            rt.insertBefore(E122, rt.getFirstChild());
            //both offsets move as new node is not part of the range
            TASSERT(range.getStartContainer() == rt);
            TASSERT(range.getStartOffset() == 2);
            TASSERT(range.getEndContainer() == rt);
            TASSERT(range.getEndOffset() == 5);

            //After above operations, now the tree looks like:
            // <Body><Element3/><H1>TitleAnother Text</H1><Element2/><Element1/><P>Blah xyz</P></Body>
            //i.e.,            Body(rt)
            //     _____________|_______________________________________________________________
            //     |                |                  |                |                      |
            //  Element3(E122)  ___H1(E11)___        Element2(E121)    Element1(E120)          P(E12)
            //                  |           |                                                  |
            //               "Title"  "Another Text"                                        "Blah xyz"
            //
            // range has rt as start and end container, and 2 as start offset, 5 as end offset

            //changing selection
            range.selectNode(rt.getLastChild().getPreviousSibling());
            TASSERT(range.getStartContainer() == rt);
            TASSERT(range.getStartOffset() == 3);
            TASSERT(range.getEndContainer() == rt);
            TASSERT(range.getEndOffset() == 4);

            //deleting related tests
            range.deleteContents();
            TASSERT(rt.getLastChild().getPreviousSibling() == E121);

            range.setStart(rt.getFirstChild().getNextSibling().getFirstChild(), 2);
            TASSERT(range.getStartContainer() == rt.getFirstChild().getNextSibling().getFirstChild());
            TASSERT(range.getStartContainer().getNodeValue().equals("Title"));
            TASSERT(range.getStartOffset() == 2);

            range.setEnd(rt.getFirstChild().getNextSibling().getFirstChild(), 4);
            TASSERT(range.getEndContainer() == rt.getFirstChild().getNextSibling().getFirstChild());
            TASSERT(range.getEndContainer().getNodeValue().equals("Title"));
            TASSERT(range.getEndOffset() == 4);
            TASSERT(range.toString().equals("tl"));

            //inserting text between a text node
            range.insertNode(E210);

            //only end offset moves and new node gets into range as being inserted at boundary point
            TASSERT(range.getStartContainer() == rt.getFirstChild().getNextSibling().getFirstChild());
            TASSERT(range.getStartOffset() == 2);
            TASSERT(range.getEndContainer() == rt.getFirstChild().getNextSibling().getLastChild().getPreviousSibling());
            TASSERT(range.getEndOffset() == 2);

            //inserting element node before the selected text node
            range.insertNode(E120);
            //only end offset moves and new node gets into range as being inserted at boundary point
            TASSERT(range.getStartContainer() == rt.getFirstChild().getNextSibling().getFirstChild());
            TASSERT(range.getStartContainer().getNodeValue().equals("Ti"));
            TASSERT(range.getStartOffset() == 2);
            TASSERT(range.getEndContainer() == rt.getFirstChild().getNextSibling().getLastChild().getPreviousSibling());
            TASSERT(range.getEndContainer().getNodeValue().equals("tle"));
            TASSERT(range.getEndOffset() == 2);
            TASSERT(E11.getChildNodes().getLength()==6);

           //checking the text replacment
            range.getStartContainer().setNodeValue("ReplacedText");
            //only the start offset is impact
            TASSERT(range.getStartContainer() == rt.getFirstChild().getNextSibling().getFirstChild());
            TASSERT(range.getStartContainer().getNodeValue().equals("ReplacedText"));
            TASSERT(range.getStartOffset() == 0);
            TASSERT(range.getEndContainer() == rt.getFirstChild().getNextSibling().getLastChild().getPreviousSibling());
            TASSERT(range.getEndContainer().getNodeValue().equals("tle"));
            TASSERT(range.getEndOffset() == 2);

            //After above operations, now the tree looks like:
            // <Body><Element3/><H1>ReplacedText<Element1/>insertedTexttleAnother Text</H1><Element2/><P>Blah xyz</P></Body>
            //i.e.,            Body(rt)
            //     _____________|_______________________________________________________________________________________________
            //     |                |                                                                          |                |
            //  Element3(E122)  ___H1(E11)___________________________________________________________        Element2(E121)    P(E12)
            //                  |              |     |                |                      |      |                             |
            //               "ReplacedText"   ""   Element1(E120)   "insertedText"(E210)   "tle"  "Another Text"              "Blah xyz"
            //
            // range has "ReplacedText" as start container and "tle" as end container
            //   and 0 as start offset, 2 as end offset

            //changing the selection. Preparing for 'surround'
            range.setStart(range.getStartContainer().getParentNode(), 2);
            range.setEnd(range.getStartContainer(), 5);
            TASSERT(range.getStartContainer().getNodeName().equals("H1"));
            TASSERT(range.getEndContainer().getNodeName().equals("H1"));
            TASSERT(range.toString().equals("insertedTexttle"));

            range.surroundContents(E311);
            TASSERT(range.getStartContainer().getNodeName().equals("H1"));
            TASSERT(range.getStartOffset() == 2);
            TASSERT(range.getEndContainer().getNodeName().equals("H1"));
            TASSERT(range.getEndOffset() == 3);
            TASSERT(E11.getChildNodes().getLength()==4);
            TASSERT(E311.getChildNodes().getLength()==3);
            TASSERT(range.toString().equals("insertedTexttle"));

            //After above operations, now the tree looks like:
            // <Body><Element3/><H1>ReplacedText<SurroundNode1><Element1/>insertedTexttle</SurroundNode1>Another Text</H1><Element2/><P>Blah xyz</P></Body>
            //i.e.,            Body(rt)
            //     _____________|_________________________________________________________________________
            //     |                |                                                    |                |
            //  Element3(E122)  ___H1(E11)___________________________________        Element2(E121)    P(E12)
            //                  |              |     |                      |                            |
            //               "ReplacedText"   ""   SurroundNode1(E311)  "Another Text"              "Blah xyz"
            //                          ____________ |_____________________________
            //                          |                    |                    |
            //                          Element1(E120)   "insertedText"(E210)   "tle"
            //
            // range has H1 as start and end container and 2 as start offset, 3 as end offset

            //testing cloning
            DOM_Range aRange = range.cloneRange();

            TASSERT(aRange.getStartContainer() == range.getStartContainer());
            TASSERT(aRange.getEndContainer() == range.getEndContainer());
            TASSERT(aRange.getStartOffset() == 2);
            TASSERT(aRange.getEndOffset() == 3);
            //changing the new ranges start
            aRange.setStart(aRange.getStartContainer().getFirstChild(), 1);

            //comparing the ranges
            short compVal = range.compareBoundaryPoints(DOM_Range::END_TO_END, aRange);
            TASSERT(compVal == 0);
            compVal = range.compareBoundaryPoints(DOM_Range::START_TO_START, aRange);
            TASSERT(compVal == 1);
            compVal = range.compareBoundaryPoints(DOM_Range::START_TO_END, aRange);
            TASSERT(compVal == 1);
            compVal = range.compareBoundaryPoints(DOM_Range::END_TO_START, aRange);
            TASSERT(compVal == -1);

            //testing collapse
            //not collapsed
            TASSERT(range.getCollapsed() == false);
            TASSERT(range.getStartOffset() == 2);
            TASSERT(range.getEndOffset() == 3);

            //selectNodeContents
            range.selectNodeContents(rt.getLastChild().getFirstChild());
            TASSERT(range.getStartContainer() == rt.getLastChild().getFirstChild());
            TASSERT(range.getEndContainer() == rt.getLastChild().getFirstChild());
            TASSERT(range.getStartOffset() == 0);
            TASSERT(range.getEndOffset() == 8);
            TASSERT(range.toString().equals("Blah xyz"));

            //testing collapse
            range.collapse(true); //collapse to start
            TASSERT(range.getCollapsed() == true);
            TASSERT(range.getStartOffset() == 0);
            TASSERT(range.getEndOffset() == 0);
            TASSERT(range.toString().equals(""));
            TASSERT(aRange.getEndOffset() == 3); //other range is unaffected
            TASSERT(aRange.toString().equals("eplacedTextinsertedTexttle"));

            //After above operations, now the tree looks like:
            // <Body><Element3/><H1>ReplacedText<SurroundNode1><Element1/>insertedTexttle</SurroundNode1>Another Text</H1><Element2/><P>Blah xyz</P></Body>
            //i.e.,            Body(rt)
            //     _____________|_________________________________________________________________________
            //     |                |                                                    |                |
            //  Element3(E122)  ___H1(E11)___________________________________        Element2(E121)    P(E12)
            //                  |              |     |                      |                            |
            //               "ReplacedText"   ""   SurroundNode1(E311)  "Another Text"              "Blah xyz"
            //                          ____________ |_____________________________
            //                          |                    |                    |
            //                          Element1(E120)   "insertedText"(E210)   "tle"
            //
            // range has "Blah xyz" as start and end container and 0 as start and end offset (collapsed)
            // aRange has "ReplacedText" as start container and H1 as end container
            //    and 1 as start offset and 3 as end offset

            DOM_DocumentFragment docFrag = aRange.cloneContents();
            TASSERT( docFrag != 0);
            range.selectNode(rt.getFirstChild());
            TASSERT(range.getStartContainer() == rt);
            TASSERT(range.getEndContainer() == rt);
            TASSERT(range.getStartOffset() == 0);
            TASSERT(range.getEndOffset() == 1);

            //Testing toString()
            const char* str = aRange.toString().transcode();
            const char* str2 = "eplacedTextinsertedTexttle";
            TASSERT(*str == *str2);

            //start and end before and after tests
            range.setStartBefore(rt.getFirstChild());
            TASSERT(range.getStartOffset() == 0);
            range.setEndBefore(rt.getFirstChild());
            TASSERT(range.getEndOffset() == 0);
            range.setStartAfter(rt.getLastChild());
            TASSERT(range.getStartOffset() == 4);

            range.setStartAfter(rt.getFirstChild());
            TASSERT(range.getStartOffset() == 1);

            range.setEndBefore(rt.getLastChild());
            TASSERT(range.getEndOffset() == 3);

            range.setEndAfter(rt.getLastChild());
            TASSERT(range.getEndOffset() == 4);

            //testing extract()
            DOM_DocumentFragment frag2 = range.extractContents();
            TASSERT( frag2 != 0);

            //After above operations, now the tree looks like:
            // <Body><Element3/></Body>
            //i.e.,            Body(rt)
            //                  |
            //               Element3(E122)
            //
            // aRange has rt as start and end container, and 1 as start and end offset (collapsed)
            // range has rt as start and end container, and 1 as start and end offset (collapsed)
            //
            //and frag2 looks:
            // <Body>ReplacedText<SurroundNode1><Element1/>insertedTexttle</SurroundNode1>Another Text</H1><Element2/><P>Blah xyz</P></Body>
            //i.e.,             Body(rt)
            //      ______________|________________________________________________________
            //      |                                                    |                |
            //   ___H1(E11)___________________________________        Element2(E121)    P(E12)
            //   |              |     |                      |                            |
            //"ReplacedText"   ""   SurroundNode1(E311)  "Another Text"              "Blah xyz"
            //           ____________ |_____________________________
            //           |                    |                    |
            //        Element1(E120)   "insertedText"(E210)   "tle"
            //

            //the tree do not have those node anymore after extract
            //only Element3 left
            TASSERT(rt.getChildNodes().getLength()==1);

            //aRange is collapsed
            TASSERT(aRange.getCollapsed() == true);
            TASSERT(aRange.getStartContainer() == rt);
            TASSERT(aRange.getStartOffset() == 1);
            TASSERT(aRange.getEndContainer() == rt);
            TASSERT(aRange.getEndOffset() == 1);

            //range is collapsed as well
            TASSERT(range.getCollapsed() == true);
            TASSERT(range.getStartContainer() == rt);
            TASSERT(range.getStartOffset() == 1);
            TASSERT(range.getEndContainer() == rt);
            TASSERT(range.getEndOffset() == 1);

            //test the document fragment frag2
            TASSERT(frag2.getChildNodes().getLength()==3);

            //detaching the other range
            aRange.detach();
            range.detach();

           //***************************************************************
           //another set of test
           //TEST createRange, setStart and setEnd, insertnode
           //***************************************************************
           DOM_Document doc2 = DOM_Document::createDocument();
           DOM_Element root2 = doc2.createElement("root2");
           doc2.appendChild(root2);
           //case 1: simple text node, start==end
           // <body>text1</body>
           DOM_Element body = doc2.createElement("body");
           DOM_Text text1 = doc2.createTextNode("text1");
           body.appendChild(text1);
           root2.appendChild(body);

           //set range
           DOM_Range range1 = doc2.createRange();
           range1.setStart(text1,1);
           range1.setEnd(text1,3);

           TASSERT(range1.toString().equals("ex"));
           TASSERT(range1.getStartOffset()==1);
           TASSERT(range1.getStartContainer().getNodeValue().equals("text1"));
           TASSERT(range1.getEndOffset()==3);
           TASSERT(range1.getEndContainer().getNodeValue().equals("text1"));

           //now insert a text node
           //<body>ttext2ext1</body>
           DOM_Text text2 = doc2.createTextNode("text2");
           range1.insertNode(text2);

           TASSERT(range1.toString().equals("text2ex"));
           TASSERT(range1.getStartOffset()==1);
           TASSERT(range1.getStartContainer().getNodeValue().equals("t"));
           TASSERT(range1.getEndOffset()==2);
           TASSERT(range1.getEndContainer().getNodeValue().equals("ext1"));

           //now insert a non-text node
           //<body>t<p1/>text2ext1</body>
           DOM_Element p1 = doc2.createElement("p1");
           range1.insertNode(p1);

           TASSERT(range1.toString().equals("text2ex"));
           TASSERT(range1.getStartOffset()==1);
           TASSERT(range1.getStartContainer().getNodeValue().equals("t"));
           TASSERT(range1.getEndOffset()==2);
           TASSERT(range1.getEndContainer().getNodeValue().equals("ext1"));

           //case 2: non-text node, start==end
           // <head><h1/></head>
           DOM_Element head = doc2.createElement("head");
           DOM_Element h1 = doc2.createElement("h1");
           head.appendChild(h1);
           root2.appendChild(head);

           //set range
           DOM_Range range2 = doc2.createRange();
           range2.setStart(head,0);
           range2.setEnd(head,1);

           TASSERT(range2.toString().equals(""));
           TASSERT(range2.getStartOffset()==0);
           TASSERT(range2.getStartContainer().getNodeName().equals("head"));
           TASSERT(range2.getEndOffset()==1);
           TASSERT(range2.getEndContainer().getNodeName().equals("head"));

           //now insert a non-text node
           //<head><h2/><h1/></head>
           DOM_Element h2 = doc2.createElement("h2");
           range2.insertNode(h2);

           TASSERT(range2.toString().equals(""));
           TASSERT(range2.getStartOffset()==0);
           TASSERT(range2.getStartContainer().getNodeName().equals("head"));
           TASSERT(range2.getEndOffset()==2);
           TASSERT(range2.getEndContainer().getNodeName().equals("head"));

           //now insert a text node
           //<head>text5<h2/><h1/></head>
           DOM_Text text5 = doc2.createTextNode("text5");
           range2.insertNode(text5);

           TASSERT(range2.toString().equals("text5"));
           TASSERT(range2.getStartOffset()==0);
           TASSERT(range2.getStartContainer().getNodeName().equals("head"));
           TASSERT(range2.getEndOffset()==3);
           TASSERT(range2.getEndContainer().getNodeName().equals("head"));

           //case 3: simple text node, start!=end
           // <body2>text3</body2>
           DOM_Element body2 = doc2.createElement("body2");
           DOM_Text text3 = doc2.createTextNode("text3");
           body2.appendChild(text3);
           root2.appendChild(body2);

           //set range
           DOM_Range range3 = doc2.createRange();
           range3.setStart(text3,1);
           range3.setEnd(body2,1);

           TASSERT(range3.toString().equals("ext3"));
           TASSERT(range3.getStartOffset()==1);
           TASSERT(range3.getStartContainer().getNodeValue().equals("text3"));
           TASSERT(range3.getEndOffset()==1);
           TASSERT(range3.getEndContainer().getNodeName().equals("body2"));

           //now insert a textnode
           //<body2>ttext4ext3</body2>
           DOM_Text text4 = doc2.createTextNode("text4");
           range3.insertNode(text4);

           TASSERT(range3.toString().equals(""));
           TASSERT(range3.getStartOffset()==1);
           TASSERT(range3.getStartContainer().getNodeValue().equals("t"));
           TASSERT(range3.getEndOffset()==1);
           TASSERT(range3.getEndContainer().getNodeName().equals("body2"));

           //now insert a non-text node
           //<body2>t<p2/>text4ext3</body2>
           DOM_Element p2 = doc2.createElement("p2");
           range3.insertNode(p2);

           //extra empty node caused by splitting 't'
           TASSERT(range3.toString().equals(""));
           TASSERT(range3.getStartOffset()==1);
           TASSERT(range3.getStartContainer().getNodeValue().equals("t"));
           TASSERT(range3.getEndOffset()==1);
           TASSERT(range3.getEndContainer().getNodeName().equals("body2"));

           //test toString a bit
           range3.setStart(body2,1);
           range3.setEnd(body2,5);

           TASSERT(range3.toString().equals("text4ext3"));

           range3.setStart(body2,0);
           range3.setEnd(body2,5);

           TASSERT(range3.toString().equals("ttext4ext3"));

           //case 4: non-text node, start!=end
           // <head2><h3/></head2>
           DOM_Element head2 = doc2.createElement("head2");
           DOM_Element h3 = doc2.createElement("h3");
           head2.appendChild(h3);
           root2.appendChild(head2);

           //set range
           DOM_Range range4 = doc2.createRange();
           range4.setStart(head2,0);
           range4.setEnd(h3,0);

           TASSERT(range4.toString().equals(""));
           TASSERT(range4.getStartOffset()==0);
           TASSERT(range4.getStartContainer().getNodeName().equals("head2"));
           TASSERT(range4.getEndOffset()==0);
           TASSERT(range4.getEndContainer().getNodeName().equals("h3"));

           //now insert a non-text node
           //<head2><h4/><h3/></head2>
           DOM_Element h4 = doc2.createElement("h4");
           range4.insertNode(h4);

           TASSERT(range4.toString().equals(""));
           TASSERT(range4.getStartOffset()==0);
           TASSERT(range4.getStartContainer().getNodeName().equals("head2"));
           TASSERT(range4.getEndOffset()==0);
           TASSERT(range4.getEndContainer().getNodeName().equals("h3"));

           //now insert a text node
           //<head2>text6<h4/><h3/></head2>
           DOM_Text text6 = doc2.createTextNode("text6");
           range4.insertNode(text6);

           TASSERT(range4.toString().equals("text6"));
           TASSERT(range4.getStartOffset()==0);
           TASSERT(range4.getStartContainer().getNodeName().equals("head2"));
           TASSERT(range4.getEndOffset()==0);
           TASSERT(range4.getEndContainer().getNodeName().equals("h3"));

           //***************************************************************
           // quick test of updating
           //***************************************************************
           // <upbody>text1</upbody>
           DOM_Element upbody = doc2.createElement("upbody");
           DOM_Text uptext1 = doc2.createTextNode("uptext1");
           upbody.appendChild(uptext1);
           root2.appendChild(upbody);

           DOM_Range uprange = doc2.createRange();
           uprange.setStart(upbody,0);
           uprange.setEnd(upbody,1);

           TASSERT(uprange.toString().equals("uptext1"));
           TASSERT(uprange.getStartOffset()==0);
           TASSERT(uprange.getStartContainer().getNodeName().equals("upbody"));
           TASSERT(uprange.getEndOffset()==1);
           TASSERT(uprange.getEndContainer().getNodeName().equals("upbody"));

           // split text
           uptext1.splitText(1);

           TASSERT(uprange.toString().equals("u"));
           TASSERT(uprange.getStartOffset()==0);
           TASSERT(uprange.getStartContainer().getNodeName().equals("upbody"));
           TASSERT(uprange.getEndOffset()==1);
           TASSERT(uprange.getEndContainer().getNodeName().equals("upbody"));

           //insert node
           DOM_Element upbody2 = doc2.createElement("upbody2");
           DOM_Text uptext2 = doc2.createTextNode("uptext2");
           upbody2.appendChild(uptext2);
           root2.appendChild(upbody2);

           DOM_Range uprange2 = doc2.createRange();
           uprange2.setStart(uptext2,1);
           uprange2.setEnd(upbody2,1);

           DOM_Range uprange3 = doc2.createRange();
           uprange3.setStart(uptext2,1);
           uprange3.setEnd(upbody2,1);

           TASSERT(uprange2.toString().equals("ptext2"));
           TASSERT(uprange2.getStartOffset()==1);
           TASSERT(uprange2.getStartContainer().getNodeValue().equals("uptext2"));
           TASSERT(uprange2.getEndOffset()==1);
           TASSERT(uprange2.getEndContainer().getNodeName().equals("upbody2"));

           TASSERT(uprange3.toString().equals("ptext2"));
           TASSERT(uprange3.getStartOffset()==1);
           TASSERT(uprange3.getStartContainer().getNodeValue().equals("uptext2"));
           TASSERT(uprange3.getEndOffset()==1);
           TASSERT(uprange3.getEndContainer().getNodeName().equals("upbody2"));

           DOM_Element upp1 = doc2.createElement("upp1");
           uprange2.insertNode(upp1);

           TASSERT(uprange2.toString().equals(""));
           TASSERT(uprange2.getStartOffset()==1);
           TASSERT(uprange2.getStartContainer().getNodeValue().equals("u"));
           TASSERT(uprange2.getEndOffset()==1);
           TASSERT(uprange2.getEndContainer().getNodeName().equals("upbody2"));

           TASSERT(uprange3.toString().equals(""));
           TASSERT(uprange3.getStartOffset()==1);
           TASSERT(uprange3.getStartContainer().getNodeValue().equals("u"));
           TASSERT(uprange3.getEndOffset()==1);
           TASSERT(uprange3.getEndContainer().getNodeName().equals("upbody2"));

           //***************************************************************
           //another set of test
           //<foo><c/><moo><b/></moo>ab<a>Hello cd</a><cool>ef</cool></foo>
           //
           //  ______________________foo_____________________
           //  |          |           |          |           |
           //  c         moo        "ab"         a          cool
           //             |                      |           |
           //             b                    "Hello cd"   "ef"
           //
           DOM_Document doc3 = DOM_Document::createDocument();
           DOM_Element root3 = doc3.createElement("root");
           doc3.appendChild(root3);

           DOM_Element foo = doc3.createElement("foo");
           DOM_Element moo = doc3.createElement("moo");
           DOM_Element cool = doc3.createElement("cool");
           DOM_Text ab = doc3.createTextNode("ab");
           DOM_Text cd = doc3.createTextNode("Hello cd");
           DOM_Text ef = doc3.createTextNode("ef");

           DOM_Element a = doc3.createElement("a");
           DOM_Element b = doc3.createElement("b");
           DOM_Element c = doc3.createElement("c");

           root3.appendChild(foo);
           foo.appendChild(c);
           foo.appendChild(moo);
           foo.appendChild(ab);
           foo.appendChild(a);
           foo.appendChild(cool);
           moo.appendChild(b);
           a.appendChild(cd);
           cool.appendChild(ef);

           //***************************************************************
           //TEST toString
           //***************************************************************
           DOM_Range newtestrange = doc3.createRange();
           //case 1:
           //start container is text node
           //   i) end container is also text node
           //    a) start==end
           //    b) start!=end
           //  ii) end container is not text node
           //    a) start==end => impossible
           //    b) start!=end
           //
           //case 2:
           //start container is not text node
           //   i) end container is text node
           //    a) start==end => impossible
           //    b) start!=end
           //  ii) end container is not text node
           //    a) start==end
           //    b) start!=end

           //case 1, i, a
           newtestrange.setStart( cd, 1 );
           newtestrange.setEnd( cd, 4 );

           TASSERT(newtestrange.toString().equals("ell"));

           //case 1, i, b
           newtestrange.setStart( cd, 1 );
           newtestrange.setEnd( ef, 2 );

           TASSERT(newtestrange.toString().equals("ello cdef"));

           //case 1, ii, b
           newtestrange.setStart( cd, 1 );
           newtestrange.setEnd( foo, 4 );

           TASSERT(newtestrange.toString().equals("ello cd"));

           //case 2, i, b
           newtestrange.setStart( foo, 1 );
           newtestrange.setEnd( cd, 5 );

           TASSERT(newtestrange.toString().equals("abHello"));

           //case 2, ii, a
           newtestrange.setStart( foo, 1 );
           newtestrange.setEnd( foo, 4 );

           TASSERT(newtestrange.toString().equals("abHello cd"));

           //case 2, ii, b
           newtestrange.setStart( moo, 1 );
           newtestrange.setEnd( foo, 4 );

           TASSERT(newtestrange.toString().equals("abHello cd"));

           //***************************************************************
           //test removeChild
           //***************************************************************
           DOM_Range newrange = doc3.createRange();
           newrange.setStart( moo, 0 );
           newrange.setEnd( foo, 4 );

           TASSERT(newrange.getStartOffset()==0);
           TASSERT(newrange.getStartContainer().getNodeName().equals("moo"));
           TASSERT(newrange.getEndOffset()==4);
           TASSERT(newrange.getEndContainer().getNodeName().equals("foo"));
           TASSERT(newrange.toString().equals("abHello cd"));

           DOM_Node n = newrange.cloneContents();
           DOM_NodeList nol = foo.getChildNodes();

           //removing moo
           foo.removeChild(nol.item(1));
           TASSERT(newrange.getStartOffset()==1);
           TASSERT(newrange.getStartContainer().getNodeName().equals("foo"));
           TASSERT(newrange.getEndOffset()==3);
           TASSERT(newrange.getEndContainer().getNodeName().equals("foo"));
           TASSERT(newrange.toString().equals("abHello cd"));

           TASSERT(newtestrange.getStartOffset()==1);
           TASSERT(newtestrange.getStartContainer().getNodeName().equals("foo"));
           TASSERT(newtestrange.getEndOffset()==3);
           TASSERT(newtestrange.getEndContainer().getNodeName().equals("foo"));
           TASSERT(newtestrange.toString().equals("abHello cd"));

    }
    TESTEPILOG;


    } //creating the dom tree and tests


    //
    //  Print Final allocation stats for full test
    //
    DomMemDebug().print();

    // And call the termination method
    XMLPlatformUtils::Terminate();

    return 0;
};



