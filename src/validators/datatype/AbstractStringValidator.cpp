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
 * Revision 1.4  2001/09/24 15:30:16  peiyongz
 * DTV Reorganization: init() to be invoked from derived class' ctor to allow
 *        correct resolution of virtual methods like assignAdditionalFacet(),
 *        inheritAdditionalFacet(), etc.
 *
 * Revision 1.3  2001/09/19 18:48:27  peiyongz
 * DTV reorganization:getLength() added, move inline to class declaration to avoid inline
 * function interdependency.
 *
 * Revision 1.2  2001/09/18 21:16:42  peiyongz
 * DTV reorganization: temp vars to replace repeated invocation of getFacetsDefined()
 *
 * Revision 1.1  2001/09/18 14:45:04  peiyongz
 * DTV reorganization
 *
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/datatype/AbstractStringValidator.hpp>
#include <validators/schema/SchemaSymbols.hpp>
#include <validators/datatype/InvalidDatatypeFacetException.hpp>
#include <validators/datatype/InvalidDatatypeValueException.hpp>
#include <util/NumberFormatException.hpp>

static const int BUF_LEN = 64;
static XMLCh value1[BUF_LEN+1];
static XMLCh value2[BUF_LEN+1];

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
AbstractStringValidator::~AbstractStringValidator()
{
    //~RefVectorOf will delete all adopted elements
    if (fEnumeration && !fEnumerationInherited)
    {
        delete fEnumeration;
        fEnumeration = 0;
    }
}

AbstractStringValidator::AbstractStringValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , const int                           finalSet
                        , const ValidatorType                 type)
:DatatypeValidator(baseValidator, facets, finalSet, type)
,fLength(0)
,fMaxLength(SchemaSymbols::fgINT_MAX_VALUE)
,fMinLength(0)
,fEnumerationInherited(false)
,fEnumeration(0)
{
    // init() is invoked from derived class's ctor instead of from
    // here to allow correct resolution of virutal method, such as
    // assigneAdditionalFacet(), inheritAdditionalFacet().
}

