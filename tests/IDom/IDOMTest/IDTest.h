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
 * Revision 1.1  2001/08/09 19:28:47  tng
 * Port test case DOMTest to  IDOMTest
 *
 */


/**
 * This class tests methods for XML IDOM implementation
 *
 * IDOMException errors are tested by calls to IDOMExceptionsTest from: Main, docBuilder...
 *
 */

#include <idom/IDOM.hpp>

//  define null for compatibility with original Java source code.
#define null 0

class IDTest {
public:
	static IDOM_Element           *testElementNode;
	static IDOM_Attr              *testAttributeNode;
	static IDOM_Text              *testTextNode;
	static IDOM_CDATASection      *testCDATASectionNode;
	static IDOM_EntityReference   *testEntityReferenceNode;
	static IDOM_Entity            *testEntityNode;
	static IDOM_ProcessingInstruction *testProcessingInstructionNode;
	static IDOM_Comment           *testCommentNode;
	static IDOM_Document          *testDocumentNode;
	static IDOM_DocumentType      *testDocumentTypeNode;
	static IDOM_DocumentFragment  *testDocumentFragmentNode;
	static IDOM_Notation          *testNotationNode;


IDTest();

IDOM_Document* createDocument();
IDOM_DocumentType* createDocumentType(IDOM_Document* doc, XMLCh* name);
IDOM_Entity* createEntity(IDOM_Document* doc, XMLCh* name);
IDOM_Notation* createNotation(IDOM_Document* doc, XMLCh* name);
void docBuilder(IDOM_Document* document, XMLCh* name);

void findTestNodes(IDOM_Document* document);
void findTestNodes(IDOM_Node* node);


void testAttr(IDOM_Document* document);
void testCDATASection(IDOM_Document* document);
void testCharacterData(IDOM_Document* document);
void testChildNodeList(IDOM_Document* document);
void testComment(IDOM_Document* document);
void testDeepNodeList(IDOM_Document* document);

/**
 **** ALL IDOM_Document create methods are run in docBuilder except createAttribute which is in testAttribute**
 */
void testDocument(IDOM_Document* document);


/**
 ********This really isn't needed, only exists to throw NO_MODIFICATION_ALLOWED_ERR ********
 */
void testDocumentFragment(IDOM_Document* document);

void testDocumentType(IDOM_Document* document);
void testIDOMerrors(IDOM_Document* document);
void testIDOMImplementation(IDOM_Document* document);
void testElement(IDOM_Document* document);
void testEntity(IDOM_Document* document);
void testEntityReference(IDOM_Document* document);


/**
 ********* This is only for a test of cloneNode "deep"*******
 ********* And for error tests*********
 */
void testNode(IDOM_Document* document);

void testNotation(IDOM_Document* document);
void testPI(IDOM_Document* document);
void testText(IDOM_Document* document);
bool treeCompare(IDOM_Node* node, IDOM_Node* node2);

};
