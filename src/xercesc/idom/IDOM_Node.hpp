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
 * $Id$
 */

#ifndef IDOM_Node_HEADER_GUARD_
#define IDOM_Node_HEADER_GUARD_

#include <xercesc/util/XercesDefs.hpp>


class IDOM_Document;
class IDOM_NamedNodeMap;
class IDOM_NodeList;

/**
 * The <code>Node</code> interface is the primary datatype for the entire
 * Document Object Model.
 *
 * It represents a single node in the document tree.
 * While all objects implementing the <code>Node</code> interface expose
 * methods for dealing with children, not all objects implementing the
 * <code>Node</code> interface may have children. For example,
 * <code>Text</code> nodes may not have children, and adding children to such
 * nodes results in a <code>DOMException</code> being raised.
 * <p>The attributes <code>nodeName</code>, <code>nodeValue</code>  and
 * <code>attributes</code> are  included as a mechanism to get at node
 * information without  casting down to the specific derived interface. In
 * cases where  there is no obvious mapping of these attributes for a specific
 *  <code>nodeType</code> (e.g., <code>nodeValue</code> for an Element  or
 * <code>attributes</code>  for a Comment), this returns <code>null</code>.
 * Note that the  specialized interfaces may contain additional and more
 * convenient mechanisms to get and set the relevant information.
 */
class  CDOM_EXPORT IDOM_Node {

    protected:
    /** @name Constructors and assignment operators */
    //@{
    /**
      * Default constructor for IDOM_Node.  Protected, because
      * IDOM_Node is an abstract base class.
      *
      */
        IDOM_Node() {};

    /**
      * Copy constructor.
      *
      * @param other The object to be copied.
      */
        IDOM_Node(const IDOM_Node &other) {};

    /**
      * Assignment operator.
      *
      * @param other The source to be assigned.
      */
        IDOM_Node & operator = (const IDOM_Node &other) {return *this;};

    public:

   //@}
    /** @name Destructor. */
    //@{
	 /**
	  * Destructor for IDOM_Node.
	  *
	  */
        virtual ~IDOM_Node() {};

    //@}


    enum NodeType {
        ELEMENT_NODE                = 1,
        ATTRIBUTE_NODE              = 2,
        TEXT_NODE                   = 3,
        CDATA_SECTION_NODE          = 4,
        ENTITY_REFERENCE_NODE       = 5,
        ENTITY_NODE                 = 6,
        PROCESSING_INSTRUCTION_NODE = 7,
        COMMENT_NODE                = 8,
        DOCUMENT_NODE               = 9,
        DOCUMENT_TYPE_NODE          = 10,
        DOCUMENT_FRAGMENT_NODE      = 11,
        NOTATION_NODE               = 12,
        XML_DECL_NODE               = 13
    };

    /** @name Get functions. */
    //@{

    /**
     * The name of this node, depending on its type; see the table above.
     */
    virtual const XMLCh *   getNodeName() const = 0;

    /**
     * Gets the value of this node, depending on its type.
     *
     * @exception DOMException
     *   NO_MODIFICATION_ALLOWED_ERR: Raised when the node is readonly.
     */
    virtual const XMLCh *       getNodeValue() const = 0;

    /**
     * An enum value representing the type of the underlying object.
     */
    virtual short           getNodeType() const = 0;

    /**
     * Gets the parent of this node.
     *
     * All nodes, except <code>Document</code>,
     * <code>DocumentFragment</code>, and <code>Attr</code> may have a parent.
     * However, if a node has just been created and not yet added to the tree,
     * or if it has been removed from the tree, a <code>null</code> IDOM_Node
     * is returned.
     */
    virtual IDOM_Node        *getParentNode() const = 0;

