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
 * Revision 1.22  2004/03/02 23:22:06  peiyongz
 * save/load TypeUri/TypeLocalName more accurately
 *
 * Revision 1.21  2004/01/29 11:51:22  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.20  2003/12/23 21:50:36  peiyongz
 * Absorb exception thrown in getCanonicalRepresentation and return 0,
 * only validate when required
 *
 * Revision 1.19  2003/12/17 00:18:38  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.18  2003/11/28 18:53:07  peiyongz
 * Support for getCanonicalRepresentation
 *
 * Revision 1.17  2003/11/13 23:19:18  peiyongz
 * initSize
 *
 * Revision 1.16  2003/11/07 22:39:41  peiyongz
 * PSVI/schema component model implementation, thanks to David Cargill
 *
 * Revision 1.15  2003/11/06 15:30:07  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.14  2003/11/05 16:37:18  peiyongz
 * don't serialize built-in baseValidator, and don't serialize localName/uriName
 *
 * Revision 1.13  2003/10/17 21:13:44  peiyongz
 * using XTemplateSerializer
 *
 * Revision 1.12  2003/10/07 19:39:37  peiyongz
 * Use of Template_Class Object Serialization/Deserialization API
 *
 * Revision 1.11  2003/10/02 19:20:19  peiyongz
 * fWhiteSpace added; more dv serialization support added
 *
 * Revision 1.10  2003/10/01 01:09:35  knoaman
 * Refactoring of some code to improve performance.
 *
 * Revision 1.9  2003/09/30 21:32:23  peiyongz
 * AnyURI, Base64, HexBin, NOTATION, List and QName
 *
 * Revision 1.8  2003/09/30 18:17:53  peiyongz
 * Implementation of Serialization/Deserialization
 *
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

