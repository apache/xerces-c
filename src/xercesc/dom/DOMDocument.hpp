#ifndef IDOM_Document_HEADER_GUARD_
#define IDOM_Document_HEADER_GUARD_

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
#include "IDOM_Node.hpp"

class IDNodeIteratorImpl;

class IDOM_DocumentType;
class IDOM_Element;
class IDOM_DocumentFragment;
class IDOM_Comment;
class IDOM_CDATASection;
class IDOM_ProcessingInstruction;
class IDOM_Attr;
class IDOM_Entity;
class IDOM_EntityReference;
class IDOM_DOMImplementation;
class IDOM_NodeFilter;
class IDOM_NodeList;
class IDOM_Notation;
class IDOM_Text;
class IDOM_Node;
class IDOM_NodeIterator;
class IDOM_TreeWalker;
class IDOM_Range;


/**
* Class to refer to XML Document nodes in the IDOM.
*
* Conceptually, a IDOM document node is the root of the document tree, and provides
* the  primary access to the document's data.
* <p>Since elements, text nodes, comments, processing instructions, etc.
* cannot exist outside the context of a <code>Document</code>, the
* <code>Document</code> interface also contains the factory methods needed
* to create these objects.  The <code>Node</code> objects created have a
* <code>ownerDocument</code> attribute which associates them with the
* <code>Document</code> within whose  context they were created.
*/
class CDOM_EXPORT IDOM_Document: public IDOM_Node {

protected:
    /** @name Constructors and assignment operators */
    //@{
    /**
     * The default constructor for IDOM_Document creates a null
     * IDOM_Document object that refers to no document.  It may subsequently be
     * assigned to refer to an actual Document node.
     *
     * To create a new document, use the DOMImplementation
     *   <code> IDOM_DOMImplementation::createDocument(). </code>
     *
     */
    IDOM_Document() {};

    /**
      * Copy constructor.  Creates a new <code>IDOM_Document</code> that refers to the
      * same underlying actual document as the original.
      *
      * @param other The object to be copied
      */
    IDOM_Document(const IDOM_Document &other) {};
    /**
      * Assignment operator
      *
      * @param other The object to be copied
      */
    IDOM_Document & operator = (const IDOM_Document &other) {return *this;};


	//@}

public:
  /** @name Destructor */
  //@{
	
    virtual ~IDOM_Document() {};

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
    virtual IDOM_Entity     *createEntity(const XMLCh *name) = 0;

    /**
    * Creates an element of the type specified.
    *
    * Note that the instance returned
    * implements the Element interface, so attributes can be specified
    * directly  on the returned object.
    * @param tagName The name of the element type to instantiate.
    * @return A <code>IDOM_Element</code> that reference the new element.
    * @exception IDOMException
    *   INVALID_CHARACTER_ERR: Raised if the specified name contains an
    *   illegal character.
    */
    virtual IDOM_Element     *createElement(const XMLCh *tagName) = 0;



    /**
    * Creates an empty DocumentFragment object.
    *
    * @return A <code>IDOM_DocumentFragment</code> that references the newly
    * created document fragment.
    */
    virtual IDOM_DocumentFragment   *createDocumentFragment() = 0;

    /**
    * Creates a Text node given the specified string.
    *
    * @param data The data for the node.
    * @return A <code>IDOM_Text</code> object that references the newly
    *  created text node.
    */
    virtual IDOM_Text         *createTextNode(const XMLCh *data) = 0;

    /**
    * Creates a Comment node given the specified string.
    *
    * @param data The data for the comment.
    * @return A <code>IDOM_Comment</code> that references the newly
    *  created comment node.
    */
    virtual IDOM_Comment      *createComment(const XMLCh *data) = 0;

    /**
    * Creates a CDATASection node whose value  is the specified
    * string.
    *
    * @param data The data for the <code>IDOM_CDATASection</code> contents.
    * @return A <code>IDOM_CDATASection</code> object.
    * @exception IDOMException
    *   NOT_SUPPORTED_ERR: Raised if this document is an HTML document.
    */
    virtual IDOM_CDATASection   *createCDATASection(const XMLCh *data) = 0;

    /**
    *  Create a DocumentType node.  Non-standard extension.
    *
    * @return A <code>IDOM_DocumentType</code> that references the newly
    *  created DocumentType node.
    *
    */
    virtual IDOM_DocumentType *createDocumentType(const XMLCh *name) = 0;


