/*
* The Apache Software License, Version 1.1
*
* Copyright (c) 1999-2004 The Apache Software Foundation.  All rights
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
* $Id$
* $Log$
* Revision 1.6  2004/08/31 20:53:43  peiyongz
* Testing time zone
*
* Revision 1.5  2004/08/31 15:15:16  peiyongz
* remove XSValueContext
*
* Revision 1.4  2004/08/24 16:00:15  peiyongz
* To build on AIX/Win2003-ecl
*
* Revision 1.3  2004/08/23 17:07:57  peiyongz
* Minimum representable range on all platforms
*
* Revision 1.2  2004/08/19 21:29:28  peiyongz
* no message
*
* Revision 1.1  2004/08/19 17:17:21  peiyongz
* XSValueTest
*
*
*/

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "XSValueTest.hpp"

#if defined(XERCES_NEW_IOSTREAMS)
#include <fstream>
#else
#include <fstream.h>
#endif

#include <stdio.h>

#include <xercesc/framework/psvi/XSValue.hpp>
#include <xercesc/util/PlatformUtils.hpp>

static const bool  EXP_RET_VALID_TRUE  = true;
static const bool  EXP_RET_VALUE_TRUE  = true;
static const bool  EXP_RET_CANREP_TRUE = true;

static const bool  EXP_RET_VALID_FALSE  = false;
static const bool  EXP_RET_VALUE_FALSE  = false;
static const bool  EXP_RET_CANREP_FALSE = false;

static const XSValue::Status DONT_CARE = XSValue::st_UnknownType;
static bool  errSeen = false;

/**
 * This is to test methods for XSValue
 */

#ifdef _DEBUG
void VALIDATE_TEST( const char*                  const  data
                  , const XSValue::DataType             datatype
                  ,       bool                          expRetValid
                  , const XSValue::Status               expStatus
                  )
{
    XSValue::Status myStatus = XSValue::st_Init;

    bool actRetValid = XSValue::validate(
                                          StrX(data).unicodeForm()
                                        , datatype
                                        , myStatus
                                        , XSValue::ver_10
                                        , true
                                        , XMLPlatformUtils::fgMemoryManager);

    if (actRetValid != expRetValid)
    {
        printf("VALIDATE_TEST Validation Fail: data=<%s>, datatype=<%d>, expRetVal=<%d>\n",
                data, datatype, expRetValid);
        errSeen = true;
    }
    else 
    {
        if (!expRetValid             && 
             expStatus != DONT_CARE  && 
             expStatus != myStatus ) 
        {
            printf("VALIDATE_TEST Context Diff, data=<%s> datatype=<%d>, expStatus=<%d>, actStatus=<%d>\n",
                    data, datatype, expStatus, myStatus);
            errSeen = true;
        }
    }
}
#else
#define VALIDATE_TEST(data, datatype, expRetValid, expStatus)                        \
{                                                                                    \
    XSValue::Status myStatus = XSValue::st_Init;                                     \
    bool actRetValid = XSValue::validate(                                            \
                                          StrX(data).unicodeForm()                   \
                                        , datatype                                   \
                                        , myStatus                                   \
                                        , XSValue::ver_10                            \
                                        , true                                       \
                                        , XMLPlatformUtils::fgMemoryManager);        \
    if (actRetValid != expRetValid) {                                                \
        printf("VALIDATE_TEST Validation Fail:                                       \
                at line <%d>, data=<%s>, datatype=<%d>, expRetVal=<%d>\n"            \
              , __LINE__, data, datatype, expRetValid);                              \
        errSeen = true;                                                              \
    }                                                                                \
    else {                                                                           \
        if (!expRetValid             &&                                              \
             expStatus != DONT_CARE  &&                                              \
             expStatus != myStatus ) {                                               \
             printf("VALIDATE_TEST Context Diff,                                     \
                   at line <%d>, data=<%s> datatype=<%d>,                            \
                   expStatus=<%d>, actStatus=<%d>\n"                                 \
                  , __LINE__, data, datatype, expStatus, myStatus);                  \
             errSeen = true;                                                         \
        }                                                                            \
    }                                                                                \
}
#endif

#ifdef _DEBUG
void ACTVALUE_TEST(  const char*                  const  data
                   , const XSValue::DataType             datatype
                   ,       bool                          toValidate
                   ,       bool                          expRetValue
                   , const XSValue::Status               expStatus
                    )
{
    XSValue::Status myStatus = XSValue::st_Init;

    XSValue* actRetValue = XSValue::getActualValue(
                                                   StrX(data).unicodeForm()
                                                 , datatype
                                                 , myStatus
                                                 , XSValue::ver_10
                                                 , toValidate
                                                 , XMLPlatformUtils::fgMemoryManager);
    if (actRetValue) 
    {
        if (!expRetValue) 
        {
            printf("ACTVALUE_TEST XSValue returned: data=<%s>, datatype=<%d>\n",
                    data, datatype);
            errSeen = true;
        }                                                                            

        delete actRetValue;
    }
    else 
    {
        if (expRetValue) 
        {
            printf("ACTVALUE_TEST No XSValue returned, data=<%s>, datatype=<%d>\n" ,
                    data, datatype);
            errSeen = true;
        }
        else
        {
            if (expStatus != DONT_CARE  &&
                expStatus != myStatus    ) 
            {
                printf("ACTVALUE_TEST Context Diff, data=<%s>, datatype=<%d>, expStatus=<%d>, actStatus=<%d>\n" ,
                       data, datatype, expStatus, myStatus);
                errSeen = true;
            }
        }
    }
}
#else
#define ACTVALUE_TEST(data, datatype, toValidate, expRetValue, expStatus)              \
{                                                                                      \
    XSValue::Status myStatus = XSValue::st_Init;                                       \
    XSValue* actRetValue = XSValue::getActualValue(                                    \
                                                   StrX(data).unicodeForm()            \
                                                 , datatype                            \
                                                 , myStatus                            \
                                                 , XSValue::ver_10                     \
                                                 , toValidate                          \
                                                 , XMLPlatformUtils::fgMemoryManager); \
    if (actRetValue) {                                                                 \
       if (!expRetValue) {                                                             \
            printf("ACTVALUE_TEST XSValue returned,                                    \
                    at line <%d> data=<%s>, datatype=<%d>\n"                           \
                  ,__LINE__, data, datatype);                                          \
            errSeen = true;                                                            \
       }                                                                               \
       delete actRetValue;                                                             \
    }                                                                                  \
    else {                                                                             \
        if (expRetValue) {                                                             \
            printf("ACTVALUE_TEST No XSValue returned,                                 \
                    at line <%d> data=<%s>, datatype=<%d>\n"                           \
                  , __LINE__, data, datatype);                                         \
            errSeen = true;                                                            \
         }                                                                             \
         else {                                                                        \
             if (expStatus != DONT_CARE &&                                             \
                 expStatus != myStatus) {                                              \
                 printf("ACTVALUE_TEST Context Diff,                                   \
                         at line <%d> data=<%s>, datatype=<%d>,                        \
                         expStatus=<%d>, actStatus=<%d>\n"                             \
                        , __LINE__, data, datatype, expStatus, myStatus);              \
                 errSeen = true;                                                       \
             }                                                                         \
        }                                                                              \
    }                                                                                  \
}
#endif

#ifdef _DEBUG
void CANREP_TEST(const char*                  const  data
               , const XSValue::DataType             datatype
               ,       bool                          toValidate
               ,       bool                          expRetCanRep
               , const char*                  const  toCompare
               , const XSValue::Status               expStatus
                )
{
    XSValue::Status myStatus = XSValue::st_Init;

    XMLCh* actRetCanRep = XSValue::getCanonicalRepresentation(
                                                            StrX(data).unicodeForm()
                                                          , datatype
                                                          , myStatus
                                                          , XSValue::ver_10
                                                          , toValidate
                                                          , XMLPlatformUtils::fgMemoryManager);
    if (actRetCanRep)
    {
        if (!expRetCanRep)
        {
            printf("CANREP_TEST CanRep returned, data=<%s>, datatype=<%d>\n" ,
                    data, datatype);
            XMLPlatformUtils::fgMemoryManager->deallocate(actRetCanRep);
            errSeen = true;
        }
        else
        {
            char* actRetCanRep_inchar = XMLString::transcode(actRetCanRep);

            if (!XMLString::equals(actRetCanRep_inchar, toCompare)) 
            {
                printf("CANREP_TEST CanRep Diff , data=<%s>, datatype=<%d>, actCanRep=<%s>, toCompare=<%s>\n" ,
                    data, datatype, actRetCanRep_inchar, toCompare);
                errSeen = true;
            }

            XMLPlatformUtils::fgMemoryManager->deallocate(actRetCanRep);
            XMLString::release(&actRetCanRep_inchar);
        }
    }                                                                                 
    else 
    {                                                                            
        if (expRetCanRep)
        {
            printf("CANREP_TEST No CanRep returned, data=<%s>, datatype=<%d>\n" ,
                    data, datatype);
            errSeen = true;
        }
        else
        {
            if (expStatus != DONT_CARE  &&
                expStatus != myStatus    )
            {
                printf("CANREP_TEST Context Diff, data=<%s>, datatype=<%d>\n expStatus=<%d>, actStatus=<%d>\n" ,
                    data, datatype, expStatus, myStatus);
                errSeen = true;
            }
        }
    }                                                                                 
}
#else
#define CANREP_TEST(data, datatype, toValidate, expRetCanRep, toCompare, expStatus)   \
{                                                                                     \
    XSValue::Status myStatus = XSValue::st_Init;                                      \
    XMLCh* actRetCanRep = XSValue::getCanonicalRepresentation(                        \
                                                  StrX(data).unicodeForm()            \
                                                , datatype                            \
                                                , myStatus                            \
                                                , XSValue::ver_10                     \
                                                , toValidate                          \
                                                , XMLPlatformUtils::fgMemoryManager); \
    if (actRetCanRep) {                                                               \
        if (!expRetCanRep) {                                                          \
            printf("CANREP_TEST CanRep returned:                                      \
                    at line <%d> data=<%s>, datatype=<%d>\n"                          \
                  , __LINE__, data, datatype);                                        \
            XMLPlatformUtils::fgMemoryManager->deallocate(actRetCanRep);              \
            errSeen = true;                                                           \
        }                                                                             \
        else {                                                                        \
            char* actRetCanRep_inchar = XMLString::transcode(actRetCanRep);           \
            if (!XMLString::equals(actRetCanRep_inchar, toCompare)) {                 \
                printf("CANREP_TEST CanRep Diff                                       \
                  , at line <%d> data=<%s>, datatype=<%d>,                            \
                    actCanRep=<%s>, toCompare=<%s>\n"                                 \
                  , __LINE__, data, datatype, actRetCanRep_inchar, toCompare);        \
                errSeen = true;                                                       \
            }                                                                         \
            XMLPlatformUtils::fgMemoryManager->deallocate(actRetCanRep);              \
            XMLString::release(&actRetCanRep_inchar);                                 \
        }                                                                             \
    }                                                                                 \
    else {                                                                            \
        if (expRetCanRep){                                                            \
            printf("CANREP_TEST No CanRep returned,                                   \
                    at line <%d> data=<%s>, datatype=<%d>\n"                          \
                  , __LINE__, data, datatype);                                        \
            errSeen = true;                                                           \
        }                                                                             \
        else {                                                                        \
        if (expStatus != myStatus) {                                                  \
                printf("CANREP_TEST Context Diff,                                     \
                        at line <%d> data=<%s>, datatype=<%d>\n                       \
                        expStatus=<%d>, actStatus=<%d>\n"                             \
                      , __LINE__, data, datatype, expStatus, myStatus);               \
                errSeen = true;                                                       \
            }                                                                         \
        }                                                                             \
    }                                                                                 \
}    
#endif

