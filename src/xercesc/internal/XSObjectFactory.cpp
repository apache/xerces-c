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
 * Revision 1.2  2003/11/21 22:34:46  neilg
 * More schema component model implementation, thanks to David Cargill.
 * In particular, this cleans up and completes the XSModel, XSNamespaceItem,
 * XSAttributeDeclaration and XSAttributeGroup implementations.
 *
 * Revision 1.1  2003/11/21 17:11:24  knoaman
 * Initial revision
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/internal/XSObjectFactory.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/framework/psvi/XSParticle.hpp>
#include <xercesc/framework/psvi/XSModelGroup.hpp>
#include <xercesc/framework/psvi/XSElementDeclaration.hpp>
#include <xercesc/framework/psvi/XSComplexTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSModelGroupDefinition.hpp>
#include <xercesc/framework/psvi/XSAttributeGroupDefinition.hpp>
#include <xercesc/framework/psvi/XSWildcard.hpp>
#include <xercesc/framework/psvi/XSNamespaceItem.hpp>
#include <xercesc/framework/psvi/XSIDCDefinition.hpp>
#include <xercesc/framework/psvi/XSAttributeUse.hpp>
#include <xercesc/framework/psvi/XSAttributeDeclaration.hpp>
#include <xercesc/framework/psvi/XSNotationDeclaration.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/datatype/DatatypeValidator.hpp>
#include <xercesc/validators/schema/SchemaAttDefList.hpp>
#include <xercesc/validators/schema/SchemaGrammar.hpp>
#include <xercesc/validators/schema/XercesGroupInfo.hpp>
#include <xercesc/validators/schema/XercesAttGroupInfo.hpp>
#include <xercesc/validators/schema/identity/IdentityConstraint.hpp>
#include <xercesc/validators/schema/identity/IC_KeyRef.hpp>
#include <xercesc/validators/schema/identity/XercesXPath.hpp>
#include <xercesc/util/HashPtr.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSObjectFactory: Constructors and Destructor
// ---------------------------------------------------------------------------
XSObjectFactory::XSObjectFactory(MemoryManager* const manager)
    : fMemoryManager(manager)
    , fXercesToXSMap(0)
    , fDeleteVector(0)
{
    fDeleteVector = new (manager) RefVectorOf<XSObject>(20, true, manager);
    fXercesToXSMap = new (manager) RefHashTableOf<XSObject> (
        109, false, new (manager) HashPtr(), manager);
}

XSObjectFactory::~XSObjectFactory()
{
    delete fXercesToXSMap;
    delete fDeleteVector;
}

// ---------------------------------------------------------------------------
//  XSObjectFactory: factory methods
// ---------------------------------------------------------------------------
XSParticle*
XSObjectFactory::createModelGroupParticle(const ContentSpecNode* const rootNode,
                                          XSModel* const xsModel)
{
    ContentSpecNode::NodeTypes nodeType = rootNode->getType();

    if (nodeType == ContentSpecNode::All
        || nodeType == ContentSpecNode::ModelGroupChoice
        || nodeType == ContentSpecNode::ModelGroupSequence)
    {
        XSParticleList* particleList = new (fMemoryManager) RefVectorOf<XSParticle> (4, true, fMemoryManager);
        XSAnnotation* annot = getAnnotationFromModel(xsModel, rootNode);
        XSModelGroup* modelGroup = 0;

        if (nodeType == ContentSpecNode::All)
        {
            modelGroup = new (fMemoryManager) XSModelGroup(XSModelGroup::COMPOSITOR_ALL, particleList, annot, xsModel, fMemoryManager);
            buildAllParticles(rootNode, particleList, xsModel);
        }
        else
        {
            if (nodeType == ContentSpecNode::ModelGroupChoice)
                modelGroup = new (fMemoryManager) XSModelGroup(XSModelGroup::COMPOSITOR_CHOICE, particleList, annot, xsModel, fMemoryManager);
            else
                modelGroup = new (fMemoryManager) XSModelGroup(XSModelGroup::COMPOSITOR_SEQUENCE, particleList, annot, xsModel, fMemoryManager);

            buildChoiceSequenceParticles(rootNode->getFirst(), particleList, xsModel);
            buildChoiceSequenceParticles(rootNode->getSecond(), particleList, xsModel);
        }

        XSParticle* groupParticle = new (fMemoryManager) XSParticle
        (
            XSParticle::TERM_MODELGROUP
            , xsModel
            , modelGroup
            , rootNode->getMinOccurs()
            , rootNode->getMaxOccurs()
            , fMemoryManager
        );
    
        return groupParticle;
    }
    else
        return 0;
}

