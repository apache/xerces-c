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

#if !defined(ANYSIMPLETYPEDATATYPEVALIDATOR_HPP)
#define ANYSIMPLETYPEDATATYPEVALIDATOR_HPP

#include <xercesc/validators/datatype/DatatypeValidator.hpp>


class VALIDATORS_EXPORT AnySimpleTypeDatatypeValidator : public DatatypeValidator
{
public:
    // -----------------------------------------------------------------------
    //  Public Constructor
    // -----------------------------------------------------------------------
	/** @name Constructor */
    //@{

    AnySimpleTypeDatatypeValidator();

	//@}

    // -----------------------------------------------------------------------
    //  Public Destructor
    // -----------------------------------------------------------------------
	/** @name Destructor. */
    //@{

    virtual ~AnySimpleTypeDatatypeValidator();

	//@}

    // -----------------------------------------------------------------------
    // Getter methods
    // -----------------------------------------------------------------------
    /** @name Getter Functions */
    //@{

    /**
      * Returns default value (collapse) for whiteSpace facet.
      * This function is overwritten in StringDatatypeValidator.
      */
    short getWSFacet () const;

    //@}

    // -----------------------------------------------------------------------
    // Validation methods
    // -----------------------------------------------------------------------
    /** @name Validation Function */
    //@{

     /**
	   * Checks that the "content" string is valid datatype.
       * If invalid, a Datatype validation exception is thrown.
	   *
	   * @param  content   A string containing the content to be validated
	   *
	   */
	void validate(const XMLCh* const content);

    /**
      * Checks whether a given type can be used as a substitute
      *
      * @param  toCheck    A datatype validator of the type to be used as a
      *                    substitute
      *
      */

    bool isSubstitutableBy(const DatatypeValidator* const toCheck);

	 //@}

    // -----------------------------------------------------------------------
    // Compare methods
    // -----------------------------------------------------------------------
    /** @name Compare Function */
    //@{

    /**
      * Compares content in the Domain value vs. lexical value.
      *
      * @param  value1    string to compare
      *
      * @param  value2    string to compare
      *
      */
    int compare(const XMLCh* const value1, const XMLCh* const value2);

    //@}

    /**
      * Returns an instance of the base datatype validator class
	  * Used by the DatatypeValidatorFactory.
      */
	DatatypeValidator* newInstance(RefHashTableOf<KVStringPair>* const,
                                   RefVectorOf<XMLCh>* const enums,
                                   const int finalSet);
};


// ---------------------------------------------------------------------------
//  DatatypeValidator: Getters
// ---------------------------------------------------------------------------
inline short AnySimpleTypeDatatypeValidator::getWSFacet() const {

    return DatatypeValidator::PRESERVE;
}


// ---------------------------------------------------------------------------
//  DatatypeValidators: Compare methods
// ---------------------------------------------------------------------------
inline int AnySimpleTypeDatatypeValidator::compare(const XMLCh* const lValue,
                                                   const XMLCh* const rValue)
{
    return -1;
}

// ---------------------------------------------------------------------------
//  DatatypeValidators: Validation methods
// ---------------------------------------------------------------------------
inline bool
AnySimpleTypeDatatypeValidator::isSubstitutableBy(const DatatypeValidator* const toCheck)
{
    return true;
}

inline void
AnySimpleTypeDatatypeValidator::validate(const XMLCh* const content)
{
    return;
}

#endif

/**
  * End of file AnySimpleTypeDatatypeValidator.hpp
  */

