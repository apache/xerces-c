#ifndef DOMStringPool_HEADER_GUARD_
#define DOMStringPool_HEADER_GUARD_

/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2002 The Apache Software Foundation.  All rights
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
 */

//
//  This file is part of the internal implementation of the C++ XML DOM.
//  It should NOT be included or used directly by application programs.
//
//  Applications should include the file <xercesc/dom/DOM.hpp> for the entire
//  DOM API, or xercesc/dom/DOM*.hpp for individual DOM classes, where the class
//  name is substituded for the *.
//

#include <xercesc/util/XercesDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN


struct  DOMStringPoolEntry;
class   DOMDocumentImpl;

//
// DOMStringPool is a hash table of XMLCh* Strings.
//  Each DOM Document maintains a DOMStringPool containing a XMLCh* String
//  for each Element tag name and Attribute Name that has been added
//  to the document.  When creating additional elements or attributes,
//  if the name has been seen before, the already existing string
//  will be reused.
//
class DOMStringPool
{
public:
    DOMStringPool(int  hashTableSize, DOMDocumentImpl *doc);
    ~DOMStringPool();

    const XMLCh *getPooledString(const XMLCh *in);


private:
    DOMStringPool(const DOMStringPool &other);      // Copy constructor and assignment
    DOMStringPool& operator = (const DOMStringPool &other); //  of DOMStringPool are not supported.


    DOMDocumentImpl     *fDoc;
    DOMStringPoolEntry **fHashTable;
    int                 fHashTableSize;

};


//
// DOMBuffer is a lightweight text buffer
// The buffer is not nul terminated until some asks to see the raw buffer
// contents. This also avoids overhead during append operations.
class DOMBuffer
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    DOMBuffer(DOMDocumentImpl *doc, int capacity = 31);

    DOMBuffer(DOMDocumentImpl *doc, const XMLCh* string);

    ~DOMBuffer()
    {
    }

    // -----------------------------------------------------------------------
    //  Buffer Management
    // -----------------------------------------------------------------------
    void append
    (
        const   XMLCh* const    chars
        , const unsigned int    count = 0
    );

    const XMLCh* getRawBuffer() const
    {
        fBuffer[fIndex] = 0;
        return fBuffer;
    }

    void reset()
    {
        fIndex = 0;
        fBuffer[0] = 0;
    }

    void set
    (
        const   XMLCh* const    chars
        , const unsigned int    count = 0
    );

    void chop
    (
        const unsigned int    count
    )
    {
        fBuffer[count] = 0;
        fIndex = count;
    }


    // -----------------------------------------------------------------------
    //  Getters
    // -----------------------------------------------------------------------
    unsigned int getLen() const
    {
        return fIndex;
    }

    // -----------------------------------------------------------------------
    //  Private helpers
    // -----------------------------------------------------------------------
    void expandCapacity(const unsigned int extraNeeded);


private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fBuffer
    //      The pointer to the buffer data. Its grown as needed. Its always
    //      one larger than fCapacity, to leave room for the null terminator.
    //
    //  fIndex
    //      The current index into the buffer, as characters are appended
    //      to it. If its zero, then the buffer is empty.
    //
    //  fCapacity
    //      The current capacity of the buffer. Its actually always one
    //      larger, to leave room for the null terminator.
    //
    //  fDoc
    //      For allocating memory
    // -----------------------------------------------------------------------
    XMLCh*          fBuffer;
    unsigned int    fIndex;
    unsigned int    fCapacity;
    DOMDocumentImpl* fDoc;

    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DOMBuffer(const DOMBuffer &);
    DOMBuffer & operator = (const DOMBuffer &);
};

XERCES_CPP_NAMESPACE_END

#endif
