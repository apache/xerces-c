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
 * Revision 1.1  2001/11/19 21:33:42  peiyongz
 * Reorganization: Double/Float
 *
 *
 */

#ifndef XML_ABSTRACT_DOUBLE_FLOAT_HPP
#define XML_ABSTRACT_DOUBLE_FLOAT_HPP

#include <util/XercesDefs.hpp>
#include <util/XMLNumber.hpp>
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

/***
 * 3.2.4.1 Lexical representation
 *
 *   float values have a lexical representation consisting of a mantissa followed, 
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

class XMLUTIL_EXPORT XMLAbstractDoubleFloat : public XMLNumber
{
public:

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

    virtual ~XMLAbstractDoubleFloat();
  
    virtual XMLCh*        toString() const;

    virtual int           getSign() const;

protected:

    //
    // To be used by derived class exclusively
    //
    XMLAbstractDoubleFloat();
    
    void                  init(const XMLCh* const strValue);

    /**
	 * Compares this object to the specified object.
	 * The result is <code>true</code> if and only if the argument is not
	 * <code>null</code> and is an <code>XMLAbstractDoubleFloat</code> object that contains
	 * the same <code>int</code> value as this object.
	 *
	 * @param   obj   the object to compare with.
	 * @return  <code>true</code> if the objects are the same;
	 *          <code>false</code> otherwise.
	 */

    static int            compareValues(const XMLAbstractDoubleFloat* const lValue
                                      , const XMLAbstractDoubleFloat* const rValue);

    //
    // to be overwritten by derived class
    //
    virtual void          checkBoundary(const XMLCh* const strValue) = 0;

private:
    //
    // Unimplemented
    //
    // copy ctor
    // assignment ctor
    //
    XMLAbstractDoubleFloat(const XMLAbstractDoubleFloat& toCopy);
    operator=(const XMLAbstractDoubleFloat& toAssign);

    inline bool           isSpecialValue() const;

    static int            compareSpecial(const XMLAbstractDoubleFloat* const specialValue
                                       , const XMLAbstractDoubleFloat* const normalValue);

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
    // -----------------------------------------------------------------------

    XMLBigDecimal*          fMantissa;
	XMLBigInteger*          fExponent;   
    LiteralType             fType;
};

bool XMLAbstractDoubleFloat::isSpecialValue() const
{
    return (fType < SpecialTypeNum);
}

#endif
