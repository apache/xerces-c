/*
 * Copyright 2004,2004 The Apache Software Foundation.
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
 * $Log$
 * Revision 1.13  2004/09/28 08:54:34  amassari
 * Silence a warning about missing final "return"
 *
 * Revision 1.12  2004/09/23 21:22:47  peiyongz
 * Documentation
 * st_noContent added
 * unused parameter removed
 *
 * Revision 1.11  2004/09/13 21:24:20  peiyongz
 * 1. returned data to contain datatype in addition to value
 * 2. registry to map type name (in string) to type name enum
 *
 * Revision 1.10  2004/09/09 20:08:31  peiyongz
 * Using new error code
 *
 * Revision 1.9  2004/09/08 19:56:05  peiyongz
 * Remove parameter toValidate from validation interface
 *
 * Revision 1.8  2004/09/08 13:56:09  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.7  2004/08/31 20:52:25  peiyongz
 * Return additional double value for decimal
 * remove tz_hh/tz_mm
 *
 * Revision 1.6  2004/08/31 15:14:47  peiyongz
 * remove XSValueContext
 *
 * Revision 1.5  2004/08/24 15:59:20  peiyongz
 * using SCHAR_MIN/SCHAR_MAX
 *
 * Revision 1.4  2004/08/17 21:11:41  peiyongz
 * no more Unrepresentable
 *
 * Revision 1.3  2004/08/13 21:29:21  peiyongz
 * fMemAllocated
 *
 * Revision 1.2  2004/08/11 17:06:44  peiyongz
 * Do not panic if can't create RegEx
 *
 * $Id$
 */

#include <limits.h>
#include <errno.h>

#include <xercesc/framework/psvi/XSValue.hpp>

#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLStringTokenizer.hpp>

#include <xercesc/util/XMLBigDecimal.hpp>
#include <xercesc/util/XMLBigInteger.hpp>
#include <xercesc/util/XMLFloat.hpp>
#include <xercesc/util/XMLDouble.hpp>
#include <xercesc/util/XMLDateTime.hpp>
#include <xercesc/util/HexBin.hpp>
#include <xercesc/util/Base64.hpp>
#include <xercesc/util/XMLUri.hpp>
#include <xercesc/util/XMLChar.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/util/regx/RegularExpression.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/*** issues
 *
 *   1. For float, double, datetime family, the validation is almost similar to getActualValue
 *  
 *
 *   DataType                   DataGroup
 *                             num  dtm  str             validation           canonical      actual-value
 *   ======================================================================================================
 *    dt_string                          str              [2] Char              NA             content
 *    dt_boolean                         str           {true, false, 1, 0}   {true, false}    bool
 *    dt_decimal               num                         lexical only         yes            double
 *    dt_float                 num                         lexical/value        yes            double
 *    dt_double                num                         lexical/value        yes            double
 *    ---------------------------------------------------------------------------------------------------------
 * 5  dt_duration                   dtm                    yes                  NA             struct datetime
 *    dt_dateTime                   dtm                    yes                  yes            struct datetime
 *    dt_time                       dtm                    yes                  yes            struct datetime
 *    dt_date                       dtm                    yes                  NA             struct datetime
 *    dt_gYearMonth                 dtm                    yes                  NA             struct datetime
 *    ---------------------------------------------------------------------------------------------------------
 * 10 dt_gYear                      dtm                    yes                  NA             struct datetime
 *    dt_gMonthDay                  dtm                    yes                  NA             struct datetime
 *    dt_gDay                       dtm                    yes                  NA             struct datetime
 *    dt_gMonth                     dtm                    yes                  NA             struct datetime         
 *    dt_hexBinary                        str              decoding            ([a-f])         unsigned long ?
 *    ---------------------------------------------------------------------------------------------------------
 * 15 dt_base64Binary                     str              decoding             NA (errata?)   unsigned long ?
 *    dt_anyURI                           str              yes                  NA             content
 *    dt_QName                            str              a:b , [6]QName       NA             content
 *    dt_NOTATION                         str              [6]QName             NA             content
 *    dt_normalizedString                 str              no #xD #xA #x9       NA             content
 *    ---------------------------------------------------------------------------------------------------------
 * 20 dt_token                            str              no #xD #xA #x9 traling   NA         content
 *    dt_language                         str              language id          NA             content
 *    dt_NMTOKEN                          str              [7] Nmtoken          NA             content
 *    dt_NMTOKENS                         str              [8] Nmtokens         NA             content
 *    dt_Name                             str              [5] Name             NA             content
 *    ---------------------------------------------------------------------------------------------------------
 * 25 dt_NCName                           str              [4] NCName           NA             content
 *    dt_ID                               str              [4] NCName           NA             content
 *    dt_IDREF                            str              [4] NCName           NA             content
 *    dt_IDREFS                           str              ws seped IDREF       NA             content
 *    dt_ENTITY                           str              [4] NCName           NA             content
 *    ---------------------------------------------------------------------------------------------------------
 * 30 dt_ENTITIES                         str              ws seped ENTITY      NA             content
 *    dt_integer               num                         lexical              yes            long
 *    dt_nonPositiveInteger    num                         lexical              yes            long
 *    dt_negativeInteger       num                         lexical              yes            long
 *    dt_long                  num                         lexical              yes            long
 *    ---------------------------------------------------------------------------------------------------------
 * 35 dt_int                   num                         lexical              yes            int
 *    dt_short                 num                         lexical              yes            short
 *    dt_byte                  num                         lexical              yes            char
 *    dt_nonNegativeInteger    num                         lexical              yes            unsigned long
 *    dt_unsignedLong          num                         lexical              yes            unsigned long
 *    ---------------------------------------------------------------------------------------------------------
 * 40 dt_unsignedInt           num                         lexical              yes            unsigned int
 *    dt_unsignedShort         num                         lexical              yes            unsigned short
 *    dt_unsignedByte          num                         lexical              yes            unsigned char
 *    dt_positiveInteger       num                         lexical              yes            unsigned long
 *
 ***/

const XSValue::DataGroup XSValue::inGroup[XSValue::dt_MAXCOUNT] = 
{
    dg_strings,   dg_strings,   dg_numerics,  dg_numerics,  dg_numerics, 
    dg_datetimes, dg_datetimes, dg_datetimes, dg_datetimes, dg_datetimes,
    dg_datetimes, dg_datetimes, dg_datetimes, dg_datetimes, dg_strings, 
    dg_strings,   dg_strings,   dg_strings,   dg_strings,   dg_strings,        
    dg_strings,   dg_strings,   dg_strings,   dg_strings,   dg_strings, 
    dg_strings,   dg_strings,   dg_strings,   dg_strings,   dg_strings,        
    dg_strings,   dg_numerics,  dg_numerics,  dg_numerics,  dg_numerics, 
    dg_numerics,  dg_numerics,  dg_numerics,  dg_numerics,  dg_numerics,         
    dg_numerics,  dg_numerics,  dg_numerics,  dg_numerics
};

