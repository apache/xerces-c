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
#include <dom/DOM.hpp>
#include <dom/DomMemDebug.hpp>
#include <parsers/DOMParser.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLException.hpp>
#include <util/XMLString.hpp>
#include <dom/DOM_Range.hpp>
#include <dom/DOM_XMLDecl.hpp>
#include <framework/MemBufInputSource.hpp>


#define TASSERT(c) tassert((c), __FILE__, __LINE__)

void tassert(bool c, char *file, int line)
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
            // <Body><H1>Title</H1><P>Blah xyz</P>
            //i.e.,            Body
            //     _____________|______________
            //     |                           |
            //     H1                          P
            //     |                           |
            //    "Title"                    "Blah xyz"

  
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
            TASSERT(range.getStartOffset() == 2);
          
            range.setEnd(rt.getFirstChild().getNextSibling().getFirstChild(), 4);
            TASSERT(range.getEndContainer() == rt.getFirstChild().getNextSibling().getFirstChild());
            TASSERT(range.getEndOffset() == 4);

            //inserting text between a text node
            range.insertNode(E210);

            //only end offset moves and new node gets into range as being inserted at boundary point
            TASSERT(range.getStartContainer() == rt.getFirstChild().getNextSibling().getLastChild().getPreviousSibling());
            TASSERT(range.getStartOffset() == 0);
            TASSERT(range.getEndContainer() == rt.getFirstChild().getNextSibling().getLastChild().getPreviousSibling());
            TASSERT(range.getEndOffset() == 2);

            //inserting element node before the selcted text node
            range.insertNode(E120);
            //only end offset moves and new node gets into range as being inserted at boundary point
            TASSERT(range.getStartContainer() == rt.getFirstChild().getNextSibling().getLastChild().getPreviousSibling());
            TASSERT(range.getStartOffset() == 0);
            TASSERT(range.getEndContainer() == rt.getFirstChild().getNextSibling().getLastChild().getPreviousSibling());
            TASSERT(range.getEndOffset() == 2);

           //checking the text replacment
            range.getStartContainer().setNodeValue("ReplacedText");
            //collapsed
            TASSERT(range.getCollapsed() == true);

            //the offsets are set to 0
            TASSERT(range.getStartOffset() == 0);
            TASSERT(range.getEndOffset() == 0);
       
            //changing the selection. Preparing for 'surround'
            range.setStart(range.getStartContainer().getParentNode(), 2);
            range.setEnd(range.getStartContainer(), 5);
            range.surroundContents(E311);          
                 
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
            TASSERT(compVal == -1);
            compVal = range.compareBoundaryPoints(DOM_Range::START_TO_END, aRange);
            TASSERT(compVal == -1);
            compVal = range.compareBoundaryPoints(DOM_Range::END_TO_START, aRange);
            TASSERT(compVal == 1);

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
            
            //testing collapse
            range.collapse(true); //collapse to start
            TASSERT(range.getCollapsed() == true);
            TASSERT(range.getStartOffset() == 0);
            TASSERT(range.getEndOffset() == 0);
            TASSERT(aRange.getEndOffset() == 3); //other range is unaffected
           

            // DOM Tree now looks like this
            //          <Body>
            //  |---------|----------|--------------|
            //  Element3  H1      Element2          P
            //            |                         |
            //  |------|-----------|             "Blah xyz"
            // "Ti" "insertedText" SurroundNode     
            //                     |
            //        |--------|---------------|
            //        Element1 "ReplacedText   "AnotherText"

            // range has H1 as start and end container and 2 as start and end offset. in collapsed state
            // aRange has "Ti" as start with 1 as start offset, H1 as end and 3 as end offset


       
            DOM_DocumentFragment docFrag = aRange.cloneContents();
            TASSERT( docFrag != 0);
            range.selectNode(rt.getFirstChild());
            TASSERT(range.getStartContainer() == rt); 
            
            //Testing toString()
            const char* str = aRange.toString().transcode();
            char* str2 = "iinsertedTextReplacedTextAnotherText";
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
            //detaching the other range
            aRange.detach();
            range.detach();


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

 
    
