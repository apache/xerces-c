/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation->  All rights
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
 *    distribution->
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
 *    software without prior written permission-> For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation->
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

/*
 * $Log$
 * Revision 1.6  2002/04/01 21:04:00  tng
 * According to DOM spec, setNodeValue by default is no-op.
 *
 * Revision 1.5  2002/03/14 21:59:29  tng
 * Run methods test[NodeType] in the IDOMTest and other fixes.
 *
 * Revision 1.4  2002/02/01 22:44:24  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/12/07 01:48:27  tng
 * [Bug 1959] setNodeValue throws exception when spec specifies NOP.
 *
 * Revision 1.2  2001/11/23 16:16:52  tng
 * Elimiate compiler warning Warning: String literal converted to char* in initialization.
 *
 * Revision 1.1  2001/08/09 19:28:47  tng
 * Port test case DOMTest to  IDOMTest
 *
 */



/**
 * This class tests methods for XML IDOM implementation
 * IDOM_DOMException errors are tested by calls to DOMExceptionsTest from: Main, docBuilder...
 *
 */

#include <stdio.h>
#include "IDTest.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLString.hpp>


#define EXCEPTIONSTEST(operation, expectedException, resultFlag, testNum) \
    {                                                               \
        try                                                         \
        {                                                           \
            operation;                                              \
            fprintf(stderr, "Exceptions Test # %d: no Exception thrown->\n", testNum); \
        }                                                           \
        catch (IDOM_DOMException &e)                                 \
        {                                                           \
            if (e.code != expectedException) {                      \
                fprintf(stderr, "Exceptions Test # %d: wrong IDOM_DOMException thrown->\n", \
                    testNum);                                       \
                resultFlag = false;                                 \
            }                                                       \
        }                                                           \
        catch (...)                                                 \
        {                                                           \
            fprintf(stderr, "Exceptions Test # %d: unknown exception thrown->\n",    \
                testNum);                                           \
            resultFlag = false;                                     \
        }                                                           \
    }

//temp XMLCh String Buffer
XMLCh tempStr[4000];
XMLCh tempStr2[4000];
XMLCh tempStr3[4000];

IDOM_Element*                 IDTest::testElementNode;
IDOM_Attr*                    IDTest::testAttributeNode;
IDOM_Text*                    IDTest::testTextNode;
IDOM_CDATASection*            IDTest::testCDATASectionNode;
IDOM_EntityReference*         IDTest::testEntityReferenceNode;
IDOM_Entity*                  IDTest::testEntityNode;
IDOM_ProcessingInstruction*   IDTest::testProcessingInstructionNode;
IDOM_Comment*                 IDTest::testCommentNode;
IDOM_Document*                IDTest::testDocumentNode;
IDOM_DocumentType*            IDTest::testDocumentTypeNode;
IDOM_DocumentFragment*        IDTest::testDocumentFragmentNode;
IDOM_Notation*                IDTest::testNotationNode;

/**
 *
 *
 */

IDTest::IDTest()
{
};


/**
 *
 * @return IDOM_Document
 *
 */
IDOM_Document* IDTest::createDocument() {
    IDOM_DOMImplementation* impl = IDOM_DOMImplementation::getImplementation();
    return impl->createDocument();
};


/**
 *
 * @return IDOM_DocumentType
 * @param name XMLCh*
 *
 */
IDOM_DocumentType* IDTest::createDocumentType(IDOM_Document* doc, XMLCh* name) {
    return doc->createDocumentType(name);    //Replace with a IDOM_DocumentType* creator
};


/**
 *
 * @return org.w3c.dom.IDOM_Entity
 * @param doc org.w3c.dom.IDOM_Document
 * @param name XMLCh*
 *
 */
IDOM_Entity* IDTest::createEntity(IDOM_Document* doc, XMLCh* name) {
    return doc->createEntity(name);
};



/**
 *
 * @return org.w3c.dom.IDOM_Notation
 * @param doc org.w3c.dom.IDOM_Document
 * @param name XMLCh*
 *
 */
IDOM_Notation* IDTest::createNotation(IDOM_Document* doc, XMLCh* name) {
    return doc->createNotation(name);
};


/**
 * This method builds test documents for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 * @param name document's name
 * @param type document's type
 *
 */
