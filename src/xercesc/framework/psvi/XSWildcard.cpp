/*
 * Copyright 2003,2004 The Apache Software Foundation.
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
 * Revision 1.6  2004/09/08 13:56:09  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/11/21 17:34:04  knoaman
 * PSVI update
 *
 * Revision 1.4  2003/11/14 22:47:53  neilg
 * fix bogus log message from previous commit...
 *
 * Revision 1.3  2003/11/14 22:33:30  neilg
 * Second phase of schema component model implementation.  
 * Implement XSModel, XSNamespaceItem, and the plumbing necessary
 * to connect them to the other components.
 * Thanks to David Cargill.
 *
 * Revision 1.2  2003/11/06 15:30:04  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSWildcard.hpp>
#include <xercesc/framework/psvi/XSModel.hpp>
#include <xercesc/util/ValueVectorOf.hpp>
#include <xercesc/util/StringPool.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/schema/SchemaAttDef.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSWildcard: Constructors and Destructor
// ---------------------------------------------------------------------------
XSWildcard::XSWildcard(SchemaAttDef* const  attWildCard,
                       XSAnnotation* const  annot,
                       XSModel* const       xsModel,
                       MemoryManager* const manager)
    : XSObject(XSConstants::WILDCARD, xsModel, manager)
    , fConstraintType(NSCONSTRAINT_ANY)
    , fProcessContents(PC_STRICT)
    , fNsConstraintList(0)
    , fAnnotation(annot)
{
    XMLAttDef::AttTypes attType = attWildCard->getType();
    if (attType == XMLAttDef::Any_Other)
    {
        fConstraintType = NSCONSTRAINT_NOT;
        fNsConstraintList = new (manager) RefArrayVectorOf<XMLCh>(1, true, manager);
        fNsConstraintList->addElement
        (
            XMLString::replicate(fXSModel->getURIStringPool()->getValueForId(
                    attWildCard->getAttName()->getURI()), manager)
        );
    }
    else if (attType == XMLAttDef::Any_List)
    {
        fConstraintType = NSCONSTRAINT_DERIVATION_LIST;
        ValueVectorOf<unsigned int>* nsList = attWildCard->getNamespaceList();
        if (nsList)
        {
            unsigned int nsListSize = nsList->size();
            if (nsListSize)
            {
                fNsConstraintList = new (manager) RefArrayVectorOf<XMLCh>(nsListSize, true, manager);
                for (unsigned int i=0; i < nsListSize; i++)
                {
                    fNsConstraintList->addElement
                    (
                        XMLString::replicate
                        (
                            fXSModel->getURIStringPool()->getValueForId
                            (
                                nsList->elementAt(i)
                            )
                            , manager
                        )
                    );
                }
            }
        }
    }

    XMLAttDef::DefAttTypes attDefType = attWildCard->getDefaultType();
    if (attDefType == XMLAttDef::ProcessContents_Skip)
        fProcessContents = PC_SKIP;
    else if (attDefType == XMLAttDef::ProcessContents_Lax)
        fProcessContents = PC_LAX;
}

XSWildcard::XSWildcard(const ContentSpecNode* const elmWildCard,
                       XSAnnotation* const annot,
                       XSModel* const xsModel,
                       MemoryManager* const manager)
    : XSObject(XSConstants::WILDCARD, xsModel, manager)
    , fConstraintType(NSCONSTRAINT_ANY)
    , fProcessContents(PC_STRICT)
    , fNsConstraintList(0)
    , fAnnotation(annot)
{
    ContentSpecNode::NodeTypes nodeType = elmWildCard->getType();
    if ((nodeType & 0x0f) == ContentSpecNode::Any_Other)
    {
        fConstraintType = NSCONSTRAINT_NOT;
        if (nodeType == ContentSpecNode::Any_Other_Lax)
            fProcessContents = PC_LAX;
        else if (nodeType == ContentSpecNode::Any_Other_Skip)
            fProcessContents = PC_SKIP;
    }
    else if ((nodeType & 0x0f) == ContentSpecNode::Any_NS)
    {
        fConstraintType = NSCONSTRAINT_DERIVATION_LIST;
        if (nodeType == ContentSpecNode::Any_NS_Lax)
            fProcessContents = PC_LAX;
        else if (nodeType == ContentSpecNode::Any_NS_Skip)
            fProcessContents = PC_SKIP;
    }
    else if (nodeType == ContentSpecNode::Any_NS_Choice)
    {
        fConstraintType = NSCONSTRAINT_DERIVATION_LIST;
        ContentSpecNode::NodeTypes anyType = elmWildCard->getFirst()->getType();

        if (anyType == ContentSpecNode::Any_NS_Lax)
            fProcessContents = PC_LAX;
        else if (anyType == ContentSpecNode::Any_NS_Skip)
            fProcessContents = PC_SKIP;

        fNsConstraintList = new (manager) RefArrayVectorOf<XMLCh>(4, true, manager);
        buildNamespaceList(elmWildCard);
    }
    // must be any
    else
    {
        if (nodeType == ContentSpecNode::Any_Lax)
            fProcessContents = PC_LAX;
        else if (nodeType == ContentSpecNode::Any_Skip)
            fProcessContents = PC_SKIP;
    }

    if (fConstraintType == NSCONSTRAINT_NOT
        || (fConstraintType == NSCONSTRAINT_DERIVATION_LIST
            && !fNsConstraintList))
    {
        fNsConstraintList = new (manager) RefArrayVectorOf<XMLCh>(1, true, manager);
        fNsConstraintList->addElement
        (
             XMLString::replicate(fXSModel->getURIStringPool()->getValueForId(
                     elmWildCard->getElement()->getURI()), manager)
        );
    }
}

XSWildcard::~XSWildcard() 
{
    if (fNsConstraintList) 
        delete fNsConstraintList;
}

// ---------------------------------------------------------------------------
//  XSWildcard: helper methods
// ---------------------------------------------------------------------------
void XSWildcard::buildNamespaceList(const ContentSpecNode* const rootNode)
{
    ContentSpecNode::NodeTypes nodeType = rootNode->getType();
    if (nodeType == ContentSpecNode::Any_NS_Choice)
    {
        buildNamespaceList(rootNode->getFirst());
        buildNamespaceList(rootNode->getSecond());
    }
    else
    {
        fNsConstraintList->addElement
        (
            XMLString::replicate(fXSModel->getURIStringPool()->getValueForId(
                    rootNode->getElement()->getURI()), fMemoryManager)
        );
    }
}

XERCES_CPP_NAMESPACE_END
