#ifndef NamedNodeMapImpl_HEADER_GUARD_
#define NamedNodeMapImpl_HEADER_GUARD_

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
 * Revision 1.10  2000/04/07 20:58:36  lehors
 * fixed all cloneMap so that the ownerNode is correct
 *
 * Revision 1.9  2000/04/06 21:17:48  lehors
 * got rid of the owned attribute,
 * renamed parentNode to ownerNode to make it clear it's not always the parent
 *
 * Revision 1.8  2000/03/02 19:54:02  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.7  2000/02/24 20:11:30  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.6  2000/02/06 07:47:33  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.5  2000/02/04 01:49:26  aruna1
 * TreeWalker and NodeIterator changes
 *
 * Revision 1.4  2000/01/22 01:38:30  andyh
 * Remove compiler warnings in DOM impl classes
 *
 * Revision 1.3  2000/01/08 00:09:28  andyh
 * Correcf failures in DOMTest with entity references and read-only nodes.
 * Correct reference counting problem NamedNodeMap.
 * Add export methods to NamedNodeMap and DocumentTypeImpl.
 * Redo DocumentImpl::cloneNode
 *
 * (Changes by Chih-Hsiang Chou)
 *
 * Revision 1.2  2000/01/05 01:16:08  andyh
 * DOM Level 2 core, namespace support added.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:12  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:29  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

//
//  This file is part of the internal implementation of the C++ XML DOM.
//  It should NOT be included or used directly by application programs.
//
//  Applications should include the file <dom/DOM.hpp> for the entire
//  DOM API, or DOM_*.hpp for individual DOM classes, where the class
//  name is substituded for the *.
//

#include <util/XercesDefs.hpp>
#include "NodeImpl.hpp"

class NodeVector;
class DocumentImpl;
class NodeImpl;

class CDOM_EXPORT NamedNodeMapImpl {
private:
    NodeVector       *nodes;
    NodeImpl     *ownerNode;    // the node this map belongs to
    NamedNodeMapImpl *defaults;
    int               changes;    
    int               lastDefaultsChanges;
    bool              readOnly;
    int               refCount;
    static int        gLiveNamedNodeMaps;
    static int        gTotalNamedNodeMaps;
    
    
    friend class      DOM_NamedNodeMap;
    friend class      DomMemDebug;
    friend class      ElementImpl;
    
public:
    NamedNodeMapImpl(NodeImpl *ownerNode, NamedNodeMapImpl *defaults);
    
    virtual void            reconcileDefaults();
    virtual                 ~NamedNodeMapImpl();
    virtual NamedNodeMapImpl *cloneMap(NodeImpl *ownerNode);
    static  void            addRef(NamedNodeMapImpl *);
    virtual int             findNamePoint(const DOMString &name);
    virtual unsigned int    getLength();
    virtual NodeImpl        *getNamedItem(const DOMString &name);
    virtual NodeImpl        *item(unsigned int index);
    virtual void            removeAll();
    virtual NodeImpl        *removeNamedItem(const DOMString &name);
    static  void            removeRef(NamedNodeMapImpl *);
    virtual NodeImpl        *setNamedItem(NodeImpl *arg);
    virtual void            setReadOnly(bool readOnly, bool deep);

    //Introduced in DOM Level 2
    virtual NamedNodeMapImpl *exportNode(NodeImpl *node);
    virtual int             findNamePoint(const DOMString &namespaceURI,
	const DOMString &localName);
    virtual NodeImpl        *getNamedItemNS(const DOMString &namespaceURI,
	const DOMString &localName);
    virtual NodeImpl        *setNamedItemNS(NodeImpl *arg);
    virtual NodeImpl        *removeNamedItemNS(const DOMString &namespaceURI,
	const DOMString &localName);
};

#endif