// ---------------------------------------------------------------------------
//  Local static functions
// ---------------------------------------------------------------------------
static XMLMutex* sXSValueMutext = 0;
static XMLRegisterCleanup XSValueMutexCleanup;

static RegularExpression* sXSValueRegEx = 0;
static XMLRegisterCleanup XSValueRegExCleanup;

RefHashTableOf<XSValue>*  XSValue::fDataTypeRegistry = 0;
static XMLRegisterCleanup XSValueRegistryCleanup;

static XMLMutex& gXSValueMutex()
{
    if (!sXSValueMutext)
    {
        XMLMutexLock lockInit(XMLPlatformUtils::fgAtomicMutex);

        if (!sXSValueMutext)
        {
            sXSValueMutext = new XMLMutex;
            XSValueRegExCleanup.registerCleanup(XSValue::reinitMutex);
        }
    }

    return *sXSValueMutext;
}

static RegularExpression* getRegEx()
{
    if (!sXSValueRegEx)
    {
	    // Lock the mutex
	    XMLMutexLock lockInit(&gXSValueMutex());

        if (!sXSValueRegEx)
		{
            try {
                sXSValueRegEx = new (XMLPlatformUtils::fgMemoryManager) 
                         RegularExpression(
                                            XMLUni::fgLangPattern
                                          , SchemaSymbols::fgRegEx_XOption
                                          , XMLPlatformUtils::fgMemoryManager
                                          );
            }
            catch (...)
            {
                return 0;
            }

            XSValueRegExCleanup.registerCleanup(XSValue::reinitRegEx);
        }
    }

    return sXSValueRegEx;
}

XSValue::DataType  XSValue::getDataType(const XMLCh* const dtString)
{

    if (!fDataTypeRegistry)
    {
	    // Lock the mutex
	    XMLMutexLock lockInit(&gXSValueMutex());

        if (!fDataTypeRegistry)
		{
            try {
                //using the XMLPlatformUtils::fgMemoryManager
                fDataTypeRegistry  = new RefHashTableOf<XSValue>(43, true, new HashXMLCh() );

                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_STRING,             new  XSValue(dt_string));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_BOOLEAN,            new  XSValue(dt_boolean));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_DECIMAL,            new  XSValue(dt_decimal));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_FLOAT,              new  XSValue(dt_float));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_DOUBLE,             new  XSValue(dt_double));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_DURATION,           new  XSValue(dt_duration));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_DATETIME,           new  XSValue(dt_dateTime));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_TIME,               new  XSValue(dt_time));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_DATE,               new  XSValue(dt_date));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_YEARMONTH,          new  XSValue(dt_gYearMonth));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_YEAR,               new  XSValue(dt_gYear));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_MONTHDAY,           new  XSValue(dt_gMonthDay));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_DAY,                new  XSValue(dt_gDay));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_MONTH,              new  XSValue(dt_gMonth));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_HEXBINARY,          new  XSValue(dt_hexBinary));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_BASE64BINARY,       new  XSValue(dt_base64Binary));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_ANYURI,             new  XSValue(dt_anyURI));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_QNAME,              new  XSValue(dt_QName));
                fDataTypeRegistry->put((void*) XMLUni::fgNotationString,               new  XSValue(dt_NOTATION));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_NORMALIZEDSTRING,   new  XSValue(dt_normalizedString));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_TOKEN,              new  XSValue(dt_token));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_LANGUAGE,           new  XSValue(dt_language));
                fDataTypeRegistry->put((void*) XMLUni::fgNmTokenString,                new  XSValue(dt_NMTOKEN));
                fDataTypeRegistry->put((void*) XMLUni::fgNmTokensString,               new  XSValue(dt_NMTOKENS));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_NAME,               new  XSValue(dt_Name));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_NCNAME,             new  XSValue(dt_NCName));
                fDataTypeRegistry->put((void*) XMLUni::fgIDString,                     new  XSValue(dt_ID));
                fDataTypeRegistry->put((void*) XMLUni::fgIDRefString,                  new  XSValue(dt_IDREF));
                fDataTypeRegistry->put((void*) XMLUni::fgIDRefsString,                 new  XSValue(dt_IDREFS));
                fDataTypeRegistry->put((void*) XMLUni::fgEntityString,                 new  XSValue(dt_ENTITY));
                fDataTypeRegistry->put((void*) XMLUni::fgEntitiesString,               new  XSValue(dt_ENTITIES));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_INTEGER,            new  XSValue(dt_integer));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_NONPOSITIVEINTEGER, new  XSValue(dt_nonPositiveInteger));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_NEGATIVEINTEGER,    new  XSValue(dt_negativeInteger));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_LONG,               new  XSValue(dt_long));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_INT,                new  XSValue(dt_int));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_SHORT,              new  XSValue(dt_short));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_BYTE,               new  XSValue(dt_byte));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_NONNEGATIVEINTEGER, new  XSValue(dt_nonNegativeInteger));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_ULONG,              new  XSValue(dt_unsignedLong));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_UINT,               new  XSValue(dt_unsignedInt));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_USHORT,             new  XSValue(dt_unsignedShort));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_UBYTE,              new  XSValue(dt_unsignedByte));
                fDataTypeRegistry->put((void*) SchemaSymbols::fgDT_POSITIVEINTEGER,    new  XSValue(dt_positiveInteger));

            }
            catch (...)
            {
                return dt_MAXCOUNT;
            }

            XSValueRegistryCleanup.registerCleanup(XSValue::reinitRegistry);
        }
    }

    XSValue* data = fDataTypeRegistry->get(dtString);
    return data? data->fData.f_datatype : dt_MAXCOUNT;

}

inline
static bool checkTimeZoneError(XSValue::DataType       const &datatype
                             , SchemaDateTimeException const &e       )
{
    return (((datatype == XSValue::dt_dateTime) || (datatype == XSValue::dt_time)) &&
            ((e.getCode() == XMLExcepts::DateTime_tz_noUTCsign)   ||
             (e.getCode() == XMLExcepts::DateTime_tz_stuffAfterZ) ||
             (e.getCode() == XMLExcepts::DateTime_tz_invalid)     ||
             (e.getCode() == XMLExcepts::DateTime_tz_hh_invalid)));
}

// ---------------------------------------------------------------------------
//  Local Data
// ---------------------------------------------------------------------------
          
static const XMLCh Separator_20[] = {chSpace, chNull};
static const XMLCh Separator_ws[] = {chSpace, chLF, chCR, chHTab, chNull};

static const int convert_2_long  = 1;
static const int convert_2_ulong = 2;
static const int base_decimal    = 10;

// ---------------------------------------------------------------------------
//  XSValue: Constructors and Destructor
// ---------------------------------------------------------------------------
XSValue::XSValue(DataType        const dt
               , MemoryManager*  const manager)