#include <xercesc/internal/XTemplateSerializer.hpp>

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
    , fWhiteSpace(COLLAPSE)
    , fFinalSet(finalSet)
    , fFacetsDefined(0)
    , fFixed(0)
    , fType(type)
    , fBaseValidator(baseValidator)
    , fFacets(facets)
    , fPattern(0)
    , fRegex(0)
    , fTypeName(0)
    , fTypeLocalName(XMLUni::fgZeroLenString)
    , fTypeUri(XMLUni::fgZeroLenString)
    , fOrdered(XSSimpleTypeDefinition::ORDERED_FALSE)
    , fFinite(false)
    , fBounded(false)
    , fNumeric(false)
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
        serEng<<fWhiteSpace;
        serEng<<fFinalSet;
        serEng<<fFacetsDefined;
        serEng<<fFixed;
        serEng<<(int)fType;

        serEng<<(int)fOrdered;
        serEng<<fFinite;
        serEng<<fBounded;
        serEng<<fNumeric;

        /***
         * don't serialize the fBaseValidator if it is a built-in
         ***/
        if (isBuiltInDV(fBaseValidator))
        {
            serEng<<true;
            serEng.writeString(fBaseValidator->getTypeName());
        }
        else
        {
            serEng<<false;
            storeDV(serEng, fBaseValidator);
        }

        /***
         *  Serialize RefHashTableOf<KVStringPair>
         ***/
        XTemplateSerializer::storeObject(fFacets, serEng);

        serEng.writeString(fPattern);

        if (fTypeUri==XMLUni::fgZeroLenString)
        {
            serEng<<(unsigned int)1;
        }
        else if (fTypeUri == SchemaSymbols::fgURI_SCHEMAFORSCHEMA)
        {
            serEng<<(unsigned int)2;
            serEng.writeString(fTypeLocalName);
        }
        else
        {
            serEng<<(unsigned int)3;
            serEng.writeString(fTypeLocalName);
            serEng.writeString(fTypeUri);
        }

        /***
         * don't serialize 
         *       fRegex
         ***/    
    }
    else
    {
        serEng>>fAnonymous;
        serEng>>fWhiteSpace;
        serEng>>fFinalSet;
        serEng>>fFacetsDefined;
        serEng>>fFixed;

        int type;
        serEng>>type;
        fType=(ValidatorType)type;

        serEng>>type;
        fOrdered = (XSSimpleTypeDefinition::ORDERING)type;
        serEng>>fFinite;
        serEng>>fBounded;
        serEng>>fNumeric;

        /***
         *
         *  get the basevalidator's type
         *
         ***/
        bool isBuiltInDV = false;
        serEng>>isBuiltInDV;

        if (isBuiltInDV)
        {
            XMLCh* baseTypeName;
            serEng.readString(baseTypeName);
            ArrayJanitor<XMLCh> janName(baseTypeName, fMemoryManager);

            /***
             *  Link to the fBuiltInRegistry
             *
             *  Since DatatypeValidatorFactory is always the first one
             *  to be deserialized in SchemaGrammar, we are sure that
             *  the BuiltInRegistry shall be available now.
             ***/
            fBaseValidator = DatatypeValidatorFactory::getBuiltInRegistry()->get(baseTypeName);
        }
        else
        {
            fBaseValidator = loadDV(serEng);
        }

        /***
         *
         *  Deserialize RefHashTableOf<KVStringPair>
         *
         ***/
        XTemplateSerializer::loadObject(&fFacets, 29, false, serEng);
        //XTemplateSerializer::loadObject(&fFacets, 29, true, serEng);

        serEng.readString(fPattern);

        /***
         *   Recreate through setTypeName()
         *       fTypeName
         ***/

        unsigned int flag;
        serEng>>flag;

        if ( 1 == flag )
        {
            setTypeName(0);
        }
        else if ( 2 == flag )
        {
            XMLCh* typeLocalName;
            serEng.readString(typeLocalName);
            ArrayJanitor<XMLCh> janName(typeLocalName, fMemoryManager);

            setTypeName(typeLocalName);
        }
        else //3
        {
            XMLCh* typeLocalName;
            serEng.readString(typeLocalName);
            ArrayJanitor<XMLCh> janName(typeLocalName, fMemoryManager);

            XMLCh* typeUri;
            serEng.readString(typeUri);
            ArrayJanitor<XMLCh> janUri(typeUri, fMemoryManager);

            setTypeName(typeLocalName, typeUri);

        }

        /***
         * don't serialize fRegex
         ***/
        fRegex = 0;

    }

}

/***
 *
 *  When deserialized, we need to know, exactly what
 *  validator was serialized here.
 *
 *  Design Issue:
 *    
 *    This extra type information is only necessary when
 *  we need to create and deserialize an DatatypeValidator 
 *  derivative by operator >>, but not by object.serialize().
 *  Therefore it is appropriate to save this type info by
 *  hosting object rather than by derivative.serialize().
 *
 *
 ***/
void DatatypeValidator::storeDV(XSerializeEngine&        serEng
                              , DatatypeValidator* const dv)
{
    if (dv)
    {
        serEng<<(int) dv->getType();
        serEng<<dv;
    }
    else
    {
        serEng<<(int) UnKnown;
    }

}

