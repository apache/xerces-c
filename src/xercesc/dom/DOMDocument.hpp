#ifndef DOMDocument_HEADER_GUARD_
#define DOMDocument_HEADER_GUARD_

/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2002 The Apache Software Foundation.  All rights
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
 * $Id$
*/

#include <xercesc/util/XercesDefs.hpp>
#include "DOMNode.hpp"

class DOMNodeIteratorImpl;

class DOMDocumentType;
class DOMElement;
class DOMDocumentFragment;
class DOMComment;
class DOMCDATASection;
class DOMProcessingInstruction;
class DOMAttr;
class DOMEntity;
class DOMEntityReference;
class DOMImplementation;
class DOMNodeFilter;
class DOMNodeList;
class DOMNotation;
class DOMText;
class DOMNode;
class DOMNodeIterator;
class DOMTreeWalker;
class DOMRange;


/**
* Class to refer to XML Document nodes in the DOM.
*
* Conceptually, a DOM document node is the root of the document tree, and provides
* the  primary access to the document's data.
* <p>Since elements, text nodes, comments, processing instructions, etc.
* cannot exist outside the context of a <code>Document</code>, the
* <code>Document</code> interface also contains the factory methods needed
* to create these objects.  The <code>Node</code> objects created have a
* <code>ownerDocument</code> attribute which associates them with the
* <code>Document</code> within whose  context they were created.
*/
class CDOM_EXPORT DOMDocument: public DOMNode {

protected:
    /** @name Constructors and assignment operators */
    //@{
    /**
     * The default constructor for DOMDocument creates a null
     * DOMDocument object that refers to no document.  It may subsequently be
     * assigned to refer to an actual Document node.
     *
     * To create a new document, use the DOMImplementation
     *   <code> DOMImplementation::createDocument(). </code>
     *
     */
    DOMDocument() {};

    /**
      * Copy constructor.  Creates a new <code>DOMDocument</code> that refers to the
      * same underlying actual document as the original.
      *
      * @param other The object to be copied
      */
    DOMDocument(const DOMDocument &other) {};
    /**
      * Assignment operator
      *
      * @param other The object to be copied
      */
    DOMDocument & operator = (const DOMDocument &other) {return *this;};


	//@}

public:
  /** @name Destructor */
  //@{
	
    virtual ~DOMDocument() {};

  //@}
  /** @name Factory methods to create new nodes for the Document */
  //@{

    /**
    *  Create a new entity.
    *
    *  Non-standard extension.
    * @param name The name of the entity to instantiate
    *
    */
    virtual DOMEntity     *createEntity(const XMLCh *name) = 0;

    /**
    * Creates an element of the type specified.
    *
    * Note that the instance returned
    * implements the Element interface, so attributes can be specified
    * directly  on the returned object.
    * @param tagName The name of the element type to instantiate.
    * @return A <code>DOMElement</code> that reference the new element.
    * @exception DOMException
    *   INVALID_CHARACTER_ERR: Raised if the specified name contains an
    *   illegal character.
    */
    virtual DOMElement     *createElement(const XMLCh *tagName) = 0;



    /**
    * Creates an empty DocumentFragment object.
    *
    * @return A <code>DOMDocumentFragment</code> that references the newly
    * created document fragment.
    */
    virtual DOMDocumentFragment   *createDocumentFragment() = 0;

    /**
    * Creates a Text node given the specified string.
    *
    * @param data The data for the node.
    * @return A <code>DOMText</code> object that references the newly
    *  created text node.
    */
    virtual DOMText         *createTextNode(const XMLCh *data) = 0;

    /**
    * Creates a Comment node given the specified string.
    *
    * @param data The data for the comment.
    * @return A <code>DOMComment</code> that references the newly
    *  created comment node.
    */
    virtual DOMComment      *createComment(const XMLCh *data) = 0;

    /**
    * Creates a CDATASection node whose value  is the specified
    * string.
    *
    * @param data The data for the <code>DOMCDATASection</code> contents.
    * @return A <code>DOMCDATASection</code> object.
    * @exception DOMException
    *   NOT_SUPPORTED_ERR: Raised if this document is an HTML document.
    */
    virtual DOMCDATASection   *createCDATASection(const XMLCh *data) = 0;

    /**
    *  Create a DocumentType node.  Non-standard extension.
    *
    * @return A <code>DOMDocumentType</code> that references the newly
    *  created DocumentType node.
    *
    */
    virtual DOMDocumentType *createDocumentType(const XMLCh *name) = 0;