:fMemAllocated(false)
,fMemoryManager(manager)
{
    fData.f_datatype = dt;
}

XSValue::~XSValue()
{
    if (fMemAllocated)
        fMemoryManager->deallocate(fData.fValue.f_strVal);
}
  
// ---------------------------------------------------------------------------
//  XSValue: Public Interface
//
//    No exception is thrown from these methods
//
// ---------------------------------------------------------------------------
bool XSValue::validate(const XMLCh*         const content    
                     ,       DataType             datatype
                     ,       Status&              status
                     ,       XMLVersion           version
                     ,       MemoryManager* const manager)
{

    if (!content || !*content)
    {
        status=st_NoContent;
        return false;
    }

    switch (inGroup[datatype])
    {
    case XSValue::dg_numerics :
        return validateNumerics(content, datatype, status, version, manager);
        break;
    case XSValue::dg_datetimes:
        return validateDateTimes(content, datatype, status, version, manager);
        break;
    case XSValue::dg_strings:
        return validateStrings(content, datatype, status, version, manager);
        break;
    default:
        status = st_UnknownType;
        return false;
        break;
    }
 
}

XMLCh* 
XSValue::getCanonicalRepresentation(const XMLCh*         const content    
                                  ,       DataType             datatype
                                  ,       Status&              status
                                  ,       XMLVersion           version
                                  ,       bool                 toValidate
                                  ,       MemoryManager* const manager)
{
 
    if (!content || !*content)
    {
        status=st_NoContent;
        return 0;
    }
    
    switch (inGroup[datatype])
    {
    case XSValue::dg_numerics :
        return getCanRepNumerics(content, datatype,  status, version, toValidate, manager);
        break;
    case XSValue::dg_datetimes:
        return getCanRepDateTimes(content, datatype,  status, version, toValidate, manager);
        break;
    case XSValue::dg_strings:
        return getCanRepStrings(content, datatype,  status, version, toValidate, manager);
        break;
    default:
        status = st_UnknownType;
        return 0;
        break;
    }

}

XSValue* XSValue::getActualValue(const XMLCh*         const content    
                               ,       DataType             datatype
                               ,       Status&              status
                               ,       XMLVersion           version
                               ,       bool                 toValidate
                               ,       MemoryManager* const manager)
{

    if (!content || !*content)
    {
        status=st_NoContent;
        return 0;
    }


    switch (inGroup[datatype])
    {
    case XSValue::dg_numerics :
        return getActValNumerics(content, datatype,  status, version, manager);
        break;
    case XSValue::dg_datetimes:
        return getActValDateTimes(content, datatype,  status, version, manager);
        break;
    case XSValue::dg_strings:
        return getActValStrings(content, datatype,  status, version, toValidate, manager);
        break;
    default:
        status = st_UnknownType;
        return 0;
        break;
    }
    
}

// ---------------------------------------------------------------------------
//  XSValue: Helpers
// --------------------------------------------------------------------------- 

/***
 *
 *  Boundary checking is done against Schema Type's lexcial space
 ***/
bool 
XSValue::validateNumerics(const XMLCh*         const content    
                        ,       DataType             datatype
                        ,       Status&              status
                        ,       XMLVersion           version
                        ,       MemoryManager* const manager)
{

    try {

        if (datatype == XSValue::dt_decimal)
        {
            XMLBigDecimal::parseDecimal(content, manager); 
            //error: no
        }
        else if (datatype == XSValue::dt_float)
        {
            //XMLFloat takes care of 0, -0, -INF, INF and NaN
            //XMLFloat::checkBoundary() handles error and outofbound issues
            XMLFloat data(content, manager);
            if (data.isDataConverted())
            {
                status = st_InvalidRange;
                return false;
            }
        }
        else if (datatype == XSValue::dt_double)
        {
            //XMLDouble takes care of 0, -0, -INF, INF and NaN
            //XMLDouble::checkBoundary() handles error and outofbound issues
            XMLDouble  data(content, manager);
            if (data.isDataConverted())
            {
                status = st_InvalidRange;
                return false;
            }
        }
        /***
         *   For all potentially unrepresentable types
         *   
         *   For dt_long, dt_unsignedLong, doing lexical space
         *   checking ensures consistent behaviour on 32/64 boxes
         *
         ***/
        else if (datatype == XSValue::dt_integer            ||
                 datatype == XSValue::dt_negativeInteger    ||
                 datatype == XSValue::dt_nonPositiveInteger ||
                 datatype == XSValue::dt_nonNegativeInteger ||
                 datatype == XSValue::dt_positiveInteger    ||
                 datatype == XSValue::dt_long               ||
                 datatype == XSValue::dt_unsignedLong        )
        {
            XMLCh* compareData = (XMLCh*) manager->allocate((XMLString::stringLen(content) + 1) * sizeof(XMLCh));
            ArrayJanitor<XMLCh> janName(compareData, manager);
            int    signValue = 0;
            XMLBigInteger::parseBigInteger(content, compareData, signValue,  manager);

            switch (datatype)
            { 
            case XSValue::dt_integer:
                //error: no
                break;
            case XSValue::dt_negativeInteger:
                // error: > -1
                {
                    if  (XMLBigInteger::compareValues(compareData
                                                    , signValue
                                                    , &(XMLUni::fgNegOne[1])
                                                    , -1
                                                    , manager) 
                                                    == XMLNumber::GREATER_THAN)
                    {
                        status = st_InvalidRange;
                        return false;
                    }
                }
                break;
            case XSValue::dt_nonPositiveInteger:
                // error: > 0
                {
                    if (XMLBigInteger::compareValues(compareData
                                                   , signValue
                                                   , XMLUni::fgValueZero
                                                   , 0
                                                   , manager) 
                                                   == XMLNumber::GREATER_THAN)
                    {
                        status = st_InvalidRange;
                        return false;
                    }
                }
                break;
            case XSValue::dt_nonNegativeInteger:
                // error: < 0
                {
                    if (XMLBigInteger::compareValues(compareData
                                                   , signValue
                                                   , XMLUni::fgValueZero
                                                   , 0
                                                   , manager) 
                                                   == XMLNumber::LESS_THAN)
                    {
                        status = st_InvalidRange;
                        return false;
                    }
                }
                break;
            case XSValue::dt_positiveInteger:
                // error: < 1
                {
                    if (XMLBigInteger::compareValues(compareData
                                                   , signValue
                                                   , XMLUni::fgValueOne
                                                   , 1
                                                   , manager) 
                                                   == XMLNumber::LESS_THAN)
                    {
                        status = st_InvalidRange;
                        return false;
                    }
                }
                break;
            case XSValue::dt_long:
                // error: < -9223372036854775808 || > 9223372036854775807
                {
                    if ((XMLBigInteger::compareValues(compareData
                                                    , signValue
                                                    , &(XMLUni::fgLongMinInc[1])
                                                    , -1
                                                    , manager) 
                                                    == XMLNumber::LESS_THAN) ||
                        (XMLBigInteger::compareValues(compareData
                                                    , signValue
                                                    , XMLUni::fgLongMaxInc
                                                    , 1
                                                    , manager) 
                                                    == XMLNumber::GREATER_THAN))
                    {
                        status = st_InvalidRange;
                        return false;
                    }
                }
                break;
            case XSValue::dt_unsignedLong:
                // error: < 0 || > 18446744073709551615 
                {
                    if ((XMLBigInteger::compareValues(compareData
                                                    , signValue
                                                    , XMLUni::fgValueZero
                                                    , 0
                                                    , manager) 
                                                    == XMLNumber::LESS_THAN) ||
                        (XMLBigInteger::compareValues(compareData
                                                    , signValue
                                                    , XMLUni::fgULongMaxInc
                                                    , 1
                                                    , manager) 
                                                    == XMLNumber::GREATER_THAN))
                    {
                        status = st_InvalidRange;
                        return false;
                    }
                }
                break;
            default:
                return false;
                break;
            }

        }
        /***
         *   For all singed integer types
         ***/
        else if (datatype == XSValue::dt_int   ||
                 datatype == XSValue::dt_short ||
                 datatype == XSValue::dt_byte   )
        {
            t_value   actVal;

            if ( !getActualValue( 
                                  content
                                , status
                                , version
                                , convert_2_long
                                , actVal
                                , base_decimal
                                , manager
                                 )
                )
                return false;

            switch (datatype)
            { 
            case XSValue::dt_int:
                // error: < INT_MIN || > INT_MAX
                // strtol() won't overflow for INT_MIN-1 or INT_MAX+1 on 64 box
                {
                    if ((actVal.f_long < INT_MIN) || 
                        (actVal.f_long > INT_MAX)  )
                    {
                        status = st_InvalidRange;
                        return false;
                    }
                }
                break;
            case XSValue::dt_short:
                // error: < SHRT_MIN || > SHRT_MAX
                {
                    if ((actVal.f_long < SHRT_MIN) || 
                        (actVal.f_long > SHRT_MAX)  )
                    {
                        status = st_InvalidRange;
                        return false;
                    }
                }
                break;
            case XSValue::dt_byte:
                // error: < SCHAR_MIN || > SCHAR_MAX
                {
                    if ((actVal.f_long < SCHAR_MIN) || 
                        (actVal.f_long > SCHAR_MAX)  )
                    {
                        status = st_InvalidRange;
                        return false;
                    }
                }
                break;
            default:
                return false;
                break;
            } 
        }
        /***
         *   For all unsinged integer types
         ***/
        else
        {
            t_value   actVal;

            if ( !getActualValue( 
                                  content
                                , status
                                , version
                                , convert_2_ulong
                                , actVal
                                , base_decimal
                                , manager
                                 )
                )
                return false;

            switch (datatype)
            { 
            case XSValue::dt_unsignedInt:     
                // error: > UINT_MAX
                // strtol() won't overflow for UINT_MAX+1 on 64 box
                if (actVal.f_long > UINT_MAX) 
                {
                    status = st_InvalidRange;
                    return false;
                }
                break;
            case XSValue::dt_unsignedShort:
                // error: > USHRT_MAX
                if (actVal.f_ulong > USHRT_MAX) 
                {
                    status = st_InvalidRange;
                    return false;
                }
                break;
            case XSValue::dt_unsignedByte:   
                // error: > UCHAR_MAX
                if (actVal.f_ulong > UCHAR_MAX) 
                {
                    status = st_InvalidRange;
                    return false;
                }
                break;
            default:
                return false;
                break;
            } 
        }

        return true;  //both valid chars and within boundary
    }

    catch (...)
    {
        //getActValue()/getCanonical() need to know the failure details
        //if validation is required
        status = st_FOCA0002; 
        return false; 
    }

}

