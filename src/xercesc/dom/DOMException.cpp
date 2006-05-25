/*
 * Copyright 2001-2002,2004 The Apache Software Foundation.
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
 */

#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/framework/MemoryManager.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLMsgLoader.hpp>
#include <xercesc/util/XMLDOMMsg.hpp>
#include "impl/DOMImplementationImpl.hpp"

#include "DOMException.hpp"

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Destructor and Constructor
// ---------------------------------------------------------------------------
DOMException::~DOMException()
{
    if (msg && fMsgOwned)
        fMemoryManager->deallocate((void*)msg);
}

DOMException::DOMException()
:code((ExceptionCode) 0)
,msg(0)
,fMemoryManager(0)
,fMsgOwned(false)
{      
}

DOMException::DOMException(      short                 exCode
                         ,       short                 messageCode
                         ,       MemoryManager* const  memoryManager)
:code((ExceptionCode) exCode)
,fMemoryManager(memoryManager)
,fMsgOwned(true)
{
    const unsigned int msgSize = 2047;
    XMLCh errText[msgSize + 1];

    // load the text
    if(messageCode==0)
        messageCode=XMLDOMMsg::DOMEXCEPTION_ERRX+exCode;
    msg = XMLString::replicate
         (
          DOMImplementationImpl::getMsgLoader4DOM()->loadMsg(messageCode, errText, msgSize) ? errText : XMLUni::fgDefErrMsg
        , fMemoryManager
         );
}

DOMException::DOMException(const DOMException &other)
:code(other.code)
,msg(0)
,fMemoryManager(other.fMemoryManager)
,fMsgOwned(other.fMsgOwned)
{
    msg = other.fMsgOwned? XMLString::replicate(other.msg, other.fMemoryManager) : other.msg;
}

XERCES_CPP_NAMESPACE_END

