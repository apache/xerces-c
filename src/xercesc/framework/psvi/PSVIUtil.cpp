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


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/psvi/PSVIUtil.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/framework/psvi/XSParticle.hpp>
#include <xercesc/framework/psvi/XSModelGroup.hpp>
#include <xercesc/framework/psvi/XSElementDeclaration.hpp>
#include <xercesc/framework/psvi/XSComplexTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSWildcard.hpp>
#include <xercesc/framework/psvi/XSNamespaceItem.hpp>
#include <xercesc/framework/psvi/XSIDCDefinition.hpp>
#include <xercesc/framework/psvi/XSAttributeUse.hpp>
#include <xercesc/framework/psvi/XSAttributeDeclaration.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/datatype/DatatypeValidator.hpp>
#include <xercesc/validators/schema/SchemaAttDef.hpp>
#include <xercesc/validators/schema/SchemaGrammar.hpp>
#include <xercesc/validators/schema/identity/IdentityConstraint.hpp>

XERCES_CPP_NAMESPACE_BEGIN

XSParticle* PSVIUtil::createModelGroupParticle(const ContentSpecNode* const rootNode,
                                               XSModel* const xsModel,
                                               MemoryManager* const manager)
{
    ContentSpecNode::NodeTypes nodeType = rootNode->getType();

    if (nodeType == ContentSpecNode::All
        || nodeType == ContentSpecNode::ModelGroupChoice
        || nodeType == ContentSpecNode::ModelGroupSequence)
    {
        XSParticle* groupParticle = new (manager) XSParticle(XSParticle::TERM_MODELGROUP, xsModel, manager);
        XSParticleList* particleList = new (manager) RefVectorOf<XSParticle> (4, true, manager);
        XSAnnotation* annot = getAnnotationFromModel(xsModel, rootNode);
        XSModelGroup* modelGroup = 0;

        groupParticle->setMinOccurs(rootNode->getMinOccurs());
        groupParticle->setMaxOccurs(rootNode->getMaxOccurs());

        if (nodeType == ContentSpecNode::All)
        {
            modelGroup = new (manager) XSModelGroup(XSModelGroup::COMPOSITOR_ALL, particleList, annot, xsModel, manager);
            groupParticle->setTerm(modelGroup);
            buildAllParticles(rootNode, particleList, xsModel, manager);
        }
        else
        {
            if (nodeType == ContentSpecNode::ModelGroupChoice)
                modelGroup = new (manager) XSModelGroup(XSModelGroup::COMPOSITOR_CHOICE, particleList, annot, xsModel, manager);
            else
                modelGroup = new (manager) XSModelGroup(XSModelGroup::COMPOSITOR_SEQUENCE, particleList, annot, xsModel, manager);

            groupParticle->setTerm(modelGroup);
            buildChoiceSequenceParticles(rootNode->getFirst(), particleList, xsModel, manager);
            buildChoiceSequenceParticles(rootNode->getSecond(), particleList, xsModel, manager);
        }
    
        return groupParticle;
    }
    else
        return 0;
}

void PSVIUtil::buildAllParticles(const ContentSpecNode* const rootNode,
                                 XSParticleList* const particleList,
                                 XSModel* const xsModel,
                                 MemoryManager* const manager)
{
    // Get the type of spec node our current node is
    const ContentSpecNode::NodeTypes nodeType = rootNode->getType();

    if (nodeType == ContentSpecNode::All)
    {
        const ContentSpecNode* rightNode = rootNode->getSecond();

        buildAllParticles(rootNode->getFirst(), particleList, xsModel, manager);
        if (rightNode)
            buildAllParticles(rightNode, particleList, xsModel, manager);
    }
    else if (nodeType == ContentSpecNode::Leaf)
    {
        XSParticle* elemParticle = PSVIUtil::createElementParticle(rootNode, xsModel, manager);
        if (elemParticle)
            particleList->addElement(elemParticle);
    }
}

