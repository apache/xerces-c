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
 * Revision 1.7  2005/01/07 15:12:10  amassari
 * Removed warnings
 *
 * Revision 1.6  2004/09/08 13:56:24  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2004/01/29 11:48:47  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.4  2003/10/17 21:10:55  peiyongz
 * loadNumber() added
 *
 * Revision 1.3  2003/09/23 18:16:07  peiyongz
 * Inplementation for Serialization/Deserialization
 *
 * Revision 1.2  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:15  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/10/09 21:28:28  peiyongz
 * explicit ctor/dtor defined.
 *
 * Revision 1.1  2001/09/27 14:54:03  peiyongz
 * DTV Reorganization: new class
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLNumber.hpp>

//since we need to dynamically created each and every derivatives 
//during deserialization by XSerializeEngine>>Derivative, we got
//to include all hpp
#include <xercesc/util/XMLDouble.hpp>
#include <xercesc/util/XMLFloat.hpp>
#include <xercesc/util/XMLDateTime.hpp>
#include <xercesc/util/XMLBigDecimal.hpp>

XERCES_CPP_NAMESPACE_BEGIN

XMLNumber::XMLNumber()
{}

XMLNumber::XMLNumber(const XMLNumber& toCopy)
: XSerializable(toCopy)
, XMemory(toCopy)
{}

XMLNumber::~XMLNumber()
{}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_NOCREATE(XMLNumber)

void XMLNumber::serialize(XSerializeEngine&)
{
    // this class has no data to serialize/de-serilize
}

XMLNumber* XMLNumber::loadNumber(XMLNumber::NumberType  numType
                               , XSerializeEngine&      serEng)
{

    switch((XMLNumber::NumberType) numType)
    {
    case XMLNumber::Float: 
        XMLFloat* floatNum;
        serEng>>floatNum;
        return floatNum;
        break;
    case XMLNumber::Double:
        XMLDouble* doubleNum;
        serEng>>doubleNum;
        return doubleNum;
        break;
    case XMLNumber::BigDecimal: 
        XMLBigDecimal* bigdecimalNum;
        serEng>>bigdecimalNum;
        return bigdecimalNum;
        break;
    case XMLNumber::DateTime: 
        XMLDateTime* datetimeNum;
        serEng>>datetimeNum;
        return datetimeNum;
        break;
    case XMLNumber::UnKnown:
        return 0;
        break;
    default: //we treat this same as UnKnown
        return 0;
        break;
    }

}

XERCES_CPP_NAMESPACE_END
