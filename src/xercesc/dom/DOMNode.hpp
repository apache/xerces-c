#ifndef DOMNode_HEADER_GUARD_
#define DOMNode_HEADER_GUARD_

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


class DOMDocument;
class DOMNamedNodeMap;
class DOMNodeList;
class DOMUserDataHandler;

/**
 * The <code>DOMNode</code> interface is the primary datatype for the entire
 * Document Object Model. It represents a single node in the document tree.
 * While all objects implementing the <code>DOMNode</code> interface expose
 * methods for dealing with children, not all objects implementing the
 * <code>DOMNode</code> interface may have children. For example,
 * <code>DOMText</code> nodes may not have children, and adding children to
 * such nodes results in a <code>DOMException</code> being raised.
 * <p>The attributes <code>nodeName</code>, <code>nodeValue</code> and
 * <code>attributes</code> are included as a mechanism to get at node
 * information without casting down to the specific derived interface. In
 * cases where there is no obvious mapping of these attributes for a
 * specific <code>nodeType</code> (e.g., <code>nodeValue</code> for an
 * <code>DOMElement</code> or <code>attributes</code> for a <code>DOMComment</code>
 * ), this returns <code>null</code>. Note that the specialized interfaces
 * may contain additional and more convenient mechanisms to get and set the
 * relevant information.
 * <p>The values of <code>nodeName</code>,
 * <code>nodeValue</code>, and <code>attributes</code> vary according to the
 * node type as follows:
 * <table border='1'>
 * <tr>
 * <td>Interface</td>
 * <td>nodeName</td>
 * <td>nodeValue</td>
 * <td>attributes</td>
 * </tr>
 * <tr>
 * <td valign='top' rowspan='1' colspan='1'>DOMAttr</td>
 * <td valign='top' rowspan='1' colspan='1'>name of
 * attribute</td>
 * <td valign='top' rowspan='1' colspan='1'>value of attribute</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * </tr>
 * <tr>
 * <td valign='top' rowspan='1' colspan='1'>DOMCDATASection</td>
 * <td valign='top' rowspan='1' colspan='1'><code>"#cdata-section"</code></td>
 * <td valign='top' rowspan='1' colspan='1'>
 * content of the CDATA Section</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * </tr>
 * <tr>
 * <td valign='top' rowspan='1' colspan='1'>DOMComment</td>
 * <td valign='top' rowspan='1' colspan='1'><code>"#comment"</code></td>
 * <td valign='top' rowspan='1' colspan='1'>content of
 * the comment</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * </tr>
 * <tr>
 * <td valign='top' rowspan='1' colspan='1'>DOMDocument</td>
 * <td valign='top' rowspan='1' colspan='1'><code>"#document"</code></td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * </tr>
 * <tr>
 * <td valign='top' rowspan='1' colspan='1'>DOMDocumentFragment</td>
 * <td valign='top' rowspan='1' colspan='1'>
 * <code>"#document-fragment"</code></td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * </tr>
 * <tr>
 * <td valign='top' rowspan='1' colspan='1'>DOMDocumentType</td>
 * <td valign='top' rowspan='1' colspan='1'>document type name</td>
 * <td valign='top' rowspan='1' colspan='1'>
 * null</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * </tr>
 * <tr>
 * <td valign='top' rowspan='1' colspan='1'>DOMElement</td>
 * <td valign='top' rowspan='1' colspan='1'>tag name</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * <td valign='top' rowspan='1' colspan='1'>NamedNodeMap</td>
 * </tr>
 * <tr>
 * <td valign='top' rowspan='1' colspan='1'>DOMEntity</td>
 * <td valign='top' rowspan='1' colspan='1'>entity name</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * </tr>
 * <tr>
 * <td valign='top' rowspan='1' colspan='1'>DOMEntityReference</td>
 * <td valign='top' rowspan='1' colspan='1'>name of entity referenced</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * </tr>
 * <tr>
 * <td valign='top' rowspan='1' colspan='1'>DOMNotation</td>
 * <td valign='top' rowspan='1' colspan='1'>notation name</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * <td valign='top' rowspan='1' colspan='1'>
 * null</td>
 * </tr>
 * <tr>
 * <td valign='top' rowspan='1' colspan='1'>DOMProcessingInstruction</td>
 * <td valign='top' rowspan='1' colspan='1'>target</td>
 * <td valign='top' rowspan='1' colspan='1'>entire content excluding the target</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * </tr>
 * <tr>
 * <td valign='top' rowspan='1' colspan='1'>DOMText</td>
 * <td valign='top' rowspan='1' colspan='1'>
 * <code>"#text"</code></td>
 * <td valign='top' rowspan='1' colspan='1'>content of the text node</td>
 * <td valign='top' rowspan='1' colspan='1'>null</td>
 * </tr>
 * </table>
 * <p>See also the <a href='http://www.w3.org/TR/2000/REC-DOM-Level-2-Core-20001113'>Document Object Model (DOM) Level 2 Core Specification</a>.
 *
 * @since DOM Level 1
 */
