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
 * Revision 1.5  2001/09/20 13:11:42  knoaman
 * Regx  + misc. fixes
 *
 * Revision 1.4  2001/08/29 19:03:40  peiyongz
 * Bugzilla# 2816:on AIX 4.2, xlC 3 r ev.1, Compilation error on inline method
 *
 * Revision 1.3  2001/08/21 18:42:53  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.2  2001/08/14 22:11:56  peiyongz
 * new exception message added
 *
 * Revision 1.1  2001/07/11 21:37:17  peiyongz
 * ListDatatypeDTV
 *
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/datatype/ListDatatypeValidator.hpp>
#include <validators/datatype/InvalidDatatypeFacetException.hpp>
#include <validators/datatype/InvalidDatatypeValueException.hpp>
#include <util/NumberFormatException.hpp>

static const int BUF_LEN = 64;
static XMLCh value1[BUF_LEN+1];
static XMLCh value2[BUF_LEN+1];

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
ListDatatypeValidator::ListDatatypeValidator()
:DatatypeValidator(0, 0, 0, DatatypeValidator::List)
,fLength(0)
,fMaxLength(SchemaSymbols::fgINT_MAX_VALUE)
,fMinLength(0)
,fEnumerationInherited(false)
,fEnumeration(0)
,fContent(0)
{}

ListDatatypeValidator::~ListDatatypeValidator()
{
    cleanUp();
}

ListDatatypeValidator::ListDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefVectorOf<XMLCh>*           const enums
                        , const int                           finalSet)
:DatatypeValidator(baseValidator, facets, finalSet, DatatypeValidator::List)
,fLength(0)
,fMaxLength(SchemaSymbols::fgINT_MAX_VALUE)
,fMinLength(0)
,fEnumerationInherited(false)
,fEnumeration(0)
{
    //
    // baseValidator shall either 
    // an atomic DTV which servers as itemType, or
    // another ListDTV from which, this ListDTV is derived by restriction.
    // 
    // In either case, it shall be not null
    //
    if (!baseValidator)
        ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_List_Null_baseValidator);

    try
    {
        init(baseValidator, facets, enums);
    }
    catch (...)
    {
        cleanUp();
        throw;
    }
}