void test_dt_decimal()
{
    const XSValue::DataType dt = XSValue::dt_decimal; 
    bool  toValidate = true;

    const char lex_v_ran_v_1[]="1234.567";
    const char lex_v_ran64_v_1[]="18446744073709551615.999";
    const char lex_v_ran64_v_2[]="999.18446744073709551615";
    const char lex_v_ran64_iv_1[]="18446744073709551616.999";
    const char lex_v_ran64_iv_2[]="999.18446744073709551616";

    const char lex_v_ran32_v_1[]="4294967295.999";
    const char lex_v_ran32_v_2[]="999.4294967295";
    const char lex_v_ran32_iv_1[]="4294967296.999";
    const char lex_v_ran32_iv_2[]="999.4294967296";

    const char lex_iv_1[]="12b34.456";
    const char lex_iv_2[]="1234.56.789";

/***
 * 3.2.3.2 Canonical representation
 *
 * The canonical representation for decimal is defined by prohibiting certain options from the Lexical 
 * representation (§3.2.3.1). Specifically, 
 * 1. the preceding optional "+" sign is prohibited. 
 * 2. The decimal point is required. 
 * 3. Leading and trailing zeroes are prohibited subject to the following: 
 *       there must be at least one digit to the right and 
 *       to the left of the decimal point which may be a zero.
 ***/

    const char data_rawstr_1[]="-123.45";
    const char data_canrep_1[]="-123.45";
    const char data_rawstr_2[]="+123.45";
    const char data_canrep_2[]="123.45";
    const char data_rawstr_3[]="12345";
    const char data_canrep_3[]="12345.0";
    const char data_rawstr_4[]="000123.45";
    const char data_canrep_4[]="123.45";
    const char data_rawstr_5[]="123.45000";
    const char data_canrep_5[]="123.45";
    const char data_rawstr_6[]="00.12345";
    const char data_canrep_6[]="0.12345";
    const char data_rawstr_7[]="123.00";
    const char data_canrep_7[]="123.0";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid       n/a   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid
    VALIDATE_TEST(lex_v_ran_v_1    , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_v_1  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_iv_1 , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_iv_2 , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_1,    dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

#if defined(XML_BITSTOBUILD_64)
        ACTVALUE_TEST(lex_v_ran64_v_1 , dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE); 
        ACTVALUE_TEST(lex_v_ran64_v_2 , dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran64_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#else
        ACTVALUE_TEST(lex_v_ran32_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran32_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran32_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran32_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#endif

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid          n/a                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid          n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,    DONT_CARE);
        CANREP_TEST(data_rawstr_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,    DONT_CARE);
        CANREP_TEST(data_rawstr_3,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_3,    DONT_CARE);
        CANREP_TEST(data_rawstr_4,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_4,    DONT_CARE);
        CANREP_TEST(data_rawstr_5,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_5,    DONT_CARE);
        CANREP_TEST(data_rawstr_6,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_6,    DONT_CARE);
        CANREP_TEST(data_rawstr_7,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_7,    DONT_CARE);

        CANREP_TEST(lex_v_ran64_v_1,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_v_1,  DONT_CARE);
        CANREP_TEST(lex_v_ran64_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_v_2,  DONT_CARE);
        CANREP_TEST(lex_v_ran64_iv_1, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_iv_1, DONT_CARE);
        CANREP_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_iv_2, DONT_CARE);
        CANREP_TEST(lex_iv_1        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);

    }

}

/***
  * FLT_EPSILON 1.192092896e-07F 
  * FLT_MIN 1.175494351e-38F 
  * FLT_MAX 3.402823466e+38F 
***/

