/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights
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
 * Revision 1.1  2003/10/09 13:51:16  neilg
 * implementation of a StringPool implementation that permits thread-safe updates.  This can now be used by a grammar pool that is locked so that scanners have somehwere to store information about newly-encountered URIs
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/SynchronizedStringPool.hpp>


XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLSynchronizedStringPool: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLSynchronizedStringPool::XMLSynchronizedStringPool(const XMLStringPool *constPool
                , const  unsigned int  modulus
                , MemoryManager* const manager) :

    XMLStringPool(modulus, manager)
    , fConstPool(constPool)
{
}

XMLSynchronizedStringPool::~XMLSynchronizedStringPool()
{
}


// ---------------------------------------------------------------------------
//  XMLSynchronizedStringPool: Pool management methods
// ---------------------------------------------------------------------------
unsigned int XMLSynchronizedStringPool::addOrFind(const XMLCh* const newString)
{
    unsigned int id = fConstPool->getId(newString);
    if(id)
        return id;
    // might have to add it to our own table.
    // synchronize this bit
    {
        XMLMutexLock lockInit(&fMutex);
        id = XMLStringPool::addOrFind(newString);
        return id;
    }
}

bool XMLSynchronizedStringPool::exists(const XMLCh* const newString) const
{
    if(fConstPool->exists(newString))
        return true;
    return XMLStringPool::exists(newString);
}

bool XMLSynchronizedStringPool::exists(const unsigned int id) const
{
    if (!id || (id >= fCurId+fConstPool->getStringCount()))
        return false;

    return true;
}

void XMLSynchronizedStringPool::flushAll()
{
    // don't touch const pool!
    XMLStringPool::flushAll();
}


unsigned int XMLSynchronizedStringPool::getId(const XMLCh* const toFind) const
{
    unsigned int retVal = fConstPool->getId(toFind);
    if(retVal)
        return retVal;
    // make sure we return a truly unique id
    return XMLStringPool::getId(toFind)+fConstPool->getStringCount();
}


const XMLCh* XMLSynchronizedStringPool::getValueForId(const unsigned int id) const
{
    if (id <= fConstPool->getStringCount())
        return fConstPool->getValueForId(id);
    return XMLStringPool::getValueForId(id-fConstPool->getStringCount());
}

unsigned int XMLSynchronizedStringPool::getStringCount() const
{
    return fCurId+fConstPool->getStringCount()-1;
}

XERCES_CPP_NAMESPACE_END