    /**
    *  Create a Notation.
    *
    *  Non-standard extension.
    *
    *  @param name The name of the notation to instantiate
    * @return A <code>DOMNotation</code> that references the newly
    *  created Notation node.
    */
    virtual DOMNotation *createNotation(const XMLCh *name) = 0;


    /**
    * Creates a ProcessingInstruction node given the specified
    * name and data strings.
    *
    * @param target The target part of the processing instruction.
    * @param data The data for the node.
    * @return A <code>DOMProcessingInstruction</code> that references the newly
    *  created PI node.
    * @exception DOMException
    *   INVALID_CHARACTER_ERR: Raised if an illegal character is specified.
    */
    virtual DOMProcessingInstruction *createProcessingInstruction(const XMLCh *target,
        const XMLCh *data) = 0;


    /**
     * Creates an Attr of the given name.
     *
     * Note that the
     * <code>Attr</code> instance can then be attached to an Element
     * using the <code>DOMElement::setAttribute()</code> method.
     * @param name The name of the attribute.
     * @return A new <CODE>DOMAttr</CODE>
     *       object with the <CODE>nodeName</CODE> attribute set to
     *       <CODE>name</CODE>, and <CODE>localName</CODE>, <CODE>prefix</CODE>,
     *       and <CODE>namespaceURI</CODE> set to
     *       <CODE>null</CODE>.
     * @exception DOMException
     *   INVALID_CHARACTER_ERR: Raised if the specified name contains an
     *   illegal character.
     */
    virtual DOMAttr     *createAttribute(const XMLCh *name) = 0;


    /**
     * Creates an EntityReference object.
     *
     * @param name The name of the entity to reference.
     * @return A <code>DOMEntityReference</code> that references the newly
     *  created EntityReference node.
     * @exception DOMException
     *   INVALID_CHARACTER_ERR: Raised if the specified name contains an
     *   illegal character.
     */
    virtual DOMEntityReference    *createEntityReference(const XMLCh *name) = 0;


    /**
     * Creates a NodeIterator object.   (DOM2)
     *
     * NodeIterators are used to step through a set of nodes, e.g. the set of nodes in a NodeList, the
     * document subtree governed by a particular node, the results of a query, or any other set of nodes.
     * The set of nodes to be iterated is determined by the implementation of the NodeIterator. DOM Level 2
     * specifies a single NodeIterator implementation for document-order traversal of a document subtree.
     * Instances of these iterators are created by calling <code>DocumentTraversal.createNodeIterator()</code>.
     *
     * To produce a view of the document that has entity references expanded and does not
     * expose the entity reference node itself, use the <code>whatToShow</code> flags to hide the entity
     * reference node and set expandEntityReferences to true when creating the iterator. To
     * produce a view of the document that has entity reference nodes but no entity expansion,
     * use the <code>whatToShow</code> flags to show the entity reference node and set
     * expandEntityReferences to false.
     *
     * @param root The root node of the DOM tree
     * @param whatToShow This attribute determines which node types are presented via the iterator.
     * @param filter The filter used to screen nodes
     * @param entityReferenceExpansion The value of this flag determines whether the children of entity reference nodes are
     *                   visible to the iterator. If false, they will be skipped over.
     */

    virtual DOMNodeIterator *createNodeIterator(DOMNode         *root,
                                                   unsigned long    whatToShow,
                                                   DOMNodeFilter* filter,
                                                   bool             entityReferenceExpansion) = 0;
     /**
     * Creates a TreeWalker object.   (DOM2)
     *
     * TreeWalker objects are used to navigate a document tree or subtree using the view of the document defined
     * by its whatToShow flags and any filters that are defined for the TreeWalker. Any function which performs
     * navigation using a TreeWalker will automatically support any view defined by a TreeWalker.
     *
     * Omitting nodes from the logical view of a subtree can result in a structure that is substantially different from
     * the same subtree in the complete, unfiltered document. Nodes that are siblings in the TreeWalker view may
     * be children of different, widely separated nodes in the original view. For instance, consider a Filter that skips
     * all nodes except for Text nodes and the root node of a document. In the logical view that results, all text
     * nodes will be siblings and appear as direct children of the root node, no matter how deeply nested the
     * structure of the original document.
     *
     * To produce a view of the document that has entity references expanded
     * and does not expose the entity reference node itself, use the whatToShow
     * flags to hide the entity reference node and set <code>expandEntityReferences</code> to
     * true when creating the TreeWalker. To produce a view of the document
     * that has entity reference nodes but no entity expansion, use the
     * <code>whatToShow</code> flags to show the entity reference node and set
     * <code>expandEntityReferences</code> to false
     *
     * @param root The root node of the DOM tree
     * @param whatToShow This attribute determines which node types are presented via the tree-walker.
     * @param filter The filter used to screen nodes
     * @param entityReferenceExpansion The value of this flag determines whether the children of entity reference nodes are
     *                   visible to the tree-walker. If false, they will be skipped over.
     */

