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
 * Revision 1.2  2001/08/21 18:42:53  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.1  2001/07/24 13:59:03  peiyongz
 * DoubleDTV
 *
 */

#if !defined(DOUBLE_DATATYPEVALIDATOR_HPP)
#define DOUBLE_DATATYPEVALIDATOR_HPP

#include <validators/datatype/DatatypeValidator.hpp>
#include <util/RefVectorOf.hpp>
#include <util/XMLDouble.hpp>

class VALIDATORS_EXPORT DoubleDatatypeValidator : public DatatypeValidator
{
public:

    // -----------------------------------------------------------------------
    //  Public ctor/dtor
    // -----------------------------------------------------------------------
	/** @name Constructor. */
    //@{

    DoubleDatatypeValidator();

    DoubleDatatypeValidator(DatatypeValidator*            const baseValidator
                           , RefHashTableOf<KVStringPair>* const facets
                           , RefVectorOf<XMLCh>*           const enums
                           , const int                           finalSet);

    virtual ~DoubleDatatypeValidator();

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

    XMLDouble* const            getMaxInclusive() const;

    XMLDouble* const            getMaxExclusive() const;

    XMLDouble* const            getMinInclusive() const;

    XMLDouble* const            getMinExclusive() const;

    RefVectorOf<XMLDouble>*     getEnumeration() const;

// -----------------------------------------------------------------------
// Setter methods
// -----------------------------------------------------------------------

    void  setMaxInclusive(XMLDouble* const);

    void  setMaxExclusive(XMLDouble* const);

    void  setMinInclusive(XMLDouble* const);

    void  setMinExclusive(XMLDouble* const);

    void  setEnumeration(RefVectorOf<XMLDouble>* );

    // -----------------------------------------------------------------------
    //  Private data members
    //
    // -----------------------------------------------------------------------
     bool                 fEnumerationInherited;

     XMLDouble*       fMaxInclusive;
     XMLDouble*       fMaxExclusive;
     XMLDouble*       fMinInclusive;
     XMLDouble*       fMinExclusive;

     RefVectorOf<XMLDouble>*  fEnumeration;    // save the actual value

};

// -----------------------------------------------------------------------
// Compare methods
// -----------------------------------------------------------------------
inline int DoubleDatatypeValidator::compare(const XMLCh* const lValue
                                           , const XMLCh* const rValue)
{
    return XMLDouble::compareValues(new XMLDouble(lValue)
                                      , new XMLDouble(rValue));
}

inline DatatypeValidator* DoubleDatatypeValidator::newInstance(
                                      DatatypeValidator* const            baseValidator
                                    , RefHashTableOf<KVStringPair>* const facets
                                    , RefVectorOf<XMLCh>*           const enums
                                    , const int                           finalSet)
{
    return (DatatypeValidator*) new DoubleDatatypeValidator(baseValidator, facets, enums, finalSet);
}

inline void DoubleDatatypeValidator::validate( const XMLCh* const content)
{
    checkContent(content, false);
}

inline void DoubleDatatypeValidator::cleanUp()
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
inline XMLDouble* const DoubleDatatypeValidator::getMaxInclusive() const
{
    return fMaxInclusive;
}

inline XMLDouble* const DoubleDatatypeValidator::getMaxExclusive() const
{
    return fMaxExclusive;
}

inline XMLDouble* const DoubleDatatypeValidator::getMinInclusive() const
{
    return fMinInclusive;
}

inline XMLDouble* const DoubleDatatypeValidator::getMinExclusive() const
{
    return fMinExclusive;
}

inline RefVectorOf<XMLDouble>* DoubleDatatypeValidator::getEnumeration() const
{
    return fEnumeration;
}

// -----------------------------------------------------------------------
// Setter methods
// -----------------------------------------------------------------------
inline void  DoubleDatatypeValidator::setMaxInclusive(XMLDouble* const newMaxInclusive)
{
    if (fMaxInclusive) delete fMaxInclusive;
    fMaxInclusive = newMaxInclusive;
}

inline void  DoubleDatatypeValidator::setMaxExclusive(XMLDouble* const newMaxExclusive)
{
    if (fMaxExclusive) delete fMaxExclusive;
    fMaxExclusive = newMaxExclusive;
}

inline void  DoubleDatatypeValidator::setMinInclusive(XMLDouble* const newMinInclusive)
{
    if (fMinInclusive) delete fMinInclusive;
    fMinInclusive = newMinInclusive;
}

inline void  DoubleDatatypeValidator::setMinExclusive(XMLDouble* const newMinExclusive)
{
    if (fMinExclusive) delete fMinExclusive;
    fMinExclusive = newMinExclusive;
}

inline void  DoubleDatatypeValidator::setEnumeration(RefVectorOf<XMLDouble>* newEnum)
{
    if (fEnumeration) delete fEnumeration;
    fEnumeration = newEnum;
}

/**
  * End of file DoubleDatatypeValidator.hpp
  */
#endif
