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
 * Revision 1.2  2000/02/04 01:49:28  aruna1
 * TreeWalker and NodeIterator changes
 *
 * Revision 1.1.1.1  1999/11/09 01:09:00  twl
 * Initial checkin
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
		enum FilterAction {FILTER_ACCEPT, FILTER_REJECT, FILTER_SKIP};
        enum ShowType {
            SHOW_ALL                       = 0x0000FFFF,
            SHOW_ELEMENT                   = 0x00000001,
            SHOW_ATTRIBUTE                 = 0x00000002,
            SHOW_TEXT                      = 0x00000004,
            SHOW_CDATA_SECTION             = 0x00000008,
            SHOW_ENTITY_REFERENCE          = 0x00000010,
            SHOW_ENTITY                    = 0x00000020,
            SHOW_PROCESSING_INSTRUCTION    = 0x00000040,
            SHOW_COMMENT                   = 0x00000080,
            SHOW_DOCUMENT                  = 0x00000100,
            SHOW_DOCUMENT_TYPE             = 0x00000200,
            SHOW_DOCUMENT_FRAGMENT         = 0x00000400,
            SHOW_NOTATION                  = 0x00000800
        };

    DOM_NodeFilter();
    virtual ~DOM_NodeFilter();
    virtual short acceptNode (DOM_Node node) =0;

private:
    DOM_NodeFilter(const DOM_NodeFilter &other);
    DOM_NodeFilter & operator = (const DOM_NodeFilter &other);
    bool operator == (const DOM_NodeFilter &other) const;
    bool operator != (const DOM_NodeFilter &other) const;

};

#endif
