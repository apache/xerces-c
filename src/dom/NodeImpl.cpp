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

/**
* $Log$
* Revision 1.10  2000/02/06 07:47:33  rahulj
* Year 2K copyright swat.
*
* Revision 1.9  2000/02/04 01:49:26  aruna1
* TreeWalker and NodeIterator changes
*
* Revision 1.8  2000/01/22 01:38:30  andyh
* Remove compiler warnings in DOM impl classes
*
* Revision 1.7  2000/01/19 21:39:19  andyh
* DOM L2, fix problems with new style createDocument.
*
* Revision 1.6  2000/01/08 00:09:28  andyh
* Correcf failures in DOMTest with entity references and read-only nodes.
* Correct reference counting problem NamedNodeMap.
* Add export methods to NamedNodeMap and DocumentTypeImpl.
* Redo DocumentImpl::cloneNode
*
* (Changes by Chih-Hsiang Chou)
*
* Revision 1.5  2000/01/06 19:43:25  aruna1
* Modifed ?: consturct on solaris to assign DOMString objects
*
* Revision 1.4  2000/01/05 01:16:08  andyh
* DOM Level 2 core, namespace support added.
*
* Revision 1.3  1999/12/03 00:11:23  andyh
* Added DOMString.clone() to node parameters in and out of the DOM,
* where they had been missed.
*
* DOMString::rawBuffer, removed incorrect assumptions about it
* being null terminated.
*
* Revision 1.2  1999/11/30 21:16:25  roddey
* Changes to add the transcode() method to DOMString, which returns a transcoded
* version (to local code page) of the DOM string contents. And I changed all of the
* exception 'throw by pointer' to 'throw by value' style.
*
* Revision 1.1.1.1  1999/11/09 01:09:13  twl
* Initial checkin
*
* Revision 1.3  1999/11/08 20:44:29  rahul
* Swat for adding in Product name and CVS comment log variable.
*
*/


#include "NodeImpl.hpp"
#include "DOM_DOMException.hpp"
#include "DOM_Node.hpp"
#include "DOM_DOMImplementation.hpp"
#include "DOMString.hpp"
#include "DStringPool.hpp"
#include "DocumentImpl.hpp"
#include "stdio.h"
#include <util/XMLString.hpp>
#include <util/XMLUni.hpp>
#include "TextImpl.hpp"

static DOMString *s_xml = null;
static DOMString *s_xmlURI = null;
static DOMString *s_xmlns = null;
static DOMString *s_xmlnsURI = null;

NodeImpl::NodeImpl(DocumentImpl *ownerDoc,
                   const DOMString &nam,  short nTyp,
                   bool isLeafNod, const DOMString &initValue)
{
    // Do we want to add isLeafNode to this? How about initial value?
    this->ownerDocument=ownerDoc;
    this->namespaceURI=null;	//DOM Level 2
    this->prefix=null;			//DOM Level 2
    this->localName=null;		//DOM Level 2
    this->name=nam.clone();
    this->nType=nTyp;
    this->isLeafNode=isLeafNod;
    this->value=initValue.clone();
    
    this->changes = 0;
    this->userData = null;
    this->readOnly = false;
    this->owned    = false;
    this->firstChild = null;
    this->lastChild  = null;
    this->previousSibling  = null;
    this->nextSibling  = null;
    this->parentNode  = null;
    
    this->nodeRefCount = 0;
    NodeImpl::gLiveNodeImpls++; 
    NodeImpl::gTotalNodeImpls++;
};  


