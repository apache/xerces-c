/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 *    permission, please contact apache@apache.org.
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
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.apache.org.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
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
,fStringPool(109, manager)
,fGrammarBucket(0)
,fDataTypeReg(0)
,fMemoryManager(manager)
,fGrammarPool(gramPool)
{

    fGrammarBucket = new (manager) RefHashTableOf<GrammarEntry>(29, true,  manager);

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

}

GrammarResolver::~GrammarResolver()
{  
    delete fGrammarBucket;

    if (fDataTypeReg)
      delete fDataTypeReg;

   /***
    *  delete the grammar pool iff it is created by this resolver
    */
   if (!fGrammarPoolFromExternalApplication)
       delete fGrammarPool;
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

//Deprecated
Grammar* GrammarResolver::getGrammar(const XMLCh* const nameSpaceKey)
{
    if (!nameSpaceKey)
        return 0;

    XMLGrammarDescription* gramDesc = getGrammarDescription(nameSpaceKey);
    Janitor<XMLGrammarDescription> janName(gramDesc);
    return getGrammar(gramDesc);

}

Grammar* GrammarResolver::getGrammar( XMLGrammarDescription* const gramDesc)
{
    if (!gramDesc)
        return 0;

    GrammarEntry* gramEntry = fGrammarBucket->get(gramDesc->getGrammarKey());

    if (gramEntry)
        return gramEntry->getGrammar();

    /***
     * if not found locally, try grammarPool if necessary
     */
    if (fUseCachedGrammar)
        return fGrammarPool->retrieveGrammar(gramDesc);

    return 0;
}

RefHashTableOfEnumerator<GrammarEntry>
GrammarResolver::getGrammarEnumerator() const
{
    return RefHashTableOfEnumerator<GrammarEntry>(fGrammarBucket);
}

bool GrammarResolver::containsNameSpace( const XMLCh* const nameSpaceKey )
{
   return fGrammarBucket->containsKey( nameSpaceKey );
}

//Deprecated
void GrammarResolver::putGrammar( const XMLCh* const nameSpaceKey, Grammar* const grammarToAdopt )
{
    XMLGrammarDescription* gramDesc = getGrammarDescription(nameSpaceKey);
    putGrammar(gramDesc, grammarToAdopt);
}

void GrammarResolver::putGrammar(XMLGrammarDescription* const gramDesc, Grammar* const grammarToAdopt)
{
    if (!gramDesc || !grammarToAdopt)
        return;

    /***
     * the grammar will be either in the grammarpool, or in the grammarbucket
     */
    if (fCacheGrammar)
       fGrammarPool->cacheGrammar(gramDesc, grammarToAdopt);
    else
    {
        /***
         * The grammarEntry in the GrammarBucket can use the parser's memory,
         * since itself won't go into the GrammarPool while its contained 
         * GrammarDescription and Grammar will.
         */
        GrammarEntry *theEntry = new (fMemoryManager) GrammarEntry(gramDesc, grammarToAdopt);
        fGrammarBucket->put( (void*) gramDesc->getGrammarKey(), theEntry );
    }

}

// ---------------------------------------------------------------------------
//  GrammarResolver: methods
// ---------------------------------------------------------------------------
void GrammarResolver::reset() {
   fGrammarBucket->removeAll();
}

void GrammarResolver::resetCachedGrammar()
{
    fGrammarPool->clear();

}

void GrammarResolver::cacheGrammars()
{
    RefHashTableOfEnumerator<GrammarEntry> grammarEnum(fGrammarBucket);

    /***
     * It is up to the GrammarPool to handle duplicated grammar
     *
     * Destroy the grammarEntry but reuse the grammar and description
     * embedded.
     *
     */
    while (grammarEnum.hasMoreElements()) 
    {
        XMLCh* grammarKey = (XMLCh*) grammarEnum.nextElementKey();
        GrammarEntry* theEntry = fGrammarBucket->orphanKey(grammarKey);
        XMLGrammarDescription* description = theEntry->getDescription();
        Grammar*               grammar     = theEntry->getGrammar();
        theEntry->nullGrammar();
        theEntry->nullDescription();
        delete theEntry;

        fGrammarPool->cacheGrammar(description, grammar);
    }

}

// ---------------------------------------------------------------------------
//  GrammarResolver: Setter methods
// ---------------------------------------------------------------------------
void GrammarResolver::cacheGrammarFromParse(const bool aValue)
{

    fCacheGrammar = aValue;
    fGrammarBucket->removeAll();
    fGrammarBucket->setAdoptElements(!fCacheGrammar);
}

Grammar* GrammarResolver::orphanGrammar(const XMLCh* const nameSpaceKey)
{
    XMLGrammarDescription* gramDesc = getGrammarDescription(nameSpaceKey);
    Janitor<XMLGrammarDescription> janName(gramDesc);
    return orphanGrammar(gramDesc);
}

Grammar* GrammarResolver::orphanGrammar(XMLGrammarDescription* const gramDesc)
{
    if (!gramDesc)
        return 0;

    if (fCacheGrammar)
        return fGrammarPool->orphanGrammar(gramDesc);

    GrammarEntry* theEntry = fGrammarBucket->orphanKey(gramDesc->getGrammarKey());
    if (theEntry)
    {
        Grammar* aGrammar = theEntry->getGrammar();
        theEntry->nullGrammar();
        delete theEntry;
        return aGrammar;
    }

    return 0;
}

XMLGrammarDescription* GrammarResolver::getGrammarDescription(const XMLCh* const nameSpaceKey)
{
    if (XMLString::equals(XMLUni::fgDTDEntityString, nameSpaceKey))
        return (XMLGrammarDescription*) fGrammarPool->createDTDDescription(nameSpaceKey);
    else 
        return (XMLGrammarDescription*) fGrammarPool->createSchemaDescription(nameSpaceKey);
}

XERCES_CPP_NAMESPACE_END
