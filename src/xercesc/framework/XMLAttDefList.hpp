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
 * Revision 1.1  2002/02/01 22:21:50  peiyongz
 * Initial revision
 *
 * Revision 1.4  2000/02/24 20:00:22  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.3  2000/02/15 01:21:30  roddey
 * Some initial documentation improvements. More to come...
 *
 * Revision 1.2  2000/02/06 07:47:46  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:28  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:35  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(XMLATTDEFLIST_HPP)
#define XMLATTDEFLIST_HPP

#include <xercesc/util/XMLEnumerator.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/XMLAttDef.hpp>


/**
 *  This class defines an abstract interface that all validators must support.
 *  When the scanner scans the attributes in a start tag, it must have a list
 *  of the defined attributes for that element. This is used to fault in
 *  defaulted and fixed attributes, to know which ones are required, and to
 *  know the their types in order to do the correct normalization.
 *
 *  Since each validator will have its own derivatives of XMLAttDef and will
 *  have its own specialized storage mechanisms for elements and the att
 *  defs that they own, there must be an abstracted way for the scanner to
 *  deal with this list.
 *
 *  It does not derive from the generic Enumerator template class, because
 *  there are portability issues with deriving from a template class in a
 *  DLL. It does though provide a similar enumerator interface.
 */
class XMLPARSER_EXPORT XMLAttDefList
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------

    /** @name Destructor */
    //@{
    virtual ~XMLAttDefList();
    //@}


    // -----------------------------------------------------------------------
    //  The virtual interface
    // -----------------------------------------------------------------------
    virtual bool hasMoreElements() const = 0;
    virtual bool isEmpty() const = 0;
    virtual XMLAttDef* findAttDef
    (
        const   unsigned long       uriID
        , const XMLCh* const        attName
    ) = 0;
    virtual const XMLAttDef* findAttDef
    (
        const   unsigned long       uriID
        , const XMLCh* const        attName
    )   const = 0;
    virtual XMLAttDef* findAttDef
    (
        const   XMLCh* const        attURI
        , const XMLCh* const        attName
    ) = 0;
    virtual const XMLAttDef* findAttDef
    (
        const   XMLCh* const        attURI
        , const XMLCh* const        attName
    )   const = 0;
    virtual XMLAttDef& nextElement() = 0;
    virtual void Reset() = 0;


protected :
    // -----------------------------------------------------------------------
    //  Hidden constructors and operators
    // -----------------------------------------------------------------------
    XMLAttDefList();
    XMLAttDefList(const XMLAttDefList&);
    XMLAttDefList& operator=(const XMLAttDefList&);
};


// ---------------------------------------------------------------------------
//  XMLAttDefList: Constructors and Destructor
// ---------------------------------------------------------------------------
inline XMLAttDefList::~XMLAttDefList()
{
}


// ---------------------------------------------------------------------------
//  XMLAttDefList: Hidden Constructors and Operators
// ---------------------------------------------------------------------------
inline XMLAttDefList::XMLAttDefList()
{
}

inline XMLAttDefList::XMLAttDefList(const XMLAttDefList&)
{
}

inline XMLAttDefList& XMLAttDefList::operator=(const XMLAttDefList&)
{
    return *this;
}

#endif