    virtual DOMTreeWalker  *createTreeWalker(DOMNode        *root,
                                               unsigned long     whatToShow,
                                               DOMNodeFilter  *filter,
                                               bool              entityReferenceExpansion) = 0;

    /**
	  * To create the range  consisting of boundary-points and offset of the
      * selected contents
      *
      * @return The initial state of the Range such that both the boundary-points
      * are positioned at the beginning of the corresponding DOMDOcument, before
      * any content. The range returned can only be used to select content
      * associated with this document, or with documentFragments and Attrs for
      * which this document is the ownerdocument
	  */
    virtual DOMRange    *createRange() = 0;

    //@}
    /** @name Getter functions */
    //@{
    /**
     * Get Document Type Declaration (see <code>DOMDocumentType</code>) associated
     * with  this document.
     *
     * For documents without
     * a document type declaration this returns <code>null</code> reference object. The DOM Level
     *  1 does not support editing the Document Type Declaration, therefore
     * <code>docType</code> cannot be altered in any way.
     */
    virtual DOMDocumentType       *getDoctype() const = 0;



    /**
     * Return the <code>DOMImplementation</code> object that handles this document.
     */
    virtual DOMImplementation  *getImplementation() const = 0;


    /**
     * Return a reference to the root element of the document.
     */
    virtual DOMElement     *getDocumentElement() const = 0;

    /**
     * Returns a <code>DOMNodeList</code> of all the elements with a
     * given tag name.  The returned node list is "live", in that changes
     * to the document tree made after a nodelist was initially
     * returned will be immediately reflected in the node list.
     *
     * The elements in the node list are ordered in the same order in which they
     * would be encountered in a
     * preorder traversal of the <code>Document</code> tree.
     * @param tagname The name of the tag to match on. The special value "*"
     *   matches all tags.
     * @return A reference to a NodeList containing all the matched
     *   <code>Element</code>s.
     */
    virtual DOMNodeList      *getElementsByTagName(const XMLCh *tagname) const = 0;

    //@}
    /** @name Functions introduced in DOM Level 2. */
    //@{

    /**
     * Imports a node from another document to this document.
     * The returned node has no parent (<CODE>parentNode</CODE> is
     * <CODE>null</CODE>). The source node is not altered or removed from the
     * original document; this method creates a new copy of the source
     * node.<BR>For all nodes, importing a node creates a node object owned by
     * the importing document, with attribute values identical to the source
     * node's <CODE>nodeName</CODE> and <CODE>nodeType</CODE>, plus the
     * attributes related to namespaces (prefix and namespaces URI).
     *
     * @param importedNode The node to import.
     * @param deep If <CODE>true</CODE>, recursively import the subtree under the
     *      specified node; if <CODE>false</CODE>, import only the node itself,
     *      as explained above. This does not apply to <CODE>DOMAttr</CODE>,
     *      <CODE>DOMEntityReference</CODE>, and <CODE>DOMNotation</CODE> nodes.
     * @return The imported node that belongs to this <CODE>DOMDocument</CODE>.
     * @exception DOMException
     *   NOT_SUPPORTED_ERR: Raised if the type of node being imported is
     *                      not supported.
     */
    virtual DOMNode        *importNode(DOMNode *importedNode, bool deep) = 0;

    /**
     * Creates an element of the given qualified name and
     * namespace URI.
     *
     * @param namespaceURI The <em>namespace URI</em> of
     *   the element to create.
     * @param qualifiedName The <em>qualified name</em>
     *   of the element type to instantiate.
     * @return A new <code>DOMElement</code> object.
     * @exception DOMException
     *   INVALID_CHARACTER_ERR: Raised if the specified qualified name contains
     *                          an illegal character.
     * <br>
     *   NAMESPACE_ERR: Raised if the <CODE>qualifiedName</CODE> is
     *      malformed, if the <CODE>qualifiedName</CODE> has a prefix and the
     *      <CODE>namespaceURI</CODE> is <CODE>null</CODE> or an empty string,
     *      or if the <CODE>qualifiedName</CODE> has a prefix that is "xml" and
     *      the <CODE>namespaceURI</CODE> is different from
     *      "http://www.w3.org/XML/1998/namespace".
     */
    virtual DOMElement         *createElementNS(const XMLCh *namespaceURI,
	                                              const XMLCh *qualifiedName) = 0;

