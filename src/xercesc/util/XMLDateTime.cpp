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
 * Revision 1.7  2003/02/22 22:49:09  peiyongz
 * Schema Errata E2-45 24:00:00 allowed
 *
 * Revision 1.6  2003/02/02 23:54:43  peiyongz
 * getFormattedString() added to return original and converted value.
 *
 * Revision 1.5  2003/01/30 21:55:22  tng
 * Performance: create getRawData which is similar to toString but return the internal data directly, user is not required to delete the returned memory.
 *
 * Revision 1.4  2002/11/28 20:39:27  peiyongz
 * Schema Errata: E2-23 seconds part shall have at least one digit after the dot
 *                           if it appears.
 *
 * Revision 1.3  2002/11/06 22:22:21  peiyongz
 * Schema-Errata: E2-12: gMonth
 *
 * Revision 1.2  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:14  peiyongz
 * sane_include
 *
 * Revision 1.4  2001/11/14 22:04:03  peiyongz
 * Patch to apply check on Year and more rigorous on other fields as well.
 *
 * Revision 1.3  2001/11/12 20:36:54  peiyongz
 * SchemaDateTimeException defined
 *
 * Revision 1.2  2001/11/09 20:41:45  peiyongz
 * Fix: compilation error on Solaris and AIX.
 *
 * Revision 1.1  2001/11/07 19:16:03  peiyongz
 * DateTime Port
 *
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <stdlib.h>
#include <xercesc/util/XMLDateTime.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/Janitor.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
// constants used to process raw data (fBuffer)
//
// [-]{CCYY-MM-DD}'T'{HH:MM:SS.MS}['Z']
//                                [{+|-}hh:mm']
//

static const XMLCh DURATION_STARTER     = chLatin_P;              // 'P'
static const XMLCh DURATION_Y           = chLatin_Y;              // 'Y'
static const XMLCh DURATION_M           = chLatin_M;              // 'M'
static const XMLCh DURATION_D           = chLatin_D;              // 'D'
static const XMLCh DURATION_H           = chLatin_H;              // 'H'
static const XMLCh DURATION_S           = chLatin_S;              // 'S'

static const XMLCh DATE_SEPARATOR       = chDash;                 // '-'
static const XMLCh TIME_SEPARATOR       = chColon;                // ':'
static const XMLCh TIMEZONE_SEPARATOR   = chColon;                // ':'
static const XMLCh DATETIME_SEPARATOR   = chLatin_T;              // 'T'
static const XMLCh MILISECOND_SEPARATOR = chPeriod;               // '.'

static const XMLCh UTC_STD_CHAR         = chLatin_Z;              // 'Z'
static const XMLCh UTC_POS_CHAR         = chPlus;                 // '+'
static const XMLCh UTC_NEG_CHAR         = chDash;                 // '-'

static const XMLCh UTC_SET[]            = {UTC_STD_CHAR           //"Z+-"
                                         , UTC_POS_CHAR
                                         , UTC_NEG_CHAR
                                         , chNull};

static const int YMD_MIN_SIZE    = 10;   // CCYY-MM-DD
static const int YMONTH_MIN_SIZE = 7;    // CCYY_MM
static const int TIME_MIN_SIZE   = 8;    // hh:mm:ss
static const int TIMEZONE_SIZE   = 5;    // hh:mm
static const int DAY_SIZE        = 5;    // ---DD
//static const int MONTH_SIZE      = 6;    // --MM--
static const int MONTHDAY_SIZE   = 7;    // --MM-DD
static const int NOT_FOUND       = -1;

//define constants to be used in assigning default values for
//all date/time excluding duration
static const int YEAR_DEFAULT  = 2000;
static const int MONTH_DEFAULT = 01;
static const int DAY_DEFAULT   = 15;

// order-relation on duration is a partial order. The dates below are used to
// for comparison of 2 durations, based on the fact that
// duration x and y is x<=y iff s+x<=s+y
// see 3.2.6 duration W3C schema datatype specs
//
// the dates are in format: {CCYY,MM,DD, H, S, M, MS, timezone}
static const int DATETIMES[][XMLDateTime::TOTAL_SIZE] =
{
    {1696, 9, 1, 0, 0, 0, 0, XMLDateTime::UTC_STD},
	{1697, 2, 1, 0, 0, 0, 0, XMLDateTime::UTC_STD},
	{1903, 3, 1, 0, 0, 0, 0, XMLDateTime::UTC_STD},
	{1903, 7, 1, 0, 0, 0, 0, XMLDateTime::UTC_STD}
};