void PSVIUtil::buildChoiceSequenceParticles(const ContentSpecNode* const rootNode,
                                            XSParticleList* const particleList,
                                            XSModel* const xsModel,
                                            MemoryManager* const manager)
{
    if (rootNode)
    {
        const ContentSpecNode::NodeTypes nodeType = rootNode->getType();

        if (nodeType == ContentSpecNode::Sequence)
        {
            buildChoiceSequenceParticles(rootNode->getFirst(), particleList, xsModel, manager);
            buildChoiceSequenceParticles(rootNode->getSecond(), particleList, xsModel, manager);
        }
        else if (nodeType == ContentSpecNode::Choice)
        {
            buildChoiceSequenceParticles(rootNode->getFirst(), particleList, xsModel, manager);
            buildChoiceSequenceParticles(rootNode->getSecond(), particleList, xsModel, manager);
        }
        else if ((nodeType & 0x0f) == ContentSpecNode::Any
                 || (nodeType & 0x0f) == ContentSpecNode::Any_Other
                 || (nodeType & 0x0f) == ContentSpecNode::Any_NS
                 || nodeType == ContentSpecNode::Any_NS_Choice)
        {
            XSParticle* wildcardParticle = createWildcardParticle(rootNode, xsModel, manager);
            if (wildcardParticle)
                particleList->addElement(wildcardParticle);
        }
        else if (nodeType == ContentSpecNode::Leaf)
        {
            XSParticle* elemParticle = createElementParticle(rootNode, xsModel, manager);
            if (elemParticle)
                particleList->addElement(elemParticle);
        }
        // must be a model group
        else
        {
            XSParticle* xsParticle = createModelGroupParticle(rootNode, xsModel, manager);
            if (xsParticle)
                particleList->addElement(xsParticle);
        }
    }
}

XSParticle* PSVIUtil::createElementParticle(const ContentSpecNode* const rootNode,
                                            XSModel* const xsModel,
                                            MemoryManager* const manager)
{
    if (rootNode->getElementDecl())
    {
        XSElementDeclaration* xsElemDecl = addOrFind(
            (SchemaElementDecl*) rootNode->getElementDecl(), xsModel, manager
        );

        if (xsElemDecl)
        {
            XSParticle* particle = new (manager) XSParticle(XSParticle::TERM_ELEMENT, xsModel, manager);
            particle->setTerm(xsElemDecl);
            particle->setMaxOccurs(rootNode->getMaxOccurs());
            particle->setMinOccurs(rootNode->getMinOccurs());
        }
    }

    return 0;
}

XSParticle* PSVIUtil::createWildcardParticle(const ContentSpecNode* const rootNode,
                                             XSModel* const xsModel,
                                             MemoryManager* const manager)
{
    XSWildcard* xsWildcard = createXSWildcard(rootNode, xsModel, manager);
    if (xsWildcard)
    {
        XSParticle* particle = new (manager) XSParticle(XSParticle::TERM_WILDCARD, xsModel, manager);
        particle->setTerm(xsWildcard);
        particle->setMaxOccurs(rootNode->getMaxOccurs());
        particle->setMinOccurs(rootNode->getMinOccurs());
    }

    return 0;
}

XSAttributeDeclaration* PSVIUtil::addOrFind(SchemaAttDef* const attDef,
                                            XSModel* const xsModel,
                                            MemoryManager* const manager)
{
    XSAttributeDeclaration* xsAtt = (XSAttributeDeclaration*) getObjectFromMap(attDef, xsModel);
    if (!xsAtt)
    {
        xsAtt = new (manager) XSAttributeDeclaration(attDef, xsModel, manager);
        putObjectInMap(attDef, xsAtt, xsModel);
    }

    return xsAtt;
}

XSSimpleTypeDefinition* PSVIUtil::addOrFind(DatatypeValidator* const validator,
                                            XSModel* const xsModel,
                                            MemoryManager* const manager)
{
    XSSimpleTypeDefinition* xsST = (XSSimpleTypeDefinition*) getObjectFromMap(validator, xsModel);
    if (!xsST)
    {
        xsST = new (manager) XSSimpleTypeDefinition(validator, xsModel, manager);
        putObjectInMap(validator, xsST, xsModel);
        xsST->construct();
    }

    return xsST;
}