//Introduced in DOM Level 2
NodeImpl::NodeImpl(DocumentImpl *ownerDoc,
                   const DOMString &fNamespaceURI, const DOMString &qualifiedName, short nTyp,
                   bool isLeafNod, const DOMString &initValue)
{
    DOMString xmlns = DStringPool::getStaticString("xmlns", &s_xmlns);
    DOMString xmlnsURI = DStringPool::getStaticString("http://www.w3.org/2000/xmlns/", &s_xmlnsURI);
    // Do we want to add isLeafNode to this? How about initial value?
    this->ownerDocument=ownerDoc;

    //Check if qualifiedName = prefix:localName, name or malformed
    this->name = qualifiedName.clone();
    XMLCh *qNameP = this->name.rawBuffer();
    int qNameLen = this->name.length();	//note: qNameP[qNameLen] may not be 0
    int index = -1, count = 0;
    for (int i = 0; i < qNameLen; ++i)
	if (*qNameP++ == chColon) {
	    index = i;
	    ++count;	//number of ':' found
	}
    if (qNameLen == 0 || count > 1 || index == 0 || index == qNameLen-1)
	throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);

    bool xmlnsAlone = false;	//true if attribute name is "xmlns"
    if (count == 0) {	//count == 0 && index == -1
        if (nTyp == DOM_Node::ATTRIBUTE_NODE && this->name.equals(xmlns)) {
	    if (fNamespaceURI != null && fNamespaceURI.length() != 0 &&
		!fNamespaceURI.equals(xmlnsURI))
		throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);
	    xmlnsAlone = true;
	}
	this -> prefix = null;
	this -> localName = this -> name;
    } else {	//count == 1 && 0 < index < qNameLen-1
	this -> prefix = this->name.substringData(0, index);
	this -> localName = this->name.substringData(index+1, qNameLen-index-1);
    }

    const DOMString& URI = xmlnsAlone ? xmlnsURI : mapPrefix(prefix, fNamespaceURI, nTyp);
    this -> namespaceURI = URI == null ? DOMString(null) : URI.clone();

    this->nType=nTyp;
    this->isLeafNode=isLeafNod;
    this->value=initValue.clone();
    
    this->changes = 0;
    this->userData = null;
    this->readOnly = false;
    this->owned    = false;
    this->firstChild = null;
    this->lastChild  = null;
    this->previousSibling  = null;
    this->nextSibling  = null;
    this->parentNode  = null;
    
    this->nodeRefCount = 0;
    NodeImpl::gLiveNodeImpls++; 
    NodeImpl::gTotalNodeImpls++;
};  


NodeImpl::NodeImpl(const NodeImpl &other, bool deep) {
    this->nType = other.nType;
    this->namespaceURI = other.namespaceURI.clone();	//DOM Level 2
    this->prefix = other.prefix.clone();                //DOM Level 2
    this->localName = other.localName.clone();          //DOM Level 2
    this->name  = other.name.clone();
    this->value = other.value.clone();
    this->isLeafNode = other.isLeafNode;
    this->readOnly = false;
    this->owned = false;
    this->ownerDocument = other.ownerDocument;
    this->userData = other.userData;
    this->changes = 0;
    
    this->nodeRefCount = 0;
    NodeImpl::gLiveNodeImpls++; 
    NodeImpl::gTotalNodeImpls++;
    
    
    // Need to break the association w/ original kids
    this->previousSibling = null;
    this->nextSibling = null;
    this->parentNode = null;
    this->firstChild = null;
    this->lastChild = null;
    
    // Then, if deep, clone the kids too.
    if (deep)
    {
        for (NodeImpl *mykid = other.firstChild; 
        mykid != null; 
        mykid = mykid->nextSibling)
            this->appendChild(mykid->cloneNode(true));
    }
    
};


int  NodeImpl::gLiveNodeImpls = 0;         // Counters for debug & tuning.
int  NodeImpl::gTotalNodeImpls= 0;



NodeImpl::~NodeImpl() {
    NodeImpl::gLiveNodeImpls--;
};


// Dynamic Cast substitute functions
bool NodeImpl::isAttrImpl()              {return false;};
bool NodeImpl::isCDATASectionImpl()      {return false;};
bool NodeImpl::isDocumentFragmentImpl()  {return false;};
bool NodeImpl::isDocumentImpl()          {return false;};
bool NodeImpl::isDocumentTypeImpl()      {return false;};
bool NodeImpl::isElementImpl()           {return false;};
bool NodeImpl::isEntityReference()       {return false;};
bool NodeImpl::isTextImpl()              {return false;};


NodeImpl * NodeImpl::appendChild(NodeImpl *newChild)      
{
    return insertBefore(newChild, null);
};



