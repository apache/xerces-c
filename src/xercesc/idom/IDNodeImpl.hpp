#ifndef IDNodeImpl_HEADER_GUARD_
#define IDNodeImpl_HEADER_GUARD_

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

//
//  This file is part of the internal implementation of the C++ XML DOM.
//  It should NOT be included or used directly by application programs.
//
//  Applications should include the file <dom/DOM.hpp> for the entire
//  DOM API, or DOM_*.hpp for individual DOM classes, where the class
//  name is substituded for the *.
//

/**
 * A IDNodeImpl doesn't have any children, and can therefore only be directly
 * inherited by classes of nodes that never have any, such as Text nodes. For
 * other types, such as Element, classes must inherit from ParentNode.
 * <P>
 * All nodes in a single document must originate
 * in that document. (Note that this is much tighter than "must be
 * same implementation") Nodes are all aware of their ownerDocument,
 * and attempts to mismatch will throw WRONG_DOCUMENT_ERR.
 * <P>
 * However, to save memory not all nodes always have a direct reference
 * to their ownerDocument. When a node is owned by another node it relies
 * on its owner to store its ownerDocument. Parent nodes always store it
 * though, so there is never more than one level of indirection.
 * And when a node doesn't have an owner, ownerNode refers to its
 * ownerDocument.
 **/

#include <xercesc/util/XercesDefs.hpp>

class IDOM_NamedNodeMap;
class IDOM_NodeList;
class IDOM_Node;
class IDOM_Document;

class CDOM_EXPORT IDNodeImpl {
public:

    // data
    IDOM_Node                *fOwnerNode; // typically the parent but not always!
    unsigned short flags;

    static const unsigned short READONLY;
    static const unsigned short SYNCDATA;
    static const unsigned short SYNCCHILDREN;
    static const unsigned short OWNED;
    static const unsigned short FIRSTCHILD;
    static const unsigned short SPECIFIED;
    static const unsigned short IGNORABLEWS;
    static const unsigned short SETVALUE;
    static const unsigned short ID_ATTR;
	static const unsigned short USERDATA;
    static const unsigned short LEAFNODETYPE;
    static const unsigned short CHILDNODE;


public:
    IDNodeImpl(IDOM_Node *ownerDocument);
    IDNodeImpl(const IDNodeImpl &other);
    ~IDNodeImpl();

     IDOM_Node         *appendChild(IDOM_Node *newChild);
     IDOM_NamedNodeMap * getAttributes() const;
     IDOM_NodeList     * getChildNodes() const;
     IDOM_Node         * getFirstChild() const;
     IDOM_Node         * getLastChild() const;
     const XMLCh       * getLocalName() const;
     const XMLCh       * getNamespaceURI() const;
     IDOM_Node         * getNextSibling() const;
     const XMLCh       * getNodeValue() const;
     IDOM_Document     * getOwnerDocument() const;
     IDOM_Node         * getParentNode() const;
     const XMLCh       * getPrefix() const;
     IDOM_Node         * getPreviousSibling() const;
     void              * getUserData() const;
     bool                hasChildNodes() const;
     IDOM_Node         * insertBefore(IDOM_Node *newChild, IDOM_Node *refChild);
     void                normalize();
     IDOM_Node         * removeChild(IDOM_Node *oldChild);
     IDOM_Node         * replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild);
     void                setNodeValue(const XMLCh *value);
     void                setPrefix(const XMLCh *fPrefix);
     void                setReadOnly(bool readOnly, bool deep);
     void                setUserData(void *value);
     bool                supports(const XMLCh *feature, const XMLCh *version) const;

    static  bool         isKidOK(IDOM_Node *parent, IDOM_Node *child);

    //Utility, not part of DOM Level 2 API
    static const XMLCh * mapPrefix(const XMLCh *prefix,
	                                  const XMLCh *namespaceURI, short nType);

    static const XMLCh *getXmlnsString();
    static const XMLCh *getXmlnsURIString();
    static const XMLCh *getXmlString();
    static const XMLCh *getXmlURIString();

