/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2003 The Apache Software Foundation.  All rights
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
 * Revision 1.3  2003/12/17 00:18:34  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.2  2003/11/24 05:10:26  neilg
 * implement method for determining member type of union that validated some value
 *
 * Revision 1.1  2003/11/12 20:29:47  peiyongz
 * Stateless Grammar: ValidationContext
 *
 * $Id$ 
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/internal/ValidationContextImpl.hpp>
#include <xercesc/framework/XMLRefInfo.hpp>
#include <xercesc/validators/DTD/DTDEntityDecl.hpp>
#include <xercesc/validators/datatype/InvalidDatatypeValueException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Constructor and Destructor
// ---------------------------------------------------------------------------
ValidationContextImpl::~ValidationContextImpl()
{
    if (fIdRefList)
        delete fIdRefList;
}

ValidationContextImpl::ValidationContextImpl(MemoryManager* const manager)
:ValidationContext(manager)
,fIdRefList(0)
,fEntityDeclPool(0)
,fToCheckIdRefList(true)
,fValidatingMemberType(0)
{
    fIdRefList = new (fMemoryManager) RefHashTableOf<XMLRefInfo>(109, fMemoryManager);
}

/**
 * IdRefList
 *
 */
RefHashTableOf<XMLRefInfo>* ValidationContextImpl::getIdRefList() const
{
    return fIdRefList;
}

void ValidationContextImpl::setIdRefList(RefHashTableOf<XMLRefInfo>* const newIdRefList)
{
    if (fIdRefList)
        delete fIdRefList;

    fIdRefList = newIdRefList;
}

void ValidationContextImpl::clearIdRefList()
{
    if (fIdRefList)
        fIdRefList->removeAll();

}

void ValidationContextImpl::addId(const XMLCh * const content)
{
    if (!fIdRefList || !fToCheckIdRefList)
        return;

    XMLRefInfo* idEntry = fIdRefList->get(content);

    if (idEntry)
    {
        if (idEntry->getDeclared())
        {
            ThrowXMLwithMemMgr1(InvalidDatatypeValueException
                    , XMLExcepts::VALUE_ID_Not_Unique
                    , content
                    , fMemoryManager);
        }
    }
    else
    {
        idEntry = new (fMemoryManager) XMLRefInfo(content, false, false, fMemoryManager);
        fIdRefList->put((void*)idEntry->getRefName(), idEntry);
    }

    //
    //  Mark it declared
    //
    idEntry->setDeclared(true);

}

void ValidationContextImpl::addIdRef(const XMLCh * const content)
{
    if (!fIdRefList || !fToCheckIdRefList)
        return;

    XMLRefInfo* idEntry = fIdRefList->get(content);

    if (!idEntry)
    {
        idEntry = new (fMemoryManager) XMLRefInfo(content, false, false, fMemoryManager);
        fIdRefList->put((void*)idEntry->getRefName(), idEntry);
    }

    //
    //  Mark it used
    //
    idEntry->setUsed(true);

}

void ValidationContextImpl::toCheckIdRefList(bool toCheck)
{ 
    fToCheckIdRefList = toCheck;
}

/**
 * EntityDeclPool
 *
 */
const NameIdPool<DTDEntityDecl>* ValidationContextImpl::getEntityDeclPool() const
{
    return fEntityDeclPool;
}

const NameIdPool<DTDEntityDecl>* ValidationContextImpl::setEntityDeclPool(const NameIdPool<DTDEntityDecl>* const newEntityDeclPool)
{
    // we don't own it so we return the existing one for the owner to delete
    const NameIdPool<DTDEntityDecl>* tempPool = fEntityDeclPool;
    fEntityDeclPool = newEntityDeclPool;
    return tempPool;

}
           
void ValidationContextImpl::checkEntity(const XMLCh * const content) const
{

    if (fEntityDeclPool)
    {
        const DTDEntityDecl* decl = fEntityDeclPool->getByKey(content);

        if (!decl || !decl->isUnparsed())
        {
            ThrowXMLwithMemMgr1(InvalidDatatypeValueException
                    , XMLExcepts::VALUE_ENTITY_Invalid
                    , content
                    , fMemoryManager);
        }

    }
    else 
    {
        ThrowXMLwithMemMgr1
        (
            InvalidDatatypeValueException
            , XMLExcepts::VALUE_ENTITY_Invalid
            , content
            , fMemoryManager
        );
    }

}

XERCES_CPP_NAMESPACE_END