// ---------------------------------------------------------------------------
//  local methods
// ---------------------------------------------------------------------------
static inline int fQuotient(int a, int b)
{
    div_t div_result = div(a, b);
    return div_result.quot;
}

static inline int fQuotient(int temp, int low, int high)
{
    return fQuotient(temp - low, high - low);
}

static inline int mod(int a, int b, int quotient)
{
	return (a - quotient*b) ;
}

static inline int modulo (int temp, int low, int high)
{
    //modulo(a - low, high - low) + low
    int a = temp - low;
    int b = high - low;
    return (mod (a, b, fQuotient(a, b)) + low) ;
}

static inline bool isLeapYear(int year)
{
    return((year%4 == 0) && ((year%100 != 0) || (year%400 == 0)));
}

static int maxDayInMonthFor(int year, int month)
{

    if ( month == 4 || month == 6 || month == 9 || month == 11 )
    {
        return 30;
    }
    else if ( month==2 )
    {
        if ( isLeapYear(year) )
            return 29;
        else
            return 28;
    }
    else
    {
        return 31;
    }

}

// ---------------------------------------------------------------------------
//  static methods : for duration
// ---------------------------------------------------------------------------
/**
 * Compares 2 given durations. (refer to W3C Schema Datatypes "3.2.6 duration")
 *
 * 3.2.6.2 Order relation on duration
 *
 *     In general, the order-relation on duration is a partial order since there is no
 *  determinate relationship between certain durations such as one month (P1M) and 30 days (P30D).
 *  The order-relation of two duration values x and y is x < y iff s+x < s+y for each qualified
 *  dateTime s in the list below.
 *
 *     These values for s cause the greatest deviations in the addition of dateTimes and durations
 *
 **/
int XMLDateTime::compare(const XMLDateTime* const pDate1
                       , const XMLDateTime* const pDate2
                       , bool  strict)
{
    //REVISIT: this is unoptimazed vs of comparing 2 durations
    //         Algorithm is described in 3.2.6.2 W3C Schema Datatype specs
    //

    int resultA, resultB = INDETERMINATE;

    //try and see if the objects are equal
    if ( (resultA = compareOrder(pDate1, pDate2)) == EQUAL)
        return EQUAL;

    //long comparison algorithm is required
    XMLDateTime tempA, *pTempA = &tempA;
    XMLDateTime tempB, *pTempB = &tempB;

    addDuration(pTempA, pDate1, 0);
    addDuration(pTempB, pDate2, 0);
    resultA = compareOrder(pTempA, pTempB);
    if ( resultA == INDETERMINATE )
        return INDETERMINATE;

    addDuration(pTempA, pDate1, 1);
    addDuration(pTempB, pDate2, 1);
    resultB = compareOrder(pTempA, pTempB);
    resultA = compareResult(resultA, resultB, strict);
    if ( resultA == INDETERMINATE )
        return INDETERMINATE;

    addDuration(pTempA, pDate1, 2);
    addDuration(pTempB, pDate2, 2);
    resultB = compareOrder(pTempA, pTempB);
    resultA = compareResult(resultA, resultB, strict);
    if ( resultA == INDETERMINATE )
        return INDETERMINATE;

    addDuration(pTempA, pDate1, 3);
    addDuration(pTempB, pDate2, 3);
    resultB = compareOrder(pTempA, pTempB);
    resultA = compareResult(resultA, resultB, strict);

    return resultA;

}

//
// Form a new XMLDateTime with duration and baseDate array
// Note: C++        Java
//       fNewDate   duration
//       fDuration  date
//

void XMLDateTime::addDuration(XMLDateTime*             fNewDate
                            , const XMLDateTime* const fDuration
                            , int index)

