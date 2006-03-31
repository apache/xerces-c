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
 * $Id$
 */


#ifndef DOMLSOutputImpl_HEADER_GUARD_
#define DOMLSOutputImpl_HEADER_GUARD_

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class CDOM_EXPORT DOMLSOutputImpl : public XMemory, public DOMLSOutput
{

public:

    DOMLSOutputImpl(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    ~DOMLSOutputImpl();

    virtual XMLFormatTarget* getByteStream() const;
    virtual const XMLCh* getEncoding() const;
    virtual const XMLCh* getSystemId() const;

    virtual void setByteStream(XMLFormatTarget* stream);
    virtual void setEncoding(const XMLCh* const encodingStr);
    virtual void setSystemId(const XMLCh* const systemId);

    virtual void release();

private:

    /** unimplemented copy ctor and assignment operator */
    DOMLSOutputImpl(const DOMLSOutputImpl&);
    DOMLSOutputImpl & operator = (const DOMLSOutputImpl&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fByteStream
    //    We don't own it
    //
    //  fEncoding
    //    We own it
    //
    //  fSystemId
    //    We own it
    // 
    // -----------------------------------------------------------------------

    XMLFormatTarget              *fByteStream;
    XMLCh                        *fEncoding;
    XMLCh                        *fSystemId;
    MemoryManager*               fMemoryManager;
};

inline XMLFormatTarget* DOMLSOutputImpl::getByteStream() const
{
    return fByteStream;
}

inline const XMLCh* DOMLSOutputImpl::getEncoding() const
{
    return fEncoding;
}

inline const XMLCh* DOMLSOutputImpl::getSystemId() const
{
    return fSystemId;
}

XERCES_CPP_NAMESPACE_END

#endif
