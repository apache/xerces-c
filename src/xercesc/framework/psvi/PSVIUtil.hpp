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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.1  2003/11/20 18:21:22  knoaman
 * Initial revision.
 *
 */

#if !defined(PSVIUTIL_HPP)
#define PSVIUTIL_HPP

#include <xercesc/framework/psvi/XSConstants.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XSObject;
class XSAttributeUse;
class XSAttributeDeclaration;
class XSModel;
class XSElementDeclaration;
class XSComplexTypeDefinition;
class XSWildcard;
class SchemaAttDef;
class SchemaElementDecl;
class DatatypeValidator;
class ContentSpecNode;
class ComplexTypeInfo;
class XSIDCDefinition;
class IdentityConstraint;

/**
 * Some useful utility methods.
 */
class XMLPARSER_EXPORT PSVIUtil
{
public:

    // Creates a particle that corresponds to the content spec node
    static XSParticle* createModelGroupParticle
    (
        const ContentSpecNode* const node
        , XSModel* const             xsModel
        , MemoryManager* const       manager = XMLPlatformUtils::fgMemoryManager
    );

    static XSAttributeDeclaration* addOrFind
    (
        SchemaAttDef* const attDef
        , XSModel* const xsModel
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    static XSSimpleTypeDefinition* addOrFind
    (
        DatatypeValidator* const validator
        , XSModel* const xsModel
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    static XSElementDeclaration* addOrFind
    (
        SchemaElementDecl* const elemDecl
        , XSModel* const xsModel
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    static XSComplexTypeDefinition* addOrFind
    (
        ComplexTypeInfo* const typeInfo
        , XSModel* const xsModel
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    static XSIDCDefinition* addOrFind
    (
        IdentityConstraint* const ic
        , XSModel* const xsModel
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );

    static XSAttributeUse* createXSAttributeUse
    (
        XSAttributeDeclaration* const xsAttDecl
        , XSModel* const xsModel
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    static XSWildcard* createXSWildcard
    (
        SchemaAttDef* const attDef
        , XSModel* const xsModel
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );

    static XSAnnotation* getAnnotationFromModel
    (
        XSModel* const xsModel
        , const void* const key
    );

    static XSNamespaceItem* getNamespaceItem
    (
        const XMLCh* const namespaceKey
        , XSModel* const xsModel
    );

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and destructor
    // -----------------------------------------------------------------------
    PSVIUtil();
    PSVIUtil(const PSVIUtil&);
    PSVIUtil& operator=(const PSVIUtil&);

    // -----------------------------------------------------------------------
    //  Helper methods
    // -----------------------------------------------------------------------
    // creates a particle corresponding to an element
    static XSParticle* createElementParticle
    (
        const ContentSpecNode* const rootNode
        , XSModel* const             xsModel
        , MemoryManager* const       manager
    );

    // creates a particle corresponding to a wildcard
    static XSParticle* createWildcardParticle
    (
        const ContentSpecNode* const rootNode
        , XSModel* const             xsModel
        , MemoryManager* const       manager
    );

    static XSWildcard* createXSWildcard
    (
        const ContentSpecNode* const rootNode
        , XSModel* const xsModel
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );

    static void buildAllParticles
    (
        const ContentSpecNode* const rootNode
        , XSParticleList* const particleList
        , XSModel* const xsModel
        , MemoryManager* const manager
    );

    static void buildChoiceSequenceParticles
    (
        const ContentSpecNode* const rootNode
        , XSParticleList* const particleList
        , XSModel* const xsModel
        , MemoryManager* const manager
    );

    static void putObjectInMap
    (
        void* key
        , XSObject* const object
        , XSModel* const xsModel
    );

    static XSObject* getObjectFromMap
    (
        void* key
        , XSModel* const xsModel
    );
};

XERCES_CPP_NAMESPACE_END

#endif