    /**
    *  Create a Notation.
    *
    *  Non-standard extension.
    *
    *  @param name The name of the notation to instantiate
    * @return A <code>IDOM_Notation</code> that references the newly
    *  created Notation node.
    */
    virtual IDOM_Notation *createNotation(const XMLCh *name) = 0;


    /**
    * Creates a ProcessingInstruction node given the specified
    * name and data strings.
    *
    * @param target The target part of the processing instruction.
    * @param data The data for the node.
    * @return A <code>IDOM_ProcessingInstruction</code> that references the newly
    *  created PI node.
    * @exception IDOMException
    *   INVALID_CHARACTER_ERR: Raised if an illegal character is specified.
    */
    virtual IDOM_ProcessingInstruction *createProcessingInstruction(const XMLCh *target,
        const XMLCh *data) = 0;


    /**
     * Creates an Attr of the given name.
     *
     * Note that the
     * <code>Attr</code> instance can then be attached to an Element
     * using the <code>IDOMElement::setAttribute()</code> method.
     * @param name The name of the attribute.
     * @return A new <CODE>IDOM_Attr</CODE>
     *       object with the <CODE>nodeName</CODE> attribute set to
     *       <CODE>name</CODE>, and <CODE>localName</CODE>, <CODE>prefix</CODE>,
     *       and <CODE>namespaceURI</CODE> set to
     *       <CODE>null</CODE>.
     * @exception IDOMException
     *   INVALID_CHARACTER_ERR: Raised if the specified name contains an
     *   illegal character.
     */
    virtual IDOM_Attr     *createAttribute(const XMLCh *name) = 0;


    /**
     * Creates an EntityReference object.
     *
     * @param name The name of the entity to reference.
     * @return A <code>IDOM_EntityReference</code> that references the newly
     *  created EntityReference node.
     * @exception IDOMException
     *   INVALID_CHARACTER_ERR: Raised if the specified name contains an
     *   illegal character.
     */
    virtual IDOM_EntityReference    *createEntityReference(const XMLCh *name) = 0;


    /**
     * Creates a NodeIterator object.   (IDOM2)
     *
     * NodeIterators are used to step through a set of nodes, e.g. the set of nodes in a NodeList, the
     * document subtree governed by a particular node, the results of a query, or any other set of nodes.
     * The set of nodes to be iterated is determined by the implementation of the NodeIterator. IDOM Level 2
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
     * @param root The root node of the IDOM tree
     * @param whatToShow This attribute determines which node types are presented via the iterator.
     * @param filter The filter used to screen nodes
     * @param entityReferenceExpansion The value of this flag determines whether the children of entity reference nodes are
     *                   visible to the iterator. If false, they will be skipped over.
     */

    virtual IDOM_NodeIterator *createNodeIterator(IDOM_Node         *root,
                                                   unsigned long    whatToShow,
                                                   IDOM_NodeFilter* filter,
                                                   bool             entityReferenceExpansion) = 0;
     /**
     * Creates a TreeWalker object.   (IDOM2)
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
     * @param root The root node of the IDOM tree
     * @param whatToShow This attribute determines which node types are presented via the tree-walker.
     * @param filter The filter used to screen nodes
     * @param entityReferenceExpansion The value of this flag determines whether the children of entity reference nodes are
     *                   visible to the tree-walker. If false, they will be skipped over.
     */

    virtual IDOM_TreeWalker  *createTreeWalker(IDOM_Node        *root,
                                               unsigned long     whatToShow,
                                               IDOM_NodeFilter  *filter,
                                               bool              entityReferenceExpansion) = 0;

    /**
	  * To create the range  consisting of boundary-points and offset of the
      * selected contents
      *
      * @return The initial state of the Range such that both the boundary-points
      * are positioned at the beginning of the corresponding IDOM_DOcument, before
      * any content. The range returned can only be used to select content
      * associated with this document, or with documentFragments and Attrs for
      * which this document is the ownerdocument
	  */
    virtual IDOM_Range    *createRange() = 0;

    //@}
    /** @name Getter functions */
    //@{
    /**
     * Get Document Type Declaration (see <code>IDOM_DocumentType</code>) associated
     * with  this document.
     *
     * For documents without
     * a document type declaration this returns <code>null</code> reference object. The IDOM Level
     *  1 does not support editing the Document Type Declaration, therefore
     * <code>docType</code> cannot be altered in any way.
     */
    virtual IDOM_DocumentType       *getDoctype() const = 0;



