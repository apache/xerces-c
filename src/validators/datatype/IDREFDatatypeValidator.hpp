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
 * Revision 1.6  2001/09/27 13:51:25  peiyongz
 * DTV Reorganization: ctor/init created to be used by derived class
 *
 * Revision 1.5  2001/09/25 14:23:42  peiyongz
 * DTV Reorganization: checkValueSpace()
 *
 * Revision 1.4  2001/08/24 17:12:01  knoaman
 * Add support for anySimpleType.
 * Remove parameter 'baseValidator' from the virtual method 'newInstance'.
 *
 * Revision 1.3  2001/07/25 17:58:08  tng
 * Fix compilation errors.
 *
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

	virtual void validate(const XMLCh* const content);

    //@}

    /**
      * Returns an instance of the base datatype validator class
	  * Used by the DatatypeValidatorFactory.
      */
    virtual DatatypeValidator* newInstance(RefHashTableOf<KVStringPair>* const facets
                                         , RefVectorOf<XMLCh>*           const enums
                                         , const int                           finalSet);

    inline void setIDRefList(RefHashTableOf<XMLRefInfo>* fIDRefList);

protected:

    //
    // ctor provided to be used by derived classes
    //
    IDREFDatatypeValidator(DatatypeValidator*            const baseValidator
                         , RefHashTableOf<KVStringPair>* const facets
                         , const int                           finalSet
                         , const ValidatorType                 type);

    inline void init(DatatypeValidator*            const baseValidator
                   , RefHashTableOf<KVStringPair>* const facets
                   , RefVectorOf<XMLCh>*           const enums);

    virtual void checkValueSpace(const XMLCh* const content);

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

// -----------------------------------------------------------------------
// Validation methods
// -----------------------------------------------------------------------
void IDREFDatatypeValidator::setIDRefList(RefHashTableOf<XMLRefInfo>* newIDRefList)
{
    fIDRefList = newIDRefList;
}

void IDREFDatatypeValidator::init(
                               DatatypeValidator*            const baseValidator
                             , RefHashTableOf<KVStringPair>* const facets
                             , RefVectorOf<XMLCh>*           const enums)
{
    StringDatatypeValidator::init(baseValidator, facets, enums);
}

/**
  * End of file IDREFDatatypeValidator.hpp
  */
#endif
