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
 * Revision 1.5  2002/10/02 13:29:12  tng
 * Since the compare function return int, so use XMLString::compareString instead of XMLString::equals there.
 *
 * Revision 1.4  2002/09/24 19:44:40  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.3  2002/04/01 20:17:46  peiyongz
 * Bug#7551: Exceptions are caught by value, rather than by reference
 *
 * Revision 1.2  2002/02/14 15:17:31  peiyongz
 * getEnumString()
 *
 * Revision 1.1.1.1  2002/02/01 22:22:39  peiyongz
 * sane_include
 *
 * Revision 1.7  2001/12/13 16:48:29  peiyongz
 * Avoid dangling pointer
 *
 * Revision 1.6  2001/10/15 20:57:27  tng
 * Schema: we should propagate the exception thrown from checkContent.
 *
 * Revision 1.5  2001/10/09 21:00:54  peiyongz
 * . init() take 1 arg,
 * . make inspectFacetBase() virtual to allow ListDTV provide its own method,
 * . reorganize init() into assignFacet(), inspectFacet(), inspectFacetBase() and
 * inheritFacet() to improve mantainability.
 * . macro to simplify code
 * . save get***() to temp vars
 *
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
#include <xercesc/validators/datatype/AbstractStringValidator.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>
#include <xercesc/util/NumberFormatException.hpp>

static const int BUF_LEN = 64;
static XMLCh value1[BUF_LEN+1];
static XMLCh value2[BUF_LEN+1];

#define  REPORT_FACET_ERROR(val1, val2, except_code)    \
   XMLString::binToText(val1, value1, BUF_LEN, 10);     \
   XMLString::binToText(val2, value2, BUF_LEN, 10);     \
   ThrowXML2(InvalidDatatypeFacetException              \
           , except_code                                \
           , value1                                     \
           , value2);

#define  REPORT_VALUE_ERROR(data, val1, val2, except_code)      \
   XMLString::binToText(val1, value1, BUF_LEN, 10);             \
   XMLString::binToText(val2, value2, BUF_LEN, 10);             \
   ThrowXML3(InvalidDatatypeValueException                      \
           , except_code                                        \
           , data                                               \
           , value1                                             \
           , value2);

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
AbstractStringValidator::~AbstractStringValidator()
{
    //~RefVectorOf will delete all adopted elements
    if ( !fEnumerationInherited && fEnumeration)
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

void AbstractStringValidator::init(RefVectorOf<XMLCh>*           const enums)
{

    if (enums)
        setEnumeration(enums, false);

    assignFacet();
    inspectFacet();
    inspectFacetBase();
    inheritFacet();

}

//
//   Assign facets
//        assign common facets
//        assign additional facet
//
void AbstractStringValidator::assignFacet()
{

    RefHashTableOf<KVStringPair>* facets = getFacets();

    if (!facets)
        return;

    XMLCh* key;
    XMLCh* value;
    RefHashTableOfEnumerator<KVStringPair> e(facets);

    while (e.hasMoreElements())
    {
        KVStringPair pair = e.nextElement();
        key = pair.getKey();
        value = pair.getValue();

        if (XMLString::equals(key, SchemaSymbols::fgELT_LENGTH))
        {
            int val;
            try
            {
                val = XMLString::parseInt(value);
            }
            catch (NumberFormatException&)
            {
                ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_Len, value);
            }

            if ( val < 0 )
                ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_NonNeg_Len, value);

                setLength(val);
                setFacetsDefined(DatatypeValidator::FACET_LENGTH);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MINLENGTH))
        {
            int val;
            try
            {
                val = XMLString::parseInt(value);
            }
            catch (NumberFormatException&)
            {
                ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_minLen, value);
            }

            if ( val < 0 )
                ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_NonNeg_minLen, value);

            setMinLength(val);
            setFacetsDefined(DatatypeValidator::FACET_MINLENGTH);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_MAXLENGTH))
        {
            int val;
            try
            {
                val = XMLString::parseInt(value);
            }
            catch (NumberFormatException&)
            {
                ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_maxLen, value);
            }

            if ( val < 0 )
                ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_NonNeg_maxLen, value);

            setMaxLength(val);
            setFacetsDefined(DatatypeValidator::FACET_MAXLENGTH);
        }
        else if (XMLString::equals(key, SchemaSymbols::fgELT_PATTERN))
        {
            setPattern(value);
            if (getPattern())
                setFacetsDefined(DatatypeValidator::FACET_PATTERN);
            // do not construct regex until needed
        }
        else if (XMLString::equals(key, SchemaSymbols::fgATT_FIXED))
        {
            unsigned int val;
            bool         retStatus;
            try
            {
                retStatus = XMLString::textToBin(value, val);
            }
            catch (RuntimeException&)
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
        // else if (XMLString::equals(key, SchemaSymbols::fgELT_SPECIAL_TOKEN))
        // TODO
        //
        // Note: whitespace is taken care of by TraverseSchema.
        //
        else
        {
            assignAdditionalFacet(key, value);
        }
    }//while
}//end of assigneFacet()

