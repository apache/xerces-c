/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2004 The Apache Software Foundation.  All rights
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
XSValue::XSValue(MemoryManager*  const manager)
:fMemAllocated(false)
,fMemoryManager(manager)
{
}

XSValue::~XSValue()
{
    if (fMemAllocated)
        fMemoryManager->deallocate(fData.f_strVal);
}
  
// ---------------------------------------------------------------------------
//  XSValue: Public Interface
//
//    No exception is thrown from these methods
//
// ---------------------------------------------------------------------------
bool XSValue::validate(const XMLCh*         const content    
                     ,       DataType             datatype
                     ,       XSValueContext&      context
                     ,       MemoryManager* const manager)
{

    if (!content || !*content)
        return false;

    switch (inGroup[datatype])
    {
    case XSValue::dg_numerics :
        return validateNumerics(content, datatype, context, manager);
        break;
    case XSValue::dg_datetimes:
        return validateDateTimes(content, datatype, context, manager);
        break;
    case XSValue::dg_strings:
        return validateStrings(content, datatype, context, manager);
        break;
    default:
        context.fStatus = XSValueContext::st_UnknownType;
        return false;
        break;
    }
 
}

XMLCh* 
XSValue::getCanonicalRepresentation(const XMLCh*         const content    
                                  ,       DataType             datatype
                                  ,       XSValueContext&      context
                                  ,       MemoryManager* const manager)
{
 
    if (!content || !*content)
        return 0;
    
    switch (inGroup[datatype])
    {
    case XSValue::dg_numerics :
        return getCanRepNumerics(content, datatype, context, manager);
        break;
    case XSValue::dg_datetimes:
        return getCanRepDateTimes(content, datatype, context, manager);
        break;
    case XSValue::dg_strings:
        return getCanRepStrings(content, datatype, context, manager);
        break;
    default:
        context.fStatus = XSValueContext::st_UnknownType;
        return 0;
        break;
    }

}

XSValue* XSValue::getActualValue(const XMLCh*         const content    
                               ,       DataType             datatype
                               ,       XSValueContext&      context
                               ,       MemoryManager* const manager)
{

    if (!content || !*content)
        return 0;

    switch (inGroup[datatype])
    {
    case XSValue::dg_numerics :
        return getActValNumerics(content, datatype, context, manager);
        break;
    case XSValue::dg_datetimes:
        return getActValDateTimes(content, datatype, context, manager);
        break;
    case XSValue::dg_strings:
        return getActValStrings(content, datatype, context, manager);
        break;
    default:
        context.fStatus = XSValueContext::st_UnknownType;
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
                        ,       XSValueContext&      context
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
                context.fStatus = XSValueContext::st_UnRepresentable;
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
                context.fStatus = XSValueContext::st_UnRepresentable;
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
            XMLCh* compareData = (XMLCh*) manager->allocate(XMLString::stringLen(content) * sizeof(XMLCh));
            ArrayJanitor<XMLCh> janName(compareData);
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
                        context.fStatus = XSValueContext::st_InvalidRange;
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
                        context.fStatus = XSValueContext::st_InvalidRange;
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
                        context.fStatus = XSValueContext::st_InvalidRange;
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
                        context.fStatus = XSValueContext::st_InvalidRange;
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
                        context.fStatus = XSValueContext::st_InvalidRange;
                        return false;
                    }
                }
                break;
            case XSValue::dt_unsignedLong:
                // error: > 18446744073709551615
                {
                    if (XMLBigInteger::compareValues(compareData
                                                   , signValue
                                                   , XMLUni::fgULongMaxInc
                                                   , 1
                                                   , manager) 
                                                   == XMLNumber::GREATER_THAN)
                    {
                        context.fStatus = XSValueContext::st_InvalidRange;
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
                                , context
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
                        context.fStatus = XSValueContext::st_InvalidRange;
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
                        context.fStatus = XSValueContext::st_InvalidRange;
                        return false;
                    }
                }
                break;
            case XSValue::dt_byte:
                // error: < CHAR_MIN || > CHAR_MAX
                {
                    if ((actVal.f_long < CHAR_MIN) || 
                        (actVal.f_long > CHAR_MAX)  )
                    {
                        context.fStatus = XSValueContext::st_InvalidRange;
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
                                , context
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
                    context.fStatus = XSValueContext::st_InvalidRange;
                    return false;
                }
                break;
            case XSValue::dt_unsignedShort:
                // error: > USHRT_MAX
                if (actVal.f_ulong > USHRT_MAX) 
                {
                    context.fStatus = XSValueContext::st_InvalidRange;
                    return false;
                }
                break;
            case XSValue::dt_unsignedByte:   
                // error: > UCHAR_MAX
                if (actVal.f_ulong > UCHAR_MAX) 
                {
                    context.fStatus = XSValueContext::st_InvalidRange;
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
        context.fStatus = XSValueContext::st_InvalidChar; 
        return false; 
    }

}

