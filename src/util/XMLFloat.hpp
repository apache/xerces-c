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
 * Revision 1.5  2001/10/25 21:54:50  peiyongz
 * Apply XMLRegisterCleanup
 *
 * Revision 1.4  2001/09/27 14:54:20  peiyongz
 * DTV Reorganization: derived from XMLNumber
 *
 * Revision 1.3  2001/08/29 19:03:03  peiyongz
 * Bugzilla# 2816:on AIX 4.2, xlC 3 r ev.1, Compilation error on inline method
 *
 * Revision 1.2  2001/07/31 13:48:29  peiyongz
 * fValue removed
 *
 * Revision 1.1  2001/07/26 20:41:37  peiyongz
 * XMLFloat
 *
 *
 */

#ifndef XML_FLOAT_HPP
#define XML_FLOAT_HPP

#include <util/XercesDefs.hpp>
#include <util/XMLNumber.hpp>
#include <util/XMLBigDecimal.hpp>

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

class XMLUTIL_EXPORT XMLFloat : public XMLNumber
{
public:

	/**
	 * Constructs a newly allocated <code>XMLFloat</code> object that
	 * represents the value represented by the string.
	 *
	 * @param      the <code>String</code> to be converted to an
	 *                 <code>XMLFloat</code>.
	 * @exception  NumberFormatException  if the <code>String</code> does not
	 *               contain a parsable XMLFloat.
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

    XMLFloat(const XMLCh* const strValue);

    ~XMLFloat();

    XMLFloat(const XMLFloat& toCopy);
   
    virtual XMLCh*       toString() const;    

    virtual int          getSign() const;

	/**
	 * Compares this object to the specified object.
	 * The result is <code>true</code> if and only if the argument is not
	 * <code>null</code> and is an <code>XMLFloat</code> object that contains
	 * the same <code>int</code> value as this object.
	 *
	 * @param   obj   the object to compare with.
	 * @return  <code>true</code> if the objects are the same;
	 *          <code>false</code> otherwise.
	 */
	bool operator==(const XMLFloat& toCompare) const;

    static int            compareValues(const XMLFloat* const lValue
                                      , const XMLFloat* const rValue);

    // -----------------------------------------------------------------------
    //  Notification that lazy data has been deleted
    // -----------------------------------------------------------------------
	static void reinitXMLFloat();   

private:

    void                  init(const XMLCh* const strValue);

    void                  checkBoundary(const XMLCh* const strValue);

    void                  cleanUp();

    bool                  isSpecialValue() const;

    static int            compareSpecial(const XMLFloat* const specialValue
                                       , const XMLFloat* const normalValue);

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
    // -----------------------------------------------------------------------

    XMLBigDecimal*          fMantissa;
	XMLBigInteger*          fExponent;   
    LiteralType             fType;
};

inline bool XMLFloat::operator==(const XMLFloat& toCompare) const
{
    return ( XMLFloat::compareValues(this, &toCompare) == 0 ? true : false);
}

inline bool XMLFloat::isSpecialValue() const
{
    return (fType < SpecialTypeNum);
}

inline void XMLFloat::cleanUp()
{
    if (fMantissa)
        delete fMantissa;

    if (fExponent)
        delete fExponent;
}

inline int XMLFloat::getSign() const
{
    return fMantissa->getSign();
}

inline XMLFloat::~XMLFloat()
{
    cleanUp();
}

#endif