//
//  P1. Enumeration
//
//  P2. Facets
//   a. assign facets
//        assign common facets
//        assign additional facet
//
//   b. check facet among self
//   c. check vs base
//         check common facets
//         check enumeration
//         check Additional Facet Constraint
//
//  P3. Inherit facet from base
//   a. inherit common facets
//   b. inherit additional facet
//
void AbstractStringValidator::init(DatatypeValidator*            const baseValidator
                                 , RefHashTableOf<KVStringPair>* const facets
                                 , RefVectorOf<XMLCh>*           const enums)
{

    // P1. process enumeration
    if (enums)
        setEnumeration(enums, false);    

    // P2. process facets
    if (facets)
    {
        XMLCh* key;
        XMLCh* value;
        RefHashTableOfEnumerator<KVStringPair> e(facets);

        while (e.hasMoreElements())
        {
            KVStringPair pair = e.nextElement();
            key = pair.getKey();
            value = pair.getValue();

            if (XMLString::compareString(key, SchemaSymbols::fgELT_LENGTH)==0)
            {
                int val;
                try
                {
                    val = XMLString::parseInt(value);
                }
                catch (NumberFormatException nfe)
                {
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_Len, value);
                }

                if ( val < 0 )
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_NonNeg_Len, value);

                setLength(val);
                setFacetsDefined(DatatypeValidator::FACET_LENGTH);
            }
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_MINLENGTH)==0)
            {
                int val;
                try
                {
                    val = XMLString::parseInt(value);
                }
                catch (NumberFormatException nfe)
                {
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_minLen, value);
                }

                if ( val < 0 )
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_NonNeg_minLen, value);

                setMinLength(val);
                setFacetsDefined(DatatypeValidator::FACET_MINLENGTH);
            }
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_MAXLENGTH)==0)
            {
                int val;
                try
                {
                    val = XMLString::parseInt(value);
                }
                catch (NumberFormatException nfe)
                {
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_maxLen, value);
                }

                if ( val < 0 )
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_NonNeg_maxLen, value);

                setMaxLength(val);
                setFacetsDefined(DatatypeValidator::FACET_MAXLENGTH);
            }
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_PATTERN)==0)
            {
                setPattern(value);
                if (getPattern())
                    setFacetsDefined(DatatypeValidator::FACET_PATTERN);
                // do not construct regex until needed
            }
            else if (XMLString::compareString(key, SchemaSymbols::fgATT_FIXED)==0)
            {
                unsigned int val;
                bool         retStatus;
                try
                {
                     retStatus = XMLString::textToBin(value, val);
                }
                catch (RuntimeException)
                {
                    ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_internalError_fixed);
                }

                if (!retStatus)
                {
                    ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_internalError_fixed);
                }

                setFixed(val);
                //no setFacetsDefined here
            }
            //
            // else if (XMLString::compareString(key, SchemaSymbols::fgELT_SPECIAL_TOKEN)==0)
            // TODO
            //
            // Note: whitespace is taken care of by TraverseSchema.
            //
            else
            {
                assignAdditionalFacet(key, value);

            }
        }//while

        /***
           Schema constraint: Part I -- self checking
        ***/

        int thisFacetsDefined = getFacetsDefined();

        // check 4.3.1.c1 error: length & (maxLength | minLength)
        if ((thisFacetsDefined & DatatypeValidator::FACET_LENGTH) != 0)
        {
            if ((thisFacetsDefined & DatatypeValidator::FACET_MAXLENGTH) != 0)
                 ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
            else if (((thisFacetsDefined & DatatypeValidator::FACET_MINLENGTH) != 0))
                 ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_minLen);
        }

        // check 4.3.2.c1 must: minLength <= maxLength
        if ((thisFacetsDefined & (DatatypeValidator::FACET_MINLENGTH
                                  |DatatypeValidator::FACET_MAXLENGTH)) != 0)
        {
            if ( getMinLength() > getMaxLength() )
            {
                XMLString::binToText(getMaxLength(), value1, BUF_LEN, 10);
                XMLString::binToText(getMinLength(), value2, BUF_LEN, 10);

                ThrowXML2(InvalidDatatypeFacetException
                        , XMLExcepts::FACET_maxLen_minLen
                        , value1
                        , value2);
            }
        }

        /***
           Schema constraint: Part II base vs derived checking
        ***/
        if (baseValidator)
        {
            /***
                check facets against base.facets
                Note: later we need to check the "fix" option of the base type
                      and apply that to every individual facet.
            ***/
            AbstractStringValidator *pBaseValidator = (AbstractStringValidator*) baseValidator;
            int baseFacetsDefined = pBaseValidator->getFacetsDefined();
            /***
                Non coexistence of derived' length and base'    (minLength | maxLength)
                                   base'    length and derived' (minLength | maxLength)
            ***/

            // check 4.3.1.c1 error: length & (base.maxLength | base.minLength)
            if ((thisFacetsDefined & DatatypeValidator::FACET_LENGTH) !=0)
            {
                if ((baseFacetsDefined & DatatypeValidator::FACET_MAXLENGTH) !=0)
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                else if ((baseFacetsDefined & DatatypeValidator::FACET_MINLENGTH) !=0)
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_minLen);
            }

            // check 4.3.1.c1 error: base.length & (maxLength | minLength)
            if ((baseFacetsDefined & DatatypeValidator::FACET_LENGTH) !=0)
            {
                if ((thisFacetsDefined & DatatypeValidator::FACET_MAXLENGTH) !=0)
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                else if ((thisFacetsDefined & DatatypeValidator::FACET_MINLENGTH) !=0)
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_minLen);
            }

            // check 4.3.1.c2 error: length != base.length
            if (((thisFacetsDefined & DatatypeValidator::FACET_LENGTH) !=0) &&
                ((baseFacetsDefined & DatatypeValidator::FACET_LENGTH) !=0))
            {
                if ( getLength() != pBaseValidator->getLength() )
                {
                    XMLString::binToText(getLength(), value1, BUF_LEN, 10);
                    XMLString::binToText(pBaseValidator->getLength(), value2, BUF_LEN, 10);

                    ThrowXML2(InvalidDatatypeFacetException
                        , XMLExcepts::FACET_Len_baseLen
                        , value1
                        , value2);
                }                    
            }

            /***
                                   |---  derived   ---|
                base.minLength <= minLength <= maxLength <= base.maxLength
                |-------------------        base      -------------------|
            ***/

            // check 4.3.2.c1 must: minLength <= base.maxLength
            if (((thisFacetsDefined & DatatypeValidator::FACET_MINLENGTH ) != 0) &&
                ((baseFacetsDefined & DatatypeValidator::FACET_MAXLENGTH ) != 0))
            {
                if ( getMinLength() > pBaseValidator->getMaxLength() )
                {
                    XMLString::binToText(getMinLength(), value1, BUF_LEN, 10);
                    XMLString::binToText(pBaseValidator->getMaxLength(), value2, BUF_LEN, 10);

                    ThrowXML2(InvalidDatatypeFacetException
                        , XMLExcepts::FACET_minLen_baseminLen
                        , value1
                        , value2);
                }
            }

            // check 4.3.2.c2 error: minLength < base.minLength
            if (((thisFacetsDefined & DatatypeValidator::FACET_MINLENGTH) !=0) &&
                ((baseFacetsDefined & DatatypeValidator::FACET_MINLENGTH) != 0))
            {
                if ((pBaseValidator->getFixed() & DatatypeValidator::FACET_MINLENGTH) !=0)
                {
                    if ( getMinLength() != pBaseValidator->getMinLength() )
                    {
                        XMLString::binToText(getMinLength(), value1, BUF_LEN, 10);
                        XMLString::binToText(pBaseValidator->getMinLength(), value2, BUF_LEN, 10);

                        ThrowXML2(InvalidDatatypeFacetException
                        , XMLExcepts::FACET_minLen_base_fixed
                        , value1
                        , value2);
                    }
                }
                else
                {
                    if ( getMinLength() < pBaseValidator->getMinLength() )
                    {
                        XMLString::binToText(getMinLength(), value1, BUF_LEN, 10);
                        XMLString::binToText(pBaseValidator->getMinLength(), value2, BUF_LEN, 10);

                        ThrowXML2(InvalidDatatypeFacetException
                        , XMLExcepts::FACET_minLen_basemaxLen
                        , value1
                        , value2);
                    }
                }
            }

            // check 4.3.2.c1 must: base.minLength <= maxLength
            if (((baseFacetsDefined & DatatypeValidator::FACET_MINLENGTH) !=0) &&
                ((thisFacetsDefined & DatatypeValidator::FACET_MAXLENGTH) !=0))
            {
                if ( pBaseValidator->getMinLength() > getMaxLength() )
                {
                    XMLString::binToText(getMaxLength(), value1, BUF_LEN, 10);
                    XMLString::binToText(pBaseValidator->getMinLength(), value2, BUF_LEN, 10);

                    ThrowXML2(InvalidDatatypeFacetException
                        , XMLExcepts::FACET_maxLen_baseminLen
                        , value1
                        , value2);
                }
            }

            // check 4.3.3.c1 error: maxLength > base.maxLength
            if (((thisFacetsDefined & DatatypeValidator::FACET_MAXLENGTH) !=0) &&
                ((baseFacetsDefined & DatatypeValidator::FACET_MAXLENGTH) !=0))
            {
                if ((pBaseValidator->getFixed() & DatatypeValidator::FACET_MAXLENGTH) !=0)
                {
                    if ( getMaxLength() != pBaseValidator->getMaxLength() )
                    {
                        XMLString::binToText(getMaxLength(), value1, BUF_LEN, 10);
                        XMLString::binToText(pBaseValidator->getMaxLength(), value2, BUF_LEN, 10);

                        ThrowXML2(InvalidDatatypeFacetException
                        , XMLExcepts::FACET_maxLen_base_fixed
                        , value1
                        , value2);
                    }
                }
                else
                {
                    if ( getMaxLength() > pBaseValidator->getMaxLength() )
                    {
                        XMLString::binToText(getMaxLength(), value1, BUF_LEN, 10);
                        XMLString::binToText(pBaseValidator->getMaxLength(), value2, BUF_LEN, 10);

                        ThrowXML2(InvalidDatatypeFacetException
                        , XMLExcepts::FACET_maxLen_basemaxLen
                        , value1
                        , value2);
                    }
                }
            }

            // check 4.3.5.c0 must: enumeration values from the value space of base
            if ( ((thisFacetsDefined & DatatypeValidator::FACET_ENUMERATION) != 0) &&
                 (getEnumeration() !=0))
            {
                int i = 0;
                int enumLength = getEnumeration()->size();
                try
                {
                    for ( ; i < enumLength; i++)
                    {
                        // ask parent do a complete check
                        pBaseValidator->checkContent(getEnumeration()->elementAt(i), false);
                        // enum shall pass this->checkContent() as well.
                        checkContent(getEnumeration()->elementAt(i), false);
                    }
                }

                catch (...) //XMLException&
                {
                    ThrowXML1(InvalidDatatypeFacetException
                            , XMLExcepts::FACET_enum_base
                            , getEnumeration()->elementAt(i));
                }
            }

            checkAdditionalFacetConstraints();

        } //if baseValidator

    }// End of Facet setting

    /***
        P3. Inherit facets from base.facets

        The reason of this inheriting (or copying values) is to ease
        schema constraint checking, so that we need NOT trace back to our
        very first base validator in the hierachy. Instead, we are pretty
        sure checking against immediate base validator is enough.  
    ***/
    if (baseValidator)
    {
        AbstractStringValidator *pBaseValidator = (AbstractStringValidator*) baseValidator;
        int thisFacetsDefined = getFacetsDefined();
        int baseFacetsDefined = pBaseValidator->getFacetsDefined();

        // inherit length
        if (((baseFacetsDefined & DatatypeValidator::FACET_LENGTH) != 0) &&
            ((thisFacetsDefined & DatatypeValidator::FACET_LENGTH) == 0))
        {
            setLength(pBaseValidator->getLength());
            setFacetsDefined(DatatypeValidator::FACET_LENGTH);
        }

        // inherit minLength
        if (((baseFacetsDefined & DatatypeValidator::FACET_MINLENGTH) !=0) &&
            ((thisFacetsDefined & DatatypeValidator::FACET_MINLENGTH) == 0))
        {
            setMinLength(pBaseValidator->getMinLength());
            setFacetsDefined(DatatypeValidator::FACET_MINLENGTH);
        }

        // inherit maxLength
        if (((baseFacetsDefined & DatatypeValidator::FACET_MAXLENGTH) !=0) &&
            ((thisFacetsDefined & DatatypeValidator::FACET_MAXLENGTH) == 0))
        {
            setMaxLength(pBaseValidator->getMaxLength());
            setFacetsDefined(DatatypeValidator::FACET_MAXLENGTH);
        }

        // inherit enumeration
        if (((baseFacetsDefined & DatatypeValidator::FACET_ENUMERATION) !=0) &&
            ((thisFacetsDefined & DatatypeValidator::FACET_ENUMERATION) == 0))
        {
            setEnumeration(pBaseValidator->getEnumeration(), true);
        }

        // we don't inherit pattern

        // inherit "fixed" option
        setFixed(getFixed() | pBaseValidator->getFixed());

        // inherit additional facet
        inheritAdditionalFacet();
        
    } // end of inheritance
}

