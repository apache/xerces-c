/*
 * The Apache Software License, Version 1.1
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
 * $Log$
 * Revision 1.3  2002/11/04 14:49:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/07/11 18:21:20  knoaman
 * Grammar caching/preparsing - initial implementation.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:46  peiyongz
 * sane_include
 *
 * Revision 1.11  2002/01/02 15:20:22  tng
 * Schema Fix: should not store a temp value as the key in the element pool and the attribute pool.
 *
 * Revision 1.10  2001/11/19 18:26:31  knoaman
 * no message
 *
 * Revision 1.9  2001/10/09 12:18:26  tng
 * Leak fix: should use delete [] to delete fTargetNamespace.
 *
 * Revision 1.8  2001/09/14 14:50:22  tng
 * Schema: Fix some wildcard bugs, and some retrieving qualified/unqualified element decl problems.
 *
 * Revision 1.7  2001/08/21 16:06:11  tng
 * Schema: Unique Particle Attribution Constraint Checking.
 *
 * Revision 1.6  2001/07/31 15:27:10  knoaman
 * Added support for <attributeGroup>.
 *
 * Revision 1.5  2001/07/24 18:33:46  knoaman
 * Added support for <group> + extra constraint checking for complexType
 *
 * Revision 1.4  2001/06/25 12:51:56  knoaman
 * Add constraint checking on elements in complex types to prevent same
 * element names from having different definitions - use substitueGroups.
 *
 * Revision 1.3  2001/05/11 13:27:36  tng
 * Copyright update.
 *
 * Revision 1.2  2001/04/19 17:43:18  knoaman
 * More schema implementation classes.
 *
 * Revision 1.1  2001/03/21 21:56:33  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/SchemaGrammar.hpp>
#include <xercesc/validators/schema/NamespaceScope.hpp>
#include <xercesc/validators/schema/ComplexTypeInfo.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/validators/schema/XercesGroupInfo.hpp>
#include <xercesc/validators/schema/XercesAttGroupInfo.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  SchemaGrammar: Constructors and Destructor
// ---------------------------------------------------------------------------
SchemaGrammar::SchemaGrammar() :
    fTargetNamespace(0)
    , fElemDeclPool(0)
    , fElemNonDeclPool(0)
    , fGroupElemDeclPool(0)
    , fNotationDeclPool(0)
    , fAttributeDeclRegistry(0)
    , fComplexTypeRegistry(0)
    , fGroupInfoRegistry(0)
    , fAttGroupInfoRegistry(0)
    , fNamespaceScope(0)
    , fValidSubstitutionGroups(0)
    , fIDRefList(0)
    , fValidated(false)
{
    //
    //  Init all the pool members.
    //
    //  <TBD> Investigate what the optimum values would be for the various
    //  pools.
    //
    fElemDeclPool = new RefHash3KeysIdPool<SchemaElementDecl>(109);

    try {
        fElemNonDeclPool = new RefHash3KeysIdPool<SchemaElementDecl>(29);
        fGroupElemDeclPool = new RefHash3KeysIdPool<SchemaElementDecl>(109, false);
        fNotationDeclPool = new NameIdPool<XMLNotationDecl>(109);
        fIDRefList = new RefHashTableOf<XMLRefInfo>(29);
        fDatatypeRegistry.expandRegistryToFullSchemaSet();

        //
        //  Call our own reset method. This lets us have the pool setup stuff
        //  done in just one place (because this stame setup stuff has to be
        //  done every time we are reset.)
        //
        reset();
    }
    catch(...) {

        cleanUp();
        throw;
    }
}

SchemaGrammar::~SchemaGrammar()
{
    cleanUp();
}


// -----------------------------------------------------------------------
//  Virtual methods
// -----------------------------------------------------------------------
XMLElementDecl* SchemaGrammar::findOrAddElemDecl (const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    prefixName
        , const XMLCh* const    qName
        , unsigned int          scope
        ,       bool&           wasAdded )
{
    // See it it exists
    SchemaElementDecl* retVal = (SchemaElementDecl*) getElemDecl(uriId, baseName, qName, scope);

    // if not, then add this in
    if (!retVal)
    {
        retVal = new SchemaElementDecl(prefixName, baseName, uriId, SchemaElementDecl::Any);
        const unsigned int elemId = fElemNonDeclPool->put((void*)retVal->getBaseName(), uriId, scope, retVal);
        retVal->setId(elemId);
        wasAdded = true;
    }
     else
    {
        wasAdded = false;
    }
    return retVal;
}

XMLElementDecl* SchemaGrammar::putElemDecl (const   unsigned int    uriId
        , const XMLCh* const    baseName
        , const XMLCh* const    prefixName
        , const XMLCh* const    qName
        , unsigned int          scope
        , const bool            notDeclared)
{
    SchemaElementDecl* retVal = new SchemaElementDecl(prefixName, baseName, uriId, SchemaElementDecl::Any);
    const unsigned int elemId = (notDeclared) ? fElemNonDeclPool->put((void*)retVal->getBaseName(), uriId, scope, retVal)
                                              : fElemDeclPool->put((void*)retVal->getBaseName(), uriId, scope, retVal);
    retVal->setId(elemId);
    return retVal;
}

void SchemaGrammar::reset()
{
    //
    //  We need to reset all of the pools.
    //
    fElemDeclPool->removeAll();
    fElemNonDeclPool->removeAll();
    fGroupElemDeclPool->removeAll();
    fNotationDeclPool->removeAll();
    fValidated = false;
}


void SchemaGrammar::cleanUp()
{
    delete fElemDeclPool;
    delete fElemNonDeclPool;
    delete fGroupElemDeclPool;
    delete fNotationDeclPool;
    delete [] fTargetNamespace;
    delete fAttributeDeclRegistry;
    delete fComplexTypeRegistry;
    delete fGroupInfoRegistry;
    delete fAttGroupInfoRegistry;
    delete fNamespaceScope;
    delete fValidSubstitutionGroups;
    delete fIDRefList;
}

XERCES_CPP_NAMESPACE_END
