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
 * Revision 1.3  2001/11/12 20:36:54  peiyongz
 * SchemaDateTimeException defined
 *
 * Revision 1.2  2001/11/09 20:41:45  peiyongz
 * Fix: compilation error on Solaris and AIX.
 *
 * Revision 1.1  2001/11/07 19:16:03  peiyongz
 * DateTime Port
 *
 */

#ifndef XML_DATETIME_HPP
#define XML_DATETIME_HPP

#include <util/XercesDefs.hpp>
#include <util/XMLNumber.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/SchemaDateTimeException.hpp>

class XMLUTIL_EXPORT XMLDateTime : public XMLNumber
{
public:

    // to be moved to XMLNumber
    enum 
    {
        LESS_THAN     = -1,
        EQUAL         = 0,
        GREATER_THAN  = 1,
        INDETERMINATE = 2
    };

	enum valueIndex
    {
        CentYear   = 0,
        Month      ,
        Day        ,
        Hour       ,
        Minute     ,
        Second     ,
        MiliSecond ,
        utc        ,
        TOTAL_SIZE   
    };

    enum utcType 
    {
        UTC_UNKNOWN = 0,
        UTC_STD        ,          // set in parse() or normalize()
        UTC_POS        ,          // set in parse()
        UTC_NEG                   // set in parse()
    };

    // -----------------------------------------------------------------------
    // ctors and dtor
    // -----------------------------------------------------------------------

    XMLDateTime();

    XMLDateTime(const XMLCh* const);

    ~XMLDateTime();

    inline void           setBuffer(const XMLCh* const);

    // -----------------------------------------------------------------------
    // Copy ctor and Assignment operators
    // -----------------------------------------------------------------------

    XMLDateTime(const XMLDateTime&);

    XMLDateTime&          operator=(const XMLDateTime&);   

    // -----------------------------------------------------------------------
    // Implementation of Abstract Interface
    // -----------------------------------------------------------------------

    virtual XMLCh*        toString() const;

    virtual int           getSign() const;

    // -----------------------------------------------------------------------
    // parsers
    // -----------------------------------------------------------------------

    void                  parseDateTime();       //DateTime

    void                  parseDate();           //Date

    void                  parseTime();           //Time

    void                  parseDay();            //gDay

    void                  parseMonth();          //gMonth

    void                  parseYear();           //gYear

    void                  parseMonthDay();       //gMonthDay

    void                  parseYearMonth();      //gYearMonth

    void                  parseDuration();       //duration

    // -----------------------------------------------------------------------
    // Comparison 
    // -----------------------------------------------------------------------
    static int            compare(const XMLDateTime* const 
                                , const XMLDateTime* const);

    static int            compare(const XMLDateTime* const 
                                , const XMLDateTime* const
                                , bool                    );

    static int            compareOrder(const XMLDateTime* const
                                     , const XMLDateTime* const);

private:

    // -----------------------------------------------------------------------
    // Constant data
    // -----------------------------------------------------------------------
	//
    enum timezoneIndex 
    {
        hh = 0,
        mm ,
        TIMEZONE_ARRAYSIZE
    };

    // -----------------------------------------------------------------------
    // Comparison 
    // -----------------------------------------------------------------------
    static int            compareResult(short 
                                      , short 
                                      , bool);

    static void           addDuration(XMLDateTime*             pDuration
                                    , const XMLDateTime* const pBaseDate
                                    , int                      index);


    static int            compareResult(const XMLDateTime* const
                                      , const XMLDateTime* const
                                      , bool
                                      , int);

    static inline int     getRetVal(int, int);

    // -----------------------------------------------------------------------
    // helper
    // -----------------------------------------------------------------------

    inline  void          reset();

    inline  void          assertBuffer()               const;
   
    inline  void          copy(const XMLDateTime&);

    // allow multiple parsing
    inline  void          initParser();

    inline  bool          isNormalized()               const;

    // -----------------------------------------------------------------------
    // scaners 
    // -----------------------------------------------------------------------

    void                  getDate();

    void                  getTime();

    void                  getYearMonth();

    void                  getTimeZone(const int);

    void                  parseTimeZone();

    // -----------------------------------------------------------------------
    // locator and converter
    // -----------------------------------------------------------------------

    int                   findUTCSign(const int start);

    int                   indexOf(const int start
                                , const int end
                                , const XMLCh ch)     const;

    int                   parseInt(const int start
                                 , const int end)     const;

    int                   parseIntYear(const int end) const;

    // -----------------------------------------------------------------------
    // validator and normalizer
    // -----------------------------------------------------------------------

    void                  validateDateTime()          const;

    void                  normalize();

    // -----------------------------------------------------------------------
    // Unimplemented operator ==
    // -----------------------------------------------------------------------
	bool operator==(const XMLDateTime& toCompare) const;


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //     fValue[]
    //          object representation of date time.
    //
    //     fTimeZone[]
    //          temporary storage for normalization
    //
    //     fStart, fEnd
    //          pointers to the portion of fBuffer being parsed
    //
    //     fBuffer
    //          raw data to be parsed, own it.
    //
    // -----------------------------------------------------------------------

    int          fValue[TOTAL_SIZE];    
    int          fTimeZone[TIMEZONE_ARRAYSIZE];
    int          fStart;
    int          fEnd;

    XMLCh*       fBuffer;

};

void XMLDateTime::setBuffer(const XMLCh* const aString)
{
    reset();
    fBuffer = XMLString::replicate(aString);
    fEnd    = XMLString::stringLen(fBuffer);

}

void XMLDateTime::reset()
{
    for ( int i=0; i < TOTAL_SIZE; i++ ) 
        fValue[i] = 0;

    fTimeZone[hh] = fTimeZone[mm] = 0;
    fStart = fEnd = 0;

    if (fBuffer)
    {
        delete[] fBuffer;
        fBuffer = 0;
    }

}

void XMLDateTime::copy(const XMLDateTime& rhs)
{
    for ( int i = 0; i < TOTAL_SIZE; i++ ) 
        fValue[i] = rhs.fValue[i];

    fTimeZone[hh] = rhs.fTimeZone[hh];
    fTimeZone[mm] = rhs.fTimeZone[mm];
    fStart = rhs.fStart;
    fEnd   = rhs.fEnd;

    if (fBuffer)
    {
        delete[] fBuffer;
        fBuffer = 0;
    }

    if (rhs.fBuffer)
        fBuffer = XMLString::replicate(rhs.fBuffer);

}

void XMLDateTime::assertBuffer() const
{
    if ( ( !fBuffer )            || 
         ( fBuffer[0] == chNull ) )
    {
        ThrowXML(SchemaDateTimeException
               , XMLExcepts::DateTime_Assert_Buffer_Fail);
    }

}

void XMLDateTime::initParser()
{
    assertBuffer();
    fStart = 0;   // to ensure scan from the very first beginning
                  // in case the pointer is updated accidentally by someone else.
}

bool XMLDateTime::isNormalized() const
{
    return ( fValue[utc] == UTC_STD ? true : false );
}

int XMLDateTime::getRetVal(int c1, int c2)
{
    if ((c1 == LESS_THAN    && c2 == GREATER_THAN) ||
        (c1 == GREATER_THAN && c2 == LESS_THAN)      ) 
    {
        return INDETERMINATE; 
    }

    return ( c1 != INDETERMINATE ) ? c1 : c2;
}

#endif