bool XSValue::validateDateTimes(const XMLCh*         const content    
                              ,       DataType             datatype
                              ,       Status&              status
                              ,       XMLVersion           version
                              ,       MemoryManager* const manager)
{

    try
    {
        XMLDateTime coreDate = XMLDateTime(content, manager);

        switch (datatype)
        { 
        case XSValue::dt_duration:
            coreDate.parseDuration();
            break;
        case XSValue::dt_dateTime:
            coreDate.parseDateTime();
            break;
        case XSValue::dt_time:
            coreDate.parseTime();
            break;
        case XSValue::dt_date:
            coreDate.parseDate();
            break;
        case XSValue::dt_gYearMonth:
            coreDate.parseYearMonth();
            break;
        case XSValue::dt_gYear:
            coreDate.parseYear();
            break;
        case XSValue::dt_gMonthDay:
            coreDate.parseMonthDay();
            break;
        case XSValue::dt_gDay:
            coreDate.parseDay();
            break;
        case XSValue::dt_gMonth:
            coreDate.parseMonth();
            break;
        default:
            return false;
            break;
        }

        return true; //parsing succeed
    }

    catch (SchemaDateTimeException &e)
    {       
        status = checkTimeZoneError(datatype, e)? XSValue::st_FODT0003 : st_FOCA0002;
        return false;
    }

    catch (...)
    {
        //getActValue()/getCanonical() need to know the failure details
        //if validation is required
        status = st_FOCA0002;
        return false; 
    }      

}