    /**
     * Gets a <code>NodeList</code> that contains all children of this node.
     *
     * If there
     * are no children, this is a <code>NodeList</code> containing no nodes.
     * The content of the returned <code>NodeList</code> is "live" in the sense
     * that, for instance, changes to the children of the node object that
     * it was created from are immediately reflected in the nodes returned by
     * the <code>NodeList</code> accessors; it is not a static snapshot of the
     * content of the node. This is true for every <code>NodeList</code>,
     * including the ones returned by the <code>getElementsByTagName</code>
     * method.
     */
    virtual IDOM_NodeList    *getChildNodes() const = 0;
    /**
     * Gets the first child of this node.
     *
     * If there is no such node, this returns <code>null</code>.
     */
    virtual IDOM_Node        *getFirstChild() const = 0;

    /**
     * Gets the last child of this node.
     *
     * If there is no such node, this returns <code>null</code>.
     */
    virtual IDOM_Node        *getLastChild() const = 0;

    /**
     * Gets the node immediately preceding this node.
     *
     * If there is no such node, this returns <code>null</code>.
     */
    virtual IDOM_Node        *getPreviousSibling() const = 0;

    /**
     * Gets the node immediately following this node.
     *
     * If there is no such node, this returns <code>null</code>.
     */
    virtual IDOM_Node        *getNextSibling() const = 0;

    /**
     * Gets a <code>NamedNodeMap</code> containing the attributes of this node (if it
     * is an <code>Element</code>) or <code>null</code> otherwise.
     */
    virtual IDOM_NamedNodeMap  *getAttributes() const = 0;

    /**
     * Gets the <code>IDOM_Document</code> object associated with this node.
     *
     * This is also
     * the <code>IDOM_Document</code> object used to create new nodes. When this
     * node is a <code>IDOM_Document</code> or a <code>IDOM_DocumentType</code>
     * which is not used with any <code>IDOM_Document</code> yet, this is
     * <code>null</code>.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     */
    virtual IDOM_Document      *getOwnerDocument() const = 0;


    //@}
    /** @name Cloning function. */
    //@{

    /**
     * Returns a duplicate of this node.
     *
     * This function serves as a generic copy constructor for nodes.
     *
     * The duplicate node has no parent (
     * <code>parentNode</code> returns <code>null</code>.).
     * <br>Cloning an <code>Element</code> copies all attributes and their
     * values, including those generated by the  XML processor to represent
     * defaulted attributes, but this method does not copy any text it contains
     * unless it is a deep clone, since the text is contained in a child
     * <code>Text</code> node. Cloning any other type of node simply returns a
     * copy of this node.
     * @param deep If <code>true</code>, recursively clone the subtree under the
     *   specified node; if <code>false</code>, clone only the node itself (and
     *   its attributes, if it is an <code>Element</code>).
     * @return The duplicate node.
     */
    virtual IDOM_Node        * cloneNode(bool deep) const = 0;

    //@}
    /** @name Functions to modify the DOM Node. */
    //@{

    /**
     * Inserts the node <code>newChild</code> before the existing child node
     * <code>refChild</code>.
     *
     * If <code>refChild</code> is <code>null</code>,
     * insert <code>newChild</code> at the end of the list of children.
     * <br>If <code>newChild</code> is a <code>DocumentFragment</code> object,
     * all of its children are inserted, in the same order, before
     * <code>refChild</code>. If the <code>newChild</code> is already in the
     * tree, it is first removed.  Note that a <code>IDOM_Node</code> that
     * has never been assigned to refer to an actual node is == null.
     * @param newChild The node to insert.
     * @param refChild The reference node, i.e., the node before which the new
     *   node must be inserted.
     * @return The node being inserted.
     * @exception DOMException
     *   HIERARCHY_REQUEST_ERR: Raised if this node is of a type that does not
     *   allow children of the type of the <code>newChild</code> node, or if
     *   the node to insert is one of this node's ancestors.
     *   <br>WRONG_DOCUMENT_ERR: Raised if <code>newChild</code> was created
     *   from a different document than the one that created this node.
     *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this node or the node being
     *   inserted is readonly.
     *   <br>NOT_FOUND_ERR: Raised if <code>refChild</code> is not a child of
     *   this node.
     */
    virtual IDOM_Node       *insertBefore(IDOM_Node *newChild,
                                          IDOM_Node *refChild) = 0;