class  CDOM_EXPORT DOMNode {
protected:
    // -----------------------------------------------------------------------
    //  Hidden constructors
    // -----------------------------------------------------------------------
    /** @name Hidden constructors */
    //@{
    DOMNode() {};
    DOMNode(const DOMNode &other) {};
    DOMNode & operator = (const DOMNode &other) {return *this;};
    //@}

public:
    // -----------------------------------------------------------------------
    //  All constructors are hidden, just the destructor is available
    // -----------------------------------------------------------------------
    /** @name Destructor */
    //@{
    /**
     * Destructor
     *
     */
    virtual ~DOMNode() {};
    //@}

    // -----------------------------------------------------------------------
    //  Class Types
    // -----------------------------------------------------------------------
    /** @name Public Contants */
    //@{
    /**
     * NodeType
     *
     * @since DOM Level 1
     */
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
        NOTATION_NODE               = 12
    };
    //@}

    // -----------------------------------------------------------------------
    //  Virtual DOMNode interface
    // -----------------------------------------------------------------------
    /** @name Functions introduced in DOM Level 1 */
    //@{
    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    /**
     * The name of this node, depending on its type; see the table above.
     * @since DOM Level 1
     */
    virtual const XMLCh *   getNodeName() const = 0;

    /**
     * Gets the value of this node, depending on its type.
     *
     * @exception DOMException
     *   NO_MODIFICATION_ALLOWED_ERR: Raised when the node is readonly.
     * @since DOM Level 1
     */
    virtual const XMLCh *       getNodeValue() const = 0;

    /**
     * An enum value representing the type of the underlying object.
     * @since DOM Level 1
     */
    virtual short           getNodeType() const = 0;

    /**
     * Gets the parent of this node.
     *
     * All nodes, except <code>DOMDocument</code>,
     * <code>DOMDocumentFragment</code>, and <code>DOMAttr</code> may have a parent.
     * However, if a node has just been created and not yet added to the tree,
     * or if it has been removed from the tree, a <code>null</code> DOMNode
     * is returned.
     * @since DOM Level 1
     */
    virtual DOMNode        *getParentNode() const = 0;

    /**
     * Gets a <code>DOMNodeList</code> that contains all children of this node.
     *
     * If there
     * are no children, this is a <code>DOMNodeList</code> containing no nodes.
     * The content of the returned <code>DOMNodeList</code> is "live" in the sense
     * that, for instance, changes to the children of the node object that
     * it was created from are immediately reflected in the nodes returned by
     * the <code>DOMNodeList</code> accessors; it is not a static snapshot of the
     * content of the node. This is true for every <code>DOMNodeList</code>,
     * including the ones returned by the <code>getElementsByTagName</code>
     * method.
     * @since DOM Level 1
     */
    virtual DOMNodeList    *getChildNodes() const = 0;
    /**
     * Gets the first child of this node.
     *
     * If there is no such node, this returns <code>null</code>.
     * @since DOM Level 1
     */
    virtual DOMNode        *getFirstChild() const = 0;

    /**
     * Gets the last child of this node.
     *
     * If there is no such node, this returns <code>null</code>.
     * @since DOM Level 1
     */
    virtual DOMNode        *getLastChild() const = 0;

    /**
     * Gets the node immediately preceding this node.
     *
     * If there is no such node, this returns <code>null</code>.
     * @since DOM Level 1
     */
    virtual DOMNode        *getPreviousSibling() const = 0;

    /**
     * Gets the node immediately following this node.
     *
     * If there is no such node, this returns <code>null</code>.
     * @since DOM Level 1
     */
    virtual DOMNode        *getNextSibling() const = 0;

    /**
     * Gets a <code>DOMNamedNodeMap</code> containing the attributes of this node (if it
     * is an <code>DOMElement</code>) or <code>null</code> otherwise.
     * @since DOM Level 1
     */
    virtual DOMNamedNodeMap  *getAttributes() const = 0;

    /**
     * Gets the <code>DOMDocument</code> object associated with this node.
     *
     * This is also
     * the <code>DOMDocument</code> object used to create new nodes. When this
     * node is a <code>DOMDocument</code> or a <code>DOMDocumentType</code>
     * which is not used with any <code>DOMDocument</code> yet, this is
     * <code>null</code>.
     *
     * @since DOM Level 1
     */
    virtual DOMDocument      *getOwnerDocument() const = 0;

    // -----------------------------------------------------------------------
    //  Node methods
    // -----------------------------------------------------------------------
    /**
     * Returns a duplicate of this node.
     *
     * This function serves as a generic copy constructor for nodes.
     *
     * The duplicate node has no parent (
     * <code>parentNode</code> returns <code>null</code>.).
     * <br>Cloning an <code>DOMElement</code> copies all attributes and their
     * values, including those generated by the  XML processor to represent
     * defaulted attributes, but this method does not copy any text it contains
     * unless it is a deep clone, since the text is contained in a child
     * <code>DOMText</code> node. Cloning any other type of node simply returns a
     * copy of this node.
     * @param deep If <code>true</code>, recursively clone the subtree under the
     *   specified node; if <code>false</code>, clone only the node itself (and
     *   its attributes, if it is an <code>DOMElement</code>).
     * @return The duplicate node.
     * @since DOM Level 1
     */
    virtual DOMNode        * cloneNode(bool deep) const = 0;

    /**
     * Inserts the node <code>newChild</code> before the existing child node
     * <code>refChild</code>.
     *
     * If <code>refChild</code> is <code>null</code>,
     * insert <code>newChild</code> at the end of the list of children.
     * <br>If <code>newChild</code> is a <code>DOMDocumentFragment</code> object,
     * all of its children are inserted, in the same order, before
     * <code>refChild</code>. If the <code>newChild</code> is already in the
     * tree, it is first removed.  Note that a <code>DOMNode</code> that
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
     * @since DOM Level 1
     */
    virtual DOMNode       *insertBefore(DOMNode *newChild,
                                          DOMNode *refChild) = 0;


    /**
     * Replaces the child node <code>oldChild</code> with <code>newChild</code>
     * in the list of children, and returns the <code>oldChild</code> node.
     *
     * If <CODE>newChild</CODE> is a <CODE>DOMDocumentFragment</CODE> object,
     * <CODE>oldChild</CODE> is replaced by all of the <CODE>DOMDocumentFragment</CODE>
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
     * @since DOM Level 1
     */
    virtual DOMNode  *replaceChild(DOMNode *newChild,
                                     DOMNode *oldChild) = 0;
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
     * @since DOM Level 1
     */
    virtual DOMNode        *removeChild(DOMNode *oldChild) = 0;

    /**
     * Adds the node <code>newChild</code> to the end of the list of children of
     * this node.
     *
     * If the <code>newChild</code> is already in the tree, it is
     * first removed.
     * @param newChild The node to add.If it is a  <code>DOMDocumentFragment</code>
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
     * @since DOM Level 1
     */
    virtual DOMNode        *appendChild(DOMNode *newChild) = 0;

    // -----------------------------------------------------------------------
    //  Query methods
    // -----------------------------------------------------------------------
    /**
     *  This is a convenience method to allow easy determination of whether a
     * node has any children.
     *
     * @return  <code>true</code> if the node has any children,
     *   <code>false</code> if the node has no children.
     * @since DOM Level 1
     */
    virtual bool             hasChildNodes() const = 0;

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
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
     * @since DOM Level 1
     */
    virtual void              setNodeValue(const XMLCh  *nodeValue) = 0;
    //@}

    /** @name Functions introduced in DOM Level 2. */
    //@{
    /**
     * Puts all <CODE>DOMText</CODE>
     * nodes in the full depth of the sub-tree underneath this <CODE>DOMNode</CODE>,
     * including attribute nodes, into a "normal" form where only markup (e.g.,
     * tags, comments, processing instructions, CDATA sections, and entity
     * references) separates <CODE>DOMText</CODE>
     * nodes, i.e., there are neither adjacent <CODE>DOMText</CODE>
     * nodes nor empty <CODE>DOMText</CODE>
     * nodes. This can be used to ensure that the DOM view of a document is the
     * same as if it were saved and re-loaded, and is useful when operations
     * (such as XPointer lookups) that depend on a particular document tree
     * structure are to be used.
     * <P><B>Note:</B> In cases where the document contains <CODE>DOMCDATASections</CODE>,
     * the normalize operation alone may not be sufficient, since XPointers do
     * not differentiate between <CODE>DOMText</CODE>
     * nodes and <CODE>DOMCDATASection</CODE>
     * nodes.</P>
     *
     * @since DOM Level 2
     */
    virtual void              normalize() = 0;

    /**
     * Tests whether the DOM implementation implements a specific
     * feature and that feature is supported by this node.
     *
     * @param feature The string of the feature to test. This is the same
     * name as what can be passed to the method <code>hasFeature</code> on
     * <code>DOMImplementation</code>.
     * @param version This is the version number of the feature to test. In
     * Level 2, version 1, this is the string "2.0". If the version is not
     * specified, supporting any version of the feature will cause the
     * method to return <code>true</code>.
     * @return Returns <code>true</code> if the specified feature is supported
     * on this node, <code>false</code> otherwise.
     * @since DOM Level 2
     */
    virtual bool              isSupported(const XMLCh *feature,
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
     * such as <CODE>createElement</CODE> from the <CODE>DOMDocument</CODE>
     * interface, this is always <CODE>null</CODE>.
     *
     * @since DOM Level 2
     */
    virtual const XMLCh *         getNamespaceURI() const = 0;

    /**
     * Get the <em>namespace prefix</em>
     * of this node, or <code>null</code> if it is unspecified.
     *
     * @since DOM Level 2
     */
    virtual const XMLCh *          getPrefix() const = 0;

    /**
     * Returns the local part of the <em>qualified name</em> of this node.
     * <p>
     * For nodes created with a DOM Level 1 method, such as
     * <code>createElement</code> from the <code>DOMDocument</code> interface,
     * it is null.
     *
     * @since DOM Level 2
     */
    virtual const XMLCh *          getLocalName() const = 0;

    /**
     * Set the <em>namespace prefix</em> of this node.
     * <p>
     * Note that setting this attribute, when permitted, changes
     * the <CODE>nodeName</CODE> attribute, which holds the <EM>qualified
     * name</EM>, as well as the <CODE>tagName</CODE> and <CODE>name</CODE>
     * attributes of the <CODE>DOMElement</CODE> and <CODE>DOMAttr</CODE>
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
     * @since DOM Level 2
     */
    virtual void              setPrefix(const XMLCh * prefix) = 0;

    /**
     *  Returns whether this node (if it is an element) has any attributes.
     * @return <code>true</code> if this node has any attributes,
     *   <code>false</code> otherwise.
     * @since DOM Level 2
     */
    virtual bool              hasAttributes() const = 0;
    //@}

    /** @name Functions introduced in DOM Level 3. */
    //@{
    /**
     * Associate an object to a key on this node. The object can later be
     * retrieved from this node by calling <code>getUserData</code> with the
     * same key.
     *
     * Deletion of the user data remains the responsibility of the
     * application program; it will not be automatically deleted when
     * the nodes themselves are reclaimed.
     *
     * Both the parameter <code>data</code> and the returned object are
     * void pointer, it is applications' responsibility to keep track of
     * their original type.  Casting them to the wrong type may result
     * unexpected behavior.
     *
     * @param key The key to associate the object to.
     * @param data The object to associate to the given key, or
     *   <code>null</code> to remove any existing association to that key.
     * @param handler The handler to associate to that key, or
     *   <code>null</code>.
     * @return Returns the void* object previously associated to
     *   the given key on this node, or <code>null</code> if there was none.
     * @see getUserData
     *
     * @since DOM Level 3
     */
    virtual void*             setUserData(const XMLCh* key,
                                          void* data,
                                          DOMUserDataHandler* handler) = 0;

    /**
     * Retrieves the object associated to a key on a this node. The object
     * must first have been set to this node by calling
     * <code>setUserData</code> with the same key.
     * @param key The key the object is associated to.
     * @return Returns the <code>DOMObject</code> associated to the given key
     *   on this node, or <code>null</code> if there was none.
     * @see setUserData
     * @since DOM Level 3
     */
    virtual void*             getUserData(const XMLCh* key) const = 0;
    //@}

    // -----------------------------------------------------------------------
    //  Non-standard Extension
    // -----------------------------------------------------------------------
    /** @name Non-standard Extension */
    //@{
    /**
     * Called to indicate that this Node (and its associated children) is no longer in use
     * and that the implementation may relinquish any resources associated with it and
     * its associated children.
     *
     * If this is a document, any nodes it owns are also released.
     *
     * Access to a released object will lead to unexpected result.
     *
     * @exception DOMException
     *   INVALID_ACCESS_ERR: Raised if this Node has a parent and thus should not be released yet.
     */
    virtual void              release() = 0;
    //@}


};


#endif