    /**
     * Creates an element of the given qualified name and
     * namespace URI, and also stores line/column number info.
     * Non standard. Used by XSDXercesDOMParser during schema traversal.
     *
     * @param namespaceURI The <em>namespace URI</em> of
     *   the element to create.
     * @param qualifiedName The <em>qualified name</em>
     *   of the element type to instantiate.
     * @param lineNum The <em>line number</em> of the element to create.
     * @param columnNum The <em>column number</em> of the element to create.
     * @return A new <code>DOMElement</code> object.
     * @exception DOMException
     *   INVALID_CHARACTER_ERR: Raised if the specified qualified name contains
     *                          an illegal character.
     * <br>
     *   NAMESPACE_ERR: Raised if the <CODE>qualifiedName</CODE> is
     *      malformed, if the <CODE>qualifiedName</CODE> has a prefix and the
     *      <CODE>namespaceURI</CODE> is <CODE>null</CODE> or an empty string,
     *      or if the <CODE>qualifiedName</CODE> has a prefix that is "xml" and
     *      the <CODE>namespaceURI</CODE> is different from
     *      "http://www.w3.org/XML/1998/namespace".
     */
    virtual DOMElement         *createElementNS(const XMLCh *namespaceURI,
                                                  const XMLCh *qualifiedName,
                                                  const XMLSSize_t lineNum,
                                                  const XMLSSize_t columnNum) = 0;

    /**
     * Creates an attribute of the given qualified name and namespace
     * URI.
     *
     * @param namespaceURI The <em>namespace URI</em> of
     *   the attribute to create.
     * @param qualifiedName The <em>qualified name</em>
     *   of the attribute to instantiate.
     * @return A new <code>DOMAttr</code> object.
     * @exception DOMException
     *   INVALID_CHARACTER_ERR: Raised if the specified qualified name contains
     *                          an illegal character.
     * <br>
     *   NAMESPACE_ERR: Raised if the <CODE>qualifiedName</CODE> is
     *      malformed, if the <CODE>qualifiedName</CODE> has a prefix and the
     *      <CODE>namespaceURI</CODE> is <CODE>null</CODE> or an empty string,
     *      if the <CODE>qualifiedName</CODE> has a prefix that is "xml" and the
     *      <CODE>namespaceURI</CODE> is different from
     *      "http://www.w3.org/XML/1998/namespace", if the
     *      <CODE>qualifiedName</CODE> has a prefix that is "xmlns" and the
     *      <CODE>namespaceURI</CODE> is different from
     *      "http://www.w3.org/2000/xmlns/", or if the
     *      <CODE>qualifiedName</CODE> is "xmlns" and the
     *      <CODE>namespaceURI</CODE> is different from
     *      "http://www.w3.org/2000/xmlns/".
     */
    virtual DOMAttr        *createAttributeNS(const XMLCh *namespaceURI,
	                                            const XMLCh *qualifiedName) = 0;

    /**
     * Returns a <code>DOMNodeList</code> of all the <code>DOMElement</code>s
     * with a given <em>local name</em> and
     * namespace URI in the order in which they would be encountered in a
     * preorder traversal of the <code>DOMDocument</code> tree.
     *
     * @param namespaceURI The <em>namespace URI</em> of
     *   the elements to match on. The special value "*" matches all
     *   namespaces.
     * @param localName The <em>local name</em> of the
     *   elements to match on. The special value "*" matches all local names.
     * @return A new <code>DOMNodeList</code> object containing all the matched
     *  <code>DOMElement</code>s.
     */
    virtual DOMNodeList        *getElementsByTagNameNS(const XMLCh *namespaceURI,
	                                                     const XMLCh *localName) const = 0;

    /**
     * Returns the <code>DOMElement</code> whose ID is given by <code>elementId</code>.
     * If no such element exists, returns <code>null</code>.
     * Behavior is not defined if more than one element has this <code>ID</code>.
     * <P><B>Note:</B> The DOM implementation must have information that says
     * which attributes are of type ID. Attributes with the name "ID" are not of
     * type ID unless so defined. Implementations that do not know whether
     * attributes are of type ID or not are expected to return
     * <CODE>null</CODE>.</P>
     *
     * @param elementId The unique <code>id</code> value for an element.
     * @return The matching element.
     */
    virtual  DOMElement        * getElementById(const XMLCh *elementId) const = 0;

    //@}

protected:
    friend class DOMNode;
    friend class DocumentImpl;
    friend class NodeIteratorImpl;
    friend class DOMImplementation;

};


#endif
