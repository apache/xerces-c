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

/*
 * $Log$
 * Revision 1.11  2004/09/08 13:55:51  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.10  2004/04/07 15:26:45  peiyongz
 * code cleaning
 *
 * Revision 1.9  2004/04/01 22:05:32  peiyongz
 * invoke DOMException with Memory Manager
 *
 * Revision 1.8  2004/01/29 11:44:26  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.7  2003/12/02 14:33:13  amassari
 * Don't use the word "exception" as a variable name, as VC 7.1 complains about it
 *
 * Revision 1.6  2003/09/04 14:38:25  gareth
 * Fix for bug #22008. Removed the ability to adopt the DOMObject.
 *
 * Revision 1.5  2003/05/14 18:06:53  gareth
 * Updated DOMError to http://www.w3.org/TR/2003/WD-DOM-Level-3-Core-20030226/core.html.
 *
 * Revision 1.4  2002/11/04 15:07:34  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/07/15 19:25:49  tng
 * DOM L3:  declare a dummy DOMError::set/getRelatedException
 *
 * Revision 1.2  2002/05/24 14:02:07  knoaman
 * Fix complilation error.
 *
 * Revision 1.1  2002/05/23 15:47:24  knoaman
 * DOM L3 core - support for DOMError, DOMErrorHandler and DOMLocator
 *
 */

#include "DOMErrorImpl.hpp"
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMLocator.hpp>

XERCES_CPP_NAMESPACE_BEGIN


// ---------------------------------------------------------------------------
//  DOMErrorImpl: Constructors and Destructor
// ---------------------------------------------------------------------------
DOMErrorImpl::DOMErrorImpl(const short severity) :
fAdoptLocation(false)
, fSeverity(severity)
, fMessage(0)
, fLocation(0)
, fType(0)
, fRelatedData(0)
{
}

DOMErrorImpl::DOMErrorImpl(const short severity,
                           const XMLCh* const message,
                           DOMLocator* const location) :
fAdoptLocation(false)
, fSeverity(severity)
, fMessage(message)
, fLocation(location)
, fType(0)
, fRelatedData(0)
{
}

DOMErrorImpl::DOMErrorImpl(const short severity,
                           const XMLCh* type,
                           const XMLCh* message,
                           void* relatedData) :
fAdoptLocation(false)
, fSeverity(severity)
, fMessage(message)
, fLocation(0)
, fType(type)
, fRelatedData(relatedData)
{

}

DOMErrorImpl::~DOMErrorImpl()
{
    if (fAdoptLocation)
        delete fLocation;
}

// ---------------------------------------------------------------------------
//  DOMErrorImpl: Setter methods
// ---------------------------------------------------------------------------
void DOMErrorImpl::setLocation(DOMLocator* const location)
{
    if (fAdoptLocation)
        delete fLocation;

    fLocation = location;
}

void DOMErrorImpl::setRelatedException(void*) const
{
    throw DOMException(DOMException::NOT_SUPPORTED_ERR, 0);
}

XERCES_CPP_NAMESPACE_END

