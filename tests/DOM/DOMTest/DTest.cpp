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
 * Revision 1.11  2000/04/19 02:25:18  aruna1
 * Full support for DOM_EntityReference, DOM_Entity and DOM_DocumentType introduced
 *
 * Revision 1.10  2000/03/11 03:00:24  chchou
 * Fix bug # 18, remove set method of readonly attributes.
 * As a result, remove related test cases
 *
 * Revision 1.9  2000/03/02 19:55:45  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.8  2000/02/29 02:19:13  rahulj
 * No more compilation errors under HPUX 11.0. We do not build
 * DOMMemTest as it crashes the aCC compiler.
 *
 * Revision 1.7  2000/02/06 07:48:36  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.6  2000/02/04 05:46:31  andyh
 * Change offsets and lengths form signed to unsigned
 *
 * Revision 1.5  2000/02/04 01:50:13  aruna1
 * TreeWalker and NodeIterator changes
 *
 * Revision 1.4  2000/01/29 00:39:09  andyh
 * Redo synchronization in DOMStringHandle allocator.  There
 * was a bug in the use of Compare and Swap.  Switched to mutexes.
 *
 * Changed a few plain deletes to delete [].
 *
 * Revision 1.3  2000/01/19 21:40:58  andyh
 * Remove a few remaining dependencies on the (now defunct)
 * XML StdOut stream.
 *
 * Revision 1.2  1999/12/03 00:02:25  andyh
 * DOM tests updated to catch DOMException by ref.
 * Added tests of DOMString::transcode() and append()
 *
 * Revision 1.1.1.1  1999/11/09 01:02:34  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:42:24  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */



/**
 * This class tests methods for XML DOM implementation
 * version 2.0 10/12/98
 * 
 * DOM_DOMException errors are tested by calls to DOMExceptionsTest from: Main, docBuilder...
 *
 */
 
#include <stdio.h>
#include "DTest.h"
#include <util/PlatformUtils.hpp>
#include <util/XMLException.hpp>
#include <util/XMLString.hpp>


#define EXCEPTIONSTEST(operation, expectedException, resultFlag, testNum) \
    {                                                               \
        try                                                         \
        {                                                           \
            operation;                                              \
            fprintf(stderr, "Exceptions Test # %d: no Exception thrown.\n", testNum); \
        }                                                           \
        catch (DOM_DOMException &e)                                 \
        {                                                           \
            if (e.code != expectedException) {                      \
                fprintf(stderr, "Exceptions Test # %d: wrong DOM_DOMException thrown.\n", \
                    testNum);                                       \
                resultFlag = false;                                 \
            }                                                       \
        }                                                           \
        catch (...)                                                 \
        {                                                           \
            fprintf(stderr, "Exceptions Test # %d: unknown exception thrown.\n",    \
                testNum);                                           \
            resultFlag = false;                                     \
        }                                                           \
    }  


#define LEAKTEST(operation)                             \
{                                                       \
    operation;   /* Precondition  */                    \
    DomMemDebug entryMemState;                          \
    {                                                   \
        operation;                                      \
    }                                                   \
    DomMemDebug exitMemState;                           \
    if (entryMemState != exitMemState)                  \
    {                                                   \
        printf("Memory Leak at line %d\n", __LINE__);   \
        exitMemState.printDifference(entryMemState);    \
    }                                                   \
}




DOM_Element                 DTest::testElementNode;
DOM_Attr                    DTest::testAttributeNode;
DOM_Text                    DTest::testTextNode;
DOM_CDATASection            DTest::testCDATASectionNode;
DOM_EntityReference         DTest::testEntityReferenceNode;
DOM_Entity                  DTest::testEntityNode;
DOM_ProcessingInstruction   DTest::testProcessingInstructionNode;
DOM_Comment                 DTest::testCommentNode;
DOM_Document                DTest::testDocumentNode;
DOM_DocumentType            DTest::testDocumentTypeNode;
DOM_DocumentFragment        DTest::testDocumentFragmentNode;
DOM_Notation                DTest::testNotationNode;

/**
 * 
 * version 2.0 10/12/98
 *
 */
    
DTest::DTest()
{
};


/**
 * version 3.0 01/25/99
 *  
 * @return DOM_Document
 *
 */
DOM_Document DTest::createDocument() {
    return DOM_Document::createDocument();
};


/**
 * version 3.0 01/25/99
 * 
 * @return DOM_DocumentType
 * @param name DOMString
 *
 */
DOM_DocumentType DTest::createDocumentType(DOM_Document doc, DOMString name) {
    return doc.createDocumentType(name);    //Replace with a DOM_DocumentType creator
};


/**
 * version 3.0 01/25/99
 *  
 * @return org.w3c.dom.DOM_Entity
 * @param doc org.w3c.dom.DOM_Document
 * @param name DOMString
 *
 */
DOM_Entity DTest::createEntity(DOM_Document doc, DOMString name) {
    return doc.createEntity(name);
};



/**
 * version 3.0 01/25/99
 * 
 * @return org.w3c.dom.DOM_Notation
 * @param doc org.w3c.dom.DOM_Document
 * @param name DOMString
 *
 */
DOM_Notation DTest::createNotation(DOM_Document doc, DOMString name) {
    return doc.createNotation(name);
};


/**
 * This method builds test documents for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 * @param name document's name
 * @param type document's type
 *
 */
void DTest::docBuilder(DOM_Document document, DOMString name)
{
    DOM_Document doc = document;
    bool OK = true;
        
    DOM_Element docFirstElement = doc.createElement(name + "FirstElement");
    doc.appendChild(docFirstElement);
    docFirstElement.setAttribute(name + "FirstElement", name + "firstElement");
    
    DOM_ProcessingInstruction docProcessingInstruction = doc.createProcessingInstruction(name +
                    "TargetProcessorChannel", DOMString("This is ") + doc.getNodeName() + "'s processing instruction");
    docFirstElement.appendChild(docProcessingInstruction);
    
    DOM_Element docBody = doc.createElement(name + "TestBody");
    docFirstElement.appendChild(docBody);
    
    DOM_Element docBodyLevel21 = doc.createElement(name + "BodyLevel21");
    DOM_Element docBodyLevel22 = doc.createElement(name + "BodyLevel22");
    DOM_Element docBodyLevel23 = doc.createElement(name + "BodyLevel23");
    DOM_Element docBodyLevel24 = doc.createElement(name + "BodyLevel24");
    docBody.appendChild(docBodyLevel21);
    docBody.appendChild(docBodyLevel22);
    docBody.appendChild(docBodyLevel23);
    docBody.appendChild(docBodyLevel24);
    
    DOM_Element docBodyLevel31 = doc.createElement(name + "BodyLevel31");
    DOM_Element docBodyLevel32 = doc.createElement(name + "BodyLevel32");
    DOM_Element docBodyLevel33 = doc.createElement(name + "BodyLevel33");
    DOM_Element docBodyLevel34 = doc.createElement(name + "BodyLevel34");
    docBodyLevel21.appendChild(docBodyLevel31);
    docBodyLevel21.appendChild(docBodyLevel32);
    docBodyLevel22.appendChild(docBodyLevel33);
    docBodyLevel22.appendChild(docBodyLevel34);
    
    DOM_Text docTextNode11 = doc.createTextNode(name + "BodyLevel31'sChildTextNode11");
    DOM_Text docTextNode12 = doc.createTextNode(name + "BodyLevel31'sChildTextNode12");
    DOM_Text docTextNode13 = doc.createTextNode(name + "BodyLevel31'sChildTextNode13");
    DOM_Text docTextNode2 = doc.createTextNode(name + "TextNode2");
    DOM_Text docTextNode3 = doc.createTextNode(name + "TextNode3");
    DOM_Text docTextNode4 = doc.createTextNode(name + "TextNode4");
    docBodyLevel31.appendChild(docTextNode11);
    docBodyLevel31.appendChild(docTextNode12);
    docBodyLevel31.appendChild(docTextNode13);
    docBodyLevel32.appendChild(docTextNode2);
    docBodyLevel33.appendChild(docTextNode3);
    docBodyLevel34.appendChild(docTextNode4);
    
    DOM_CDATASection docCDATASection = doc.createCDATASection("<![CDATA[<greeting>Hello, world!</greeting>]]>");
    docBodyLevel23.appendChild(docCDATASection);
    
    DOM_Comment docComment = doc.createComment("This should be a comment of some kind ");
    docBodyLevel23.appendChild(docComment);
    
    DOM_EntityReference docReferenceEntity = doc.createEntityReference("ourEntityNode");
    docBodyLevel24.appendChild(docReferenceEntity);

    DTest make;
    DOM_Notation docNotation = make.createNotation(doc, "ourNotationNode");
    DOM_Node abc1 = doc.getFirstChild();
    DOM_DocumentType docType = (DOM_DocumentType &) abc1;
    docType.getNotations().setNamedItem(docNotation);
    
    DOM_DocumentFragment docDocFragment = doc.createDocumentFragment();
    
//  printf("This document's first element name is " + docFirstElement.getTagName() + "\n");


//***********Following are for errorTests
    DOM_Text docNode3 = doc.createTextNode(name + "docTextNode3");
    DOM_Text docNode4 = doc.createTextNode(name + "docTextNode4");
    
    DOM_Node  abc2 =  doc.getDoctype().getEntities().getNamedItem("ourEntityNode"); // Get the DOM_Entity node
    DOM_Entity docEntity = (DOM_Entity &) abc2;
    DOM_Node  abc3 = doc.getFirstChild(); // Get the DOM_DocumentType node
    DOM_DocumentType docDocType = (DOM_DocumentType &) abc3;
    DOM_Node  abc4 = doc.getLastChild().getLastChild().getLastChild().getFirstChild();
    DOM_EntityReference & entityReferenceText = (DOM_EntityReference &) abc4;
    DOM_Text entityReferenceText2 = doc.createTextNode("entityReferenceText information");
//************************************************* ERROR TESTS
    DTest tests;

    EXCEPTIONSTEST(document.appendChild(docBody), DOM_DOMException::HIERARCHY_REQUEST_ERR, OK,  1);

    EXCEPTIONSTEST(document.appendChild(docBody), DOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 2);
    EXCEPTIONSTEST(docNode3.appendChild(docNode4), DOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 3); 
    EXCEPTIONSTEST(doc.insertBefore(docEntity, docFirstElement), DOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 4); 
    EXCEPTIONSTEST(doc.replaceChild(docCDATASection, docFirstElement), DOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 5); 
    EXCEPTIONSTEST(docFirstElement.setNodeValue("This shouldn't work!" ), DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 6);    
    EXCEPTIONSTEST(docReferenceEntity.setNodeValue("This shouldn't work!" ), DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 7);
    EXCEPTIONSTEST(docEntity.setNodeValue("This shouldn't work!" ), DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 8);
    EXCEPTIONSTEST(doc.setNodeValue("This shouldn't work!" ), DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 9);
    EXCEPTIONSTEST(docDocType.setNodeValue("This shouldn't work!" ), DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 10);
    EXCEPTIONSTEST(docDocFragment.setNodeValue("This shouldn't work!" ), DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 11);
    EXCEPTIONSTEST(docNotation.setNodeValue("This shouldn't work!" ), DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 12);
    EXCEPTIONSTEST(docReferenceEntity.appendChild(entityReferenceText2 ), DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR , OK, 13);
    EXCEPTIONSTEST(docBodyLevel32.insertBefore(docTextNode11,docBody ), DOM_DOMException::NOT_FOUND_ERR, OK, 14);
    EXCEPTIONSTEST(docBodyLevel32.removeChild(docFirstElement), DOM_DOMException::NOT_FOUND_ERR, OK, 15);
    EXCEPTIONSTEST(docBodyLevel32.replaceChild(docTextNode11,docFirstElement ), DOM_DOMException::NOT_FOUND_ERR, OK, 16);


//!! Throws a NOT_FOUND_ERR ********
     
     // docBodyLevel32.getAttributes().removeNamedItem(testAttribute.getName());    16  // To test removeNamedItem
     
};  //END OF DOCBUILDER