void test_dt_float()
{

    const XSValue::DataType dt = XSValue::dt_float; 
    bool  toValidate = true;

    const char lex_v_ran_v_0[]="1234.e+10";
    const char lex_v_ran_v_1[]="+3.402823466e+38";
    const char lex_v_ran_v_2[]="-3.402823466e+38";
    const char lex_v_ran_v_3[]="+1.175494351e-38";
    const char lex_v_ran_v_4[]="-1.175494351e-38";

    const char lex_v_ran_iv_1[]="+3.402823466e+39";
    const char lex_v_ran_iv_2[]="-3.402823466e+39";
    const char lex_v_ran_iv_3[]="+1.175494351e-39";
    const char lex_v_ran_iv_4[]="-1.175494351e-39";

    const char lex_v_ran_iv_1_canrep[]="3.402823466E39";
    const char lex_v_ran_iv_2_canrep[]="-3.402823466E39";
    const char lex_v_ran_iv_3_canrep[]="1.175494351E-39";
    const char lex_v_ran_iv_4_canrep[]="-1.175494351E-39";

    const char lex_iv_1[]="12x.e+10";
    const char lex_iv_2[]="12.e+1x";

/***
 * 3.2.4.2 Canonical representation
 *
 * The canonical representation for float is defined by prohibiting certain options from the Lexical 
 * representation (§3.2.4.1). 
 * Specifically, 
 * 1. the exponent must be indicated by "E". 
 * 2. Leading zeroes and the preceding optional "+" sign are prohibited in the exponent. 
 * 3. For the mantissa, the preceding optional "+" sign is prohibited and the decimal point is required. 
 *    Leading and trailing zeroes are prohibited subject to the following: 
 *    number representations must be normalized such that there is a single digit to the left of the decimal point
 *    and at least a single digit to the right of the decimal point.
 *
 ***/

    const char data_rawstr_1[]="-123.45";
    const char data_canrep_1[]="-1.2345E2";
    const char data_rawstr_2[]="+123.45";
    const char data_canrep_2[]="1.2345E2";
    const char data_rawstr_3[]="+123.45e+0012";
    const char data_canrep_3[]="1.2345E14";
    const char data_rawstr_4[]="+100.000e2";
    const char data_canrep_4[]="1.0E4";
    const char data_rawstr_5[]="00100.23e2";
    const char data_canrep_5[]="1.0023E4";

    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid                         false           st_InvalidChar   
     *    lexical invalid                                false           st_Invalid
     * 
     ***/

    // lexical valid, range valid
    VALIDATE_TEST(lex_v_ran_v_0 , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_1 , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_2 , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_3 , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_4 , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical valid, range invalid
    VALIDATE_TEST(lex_v_ran_iv_1 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran_iv_2 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran_iv_3 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran_iv_4 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        // lexical valid, range valid
        ACTVALUE_TEST(lex_v_ran_v_0,  dt, toValidate, EXP_RET_VALUE_TRUE, DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE, DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE, DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_3,  dt, toValidate, EXP_RET_VALUE_TRUE, DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_4,  dt, toValidate, EXP_RET_VALUE_TRUE, DONT_CARE);

        // lexical valid, range invalid
        ACTVALUE_TEST(lex_v_ran_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran_iv_3, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran_iv_4, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);

        // lexical invalid
        ACTVALUE_TEST(lex_iv_1      , dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2      , dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);

    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid                              0            st_InvalidRange
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid                            XMLCh          n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        // lexical valid, range valid
        CANREP_TEST(data_rawstr_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,    DONT_CARE);
        CANREP_TEST(data_rawstr_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,    DONT_CARE);
        CANREP_TEST(data_rawstr_3,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_3,    DONT_CARE);
        CANREP_TEST(data_rawstr_4,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_4,    DONT_CARE);
        CANREP_TEST(data_rawstr_5,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_5,    DONT_CARE);

        // lexical invalid
        CANREP_TEST(lex_iv_1,         dt, toValidate, EXP_RET_CANREP_FALSE,  0,               XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2,         dt, toValidate, EXP_RET_CANREP_FALSE,  0,               XSValue::st_InvalidChar);
    }

    // lexical valid, range invalid

    toValidate = true;
    CANREP_TEST(lex_v_ran_iv_1,    dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran_iv_2,    dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran_iv_3,    dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran_iv_4,    dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);

    toValidate = false;
    CANREP_TEST(lex_v_ran_iv_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_1_canrep,    DONT_CARE);
    CANREP_TEST(lex_v_ran_iv_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_2_canrep,    DONT_CARE);
    CANREP_TEST(lex_v_ran_iv_3,    dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_3_canrep,    DONT_CARE);
    CANREP_TEST(lex_v_ran_iv_4,    dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_4_canrep,    DONT_CARE);

}

/***
DBL_EPSILON 2.2204460492503131e-016
DBL_MAX 1.7976931348623158e+308
DBL_MIN 2.2250738585072014e-308 
***/
void test_dt_double()
{

    const XSValue::DataType dt = XSValue::dt_double; 
    bool  toValidate = true;

    const char lex_v_ran_v_0[]="1234.e+10";
    const char lex_v_ran_v_1[]="+1.7976931348623158e+308";
    const char lex_v_ran_v_2[]="-1.7976931348623158e+308";
    const char lex_v_ran_v_3[]="+2.2250738585072014e-308";
    const char lex_v_ran_v_4[]="-2.2250738585072014e-308";

    const char lex_v_ran_iv_1[]="+1.7976931348623158e+309";
    const char lex_v_ran_iv_2[]="-1.7976931348623158e+309";
    // on linux, hp, aix, the representable range is around e-324
    // or e-325, using e-329 to gain consistent result on all
    // platforms
    const char lex_v_ran_iv_3[]="+2.2250738585072014e-329";
    const char lex_v_ran_iv_4[]="-2.2250738585072014e-329";

    const char lex_v_ran_iv_1_canrep[]="1.7976931348623158E309";
    const char lex_v_ran_iv_2_canrep[]="-1.7976931348623158E309";
    const char lex_v_ran_iv_3_canrep[]="2.2250738585072014E-329";
    const char lex_v_ran_iv_4_canrep[]="-2.2250738585072014E-329";

    const char lex_iv_1[]="12x.e+10";
    const char lex_iv_2[]="12.e+1x";

/***
 * 3.2.5.2 Canonical representation
 *
 * The canonical representation for float is defined by prohibiting certain options from the Lexical 
 * representation (§3.2.5.1). 
 * Specifically, 
 * 1. the exponent must be indicated by "E". 
 * 2. Leading zeroes and the preceding optional "+" sign are prohibited in the exponent. 
 * 3. For the mantissa, the preceding optional "+" sign is prohibited and the decimal point is required. 
 *    Leading and trailing zeroes are prohibited subject to the following: 
 *    number representations must be normalized such that there is a single digit to the left of the decimal point
 *    and at least a single digit to the right of the decimal point.
 *
 ***/

    const char data_rawstr_1[]="-123.45";
    const char data_canrep_1[]="-1.2345E2";
    const char data_rawstr_2[]="+123.45";
    const char data_canrep_2[]="1.2345E2";
    const char data_rawstr_3[]="+123.45e+0012";
    const char data_canrep_3[]="1.2345E14";
    const char data_rawstr_4[]="+100.000e2";
    const char data_canrep_4[]="1.0E4";
    const char data_rawstr_5[]="00100.23e2";
    const char data_canrep_5[]="1.0023E4";

    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid                         false           st_InvalidChar   
     *    lexical invalid                                false           st_Invalid
     * 
     ***/

    // lexical valid, range valid
    VALIDATE_TEST(lex_v_ran_v_0 , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_1 , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_2 , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_3 , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_4 , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical valid, range invalid
    VALIDATE_TEST(lex_v_ran_iv_1 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran_iv_2 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran_iv_3 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran_iv_4 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        // lexical valid, range valid
        ACTVALUE_TEST(lex_v_ran_v_0,  dt, toValidate, EXP_RET_VALUE_TRUE, DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE, DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE, DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_3,  dt, toValidate, EXP_RET_VALUE_TRUE, DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_4,  dt, toValidate, EXP_RET_VALUE_TRUE, DONT_CARE);

        // lexical valid, range invalid
        ACTVALUE_TEST(lex_v_ran_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran_iv_3, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran_iv_4, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);

        // lexical invalid
        ACTVALUE_TEST(lex_iv_1      , dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2      , dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);

    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid                              0            st_InvalidRange
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid                            XMLCh          n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        // lexical valid, range valid
        CANREP_TEST(data_rawstr_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,    DONT_CARE);
        CANREP_TEST(data_rawstr_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,    DONT_CARE);
        CANREP_TEST(data_rawstr_3,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_3,    DONT_CARE);
        CANREP_TEST(data_rawstr_4,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_4,    DONT_CARE);
        CANREP_TEST(data_rawstr_5,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_5,    DONT_CARE);

        // lexical invalid
        CANREP_TEST(lex_iv_1,         dt, toValidate, EXP_RET_CANREP_FALSE,  0,               XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2,         dt, toValidate, EXP_RET_CANREP_FALSE,  0,               XSValue::st_InvalidChar);
    }

    // lexical valid, range invalid

    toValidate = true;
    CANREP_TEST(lex_v_ran_iv_1,    dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran_iv_2,    dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran_iv_3,    dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran_iv_4,    dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);

    toValidate = false;
    CANREP_TEST(lex_v_ran_iv_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_1_canrep,    DONT_CARE);
    CANREP_TEST(lex_v_ran_iv_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_2_canrep,    DONT_CARE);
    CANREP_TEST(lex_v_ran_iv_3,    dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_3_canrep,    DONT_CARE);
    CANREP_TEST(lex_v_ran_iv_4,    dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_4_canrep,    DONT_CARE);

}

/***
 *  9223372036854775807 
 * -9223372036854775808
 *  2147483647 
 * -2147483648
 ***/

void test_dt_integer()
{
    const XSValue::DataType dt = XSValue::dt_integer; 
    bool  toValidate = true;

    const char lex_v_ran_v_1[]="1234";
    const char lex_v_ran64_v_1[]="+9223372036854775807";
    const char lex_v_ran64_v_2[]="-9223372036854775808";
    const char lex_v_ran64_iv_1[]="+9223372036854775808";
    const char lex_v_ran64_iv_2[]="-9223372036854775809";

    const char lex_v_ran32_v_1[]="+2147483647";
    const char lex_v_ran32_v_2[]="-2147483648";
    const char lex_v_ran32_iv_1[]="+2147483648";
    const char lex_v_ran32_iv_2[]="-2147483649";

    const char lex_v_ran64_v_1_canrep[]="9223372036854775807";
    const char lex_v_ran64_v_2_canrep[]="-9223372036854775808";
    const char lex_v_ran64_iv_1_canrep[]="9223372036854775808";
    const char lex_v_ran64_iv_2_canrep[]="-9223372036854775809";

    const char lex_iv_1[]="12b34.456";
    const char lex_iv_2[]="1234b56";

/***
 * 3.3.13.2 Canonical representation
 *
 * The canonical representation for integer is defined by prohibiting certain options from the Lexical 
 * representation (§3.3.13.1). Specifically, 
 * 1. the preceding optional "+" sign is prohibited and 
 * 2. leading zeroes are prohibited.
 *
 ***/

    const char data_rawstr_1[]="+12345";
    const char data_canrep_1[]="12345";
    const char data_rawstr_2[]="00012345";
    const char data_canrep_2[]="12345";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid       n/a   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid
    VALIDATE_TEST(lex_v_ran_v_1    , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_v_1  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_iv_1 , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_iv_2 , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_1,    dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

#if defined(XML_BITSTOBUILD_64)
        ACTVALUE_TEST(lex_v_ran64_v_1 , dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE); 
        ACTVALUE_TEST(lex_v_ran64_v_2 , dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran64_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#else
        ACTVALUE_TEST(lex_v_ran32_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran32_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran32_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran32_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#endif

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid          n/a                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid          n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,    DONT_CARE);
        CANREP_TEST(data_rawstr_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,    XSValue::st_InvalidRange);

        CANREP_TEST(lex_v_ran64_v_1,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_v_1_canrep,  DONT_CARE);
        CANREP_TEST(lex_v_ran64_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_v_2_canrep,  DONT_CARE);
        CANREP_TEST(lex_v_ran64_iv_1, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_iv_1_canrep, DONT_CARE);
        CANREP_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_iv_2_canrep, DONT_CARE);
        CANREP_TEST(lex_iv_1        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);

    }

}

void test_dt_nonPositiveInteger()
{
    const XSValue::DataType dt = XSValue::dt_nonPositiveInteger; 
    bool  toValidate = true;

    const char lex_v_ran_v_1[]="-1234";
    const char lex_v_ran_iv_1[]="+1";

    const char lex_v_ran64_v_2[]="-9223372036854775808";
    const char lex_v_ran64_iv_2[]="-9223372036854775809";

    const char lex_v_ran32_v_2[]="-2147483648";
    const char lex_v_ran32_iv_2[]="-2147483649";

    const char lex_v_ran64_v_2_canrep[]="-9223372036854775808";
    const char lex_v_ran64_iv_2_canrep[]="-9223372036854775809";

    const char lex_iv_1[]="12b34.456";
    const char lex_iv_2[]="1234b56";


/***
 * 3.3.14.2 Canonical representation
 *
 * The canonical representation for nonPositiveInteger is defined by prohibiting certain options from the 
 * Lexical representation (§3.3.14.1). Specifically, 
 * 1. the negative sign ("-") is required with the token "0" and 
 * 2. leading zeroes are prohibited.
 *
 ***/

    const char data_rawstr_1[]="0";
    const char data_canrep_1[]="-0";
    const char data_rawstr_2[]="-00012345";
    const char data_canrep_2[]="-12345";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid       n/a   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid
    VALIDATE_TEST(lex_v_ran_v_1    , dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(lex_v_ran_iv_1   , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    VALIDATE_TEST(lex_v_ran64_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_iv_2 , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_1,    dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

#if defined(XML_BITSTOBUILD_64)
        ACTVALUE_TEST(lex_v_ran64_v_2 , dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#else
        ACTVALUE_TEST(lex_v_ran32_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran32_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#endif

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid          n/a                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid          n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,    DONT_CARE);
        CANREP_TEST(data_rawstr_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,    DONT_CARE);

        CANREP_TEST(lex_v_ran64_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_v_2_canrep,  DONT_CARE);
        CANREP_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_iv_2_canrep, DONT_CARE);
        CANREP_TEST(lex_iv_1        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);

    }

}

void test_dt_negativeInteger()
{
    const XSValue::DataType dt = XSValue::dt_negativeInteger; 
    bool  toValidate = true;

    const char lex_v_ran_v_1[]="-1234";
    const char lex_v_ran_iv_1[]="0";

    const char lex_v_ran64_v_2[]="-9223372036854775808";
    const char lex_v_ran64_iv_2[]="-9223372036854775809";

    const char lex_v_ran32_v_2[]="-2147483648";
    const char lex_v_ran32_iv_2[]="-2147483649";

    const char lex_v_ran64_v_2_canrep[]="-9223372036854775808";
    const char lex_v_ran64_iv_2_canrep[]="-9223372036854775809";

    const char lex_iv_1[]="12b34.456";
    const char lex_iv_2[]="1234b56";


/***
 * 3.3.15.2 Canonical representation
 *
 * The canonical representation for negativeInteger is defined by prohibiting certain options 
 * from the Lexical representation (§3.3.15.1). Specifically, 
 * 1. leading zeroes are prohibited.
 *
 ***/

    const char data_rawstr_1[]="-00012345";
    const char data_canrep_1[]="-12345";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid       n/a   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid
    VALIDATE_TEST(lex_v_ran_v_1    , dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(lex_v_ran_iv_1   , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    VALIDATE_TEST(lex_v_ran64_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_iv_2 , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_1,    dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

#if defined(XML_BITSTOBUILD_64)
        ACTVALUE_TEST(lex_v_ran64_v_2 , dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#else
        ACTVALUE_TEST(lex_v_ran32_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran32_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#endif

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid          n/a                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid          n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,    DONT_CARE);

        CANREP_TEST(lex_v_ran64_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_v_2_canrep,  DONT_CARE);
        CANREP_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_iv_2_canrep, DONT_CARE);
        CANREP_TEST(lex_iv_1        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);

    }

}

void test_dt_long()
{
    const XSValue::DataType dt = XSValue::dt_long; 
    bool  toValidate = true;

    const char lex_v_ran_v_1[]="1234";
    const char lex_v_ran64_v_1[]="+9223372036854775807";
    const char lex_v_ran64_v_2[]="-9223372036854775808";
    const char lex_v_ran64_iv_1[]="+9223372036854775808";
    const char lex_v_ran64_iv_2[]="-9223372036854775809";

    const char lex_v_ran32_v_1[]="+2147483647";
    const char lex_v_ran32_v_2[]="-2147483648";
    const char lex_v_ran32_iv_1[]="+2147483648";
    const char lex_v_ran32_iv_2[]="-2147483649";

    const char lex_v_ran64_v_1_canrep[]="9223372036854775807";
    const char lex_v_ran64_v_2_canrep[]="-9223372036854775808";
    const char lex_v_ran64_iv_1_canrep[]="9223372036854775808";
    const char lex_v_ran64_iv_2_canrep[]="-9223372036854775809";

    const char lex_iv_1[]="12b34.456";
    const char lex_iv_2[]="1234b56";

/***
 * 3.3.16.2 Canonical representation
 *
 * The canonical representation for long is defined by prohibiting certain options from the 
 * Lexical representation (§3.3.16.1). Specifically, 
 * 1. the the optional "+" sign is prohibited and 
 * 2. leading zeroes are prohibited.

 ***/

    const char data_rawstr_1[]="+12345";
    const char data_canrep_1[]="12345";
    const char data_rawstr_2[]="00012345";
    const char data_canrep_2[]="12345";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid                         false           st_InvalidRange   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid, valid range
    VALIDATE_TEST(lex_v_ran_v_1    , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_v_1  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical valid, invalid range
    VALIDATE_TEST(lex_v_ran64_iv_1 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran64_iv_2 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_1,    dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

#if defined(XML_BITSTOBUILD_64)
        ACTVALUE_TEST(lex_v_ran64_v_1 , dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE); 
        ACTVALUE_TEST(lex_v_ran64_v_2 , dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran64_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#else
        ACTVALUE_TEST(lex_v_ran32_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran32_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran32_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran32_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#endif

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid                              0            st_InvalidRange
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid                            XMLCh         n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,    DONT_CARE);
        CANREP_TEST(data_rawstr_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,    XSValue::st_InvalidRange);

        CANREP_TEST(lex_v_ran64_v_1,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_v_1_canrep,  DONT_CARE);
        CANREP_TEST(lex_v_ran64_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_v_2_canrep,  DONT_CARE);

        CANREP_TEST(lex_iv_1        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);

    }

    //validation on
    CANREP_TEST(lex_v_ran64_iv_1, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran64_iv_2, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);

    //validation off
    CANREP_TEST(lex_v_ran64_iv_1, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran64_iv_1_canrep, DONT_CARE);
    CANREP_TEST(lex_v_ran64_iv_2, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran64_iv_2_canrep, DONT_CARE);
}

void test_dt_int()
{
    const XSValue::DataType dt = XSValue::dt_int; 
    bool  toValidate = true;

    const char lex_v_ran_v_0[]="1234";

    const char lex_v_ran_v_1[]="+2147483647";
    const char lex_v_ran_v_2[]="-2147483648";
    const char lex_v_ran_iv_1[]="+2147483648";
    const char lex_v_ran_iv_2[]="-2147483649";

    const char lex_v_ran_v_1_canrep[]="2147483647";
    const char lex_v_ran_v_2_canrep[]="-2147483648";
    const char lex_v_ran_iv_1_canrep[]="2147483648";
    const char lex_v_ran_iv_2_canrep[]="-2147483649";

    const char lex_iv_1[]="1234.456";
    const char lex_iv_2[]="1234b56";

/***
 * 3.3.17.2 Canonical representation
 *
 * The canonical representation for int is defined by prohibiting certain options from the 
 * Lexical representation (§3.3.17.1). Specifically, 
 * 1. the the optional "+" sign is prohibited and 
 * 2. leading zeroes are prohibited.
 *
 ***/

    const char data_rawstr_1[]="+12345";
    const char data_canrep_1[]="12345";
    const char data_rawstr_2[]="00012345";
    const char data_canrep_2[]="12345";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid                         false           st_InvalidRange   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid, valid range
    VALIDATE_TEST(lex_v_ran_v_0  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_1  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical valid, invalid range
    VALIDATE_TEST(lex_v_ran_iv_1 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran_iv_2 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_1,    dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        ACTVALUE_TEST(lex_v_ran_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid                              0            st_InvalidRange
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid                            XMLCh         n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1, dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,        DONT_CARE);
        CANREP_TEST(data_rawstr_2, dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,        XSValue::st_InvalidRange);

        CANREP_TEST(lex_v_ran_v_1, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_v_1_canrep, DONT_CARE);
        CANREP_TEST(lex_v_ran_v_2, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_v_2_canrep, DONT_CARE);

        CANREP_TEST(lex_iv_1,      dt, toValidate, EXP_RET_CANREP_FALSE, 0,                    XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2,      dt, toValidate, EXP_RET_CANREP_FALSE, 0,                    XSValue::st_InvalidChar);

    }

    //validation on
    CANREP_TEST(lex_v_ran_iv_1, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran_iv_2, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);

    //validation off
    CANREP_TEST(lex_v_ran_iv_1, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_1_canrep, DONT_CARE);
    CANREP_TEST(lex_v_ran_iv_2, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_2_canrep, DONT_CARE);
}

// 32767 
// -32768

void test_dt_short()
{
    const XSValue::DataType dt = XSValue::dt_short; 
    bool  toValidate = true;

    const char lex_v_ran_v_0[]="1234";

    const char lex_v_ran_v_1[]="+32767";
    const char lex_v_ran_v_2[]="-32768";
    const char lex_v_ran_iv_1[]="+32768";
    const char lex_v_ran_iv_2[]="-32769";

    const char lex_v_ran_v_1_canrep[]="32767";
    const char lex_v_ran_v_2_canrep[]="-32768";
    const char lex_v_ran_iv_1_canrep[]="32768";
    const char lex_v_ran_iv_2_canrep[]="-32769";

    const char lex_iv_1[]="1234.456";
    const char lex_iv_2[]="1234b56";

/***
 *
 * 3.3.18.2 Canonical representation
 *
 * The canonical representation for short is defined by prohibiting certain options from the 
 * Lexical representation (§3.3.18.1). Specifically, 
 * 1. the the optional "+" sign is prohibited and 
 * 2. leading zeroes are prohibited.
 *
 ***/

    const char data_rawstr_1[]="+12345";
    const char data_canrep_1[]="12345";
    const char data_rawstr_2[]="00012345";
    const char data_canrep_2[]="12345";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid                         false           st_InvalidRange   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid, valid range
    VALIDATE_TEST(lex_v_ran_v_0  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_1  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical valid, invalid range
    VALIDATE_TEST(lex_v_ran_iv_1 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran_iv_2 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_1,    dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        ACTVALUE_TEST(lex_v_ran_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid                              0            st_InvalidRange
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid                            XMLCh         n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1, dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,        DONT_CARE);
        CANREP_TEST(data_rawstr_2, dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,        XSValue::st_InvalidRange);

        CANREP_TEST(lex_v_ran_v_1, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_v_1_canrep, DONT_CARE);
        CANREP_TEST(lex_v_ran_v_2, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_v_2_canrep, DONT_CARE);

        CANREP_TEST(lex_iv_1,      dt, toValidate, EXP_RET_CANREP_FALSE, 0,                    XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2,      dt, toValidate, EXP_RET_CANREP_FALSE, 0,                    XSValue::st_InvalidChar);

    }

    //validation on
    CANREP_TEST(lex_v_ran_iv_1, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran_iv_2, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);

    //validation off
    CANREP_TEST(lex_v_ran_iv_1, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_1_canrep, DONT_CARE);
    CANREP_TEST(lex_v_ran_iv_2, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_2_canrep, DONT_CARE);
}

//127 
//-128

void test_dt_byte()
{
    const XSValue::DataType dt = XSValue::dt_byte; 
    bool  toValidate = true;

    const char lex_v_ran_v_0[]="12";

    const char lex_v_ran_v_1[]="+127";
    const char lex_v_ran_v_2[]="-128";
    const char lex_v_ran_iv_1[]="+128";
    const char lex_v_ran_iv_2[]="-129";

    const char lex_v_ran_v_1_canrep[]="127";
    const char lex_v_ran_v_2_canrep[]="-128";
    const char lex_v_ran_iv_1_canrep[]="128";
    const char lex_v_ran_iv_2_canrep[]="-129";

    const char lex_iv_1[]="1234.456";
    const char lex_iv_2[]="1234b56";

/***
 *
 * 3.3.19.2 Canonical representation
 *
 * The canonical representation for byte is defined by prohibiting certain options from the 
 * Lexical representation (§3.3.19.1). Specifically, 
 * 1. the the optional "+" sign is prohibited and 
 * 2. leading zeroes are prohibited.
 *
 ***/
  
    const char data_rawstr_1[]="+123";
    const char data_canrep_1[]="123";
    const char data_rawstr_2[]="000123";
    const char data_canrep_2[]="123";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid                         false           st_InvalidRange   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid, valid range
    VALIDATE_TEST(lex_v_ran_v_0  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_1  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical valid, invalid range
    VALIDATE_TEST(lex_v_ran_iv_1 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran_iv_2 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_1,    dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        ACTVALUE_TEST(lex_v_ran_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid                              0            st_InvalidRange
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid                            XMLCh         n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1, dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,        DONT_CARE);
        CANREP_TEST(data_rawstr_2, dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,        XSValue::st_InvalidRange);

        CANREP_TEST(lex_v_ran_v_1, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_v_1_canrep, DONT_CARE);
        CANREP_TEST(lex_v_ran_v_2, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_v_2_canrep, DONT_CARE);

        CANREP_TEST(lex_iv_1,      dt, toValidate, EXP_RET_CANREP_FALSE, 0,                    XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2,      dt, toValidate, EXP_RET_CANREP_FALSE, 0,                    XSValue::st_InvalidChar);

    }

    //validation on
    CANREP_TEST(lex_v_ran_iv_1, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran_iv_2, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);

    //validation off
    CANREP_TEST(lex_v_ran_iv_1, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_1_canrep, DONT_CARE);
    CANREP_TEST(lex_v_ran_iv_2, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_2_canrep, DONT_CARE);
}

void test_dt_nonNegativeInteger()
{
    const XSValue::DataType dt = XSValue::dt_nonNegativeInteger; 
    bool  toValidate = true;

    const char lex_v_ran_v_1[]="1234";
    const char lex_v_ran_iv_1[]="-1";

    const char lex_v_ran64_v_2[]="+9223372036854775807";
    const char lex_v_ran64_iv_2[]="+9223372036854775808";

    const char lex_v_ran32_v_2[]="2147483647";
    const char lex_v_ran32_iv_2[]="2147483648";

    const char lex_v_ran64_v_2_canrep[]="9223372036854775807";
    const char lex_v_ran64_iv_2_canrep[]="9223372036854775808";

    const char lex_iv_1[]="12b34.456";
    const char lex_iv_2[]="1234b56";


/***
 * 3.3.20.2 Canonical representation
 *
 * The canonical representation for nonNegativeInteger is defined by prohibiting certain options from the 
 * Lexical representation (§3.3.20.1). Specifically, 
 * 1. the the optional "+" sign is prohibited and 
 * 2. leading zeroes are prohibited.
 *
 ***/

    const char data_rawstr_1[]="0";
    const char data_canrep_1[]="0";
    const char data_rawstr_2[]="+00012345";
    const char data_canrep_2[]="12345";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid       n/a   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid
    VALIDATE_TEST(lex_v_ran_v_1    , dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(lex_v_ran_iv_1   , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    VALIDATE_TEST(lex_v_ran64_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_iv_2 , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_1,    dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

#if defined(XML_BITSTOBUILD_64)
        ACTVALUE_TEST(lex_v_ran64_v_2 , dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#else
        ACTVALUE_TEST(lex_v_ran32_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran32_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#endif

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid          n/a                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid          n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,    DONT_CARE);
        CANREP_TEST(data_rawstr_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,    DONT_CARE);

        CANREP_TEST(lex_v_ran64_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_v_2_canrep,  DONT_CARE);
        CANREP_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_iv_2_canrep, DONT_CARE);
        CANREP_TEST(lex_iv_1        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);

    }

}

//18446744073709551615
// 4294967295
void test_dt_unsignedLong()
{
    const XSValue::DataType dt = XSValue::dt_unsignedLong; 
    bool  toValidate = true;

    const char lex_v_ran_v_1[]="1234";
    const char lex_v_ran64_v_1[]="+18446744073709551615";
    const char lex_v_ran64_v_2[]="0";
    const char lex_v_ran64_iv_1[]="+18446744073709551616";
    const char lex_v_ran64_iv_2[]="-1";

    const char lex_v_ran32_v_1[]="+4294967295";
    const char lex_v_ran32_v_2[]="0";
    const char lex_v_ran32_iv_1[]="4294967296";
    const char lex_v_ran32_iv_2[]="-1";

    const char lex_v_ran64_v_1_canrep[]="18446744073709551615";
    const char lex_v_ran64_v_2_canrep[]="0";
    const char lex_v_ran64_iv_1_canrep[]="18446744073709551616";
    const char lex_v_ran64_iv_2_canrep[]="-1";

    const char lex_iv_1[]="12b34.456";
    const char lex_iv_2[]="1234b56";

/***
 * 3.3.16.2 Canonical representation
 *
 * The canonical representation for long is defined by prohibiting certain options from the 
 * Lexical representation (§3.3.16.1). Specifically, 
 * 1. the the optional "+" sign is prohibited and 
 * 2. leading zeroes are prohibited.

 ***/

    const char data_rawstr_1[]="+12345";
    const char data_canrep_1[]="12345";
    const char data_rawstr_2[]="00012345";
    const char data_canrep_2[]="12345";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid                         false           st_InvalidRange   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid, valid range
    VALIDATE_TEST(lex_v_ran_v_1    , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_v_1  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical valid, invalid range
    VALIDATE_TEST(lex_v_ran64_iv_1 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran64_iv_2 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_1,    dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

#if defined(XML_BITSTOBUILD_64)
        ACTVALUE_TEST(lex_v_ran64_v_1 , dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE); 
        ACTVALUE_TEST(lex_v_ran64_v_2 , dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran64_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#else
        ACTVALUE_TEST(lex_v_ran32_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran32_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran32_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran32_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#endif

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid                              0            st_InvalidRange
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid                            XMLCh         n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,    DONT_CARE);
        CANREP_TEST(data_rawstr_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,    XSValue::st_InvalidRange);

        CANREP_TEST(lex_v_ran64_v_1,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_v_1_canrep,  DONT_CARE);
        CANREP_TEST(lex_v_ran64_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_v_2_canrep,  DONT_CARE);

        CANREP_TEST(lex_iv_1        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);

    }

    //validation on
    CANREP_TEST(lex_v_ran64_iv_1, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran64_iv_2, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);

    //validation off
    CANREP_TEST(lex_v_ran64_iv_1, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran64_iv_1_canrep, DONT_CARE);
    CANREP_TEST(lex_v_ran64_iv_2, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran64_iv_2_canrep, DONT_CARE);
}

//4294967295
void test_dt_unsignedInt()
{
    const XSValue::DataType dt = XSValue::dt_unsignedInt; 
    bool  toValidate = true;

    const char lex_v_ran_v_0[]="1234";

    const char lex_v_ran_v_1[]="+4294967295";
    const char lex_v_ran_v_2[]="0";
    const char lex_v_ran_iv_1[]="4294967296";
    const char lex_v_ran_iv_2[]="-1";

    const char lex_v_ran_v_1_canrep[]="4294967295";
    const char lex_v_ran_v_2_canrep[]="0";
    const char lex_v_ran_iv_1_canrep[]="4294967296";
    const char lex_v_ran_iv_2_canrep[]="-1";

    const char lex_iv_1[]="12b34.456";
    const char lex_iv_2[]="1234b56";

/***
 * 3.3.22.2 Canonical representation
 *
 * The canonical representation for unsignedInt is defined by prohibiting certain options from the 
 * Lexical representation (§3.3.22.1). Specifically, 
 * leading zeroes are prohibited.
 *
 ***/

    const char data_rawstr_1[]="+12345";
    const char data_canrep_1[]="12345";
    const char data_rawstr_2[]="00012345";
    const char data_canrep_2[]="12345";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid                         false           st_InvalidRange   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid, valid range
    VALIDATE_TEST(lex_v_ran_v_0  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_1  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical valid, invalid range
    VALIDATE_TEST(lex_v_ran_iv_1 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran_iv_2 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_0,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        ACTVALUE_TEST(lex_v_ran_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid                              0            st_InvalidRange
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid                            XMLCh         n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,    DONT_CARE);
        CANREP_TEST(data_rawstr_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,    XSValue::st_InvalidRange);

        CANREP_TEST(lex_v_ran_v_1,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_v_1_canrep,  DONT_CARE);
        CANREP_TEST(lex_v_ran_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_v_2_canrep,  DONT_CARE);

        CANREP_TEST(lex_iv_1        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);

    }

    //validation on
    CANREP_TEST(lex_v_ran_iv_1, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran_iv_2, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);

    //validation off
    CANREP_TEST(lex_v_ran_iv_1, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_1_canrep, DONT_CARE);
    CANREP_TEST(lex_v_ran_iv_2, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_2_canrep, DONT_CARE);
}

//65535
void test_dt_unsignedShort()
{
    const XSValue::DataType dt = XSValue::dt_unsignedShort; 
    bool  toValidate = true;

    const char lex_v_ran_v_0[]="1234";

    const char lex_v_ran_v_1[]="+65535";
    const char lex_v_ran_v_2[]="0";
    const char lex_v_ran_iv_1[]="+65536";
    const char lex_v_ran_iv_2[]="-1";

    const char lex_v_ran_v_1_canrep[]="65535";
    const char lex_v_ran_v_2_canrep[]="0";
    const char lex_v_ran_iv_1_canrep[]="65536";
    const char lex_v_ran_iv_2_canrep[]="-1";

    const char lex_iv_1[]="12b34.456";
    const char lex_iv_2[]="1234b56";

/***
 * 3.3.23.2 Canonical representation
 *
 * The canonical representation for unsignedShort is defined by prohibiting certain options from the 
 * Lexical representation (§3.3.23.1). Specifically, 
 * 1. the leading zeroes are prohibited.
 *
 ***/

    const char data_rawstr_1[]="+12345";
    const char data_canrep_1[]="12345";
    const char data_rawstr_2[]="00012345";
    const char data_canrep_2[]="12345";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid                         false           st_InvalidRange   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid, valid range
    VALIDATE_TEST(lex_v_ran_v_0  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_1  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical valid, invalid range
    VALIDATE_TEST(lex_v_ran_iv_1 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran_iv_2 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_0,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        ACTVALUE_TEST(lex_v_ran_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid                              0            st_InvalidRange
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid                            XMLCh         n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,    DONT_CARE);
        CANREP_TEST(data_rawstr_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,    XSValue::st_InvalidRange);

        CANREP_TEST(lex_v_ran_v_1,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_v_1_canrep,  DONT_CARE);
        CANREP_TEST(lex_v_ran_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_v_2_canrep,  DONT_CARE);

        CANREP_TEST(lex_iv_1        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);

    }

    //validation on
    CANREP_TEST(lex_v_ran_iv_1, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran_iv_2, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);

    //validation off
    CANREP_TEST(lex_v_ran_iv_1, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_1_canrep, DONT_CARE);
    CANREP_TEST(lex_v_ran_iv_2, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_2_canrep, DONT_CARE);
}

// 255
void test_dt_unsignedByte()
{
    const XSValue::DataType dt = XSValue::dt_unsignedByte; 
    bool  toValidate = true;

    const char lex_v_ran_v_0[]="123";

    const char lex_v_ran_v_1[]="+255";
    const char lex_v_ran_v_2[]="0";
    const char lex_v_ran_iv_1[]="+256";
    const char lex_v_ran_iv_2[]="-1";

    const char lex_v_ran_v_1_canrep[]="255";
    const char lex_v_ran_v_2_canrep[]="0";
    const char lex_v_ran_iv_1_canrep[]="256";
    const char lex_v_ran_iv_2_canrep[]="-1";

    const char lex_iv_1[]="12b34.456";
    const char lex_iv_2[]="1234b56";

/***
 * 3.3.24.2 Canonical representation
 *
 * The canonical representation for unsignedByte is defined by prohibiting certain options from the 
 * Lexical representation (§3.3.24.1). Specifically, 
 * 1. leading zeroes are prohibited.
 *
 ***/

    const char data_rawstr_1[]="+123";
    const char data_canrep_1[]="123";
    const char data_rawstr_2[]="000123";
    const char data_canrep_2[]="123";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid                         false           st_InvalidRange   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid, valid range
    VALIDATE_TEST(lex_v_ran_v_0  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_1  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical valid, invalid range
    VALIDATE_TEST(lex_v_ran_iv_1 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);
    VALIDATE_TEST(lex_v_ran_iv_2 , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_0,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        ACTVALUE_TEST(lex_v_ran_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
        ACTVALUE_TEST(lex_v_ran_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid                              0            st_InvalidRange
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid                            XMLCh         n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,    DONT_CARE);
        CANREP_TEST(data_rawstr_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,    XSValue::st_InvalidRange);

        CANREP_TEST(lex_v_ran_v_1,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_v_1_canrep,  DONT_CARE);
        CANREP_TEST(lex_v_ran_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran_v_2_canrep,  DONT_CARE);

        CANREP_TEST(lex_iv_1        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);

    }

    //validation on
    CANREP_TEST(lex_v_ran_iv_1, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);
    CANREP_TEST(lex_v_ran_iv_2, dt, true,  EXP_RET_CANREP_FALSE, 0, XSValue::st_InvalidRange);

    //validation off
    CANREP_TEST(lex_v_ran_iv_1, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_1_canrep, DONT_CARE);
    CANREP_TEST(lex_v_ran_iv_2, dt, false, EXP_RET_CANREP_TRUE,  lex_v_ran_iv_2_canrep, DONT_CARE);
}

void test_dt_positiveInteger()
{
    const XSValue::DataType dt = XSValue::dt_positiveInteger; 
    bool  toValidate = true;

    const char lex_v_ran_v_1[]="1234";
    const char lex_v_ran_iv_1[]="0";

    const char lex_v_ran64_v_2[]="+9223372036854775807";
    const char lex_v_ran64_iv_2[]="+9223372036854775808";

    const char lex_v_ran32_v_2[]="2147483647";
    const char lex_v_ran32_iv_2[]="2147483648";

    const char lex_v_ran64_v_2_canrep[]="9223372036854775807";
    const char lex_v_ran64_iv_2_canrep[]="9223372036854775808";

    const char lex_iv_1[]="12b34.456";
    const char lex_iv_2[]="1234b56";

/***
 * 3.3.25.2 Canonical representation 
 *
 * The canonical representation for positiveInteger is defined by prohibiting certain options from the 
 * Lexical representation (§3.3.25.1). Specifically, 
 * 1. the optional "+" sign is prohibited and 
 * 2. leading zeroes are prohibited.
 *
 ***/

    const char data_rawstr_1[]="+1";
    const char data_canrep_1[]="1";
    const char data_rawstr_2[]="+00012345";
    const char data_canrep_2[]="12345";
   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid          
     *            range  valid                           true              n/a
     *            range  invalid       n/a   
     *    lexical invalid                                false           st_InvalidChar
     * 
     ***/

    // lexical valid
    VALIDATE_TEST(lex_v_ran_v_1    , dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(lex_v_ran_iv_1   , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidRange);

    VALIDATE_TEST(lex_v_ran64_v_2  , dt, EXP_RET_VALID_TRUE, DONT_CARE);
    VALIDATE_TEST(lex_v_ran64_iv_2 , dt, EXP_RET_VALID_TRUE, DONT_CARE);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);
    VALIDATE_TEST(lex_iv_2         , dt, EXP_RET_VALID_FALSE, XSValue::st_InvalidChar);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XSValue         n/a
     *          range  invalid                              0            st_Unpresentable                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        ACTVALUE_TEST(lex_v_ran_v_1,    dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

#if defined(XML_BITSTOBUILD_64)
        ACTVALUE_TEST(lex_v_ran64_v_2 , dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#else
        ACTVALUE_TEST(lex_v_ran32_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_ran32_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidRange);
#endif

        ACTVALUE_TEST(lex_iv_1,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
        ACTVALUE_TEST(lex_iv_2,         dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_InvalidChar);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid          
     *          range  valid                              XMLCh          n/a
     *          range  invalid          n/a                  
     *  lexical invalid                                     0            st_InvalidChar
     *
     *  validation off
     *  ============== 
     *  lexical valid          
     *          range  valid                              XMLCh         n/a
     *          range  invalid          n/a
     *  lexical invalid                                     0            st_InvalidChar
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j)? true : false;

        CANREP_TEST(data_rawstr_1,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_1,    DONT_CARE);
        CANREP_TEST(data_rawstr_2,    dt, toValidate, EXP_RET_CANREP_TRUE,  data_canrep_2,    DONT_CARE);

        CANREP_TEST(lex_v_ran64_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_v_2_canrep,  DONT_CARE);
        CANREP_TEST(lex_v_ran64_iv_2, dt, toValidate, EXP_RET_CANREP_TRUE,  lex_v_ran64_iv_2_canrep, DONT_CARE);
        CANREP_TEST(lex_iv_1        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);
        CANREP_TEST(lex_iv_2        , dt, toValidate, EXP_RET_CANREP_FALSE, 0,                XSValue::st_InvalidChar);

    }

}

void test_dt_boolean()
{
    const XSValue::DataType dt = XSValue::dt_boolean; 
    bool  toValidate = true;

    const char lex_v_1[]="1";
    const char lex_v_2[]="0";
    const char lex_v_3[]="true";
    const char lex_v_4[]="false";

    const char lex_iv_1[]="2";

    const char lex_v_1_canrep[]="true";
    const char lex_v_2_canrep[]="false";
    const char lex_v_3_canrep[]="true";
    const char lex_v_4_canrep[]="false";

/***
 * 3.2.2.2 Canonical representation
 *
 * The canonical representation for boolean is the set of literals {true, false}.
 *
 ***/
  
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid                                  true              n/a
     *    lexical invalid                                false           st_Invalid
     * 
     ***/

    // lexical valid
    VALIDATE_TEST(lex_v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(lex_v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(lex_v_3,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(lex_v_4,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid                                    XSValue         n/a
     *  lexical invalid                                     0            st_Invalid
     *
     *  validation off
     *  ============== 
     *  lexical valid                                    XSValue         n/a
     *  lexical invalid                                     0            st_Invalid
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        // lexical valid
        ACTVALUE_TEST(lex_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_3,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_4,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        // lexical invalid
        ACTVALUE_TEST(lex_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *  lexical valid                                    XMLCh            n/a
     *  lexical invalid                                     0            st_Invalid
     *
     *  validation off
     *  ============== 
     *  lexical valid                                    XMLCh            n/a
     *  lexical invalid                                     0            st_Invalid
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        // lexical valid
        CANREP_TEST(lex_v_1,  dt, toValidate, EXP_RET_CANREP_TRUE, lex_v_1_canrep, DONT_CARE);
        CANREP_TEST(lex_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE, lex_v_2_canrep, DONT_CARE);
        CANREP_TEST(lex_v_3,  dt, toValidate, EXP_RET_CANREP_TRUE, lex_v_3_canrep, DONT_CARE);
        CANREP_TEST(lex_v_4,  dt, toValidate, EXP_RET_CANREP_TRUE, lex_v_4_canrep, DONT_CARE);

        // lexical invalid
        CANREP_TEST(lex_iv_1, dt, toValidate, EXP_RET_CANREP_FALSE, 0,             XSValue::st_Invalid);
    }

}

void test_dt_hexBinary()
{
    const XSValue::DataType dt = XSValue::dt_hexBinary; 
    bool  toValidate = true;

    const char lex_v_1[]="0fb7";
    const char lex_v_2[]="1234";

    const char lex_iv_1[]="0gb7";
    const char lex_iv_2[]="123";

    const char lex_v_1_canrep[]="0FB7";
    const char lex_v_2_canrep[]="1234";

/***
 * 3.2.15.2 Canonical Rrepresentation
 *
 * The canonical representation for hexBinary is defined by prohibiting certain options from the 
 * Lexical Representation (§3.2.15.1). Specifically, 
 * 1. the lower case hexadecimal digits ([a-f]) are not allowed.
 *
 ***/
  
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid                                  true              n/a
     *    lexical invalid                                false           st_Invalid
     * 
     ***/

    // lexical valid
    VALIDATE_TEST(lex_v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(lex_v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(lex_iv_2, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid                                    XSValue         n/a
     *  lexical invalid                                     0            st_Invalid
     *
     *  validation off
     *  ============== 
     *  lexical valid                                    XSValue         n/a
     *  lexical invalid                                     0            st_Invalid
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        // lexical valid
        ACTVALUE_TEST(lex_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        // lexical invalid
        ACTVALUE_TEST(lex_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(lex_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  
     *  validation on
     *  ============= 
     *  lexical valid                                    XMLCh            n/a
     *  lexical invalid                                     0            st_Invalid
     *
     *  validation off
     *  ============== 
     *  lexical valid                                    XMLCh            n/a
     *  lexical invalid                                     0            st_Invalid
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        // lexical valid
        CANREP_TEST(lex_v_1,  dt, toValidate, EXP_RET_CANREP_TRUE, lex_v_1_canrep, DONT_CARE);
        CANREP_TEST(lex_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE, lex_v_2_canrep, DONT_CARE);

        // lexical invalid
        CANREP_TEST(lex_iv_1, dt, toValidate, EXP_RET_CANREP_FALSE, 0,             XSValue::st_Invalid);
        CANREP_TEST(lex_iv_2, dt, toValidate, EXP_RET_CANREP_FALSE, 0,             XSValue::st_Invalid);
    }

}

void test_dt_base64Binary()
{
    const XSValue::DataType dt = XSValue::dt_base64Binary; 
    bool  toValidate = true;

    const char lex_v_1[]="134x cv56 gui0";
    const char lex_v_2[]="wxtz 8e4k";

    const char lex_iv_2[]="134xcv56gui";
    const char lex_iv_1[]="wxtz8e4";

    const char lex_v_1_canrep[]="134xcv56gui0";
    const char lex_v_2_canrep[]="wxtz8e4k";

 
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *    lexical valid                                  true              n/a
     *    lexical invalid                                false           st_Invalid
     * 
     ***/

    // lexical valid
    VALIDATE_TEST(lex_v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(lex_v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    // lexical invalid
    VALIDATE_TEST(lex_iv_1, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(lex_iv_2, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *  lexical valid                                    XSValue         n/a
     *  lexical invalid                                     0            st_Invalid
     *
     *  validation off
     *  ============== 
     *  lexical valid                                    XSValue         n/a
     *  lexical invalid                                     0            st_Invalid
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        // lexical valid
        ACTVALUE_TEST(lex_v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(lex_v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        // lexical invalid
        ACTVALUE_TEST(lex_iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(lex_iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  
     *  validation on
     *  ============= 
     *  lexical valid                                    XMLCh            n/a
     *  lexical invalid                                     0            st_Invalid
     *
     *  validation off
     *  ============== 
     *  lexical valid                                    XMLCh            n/a
     *  lexical invalid                                     0            st_Invalid
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        // lexical valid
        CANREP_TEST(lex_v_1,  dt, toValidate, EXP_RET_CANREP_TRUE, lex_v_1_canrep, DONT_CARE);
        CANREP_TEST(lex_v_2,  dt, toValidate, EXP_RET_CANREP_TRUE, lex_v_2_canrep, DONT_CARE);

        // lexical invalid
        CANREP_TEST(lex_iv_1, dt, toValidate, EXP_RET_CANREP_FALSE, 0,             XSValue::st_Invalid);
        CANREP_TEST(lex_iv_2, dt, toValidate, EXP_RET_CANREP_FALSE, 0,             XSValue::st_Invalid);
    }

}

void test_dt_duration()
{
    const XSValue::DataType dt = XSValue::dt_duration; 
    bool  toValidate = true;

    const char v_1[]="P1Y1M1DT1H1M1S";
    const char v_2[]="P1Y1M1DT23H59M59S";
    const char v_3[]="P1Y1M1DT23H";

    const char iv_1[]="P-1Y2M3DT10H30M";
    const char iv_2[]="P1Y1M1DT1H1M1X";
    const char iv_3[]="P1Z1M1DT23H";

    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                                     false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_2, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_3, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        //  invalid
        ACTVALUE_TEST(iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_3, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_2, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_3, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));

    }

}

void test_dt_date()
{
    const XSValue::DataType dt = XSValue::dt_date; 
    bool  toValidate = true;

    const char v_1[]="1991-05-31";
    const char v_2[]="9999-06-30";
    const char v_3[]="99991-07-31";

    const char iv_1[]="2000-12-32";
    const char iv_2[]="2001-02-29";
    const char iv_3[]="2001-06-31";

    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                                     false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_2, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_3, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        //  invalid
        ACTVALUE_TEST(iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_3, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_2, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_3, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));

    }

}

void test_dt_gYearMonth()
{
    const XSValue::DataType dt = XSValue::dt_gYearMonth; 
    bool  toValidate = true;

    const char v_1[]="20000-02";
    const char v_2[]="0200-11";
    const char v_3[]="2000-02";

    const char iv_1[]="0000-12";
    const char iv_2[]="+2000-11";
    const char iv_3[]="2000.90-02";

    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                                     false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_2, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_3, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        //  invalid
        ACTVALUE_TEST(iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_3, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_2, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_3, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));

    }

}

void test_dt_gYear()
{
    const XSValue::DataType dt = XSValue::dt_gYear; 
    bool  toValidate = true;

    const char v_1[]="0001";
    const char v_2[]="9999";
    const char v_3[]="-1999";

    const char iv_1[]="0000";
    const char iv_2[]="+2000";
    const char iv_3[]="2000.90";

    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                                     false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_2, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_3, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        //  invalid
        ACTVALUE_TEST(iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_3, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_2, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_3, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));

    }

}

void test_dt_gMonthDay()
{
    const XSValue::DataType dt = XSValue::dt_gMonthDay; 
    bool  toValidate = true;

    const char v_1[]="--01-31";
    const char v_2[]="--03-31";
    const char v_3[]="--04-01";

    const char iv_1[]="--14-31";
    const char iv_2[]="--12-32";
    const char iv_3[]="--02-30";

    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                                     false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_2, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_3, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        //  invalid
        ACTVALUE_TEST(iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_3, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_2, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_3, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));

    }

}

void test_dt_gDay()
{
    const XSValue::DataType dt = XSValue::dt_gDay; 
    bool  toValidate = true;

    const char v_1[]="---31";
    const char v_2[]="---01";
    const char v_3[]="---28";

    const char iv_1[]="---+31";
    const char iv_2[]="---28.00";
    const char iv_3[]="--31";

    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                                     false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_2, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_3, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        //  invalid
        ACTVALUE_TEST(iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_3, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_2, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_3, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));

    }

}

void test_dt_gMonth()
{
    const XSValue::DataType dt = XSValue::dt_gMonth; 
    bool  toValidate = true;

    const char v_1[]="--02";
    const char v_2[]="--10";
    const char v_3[]="--12";

    const char iv_1[]="--+11";
    const char iv_2[]="---02.09";
    const char iv_3[]="--14--";

    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                                     false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_2, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_3, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        //  invalid
        ACTVALUE_TEST(iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_3, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XMLCh            st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_2, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_3, dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));

    }

}

void test_dt_dateTime()
{
    const XSValue::DataType dt = XSValue::dt_dateTime; 
    bool  toValidate = true;

    const char v_1[]="2000-12-31T23:59:59.00389";
    const char v_2[]="2000-10-01T11:10:20+06:00";
    const char v_3[]="2000-10-01T11:10:20-06:00";

    const char iv_1[]="0000-12-31T23:59:59";
    const char iv_2[]="+2000-11-30T23:59:59Z";
    const char iv_3[]="2000-02-28T23:59.1:59Z";

    const char v_1_canrep[]="2000-12-31T23:59:59.00389Z";
    const char v_2_canrep[]="2000-10-01T05:10:20Z";
    const char v_3_canrep[]="2000-10-01T17:10:20Z";

 /***
 * E2-41
 *
 *  3.2.7.2 Canonical representation
 * 
 *  Except for trailing fractional zero digits in the seconds representation, 
 *  '24:00:00' time representations, and timezone (for timezoned values), 
 *  the mapping from literals to values is one-to-one. Where there is more 
 *  than one possible representation, the canonical representation is as follows: 
 *  redundant trailing zero digits in fractional-second literals are prohibited. 
 *  An hour representation of '24' is prohibited. Timezoned values are canonically
 *  represented by appending 'Z' to the nontimezoned representation. (All 
 *  timezoned dateTime values are UTC.) 
 *
 *  .'24:00:00' -> '00:00:00'
 *  .milisecond: trailing zeros removed
 *  .'Z'
 *
 ***/

    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                                     false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_2, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_3, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        //  invalid
        ACTVALUE_TEST(iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_3, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                          XMLCh            n/a
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XMLCh            n/a
     *   invalid                                          0              st_Invalid
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_TRUE, v_1_canrep, DONT_CARE);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_TRUE, v_2_canrep, DONT_CARE);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_TRUE, v_3_canrep, DONT_CARE);

        //  invalid
        CANREP_TEST(iv_1, dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_Invalid);
        CANREP_TEST(iv_2, dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_Invalid);
        CANREP_TEST(iv_3, dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_Invalid);

    }

}

void test_dt_time()
{
    const XSValue::DataType dt = XSValue::dt_time; 
    bool  toValidate = true;

    const char v_1[]="23:59:59.38900";
    const char v_2[]="24:00:00";
    const char v_3[]="23:59:59";

    const char iv_1[]="55:59:59";
    const char iv_2[]="03:99:59";
    const char iv_3[]="23:59.1:59";

    const char v_1_canrep[]="23:59:59.389Z";
    const char v_2_canrep[]="00:00:00Z";
    const char v_3_canrep[]="23:59:59Z";

/***
 * 3.2.8.2 Canonical representation
 *
 * The canonical representation for time is defined by prohibiting certain options 
 * from the Lexical representation (§3.2.8.1). Specifically, 
 * 1. either the time zone must be omitted or, 
 * 2. if present, the time zone must be Coordinated Universal Time (UTC) 
 *    indicated by a "Z". 
 * 3. Additionally, the canonical representation for midnight is 00:00:00.
 *
 ***/


    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                                     false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_2, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);
    VALIDATE_TEST(iv_3, dt, EXP_RET_VALID_FALSE, XSValue::st_Invalid);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XSValue         n/a
     *   invalid                                           0            st_Invalid
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_TRUE,  DONT_CARE);

        //  invalid
        ACTVALUE_TEST(iv_1, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_2, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
        ACTVALUE_TEST(iv_3, dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_Invalid);
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                          XMLCh            n/a
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                          XMLCh            n/a
     *   invalid                                          0              st_Invalid
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_TRUE, v_1_canrep, DONT_CARE);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_TRUE, v_2_canrep, DONT_CARE);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_TRUE, v_3_canrep, DONT_CARE);

        //  invalid
        CANREP_TEST(iv_1, dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_Invalid);
        CANREP_TEST(iv_2, dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_Invalid);
        CANREP_TEST(iv_3, dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_Invalid);

    }

}

void test_dt_string()
{
    const XSValue::DataType dt = XSValue::dt_string; 
    bool  toValidate = true;

    const char v_1[]="mystring";

    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                     n/a             false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                             0             n/a
     *   invalid                      n/a                  0             st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                             0             n/a
     *   invalid                      n/a                  0             st_Invalid
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);

        //  invalid
    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                            0              n/a
     *   invalid                       n/a                0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                            0              n/a
     *   invalid                       n/a                0              st_Invalid
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid

    }

}

void test_dt_anyURI()
{
    const XSValue::DataType dt = XSValue::dt_anyURI; 
    bool  toValidate = true;

    const char v_1[]="http://www.schemaTest.org/IBMd3_2_17v01";
    const char v_2[]="gopher://spinaltap.micro.umn.edu/00/Weather/California/Los%20Angeles";
    const char v_3[]="ftp://www.noNamespace.edu";
  	
    const char iv_1[]="+htp://peiyongz@:90";
    const char iv_2[]=">////1.2.3.4.";
    const char iv_3[]="<///www.ibm.9om";

    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                                     false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1, dt, EXP_RET_VALID_FALSE, DONT_CARE);
    VALIDATE_TEST(iv_2, dt, EXP_RET_VALID_FALSE, DONT_CARE);
    VALIDATE_TEST(iv_3, dt, EXP_RET_VALID_FALSE, DONT_CARE);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_NoActVal
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);

        //  invalid
        ACTVALUE_TEST(iv_1,  dt, toValidate, EXP_RET_VALUE_FALSE, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_2,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate? XSValue::st_Invalid: XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_3,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate? XSValue::st_Invalid: XSValue::st_NoActVal));

    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
    }

}

void test_dt_QName()
{
    const XSValue::DataType dt = XSValue::dt_QName; 
    bool  toValidate = true;

    const char v_1[]="Ant:Eater";
    const char v_2[]="Minimum_Length";
    const char v_3[]="abcde:a2345";
  	
    const char iv_1[]="Three:Two:One";
    const char iv_2[]=":my";
    const char iv_3[]="+name";

    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                                     false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,  dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1, dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_2, dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_3, dt, EXP_RET_VALID_FALSE,  DONT_CARE);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_NoActVal
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);

        //  invalid
        ACTVALUE_TEST(iv_1,  dt, toValidate, EXP_RET_VALUE_FALSE, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_2,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate? XSValue::st_Invalid: XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_3,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate? XSValue::st_Invalid: XSValue::st_NoActVal));

    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
        CANREP_TEST(iv_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate? XSValue::st_Invalid: XSValue::st_NoCanRep));
    }

}

void test_dt_NOTATION()
{
    const XSValue::DataType dt = XSValue::dt_NOTATION; 
    bool  toValidate = true;

    const char v_1[]="http://www.ibm.com/test:notation1";
    const char iv_1[]="invaliduri:notation2";
  	
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                      n/a            false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                             0             st_NoActVal
     *   invalid                         n/a               0             st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                             0             st_NoActVal
     *   invalid                         n/a               0             st_NoActVal
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);

        //  invalid


    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                            0              st_NoCanRep
     *   invalid                            n/a           0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                            0              st_NoCanRep
     *   invalid                            n/a           0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid

    }

}

void test_dt_normalizedString()
{
    const XSValue::DataType dt = XSValue::dt_normalizedString; 
    bool  toValidate = true;
	
    const char v_1[]="4+4=8";
    const char v_2[]="a  b";
    const char v_3[]="someChars=*_-";
    
    const char iv_1[]="a\tb";
    const char iv_2[]="a\nb";
    
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                      n/a            false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_2,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_NoActVal
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);

        //  invalid
        ACTVALUE_TEST(iv_1,  dt, toValidate, EXP_RET_VALUE_FALSE, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_2,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));

    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));

    }

}