{

    //REVISIT: some code could be shared between normalize() and this method,
    //         however is it worth moving it? The structures are different...
    //

    fNewDate->reset();
    //add months (may be modified additionaly below)
    int temp = DATETIMES[index][Month] + fDuration->fValue[Month];
    fNewDate->fValue[Month] = modulo(temp, 1, 13);
    int carry = fQuotient(temp, 1, 13);

    //add years (may be modified additionaly below)
    fNewDate->fValue[CentYear] = DATETIMES[index][CentYear] + fDuration->fValue[CentYear] + carry;

    //add seconds
    temp = DATETIMES[index][Second] + fDuration->fValue[Second];
    carry = fQuotient (temp, 60);
    fNewDate->fValue[Second] =  mod(temp, 60, carry);
		
    //add minutes
    temp = DATETIMES[index][Minute] + fDuration->fValue[Minute] + carry;
    carry = fQuotient(temp, 60);
    fNewDate->fValue[Minute] = mod(temp, 60, carry);

    //add hours
    temp = DATETIMES[index][Hour] + fDuration->fValue[Hour] + carry;
    carry = fQuotient(temp, 24);
    fNewDate->fValue[Hour] = mod(temp, 24, carry);
		
    fNewDate->fValue[Day] = DATETIMES[index][Day] + fDuration->fValue[Day] + carry;

    while ( true )
    {
        temp = maxDayInMonthFor(fNewDate->fValue[CentYear], fNewDate->fValue[Month]);
        if ( fNewDate->fValue[Day] < 1 )
        { //original fNewDate was negative
            fNewDate->fValue[Day] += maxDayInMonthFor(fNewDate->fValue[CentYear], fNewDate->fValue[Month]-1);
            carry = -1;
        }
        else if ( fNewDate->fValue[Day] > temp )
        {
            fNewDate->fValue[Day] -= temp;
            carry = 1;
        }
        else
        {
            break;
        }

        temp = fNewDate->fValue[Month] + carry;
        fNewDate->fValue[Month] = modulo(temp, 1, 13);
        fNewDate->fValue[CentYear] += fQuotient(temp, 1, 13);
    }

    //fNewDate->fValue[utc] = UTC_STD_CHAR;
    fNewDate->fValue[utc] = UTC_STD;
}

int XMLDateTime::compareResult(short resultA
                             , short resultB
                             , bool strict)
{

    if ( resultB == INDETERMINATE )
    {
        return INDETERMINATE;
    }
    else if ( (resultA != resultB) &&
              strict                )
    {
        return INDETERMINATE;
    }
    else if ( (resultA != resultB) &&
              !strict               )
    {
        if ( (resultA != EQUAL) &&
             (resultB != EQUAL)  )
        {
            return INDETERMINATE;
        }
        else
        {
            return (resultA != EQUAL)? resultA : resultB;
        }
    }

    return resultA;
	
}

// ---------------------------------------------------------------------------
//  static methods : for others
// ---------------------------------------------------------------------------
int XMLDateTime::compare(const XMLDateTime* const pDate1
                       , const XMLDateTime* const pDate2)
{

    if (pDate1->fValue[utc] == pDate2->fValue[utc])
    {
        return XMLDateTime::compareOrder(pDate1, pDate2);
    }

    short c1, c2;

    if ( pDate1->isNormalized())
    {
        c1 = compareResult(pDate1, pDate2, false, UTC_POS);
        c2 = compareResult(pDate1, pDate2, false, UTC_NEG);
        return getRetVal(c1, c2);
    }
    else if ( pDate2->isNormalized())
    {
        c1 = compareResult(pDate1, pDate2, true, UTC_POS);
        c2 = compareResult(pDate1, pDate2, true, UTC_NEG);
        return getRetVal(c1, c2);
    }

    return INDETERMINATE;	
}

int XMLDateTime::compareResult(const XMLDateTime* const pDate1
                             , const XMLDateTime* const pDate2
                             , bool  set2Left
                             , int   utc_type)
{
    XMLDateTime tmpDate = (set2Left ? *pDate1 : *pDate2);

    tmpDate.fTimeZone[hh] = 14;
    tmpDate.fTimeZone[mm] = 0;
    tmpDate.fValue[utc] = utc_type;
    tmpDate.normalize();

    return (set2Left? XMLDateTime::compareOrder(&tmpDate, pDate2) :
                      XMLDateTime::compareOrder(pDate1, &tmpDate));
}

int XMLDateTime::compareOrder(const XMLDateTime* const lValue
                            , const XMLDateTime* const rValue)
{
    //
    // If any of the them is not normalized() yet,
    // we need to do something here.
    //
    XMLDateTime lTemp = *lValue;
    XMLDateTime rTemp = *rValue;

    lTemp.normalize();
    rTemp.normalize();

    for ( int i = 0 ; i < TOTAL_SIZE; i++ )
    {
        if ( lTemp.fValue[i] < rTemp.fValue[i] )
        {
            return LESS_THAN;
        }
        else if ( lTemp.fValue[i] > rTemp.fValue[i] )
        {
            return GREATER_THAN;
        }
    }

    return EQUAL;
}

// ---------------------------------------------------------------------------
//  ctor and dtor
// ---------------------------------------------------------------------------
XMLDateTime::~XMLDateTime()
{
    if (fBuffer)
        delete[] fBuffer;
}

XMLDateTime::XMLDateTime()
:fBuffer(0)
{
    reset();
}

XMLDateTime::XMLDateTime(const XMLCh* const aString)
:fBuffer(0)
{
    setBuffer(aString);
}