/**
 * @param document org.w3c.dom.DOM_Document
 */
 void DTest::findTestNodes(DOM_Document document) {
    DOM_Node node = document;
    int nodeCount = 0;

    // Walk the tree until you find and assign all node types needed that exist.
    while (node != null && nodeCount < 12)
    {

        switch (node.getNodeType())
    {
        case DOM_Node::ELEMENT_NODE :
            if (testElementNode == null) {testElementNode = (DOM_Element &)node; nodeCount++;}
            break;
        case DOM_Node::ATTRIBUTE_NODE :
            if (testAttributeNode == null) {testAttributeNode = (DOM_Attr &)node; nodeCount++;}
            break;
        case DOM_Node::TEXT_NODE :
            if (testTextNode == null) {testTextNode = (DOM_Text &)node; nodeCount++;}
            break;
        case DOM_Node::CDATA_SECTION_NODE :
            if (testCDATASectionNode == null) {testCDATASectionNode = (DOM_CDATASection &)node; nodeCount++;}
            break;
        case DOM_Node::ENTITY_REFERENCE_NODE :
            if (testEntityReferenceNode == null) {testEntityReferenceNode = (DOM_EntityReference &)node; nodeCount++;}
            break;
        case DOM_Node::ENTITY_NODE :
            if (testEntityNode == null) {testEntityNode = (DOM_Entity &)node; nodeCount++;}
            break;
        case DOM_Node::PROCESSING_INSTRUCTION_NODE :
            if (testProcessingInstructionNode == null) {testProcessingInstructionNode = (DOM_ProcessingInstruction &)node; nodeCount++;}
            break;
        case DOM_Node::COMMENT_NODE :
            if (testCommentNode == null) {testCommentNode = (DOM_Comment &)node; nodeCount++;}
            break;
        case DOM_Node::DOCUMENT_TYPE_NODE :
            if (testDocumentTypeNode == null) {testDocumentTypeNode = (DOM_DocumentType &)node; nodeCount++;}
            break;
        case DOM_Node::DOCUMENT_FRAGMENT_NODE :
            if (testDocumentFragmentNode == null) {testDocumentFragmentNode = (DOM_DocumentFragment &)node; nodeCount++;}
            break;
        case DOM_Node::NOTATION_NODE :
            if (testNotationNode == null) {testNotationNode = (DOM_Notation &)node; nodeCount++;}
            break;
        case DOM_Node::DOCUMENT_NODE :
            if (testDocumentNode == null) {testDocumentNode = (DOM_Document &)node; nodeCount++;}
            break;
        default:
            ;
    }// End of switch

    }   // End of while
};


/**
 * @param document org.w3c.dom.DOM_Document
 */
 void DTest::findTestNodes(DOM_Node node) {
    DTest test;
    DOM_Node kid;
    // Walk the tree until you find and assign all node types needed that exist.
    
        
    if (node.getFirstChild() != null)
    {
        kid = node.getFirstChild();
        test.findTestNodes(kid);
    }
            
                
    if (node.getNextSibling() != null)
    {
        kid = node.getNextSibling();
        test.findTestNodes(kid);
    }

        
    switch (node.getNodeType())
    {
        case DOM_Node::ELEMENT_NODE :
            if (testElementNode == null) {testElementNode = (DOM_Element &)node; }
            break;
        case DOM_Node::ATTRIBUTE_NODE :
            if (testAttributeNode == null) {testAttributeNode = (DOM_Attr &)node; }
            break;
        case DOM_Node::TEXT_NODE :
            if (testTextNode == null) {testTextNode = (DOM_Text &)node; }
            break;
        case DOM_Node::CDATA_SECTION_NODE :
            if (testCDATASectionNode == null) {testCDATASectionNode = (DOM_CDATASection &)node; }
            break;
        case DOM_Node::ENTITY_REFERENCE_NODE :
            if (testEntityReferenceNode == null) {testEntityReferenceNode = (DOM_EntityReference &)node;}
            break;
        case DOM_Node::ENTITY_NODE :
            if (testEntityNode == null) {testEntityNode = (DOM_Entity &)node;}
            break;
        case DOM_Node::PROCESSING_INSTRUCTION_NODE :
            if (testProcessingInstructionNode == null) {testProcessingInstructionNode = (DOM_ProcessingInstruction &)node;}
            break;
        case DOM_Node::COMMENT_NODE :
            if (testCommentNode == null) {testCommentNode = (DOM_Comment &)node;}
            break;
        case DOM_Node::DOCUMENT_TYPE_NODE :
            if (testDocumentTypeNode == null) {testDocumentTypeNode = (DOM_DocumentType &)node; }
            break;
        case DOM_Node::DOCUMENT_FRAGMENT_NODE :
            if (testDocumentFragmentNode == null) {testDocumentFragmentNode = (DOM_DocumentFragment &)node;}
            break;
        case DOM_Node::NOTATION_NODE :
            if (testNotationNode == null) {testNotationNode = (DOM_Notation &)node;}
            break;
        case DOM_Node::DOCUMENT_NODE :
            if (testDocumentNode == null) {testDocumentNode = (DOM_Document &)node;}
            break;
        default:
            ;
    }// End of switch
};//End of class