void ListDatatypeValidator::init(DatatypeValidator*            const baseValidator
                               , RefHashTableOf<KVStringPair>* const facets
                               , RefVectorOf<XMLCh>*           const enums)
{
    // Set Facets if any defined
    if (facets)
    {
        if (enums)
            setEnumeration(enums, false);

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
            else
            {
                 ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_Tag);
            }
        }//while

        /***
           Schema constraint: Part I -- self checking
        ***/

        // check 4.3.1.c1 error: length & (maxLength | minLength)
        if ((getFacetsDefined() & DatatypeValidator::FACET_LENGTH) != 0)
        {
            if ((getFacetsDefined() & DatatypeValidator::FACET_MAXLENGTH) != 0)
                 ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
            else if (((getFacetsDefined() & DatatypeValidator::FACET_MINLENGTH) != 0))
                 ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_minLen);
        }

        // check 4.3.2.c1 must: minLength <= maxLength
        if ((getFacetsDefined() & (DatatypeValidator::FACET_MINLENGTH
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
        if (baseValidator->getType() == DatatypeValidator::List)
        {
            /***
                check facets against base.facets
            ***/
            ListDatatypeValidator *pBaseValidator = (ListDatatypeValidator*) baseValidator;

            /***
                Non coexistence of derived' length and base'    (minLength | maxLength)
                                   base'    length and derived' (minLength | maxLength)
            ***/

            // check 4.3.1.c1 error: length & (base.maxLength | base.minLength)
            if ((getFacetsDefined() & DatatypeValidator::FACET_LENGTH) !=0)
            {
                if ((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_MAXLENGTH) !=0)
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                else if ((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_MINLENGTH) !=0)
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_minLen);
            }

            // check 4.3.1.c1 error: base.length & (maxLength | minLength)
            if ((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_LENGTH) !=0)
            {
                if ((getFacetsDefined() & DatatypeValidator::FACET_MAXLENGTH) !=0)
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_maxLen);
                else if ((getFacetsDefined() & DatatypeValidator::FACET_MINLENGTH) !=0)
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_Len_minLen);
            }

            // check 4.3.1.c2 error: length != base.length
            if (((getFacetsDefined() & DatatypeValidator::FACET_LENGTH) !=0) &&
                ((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_LENGTH) !=0))
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
            if (((getFacetsDefined() & DatatypeValidator::FACET_MINLENGTH ) != 0) &&
                ((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_MAXLENGTH ) != 0))
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
            if (((getFacetsDefined() & DatatypeValidator::FACET_MINLENGTH) !=0) &&
                ((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_MINLENGTH) != 0))
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
            if (((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_MINLENGTH) !=0) &&
                ((getFacetsDefined() & DatatypeValidator::FACET_MAXLENGTH) !=0))
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
            if (((getFacetsDefined() & DatatypeValidator::FACET_MAXLENGTH) !=0) &&
                ((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_MAXLENGTH) !=0))
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
            if ( ((getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) != 0) &&
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

                catch ( XMLException& )
                {
                    ThrowXML1(InvalidDatatypeFacetException
                            , XMLExcepts::FACET_enum_base
                            , getEnumeration()->elementAt(i));
                }
            }

        } //ListDTV
        else
        {
            // the first level ListDTV
            // check 4.3.5.c0 must: enumeration values from the value space of base
            if ( ((getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) != 0) &&
                 (getEnumeration() !=0))
            {
                int i;
                int enumLength = getEnumeration()->size();
                try
                {
                    for ( i = 0; i < enumLength; i++)
                    {
                        // ask the itemType for a complete check
                        RefVectorOf<XMLCh>* tempList = XMLString::tokenizeString(getEnumeration()->elementAt(i));
                        int tokenNumber = tempList->size();

                        try 
                        {
                            for ( int j = 0; j < tokenNumber; j++)
                                baseValidator->validate(tempList->elementAt(j));
                        }
                        catch (...)
                        {
                            delete tempList;
                            throw;
                        }

                        delete tempList;

                        // enum shall pass this->checkContent() as well.
                        checkContent(getEnumeration()->elementAt(i), false);
                    }
                }

                catch ( XMLException& )
                {
                    ThrowXML1(InvalidDatatypeFacetException
                            , XMLExcepts::FACET_enum_base
                            , getEnumeration()->elementAt(i));
                }
            }
        }

    }// End of Facet setting

    /***
        Inherit facets from base.facets

        The reason of this inheriting (or copying values) is to ease
        schema constraint checking, so that we need NOT trace back to our
        very first base validator in the hierachy. Instead, we are pretty
        sure checking against immediate base validator is enough.  
    ***/
    if (baseValidator->getType() == DatatypeValidator::List)
    {
        ListDatatypeValidator *pBaseValidator = (ListDatatypeValidator*) baseValidator;

        // inherit length
        if (((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_LENGTH) != 0) &&
            ((getFacetsDefined() & DatatypeValidator::FACET_LENGTH) == 0))
        {
            setLength(pBaseValidator->getLength());
            setFacetsDefined(DatatypeValidator::FACET_LENGTH);
        }

        // inherit minLength
        if (((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_MINLENGTH) !=0) &&
            ((getFacetsDefined() & DatatypeValidator::FACET_MINLENGTH) == 0))
        {
            setMinLength(pBaseValidator->getMinLength());
            setFacetsDefined(DatatypeValidator::FACET_MINLENGTH);
        }

        // inherit maxLength
        if (((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_MAXLENGTH) !=0) &&
            ((getFacetsDefined() & DatatypeValidator::FACET_MAXLENGTH) == 0))
        {
            setMaxLength(pBaseValidator->getMaxLength());
            setFacetsDefined(DatatypeValidator::FACET_MAXLENGTH);
        }

        // inherit enumeration
        if (((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) !=0) &&
            ((getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) == 0))
        {
            setEnumeration(pBaseValidator->getEnumeration(), true);
        }

        // we don't inherit pattern

        // inherit "fixed" option
        setFixed(getFixed() | pBaseValidator->getFixed());

    } // end of inheritance
}

