/*
 * Copyright 2001,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id$
 * $Log$
 * Revision 1.23  2004/09/08 13:56:53  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.22  2004/01/29 11:51:22  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.21  2004/01/13 21:18:18  peiyongz
 * revert code back to previous version
 *
 * Revision 1.20  2004/01/12 16:25:09  neilg
 * remove use of static buffers
 *
 * Revision 1.19  2004/01/06 18:13:59  peiyongz
 * using the no-exception-thrown ctor
 *
 * Revision 1.18  2003/12/31 02:34:41  neilg
 * fix one more buffer overrun, affecting boolean lists
 *
 * Revision 1.17  2003/12/23 21:50:36  peiyongz
 * Absorb exception thrown in getCanonicalRepresentation and return 0,
 * only validate when required
 *
 * Revision 1.16  2003/12/17 20:41:47  neilg
 * fix a segfault and a possible buffer overflow condition
 *
 * Revision 1.15  2003/12/17 00:18:39  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.14  2003/12/10 20:52:27  neilg
 * fixes for canonical value production; memory management was not implemented correctly
 *
 * Revision 1.13  2003/11/28 18:53:07  peiyongz
 * Support for getCanonicalRepresentation
 *
 * Revision 1.12  2003/11/12 20:32:03  peiyongz
 * Statless Grammar: ValidationContext
 *
 * Revision 1.11  2003/10/01 16:32:41  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.10  2003/09/30 21:31:30  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.9  2003/05/16 06:01:57  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.8  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.7  2002/12/18 14:17:55  gareth
 * Fix to bug #13438. When you eant a vector that calls delete[] on its members you should use RefArrayVectorOf.
 *
 * Revision 1.6  2002/12/16 22:28:25  knoaman
 * Make isAtomic inline.
 *
 * Revision 1.5  2002/12/06 16:45:54  tng
 * header include cleanup.
 *
 * Revision 1.4  2002/11/20 19:57:44  peiyongz
 * validate content as a whole against pattern.
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
 * Revision 1.8  2001/10/11 19:32:27  peiyongz
 * To overwrite inheritFacet()
 *
 * Revision 1.7  2001/10/09 20:56:21  peiyongz
 * inherit from AbstractStringValidator instead of DatatypeValidator to reuse
 * the code.
 *
 * Revision 1.6  2001/10/02 18:59:29  peiyongz
 * Invalid_Facet_Tag to display the tag name
 *
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
#include <xercesc/validators/datatype/ListDatatypeValidator.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeFacetException.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

static const int BUF_LEN = 64;

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
ListDatatypeValidator::ListDatatypeValidator(MemoryManager* const manager)
:AbstractStringValidator(0, 0, 0, DatatypeValidator::List, manager)
,fContent(0)
{}

ListDatatypeValidator::ListDatatypeValidator(
                          DatatypeValidator*            const baseValidator
                        , RefHashTableOf<KVStringPair>* const facets
                        , RefArrayVectorOf<XMLCh>*           const enums
                        , const int                           finalSet
                        , MemoryManager* const manager)
:AbstractStringValidator(baseValidator, facets, finalSet, DatatypeValidator::List, manager)
,fContent(0)
{
    //
    // baseValidator shall either
    // an atomic DTV which servers as itemType, or
    // another ListDTV from which, this ListDTV is derived by restriction.
    //
    // In either case, it shall be not null
    //
    if (!baseValidator)
        ThrowXMLwithMemMgr(InvalidDatatypeFacetException, XMLExcepts::FACET_List_Null_baseValidator, manager);

    init(enums, manager);
}

ListDatatypeValidator::~ListDatatypeValidator()
{}

DatatypeValidator* ListDatatypeValidator::newInstance
(
      RefHashTableOf<KVStringPair>* const facets
    , RefArrayVectorOf<XMLCh>* const      enums
    , const int                           finalSet
    , MemoryManager* const                manager
)
{
    return (DatatypeValidator*) new (manager) ListDatatypeValidator(this, facets, enums, finalSet, manager);
}


int ListDatatypeValidator::compare(const XMLCh*     const lValue
                                 , const XMLCh*     const rValue
                                 , MemoryManager*   const manager)
{
    DatatypeValidator* theItemTypeDTV = getItemTypeDTV();
    BaseRefVectorOf<XMLCh>* lVector = XMLString::tokenizeString(lValue, manager);
    Janitor<BaseRefVectorOf<XMLCh> > janl(lVector);
    BaseRefVectorOf<XMLCh>* rVector = XMLString::tokenizeString(rValue, manager);
    Janitor<BaseRefVectorOf<XMLCh> > janr(rVector);

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
            int returnValue = theItemTypeDTV->compare(lVector->elementAt(i), rVector->elementAt(i), manager);
            if (returnValue != 0)
                return returnValue; //REVISIT: does it make sense to return -1 or +1..?
        }
        return 0;
    }

}

void ListDatatypeValidator::validate( const XMLCh*             const content
                                    ,       ValidationContext* const context
                                    ,       MemoryManager*     const manager)
{
    setContent(content);
    BaseRefVectorOf<XMLCh>* tokenVector = XMLString::tokenizeString(content, manager);
    Janitor<BaseRefVectorOf<XMLCh> > janName(tokenVector);
    checkContent(tokenVector, content, context, false, manager);
}

void ListDatatypeValidator::checkContent( const XMLCh*             const content
                                         ,      ValidationContext* const context
                                         ,      bool                     asBase
                                         ,      MemoryManager*     const manager)
{
    setContent(content);
    BaseRefVectorOf<XMLCh>* tokenVector = XMLString::tokenizeString(content, manager);
    Janitor<BaseRefVectorOf<XMLCh> > janName(tokenVector);
    checkContent(tokenVector, content, context, asBase, manager);
}

//
// here content is a list of items
//
void ListDatatypeValidator::checkContent(       BaseRefVectorOf<XMLCh>*       tokenVector
                                        , const XMLCh*                  const content
                                        ,       ValidationContext*      const context
                                        ,       bool                          asBase
                                        ,       MemoryManager*          const manager)
{
    DatatypeValidator* bv = getBaseValidator();

    if (bv->getType() == DatatypeValidator::List)
        ((ListDatatypeValidator*)bv)->checkContent(tokenVector, content, context, true, manager);
    else
    {   // the ultimate itemType DTV
        for (unsigned int i = 0; i < tokenVector->size(); i++)
            bv->validate(tokenVector->elementAt(i), context, manager);
    }

    int thisFacetsDefined = getFacetsDefined();

    // we check pattern first
    if ( (thisFacetsDefined & DatatypeValidator::FACET_PATTERN ) != 0 )
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

        //check every item in the list as a whole
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

    unsigned int tokenNumber = tokenVector->size();

    if (((thisFacetsDefined & DatatypeValidator::FACET_MAXLENGTH) != 0) &&
        (tokenNumber > getMaxLength()))
    {
        XMLCh value1[BUF_LEN+1];
        XMLCh value2[BUF_LEN+1];
        XMLString::binToText(tokenNumber, value1, BUF_LEN, 10, manager);
        XMLString::binToText(getMaxLength(), value2, BUF_LEN, 10, manager);

        ThrowXMLwithMemMgr3(InvalidDatatypeValueException
                , XMLExcepts::VALUE_GT_maxLen
                , getContent()
                , value1
                , value2
                , manager);
    }

    if (((thisFacetsDefined & DatatypeValidator::FACET_MINLENGTH) != 0) &&
        (tokenNumber < getMinLength()))
    {
        XMLCh value1[BUF_LEN+1];
        XMLCh value2[BUF_LEN+1];
        XMLString::binToText(tokenNumber, value1, BUF_LEN, 10, manager);
        XMLString::binToText(getMinLength(), value2, BUF_LEN, 10, manager);

        ThrowXMLwithMemMgr3(InvalidDatatypeValueException
                , XMLExcepts::VALUE_LT_minLen
                , getContent()
                , value1
                , value2
                , manager);
    }

    if (((thisFacetsDefined & DatatypeValidator::FACET_LENGTH) != 0) &&
        (tokenNumber != AbstractStringValidator::getLength()))
    {
        XMLCh value1[BUF_LEN+1];
        XMLCh value2[BUF_LEN+1];
        XMLString::binToText(tokenNumber, value1, BUF_LEN, 10, manager);
        XMLString::binToText(AbstractStringValidator::getLength(), value2, BUF_LEN, 10, manager);

        ThrowXMLwithMemMgr3(InvalidDatatypeValueException
                , XMLExcepts::VALUE_NE_Len
                , getContent()
                , value1
                , value2
                , manager);
    }

    if ((thisFacetsDefined & DatatypeValidator::FACET_ENUMERATION) != 0 &&
        (getEnumeration() != 0))
    {
        int i;
        int enumLength = getEnumeration()->size();

        for ( i = 0; i < enumLength; i++)
        {
            //optimization: we do a lexical comparision first
            // this may be faster for string and its derived
            if (XMLString::equals(getEnumeration()->elementAt(i), getContent()))
                break; // a match found

            // do a value space check
            // this is needed for decimal (and probably other types
            // such as datetime related)
            // eg.
            // tokenVector = "1 2 3.0 4" vs enumeration = "1 2 3 4.0"
            //
            if (valueSpaceCheck(tokenVector, getEnumeration()->elementAt(i), manager))
                break;
        }

        if (i == enumLength)
            ThrowXMLwithMemMgr1(InvalidDatatypeValueException, XMLExcepts::VALUE_NotIn_Enumeration, getContent(), manager);

    } // enumeration

}

bool ListDatatypeValidator::valueSpaceCheck(BaseRefVectorOf<XMLCh>* tokenVector
                                          , const XMLCh*    const  enumStr
                                          , MemoryManager*  const  manager) const
{
    DatatypeValidator* theItemTypeDTV = getItemTypeDTV();
    BaseRefVectorOf<XMLCh>* enumVector = XMLString::tokenizeString(enumStr, manager);
    Janitor<BaseRefVectorOf<XMLCh> > janName(enumVector);

    if (tokenVector->size() != enumVector->size())
        return false;

    for ( unsigned int j = 0; j < tokenVector->size(); j++ )
    {
        if (theItemTypeDTV->compare(tokenVector->elementAt(j), enumVector->elementAt(j), manager) != 0)
            return false;
    }

    return true;
}

DatatypeValidator* ListDatatypeValidator::getItemTypeDTV() const
{
    DatatypeValidator* bdv = this->getBaseValidator();

    while (bdv->getType() == DatatypeValidator::List)
        bdv = bdv->getBaseValidator();

    return bdv;
}

// ---------------------------------------------------------------------------
//  Utilities
// ---------------------------------------------------------------------------

void ListDatatypeValidator::checkValueSpace(const XMLCh* const
                                            , MemoryManager* const)
{}

int ListDatatypeValidator::getLength(const XMLCh* const content
                                     , MemoryManager* const manager) const
{
    BaseRefVectorOf<XMLCh>* tokenVector = XMLString::tokenizeString(content, manager);
    Janitor<BaseRefVectorOf<XMLCh> > janName(tokenVector);

    return tokenVector->size();
}

void ListDatatypeValidator::inspectFacetBase(MemoryManager* const manager)
{

    //
    // we are pretty sure baseValidator is not null
    //

    if (getBaseValidator()->getType() == DatatypeValidator::List)
    {
        AbstractStringValidator::inspectFacetBase(manager);
    }
    else
    {
        // the first level ListDTV
        // check 4.3.5.c0 must: enumeration values from the value space of base
        if ( ((getFacetsDefined() & DatatypeValidator::FACET_ENUMERATION) != 0) &&
             (getEnumeration() !=0)                                              )
        {
            int i;
            int enumLength = getEnumeration()->size();
            try
            {
                for ( i = 0; i < enumLength; i++)
                {
                    // ask the itemType for a complete check
                    BaseRefVectorOf<XMLCh>* tempList = XMLString::tokenizeString(getEnumeration()->elementAt(i), manager);
                    Janitor<BaseRefVectorOf<XMLCh> >    jan(tempList);
                    int tokenNumber = tempList->size();

                    try
                    {
                        for ( int j = 0; j < tokenNumber; j++)
                            getBaseValidator()->validate(tempList->elementAt(j), (ValidationContext*)0, manager);
                    }
                    catch(const OutOfMemoryException&)
                    {
                        jan.release();

                        throw;
                    }

                    // enum shall pass this->checkContent() as well.
                    checkContent(getEnumeration()->elementAt(i), (ValidationContext*)0, false, manager);
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

    }

}// End of inspectFacetBase()

void ListDatatypeValidator::inheritFacet()
{

    //iff the base validator is List, then we inherit
    //
    if (getBaseValidator()->getType() == DatatypeValidator::List)
    {
        AbstractStringValidator::inheritFacet();
    }

}

/***
 * 2.5.1.2 List datatypes   
 *   
 * The canonical-lexical-representation for the ·list· datatype is defined as 
 * the lexical form in which each item in the ·list· has the canonical 
 * lexical representation of its ·itemType·.
 ***/
