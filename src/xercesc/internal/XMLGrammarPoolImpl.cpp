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
 * Revision 1.12  2003/11/07 20:30:28  neilg
 * fix compilation errors on AIX and HPUX; thanks to David Cargill
 *
 * Revision 1.11  2003/11/06 21:53:52  neilg
 * update grammar pool interface so that cacheGrammar(Grammar) can tell the caller whether the grammar was accepted.  Also fix some documentation errors.
 *
 * Revision 1.10  2003/11/06 15:30:06  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.9  2003/11/05 18:20:20  peiyongz
 * cleanup deserialized grammars if exception thrown during loading
 *
 * Revision 1.8  2003/10/29 16:16:08  peiyongz
 * GrammarPool' serialization/deserialization support
 *
 * Revision 1.7  2003/10/10 18:36:41  neilg
 * update XMLGrammarPool default implementation to reflect recent modifications to the base interface.
 *
 * Revision 1.6  2003/10/09 13:54:25  neilg
 * modify grammar pool implementation to that, once locked, a thread-safe StringPool is used
 *
 * Revision 1.5  2003/09/16 18:30:54  neilg
 * make Grammar pool be responsible for creating and owning URI string pools.  This is one more step towards having grammars be independent of the parsers involved in their creation
 *
 * Revision 1.4  2003/09/02 08:59:02  gareth
 * Added API to get enumerator of grammars.
 *
 * Revision 1.3  2003/07/31 17:05:03  peiyongz
 * Grammar embed Grammar Description
 * using getGrammar(URI)
 * update GrammarDescription info
 *
 * Revision 1.2  2003/06/20 22:19:30  peiyongz
 * Stateless Grammar Pool :: Part I
 *
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
#include <xercesc/internal/XSerializeEngine.hpp>
#include <xercesc/internal/XTemplateSerializer.hpp>
#include <xercesc/validators/DTD/DTDGrammar.hpp>
#include <xercesc/validators/DTD/XMLDTDDescriptionImpl.hpp>
#include <xercesc/validators/schema/SchemaGrammar.hpp>
#include <xercesc/validators/schema/XMLSchemaDescriptionImpl.hpp>

#include <xercesc/util/SynchronizedStringPool.hpp>

XERCES_CPP_NAMESPACE_BEGIN

void updatePSVIvectorElemIds(ValueVectorOf<SchemaElementDecl*>* vectorElemDecls, 
                             SchemaGrammar* const grammar);


// ---------------------------------------------------------------------------
//  XMLGrammarPoolImpl: constructor and destructor
// ---------------------------------------------------------------------------
XMLGrammarPoolImpl::~XMLGrammarPoolImpl()
{
    delete fGrammarRegistry;
    delete fStringPool;
    if(fSynchronizedStringPool)
        delete fSynchronizedStringPool;
}

XMLGrammarPoolImpl::XMLGrammarPoolImpl(MemoryManager* const memMgr)
:XMLGrammarPool(memMgr)
,fGrammarRegistry(0)
,fStringPool(0)
,fSynchronizedStringPool(0)
,fPSVIvectorElemDecls(0)
,fLocked(false)
,fDoPSVI(false)
{
    fGrammarRegistry = new (memMgr) RefHashTableOf<Grammar>(29, true, memMgr);
    fStringPool = new (memMgr) XMLStringPool(109, memMgr);
}

// -----------------------------------------------------------------------
// Implementation of Grammar Pool Interface 
// -----------------------------------------------------------------------
bool XMLGrammarPoolImpl::cacheGrammar(Grammar* const               gramToCache )
{
    if(fLocked)
        return false;
    if (!gramToCache )
        return false;

    const XMLCh* grammarKey = gramToCache->getGrammarDescription()->getGrammarKey();

    if (fGrammarRegistry->containsKey(grammarKey)) 
    {
        ThrowXML(RuntimeException, XMLExcepts::GC_ExistingGrammar);
    }

    fGrammarRegistry->put((void*) grammarKey, gramToCache);
    
    if (fDoPSVI && gramToCache->getGrammarType() == Grammar::SchemaGrammarType) {
        if (!fPSVIvectorElemDecls) {
            // REVISIT: what should the be the initial size?
            MemoryManager *memMgr = getMemoryManager();
            fPSVIvectorElemDecls = new (memMgr) ValueVectorOf<SchemaElementDecl*>(64, memMgr);
        }
        updatePSVIvectorElemIds(fPSVIvectorElemDecls, (SchemaGrammar*) gramToCache);
    }
    return true;

}

Grammar* XMLGrammarPoolImpl::retrieveGrammar(XMLGrammarDescription* const gramDesc)
{
    if (!gramDesc)
        return 0;

    /***
     * This implementation simply use GrammarKey
     */
    return fGrammarRegistry->get(gramDesc->getGrammarKey());
    
}

Grammar* XMLGrammarPoolImpl::orphanGrammar(const XMLCh* const nameSpaceKey)
{
    return fGrammarRegistry->orphanKey(nameSpaceKey); 
}

RefHashTableOfEnumerator<Grammar>
XMLGrammarPoolImpl::getGrammarEnumerator() const
{
    return RefHashTableOfEnumerator<Grammar>(fGrammarRegistry);
}


void XMLGrammarPoolImpl::clear()
{
    fGrammarRegistry->removeAll();
}

void XMLGrammarPoolImpl::lockPool()
{
    fLocked = true;
    MemoryManager *memMgr = getMemoryManager();
    if(!fSynchronizedStringPool)
    {
        fSynchronizedStringPool = new (memMgr) XMLSynchronizedStringPool(fStringPool, 109, memMgr);
    }
    if (fDoPSVI) 
    {
        fXSModel = new XSModel(this, memMgr);
    }
}