//
// here content is a list of items
//
void ListDatatypeValidator::checkContent( RefVectorOf<XMLCh>* tokenVector, bool asBase)
{
    DatatypeValidator* bv = getBaseValidator();
    if (bv->getType() == DatatypeValidator::List)
        ((ListDatatypeValidator*)bv)->checkContent(tokenVector, true);
    else
    {   // the ultimate itemType DTV
        for (unsigned int i = 0; i < tokenVector->size(); i++)
            bv->validate(tokenVector->elementAt(i));
    }

    // we check pattern first
    if ( (getFacetsDefined() & DatatypeValidator::FACET_PATTERN ) != 0 )
    {
        // lazy construction
        if (getRegex() == 0) 
        {
            try {
                setRegex(new RegularExpression(getPattern(), SchemaSymbols::fgRegEx_XOption));
            }
            catch (XMLException &e)
            {
                ThrowXML1(InvalidDatatypeValueException, XMLExcepts::RethrowError, e.getMessage());
            }
        }

        //check each and every item in the list   
        for (unsigned int i = 0; i < tokenVector->size(); i++)
        {
            if (getRegex()->matches(tokenVector->elementAt(i)) ==false)
            {
                ThrowXML2(InvalidDatatypeValueException
                    , XMLExcepts::VALUE_NotMatch_Pattern
                    , tokenVector->elementAt(i)
                    , getPattern());
            }

        }
    }

    // if this is a base validator, we only need to check pattern facet
    // all other facet were inherited by the derived type
    if (asBase)
        return;

    unsigned int tokenNumber = tokenVector->size();

    if (((getFacetsDefined() & DatatypeValidator::FACET_MAXLENGTH) != 0) &&
        (tokenNumber > getMaxLength()))
    {
        XMLString::binToText(tokenNumber, value1, BUF_LEN, 10);
        XMLString::binToText(getMaxLength(), value2, BUF_LEN, 10);

        ThrowXML3(InvalidDatatypeValueException
                , XMLExcepts::VALUE_GT_maxLen
                , getContent()
                , value1
                , value2);
    }

    if (((getFacetsDefined() & DatatypeValidator::FACET_MINLENGTH) != 0) &&
        (tokenNumber < getMinLength()))
    {
        XMLString::binToText(tokenNumber, value1, BUF_LEN, 10);
        XMLString::binToText(getMinLength(), value2, BUF_LEN, 10);

        ThrowXML3(InvalidDatatypeValueException
                , XMLExcepts::VALUE_LT_minLen
                , getContent()
                , value1
                , value2);
    }

    if (((getFacetsDefined() & DatatypeValidator::FACET_LENGTH) != 0) &&
        (tokenNumber != getLength()))
    {
        XMLString::binToText(tokenNumber, value1, BUF_LEN, 10);
        XMLString::binToText(getLength(), value2, BUF_LEN, 10);

        ThrowXML3(InvalidDatatypeValueException
                , XMLExcepts::VALUE_NE_Len
                , getContent()
                , value1
                , value2);
    }

    if ((getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) != 0 &&
        (getEnumeration() != 0))
    {
        int i;
        int enumLength = getEnumeration()->size();

        for ( i = 0; i < enumLength; i++)
        {
            //optimization: we do a lexical comparision first
            // this may be faster for string and its derived
            if (XMLString::compareString(getEnumeration()->elementAt(i), getContent()) == 0)
                break; // a match found

            // do a value space check
            // this is needed for decimal (and probably other types 
            // such as datetime related)
            // eg.
            // tokenVector = "1 2 3.0 4" vs enumeration = "1 2 3 4.0"
            //
            if (valueSpaceCheck(tokenVector, getEnumeration()->elementAt(i)))
                break;
        }

        if (i == enumLength)
            ThrowXML1(InvalidDatatypeValueException, XMLExcepts::VALUE_NotIn_Enumeration, getContent());

    } // enumeration

}

bool ListDatatypeValidator::valueSpaceCheck(RefVectorOf<XMLCh>* tokenVector
                                          , const XMLCh* const  enumStr) const
{
    DatatypeValidator* theItemTypeDTV = getItemTypeDTV();
    RefVectorOf<XMLCh>* enumVector = XMLString::tokenizeString(enumStr);
    Janitor<RefVectorOf<XMLCh> > janName(enumVector);

    if (tokenVector->size() != enumVector->size())
        return false;

    for ( unsigned int j = 0; j < tokenVector->size(); j++ )
    {
        if (theItemTypeDTV->compare(tokenVector->elementAt(j), enumVector->elementAt(j)) != 0)
            return false;
    }

    return true;
}

int ListDatatypeValidator::compare(const XMLCh* const lValue
                                 , const XMLCh* const rValue)
{
    DatatypeValidator* theItemTypeDTV = getItemTypeDTV();
    RefVectorOf<XMLCh>* lVector = XMLString::tokenizeString(lValue);
    Janitor<RefVectorOf<XMLCh> > janl(lVector);
    RefVectorOf<XMLCh>* rVector = XMLString::tokenizeString(rValue);
    Janitor<RefVectorOf<XMLCh> > janr(rVector);
   
    int lNumberOfTokens = lVector->size();         
    int rNumberOfTokens = rVector->size();         

    if (lNumberOfTokens < rNumberOfTokens)
        return -1; 
    else if (lNumberOfTokens > rNumberOfTokens)
        return 1;
    else 
    { //compare each token   
        for ( int i = 0; i < lNumberOfTokens; i++)
        {
            int returnValue = theItemTypeDTV->compare(lVector->elementAt(i), rVector->elementAt(i));
            if (returnValue != 0) 
                return returnValue; //REVISIT: does it make sense to return -1 or +1..?               
        }
        return 0;
    }
       
}

void ListDatatypeValidator::validate( const XMLCh* const content)
{
    setContent(content);
    RefVectorOf<XMLCh>* tokenVector = XMLString::tokenizeString(content);
    Janitor<RefVectorOf<XMLCh> > janName(tokenVector);
    checkContent(tokenVector, false);
}

void ListDatatypeValidator::checkContent( const XMLCh* const content, bool asBase)
{
    setContent(content);
    RefVectorOf<XMLCh>* tokenVector = XMLString::tokenizeString(content);
    Janitor<RefVectorOf<XMLCh> > janName(tokenVector);
    checkContent(tokenVector, asBase);
}

/**
  * End of file ListDatatypeValidator.cpp
  */
