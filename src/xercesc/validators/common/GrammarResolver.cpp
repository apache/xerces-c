/*
 * Copyright 2001,2004 The Apache Software Foundation.
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
 * Revision 1.32  2005/04/29 20:21:02  dbertoni
 * Patch for Jira issue XERCESC-1407.
 *
 * Revision 1.31  2005/03/20 14:53:35  knoaman
 * [jira 1381] Memory leak in GrammarResolver - patch by Christian Will.
 *
 * Revision 1.30  2004/12/03 17:00:13  amassari
 * Avoid throwing an exception when orphaning a cached grammar that has not been used yet
 *
 * Revision 1.29  2004/09/29 19:27:07  cargilld
 * Fix for Jira-1217: fixing problems with getXSModel.
 *
 * Revision 1.28  2004/09/08 13:56:51  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.27  2004/09/02 14:13:13  amassari
 * If getXSModel was invoked when schema caching was off and nothing had changed, a new (empty) model was created even if a non-empty one was already available
 *
 * Revision 1.26  2004/05/21 15:19:16  peiyongz
 * [jira1216]  GrammarResolver never clears internal cache of
 * Grammar instances -- patch from David Bertoni
 *
 * Revision 1.25  2004/02/17 15:53:50  neilg
 * fix potential memory leak
 *
 * Revision 1.24  2004/01/06 18:52:15  knoaman
 * Reset list of grammars after building XSModel
 *
 * Revision 1.23  2003/12/31 02:36:03  neilg
 * Even if the resolver has no grammars, since all schema
 * processors are aware of the schema-for-schemas, an XSModel
 * should be produced.
 *
 * Revision 1.22  2003/12/17 00:18:38  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.21  2003/11/21 22:38:50  neilg
 * Enable grammar pools and grammar resolvers to manufacture
 * XSModels.  This also cleans up handling in the
 * parser classes by eliminating the need to tell
 * the grammar pool that schema compoments need to be produced.
 * Thanks to David Cargill.
 *
 * Revision 1.20  2003/11/06 21:53:52  neilg
 * update grammar pool interface so that cacheGrammar(Grammar) can tell the caller whether the grammar was accepted.  Also fix some documentation errors.
 *
 * Revision 1.19  2003/09/16 18:30:54  neilg
 * make Grammar pool be responsible for creating and owning URI string pools.  This is one more step towards having grammars be independent of the parsers involved in their creation
 *
 * Revision 1.18  2003/09/02 09:04:44  gareth
 * added API to get enumerator to referenced grammars.
 *
 * Revision 1.17  2003/09/01 12:59:59  gareth
 * added API to get an enumerator for the cached grammars.
 *
 * Revision 1.16  2003/08/22 09:04:41  gareth
 * keep the fGrammarFromPool in sync to avoid problems when parseing multiple times. Patch by Alberto Massari.
 *
 * Revision 1.15  2003/07/31 17:08:39  peiyongz
 * Grammar embed grammar description
 * fGrammarFromPool introduced
 *
 * Revision 1.14  2003/06/25 22:38:40  peiyongz
 * remove old getGrammar()
 *
 * Revision 1.13  2003/06/25 19:32:05  peiyongz
 * remove old putGrammar()
 *
 * Revision 1.12  2003/06/25 18:57:56  peiyongz
 * remove old orphanGrammar()
 *
 * Revision 1.11  2003/06/25 17:44:04  peiyongz
 * cacheGrammar() revised
 *
 * Revision 1.10  2003/06/23 15:53:50  peiyongz
 * clean up temporary XMLGrammarDescription to make MemoryTest happy
 *
 * Revision 1.9  2003/06/20 18:58:45  peiyongz
 * Stateless Grammar Pool :: Part I
 *
 * Revision 1.8  2003/05/18 14:02:06  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.7  2003/05/16 21:43:20  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.6  2003/05/15 18:48:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.5  2002/11/04 14:54:58  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/09/24 19:48:39  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.3  2002/07/12 14:35:37  knoaman
 * Add an error message and use it in the scanner.
 *
 * Revision 1.2  2002/07/11 18:17:43  knoaman
 * Grammar caching/preparsing - initial implementation.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:38  peiyongz
 * sane_include
 *
 * Revision 1.5  2001/08/28 19:20:54  tng
 * Schema: xsi:type support
 *
 * Revision 1.4  2001/07/24 18:33:13  knoaman
 * Added support for <group> + extra constraint checking for complexType
 *
 * Revision 1.3  2001/06/07 20:58:39  tng
 * Fix no newline at the end warning.  By Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/11 13:27:19  tng
 * Copyright update.
 *
 * Revision 1.1  2001/03/21 21:56:28  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 */

