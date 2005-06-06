/*
 * Copyright 1999-2004 The Apache Software Foundation.
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
 * Revision 1.26  2005/01/06 21:39:43  amassari
 * Removed warnings
 *
 * Revision 1.25  2004/11/09 03:09:21  peiyongz
 * Storer_NewerThan_Loader
 *
 * Revision 1.24  2004/11/08 03:57:22  peiyongz
 * read/write Storer level
 *
 * Revision 1.23  2004/09/29 19:27:07  cargilld
 * Fix for Jira-1217: fixing problems with getXSModel.
 *
 * Revision 1.22  2004/09/10 17:36:13  cargilld
 * Return bool as described in the interface for cacheGrammar instead of throwing an exception.  Fix from Dave Bertoni.
 *
 * Revision 1.21  2004/09/08 13:56:13  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.20  2004/04/07 14:13:42  peiyongz
 * systemId (to replace rootElemName) as DTDGrammar Key
 *
 * Revision 1.19  2004/03/01 23:18:28  peiyongz
 * Instantiate XSerializeEngine with GrammarPool
 *
 * Revision 1.18  2004/01/29 11:46:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.17  2004/01/13 16:34:20  cargilld
 * Misc memory management changes.
 *
 * Revision 1.16  2003/12/17 00:18:34  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.15  2003/11/25 18:18:39  knoaman
 * Check for out of memory exception. Thanks to David Cargill.
 *
 * Revision 1.14  2003/11/21 22:38:50  neilg
 * Enable grammar pools and grammar resolvers to manufacture
 * XSModels.  This also cleans up handling in the
 * parser classes by eliminating the need to tell
 * the grammar pool that schema compoments need to be produced.
 * Thanks to David Cargill.
 *
 * Revision 1.13  2003/11/14 22:34:20  neilg
 * removed methods made unnecessary by new XSModel implementation design; thanks to David Cargill
 *
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
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/SynchronizedStringPool.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// private function used to update fXSModel
void XMLGrammarPoolImpl::createXSModel()
{
    delete fXSModel;
    fXSModel = new (getMemoryManager()) XSModel(this, getMemoryManager());
    fXSModelIsValid = true; 
}

// ---------------------------------------------------------------------------
//  XMLGrammarPoolImpl: constructor and destructor
// ---------------------------------------------------------------------------
XMLGrammarPoolImpl::~XMLGrammarPoolImpl()
{
    delete fGrammarRegistry;
    delete fStringPool;
    if(fSynchronizedStringPool)
        delete fSynchronizedStringPool;
    if(fXSModel)
        delete fXSModel;
}

XMLGrammarPoolImpl::XMLGrammarPoolImpl(MemoryManager* const memMgr)
:XMLGrammarPool(memMgr)
,fGrammarRegistry(0)
,fStringPool(0)
,fSynchronizedStringPool(0)
,fXSModel(0)
,fLocked(false)
,fXSModelIsValid(false)
{
    fGrammarRegistry = new (memMgr) RefHashTableOf<Grammar>(29, true, memMgr);
    fStringPool = new (memMgr) XMLStringPool(109, memMgr);
}

// -----------------------------------------------------------------------
// Implementation of Grammar Pool Interface 
// -----------------------------------------------------------------------
bool XMLGrammarPoolImpl::cacheGrammar(Grammar* const               gramToCache )
{
    if(fLocked || !gramToCache)
        return false;

    const XMLCh* grammarKey = gramToCache->getGrammarDescription()->getGrammarKey();

    if (fGrammarRegistry->containsKey(grammarKey)) 
    {
        return false;
    }

    fGrammarRegistry->put((void*) grammarKey, gramToCache);
    
    if (fXSModelIsValid && gramToCache->getGrammarType() == Grammar::SchemaGrammarType)
    {
        fXSModelIsValid = false;
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
    if (!fLocked)
    {        
        Grammar* grammar = fGrammarRegistry->orphanKey(nameSpaceKey); 
        if (fXSModelIsValid && grammar && grammar->getGrammarType() == Grammar::SchemaGrammarType)
        {
            fXSModelIsValid = false;
        }
        return grammar;
    }
    return 0;
}

RefHashTableOfEnumerator<Grammar>
XMLGrammarPoolImpl::getGrammarEnumerator() const
{
    return RefHashTableOfEnumerator<Grammar>(fGrammarRegistry, false, fGrammarRegistry->getMemoryManager());
}


bool XMLGrammarPoolImpl::clear()
{
    if (!fLocked)
    {
        fGrammarRegistry->removeAll();

        fXSModelIsValid = false;
        if (fXSModel)
        {
            delete fXSModel;
            fXSModel = 0;
        }
        return true;
    }
    return false;
}

void XMLGrammarPoolImpl::lockPool()
{
    if (!fLocked)
    {
        fLocked = true;
        MemoryManager *memMgr = getMemoryManager();
        if(!fSynchronizedStringPool)
        {
            fSynchronizedStringPool = new (memMgr) XMLSynchronizedStringPool(fStringPool, 109, memMgr);
        }
        if (!fXSModelIsValid)
        {
            createXSModel();                  
        }
    }
}

void XMLGrammarPoolImpl::unlockPool()
{
    if (fLocked)
    {
        fLocked = false;
        if(fSynchronizedStringPool)
        {
            fSynchronizedStringPool->flushAll();
            // if user calls Lock again, need to have null fSynchronizedStringPool
            delete fSynchronizedStringPool;
            fSynchronizedStringPool = 0;
        }
        fXSModelIsValid = false;
        if (fXSModel) 
        {     
            delete fXSModel;
            fXSModel = 0;
        }
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

XMLDTDDescription*  XMLGrammarPoolImpl::createDTDDescription(const XMLCh* const systemId)
{ 
	return new (getMemoryManager()) XMLDTDDescriptionImpl(systemId, getMemoryManager()); 
}
								   
XMLSchemaDescription* XMLGrammarPoolImpl::createSchemaDescription(const XMLCh* const targetNamespace)
{
	return new (getMemoryManager()) XMLSchemaDescriptionImpl(targetNamespace, getMemoryManager()); 
}

XSModel *XMLGrammarPoolImpl::getXSModel() 
{
    if (fLocked)
        return fXSModel;

    if (fXSModelIsValid)
        return fXSModel;
        
    createXSModel();    
    return fXSModel;
}

XSModel *XMLGrammarPoolImpl::getXSModel(bool& XSModelWasChanged)
{
    XSModelWasChanged = false;
    if (fLocked || fXSModelIsValid)
        return fXSModel;
        
    createXSModel();        
    XSModelWasChanged = true;
    return fXSModel;
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
 */

