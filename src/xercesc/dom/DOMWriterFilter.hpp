/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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
 * $Log$
 * Revision 1.1  2002/05/28 22:38:55  peiyongz
 * DOM3 Save Interface: DOMWriter/DOMWriterFilter
 *
 */

//////////////////////////////////////////////////////////////////////
// DOMWriterFilter.hpp: interface for the DOMWriterFilter class.
//
// DOMWriterFilters provide applications the ability to examine nodes 
// as they are being serialized. 
// DOMWriterFilter lets the application decide what nodes should be 
// serialized or not.
//
// The Document, DocumentType, Notation, and Entity nodes are not passed 
// to the filter.
//
//////////////////////////////////////////////////////////////////////

#ifndef DOMWriterFilter_HEADER_GUARD_
#define DOMWriterFilter_HEADER_GUARD_

#include <xercesc/dom/DOMNodeFilter.hpp>

class CDOM_EXPORT DOMWriterFilter : public DOMNodeFilter {
public:

    /** @name Destructors */
	virtual ~DOMWriterFilter(){};
    //@{

	/** @ interface from DOMNodeFilter, 
	      to be implemented by implementation (derived class) */
	virtual short acceptNode(const DOMNode*) const = 0;
    //@{

	/** @Query */
	bool   showNode(const DOMNode* const) const;
    //@{

protected:

    /** @name Constructors */
	DOMWriterFilter(unsigned long toShowMask = DOMNodeFilter::SHOW_ALL);
    //@{

private:
	// unimplemented copy ctor and assignement operator
	DOMWriterFilter(const DOMWriterFilter&);
	DOMWriterFilter & operator = (const DOMWriterFilter&);

	//
	// The whatToShow mask.
	//
    // Tells the DOMWriter what types of nodes to show to the filter. 
	// See NodeFilter for definition of the constants. 
	// The constants 
	// SHOW_ATTRIBUTE, 
	// SHOW_DOCUMENT, 
	// SHOW_DOCUMENT_TYPE, 
	// SHOW_NOTATION, and 
	// SHOW_DOCUMENT_FRAGMENT are meaningless here, 
	// those nodes will never be passed to a DOMWriterFilter. 
	//
	// Entity nodes are not passed to the filter.

	const unsigned long fWhatToShow;
};

#endif