DatatypeValidator* DatatypeValidator::loadDV(XSerializeEngine& serEng)
{

    int type;
    serEng>>type;

    switch((ValidatorType)type)
    {
    case String: 
        StringDatatypeValidator* stringdv;
        serEng>>stringdv;
        return stringdv;        
    case AnyURI:
        AnyURIDatatypeValidator* anyuridv;
        serEng>>anyuridv;
        return anyuridv;        
    case QName: 
        QNameDatatypeValidator* qnamedv;
        serEng>>qnamedv;
        return qnamedv;        
    case Name: 
        NameDatatypeValidator* namedv;
        serEng>>namedv;
        return namedv;        
    case NCName:  
        NCNameDatatypeValidator* ncnamedv;
        serEng>>ncnamedv;
        return ncnamedv;        
    case Boolean: 
        BooleanDatatypeValidator* booleandv;
        serEng>>booleandv;
        return booleandv;        
    case Float: 
        FloatDatatypeValidator* floatdv;
        serEng>>floatdv;
        return floatdv;        
    case Double: 
        DoubleDatatypeValidator* doubledv;
        serEng>>doubledv;
        return doubledv;        
    case Decimal: 
        DecimalDatatypeValidator* decimaldv;
        serEng>>decimaldv;
        return decimaldv;        
    case HexBinary:  
        HexBinaryDatatypeValidator* hexbinarydv;
        serEng>>hexbinarydv;
        return hexbinarydv;       
    case Base64Binary: 
        Base64BinaryDatatypeValidator* base64binarydv;
        serEng>>base64binarydv;
        return base64binarydv;      
    case Duration:     
        DurationDatatypeValidator* durationdv;
        serEng>>durationdv;
        return durationdv;
    case DateTime:       
        DateTimeDatatypeValidator* datetimedv;
        serEng>>datetimedv;
        return datetimedv; 
    case Date:          
        DateDatatypeValidator* datedv;
        serEng>>datedv;
        return datedv;
    case Time:         
        TimeDatatypeValidator* timedv;
        serEng>>timedv;
        return timedv;
    case MonthDay:      
        MonthDayDatatypeValidator* monthdaydv;
        serEng>>monthdaydv;
        return monthdaydv;
    case YearMonth:     
        YearMonthDatatypeValidator* yearmonthdv;
        serEng>>yearmonthdv;
        return yearmonthdv;
    case Year:          
        YearDatatypeValidator* yeardv;
        serEng>>yeardv;
        return yeardv;
    case Month:        
        MonthDatatypeValidator* monthdv;
        serEng>>monthdv;
        return monthdv;
    case Day:           
        DayDatatypeValidator* daydv;
        serEng>>daydv;
        return daydv;
    case ID:           
        IDDatatypeValidator* iddv;
        serEng>>iddv;
        return iddv;
    case IDREF:         
        IDREFDatatypeValidator* idrefdv;
        serEng>>idrefdv;
        return idrefdv;
    case ENTITY:       
        ENTITYDatatypeValidator* entitydv;
        serEng>>entitydv;
        return entitydv;
    case NOTATION:     
        NOTATIONDatatypeValidator* notationdv;
        serEng>>notationdv;
        return notationdv;
    case List:          
        ListDatatypeValidator* listdv;
        serEng>>listdv;
        return listdv;
    case Union:         
        UnionDatatypeValidator* uniondv;
        serEng>>uniondv;
        return uniondv;
    case AnySimpleType:  
        AnySimpleTypeDatatypeValidator* anysimpletypedv;
        serEng>>anysimpletypedv;
        return anysimpletypedv;
    case UnKnown:
        return 0;
    default: //we treat this same as UnKnown
        return 0;
    }

}

inline bool 
DatatypeValidator::isBuiltInDV(DatatypeValidator* const dv)
{
    return dv? DatatypeValidatorFactory::getBuiltInRegistry()->containsKey(dv->getTypeName())
             : false;
}

/**
 * Canonical Representation
 *
 */
const XMLCh* DatatypeValidator::getCanonicalRepresentation(const XMLCh*         const rawData
                                                          ,      MemoryManager* const memMgr
                                                          ,      bool                 toValidate) const
{
    MemoryManager* toUse = memMgr? memMgr : fMemoryManager;

    if (toValidate)
    {
        DatatypeValidator *temp = (DatatypeValidator*) this;

        try
        {
            temp->validate(rawData, 0, toUse);    
        }
        catch (...)
        {
            return 0;
        }
    }

    return XMLString::replicate(rawData, toUse);
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file DatatypeValidator.cpp
  */