XSElementDeclaration* PSVIUtil::addOrFind(SchemaElementDecl* const elemDecl,
                                          XSModel* const xsModel,
                                          MemoryManager* const manager)
{
    XSElementDeclaration* xsElemDecl = (XSElementDeclaration*) getObjectFromMap(elemDecl, xsModel);
    if (!xsElemDecl)
    {
        xsElemDecl = new (manager) XSElementDeclaration(elemDecl, xsModel, manager);
        putObjectInMap(elemDecl, xsElemDecl, xsModel);
        xsElemDecl->construct();
    }

    return xsElemDecl;
}

XSComplexTypeDefinition* PSVIUtil::addOrFind(ComplexTypeInfo* const typeInfo,
                                             XSModel* const xsModel,
                                             MemoryManager* const manager)
{
    XSComplexTypeDefinition* xsCT = (XSComplexTypeDefinition*) getObjectFromMap(typeInfo, xsModel);
    if (!xsCT)
    {
        xsCT = new (manager) XSComplexTypeDefinition(typeInfo, xsModel, manager);
        putObjectInMap(typeInfo, xsCT, xsModel);
        xsCT->construct();
    }

    return xsCT;
}

XSIDCDefinition* PSVIUtil::addOrFind(IdentityConstraint* const ic,
                                     XSModel* const xsModel,
                                     MemoryManager* const manager)
{
    XSIDCDefinition* xsIC = (XSIDCDefinition*) getObjectFromMap(ic, xsModel);
    if (!xsIC)
    {
        xsIC = new (manager) XSIDCDefinition(ic, xsModel, manager);
        putObjectInMap(ic, xsIC, xsModel);
    }

    return xsIC;
}

XSAttributeUse*
PSVIUtil::createXSAttributeUse(XSAttributeDeclaration* const xsAttDecl,
                               XSModel* const xsModel,
                               MemoryManager* const manager)
{
    XSAttributeUse* attrUse = new (manager) XSAttributeUse(xsAttDecl, xsModel, manager);
    xsModel->fDeleteVector->addElement(attrUse);

    return attrUse;
}

XSWildcard*
PSVIUtil::createXSWildcard(SchemaAttDef* const attDef,
                           XSModel* const xsModel,
                           MemoryManager* const manager)
{
    XSWildcard* xsWildcard = new (manager) XSWildcard(attDef, xsModel, manager);
    xsModel->fDeleteVector->addElement(xsWildcard);

    return xsWildcard;
}

XSWildcard* PSVIUtil::createXSWildcard(const ContentSpecNode* const rootNode,
                                       XSModel* const xsModel,
                                       MemoryManager* const manager)
{
    XSWildcard* xsWildcard = new (manager) XSWildcard(rootNode, xsModel, manager);
    xsModel->fDeleteVector->addElement(xsWildcard);

    return xsWildcard;
}

XSAnnotation* PSVIUtil::getAnnotationFromModel(XSModel* const xsModel,
                                               const void* const key)
{
    XSNamespaceItemList* namespaceItemList = xsModel->getNamespaceItems();

    XSAnnotation* annot = 0;
    for (unsigned int i=0; i<namespaceItemList->size(); i++)
    {
        annot = namespaceItemList->elementAt(i)->getSchemaGrammar()->getAnnotation(key);
        if (annot)
            break;
    }

    return annot;
}


XSObject* PSVIUtil::getObjectFromMap(void* key, XSModel* const xsModel)
{
    return xsModel->fXercesToXSMap->get(key);
}

void PSVIUtil::putObjectInMap(void* key, XSObject* const object, XSModel* const xsModel)
{
     xsModel->fXercesToXSMap->put(key, object);
     xsModel->fDeleteVector->addElement(object);
}

XSNamespaceItem* PSVIUtil::getNamespaceItem(const XMLCh* const namespaceKey,
                                            XSModel* const xsModel)
{
    return xsModel->fHashNamespace->get(namespaceKey);
}

XERCES_CPP_NAMESPACE_END