/**
 * 
 * version 2.0 10/12/98
 *
 */
 void main(int argc, char **argv)
 {
     
     {
         //  Nest entire test in an inner block.
         //     Reference counting should recover all document
         //     storage when this block exits.
         
         DTest test;
         try {
             XMLPlatformUtils::Initialize();
         }
         catch (const XMLException& toCatch) {
             char *pMessage = XMLString::transcode(toCatch.getMessage());
             fprintf(stderr, "Error during initialization! \n  %s \n", pMessage);
             delete [] pMessage;
             return;
         } 
         
         long avgTime = 0;
         bool OK = true;
         long startTime = 0;//****************Time the whole thing for efficiency of DOM implementation
         
         // for (int i=0; i< 1000; i++)
         // {   
         // AH Revisit  //  startTime = System.currentTimeMillis();
         //     if(!OK)
         //     break;
         
         DOM_Document d = test.createDocument();
         // DOM_Document z = test.createDocument();
         
         DOM_DocumentType docDocType = test.createDocumentType(d,"testDocument1");
         d.appendChild(docDocType);
         
         DOM_Entity docEntity = test.createEntity( d, "ourEntityNode");
         DOM_Text entityChildText = d.createTextNode("entityChildText information"); // Build a branch for entityReference tests
         docEntity.appendChild(entityChildText);                      // & for READONLY_ERR tests
         docDocType.getEntities().setNamedItem(docEntity);
         
         test.docBuilder(d, "d");
         
         test.findTestNodes((DOM_Node)d);
         // test.docBuilder(z, "z");
         // try {
         LEAKTEST(test.testAttr(d););
         LEAKTEST(test.testCDATASection(d););
         LEAKTEST(test.testCharacterData(d););
         LEAKTEST(test.testChildNodeList(d););
         LEAKTEST(test.testComment(d););
         LEAKTEST(test.testDeepNodeList(d););
         LEAKTEST(test.testDocument(d););
         LEAKTEST(test.testDocumentFragment(d););
         LEAKTEST(test.testDocumentType(d);); 
         LEAKTEST(test.testDOMImplementation(d););
         LEAKTEST(test.testElement(d););
         LEAKTEST(test.testEntity(d););
         LEAKTEST(test.testEntityReference(d););
         LEAKTEST(test.testNode(d););
         LEAKTEST(test.testNotation(d););  
         LEAKTEST(test.testPI(d);); 
         LEAKTEST(test.testText(d););  
         LEAKTEST(test.testDOMerrors(d););
         
         //!! Throws WRONG_DOCUMENT_ERR **********
         
         // z.appendChild(d.createComment("Test doc d comment"));// Tries to append z document with document d comment
         // d.getDocumentElement().appendChild(z.createElement("newZdocElement"));// Tries to append d document with document z DOM_Element
         // d.getLastChild().getLastChild().insertBefore(z.createElement("newZdocElement"),d.getLastChild().getLastChild().getFirstChild());// Tries to insert into d document with document z DOM_Element
         // d.replaceChild(z.createElement("newZdocElement"),d.getLastChild().getLastChild().getFirstChild());  // Tries to replace in d document with document z DOM_Element
         
         /* Attribute newAttribute = d.createAttribute("newAttribute");
         d.getDocumentElement().setAttributeNode(newAttribute);
         d.getDocumentElement().getAttributes().setNamedItem(z.createAttribute("newzAttribute"));
         */
         
         //!! Throws INVALID_CHARACTER_ERR **********
         // ******This just gets us through each method. JKess has a comprehensive test of Invalid Names******
         // d.createAttribute("Invalid Name"); // Name with blank space
         // d.createElement("5InvalidName"); // Name starts with numeric
         // d.createProcessingInstruction("This is the target processor channel","InvalidName>");// Name ends with >
         // d.getDocumentElement().setAttribute("Invalid%Name",""); // Name contains %
         
         
         //!!   ******** NO_DATA_ALLOWED_ERR ********No cases to test as of 9/15
         
         
         //!!   ******** NO_MODIFICATION_ALLOWED_ERR ******** When read only exists
         /*
         
           
             
               // **** FOR DOM_Element when read only exists********
               .removeAttribute("aString");        // ***** Not until read only exists.
               .removeAttributeNode(Attribute);        // ***** Not until read only exists.
               .setAttribute("aString", "anotherString"); // ***** Not until read only exists.
               
                 
                   // **** FOR DOM_Node when read only exists********
                   .appendChild(aNode);         // ***** Not until read only exists.
                   .insertBefore(aNode, AnotherNode);   // ***** Not until read only exists.
                   .removeChild(aNode);         // ***** Not until read only exists.
                   .replaceChild(aNode);            // ***** Not until read only exists.
                   
                     .splitText(2); // ***** Not until read only exists.
                     
                       .setNamedItem(DOM_Node); // ***** Not until read only exists.
         */
         
         
         //!!******** NOT_SUPPORTED_ERR ********For HTML when implemented
         /*
         .createCDATASection("String stuff");
         .createEntityReference("String stuff");
         .createProcessingInstruction("String stuff", "Some more String stuff");
         */
         
         // } catch (DOM_DOMException &e) {
         //     fprintf(stderr, "Unexpected DOM Exception caught.  Code is: %d\n", e.code);
         //     OK = false;
         // }
         //    catch (...) {
         //        fprintf(stderr, "Unexpected non-DOM exception caught.");
         //    };
         
         
         //System.err.println("Elapsed time (measured in seconds): " +   ((System.currentTimeMillis() - startTime) / 1000.0));
         // AH revisit avgTime += System.currentTimeMillis() - startTime;
         // }//END OF FOR
         
         
         // System.err.println("Elapsed time (measured in seconds): " +
         //                    ((System.currentTimeMillis() - startTime) / 1000.0));
         //       System.err.println("Elapsed time (measured in mili-seconds): " +
         //                    ((System.currentTimeMillis() - startTime)));
         
         
         // System.err.println("Average Elapsed time (measured in seconds): " + (avgTime/10000000.0) );

         // Null out the static object references in class DTest,
         // which will recover their storage.  Part of testing for memory leaks.
         DTest::testElementNode = null;
         DTest::testAttributeNode = null;
         DTest::testTextNode = null;
         DTest::testCDATASectionNode = null;
         DTest::testEntityReferenceNode = null;
         DTest::testEntityNode = null;
         DTest::testProcessingInstructionNode = null;
         DTest::testCommentNode = null;
         DTest::testDocumentNode = null;
         DTest::testDocumentTypeNode = null;
         DTest::testDocumentFragmentNode = null;
         DTest::testNotationNode = null;
         
    };
    DomMemDebug().print();
};


/**
 * This method tests DOM_Attr methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testAttr(DOM_Document document)
{
    DOM_Node node;  
    DOM_Attr attributeNode, attribute2;
    DOMString compare;
    bool T = true;
    bool F = false;
    bool OK = true;
// For debugging*****   printf("\n          testAttr's outputs:\n\n");

    DOM_Attr testAttribute = document.createAttribute("testAttribute");
    testAttribute.setValue("testAttribute's value");
    node = document.getDocumentElement(); // node gets first element

    // ((DOM_Element &)node).setAttributeNode(testAttribute);
    // attributeNode = ((DOM_Element &)node).getAttributeNode("testAttribute");
    DOM_Element el = (DOM_Element &)node;
    el.setAttributeNode(testAttribute); 
    attributeNode = el.getAttributeNode("testAttribute");
    
    compare = "testAttribute";
    if (!compare.equals(attributeNode.getName()))
    {
        printf("Warning!!! DOM_Attr's 'getName' method failed to work properly!\n");
        OK = false;
    }
    compare = "testAttribute's value";
    if (!compare.equals(attributeNode.getNodeValue()))
    {
        printf("Warning!!! DOM_Attr's 'getNodeValue' method failed to work properly!\n");
        OK = false;
    }
    if (! T ==attributeNode.getSpecified())
    {
        printf("Warning!!! DOM_Attr's 'getSpecified' method failed to work properly!\n");
        OK = false;
    }
    
    if (!compare.equals(attributeNode.getValue()))
    {
        printf("Warning!!! DOM_Attr's 'getValue' method failed to work properly!\n");
        OK = false;
    }


    attributeNode.setNodeValue("Reset Value");   /// LEAK!!!!!
    compare = "Reset Value";
    if (!compare.equals(attributeNode.getNodeValue()))
    {
        printf("Warning!!! DOM_Attr's 'setNodeValue' method failed to work properly!\n");
        OK = false;
    }

    attributeNode.setValue(null);
    if (! attributeNode.getValue().equals(""))
    {
        printf("Warning!!! DOM_Attr's 'setValue' to 'null' method failed to work properly!\n");
        OK = false;
    }
    
    attributeNode.setValue("Another value ");
    compare = "Another value ";
    if (!compare.equals(attributeNode.getValue()))
    {
        printf("Warning!!! DOM_Attr's 'setValue' method failed to work properly!");
        OK = false;
    }

    node = attributeNode.cloneNode(T);
                                      
    // Check nodes for equality, both their name and value or lack thereof
    bool cloneOK = true;
    if (!(node.getNodeName().equals(attributeNode.getNodeName())))
        cloneOK = false;
    if (node.getNodeValue() == null &&
        attributeNode.getNodeValue() != null)
    {
        attributeNode.getNodeValue().print();
        cloneOK = false;
    }

    if (node.getNodeValue() != null && attributeNode.getNodeValue() == null)
    {
        cloneOK = false;
        node.getNodeValue().print();
    };

    if (node.getNodeValue() != null && attributeNode.getNodeValue() != null)
    {
        if (!(node.getNodeValue().equals(attributeNode.getNodeValue())))
            cloneOK = false;
    }


/*
    if (! (node.getNodeName().equals(attributeNode.getNodeName()) &&         // Compares node names for equality
          (node.getNodeValue() != null && attributeNode.getNodeValue() != null)  // Checks to make sure each node has a value node
        ?  node.getNodeValue().equals(attributeNode.getNodeValue())          // If both have value nodes test those value nodes for equality
        : (node.getNodeValue() == null && attributeNode.getNodeValue() == null)))// If one node doesn't have a value node make sure both don't
*/
    if (cloneOK == false)
        {   
            printf("'cloneNode' did not clone the Attribute node correctly\n");
            OK = false;
        }
        // Deep clone test comparison is in testNode & testDocument

