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
 * $Log$
 * Revision 1.1  1999/11/09 01:08:57  twl
 * Initial revision
 *
 * Revision 1.3  1999/11/08 20:44:17  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#ifndef DOM_Element_HEADER_GUARD_
#define DOM_Element_HEADER_GUARD_

#include <util/XML4CDefs.hpp>
#include <dom/DOM_Node.hpp>

class DOM_Attr;
class DOM_NodeList;
class ElementImpl;

/**
 * By far the vast majority of objects (apart from text) that authors 
 * encounter when traversing a document are <code>DOM_Element</code> nodes.  
 *
 * Assume the following XML document:&lt;elementExample id="demo"&gt; 
 * &lt;subelement1/&gt; 
 * &lt;subelement2&gt;&lt;subsubelement/&gt;&lt;/subelement2&gt;
 * &lt;/elementExample&gt;  
 * <p>When represented using DOM, the top node is an <code>DOM_Element</code> node 
 * for "elementExample", which contains two child <code>DOM_Element</code> nodes, 
 * one for "subelement1" and one for "subelement2". "subelement1" contains no 
 * child nodes.
 * <p>Elements may have attributes associated with them; since the 
 * <code>DOM_Element</code> interface inherits from <code>DOM_Node</code>, the generic
 *  <code>DOM_Node</code> interface method <code>getAttributes</code> may be used 
 * to retrieve the set of all attributes for an element.  There are methods on
 *  the <code>DOM_Element</code> interface to retrieve either an <code>DOM_Attr</code>
 *  object by name or an attribute value by name. In XML, where an attribute 
 * value may contain entity references, an <code>DOM_Attr</code> object should be 
 * retrieved to examine the possibly fairly complex sub-tree representing the 
 * attribute value. On the other hand, in HTML, where all attributes have 
 * simple string values, methods to directly access an attribute value can 
 * safely be used as a convenience.
 */

class CDOM_EXPORT DOM_Element: public DOM_Node {
private:

public:
    /** @name Constructors and assignment operator */
    //@{
    /**
    * Default constructor for DOM_Element.  The resulting object does not
    * refer to an actual Element node; it will compare == to 0, and is similar
    * to a null object reference variable in Java.  It may subsequently be
    * assigned to refer to an actual Element node.
    * <p>
    * New comment nodes are created by DOM_Document::createElement().
      *
      */
    DOM_Element();

    /**
      * Copy constructor.  Creates a new <code>DOM_Element</code> that refers to the
      * same underlying actual element as the original.
      *
      * @param other The object to be copied
      */
    DOM_Element(const DOM_Element &other);

    /**
      * Assignment operator.
      *
      * @param other The object to be copied.
      */
    DOM_Element & operator = (const DOM_Element &other);

    /**
      * Assignment operator.  This overloaded variant is provided for
      *   the sole purpose of setting a DOM_Node reference variable to
      *   zero.  Nulling out a reference variable in this way will decrement
      *   the reference count on the underlying Node object that the variable
      *   formerly referenced.  This effect is normally obtained when reference
      *   variable goes out of scope, but zeroing them can be useful for
      *   global instances, or for local instances that will remain in scope
      *   for an extended time,  when the storage belonging to the underlying
      *   node needs to be reclaimed.
      *
      * @param val.  Only a value of 0, or null, is allowed.
      */
    DOM_Element & operator = (const DOM_NullPtr *val);

    //@}
    /** @name Destructor. */
    //@{
	 /**
      * Destructor.  The object being destroyed is the reference
      * object, not the underlying Element itself.
	  *
	  */
    ~DOM_Element();
    //@}
    /** @name Getter functions. */
    //@{

  /**
   * The name of the element. 
   *
   * For example, in: &lt;elementExample 
   * id="demo"&gt;  ... &lt;/elementExample&gt; , <code>tagName</code> has 
   * the value <code>"elementExample"</code>. Note that this is 
   * case-preserving in XML, as are all of the operations of the DOM. 
   */
  DOMString         getTagName() const;

  /**
   * Retrieves an attribute value by name.
   *
   * @param name The name of the attribute to retrieve.
   * @return The <code>DOM_Attr</code> value as a string, or the empty  string if 
   *   that attribute does not have a specified or default value.
   */
  DOMString         getAttribute(const DOMString &name) const;

  /**
   * Retrieves an <code>DOM_Attr</code> node by name.
   *
   * @param name The name of the attribute to retrieve.
   * @return The <code>DOM_Attr</code> node with the specified attribute name or 
   *   <code>null</code> if there is no such attribute.
   */
  DOM_Attr        getAttributeNode(const DOMString &name) const;

