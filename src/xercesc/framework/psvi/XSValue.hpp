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
 * Revision 1.7  2004/08/31 20:52:25  peiyongz
 * Return additional double value for decimal
 * remove tz_hh/tz_mm
 *
 * Revision 1.6  2004/08/31 15:14:47  peiyongz
 * remove XSValueContext
 *
 * Revision 1.5  2004/08/17 21:11:41  peiyongz
 * no more Unrepresentable
 *
 * Revision 1.4  2004/08/13 21:29:20  peiyongz
 * fMemAllocated
 *
 * Revision 1.3  2004/08/12 14:24:34  peiyongz
 * HP aCC A.03
 *
 * Revision 1.2  2004/08/11 17:06:44  peiyongz
 * Do not panic if can't create RegEx
 *
 * $Id$
 */

#if !defined(XSVALUE_HPP)
#define XSVALUE_HPP

#include <xercesc/util/PlatformUtils.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class RegularExpression;

class XMLPARSER_EXPORT XSValue : public XMemory
{
public:

    enum DataType {
              dt_string               = 0,
              dt_boolean              = 1,
              dt_decimal              = 2,
              dt_float                = 3,
              dt_double               = 4,
              dt_duration             = 5,
              dt_dateTime             = 6,
              dt_time                 = 7,
              dt_date                 = 8,
              dt_gYearMonth           = 9,
              dt_gYear                = 10,
              dt_gMonthDay            = 11,
              dt_gDay                 = 12,
              dt_gMonth               = 13,
              dt_hexBinary            = 14,
              dt_base64Binary         = 15,
              dt_anyURI               = 16,
              dt_QName                = 17,
              dt_NOTATION             = 18,
              dt_normalizedString     = 19,
              dt_token                = 20,
              dt_language             = 21,
              dt_NMTOKEN              = 22,
              dt_NMTOKENS             = 23,
              dt_Name                 = 24,
              dt_NCName               = 25,
              dt_ID                   = 26,
              dt_IDREF                = 27,
              dt_IDREFS               = 28,
              dt_ENTITY               = 29,
              dt_ENTITIES             = 30,
              dt_integer              = 31,
              dt_nonPositiveInteger   = 32,
              dt_negativeInteger      = 33,
              dt_long                 = 34,
              dt_int                  = 35,
              dt_short                = 36,
              dt_byte                 = 37,
              dt_nonNegativeInteger   = 38,
              dt_unsignedLong         = 39,
              dt_unsignedInt          = 40,
              dt_unsignedShort        = 41,
              dt_unsignedByte         = 42,
              dt_positiveInteger      = 43,
              dt_MAXCOUNT             = 44
    };

    enum XMLVersion {
            ver_10,
            ver_11
    };

    enum Status {
            st_Init,
            st_Invalid,
            st_InvalidChar,     //for numeric
            st_InvalidRange,    //for numeric
            st_NoCanRep,
            st_NoActVal,
            st_NotSupported,
            st_CantCreateRegEx,
            st_UnknownType
    };

    enum DataGroup {
            dg_numerics,
            dg_datetimes,
            dg_strings
    };

    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    /** @name Destructor */
    //@{
    ~XSValue();
    //@}

    //---------------------------------
    /** @name Externalization methods */
    //@{

    static
    bool     validate
             (
                const XMLCh*          const content    
              ,       DataType              datatype
              ,       Status&               status
              ,       XMLVersion            version    = ver_10
              ,       bool                  toValidate = true 
              ,       MemoryManager*  const manager    = XMLPlatformUtils::fgMemoryManager
             );

    static
    XMLCh*   getCanonicalRepresentation
             (
                const XMLCh*          const content    
              ,       DataType              datatype
              ,       Status&               status
              ,       XMLVersion            version    = ver_10
              ,       bool                  toValidate = true 
              ,       MemoryManager*  const manager    = XMLPlatformUtils::fgMemoryManager
             );

    static
    XSValue* getActualValue
             (
                const XMLCh*          const content    
              ,       DataType              datatype
              ,       Status&               status
              ,       XMLVersion            version    = ver_10
              ,       bool                  toValidate = true 
              ,       MemoryManager*  const manager    = XMLPlatformUtils::fgMemoryManager
             );

