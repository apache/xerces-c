#ifndef IDEntityImpl_HEADER_GUARD_
#define IDEntityImpl_HEADER_GUARD_

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

#include <xercesc/util/XercesDefs.hpp>
#include "IDNodeImpl.hpp"
#include "IDParentNode.hpp"
#include "IDOM_Entity.hpp"

class    IDOM_EntityReference;

class CDOM_EXPORT IDEntityImpl: public IDOM_Entity {
private:
    IDNodeImpl      fNode;
    IDParentNode    fParent;

    const XMLCh *   fName;
    const XMLCh *   fPublicId;
    const XMLCh *   fSystemId;
    const XMLCh *   fNotationName;
    IDOM_EntityReference*	fRefEntity;
	void	cloneEntityRefTree() const;

	friend class IDOMParser;

public:
    IDEntityImpl(IDOM_Document *doc, const XMLCh *eName);
    IDEntityImpl(const IDEntityImpl &other, bool deep=false);
    virtual ~IDEntityImpl();

    // Declare all of the functions from IDOM_Node.
    IDOM_NODE_FUNCTIONS;


    virtual const XMLCh *   getPublicId() const;
    virtual const XMLCh *   getSystemId() const;
    virtual const XMLCh *   getNotationName() const;
    virtual void            setNotationName(const XMLCh *arg);
    virtual void            setPublicId(const XMLCh *arg);
    virtual void            setSystemId(const XMLCh *arg);

	//DOM Level 2 additions. Non standard functions
	virtual void		setEntityRef(IDOM_EntityReference *);
	virtual IDOM_EntityReference*	getEntityRef() const;
};

#endif