bool XSValue::validateDateTimes(const XMLCh*         const content    
                              ,       DataType             datatype
                              ,       XSValueContext&      context
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

    catch (...)
    {
        //getActValue()/getCanonical() need to know the failure details
        //if validation is required
        context.fStatus = XSValueContext::st_Invalid;
        return false; 
    }      

}

bool XSValue::validateStrings(const XMLCh*         const content    
                            ,       DataType             datatype
                            ,       XSValueContext&      context
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

                return (i == XMLUni::fgBooleanValueSpaceArraySize)? false : true;
            }
            break;
        case XSValue::dt_hexBinary:
            {
                if (HexBin::getDataLength(content) == -1) 
                {
                    context.fStatus = XSValueContext::st_Invalid;
                    return false;
                }
            }
            break;
        case XSValue::dt_base64Binary:
            {
                if (Base64::getDataLength(content, manager) == -1) 
                {
                    context.fStatus = XSValueContext::st_Invalid;
                    return false;
                }
            }
            break;
        case XSValue::dt_anyURI:
            return XMLUri::isValidURI(true, content);
            break;
        case XSValue::dt_QName:
            return (context.getVersion() == XSValueContext::ver_10) ? 
                XMLChar1_0::isValidQName(content, XMLString::stringLen(content)) :
                XMLChar1_1::isValidQName(content, XMLString::stringLen(content));
            break;
        case XSValue::dt_NOTATION:
            return XMLString::isValidNOTATION(content, manager);
            break;
        case XSValue::dt_string:
            {
                XMLCh*   rawPtr = (XMLCh*) content;

                if (context.getVersion() == XSValueContext::ver_10)
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

                if (context.getVersion() == XSValueContext::ver_10)
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

                if (context.getVersion() == XSValueContext::ver_10)
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
                    context.fStatus = XSValueContext::st_CantCreateRegEx;
                    return false;
                }

                return regEx->matches(content, manager);
            }

            break;
        case XSValue::dt_NMTOKEN:
            return (context.getVersion() == XSValueContext::ver_10) ? 
                XMLChar1_0::isValidNmtoken(content, XMLString::stringLen(content)) :
                XMLChar1_1::isValidNmtoken(content, XMLString::stringLen(content));
            break;
        case XSValue::dt_NMTOKENS:
            // [8]    Nmtokens   ::=    Nmtoken (#x20 Nmtoken)*
            {
                XMLStringTokenizer tokenizer(content, Separator_20, manager);

                if (context.getVersion() ==  XSValueContext::ver_10 )
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
            return (context.getVersion() == XSValueContext::ver_10) ? 
                XMLChar1_0::isValidName(content, XMLString::stringLen(content)) :
                XMLChar1_1::isValidName(content, XMLString::stringLen(content));
            break;
        case XSValue::dt_NCName:
        case XSValue::dt_ID:
        case XSValue::dt_IDREF:
        case XSValue::dt_ENTITY:
            return (context.getVersion() == XSValueContext::ver_10) ? 
                XMLChar1_0::isValidNCName(content, XMLString::stringLen(content)) :
                XMLChar1_1::isValidNCName(content, XMLString::stringLen(content));
            break;
        case XSValue::dt_ENTITIES:
        case XSValue::dt_IDREFS:
            {
                XMLStringTokenizer tokenizer(content, Separator_ws, manager);

                if (context.getVersion() ==  XSValueContext::ver_10 )
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
        context.fStatus = XSValueContext::st_Invalid;
        return false; 
    }

}