void test_dt_token()
{
    const XSValue::DataType dt = XSValue::dt_token; 
    bool  toValidate = true;
	
    const char v_1[]="4+4=8";
    const char v_2[]="Number2";
    const char v_3[]="someChars=*_-";
    
    const char iv_1[]="a\tb";
    const char iv_2[]="a\nb";
    const char iv_3[]="a  b";
    
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                      n/a            false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_2,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_3,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_NoActVal
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);

        //  invalid
        ACTVALUE_TEST(iv_1,  dt, toValidate, EXP_RET_VALUE_FALSE, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_2,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_3,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));

    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));

    }

}

void test_dt_language()
{
    const XSValue::DataType dt = XSValue::dt_language; 
    bool  toValidate = true;
	
    const char v_1[]="en-AT";
    const char v_2[]="ja";
    const char v_3[]="uk-GB";
    
    const char iv_1[]="ja_JP";
    const char iv_2[]="en+US";
    const char iv_3[]="12-en";
    
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                      n/a            false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_2,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_3,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_NoActVal
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);

        //  invalid
        ACTVALUE_TEST(iv_1,  dt, toValidate, EXP_RET_VALUE_FALSE, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_2,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_3,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));

    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));

    }

}

void test_dt_NMTOKEN()
{
    const XSValue::DataType dt = XSValue::dt_NMTOKEN; 
    bool  toValidate = true;
  
    const char v_1[]="Four:-_.";
    const char v_2[]="Zeerochert";
    const char v_3[]="007";
    
    const char iv_1[]="#board";
    const char iv_2[]="@com";
    const char iv_3[]=";abc";
    
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                      n/a            false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_2,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_3,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_NoActVal
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);

        //  invalid
        ACTVALUE_TEST(iv_1,  dt, toValidate, EXP_RET_VALUE_FALSE, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_2,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_3,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));

    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));

    }

}