void XSObjectFactory::buildAllParticles(const ContentSpecNode* const rootNode,
                                 XSParticleList* const particleList,
                                 XSModel* const xsModel)
{
    // Get the type of spec node our current node is
    const ContentSpecNode::NodeTypes nodeType = rootNode->getType();

    if (nodeType == ContentSpecNode::All)
    {
        const ContentSpecNode* rightNode = rootNode->getSecond();

        buildAllParticles(rootNode->getFirst(), particleList, xsModel);
        if (rightNode)
            buildAllParticles(rightNode, particleList, xsModel);
    }
    else if (nodeType == ContentSpecNode::Leaf)
    {
        XSParticle* elemParticle = createElementParticle(rootNode, xsModel);
        if (elemParticle)
            particleList->addElement(elemParticle);
    }
}

void XSObjectFactory::buildChoiceSequenceParticles(const ContentSpecNode* const rootNode,
                                            XSParticleList* const particleList,
                                            XSModel* const xsModel)
{
    if (rootNode)
    {
        const ContentSpecNode::NodeTypes nodeType = rootNode->getType();

        if (nodeType == ContentSpecNode::Sequence)
        {
            buildChoiceSequenceParticles(rootNode->getFirst(), particleList, xsModel);
            buildChoiceSequenceParticles(rootNode->getSecond(), particleList, xsModel);
        }
        else if (nodeType == ContentSpecNode::Choice)
        {
            buildChoiceSequenceParticles(rootNode->getFirst(), particleList, xsModel);
            buildChoiceSequenceParticles(rootNode->getSecond(), particleList, xsModel);
        }
        else if ((nodeType & 0x0f) == ContentSpecNode::Any
                 || (nodeType & 0x0f) == ContentSpecNode::Any_Other
                 || (nodeType & 0x0f) == ContentSpecNode::Any_NS
                 || nodeType == ContentSpecNode::Any_NS_Choice)
        {
            XSParticle* wildcardParticle = createWildcardParticle(rootNode, xsModel);
            if (wildcardParticle)
                particleList->addElement(wildcardParticle);
        }
        else if (nodeType == ContentSpecNode::Leaf)
        {
            XSParticle* elemParticle = createElementParticle(rootNode, xsModel);
            if (elemParticle)
                particleList->addElement(elemParticle);
        }
        // must be a model group
        else
        {
            XSParticle* xsParticle = createModelGroupParticle(rootNode, xsModel);
            if (xsParticle)
                particleList->addElement(xsParticle);
        }
    }
}

XSParticle*
XSObjectFactory::createElementParticle(const ContentSpecNode* const rootNode,
                                       XSModel* const xsModel)
{
    if (rootNode->getElementDecl())
    {
        XSElementDeclaration* xsElemDecl = addOrFind(
            (SchemaElementDecl*) rootNode->getElementDecl(), xsModel);

        if (xsElemDecl)
        {
            XSParticle* particle = new (fMemoryManager) XSParticle
            (
                XSParticle::TERM_ELEMENT
                , xsModel
                , xsElemDecl
                , rootNode->getMinOccurs()
                , rootNode->getMaxOccurs()
                , fMemoryManager
            );

            return particle;
        }
    }

    return 0;
}

XSParticle*
XSObjectFactory::createWildcardParticle(const ContentSpecNode* const rootNode,
                                        XSModel* const xsModel)
{
    XSWildcard* xsWildcard = createXSWildcard(rootNode, xsModel);
    if (xsWildcard)
    {
        XSParticle* particle = new (fMemoryManager) XSParticle
        (
            XSParticle::TERM_WILDCARD
            , xsModel
            , xsWildcard
            , rootNode->getMinOccurs()
            , rootNode->getMaxOccurs()
            , fMemoryManager
        );

        return particle;
    }

    return 0;
}

