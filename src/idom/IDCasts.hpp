#ifndef IDCasts_HEADER_GUARD_
#define IDCasts_HEADER_GUARD_

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


//
//  Define inline casting functions to convert from
//    (IDOM_Node *) to IDParentNode or IDChildNode *.
//
//  This requires knowledge of the structure of the fields of
//   for all node types.  There are three categories -
//
//  Nodetypes that can have children and can be a child themselves.
//    e.g.  Elements
//
//       Object
//           IDNodeImpl     fNode;
//           IDParentNode   fParent;
//           IDChildNode    fChild;
//             ...            // other fields, depending on node type.
//
//  Nodetypes that can not have children, e.g. TEXT
//
//       Object
//           IDNodeImpl     fNode;
//           IDChildNode    fChild;
//              ...            // other fields, depending on node type
//
//  Nodetypes that can not be a child of other nodes, but that can
//  have children (are a parent)  e.g. ATTR
//       Object
//           IDNodeImpl     fNode;
//           IDParentNode   fParent
//               ...           // other fields, depending on node type
//
//   The casting functions make these assumptions:
//      1.  The cast is possible.  Using code will not attempt to
//          cast to something that does not exist, such as the child
//          part of an ATTR
//
//      2.  The nodes belong to this implementation.
//
//    Some of the casts use the LEAFNODE flag in the common fNode part to
//    determine whether an fParent field exists, and thus the
//    position of the fChild part within the node.
//
//  These functions also cast off const.  It was either do that, or make
//  a second overloaded set that took and returned const arguements.
//

#include "IDElementImpl.hpp"
#include "IDTextImpl.hpp"

static inline IDNodeImpl *castToNodeImpl(const IDOM_Node *p)
{
    IDElementImpl *pE = (IDElementImpl *)p;
    return &(pE->fNode);
}


static inline IDParentNode *castToParentImpl(const IDOM_Node *p) {
    IDElementImpl *pE = (IDElementImpl *)p;
    return &(pE->fParent);
}


static inline IDChildNode *castToChildImpl(const IDOM_Node *p) {
    IDElementImpl *pE = (IDElementImpl *)p;
    if (pE->fNode.isLeafNode())  {
        IDTextImpl *pT = (IDTextImpl *)p;
        return &(pT->fChild);
    }
    return &(pE->fChild);
}


static inline IDOM_Node *castToNode(const IDParentNode *p ) {
    int parentOffset = (char *)&(((IDElementImpl *)0)->fParent) - (char *)0;
    char *retPtr = (char *)p - parentOffset;
    return (IDOM_Node *)retPtr;
}

static inline IDOM_Node *castToNode(const IDNodeImpl *p) {
    int nodeImplOffset = (char *)&(((IDElementImpl *)0)->fNode) - (char *)0;
    char *retPtr = (char *)p - nodeImplOffset;
    return (IDOM_Node *)retPtr;
}


static inline IDNodeImpl *castToNodeImpl(const IDParentNode *p)
{
    char *nodeImplOffset = (char *)&(((IDElementImpl *)0)->fNode);
    char *parentOffset   = (char *)&(((IDElementImpl *)0)->fParent);
    char *retPtr = (char *)p - parentOffset + nodeImplOffset;
    return (IDNodeImpl *)retPtr;
}

#endif