bool XSValue::validateStrings(const XMLCh*         const content    
                            ,       DataType             datatype
                            ,       Status&              status
                            ,       XMLVersion           version
                            ,       MemoryManager* const manager)
{

    try
    {
        switch (datatype)
        { 
        case XSValue::dt_boolean:
            {
                unsigned int i = 0;
                for ( ; i < XMLUni::fgBooleanValueSpaceArraySize; i++ )
                {
                    if ( XMLString::equals(content, XMLUni::fgBooleanValueSpace[i]))
                        break;
                }

                if (i == XMLUni::fgBooleanValueSpaceArraySize)
                {
                    status = st_FOCA0002;
                    return false;
                }
            }
            break;
        case XSValue::dt_hexBinary:
            {
                if (HexBin::getDataLength(content) == -1) 
                {
                    status = st_FOCA0002;
                    return false;
                }
            }
            break;
        case XSValue::dt_base64Binary:
            {
                if (Base64::getDataLength(content, manager) == -1) 
                {
                    status = st_FOCA0002;
                    return false;
                }
            }
            break;
        case XSValue::dt_anyURI:
            return XMLUri::isValidURI(true, content);
            break;
        case XSValue::dt_QName:
            return (version == ver_10) ? 
                XMLChar1_0::isValidQName(content, XMLString::stringLen(content)) :
                XMLChar1_1::isValidQName(content, XMLString::stringLen(content));
            break;
        case XSValue::dt_NOTATION:
            return XMLString::isValidNOTATION(content, manager);
            break;
        case XSValue::dt_string:
            {
                XMLCh*   rawPtr = (XMLCh*) content;

                if (version == ver_10)
                {
                    while (*rawPtr) 
                        if (!XMLChar1_0::isXMLChar(*rawPtr++)) 
                            return false;
                }
                else
                {
                    while (*rawPtr) 
                        if (!XMLChar1_1::isXMLChar(*rawPtr++)) 
                            return false;
                }

            }
            break;
        case XSValue::dt_normalizedString:
            {
                XMLCh*   rawPtr = (XMLCh*) content;

                if (version == ver_10)
                {
                    while (*rawPtr) 
                    {
                        if (!XMLChar1_0::isXMLChar(*rawPtr)) 
                            return false;

                        if (*rawPtr == chCR || *rawPtr == chLF || *rawPtr == chHTab )
                            return false;

                        rawPtr++;
                    }
                }
                else
                {
                    while (*rawPtr) 
                    {
                        if (!XMLChar1_1::isXMLChar(*rawPtr)) return false;

                        if (*rawPtr == chCR || *rawPtr == chLF || *rawPtr == chHTab )
                            return false;

                        rawPtr++;
                    }
                }

            }
            break;
        case XSValue::dt_token:
        case XSValue::dt_language:
            {
                unsigned int strLen = XMLString::stringLen(content);
                XMLCh*   rawPtr = (XMLCh*) content;
                bool     inWS = false;

                if (version == ver_10)
                {

                    // Check leading/Trailing white space
                    if (XMLChar1_0::isWhitespace(content[0])      ||
                        XMLChar1_0::isWhitespace(content[strLen])  )
                        return false;

                    while (*rawPtr) 
                    {
                        if (!XMLChar1_0::isXMLChar(*rawPtr)) 
                            return false;

                        if (*rawPtr == chCR || *rawPtr == chLF || *rawPtr == chHTab )
                            return false;

                        if (XMLChar1_0::isWhitespace(*rawPtr))
                        {
                            if (inWS)
                                return false;
                            else
                                inWS = true;
                        }
                        else
                            inWS = false;

                        rawPtr++;
                    }
                }
                else
                {

                    // Check leading/Trailing white space
                    if (XMLChar1_1::isWhitespace(content[0])      ||
                        XMLChar1_1::isWhitespace(content[strLen])  )
                        return false;

                    while (*rawPtr) 
                    {
                        if (!XMLChar1_1::isXMLChar(*rawPtr)) 
                            return false;

                        if (*rawPtr == chCR || *rawPtr == chLF || *rawPtr == chHTab )
                            return false;

                        if (XMLChar1_1::isWhitespace(*rawPtr))
                        {
                            if (inWS)
                                return false;
                            else
                                inWS = true;
                        }
                        else
                            inWS = false;

                        rawPtr++;
                    }
                }

            }

            if (datatype == XSValue::dt_language)
            {
                RegularExpression* regEx = getRegEx();
                if (!regEx)
                {
                    status = st_CantCreateRegEx;
                    return false;
                }

                return regEx->matches(content, manager);
            }

            break;
        case XSValue::dt_NMTOKEN:
            return (version == ver_10) ? 
                XMLChar1_0::isValidNmtoken(content, XMLString::stringLen(content)) :
                XMLChar1_1::isValidNmtoken(content, XMLString::stringLen(content));
            break;
        case XSValue::dt_NMTOKENS:
            // [8]    Nmtokens   ::=    Nmtoken (#x20 Nmtoken)*
            {
                XMLStringTokenizer tokenizer(content, Separator_20, manager);

                if (version ==  ver_10 )
                {
                    while (tokenizer.hasMoreTokens()) 
                    {
                        const XMLCh* token = tokenizer.nextToken();

                        if (!XMLChar1_0::isValidNmtoken(token, XMLString::stringLen(token)))
                            return false;
                    }
                }
                else
                {
                    while (tokenizer.hasMoreTokens()) 
                    {
                        const XMLCh* token = tokenizer.nextToken();

                        if (!XMLChar1_1::isValidNmtoken(token, XMLString::stringLen(token)))
                            return false;
                    }
                }
            }
            break;
        case XSValue::dt_Name:
            return (version == ver_10) ? 
                XMLChar1_0::isValidName(content, XMLString::stringLen(content)) :
                XMLChar1_1::isValidName(content, XMLString::stringLen(content));
            break;
        case XSValue::dt_NCName:
        case XSValue::dt_ID:
        case XSValue::dt_IDREF:
        case XSValue::dt_ENTITY:
            return (version == ver_10) ? 
                XMLChar1_0::isValidNCName(content, XMLString::stringLen(content)) :
                XMLChar1_1::isValidNCName(content, XMLString::stringLen(content));
            break;
        case XSValue::dt_ENTITIES:
        case XSValue::dt_IDREFS:
            {
                XMLStringTokenizer tokenizer(content, Separator_ws, manager);

                if (version ==  ver_10 )
                {
                    while (tokenizer.hasMoreTokens()) 
                    {
                        const XMLCh* token = tokenizer.nextToken();

                        if (!XMLChar1_0::isValidNCName(token, XMLString::stringLen(token)))
                            return false;
                    }
                }
                else
                {
                    while (tokenizer.hasMoreTokens()) 
                    {
                        const XMLCh* token = tokenizer.nextToken();

                        if (!XMLChar1_1::isValidNCName(token, XMLString::stringLen(token)))
                            return false;
                    }
                }
            }
            break;
        default:
            return false;
            break;
        }

        return true; //parsing succeed
    }

    catch (...)
    {
        status = st_FOCA0002;
        return false; 
    }

}


XMLCh* XSValue::getCanRepNumerics(const XMLCh*         const content    
                                ,       DataType             datatype
                                ,       Status&              status
                                ,       XMLVersion           version
                                ,       bool                 toValidate
                                ,       MemoryManager* const manager)
{
    try 
    {

        // All getCanonicalRepresentation does lexcial space validation only
        // (no range checking), therefore if validation is requied,
        // we need to pass the content to the validate interface for complete checking
        if (toValidate && !validateNumerics(content, datatype, status, version, manager))
            return 0;

        XMLCh* retVal;

        if (datatype == XSValue::dt_decimal)
        {
            retVal = XMLBigDecimal::getCanonicalRepresentation(content, manager);

            if (!retVal)
                status = st_FOCA0002;

            return retVal;

        }
        else if (datatype == XSValue::dt_float || datatype == XSValue::dt_double  )
        {
            retVal = XMLAbstractDoubleFloat::getCanonicalRepresentation(content, manager);

            if (!retVal)
                status = st_FOCA0002;

            return retVal;
        }  
        else 
        {
            retVal = XMLBigInteger::getCanonicalRepresentation(content, manager, datatype == XSValue::dt_nonPositiveInteger);

            if (!retVal)
                status = st_FOCA0002;

            return retVal;
        }
    }

    catch (...)
    {
        status = st_FOCA0002;
        return 0;
    }

}

