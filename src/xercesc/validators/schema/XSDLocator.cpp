/*
 * Copyright 2002,2004 The Apache Software Foundation.
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

/**
  * $Log$
  * Revision 1.4  2004/09/08 13:56:57  peiyongz
  * Apache License Version 2.0
  *
  * Revision 1.3  2002/11/04 14:49:42  tng
  * C++ Namespace Support.
  *
  * Revision 1.2  2002/05/27 18:44:07  tng
  * To get ready for 64 bit large file, use XMLSSize_t to represent line and column number.
  *
  * Revision 1.1  2002/03/21 15:34:40  knoaman
  * Add support for reporting line/column numbers of schema errors.
  *
  */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/XSDLocator.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSDLocator: Constructors and Destructor
// ---------------------------------------------------------------------------
XSDLocator::XSDLocator() :
    fLineNo(0)
    , fColumnNo(0)
    , fSystemId(0)
    , fPublicId(0)
{

}


// ---------------------------------------------------------------------------
//  XSDLocator: Setter methods
// ---------------------------------------------------------------------------
void XSDLocator::setValues(const XMLCh* const systemId,
                           const XMLCh* const publicId,
                           const XMLSSize_t lineNo, const XMLSSize_t columnNo)
{
    fLineNo = lineNo;
    fColumnNo = columnNo;
    fSystemId = systemId;
    fPublicId = publicId;
}

XERCES_CPP_NAMESPACE_END