void XMLGrammarPoolImpl::unlockPool()
{
    fLocked = false;
    if(fSynchronizedStringPool)
        fSynchronizedStringPool->flushAll();
    if (fDoPSVI) 
    {
        delete fXSModel;
        fXSModel = 0;
    }
}

// -----------------------------------------------------------------------
// Implementation of PSVI
// -----------------------------------------------------------------------
void XMLGrammarPoolImpl::setPSVI(const bool doPSVI)
{
    fDoPSVI = doPSVI;
}

void updatePSVIvectorElemIds(ValueVectorOf<SchemaElementDecl*>* vectorElemDecls, 
                             SchemaGrammar* const grammar) {

    unsigned int vectorElemDeclsIndex = vectorElemDecls->size();
    RefHash3KeysIdPoolEnumerator<SchemaElementDecl> elemEnum = grammar->getElemEnumerator();

    while (elemEnum.hasMoreElements())
    {
        SchemaElementDecl& curElem = elemEnum.nextElement();
        vectorElemDecls->addElement(&curElem);
        curElem.setElemId(vectorElemDeclsIndex);
        vectorElemDeclsIndex++;
    }
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

XSModel *XMLGrammarPoolImpl::getXSModel() 
{
    // REVISIT:  implement along with XSModel implementation
    return 0;
}

XMLStringPool *XMLGrammarPoolImpl::getURIStringPool() 
{
    if(fLocked)
        return fSynchronizedStringPool;
    return fStringPool;
}

// -----------------------------------------------------------------------
// serialization and deserialization support
// -----------------------------------------------------------------------
/***
 *
 * don't serialize
 *
 *   XMLSynchronizedStringPool*  fSynchronizedStringPool;
 *   bool                        fLocked;
 */

/***
 *   .not locked
 *   .non-empty gramamrRegistry
 ***/
void XMLGrammarPoolImpl::serializeGrammars(BinOutputStream* const binOut)
{
    if (fLocked)
    {
        ThrowXML(XSerializationException, XMLExcepts::XSer_GrammarPool_Locked);
    }

    fLocked = true;
    RefHashTableOfEnumerator<Grammar> grammarEnum(fGrammarRegistry);
    if (!(grammarEnum.hasMoreElements())) 
    {
        fLocked = false;
        ThrowXML(XSerializationException, XMLExcepts::XSer_GrammarPool_Empty);
    }

    try 
    {
        XSerializeEngine  serEng(binOut, getMemoryManager());

        //version information
        serEng<<gXercesMajVersion;
        serEng<<gXercesMinVersion;
        serEng<<gXercesRevision;

        //StringPool, don't use <<
        fStringPool->serialize(serEng);

        /***
         * Serialize RefHashTableOf<Grammar>*    fGrammarRegistry; 
         ***/
        XTemplateSerializer::storeObject(fGrammarRegistry, serEng);
          
    }
    catch(...)
    {
        fLocked = false;
        throw;
    }

    fLocked = false;
}

/***
 *   .not locked
 *   .empty stringPool
 *   .empty gramamrRegistry
 ***/
void XMLGrammarPoolImpl::deserializeGrammars(BinInputStream* const binIn)
{
    if (fLocked)
    {
        ThrowXML(XSerializationException, XMLExcepts::XSer_GrammarPool_Locked);
    }

    fLocked = true;
    unsigned int stringCount = fStringPool->getStringCount();
    if (stringCount)
    {
        /***
         * it contains only the four predefined one, that is ok
         * but we need to reset the string before deserialize it
         *
         ***/
        if ( stringCount <= 4 )
        {
            fStringPool->flushAll();
        }
        else
        {
            fLocked = false;
            ThrowXML(XSerializationException, XMLExcepts::XSer_StringPool_NotEmpty);
        }
    }

    RefHashTableOfEnumerator<Grammar> grammarEnum(fGrammarRegistry);
    if (grammarEnum.hasMoreElements()) 
    {
        fLocked = false;
        ThrowXML(XSerializationException, XMLExcepts::XSer_GrammarPool_NotEmpty);
    }

    try 
    {
        XSerializeEngine  serEng(binIn, getMemoryManager());

        //version information
        unsigned int  MajVer;
        unsigned int  MinVer;
        unsigned int  Revision;

        serEng>>MajVer;
        serEng>>MinVer;
        serEng>>Revision;

        //we may change the logic once we have more
        //versions
        if ((MajVer   != gXercesMajVersion) ||
            (MinVer   != gXercesMinVersion) ||
            (Revision != gXercesRevision)     )
        {
            fLocked = false;
            XMLCh     MajVerChar[4];
            XMLCh     MinVerChar[4];
            XMLCh     RevisionChar[4];
            XMLString::binToText(MajVer,   MajVerChar,   4, 10);
            XMLString::binToText(MinVer,   MinVerChar,   4, 10);
            XMLString::binToText(Revision, RevisionChar, 4, 10);
            
            ThrowXML3(XSerializationException
                    , XMLExcepts::XSer_BinaryData_Version_NotSupported
                    , MajVerChar
                    , MinVerChar
                    , RevisionChar);
        }

        //StringPool, don't use >>
        fStringPool->serialize(serEng);

        /***
         * Deserialize RefHashTableOf<Grammar>*    fGrammarRegistry; 
         ***/
        XTemplateSerializer::loadObject(&fGrammarRegistry, 29, true, serEng);

    }
    catch(...)
    {
        clear();  //clear all deserialized grammars
        fLocked = false;
        throw;
    }

    fLocked = false;
}


XERCES_CPP_NAMESPACE_END
