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

/*
 * $Log$
 * Revision 1.7  2002/06/12 18:31:17  tng
 * DOM L3: test the DOMUserDataHandler and set/getUserData
 *
 * Revision 1.6  2002/05/21 18:50:16  tng
 * Test case update: modify to use the latest DOM interface
 *
 * Revision 1.3  2002/03/14 21:59:29  tng
 * Run methods test[NodeType] in the IDOMTest and other fixes.
 *
 * Revision 1.2  2002/02/01 22:44:24  peiyongz
 * sane_include
 *
 * Revision 1.1  2001/08/09 19:28:47  tng
 * Port test case DOMTest to  IDOMTest
 *
 */


/**
 * This class tests methods for XML DOM implementation
 *
 * DOMException errors are tested by calls to DOMExceptionsTest from: Main, docBuilder...
 *
 */

#include <xercesc/dom/DOM.hpp>

//  define null for compatibility with original Java source code.
#define null 0

class DOMTest {
public:
	static DOMElement           *testElementNode;
	static DOMAttr              *testAttributeNode;
	static DOMText              *testTextNode;
	static DOMCDATASection      *testCDATASectionNode;
	static DOMEntityReference   *testEntityReferenceNode;
	static DOMEntity            *testEntityNode;
	static DOMProcessingInstruction *testProcessingInstructionNode;
	static DOMComment           *testCommentNode;
	static DOMDocument          *testDocumentNode;
	static DOMDocumentType      *testDocumentTypeNode;
	static DOMDocumentFragment  *testDocumentFragmentNode;
	static DOMNotation          *testNotationNode;


DOMTest();

DOMDocument* createDocument();
DOMDocumentType* createDocumentType(DOMDocument* doc, XMLCh* name);
DOMEntity* createEntity(DOMDocument* doc, XMLCh* name);
DOMNotation* createNotation(DOMDocument* doc, XMLCh* name);
bool docBuilder(DOMDocument* document, XMLCh* name);

void findTestNodes(DOMDocument* document);
void findTestNodes(DOMNode* node);


bool testAttr(DOMDocument* document);
bool testCDATASection(DOMDocument* document);
bool testCharacterData(DOMDocument* document);
bool testChildNodeList(DOMDocument* document);
bool testComment(DOMDocument* document);
bool testDeepNodeList(DOMDocument* document);

/**
 **** ALL DOMDocument create methods are run in docBuilder except createAttribute which is in testAttribute**
 */
bool testDocument(DOMDocument* document);


/**
 ********This really isn't needed, only exists to throw NO_MODIFICATION_ALLOWED_ERR ********
 */
bool testDocumentFragment(DOMDocument* document);

bool testDocumentType(DOMDocument* document);
bool testDOMerrors(DOMDocument* document);
bool testDOMImplementation(DOMDocument* document);
bool testElement(DOMDocument* document);
bool testEntity(DOMDocument* document);
bool testEntityReference(DOMDocument* document);


/**
 ********* This is only for a test of cloneNode "deep"*******
 ********* And for error tests*********
 */
bool testNode(DOMDocument* document);

bool testNotation(DOMDocument* document);
bool testPI(DOMDocument* document);
bool testText(DOMDocument* document);
bool treeCompare(DOMNode* node, DOMNode* node2);

};

class myUserDataHandler : public DOMUserDataHandler {
private:
    DOMOperationType currentType;
    XMLCh* currentKey;
    void* currentData;
    DOMNode* currentSrc;
    DOMNode* currentDst;

public:
    myUserDataHandler() :
      currentKey(0),
      currentData(0),
      currentSrc(0),
      currentDst(0) {};

    virtual void handle(DOMOperationType operation,
                const XMLCh* const key,
                void* data,
                const DOMNode* src,
                const DOMNode* dst)
    {
        currentType = operation;
        currentKey = (XMLCh*) key;
        currentData = data;
        currentSrc = (DOMNode*) src;
        currentDst = (DOMNode*) dst;
    };

    DOMOperationType getCurrentType() {
        return currentType;
    };
    XMLCh* getCurrentKey() const {
        return currentKey;
    };
    void* getCurrentData() const {
        return currentData;
    };
    DOMNode* getCurrentSrc() const {
        return currentSrc;
    };
    DOMNode* getCurrentDst() const {
        return currentDst;
    };

};