XSAttributeDeclaration* XSObjectFactory::addOrFind(SchemaAttDef* const attDef,
                                                   XSModel* const xsModel)
{
    XSAttributeDeclaration* xsObj = (XSAttributeDeclaration*) getObjectFromMap(attDef, xsModel);
    if (!xsObj)
    {
        XSSimpleTypeDefinition* xsType = 0;
        if (attDef->getDatatypeValidator())
            xsType = addOrFind(attDef->getDatatypeValidator(), xsModel);

        // need grammar to determine if scope is global:                   
                                               
        XSConstants::SCOPE scope = XSConstants::SCOPE_ABSENT;
        XSComplexTypeDefinition* enclosingCTDefinition = 0;
        XSNamespaceItem* namespaceItem = xsModel->getNamespaceItem(xsModel->getURIStringPool()->getValueForId(attDef->getAttName()->getURI()));
        if (namespaceItem)
        {
            // REVISIT: what if local name and global name are the same???
            if (namespaceItem->getSchemaGrammar()->getAttributeDeclRegistry()->get(attDef))
                scope = XSConstants::SCOPE_GLOBAL;
            else if (attDef->getEnclosingCT())
            {
                scope = XSConstants::SCOPE_LOCAL;
                enclosingCTDefinition = addOrFind(attDef->getEnclosingCT(), xsModel);
            }
        }        

        xsObj = new (fMemoryManager) XSAttributeDeclaration
        (
            attDef
            , xsType
            , getAnnotationFromModel(xsModel, attDef)
            , xsModel
            , scope
            , enclosingCTDefinition
            , fMemoryManager
        );
        putObjectInMap(attDef, xsObj, xsModel);
    }

    return xsObj;
}

XSSimpleTypeDefinition*
XSObjectFactory::addOrFind(DatatypeValidator* const validator,
                           XSModel* const xsModel)
{
    XSSimpleTypeDefinition* xsObj = (XSSimpleTypeDefinition*) getObjectFromMap(validator, xsModel);

    if (!xsObj)
    {
        //REVISIT
    }

    return xsObj;
}

XSElementDeclaration*
XSObjectFactory::addOrFind(SchemaElementDecl* const elemDecl,
                           XSModel* const xsModel)
{
    XSElementDeclaration* xsObj = (XSElementDeclaration*) getObjectFromMap(elemDecl, xsModel);
    if (!xsObj)
    {
        XSElementDeclaration*        xsSubElem = 0;
        XSTypeDefinition*            xsType = 0;
        XSNamedMap<XSIDCDefinition>* icMap = 0;

        if (elemDecl->getSubstitutionGroupElem())
            xsSubElem = addOrFind(elemDecl->getSubstitutionGroupElem(), xsModel);

        if (elemDecl->getComplexTypeInfo())
            xsType = addOrFind(elemDecl->getComplexTypeInfo(), xsModel);
        else if (elemDecl->getDatatypeValidator())
            xsType = addOrFind(elemDecl->getDatatypeValidator(), xsModel);

        unsigned int count = elemDecl->getIdentityConstraintCount();
        if (count)
        {
            //REVISIT: size of hash table....   
            icMap = new (fMemoryManager) XSNamedMap<XSIDCDefinition>
            (
                count
                , 29
                , xsModel->getURIStringPool()
                , false
                , fMemoryManager
            );

            for (unsigned int i = 0; i < count; i++) 
            {
                XSIDCDefinition* icDef = addOrFind
                (
                    elemDecl->getIdentityConstraintAt(i)
                    , xsModel
                );
                if (icDef)
                {
                    icMap->addElement
                    (
                        icDef
                        , icDef->getName()
                        , icDef->getNamespace()
                    );
                }
            }
        }

        xsObj = new (fMemoryManager) XSElementDeclaration
        (
            elemDecl
            , xsType
            , xsSubElem
            , getAnnotationFromModel(xsModel, elemDecl)
            , icMap
            , xsModel
            , fMemoryManager
        );
        putObjectInMap(elemDecl, xsObj, xsModel);
    }

    return xsObj;
}

