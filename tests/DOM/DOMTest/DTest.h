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
 * Revision 1.4  2002/02/01 22:43:17  peiyongz
 * sane_include
 *
 * Revision 1.3  2000/03/02 19:55:45  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:36  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:02:38  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:42:24  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


/**
 * This class tests methods for XML DOM implementation
 * 
 * DOMException errors are tested by calls to DOMExceptionsTest from: Main, docBuilder...
 *
 */

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DomMemDebug.hpp>

//  define null for compatibility with original Java source code.
#define null 0

class DTest {
public:
	static DOM_Element 				testElementNode;
	static DOM_Attr 				testAttributeNode;
	static DOM_Text 				testTextNode;
	static DOM_CDATASection 		testCDATASectionNode;
	static DOM_EntityReference 		testEntityReferenceNode;
	static DOM_Entity 				testEntityNode;
	static DOM_ProcessingInstruction testProcessingInstructionNode;
	static DOM_Comment 				testCommentNode;
	static DOM_Document 			testDocumentNode;
	static DOM_DocumentType 		testDocumentTypeNode;
	static DOM_DocumentFragment 	testDocumentFragmentNode;
	static DOM_Notation 			testNotationNode;

    
DTest();


/**
 * version 3.0 01/25/99
 *  
 * @return DOM_Document
 *
 */
DOM_Document createDocument();


/**
 * version 3.0 01/25/99
 * 
 * @return DOM_DocumentType
 * @param name DOMString
 *
 */
DOM_DocumentType createDocumentType(DOM_Document doc, DOMString name);


/**
 * version 3.0 01/25/99
 *  
 * @return org.w3c.dom.DOM_Entity
 * @param doc org.w3c.dom.DOM_Document
 * @param name DOMString
 *
 */
DOM_Entity createEntity(DOM_Document doc, DOMString name);


/**
 * version 3.0 01/25/99
 * 
 * @return org.w3c.dom.DOM_Notation
 * @param doc org.w3c.dom.DOM_Document
 * @param name DOMString
 *
 */
DOM_Notation createNotation(DOM_Document doc, DOMString name);

/**
 * This method builds test documents for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 * @param name document's name
 * @param type document's type
 *
 */
void docBuilder(DOM_Document document, DOMString name);


/**
 * version 3.0 01/25/99
 * 
 * @return boolean
 * @param node java.lang.Object
 * @param mNameIndex int
 * @param signatureIndex int
 * @param parameters java.lang.Object[]
 * @param code short
 *
 */
//static bool DOMExceptionsTest(Object node, DOMString methodName, Class[] methodSignature, Object[] parameters, short code) {


/**
 * @param document org.w3c.dom.DOM_Document
 */
void findTestNodes(DOM_Document document);

/**
 * @param document org.w3c.dom.DOM_Document
 */
void findTestNodes(DOM_Node node);


/**
 * This method tests DOM_Attr methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testAttr(DOM_Document document);


/**
 * This method tests DOM_CDATASection methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testCDATASection(DOM_Document document);


/**
 * This method tests CharacterData methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testCharacterData(DOM_Document document);


/**
 * This method tests ChildNodeList methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testChildNodeList(DOM_Document document);


/**
 * This method tests DOM_Comment methods for the XML DOM implementation
 * version 1.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testComment(DOM_Document document);



/**
 * This method tests DeepNodeList methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testDeepNodeList(DOM_Document document);

/**
 * This method tests DOM_Document methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 *
 **** ALL DOM_Document create methods are run in docBuilder except createAttribute which is in testAttribute**
 */
void testDocument(DOM_Document document);


/**
 * This method tests DOM_DocumentFragment methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 *
 *
 ********This really isn't needed, only exists to throw NO_MODIFICATION_ALLOWED_ERR ********
 */
void testDocumentFragment(DOM_Document document);


/**
 * This method tests DOM_DocumentType methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testDocumentType(DOM_Document document);

/**
 * @param document org.w3c.dom.DOM_Document
 */
void testDOMerrors(DOM_Document document);



/**
 * This method tests DOMImplementation methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testDOMImplementation(DOM_Document document);


/**
 * This method tests DOM_Element methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testElement(DOM_Document document);


/**
 * This method tests DOM_Entity methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testEntity(DOM_Document document);


/**
 * This method tests DOM_EntityReference methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testEntityReference(DOM_Document document);


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
void testNode(DOM_Document document);


/**
 * This method tests DOM_Notation methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testNotation(DOM_Document document);


/**
 * This method tests DOM_ProcessingInstruction methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testPI(DOM_Document document);


/**
 * This method tests DOM_Text methods for the XML DOM implementation
 * version 2.0 10/12/98
 * @param document org.w3c.dom.DOM_Document
 *
 */
void testText(DOM_Document document);

/**
 * 
 * @param node org.w3c.dom.DOM_Node
 * @param node2 org.w3c.dom.DOM_Node
 *
 */
bool treeCompare(DOM_Node node, DOM_Node node2);

};
