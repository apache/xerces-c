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
 * Revision 1.6  2001/02/27 14:48:30  tng
 * Schema: Add CMAny and ContentLeafNameTypeVector, by Pei Yong Zhang
 *
 * Revision 1.5  2000/03/02 19:54:24  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.4  2000/02/24 20:00:23  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.3  2000/02/15 01:21:30  roddey
 * Some initial documentation improvements. More to come...
 *
 * Revision 1.2  2000/02/06 07:47:48  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:30  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:37  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(CONTENTMODEL_HPP)
#define CONTENTMODEL_HPP

#include <util/XercesDefs.hpp>
#include <util/RefVectorOf.hpp>

class ContentLeafNameTypeVector;

/**
 *  This class defines the abstract interface for all content models. All
 *  elements have a content model against which (if validating) its content
 *  is checked. Each type of validator (DTD, Schema, etc...) can have 
 *  different types of content models, and even with each type of validator
 *  there can be specialized content models. So this simple class provides
 *  the abstract API via which all the types of contents models are dealt
 *  with generically. Its pretty simple.
 */
class XMLPARSER_EXPORT XMLContentModel
{
public:
    // -----------------------------------------------------------------------
    //  Constructors are hidden, only the virtual Destructor is exposed
    // -----------------------------------------------------------------------
    /** @name Destructor */
    //@{
    virtual ~XMLContentModel()
    {
    }
    //@}


    // -----------------------------------------------------------------------
    //  The virtual content model interface provided by derived classes
    // -----------------------------------------------------------------------
    virtual bool getIsAmbiguous() const = 0;

	virtual int validateContent
    (
        const   unsigned int*   childIds
        , const unsigned int    childCount
    ) const = 0;

	virtual int validateContentSpecial
    (
        const   unsigned int*   childIds
        , const unsigned int    childCount
    ) const = 0;

    virtual ContentLeafNameTypeVector* getContentLeafNameTypeVector()
	  const = 0;

//  Onhold, until EquivClassComparator is defined
//  void setEquivClassComparator(EquivClassComparator comparator) = 0;

//  Do not implement, it is NOT used in XercesJ.
//  int whatCanGoHere(bool, InsertableElementsInfo info) = 0;

protected :
    // -----------------------------------------------------------------------
    //  Hidden Constructors
    // -----------------------------------------------------------------------
    XMLContentModel()
    {
    }


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLContentModel(const XMLContentModel&);
    void operator=(const XMLContentModel&);
};

#endif
