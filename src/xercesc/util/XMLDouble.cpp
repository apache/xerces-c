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
 * Revision 1.1  2002/02/01 22:22:15  peiyongz
 * Initial revision
 *
 * Revision 1.16  2001/11/19 21:33:42  peiyongz
 * Reorganization: Double/Float
 *
 * Revision 1.15  2001/11/19 17:27:55  peiyongz
 * Boundary Values updated
 *
 * Revision 1.14  2001/10/26 16:37:46  peiyongz
 * Add thread safe code
 *
 * Revision 1.12  2001/09/20 13:11:41  knoaman
 * Regx  + misc. fixes
 *
 * Revision 1.11  2001/09/14 13:57:59  peiyongz
 * exponent is a must if 'E' or 'e' is present.
 *
 * Revision 1.10  2001/08/23 11:54:26  tng
 * Add newline at the end and various typo fixes.
 *
 * Revision 1.9  2001/08/21 15:10:15  peiyongz
 * Bugzilla# 3017: MSVC5.0: C2202: 'compareSpecial' : not all
 * control paths return a value
 *
 * Revision 1.8  2001/08/14 22:10:20  peiyongz
 * new exception message added
 *
 * Revision 1.7  2001/07/31 17:38:16  peiyongz
 * Fix: memory leak by static (boundry) objects
 *
 * Revision 1.6  2001/07/31 13:48:29  peiyongz
 * fValue removed
 *
 * Revision 1.5  2001/07/27 20:43:53  peiyongz
 * copy ctor: to check for special types.
 *
 * Revision 1.4  2001/07/26 18:21:15  peiyongz
 * Boundary Checking
 *
 * Revision 1.3  2001/07/25 19:06:56  peiyongz
 * Fix to Linux compilation error: XMLDouble::LiteralType' is not an aggregate type
 *
 * Revision 1.2  2001/07/24 21:52:27  peiyongz
 * XMLDouble: move fg...String to XMLUni
 *
 * Revision 1.1  2001/07/24 13:58:11  peiyongz
 * XMLDouble and related supporting methods from XMLBigInteger/XMLBigDecimal
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLDouble.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/NumberFormatException.hpp>

//---------
// TODO:
//
//   Double is supposed to hold 64 bit number and currently
//   we have implemented the checking on boundary values, such
//   the maximum/minimum positive/negative number.
//
//   We also need to do roundup/roundoff for extra bit in the
//   lexical representation.
//   In order to do that, we need to store them in a canonical form,
//   like this.
//   [-]?[1-9].[0-9]+[E|e][-]?[0-9]+
//
//   Then we can impose the limit of length after the '.' before 'E'.
//
//---------

// ---------------------------------------------------------------------------
//  local data member
// ---------------------------------------------------------------------------

//
//   2^53 x 2^970 	=   2^1023	=  8.9884656743115795386465259539451e+307
//                                +
//                                  8.9884 656743 115795 386465 259539 451
//                                e+307
//
static const XMLCh DBL_MAX_POSITIVE[] =
{
    chPlus,
    chDigit_8, chPeriod,  chDigit_9, chDigit_8, chDigit_8, chDigit_4,
    chDigit_6, chDigit_5, chDigit_6, chDigit_7, chDigit_4, chDigit_3,
    chDigit_1, chDigit_1, chDigit_5, chDigit_7, chDigit_9, chDigit_5,
    chDigit_3, chDigit_8, chDigit_6, chDigit_4, chDigit_6, chDigit_5,
    chDigit_2, chDigit_5, chDigit_9, chDigit_5, chDigit_3, chDigit_9,
    chDigit_4, chDigit_5, chDigit_1, 
    chLatin_E, chPlus,    chDigit_3, chDigit_0, chDigit_7, chNull
};

//
//   2^0  x 2^-1075 =   2^-1075	=  2.4703282292062327208828439643411e-324
//                                 +
//                                  2.4703 282292 062327 208828 439643 411
//                                 e-324
//
static const XMLCh DBL_MIN_POSITIVE[] =
{
    chPlus,
    chDigit_2, chPeriod,  chDigit_4, chDigit_7, chDigit_0, chDigit_3,
    chDigit_2, chDigit_8, chDigit_2, chDigit_2, chDigit_9, chDigit_2,
    chDigit_0, chDigit_6, chDigit_2, chDigit_3, chDigit_2, chDigit_7,
    chDigit_2, chDigit_0, chDigit_8, chDigit_8, chDigit_2, chDigit_8,
    chDigit_4, chDigit_3, chDigit_9, chDigit_6, chDigit_4, chDigit_3,
    chDigit_4, chDigit_1, chDigit_1, 
    chLatin_E, chDash,    chDigit_3, chDigit_2, chDigit_4, chNull
};