//
// Check facet among self
//         check common facets
//         check Additional Facet Constraint
//
void AbstractStringValidator::inspectFacet()
{

    int thisFacetsDefined = getFacetsDefined();

    if (!thisFacetsDefined)
        return;

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
        int thisMinLength = getMinLength();
        int thisMaxLength = getMaxLength();
        if ( thisMinLength > thisMaxLength )
        {
            REPORT_FACET_ERROR(thisMaxLength
                             , thisMinLength
                             , XMLExcepts::FACET_maxLen_minLen)
        }
    }

}// end of inspectFacet()

//
//  Check vs base
//         check common facets
//         check enumeration
//         check Additional Facet Constraint
//
void AbstractStringValidator::inspectFacetBase()
{

    AbstractStringValidator *pBaseValidator = (AbstractStringValidator*) getBaseValidator();
    int thisFacetsDefined = getFacetsDefined();

    if ( (!thisFacetsDefined && !fEnumeration) ||
         (!pBaseValidator)                      )
        return;

    int baseFacetsDefined = pBaseValidator->getFacetsDefined();

    int thisLength    = getLength();
    int thisMinLength = getMinLength();
    int thisMaxLength = getMaxLength();

    int baseLength    = pBaseValidator->getLength();
    int baseMinLength = pBaseValidator->getMinLength();
    int baseMaxLength = pBaseValidator->getMaxLength();
    int baseFixed     = pBaseValidator->getFixed();

    /***
       check facets against base.facets
       Note: later we need to check the "fix" option of the base type
            and apply that to every individual facet.
    ***/

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
        if ( thisLength != baseLength )
        {
            REPORT_FACET_ERROR(thisLength
                             , baseLength
                             , XMLExcepts::FACET_Len_baseLen)
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
        if ( thisMinLength > baseMaxLength )
        {
            REPORT_FACET_ERROR(thisMinLength
                             , baseMaxLength
                             , XMLExcepts::FACET_minLen_basemaxLen)
        }
    }

    // check 4.3.2.c2 error: minLength < base.minLength
    if (((thisFacetsDefined & DatatypeValidator::FACET_MINLENGTH) !=0) &&
        ((baseFacetsDefined & DatatypeValidator::FACET_MINLENGTH) != 0))
    {
        if ((baseFixed & DatatypeValidator::FACET_MINLENGTH) !=0)
        {
            if ( thisMinLength != baseMinLength )
            {
                REPORT_FACET_ERROR(thisMinLength
                                 , baseMinLength
                                 , XMLExcepts::FACET_minLen_base_fixed)
            }

        }
        else
        {
            if ( thisMinLength < baseMinLength )
            {
                REPORT_FACET_ERROR(thisMinLength
                                 , baseMinLength
                                 , XMLExcepts::FACET_minLen_baseminLen)
            }
        }
    }

    // check 4.3.2.c1 must: base.minLength <= maxLength
    if (((baseFacetsDefined & DatatypeValidator::FACET_MINLENGTH) !=0) &&
        ((thisFacetsDefined & DatatypeValidator::FACET_MAXLENGTH) !=0))
    {
        if ( baseMinLength > thisMaxLength )
        {
            REPORT_FACET_ERROR(thisMaxLength
                             , baseMinLength
                             , XMLExcepts::FACET_maxLen_baseminLen)
        }
    }

    // check 4.3.3.c1 error: maxLength > base.maxLength
    if (((thisFacetsDefined & DatatypeValidator::FACET_MAXLENGTH) !=0) &&
        ((baseFacetsDefined & DatatypeValidator::FACET_MAXLENGTH) !=0))
    {
        if ((baseFixed & DatatypeValidator::FACET_MAXLENGTH) !=0)
        {
            if ( thisMaxLength != baseMaxLength )
            {
                REPORT_FACET_ERROR(thisMaxLength
                                 , baseMaxLength
                                 , XMLExcepts::FACET_maxLen_base_fixed)
            }
        }
        else
        {
            if ( thisMaxLength > baseMaxLength )
            {
                REPORT_FACET_ERROR(thisMaxLength
                                 , baseMaxLength
                                 , XMLExcepts::FACET_maxLen_basemaxLen)
            }
        }
    }

    // check 4.3.5.c0 must: enumeration values from the value space of base
    if ( ((thisFacetsDefined & DatatypeValidator::FACET_ENUMERATION) != 0) &&
        (getEnumeration() !=0))
    {
        int i = 0;
        int enumLength = getEnumeration()->size();
        for ( ; i < enumLength; i++)
        {
            // ask parent do a complete check
            pBaseValidator->checkContent(getEnumeration()->elementAt(i), false);
            // enum shall pass this->checkContent() as well.
            checkContent(getEnumeration()->elementAt(i), false);
        }
    }

    checkAdditionalFacetConstraints();

} //end of inspectFacetBase