#include <xercesc/validators/common/GrammarResolver.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/validators/schema/SchemaGrammar.hpp>
#include <xercesc/validators/schema/XMLSchemaDescriptionImpl.hpp>
#include <xercesc/validators/DTD/XMLDTDDescriptionImpl.hpp>
#include <xercesc/internal/XMLGrammarPoolImpl.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  GrammarResolver: Constructor and Destructor
// ---------------------------------------------------------------------------
GrammarResolver::GrammarResolver(XMLGrammarPool* const gramPool
                               , MemoryManager*  const manager)
:fCacheGrammar(false)
,fUseCachedGrammar(false)
,fGrammarPoolFromExternalApplication(true)
,fStringPool(0)
,fGrammarBucket(0)
,fGrammarFromPool(0)
,fDataTypeReg(0)
,fMemoryManager(manager)
,fGrammarPool(gramPool)
,fXSModel(0)
,fGrammarPoolXSModel(0)
{
    fGrammarBucket = new (manager) RefHashTableOf<Grammar>(29, true,  manager);

    /***
     * Grammars in this set are not owned
     */    
    fGrammarFromPool = new (manager) RefHashTableOf<Grammar>(29, false,  manager);

    if (!gramPool)
    {
        /***
         * We need to instantiate a default grammar pool object so that
         * all grammars and grammar components could be created through
         * the Factory methods
         */
        fGrammarPool = new (manager) XMLGrammarPoolImpl(manager);     
        fGrammarPoolFromExternalApplication=false;
    }
    fStringPool = fGrammarPool->getURIStringPool();

    // REVISIT: size
    fGrammarsToAddToXSModel = new (manager) ValueVectorOf<SchemaGrammar*> (29, manager);
}

GrammarResolver::~GrammarResolver()
{  
    delete fGrammarBucket;
    delete fGrammarFromPool;

    if (fDataTypeReg)
      delete fDataTypeReg;

   /***
    *  delete the grammar pool iff it is created by this resolver
    */
   if (!fGrammarPoolFromExternalApplication)
       delete fGrammarPool;

   if (fXSModel)
       delete fXSModel;
   // don't delete fGrammarPoolXSModel! we don't own it!
   delete fGrammarsToAddToXSModel;
}

// ---------------------------------------------------------------------------
//  GrammarResolver: Getter methods
// ---------------------------------------------------------------------------
DatatypeValidator*
GrammarResolver::getDatatypeValidator(const XMLCh* const uriStr,
                                      const XMLCh* const localPartStr) {

    DatatypeValidator* dv = 0;

    if (XMLString::equals(uriStr, SchemaSymbols::fgURI_SCHEMAFORSCHEMA)) {

        if (!fDataTypeReg) {

            fDataTypeReg = new (fMemoryManager) DatatypeValidatorFactory(fMemoryManager);
            fDataTypeReg->expandRegistryToFullSchemaSet();
        }

        dv = fDataTypeReg->getDatatypeValidator(localPartStr);
    }
    else {

        Grammar* grammar = getGrammar(uriStr);

        if (grammar && grammar->getGrammarType() == Grammar::SchemaGrammarType) {

            XMLBuffer nameBuf(128, fMemoryManager);

            nameBuf.set(uriStr);
            nameBuf.append(chComma);
            nameBuf.append(localPartStr);

            dv = ((SchemaGrammar*) grammar)->getDatatypeRegistry()->getDatatypeValidator(nameBuf.getRawBuffer());
        }
    }

    return dv;
}

Grammar* GrammarResolver::getGrammar( const XMLCh* const namespaceKey)
{
    if (!namespaceKey)
        return 0;

    Grammar* grammar = fGrammarBucket->get(namespaceKey);

    if (grammar)
        return grammar;

    if (fUseCachedGrammar)
    {
        grammar = fGrammarFromPool->get(namespaceKey);
        if (grammar)
        {
            return grammar;
        }
        else
        {
            XMLSchemaDescription* gramDesc = fGrammarPool->createSchemaDescription(namespaceKey);
            Janitor<XMLGrammarDescription> janName(gramDesc);
            grammar = fGrammarPool->retrieveGrammar(gramDesc);
            if (grammar)
            {
                fGrammarFromPool->put((void*) grammar->getGrammarDescription()->getGrammarKey(), grammar);
            }
            return grammar;
        }
    }

    return 0;
}