//************************************************* ERROR TESTS
    DTest tests;
//!! Throws HIERARCHY_REQUEST_ERR ****************  
    //  doc.getDocumentElement().appendChild(attributeNode);

//!! Throws a NOT_FOUND_ERR ********
    //  attribute2 = doc.createAttribute("testAttribute2");
    //  doc.getDocumentElement().removeAttributeNode(attribute2);

//!! Throws an INUSE_ATTRIBUTE_ERR ******
    //  DOM_Element element = (DOM_Element &)doc.getLastChild().getLastChild();
    //  element.setAttributeNode(testAttribute );// Tests setNamedItem which generates error through justSetNamedItem.
    
// For debugging*****       printf("All DOM_Attr method calls worked correctly.\n");
    if (! OK)
        printf("\n*****The DOM_Attr method calls listed above failed, all others worked correctly.*****\n");
//  printf("");

};




/**
 * This method tests DOM_CDATASection methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testCDATASection(DOM_Document document)
{
    DOM_Node node, node2;
    bool T = true;
    bool OK = true;
// For debugging*****   printf("\n          testCDATASection's outputs:\n");
    node = document.getDocumentElement().getElementsByTagName("dBodyLevel23").item(0).getFirstChild(); // node gets DOM_CDATASection node

    node2 = node.cloneNode(T);//*****?
    // Check nodes for equality, both their name and value or lack thereof
    if (! (node.getNodeName().equals(node2.getNodeName()) &&        // Compares node names for equality
          (node.getNodeValue() != null && node2.getNodeValue() != null)     // Checks to make sure each node has a value node
        ?  node.getNodeValue().equals(node2.getNodeValue())         // If both have value nodes test those value nodes for equality
        : (node.getNodeValue() == null && node2.getNodeValue() == null)))   // If one node doesn't have a value node make sure both don't
    {
        printf("'cloneNode' did not clone the DOM_CDATASection node correctly\n");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument
    
// For debugging*****   printf("All DOM_CDATASection method calls worked correctly.\n");
        
    if (! OK)
        printf("\n*****The DOM_CDATASection method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
};



/**
 * This method tests DOM_CharacterData methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testCharacterData(DOM_Document document)
{
    DOM_CharacterData charData;
    DOMString compareData, newData, resetData;
    bool OK = true;
// For debugging*****   printf("\n          testCharacterData's outputs:\n");
    DOM_Node abc1 = document.getDocumentElement().getElementsByTagName("dBodyLevel31").item(0).getFirstChild(); // charData gets textNode11
    charData = (DOM_CharacterData &) abc1;
    compareData = "dBodyLevel31'sChildTextNode11";
    if (!compareData.equals(charData.getData()))
    {
        printf("Warning!!! DOM_CharacterData's 'getData' failed to work properly!\n This may corrupt other DOM_CharacterData tests!!!*****\n");
        OK = false;
    }   
    
    resetData = charData.getData();
    //  printf("This node's original data is: " + charData.getData());

    newData = " This is new data for this node";
    compareData = charData.getData() + newData;
    charData.appendData(newData);
    if (!compareData.equals(charData.getData()))
    {
        printf("Warning!!! DOM_CharacterData's 'appendData' failed to work properly!\n");
        OK = false;
    }
    //  printf("This node's appended data is: " + charData.getData());

    compareData = "dBodyLevel";
    charData.deleteData(10, 100);
    if (!compareData.equals(charData.getData()))
    {
        printf("Warning!!! DOM_CharacterData's 'deleteData' failed to work properly!\n");
        OK = false;
    }
    //  printf("This node's partially deleted data is: " + charData.getData());

    unsigned int length = 10;
    if (!(length == charData.getLength()))
    {
        printf("Warning!!! DOM_CharacterData's 'getLength' failed to work properly!\n");
        OK = false;
    }
    //  printf("This node's data length is: " + charData.getLength());

    compareData = "dBody' This is data inserted into this node'Level";
    charData.insertData(5, "' This is data inserted into this node'");
    if (!compareData.equals(charData.getData()))
    {
        printf("Warning!!! DOM_CharacterData's 'insertData' failed to work properly!\n");
        OK = false;
    }
    //  printf("This node's updated with insert data is: " + charData.getData());

    compareData = "dBody' This is ' replacement data'ted into this node'Level";
    charData.replaceData(15, 10, "' replacement data'");
    if (!compareData.equals(charData.getData()))
    {
        printf("Warning!!! DOM_CharacterData's 'replaceData' failed to work properly!\n");
        OK = false;
    }
    //  printf("This node's updated with replacement data is: " +charData.getData());

    compareData = "New data A123456789B123456789C123456789D123456789E123456789";
    charData.setData("New data A123456789B123456789C123456789D123456789E123456789");
    if (!compareData.equals(charData.getData()))
    {
        printf("Warning!!! DOM_CharacterData's 'setData' failed to work properly!");
        OK = false;
    }
    //  printf("This node's new data via setData: " + charData.getData());

    compareData = "123456789D123456789E123456789";
    if (!compareData.equals(charData.substringData(30, 30)))
    {
        printf("Warning!!! DOM_CharacterData's 'substringData' failed to work properly!\n");
        OK = false;
    }
    //  printf("Using subString 30,30 you get:");  charData.substringData(30,30)).print();

    compareData = "New data A123456789B12345";
    if (!compareData.equals(charData.substringData(0, 25)))
    {
        printf("Warning!!! DOM_CharacterData's 'substringData' failed to work properly!\n");
        OK = false;
    }
    //  printf("Using subString 0,25 you get: ");   charData.substringData(0,25)).print();

//************************************************* ERROR TESTS
    DTest tests;   // What is this for?  'tests' is never used.

//!! Throws INDEX_SIZE_ERR ********************
    EXCEPTIONSTEST(charData.deleteData(-1, 5), DOM_DOMException::INDEX_SIZE_ERR, OK, 101 );
    // Test 102 is not an error because the -1 parameter is an unsigned value, and counts
    //   that exceed the length of the string are allowed.
//    EXCEPTIONSTEST(charData.deleteData(2, -1), DOM_DOMException::INDEX_SIZE_ERR, OK, 102 );
    EXCEPTIONSTEST(charData.deleteData(100, 5), DOM_DOMException::INDEX_SIZE_ERR, OK,103 );

//can't set negative unsigned int in c++ compiler
    
  //  EXCEPTIONSTEST(charData.insertData(-1, "Stuff inserted"), DOM_DOMException::INDEX_SIZE_ERR, OK, 104 );
    EXCEPTIONSTEST(charData.insertData(100,"Stuff inserted"), DOM_DOMException::INDEX_SIZE_ERR, OK, 105 );
    
  //  EXCEPTIONSTEST(charData.replaceData(-1, 5, "Replacement stuff") , DOM_DOMException::INDEX_SIZE_ERR, OK, 106 );
    EXCEPTIONSTEST(charData.replaceData(100, 5 ,"Replacement stuff"), DOM_DOMException::INDEX_SIZE_ERR, OK, 107 );
  //  EXCEPTIONSTEST(charData.replaceData(2, -1, "Replacement stuff"), DOM_DOMException::INDEX_SIZE_ERR,  OK, 108 );
    
    EXCEPTIONSTEST(charData.substringData(-1, 5), DOM_DOMException::INDEX_SIZE_ERR, OK, 109 );
    EXCEPTIONSTEST(charData.substringData(100, 5), DOM_DOMException::INDEX_SIZE_ERR, OK, 110 );
 //   EXCEPTIONSTEST(charData.substringData(2, -1), DOM_DOMException::INDEX_SIZE_ERR, OK, 111 );
    

//!! Throws NO_MODIFICATION_ALLOWED_ERR ******** 
// Since user cannot add children to Entity and EntityReference Nodes this test can't be done.
#if 0

    DOM_Node abc8 = document.getDocumentElement().getElementsByTagName("dBodyLevel24").
        item(0).getFirstChild().getChildNodes().item(0); // node gets ourEntityReference node's child text
    DOM_Text node = (DOM_Text &) abc8;

    EXCEPTIONSTEST(node.appendData("new data"), DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 112 );
    EXCEPTIONSTEST(node.deleteData(5, 10), DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 113 );
    EXCEPTIONSTEST(node.insertData(5, "Stuff inserted"), DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 114 );
    EXCEPTIONSTEST(node.replaceData(5, 10, "Replacementstuff"), DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 115 );
    EXCEPTIONSTEST(node.setData("New setdata stuff"), DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 116 );

#endif
        
// For debugging*****       printf("All DOM_CharacterData method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The DOM_CharacterData method calls listed above failed, all others worked correctly.*****\n");
    charData.setData(resetData); // reset node to original data
//  printf(""\n);
};




/** 
 * This method tests ChildNodeList methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testChildNodeList(DOM_Document document)
{
    DOM_Node node, node2;
    bool OK = true;
// For debugging*****   printf("\n          testChildNodeList's outputs:\n");
    node = document.getDocumentElement().getLastChild(); // node gets doc's testBody element
    
    if (!(node.getChildNodes().getLength()== 4))
        OK = false;
    node2 = node.getChildNodes().item(2);
    if (! node2.getNodeName().equals("dBodyLevel23"))
        OK = false;
    
// For debugging*****       printf("All ChildNodeList method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The ChildNodeList method calls listed above failed, all others worked correctly.*****\n");       
//  printf("");
};



/**
 * This method tests DOM_Comment methods for the XML DOM implementation
 * version 1.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testComment(DOM_Document document)
{
    DOM_Node node, node2;
    bool T = true;
    bool OK = true;
// For debugging*****   printf("\n          testComment's outputs:\n");
    node = document.getDocumentElement().getElementsByTagName("dBodyLevel31").item(0).getFirstChild(); // node gets textNode11
    node2 = node.cloneNode(T);
    // Check nodes for equality, both their name and value or lack thereof
    if (!(node.getNodeName().equals(node2.getNodeName()) &&         // Compares node names for equality
          (node.getNodeValue() != null && node2.getNodeValue() != null)     // Checks to make sure each node has a value node
        ?  node.getNodeValue().equals(node2.getNodeValue())         // If both have value nodes test those value nodes for equality
        : (node.getNodeValue() == null && node2.getNodeValue() == null)))   // If one node doesn't have a value node make sure both don't
        //printf("'cloneNode' did not clone the DOM_Comment node correctly\n");
        OK = false;
    // Deep clone test comparison is in testNode & testDocument
    if (OK)
// For debugging*****       printf("All DOM_Comment method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The DOM_Comment method calls listed above failed, all others worked correctly.*****\n");
//  printf("\n");
};



/**
 * This method tests DeepNodeList methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testDeepNodeList(DOM_Document document)
{
    DOM_Node node, node2;
    bool OK = true;
// For debugging*****   printf("\n          testDeepNodeList's outputs:\n\n");
    node = document.getLastChild().getLastChild(); // node gets docBody element
//  DOM_Element el = (DOM_Element &)node;
//  DOM_NodeList nl = el.getElementsByTagName("*");
//  int len = nl.getLength();
//  if (len != 8)
  if (!(8 == ((DOM_Element &) node).getElementsByTagName("*").getLength()))
        {
            printf ("Warning!!! DeepNodeList's 'getLength' failed to work properly!\n");
            OK = false;     
        }
    node2 = ((DOM_Element &) node).getElementsByTagName("*").item(2); //This also runs through 'nextMatchingElementAfter"
    if (! node2.getNodeName().equals("dBodyLevel32"))
        {
            printf ("Warning!!! DeepNodeList's 'item' (or DOM_Element's 'getElementsBy TagName)failed to work properly!\n");
            OK = false;     
        }
    node2 = document.getLastChild();
    if (! ((DOM_Element &) node2).getElementsByTagName("dTestBody").item(0).getNodeName().equals("dTestBody"))//This also runs through 'nextMatchingElementAfter"
        {
            printf ("Warning!!! DeepNodeList's 'item' (or DOM_Element's 'getElementsBy TagName)failed to work properly!\n");
            OK = false;     
        }
        
    
// For debugging*****       printf("All DeepNodeList method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The DeepNodeList method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
};



/**
 * This method tests DOM_Document methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 *
 **** ALL DOM_Document create methods are run in docBuilder except createAttribute which is in testAttribute**
 */