XMLCh* XSValue::getCanRepDateTimes(const XMLCh*         const content    
                                 ,       DataType             datatype
                                 ,       Status&              status
                                 ,       XMLVersion           version
                                 ,       bool                 toValidate
                                 ,       MemoryManager* const manager)
{

    try
    {

        XMLDateTime coreDate = XMLDateTime(content, manager);

        switch (datatype)
        { 
        case XSValue::dt_dateTime:
            //we need this parsing
            coreDate.parseDateTime();
            return coreDate.getDateTimeCanonicalRepresentation(manager);
            break;
        case XSValue::dt_time:
            // we need this parsing
            coreDate.parseTime(); 
            return coreDate.getTimeCanonicalRepresentation(manager);
            break;
        case XSValue::dt_duration:
        case XSValue::dt_date:
        case XSValue::dt_gYearMonth:
        case XSValue::dt_gYear:
        case XSValue::dt_gMonthDay:
        case XSValue::dt_gDay:
        case XSValue::dt_gMonth:
            {
                if (toValidate && !validateDateTimes(content, datatype, status, version, manager))
                    status = st_FOCA0002;
                else
                    status = st_NoCanRep;

                return 0;
            }
            break;
        default:
            return 0;
            break;
        }
    }

    catch (SchemaDateTimeException &e)
    {
        status = checkTimeZoneError(datatype, e)? XSValue::st_FODT0003 : st_FOCA0002;
        return 0;
    }

    catch (...)
    {
        status = st_FOCA0002;
        return 0;
    }
    return 0;
}

XMLCh* XSValue::getCanRepStrings(const XMLCh*         const content    
                               ,       DataType             datatype
                               ,       Status&              status
                               ,       XMLVersion           version
                               ,       bool                 toValidate
                               ,       MemoryManager* const manager)
{
    try
    {
        switch (datatype)
        { 
        case XSValue::dt_boolean:
            {
                //always validate before getting canRep
                if (XMLString::equals(content, XMLUni::fgBooleanValueSpace[0]) ||
                    XMLString::equals(content, XMLUni::fgBooleanValueSpace[2])  ) 
                {
                    return XMLString::replicate(XMLUni::fgBooleanValueSpace[0], manager);
                }
                else if (XMLString::equals(content, XMLUni::fgBooleanValueSpace[1]) ||
                         XMLString::equals(content, XMLUni::fgBooleanValueSpace[3])  ) 
                {
                    return XMLString::replicate(XMLUni::fgBooleanValueSpace[1], manager);
                }
                else
                {
                    status = st_FOCA0002;
                    return 0;
                }
            }
            break;
        case XSValue::dt_hexBinary: 
            {
                //HexBin::getCanonicalRepresentation does validation automatically
                XMLCh* canRep = HexBin::getCanonicalRepresentation(content, manager);
                if (!canRep)
                    status = st_FOCA0002;

                return canRep;
            }
            break;
        case XSValue::dt_base64Binary:
            {
                //Base64::getCanonicalRepresentation does validation automatically
                XMLCh* canRep = Base64::getCanonicalRepresentation(content, manager);
                if (!canRep)
                    status = st_FOCA0002;

                return canRep;
            }
            break;
        case XSValue::dt_anyURI:
        case XSValue::dt_QName:
        case XSValue::dt_NOTATION:
        case XSValue::dt_string:
        case XSValue::dt_normalizedString:
        case XSValue::dt_token:
        case XSValue::dt_language:
        case XSValue::dt_NMTOKEN:
        case XSValue::dt_NMTOKENS:
        case XSValue::dt_Name:
        case XSValue::dt_NCName:
        case XSValue::dt_ID:
        case XSValue::dt_IDREF:
        case XSValue::dt_ENTITY:
        case XSValue::dt_ENTITIES:
        case XSValue::dt_IDREFS:
            {
                if (toValidate && !validateStrings(content, datatype, status, version, manager))
                    status = st_FOCA0002;
                else
                    status = st_NoCanRep;

                return 0;
            }
            break;
        default:
            return 0;
            break;
        }
    }

    catch (...)
    {
        status = st_FOCA0002;
        return 0;
    }
    return 0;
}