/***
 *   .non-empty gramamrRegistry
 ***/
void XMLGrammarPoolImpl::serializeGrammars(BinOutputStream* const binOut)
{
    RefHashTableOfEnumerator<Grammar> grammarEnum(fGrammarRegistry, false, getMemoryManager());
    if (!(grammarEnum.hasMoreElements())) 
    {        
        ThrowXMLwithMemMgr(XSerializationException, XMLExcepts::XSer_GrammarPool_Empty, getMemoryManager());
    }
        
    XSerializeEngine  serEng(binOut, this);

    //version information
    serEng<<(unsigned int)XERCES_GRAMMAR_SERIALIZATION_LEVEL;

    //lock status
    serEng<<fLocked;

    //StringPool, don't use <<
    fStringPool->serialize(serEng);

    /***
     * Serialize RefHashTableOf<Grammar>*    fGrammarRegistry; 
     ***/
    XTemplateSerializer::storeObject(fGrammarRegistry, serEng);
}

/***
 *   .empty stringPool
 *   .empty gramamrRegistry
 ***/
void XMLGrammarPoolImpl::deserializeGrammars(BinInputStream* const binIn)
{
    MemoryManager *memMgr = getMemoryManager();
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
            ThrowXMLwithMemMgr(XSerializationException, XMLExcepts::XSer_StringPool_NotEmpty, memMgr);
        }
    }

    RefHashTableOfEnumerator<Grammar> grammarEnum(fGrammarRegistry, false, memMgr);
    if (grammarEnum.hasMoreElements()) 
    {
        ThrowXMLwithMemMgr(XSerializationException, XMLExcepts::XSer_GrammarPool_NotEmpty, memMgr);
    }

    // This object will take care of cleaning up if an exception is
    // thrown during deserialization.
    JanitorMemFunCall<XMLGrammarPoolImpl>   cleanup(this, &XMLGrammarPoolImpl::cleanUp);

    try 
    {
        XSerializeEngine  serEng(binIn, this);

        //version information
        unsigned int  StorerLevel;
        serEng>>StorerLevel;
        serEng.fStorerLevel = (short)StorerLevel;

        //if the storer level is after the loader level
        //the loader shall not read the data any more
        if (StorerLevel > (unsigned int) XERCES_GRAMMAR_SERIALIZATION_LEVEL)
        {
            XMLCh     StorerLevelChar[5];
            XMLCh     LoaderLevelChar[5];
            XMLString::binToText(StorerLevel,                          StorerLevelChar,   4, 10, memMgr);
            XMLString::binToText(XERCES_GRAMMAR_SERIALIZATION_LEVEL,   LoaderLevelChar,   4, 10, memMgr);
            
            ThrowXMLwithMemMgr2(XSerializationException
                    , XMLExcepts::XSer_Storer_NewerThan_Loader
                    , StorerLevelChar
                    , LoaderLevelChar
                    , memMgr);
        }

        //lock status
        serEng>>fLocked;

        //StringPool, don't use >>
        fStringPool->serialize(serEng);

        /***
         * Deserialize RefHashTableOf<Grammar>*    fGrammarRegistry; 
         ***/
        XTemplateSerializer::loadObject(&fGrammarRegistry, 29, true, serEng);

    }
    catch(const OutOfMemoryException&)
    {
        // This is a special case, because we don't want
        // to execute cleanup code on out-of-memory
        // conditions.
        cleanup.release();

        throw;
    }

    // Everything is OK, so we can release the cleanup object.
    cleanup.release();

    if (fLocked) 
    {
        createXSModel();              
    }
}


void
XMLGrammarPoolImpl::cleanUp()
{
    fLocked = false;

    clear();
}


XERCES_CPP_NAMESPACE_END