public: // should really be protected - ALH

     void setOwnerDocument(IDOM_Document *doc);

    /*
     * Flags setters and getters
     */

    inline bool isReadOnly() const {
        return (flags & READONLY) != 0;
    }

    inline void isReadOnly(bool value) {
        flags = (value ? flags | READONLY : flags & ~READONLY);
    }

    inline bool needsSyncData() const {
        return (flags & SYNCDATA) != 0;
    }

    inline void needsSyncData(bool value) {
        flags = (value ? flags | SYNCDATA : flags & ~SYNCDATA);
    }

    inline bool needsSyncChildren() const {
        return (flags & SYNCCHILDREN) != 0;
    }

    inline void needsSyncChildren(bool value) {
        flags = (value ? flags | SYNCCHILDREN : flags & ~SYNCCHILDREN);
    }

    // For Attributes, true if the attr node is attached to an element.
    // For all other node types, true if the node has a parent node.
    inline bool isOwned() const {
        return (flags & OWNED) != 0;
    }

    inline void isOwned(bool value) {
        flags = (value ? flags | OWNED : flags & ~OWNED);
    }

    inline bool isFirstChild() const {
        return (flags & FIRSTCHILD) != 0;
    }

    inline void isFirstChild(bool value) {
        flags = (value ? flags | FIRSTCHILD : flags & ~FIRSTCHILD);
    }

    inline bool isSpecified() const {
        return (flags & SPECIFIED) != 0;
    }

    inline void isSpecified(bool value) {
        flags = (value ? flags | SPECIFIED : flags & ~SPECIFIED);
    }

    inline bool ignorableWhitespace() const {
        return (flags & IGNORABLEWS) != 0;
    }

    inline void ignorableWhitespace(bool value) {
        flags = (value ? flags | IGNORABLEWS : flags & ~IGNORABLEWS);
    }

    inline bool setValue() const {
        return (flags & SETVALUE) != 0;
    }

    inline void setValue(bool value) {
        flags = (value ? flags | SETVALUE : flags & ~SETVALUE);
    }

    inline bool isIdAttr() const {
        return (flags & ID_ATTR) != 0;
    }

    inline void isIdAttr(bool value) {
        flags = (value ? flags | ID_ATTR : flags & ~ID_ATTR);
    }

    inline bool hasUserData() const {
        return (flags & USERDATA) != 0;
    }

    inline void hasUserData(bool value) {
        flags = (value ? flags | USERDATA : flags & ~USERDATA);
    }

    //
    //  LeafNode is set true for node types that can not be ParentNodes (can't have children)
    //    This knowledge is used to allow casting from any unknown node type to the
    //    IDParentImpl or IDChildImpl parts of the node.
    //
    inline bool isLeafNode() const {
        return (flags & LEAFNODETYPE) != 0;
    }

    inline void setIsLeafNode(bool value) {
        flags = (value ? flags | LEAFNODETYPE : flags & ~LEAFNODETYPE);
    }


    //
    // ChildNode is set true for node types that can be children of other nodes, and
    //   therefore include a IDChildNode data member.  Note that all of the leaf
    //   node types (above flag) are also ChildNodes, but not all ChildNodes are
    //   leaf nodes.
    inline bool isChildNode() const {
        return (flags & CHILDNODE) != 0;
    }

    inline void setIsChildNode(bool value) {
        flags = (value ? flags | CHILDNODE : flags & ~CHILDNODE);
    }
};


