/*
 * Copyright 1999-2001,2004 The Apache Software Foundation.
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
 * Revision 1.22  2004/10/28 20:18:26  peiyongz
 * Data member reshuffle
 *
 * Revision 1.21  2004/10/20 15:19:07  knoaman
 * Allow option of initializing static data in XMLPlatformUtils::Initialize
 *
 * Revision 1.20  2004/09/08 13:56:50  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.19  2004/07/21 15:07:13  peiyongz
 * rewind previous change
 *
 * Revision 1.17  2004/03/03 23:03:28  peiyongz
 * Using serialize() to save/load object created in ctor
 *
 * Revision 1.16  2004/01/29 11:52:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.15  2004/01/13 16:17:09  knoaman
 * Fo sanity, use class name to qualify method
 *
 * Revision 1.14  2004/01/09 22:41:58  knoaman
 * Use a global static mutex for locking when creating local static mutexes instead of compareAndSwap
 *
 * Revision 1.13  2003/11/12 20:32:47  peiyongz
 * Do not serialize/deserialize fElemNonDeclPool
 *
 * Revision 1.12  2003/11/06 15:03:45  peiyongz
 * initialize data member
 *
 * Revision 1.11  2003/10/17 21:14:30  peiyongz
 * using XTemplateSerializer
 *
 * Revision 1.10  2003/10/14 15:20:42  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.9  2003/09/22 19:49:02  neilg
 * implement change to Grammar::putElem(XMLElementDecl, bool).  If Grammars are used only to hold declared objects, there will be no need for the fElemNonDeclPool tables; make Grammar implementations lazily create them only if the application requires them (which good cpplications should not.)
 *
 * Revision 1.8  2003/08/14 03:00:46  knoaman
 * Code refactoring to improve performance of validation.
 *
 * Revision 1.7  2003/07/31 17:09:59  peiyongz
 * Grammar embed grammar description
 *
 * Revision 1.6  2003/05/18 14:02:06  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.5  2003/05/15 18:54:50  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2002/12/04 02:47:25  knoaman
 * scanner re-organization.
 *
 * Revision 1.3  2002/11/04 14:50:40  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/07/11 18:19:28  knoaman
 * Grammar caching/preparsing - initial implementation.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:43  peiyongz
 * sane_include
 *
 * Revision 1.4  2001/09/14 14:50:22  tng
 * Schema: Fix some wildcard bugs, and some retrieving qualified/unqualified element decl problems.
 *
 * Revision 1.3  2001/05/11 13:27:08  tng
 * Copyright update.
 *
 * Revision 1.2  2001/04/19 18:17:20  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.1  2001/03/21 21:56:20  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMLRegisterCleanup.hpp>
#include <xercesc/util/XMLInitializer.hpp>
#include <xercesc/validators/DTD/DTDGrammar.hpp>
#include <xercesc/validators/DTD/XMLDTDDescriptionImpl.hpp>

#include <xercesc/internal/XTemplateSerializer.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Local static data
// ---------------------------------------------------------------------------
static bool               sEntityPoolMutexRegistered = false;
static XMLMutex*          sEntityPoolMutex = 0;
static XMLRegisterCleanup entityPoolRegistryCleanup;

// ---------------------------------------------------------------------------
//  DTDGrammar: Static member data
// ---------------------------------------------------------------------------
NameIdPool<DTDEntityDecl>* DTDGrammar::fDefaultEntities = 0;

void XMLInitializer::initializeDTDGrammarDfltEntities()
{
    DTDGrammar::fDefaultEntities = new NameIdPool<DTDEntityDecl>(11, 12);

    // Add the default entity entries for the character refs that must
    // always be present. We indicate that they are from the internal
    // subset. They aren't really, but they have to look that way so
    // that they are still valid for use within a standalone document.
    //
    // We also mark them as special char entities, which allows them
    // to be used in places whether other non-numeric general entities
    // cannot.
    //
    if (DTDGrammar::fDefaultEntities)
    {
        DTDGrammar::fDefaultEntities->put(new DTDEntityDecl(XMLUni::fgAmp, chAmpersand, true, true));
        DTDGrammar::fDefaultEntities->put(new DTDEntityDecl(XMLUni::fgLT, chOpenAngle, true, true));
        DTDGrammar::fDefaultEntities->put(new DTDEntityDecl(XMLUni::fgGT, chCloseAngle, true, true));
        DTDGrammar::fDefaultEntities->put(new DTDEntityDecl(XMLUni::fgQuot, chDoubleQuote, true, true));
        DTDGrammar::fDefaultEntities->put(new DTDEntityDecl(XMLUni::fgApos, chSingleQuote, true, true));

        // register cleanup method
        entityPoolRegistryCleanup.registerCleanup(DTDGrammar::reinitDfltEntities);
        sEntityPoolMutexRegistered = true;
    }
}

//---------------------------------------------------------------------------
//  DTDGrammar: Constructors and Destructor
// ---------------------------------------------------------------------------
DTDGrammar::DTDGrammar(MemoryManager* const manager) :
    fMemoryManager(manager)
    , fElemDeclPool(0)
    , fElemNonDeclPool(0)
    , fEntityDeclPool(0)
    , fNotationDeclPool(0)
    , fGramDesc(0)
    , fRootElemId(0)
    , fValidated(false)
{
    //
    //  Init all the pool members.
    //
    //  <TBD> Investigate what the optimum values would be for the various
    //  pools.
    //
    fElemDeclPool = new (fMemoryManager) NameIdPool<DTDElementDecl>(109, 128, fMemoryManager);
    // should not need this in the common situation where grammars
    // are built once and then read - NG
    //fElemNonDeclPool = new (fMemoryManager) NameIdPool<DTDElementDecl>(29, 128, fMemoryManager);
    fEntityDeclPool = new (fMemoryManager) NameIdPool<DTDEntityDecl>(109, 128, fMemoryManager);
    fNotationDeclPool = new (fMemoryManager) NameIdPool<XMLNotationDecl>(109, 128, fMemoryManager);

    //REVISIT: use grammarPool to create
    fGramDesc = new (fMemoryManager) XMLDTDDescriptionImpl(XMLUni::fgDTDEntityString, fMemoryManager);

    // Create default entities
    resetEntityDeclPool();
}

DTDGrammar::~DTDGrammar()
{
    delete fElemDeclPool;
    if(fElemNonDeclPool) 
    {
        delete fElemNonDeclPool;
    }
    delete fEntityDeclPool;
    delete fNotationDeclPool;
    delete fGramDesc;
}

// -----------------------------------------------------------------------
//  Notification that lazy data has been deleted
// -----------------------------------------------------------------------
void DTDGrammar::reinitDfltEntities() {

    delete fDefaultEntities;
    fDefaultEntities = 0;

    // delete local static data
    delete sEntityPoolMutex;
    sEntityPoolMutex = 0;
    sEntityPoolMutexRegistered = false;
}

// -----------------------------------------------------------------------
//  Virtual methods
// -----------------------------------------------------------------------
XMLElementDecl* DTDGrammar::findOrAddElemDecl (const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const
        , const XMLCh* const    qName
        , unsigned int          scope
        ,       bool&           wasAdded )
{
    // See it it exists
    DTDElementDecl* retVal = (DTDElementDecl*) getElemDecl(uriId, baseName, qName, scope);

    // if not, then add this in
    if (!retVal)
    {
        retVal = new (fMemoryManager) DTDElementDecl
        (
            qName
            , uriId
            , DTDElementDecl::Any
            , fMemoryManager
        );
        if(!fElemNonDeclPool)
            fElemNonDeclPool = new (fMemoryManager) NameIdPool<DTDElementDecl>(29, 128, fMemoryManager);
        const unsigned int elemId = fElemNonDeclPool->put(retVal);
        retVal->setId(elemId);
        wasAdded = true;
    }
     else
    {
        wasAdded = false;
    }
    return retVal;
}

XMLElementDecl* DTDGrammar::putElemDecl (const   unsigned int    uriId
        , const XMLCh* const
        , const XMLCh* const
        , const XMLCh* const    qName
        , unsigned int
        , const bool            notDeclared)
{
    DTDElementDecl* retVal = new (fMemoryManager) DTDElementDecl
    (
        qName
        , uriId
        , DTDElementDecl::Any
        , fMemoryManager
    );
    if(notDeclared)
    {
        if(!fElemNonDeclPool)
            fElemNonDeclPool = new (fMemoryManager) NameIdPool<DTDElementDecl>(29, 128, fMemoryManager);
        retVal->setId(fElemNonDeclPool->put(retVal));
    } else 
    {
        retVal->setId(fElemDeclPool->put(retVal));
    }
    return retVal;
}

void DTDGrammar::reset()
{
    //
    //  We need to reset all of the pools.
    //
    fElemDeclPool->removeAll();
    // now that we have this, no point in deleting it...
    if(fElemNonDeclPool)
        fElemNonDeclPool->removeAll();
    fNotationDeclPool->removeAll();
    fEntityDeclPool->removeAll();
    fValidated = false;
}

void DTDGrammar::resetEntityDeclPool() {

    // Initialize default entities if not initialized
    if (!sEntityPoolMutexRegistered)
    {
        if (!sEntityPoolMutex)
        {
            XMLMutexLock lock(XMLPlatformUtils::fgAtomicMutex);
            if (!sEntityPoolMutex)
                sEntityPoolMutex = new XMLMutex;
        }

        // Use a faux scope to synchronize while we do this
        {
            XMLMutexLock lock(sEntityPoolMutex);

            // If we got here first, then register it and set the registered flag
            if (!sEntityPoolMutexRegistered)
            {
                fDefaultEntities = new NameIdPool<DTDEntityDecl>(11, 12);

                //
                // Add the default entity entries for the character refs that must
                // always be present. We indicate that they are from the internal
                // subset. They aren't really, but they have to look that way so
                // that they are still valid for use within a standalone document.
                //
                // We also mark them as special char entities, which allows them
                // to be used in places whether other non-numeric general entities
                // cannot.
                //
                fDefaultEntities->put(new DTDEntityDecl(XMLUni::fgAmp, chAmpersand, true, true));
                fDefaultEntities->put(new DTDEntityDecl(XMLUni::fgLT, chOpenAngle, true, true));
                fDefaultEntities->put(new DTDEntityDecl(XMLUni::fgGT, chCloseAngle, true, true));
                fDefaultEntities->put(new DTDEntityDecl(XMLUni::fgQuot, chDoubleQuote, true, true));
                fDefaultEntities->put(new DTDEntityDecl(XMLUni::fgApos, chSingleQuote, true, true));

                // register cleanup method
                entityPoolRegistryCleanup.registerCleanup(DTDGrammar::reinitDfltEntities);
                sEntityPoolMutexRegistered = true;
            }
        }
    }
}

void DTDGrammar::setGrammarDescription( XMLGrammarDescription* gramDesc)
{
    if ((!gramDesc) || 
        (gramDesc->getGrammarType() != Grammar::DTDGrammarType))
        return;

    if (fGramDesc)
        delete fGramDesc;

    //adopt the grammar Description
    fGramDesc = (XMLDTDDescription*) gramDesc;
}

XMLGrammarDescription*  DTDGrammar::getGrammarDescription() const
{
    return fGramDesc;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(DTDGrammar)

void DTDGrammar::serialize(XSerializeEngine& serEng)
{

    Grammar::serialize(serEng);

    //don't serialize fDefaultEntities

    if (serEng.isStoring())
    {
        /***
         *
         * Serialize NameIdPool<DTDElementDecl>*       fElemDeclPool;
         * Serialize NameIdPool<DTDEntityDecl>*        fEntityDeclPool;
         * Serialize NameIdPool<XMLNotationDecl>*      fNotationDeclPool;
         ***/
        XTemplateSerializer::storeObject(fElemDeclPool, serEng);
        XTemplateSerializer::storeObject(fEntityDeclPool, serEng);
        XTemplateSerializer::storeObject(fNotationDeclPool, serEng);

        /***
         * serialize() method shall be used to store object
         * which has been created in ctor
         ***/
        fGramDesc->serialize(serEng);

        serEng<<fRootElemId;
        serEng<<fValidated;
    }
    else
    {

       /***
         *
         * Deserialize NameIdPool<DTDElementDecl>*       fElemDeclPool;
         * Deserialize NameIdPool<DTDEntityDecl>*        fEntityDeclPool;
         * Deerialize NameIdPool<XMLNotationDecl>*       fNotationDeclPool;
         ***/
        XTemplateSerializer::loadObject(&fElemDeclPool, 109, 128, serEng);
        fElemNonDeclPool = 0;
        XTemplateSerializer::loadObject(&fEntityDeclPool, 109, 128, serEng);
        XTemplateSerializer::loadObject(&fNotationDeclPool, 109, 128, serEng);

        /***
         * serialize() method shall be used to load object
         * which has been created in ctor
         ***/
        fGramDesc->serialize(serEng);

        serEng>>fRootElemId;
        serEng>>fValidated;
    }

}

XERCES_CPP_NAMESPACE_END