    /**
     * Replaces the child node <code>oldChild</code> with <code>newChild</code>
     * in the list of children, and returns the <code>oldChild</code> node.
     *
     * If <CODE>newChild</CODE> is a <CODE>IDOM_DocumentFragment</CODE> object,
     * <CODE>oldChild</CODE> is replaced by all of the <CODE>IDOM_DocumentFragment</CODE>
     * children, which are inserted in the same order.
     *
     * If the <code>newChild</code> is already in the tree, it is first removed.
     * @param newChild The new node to put in the child list.
     * @param oldChild The node being replaced in the list.
     * @return The node replaced.
     * @exception DOMException
     *   HIERARCHY_REQUEST_ERR: Raised if this node is of a type that does not
     *   allow children of the type of the <code>newChild</code> node, or it
     *   the node to put in is one of this node's ancestors.
     *   <br>WRONG_DOCUMENT_ERR: Raised if <code>newChild</code> was created
     *   from a different document than the one that created this node.
     *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this node or the new node is readonly.
     *   <br>NOT_FOUND_ERR: Raised if <code>oldChild</code> is not a child of
     *   this node.
     */
    virtual IDOM_Node  *replaceChild(IDOM_Node *newChild,
                                     IDOM_Node *oldChild) = 0;
    /**
     * Removes the child node indicated by <code>oldChild</code> from the list
     * of children, and returns it.
     *
     * @param oldChild The node being removed.
     * @return The node removed.
     * @exception DOMException
     *   NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
     *   <br>NOT_FOUND_ERR: Raised if <code>oldChild</code> is not a child of
     *   this node.
     */
    virtual IDOM_Node        *removeChild(IDOM_Node *oldChild) = 0;

    /**
     * Adds the node <code>newChild</code> to the end of the list of children of
     * this node.
     *
     * If the <code>newChild</code> is already in the tree, it is
     * first removed.
     * @param newChild The node to add.If it is a  <code>DocumentFragment</code>
     *   object, the entire contents of the document fragment are moved into
     *   the child list of this node
     * @return The node added.
     * @exception DOMException
     *   HIERARCHY_REQUEST_ERR: Raised if this node is of a type that does not
     *   allow children of the type of the <code>newChild</code> node, or if
     *   the node to append is one of this node's ancestors.
     *   <br>WRONG_DOCUMENT_ERR: Raised if <code>newChild</code> was created
     *   from a different document than the one that created this node.
     *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this node or the node being
     *   appended is readonly.
     */
    virtual IDOM_Node        *appendChild(IDOM_Node *newChild) = 0;

    //@}
    /** @name Query functions. */
    //@{

    /**
     *  This is a convenience method to allow easy determination of whether a
     * node has any children.
     *
     * @return  <code>true</code> if the node has any children,
     *   <code>false</code> if the node has no children.
     */
    virtual bool             hasChildNodes() const = 0;



    //@}
    /** @name Set functions. */
    //@{


    /**
    * Sets the value of the node.
    *
    * Any node which can have a nodeValue (@see getNodeValue) will
    * also accept requests to set it to a string. The exact response to
    * this varies from node to node -- Attribute, for example, stores
    * its values in its children and has to replace them with a new Text
    * holding the replacement value.
    *
    * For most types of Node, value is null and attempting to set it
    * will throw DOMException(NO_MODIFICATION_ALLOWED_ERR). This will
    * also be thrown if the node is read-only.
    */
    virtual void              setNodeValue(const XMLCh  *nodeValue) = 0;

    //@}
    /** @name Functions introduced in DOM Level 2. */
    //@{

    /**
     * Puts all <CODE>IDOM_Text</CODE>
     * nodes in the full depth of the sub-tree underneath this <CODE>IDOM_Node</CODE>,
     * including attribute nodes, into a "normal" form where only markup (e.g.,
     * tags, comments, processing instructions, CDATA sections, and entity
     * references) separates <CODE>IDOM_Text</CODE>
     * nodes, i.e., there are neither adjacent <CODE>IDOM_Text</CODE>
     * nodes nor empty <CODE>IDOM_Text</CODE>
     * nodes. This can be used to ensure that the DOM view of a document is the
     * same as if it were saved and re-loaded, and is useful when operations
     * (such as XPointer lookups) that depend on a particular document tree
     * structure are to be used.
     * <P><B>Note:</B> In cases where the document contains <CODE>IDOM_CDATASections</CODE>,
     * the normalize operation alone may not be sufficient, since XPointers do
     * not differentiate between <CODE>IDOM_Text</CODE>
     * nodes and <CODE>IDOM_CDATASection</CODE>
     * nodes.</P>
     *
     *
     */
    virtual void              normalize() = 0;