    /**
     * Return the <code>IDOMImplementation</code> object that handles this document.
     */
    virtual IDOM_DOMImplementation  *getImplementation() const = 0;


    /**
     * Return a reference to the root element of the document.
     */
    virtual IDOM_Element     *getDocumentElement() const = 0;

    /**
     * Returns a <code>IDOM_NodeList</code> of all the elements with a
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
    virtual IDOM_NodeList      *getElementsByTagName(const XMLCh *tagname) const = 0;

    //@}
    /** @name Functions introduced in IDOM Level 2. */
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
     *      as explained above. This does not apply to <CODE>IDOM_Attr</CODE>,
     *      <CODE>IDOM_EntityReference</CODE>, and <CODE>IDOM_Notation</CODE> nodes.
     * @return The imported node that belongs to this <CODE>IDOM_Document</CODE>.
     * @exception IDOMException
     *   NOT_SUPPORTED_ERR: Raised if the type of node being imported is
     *                      not supported.
     */
    virtual IDOM_Node        *importNode(IDOM_Node *importedNode, bool deep) = 0;

    /**
     * Creates an element of the given qualified name and
     * namespace URI.
     *
     * @param namespaceURI The <em>namespace URI</em> of
     *   the element to create.
     * @param qualifiedName The <em>qualified name</em>
     *   of the element type to instantiate.
     * @return A new <code>IDOM_Element</code> object.
     * @exception IDOMException
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
    virtual IDOM_Element         *createElementNS(const XMLCh *namespaceURI,
	                                              const XMLCh *qualifiedName) = 0;

    /**
     * Creates an element of the given qualified name and
     * namespace URI, and also stores line/column number info.
     * Non standard. Used by XIDOMParser during schema traversal.
     *
     * @param namespaceURI The <em>namespace URI</em> of
     *   the element to create.
     * @param qualifiedName The <em>qualified name</em>
     *   of the element type to instantiate.
     * @param lineNum The <em>line number</em> of the element to create.
     * @param columnNum The <em>column number</em> of the element to create.
     * @return A new <code>IDOM_Element</code> object.
     * @exception IDOMException
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
    virtual IDOM_Element         *createElementNS(const XMLCh *namespaceURI,
                                                  const XMLCh *qualifiedName,
                                                  const int lineNum,
                                                  const int columnNum) = 0;

    /**
     * Creates an attribute of the given qualified name and namespace
     * URI.
     *
     * @param namespaceURI The <em>namespace URI</em> of
     *   the attribute to create.
     * @param qualifiedName The <em>qualified name</em>
     *   of the attribute to instantiate.
     * @return A new <code>IDOM_Attr</code> object.
     * @exception IDOMException
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
    virtual IDOM_Attr        *createAttributeNS(const XMLCh *namespaceURI,
	                                            const XMLCh *qualifiedName) = 0;

    /**
     * Returns a <code>IDOM_NodeList</code> of all the <code>IDOM_Element</code>s
     * with a given <em>local name</em> and
     * namespace URI in the order in which they would be encountered in a
     * preorder traversal of the <code>IDOM_Document</code> tree.
     *
     * @param namespaceURI The <em>namespace URI</em> of
     *   the elements to match on. The special value "*" matches all
     *   namespaces.
     * @param localName The <em>local name</em> of the
     *   elements to match on. The special value "*" matches all local names.
     * @return A new <code>IDOM_NodeList</code> object containing all the matched
     *  <code>IDOM_Element</code>s.
     */
    virtual IDOM_NodeList        *getElementsByTagNameNS(const XMLCh *namespaceURI,
	                                                     const XMLCh *localName) const = 0;

    /**
     * Returns the <code>IDOM_Element</code> whose ID is given by <code>elementId</code>.
     * If no such element exists, returns <code>null</code>.
     * Behavior is not defined if more than one element has this <code>ID</code>.
     * <P><B>Note:</B> The IDOM implementation must have information that says
     * which attributes are of type ID. Attributes with the name "ID" are not of
     * type ID unless so defined. Implementations that do not know whether
     * attributes are of type ID or not are expected to return
     * <CODE>null</CODE>.</P>
     *
     * @param elementId The unique <code>id</code> value for an element.
     * @return The matching element.
     */
    virtual  IDOM_Element        * getElementById(const XMLCh *elementId) const = 0;

    //@}

protected:
    friend class IDOM_Node;
    friend class DocumentImpl;
    friend class NodeIteratorImpl;
    friend class IDOM_IDOMImplementation;

};


#endif
