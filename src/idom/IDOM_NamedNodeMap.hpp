#ifndef IDOM_NamedNodeMap_HEADER_GUARD_
#define IDOM_NamedNodeMap_HEADER_GUARD_


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
 * Revision 1.1  2001/04/03 00:14:30  andyh
 * Initial revision
 *
 * Revision 1.13  2000/06/07 22:49:40  andyh
 * Memory usage reduction:  DOM NamedNodeMaps for attributes are allocated
 * only for elements that actually have attributes.  By Joe Polastre.
 *
 * Revision 1.12  2000/03/11 03:19:13  chchou
 * Fix bug # 19, add const keyword to API
 *
 * Revision 1.11  2000/03/02 19:53:56  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.10  2000/02/24 20:11:28  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.9  2000/02/17 17:47:24  andyh
 * Update Doc++ API comments
 * NameSpace update to track W3C
 * Changes were made by Chih Hsiang Chou
 *
 * Revision 1.8  2000/02/15 23:17:36  andyh
 * Update Doc++ API comments
 * NameSpace bugfix and update to track W3C
 * Chih Hsiang Chou
 *
 * Revision 1.7  2000/02/10 20:20:42  abagchi
 * Added docs for assignment operators
 *
 * Revision 1.6  2000/02/10 20:19:24  abagchi
 * Added docs for equality operators
 *
 * Revision 1.5  2000/02/06 07:47:30  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.4  2000/02/04 01:49:28  aruna1
 * TreeWalker and NodeIterator changes
 *
 * Revision 1.3  2000/01/22 01:38:29  andyh
 * Remove compiler warnings in DOM impl classes
 *
 * Revision 1.2  2000/01/05 01:16:07  andyh
 * DOM Level 2 core, namespace support added.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:59  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:18  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#include <util/XercesDefs.hpp>

class IDOM_Node;

/**
*  <code>NamedNodeMap</code>s  are used to 
* represent collections of nodes that can be accessed by name.
*
* Note that <code>NamedNodeMap</code> does not inherit from <code>NodeList</code>; 
* <code>NamedNodeMap</code>s are not maintained in any particular order. 
* Nodes contained in a <code>NamedNodeMap</code> may 
* also be accessed by an ordinal index, but this is simply to allow 
* convenient enumeration of the contents, and 
* does not imply that the DOM specifies an order to these Nodes. 
*/
class CDOM_EXPORT IDOM_NamedNodeMap {
protected:
    IDOM_NamedNodeMap() {};
    IDOM_NamedNodeMap(const IDOM_NamedNodeMap &other) {};
    IDOM_NamedNodeMap & operator = (const IDOM_NamedNodeMap &other) {return *this;};
  


public:
    /** @name Destructor. */
    //@{
    virtual ~IDOM_NamedNodeMap() {};
    
    //@}

    /** @name Set functions. */
    //@{

    /**
    * Adds a node using its <code>nodeName</code> attribute. 
    *
    * <br>As the <code>nodeName</code> attribute is used to derive the name 
    * which the node must be stored under, multiple nodes of certain types 
    * (those that have a "special" string value) cannot be stored as the names 
    * would clash. This is seen as preferable to allowing nodes to be aliased.
    * @param arg A node to store in a named node map. The node will later be 
    *   accessible using the value of the <code>nodeName</code> attribute of 
    *   the node. If a node with that name is already present in the map, it 
    *   is replaced by the new one.
    * @return If the new <code>Node</code> replaces an existing node the
    *   replaced <code>Node</code> is returned, 
    *   otherwise <code>null</code> is returned.
    * @exception DOMException
    *   WRONG_DOCUMENT_ERR: Raised if <code>arg</code> was created from a 
    *   different document than the one that created the 
    *   <code>NamedNodeMap</code>.
    *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this 
    *   <code>NamedNodeMap</code> is readonly.
    *   <br>INUSE_ATTRIBUTE_ERR: Raised if <code>arg</code> is an 
    *   <code>Attr</code> that is already an attribute of another 
    *   <code>Element</code> object. The DOM user must explicitly clone 
    *   <code>Attr</code> nodes to re-use them in other elements.
    */
    virtual IDOM_Node   *setNamedItem(IDOM_Node *arg) = 0;

    //@}
    /** @name Get functions. */
    //@{

    /**
    * Returns the <code>index</code>th item in the map. 
    *
    * If <code>index</code> 
    * is greater than or equal to the number of nodes in the map, this returns 
    * <code>null</code>.
    * @param index Index into the map.
    * @return The node at the <code>index</code>th position in the 
    *   <code>NamedNodeMap</code>, or <code>null</code> if that is not a valid 
    *   index.
    */
    virtual IDOM_Node     *item(unsigned int index) const = 0;

