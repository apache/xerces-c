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
 * Revision 1.7  2003/09/29 21:47:35  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.6  2003/05/15 18:53:26  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.5  2003/01/29 19:52:32  gareth
 * PSVI API additions.
 *
 * Revision 1.4  2002/12/19 14:03:10  gareth
 * get/set methods to see if the represented type is anonymous. Patch by Peter Volchek.
 *
 * Revision 1.3  2002/11/04 14:53:28  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/10/15 18:04:31  knoaman
 * Bug [13485]: incorrect return from getWSstring
 *
 * Revision 1.1.1.1  2002/02/01 22:22:40  peiyongz
 * sane_include
 *
 * Revision 1.7  2001/06/20 17:56:56  peiyongz
 * support for "fixed" option on constrainning facets
 *
 * Revision 1.6  2001/05/11 21:51:09  knoaman
 * Schema updates and fixes.
 *
 * Revision 1.5  2001/05/11 13:27:27  tng
 * Copyright update.
 *
 * Revision 1.4  2001/05/09 18:43:38  tng
 * Add StringDatatypeValidator and BooleanDatatypeValidator.  By Pei Yong Zhang.
 *
 * Revision 1.3  2001/05/03 21:09:52  tng
 * Schema: DatatypeValidator Update.  By Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/03 19:17:43  knoaman
 * TraverseSchema Part II.
 *
 * Revision 1.1  2001/03/21 21:39:13  knoaman
 * Schema symbols and Datatype validator part I
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/datatype/DatatypeValidator.hpp>
#include <xercesc/framework/MemoryManager.hpp>

//since we need to dynamically created each and every derivatives 
//during deserialization by XSerializeEngine>>Derivative, we got
//to include all hpp

#include <xercesc/validators/datatype/StringDatatypeValidator.hpp>
#include <xercesc/validators/datatype/AnyURIDatatypeValidator.hpp>
#include <xercesc/validators/datatype/QNameDatatypeValidator.hpp>
#include <xercesc/validators/datatype/NameDatatypeValidator.hpp>
#include <xercesc/validators/datatype/NCNameDatatypeValidator.hpp>
#include <xercesc/validators/datatype/BooleanDatatypeValidator.hpp>
#include <xercesc/validators/datatype/FloatDatatypeValidator.hpp>
#include <xercesc/validators/datatype/DoubleDatatypeValidator.hpp>
#include <xercesc/validators/datatype/DecimalDatatypeValidator.hpp>
#include <xercesc/validators/datatype/HexBinaryDatatypeValidator.hpp>
#include <xercesc/validators/datatype/Base64BinaryDatatypeValidator.hpp>
#include <xercesc/validators/datatype/DurationDatatypeValidator.hpp>
#include <xercesc/validators/datatype/DateTimeDatatypeValidator.hpp>
#include <xercesc/validators/datatype/DateDatatypeValidator.hpp>
#include <xercesc/validators/datatype/TimeDatatypeValidator.hpp>
#include <xercesc/validators/datatype/MonthDayDatatypeValidator.hpp>
#include <xercesc/validators/datatype/YearMonthDatatypeValidator.hpp>
#include <xercesc/validators/datatype/YearDatatypeValidator.hpp>
#include <xercesc/validators/datatype/MonthDatatypeValidator.hpp>
#include <xercesc/validators/datatype/DayDatatypeValidator.hpp>
#include <xercesc/validators/datatype/IDDatatypeValidator.hpp>
#include <xercesc/validators/datatype/IDREFDatatypeValidator.hpp>
#include <xercesc/validators/datatype/ENTITYDatatypeValidator.hpp>
#include <xercesc/validators/datatype/NOTATIONDatatypeValidator.hpp>
#include <xercesc/validators/datatype/ListDatatypeValidator.hpp>
#include <xercesc/validators/datatype/UnionDatatypeValidator.hpp>
#include <xercesc/validators/datatype/AnySimpleTypeDatatypeValidator.hpp>