static const XMLCh DBL_MAX_NEGATIVE[] =
{
    chDash,
    chDigit_8, chPeriod,  chDigit_9, chDigit_8, chDigit_8, chDigit_4,
    chDigit_6, chDigit_5, chDigit_6, chDigit_7, chDigit_4, chDigit_3,
    chDigit_1, chDigit_1, chDigit_5, chDigit_7, chDigit_9, chDigit_5,
    chDigit_3, chDigit_8, chDigit_6, chDigit_4, chDigit_6, chDigit_5,
    chDigit_2, chDigit_5, chDigit_9, chDigit_5, chDigit_3, chDigit_9,
    chDigit_4, chDigit_5, chDigit_1, 
    chLatin_E, chPlus,    chDigit_3, chDigit_0, chDigit_7, chNull
};

static const XMLCh DBL_MIN_NEGATIVE[] =
{
    chDash,
    chDigit_2, chPeriod,  chDigit_4, chDigit_7, chDigit_0, chDigit_3,
    chDigit_2, chDigit_8, chDigit_2, chDigit_2, chDigit_9, chDigit_2,
    chDigit_0, chDigit_6, chDigit_2, chDigit_3, chDigit_2, chDigit_7,
    chDigit_2, chDigit_0, chDigit_8, chDigit_8, chDigit_2, chDigit_8,
    chDigit_4, chDigit_3, chDigit_9, chDigit_6, chDigit_4, chDigit_3,
    chDigit_4, chDigit_1, chDigit_1, 
    chLatin_E, chDash,    chDigit_3, chDigit_2, chDigit_4, chNull
};

//
// maxNegativeValue < minNegativeValue < 0 < minPositiveValue < maxPositiveValue
// They are all "Inclusive value"
//

static XMLDouble*  maxNegativeValue = 0;
static XMLDouble*  minNegativeValue = 0;
static XMLDouble*  minPositiveValue = 0;
static XMLDouble*  maxPositiveValue = 0;

static XMLMutex* sDoubleMutex = 0;
static XMLRegisterCleanup XMLDoubleCleanup;

// ---------------------------------------------------------------------------
//  ctor/dtor
// ---------------------------------------------------------------------------
XMLDouble::XMLDouble(const XMLCh* const strValue)
:XMLAbstractDoubleFloat()
{
    init(strValue);
}

XMLDouble::~XMLDouble()
{
}

//
//
void XMLDouble::checkBoundary(const XMLCh* const strValue)
{
    if (!sDoubleMutex)
    {
        XMLMutex* tmpMutex = new XMLMutex;
        if (XMLPlatformUtils::compareAndSwap((void**)&sDoubleMutex, tmpMutex, 0))
        {
            // Some other thread beat us to it, so let's clean up ours.
            delete tmpMutex;
        }
        else
        {
            //
            // the thread who creates the mutex succesfully, to
            // initialize the followings
            //
            maxNegativeValue = new XMLDouble(DBL_MAX_NEGATIVE);
            minNegativeValue = new XMLDouble(DBL_MIN_NEGATIVE);
            minPositiveValue = new XMLDouble(DBL_MIN_POSITIVE);
            maxPositiveValue = new XMLDouble(DBL_MAX_POSITIVE);

            // This is the real mutex.  Register it for cleanup at Termination.
            XMLDoubleCleanup.registerCleanup(reinitXMLDouble);
        }
    }

    //
    // by-pass boundary check for boundary value itself
    //
    if (( XMLString::compareString(strValue, DBL_MAX_NEGATIVE) == 0 ) ||
        ( XMLString::compareString(strValue, DBL_MIN_NEGATIVE) == 0 ) ||
        ( XMLString::compareString(strValue, DBL_MIN_POSITIVE) == 0 ) ||
        ( XMLString::compareString(strValue, DBL_MAX_POSITIVE) == 0 )  )
        return;

    //  error: this < maxNegativeValue
    if ( compareValues(this, maxNegativeValue) == -1 )
    {
        ThrowXML2(NumberFormatException
                , XMLExcepts::XMLNUM_DBL_FLT_maxNeg
                , strValue
                , DBL_MAX_NEGATIVE);
    }

    //  error: this > maxPositiveValue
    if ( compareValues(this, maxPositiveValue) ==  1 )
    {
        ThrowXML2(NumberFormatException
                , XMLExcepts::XMLNUM_DBL_FLT_maxPos
                , strValue
                , DBL_MAX_POSITIVE);
    }

    //  minNegativeValue < this < minPositiveValue
    //  value is not be representable
    if (( compareValues(this, minNegativeValue) ==  1 ) &&
         ( compareValues(this, minPositiveValue) == -1 )  )
    {
        ThrowXML3(NumberFormatException
                , XMLExcepts::XMLNUM_DBL_FLT_minNegPos
                , strValue
                , DBL_MIN_NEGATIVE
                , DBL_MIN_POSITIVE);
    }

}

// -----------------------------------------------------------------------
//  Notification that lazy data has been deleted
// -----------------------------------------------------------------------
void XMLDouble::reinitXMLDouble() {

    delete sDoubleMutex;
    sDoubleMutex = 0;

    delete maxNegativeValue;
    maxNegativeValue = 0;

    delete minNegativeValue;
    minNegativeValue = 0;

    delete minPositiveValue;
    minPositiveValue = 0;

    delete maxPositiveValue;
    maxPositiveValue = 0;

}