//
//  Inherit facet from base
//    a. inherit common facets
//    b. inherit additional facet
//
void AbstractStringValidator::inheritFacet()
{
    /***
        P3. Inherit facets from base.facets

        The reason of this inheriting (or copying values) is to ease
        schema constraint checking, so that we need NOT trace back to our
        very first base validator in the hierachy. Instead, we are pretty
        sure checking against immediate base validator is enough.
    ***/

    AbstractStringValidator *pBaseValidator = (AbstractStringValidator*) getBaseValidator();

    if (!pBaseValidator)
        return;

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
        REPORT_VALUE_ERROR(content
                         , length
                         , getMaxLength()
                         , XMLExcepts::VALUE_GT_maxLen)
    }

    if (((thisFacetsDefined & DatatypeValidator::FACET_MINLENGTH) != 0) &&
        (length < getMinLength()))
    {
        REPORT_VALUE_ERROR(content
                         , length
                         , getMinLength()
                         , XMLExcepts::VALUE_LT_minLen)
    }

    if (((thisFacetsDefined & DatatypeValidator::FACET_LENGTH) != 0) &&
        (length != getLength()))
    {
        REPORT_VALUE_ERROR(content
                         , length
                         , getLength()
                         , XMLExcepts::VALUE_NE_Len)
    }

    if ((thisFacetsDefined & DatatypeValidator::FACET_ENUMERATION) != 0 &&
        (getEnumeration() != 0))
    {
        int i=0;
        int enumLength = getEnumeration()->size();
        for ( ; i < enumLength; i++)
        {
            if (XMLString::equals(content, getEnumeration()->elementAt(i)))
                break;
        }

        if (i == enumLength)
            ThrowXML1(InvalidDatatypeValueException, XMLExcepts::VALUE_NotIn_Enumeration, content);
    }

    checkAdditionalFacet(content);

}

const RefVectorOf<XMLCh>* AbstractStringValidator::getEnumString() const
{
	return getEnumeration();
}

/**
  * End of file AbstractStringValidator.cpp
  */