void NodeImpl::changed()
{
    for (NodeImpl *n=this; n != null; n=n->parentNode)
        ++n->changes;
};  


NodeImpl * NodeImpl::cloneNode(bool deep) {
    NodeImpl *newnode;
    newnode = new NodeImpl(*this, deep);
    return newnode;
};


//  NodeImpl::deleteIf is called when a node's reference count goes
//  to 0.  It is separate function from removeRef because removeRef
//  is likely to be in-lined.
//
//  See comments at RefCountedImpl::removeRef().
//
void NodeImpl::deleteIf(NodeImpl *thisNode)
{
    if (thisNode == 0)
        return;
    
    if (thisNode->parentNode != 0 || thisNode->owned)
        return;
    
    // Delete this node.  There should be no siblings, as the DOM
    //  supports no node operations that would detach a node from
    //  its parent while retaining siblings.
    //  The target node may have children, in which case they must
    //  be removed from this node before deleting this node.
    thisNode->readOnly = false;   // removeChild requires node not be readonly.
    NodeImpl *theNextChild;
    for (NodeImpl *child = thisNode->firstChild; child != 0; child=theNextChild)
    {
        theNextChild = child->nextSibling;
        thisNode->removeChild(child);
        if (child->nodeRefCount == 0)
            deleteIf(child);
    }
    delete thisNode;
};



NamedNodeMapImpl * NodeImpl::getAttributes() {
    return null; // overridden in ElementImpl
};


NodeListImpl *NodeImpl::getChildNodes() {
    return this;
};



NodeImpl * NodeImpl::getFirstChild() {
    return firstChild;
}; 


NodeImpl * NodeImpl::getLastChild()
{
    return lastChild;
}; 


unsigned int NodeImpl::getLength() {
    unsigned int count = 0;
    NodeImpl *node = firstChild;
    while(node != null)
    {
        ++count;
        node = node->nextSibling;
    }
    return count;
};


NodeImpl * NodeImpl::getNextSibling() {
    return nextSibling;
}; 



DOMString NodeImpl::getNodeName() {
    return name;
};


short NodeImpl::getNodeType()
{
    return nType;
};



DOMString NodeImpl::getNodeValue()
{
    return value;
};


DocumentImpl *NodeImpl::getOwnerDocument()
{
    return ownerDocument;
};  


NodeImpl * NodeImpl::getParentNode()
{
    return parentNode;
};  


NodeImpl*  NodeImpl::getPreviousSibling()
{
    return previousSibling;
}; 


void *NodeImpl::getUserData()
{
    return userData;
};  


bool NodeImpl::hasChildNodes()
{ 
    return firstChild!=null;
}; 



