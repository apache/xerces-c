/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
 * $Id$
 * $Log$
 * Revision 1.2  2001/07/24 21:23:40  tng
 * Schema: Use DatatypeValidator for ID/IDREF/ENTITY/ENTITIES/NOTATION.
 *
 * Revision 1.1  2001/07/04 20:16:59  peiyongz
 * IDREFDatatypeValidator
 *
 */

#if !defined(IDREF_DATATYPEVALIDATOR_HPP)
#define IDREF_DATATYPEVALIDATOR_HPP

#include <validators/datatype/StringDatatypeValidator.hpp>
#include <framework/XMLRefInfo.hpp>

class VALIDATORS_EXPORT IDREFDatatypeValidator : public StringDatatypeValidator
{
public:

    // -----------------------------------------------------------------------
    //  Public ctor/dtor
    // -----------------------------------------------------------------------
	/** @name Constructor. */
    //@{

    IDREFDatatypeValidator();

    IDREFDatatypeValidator(DatatypeValidator*            const baseValidator
                         , RefHashTableOf<KVStringPair>* const facets
                         , RefVectorOf<XMLCh>*           const enums
                         , const int                           finalSet);

    virtual ~IDREFDatatypeValidator();

	//@}

    // -----------------------------------------------------------------------
    // Validation methods
    // -----------------------------------------------------------------------
    /** @name Validation Function */
    //@{

    /**
     * validate that a string matches the boolean datatype
     * @param content A string containing the content to be validated
     *
     * @exception throws InvalidDatatypeException if the content is
     * is not valid.
     */

	void validate(const XMLCh* const content);

    //@}

    /**
      * Returns an instance of the base datatype validator class
	  * Used by the DatatypeValidatorFactory.
      */
    DatatypeValidator* newInstance(DatatypeValidator* const            baseValidator
                                 , RefHashTableOf<KVStringPair>* const facets
                                 , RefVectorOf<XMLCh>*           const enums
                                 , const int                           finalSet);

    void setIDRefList(RefHashTableOf<XMLRefInfo>* fIDRefList);

private:

    void addIdRef(const XMLCh* const content);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fIDRefList
    //      we do not own it.
    //
    // -----------------------------------------------------------------------
    RefHashTableOf<XMLRefInfo>* fIDRefList;

};

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
inline IDREFDatatypeValidator::IDREFDatatypeValidator()
:StringDatatypeValidator()
,fIDRefList(0)
{
    DatatypeValidator::setType(DatatypeValidator::IDREF);
}

inline IDREFDatatypeValidator::~IDREFDatatypeValidator()
{
}

inline DatatypeValidator* IDREFDatatypeValidator::newInstance(
                                      DatatypeValidator* const            baseValidator
                                    , RefHashTableOf<KVStringPair>* const facets
                                    , RefVectorOf<XMLCh>*           const enums
                                    , const int                           finalSet)
{
    return (DatatypeValidator*) new IDREFDatatypeValidator(baseValidator, facets, enums, finalSet);
}

// -----------------------------------------------------------------------
// Validation methods
// -----------------------------------------------------------------------
inline void IDREFDatatypeValidator::setIDRefList(RefHashTableOf<XMLRefInfo>* const newIDRefList)
{
    fIDRefList = newIDRefList;
}

/**
  * End of file IDREFDatatypeValidator.hpp
  */
#endif