XSComplexTypeDefinition*
XSObjectFactory::addOrFind(ComplexTypeInfo* const typeInfo,
                           XSModel* const xsModel)
{
    XSComplexTypeDefinition* xsObj = (XSComplexTypeDefinition*) getObjectFromMap(typeInfo, xsModel);
    if (!xsObj)
    {
        XSWildcard*             xsWildcard = 0;
        XSSimpleTypeDefinition* xsSimpleType = 0;
        XSAttributeUseList*     xsAttList = 0;
        XSTypeDefinition*       xsBaseType = 0;
        XSParticle*             xsParticle = 0;

        if (typeInfo->getAttWildCard())
            xsWildcard = createXSWildcard(typeInfo->getAttWildCard(), xsModel);

        if ((typeInfo->getContentType() == SchemaElementDecl::Simple) &&
            (typeInfo->getDatatypeValidator()))
            xsSimpleType = addOrFind(typeInfo->getDatatypeValidator(), xsModel);

        if (typeInfo->hasAttDefs())
        {
            SchemaAttDefList& attDefList = (SchemaAttDefList&) typeInfo->getAttDefList();
            unsigned int attCount = attDefList.getAttDefCount();
            xsAttList = new (fMemoryManager) RefVectorOf<XSAttributeUse>(attCount, false, fMemoryManager);
            
            for(unsigned int i=0; i<attCount; i++)
            {
                SchemaAttDef& attDef = (SchemaAttDef&) attDefList.getAttDef(i);
                XSAttributeDeclaration* xsAttDecl = addOrFind(&attDef, xsModel);
                XSAttributeUse* attUse = createXSAttributeUse(xsAttDecl, xsModel);
                xsAttList->addElement(attUse);
            }
        }

        // compute fBase
        if (typeInfo->getBaseComplexTypeInfo())
            xsBaseType = addOrFind(typeInfo->getBaseComplexTypeInfo(), xsModel);
        else if (typeInfo->getBaseDatatypeValidator())
            xsBaseType = addOrFind(typeInfo->getBaseDatatypeValidator(), xsModel);

        // compute particle
        ContentSpecNode* contentSpec = typeInfo->getContentSpec();
        if (contentSpec)
            xsParticle = createModelGroupParticle(contentSpec, xsModel);

        xsObj = new (fMemoryManager) XSComplexTypeDefinition
        (
            typeInfo
            , xsWildcard
            , xsSimpleType
            , xsAttList
            , xsBaseType
            , xsParticle
            , getAnnotationFromModel(xsModel, typeInfo)
            , xsModel
            , fMemoryManager
        );
        putObjectInMap(typeInfo, xsObj, xsModel);

        // REVISIT
        // process elements
    }

    return xsObj;
}

XSIDCDefinition* XSObjectFactory::addOrFind(IdentityConstraint* const ic,
                                            XSModel* const xsModel)
{
    XSIDCDefinition* xsObj = (XSIDCDefinition*) getObjectFromMap(ic, xsModel);
    if (!xsObj)
    {
        XSIDCDefinition* keyIC = 0;
        StringList*      stringList = 0;
        unsigned int     fieldCount = ic->getFieldCount();

        if (fieldCount) 
        {
            stringList = new (fMemoryManager) RefArrayVectorOf<XMLCh>(
                fieldCount, true, fMemoryManager);

            for(unsigned int i=0; i<fieldCount; i++)
            {
                XMLCh* expr = XMLString::replicate
                (
                    ic->getFieldAt(i)->getXPath()->getExpression()
                    , fMemoryManager
                );
                stringList->addElement(expr);
            }
        }

        if (ic->getType() == IdentityConstraint::KEYREF)
            keyIC = addOrFind(((IC_KeyRef*) ic)->getKey(), xsModel);

        xsObj= new (fMemoryManager) XSIDCDefinition
        (
            ic
            , keyIC
            , getAnnotationFromModel(xsModel, ic)
            , stringList
            , xsModel
            , fMemoryManager
        );
        putObjectInMap(ic, xsObj, xsModel);
    }

    return xsObj;
}

XSNotationDeclaration* XSObjectFactory::addOrFind(XMLNotationDecl* const notDecl,
                                                  XSModel* const xsModel)
{
    XSNotationDeclaration* xsObj = (XSNotationDeclaration*) getObjectFromMap(notDecl, xsModel);
    if (!xsObj)
    {
        xsObj = new (fMemoryManager) XSNotationDeclaration
        (
            notDecl
            , getAnnotationFromModel(xsModel, notDecl)
            , xsModel
            , fMemoryManager
        );
        putObjectInMap(notDecl, xsObj, xsModel);
    }

    return xsObj;
}