// -----------------------------------------------------------------------
// Copy ctor and Assignment operators
// -----------------------------------------------------------------------

XMLDateTime::XMLDateTime(const XMLDateTime &toCopy)
:fBuffer(0)
{
    copy(toCopy);
}

XMLDateTime& XMLDateTime::operator=(const XMLDateTime& rhs)
{
    if (this == &rhs)
        return *this;

    copy(rhs);
    return *this;
}

// -----------------------------------------------------------------------
// Implementation of Abstract Interface
// -----------------------------------------------------------------------

//
// We may simply return the handle to fBuffer, but
// for the sake of consistency, we return a duplicated copy
// and the caller is responsible for the release of the buffer
// just like any other things in the XMLNumber family.
//
XMLCh*  XMLDateTime::toString() const
{
    assertBuffer();

    XMLCh* retBuf = XMLString::replicate(fBuffer);
    return retBuf;
}

//
// We may simply return the handle to fBuffer
//
XMLCh*  XMLDateTime::getRawData() const
{
    assertBuffer();    
    return fBuffer;
}


const XMLCh*  XMLDateTime::getFormattedString() const
{
    return getRawData();
}

int XMLDateTime::getSign() const
{
    return 0;
}

// ---------------------------------------------------------------------------
//  Parsers
// ---------------------------------------------------------------------------

//
// [-]{CCYY-MM-DD}'T'{HH:MM:SS.MS}[TimeZone]
//
void XMLDateTime::parseDateTime()
{
    initParser();
    getDate();

    //fStart is supposed to point to 'T'
    if (fBuffer[fStart++] != DATETIME_SEPARATOR)
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_dt_missingT
                , fBuffer);

    getTime();
    validateDateTime();
    normalize();
}

//
// [-]{CCYY-MM-DD}[TimeZone]
//
void XMLDateTime::parseDate()
{
    initParser();
    getDate();
    parseTimeZone();
    validateDateTime();
    normalize();
}

void XMLDateTime::parseTime()
{
    initParser();

    // time initialize to default values
    fValue[CentYear]= YEAR_DEFAULT;
    fValue[Month]   = MONTH_DEFAULT;
    fValue[Day]     = DAY_DEFAULT;

    getTime();

    validateDateTime();
    normalize();
}

//
// {---DD}[TimeZone]
//  01234
//
void XMLDateTime::parseDay()
{
    initParser();

    if (fBuffer[0] != DATE_SEPARATOR ||
        fBuffer[1] != DATE_SEPARATOR ||
        fBuffer[2] != DATE_SEPARATOR  )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_gDay_invalid
                , fBuffer);
    }

    //initialize values
    fValue[CentYear] = YEAR_DEFAULT;
    fValue[Month]    = MONTH_DEFAULT;
    fValue[Day]      = parseInt(fStart+3, fStart+5);

    if ( DAY_SIZE < fEnd )
    {
        int sign = findUTCSign(DAY_SIZE);
        if ( sign < 0 )
        {
            ThrowXML1(SchemaDateTimeException
                    , XMLExcepts::DateTime_gDay_invalid
                    , fBuffer);
        }
        else
        {
            getTimeZone(sign);
        }
    }

    validateDateTime();
    normalize();
}

//
// {--MM--}[TimeZone]
// {--MM}[TimeZone]
//  012345
//
void XMLDateTime::parseMonth()
{
    initParser();

    if (fBuffer[0] != DATE_SEPARATOR ||
        fBuffer[1] != DATE_SEPARATOR  )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_gMth_invalid
                , fBuffer);
    }

    //set constants
    fValue[CentYear] = YEAR_DEFAULT;
    fValue[Day]      = DAY_DEFAULT;
    fValue[Month]    = parseInt(2, 4);

    // REVISIT: allow both --MM and --MM-- now. 
    // need to remove the following lines to disallow --MM-- 
    // when the errata is officially in the rec. 
    fStart = 4;
    if ( fEnd >= fStart+2 && fBuffer[fStart] == DATE_SEPARATOR && fBuffer[fStart+1] == DATE_SEPARATOR ) 
    { 
        fStart += 2; 
    } 

    //
    // parse TimeZone if any
    //
    if ( fStart < fEnd )
    {
        int sign = findUTCSign(fStart);
        if ( sign < 0 )
        {
            ThrowXML1(SchemaDateTimeException
                    , XMLExcepts::DateTime_gMth_invalid
                    , fBuffer);
        }
        else
        {
            getTimeZone(sign);
        }
    }

    validateDateTime();
    normalize();
}