XSValue*
XSValue::getActValNumerics(const XMLCh*         const content    
                         ,       DataType             datatype
                         ,       Status&              status
                         ,       XMLVersion           version
                         ,       MemoryManager* const manager)
{

    try {

        if (datatype == XSValue::dt_decimal)
        {
            //Prepare for the quadruplet
            XMLBigDecimal data(content, manager); 

            int      totalDigit = data.getTotalDigit();
            int      scale      = data.getScale();
            XMLCh*   intVal     = data.getIntVal();

            // get the fraction
            t_value  actValFract;

            if ( !getActualValue( 
                                  &(intVal[totalDigit - scale])
                                , status
                                , version
                                , convert_2_ulong
                                , actValFract
                                , base_decimal
                                , manager
                                )
                )
            {
                if (status != st_FOCA0002)
                    status = st_FOCA0001;

                return 0;
            }

            // get the integer
            t_value  actValInt;
            intVal[totalDigit - scale] = 0;

            if ( !getActualValue( 
                                  intVal
                                , status
                                , version
                                , convert_2_ulong
                                , actValInt
                                , base_decimal
                                , manager
                                )
                )
            {
                if (status != st_FOCA0002)
                    status = st_FOCA0001;

                return 0;
            }

            //Prepare the double value
            XMLDouble  data2(content, manager);
            if (data2.isDataConverted())
            {
                status = data2.isDataOverflowed()? st_FOCA0001 : st_InvalidRange;
                return 0;
            }

            XSValue* retVal = new (manager) XSValue(dt_decimal, manager);

            retVal->fData.fValue.f_decimal.f_sign     = data.getSign();
            retVal->fData.fValue.f_decimal.f_scale    = data.getScale();
            retVal->fData.fValue.f_decimal.f_fraction = actValFract.f_ulong;
            retVal->fData.fValue.f_decimal.f_integral = actValInt.f_ulong;

            retVal->fData.fValue.f_decimal.f_dvalue   = data2.getValue();

            return retVal;
        }
        else if (datatype == XSValue::dt_float)
        {
            //XMLFloat takes care of 0, -0, -INF, INF and NaN
            //XMLFloat::checkBoundary() handles error and outofbound issues
            XMLFloat data(content, manager);
            if (data.isDataConverted())
            {
                status = st_InvalidRange;
                return 0;
            }
            else
            {
                XSValue* retVal = new (manager) XSValue(dt_float, manager);
                retVal->fData.fValue.f_float = (float) data.getValue();
                return retVal;
            }
        }
        else if (datatype == XSValue::dt_double)
        {
            //XMLDouble takes care of 0, -0, -INF, INF and NaN
            //XMLDouble::checkBoundary() handles error and outofbound issues
            XMLDouble  data(content, manager);
            if (data.isDataConverted())
            {
                status = st_InvalidRange;
                return 0;
            }
            else
            {
                XSValue* retVal = new (manager) XSValue(dt_double, manager);
                retVal->fData.fValue.f_double = data.getValue();
                return retVal;
            }
        }
        else if (datatype == XSValue::dt_integer            ||
                 datatype == XSValue::dt_negativeInteger    ||
                 datatype == XSValue::dt_nonPositiveInteger ||
                 datatype == XSValue::dt_nonNegativeInteger ||
                 datatype == XSValue::dt_positiveInteger      )
        {
            t_value   actVal;                

            if ( !getActualValue( 
                                  content
                                , status
                                , version
                                , convert_2_long
                                , actVal
                                , base_decimal
                                , manager
                                )
                )
            {
                if (status != st_FOCA0002)
                    status = st_FOCA0003;

                return 0;
            }

            switch (datatype)
            { 
            case XSValue::dt_integer:
                // error: no
                {
                    XSValue* retVal = new (manager) XSValue(dt_integer, manager);
                    retVal->fData.fValue.f_long = actVal.f_long;
                    return retVal;
                }
                break;
            case XSValue::dt_negativeInteger:
                // error: > -1
                {
                    if (actVal.f_long > -1)
                        return 0;

                    XSValue* retVal = new (manager) XSValue(dt_negativeInteger, manager);
                    retVal->fData.fValue.f_long = actVal.f_long;
                    return retVal;
                }
                break;
            case XSValue::dt_nonPositiveInteger:
                // error: > 0
                {
                    if (actVal.f_long > 0)
                        return 0;

                    XSValue* retVal = new (manager) XSValue(dt_nonPositiveInteger, manager);
                    retVal->fData.fValue.f_long = actVal.f_long;
                    return retVal;
                }
                break;
            case XSValue::dt_nonNegativeInteger:
                // error: < 0
                {
                    if (actVal.f_long < 0)
                        return 0;

                    XSValue* retVal = new (manager) XSValue(dt_nonNegativeInteger, manager);
                    retVal->fData.fValue.f_long = actVal.f_long;
                    return retVal;
                }
                break;
            case XSValue::dt_positiveInteger:
                // error: < 1
                {
                    if (actVal.f_long < 1)
                        return 0;

                    XSValue* retVal = new (manager) XSValue(dt_positiveInteger, manager);
                    retVal->fData.fValue.f_long = actVal.f_long;
                    return retVal;
                }
                break;
            default:
                return 0;
                break;
            }
        }
        else if (datatype == XSValue::dt_long  ||
                 datatype == XSValue::dt_int   ||
                 datatype == XSValue::dt_short ||
                 datatype == XSValue::dt_byte   )
        {
            t_value   actVal;                

            if ( !getActualValue( 
                                  content
                                , status
                                , version
                                , convert_2_long
                                , actVal
                                , base_decimal
                                , manager
                                 )
               )
               return 0;

            switch (datatype)
            { 
            case XSValue::dt_long:
                // error : no
                {
                    XSValue* retVal = new (manager) XSValue(dt_long, manager);
                    retVal->fData.fValue.f_long = actVal.f_long;
                    return retVal;
                }
                break;
            case XSValue::dt_int:
                // error: < INT_MIN || > INT_MAX
                // strtol() won't overflow for INT_MIN-1 or INT_MAX+1 on 64 box
                {
                    if ((actVal.f_long < INT_MIN) || 
                        (actVal.f_long > INT_MAX)  )
                    {
                        status = st_InvalidRange;
                        return 0;
                    }

                    XSValue* retVal = new (manager) XSValue(dt_int, manager);
                    retVal->fData.fValue.f_int = (int) actVal.f_long;
                    return retVal;
                }
                break;
            case XSValue::dt_short:
                // error: < SHRT_MIN || > SHRT_MAX
                {
                    if ((actVal.f_long < SHRT_MIN) || (actVal.f_long > SHRT_MAX))
                    {
                        status = st_InvalidRange;
                        return 0;
                    }

                    XSValue* retVal = new (manager) XSValue(dt_short, manager);
                    retVal->fData.fValue.f_short = (short) actVal.f_long;
                    return retVal;
                }
                break;
            case XSValue::dt_byte:
                // error: < SCHAR_MIN || > SCHAR_MAX
                {
                    if ((actVal.f_long < SCHAR_MIN) || (actVal.f_long > SCHAR_MAX))
                    {
                        status = st_InvalidRange;
                        return 0;
                    }

                    XSValue* retVal = new (manager) XSValue(dt_byte, manager);
                    retVal->fData.fValue.f_char = (char) actVal.f_long;
                    return retVal;
                }
                break;
            default:
                return 0;
                break;
            } 
        }
        else
        {

            t_value   actVal;                

            if ( !getActualValue( 
                                  content
                                , status
                                , version
                                , convert_2_ulong
                                , actVal
                                , base_decimal
                                , manager
                                 )
               )
               return 0;

            switch (datatype)
            { 
            case XSValue::dt_unsignedLong:
                // error: no
                {
                    XSValue* retVal = new (manager) XSValue(dt_unsignedLong, manager);
                    retVal->fData.fValue.f_ulong = actVal.f_ulong;
                    return retVal;
                }
                break;
            case XSValue::dt_unsignedInt:     
                // error: > UINT_MAX
                // strtol() won't overflow for UINT_MAX+1 on 64 box
                {

                    if (actVal.f_long > UINT_MAX) 
                    {
                        status = st_InvalidRange;
                        return 0;
                    }

                    XSValue* retVal = new (manager) XSValue(dt_unsignedInt, manager);
                    retVal->fData.fValue.f_uint = (unsigned int) actVal.f_ulong;
                    return retVal;
                }
                break;
            case XSValue::dt_unsignedShort:
                // error: > USHRT_MAX
                {
                    if (actVal.f_ulong > USHRT_MAX)
                    {
                        status = st_InvalidRange;
                        return 0;
                    }

                    XSValue* retVal = new (manager) XSValue(dt_unsignedShort, manager);
                    retVal->fData.fValue.f_ushort = (unsigned short) actVal.f_ulong;
                    return retVal;
                }
                break;
            case XSValue::dt_unsignedByte:   
                // error: > UCHAR_MAX
                {
                    if (actVal.f_ulong > UCHAR_MAX)
                    {
                        status = st_InvalidRange;
                        return 0;
                    }

                    XSValue* retVal = new (manager) XSValue(dt_unsignedByte, manager);
                    retVal->fData.fValue.f_uchar = (unsigned char) actVal.f_ulong;
                    return retVal;
                }
                break;
            default:
                return 0;
                break;
            } 
        }
    }

    catch (...)
    {
        status = st_FOCA0002;
        return 0; 
    }
    return 0; 
}