NodeImpl *NodeImpl::insertBefore(NodeImpl *newChild, NodeImpl *refChild) {
    if (readOnly)
        throw DOM_DOMException(
        DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    
    if(  !(newChild->getOwnerDocument() == ownerDocument   ||
        // SPECIAL-CASE: Document has no owner, but may be the owner.
        ( this->isDocumentImpl() &&
        newChild->getOwnerDocument() == (DocumentImpl *)this ) 
        ) )
        throw DOM_DOMException(DOM_DOMException::WRONG_DOCUMENT_ERR, null);
    
    // Convert to internal type, to avoid repeated casting  
    //   (left over from the original Java.  Meaningless in this version.)
    NodeImpl * newInternal= newChild;
    
    // Prevent cycles in the tree
    bool treeSafe=true;
    for(NodeImpl *a=this->parentNode;treeSafe && a!=null;a=a->parentNode)
        treeSafe=(newInternal!=a);
    if(!treeSafe)
        throw DOM_DOMException(DOM_DOMException::HIERARCHY_REQUEST_ERR,null);
    
    // refChild must in fact be a child of this node (or null)
    if(refChild!=null && refChild->parentNode != this)
        throw DOM_DOMException(DOM_DOMException::NOT_FOUND_ERR,null);
    
    if (newInternal->isDocumentFragmentImpl())
    {
        // SLOW BUT SAFE: We could insert the whole subtree without
        // juggling so many next/previous pointers. (Wipe out the
        // parent's child-list, patch the parent pointers, set the
        // ends of the list.) But we know some subclasses have special-
        // case behavior they add to insertBefore(), so we don't risk it.
        // This approch also takes fewer bytecodes.
        
        // NOTE: If one of the children is not a legal child of this
        // node, throw HIERARCHY_REQUEST_ERR before _any_ of the children
        // have been transferred. (Alternative behaviors would be to
        // reparent up to the first failure point or reparent all those
        // which are acceptable to the target node, neither of which is
        // as robust. PR-DOM-0818 isn't entirely clear on which it
        // recommends?????
        
        // No need to check kids for right-document; if they weren't,
        // they wouldn't be kids of that DocFrag.
        for(NodeImpl *kid=newInternal->getFirstChild(); // Prescan
        kid!=null;
        kid=kid->getNextSibling())
        {
            if(!isKidOK(this,kid))
                throw DOM_DOMException(DOM_DOMException::HIERARCHY_REQUEST_ERR,null);
        }                       
        while(newInternal->hasChildNodes())     // Move
            insertBefore(newInternal->getFirstChild(),refChild);
    }
    
    else if(!isKidOK(this, newInternal))
        throw DOM_DOMException(DOM_DOMException::HIERARCHY_REQUEST_ERR,null);
    
    else
    {
        NodeImpl *oldparent=newInternal->parentNode;
        if(oldparent!=null)
            oldparent->removeChild(newInternal);
        
        NodeImpl *prev;
        // Find the node we're inserting after, if any (null if
        // inserting to head of list)
        prev= (refChild==null)
            ? lastChild
            : refChild->previousSibling;
        
        // Attach up
        newInternal->parentNode=this;
        
        // Attach after
        newInternal->previousSibling=prev;
        if(prev==null)
            firstChild=newInternal;
        else
            prev->nextSibling=newInternal;
        
        // Attach before
        newInternal->nextSibling = refChild;
        if(refChild==null)
            lastChild=newInternal;
        else
            refChild->previousSibling=newInternal;
    }
    changed();
    
    return newInternal;
};


/** Uses the kidOK lookup table to check whether the proposed
tree structure is legal.

  ????? It feels like there must be a more efficient solution,
  but for the life of me I can't think what it would be.
  */
  bool NodeImpl::isKidOK(NodeImpl *parent, NodeImpl *child)
  {
      static int kidOK[13];
      
      if (kidOK[DOM_Node::DOCUMENT_NODE] == 0)
      {
          kidOK[DOM_Node::DOCUMENT_NODE] = 
              1 << DOM_Node::ELEMENT_NODE |
              1 << DOM_Node::PROCESSING_INSTRUCTION_NODE | 
              1 << DOM_Node::COMMENT_NODE | 
              1 << DOM_Node::DOCUMENT_TYPE_NODE;
          
          kidOK[DOM_Node::DOCUMENT_FRAGMENT_NODE] = 
              kidOK[DOM_Node::ENTITY_NODE] = 
              kidOK[DOM_Node::ENTITY_REFERENCE_NODE] = 
              kidOK[DOM_Node::ELEMENT_NODE] = 
              1 << DOM_Node::ELEMENT_NODE |
              1 << DOM_Node::PROCESSING_INSTRUCTION_NODE | 
              1 << DOM_Node::COMMENT_NODE |
              1 << DOM_Node::TEXT_NODE |
              1 << DOM_Node::CDATA_SECTION_NODE |
              1 << DOM_Node::ENTITY_REFERENCE_NODE;
          
          kidOK[DOM_Node::DOCUMENT_TYPE_NODE] = 
              1 << DOM_Node::NOTATION_NODE |
              1 << DOM_Node::ENTITY_NODE;
          
          kidOK[DOM_Node::ATTRIBUTE_NODE] = 
              1 << DOM_Node::TEXT_NODE |
              1 << DOM_Node::ENTITY_REFERENCE_NODE;
          
          kidOK[DOM_Node::PROCESSING_INSTRUCTION_NODE] = 
              kidOK[DOM_Node::COMMENT_NODE] = 
              kidOK[DOM_Node::TEXT_NODE] = 
              kidOK[DOM_Node::CDATA_SECTION_NODE] = 
              kidOK[DOM_Node::NOTATION_NODE] = 
              0;
      };
      int p=parent->getNodeType();
      int ch = child->getNodeType();
      return (kidOK[p] & 1<<ch) != 0;
  };
  
  
  
  NodeImpl *NodeImpl::item(unsigned int index) {
      NodeImpl *node = firstChild;
      for(unsigned int i=0; i<index && node!=null; ++i)
          node = node->nextSibling;
      return node;
  };
  
  
  void NodeImpl::referenced()
  {
      RefCountedImpl::addRef(this->ownerDocument);
  };
  
  
  
  NodeImpl *NodeImpl::removeChild(NodeImpl *oldChild) 
  {
      if (readOnly)
          throw DOM_DOMException(
          DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
      
      if (oldChild != null && oldChild->parentNode != this)
          throw DOM_DOMException(DOM_DOMException::NOT_FOUND_ERR, null);
      
      // Patch tree past oldChild
      NodeImpl *prev = oldChild->previousSibling;
      NodeImpl *next = oldChild->nextSibling;
      if (prev != null)
          prev->nextSibling = next;
      else
          firstChild = next;
      if (next != null)
          next->previousSibling = prev;
      else
          lastChild = prev;
      
      // Remove oldChild's references to tree
      oldChild->parentNode = null;
      oldChild->nextSibling = null;
      oldChild->previousSibling = null;
      changed();
      return oldChild;
  };
  
  
  //
  //    unreferenced  will be called whenever the refernce count on
  //            this node goes from 1 to 0.  This node will only be
  //            directly deleted here  (by deleteIf) if it is outside 
  //            of the document tree.  
  //
  void NodeImpl::unreferenced()
  {
      DocumentImpl *doc = this->ownerDocument;
      deleteIf(this);       // This gets nodes outside of the document -
      //   deleteIf() deletes only if the parent
      //   node is null.  
      
      // If this was the last external reference within the document,
      //    the entire document will be deleted as well.
      RefCountedImpl::removeRef(doc);
  };
  
  
  
  NodeImpl *NodeImpl::replaceChild(NodeImpl *newChild, NodeImpl *oldChild)
  {
  /********* insertBefore should already do this test. ****************
  if (newChild.getOwnerDocument() != ownerDocument
  // SPECIAL-CASE: Document has no Owner.
  &&
  this instanceof Document && 
  newChild.getOwnerDocument() != (Document) this)
  {
  throw DOMExceptionImpl(DOMException.WRONG_DOCUMENT_ERR, null);
  }
      *********************************************************************/
      insertBefore(newChild, oldChild);
      // changed() already done.
      return removeChild(oldChild);
  };
  
  
  
  
  void NodeImpl::setNodeValue(const DOMString &val)
  {
      if (readOnly)
          throw DOM_DOMException(
          DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
      
      // Default behavior, overridden in some subclasses
      this->value = val.clone();
  };
  
  
  
  
  void NodeImpl::setReadOnly(bool readOnl, bool deep)
  {
      this->readOnly = readOnl;
      
      if (deep)
          // Recursively set kids
          for (NodeImpl *mykid = firstChild; 
          mykid != null; 
          mykid = mykid->nextSibling)
              if(! (mykid->isEntityReference()))
                  mykid->setReadOnly(readOnl,true);
  };
  
  
  
  void NodeImpl::setUserData(void * val)
  {
      userData = val;
  };  
  
  
  
  DOMString NodeImpl::toString()
  {
      return DOMString("[")+getNodeName()+": "+getNodeValue()+"]";
      // return getNodeName();
  };    
  
//Introduced in DOM Level 2
  
void NodeImpl::normalize()
{
    NodeImpl *kid, *next;
    for (kid = getFirstChild(); kid != null; kid = next)
    {
        next = kid->getNextSibling();
        
        // If kid and next are both Text nodes (but _not_ CDATASection,
        // which is a subclass of Text), they can be merged.
        if (next != null && 
            kid->isTextImpl()   && !(kid->isCDATASectionImpl())  && 
            next->isTextImpl()  && !(next->isCDATASectionImpl()) )
        {
            ((TextImpl *) kid)->appendData(((TextImpl *) next)->getData());
            removeChild(next);
            if (next->nodeRefCount == 0)
                deleteIf(next);
            next = kid; // Don't advance; there might be another.
        }
        
        // Otherwise it might be an Element, which is handled recursively  
        else
            if (kid->isElementImpl())
                kid->normalize();
    };
    
    // changed() will have occurred when the removeChild() was done,
    // so does not have to be reissued.
};


bool NodeImpl::supports(const DOMString &feature, const DOMString &version)
{
    return DOM_DOMImplementation::getImplementation().hasFeature(feature, version);
}

DOMString NodeImpl::getNamespaceURI()
{
    return namespaceURI;
}

DOMString NodeImpl::getPrefix()
{
    return prefix;
}

DOMString NodeImpl::getLocalName()
{
    return localName;
}


void NodeImpl::setPrefix(const DOMString &fPrefix)
{
    DOMString xml = DStringPool::getStaticString("xml", &s_xml);
    DOMString xmlURI = DStringPool::getStaticString("http://www.w3.org/XML/1998/namespace", &s_xmlURI);
    DOMString xmlns = DStringPool::getStaticString("xmlns", &s_xmlns);

    if (readOnly)
	throw DOM_DOMException(DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
    if(fPrefix != null && !DocumentImpl::isXMLName(fPrefix))
        throw DOM_DOMException(DOM_DOMException::INVALID_CHARACTER_ERR,null);
    if (localName == null)  //if not Element or Attr node
	throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);

    if (fPrefix == null || fPrefix.length() == 0) {
	this -> prefix = null;
	name = localName;
	return;
    }

    XMLCh *p = fPrefix.rawBuffer();
    for (int i = fPrefix.length(); --i >= 0;)
	if (*p++ == chColon)	//prefix is malformed
	    throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);
    if (fPrefix.equals(xml) && !namespaceURI.equals(xmlURI) ||
	fPrefix.equals(xmlns) && namespaceURI != null && namespaceURI.length() != 0)
	throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);

    name = this -> prefix = fPrefix;
    name = name + chColon + localName;    //nodeName is changed too
}

//Return a URI mapped from the given prefix and namespaceURI as below
//	prefix	namespaceURI		output
//---------------------------------------------------
//	"xml"	null, "" or xmlURI	xmlURI
//	"xml"	otherwise		NAMESPACE_ERR
//	"xmlns"	null or "" or xmlnsURI	xmlnsURI (nType = ATTRIBUTE_NODE only)
//	"xmlns"	otherwise		NAMESPACE_ERR (nType = ATTRIBUTE_NODE only)
//	else	any			namesapceURI
const DOMString& NodeImpl::mapPrefix(const DOMString &prefix,
	const DOMString &namespaceURI, short nType)
{
    DOMString xml = DStringPool::getStaticString("xml", &s_xml);
    DOMString xmlURI = DStringPool::getStaticString("http://www.w3.org/XML/1998/namespace", &s_xmlURI);
    DOMString xmlns = DStringPool::getStaticString("xmlns", &s_xmlns);
    DOMString xmlnsURI = DStringPool::getStaticString("http://www.w3.org/2000/xmlns/", &s_xmlnsURI);

    if (prefix == null)
	return namespaceURI;
    if (prefix.equals(xml)) {
	if (namespaceURI == null || namespaceURI.length() == 0 || namespaceURI.equals(xmlURI))
	    return *s_xmlURI;
	throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);
    } else if (nType == DOM_Node::ATTRIBUTE_NODE && prefix.equals(xmlns)) {
	if (namespaceURI == null || namespaceURI.length() == 0 || namespaceURI.equals(xmlnsURI))
	    return *s_xmlnsURI;
	throw DOM_DOMException(DOM_DOMException::NAMESPACE_ERR, null);
    } else
	return namespaceURI;
}