//
//[-]{CCYY}[TimeZone]
// 0  1234
//
void XMLDateTime::parseYear()
{
    initParser();

    // skip the first '-' and search for timezone
    //
    int sign = findUTCSign((fBuffer[0] == chDash) ? 1 : 0);

    if (sign == NOT_FOUND)
    {
        fValue[CentYear] = parseIntYear(fEnd);
    }
    else
    {
        fValue[CentYear] = parseIntYear(sign);
        getTimeZone(sign);
    }

    //initialize values
    fValue[Month] = MONTH_DEFAULT;
    fValue[Day]   = DAY_DEFAULT;   //java is 1

    validateDateTime();
    normalize();
}

//
//{--MM-DD}[TimeZone]
// 0123456
//
void XMLDateTime::parseMonthDay()
{
    initParser();

    if (fBuffer[0] != DATE_SEPARATOR ||
        fBuffer[1] != DATE_SEPARATOR ||
        fBuffer[4] != DATE_SEPARATOR )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_gMthDay_invalid
                , fBuffer);
    }


    //initialize
    fValue[CentYear] = YEAR_DEFAULT;
    fValue[Month]    = parseInt(2, 4);	
    fValue[Day]      = parseInt(5, 7);

    if ( MONTHDAY_SIZE < fEnd )
    {
        int sign = findUTCSign(MONTHDAY_SIZE);
        if ( sign<0 )
        {
            ThrowXML1(SchemaDateTimeException
                    , XMLExcepts::DateTime_gMthDay_invalid
                    , fBuffer);
        }
        else
        {
            getTimeZone(sign);
        }
    }

    validateDateTime();
    normalize();
}

void XMLDateTime::parseYearMonth()
{
    initParser();

    // get date
    getYearMonth();
    fValue[Day] = DAY_DEFAULT;
    parseTimeZone();

    validateDateTime();
    normalize();
}

//
//PnYn MnDTnH nMnS: -P1Y2M3DT10H30M
//
// [-]{'P'{[n'Y'][n'M'][n'D']['T'][n'H'][n'M'][n'S']}}
//
//  Note: the n above shall be >= 0
//        if no time element found, 'T' shall be absent
//
void XMLDateTime::parseDuration()
{
    initParser();

    // must start with '-' or 'P'
    //
    XMLCh c = fBuffer[fStart++];
    if ( (c != DURATION_STARTER) &&
         (c != chDash)            )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_dur_Start_dashP
                , fBuffer);
    }

    // 'P' must ALWAYS be present in either case
    if ( (c == chDash) &&
         (fBuffer[fStart++]!= DURATION_STARTER ))
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_dur_noP
                , fBuffer);
    }

    // java code
    //date[utc]=(c=='-')?'-':0;
    //fValue[utc] = UTC_STD;
    fValue[utc] = (fBuffer[0] == chDash? UTC_NEG : UTC_STD);

    int negate = ( fBuffer[0] == chDash ? -1 : 1);

    //
    // No negative value is allowed after 'P'
    //
    // eg P-1234, invalid
    //
    if (indexOf(fStart, fEnd, chDash) != NOT_FOUND)
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_dur_DashNotFirst
                , fBuffer);
    }

    //at least one number and designator must be seen after P
    bool designator = false;

    int endDate = indexOf(fStart, fEnd, DATETIME_SEPARATOR);
    if ( endDate == NOT_FOUND )
    {
        endDate = fEnd;  // 'T' absent
    }

    //find 'Y'
    int end = indexOf(fStart, endDate, DURATION_Y);
    if ( end != NOT_FOUND )
    {
        //scan year
        fValue[CentYear] = negate * parseInt(fStart, end);
        fStart = end+1;
        designator = true;
    }

    end = indexOf(fStart, endDate, DURATION_M);
    if ( end != NOT_FOUND )
    {
        //scan month
        fValue[Month] = negate * parseInt(fStart, end);
        fStart = end+1;
        designator = true;
    }

    end = indexOf(fStart, endDate, DURATION_D);
    if ( end != NOT_FOUND )
    {
        //scan day
        fValue[Day] = negate * parseInt(fStart,end);
        fStart = end+1;
        designator = true;
    }

    if ( (fEnd == endDate) &&   // 'T' absent
         (fStart != fEnd)   )   // something after Day
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_dur_inv_b4T
                , fBuffer);
    }

    if ( fEnd != endDate ) // 'T' present
    {
        //scan hours, minutes, seconds
        //

        // skip 'T' first
        end = indexOf(++fStart, fEnd, DURATION_H);
        if ( end != NOT_FOUND )
        {
            //scan hours
            fValue[Hour] = negate * parseInt(fStart, end);
            fStart = end+1;
            designator = true;
        }

        end = indexOf(fStart, fEnd, DURATION_M);
        if ( end != NOT_FOUND )
        {
            //scan min
            fValue[Minute] = negate * parseInt(fStart, end);
            fStart = end+1;
            designator = true;
        }

        end = indexOf(fStart, fEnd, DURATION_S);
        if ( end != NOT_FOUND )
        {
            //scan seconds
            int mlsec = indexOf (fStart, end, MILISECOND_SEPARATOR);

            /***
             * Schema Errata: E2-23
             * at least one digit must follow the decimal point if it appears. 
             * That is, the value of the seconds component must conform 
             * to the following pattern: [0-9]+(.[0-9]+)? 
             */
            if ( mlsec != NOT_FOUND )
            {
                /***
                 * make usure there is something after the '.' and before the end.
                 */
                if ( mlsec+1 == end )
                {
                    ThrowXML1(SchemaDateTimeException
                            , XMLExcepts::DateTime_dur_inv_seconds
                            ,fBuffer);
                }

                fValue[Second]     = negate * parseInt(fStart, mlsec);
                fValue[MiliSecond] = negate * parseInt(mlsec+1, end);
            }
            else
            {
                fValue[Second] = negate * parseInt(fStart,end);
            }

            fStart = end+1;
            designator = true;
        }

        // no additional data should appear after last item
        // P1Y1M1DT is illigal value as well
        if ( (fStart != fEnd) ||
              fBuffer[--fStart] == DATETIME_SEPARATOR )
        {
            ThrowXML1(SchemaDateTimeException
                    , XMLExcepts::DateTime_dur_NoTimeAfterT
                    ,fBuffer);
        }
    }

    if ( !designator )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_dur_NoElementAtAll
                , fBuffer);
    }

}