// This macro lists all of the pure virtual functions declared in IDOM_Node that must
//   be implemented by all node types.  Since there is no inheritance of implementation,
//   using this macro in the class declaration of the node types make it easier to
//   accurately get all of the functions declared.
//
#define IDOM_NODE_FUNCTIONS \
    virtual       IDOM_Node          *appendChild(IDOM_Node *newChild) ;\
    virtual       IDOM_Node          *cloneNode(bool deep) const ;\
    virtual       IDOM_NamedNodeMap  *getAttributes() const ;\
    virtual       IDOM_NodeList      *getChildNodes() const ;\
    virtual       IDOM_Node          *getFirstChild() const ;\
    virtual       IDOM_Node          *getLastChild() const ;\
    virtual const XMLCh *             getLocalName() const ;\
    virtual const XMLCh *             getNamespaceURI() const ;\
    virtual       IDOM_Node          *getNextSibling() const ;\
    virtual const XMLCh              *getNodeName() const ;\
    virtual       short               getNodeType() const ;\
    virtual const XMLCh              *getNodeValue() const ;\
    virtual       IDOM_Document      *getOwnerDocument() const ;\
    virtual const XMLCh *             getPrefix() const ;\
    virtual       IDOM_Node          *getParentNode() const ;\
    virtual       IDOM_Node          *getPreviousSibling() const ;\
    virtual       bool                hasChildNodes() const ;\
    virtual       IDOM_Node          *insertBefore(IDOM_Node *newChild, IDOM_Node *refChild) ;\
    virtual       void                normalize() ;\
    virtual       IDOM_Node          *removeChild(IDOM_Node *oldChild) ;\
    virtual       IDOM_Node          *replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild) ;\
    virtual       void                setNodeValue(const XMLCh  *nodeValue) ;\
    virtual       bool                supports(const XMLCh *feature, const XMLCh *version) const ;\
    virtual       void                setPrefix(const XMLCh * prefix)


/*
 *  Here are dummy stubs for most of the functions introduced by IDOM_Node.
 *    Each subclass of IDOM_Node will have something like this that delegates each
 *    function to the appropriate implementation.
 *    Functions that must be supplied by every node class are omitted.
 *
           IDOM_Node          *xxx::appendChild(IDOM_Node *newChild)        {return fParent.appendChild (newChild); };
           IDOM_NamedNodeMap  *xxx::getAttributes() const 			        {return fNode.getAttributes (); };
           IDOM_NodeList      *xxx::getChildNodes() const 			        {return fParent.getChildNodes (); };
           IDOM_Node          *xxx::getFirstChild() const 			        {return fParent.getFirstChild (); };
           IDOM_Node          *xxx::getLastChild() const 		            {return fParent.getLastChild (); };
     const XMLCh              *xxx::getLocalName() const                    {return fNode.getLocalName (); };
     const XMLCh              *xxx::getNamespaceURI() const                 {return fNode.getNamespaceURI (); };
           IDOM_Node          *xxx::getNextSibling() const                  {return fChild.getNextSibling (); };
     const XMLCh              *xxx::getNodeValue() const                    {return fNode.getNodeValue (); };
           IDOM_Document      *xxx::getOwnerDocument() const                {return fNode.getOwnerDocument (); };
     const XMLCh              *xxx::getPrefix() const                       {return fNode.getPrefix (); };
           IDOM_Node          *xxx::getParentNode() const                   {return fChild.getParentNode (this); };
           IDOM_Node          *xxx::getPreviousSibling() const              {return fChild.getPreviousSibling (this); };
           bool                xxx::hasChildNodes() const                   {return fParent.hasChildNodes (); };
           IDOM_Node          *xxx::insertBefore(IDOM_Node *newChild, IDOM_Node *refChild)
                                                                            {return fParent.insertBefore (newChild, refChild); };
           void                xxx::normalize()                             {fNode.normalize (); };
           IDOM_Node          *xxx::removeChild(IDOM_Node *oldChild)        {return fParent.removeChild (oldChild); };
           IDOM_Node          *xxx::replaceChild(IDOM_Node *newChild, IDOM_Node *oldChild)
                                                                            {return fParent.replaceChild (newChild, oldChild); };
           void                xxx::setNodeValue(const XMLCh  *nodeValue)   {fNode.setNodeValue (nodeValue); };
           bool                xxx::supports(const XMLCh *feature, const XMLCh *version) const
                                                                            {return fNode.supports (feature, version); };
           void                xxx::setPrefix(const XMLCh  *prefix)         {fNode.setPrefix(prefix); };

*/



#endif
