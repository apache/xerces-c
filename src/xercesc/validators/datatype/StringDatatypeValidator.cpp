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
 * Revision 1.6  2003/09/29 21:47:35  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.5  2003/05/15 18:53:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.3  2002/11/04 14:53:28  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/09/24 19:44:40  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.1.1.1  2002/02/01 22:22:42  peiyongz
 * sane_include
 *
 * Revision 1.17  2001/10/09 20:54:35  peiyongz
 * init(): take 1 arg.
 *
 * Revision 1.16  2001/09/27 13:51:25  peiyongz
 * DTV Reorganization: ctor/init created to be used by derived class
 *
 * Revision 1.15  2001/09/25 15:58:14  peiyongz
 * Check baseValidator in assignAdditinoalFacet() and inheritAdditionalFacet()
 *
 * Revision 1.14  2001/09/24 15:31:13  peiyongz
 * DTV Reorganization: inherit from AbstractStringValidator
 *
 * Revision 1.12  2001/08/21 18:42:53  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.11  2001/06/20 17:56:09  peiyongz
 * support for "fixed" option on constrainning facets
 *
 * Revision 1.10  2001/05/29 19:49:36  tng
 * Schema: Constraint Checking Fix in datatypeValidators.  By Pei Yong Zhang.
 *
 * Revision 1.9  2001/05/28 21:11:18  tng
 * Schema: Various DatatypeValidator fix.  By Pei Yong Zhang
 *
 * Revision 1.8  2001/05/23 16:05:11  tng
 * Schema: NormalizedString fix.  By Pei Yong Zhang.
 *
 * Revision 1.7  2001/05/23 15:45:25  tng
 * Schema: NormalizedString fix.  By Pei Yong Zhang.
 *
 * Revision 1.6  2001/05/18 13:36:48  tng
 * Schema: Catch RegularExpression exception and NumberFormatException
 *
 * Revision 1.5  2001/05/18 13:23:51  tng
 * Schema: Exception messages in DatatypeValidator.  By Pei Yong Zhang.
 *
 * Revision 1.4  2001/05/16 14:33:40  tng
 * Schema Constraint checking fix.
 *
 * Revision 1.3  2001/05/11 17:17:28  tng
 * Schema: DatatypeValidator fixes.  By Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/11 13:27:29  tng
 * Copyright update.
 *
 * Revision 1.1  2001/05/09 18:43:42  tng
 * Add StringDatatypeValidator and BooleanDatatypeValidator.  By Pei Yong Zhang.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/StringDatatypeValidator.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

static const int BUF_LEN = 64;
static XMLCh value1[BUF_LEN+1];
static XMLCh value2[BUF_LEN+1];

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
StringDatatypeValidator::StringDatatypeValidator(MemoryManager* const manager)
:AbstractStringValidator(0, 0, 0, DatatypeValidator::String, manager)
,fWhiteSpace(DatatypeValidator::PRESERVE)
{}

StringDatatypeValidator::StringDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefArrayVectorOf<XMLCh>*      const enums
                        , const int                           finalSet
                        , MemoryManager* const                manager)
:AbstractStringValidator(baseValidator, facets, finalSet, DatatypeValidator::String, manager)
,fWhiteSpace(DatatypeValidator::PRESERVE)
{
    init(enums);
}

StringDatatypeValidator::~StringDatatypeValidator()
{}

DatatypeValidator* StringDatatypeValidator::newInstance
(
      RefHashTableOf<KVStringPair>* const facets
    , RefArrayVectorOf<XMLCh>* const      enums
    , const int                           finalSet
    , MemoryManager* const                manager
)
{
    return (DatatypeValidator*) new (manager) StringDatatypeValidator(this, facets, enums, finalSet, manager);
}

StringDatatypeValidator::StringDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , const int                           finalSet
                        , const ValidatorType                 type
                        , MemoryManager* const                manager)
:AbstractStringValidator(baseValidator, facets, finalSet, type, manager)
,fWhiteSpace(DatatypeValidator::PRESERVE)
{
    // do not invoke init() here!!!
}

short StringDatatypeValidator::getWSFacet() const
{
    return fWhiteSpace;
}