// ---------------------------------------------------------------------------
//  Scanners
// ---------------------------------------------------------------------------

//
// [-]{CCYY-MM-DD}
//
// Note: CCYY could be more than 4 digits
//       Assuming fStart point to the beginning of the Date Section
//       fStart updated to point to the position right AFTER the second 'D'
//       Since the lenght of CCYY might be variable, we can't check format upfront
//
void XMLDateTime::getDate()
{

    // Ensure enough chars in buffer
    if ( (fStart+YMD_MIN_SIZE) > fEnd)
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_date_incomplete
                , fBuffer);

    getYearMonth();    // Scan YearMonth and
                       // fStart point to the next '-'

    if (fBuffer[fStart++] != DATE_SEPARATOR)
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_date_invalid
                , fBuffer);
        //("CCYY-MM must be followed by '-' sign");
    }

    fValue[Day] = parseInt(fStart, fStart+2);
    fStart += 2 ;  //fStart points right after the Day

    return;
}

//
// hh:mm:ss[.msssss]['Z']
// hh:mm:ss[.msssss][['+'|'-']hh:mm]
// 012345678
//
// Note: Assuming fStart point to the beginning of the Time Section
//       fStart updated to point to the position right AFTER the second 's'
//                                                  or ms if any
//
void XMLDateTime::getTime()
{

    // Ensure enough chars in buffer
    if ( (fStart+TIME_MIN_SIZE) > fEnd)
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_time_incomplete
                , fBuffer);
        //"Imcomplete Time Format"

    // check (fixed) format first
    if ((fBuffer[fStart + 2] != TIME_SEPARATOR) ||
        (fBuffer[fStart + 5] != TIME_SEPARATOR)  )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_time_invalid
                , fBuffer);
        //("Error in parsing time" );
    }

    //
    // get hours, minute and second
    //
    fValue[Hour]   = parseInt(fStart + 0, fStart + 2);
    fValue[Minute] = parseInt(fStart + 3, fStart + 5);
    fValue[Second] = parseInt(fStart + 6, fStart + 8);
    fStart += 8;

    // to see if any ms and/or utc part after that
    if (fStart >= fEnd)
        return;

    //find UTC sign if any
    int sign = findUTCSign(fStart);

    //parse miliseconds
    int milisec = (fBuffer[fStart] == MILISECOND_SEPARATOR)? fStart : NOT_FOUND;
    if ( milisec != NOT_FOUND )
    {
        fStart++;   // skip the '.'
        // make sure we have some thing between the '.' and fEnd
        if (fStart >= fEnd)
        {
            ThrowXML1(SchemaDateTimeException
                    , XMLExcepts::DateTime_ms_noDigit
                    , fBuffer);
            //("ms shall be present once '.' is present" );
        }

        if ( sign == NOT_FOUND )
        {
            fValue[MiliSecond] = parseInt(fStart, fEnd);  //get ms between '.' and fEnd
            fStart = fEnd;
        }
        else
        {
            fValue[MiliSecond] = parseInt(fStart, sign);  //get ms between UTC sign and fEnd
        }
	}

    //parse UTC time zone (hh:mm)
    if ( sign > 0 ) {
        getTimeZone(sign);
    }

}