void test_dt_NMTOKENS()
{
    const XSValue::DataType dt = XSValue::dt_NMTOKENS; 
    bool  toValidate = true;
  
    const char v_1[]="name1 name2 name3 ";
    const char v_2[]="Zeerochert total number";
    const char v_3[]="007 009 123";
    
    const char iv_1[]="#board";
    const char iv_2[]="@com";
    const char iv_3[]=";abc";
    
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                      n/a            false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_2,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_3,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_NoActVal
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);

        //  invalid
        ACTVALUE_TEST(iv_1,  dt, toValidate, EXP_RET_VALUE_FALSE, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_2,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_3,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));

    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));

    }

}

void test_dt_Name()
{
    const XSValue::DataType dt = XSValue::dt_Name; 
    bool  toValidate = true;
  
    const char v_1[]="Four:-_.";
    const char v_2[]="_Zeerochert";
    const char v_3[]=":007";
    
    const char iv_1[]="9name";
    const char iv_2[]="-name";
    const char iv_3[]=".name";
    
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                      n/a            false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_2,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_3,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_NoActVal
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);

        //  invalid
        ACTVALUE_TEST(iv_1,  dt, toValidate, EXP_RET_VALUE_FALSE, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_2,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_3,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));

    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));

    }

}

void test_dt_NCName_ID_IDREF_ENTITY(XSValue::DataType dt)
{
    bool  toValidate = true;
  
    const char v_1[]="Four-_.";
    const char v_2[]="_Zeerochert";
    const char v_3[]="L007";
    
    const char iv_1[]=":Four-_.";
    const char iv_2[]="_Zeerochert:";
    const char iv_3[]="0:07";

   
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                      n/a            false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_2,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_3,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_NoActVal
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);

        //  invalid
        ACTVALUE_TEST(iv_1,  dt, toValidate, EXP_RET_VALUE_FALSE, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_2,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_3,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));

    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));

    }

}

