#ifndef IDOM_DocumentFragment_HEADER_GUARD_
#define IDOM_DocumentFragment_HEADER_GUARD_
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


#include <util/XercesDefs.hpp>
#include "IDOM_Node.hpp"


/**
 * <code>DocumentFragment</code> is a "lightweight" or "minimal"
 * <code>Document</code> object.
 *
 * It is very common to want to be able to
 * extract a portion of a document's tree or to create a new fragment of a
 * document. Imagine implementing a user command like cut or rearranging a
 * document by moving fragments around. It is desirable to have an object
 * which can hold such fragments and it is quite natural to use a Node for
 * this purpose. While it is true that a <code>Document</code> object could
 * fulfil this role,  a <code>Document</code> object can potentially be a
 * heavyweight  object, depending on the underlying implementation. What is
 * really needed for this is a very lightweight object.
 * <code>DocumentFragment</code> is such an object.
 * <p>Furthermore, various operations -- such as inserting nodes as children
 * of another <code>Node</code> -- may take <code>DocumentFragment</code>
 * objects as arguments;  this results in all the child nodes of the
 * <code>DocumentFragment</code>  being moved to the child list of this node.
 * <p>The children of a <code>DocumentFragment</code> node are zero or more
 * nodes representing the tops of any sub-trees defining the structure of the
 * document. <code>DocumentFragment</code> nodes do not need to be
 * well-formed XML documents (although they do need to follow the rules
 * imposed upon well-formed XML parsed entities, which can have multiple top
 * nodes).  For example, a <code>DocumentFragment</code> might have only one
 * child and that child node could be a <code>Text</code> node. Such a
 * structure model  represents neither an HTML document nor a well-formed XML
 * document.
 * <p>When a <code>DocumentFragment</code> is inserted into a
 * <code>Document</code> (or indeed any other <code>Node</code> that may take
 * children) the children of the <code>DocumentFragment</code> and not the
 * <code>DocumentFragment</code>  itself are inserted into the
 * <code>Node</code>. This makes the <code>DocumentFragment</code> very
 * useful when the user wishes to create nodes that are siblings; the
 * <code>DocumentFragment</code> acts as the parent of these nodes so that the
 *  user can use the standard methods from the <code>Node</code>  interface,
 * such as <code>insertBefore()</code> and  <code>appendChild()</code>.
 */

class CDOM_EXPORT IDOM_DocumentFragment: public IDOM_Node {

protected:
    IDOM_DocumentFragment() {};
    IDOM_DocumentFragment(const IDOM_DocumentFragment &other) {};
    IDOM_DocumentFragment & operator = (const IDOM_DocumentFragment &other) {return *this;};
	//@}
    /** @name Destructor */
    //@{
	
public:
    virtual ~IDOM_DocumentFragment() {};

	//@}

};

#endif