XMLCh* XSValue::getCanRepNumerics(const XMLCh*         const content    
                                ,       DataType             datatype
                                ,       XSValueContext&      context
                                ,       MemoryManager* const manager)
{
    try 
    {

        // All getCanonicalRepresentation does lexcial space validation only
        // (no range checking), therefore if validation is requied,
        // we need to pass the content to the validate interface for complete checking
        if (context.getValidation() && !validateNumerics(content, datatype, context, manager))
            return 0;

        XMLCh* retVal;

        if (datatype == XSValue::dt_decimal)
        {
            retVal = XMLBigDecimal::getCanonicalRepresentation(content, manager);

            if (!retVal)
                context.fStatus = XSValueContext::st_InvalidChar;

            return retVal;

        }
        else if (datatype == XSValue::dt_float || datatype == XSValue::dt_double  )
        {
            retVal = XMLAbstractDoubleFloat::getCanonicalRepresentation(content, manager);

            if (!retVal)
                context.fStatus = XSValueContext::st_InvalidChar;

            return retVal;
        }  
        else 
        {
            retVal = XMLBigInteger::getCanonicalRepresentation(content, manager);

            if (!retVal)
                context.fStatus = XSValueContext::st_InvalidChar;

            return retVal;
        }
    }

    catch (...)
    {
        context.fStatus = XSValueContext::st_InvalidChar;
        return 0;
    }

}

XMLCh* XSValue::getCanRepDateTimes(const XMLCh*         const content    
                                 ,       DataType             datatype
                                 ,       XSValueContext&      context
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
                if (context.getValidation() && !validateDateTimes(content, datatype, context, manager))
                    context.fStatus = XSValueContext::st_Invalid;
                else
                    context.fStatus = XSValueContext::st_NoCanRep;

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
        context.fStatus = XSValueContext::st_Invalid;
        return 0;
    }

}

XMLCh* XSValue::getCanRepStrings(const XMLCh*         const content    
                               ,       DataType             datatype
                               ,       XSValueContext&      context
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
                    context.fStatus = XSValueContext::st_Invalid;
                    return 0;
                }
            }
            break;
        case XSValue::dt_hexBinary: 
            //HexBin::getCanonicalRepresentation does validation automatically
            return HexBin::getCanonicalRepresentation(content, manager);
            break;
        case XSValue::dt_base64Binary:
            //Base64::getCanonicalRepresentation does validation automatically
            return Base64::getCanonicalRepresentation(content, manager);
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
                if (context.getValidation() && !validateStrings(content, datatype, context, manager))
                    context.fStatus = XSValueContext::st_Invalid;
                else
                    context.fStatus = XSValueContext::st_NoCanRep;

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
        context.fStatus = XSValueContext::st_Invalid;
        return 0;
    }
}