Grammar* GrammarResolver::getGrammar( XMLGrammarDescription* const gramDesc)
{
    if (!gramDesc)
        return 0;

    Grammar* grammar = fGrammarBucket->get(gramDesc->getGrammarKey());

    if (grammar)
        return grammar;

    if (fUseCachedGrammar)
    {
        grammar = fGrammarFromPool->get(gramDesc->getGrammarKey());
        if (grammar)
        {
            return grammar;
        }
        else
        {
            grammar = fGrammarPool->retrieveGrammar(gramDesc);
            if (grammar)
            {
                fGrammarFromPool->put((void*) grammar->getGrammarDescription()->getGrammarKey(), grammar);
            }
            return grammar;
        }
    }

    return 0;
}

RefHashTableOfEnumerator<Grammar>
GrammarResolver::getGrammarEnumerator() const
{
    return RefHashTableOfEnumerator<Grammar>(fGrammarBucket, false, fMemoryManager);
}

RefHashTableOfEnumerator<Grammar>
GrammarResolver::getReferencedGrammarEnumerator() const
{
    return RefHashTableOfEnumerator<Grammar>(fGrammarFromPool, false, fMemoryManager);
}

RefHashTableOfEnumerator<Grammar>
GrammarResolver::getCachedGrammarEnumerator() const
{
    return fGrammarPool->getGrammarEnumerator();
}

bool GrammarResolver::containsNameSpace( const XMLCh* const nameSpaceKey )
{
    if (!nameSpaceKey)
        return false;
    if (fGrammarBucket->containsKey(nameSpaceKey))
        return true;
    if (fUseCachedGrammar)
    {
        if (fGrammarFromPool->containsKey(nameSpaceKey))
            return true;

        // Lastly, need to check in fGrammarPool        
        XMLSchemaDescription* gramDesc = fGrammarPool->createSchemaDescription(nameSpaceKey);
        Janitor<XMLGrammarDescription> janName(gramDesc);
        Grammar* grammar = fGrammarPool->retrieveGrammar(gramDesc);
        if (grammar)
            return true;
    }

    return false;
}

void GrammarResolver::putGrammar(Grammar* const grammarToAdopt)
{
    if (!grammarToAdopt)
        return;

    /***
     * the grammar will be either in the grammarpool, or in the grammarbucket
     */
    if (!fCacheGrammar || !fGrammarPool->cacheGrammar(grammarToAdopt))
    {
        // either we aren't caching or the grammar pool doesn't want it
        // so we need to look after it
        fGrammarBucket->put( (void*) grammarToAdopt->getGrammarDescription()->getGrammarKey(), grammarToAdopt );
        if (grammarToAdopt->getGrammarType() == Grammar::SchemaGrammarType)
        {
            fGrammarsToAddToXSModel->addElement((SchemaGrammar*) grammarToAdopt);
        }
    }
}

// ---------------------------------------------------------------------------
//  GrammarResolver: methods
// ---------------------------------------------------------------------------
void GrammarResolver::reset() {
    fGrammarBucket->removeAll();
    fGrammarsToAddToXSModel->removeAllElements();
    delete fXSModel;
    fXSModel = 0;
}

void GrammarResolver::resetCachedGrammar()
{
    //REVISIT: if the pool is locked this will fail... should throw an exception?
    fGrammarPool->clear();
    // Even though fXSModel and fGrammarPoolXSModel will be invalid don't touch 
    // them here as getXSModel will handle this.

    //to clear all other references to the grammars just deleted from fGrammarPool
    fGrammarFromPool->removeAll(); 

}

void GrammarResolver::cacheGrammars()
{
    RefHashTableOfEnumerator<Grammar> grammarEnum(fGrammarBucket, false, fMemoryManager);
    ValueVectorOf<XMLCh*> keys(8, fMemoryManager);
    unsigned int keyCount = 0;

    // Build key set
    while (grammarEnum.hasMoreElements()) 
    {
        XMLCh* grammarKey = (XMLCh*) grammarEnum.nextElementKey();
        keys.addElement(grammarKey);
        keyCount++;
    }

    // PSVI: assume everything will be added, if caching fails add grammar back 
    //       into vector
    fGrammarsToAddToXSModel->removeAllElements();

    // Cache
    for (unsigned int i = 0; i < keyCount; i++) 
    {
        XMLCh* grammarKey = keys.elementAt(i);    

        /***
         * It is up to the GrammarPool implementation to handle duplicated grammar
         */
        Grammar* grammar = fGrammarBucket->get(grammarKey);
        if(fGrammarPool->cacheGrammar(grammar))
        {
            // only orphan grammar if grammar pool accepts caching of it
            fGrammarBucket->orphanKey(grammarKey);
        }
        else if (grammar->getGrammarType() == Grammar::SchemaGrammarType)
        {
            // add it back to list of grammars not in grammar pool
            fGrammarsToAddToXSModel->addElement((SchemaGrammar*) grammar);           
        }
    }

}