  /**
   * Returns a <code>NodeList</code> of all descendant elements with a given 
   * tag name, in the order in which they would be encountered in a preorder 
   * traversal of the <code>DOM_Element</code> tree.
   *
   * @param name The name of the tag to match on. The special value "*" 
   *   matches all tags.
   * @return A list of matching <code>DOM_Element</code> nodes.
   */
  DOM_NodeList    getElementsByTagName(const DOMString &name) const;

  //@}
  /** @name Set functions. */
  //@{

  /**
   * Adds a new attribute. 
   * 
   * If an attribute with that name is already present 
   * in the element, its value is changed to be that of the value parameter. 
   * This value is a simple string, it is not parsed as it is being set. So 
   * any markup (such as syntax to be recognized as an entity reference) is 
   * treated as literal text, and needs to be appropriately escaped by the 
   * implementation when it is written out. In order to assign an attribute 
   * value that contains entity references, the user must create an 
   * <code>DOM_Attr</code> node plus any <code>Text</code> and 
   * <code>EntityReference</code> nodes, build the appropriate subtree, and 
   * use <code>setAttributeNode</code> to assign it as the value of an 
   * attribute.
   * @param name The name of the attribute to create or alter.
   * @param value Value to set in string form.
   * @exception DOMException
   *   INVALID_CHARACTER_ERR: Raised if the specified name contains an 
   *   invalid character.
   *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
   */
   void             setAttribute(const DOMString &name, 
                                 const DOMString &value);
   /**
    * Adds a new attribute. 
    * 
    * If an attribute with that name is already present 
    * in the element, it is replaced by the new one.
    * @param newAttr The <code>DOM_Attr</code> node to add to the attribute list.
    * @return If the <code>newAttr</code> attribute replaces an existing 
    *   attribute with the same name, the  previously existing 
    *   <code>DOM_Attr</code> node is returned, otherwise <code>null</code> is 
    *   returned.
    * @exception DOMException
    *   WRONG_DOCUMENT_ERR: Raised if <code>newAttr</code> was created from a 
    *   different document than the one that created the element.
    *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
    *   <br>INUSE_ATTRIBUTE_ERR: Raised if <code>newAttr</code> is already an 
    *   attribute of another <code>DOM_Element</code> object. The DOM user must 
    *   explicitly clone <code>DOM_Attr</code> nodes to re-use them in other 
    *   elements.
    */
   DOM_Attr        setAttributeNode(DOM_Attr newAttr);

   //@}
   /** @name Functions which modify the Element. */
   //@{
  /**
   * Removes the specified attribute.
   *
   * @param oldAttr The <code>DOM_Attr</code> node to remove from the attribute 
   *   list. If the removed <code>DOM_Attr</code> has a default value it is 
   *   immediately replaced.
   * @return The <code>DOM_Attr</code> node that was removed.
   * @exception DOMException
   *   NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
   *   <br>NOT_FOUND_ERR: Raised if <code>oldAttr</code> is not an attribute 
   *   of the element.
   */
  DOM_Attr        removeAttributeNode(DOM_Attr oldAttr);

  /**
   * Puts all <code>Text</code> nodes in the full depth of the sub-tree 
   * underneath this <code>DOM_Element</code> into a "normal" form.
    
   * In the "normal" form  
   * markup (e.g., tags, comments, processing instructions, CDATA sections, 
   * and entity references) separates <code>Text</code> nodes, i.e., there 
   * are no adjacent <code>Text</code> nodes.  This can be used to ensure 
   * that the DOM view of a document is the same as if it were saved and 
   * re-loaded, and is useful when operations (such as XPointer lookups) that 
   * depend on a particular document tree structure are to be used.
   */
  void              normalize();

  /**
   * Removes an attribute by name. 
   *
   * If the removed attribute has a default 
   * value it is immediately replaced.
   * @param name The name of the attribute to remove.
   * @exception DOMException
   *   NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
   */
  void              removeAttribute(const DOMString &name);

  //@}
  /** @name Functions introduced in DOM Level 2. */
  //@{

  /**
   * Retrieves an attribute value by local name and namespace URI.
   *
   * @param namespaceURI The <em>namespace URI</em> of
   *    the attribute to retrieve. When it is <code>null</code> or an empty
   *    string, this method behaves like <code>getAttribute</code>.
   * @param localName The <em>local name</em> of the
   *    attribute to retrieve.
   * @return The <code>DOM_Attr</code> value as a string, or an empty string if
   *    that attribute does not have a specified or default value.
   */
  DOMString         getAttributeNS(const DOMString &namespaceURI,
	const DOMString &localName) const;

