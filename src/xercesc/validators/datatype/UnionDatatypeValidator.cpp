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
 * Revision 1.21  2003/12/17 00:18:39  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.20  2003/12/10 20:55:18  neilg
 * fixes for canonical value production; memory management was not implemented correctly
 *
 * Revision 1.19  2003/11/28 18:53:07  peiyongz
 * Support for getCanonicalRepresentation
 *
 * Revision 1.18  2003/11/24 05:10:26  neilg
 * implement method for determining member type of union that validated some value
 *
 * Revision 1.17  2003/11/13 23:19:18  peiyongz
 * initSize
 *
 * Revision 1.16  2003/11/12 20:32:03  peiyongz
 * Statless Grammar: ValidationContext
 *
 * Revision 1.15  2003/10/31 22:17:11  peiyongz
 * solve ownership
 *
 * Revision 1.14  2003/10/17 21:13:44  peiyongz
 * using XTemplateSerializer
 *
 * Revision 1.13  2003/10/07 19:39:03  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.12  2003/10/01 16:32:41  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.11  2003/08/16 18:42:49  neilg
 * fix for bug 22457.  Union types that are restrictions of other union types were previously considered not to inherit their parents member types.  This is at variance with the behaviour of the Java parser and apparently with the spec, so I have changed this.
 *
 * Revision 1.10  2003/05/16 06:01:57  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.9  2003/05/15 18:53:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.8  2003/02/06 13:51:55  gareth
 * fixed bug with multiple attributes being validated by the same union type.
 *
 * Revision 1.7  2003/01/29 19:53:35  gareth
 * we now store information about which validator was used to validate.
 *
 * Revision 1.6  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.5  2002/12/06 16:45:54  tng
 * header include cleanup.
 *
 * Revision 1.4  2002/11/04 14:53:28  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/09/24 19:44:40  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.2  2002/02/14 15:17:31  peiyongz
 * getEnumString()
 *
 * Revision 1.1.1.1  2002/02/01 22:22:42  peiyongz
 * sane_include
 *
 * Revision 1.5  2001/10/02 18:59:29  peiyongz
 * Invalid_Facet_Tag to display the tag name
 *
 * Revision 1.4  2001/09/20 13:11:42  knoaman
 * Regx  + misc. fixes
 *
 * Revision 1.3  2001/08/21 18:42:54  peiyongz
 * Bugzilla# 2816: cleanUp() declared with external linkage and called
 *                          before defined as inline
 *
 * Revision 1.2  2001/08/14 22:11:56  peiyongz
 * new exception message added
 *
 * Revision 1.1  2001/07/13 14:10:40  peiyongz
 * UnionDTV
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/UnionDatatypeValidator.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

#include <xercesc/internal/XTemplateSerializer.hpp>

XERCES_CPP_NAMESPACE_BEGIN

static const unsigned int BUF_LEN = 64;
static XMLCh value1[BUF_LEN+1];

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
UnionDatatypeValidator::UnionDatatypeValidator(MemoryManager* const manager)
:DatatypeValidator(0, 0, 0, DatatypeValidator::Union, manager)
,fEnumerationInherited(false)
,fEnumeration(0)
,fMemberTypeValidators(0)
,fValidatedDatatype(0)
,fMemberTypesInherited(false)
{}

UnionDatatypeValidator::~UnionDatatypeValidator()
{
    cleanUp();
}

UnionDatatypeValidator::UnionDatatypeValidator(
                        RefVectorOf<DatatypeValidator>* const memberTypeValidators
                      , const int                             finalSet
                      , MemoryManager* const                  manager)
:DatatypeValidator(0, 0, finalSet, DatatypeValidator::Union, manager)
,fEnumerationInherited(false)
,fEnumeration(0)
,fMemberTypeValidators(0)
,fValidatedDatatype(0)
,fMemberTypesInherited(false)
{
    if ( !memberTypeValidators )
    {
        ThrowXMLwithMemMgr(InvalidDatatypeFacetException
               , XMLExcepts::FACET_Union_Null_memberTypeValidators, manager);
    }

    // no pattern, no enumeration
    fMemberTypeValidators = memberTypeValidators;
}

UnionDatatypeValidator::UnionDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefArrayVectorOf<XMLCh>*      const enums
                        , const int                           finalSet
                        , MemoryManager* const                manager
                        , RefVectorOf<DatatypeValidator>* const memberTypeValidators 
                        , const bool memberTypesInherited
                        )
