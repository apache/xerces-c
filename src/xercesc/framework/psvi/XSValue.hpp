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