void test_dt_IDREFS_ENTITIES(XSValue::DataType dt)
{
    bool  toValidate = true;

    const char v_1[]="Four-_. Five Seven";
    const char v_2[]="_Zeerochert _Hundere Bye";
    const char v_3[]="L007 L009 L008";
    
    const char iv_1[]=":Four-_.";
    const char iv_2[]="_Zeerochert:";
    const char iv_3[]="0:07";
     
    /***    
     *
     * validate
     * ---------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *     valid                                       true              n/a
     *     invalid                      n/a            false             st_Invalid
     * 
     ***/

    //  valid
    VALIDATE_TEST(v_1,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_2,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);
    VALIDATE_TEST(v_3,   dt, EXP_RET_VALID_TRUE,  DONT_CARE);

    //  invalid
    VALIDATE_TEST(iv_1,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_2,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);
    VALIDATE_TEST(iv_3,  dt, EXP_RET_VALID_FALSE,  DONT_CARE);

    /***
     *
     * getActualValue
     * ---------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *  validation on
     *  ============= 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                             0             st_NoActVal
     *   invalid                                           0             st_NoActVal
     *
     ***/
   
    for (int i = 0; i < 2; i++)
    {
        //validation on/off
        toValidate = ( 0 == i) ? true : false;

        //  valid
        ACTVALUE_TEST(v_1,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_2,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);
        ACTVALUE_TEST(v_3,  dt, toValidate, EXP_RET_VALUE_FALSE, XSValue::st_NoActVal);

        //  invalid
        ACTVALUE_TEST(iv_1,  dt, toValidate, EXP_RET_VALUE_FALSE, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_2,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));
        ACTVALUE_TEST(iv_3,  dt, toValidate, EXP_RET_VALUE_FALSE,
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoActVal));

    }

    /***
     *
     * getCanonicalRepresentation
     * ---------------------------
     *                             availability        return value      context
     *                             ----------------------------------------------
     *
     *  validation on
     *  ============= 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_Invalid
     *
     *  validation off
     *  ============== 
     *   valid                                            0              st_NoCanRep
     *   invalid                                          0              st_NoCanRep
     *
     ***/

    for (int j = 0; j < 2; j++)
    {
        //validation on/off
        toValidate = ( 0 == j) ? true : false;

        //  valid
        CANREP_TEST(v_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);
        CANREP_TEST(v_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, XSValue::st_NoCanRep);

        //  invalid
        CANREP_TEST(iv_1,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_2,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));
        CANREP_TEST(iv_3,  dt, toValidate, EXP_RET_CANREP_FALSE, 0, 
            (toValidate ? XSValue::st_Invalid : XSValue::st_NoCanRep));

    }

}