void DTest::testDocument(DOM_Document document)
{
    DTest make;
    DOM_DocumentFragment docFragment, docFragment2;
    DOM_Element newElement;
    DOM_Node node, node2;
    DOMString elementNames[] =  {"dFirstElement", "dTestBody", "dBodyLevel21","dBodyLevel31","dBodyLevel32",
                   "dBodyLevel22","dBodyLevel33","dBodyLevel34","dBodyLevel23","dBodyLevel24"};
    DOMString newElementNames[] = {"dFirstElement", "dTestBody", "dBodyLevel22","dBodyLevel33","dBodyLevel34","dBodyLevel23"};
    bool result;
    bool OK = true;
// For debugging*****   printf("\n          testDocument's outputs:\n \n");
    
    DOM_DocumentType checkDocType =  make.createDocumentType(document,"testDocument1");
    DOM_DocumentType docType = document.getDoctype();

    if (! checkDocType.getNodeName().equals(docType.getNodeName() ))
    {
        printf("Warning!!! DOM_Document's 'getDocType method failed!\n" );
        OK = false;
    }

    if (!checkDocType.getNodeValue().equals(docType.getNodeValue()))
    {
        printf("Warning!!! DOM_Document's 'getDocType method failed!\n" );
        OK = false;
    }

    /*
    if (! (checkDocType.getNodeName().equals(docType.getNodeName()) &&      // Compares node names for equality
          (checkDocType.getNodeValue() != null && docType.getNodeValue() != null)   // Checks to make sure each node has a value node
        ?  checkDocType.getNodeValue().equals(docType.getNodeValue())       // If both have value nodes test those value nodes for equality
        : (checkDocType.getNodeValue() == null && docType.getNodeValue() == null))) // If one node doesn't have a value node make sure both don't
    {
        printf("Warning!!! DOM_Document's 'getDocType method failed!\n" );
        OK = false;
    }
    */
        
    DOM_Node rootElement = document.getLastChild();
    if (! (rootElement.getNodeName().equals(document.getDocumentElement().getNodeName()) &&         // Compares node names for equality
          (rootElement.getNodeValue() != null && document.getDocumentElement().getNodeValue() != null)   // Checks to make sure each node has a value node
        ?  rootElement.getNodeValue().equals(document.getDocumentElement().getNodeValue())      // If both have value nodes test those value nodes for equality
        : (rootElement.getNodeValue() == null && document.getDocumentElement().getNodeValue() == null)))    // If one node doesn't have a value node make sure both don't
    {
        printf("Warning!!! DOM_Document's 'getDocumentElement' method failed!\n" );
        OK = false;
    }
    
    DOM_NodeList docElements = document.getElementsByTagName("*");
    int docSize = docElements.getLength();
    int i;
    for (i = 0; i < docSize; i++)
    {
        DOM_Node n = (DOM_Node) docElements.item(i);
        if (! (elementNames[i].equals(n.getNodeName())))
        {
            printf("Comparison of this document's elements failed at element number %d : ", i); 
            n.getNodeName().print();
            OK = false;
            break;
        }
    }

    // What is this supposed to be doing?
    //
    //if (document.equals(document.getImplementation()))
    //{
    //  printf("Warning!!! DOM_Document's 'getImplementation' method failed!\n" );
    //  OK = false;     
    //}
        
    newElement = document.createElement("NewElementTestsInsertBefore");
    //  doc.insertBefore(newElement,null);//!! Throws a HIERARCHY_REQUEST_ERR   ******* 
    //  doc.removeChild(docElements.item(9));//!! Throws a NOT_FOUND_ERR  ********

    docFragment = document.createDocumentFragment();
    //Tests removeChild and stores removed branch for tree reconstruction
    docFragment.appendChild(docElements.item(1).removeChild(docElements.item(9)));
    docFragment2 = document.createDocumentFragment();
    //Tests removeChild and stores removed branch for tree reconstruction
    docFragment2.appendChild(docElements.item(1).removeChild(docElements.item(2)));
    docSize = docElements.getLength();
    for (i = 0; i < docSize; i++)
    {
        DOM_Node n = (DOM_Node) docElements.item(i);
        if (! (newElementNames[i].equals(n.getNodeName())))
        {
            printf("Comparison of new document's elements failed at element number %d : ", i);
            n.getNodeName().print();
            OK = false;
            break;
        }
    }
    docElements.item(1).insertBefore(docFragment, DOM_Node()); //Reattaches removed branch to restore tree to the original
                                                // AH Revist.  Note: insertBefore should be able to take 
                                                //   a null as its second parameter.
    docElements.item(1).insertBefore(docFragment2, docElements.item(2)); //Reattaches removed branch to restore tree to the original

    //  printf(docElements.item(2).getNodeName());

    docSize = docElements.getLength();
    for (i = 0; i < docSize; i++)
    {
        DOM_Node n = (DOM_Node) docElements.item(i);
        if (! (elementNames[i].equals(n.getNodeName())))
        {
            printf("Comparison of restored document's elements failed at element number %d : ", i);
            n.getNodeName().print();
            OK = false;
            break;
        }
    }

    DTest tests;

    
//  DOM_Document z = tests.createDocument();
//  tests.docBuilder(z, "z");

//!! Throws WRONG_DOCUMENT_ERR **********
//  EXCEPTIONSTEST(z.appendChild(       
    //  z.appendChild(d.createComment("Test doc d comment"));// Tries to append z document with document d comment
    //  d.getDocumentElement().appendChild(z.createElement("newZdocElement"));// Tries to append d document with document z DOM_Element
    //  d.getLastChild().getLastChild().insertBefore(z.createElement("newZdocElement"),d.getLastChild().getLastChild().getFirstChild());// Tries to insert into d document with document z DOM_Element
    //  d.replaceChild(z.createElement("newZdocElement"),d.getLastChild().getLastChild().getFirstChild());  // Tries to replace in d document with document z DOM_Element

    //  doc.setNodeValue("This shouldn't work");//!! Throws a NO_MODIFICATION_ALLOWED_ERR ********
    
    node = document;
    node2 = document.cloneNode(true);
    result = treeCompare(node, node2); // Deep clone test comparison of document cloneNode
    if (!result)
    {
        printf("Warning!!! Deep clone of the document failed!\n");
        OK = false;
    }

    // Deep clone test comparison is also in testNode
    
    
// For debugging*****       printf("All DOM_Document method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The DOM_Document method calls listed above failed, all others worked correctly.*****\n");
//  printf("\n");
};



