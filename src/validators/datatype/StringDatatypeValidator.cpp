/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.1  2001/05/09 18:43:42  tng
 * Add StringDatatypeValidator and BooleanDatatypeValidator.  By Pei Yong Zhang.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/datatype/StringDatatypeValidator.hpp>
#include <validators/datatype/InvalidDatatypeFacetException.hpp>
#include <validators/datatype/InvalidDatatypeValueException.hpp>
#include <util/NumberFormatException.hpp>

static const int BUF_LEN = 64;
static XMLCh value1[BUF_LEN+1];
static XMLCh value2[BUF_LEN+1];

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
StringDatatypeValidator::StringDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets     
                        , const int                           finalSet)
:DatatypeValidator(baseValidator, facets, finalSet, DatatypeValidator::String)
,fLength(0)
,fMaxLength(SchemaSymbols::fgINT_MAX_VALUE)
,fMinLength(0)
,fEnumeration(0)
,fWhiteSpace(DatatypeValidator::PRESERVE)
{
    try
    {
        init(baseValidator, facets);
    }
    catch (XMLException&)
    {
        cleanUp();
    }
}

void StringDatatypeValidator::init(DatatypeValidator*            const baseValidator
                                 , RefHashTableOf<KVStringPair>* const facets)
{
    // Set Facets if any defined
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
                try
                {
                    setLength(XMLString::parseInt(value));
                }
                catch (NumberFormatException nfe)
                {
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_Len, value);
                }

                if (getLength() < 0)
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_NonNeg_Len, value);

                setFacetsDefined(DatatypeValidator::FACET_LENGTH);
            } 
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_MINLENGTH)==0)
            {
                try
                {
                    setMinLength(XMLString::parseInt(value));
                }
                catch (NumberFormatException nfe)
                {
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_minLen, value);
                }

                if (getMinLength() < 0)
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_NonNeg_minLen, value);

                setFacetsDefined(DatatypeValidator::FACET_MINLENGTH);
            } 
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_MAXLENGTH)==0)
            {
                try
                {
                    setMaxLength(XMLString::parseInt(value));
                }
                catch (NumberFormatException nfe)
                {
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_maxLen, value);
                }

                if (getMaxLength() < 0)
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_NonNeg_maxLen, value);

                setFacetsDefined(DatatypeValidator::FACET_MAXLENGTH);            
            } 
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_PATTERN)==0)
            {
                setPattern(value);
                if (getPattern())
                    setFacetsDefined(DatatypeValidator::FACET_PATTERN);
                // do not construct regex until needed
            } 
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_ENUMERATION)==0)
            {
                setEnumeration(XMLString::tokenizeString(value));
                setFacetsDefined(DatatypeValidator::FACET_ENUMERATION);
            } 
            else if (XMLString::compareString(key, SchemaSymbols::fgELT_WHITESPACE)==0)
            {
                // whiteSpace = preserve | replace | collapse
                if (XMLString::compareString(value, SchemaSymbols::fgWS_PRESERVE) == 0) 
                    setWhiteSpace(DatatypeValidator::PRESERVE);
                else if (XMLString::compareString(value, SchemaSymbols::fgWS_REPLACE) == 0) 
                    setWhiteSpace(DatatypeValidator::REPLACE);
                else if (XMLString::compareString(value, SchemaSymbols::fgWS_COLLAPSE) == 0) 
                    setWhiteSpace(DatatypeValidator::COLLAPSE);
                else 
                    ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_WS, value);
                    //("whiteSpace value '" + ws + "' must be one of 'preserve', 'replace', 'collapse'.");

                setFacetsDefined(DatatypeValidator::FACET_WHITESPACE);
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
            StringDatatypeValidator *pBaseValidator = (StringDatatypeValidator*) baseValidator;
                  
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
                // if (pBaseVlidator->getFixed(DatatypeValidator::FACET_LENGTH) == true)
                //     if ( getLength() != pBaseValidator->getLength() )
                //         ThrowXML
                // else
                //     if ( getLength() > pBaseValidator->getLength() )
                //         ThrowXML
                if ( getLength() > pBaseValidator->getLength() )
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
                // if (pBaseVlidator->getFixed(DatatypeValidator::FACET_MINLENGTH) == true)
                //     if ( getMinLength() != pBaseValidator->getMinLength() )
                //         ThrowXML
                // else
                //     if ( getMinLength() < pBaseValidator->getMinLength() )
                //         ThrowXML
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
                // if (pBaseVlidator->getFixed(DatatypeValidator::FACET_MAXLENGTH) == true)
                //     if ( getMaxLength() != pBaseValidator->getMaxLength() )
                //         ThrowXML
                // else
                //     if ( getMaxLength() > pBaseValidator->getMaxLength() )
                //         ThrowXML
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

            // check 4.3.5.c0 must: enumeration values from the value space of base
            if ( ((getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) != 0) &&
                 (getEnumeration() !=0)) 
            {
                int i = 0;
                int enumLength = getEnumeration()->size();
                try 
                {
                    for ( ; i < enumLength; i++) 
                        // ask parent do a complete check
                        pBaseValidator->checkContent(getEnumeration()->elementAt(i), false);
                        // REVISIT: enum shall pass this->checkContent() as well.
                        //checkContent(getEnumeration()->elementAt(i), false);
                } 

                catch ( XMLException& )
                {
                    ThrowXML1(InvalidDatatypeFacetException
                            , XMLExcepts::FACET_enum_base
                            , getEnumeration()->elementAt(i));
                }
            }

            // check 4.3.6.c1 error: whitespace
            if (((getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) != 0) &&
                ((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) != 0 )) 
            {
                if ((pBaseValidator->getWSFacet() == DatatypeValidator::COLLAPSE) &&
                    ((getWSFacet() == DatatypeValidator::PRESERVE) ||
                     (getWSFacet() == DatatypeValidator::REPLACE)))
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_WS_collapse);

                if ((pBaseValidator->getWSFacet() == DatatypeValidator::PRESERVE) &&
                    (getWSFacet() == DatatypeValidator::REPLACE))
                     ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_WS_replace);
            }

            /***
                Inherit facets from base.facets
                
                The reason of this inheriting (or copying values) is to ease
                schema constraint checking, so that we need NOT trace back to our 
                very first base validator in the hierachy. Instead, we are pretty 
                sure checking against immediate base validator is enough.
            
            ***/

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
                // need to adopt the Vector   
                RefVectorOf<XMLCh>*  fBaseEnumeration = pBaseValidator->getEnumeration();
                int enumLength = fBaseEnumeration->size();
                
                RefVectorOf<XMLCh>* tmpEnumeration = new RefVectorOf<XMLCh>(enumLength, true);
                for ( int i = 0; i < enumLength; i++) 
                {
                    int strLen = XMLString::stringLen(fBaseEnumeration->elementAt(i));
                    XMLCh* tmp = new XMLCh[strLen+1];
                    XMLString::moveChars(tmp, fBaseEnumeration->elementAt(i), strLen);
                    tmpEnumeration->insertElementAt(tmp, i);
                }

                setEnumeration(tmpEnumeration);
                setFacetsDefined(DatatypeValidator::FACET_ENUMERATION);
            }

            // we don't inherit pattern

            // inherit whitespace
            if (((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) !=0) &&
                ((getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) == 0))
            {
                setWhiteSpace(pBaseValidator->getWSFacet());
                setFacetsDefined(DatatypeValidator::FACET_WHITESPACE);
            }

        } //if baseValidator

    }// End of Facet setting

}

void StringDatatypeValidator::checkContent( const XMLCh* const content, bool asBase)
{

    //validate against base validator if any
    StringDatatypeValidator *pBaseValidator = (StringDatatypeValidator*) this->getBaseValidator();
    if (pBaseValidator !=0) 
        pBaseValidator->checkContent(content, true);

    // we check pattern first
    if ( (getFacetsDefined() & DatatypeValidator::FACET_PATTERN ) != 0 ) 
    {
        // lazy construction
        if (getRegex() ==0)
            setRegex(new RegularExpression(getPattern(), SchemaSymbols::fgRegEx_XOption));

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

}

/**
  * End of file StringDatatypeValidator.cpp
  */