XERCES_CPP_NAMESPACE_BEGIN



// ---------------------------------------------------------------------------
//  DatatypeValidator: Constructors and Destructor
// ---------------------------------------------------------------------------
DatatypeValidator::DatatypeValidator(DatatypeValidator* const baseValidator,
                                     RefHashTableOf<KVStringPair>* const facets,
                                     const int finalSet,
                                     const ValidatorType type,
                                     MemoryManager* const manager)
    : fMemoryManager(manager)
    , fAnonymous(false)
    , fFinalSet(finalSet)
    , fFacetsDefined(0)
    , fFixed(0)
    , fType(type)
    , fBaseValidator(baseValidator)
    , fFacets(facets)
    , fPattern(0)
    , fRegex(0)
    , fTypeLocalName(XMLUni::fgZeroLenString)
    , fTypeName(0)
    , fTypeUri(XMLUni::fgZeroLenString)
{
}

DatatypeValidator::~DatatypeValidator()
{
	cleanUp();
}

const XMLCh* DatatypeValidator::getWSstring(const short theType) const
{
    switch (theType)
    {
    case PRESERVE:
         return SchemaSymbols::fgWS_PRESERVE;
    case REPLACE:
         return SchemaSymbols::fgWS_REPLACE;
    case COLLAPSE:
         return SchemaSymbols::fgWS_COLLAPSE;
    default:
         return SchemaSymbols::fgWS_PRESERVE;
    }

}

void DatatypeValidator::setTypeName(const XMLCh* const name, const XMLCh* const uri)
{
    if (fTypeName) {

        fMemoryManager->deallocate(fTypeName);
        fTypeName = 0;
    }

    if (name || uri) {

        unsigned int nameLen = XMLString::stringLen(name);
        unsigned int uriLen = XMLString::stringLen(uri);

        fTypeName = (XMLCh*) fMemoryManager->allocate
        (
            (nameLen + uriLen + 2)*sizeof(XMLCh)
        );
        fTypeUri = fTypeName;
        fTypeLocalName = &fTypeName[uriLen+1];

        if (uri)
			XMLString::moveChars(fTypeName, uri, uriLen+1);
        else
			fTypeName[0] = chNull;

        if (name)
            XMLString::moveChars(&fTypeName[uriLen+1], name, nameLen+1);
        else
            fTypeName[uriLen+1] = chNull;
    }
    else
    {
        fTypeUri = fTypeLocalName = XMLUni::fgZeroLenString;
    }
}

void DatatypeValidator::setTypeName(const XMLCh* const typeName)
{
    if (fTypeName)
    {
        fMemoryManager->deallocate(fTypeName);
        fTypeName = 0;
    }

    if (typeName)
    {
        unsigned int nameLen = XMLString::stringLen(typeName);
        int commaOffset = XMLString::indexOf(typeName, chComma);

        fTypeName = (XMLCh*) fMemoryManager->allocate
        (
            (nameLen + 1) * sizeof(XMLCh)
        );
	    XMLString::moveChars(fTypeName, typeName, nameLen+1);

        if ( commaOffset == -1) {
            fTypeUri = SchemaSymbols::fgURI_SCHEMAFORSCHEMA;
            fTypeLocalName = fTypeName;
        }
        else {
            fTypeUri = fTypeName;
            fTypeLocalName = &fTypeName[commaOffset+1];
            fTypeName[commaOffset] = chNull;
        }
    }
    else
    {
        fTypeUri = fTypeLocalName = XMLUni::fgZeroLenString;
    }
}