    /**
    * Retrieves a node specified by name.
    *
    * @param name The <code>nodeName</code> of a node to retrieve.
    * @return A <code>IDOM_Node</code> (of any type) with the specified <code>nodeName</code>, or 
    *   <code>null</code> if it does not identify any node in 
    *   the map. 
    */
    virtual IDOM_Node   *getNamedItem(const XMLCh *name) const = 0;

    /**
    * The number of nodes in the map. 
    *
    * The range of valid child node indices is 
    * 0 to <code>length-1</code> inclusive. 
    */
    virtual unsigned int   getLength() const = 0;

    //@}
    /** @name Functions to change the node collection. */
    //@{

    /**
    * Removes a node specified by name.
    *
    * If the removed node is an 
    * <code>Attr</code> with a default value it is immediately replaced.
    * @param name The <code>nodeName</code> of a node to remove.
    * @return The node removed from the map or <code>null</code> if no node 
    *   with such a name exists.
    * @exception DOMException
    *   NOT_FOUND_ERR: Raised if there is no node named <code>name</code> in 
    *   the map.
    * <br>
    *   NO_MODIFICATION_ALLOWED_ERR: Raised if this <code>NamedNodeMap</code>
    *   is readonly.
    */
    virtual IDOM_Node    *removeNamedItem(const XMLCh *name) = 0;

    //@}
    /** @name Functions introduced in DOM Level 2. */
    //@{

    /**
     * Retrieves a node specified by local name and namespace URI.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @param namespaceURI The <em>namespace URI</em> of
     *    the node to retrieve.
     * @param localName The <em>local name</em> of the node to retrieve.
     * @return A <code>IDOM_Node</code> (of any type) with the specified
     *    local name and namespace URI, or <code>null</code> if they do not
     *    identify any node in the map.
     */
    virtual IDOM_Node   *getNamedItemNS(const XMLCh *namespaceURI,
	                                        const XMLCh *localName) const = 0;

    /**
     * Adds a node using its <CODE>namespaceURI</CODE> and <CODE>localName</CODE>.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @param arg A node to store in a named node map. The node will later be 
     *       accessible using the value of the <CODE>namespaceURI</CODE> and 
     *       <CODE>localName</CODE> attribute of the node. If a node with those 
     *       namespace URI and local name is already present in the map, it is 
     *       replaced by the new one.
     * @return If the new <code>IDOM_Node</code> replaces an existing node the
     *   replaced <code>IDOM_Node</code> is returned, 
     *   otherwise <code>null</code> is returned.
     * @exception DOMException
     *   WRONG_DOCUMENT_ERR: Raised if <code>arg</code> was created from a 
     *   different document than the one that created the 
     *   <code>IDOM_NamedNodeMap</code>.
     *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this 
     *   <code>vNamedNodeMap</code> is readonly.
     *   <br>INUSE_ATTRIBUTE_ERR: Raised if <code>arg</code> is an 
     *   <code>DOM_Attr</code> that is already an attribute of another 
     *   <code>DOM_Element</code> object. The DOM user must explicitly clone 
     *   <code>DOM_Attr</code> nodes to re-use them in other elements.
     */
    virtual IDOM_Node   *setNamedItemNS(IDOM_Node *arg) = 0;

    /**
     * Removes a node specified by local name and namespace URI.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @param namespaceURI The <em>namespace URI</em> of
     *    the node to remove.
     * @param localName The <em>local name</em> of the
     *    node to remove. When this <code>IDOM_NamedNodeMap</code> contains the
     *    attributes attached to an element, as returned by the attributes
     *    attribute of the <code>IDOM_Node</code> interface, if the removed
     *    attribute is known to have a default value, an attribute
     *    immediately appears containing the default value
     *    as well as the corresponding namespace URI, local name, and prefix.
     * @return The node removed from the map if a node with such a local name
     *    and namespace URI exists.
     * @exception DOMException
     *   NOT_FOUND_ERR: Raised if there is no node named <code>name</code> in 
     *   the map.
     * <br>
     *   NO_MODIFICATION_ALLOWED_ERR: Raised if this <code>IDOM_NamedNodeMap</code>
     *   is readonly.
     */
    virtual IDOM_Node     *removeNamedItemNS(const XMLCh *namespaceURI,
	                                          const XMLCh *localName) = 0;

    //@}

};

#endif

