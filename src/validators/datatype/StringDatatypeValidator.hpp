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

#if !defined(STRING_DATATYPEVALIDATOR_HPP)
#define STRING_DATATYPEVALIDATOR_HPP

#include <validators/datatype/DatatypeValidator.hpp>
#include <validators/schema/SchemaSymbols.hpp>

class VALIDATORS_EXPORT StringDatatypeValidator : public DatatypeValidator
{
public:

    // -----------------------------------------------------------------------
    //  Public ctor/dtor
    // -----------------------------------------------------------------------
	/** @name Constructor. */
    //@{

    StringDatatypeValidator();

    StringDatatypeValidator(DatatypeValidator*            const baseValidator
                          , RefHashTableOf<KVStringPair>* const facets
                          , const int                           finalSet);


    virtual ~StringDatatypeValidator();

	//@}

    // -----------------------------------------------------------------------
    // Getter methods
    // -----------------------------------------------------------------------
    /** @name Getter Functions */
    //@{

    short getWSFacet() const;

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
                                 , const int                           finalSet);

private:

    void checkContent(const XMLCh* const content, bool asBase);

    void init(DatatypeValidator*            const baseValidator
            , RefHashTableOf<KVStringPair>* const facets);

    void cleanUp();

// -----------------------------------------------------------------------
// Getter methods
// -----------------------------------------------------------------------

    unsigned int         getLength() const;

    unsigned int         getMaxLength() const;

    unsigned int         getMinLength() const;

    RefVectorOf<XMLCh>*  getEnumeration() const;

// -----------------------------------------------------------------------
// Setter methods
// -----------------------------------------------------------------------

    void                 setLength(unsigned int);

    void                 setMaxLength(unsigned int);

    void                 setMinLength(unsigned int);

    void                 setEnumeration(RefVectorOf<XMLCh>* );

    void                 setWhiteSpace(short);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    // -----------------------------------------------------------------------
     unsigned int         fLength;
     unsigned int         fMaxLength;
     unsigned int         fMinLength;
     RefVectorOf<XMLCh>*  fEnumeration;
     short                fWhiteSpace;         //DatatypeValidator::PRESERVE

};

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
inline StringDatatypeValidator::StringDatatypeValidator()
:DatatypeValidator(0, 0, 0, DatatypeValidator::String)
,fLength(0)
,fMaxLength(SchemaSymbols::fgINT_MAX_VALUE)
,fMinLength(0)
,fEnumeration(0)
,fWhiteSpace(DatatypeValidator::PRESERVE)
{}

inline StringDatatypeValidator::~StringDatatypeValidator()
{
    cleanUp();
}

// -----------------------------------------------------------------------
// Getter methods
// -----------------------------------------------------------------------
inline short StringDatatypeValidator::getWSFacet() const {

    return fWhiteSpace;
}

// -----------------------------------------------------------------------
// Compare methods
// -----------------------------------------------------------------------
inline int StringDatatypeValidator::compare(const XMLCh* const lValue
                                          , const XMLCh* const rValue)
{
    return XMLString::compareString(lValue, rValue);
}

inline DatatypeValidator* StringDatatypeValidator::newInstance(
                                      DatatypeValidator* const            baseValidator
                                    , RefHashTableOf<KVStringPair>* const facets
                                    , const int                           finalSet)
{
    return (DatatypeValidator*) new StringDatatypeValidator(baseValidator, facets, finalSet);
}

inline void StringDatatypeValidator::validate( const XMLCh* const content)
{
    checkContent(content, false);
}

inline void StringDatatypeValidator::cleanUp()
{
    //~RefVectorOf will delete all adopted elements
    delete fEnumeration;
}

// -----------------------------------------------------------------------
// Getter methods
// -----------------------------------------------------------------------

inline unsigned int StringDatatypeValidator::getLength() const
{
    return fLength;
}

inline unsigned int StringDatatypeValidator::getMaxLength() const
{
    return fMaxLength;
}

inline unsigned int StringDatatypeValidator::getMinLength() const
{
    return fMinLength;
}

inline RefVectorOf<XMLCh>* StringDatatypeValidator:: getEnumeration() const
{
    return fEnumeration;
}

// -----------------------------------------------------------------------
// Setter methods
// -----------------------------------------------------------------------

inline void StringDatatypeValidator::setLength(unsigned int newLength)
{
    fLength = newLength;
}

inline void StringDatatypeValidator::setMaxLength(unsigned int newMaxLength)
{
    fMaxLength = newMaxLength;
}

inline void StringDatatypeValidator::setMinLength(unsigned int newMinLength)
{
    fMinLength = newMinLength;
}

inline void StringDatatypeValidator::setEnumeration(RefVectorOf<XMLCh>* newEnum)
{
    if (fEnumeration)
        delete fEnumeration;

    fEnumeration = newEnum;
}

inline void StringDatatypeValidator::setWhiteSpace(short newValue)
{
    fWhiteSpace = newValue;
}

/**
  * End of file StringDatatypeValidator.hpp
  */
#endif
