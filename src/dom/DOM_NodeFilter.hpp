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
 * Revision 1.1  1999/11/09 01:09:00  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:44:19  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

// DOM_NodeFilter.h: interface for the DOM_NodeFilter class.
//
//////////////////////////////////////////////////////////////////////

#ifndef DOM_NodeFilter_HEADER_GUARD_
#define DOM_NodeFilter_HEADER_GUARD_

#include "DOM_Node.hpp"

class NodeFilterImpl;


/*
*	This class wraps a NodeFilterImpl object, which is reference counted. Users
*	are expected to subclass NodeFilterImpl to provide the desired filter. An example
*	of this is in NameNodeFilterImpl.[cpp,hpp]
*/


class CDOM_EXPORT DOM_NodeFilter
{
	public:
		enum FilterAction {ACCEPT, REJECT, SKIP};

    DOM_NodeFilter (NodeFilterImpl* refCountedFilter);

    DOM_NodeFilter();
    DOM_NodeFilter(const DOM_NodeFilter &other);
    DOM_NodeFilter & operator = (const DOM_NodeFilter &other);
    DOM_NodeFilter & operator = (const DOM_NullPtr *val);
    ~DOM_NodeFilter();
    bool operator == (const DOM_NodeFilter &other) const;
    bool operator != (const DOM_NodeFilter &other) const;

    bool operator == (const DOM_NullPtr *nullPtr) const;
    bool operator != (const DOM_NullPtr *nullPtr) const;

		virtual FilterAction acceptNode (DOM_Node node);

  private:
    NodeFilterImpl*   fImpl;
};

#endif