XSValue*  
XSValue::getActValDateTimes(const XMLCh*         const content    
                          ,       DataType             datatype
                          ,       Status&              status
                          ,       XMLVersion           version
                          ,       MemoryManager* const manager)
{
    try
    {
        //Need not check if validation is requested since
        //parsing functions below does the validation automatically
        XMLDateTime coreDate = XMLDateTime(content, manager);

        switch (datatype)
        { 
        case XSValue::dt_duration:
            coreDate.parseDuration();
            break;
        case XSValue::dt_dateTime:
            coreDate.parseDateTime();
            break;
        case XSValue::dt_time:
            coreDate.parseTime();
            break;
        case XSValue::dt_date:
            coreDate.parseDate();
            break;
        case XSValue::dt_gYearMonth:
            coreDate.parseYearMonth();
            break;
        case XSValue::dt_gYear:
            coreDate.parseYear();
            break;
        case XSValue::dt_gMonthDay:
            coreDate.parseMonthDay();
            break;
        case XSValue::dt_gDay:
            coreDate.parseDay();
            break;
        case XSValue::dt_gMonth:
            coreDate.parseMonth();
            break;
        default:
            return 0;
            break;
        }

        XSValue* retVal = new (manager) XSValue(datatype, manager);

        retVal->fData.fValue.f_datetime.f_year    = coreDate.fValue[XMLDateTime::CentYear];
        retVal->fData.fValue.f_datetime.f_month   = coreDate.fValue[XMLDateTime::Month];
        retVal->fData.fValue.f_datetime.f_day     = coreDate.fValue[XMLDateTime::Day];
        retVal->fData.fValue.f_datetime.f_hour    = coreDate.fValue[XMLDateTime::Hour];
        retVal->fData.fValue.f_datetime.f_min     = coreDate.fValue[XMLDateTime::Minute];
        retVal->fData.fValue.f_datetime.f_second  = coreDate.fValue[XMLDateTime::Second];
        retVal->fData.fValue.f_datetime.f_milisec = coreDate.fMiliSecond;

        return retVal;
    }

    catch (SchemaDateTimeException const &e)
    {
        status = checkTimeZoneError(datatype, e)? XSValue::st_FODT0003 : st_FOCA0002;
        return 0;
    }

    catch (...)
    {
        status = st_FOCA0002;
        return 0; 
    }

}

XSValue*  
XSValue::getActValStrings(const XMLCh*         const content    
                        ,       DataType             datatype
                        ,       Status&              status
                        ,       XMLVersion           version
                        ,       bool                 toValidate
                        ,       MemoryManager* const manager)
{
    try
    {
        switch (datatype)
        { 
        case XSValue::dt_boolean:
            {
                //do validation here more efficiently
                if (XMLString::equals(content, XMLUni::fgBooleanValueSpace[0]) ||
                    XMLString::equals(content, XMLUni::fgBooleanValueSpace[2])  )
                {
                    XSValue* retVal = new (manager) XSValue(dt_boolean, manager);
                    retVal->fData.fValue.f_bool = true;
                    return retVal;
                }
                else if (XMLString::equals(content, XMLUni::fgBooleanValueSpace[1]) ||
                         XMLString::equals(content, XMLUni::fgBooleanValueSpace[3])  )
                {
                    XSValue* retVal = new (manager) XSValue(dt_boolean, manager);
                    retVal->fData.fValue.f_bool = false;
                    return retVal;
                }
                else
                {
                    status = st_FOCA0002;
                    return 0;
                }
            }
            break;
        case XSValue::dt_hexBinary:
            {
                XMLCh* decodedData = HexBin::decode(content, manager);

                if (!decodedData)
                {
                    status = st_FOCA0002;
                    return 0;
                }

                XSValue* retVal = new (manager) XSValue(dt_hexBinary, manager);
                retVal->fData.fValue.f_strVal = decodedData;
                retVal->fMemAllocated = true;
                return retVal;                
            }       
            break;
        case XSValue::dt_base64Binary:
            {
                unsigned int    len = 0;
                XMLCh* decodedData = Base64::decode(content, &len, manager);

                if (!decodedData)
                {
                    status = st_FOCA0002;
                    return 0;
                }

                XSValue* retVal = new (manager) XSValue(dt_base64Binary, manager);
                retVal->fData.fValue.f_strVal = decodedData;
                retVal->fMemAllocated = true;
                return retVal;
            }
            break;
        case XSValue::dt_anyURI:
        case XSValue::dt_QName:
        case XSValue::dt_NOTATION:
        case XSValue::dt_string:
        case XSValue::dt_normalizedString:
        case XSValue::dt_token:
        case XSValue::dt_language:
        case XSValue::dt_NMTOKEN:
        case XSValue::dt_NMTOKENS:
        case XSValue::dt_Name:
        case XSValue::dt_NCName:
        case XSValue::dt_ID:
        case XSValue::dt_IDREF:
        case XSValue::dt_ENTITY:
        case XSValue::dt_ENTITIES:
        case XSValue::dt_IDREFS:
            {
                if (toValidate && !validateStrings(content, datatype, status, version, manager))
                    status = st_FOCA0002;
                else
                    status = st_NoActVal;

                return 0;
            }
            break;
        default:
            return 0;
            break;
        }
    }

    catch (...)
    {
        status = st_FOCA0002;
        return 0; 
    }
    return 0; 
}

// ---------------------------------------------------------------------------
//  Utilities
// ---------------------------------------------------------------------------
bool XSValue::getActualValue(const XMLCh*         const content
                           ,       Status&              status
                           ,       XMLVersion           version    
                           ,       int                  ct
                           ,       t_value&             retVal               
                           ,       int                  base
                           ,       MemoryManager* const manager)
{
    char *nptr = XMLString::transcode(content, manager);
    ArrayJanitor<char> jan(nptr, manager);
    int   strLen = strlen(nptr);
    char *endptr = 0;
    errno = 0;

    if (ct == convert_2_long)
    {
        retVal.f_long = strtol(nptr, &endptr, base);
    }
    else if (ct == convert_2_ulong)
    {
        if (-1 != XMLString::indexOf(content, chDash))
        {
            status = st_InvalidRange;
            return false;
        }

        retVal.f_ulong = strtoul(nptr, &endptr, base);

    }

    // check if all chars are valid char
    if ( (endptr - nptr) != strLen)
    {
        status = st_FOCA0002;
        return false;
    }

    // check if overflow/underflow occurs
    if (errno == ERANGE)
    {
        status = st_InvalidRange;
        return false;
    }


    return true;
}

// -----------------------------------------------------------------------
//  Reinitialise the mutex and RegEx
// -----------------------------------------------------------------------
void XSValue::reinitMutex()
{
    delete sXSValueMutext;
    sXSValueMutext = 0;
}

void XSValue::reinitRegEx()
{
	delete sXSValueRegEx;
	sXSValueRegEx = 0;
}

void XSValue::reinitRegistry()
{
	delete fDataTypeRegistry;
	fDataTypeRegistry = 0;
}

XERCES_CPP_NAMESPACE_END