// ---------------------------------------------------------------------------
//  Program entry point
// ---------------------------------------------------------------------------
int main(int, char* )
{

    // Initialize the XML4C system
    try
    {
        XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& toCatch)
    {
        XERCES_STD_QUALIFIER cerr << "Error during initialization! Message:\n"
            << StrX(toCatch.getMessage()) << XERCES_STD_QUALIFIER endl;
        return 1;
    }

    test_dt_string();              
    test_dt_boolean();
    test_dt_decimal();
    test_dt_float();
    test_dt_double();
    test_dt_duration();
    test_dt_dateTime();
    test_dt_time();
    test_dt_date();
    test_dt_gYearMonth();
    test_dt_gYear();
    test_dt_gMonthDay();
    test_dt_gDay();
    test_dt_gMonth();
    test_dt_hexBinary();
    test_dt_base64Binary();
    test_dt_anyURI();
    test_dt_QName();
    test_dt_NOTATION();
    test_dt_normalizedString();
    test_dt_token();
    test_dt_language();
    test_dt_NMTOKEN();
    test_dt_NMTOKENS();
    test_dt_Name();    
    test_dt_NCName_ID_IDREF_ENTITY(XSValue::dt_NCName);
    test_dt_NCName_ID_IDREF_ENTITY(XSValue::dt_ID);
    test_dt_NCName_ID_IDREF_ENTITY(XSValue::dt_IDREF);
    test_dt_IDREFS_ENTITIES(XSValue::dt_IDREFS);
    test_dt_NCName_ID_IDREF_ENTITY(XSValue::dt_ENTITY);
    test_dt_IDREFS_ENTITIES(XSValue::dt_ENTITIES);   
    test_dt_integer();
    test_dt_nonPositiveInteger();
    test_dt_negativeInteger();
    test_dt_long();
    test_dt_int();
    test_dt_short();
    test_dt_byte();
    test_dt_nonNegativeInteger();
    test_dt_unsignedLong();
    test_dt_unsignedInt();
    test_dt_unsignedShort();
    test_dt_unsignedByte();
    test_dt_positiveInteger();

    printf("\nXSValueTest %s\n", errSeen? "Fail" : "Pass");

    // And call the termination method
    XMLPlatformUtils::Terminate();

    return 0;
}

