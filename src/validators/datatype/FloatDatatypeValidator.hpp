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
 * Revision 1.1  2001/07/26 20:42:16  peiyongz
 * FloatDatatypeValidator
 *
 *
 */

#if !defined(FLOAT_DATATYPEVALIDATOR_HPP)
#define FLOAT_DATATYPEVALIDATOR_HPP

#include <validators/datatype/DatatypeValidator.hpp>
#include <util/RefVectorOf.hpp>
#include <util/XMLFloat.hpp>

class VALIDATORS_EXPORT FloatDatatypeValidator : public DatatypeValidator
{
public:

    // -----------------------------------------------------------------------
    //  Public ctor/dtor
    // -----------------------------------------------------------------------
	/** @name Constructor. */
    //@{

    FloatDatatypeValidator();

    FloatDatatypeValidator(DatatypeValidator*            const baseValidator
                           , RefHashTableOf<KVStringPair>* const facets
                           , RefVectorOf<XMLCh>*           const enums
                           , const int                           finalSet);

    virtual ~FloatDatatypeValidator();

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
    DatatypeValidator* newInstance(DatatypeValidator* const            baseValidator
                                 , RefHashTableOf<KVStringPair>* const facets
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

    XMLFloat* const            getMaxInclusive() const;

    XMLFloat* const            getMaxExclusive() const;

    XMLFloat* const            getMinInclusive() const;

    XMLFloat* const            getMinExclusive() const;

    RefVectorOf<XMLFloat>*     getEnumeration() const;

// -----------------------------------------------------------------------
// Setter methods
// -----------------------------------------------------------------------

    void  setMaxInclusive(XMLFloat* const);

    void  setMaxExclusive(XMLFloat* const);

    void  setMinInclusive(XMLFloat* const);

    void  setMinExclusive(XMLFloat* const);

    void  setEnumeration(RefVectorOf<XMLFloat>* );

    // -----------------------------------------------------------------------
    //  Private data members
    //
    // -----------------------------------------------------------------------
     bool                 fEnumerationInherited;

     XMLFloat*       fMaxInclusive;
     XMLFloat*       fMaxExclusive;
     XMLFloat*       fMinInclusive;
     XMLFloat*       fMinExclusive;

     RefVectorOf<XMLFloat>*  fEnumeration;    // save the actual value

};

inline FloatDatatypeValidator::FloatDatatypeValidator()
:DatatypeValidator(0, 0, 0, DatatypeValidator::Float)
, fEnumerationInherited(false)
, fMaxInclusive(0)
, fMaxExclusive(0)
, fMinInclusive(0)
, fMinExclusive(0)
, fEnumeration(0)
{
}

inline FloatDatatypeValidator::~FloatDatatypeValidator()
{
    cleanUp();
}

// -----------------------------------------------------------------------
// Compare methods
// -----------------------------------------------------------------------
inline int FloatDatatypeValidator::compare(const XMLCh* const lValue
                                           , const XMLCh* const rValue)
{
    return XMLFloat::compareValues(new XMLFloat(lValue)
                                      , new XMLFloat(rValue));
}

inline DatatypeValidator* FloatDatatypeValidator::newInstance(
                                      DatatypeValidator* const            baseValidator
                                    , RefHashTableOf<KVStringPair>* const facets
                                    , RefVectorOf<XMLCh>*           const enums
                                    , const int                           finalSet)
{
    return (DatatypeValidator*) new FloatDatatypeValidator(baseValidator, facets, enums, finalSet);
}

inline void FloatDatatypeValidator::validate( const XMLCh* const content)
{
    checkContent(content, false);
}

inline void FloatDatatypeValidator::cleanUp()
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
inline XMLFloat* const FloatDatatypeValidator::getMaxInclusive() const
{
    return fMaxInclusive;
}

inline XMLFloat* const FloatDatatypeValidator::getMaxExclusive() const
{
    return fMaxExclusive;
}

inline XMLFloat* const FloatDatatypeValidator::getMinInclusive() const
{
    return fMinInclusive;
}

inline XMLFloat* const FloatDatatypeValidator::getMinExclusive() const
{
    return fMinExclusive;
}

inline RefVectorOf<XMLFloat>* FloatDatatypeValidator::getEnumeration() const
{
    return fEnumeration;
}

// -----------------------------------------------------------------------
// Setter methods
// -----------------------------------------------------------------------
inline void  FloatDatatypeValidator::setMaxInclusive(XMLFloat* const newMaxInclusive)
{
    if (fMaxInclusive) delete fMaxInclusive;
    fMaxInclusive = newMaxInclusive;
}

inline void  FloatDatatypeValidator::setMaxExclusive(XMLFloat* const newMaxExclusive)
{
    if (fMaxExclusive) delete fMaxExclusive;
    fMaxExclusive = newMaxExclusive;
}

inline void  FloatDatatypeValidator::setMinInclusive(XMLFloat* const newMinInclusive)
{
    if (fMinInclusive) delete fMinInclusive;
    fMinInclusive = newMinInclusive;
}

inline void  FloatDatatypeValidator::setMinExclusive(XMLFloat* const newMinExclusive)
{
    if (fMinExclusive) delete fMinExclusive;
    fMinExclusive = newMinExclusive;
}

inline void  FloatDatatypeValidator::setEnumeration(RefVectorOf<XMLFloat>* newEnum)
{
    if (fEnumeration) delete fEnumeration;
    fEnumeration = newEnum;
}

/**
  * End of file FloatDatatypeValidator.hpp
  */
#endif
