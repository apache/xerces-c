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
 * Revision 1.2  1999/11/30 21:16:25  roddey
 * Changes to add the transcode() method to DOMString, which returns a transcoded
 * version (to local code page) of the DOM string contents. And I changed all of the
 * exception 'throw by pointer' to 'throw by value' style.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:09  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:27  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
*/

/**
* EntityReference models the XML &entityname; syntax, when used for
* entities defined by the DOM. Entities hardcoded into XML, such as
* character entities, should instead have been translated into text
* by the code which generated the DOM tree.
* <P>
* An XML processor has the alternative of fully expanding Entities
* into the normal document tree. If it does so, no EntityReference nodes
* will appear.
* <P>
* Similarly, non-validating XML processors are not required to read
* or process entity declarations made in the external subset or
* declared in external parameter entities. Hence, some applications
* may not make the replacement value available for Parsed Entities 
* of these types.
* <P>
* EntityReference behaves as a read-only node, and the children of 
* the EntityReference (which reflect those of the Entity, and should
* also be read-only) give its replacement value, if any. They are 
* supposed to automagically stay in synch if the DocumentType is 
* updated with new values for the Entity.
* <P>
* The defined behavior makes efficient storage difficult for the DOM
* implementor. We can't just look aside to the Entity's definition
* in the DocumentType since those nodes have the wrong parent (unless
* we can come up with a clever "imaginary parent" mechanism). We
* must at least appear to clone those children... which raises the
* issue of keeping the reference synchronized with its parent.
* This leads me back to the "cached image of centrally defined data"
* solution, much as I dislike it.
* <P>
* For now I have decided, since REC-DOM-Level-1-19980818 doesn't
* cover this in much detail, that synchronization doesn't have to be
* considered while the user is deep in the tree. That is, if you're
* looking within one of the EntityReferennce's children and the Entity
* changes, you won't be informed; instead, you will continue to access
* the same object -- which may or may not still be part of the tree.
* This is the same behavior that obtains elsewhere in the DOM if the
* subtree you're looking at is deleted from its parent, so it's
* acceptable here. (If it really bothers folks, we could set things
* up so deleted subtrees are walked and marked invalid, but that's
* not part of the DOM's defined behavior.)
* <P>
* As a result, only the EntityReference itself has to be aware of
* changes in the Entity. And it can take advantage of the same
* structure-change-monitoring code I implemented to support
* DeepNodeList.
* 
* @author Rania Y. Khalaf
* @author Joseph Kesselman
* @since  PR-DOM-Level-1-19980818.
*/

#include "DocumentImpl.hpp"
#include "DocumentTypeImpl.hpp"
#include "EntityImpl.hpp"
#include "EntityReferenceImpl.hpp"
#include "DOM_DOMException.hpp"
#include "DOM_Node.hpp"
#include "NamedNodeMapImpl.hpp"
#include "NodeImpl.hpp"



EntityReferenceImpl::EntityReferenceImpl(DocumentImpl *ownerDoc, const DOMString &entityName) :
NodeImpl(ownerDoc, entityName, DOM_Node::ENTITY_REFERENCE_NODE, false, null)
{
    
    // EntityReference behaves as a read-only node, since its contents
    // reflect the Entity it refers to -- but see setNodeName().
    readOnly = false;
    entityChanges=-1;
}



EntityReferenceImpl::EntityReferenceImpl(const EntityReferenceImpl &other, bool deep)
: NodeImpl(other, deep)
{
    entityChanges = other.entityChanges;
}



EntityReferenceImpl::~EntityReferenceImpl() 
{
}


NodeImpl *EntityReferenceImpl::cloneNode(bool deep)
{
    // Note #971.  Unclear what a shallow clone should mean/do.
    synchronize();
    return new EntityReferenceImpl(*this, deep);
}


/**
* Perform synchronize() before accessing children.
* 
* @return org.w3c.dom.NodeList
*/
NodeListImpl *EntityReferenceImpl::getChildNodes()
{
    synchronize();
    return NodeImpl::getChildNodes();
}


/**
* Perform synchronize() before accessing children.
* 
* @return org.w3c.dom.NodeList
*/
NodeImpl *EntityReferenceImpl::getFirstChild()
{
    synchronize();
    return NodeImpl::getFirstChild();
}


/**
* Perform synchronize() before accessing children.
* 
* @return org.w3c.dom.NodeList
*/
NodeImpl *EntityReferenceImpl::getLastChild() 
{
    synchronize();
    return NodeImpl::getLastChild();
}