XSValue*
XSValue::getActValNumerics(const XMLCh*         const content    
                         ,       DataType             datatype
                         ,       XSValueContext&      context
                         ,       MemoryManager* const manager)
{

    try {

        if (datatype == XSValue::dt_decimal)
        {
            XMLBigDecimal data(content, manager); 

            int      totalDigit = data.getTotalDigit();
            int      scale      = data.getScale();
            XMLCh*   intVal     = data.getIntVal();

            // get the fraction
            t_value  actValFract;

            if ( !getActualValue( 
                                  &(intVal[totalDigit - scale])
                                , context
                                , convert_2_ulong
                                , actValFract
                                , base_decimal
                                , manager
                                )
                )
                return 0;

            // get the integer
            t_value  actValInt;
            intVal[totalDigit - scale] = 0;

            if ( !getActualValue( 
                                  intVal
                                , context
                                , convert_2_ulong
                                , actValInt
                                , base_decimal
                                , manager
                                )
                )
                return 0;

            XSValue* retVal = new (manager) XSValue(manager);

            retVal->fData.f_decimal.f_sign     = data.getSign();
            retVal->fData.f_decimal.f_scale    = data.getScale();
            retVal->fData.f_decimal.f_fraction = actValFract.f_ulong;
            retVal->fData.f_decimal.f_integral = actValInt.f_ulong;

            return retVal;
        }
        else if (datatype == XSValue::dt_float)
        {
            //XMLFloat::checkBoundary() handles error and outofbound issues
            XMLFloat data(content, manager);
            XSValue* retVal = new (manager) XSValue(manager);
            retVal->fData.f_float = (float) data.getValue();
            return retVal;
        }
        else if (datatype == XSValue::dt_double)
        {
            //XMLDouble::checkBoundary() handles error and outofbound issues
            XMLDouble  data(content, manager);
            XSValue* retVal = new (manager) XSValue(manager);
            retVal->fData.f_double = data.getValue();
            return retVal;
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
                                , context
                                , convert_2_long
                                , actVal
                                , base_decimal
                                , manager
                                )
                )
                return 0;

            switch (datatype)
            { 
            case XSValue::dt_integer:
                // error: no
                {
                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_long = actVal.f_long;
                    return retVal;
                }
                break;
            case XSValue::dt_negativeInteger:
                // error: > -1
                {
                    if (actVal.f_long > -1)
                        return 0;

                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_long = actVal.f_long;
                    return retVal;
                }
                break;
            case XSValue::dt_nonPositiveInteger:
                // error: > 0
                {
                    if (actVal.f_long > 0)
                        return 0;

                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_long = actVal.f_long;
                    return retVal;
                }
                break;
            case XSValue::dt_nonNegativeInteger:
                // error: < 0
                {
                    if (actVal.f_long < 0)
                        return 0;

                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_long = actVal.f_long;
                    return retVal;
                }
                break;
            case XSValue::dt_positiveInteger:
                // error: < 1
                {
                    if (actVal.f_long < 1)
                        return 0;

                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_long = actVal.f_long;
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
                                , context
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
                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_long = actVal.f_long;
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
                        context.fStatus = XSValueContext::st_InvalidRange;
                        return 0;
                    }

                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_int = (int) actVal.f_long;
                    return retVal;
                }
                break;
            case XSValue::dt_short:
                // error: < SHRT_MIN || > SHRT_MAX
                {
                    if ((actVal.f_long < SHRT_MIN) || (actVal.f_long > SHRT_MAX))
                    {
                        context.fStatus = XSValueContext::st_InvalidRange;
                        return 0;
                    }

                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_short = (short) actVal.f_long;
                    return retVal;
                }
                break;
            case XSValue::dt_byte:
                // error: < CHAR_MIN || > CHAR_MAX
                {
                    if ((actVal.f_long < CHAR_MIN) || (actVal.f_long > CHAR_MAX))
                    {
                        context.fStatus = XSValueContext::st_InvalidRange;
                        return 0;
                    }

                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_char = (char) actVal.f_long;
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
                                , context
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
                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_ulong = actVal.f_ulong;
                    return retVal;
                }
                break;
            case XSValue::dt_unsignedInt:     
                // error: > UINT_MAX
                // strtol() won't overflow for UINT_MAX+1 on 64 box
                {

                    if (actVal.f_long > UINT_MAX) 
                    {
                        context.fStatus = XSValueContext::st_InvalidRange;
                        return 0;
                    }

                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_uint = (unsigned int) actVal.f_ulong;
                    return retVal;
                }
                break;
            case XSValue::dt_unsignedShort:
                // error: > USHRT_MAX
                {
                    if (actVal.f_ulong > USHRT_MAX)
                    {
                        context.fStatus = XSValueContext::st_InvalidRange;
                        return 0;
                    }

                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_ushort = (unsigned short) actVal.f_ulong;
                    return retVal;
                }
                break;
            case XSValue::dt_unsignedByte:   
                // error: > UCHAR_MAX
                {
                    if (actVal.f_ulong > UCHAR_MAX)
                    {
                        context.fStatus = XSValueContext::st_InvalidRange;
                        return 0;
                    }

                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_uchar = (unsigned char) actVal.f_ulong;
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
        context.fStatus = XSValueContext::st_InvalidChar;
        return 0; 
    }
}

