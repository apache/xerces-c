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
 * $Log$
 * Revision 1.10  2001/11/22 20:23:20  peiyongz
 * _declspec(dllimport) and inline warning C4273
 *
 * Revision 1.9  2001/10/09 20:52:43  peiyongz
 * init():removed
 *
 * Revision 1.8  2001/10/01 21:04:40  peiyongz
 * DTV Reorganization:fix to memory leak in compare() method.
 *
 * Revision 1.7  2001/10/01 16:16:38  peiyongz
 * DTV Reorganization:derived from AbstractNumericValidator
 *
 */

#if !defined(DECIMAL_DATATYPEVALIDATOR_HPP)
#define DECIMAL_DATATYPEVALIDATOR_HPP

#include <validators/datatype/AbstractNumericValidator.hpp>
#include <util/RefVectorOf.hpp>
#include <util/XMLBigDecimal.hpp>

class VALIDATORS_EXPORT DecimalDatatypeValidator : public AbstractNumericValidator
{
public:

    // -----------------------------------------------------------------------
    //  Public ctor/dtor
    // -----------------------------------------------------------------------
	/** @name Constructor. */
    //@{

    DecimalDatatypeValidator();

    DecimalDatatypeValidator(DatatypeValidator*            const baseValidator
                           , RefHashTableOf<KVStringPair>* const facets
                           , RefVectorOf<XMLCh>*           const enums
                           , const int                           finalSet);

    virtual ~DecimalDatatypeValidator();

	//@}

    // -----------------------------------------------------------------------
    // Compare methods
    // -----------------------------------------------------------------------
    /** @name Compare Function */
    //@{

    /**
     * Compare two boolean data types
     *
     * @param content1
     * @param content2
     * @return
     */
    virtual int compare(const XMLCh* const, const XMLCh* const);

    //@}

    /**
      * Returns an instance of the base datatype validator class
	  * Used by the DatatypeValidatorFactory.
      */
    virtual DatatypeValidator* newInstance(RefHashTableOf<KVStringPair>* const facets
                                         , RefVectorOf<XMLCh>*           const enums
                                         , const int                           finalSet);

protected:

// -----------------------------------------------------------------------
// ctor provided to be used by derived classes
// -----------------------------------------------------------------------
    DecimalDatatypeValidator(DatatypeValidator*            const baseValidator
                           , RefHashTableOf<KVStringPair>* const facets
                           , const int                           finalSet
                           , const ValidatorType                 type);

// -----------------------------------------------------------------------
// Abstract interface from AbstractNumericFacetValidator
// -----------------------------------------------------------------------
    
    virtual void assignAdditionalFacet(const XMLCh* const key
                                     , const XMLCh* const value);

    virtual void inheritAdditionalFacet();

    virtual void checkAdditionalFacetConstraints() const;

    virtual void checkAdditionalFacetConstraintsBase() const;

    virtual int  compareValues(const XMLNumber* const lValue
                             , const XMLNumber* const rValue);

    virtual void  setMaxInclusive(const XMLCh* const);

    virtual void  setMaxExclusive(const XMLCh* const);

    virtual void  setMinInclusive(const XMLCh* const);

    virtual void  setMinExclusive(const XMLCh* const);

    virtual void  setEnumeration();

// -----------------------------------------------------------------------
// Abstract interface from AbstractNumericValidator
// -----------------------------------------------------------------------

    virtual void checkContent( const XMLCh* const content, bool asBase);

private:

// -----------------------------------------------------------------------
// Getter methods
// -----------------------------------------------------------------------

    inline unsigned int                    getTotalDigits() const;

    inline unsigned int                    getFractionDigits() const;

// -----------------------------------------------------------------------
// Setter methods
// -----------------------------------------------------------------------

    inline void  setTotalDigits(unsigned int);

    inline void  setFractionDigits(unsigned int);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    // -----------------------------------------------------------------------
	 unsigned int         fTotalDigits;
	 unsigned int         fFractionDigits;

};

// -----------------------------------------------------------------------
// Getter methods
// -----------------------------------------------------------------------

inline unsigned int DecimalDatatypeValidator::getTotalDigits() const
{
    return fTotalDigits;
}

inline unsigned int DecimalDatatypeValidator::getFractionDigits() const
{
    return fFractionDigits;
}

// -----------------------------------------------------------------------
// Setter methods
// -----------------------------------------------------------------------

inline void DecimalDatatypeValidator::setTotalDigits(unsigned int newTotalDigits)
{
    fTotalDigits = newTotalDigits;
}

inline void DecimalDatatypeValidator::setFractionDigits(unsigned int newFractionDigits)
{
    fFractionDigits = newFractionDigits;
}

/**
  * End of file DecimalDatatypeValidator.hpp
  */
#endif