/**
* Query the number of children in the entity definition.
* (A bit more work than asking locally, but may be able to avoid
* or defer building the clone subtree.)
*
* @return org.w3c.dom.NodeList
*/
int EntityReferenceImpl::getLength()
{
    int length = NodeImpl::getLength();

#if (0)                 // Till we add entity nodes to the doc root element.

    DocumentTypeImpl *doctype;
    NamedNodeMapImpl *entities;
    EntityImpl *entDef;
    if (null != (doctype = ownerDocument->getDoctype()) && 
        null != (entities = doctype->getEntities()) &&
        null != (entDef = (EntityImpl *)entities->getNamedItem(name))
        )
        length=entDef->getChildNodes()->getLength();
#endif
    
    return length;
}


/**
* Query the presence of children in the entity definition.
* (A bit more work than asking locally, but may be able to avoid
* or defer building the clone subtree.)
*
* @return boolean
*/
bool EntityReferenceImpl::hasChildNodes()
{
    bool haskids=false;
    
    DocumentTypeImpl *doctype;
    NamedNodeMapImpl *entities;
    EntityImpl *entDef;
    if (null != (doctype = ownerDocument->getDoctype()) && 
        null != (entities = doctype->getEntities()) &&
        null != (entDef = (EntityImpl *)entities->getNamedItem(name))
        )
        haskids=entDef->hasChildNodes();
    
    return haskids;
}



bool EntityReferenceImpl::isEntityReference()
{
    return true;
}


/**
* Perform synchronize() before accessing children.
* ????? Should just ask the Entity instead!
* (Though if we discover 0, we can flush)
*
* @return org.w3c.dom.NodeList
*/
NodeImpl *EntityReferenceImpl::item(int index) {
    synchronize();
    return NodeImpl::item(index);
}


/**
* EntityReferences never have a nodeValue.
* @throws DOMException(NO_MODIFICATION_ALLOWED_ERR)
*/
void EntityReferenceImpl::setNodeValue(const DOMString &x)
{
    throw DOM_DOMException(DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR, null);
}


/**
* EntityRef is already, and must be, a read-only node. Attempts to change
* that will throw a NO_MODIFICATION_ALLOWED_ERR DOMException.
* <P>
* If you want to alter its contents, edit the Entity definition.
* 
* @param readOnly boolean
*/
void EntityReferenceImpl::setReadOnly(bool readOnl,bool deep)
{
    if(readOnl==false)
        throw DOM_DOMException(DOM_DOMException::NO_MODIFICATION_ALLOWED_ERR,null);
    NodeImpl::setReadOnly(readOnl,deep);
}


/**
* EntityReference's children are a reflection of those defined in the
* named Entity. This method updates them if the Entity is changed.
* <P>
* It is unclear what the least-cost resynch mechanism is.
* If we expect the kids to be shallow, and/or expect changes
* to the Entity contents to be rare, wiping them all out
* and recloning is simplest.
* <P>
* If we expect them to be deep,
* it might be better to first decide which kids (if any)
* persist, and keep the ones (if any) that are unchanged
* rather than doing all the work of cloning them again.
* But that latter gets into having to convolve the two child lists,
* insert new information in the right order (and possibly reorder
* the existing kids), and a few other complexities that I really
* don't want to deal with in this implementation.
* <P>
* Note that if we decide that we need to update the EntityReference's
* contents, we have to turn off the readOnly flag temporarily to do so.
* When we get around to adding multitasking support, this whole method
* should probably be an atomic operation.
* 
* @see DocumentTypeImpl
* @see EntityImpl
*/
void EntityReferenceImpl::synchronize()
{
    DocumentTypeImpl *doctype;
    NamedNodeMapImpl *entities;
    EntityImpl *entDef;
    if (null != (doctype = ownerDocument->getDoctype()) && 
        null != (entities = doctype->getEntities())
        )
    {
        entDef = (EntityImpl *)entities->getNamedItem(name);
        
        // No Entity by this name. If we had a change count, reset it.
        if(null==entDef)
            entityChanges=-1;
        
        // If no kids availalble, wipe any pre-existing children.
        // (See discussion above.)
        // Note that we have to use the superclass to avoid recursion
        // through Synchronize.
        readOnly=false;
        if(null==entDef || !entDef->hasChildNodes())
            for(NodeImpl *kid=NodeImpl::getFirstChild();
            kid!=null;
            kid=NodeImpl::getFirstChild())
                removeChild(kid);
            
            // If entity's definition changed, clone its kids
            // (See discussion above.)
            if(null!=entDef && entDef->changes!=entityChanges)
            {
                for(NodeImpl *defkid=entDef->getFirstChild();
                defkid!=null;
                defkid=defkid->getNextSibling())
                {
                    NodeImpl *newkid=(NodeImpl *) defkid->cloneNode(true);
                    newkid->setReadOnly(true,true);
                    insertBefore(newkid,null);
                    readOnly=true;
                }
                entityChanges=entDef->changes;
            }
            readOnly=true;
    }
}