// ---------------------------------------------------------------------------
//  GrammarResolver: Setter methods
// ---------------------------------------------------------------------------
void GrammarResolver::cacheGrammarFromParse(const bool aValue)
{
    reset();
    fCacheGrammar = aValue;
}

Grammar* GrammarResolver::orphanGrammar(const XMLCh* const nameSpaceKey)
{
    if (fCacheGrammar)
    {
        Grammar* grammar = fGrammarPool->orphanGrammar(nameSpaceKey);
        if (grammar)
        {
            if (fGrammarFromPool->containsKey(nameSpaceKey))
                fGrammarFromPool->removeKey(nameSpaceKey);
        }
        // Check to see if it's in fGrammarBucket, since
        // we put it there if the grammar pool refused to
        // cache it.
        else if (fGrammarBucket->containsKey(nameSpaceKey))
        {
            grammar = fGrammarBucket->orphanKey(nameSpaceKey);
        }

        return grammar;
    }
    else
    {
        return fGrammarBucket->orphanKey(nameSpaceKey);
    }
}

XSModel *GrammarResolver::getXSModel()
{
    XSModel* xsModel;
    if (fCacheGrammar || fUseCachedGrammar)
    {
        // We know if the grammarpool changed thru caching, orphaning and erasing
        // but NOT by other mechanisms such as lockPool() or unlockPool() so it
        // is safest to always get it.  The grammarPool XSModel will only be 
        // regenerated if something changed.
        bool XSModelWasChanged;
        // The grammarpool will always return an xsmodel, even if it is just
        // the schema for schema xsmodel...
        xsModel = fGrammarPool->getXSModel(XSModelWasChanged);
        if (XSModelWasChanged)
        {
            // we know the grammarpool XSModel has changed or this is the
            // first call to getXSModel
            if (!fGrammarPoolXSModel && (fGrammarsToAddToXSModel->size() == 0) &&
                !fXSModel)
            { 
                fGrammarPoolXSModel = xsModel;
                return fGrammarPoolXSModel;
            }
            else
            {
                fGrammarPoolXSModel = xsModel;
                // We had previously augmented the grammar pool XSModel
                // with our our grammars or we would like to upate it now
                // so we have to regenerate the XSModel
                fGrammarsToAddToXSModel->removeAllElements();
                RefHashTableOfEnumerator<Grammar> grammarEnum(fGrammarBucket, false, fMemoryManager);
                while (grammarEnum.hasMoreElements()) 
                {
                    Grammar& grammar = (Grammar&) grammarEnum.nextElement();
                    if (grammar.getGrammarType() == Grammar::SchemaGrammarType)
                        fGrammarsToAddToXSModel->addElement((SchemaGrammar*)&grammar);
                }
                delete fXSModel;
                if (fGrammarsToAddToXSModel->size())
                {                    
                    fXSModel = new (fMemoryManager) XSModel(fGrammarPoolXSModel, this, fMemoryManager);
                    fGrammarsToAddToXSModel->removeAllElements();
                    return fXSModel;
                }       
                fXSModel = 0; 
                return fGrammarPoolXSModel;                                                      
            }       
        }
        else {
            // we know that the grammar pool XSModel is the same as before
            if (fGrammarsToAddToXSModel->size())
            {
                // we need to update our fXSModel with the new grammars               
                if (fXSModel)
                {
                    xsModel = new (fMemoryManager) XSModel(fXSModel, this, fMemoryManager);                   
                    fXSModel = xsModel;
                }
                else
                {
                    fXSModel = new (fMemoryManager) XSModel(fGrammarPoolXSModel, this, fMemoryManager);
                }
                fGrammarsToAddToXSModel->removeAllElements();
                return fXSModel;
            }
            // Nothing has changed!
            if (fXSModel)
            {
                return fXSModel;
            }
            else if (fGrammarPoolXSModel)
            {
                return fGrammarPoolXSModel;
            }
            fXSModel = new (fMemoryManager) XSModel(0, this, fMemoryManager);    
            return fXSModel;    
        }
    }
    // Not Caching...
    if (fGrammarsToAddToXSModel->size())
    {      
        xsModel = new (fMemoryManager) XSModel(fXSModel, this, fMemoryManager);
        fGrammarsToAddToXSModel->removeAllElements();
        fXSModel = xsModel;             
    }
    else if (!fXSModel)
    {
        // create a new model only if we didn't have one already
        fXSModel = new (fMemoryManager) XSModel(0, this, fMemoryManager);
    }
    return fXSModel; 
}

XERCES_CPP_NAMESPACE_END