/**
 * This method tests DOM_DocumentFragment methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 *
 *
 ********This really isn't needed, only exists to throw NO_MODIFICATION_ALLOWED_ERR ********
 */
void DTest::testDocumentFragment(DOM_Document document)
{
    bool OK = true;
// For debugging*****   printf("\n          testDocumentFragment's outputs:\n");
    DOM_DocumentFragment testDocFragment = document.createDocumentFragment();
        
    //  testDocFragment.setNodeValue("This is a document fragment!");//!! Throws a NO_MODIFICATION_ALLOWED_ERR ********
    
// For debugging*****       printf("All DOM_DocumentFragment method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The DOM_DocumentFragment method calls listed above failed, all others worked correctly.*****\n");
//  printf("\n");
};



/**
 * This method tests DOM_DocumentType methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testDocumentType(DOM_Document document)
{
    DTest test;
    DOM_DocumentType docType, holdDocType;
    DOM_NamedNodeMap docEntityMap, docNotationMap;
    DOM_Node node, node2;
    DOMString compare;
    bool OK = true;
// For debugging*****   printf("\n          testDocumentType's outputs:\n");
    DOM_DocumentType newDocumentType =  test.createDocumentType(document, "TestDocument");
    node = document.getFirstChild(); // node gets doc's docType node
    node2 = node.cloneNode(true);
    // Check nodes for equality, both their name and value or lack thereof
    if (! (node.getNodeName().equals(node2.getNodeName()) &&         // Compares node names for equality
          (node.getNodeValue() != null && node2.getNodeValue() != null)  // Checks to make sure each node has a value node
        ?  node.getNodeValue().equals(node2.getNodeValue())          // If both have value nodes test those value nodes for equality
        : (node.getNodeValue() == null && node2.getNodeValue() == null)))// If one node doesn't have a value node make sure both don't
    {   
        printf("'cloneNode' did not clone the DOM_DocumentType node correctly\n");
        OK = false;
    }
     // Deep clone test comparison is in testNode & testDocument

    DOM_Node  abc9 = document.getFirstChild();
    docType = (DOM_DocumentType &) abc9;
    compare = "ourEntityNode";
    docEntityMap = docType.getEntities();
    if (! compare.equals(docEntityMap.item(0).getNodeName()))
    {
        printf("Warning!!! DOM_DocumentType's 'getEntities' failed!\n" );
        OK = false;
    }
    docNotationMap = docType.getNotations();
    compare = "ourNotationNode";
    if (! compare.equals(docNotationMap.item(0).getNodeName()))
    {
        printf("Warning!!! DOM_DocumentType's 'getNotations' failed!\n");
        OK = false;
    }
    //  doc.appendChild(newDocumentTypeImpl);//!! Throws a HIERARCHY_REQUEST_ERR    ******* 
    DOM_Node abc10 = document.removeChild(document.getFirstChild()); //Tests removeChild and stores removed branch for tree reconstruction
    holdDocType = (DOM_DocumentType &) abc10;
    document.insertBefore(newDocumentType, document.getDocumentElement());
    //** Other aspects of insertBefore are tested in docBuilder through appendChild*

    document.removeChild(document.getFirstChild()); //Removes newDocumentType for tree restoral
    document.insertBefore(holdDocType, document.getFirstChild()); //Reattaches removed branch to restore tree to the original

    
// For debugging*****       printf("All DOM_DocumentType method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The DOM_DocumentType method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
};



/**
 * @param document org.w3c.dom.DOM_Document
 */
void DTest::testDOMerrors(DOM_Document document) {
    bool OK = true;

    DTest tests;

    EXCEPTIONSTEST(document.appendChild(testElementNode), DOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 201 ); 
    EXCEPTIONSTEST(testTextNode.appendChild(testTextNode), DOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 202 ); 
#ifdef skipthese
    EXCEPTIONSTEST(document.insertBefore(document.getElementsByTagName("docEntity").item(0),
        document.getElementsByTagName("docFirstElement").item(0)), DOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 203 ); 
    EXCEPTIONSTEST(document.replaceChild(document.getElementsByTagName("docCDATASection").item(0),
        document.getElementsByTagName("docFirstElement").item(0)), DOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 204 ); 

    EXCEPTIONSTEST(document.getElementsByTagName("docFirstElement").item(0).setNodeValue("This shouldn't work!" ),
            DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 205 );   
    EXCEPTIONSTEST(docReferenceEntity.setNodeValue("This shouldn't work!"),
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 206 );
    EXCEPTIONSTEST(docEntity.setNodeValue("This shouldn't work!"), 
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 207 );
    EXCEPTIONSTEST(document.setNodeValue("This shouldn't work!"),
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 208 );
    EXCEPTIONSTEST(docDocType.setNodeValue("This shouldn't work!"),
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 209 );
    EXCEPTIONSTEST(docDocFragment.setNodeValue("This shouldn't work!"),
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 210 );
    EXCEPTIONSTEST(docNotation.setNodeValue("This shouldn't work!"),
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 211 );
    EXCEPTIONSTEST(docReferenceEntity.appendChild(entityReferenceText2)
        , DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 212 );


    EXCEPTIONSTEST(docBodyLevel32.insertBefore(docTextNode11,docBody), DOM_DOMException::NOT_FOUND_ERR, OK, 213 );
    EXCEPTIONSTEST(docBodyLevel32.removeChild(docFirstElement), DOM_DOMException::NOT_FOUND_ERR, OK, 214 );
    EXCEPTIONSTEST(docBodyLevel32.replaceChild(docTextNode11,docFirstElement), DOM_DOMException::NOT_FOUND_ERR );

#endif

//!! Throws a NOT_FOUND_ERR ********
     
     // docBodyLevel32.getAttributes().removeNamedItem(testAttribute.getName());    16  // To test removeNamedItem
     
};