    //@}

    //----------------------------------
    /** public data */

    union {
                     bool      f_bool;
                     char      f_char;                    
            unsigned char      f_uchar;
                     short     f_short;
            unsigned short     f_ushort;
                     int       f_int;
            unsigned int       f_uint;
                     long      f_long;
            unsigned long      f_ulong;
                     float     f_float;
                     double    f_double;
                     XMLCh*    f_strVal;

            struct decimal {
                            int              f_sign;
                            unsigned int     f_scale;  
                            unsigned long    f_integral;
                            unsigned long    f_fraction;
                            double           f_dvalue;
            } f_decimal;

            struct datetime {
                            int       f_year;
                            int       f_month;
                            int       f_day;
                            int       f_hour;
                            int       f_min;
                            int       f_second;
                            double    f_milisec;

            } f_datetime;

    } fData;

    static
    void reinitMutex();

    static
    void reinitRegEx();

private:

    typedef union
    {
                long  f_long;
       unsigned long  f_ulong;
    } t_value;

    /** @name Constructors */
    //@{
    /**
      * The default constructor 
      *
      */
    XSValue(MemoryManager*  const manager);

    //@};

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XSValue(const XSValue&);
    XSValue & operator=(const XSValue &);

    //---------------------------------
    /** @name Helpers */

    //@{

    static const XSValue::DataGroup inGroup[];

    //@}

    static
    bool     validateNumerics
             (
                const XMLCh*          const content    
              ,       DataType              datatype
              ,       Status&               status
              ,       XMLVersion            version
              ,       bool                  toValidate
              ,       MemoryManager*  const manager
             );

    static
    bool     validateDateTimes
             (
                const XMLCh*          const content    
              ,       DataType              datatype
              ,       Status&               status
              ,       XMLVersion            version
              ,       bool                  toValidate
              ,       MemoryManager*  const manager
             );

    static
    bool     validateStrings
             (
                const XMLCh*          const content    
              ,       DataType              datatype
              ,       Status&               status
              ,       XMLVersion            version
              ,       bool                  toValidate
              ,       MemoryManager*  const manager
             );

    static
    XMLCh*   getCanRepNumerics
             (
                const XMLCh*          const content    
              ,       DataType              datatype
              ,       Status&               status
              ,       XMLVersion            version
              ,       bool                  toValidate
              ,       MemoryManager*  const manager
             );

    static
    XMLCh*   getCanRepDateTimes
             (
                const XMLCh*          const content    
              ,       DataType              datatype
              ,       Status&               status
              ,       XMLVersion            version
              ,       bool                  toValidate
              ,       MemoryManager*  const manager
             );

    static
    XMLCh*   getCanRepStrings
             (
                const XMLCh*          const content    
              ,       DataType              datatype
              ,       Status&               status
              ,       XMLVersion            version
              ,       bool                  toValidate
              ,       MemoryManager*  const manager
             );

    static
    XSValue*  getActValNumerics
             (
                const XMLCh*          const content    
              ,       DataType              datatype
              ,       Status&               status
              ,       XMLVersion            version
              ,       bool                  toValidate
              ,       MemoryManager*  const manager
             );

    static
    XSValue*  getActValDateTimes
             (
                const XMLCh*          const content    
              ,       DataType              datatype
              ,       Status&               status
              ,       XMLVersion            version
              ,       bool                  toValidate
              ,       MemoryManager*  const manager
             );

    static
    XSValue*  getActValStrings
             (
                const XMLCh*          const content    
              ,       DataType              datatype
              ,       Status&               status
              ,       XMLVersion            version
              ,       bool                  toValidate
              ,       MemoryManager*  const manager
             );

    static
    bool      getActualValue
              (
                 const XMLCh*         const content
               ,       Status&               status
               ,       XMLVersion            version    
               ,       bool                  toValidate 
               ,       int                   ct
               ,       t_value&              retVal               
               ,       int                   base
               ,       MemoryManager* const  manager
               );

    // -----------------------------------------------------------------------
    //  data members
    // -----------------------------------------------------------------------
    bool                fMemAllocated;
    MemoryManager*      fMemoryManager;

};


XERCES_CPP_NAMESPACE_END

#endif