//
// [-]{CCYY-MM}
//
// Note: CCYY could be more than 4 digits
//       fStart updated to point AFTER the second 'M' (probably meet the fEnd)
//
void XMLDateTime::getYearMonth()
{

    // Ensure enough chars in buffer
    if ( (fStart+YMONTH_MIN_SIZE) > fEnd)
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_ym_incomplete
                , fBuffer);
        //"Imcomplete YearMonth Format";

    // skip the first leading '-'
    int start = ( fBuffer[0] == chDash ) ? fStart + 1 : fStart;

    //
    // search for year separator '-'
    //
    int yearSeparator = indexOf(start, fEnd, DATE_SEPARATOR);
    if ( yearSeparator == NOT_FOUND)
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_ym_invalid
                , fBuffer);
        //("Year separator is missing or misplaced");

    fValue[CentYear] = parseIntYear(yearSeparator);
    fStart = yearSeparator + 1;  //skip the '-' and point to the first M

    //
    //gonna check we have enough byte for month
    //
    if ((fStart + 2) > fEnd )
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_ym_noMonth
                , fBuffer);
        //"no month in buffer"

    fValue[Month] = parseInt(fStart, yearSeparator + 3);
    fStart += 2;  //fStart points right after the MONTH

    return;
}

void XMLDateTime::parseTimeZone()
{
    if ( fStart < fEnd )
    {
        int sign = findUTCSign(fStart);
        if ( sign < 0 )
        {
            ThrowXML1(SchemaDateTimeException
                    , XMLExcepts::DateTime_tz_noUTCsign
                    , fBuffer);
            //("Error in month parsing");
        }
        else
        {
            getTimeZone(sign);
        }
    }

    return;
}

//
// 'Z'
// ['+'|'-']hh:mm
//
// Note: Assuming fStart points to the beginning of TimeZone section
//       fStart updated to meet fEnd
//
void XMLDateTime::getTimeZone(const int sign)
{

    if ( fBuffer[sign] == UTC_STD_CHAR )
    {
        if ((sign + 1) != fEnd )
        {
            ThrowXML1(SchemaDateTimeException
                    , XMLExcepts::DateTime_tz_stuffAfterZ
                    , fBuffer);
            //"Error in parsing time zone");
        }		

        return;	
    }

    //
    // otherwise, it has to be this format
    // '[+|-]'hh:mm
    //    1   23456 7
    //   sign      fEnd
    //
    if ( ( ( sign + TIMEZONE_SIZE + 1) != fEnd )      ||
         ( fBuffer[sign + 3] != TIMEZONE_SEPARATOR ) )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_tz_invalid
                , fBuffer);
        //("Error in parsing time zone");
    }

    fTimeZone[hh] = parseInt(sign+1, sign+3);		
    fTimeZone[mm] = parseInt(sign+4, fEnd);
        		
    return;
}

// ---------------------------------------------------------------------------
//  Validator and normalizer
// ---------------------------------------------------------------------------

/**
 * If timezone present - normalize dateTime  [E Adding durations to dateTimes]
 *
 * @param date   CCYY-MM-DDThh:mm:ss+03
 * @return CCYY-MM-DDThh:mm:ssZ
 */
void XMLDateTime::normalize()
{

    if ((fValue[utc] == UTC_UNKNOWN) ||
        (fValue[utc] == UTC_STD)      )
        return;

    int negate = (fValue[utc] == UTC_POS)? -1: 1;

    // add mins
    int temp = fValue[Minute] + negate * fTimeZone[mm];
    int carry = fQuotient(temp, 60);
    fValue[Minute] = mod(temp, 60, carry);

    //add hours
    temp = fValue[Hour] + negate * fTimeZone[hh] + carry;
    carry = fQuotient(temp, 24);
    fValue[Hour] = mod(temp, 24, carry);

    fValue[Day] += carry;

    while (1)
    {
        temp = maxDayInMonthFor(fValue[CentYear], fValue[Month]);
        if (fValue[Day] < 1)
        {
            fValue[Day] += maxDayInMonthFor(fValue[CentYear], fValue[Month] - 1);
            carry = -1;
        }
        else if ( fValue[Day] > temp )
        {
            fValue[Day] -= temp;
            carry = 1;
        }
        else
        {
            break;
        }

        temp = fValue[Month] + carry;
        fValue[Month] = modulo(temp, 1, 13);
        fValue[CentYear] += fQuotient(temp, 1, 13);
    }

    // set to normalized
    fValue[utc] = UTC_STD;

    return;
}

