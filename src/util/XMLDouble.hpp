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
 * Revision 1.3  2001/07/26 18:21:15  peiyongz
 * Boundary Checking
 *
 * Revision 1.2  2001/07/24 21:52:27  peiyongz
 * XMLDouble: move fg...String to XMLUni
 *
 * Revision 1.1  2001/07/24 13:58:11  peiyongz
 * XMLDouble and related supporting methods from XMLBigInteger/XMLBigDecimal
 *
 */

#ifndef XML_DOUBLE_HPP
#define XML_DOUBLE_HPP

#include <util/XercesDefs.hpp>
#include <util/XMLBigDecimal.hpp>

/***
 * 3.2.5.1 Lexical representation
 *
 *   double values have a lexical representation consisting of a mantissa followed, 
 *   optionally, by the character "E" or "e", followed by an exponent. 
 *
 *   The exponent ·must· be an integer. 
 *   The mantissa must be a decimal number. 
 *   The representations for exponent and mantissa must follow the lexical rules 
 *   for integer and decimal. 
 *
 *   If the "E" or "e" and the following exponent are omitted, 
 *   an exponent value of 0 is assumed. 
***/

class XMLUTIL_EXPORT XMLDouble
{
public:

	/**
	 * Constructs a newly allocated <code>XMLDouble</code> object that
	 * represents the value represented by the string.
	 *
	 * @param      the <code>String</code> to be converted to an
	 *                 <code>XMLDouble</code>.
	 * @exception  NumberFormatException  if the <code>String</code> does not
	 *               contain a parsable XMLDouble.
	 */

    enum LiteralType
    {
        NegINF,
        NegZero,
        PosZero,
        PosINF,
        NaN,
        SpecialTypeNum = 5,
        Normal
    };

    XMLDouble(const XMLCh* const strValue);

    ~XMLDouble();

    XMLDouble(const XMLDouble& toCopy);
   
    double                doubleValue() const;

	/**
	 *  Return string representation of the decimal value.
     *  A decimal point will be included as necessary, 
     *  the caller of this method is responsible for the 
     *  de-allocation of the memory.
	 */
    XMLCh*                toString() const;

	/**
	 * Compares this object to the specified object.
	 * The result is <code>true</code> if and only if the argument is not
	 * <code>null</code> and is an <code>XMLDouble</code> object that contains
	 * the same <code>int</code> value as this object.
	 *
	 * @param   obj   the object to compare with.
	 * @return  <code>true</code> if the objects are the same;
	 *          <code>false</code> otherwise.
	 */
	bool operator==(const XMLDouble& toCompare) const;

    static int            compareValues(const XMLDouble* const lValue
                                      , const XMLDouble* const rValue);

private:

    void                  init(const XMLCh* const strValue);

    void                  checkBoundary(const XMLCh* const strValue);

    void                  cleanUp();

    bool                  isSpecialValue() const;

    static int            compareSpecial(const XMLDouble* const specialValue
                                       , const XMLDouble* const normalValue);

    static bool           isInitialized;

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fMantissa
    //     the XMLBigDecimal holding the value of mantissa.
    //
    //  fExponent
    //     the XMLBigInteger holding the value of exponent.
    //
    //  fType
    //     the type of the object.
    //
    //  fValue
    //     the built-in double value of the object.
    //
    // -----------------------------------------------------------------------

    XMLBigDecimal*          fMantissa;
	XMLBigInteger*          fExponent;   
    LiteralType             fType;
    double                  fValue;
};

inline XMLDouble::~XMLDouble()
{
    cleanUp();
}

inline double XMLDouble::doubleValue() const
{
    return fValue;
}

inline bool XMLDouble::operator==(const XMLDouble& toCompare) const
{
    return ( XMLDouble::compareValues(this, &toCompare) == 0 ? true : false);
}

inline bool XMLDouble::isSpecialValue() const
{
    return (fType < SpecialTypeNum);
}

inline void XMLDouble::cleanUp()
{
    if (fMantissa)
        delete fMantissa;

    if (fExponent)
        delete fExponent;
}

#endif