    /**
     * Tests whether the DOM implementation implements a specific
     * feature and that feature is supported by this node.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @param feature The string of the feature to test. This is the same
     * name as what can be passed to the method <code>hasFeature</code> on
     * <code>IDOM_DOMImplementation</code>.
     * @param version This is the version number of the feature to test. In
     * Level 2, version 1, this is the string "2.0". If the version is not
     * specified, supporting any version of the feature will cause the
     * method to return <code>true</code>.
     * @return Returns <code>true</code> if the specified feature is supported
     * on this node, <code>false</code> otherwise.
     */
    virtual bool      supports(const XMLCh *feature,
	                           const XMLCh *version) const = 0;

    /**
     * Get the <em>namespace URI</em> of
     * this node, or <code>null</code> if it is unspecified.
     * <p>
     * This is not a computed value that is the result of a namespace lookup
     * based on an examination of the namespace declarations in scope. It is
     * merely the namespace URI given at creation time.
     * <p>
     * For nodes of any type other than <CODE>ELEMENT_NODE</CODE> and
     * <CODE>ATTRIBUTE_NODE</CODE> and nodes created with a DOM Level 1 method,
     * such as <CODE>createElement</CODE> from the <CODE>IDOM_Document</CODE>
     * interface, this is always <CODE>null</CODE>.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     */
    virtual const XMLCh *         getNamespaceURI() const = 0;

    /**
     * Get the <em>namespace prefix</em>
     * of this node, or <code>null</code> if it is unspecified.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     */
    virtual const XMLCh *          getPrefix() const = 0;

    /**
     * Returns the local part of the <em>qualified name</em> of this node.
     * <p>
     * For nodes created with a DOM Level 1 method, such as
     * <code>createElement</code> from the <code>IDOM_Document</code> interface,
     * it is null.
     *
     *
     */
    virtual const XMLCh *          getLocalName() const = 0;

    /**
     * Set the <em>namespace prefix</em> of this node.
     * <p>
     * Note that setting this attribute, when permitted, changes
     * the <CODE>nodeName</CODE> attribute, which holds the <EM>qualified
     * name</EM>, as well as the <CODE>tagName</CODE> and <CODE>name</CODE>
     * attributes of the <CODE>IDOM_Element</CODE> and <CODE>IDOM_Attr</CODE>
     * interfaces, when applicable.
     * <p>
     * Note also that changing the prefix of an
     * attribute, that is known to have a default value, does not make a new
     * attribute with the default value and the original prefix appear, since the
     * <CODE>namespaceURI</CODE> and <CODE>localName</CODE> do not change.
     *
     *
     * @param prefix The prefix of this node.
     * @exception DOMException
     *   INVALID_CHARACTER_ERR: Raised if the specified prefix contains
     *                          an illegal character.
     * <br>
     *   NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
     * <br>
     *   NAMESPACE_ERR: Raised if the specified <CODE>prefix</CODE> is
     *      malformed, if the <CODE>namespaceURI</CODE> of this node is
     *      <CODE>null</CODE>, if the specified prefix is "xml" and the
     *      <CODE>namespaceURI</CODE> of this node is different from
     *      "http://www.w3.org/XML/1998/namespace", if this node is an attribute
     *      and the specified prefix is "xmlns" and the
     *      <CODE>namespaceURI</CODE> of this node is different from
     *      "http://www.w3.org/2000/xmlns/", or if this node is an attribute and
     *      the <CODE>qualifiedName</CODE> of this node is "xmlns".
     */
    virtual void              setPrefix(const XMLCh * prefix) = 0;

    //@}


};


#endif

