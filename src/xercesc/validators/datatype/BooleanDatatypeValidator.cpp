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
 * $Log$
 * Revision 1.2  2002/02/14 15:17:31  peiyongz
 * getEnumString()
 *
 * Revision 1.1.1.1  2002/02/01 22:22:40  peiyongz
 * sane_include
 *
 * Revision 1.7  2001/10/02 18:59:29  peiyongz
 * Invalid_Facet_Tag to display the tag name
 *
 * Revision 1.6  2001/06/22 16:26:01  peiyongz
 * fix: redefinition of SIZE
 *
 * Revision 1.5  2001/05/29 19:49:33  tng
 * Schema: Constraint Checking Fix in datatypeValidators.  By Pei Yong Zhang.
 *
 * Revision 1.4  2001/05/28 21:11:17  tng
 * Schema: Various DatatypeValidator fix.  By Pei Yong Zhang
 *
 * Revision 1.3  2001/05/18 13:36:44  tng
 * Schema: Catch RegularExpression exception and NumberFormatException
 *
 * Revision 1.2  2001/05/11 13:27:26  tng
 * Copyright update.
 *
 * Revision 1.1  2001/05/09 18:43:35  tng
 * Add StringDatatypeValidator and BooleanDatatypeValidator.  By Pei Yong Zhang.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/BooleanDatatypeValidator.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>

const int   ARRAYSIZE = 4;
const XMLCh fgValueSpace[][32] =
{
    { chLatin_f, chLatin_a, chLatin_l, chLatin_s, chLatin_e, chNull },
    { chLatin_t, chLatin_r, chLatin_u, chLatin_e, chNull },
    { chDigit_0, chNull },
    { chDigit_1, chNull }
};

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
BooleanDatatypeValidator::BooleanDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefVectorOf<XMLCh>*           const enums
                        , const int                           finalSet)
:DatatypeValidator(baseValidator, facets, finalSet, DatatypeValidator::Boolean)
{

    // Set Facets if any defined
    if ( facets )
    {

        // Boolean shall NOT have enumeration
        if (enums)
            ThrowXML1(InvalidDatatypeFacetException
                    , XMLExcepts::FACET_Invalid_Tag
                    , "enumeration");
    
        XMLCh* key;
        XMLCh* value;
        RefHashTableOfEnumerator<KVStringPair> e(facets);

        while (e.hasMoreElements())
        {
            KVStringPair pair = e.nextElement();
            key = pair.getKey();
            value = pair.getValue();

            if (XMLString::compareString(key, SchemaSymbols::fgELT_PATTERN)==0)
            {
                setPattern(value);
                setFacetsDefined(DatatypeValidator::FACET_PATTERN);
            }
            else
            {
                ThrowXML1(InvalidDatatypeFacetException
                        , XMLExcepts::FACET_Invalid_Tag
                        , key);
            }

        }

    }// End of facet setting
}

void BooleanDatatypeValidator::checkContent( const XMLCh* const content, bool asBase)
{

    //validate against base validator if any
    BooleanDatatypeValidator *pBaseValidator = (BooleanDatatypeValidator*) this->getBaseValidator();
    if (pBaseValidator !=0)
        pBaseValidator->checkContent(content, true);

    // we check pattern first
    if ( (getFacetsDefined() & DatatypeValidator::FACET_PATTERN ) != 0 )
    {
        // lazy construction
        if (getRegex() ==0) {
            try {         
                setRegex(new RegularExpression(getPattern(), SchemaSymbols::fgRegEx_XOption));
            }
            catch (XMLException &e)
            {
                ThrowXML1(InvalidDatatypeValueException, XMLExcepts::RethrowError, e.getMessage());
            }
        }

        if (getRegex()->matches(content) ==false)
        {
            ThrowXML2(InvalidDatatypeValueException
                    , XMLExcepts::VALUE_NotMatch_Pattern
                    , content
                    , getPattern());
        }
    }

    // if this is a base validator, we only need to check pattern facet
    // all other facet were inherited by the derived type
    if (asBase)
        return;

    int   i = 0;
    for ( ; i < ARRAYSIZE; i++ )
    {
        if ( XMLString::compareString(content, fgValueSpace[i]) ==0 )
            break;
    }

    if (i == ARRAYSIZE)
        ThrowXML(InvalidDatatypeValueException, XMLExcepts::CM_UnaryOpHadBinType);
        //Not valid boolean type

}

int BooleanDatatypeValidator::compare(const XMLCh* const lValue
                                    , const XMLCh* const rValue)
{
    // need to check by bool semantics
    // 1 == true
    // 0 == false

    if (( XMLString::compareString(lValue, fgValueSpace[0]) ==0 ) ||
        ( XMLString::compareString(lValue, fgValueSpace[2]) ==0 ) )
    {
        if (( XMLString::compareString(rValue, fgValueSpace[0]) ==0 ) ||
            ( XMLString::compareString(rValue, fgValueSpace[2]) ==0 ) )
            return 0;
    }
    else
    if (( XMLString::compareString(lValue, fgValueSpace[1]) ==0 ) ||
        ( XMLString::compareString(lValue, fgValueSpace[3]) ==0 ) )
    {
        if (( XMLString::compareString(rValue, fgValueSpace[1]) ==0 ) ||
            ( XMLString::compareString(rValue, fgValueSpace[3]) ==0 ) )
            return 0;
    }

    return 1;
}

const RefVectorOf<XMLCh>* BooleanDatatypeValidator::getEnumString() const
{
	return 0;
}

/**
  * End of file BooleanDatatypeValidator.cpp
  */