void XMLDateTime::validateDateTime() const
{

    //REVISIT: should we throw an exception for not valid dates
    //          or reporting an error message should be sufficient?
    if ( fValue[CentYear] == 0 )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_year_zero
                , fBuffer);
        //"The year \"0000\" is an illegal year value");
    }

    if ( fValue[Month] < 1  ||
         fValue[Month] > 12  )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_mth_invalid
                , fBuffer);
		//"The month must have values 1 to 12");
    }

    //validate days
    if ( fValue[Day] > maxDayInMonthFor( fValue[CentYear], fValue[Month]) ||
         fValue[Day] == 0 )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_day_invalid
                , fBuffer);
        //"The day must have values 1 to 31");
    }

    //validate hours
    if ((fValue[Hour] < 0)  ||
        (fValue[Hour] > 24) ||
        ((fValue[Hour] == 24) && ((fValue[Minute] !=0) ||
                                  (fValue[Second] !=0) ||
                                  (fValue[MiliSecond] !=0))))
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_hour_invalid
                , fBuffer);
        //("Hour must have values 0-23");
    }

    //validate minutes
    if ( fValue[Minute] < 0 ||
         fValue[Minute] > 59 )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_min_invalid
                , fBuffer);
        //"Minute must have values 0-59");
    }

    //validate seconds
    if ( fValue[Second] < 0 ||
         fValue[Second] > 60 )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_second_invalid
                , fBuffer);
        //"Second must have values 0-60");
    }

    //validate time-zone hours
    if ( (abs(fTimeZone[hh]) > 14) ||
         ((abs(fTimeZone[hh]) == 14) && (fTimeZone[mm] != 0)) )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_tz_hh_invalid
                , fBuffer);
        //"Time zone should have range -14..+14");
    }

    //validate time-zone minutes
    if ( abs(fTimeZone[mm]) > 59 )
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_min_invalid
                , fBuffer);
        //("Minute must have values 0-59");
    }
	
    return;
}

// -----------------------------------------------------------------------
// locator and converter
// -----------------------------------------------------------------------
int XMLDateTime::indexOf(const int start, const int end, const XMLCh ch) const
{
    for ( int i = start; i < end; i++ )
        if ( fBuffer[i] == ch )
            return i;

    return NOT_FOUND;
}

int XMLDateTime::findUTCSign (const int start)
{
    int  pos;
    for ( int index = start; index < fEnd; index++ )
    {
        pos = XMLString::indexOf(UTC_SET, fBuffer[index]);
        if ( pos != NOT_FOUND)
        {
            fValue[utc] = pos+1;   // refer to utcType, there is 1 diff
            return index;
        }
    }

    return NOT_FOUND;
}

//
// Note:
//    start: starting point in fBuffer
//    end:   ending point in fBuffer (exclusive)
//    fStart NOT updated
//
int XMLDateTime::parseInt(const int start, const int end) const
{

    XMLCh* strToScan = new XMLCh[end - start + 1];
    ArrayJanitor<XMLCh>  jname(strToScan);
    XMLString::subString(strToScan, fBuffer, start, end);

    unsigned int retVal;
    XMLString::textToBin(strToScan, retVal);

    return (int) retVal;
}

//
// [-]CCYY
//
// Note: start from fStart
//       end (exclusive)
//       fStart NOT updated
//
int XMLDateTime::parseIntYear(const int end) const
{
    // skip the first leading '-'
    int start = ( fBuffer[0] == chDash ) ? fStart + 1 : fStart;

    int length = end - start;
    if (length < 4)
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_year_tooShort
                , fBuffer);
        //"Year must have 'CCYY' format");
    }
    else if (length > 4 &&
             fBuffer[start] == chDigit_0)
    {
        ThrowXML1(SchemaDateTimeException
                , XMLExcepts::DateTime_year_leadingZero
                , fBuffer);
        //"Leading zeros are required if the year value would otherwise have fewer than four digits;
        // otherwise they are forbidden");
    }

    bool negative = (fBuffer[0] == chDash);
    int  yearVal = parseInt((negative ? 1 : 0), end);
    return ( negative ? (-1) * yearVal : yearVal );
}

XERCES_CPP_NAMESPACE_END
