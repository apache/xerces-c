#ifndef DOMNodeIteratorImpl_HEADER_GUARD_
#define DOMNodeIteratorImpl_HEADER_GUARD_

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

//
//  This file is part of the internal implementation of the C++ XML DOM.
//  It should NOT be included or used directly by application programs.
//
//  Applications should include the file <xercesc/dom/DOM.hpp> for the entire
//  DOM API, or xercesc/dom/DOM*.hpp for individual DOM classes, where the class
//  name is substituded for the *.
//

//////////////////////////////////////////////////////////////////////
// DOMNodeIteratorImpl.hpp: interface for the DOMNodeIteratorImpl class.
//
//////////////////////////////////////////////////////////////////////

#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class CDOM_EXPORT DOMNodeIteratorImpl : public DOMNodeIterator {
    private:
        //
        // Data
        //
        // The root.
        DOMNode* fRoot;

        // The Document used to create this iterator
        DOMDocument* fDocument;

        // The whatToShow mask.
        unsigned long fWhatToShow;

        // The NodeFilter reference.
        DOMNodeFilter* fNodeFilter;


        // The expandEntity reference flag.
        bool  fExpandEntityReferences;
        bool fDetached;


        //
        // Iterator state - current node and direction.
        //
        // Note: The current node and direction are sufficient to implement
        // the desired behaviour of the current pointer being _between_
        // two nodes. The fCurrentNode is actually the last node returned,
        // and the
        // direction is whether the pointer is in front or behind this node.
        // (usually akin to whether the node was returned via nextNode())
        // (eg fForward = true) or previousNode() (eg fForward = false).
        // The last Node returned.
        DOMNode* fCurrentNode;

        // The direction of the iterator on the fCurrentNode.
        //  <pre>
        //  nextNode()  ==      fForward = true;
        //  previousNode() ==   fForward = false;
        //  </pre>
        bool fForward;

    public:
        virtual ~DOMNodeIteratorImpl ();
        DOMNodeIteratorImpl (
            DOMDocument* fDocument,
            DOMNode* root,
            unsigned long whatToShow,
            DOMNodeFilter* nodeFilter,
            bool expandEntityRef);

        DOMNodeIteratorImpl ( const DOMNodeIteratorImpl& toCopy);
        DOMNodeIteratorImpl& operator= (const DOMNodeIteratorImpl& other);

        virtual DOMNode* getRoot ();                	
        virtual unsigned long getWhatToShow ();
        virtual DOMNodeFilter* getFilter ();
        // Get the expandEntity reference flag.
        virtual bool getExpandEntityReferences();

        virtual DOMNode* nextNode ();
        virtual DOMNode* previousNode ();
        virtual void detach ();

        virtual void release();
        void removeNode (DOMNode* node);

    private:
        DOMNode* matchNodeOrParent (DOMNode* node);
        DOMNode* nextNode (DOMNode* node, bool visitChildren);
        DOMNode* previousNode (DOMNode* node);
        bool acceptNode (DOMNode* node);

};

XERCES_CPP_NAMESPACE_END

#endif