// ---------------------------------------------------------------------------
//  Utilities
// ---------------------------------------------------------------------------
void StringDatatypeValidator::assignAdditionalFacet(const XMLCh* const key
                                                  , const XMLCh* const value)
{
    if (XMLString::equals(key, SchemaSymbols::fgELT_WHITESPACE))
    {
        // whiteSpace = preserve | replace | collapse
        if (XMLString::equals(value, SchemaSymbols::fgWS_PRESERVE))
            setWhiteSpace(DatatypeValidator::PRESERVE);
        else if (XMLString::equals(value, SchemaSymbols::fgWS_REPLACE))
            setWhiteSpace(DatatypeValidator::REPLACE);
        else if (XMLString::equals(value, SchemaSymbols::fgWS_COLLAPSE))
            setWhiteSpace(DatatypeValidator::COLLAPSE);
        else
            ThrowXML1(InvalidDatatypeFacetException, XMLExcepts::FACET_Invalid_WS, value);
        //("whiteSpace value '" + ws + "' must be one of 'preserve', 'replace', 'collapse'.");

        setFacetsDefined(DatatypeValidator::FACET_WHITESPACE);
    }
    else
    {
        ThrowXML1(InvalidDatatypeFacetException
                , XMLExcepts::FACET_Invalid_Tag
                , key);
    }
}

void StringDatatypeValidator::inheritAdditionalFacet()
{
    StringDatatypeValidator *pBaseValidator = (StringDatatypeValidator*) getBaseValidator();

    if (!pBaseValidator)
        return;

    // inherit whitespace
    if (((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) !=0) &&
        ((getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) == 0))
    {
        setWhiteSpace(getBaseValidator()->getWSFacet());
        setFacetsDefined(DatatypeValidator::FACET_WHITESPACE);
    }
}

void StringDatatypeValidator::checkAdditionalFacetConstraints() const
{

    StringDatatypeValidator *pBaseValidator = (StringDatatypeValidator*) getBaseValidator();

    if (!pBaseValidator)
        return;

    short    thisWSFacet = getWSFacet();
    short    baseWSFacet = pBaseValidator->getWSFacet();

    // check 4.3.6.c1 error: whitespace
    if (((getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) != 0) &&
        ((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) != 0 ))
    {
        if ((baseWSFacet == DatatypeValidator::COLLAPSE) &&
            ((thisWSFacet == DatatypeValidator::PRESERVE) ||
             (thisWSFacet == DatatypeValidator::REPLACE)))
             ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_WS_collapse);

        if ((baseWSFacet == DatatypeValidator::REPLACE) &&
            (thisWSFacet == DatatypeValidator::PRESERVE))
            ThrowXML(InvalidDatatypeFacetException, XMLExcepts::FACET_WS_replace);

        if (((pBaseValidator->getFixed() & DatatypeValidator::FACET_WHITESPACE) !=0) &&
            ( thisWSFacet != baseWSFacet))
        {
            ThrowXML2(InvalidDatatypeFacetException
                        , XMLExcepts::FACET_whitespace_base_fixed
                        , getWSstring(thisWSFacet)
                        , getWSstring(baseWSFacet));
        }
    }

}

void StringDatatypeValidator::checkAdditionalFacet(const XMLCh* const content) const
{
    //
    // check WhiteSpace
    //
    if ((getFacetsDefined() & DatatypeValidator::FACET_WHITESPACE) != 0 )
    {
        if ( getWSFacet() == DatatypeValidator::REPLACE )
        {
            if (!XMLString::isWSReplaced(content))
                ThrowXML1(InvalidDatatypeValueException, XMLExcepts::VALUE_WS_replaced, content);
        }
        else if ( getWSFacet() == DatatypeValidator::COLLAPSE )
        {
            if (!XMLString::isWSCollapsed(content))
                ThrowXML1(InvalidDatatypeValueException, XMLExcepts::VALUE_WS_collapsed, content);
        }

    }
}

void StringDatatypeValidator::checkValueSpace(const XMLCh* const content)
{}

int StringDatatypeValidator::getLength(const XMLCh* const content) const
{
    return XMLString::stringLen(content);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(StringDatatypeValidator)

void StringDatatypeValidator::serialize(XSerializeEngine& serEng)
{
    AbstractStringValidator::serialize(serEng);
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file StringDatatypeValidator.cpp
  */
