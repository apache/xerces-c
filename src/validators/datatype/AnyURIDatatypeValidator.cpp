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
 * Revision 1.1  2001/08/01 18:49:16  peiyongz
 * AnyRUIDatatypeValidator
 *
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/datatype/AnyURIDatatypeValidator.hpp>
#include <validators/datatype/InvalidDatatypeFacetException.hpp>
#include <validators/datatype/InvalidDatatypeValueException.hpp>
#include <util/NumberFormatException.hpp>

static const int BUF_LEN = 64;
static XMLCh value1[BUF_LEN+1];
static XMLCh value2[BUF_LEN+1];

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
AnyURIDatatypeValidator::AnyURIDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefVectorOf<XMLCh>*           const enums
                        , const int                           finalSet)
:DatatypeValidator(baseValidator, facets, finalSet, DatatypeValidator::AnyURI)
,fLength(0)
,fMaxLength(SchemaSymbols::fgINT_MAX_VALUE)
,fMinLength(0)
,fEnumerationInherited(false)
,fEnumeration(0)
,fTempURI(0)
{
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

void AnyURIDatatypeValidator::init(DatatypeValidator*            const baseValidator
                                 , RefHashTableOf<KVStringPair>* const facets
                                 , RefVectorOf<XMLCh>*           const enums)
{
    //
    // Make sure that baseValidator is of the same type
    // as the one to be created.
    //
    // TO DO: moving this checking to DatatypeValidator
    //        to avoid repeat this code in all DTV.
    //        special case:: union and list
    //
    if ( baseValidator && 
         (baseValidator->getType() != DatatypeValidator::AnyURI))
    {
        //invalid baseValidator's type;
    }

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
        if (baseValidator != 0)
        {
            /***
                check facets against base.facets
                Note: later we need to check the "fix" option of the base type
                      and apply that to every individual facet.
            ***/
            AnyURIDatatypeValidator *pBaseValidator = (AnyURIDatatypeValidator*) baseValidator;

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
                        // REVISIT: enum shall pass this->checkContent() as well.
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

         } // if baseValidator

     }// End of Facet setting

     /***
        Inherit facets from base.facets

        The reason of this inheriting (or copying values) is to ease
        schema constraint checking, so that we need NOT trace back to our
        very first base validator in the hierachy. Instead, we are pretty
        sure checking against immediate base validator is enough.

     ***/
   
    if ( baseValidator )
    {
        AnyURIDatatypeValidator *pBaseValidator = (AnyURIDatatypeValidator*) baseValidator;

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

    }// End of inheritance

}

void AnyURIDatatypeValidator::checkContent( const XMLCh* const content, bool asBase)
{
    char* p=XMLString::transcode(content);

    //validate against base validator if any
    AnyURIDatatypeValidator *pBaseValidator = (AnyURIDatatypeValidator*) this->getBaseValidator();
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

    unsigned int strLen = XMLString::stringLen(content);

    if (((getFacetsDefined() & DatatypeValidator::FACET_MAXLENGTH) != 0) &&
        (strLen > getMaxLength()))
    {
        XMLString::binToText(strLen, value1, BUF_LEN, 10);
        XMLString::binToText(getMaxLength(), value2, BUF_LEN, 10);

        ThrowXML3(InvalidDatatypeValueException
                , XMLExcepts::VALUE_GT_maxLen
                , content
                , value1
                , value2);
    }

    if (((getFacetsDefined() & DatatypeValidator::FACET_MINLENGTH) != 0) &&
        (strLen < getMinLength()))
    {
        XMLString::binToText(strLen, value1, BUF_LEN, 10);
        XMLString::binToText(getMinLength(), value2, BUF_LEN, 10);

        ThrowXML3(InvalidDatatypeValueException
                , XMLExcepts::VALUE_LT_minLen
                , content
                , value1
                , value2);
    }

    if (((getFacetsDefined() & DatatypeValidator::FACET_LENGTH) != 0) &&
        (strLen != getLength()))
    {
        XMLString::binToText(strLen, value1, BUF_LEN, 10);
        XMLString::binToText(getLength(), value2, BUF_LEN, 10);

        ThrowXML3(InvalidDatatypeValueException
                , XMLExcepts::VALUE_NE_Len
                , content
                , value1
                , value2);
    }

    if ((getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) != 0 &&
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

    // check 3.2.17.c0 must: URI (rfc 2396/2723)
    XMLURL  *newURI;

#ifdef REVISIT
    try 
    {
        if (!fTempURI) 
            fTempURI = new XMLURL("http://www.template.com");

        // Support for relative URLs
        // According to Java 1.1: URLs may also be specified with a 
        // String and the URL object that it is related to.
        //
        if (strLen) 
        {
            newURI = new XMLURL(*fTempURI, content );   
            delete newURI;
        }
    } 
#endif
    try 
    {
        if (strLen) 
        {
            newURI = new XMLURL(content );   
            delete newURI;
        }
    } 
    catch (...) 
    {
        ThrowXML1(InvalidDatatypeValueException, XMLExcepts::VALUE_NotIn_Enumeration, content);
        //("Value '"+content+"' is a Malformed URI ");
    }
      
}

/**
  * End of file AnyURIDatatypeValidator.cpp
  */