  /**
   * Adds a new attribute. If an attribute with the same local name and
   * namespace URI is already present in the element, its prefix is changed
   * to be that of the <code>qualifiedName</code> parameter,
   * and its value is changed to be that of the <code>value</code> parameter.
   * This value is a simple string, it is
   * not parsed as it is being set. So any markup (such as syntax to be
   * recognized as an entity reference) is treated as literal text, and
   * needs to be appropriately escaped by the implementation when it is
   * written out. In order to assign an attribute value that contains entity
   * references, the user must create a <code>DOM_Attr</code> node plus any
   * <code>DOM_Text</code> and <code>DOM_EntityReference</code> nodes, build the
   * appropriate subtree, and use <code>setAttributeNodeNS</code> or
   * <code>setAttributeNode</code> to assign it as the value of an
   * attribute.
   * @param namespaceURI The <em>namespace URI</em> of
   *    the attribute to create or alter. When it is <code>null</code> or an empty
   *    string, this method behaves like <code>setAttribute</code>.
   * @param localName The <em>local name</em> of the
   *    attribute to create or alter.
   * @param value The value to set in string form.
   * @exception DOMException
   *   INVALID_CHARACTER_ERR: Raised if the specified name contains an 
   *   invalid character.
   *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
   */
   void             setAttributeNS(const DOMString &namespaceURI,
	const DOMString &qualifiedName, const DOMString &value);

  /**
   * Removes an attribute by local name and namespace URI. If the
   * removed attribute has a default value it is immediately replaced.
   *
   * @param namespaceURI The <em>namespace URI</em> of
   *    the attribute to remove. When it is <code>null</code> or an empty
   *    string, this method behaves like <code>removeAttribute</code>.
   * @param localName The <em>local name</em> of the
   *    attribute to remove.
   * @exception DOMException
   *   NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
   */
  void              removeAttributeNS(const DOMString &namespaceURI,
	const DOMString &localName);

  /**
   * Retrieves an <code>DOM_Attr</code> node by local name and namespace URI.
   *
   * @param namespaceURI The <em>namespace URI</em> of
   *    the attribute to retrieve. When it is <code>null</code> or an empty
   *    string, this method behaves like <code>getAttributeNode</code>.
   * @param localName The <em>local name</em> of the
   *    attribute to retrieve.
   * @return The <code>DOM_Attr</code> node with the specified attribute local
   *    name and namespace URI or <code>null</code> if there is no such attribute.
   */
  DOM_Attr        getAttributeNodeNS(const DOMString &namespaceURI,
	const DOMString &localName) const;

   /**
    * Adds a new attribute. 
    * 
    * If an attribute with that local name and namespace URI is already present 
    * in the element, it is replaced by the new one.
    * @param newAttr The <code>DOM_Attr</code> node to add to the attribute list.
    *    When the node has no <code>namespaceURI</code>, this method behaves like
    *    <code>setAttributeNode</code>.
    * @return If the <code>newAttr</code> attribute replaces an existing
    *    attribute with the same <em>local name</em> and <em>namespace URI</em>,
    *    the previously existing <code>DOM_Attr</code> node is
    *    returned, otherwise <code>null</code> is returned.
    * @exception DOMException
    *   WRONG_DOCUMENT_ERR: Raised if <code>newAttr</code> was created from a 
    *   different document than the one that created the element.
    *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
    *   <br>INUSE_ATTRIBUTE_ERR: Raised if <code>newAttr</code> is already an 
    *   attribute of another <code>DOM_Element</code> object. The DOM user must 
    *   explicitly clone <code>DOM_Attr</code> nodes to re-use them in other 
    *   elements.
    */
   DOM_Attr        setAttributeNodeNS(DOM_Attr newAttr);

  /**
   * Returns a <code>DOM_NodeList</code> of all the <code>DOM_Element</code>s
   * with a given local name and namespace URI in the order in which they
   * would be encountered in a preorder traversal of the
   * <code>DOM_Document</code> tree, starting from this node.
   *
   * @param namespaceURI The <em>namespace URI</em> of
   *    the elements to match on. The special value "*" matches all
   *    namespaces. When it is <code>null</code> or an empty string, this
   *    method behaves like <code>getElementsByTagName</code>.
   * @param localName The <em>local name</em> of the
   *    elements to match on. The special value "*" matches all local names.
   * @return A new <code>DOM_NodeList</code> object containing all the matched
   *    <code>Element</code>s.
   */
  DOM_NodeList    getElementsByTagNameNS(const DOMString &namespaceURI,
	const DOMString &localName) const;

  //@}

  private:
     DOM_Element(ElementImpl *impl);

     friend class DOM_Document;
     friend class DOM_Attr;

};

#endif