XSAttributeUse*
XSObjectFactory::createXSAttributeUse(XSAttributeDeclaration* const xsAttDecl,
                               XSModel* const xsModel)
{
    XSAttributeUse* attrUse = new (fMemoryManager) XSAttributeUse(xsAttDecl, xsModel, fMemoryManager);
    fDeleteVector->addElement(attrUse);

    return attrUse;
}

XSWildcard*
XSObjectFactory::createXSWildcard(SchemaAttDef* const attDef,
                                  XSModel* const xsModel)
{
    XSWildcard* xsWildcard = new (fMemoryManager) XSWildcard
    (
        attDef
        , getAnnotationFromModel(xsModel, attDef)
        , xsModel
        , fMemoryManager
    );
    fDeleteVector->addElement(xsWildcard);

    return xsWildcard;
}

XSWildcard*
XSObjectFactory::createXSWildcard(const ContentSpecNode* const rootNode,
                                  XSModel* const xsModel)
{
    XSWildcard* xsWildcard = new (fMemoryManager) XSWildcard
    (
        rootNode
        , getAnnotationFromModel(xsModel, rootNode)
        , xsModel
        , fMemoryManager
    );
    fDeleteVector->addElement(xsWildcard);

    return xsWildcard;
}

XSModelGroupDefinition*
XSObjectFactory::createXSModelGroupDefinition(XercesGroupInfo* const groupInfo,
                                              XSModel* const xsModel)
{
    XSParticle* particle = createModelGroupParticle(
            groupInfo->getContentSpec(), xsModel);

    XSModelGroupDefinition* xsObj = new (fMemoryManager) XSModelGroupDefinition
    (
        groupInfo
        , particle
        , getAnnotationFromModel(xsModel, groupInfo)
        , xsModel
        , fMemoryManager
    );

    return xsObj;
}


XSAttributeGroupDefinition*
XSObjectFactory::createXSAttGroupDefinition(XercesAttGroupInfo* const attGroupInfo,
                                            XSModel* const xsModel)
{
    XSAttributeUseList* xsAttList = 0;
    XSWildcard*         xsWildcard = 0;
    unsigned int        attCount = attGroupInfo->attributeCount();

    if (attCount)
    {
        xsAttList = new (fMemoryManager) RefVectorOf<XSAttributeUse>(attCount, false, fMemoryManager);
        for (unsigned int i=0; i < attCount; i++) 
        {
            XSAttributeDeclaration* xsAttDecl = addOrFind(attGroupInfo->attributeAt(i), xsModel);
            if (xsAttDecl) // just for sanity
            {
				XSAttributeUse* attUse = createXSAttributeUse(xsAttDecl, xsModel);
                xsAttList->addElement(attUse);
            }
        }
    }
    
    if (attGroupInfo->getCompleteWildCard())
        xsWildcard = createXSWildcard(attGroupInfo->getCompleteWildCard(), xsModel);

    XSAttributeGroupDefinition* xsObj = new (fMemoryManager) XSAttributeGroupDefinition
    (
        attGroupInfo
        , xsAttList
        , xsWildcard
        , getAnnotationFromModel(xsModel, attGroupInfo)
        , xsModel
        , fMemoryManager
    );

    return xsObj;
}

XSAnnotation* XSObjectFactory::getAnnotationFromModel(XSModel* const xsModel,
                                               const void* const key)
{
    XSNamespaceItemList* namespaceItemList = xsModel->getNamespaceItems();

    XSAnnotation* annot = 0;
    for (unsigned int i=0; i<namespaceItemList->size(); i++)
    {
        annot = namespaceItemList->elementAt(i)->getSchemaGrammar()->getAnnotation(key);
        if (annot)
            return annot;
    }

    if (xsModel->fParent)
        return getAnnotationFromModel(xsModel->fParent, key);
    return 0;
}


XSObject* XSObjectFactory::getObjectFromMap(void* key, XSModel* const xsModel)
{
    XSObject* xsObj = fXercesToXSMap->get(key);
    if (xsObj)
        return xsObj;
    if (xsModel->fParent)
        return getObjectFromMap(key, xsModel->fParent);
    return 0;
}

void XSObjectFactory::putObjectInMap(void* key, XSObject* const object, XSModel* const xsModel)
{
     fXercesToXSMap->put(key, object);
     fDeleteVector->addElement(object);
}


XERCES_CPP_NAMESPACE_END