/**
 * This method tests DOM_DOMImplementation methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testDOMImplementation(DOM_Document document)
{
    
    DOM_DOMImplementation implementation;
    bool result = false;
    bool OK = true;
// For debugging*****   printf("\n          testDOMImplementation's outputs:\n");
    implementation = document.getImplementation(); //Uses getDOMImplementation to obtain implementation 

    result = implementation.hasFeature("XML", "1.0");
    if(!result)
    {
        fprintf(stderr, "Warning!!! DOM_DOMImplementation's 'hasFeature' that should be 'true' failed!");
        OK = false;
    }
    
    result = implementation.hasFeature("HTML", "4.0");
    if(result)
    {
        fprintf(stderr, "Warning!!! DOM_DOMImplementation's 'hasFeature' that should be 'false' failed!");
        OK = false;
    }
    
    
// For debugging*****       printf("All DOM_DOMImplementation method calls worked correctly.\n");
    if (!OK)
        fprintf(stderr, "\n*****The DOM_DOMImplementation method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
};



/**
 * This method tests DOM_Element methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testElement(DOM_Document document)
{
    DOM_Attr attributeNode, newAttributeNode;
    DOM_Element element, element2;
    DOM_Node node, node2;
    DOMString attribute, compare;
    DOMString attributeCompare[] = {"AnotherFirstElementAttribute", "dFirstElement", "testAttribute"};
    DOMString elementNames[] =  {"dFirstElement", "dTestBody", "dBodyLevel21","dBodyLevel31","dBodyLevel32",
                   "dBodyLevel22","dBodyLevel33","dBodyLevel34","dBodyLevel23","dBodyLevel24"};
    DOMString textCompare[] = {"dBodyLevel31'sChildTextNode11",
                                "dBodyLevel31'sChildTextNode12",
                                "dBodyLevel31'sChildTextNode13"};
    DOM_NamedNodeMap nodeMap;
    bool OK = true;
// For debugging*****   printf("\n          testElement's outputs:\n");
    node = document.getDocumentElement(); // node gets doc's firstElement
    node2 = node.cloneNode(true);
    // Check nodes for equality, both their name and value or lack thereof
    if (!(node.getNodeName().equals(node2.getNodeName()) &&         // Compares node names for equality
         (node.getNodeValue() != null && node2.getNodeValue() != null)  // Checks to make sure each node has a value node
        ? node.getNodeValue().equals(node2.getNodeValue())          // If both have value nodes test those value nodes for equality
        :(node.getNodeValue() == null && node2.getNodeValue() == null)))// If one node doesn't have a value node make sure both don't
    {   
        fprintf(stderr, "'cloneNode' did not clone the DOM_Element node correctly.\n");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument

    element = document.getDocumentElement(); // element gets doc's firstElement
    compare = "";
    attribute = element.getAttribute(document.getNodeValue() + "'s test attribute");
    if (! compare.equals(element.getAttribute(document.getNodeValue() + "'s test attribute")))
    {
        fprintf(stderr, "Warning!!! DOM_Element's 'getAttribute' failed!\n");
        OK = false;
    }
    
    attributeNode = element.getAttributeNode(document.getNodeValue() + "FirstElement");
    if(! (attributeNode == null))
    {
        fprintf(stderr, "Warning!!! DOM_Element's 'getAttributeNode' failed! It should have returned 'null' here!\n");
        OK = false;
    }
    

    newAttributeNode = document.createAttribute("AnotherFirstElementAttribute");
    newAttributeNode.setValue("A new attribute which helps test calls in DOM_Element");
    // This test is incorrect.  It assumes that there is a defined ordering of the entries
    //  in a nodeMap, but there is no ordering required.
#ifdef TheFollowingCheckIsInvalid
    element.setAttributeNode(newAttributeNode);
    nodeMap = element.getAttributes();
    int size = nodeMap.getLength();
    int k;
    for (k = 0; k < size; k++)
    {
        DOM_Node n = (DOM_Node) nodeMap.item(k);
        if (! (attributeCompare[k].equals(n.getNodeName())))
        {
            printf("Warning!!! Comparison of firstElement's attributes failed.\n");
            printf("   n.getNodeName() = \""); n.getNodeValue().print(); printf("\"\n");
            printf("   expected value  = \""); attributeCompare[k].print(); printf("\"\n");
            printf("   This failure can be a result of DOM_Element's 'setValue' and/or 'setAttributeNode' and/or 'getAttributes' failing.\n");
            OK = false;
            break;
        }
    //  printf("firstElement's attribute number " + k + " : " + n.getNodeName());
    }
#endif

    nodeMap = element.getAttributes();
    int size = nodeMap.getLength();
    if (size != 2)
    {
        printf("DOM_Element Tests Failure 001\n");
        OK = false;
    };
    element.setAttributeNode(newAttributeNode);
    size = nodeMap.getLength();
    if (size != 3)
    {
        printf("DOM_Element Tests Failure 002\n");
        OK = false;
    };

    // Fetch the newly added attribute node back out of from the named node map,
    //  and check that we are returned the same node that we put in.
    DOM_Node abc12 = nodeMap.getNamedItem("AnotherFirstElementAttribute");
    DOM_Attr fetchedAttr = (DOM_Attr &) abc12;
    if (fetchedAttr != newAttributeNode)
    {       
        printf("DOM_Element Tests Failure 003\n");
        OK = false;
    };

    // Fetch the newly added attribute back out directly from the element itself.
    fetchedAttr = element.getAttributeNode("AnotherFirstElementAttribute");
    if (fetchedAttr != newAttributeNode)
    {       
        printf("DOM_Element Tests Failure 004\n");
        OK = false;
    };



    DOM_NodeList docElements = document.getElementsByTagName("*");
    int docSize = docElements.getLength();
    int i;
    for (i = 0; i < docSize; i++)
    {
        DOM_Node n = docElements.item(i);
        if (! (elementNames[i].equals(n.getNodeName())))
        {
            printf("Warning!!! Comparison of DOM_Element's 'getElementsByTagName' "
                            "and/or 'item' failed at element number %d : ", i );
            n.getNodeName().print();
            printf("\n");
            OK = false;
            break;
        }       
    //  printf("docElement's number " + i + " is: " + n.getNodeName());
    }
    DOM_Node abc15 = document.getElementsByTagName("dBodyLevel21").item(0); // element gets DOM_Element test BodyLevel21 
    element = (DOM_Element &) abc15;

    DOM_Node abc16 = document.getElementsByTagName("dBodyLevel31").item(0); // element2 gets DOM_Element test BodyLevel31 
    element2 = (DOM_Element &) abc16;
    DOM_NodeList text = ((DOM_Node &) element2).getChildNodes();
    int textSize = text.getLength();
    int j;
    static bool firstTime = true;
    if (firstTime)
    {
        firstTime = false;      // Temporary fix.  Subsequent tests alter the doc, causing
                                //   this test to fail on all but the first time through.
        for (j = 0; j < textSize; j++)
        {
            DOM_Node n = text.item(j);
            if (! (textCompare[j].equals(n.getNodeValue())))
            {
                printf("Warning!!! Comparison of original text nodes via DOM_Node 'getChildNodes' & DOM_NodeList 'item'\n"
                    "     failed at text node: #%d \n     ", j );
                n.getNodeValue().print();
                printf("\n");
                OK = false;
                break;
            }
            //  printf("DOM_Element testBodyLevel31's child text node " + j + " is: " + n.getNodeValue());
        }
    }

    element = document.getDocumentElement(); // element gets doc's firstElement
    element.normalize();        // Concatenates all adjacent text nodes in this element's subtree
    DOM_NodeList text2 = ((DOM_Node) element2).getChildNodes();
    compare = "dBodyLevel31'sChildTextNode11dBodyLevel31'sChildTextNode12dBodyLevel31'sChildTextNode13";
    DOM_Node n = text2.item(0);
    if (! (compare.equals(n.getNodeValue())))
    {
        printf("Warning!!! Comparison of concatenated text nodes created by DOM_Element's 'normalize' failed!\n");
        OK = false;
    }
    
    element.setAttribute("FirstElementLastAttribute", "More attribute stuff for firstElement!!");
    element.removeAttribute("FirstElementLastAttribute");
    element.removeAttributeNode(newAttributeNode);

    //  doc.getLastChild().setNodeValue("This shouldn't work");//!! Throws a NO_MODIFICATION_ALLOWED_ERR***
    
// For debugging*****       printf("All DOM_Element method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The DOM_Element method calls listed above failed, all others worked correctly.*****\n");
//  printf("");

};



/**
 * This method tests DOM_Entity methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testEntity(DOM_Document document)
{
    DOM_Entity entity;
    DOM_Node node, node2;
    bool OK = true;
    DOMString compare;
// For debugging*****   printf("\n          testEntity's outputs:\n\n");
    DOM_Node abc20 = document.getDoctype().getEntities().getNamedItem("ourEntityNode");
    entity = (DOM_Entity &) abc20;
    node = entity;
    node2 = entity.cloneNode(true);
    // Check nodes for equality, both their name and value or lack thereof
    if (!(node.getNodeName().equals(node2.getNodeName()) &&         // Compares node names for equality
            ((node.getNodeValue() != null && node2.getNodeValue() != null) ?    // Checks to make sure each node has a value node
               node.getNodeValue().equals(node2.getNodeValue()) :       // If both have value nodes test those value nodes for equality
               (node.getNodeValue() == null && node2.getNodeValue() == null)))) // If one node doesn't have a value node make sure both don't
    {   
        printf("Warning!!! 'cloneNode' did not clone the DOM_Entity node correctly");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument

// For debugging*****       printf("All DOM_Entity method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The DOM_Entity method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
};


/**
 * This method tests DOM_EntityReference methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testEntityReference(DOM_Document document)
{
    DOM_EntityReference entityReference;
    DOM_Node node, node2;
    bool OK = true;
// For debugging*****   printf("\n          testEntityReference's outputs:\n");
    DOM_Node abc30 = document.getLastChild().getLastChild().getLastChild().getFirstChild();
    entityReference = (DOM_EntityReference &) abc30;
    node = entityReference;
    node2 = node.cloneNode(true);
    // Check nodes for equality, both their name and value or lack thereof
    if (!(node.getNodeName().equals(node2.getNodeName()) &&         // Compares node names for equality
         (node.getNodeValue() != null && node2.getNodeValue() != null)  // Checks to make sure each node has a value node
        ? node.getNodeValue().equals(node2.getNodeValue())          // If both have value nodes test those value nodes for equality
        :(node.getNodeValue() == null && node2.getNodeValue() == null)))// If one node doesn't have a value node make sure both don't
    {   
        printf("'cloneNode' did not clone the DOM_EntityReference node correctly\n");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument

    //  entityReference.setNodeValue("This shouldn't work");//!! Throws a NO_MODIFICATION_ALLOWED_ERR ********
    
// For debugging*****       printf("All DOM_EntityReference method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The DOM_EntityReference method calls listed above failed, all others worked correctly.*****\n");
//  printf("\n");
};



/**
 * This method tests DOM_Node methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 *
 *
 ********* This is only for a test of cloneNode "deep"*******
 ********* And for error tests*********
 */