const XMLCh* ListDatatypeValidator::getCanonicalRepresentation(const XMLCh*         const rawData
                                                             ,       MemoryManager* const memMgr
                                                             ,       bool                 toValidate) const
{
    MemoryManager* toUse = memMgr? memMgr : getMemoryManager();
    ListDatatypeValidator* temp = (ListDatatypeValidator*) this;
    temp->setContent(rawData);
    BaseRefVectorOf<XMLCh>* tokenVector = XMLString::tokenizeString(rawData, toUse);
    Janitor<BaseRefVectorOf<XMLCh> > janName(tokenVector);    

    if (toValidate)
    {
        try
        {
            temp->checkContent(tokenVector, rawData, 0, false, toUse);
        }
        catch (...)
        {
            return 0;
        }
    }
   
    unsigned int  retBufSize = 2 * XMLString::stringLen(rawData);
    XMLCh* retBuf = (XMLCh*) toUse->allocate(retBufSize * sizeof(XMLCh));
    retBuf[0] = 0;
    XMLCh* retBufPtr = retBuf;
    DatatypeValidator* itemDv = this->getItemTypeDTV();

    try 
    {
        for (unsigned int i = 0; i < tokenVector->size(); i++)
        {
            XMLCh* itemCanRep = (XMLCh*) itemDv->getCanonicalRepresentation(tokenVector->elementAt(i), toUse, false);
            unsigned int itemLen = XMLString::stringLen(itemCanRep); 

            if(retBufPtr+itemLen+2 >= retBuf+retBufSize)
            {
                // need to resize
                XMLCh * oldBuf = retBuf;
                retBuf = (XMLCh*) toUse->allocate(retBufSize * sizeof(XMLCh) * 4);
                memcpy(retBuf, oldBuf, retBufSize * sizeof(XMLCh ));
                retBufPtr = (retBufPtr - oldBuf) + retBuf;
                toUse->deallocate(oldBuf);
                retBufSize <<= 2;
            }

            XMLString::catString(retBufPtr, itemCanRep);
            retBufPtr = retBufPtr + itemLen + 1;
            *(retBufPtr++) = chSpace;
            *(retBufPtr) = chNull;
            toUse->deallocate(itemCanRep);
        }

        return retBuf;

    }
    catch (...)
    {
        return 0;
    }
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(ListDatatypeValidator)

void ListDatatypeValidator::serialize(XSerializeEngine& serEng)
{
    AbstractStringValidator::serialize(serEng);

    //don't serialize fContent, since it is NOT owned and 
    //will be reset each time validate()/checkContent() invoked.
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file ListDatatypeValidator.cpp
  */