XSValue*  
XSValue::getActValDateTimes(const XMLCh*         const content    
                          ,       DataType             datatype
                          ,       XSValueContext&      context
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

        XSValue* retVal = new (manager) XSValue(manager);

        retVal->fData.f_datetime.f_year    = coreDate.fValue[XMLDateTime::CentYear];
        retVal->fData.f_datetime.f_month   = coreDate.fValue[XMLDateTime::Month];
        retVal->fData.f_datetime.f_day     = coreDate.fValue[XMLDateTime::Day];
        retVal->fData.f_datetime.f_hour    = coreDate.fValue[XMLDateTime::Hour];
        retVal->fData.f_datetime.f_min     = coreDate.fValue[XMLDateTime::Minute];
        retVal->fData.f_datetime.f_second  = coreDate.fValue[XMLDateTime::Second];
        retVal->fData.f_datetime.f_milisec = coreDate.fValue[XMLDateTime::MiliSecond];
        retVal->fData.f_datetime.f_utc     = coreDate.fValue[XMLDateTime::utc];
        retVal->fData.f_datetime.f_tz_hh   = coreDate.fTimeZone[XMLDateTime::hh];
        retVal->fData.f_datetime.f_tz_mm   = coreDate.fTimeZone[XMLDateTime::mm];

        return retVal;
    }

    catch (...)
    {
        context.fStatus = XSValueContext::st_Invalid;
        return 0; 
    }

}

XSValue*  
XSValue::getActValStrings(const XMLCh*         const content    
                        ,       DataType             datatype
                        ,       XSValueContext&      context
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
                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_bool = true;
                    return retVal;
                }
                else if (XMLString::equals(content, XMLUni::fgBooleanValueSpace[1]) ||
                         XMLString::equals(content, XMLUni::fgBooleanValueSpace[3])  )
                {
                    XSValue* retVal = new (manager) XSValue(manager);
                    retVal->fData.f_bool = false;
                    return retVal;
                }
                else
                {
                    context.fStatus = XSValueContext::st_Invalid;
                    return 0;
                }
            }
            break;
        case XSValue::dt_hexBinary:
            {
                //todo: HexBinary::decode()
                XSValue* retVal = new (manager) XSValue(manager);
                return retVal;
            }       
            break;
        case XSValue::dt_base64Binary:
            {
                unsigned int    len = 0;
                XMLCh* decodedData = Base64::decode(content, &len, manager);

                if (!decodedData)
                {
                    context.fStatus = XSValueContext::st_Invalid;
                    return 0;
                }

                XSValue* retVal = new (manager) XSValue(manager);
                retVal->fData.f_strVal = decodedData;
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
                if (context.getValidation() && !validateStrings(content, datatype, context, manager))
                    context.fStatus = XSValueContext::st_Invalid;
                else
                    context.fStatus = XSValueContext::st_NoActVal;

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
        context.fStatus = XSValueContext::st_Invalid;
        return 0; 
    }
}

// ---------------------------------------------------------------------------
//  Utilities
// ---------------------------------------------------------------------------
bool XSValue::getActualValue(const XMLCh*         const content
                           ,       XSValueContext&      context
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
        retVal.f_ulong = strtoul(nptr, &endptr, base);
    }

    // check if all chars are valid char
    if ( (endptr - nptr) != strLen)
    {
        context.fStatus = XSValueContext::st_InvalidChar;
        return false;
    }

    // check if overflow/underflow occurs
    if (errno == ERANGE)
    {
        context.fStatus = XSValueContext::st_UnRepresentable;
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

// ---------------------------------------------------------------------------
//  XSValueContext: Constructors and Destructor
// ---------------------------------------------------------------------------
XSValueContext::XSValueContext(XMLVersion   version
                            ,  bool         toValidate)
:fVersion(version)
,fToValidate(toValidate)
,fStatus(st_Init)
{
}

XSValueContext::~XSValueContext()
{
}

XERCES_CPP_NAMESPACE_END