void DTest::testNode(DOM_Document document)
{
    DOM_Node node, node2;
    bool result;
    bool OK = true;
// For debugging*****   printf("\n          testNode's outputs:\n");
    node = document.getDocumentElement();
    node2 = node.cloneNode(true);
    result = treeCompare(node, node2); // Deep clone test of cloneNode
    if (result)
    {
        //printf("'cloneNode' successfully cloned this whole node tree (deep)!\n");
    }
    else
    {
        printf("'cloneNode' did not successfully clone this whole node tree (deep)!\n");
        OK = false; 
    }
    //!! The following gives a did not clone successfully message*********
    node = document.getDocumentElement();
    node2 = node.getFirstChild();
    result = treeCompare(node, node2);
    if (!result)
    {
        //printf("'cloneNode' did not successfully clone this whole node tree (deep)!\n");
    }
    else
    {
        printf("'cloneNode' was supposed to fail here, either it or 'treeCompare' failed!!!\n");
        OK = false;
    }
    // Deep clone test also in testDocument
    
// For debugging*****       printf("All DOM_Node method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The DOM_Node method calls listed above failed, all others worked correctly.*****\n");    
//  printf("\n");
};



/**
 * This method tests DOM_Notation methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testNotation(DOM_Document document)
{
    DOM_Node node, node2;
    DOM_Notation notation;
    bool OK = true;
    DOMString compare;
// For debugging*****   printf("\n          testNotation's outputs:\n");
	DOM_Node abc40 = document.getDoctype().getNotations().getNamedItem("ourNotationNode");
    notation = (DOM_Notation &) abc40;
    node = notation;
    node2 = notation.cloneNode(true);//*****?
    // Check nodes for equality, both their name and value or lack thereof
    if (!(node.getNodeName().equals(node2.getNodeName()) &&         // Compares node names for equality
         (node.getNodeValue() != null && node2.getNodeValue() != null)  // Checks to make sure each node has a value node
        ? node.getNodeValue().equals(node2.getNodeValue())          // If both have value nodes test those value nodes for equality
        :(node.getNodeValue() == null && node2.getNodeValue() == null)))// If one node doesn't have a value node make sure both don't
    {   
        printf("'cloneNode' did not clone the DOM_Notation node correctly");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument

    //  notation.setNodeValue("This shouldn't work");//!! Throws a NO_MODIFICATION_ALLOWED_ERR ********
    
// For debugging*****       printf("All DOM_Notation method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The DOM_Notation method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
};



/**
 * This method tests DOM_ProcessingInstruction methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testPI(DOM_Document document)
{
    DOM_Node node, node2;
    DOM_ProcessingInstruction pI, pI2;
    DOMString compare;
    bool OK = true;
// For debugging*****   printf("\n          testPI's outputs:\n");
	DOM_Node  abc50 = document.getDocumentElement().getFirstChild();// Get doc's DOM_ProcessingInstruction
    pI  = (DOM_ProcessingInstruction &) abc50;
	DOM_Node  abc51 = pI.cloneNode(true);//*****?
    pI2 = (DOM_ProcessingInstruction &) abc51;
    // Check nodes for equality, both their name and value or lack thereof
    if (!(pI.getNodeName().equals(pI2.getNodeName()) &&         // Compares node names for equality
         (pI.getNodeValue() != null && pI2.getNodeValue() != null)  // Checks to make sure each node has a value node
        ? pI.getNodeValue().equals(pI2.getNodeValue())      // If both have value nodes test those value nodes for equality
        :(pI.getNodeValue() == null && pI2.getNodeValue() == null)))// If one node doesn't have a value node make sure both don't
    {   
        printf("'cloneNode' did not clone the DOM_Entity node correctly\n");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument
    // compare = "This is [#document: null]'s processing instruction";  // AH Revisit.  Where id
    //                  this ": null]" stuff come from in the Java version??  I don' think that it is right.
    compare = "This is #document's processing instruction";
    if (! compare.equals(pI.getData()))
    {
        printf("Warning!!! PI's 'getData' failed!\n");
        OK = false;
    }
    
    pI.setData("PI's reset data");
    compare = "PI's reset data";
    if (! compare.equals(pI.getData()))
    {
        printf("Warning!!! PI's 'setData' failed!\n");
        OK = false;
    }   
    compare = "dTargetProcessorChannel";
    if (! compare.equals(pI.getTarget()))
    {
        printf("Warning!!! PI's 'getTarget' failed!\n");
        OK = false;
    }   
    

    // Restore original PI data.
    pI.setData("This is #document's processing instruction");

// For debugging*****       printf("All PI method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The PI method calls listed above failed, all others worked correctly.*****\n");
    
//  printf("\n");
};



/**
 * This method tests DOM_Text methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void DTest::testText(DOM_Document document)
{
    DOM_Node node, node2;
    DOM_Text text;
    DOMString compare;
    bool OK = true;
// For debugging*****   printf("\n          testText's outputs:\n");
    DOM_Node abc70 = document.getDocumentElement().getElementsByTagName("dBodyLevel31").item(0);
    DOM_Element elem = (DOM_Element &) abc70;
    node = elem.getFirstChild(); // charData gets textNode11
    text = (DOM_Text &) node;
    node2 = node.cloneNode(true);//*****?
    // Check nodes for equality, both their name and value or lack thereof
    if (!(node.getNodeName().equals(node2.getNodeName()) &&         // Compares node names for equality
         (node.getNodeValue() != null && node2.getNodeValue() != null)  // Checks to make sure each node has a value node
        ? node.getNodeValue().equals(node2.getNodeValue())          // If both have value nodes test those value nodes for equality
        :(node.getNodeValue() == null && node2.getNodeValue() == null)))// If one node doesn't have a value node make sure both don't
    {   
        printf("'cloneNode' did not clone the DOM_Text node correctly\n");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument

    text.splitText(25);
    // Three original text nodes were concatenated by 'normalize' in testElement
    compare = "dBodyLevel31'sChildTextNo";
    if (! compare.equals(text.getNodeValue()))
        {
            printf("First part of DOM_Text's split text failed!\n" );
            OK = false;
        }
    // Three original text nodes were concatenated by 'normalize' in testElement
    compare = "de11dBodyLevel31'sChildTextNode12dBodyLevel31'sChildTextNode13";
    if (! compare.equals(text.getNextSibling().getNodeValue()))
        {
            printf("The second part of DOM_Text's split text failed!\n") ;
            OK = false; 
        }

    // Re-normalize the text nodes under elem, so that this test can be rerun.
    elem.normalize();


//************************************************* ERROR TESTS
    DTest tests;        
    //!! Throws INDEX_SIZE_ERR ********************
    //  text.splitText(-1);
    //  text.splitText(100);
    
// For debugging*****       printf("All DOM_Text method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The DOM_Text method calls listed above failed, all others worked correctly.*****\n");
    
//  printf("\n");
};




/**
 * 
 * @param node org.w3c.dom.DOM_Node
 * @param node2 org.w3c.dom.DOM_Node
 *
 */
bool DTest::treeCompare(DOM_Node node, DOM_Node node2)
{
    bool answer = true;
        
    DOM_Node kid, kid2;         // Check the subtree for equality
    kid = node.getFirstChild();
    kid2 = node2.getFirstChild();
    if (!kid.isNull() && !kid2.isNull())
    {
        answer = treeCompare(kid, kid2);
        if (!answer)
            return answer;
        else
            if (!kid.getNextSibling().isNull() && !kid2.getNextSibling().isNull())
            {
                while (!kid.getNextSibling().isNull() && !kid2.getNextSibling().isNull())
                {
                    answer = treeCompare(kid.getNextSibling(), kid2.getNextSibling());
                    if (!answer)
                        return answer;
                    else
                    {
                        kid = kid.getNextSibling();
                        kid2 = kid2.getNextSibling();
                    }
                }
            } else
                if (!(kid.getNextSibling().isNull() && kid2.getNextSibling().isNull()))
                {
                    return false;
                }
    } else
        if (kid != kid2)
        {
            // One or the other of (kid1, kid2) is null, but not both.
            return false;
        }

    if (!(node.getNodeName().equals(node2.getNodeName())))
        return false;
    if (node.getNodeValue()==null && node2.getNodeValue()!=null)
        return false;
    if (node.getNodeValue()!=null && node2.getNodeValue()==null)
        return false;
    if (!(node.getNodeValue().equals(node2.getNodeValue())))
        return false;

    return answer;
};


