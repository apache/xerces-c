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
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id$
 * $Log$
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
