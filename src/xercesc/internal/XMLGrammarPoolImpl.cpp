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
 * Revision 1.1  2003/06/20 18:38:39  peiyongz
 * Stateless Grammar Pool :: Part I
 *
 * $Id$
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/internal/XMLGrammarPoolImpl.hpp>
#include <xercesc/validators/DTD/DTDGrammar.hpp>
#include <xercesc/validators/DTD/XMLDTDDescriptionImpl.hpp>
#include <xercesc/validators/Schema/SchemaGrammar.hpp>
#include <xercesc/validators/Schema/XMLSchemaDescriptionImpl.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLGrammarPoolImpl: constructor and destructor
// ---------------------------------------------------------------------------
XMLGrammarPoolImpl::~XMLGrammarPoolImpl()
{
    delete fGrammarRegistry;
}

XMLGrammarPoolImpl::XMLGrammarPoolImpl(MemoryManager* const memMgr)
:XMLGrammarPool(memMgr)
,fGrammarRegistry(0)
{
    fGrammarRegistry = new (memMgr) RefHashTableOf<GrammarEntry>(29, true, memMgr);
}

// -----------------------------------------------------------------------
// Implementation of Grammar Pool Interface 
// -----------------------------------------------------------------------
void XMLGrammarPoolImpl::cacheGrammar(XMLGrammarDescription* const gramDesc
                                    , Grammar* const               gramToCache )
{
    if (!gramDesc || !gramToCache )
        return;

    const XMLCh* grammarKey = gramDesc->getGrammarKey();

    if (fGrammarRegistry->containsKey(grammarKey)) 
    {
        ThrowXML(RuntimeException, XMLExcepts::GC_ExistingGrammar);
    }

    fGrammarRegistry->put((void*) grammarKey, new (getMemoryManager()) GrammarEntry(gramDesc, gramToCache)); 

}

Grammar* XMLGrammarPoolImpl::retrieveGrammar(XMLGrammarDescription* const gramDesc)
{
    if (!gramDesc)
        return 0;

    GrammarEntry* gramEntry = fGrammarRegistry->get(gramDesc->getGrammarKey());
    
    if (!gramEntry)
        return 0;

    return gramEntry->getGrammar();
}

Grammar* XMLGrammarPoolImpl::orphanGrammar(XMLGrammarDescription* const gramDesc)
{
    if (!gramDesc)
        return 0;

    GrammarEntry* gramEntry = fGrammarRegistry->orphanKey(gramDesc->getGrammarKey()); 

    if (!gramEntry)
        return 0;

    /***
     * Delete the grammarEntry but don't delete the grammar
     */
    Grammar* theGram = gramEntry->getGrammar();
    gramEntry->nullGrammar();
    delete gramEntry;

    return theGram;
}

void XMLGrammarPoolImpl::clear()
{
    fGrammarRegistry->removeAll();
}

void XMLGrammarPoolImpl::lockPool()
{
    //This implementation does NOT provide thread safety mechanism
}

void XMLGrammarPoolImpl::unlockPool()
{
    //This implementation does NOT provide thread safety mechanism
}

// -----------------------------------------------------------------------
// Implementation of Factory Interface 
// -----------------------------------------------------------------------
DTDGrammar*  XMLGrammarPoolImpl::createDTDGrammar()
{ 
	return new (getMemoryManager()) DTDGrammar(getMemoryManager());
}                                    

SchemaGrammar* XMLGrammarPoolImpl::createSchemaGrammar()
{
	return new (getMemoryManager()) SchemaGrammar(getMemoryManager()); 
}

XMLDTDDescription*  XMLGrammarPoolImpl::createDTDDescription(const XMLCh* const rootName)
{ 
	return new (getMemoryManager()) XMLDTDDescriptionImpl(rootName, getMemoryManager()); 
}
								   
XMLSchemaDescription* XMLGrammarPoolImpl::createSchemaDescription(const XMLCh* const targetNamespace)
{
	return new (getMemoryManager()) XMLSchemaDescriptionImpl(targetNamespace, getMemoryManager()); 
}

// ---------------------------------------------------------------------------
//  GrammarEntry: constructor and destructor
// ---------------------------------------------------------------------------
GrammarEntry::GrammarEntry(XMLGrammarDescription* const  gramDesc
                         , Grammar* const                grammar)
:fDescription(gramDesc)
,fGrammar(grammar)
{
}

/**
 *
 * A GrammarEntry is desctructed in two ways
 *    . from clear(), we need to destroy the GrammarDescription and Grammar
 *    . from orphanGrammar()
 *
 */
GrammarEntry::~GrammarEntry()
{
    if (fDescription)
        delete fDescription;

    if (fGrammar)
        delete fGrammar;
}

XERCES_CPP_NAMESPACE_END