// -----------------------------------------------------------------------
// Compare methods
// -----------------------------------------------------------------------
int AbstractStringValidator::compare(const XMLCh* const lValue
                                   , const XMLCh* const rValue)
{
    return XMLString::compareString(lValue, rValue);
}

void AbstractStringValidator::validate( const XMLCh* const content)
{
    checkContent(content, false);
}

void AbstractStringValidator::checkContent( const XMLCh* const content, bool asBase)
{

    //validate against base validator if any
    AbstractStringValidator *pBaseValidator = (AbstractStringValidator*) this->getBaseValidator();
    if (pBaseValidator)
        pBaseValidator->checkContent(content, true);

    int thisFacetsDefined = getFacetsDefined();

    // we check pattern first
    if ( (thisFacetsDefined & DatatypeValidator::FACET_PATTERN ) != 0 )
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

    checkValueSpace(content);
    unsigned int length = getLength(content);

    if (((thisFacetsDefined & DatatypeValidator::FACET_MAXLENGTH) != 0) &&
        (length > getMaxLength()))
    {
        XMLString::binToText(length, value1, BUF_LEN, 10);
        XMLString::binToText(getMaxLength(), value2, BUF_LEN, 10);

        ThrowXML3(InvalidDatatypeValueException
                , XMLExcepts::VALUE_GT_maxLen
                , content
                , value1
                , value2);
    }

    if (((thisFacetsDefined & DatatypeValidator::FACET_MINLENGTH) != 0) &&
        (length < getMinLength()))
    {
        XMLString::binToText(length, value1, BUF_LEN, 10);
        XMLString::binToText(getMinLength(), value2, BUF_LEN, 10);

        ThrowXML3(InvalidDatatypeValueException
                , XMLExcepts::VALUE_LT_minLen
                , content
                , value1
                , value2);
    }

    if (((thisFacetsDefined & DatatypeValidator::FACET_LENGTH) != 0) &&
        (length != getLength()))
    {
        XMLString::binToText(length, value1, BUF_LEN, 10);
        XMLString::binToText(getLength(), value2, BUF_LEN, 10);

        ThrowXML3(InvalidDatatypeValueException
                , XMLExcepts::VALUE_NE_Len
                , content
                , value1
                , value2);
    }

    if ((thisFacetsDefined & DatatypeValidator::FACET_ENUMERATION) != 0 &&
        (getEnumeration() != 0))
    {
        int i=0;
        int enumLength = getEnumeration()->size();
        for ( ; i < enumLength; i++)
        {
            if (XMLString::compareString(content, getEnumeration()->elementAt(i))==0)
                break;
        }

        if (i == enumLength)
            ThrowXML1(InvalidDatatypeValueException, XMLExcepts::VALUE_NotIn_Enumeration, content);
    }

    checkAdditionalFacet(content);

}

/**
  * End of file AbstractStringValidator.cpp
  */