bool IDTest::docBuilder(IDOM_Document* document, XMLCh* nameIn)
{
    XMLCh* name = XMLString::replicate(nameIn);

    IDOM_Document* doc = document;
    bool OK = true;

    //name + "FirstElement"
    XMLString::transcode("FirstElement", tempStr2, 3999);
    XMLString::copyString(tempStr, name);
    XMLString::catString(tempStr, tempStr2);

    IDOM_Element* docFirstElement = doc->createElement(tempStr);
    doc->appendChild(docFirstElement);

    //name + "FirstElement", name + "firstElement"
    XMLString::catString(tempStr, name);
    XMLString::transcode("FirstElement", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    XMLString::catString(tempStr2, name);
    XMLString::transcode("firstElement", tempStr3, 3999);
    XMLString::catString(tempStr2, tempStr3);
    docFirstElement->setAttribute(tempStr, tempStr2);

    //name + "TargetProcessorChannel" + "This is " + doc->getNodeName() + "'s processing instruction");
    XMLString::copyString(tempStr, name);
    XMLString::transcode("TargetProcessorChannel", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    XMLString::transcode("This is ", tempStr2, 3999);
    XMLString::catString(tempStr2, doc->getNodeName());
    XMLString::transcode("'s processing instruction", tempStr3, 3999);
    XMLString::catString(tempStr2, tempStr3);

    IDOM_ProcessingInstruction* docProcessingInstruction = doc->createProcessingInstruction(tempStr, tempStr2);
    docFirstElement->appendChild(docProcessingInstruction);

    //name + "TestBody"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("TestBody", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Element* docBody = doc->createElement(tempStr);
    docFirstElement->appendChild(docBody);

    //name + "BodyLevel21"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("BodyLevel21", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Element* docBodyLevel21 = doc->createElement(tempStr);

    //name + "BodyLevel22"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("BodyLevel22", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Element* docBodyLevel22 = doc->createElement(tempStr);

    //name + "BodyLevel23"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("BodyLevel23", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Element* docBodyLevel23 = doc->createElement(tempStr);

    //name + "BodyLevel24"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("BodyLevel24", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Element* docBodyLevel24 = doc->createElement(tempStr);

    docBody->appendChild(docBodyLevel21);
    docBody->appendChild(docBodyLevel22);
    docBody->appendChild(docBodyLevel23);
    docBody->appendChild(docBodyLevel24);

    //name + "BodyLevel31"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("BodyLevel31", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Element* docBodyLevel31 = doc->createElement(tempStr);

    //name + "BodyLevel32"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("BodyLevel32", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Element* docBodyLevel32 = doc->createElement(tempStr);

    //name + "BodyLevel33"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("BodyLevel33", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Element* docBodyLevel33 = doc->createElement(tempStr);

    //name + "BodyLevel34"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("BodyLevel34", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Element* docBodyLevel34 = doc->createElement(tempStr);

    docBodyLevel21->appendChild(docBodyLevel31);
    docBodyLevel21->appendChild(docBodyLevel32);
    docBodyLevel22->appendChild(docBodyLevel33);
    docBodyLevel22->appendChild(docBodyLevel34);

    //name + "BodyLevel31'sChildTextNode11"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("BodyLevel31'sChildTextNode11", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Text* docTextNode11 = doc->createTextNode(tempStr);

    //name + "BodyLevel31'sChildTextNode12"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("BodyLevel31'sChildTextNode12", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Text* docTextNode12 = doc->createTextNode(tempStr);

    //name + "BodyLevel31'sChildTextNode13"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("BodyLevel31'sChildTextNode13", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Text* docTextNode13 = doc->createTextNode(tempStr);

    //name + "TextNode2"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("TextNode2", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Text* docTextNode2 = doc->createTextNode(tempStr);

    //name + "TextNode3"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("TextNode3", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Text* docTextNode3 = doc->createTextNode(tempStr);

    //name + "TextNode4"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("TextNode4", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Text* docTextNode4 = doc->createTextNode(tempStr);

    docBodyLevel31->appendChild(docTextNode11);
    docBodyLevel31->appendChild(docTextNode12);
    docBodyLevel31->appendChild(docTextNode13);
    docBodyLevel32->appendChild(docTextNode2);
    docBodyLevel33->appendChild(docTextNode3);
    docBodyLevel34->appendChild(docTextNode4);

    //"<![CDATA[<greeting>Hello, world!</greeting>]]>"
    XMLString::transcode("<![CDATA[<greeting>Hello, world!</greeting>]]>", tempStr, 3999);
    IDOM_CDATASection* docCDATASection = doc->createCDATASection(tempStr);
    docBodyLevel23->appendChild(docCDATASection);

    //"This should be a comment of some kind "
    XMLString::transcode("This should be a comment of some kind ", tempStr, 3999);
    IDOM_Comment* docComment = doc->createComment(tempStr);
    docBodyLevel23->appendChild(docComment);

    //"ourEntityNode"
    XMLString::transcode("ourEntityNode", tempStr, 3999);
    IDOM_EntityReference* docReferenceEntity = doc->createEntityReference(tempStr);
    docBodyLevel24->appendChild(docReferenceEntity);

    IDTest make;

    //"ourNotationNode"
    XMLString::transcode("ourNotationNode", tempStr, 3999);
    IDOM_Notation* docNotation = make.createNotation(doc, tempStr);
    IDOM_Node*  abc1 = doc->getFirstChild();
    IDOM_DocumentType* docType = (IDOM_DocumentType*) abc1;
    docType->getNotations()->setNamedItem(docNotation);

    IDOM_DocumentFragment* docDocFragment = doc->createDocumentFragment();

//  printf("This document's first element name is " + docFirstElement->getTagName() + "\n");


//***********Following are for errorTests

    //name + "docTextNode3"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("docTextNode3", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Text* docNode3 = doc->createTextNode(tempStr);

    //name + "docTextNode4"
    XMLString::copyString(tempStr, name);
    XMLString::transcode("docTextNode4", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    IDOM_Text* docNode4 = doc->createTextNode(tempStr);

    //"ourEntityNode"
    XMLString::transcode("ourEntityNode", tempStr, 3999);
    IDOM_Node*   abc2 =  doc->getDoctype()->getEntities()->getNamedItem(tempStr);  // Get the IDOM_Entity* node
    IDOM_Entity* docEntity = (IDOM_Entity*) abc2;
    IDOM_Node*   abc3 = doc->getFirstChild(); // Get the IDOM_DocumentType* node
    IDOM_DocumentType* docDocType = (IDOM_DocumentType*) abc3;
    IDOM_Node*   abc4 = doc->getLastChild()->getLastChild()->getLastChild()->getFirstChild();
    IDOM_EntityReference* entityReferenceText = (IDOM_EntityReference*) abc4;

    //"entityReferenceText information"
    XMLString::transcode("entityReferenceText information", tempStr, 3999);
    IDOM_Text* entityReferenceText2 = doc->createTextNode(tempStr);
//************************************************* ERROR TESTS
    IDTest tests;

    EXCEPTIONSTEST(document->appendChild(docBody), IDOM_DOMException::HIERARCHY_REQUEST_ERR, OK,  1);

    EXCEPTIONSTEST(document->appendChild(docBody), IDOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 2);
    EXCEPTIONSTEST(docNode3->appendChild(docNode4), IDOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 3);
    // EXCEPTIONSTEST(doc->insertBefore(docEntity, docFirstElement), IDOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 4);
    EXCEPTIONSTEST(doc->replaceChild(docCDATASection, docFirstElement), IDOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 5);

    //"This shouldn't work!"
    XMLString::transcode("entityReferenceText information", tempStr, 3999);

    // The following setNodeValue tests are not invalid
    // According to DOM spec, if the node value is defined to be null in the DOM spec, setting it has no effect.
    // Only those node type that are supposed to have node value, exception will be raised if the node is readonly.
    // EXCEPTIONSTEST(docFirstElement->setNodeValue(tempStr), IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 6);
    // EXCEPTIONSTEST(docReferenceEntity->setNodeValue(tempStr), IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 7);
    // EXCEPTIONSTEST(docEntity->setNodeValue(tempStr), IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 8);
    // EXCEPTIONSTEST(doc->setNodeValue(tempStr), IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 9);
    // EXCEPTIONSTEST(docDocType->setNodeValue(tempStr), IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 10);
    // EXCEPTIONSTEST(docDocFragment->setNodeValue(tempStr), IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 11);
    // EXCEPTIONSTEST(docNotation->setNodeValue(tempStr), IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, OK, 12);
    EXCEPTIONSTEST(docReferenceEntity->appendChild(entityReferenceText2 ), IDOM_DOMException::NO_MODIFICATION_ALLOWED_ERR , OK, 13);
    EXCEPTIONSTEST(docBodyLevel32->insertBefore(docTextNode11,docBody ), IDOM_DOMException::NOT_FOUND_ERR, OK, 14);
    EXCEPTIONSTEST(docBodyLevel32->removeChild(docFirstElement), IDOM_DOMException::NOT_FOUND_ERR, OK, 15);
    EXCEPTIONSTEST(docBodyLevel32->replaceChild(docTextNode11,docFirstElement ), IDOM_DOMException::NOT_FOUND_ERR, OK, 16);

    delete [] name;

    return OK;


//!! Throws a NOT_FOUND_ERR ********

     // docBodyLevel32->getAttributes()->removeNamedItem(testAttribute->getName());    16  // To test removeNamedItem

};  //END OF DOCBUILDER



/**
 * @param document org.w3c.dom.IDOM_Document
 */
void IDTest::findTestNodes(IDOM_Document* document) {
    IDOM_Node* node = document;
    int nodeCount = 0;

    // Walk the tree until you find and assign all node types needed that exist.
    while (node != 0 && nodeCount < 12)
    {

        switch (node->getNodeType())
    {
        case IDOM_Node::ELEMENT_NODE :
            if (testElementNode == 0) {testElementNode = (IDOM_Element*)node; nodeCount++;}
            break;
        case IDOM_Node::ATTRIBUTE_NODE :
            if (testAttributeNode == 0) {testAttributeNode = (IDOM_Attr*)node; nodeCount++;}
            break;
        case IDOM_Node::TEXT_NODE :
            if (testTextNode == 0) {testTextNode = (IDOM_Text*)node; nodeCount++;}
            break;
        case IDOM_Node::CDATA_SECTION_NODE :
            if (testCDATASectionNode == 0) {testCDATASectionNode = (IDOM_CDATASection*)node; nodeCount++;}
            break;
        case IDOM_Node::ENTITY_REFERENCE_NODE :
            if (testEntityReferenceNode == 0) {testEntityReferenceNode = (IDOM_EntityReference*)node; nodeCount++;}
            break;
        case IDOM_Node::ENTITY_NODE :
            if (testEntityNode == 0) {testEntityNode = (IDOM_Entity*)node; nodeCount++;}
            break;
        case IDOM_Node::PROCESSING_INSTRUCTION_NODE :
            if (testProcessingInstructionNode == 0) {testProcessingInstructionNode = (IDOM_ProcessingInstruction*)node; nodeCount++;}
            break;
        case IDOM_Node::COMMENT_NODE :
            if (testCommentNode == 0) {testCommentNode = (IDOM_Comment*)node; nodeCount++;}
            break;
        case IDOM_Node::DOCUMENT_TYPE_NODE :
            if (testDocumentTypeNode == 0) {testDocumentTypeNode = (IDOM_DocumentType*)node; nodeCount++;}
            break;
        case IDOM_Node::DOCUMENT_FRAGMENT_NODE :
            if (testDocumentFragmentNode == 0) {testDocumentFragmentNode = (IDOM_DocumentFragment*)node; nodeCount++;}
            break;
        case IDOM_Node::NOTATION_NODE :
            if (testNotationNode == 0) {testNotationNode = (IDOM_Notation*)node; nodeCount++;}
            break;
        case IDOM_Node::DOCUMENT_NODE :
            if (testDocumentNode == 0) {testDocumentNode = (IDOM_Document*)node; nodeCount++;}
            break;
        default:
            ;
    }// End of switch

    }   // End of while
};


/**
 * @param document org.w3c.dom.IDOM_Document
 */
void IDTest::findTestNodes(IDOM_Node* node) {
    IDTest test;
    IDOM_Node*  kid;
    // Walk the tree until you find and assign all node types needed that exist.


    if (node->getFirstChild() != 0)
    {
        kid = node->getFirstChild();
        test.findTestNodes(kid);
    }


    if (node->getNextSibling() != 0)
    {
        kid = node->getNextSibling();
        test.findTestNodes(kid);
    }


    switch (node->getNodeType())
    {
        case IDOM_Node::ELEMENT_NODE :
            if (testElementNode == 0) {testElementNode = (IDOM_Element*)node; }
            break;
        case IDOM_Node::ATTRIBUTE_NODE :
            if (testAttributeNode == 0) {testAttributeNode = (IDOM_Attr*)node; }
            break;
        case IDOM_Node::TEXT_NODE :
            if (testTextNode == 0) {testTextNode = (IDOM_Text*)node; }
            break;
        case IDOM_Node::CDATA_SECTION_NODE :
            if (testCDATASectionNode == 0) {testCDATASectionNode = (IDOM_CDATASection*)node; }
            break;
        case IDOM_Node::ENTITY_REFERENCE_NODE :
            if (testEntityReferenceNode == 0) {testEntityReferenceNode = (IDOM_EntityReference*)node;}
            break;
        case IDOM_Node::ENTITY_NODE :
            if (testEntityNode == 0) {testEntityNode = (IDOM_Entity*)node;}
            break;
        case IDOM_Node::PROCESSING_INSTRUCTION_NODE :
            if (testProcessingInstructionNode == 0) {testProcessingInstructionNode = (IDOM_ProcessingInstruction*)node;}
            break;
        case IDOM_Node::COMMENT_NODE :
            if (testCommentNode == 0) {testCommentNode = (IDOM_Comment*)node;}
            break;
        case IDOM_Node::DOCUMENT_TYPE_NODE :
            if (testDocumentTypeNode == 0) {testDocumentTypeNode = (IDOM_DocumentType*)node; }
            break;
        case IDOM_Node::DOCUMENT_FRAGMENT_NODE :
            if (testDocumentFragmentNode == 0) {testDocumentFragmentNode = (IDOM_DocumentFragment*)node;}
            break;
        case IDOM_Node::NOTATION_NODE :
            if (testNotationNode == 0) {testNotationNode = (IDOM_Notation*)node;}
            break;
        case IDOM_Node::DOCUMENT_NODE :
            if (testDocumentNode == 0) {testDocumentNode = (IDOM_Document*)node;}
            break;
        default:
            ;
    }// End of switch
};//End of class

/**
 *
 *
 */
int main(int argc, char **argv)
 {
     bool OK = true;

     {
         //  Nest entire test in an inner block.
         //     Reference counting should recover all document
         //     storage when this block exits.

         IDTest test;
         try {
             XMLPlatformUtils::Initialize();
         }
         catch (const XMLException& toCatch) {
             char *pMessage = XMLString::transcode(toCatch.getMessage());
             fprintf(stderr, "Error during initialization! \n  %s \n", pMessage);
             delete [] pMessage;
             return -1;
         }

         long avgTime = 0;
         long startTime = 0;//****************Time the whole thing for efficiency of IDOM implementation

         // for (int i=0; i< 1000; i++)
         // {
         // AH Revisit  //  startTime = System.currentTimeMillis();
         //     if(!OK)
         //     break;

         IDOM_Document* d = test.createDocument();

         XMLString::transcode("testDocument1", tempStr, 3999);
         IDOM_DocumentType* docDocType = test.createDocumentType(d,tempStr);
         d->appendChild(docDocType);

         XMLString::transcode("ourEntityNode", tempStr, 3999);
         IDOM_Entity* docEntity = test.createEntity( d, tempStr);
         //Build a branch for entityReference tests
         // IDOM_Text* entityChildText = d.createTextNode("entityChildText information"); //
         // docEntity->appendChild(entityChildText);
         // docDocType->getEntities()->setNamedItem(docEntity);

         XMLString::transcode("d", tempStr3, 3999);
         OK = test.docBuilder(d, tempStr3);

         test.findTestNodes((IDOM_Node*)d);

         OK = test.testAttr(d);
         OK = test.testCDATASection(d);
         OK = test.testCharacterData(d);
         OK = test.testChildNodeList(d);
         OK = test.testComment(d);
         OK = test.testDeepNodeList(d);
         OK = test.testDocument(d);
         OK = test.testDocumentFragment(d);
         OK = test.testDocumentType(d);
         OK = test.testIDOMImplementation(d);
         OK = test.testElement(d);
//         OK = test.testEntity(d);  // Can not test entities;  only parser can create them.
         OK = test.testEntityReference(d);
         OK = test.testNode(d);
         OK = test.testNotation(d);
         OK = test.testPI(d);
         OK = test.testText(d);
         OK = test.testIDOMerrors(d);

         // Null out the static object references in class IDTest,
         // which will recover their storage.
         IDTest::testElementNode = 0;
         IDTest::testAttributeNode = 0;
         IDTest::testTextNode = 0;
         IDTest::testCDATASectionNode = 0;
         IDTest::testEntityReferenceNode = 0;
         IDTest::testEntityNode = 0;
         IDTest::testProcessingInstructionNode = 0;
         IDTest::testCommentNode = 0;
         IDTest::testDocumentNode = 0;
         IDTest::testDocumentTypeNode = 0;
         IDTest::testDocumentFragmentNode = 0;
         IDTest::testNotationNode = 0;

         delete d;

    };

    XMLPlatformUtils::Terminate();

    if (OK) printf("Test Run Successfully\n");
    return 0;
};


/**
 * This method tests IDOM_Attr* methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testAttr(IDOM_Document* document)
{
    IDOM_Node* node;
    IDOM_Attr* attributeNode;
    bool T = true;
    bool F = false;
    bool OK = true;
// For debugging*****   printf("\n          testAttr's outputs:\n\n");

    XMLString::transcode("testAttribute", tempStr, 3999);
    IDOM_Attr* testAttribute = document->createAttribute(tempStr);

    XMLString::transcode("testAttribute's value", tempStr, 3999);
    testAttribute->setValue(tempStr);
    node = document->getDocumentElement(); // node gets first element

    // ((IDOM_Element*)node)->setAttributeNode(testAttribute);
    // attributeNode = ((IDOM_Element*)node)->getAttributeNode("testAttribute");
    IDOM_Element* el = (IDOM_Element*)node;
    el->setAttributeNode(testAttribute);

    XMLString::transcode("testAttribute", tempStr, 3999);
    attributeNode = el->getAttributeNode(tempStr);

    if (XMLString::compareString(tempStr, attributeNode->getName()))
    {
        printf("Warning!!! IDOM_Attr's 'getName' method failed to work properly!\n");
        OK = false;
    }

    XMLString::transcode("testAttribute's value", tempStr, 3999);
    if (XMLString::compareString(tempStr, attributeNode->getNodeValue()))
    {
        printf("Warning!!! IDOM_Attr's 'getNodeValue' method failed to work properly!\n");
        OK = false;
    }
    if (! T ==attributeNode->getSpecified())
    {
        printf("Warning!!! IDOM_Attr's 'getSpecified' method failed to work properly!\n");
        OK = false;
    }

    if (XMLString::compareString(tempStr, attributeNode->getValue()))
    {
        printf("Warning!!! IDOM_Attr's 'getValue' method failed to work properly!\n");
        OK = false;
    }


    XMLString::transcode("Reset Value", tempStr, 3999);
    attributeNode->setNodeValue(tempStr);   /// LEAK!!!!!
    if (XMLString::compareString(tempStr, attributeNode->getNodeValue()))
    {
        printf("Warning!!! IDOM_Attr's 'setNodeValue' method failed to work properly!\n");
        OK = false;
    }

    attributeNode->setValue(XMLUni::fgZeroLenString);
    if (XMLString::compareString(XMLUni::fgZeroLenString, attributeNode->getValue()))
    {
        printf("Warning!!! IDOM_Attr's 'setValue' to '0' method failed to work properly!\n");
        OK = false;
    }

    XMLString::transcode("Another value ", tempStr, 3999);
    attributeNode->setValue(tempStr);
    if (XMLString::compareString(tempStr, attributeNode->getValue()))
    {
        printf("Warning!!! IDOM_Attr's 'setValue' method failed to work properly!");
        OK = false;
    }

    node = attributeNode->cloneNode(T);

    // Check nodes for equality, both their name and value or lack thereof
    bool cloneOK = true;
    if (XMLString::compareString(node->getNodeName(), attributeNode->getNodeName()))
        cloneOK = false;
    if (node->getNodeValue() == 0 &&
        attributeNode->getNodeValue() != 0)
    {
        cloneOK = false;
    }

    if (node->getNodeValue() != 0 && attributeNode->getNodeValue() == 0)
    {
        cloneOK = false;
    };

    if (node->getNodeValue() != 0 && attributeNode->getNodeValue() != 0)
    {
        if (XMLString::compareString(node->getNodeValue(),attributeNode->getNodeValue()))
            cloneOK = false;
    }


/*
    if (! (node->getNodeName(), attributeNode->getNodeName()) &&         // Compares node names for equality
          (node->getNodeValue() != 0 && attributeNode->getNodeValue() != 0)  // Checks to make sure each node has a value node
        ?  node->getNodeValue(), attributeNode->getNodeValue())          // If both have value nodes test those value nodes for equality
        : (node->getNodeValue() == 0 && attributeNode->getNodeValue() == 0)))// If one node doesn't have a value node make sure both don't
*/
    if (cloneOK == false)
        {
            fprintf(stderr, "'cloneNode' did not clone the Attribute node correctly\n");
            OK = false;
        }
        // Deep clone test comparison is in testNode & testDocument

//************************************************* ERROR TESTS
    IDTest tests;
//!! Throws HIERARCHY_REQUEST_ERR ****************
    //  doc->getDocumentElement()->appendChild(attributeNode);

//!! Throws a NOT_FOUND_ERR ********
    //  attribute2 = doc->createAttribute("testAttribute2");
    //  doc->getDocumentElement()->removeAttributeNode(attribute2);

//!! Throws an INUSE_ATTRIBUTE_ERR ******
    //  IDOM_Element* element = (IDOM_Element*)doc->getLastChild()->getLastChild();
    //  element->setAttributeNode(testAttribute );// Tests setNamedItem which generates error through justSetNamedItem.

// For debugging*****       printf("All IDOM_Attr* method calls worked correctly.\n");
    if (! OK)
        printf("\n*****The IDOM_Attr* method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
    return OK;

};




/**
 * This method tests IDOM_CDATASection* methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testCDATASection(IDOM_Document* document)
{
    IDOM_Node* node;
    IDOM_Node* node2;
    bool T = true;
    bool OK = true;
// For debugging*****   printf("\n          testCDATASection's outputs:\n");
    XMLString::transcode("dBodyLevel23", tempStr, 3999);
    node = document->getDocumentElement()->getElementsByTagName(tempStr)->item(0)->getFirstChild(); // node gets IDOM_CDATASection* node

    node2 = node->cloneNode(T);//*****?
    // Check nodes for equality, both their name and value or lack thereof
    if (!(!XMLString::compareString(node->getNodeName(), node2->getNodeName()) &&        // Compares node names for equality
          (node->getNodeValue() != 0 && node2->getNodeValue() != 0)     // Checks to make sure each node has a value node
        ? !XMLString::compareString(node->getNodeValue(), node2->getNodeValue())         // If both have value nodes test those value nodes for equality
        : (node->getNodeValue() == 0 && node2->getNodeValue() == 0)))   // If one node doesn't have a value node make sure both don't
    {
        fprintf(stderr, "'cloneNode' did not clone the IDOM_CDATASection* node correctly\n");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument

// For debugging*****   printf("All IDOM_CDATASection* method calls worked correctly.\n");

    if (! OK)
        printf("\n*****The IDOM_CDATASection* method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
    return OK;
};



/**
 * This method tests IDOM_CharacterData methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testCharacterData(IDOM_Document* document)
{
    IDOM_CharacterData* charData;
    XMLCh resetData[3999];
    bool OK = true;
// For debugging*****   printf("\n          testCharacterData's outputs:\n");
    XMLString::transcode("dBodyLevel31", tempStr, 3999);
    IDOM_Node*  abc1 = document->getDocumentElement()->getElementsByTagName(tempStr)->item(0)->getFirstChild(); // charData gets textNode11

    charData = (IDOM_CharacterData *) abc1;

    XMLString::transcode("dBodyLevel31'sChildTextNode11", tempStr, 3999);
    if (XMLString::compareString(tempStr, charData->getData()))
    {
        printf("Warning!!! IDOM_CharacterData's 'getData' failed to work properly!\n This may corrupt other IDOM_CharacterData tests!!!*****\n");
        OK = false;
    }

    XMLString::copyString(resetData,charData->getData());
    //  printf("This node's original data is: " + charData->getData());

    XMLString::transcode(" This is new data for this node", tempStr2, 3999);

    XMLString::copyString(tempStr, charData->getData());
    XMLString::catString(tempStr, tempStr2);

    charData->appendData(tempStr2);


    if (XMLString::compareString(tempStr, charData->getData()))
    {
        printf("Warning!!! IDOM_CharacterData's 'appendData' failed to work properly!\n");
        OK = false;
    }
    //  printf("This node's appended data is: " + charData->getData());

    XMLString::transcode("dBodyLevel", tempStr, 3999);
    charData->deleteData(10, 100);
    if (XMLString::compareString(tempStr, charData->getData()))
    {
        printf("Warning!!! IDOM_CharacterData's 'deleteData' failed to work properly!\n");
        OK = false;
    }
    //  printf("This node's partially deleted data is: " + charData->getData());

    unsigned int length = 10;
    if (!(length == charData->getLength()))
    {
        printf("Warning!!! IDOM_CharacterData's 'getLength' failed to work properly!\n");
        OK = false;
    }
    //  printf("This node's data length is: " + charData->getLength());

    XMLString::transcode("dBody' This is data inserted into this node'Level", tempStr, 3999);
    XMLString::transcode("' This is data inserted into this node'", tempStr2, 3999);
    charData->insertData(5, tempStr2);
    if (XMLString::compareString(tempStr, charData->getData()))
    {
        printf("Warning!!! IDOM_CharacterData's 'insertData' failed to work properly!\n");
        OK = false;
    }
    //  printf("This node's updated with insert data is: " + charData->getData());

    XMLString::transcode("dBody' This is ' replacement data'ted into this node'Level", tempStr, 3999);
    XMLString::transcode("' replacement data'", tempStr2, 3999);
    charData->replaceData(15, 10, tempStr2);
    if (XMLString::compareString(tempStr, charData->getData()))
    {
        printf("Warning!!! IDOM_CharacterData's 'replaceData' failed to work properly!\n");
        OK = false;
    }
    //  printf("This node's updated with replacement data is: " +charData->getData());

    XMLString::transcode("New data A123456789B123456789C123456789D123456789E123456789", tempStr, 3999);
    charData->setData(tempStr);
    if (XMLString::compareString(tempStr, charData->getData()))
    {
        printf("Warning!!! IDOM_CharacterData's 'setData' failed to work properly!");
        OK = false;
    }
    //  printf("This node's new data via setData: " + charData->getData());

    XMLString::transcode("123456789D123456789E123456789", tempStr, 3999);
    if (XMLString::compareString(tempStr, charData->substringData(30, 30)))
    {
        printf("Warning!!! IDOM_CharacterData's 'substringData' failed to work properly!\n");
        OK = false;
    }
    //  printf("Using subString 30,30 you get:");  charData->substringData(30,30)).print();

    XMLString::transcode("New data A123456789B12345", tempStr, 3999);
    if (XMLString::compareString(tempStr, charData->substringData(0, 25)))
    {
        printf("Warning!!! IDOM_CharacterData's 'substringData' failed to work properly!\n");
        OK = false;
    }
    //  printf("Using subString 0,25 you get: ");   charData->substringData(0,25)).print();

//************************************************* ERROR TESTS
    IDTest tests;   // What is this for?  'tests' is never used.

//!! Throws INDEX_SIZE_ERR ********************
    EXCEPTIONSTEST(charData->deleteData(-1, 5), IDOM_DOMException::INDEX_SIZE_ERR, OK, 101 );
    // Test 102 is not an error because the -1 parameter is an unsigned value, and counts
    //   that exceed the length of the string are allowed.
//    EXCEPTIONSTEST(charData->deleteData(2, -1), IDOM_DOMException::INDEX_SIZE_ERR, OK, 102 );
    EXCEPTIONSTEST(charData->deleteData(100, 5), IDOM_DOMException::INDEX_SIZE_ERR, OK,103 );

//can't set negative unsigned int in c++ compiler

  //  EXCEPTIONSTEST(charData->insertData(-1, "Stuff inserted"), IDOM_DOMException::INDEX_SIZE_ERR, OK, 104 );
    XMLString::transcode("Stuff inserted", tempStr, 3999);
    EXCEPTIONSTEST(charData->insertData(100, tempStr), IDOM_DOMException::INDEX_SIZE_ERR, OK, 105 );

  //  EXCEPTIONSTEST(charData->replaceData(-1, 5, "Replacement stuff") , IDOM_DOMException::INDEX_SIZE_ERR, OK, 106 );
    XMLString::transcode("Replacement stuff", tempStr, 3999);
    EXCEPTIONSTEST(charData->replaceData(100, 5 ,tempStr), IDOM_DOMException::INDEX_SIZE_ERR, OK, 107 );
  //  EXCEPTIONSTEST(charData->replaceData(2, -1, "Replacement stuff"), IDOM_DOMException::INDEX_SIZE_ERR,  OK, 108 );

    EXCEPTIONSTEST(charData->substringData(-1, 5), IDOM_DOMException::INDEX_SIZE_ERR, OK, 109 );
    EXCEPTIONSTEST(charData->substringData(100, 5), IDOM_DOMException::INDEX_SIZE_ERR, OK, 110 );
 //   EXCEPTIONSTEST(charData->substringData(2, -1), IDOM_DOMException::INDEX_SIZE_ERR, OK, 111 );


// For debugging*****       printf("All IDOM_CharacterData method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The IDOM_CharacterData method calls listed above failed, all others worked correctly.*****\n");
    charData->setData(resetData); // reset node to original data
//  printf(""\n);
    return OK;
};




/**
 * This method tests ChildNodeList methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testChildNodeList(IDOM_Document* document)
{
    IDOM_Node* node;
    IDOM_Node* node2;
    bool OK = true;
// For debugging*****   printf("\n          testChildNodeList's outputs:\n");
    node = document->getDocumentElement()->getLastChild(); // node gets doc's testBody element

    if (!(node->getChildNodes()->getLength()== 4))
        OK = false;
    node2 = node->getChildNodes()->item(2);
    XMLString::transcode("dBodyLevel23", tempStr, 3999);
    if (XMLString::compareString(tempStr, node2->getNodeName()))
        OK = false;

// For debugging*****       printf("All ChildNodeList method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The ChildNodeList method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
    return OK;
};



/**
 * This method tests IDOM_Comment* methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testComment(IDOM_Document* document)
{
    IDOM_Node* node;
    IDOM_Node* node2;
    bool T = true;
    bool OK = true;
// For debugging*****   printf("\n          testComment's outputs:\n");
    XMLString::transcode("dBodyLevel31", tempStr, 3999);
    node = document->getDocumentElement()->getElementsByTagName(tempStr)->item(0)->getFirstChild(); // node gets textNode11
    node2 = node->cloneNode(T);
    // Check nodes for equality, both their name and value or lack thereof
    if (!(!XMLString::compareString(node->getNodeName(), node2->getNodeName()) &&        // Compares node names for equality
          (node->getNodeValue() != 0 && node2->getNodeValue() != 0)     // Checks to make sure each node has a value node
        ? !XMLString::compareString(node->getNodeValue(), node2->getNodeValue())         // If both have value nodes test those value nodes for equality
        : (node->getNodeValue() == 0 && node2->getNodeValue() == 0)))   // If one node doesn't have a value node make sure both don't
    {
        fprintf(stderr, "'cloneNode' did not clone the IDOM_Comment* node correctly\n");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument
    if (OK)
// For debugging*****       printf("All IDOM_Comment* method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The IDOM_Comment* method calls listed above failed, all others worked correctly.*****\n");
//  printf("\n");
    return OK;
};



/**
 * This method tests DeepNodeList methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testDeepNodeList(IDOM_Document* document)
{
    IDOM_Node* node;
    IDOM_Node* node2;
    bool OK = true;
// For debugging*****   printf("\n          testDeepNodeList's outputs:\n\n");
    node = document->getLastChild()->getLastChild(); // node gets docBody element
//  IDOM_Element* el = (IDOM_Element*)node;
//  IDOM_NodeList nl = el->getElementsByTagName("*");
//  int len = nl->getLength();
//  if (len != 8)
    XMLString::transcode("*",tempStr, 3999);
    if (!(8 == ((IDOM_Element*) node)->getElementsByTagName(tempStr)->getLength()))
        {
            printf ("Warning!!! DeepNodeList's 'getLength' failed to work properly!\n");
            OK = false;
        }
    node2 = ((IDOM_Element*) node)->getElementsByTagName(tempStr)->item(2); //This also runs through 'nextMatchingElementAfter"

    XMLString::transcode("dBodyLevel32", tempStr, 3999);
    if (XMLString::compareString(tempStr, node2->getNodeName()))
        {
            printf ("Warning!!! DeepNodeList's 'item' (or IDOM_Element's 'getElementsBy TagName)failed to work properly!\n");
            OK = false;
        }
    node2 = document->getLastChild();
    XMLString::transcode("dTestBody", tempStr, 3999);
    if (XMLString::compareString(tempStr, ((IDOM_Element*) node2)->getElementsByTagName(tempStr)->item(0)->getNodeName()))//This also runs through 'nextMatchingElementAfter"
        {
            printf ("Warning!!! DeepNodeList's 'item' (or IDOM_Element's 'getElementsBy TagName)failed to work properly!\n");
            OK = false;
        }


// For debugging*****       printf("All DeepNodeList method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The DeepNodeList method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
    return OK;
};



/**
 * This method tests IDOM_Document* methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 *
 **** ALL IDOM_Document* create methods are run in docBuilder except createAttribute which is in testAttribute**
 */
bool IDTest::testDocument(IDOM_Document* document)
{
    IDTest make;
    IDOM_DocumentFragment* docFragment, *docFragment2;
    IDOM_Element* newElement;
    IDOM_Node* node, *node2;

    const char* elementNames[] =  {"dFirstElement", "dTestBody", "dBodyLevel21","dBodyLevel31","dBodyLevel32",
                   "dBodyLevel22","dBodyLevel33","dBodyLevel34","dBodyLevel23","dBodyLevel24"};
    const char* newElementNames[] = {"dFirstElement", "dTestBody", "dBodyLevel22","dBodyLevel33","dBodyLevel34","dBodyLevel23"};


    bool result;
    bool OK = true;
// For debugging*****   printf("\n          testDocument's outputs:\n \n");

    XMLString::transcode("testDocument1", tempStr, 3999);
    IDOM_DocumentType* checkDocType =  make.createDocumentType(document,tempStr);
    IDOM_DocumentType* docType = document->getDoctype();

    if (XMLString::compareString(checkDocType->getNodeName(),docType->getNodeName() ))
    {
        printf("Warning!!! IDOM_Document's 'getDocType method failed!\n" );
        OK = false;
    }

    if (XMLString::compareString(checkDocType->getNodeValue(), docType->getNodeValue()))
    {
        printf("Warning!!! IDOM_Document's 'getDocType method failed!\n" );
        OK = false;
    }

    /*
    if (! (checkDocType->getNodeName(), docType->getNodeName()) &&      // Compares node names for equality
          (checkDocType->getNodeValue() != 0 && docType->getNodeValue() != 0)   // Checks to make sure each node has a value node
        ?  checkDocType->getNodeValue(), docType->getNodeValue())       // If both have value nodes test those value nodes for equality
        : (checkDocType->getNodeValue() == 0 && docType->getNodeValue() == 0))) // If one node doesn't have a value node make sure both don't
    {
        printf("Warning!!! IDOM_Document's 'getDocType method failed!\n" );
        OK = false;
    }
    */

    IDOM_Node*  rootElement = document->getLastChild();

    bool check = (rootElement->getNodeValue() && document->getDocumentElement()->getNodeValue())   // Checks to make sure each node has a value node
        ?  !XMLString::compareString(rootElement->getNodeValue(), document->getDocumentElement()->getNodeValue())      // If both have value nodes test those value nodes for equality
        : (rootElement->getNodeValue() == 0 && document->getDocumentElement()->getNodeValue() == 0);    // If one node doesn't have a value node make sure both don't
    if (!(!XMLString::compareString(rootElement->getNodeName(), document->getDocumentElement()->getNodeName()) &&        // Compares node names for equality
         check))
    {
        printf("Warning!!! IDOM_Document's 'getDocumentElement' method failed!\n" );
        OK = false;
    }

    XMLString::transcode("*", tempStr, 3999);
    IDOM_NodeList* docElements = document->getElementsByTagName(tempStr);
    int docSize = docElements->getLength();
    int i;
    for (i = 0; i < docSize; i++)
    {
        IDOM_Node*  n = (IDOM_Node*) docElements->item(i);
        if (XMLString::compareString(XMLString::transcode(elementNames[i]), n->getNodeName()))
        {
            printf("Comparison of this document's elements failed at element number %d at line %i \n", i, __LINE__);
            OK = false;
            break;
        }
    }

    // What is this supposed to be doing?
    //
    //if (document->equals(document->getImplementation()))
    //{
    //  printf("Warning!!! IDOM_Document's 'getImplementation' method failed!\n" );
    //  OK = false;
    //}

    XMLString::transcode("NewElementTestsInsertBefore", tempStr, 3999);
    newElement = document->createElement(tempStr);
    //  doc->insertBefore(newElement,0);//!! Throws a HIERARCHY_REQUEST_ERR   *******
    //  doc->removeChild(docElements->item(9));//!! Throws a NOT_FOUND_ERR  ********

    docFragment = document->createDocumentFragment();
    //Tests removeChild and stores removed branch for tree reconstruction
    docFragment->appendChild(docElements->item(1)->removeChild(docElements->item(9)));
    docFragment2 = document->createDocumentFragment();
    //Tests removeChild and stores removed branch for tree reconstruction
    docFragment2->appendChild(docElements->item(1)->removeChild(docElements->item(2)));
    docSize = docElements->getLength();
    for (i = 0; i < docSize; i++)
    {
        IDOM_Node*  n = (IDOM_Node*) docElements->item(i);
        XMLString::transcode(newElementNames[i], tempStr, 3999);
        if (XMLString::compareString(tempStr, n->getNodeName()))
        {
            printf("Comparison of new document's elements failed at element number %d at line %i \n", i, __LINE__);
            OK = false;
            break;
        }
    }
    docElements->item(1)->insertBefore(docFragment, 0); //Reattaches removed branch to restore tree to the original
                                                // AH Revist.  Note: insertBefore should be able to take
                                                //   a 0 as its second parameter.
    docElements->item(1)->insertBefore(docFragment2, docElements->item(2)); //Reattaches removed branch to restore tree to the original

    //  printf(docElements->item(2)->getNodeName());

    docSize = docElements->getLength();
    for (i = 0; i < docSize; i++)
    {
        IDOM_Node*  n = (IDOM_Node*) docElements->item(i);
        XMLString::transcode(elementNames[i], tempStr, 3999);
        if (XMLString::compareString(tempStr, n->getNodeName()))
        {
            printf("Comparison of restored document's elements failed at element number %d at line %i \n", i, __LINE__);
            OK = false;
            break;
        }
    }

    IDTest tests;


//  IDOM_Document* z = tests.createDocument();
//  tests.docBuilder(z, "z");

//!! Throws WRONG_DOCUMENT_ERR **********
//  EXCEPTIONSTEST(z->appendChild(
    //  z->appendChild(d.createComment("Test doc d comment"));// Tries to append z document with document d comment
    //  d->getDocumentElement()->appendChild(z.createElement("newZdocElement"));// Tries to append d document with document z IDOM_Element
    //  d->getLastChild()->getLastChild()->insertBefore(z.createElement("newZdocElement"),d->getLastChild()->getLastChild()->getFirstChild());// Tries to insert into d document with document z IDOM_Element
    //  d->replaceChild(z.createElement("newZdocElement"),d->getLastChild()->getLastChild()->getFirstChild());  // Tries to replace in d document with document z IDOM_Element

    //  doc->setNodeValue("This shouldn't work");//!! Throws a NO_MODIFICATION_ALLOWED_ERR ********

    node = document;
    node2 = document->cloneNode(true);
    result = treeCompare(node, node2); // Deep clone test comparison of document cloneNode
    if (!result)
    {
        printf("Warning!!! Deep clone of the document failed!\n");
        OK = false;
    }

    // Deep clone test comparison is also in testNode


// For debugging*****       printf("All IDOM_Document* method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The IDOM_Document* method calls listed above failed, all others worked correctly.*****\n");
//  printf("\n");
    return OK;
};



/**
 * This method tests IDOM_DocumentFragment* methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 *
 *
 ********This really isn't needed, only exists to throw NO_MODIFICATION_ALLOWED_ERR ********
 */
bool IDTest::testDocumentFragment(IDOM_Document* document)
{
    bool OK = true;
// For debugging*****   printf("\n          testDocumentFragment's outputs:\n");
    IDOM_DocumentFragment* testDocFragment = document->createDocumentFragment();

    //  testDocFragment->setNodeValue("This is a document fragment!");//!! Throws a NO_MODIFICATION_ALLOWED_ERR ********

// For debugging*****       printf("All IDOM_DocumentFragment* method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The IDOM_DocumentFragment* method calls listed above failed, all others worked correctly.*****\n");
//  printf("\n");
    return OK;
};



/**
 * This method tests IDOM_DocumentType* methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testDocumentType(IDOM_Document* document)
{
    IDTest test;
    IDOM_DocumentType* docType, *holdDocType;
    IDOM_NamedNodeMap* docNotationMap;
    IDOM_Node* node, *node2;
    bool OK = true;
// For debugging*****   printf("\n          testDocumentType's outputs:\n");
    XMLString::transcode("TestDocument", tempStr, 3999);
    IDOM_DocumentType* newDocumentType =  test.createDocumentType(document, tempStr);
    node = document->getFirstChild(); // node gets doc's docType node
    node2 = node->cloneNode(true);
    // Check nodes for equality, both their name and value or lack thereof
    if (!(!XMLString::compareString(node->getNodeName(), node2->getNodeName()) &&        // Compares node names for equality
          (node->getNodeValue() != 0 && node2->getNodeValue() != 0)     // Checks to make sure each node has a value node
        ? !XMLString::compareString(node->getNodeValue(), node2->getNodeValue())         // If both have value nodes test those value nodes for equality
        : (node->getNodeValue() == 0 && node2->getNodeValue() == 0)))   // If one node doesn't have a value node make sure both don't
    {
        fprintf(stderr, "'cloneNode' did not clone the IDOM_DocumentType* node correctly\n");
        OK = false;
    }
     // Deep clone test comparison is in testNode & testDocument

    IDOM_Node*   abc9 = document->getFirstChild();
    docType = (IDOM_DocumentType*) abc9;

    XMLString::transcode("ourNotationNode", tempStr, 3999);
    docNotationMap = docType->getNotations();
    if (XMLString::compareString(tempStr, docNotationMap->item(0)->getNodeName()))
    {
        printf("Warning!!! IDOM_DocumentType's 'getNotations' failed!\n");
        OK = false;
    }
    //  doc->appendChild(newDocumentTypeImpl);//!! Throws a HIERARCHY_REQUEST_ERR    *******
    IDOM_Node*  abc10 = document->removeChild(document->getFirstChild()); //Tests removeChild and stores removed branch for tree reconstruction
    holdDocType = (IDOM_DocumentType*) abc10;
    document->insertBefore(newDocumentType, document->getDocumentElement());
    //** Other aspects of insertBefore are tested in docBuilder through appendChild*

    document->removeChild(document->getFirstChild()); //Removes newDocumentType for tree restoral
    document->insertBefore(holdDocType, document->getFirstChild()); //Reattaches removed branch to restore tree to the original


// For debugging*****       printf("All IDOM_DocumentType* method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The IDOM_DocumentType* method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
    return OK;
};



/**
 * @param document org.w3c.dom.IDOM_Document
 */
bool IDTest::testIDOMerrors(IDOM_Document* document) {
    bool OK = true;

    IDTest tests;

    EXCEPTIONSTEST(document->appendChild(testElementNode), IDOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 201 );
    EXCEPTIONSTEST(testTextNode->appendChild(testTextNode), IDOM_DOMException::HIERARCHY_REQUEST_ERR, OK, 202 );
    return OK;
};



/**
 * This method tests IDOM_IDOMImplementation methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testIDOMImplementation(IDOM_Document* document)
{

    IDOM_DOMImplementation* implementation;
    bool result = false;
    bool OK = true;
// For debugging*****   printf("\n          testIDOMImplementation's outputs:\n");
    implementation = document->getImplementation(); //Uses getIDOMImplementation to obtain implementation

    XMLString::transcode("XML", tempStr, 3999);
    XMLString::transcode("1.0", tempStr2, 3999);
    result = implementation->hasFeature(tempStr, tempStr2);
    if(!result)
    {
        fprintf(stderr, "Warning!!! IDOM_IDOMImplementation's 'hasFeature' that should be 'true' failed!");
        OK = false;
    }

    XMLString::transcode("HTML", tempStr, 3999);
    XMLString::transcode("4.0", tempStr2, 3999);
    result = implementation->hasFeature(tempStr, tempStr2);
    if(result)
    {
        fprintf(stderr, "Warning!!! IDOM_IDOMImplementation's 'hasFeature' that should be 'false' failed!");
        OK = false;
    }


// For debugging*****       printf("All IDOM_IDOMImplementation method calls worked correctly.\n");
    if (!OK)
        fprintf(stderr, "\n*****The IDOM_IDOMImplementation method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
    return OK;
};



/**
 * This method tests IDOM_Element* methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testElement(IDOM_Document* document)
{
    IDOM_Attr* attributeNode, *newAttributeNode;
    IDOM_Element* element, *element2;
    IDOM_Node* node, *node2;

    const char* attributeCompare[] = {"AnotherFirstElementAttribute", "dFirstElement", "testAttribute"};
    const char* elementNames[] =  {"dFirstElement", "dTestBody", "dBodyLevel21","dBodyLevel31","dBodyLevel32",
                   "dBodyLevel22","dBodyLevel33","dBodyLevel34","dBodyLevel23","dBodyLevel24"};
    const char* textCompare[] = {"dBodyLevel31'sChildTextNode11",
                                "dBodyLevel31'sChildTextNode12",
                                "dBodyLevel31'sChildTextNode13"};

    IDOM_NamedNodeMap* nodeMap;
    bool OK = true;
// For debugging*****   printf("\n          testElement's outputs:\n");
    node = document->getDocumentElement(); // node gets doc's firstElement
    node2 = node->cloneNode(true);
    // Check nodes for equality, both their name and value or lack thereof
    if (!(!XMLString::compareString(node->getNodeName(), node2->getNodeName()) &&        // Compares node names for equality
          (node->getNodeValue() != 0 && node2->getNodeValue() != 0)     // Checks to make sure each node has a value node
        ? !XMLString::compareString(node->getNodeValue(), node2->getNodeValue())         // If both have value nodes test those value nodes for equality
        : (node->getNodeValue() == 0 && node2->getNodeValue() == 0)))   // If one node doesn't have a value node make sure both don't
    {
        fprintf(stderr, "'cloneNode' did not clone the IDOM_Element* node correctly.\n");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument

    element = document->getDocumentElement(); // element gets doc's firstElement

    XMLString::copyString(tempStr, document->getNodeValue());
    XMLString::transcode("'s test attribute", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    if (XMLString::compareString(XMLUni::fgZeroLenString, element->getAttribute(tempStr)))
    {
        fprintf(stderr, "Warning!!! IDOM_Element's 'getAttribute' failed!\n");
        OK = false;
    }

    XMLString::copyString(tempStr, document->getNodeValue());
    XMLString::transcode("FirstElement", tempStr2, 3999);
    XMLString::catString(tempStr, tempStr2);
    attributeNode = element->getAttributeNode(tempStr);
    if(! (attributeNode == 0))
    {
        fprintf(stderr, "Warning!!! IDOM_Element's 'getAttributeNode' failed! It should have returned '0' here!\n");
        OK = false;
    }


    XMLString::transcode("AnotherFirstElementAttribute", tempStr, 3999);
    newAttributeNode = document->createAttribute(tempStr);

    XMLString::transcode("A new attribute which helps test calls in IDOM_Element", tempStr, 3999);
    newAttributeNode->setValue(tempStr);
    // This test is incorrect.  It assumes that there is a defined ordering of the entries
    //  in a nodeMap, but there is no ordering required.
#ifdef TheFollowingCheckIsInvalid
    element->setAttributeNode(newAttributeNode);
    nodeMap = element->getAttributes();
    int size = nodeMap->getLength();
    int k;
    for (k = 0; k < size; k++)
    {
        IDOM_Node*  n = (IDOM_Node) nodeMap->item(k);
        XMLString::transcode(attributeCompare[k], tempStr, 3999);
        if (XMLString::compareString(tempStr, n->getNodeName())))
        {
            printf("Warning!!! Comparison of firstElement's attributes failed at line %i.\n", __LINE__);
            printf("   This failure can be a result of IDOM_Element's 'setValue' and/or 'setAttributeNode' and/or 'getAttributes' failing.\n");
            OK = false;
            break;
        }
    //  printf("firstElement's attribute number " + k + " : " + n->getNodeName());
    }
#endif

    nodeMap = element->getAttributes();
    int size = nodeMap->getLength();
    if (size != 2)
    {
        printf("IDOM_Element* Tests Failure 001\n");
        OK = false;
    };
    element->setAttributeNode(newAttributeNode);
    size = nodeMap->getLength();
    if (size != 3)
    {
        printf("IDOM_Element* Tests Failure 002\n");
        OK = false;
    };

    // Fetch the newly added attribute node back out of from the named node map,
    //  and check that we are returned the same node that we put in->
    XMLString::transcode("AnotherFirstElementAttribute", tempStr, 3999);
    IDOM_Node*  abc12 = nodeMap->getNamedItem(tempStr);
    IDOM_Attr* fetchedAttr = (IDOM_Attr*) abc12;
    if (fetchedAttr != newAttributeNode)
    {
        printf("IDOM_Element* Tests Failure 003\n");
        OK = false;
    };

    // Fetch the newly added attribute back out directly from the element itself.
    XMLString::transcode("AnotherFirstElementAttribute", tempStr, 3999);
    fetchedAttr = element->getAttributeNode(tempStr);
    if (fetchedAttr != newAttributeNode)
    {
        printf("IDOM_Element* Tests Failure 004\n");
        OK = false;
    };



    XMLString::transcode("*",tempStr, 3999);
    IDOM_NodeList* docElements = document->getElementsByTagName(tempStr);
    int docSize = docElements->getLength();
    int i;
    for (i = 0; i < docSize; i++)
    {
        IDOM_Node*  n = docElements->item(i);
        XMLString::transcode(elementNames[i], tempStr, 3999);
        if (XMLString::compareString(tempStr, n->getNodeName()))
        {
            printf("Warning!!! Comparison of IDOM_Element's 'getElementsByTagName' "
                            "and/or 'item' failed at element number %d at line %i \n", i, __LINE__ );
            printf("\n");
            OK = false;
            break;
        }
    //  printf("docElement's number " + i + " is: " + n->getNodeName());
    }
    XMLString::transcode("dBodyLevel21", tempStr, 3999);
    IDOM_Node*  abc15 = document->getElementsByTagName(tempStr)->item(0); // element gets IDOM_Element* test BodyLevel21
    element = (IDOM_Element*) abc15;

    XMLString::transcode("dBodyLevel31", tempStr, 3999);
    IDOM_Node*  abc16 = document->getElementsByTagName(tempStr)->item(0); // element2 gets IDOM_Element* test BodyLevel31
    element2 = (IDOM_Element*) abc16;
    IDOM_NodeList* text = ((IDOM_Node*  &) element2)->getChildNodes();
    int textSize = text->getLength();
    int j;
    static bool firstTime = true;
    if (firstTime)
    {
        firstTime = false;      // Temporary fix.  Subsequent tests alter the doc, causing
                                //   this test to fail on all but the first time through.
        for (j = 0; j < textSize; j++)
        {
            IDOM_Node*  n = text->item(j);
            XMLString::transcode(textCompare[j], tempStr, 3999);
            if (XMLString::compareString(tempStr, n->getNodeValue()))
            {
                printf("Warning!!! Comparison of original text nodes via IDOM_Node*  'getChildNodes' & IDOM_NodeList 'item'\n"
                    "     failed at text node: #%d at line %i \n     ", j, __LINE__ );
                OK = false;
                break;
            }
            //  printf("IDOM_Element* testBodyLevel31's child text node " + j + " is: " + n->getNodeValue());
        }
    }

    element = document->getDocumentElement(); // element gets doc's firstElement
    element->normalize();        // Concatenates all adjacent text nodes in this element's subtree
    IDOM_NodeList* text2 = ((IDOM_Node*) element2)->getChildNodes();
    XMLString::transcode("dBodyLevel31'sChildTextNode11dBodyLevel31'sChildTextNode12dBodyLevel31'sChildTextNode13", tempStr, 3999);
    IDOM_Node*  n = text2->item(0);
    if (XMLString::compareString(tempStr, n->getNodeValue()))
    {
        printf("Warning!!! Comparison of concatenated text nodes created by IDOM_Element's 'normalize' failed!\n");
        OK = false;
    }

    XMLString::transcode("FirstElementLastAttribute", tempStr, 3999);
    XMLString::transcode("More attribute stuff for firstElement!!", tempStr2, 3999);
    element->setAttribute(tempStr, tempStr2);

    XMLString::transcode("FirstElementLastAttribute", tempStr, 3999);
    element->removeAttribute(tempStr);
    element->removeAttributeNode(newAttributeNode);

    //  doc->getLastChild()->setNodeValue("This shouldn't work");//!! Throws a NO_MODIFICATION_ALLOWED_ERR***

// For debugging*****       printf("All IDOM_Element* method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The IDOM_Element* method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
    return OK;
};



/**
 * This method tests IDOM_Entity* methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testEntity(IDOM_Document* document)
{
    IDOM_Entity* entity;
    IDOM_Node* node, *node2;
    bool OK = true;
// For debugging*****   printf("\n          testEntity's outputs:\n\n");
    XMLString::transcode("ourEntityNode", tempStr, 3999);
    IDOM_Node*  abc20 = document->getDoctype()->getEntities()->getNamedItem(tempStr);
    entity = (IDOM_Entity*) abc20;
    node = entity;
    node2 = entity->cloneNode(true);
    // Check nodes for equality, both their name and value or lack thereof
    if (!(!XMLString::compareString(node->getNodeName(), node2->getNodeName()) &&        // Compares node names for equality
          (node->getNodeValue() != 0 && node2->getNodeValue() != 0)     // Checks to make sure each node has a value node
        ? !XMLString::compareString(node->getNodeValue(), node2->getNodeValue())         // If both have value nodes test those value nodes for equality
        : (node->getNodeValue() == 0 && node2->getNodeValue() == 0)))   // If one node doesn't have a value node make sure both don't
    {
        fprintf(stderr, "'cloneNode' did not clone the IDOM_Entity* node correctly");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument

// For debugging*****       printf("All IDOM_Entity* method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The IDOM_Entity* method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
    return OK;
};


/**
 * This method tests IDOM_EntityReference* methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testEntityReference(IDOM_Document* document)
{
    IDOM_EntityReference* entityReference;
    IDOM_Node* node, *node2;
    bool OK = true;
// For debugging*****   printf("\n          testEntityReference's outputs:\n");
    IDOM_Node*  abc30 = document->getLastChild()->getLastChild()->getLastChild()->getFirstChild();
    entityReference = (IDOM_EntityReference*) abc30;
    node = entityReference;
    node2 = node->cloneNode(true);
    // Check nodes for equality, both their name and value or lack thereof
    if (!(!XMLString::compareString(node->getNodeName(), node2->getNodeName()) &&        // Compares node names for equality
          (node->getNodeValue() != 0 && node2->getNodeValue() != 0)     // Checks to make sure each node has a value node
        ? !XMLString::compareString(node->getNodeValue(), node2->getNodeValue())         // If both have value nodes test those value nodes for equality
        : (node->getNodeValue() == 0 && node2->getNodeValue() == 0)))   // If one node doesn't have a value node make sure both don't
    {
        fprintf(stderr, "'cloneNode' did not clone the IDOM_EntityReference* node correctly\n");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument

    //  entityReference->setNodeValue("This shouldn't work");//!! Throws a NO_MODIFICATION_ALLOWED_ERR ********

// For debugging*****       printf("All IDOM_EntityReference* method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The IDOM_EntityReference* method calls listed above failed, all others worked correctly.*****\n");
//  printf("\n");
    return OK;
};



/**
 * This method tests IDOM_Node*  methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 *
 *
 ********* This is only for a test of cloneNode "deep"*******
 ********* And for error tests*********
 */
bool IDTest::testNode(IDOM_Document* document)
{
    IDOM_Node* node, *node2;
    bool result;
    bool OK = true;
// For debugging*****   printf("\n          testNode's outputs:\n");
    node = document->getDocumentElement();
    node2 = node->cloneNode(true);
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
    node = document->getDocumentElement();
    node2 = node->getFirstChild();
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

// For debugging*****       printf("All IDOM_Node*  method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The IDOM_Node*  method calls listed above failed, all others worked correctly.*****\n");
//  printf("\n");
    return OK;
};



/**
 * This method tests IDOM_Notation* methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testNotation(IDOM_Document* document)
{
    IDOM_Node* node, *node2;
    IDOM_Notation* notation;
    bool OK = true;
// For debugging*****   printf("\n          testNotation's outputs:\n");
    XMLString::transcode("ourNotationNode", tempStr, 3999);
	IDOM_Node*  abc40 = document->getDoctype()->getNotations()->getNamedItem(tempStr);
    notation = (IDOM_Notation*) abc40;
    node = notation;
    node2 = notation->cloneNode(true);//*****?
    // Check nodes for equality, both their name and value or lack thereof
    if (!(!XMLString::compareString(node->getNodeName(), node2->getNodeName()) &&        // Compares node names for equality
          (node->getNodeValue() != 0 && node2->getNodeValue() != 0)     // Checks to make sure each node has a value node
        ? !XMLString::compareString(node->getNodeValue(), node2->getNodeValue())         // If both have value nodes test those value nodes for equality
        : (node->getNodeValue() == 0 && node2->getNodeValue() == 0)))   // If one node doesn't have a value node make sure both don't
    {
        fprintf(stderr, "'cloneNode' did not clone the IDOM_Notation* node correctly");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument

    //  notation->setNodeValue("This shouldn't work");//!! Throws a NO_MODIFICATION_ALLOWED_ERR ********

// For debugging*****       printf("All IDOM_Notation* method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The IDOM_Notation* method calls listed above failed, all others worked correctly.*****\n");
//  printf("");
    return OK;
};



/**
 * This method tests IDOM_ProcessingInstruction* methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testPI(IDOM_Document* document)
{
    IDOM_ProcessingInstruction* pI, *pI2;
    bool OK = true;
// For debugging*****   printf("\n          testPI's outputs:\n");
	IDOM_Node*   abc50 = document->getDocumentElement()->getFirstChild();// Get doc's IDOM_ProcessingInstruction
    pI  = (IDOM_ProcessingInstruction*) abc50;
	IDOM_Node*   abc51 = pI->cloneNode(true);//*****?
    pI2 = (IDOM_ProcessingInstruction*) abc51;
    // Check nodes for equality, both their name and value or lack thereof
    if (!(!XMLString::compareString(pI->getNodeName(), pI2->getNodeName()) &&         // Compares node names for equality
         (pI->getNodeValue() != 0 && pI2->getNodeValue() != 0)  // Checks to make sure each node has a value node
        ? !XMLString::compareString(pI->getNodeValue(), pI2->getNodeValue())      // If both have value nodes test those value nodes for equality
        :(pI->getNodeValue() == 0 && pI2->getNodeValue() == 0)))// If one node doesn't have a value node make sure both don't
    {
        printf("'cloneNode' did not clone the IDOM_Entity* node correctly\n");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument
    // compare = "This is [#document: 0]'s processing instruction";  // AH Revisit.  Where id
    //                  this ": 0]" stuff come from in the Java version??  I don' think that it is right.
    XMLString::transcode("This is #document's processing instruction", tempStr, 3999);
    if (XMLString::compareString(tempStr, pI->getData()))
    {
        printf("Warning!!! PI's 'getData' failed!\n");
        OK = false;
    }

    XMLString::transcode("PI's reset data", tempStr, 3999);
    pI->setData(tempStr);
    if (XMLString::compareString(tempStr, pI->getData()))
    {
        printf("Warning!!! PI's 'setData' failed!\n");
        OK = false;
    }
    XMLString::transcode("dTargetProcessorChannel", tempStr, 3999);
    if (XMLString::compareString(tempStr, pI->getTarget()))
    {
        printf("Warning!!! PI's 'getTarget' failed!\n");
        OK = false;
    }


    // Restore original PI data.
    XMLString::transcode("This is #document's processing instruction", tempStr, 3999);
    pI->setData(tempStr);

// For debugging*****       printf("All PI method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The PI method calls listed above failed, all others worked correctly.*****\n");

//  printf("\n");
    return OK;
};



/**
 * This method tests IDOM_Text* methods for the XML IDOM implementation
 * @param document org.w3c.dom.IDOM_Document
 *
 */
bool IDTest::testText(IDOM_Document* document)
{
    IDOM_Node* node, *node2;
    IDOM_Text* text;
    bool OK = true;
// For debugging*****   printf("\n          testText's outputs:\n");
    XMLString::transcode("dBodyLevel31", tempStr, 3999);
    IDOM_Node*  abc70 = document->getDocumentElement()->getElementsByTagName(tempStr)->item(0);
    IDOM_Element* elem = (IDOM_Element*) abc70;
    node = elem->getFirstChild(); // charData gets textNode11
    text = (IDOM_Text*) node;
    node2 = node->cloneNode(true);//*****?
    // Check nodes for equality, both their name and value or lack thereof
    if (!(!XMLString::compareString(node->getNodeName(), node2->getNodeName()) &&        // Compares node names for equality
          (node->getNodeValue() != 0 && node2->getNodeValue() != 0)     // Checks to make sure each node has a value node
        ? !XMLString::compareString(node->getNodeValue(), node2->getNodeValue())         // If both have value nodes test those value nodes for equality
        : (node->getNodeValue() == 0 && node2->getNodeValue() == 0)))   // If one node doesn't have a value node make sure both don't
    {
        fprintf(stderr, "'cloneNode' did not clone the IDOM_Text* node correctly\n");
        OK = false;
    }
    // Deep clone test comparison is in testNode & testDocument

    text->splitText(25);
    // Three original text nodes were concatenated by 'normalize' in testElement
    XMLString::transcode("dBodyLevel31'sChildTextNo", tempStr, 3999);
    if (XMLString::compareString(tempStr, text->getNodeValue()))
        {
            printf("First part of IDOM_Text's split text failed!\n" );
            OK = false;
        }
    // Three original text nodes were concatenated by 'normalize' in testElement
    XMLString::transcode("de11dBodyLevel31'sChildTextNode12dBodyLevel31'sChildTextNode13", tempStr, 3999);
    if (XMLString::compareString(tempStr, text->getNextSibling()->getNodeValue()))
        {
            printf("The second part of IDOM_Text's split text failed!\n") ;
            OK = false;
        }

    // Re-normalize the text nodes under elem, so that this test can be rerun->
    elem->normalize();


//************************************************* ERROR TESTS
    IDTest tests;
    //!! Throws INDEX_SIZE_ERR ********************
    //  text.splitText(-1);
    //  text.splitText(100);

// For debugging*****       printf("All IDOM_Text* method calls worked correctly.\n");
    if (!OK)
        printf("\n*****The IDOM_Text* method calls listed above failed, all others worked correctly.*****\n");

//  printf("\n");
    return OK;
};




/**
 *
 * @param node org.w3c.dom.IDOM_Node
 * @param node2 org.w3c.dom.IDOM_Node
 *
 */
bool IDTest::treeCompare(IDOM_Node* node, IDOM_Node* node2)
{
    bool answer = true;

    IDOM_Node*  kid, *kid2;         // Check the subtree for equality
    kid = node->getFirstChild();
    kid2 = node2->getFirstChild();
    if (kid && kid2)
    {
        answer = treeCompare(kid, kid2);
        if (!answer)
            return answer;
        else
            if (kid->getNextSibling() && kid2->getNextSibling())
            {
                while (kid->getNextSibling() && kid2->getNextSibling())
                {
                    answer = treeCompare(kid->getNextSibling(), kid2->getNextSibling());
                    if (!answer)
                        return answer;
                    else
                    {
                        kid = kid->getNextSibling();
                        kid2 = kid2->getNextSibling();
                    }
                }
            } else
                if (!(!kid->getNextSibling() && !kid2->getNextSibling()))
                {
                    return false;
                }
    } else
        if (kid != kid2)
        {
            // One or the other of (kid1, kid2) is 0, but not both.
            return false;
        }

    if (XMLString::compareString(node->getNodeName(), node2->getNodeName()))
        return false;
    if (node->getNodeValue()==0 && node2->getNodeValue()!=0)
        return false;
    if (node->getNodeValue()!=0 && node2->getNodeValue()==0)
        return false;
    if (XMLString::compareString(node->getNodeValue(), node2->getNodeValue()))
        return false;

    return answer;
};


