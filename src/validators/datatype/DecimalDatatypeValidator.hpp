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

#if !defined(DECIMAL_DATATYPEVALIDATOR_HPP)
#define DECIMAL_DATATYPEVALIDATOR_HPP

#include <validators/datatype/DatatypeValidator.hpp>
#include <util/RefVectorOf.hpp>
#include <util/XMLBigDecimal.hpp>

class VALIDATORS_EXPORT DecimalDatatypeValidator : public DatatypeValidator
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
    int compare(const XMLCh* const, const XMLCh* const);

    //@}

    /**
      * Returns an instance of the base datatype validator class
	  * Used by the DatatypeValidatorFactory.
      */
    DatatypeValidator* newInstance(RefHashTableOf<KVStringPair>* const facets
                                 , RefVectorOf<XMLCh>*           const enums
                                 , const int                           finalSet);

private:

    void checkContent( const XMLCh* const content, bool asBase);

    void init(DatatypeValidator*            const baseValidator
            , RefHashTableOf<KVStringPair>* const facets
            , RefVectorOf<XMLCh>*           const enums);

    void cleanUp();

// -----------------------------------------------------------------------
// Getter methods
// -----------------------------------------------------------------------

    unsigned int                    getTotalDigits() const;

    unsigned int                    getFractionDigits() const;

    XMLBigDecimal* const            getMaxInclusive() const;

    XMLBigDecimal* const            getMaxExclusive() const;

    XMLBigDecimal* const            getMinInclusive() const;

    XMLBigDecimal* const            getMinExclusive() const;

    RefVectorOf<XMLBigDecimal>*     getEnumeration() const;

// -----------------------------------------------------------------------
// Setter methods
// -----------------------------------------------------------------------

    void  setTotalDigits(unsigned int);

    void  setFractionDigits(unsigned int);

    void  setMaxInclusive(XMLBigDecimal* const);

    void  setMaxExclusive(XMLBigDecimal* const);

    void  setMinInclusive(XMLBigDecimal* const);

    void  setMinExclusive(XMLBigDecimal* const);

    void  setEnumeration(RefVectorOf<XMLBigDecimal>* );

    // -----------------------------------------------------------------------
    //  Private data members
    //
    // -----------------------------------------------------------------------
	 unsigned int         fTotalDigits;
	 unsigned int         fFractionDigits;
     bool                 fEnumerationInherited;

     XMLBigDecimal*       fMaxInclusive;
     XMLBigDecimal*       fMaxExclusive;
     XMLBigDecimal*       fMinInclusive;
     XMLBigDecimal*       fMinExclusive;

     RefVectorOf<XMLBigDecimal>*  fEnumeration;    // save the actual value

};

// -----------------------------------------------------------------------
// Compare methods
// -----------------------------------------------------------------------
inline int DecimalDatatypeValidator::compare(const XMLCh* const lValue
                                           , const XMLCh* const rValue)
{
    return XMLBigDecimal::compareValues(new XMLBigDecimal(lValue)
                                      , new XMLBigDecimal(rValue));
}

inline DatatypeValidator* DecimalDatatypeValidator::newInstance(
                                      RefHashTableOf<KVStringPair>* const facets
                                    , RefVectorOf<XMLCh>*           const enums
                                    , const int                           finalSet)
{
    return (DatatypeValidator*) new DecimalDatatypeValidator(this, facets, enums, finalSet);
}

inline void DecimalDatatypeValidator::validate( const XMLCh* const content)
{
    checkContent(content, false);
}

inline void DecimalDatatypeValidator::cleanUp()
{
    delete fMaxInclusive;
    delete fMaxExclusive;
    delete fMinInclusive;
    delete fMinExclusive;

    //~RefVectorOf will delete all adopted elements
    if (fEnumeration && !fEnumerationInherited)
        delete fEnumeration;
}

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

inline XMLBigDecimal* const DecimalDatatypeValidator::getMaxInclusive() const
{
    return fMaxInclusive;
}

inline XMLBigDecimal* const DecimalDatatypeValidator::getMaxExclusive() const
{
    return fMaxExclusive;
}

inline XMLBigDecimal* const DecimalDatatypeValidator::getMinInclusive() const
{
    return fMinInclusive;
}

inline XMLBigDecimal* const DecimalDatatypeValidator::getMinExclusive() const
{
    return fMinExclusive;
}

inline RefVectorOf<XMLBigDecimal>* DecimalDatatypeValidator::getEnumeration() const
{
    return fEnumeration;
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

inline void  DecimalDatatypeValidator::setMaxInclusive(XMLBigDecimal* const newMaxInclusive)
{
    if (fMaxInclusive) delete fMaxInclusive;
    fMaxInclusive = newMaxInclusive;
}

inline void  DecimalDatatypeValidator::setMaxExclusive(XMLBigDecimal* const newMaxExclusive)
{
    if (fMaxExclusive) delete fMaxExclusive;
    fMaxExclusive = newMaxExclusive;
}

inline void  DecimalDatatypeValidator::setMinInclusive(XMLBigDecimal* const newMinInclusive)
{
    if (fMinInclusive) delete fMinInclusive;
    fMinInclusive = newMinInclusive;
}

inline void  DecimalDatatypeValidator::setMinExclusive(XMLBigDecimal* const newMinExclusive)
{
    if (fMinExclusive) delete fMinExclusive;
    fMinExclusive = newMinExclusive;
}

inline void  DecimalDatatypeValidator::setEnumeration(RefVectorOf<XMLBigDecimal>* newEnum)
{
    if (fEnumeration) delete fEnumeration;
    fEnumeration = newEnum;
}

/**
  * End of file DecimalDatatypeValidator.hpp
  */
#endif
