#ifndef IDDeepNodeListImpl_HEADER_GUARD_
#define IDDeepNodeListImpl_HEADER_GUARD_

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
 * Revision 1.1  2001/04/03 00:14:20  andyh
 * Initial revision
 *
 * Revision 1.7  2000/03/02 19:53:59  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.6  2000/02/24 20:11:29  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.5  2000/02/06 07:47:31  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.4  2000/02/04 01:49:30  aruna1
 * TreeWalker and NodeIterator changes
 *
 * Revision 1.3  2000/01/22 01:38:29  andyh
 * Remove compiler warnings in DOM impl classes
 *
 * Revision 1.2  1999/12/21 07:47:06  robweir
 * Patches to support Xalan, where we need to create a
 * "special" DOM with subclassed Nodes.
 *
 * 1. Export the NodeImpl-derived classes
 * 2. Ensure that their constructors have at least protected access
 *
 * Revision 1.1.1.1  1999/11/09 01:08:42  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:23  rahul
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
#include "IDOM_NodeList.hpp"

class IDOM_Node;


class CDOM_EXPORT IDDeepNodeListImpl: public IDOM_NodeList {
private:
    const IDOM_Node    *fRootNode;    
    const XMLCh        *fTagName;      
    bool                fMatchAll;
    int                 fChanges;
    IDOM_Node          *fCurrentNode;
    unsigned int        fCurrentIndex;

    //DOM Level 2
    const XMLCh *	    fNamespaceURI;
    bool		        fMatchAllURI;
    bool                fMatchURIandTagname; //match both namespaceURI and tagName
    
private:
                        IDDeepNodeListImpl(const IDOM_Node *rootNode, const XMLCh *tagName);
                        IDDeepNodeListImpl(const IDOM_Node *rootNode,	//DOM Level 2
			                            const XMLCh *namespaceURI,
                                        const XMLCh *localName);

public:
    // Factory methods for getting/creating node lists. 
    // Because nothing is ever deleted, the implementation caches and recycles
    //  previously used instances of IDDeepNodeList
    //
    static IDOM_NodeList *getDeepNodeList(const IDOM_Node *rootNode, const XMLCh *tagName);
    static IDOM_NodeList *getDeepNodeList(const IDOM_Node *rootNode,	//DOM Level 2
			                            const XMLCh *namespaceURI,
                                        const XMLCh *localName);

public:
    virtual             ~IDDeepNodeListImpl();
    virtual unsigned int getLength() const;
    virtual IDOM_Node   *item(unsigned int index) const;
};

#endif