:DatatypeValidator(baseValidator, facets, finalSet, DatatypeValidator::Union, manager)
,fEnumerationInherited(false)
,fEnumeration(0)
,fMemberTypeValidators(memberTypeValidators)
,fValidatedDatatype(0)
,fMemberTypesInherited(memberTypesInherited)
{
    //
    // baseValidator another UnionDTV from which,
    // this UnionDTV is derived by restriction.
    // it shall be not null
    //
    if (!baseValidator)
    {
        ThrowXMLwithMemMgr(InvalidDatatypeFacetException
               , XMLExcepts::FACET_Union_Null_baseValidator, manager);
    }

    if (baseValidator->getType() != DatatypeValidator::Union)
    {
        XMLString::binToText(baseValidator->getType(), value1, BUF_LEN, 10, manager);
        ThrowXMLwithMemMgr1(InvalidDatatypeFacetException
                , XMLExcepts::FACET_Union_invalid_baseValidatorType
                , value1
                , manager);
    }

    try
    {
        init(baseValidator, facets, enums, manager);
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch (...)
    {
        cleanUp();
        throw;
    }
}

void UnionDatatypeValidator::init(DatatypeValidator*            const baseValidator
                                , RefHashTableOf<KVStringPair>* const facets
                                , RefArrayVectorOf<XMLCh>*      const enums
                                , MemoryManager*                const manager)
{
    if (enums)
        setEnumeration(enums, false);

    // Set Facets if any defined
    if (facets)
    {
        XMLCh* key;
        XMLCh* value;
        RefHashTableOfEnumerator<KVStringPair> e(facets, false, manager);

        while (e.hasMoreElements())
        {
            KVStringPair pair = e.nextElement();
            key = pair.getKey();
            value = pair.getValue();

            if (XMLString::equals(key, SchemaSymbols::fgELT_PATTERN))
            {
                setPattern(value);
                if (getPattern())
                    setFacetsDefined(DatatypeValidator::FACET_PATTERN);
                // do not construct regex until needed
            }
            else
            {
                 ThrowXMLwithMemMgr1(InvalidDatatypeFacetException
                         , XMLExcepts::FACET_Invalid_Tag
                         , key
                         , manager);
            }
        }//while

        /***
           Schema constraint: Part I -- self checking
        ***/
        // Nil

        /***
           Schema constraint: Part II base vs derived checking
        ***/
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
                    //
                    // enum need NOT be passed this->checkContent()
                    // since there are no other facets for Union, parent
                    // checking is good enough.
                    //
                    baseValidator->validate(getEnumeration()->elementAt(i), (ValidationContext*)0, manager);

                }
            }

            catch ( XMLException& )
            {
                ThrowXMLwithMemMgr1(InvalidDatatypeFacetException
                            , XMLExcepts::FACET_enum_base
                            , getEnumeration()->elementAt(i)
                            , manager);
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

    UnionDatatypeValidator *pBaseValidator = (UnionDatatypeValidator*) baseValidator;

    // inherit enumeration
    if (((pBaseValidator->getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) !=0) &&
        ((getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) == 0))
    {
        setEnumeration(pBaseValidator->getEnumeration(), true);
    }

}

//
// 1) the bottom level UnionDTV would check against
//        pattern and enumeration as well
// 2) each UnionDTV(s) above the bottom level UnionDTV and
//        below the native UnionDTV (the top level DTV)
//        would check against pattern only.
// 3) the natvie Union DTV (the top level DTV) would invoke
//        memberTypeValidator to validate
//
void UnionDatatypeValidator::checkContent(const XMLCh*             const content
                                        ,       ValidationContext* const context
                                        ,       bool                     asBase
                                        ,       MemoryManager*     const manager)
{

    DatatypeValidator* bv = getBaseValidator();
    if (bv)
        ((UnionDatatypeValidator*)bv)->checkContent(content, context, true, manager);
    else
    {   // 3) native union type
        // check content against each member type validator in Union
        // report an error only in case content is not valid against all member datatypes.
        //
        bool memTypeValid = false;
        for ( unsigned int i = 0; i < fMemberTypeValidators->size(); ++i )
        {
            if ( memTypeValid )
                break;

            try
            {
                fMemberTypeValidators->elementAt(i)->validate(content, context, manager);
                memTypeValid = true;
                
                //set the validator of the type actually used to validate the content
                DatatypeValidator *dtv = fMemberTypeValidators->elementAt(i);
                fValidatedDatatype = dtv;
                // context will be null during schema construction
                if(context)
                    context->setValidatingMemberType(dtv);
            }
            catch (XMLException&)
            {
                //absorbed
            }
        } // for

        if ( !memTypeValid )
        {
            ThrowXMLwithMemMgr1(InvalidDatatypeValueException
                    , XMLExcepts::VALUE_no_match_memberType
                    , content
                    , manager);
            //( "Content '"+content+"' does not match any union types" );
        }
    }

    // 1) and 2). we check pattern first
    if ( (getFacetsDefined() & DatatypeValidator::FACET_PATTERN ) != 0 )
    {
        // lazy construction
        if (getRegex() == 0)
        {
            try {
                setRegex(new (fMemoryManager) RegularExpression(getPattern(), SchemaSymbols::fgRegEx_XOption, fMemoryManager));
            }
            catch (XMLException &e)
            {
                ThrowXMLwithMemMgr1(InvalidDatatypeValueException, XMLExcepts::RethrowError, e.getMessage(), manager);
            }
        }

        if (getRegex()->matches(content, manager) == false)
        {
            ThrowXMLwithMemMgr2(InvalidDatatypeValueException
                    , XMLExcepts::VALUE_NotMatch_Pattern
                    , content
                    , getPattern()
                    , manager);
        }
    }

    // if this is a base validator, we only need to check pattern facet
    // all other facet were inherited by the derived type
    if (asBase)
        return;

    if ((getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) != 0 &&
        (getEnumeration() != 0))
    {

        // If the content match (compare equal) any enumeration with
        // any of the member types, it is considerd valid.
        //
        RefVectorOf<DatatypeValidator>* memberDTV = getMemberTypeValidators();
        RefArrayVectorOf<XMLCh>* tmpEnum = getEnumeration();
        unsigned int memberTypeNumber = memberDTV->size();
        unsigned int enumLength = tmpEnum->size();

        for ( unsigned int memberIndex = 0; memberIndex < memberTypeNumber; ++memberIndex)
        {
            for ( unsigned int enumIndex = 0; enumIndex < enumLength; ++enumIndex)
            {
                try
                {
                    if (memberDTV->elementAt(memberIndex)->compare(content, tmpEnum->elementAt(enumIndex), manager) == 0)
                        return;
                }
                catch (XMLException&)
                {
                    //absorbed
                }
            } // for enumIndex
        } // for memberIndex

        ThrowXMLwithMemMgr1(InvalidDatatypeValueException, XMLExcepts::VALUE_NotIn_Enumeration, content, manager);

    } // enumeration

}

//
//
//
int UnionDatatypeValidator::compare(const XMLCh* const lValue
                                  , const XMLCh* const rValue
                                  , MemoryManager* const manager)
{
    RefVectorOf<DatatypeValidator>* memberDTV = getMemberTypeValidators();
    unsigned int memberTypeNumber = memberDTV->size();

    for ( unsigned int memberIndex = 0; memberIndex < memberTypeNumber; ++memberIndex)
    {
        if (memberDTV->elementAt(memberIndex)->compare(lValue, rValue, manager) ==0)
            return  0;
    }

    //REVISIT: what does it mean for UNION1 to be <less than> or <greater than> UNION2 ?
    // As long as -1 or +1 indicates an unequality, return either of them is ok.
    return -1;
}

const RefArrayVectorOf<XMLCh>* UnionDatatypeValidator::getEnumString() const
{
	return getEnumeration();
}

/***
 * 2.5.1.3 Union datatypes
 *
 * The canonical-lexical-representation for a ·union· datatype is defined as the lexical form 
 * in which the values have the canonical lexical representation of the appropriate ·memberTypes·.       
 ***/
const XMLCh* UnionDatatypeValidator::getCanonicalRepresentation(const XMLCh*         const rawData
                                                              ,       MemoryManager* const memMgr ) const
{

    UnionDatatypeValidator* temp = (UnionDatatypeValidator*) this;
    temp->checkContent(rawData, 0, false, memMgr);

    //get the native unionDv
    UnionDatatypeValidator* bdv = (UnionDatatypeValidator*) temp->getBaseValidator();
    MemoryManager* toUse = memMgr? memMgr : getMemoryManager();
    while (bdv)
    {
        temp = bdv;
        bdv = (UnionDatatypeValidator*) temp->getBaseValidator();
    }

    //let the member dv which recognize the rawData, to return
    //us the canonical form
    for ( unsigned int i = 0; i < fMemberTypeValidators->size(); ++i )
    {
        try
        {
            fMemberTypeValidators->elementAt(i)->validate(rawData, 0, toUse);                       
            return fMemberTypeValidators->elementAt(i)->getCanonicalRepresentation(rawData, toUse);
        }
        catch (XMLException&)
        {
            //absorbed
        }
    }

    //its not likely we reach here, but who knows ...
    return 0;
}


/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(UnionDatatypeValidator)

void UnionDatatypeValidator::serialize(XSerializeEngine& serEng)
{

    DatatypeValidator::serialize(serEng);

    if (serEng.isStoring())
    {
        serEng<<fEnumerationInherited;
        serEng<<fMemberTypesInherited;

        /***
         * Serialize RefArrayVectorOf<XMLCh>
         * Serialize RefVectorOf<DatatypeValidator>
         ***/
        XTemplateSerializer::storeObject(fEnumeration, serEng);
        XTemplateSerializer::storeObject(fMemberTypeValidators, serEng);

        DatatypeValidator::storeDV(serEng, fValidatedDatatype);

    }
    else
    {
        serEng>>fEnumerationInherited;
        serEng>>fMemberTypesInherited;

        /***
         * Deserialize RefArrayVectorOf<XMLCh>
         * Deserialize RefVectorOf<DatatypeValidator>
         ***/
        bool owned = fEnumerationInherited ? true : false;
        XTemplateSerializer::loadObject(&fEnumeration, 8, owned, serEng);

        owned = fMemberTypesInherited ? true : false;
        XTemplateSerializer::loadObject(&fMemberTypeValidators, 4, owned, serEng);

        fValidatedDatatype = DatatypeValidator::loadDV(serEng);

    }
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file UnionDatatypeValidator.cpp
  */
