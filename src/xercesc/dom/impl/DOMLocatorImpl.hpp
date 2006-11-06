/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
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


#ifndef DOMLOCATORIMPL_HPP
#define DOMLOCATORIMPL_HPP

#include <xercesc/dom/DOMLocator.hpp>

XERCES_CPP_NAMESPACE_BEGIN



/**
  * Introduced in DOM Level 3
  *
  * Implementation of a DOMLocator interface.
  *
  * @see DOMLocator#DOMLocator
  */

class CDOM_EXPORT DOMLocatorImpl : public DOMLocator
{
public:
    /** @name Constructors and Destructor */
    //@{

    /** Constructor */
    DOMLocatorImpl();

    DOMLocatorImpl
    (
        const XMLSSize_t lineNum
        , const XMLSSize_t columnNum
        , DOMNode* const errorNode
        , const XMLCh* const uri
        , const XMLSSize_t offset = -1
        , const XMLSSize_t utf16Offset = -1
    );

    /** Desctructor */
    virtual ~DOMLocatorImpl();

    //@}

    // DOMLocator interface
    virtual XMLSSize_t getLineNumber() const;
    virtual XMLSSize_t getColumnNumber() const;
    virtual XMLSSize_t getByteOffset() const;
    virtual XMLSSize_t getUtf16Offset() const;
    virtual DOMNode* getRelatedNode() const;
    virtual const XMLCh* getURI() const;

    // Setter functions
    void setLineNumber(const XMLSSize_t lineNumber);
    void setColumnNumber(const XMLSSize_t columnNumber);
    void setByteOffset(const XMLSSize_t offset);
    void setUtf16Offset(const XMLSSize_t offset);
    void setRelatedNode(DOMNode* const errorNode);
    void setURI(const XMLCh* const uri);


private :
    /* Unimplemented constructors and operators */

    /* Copy constructor */
    DOMLocatorImpl(const DOMLocatorImpl&);

    /* Assignment operator */
    DOMLocatorImpl& operator=(const DOMLocatorImpl&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fLineNum
    //  fColumnNum
    //      Track line/column number of where the error occured
    //
    //  fByteOffset
    //      Track byte offset in the input source where the error
    //      occured
    //
    //  fUtf16Offset
    //      Track character offset in the input source where the error
    //      occured
    //
    //  fRelatedNode
    //      Current node where the error occured
    //
    //  fURI
    //      The uri where the error occured
    // -----------------------------------------------------------------------
    XMLSSize_t   fLineNum;
    XMLSSize_t   fColumnNum;
    XMLSSize_t   fByteOffset;
    XMLSSize_t   fUtf16Offset;
    DOMNode*     fRelatedNode;
    const XMLCh* fURI;
};


// ---------------------------------------------------------------------------
//  DOMLocatorImpl: Getter methods
// ---------------------------------------------------------------------------
inline XMLSSize_t DOMLocatorImpl::getLineNumber() const
{
    return fLineNum;
}

inline XMLSSize_t DOMLocatorImpl::getColumnNumber() const
{
    return fColumnNum;
}

inline XMLSSize_t DOMLocatorImpl::getByteOffset() const
{
    return fByteOffset;
}

inline XMLSSize_t DOMLocatorImpl::getUtf16Offset() const
{
    return fUtf16Offset;
}

inline DOMNode* DOMLocatorImpl::getRelatedNode() const
{
    return fRelatedNode;
}

inline const XMLCh* DOMLocatorImpl::getURI() const
{
    return fURI;
}


// ---------------------------------------------------------------------------
//  DOMLocatorImpl: Setter methods
// ---------------------------------------------------------------------------
inline void DOMLocatorImpl::setLineNumber(const XMLSSize_t lineNumber)
{
    fLineNum = lineNumber;
}

inline void DOMLocatorImpl::setColumnNumber(const XMLSSize_t columnNumber)
{
    fColumnNum = columnNumber;
}

inline void DOMLocatorImpl::setByteOffset(const XMLSSize_t offset)
{
    fByteOffset = offset;
}

inline void DOMLocatorImpl::setUtf16Offset(const XMLSSize_t offset)
{
    fUtf16Offset = offset;
}

inline void DOMLocatorImpl::setRelatedNode(DOMNode* const errorNode)
{
    fRelatedNode = errorNode;
}

inline void DOMLocatorImpl::setURI(const XMLCh* const uri)
{
    fURI = uri;
}

XERCES_CPP_NAMESPACE_END

#endif