// ---------------------------------------------------------------------------
//  DatatypeValidator: CleanUp methods
// ---------------------------------------------------------------------------
void DatatypeValidator::cleanUp() {

	delete fFacets;
    delete fRegex;
    if (fPattern)
        fMemoryManager->deallocate(fPattern);//delete [] fPattern;
    if (fTypeName)
        fMemoryManager->deallocate(fTypeName);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_NOCREATE(DatatypeValidator)

void DatatypeValidator::serialize(XSerializeEngine& serEng)
{

    if (serEng.isStoring())
    {
        serEng<<fAnonymous;
        serEng<<fFinalSet;
        serEng<<fFacetsDefined;
        serEng<<fFixed;
        serEng<<(int)fType;

        /***
         *
         *  When deserialized, we need to know, exactly what
         *  validator was serialized here.
         *
         ***/
        if (fBaseValidator)
        {
            serEng<<(int) (fBaseValidator->getType());
            serEng<<fBaseValidator;
        }
        else
        {
            serEng<<(int) UnKnown;
        }

        /***
         *  Serialize RefHashTableOf<KVStringPair>
         *
         *  Since the RefHashTableOf does not privide a size() method
         *  We have to traverse extra time to gather this information
         *  first, and then the second time for actual serialization.
         *
         *  We only need to serialize KVStringPair, when deserialize,
         *  the KVStringPair.key() can be used as the key to the hash table.
         *  
         ***/
        if (fFacets)
        {
            int itemNumber = 0;

            RefHashTableOfEnumerator<KVStringPair> e(fFacets);
            while (e.hasMoreElements())
            {
                e.nextElement();
                itemNumber++;
            }

            serEng<<itemNumber;

            e.Reset();
            while (e.hasMoreElements())
            {
                KVStringPair& curPair = e.nextElement();
                curPair.serialize(serEng);
            }
        }
        else
        {
            serEng<<0;
        }

        serEng.writeString(fPattern);

        /***
         * don't serialize fRegex
         ***/

        serEng.writeString(fTypeName);
        serEng.writeString(fTypeLocalName);
        serEng.writeString(fTypeUri);
    }
    else
    {
        int dataLen = 0;

        serEng>>fAnonymous;
        serEng>>fFinalSet;
        serEng>>fFacetsDefined;
        serEng>>fFixed;

        int type;
        serEng>>type;
        fType=(ValidatorType)type;

        /***
         *
         *  get the basevalidator's type
         *
         ***/

        serEng>>type;

        switch((ValidatorType)type)
        {
        case String: 
            StringDatatypeValidator* stringdv;
            serEng>>stringdv;
            fBaseValidator = stringdv;
            break;
        case AnyURI:
            AnyURIDatatypeValidator* anyuridv;
            //TODO
            //serEng>>anyuridv;
            fBaseValidator = anyuridv;
            break;
        case QName: 
            QNameDatatypeValidator* qnamedv;
            //TODO
            //serEng>>qnamedv;
            fBaseValidator = qnamedv;
            break;
        case Name: 
            NameDatatypeValidator* namedv;
            //TODO
            //serEng>>namedv;
            fBaseValidator = namedv;
            break;
        case NCName:  
            NCNameDatatypeValidator* ncnamedv;
            //TODO
            //serEng>>ncnamedv;
            fBaseValidator = ncnamedv;
            break;
        case Boolean: 
            BooleanDatatypeValidator* booleandv;
            //TODO
            //serEng>>booleandv;
            fBaseValidator = booleandv;
            break;
        case Float: 
            FloatDatatypeValidator* floatdv;
            //TODO
            //serEng>>floatdv;
            fBaseValidator = floatdv;
            break;
        case Double: 
            DoubleDatatypeValidator* doubledv;
            //TODO
            //serEng>>doubledv;
            fBaseValidator = doubledv;
            break;
        case Decimal: 
            DecimalDatatypeValidator* decimaldv;
            //TODO
            //serEng>>decimaldv;
            fBaseValidator = decimaldv;
            break;
        case HexBinary:  
            HexBinaryDatatypeValidator* hexbinarydv;
            //TODO
            //serEng>>hexbinarydv;
            fBaseValidator = hexbinarydv;
            break;
        case Base64Binary: 
            Base64BinaryDatatypeValidator* base64binarydv;
            //TODO
            //serEng>>base64binarydv;
            fBaseValidator = base64binarydv;
            break;
        case Duration:     
            DurationDatatypeValidator* durationdv;
            //TODO
            //serEng>>durationdv;
            fBaseValidator = durationdv;
            break;
        case DateTime:       
            DateTimeDatatypeValidator* datetimedv;
            //TODO
            //serEng>>datetimedv;
            fBaseValidator = datetimedv;
            break;
        case Date:          
            DateDatatypeValidator* datedv;
            //TODO
            //serEng>>datedv;
            fBaseValidator = datedv;
            break;
        case Time:         
            TimeDatatypeValidator* timedv;
            //TODO
            //serEng>>timedv;
            fBaseValidator = timedv;
            break;
        case MonthDay:      
            MonthDayDatatypeValidator* monthdaydv;
            //TODO
            //serEng>>monthdaydv;
            fBaseValidator = monthdaydv;
            break;
        case YearMonth:     
            YearMonthDatatypeValidator* yearmonthdv;
            //TODO
            //serEng>>yearmonthdv;
            fBaseValidator = yearmonthdv;
            break;
        case Year:          
            YearDatatypeValidator* yeardv;
            //TODO
            //serEng>>yeardv;
            fBaseValidator = yeardv;
            break;
        case Month:        
            MonthDatatypeValidator* monthdv;
            //TODO
            //serEng>>monthdv;
            fBaseValidator = monthdv;
            break;
        case Day:           
            DayDatatypeValidator* daydv;
            //TODO
            //serEng>>daydv;
            fBaseValidator = daydv;
            break;
        case ID:           
            IDDatatypeValidator* iddv;
            //TODO
            //serEng>>iddv;
            fBaseValidator = iddv;
            break;
        case IDREF:         
            IDREFDatatypeValidator* idrefdv;
            //TODO
            //serEng>>idrefdv;
            fBaseValidator = idrefdv;
            break;
        case ENTITY:       
            ENTITYDatatypeValidator* entitydv;
            //TODO
            //serEng>>entitydv;
            fBaseValidator = entitydv;
            break;
        case NOTATION:     
            NOTATIONDatatypeValidator* notationdv;
            //TODO
            //serEng>>notationdv;
            fBaseValidator = notationdv;
            break;
        case List:          
            ListDatatypeValidator* listdv;
            //TODO
            //serEng>>listdv;
            fBaseValidator = listdv;
            break;
        case Union:         
            UnionDatatypeValidator* uniondv;
            //TODO
            //serEng>>uniondv;
            fBaseValidator = uniondv;
            break;
        case AnySimpleType:  
            AnySimpleTypeDatatypeValidator* anysimpletypedv;
            //TODO
            //serEng>>anysimpletypedv;
            fBaseValidator = anysimpletypedv;
            break;
        case UnKnown:
            fBaseValidator = 0;
            break;
        default: //we treat this same as UnKnown
            fBaseValidator = 0;
            break;
        }

        /***
         *  Deserialize RefHashTableOf<KVStringPair>
         *
        ***/
        int itemNumber = 0;
        serEng>>itemNumber;

        if (itemNumber)
        {
            if (!fFacets)
            {
                fFacets = new (fMemoryManager) RefHashTableOf<KVStringPair>(3, fMemoryManager);
            }

            for (int itemIndex = 0; itemIndex < itemNumber; itemIndex++)
            {
                KVStringPair*  data = new (fMemoryManager) KVStringPair(fMemoryManager);
                data->serialize(serEng);
                fFacets->put((void*) data->getKey(), data);        
            }
        }

        serEng.readString(fPattern);

        /***
         * don't serialize fRegex
         ***/
        fRegex = 0;

        serEng.readString(fTypeName);
        serEng.readString((XMLCh*&)fTypeLocalName);
        serEng.readString((XMLCh*&)fTypeUri);

    }

}

XERCES_CPP_NAMESPACE_END

/**
  * End of file DatatypeValidator.cpp
  */

