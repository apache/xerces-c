#ifndef IDOM_Range_HEADER_GUARD_
#define IDOM_Range_HEADER_GUARD_

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

#include <xercesc/util/XercesDefs.hpp>

class IDOM_Node;
class IDOM_DocumentFragment;

class CDOM_EXPORT IDOM_Range {
protected:
    IDOM_Range() {};
    IDOM_Range(const IDOM_Range &other) {};
    IDOM_Range & operator = (const IDOM_Range &other) {return *this;};

public:

    enum CompareHow {
        START_TO_START  = 0,
        START_TO_END    = 1,
        END_TO_END      = 2,
        END_TO_START    = 3
    };

    virtual ~IDOM_Range() {};

    //getter functions
    virtual IDOM_Node* getStartContainer() const = 0;
    virtual unsigned int getStartOffset() const = 0;
    virtual IDOM_Node* getEndContainer() const = 0;
    virtual unsigned int getEndOffset() const = 0;
    virtual bool getCollapsed() const = 0;
    virtual const IDOM_Node* getCommonAncestorContainer() const = 0;

    //setter functions
    virtual void setStart(const IDOM_Node *parent, unsigned int offset) = 0;
    virtual void setEnd(const IDOM_Node *parent, unsigned int offset) = 0;

    virtual void setStartBefore(const IDOM_Node *refNode) = 0;
    virtual void setStartAfter(const IDOM_Node *refNode) = 0;
    virtual void setEndBefore(const IDOM_Node *refNode) = 0;
    virtual void setEndAfter(const IDOM_Node *refNode) = 0;

    //misc functions
    virtual void collapse(bool toStart) = 0;
    virtual void selectNode(const IDOM_Node *node) = 0;
    virtual void selectNodeContents(const IDOM_Node *node) = 0;

    //Functions related to comparing range Boundrary-Points
    virtual short compareBoundaryPoints(CompareHow how, const IDOM_Range* range) const = 0;
    virtual void deleteContents() = 0;
    virtual IDOM_DocumentFragment* extractContents() = 0;
    virtual IDOM_DocumentFragment* cloneContents() const = 0;
    virtual void insertNode(IDOM_Node *node) = 0;
    //Misc functions
    virtual void surroundContents(IDOM_Node *node) = 0;
    virtual IDOM_Range* cloneRange() const = 0;
    virtual const XMLCh* toString() const = 0;
    virtual void detach() = 0;

};


#endif
