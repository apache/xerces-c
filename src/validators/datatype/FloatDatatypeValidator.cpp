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
 * Revision 1.6  2001/10/02 18:59:29  peiyongz
 * Invalid_Facet_Tag to display the tag name
 *
 * Revision 1.5  2001/10/01 21:03:55  peiyongz
 * DTV Reorganization:derived from AbstractNumericValidator
 *
 * Revision 1.4  2001/09/20 13:11:42  knoaman
 * Regx  + misc. fixes
 *
 * Revision 1.3  2001/08/21 18:42:53  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.2  2001/08/15 18:08:44  peiyongz
 * Fix to potential leakage in strEnumeration
 *
 * Revision 1.1  2001/07/26 20:42:16  peiyongz
 * FloatDatatypeValidator
 *
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/datatype/FloatDatatypeValidator.hpp>
#include <validators/schema/SchemaSymbols.hpp>
#include <validators/datatype/InvalidDatatypeFacetException.hpp>
#include <validators/datatype/InvalidDatatypeValueException.hpp>

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
FloatDatatypeValidator::FloatDatatypeValidator()
:AbstractNumericValidator(0, 0, 0, DatatypeValidator::Float)
{}

FloatDatatypeValidator::FloatDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefVectorOf<XMLCh>*           const enums
                        , const int                           finalSet)
:AbstractNumericValidator(baseValidator, facets, finalSet, DatatypeValidator::Float)
{
    init(enums);
}

FloatDatatypeValidator::~FloatDatatypeValidator()
{}

// -----------------------------------------------------------------------
// Compare methods
// -----------------------------------------------------------------------
int FloatDatatypeValidator::compare(const XMLCh* const lValue
                                  , const XMLCh* const rValue)
{
    XMLFloat * lObj = new XMLFloat(lValue);
    Janitor<XMLFloat> jname1(lObj);
    XMLFloat * rObj = new XMLFloat(rValue);
    Janitor<XMLFloat> jname2(rObj);

    return compareValues(lObj, rObj);
}

DatatypeValidator* FloatDatatypeValidator::newInstance(
                                      RefHashTableOf<KVStringPair>* const facets
                                    , RefVectorOf<XMLCh>*           const enums
                                    , const int                           finalSet)
{
    return (DatatypeValidator*) new FloatDatatypeValidator(this, facets, enums, finalSet);
}

// -----------------------------------------------------------------------
// ctor provided to be used by derived classes
// -----------------------------------------------------------------------
FloatDatatypeValidator::FloatDatatypeValidator(DatatypeValidator*            const baseValidator
                                             , RefHashTableOf<KVStringPair>* const facets
                                             , const int                           finalSet
                                             , const ValidatorType                 type)
:AbstractNumericValidator(baseValidator, facets, finalSet, type)
{
    //do not invoke init here !!!
}

void FloatDatatypeValidator::assignAdditionalFacet(const XMLCh* const key
                                                 , const XMLCh* const)
{
    ThrowXML1(InvalidDatatypeFacetException
            , XMLExcepts::FACET_Invalid_Tag
            , key);
}

void FloatDatatypeValidator::inheritAdditionalFacet()
{}

void FloatDatatypeValidator::checkAdditionalFacetConstraints() const
{}

void FloatDatatypeValidator::checkAdditionalFacetConstraintsBase() const
{}

int  FloatDatatypeValidator::compareValues(const XMLNumber* const lValue
                                         , const XMLNumber* const rValue)
{
    return XMLFloat::compareValues((XMLFloat*) lValue, (XMLFloat*) rValue);
}

void  FloatDatatypeValidator::setMaxInclusive(const XMLCh* const value)
{
    fMaxInclusive = new XMLFloat(value);
}

void  FloatDatatypeValidator::setMaxExclusive(const XMLCh* const value)
{
    fMaxExclusive = new XMLFloat(value);
}

void  FloatDatatypeValidator::setMinInclusive(const XMLCh* const value)
{
    fMinInclusive = new XMLFloat(value);
}

void  FloatDatatypeValidator::setMinExclusive(const XMLCh* const value)
{
    fMinExclusive = new XMLFloat(value);
}

void  FloatDatatypeValidator::setEnumeration()
{
    // check 4.3.5.c0 must: enumeration values from the value space of base
    //
    // 1. shall be from base value space
    // 2. shall be from current value space as well ( shall go through boundsCheck() )
    //
    if (!fStrEnumeration)
        return;

    int i = 0;
    int enumLength = fStrEnumeration->size();

    FloatDatatypeValidator *numBase = (FloatDatatypeValidator*) getBaseValidator();
    if (numBase)
    {
        try
        {
            for ( i = 0; i < enumLength; i++)
            {
                numBase->checkContent(fStrEnumeration->elementAt(i), false);
            }
        }              
        catch (XMLException&)
        {
            ThrowXML1(InvalidDatatypeFacetException
                , XMLExcepts::FACET_enum_base
                , fStrEnumeration->elementAt(i));

        }
    }

    // We put the this->checkContent in a separate loop
    // to not block original message with in that method.
    // 
    for ( i = 0; i < enumLength; i++)
    {
        checkContent(fStrEnumeration->elementAt(i), false);
    }              

    fEnumeration = new RefVectorOf<XMLNumber>(enumLength, true);
    fEnumerationInherited = false;

    for ( i = 0; i < enumLength; i++)
    {
        fEnumeration->insertElementAt(new XMLFloat(fStrEnumeration->elementAt(i)), i);
    }              
}

// -----------------------------------------------------------------------
// Abstract interface from AbstractNumericValidator
// -----------------------------------------------------------------------
void FloatDatatypeValidator::checkContent( const XMLCh* const content, bool asBase)
{

    //validate against base validator if any
    FloatDatatypeValidator *pBase = (FloatDatatypeValidator*) this->getBaseValidator();
    if (pBase)
        pBase->checkContent(content, true);

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

    try {
        XMLFloat theValue(content);
        XMLFloat *theData = &theValue;

        if (getEnumeration() != 0)
        {
            int i=0;
            int enumLength = getEnumeration()->size();
            for ( ; i < enumLength; i++)
            {
                if (compareValues(theData, (XMLFloat*) getEnumeration()->elementAt(i))==0)
                    break;
            }

            if (i == enumLength)
                ThrowXML1(InvalidDatatypeValueException, XMLExcepts::VALUE_NotIn_Enumeration, content);
        }

        boundsCheck(theData);
    }
    catch (XMLException &e)
    {
       ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::RethrowError, e.getMessage());
    }

}

/**
  * End of file FloatDatatypeValidator::cpp
  */
