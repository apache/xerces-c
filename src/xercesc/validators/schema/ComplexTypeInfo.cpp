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
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.22  2003/12/17 00:18:40  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.21  2003/12/16 17:18:50  peiyongz
 * don't expand ContextSpecNode when deserilized
 *
 * Revision 1.20  2003/11/20 18:05:16  knoaman
 * PSVI: Use a copy of the content spec node when creating the content model.
 *
 * Revision 1.19  2003/11/13 23:20:47  peiyongz
 * initSize
 *
 * Revision 1.18  2003/11/10 21:54:51  neilg
 * implementation for new stateless means of traversing attribute definition lists
 *
 * Revision 1.17  2003/11/07 17:08:12  knoaman
 * For PSVI support, distinguish wildcard elements with namespace lists.
 *
 * Revision 1.16  2003/10/29 16:22:58  peiyongz
 * allow proper recreation of ContentModel from deserialization
 *
 * Revision 1.15  2003/10/21 10:23:53  amassari
 * When creating a new list of attributes, use the assigned memory manager
 *
 * Revision 1.14  2003/10/17 21:17:12  peiyongz
 * using XTemplateSerializer
 *
 * Revision 1.13  2003/10/14 15:22:28  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.12  2003/10/10 16:25:40  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.11  2003/10/08 21:33:48  peiyongz
 * Synchronize ContentSpec/ContentModel/FormattedModel
 *
 * Revision 1.10  2003/09/26 18:31:05  peiyongz
 * Synchronize ContentSpecNode and formattedModel
 *
 * Revision 1.9  2003/05/18 14:02:07  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.8  2003/05/16 21:43:21  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.7  2003/05/15 18:57:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.6  2003/01/29 19:57:30  gareth
 * API to retrive local and uri part of the type name
 *
 * Revision 1.5  2002/12/19 14:04:05  gareth
 * get/set methods to see if the represented type is anonymous. Patch by Peter Volchek.
 *
 * Revision 1.4  2002/11/04 14:49:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/03/21 15:34:40  knoaman
 * Add support for reporting line/column numbers of schema errors.
 *
 * Revision 1.2  2002/02/06 22:29:24  knoaman
 * Remove unnecessary data/methods.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:45  peiyongz
 * sane_include
 *
 * Revision 1.27  2002/01/15 19:09:16  knoaman
 * Fix for bug 5807.
 *
 * Revision 1.26  2002/01/02 15:20:22  tng
 * Schema Fix: should not store a temp value as the key in the element pool and the attribute pool.
 *
 * Revision 1.25  2001/12/17 21:38:59  tng
 * Fix dangling pointer.
 *
 * Revision 1.24  2001/11/21 14:30:13  knoaman
 * Fix for UPA checking.
 *
 * Revision 1.23  2001/11/15 17:10:19  knoaman
 * Particle derivation checking support.
 *
 * Revision 1.22  2001/11/07 21:50:28  tng
 * Fix comment log that lead to error.
 *
 * Revision 1.21  2001/11/07 21:12:15  tng
 * Performance: Create QName in ContentSpecNode only if it is a leaf/Any/PCDataNode.
 *
 * Revision 1.20  2001/10/12 18:08:57  tng
 * make sure the second node exists before calling getType
 *
 * Revision 1.19  2001/10/04 15:08:56  knoaman
 * Add support for circular import.
 *
 * Revision 1.18  2001/09/05 20:49:11  knoaman
 * Fix for complexTypes with mixed content model.
 *
 * Revision 1.17  2001/08/29 21:27:07  knoaman
 * no message
 *
 * Revision 1.16  2001/08/28 20:43:52  knoaman
 * Fix for content spec node adoption.
 *
 * Revision 1.15  2001/08/27 23:04:02  knoaman
 * Handle deletion of spec node tree created during UPA checking.
 *
 * Revision 1.14  2001/08/27 20:48:45  knoaman
 * Make the UPA rename before the content model expansion.
 *
 * Revision 1.13  2001/08/27 20:14:42  knoaman
 * Validate attributes in <all>, <redefine>, <group> and <attributeGroup> declarations.
 * Misc. fixes.
 *
 * Revision 1.12  2001/08/24 12:48:48  tng
 * Schema: AllContentModel
 *
 * Revision 1.11  2001/08/23 11:54:26  tng
 * Add newline at the end and various typo fixes.
 *
 * Revision 1.10  2001/08/21 18:14:55  knoaman
 * Defer creation of spec node.
 *
 * Revision 1.9  2001/08/21 16:06:11  tng
 * Schema: Unique Particle Attribution Constraint Checking.
 *
 * Revision 1.8  2001/08/09 15:23:16  knoaman
 * add support for <anyAttribute> declaration.
 *
 * Revision 1.7  2001/07/24 18:33:46  knoaman
 * Added support for <group> + extra constraint checking for complexType
 *
 * Revision 1.6  2001/06/05 13:59:53  knoaman
 * Fixes to include and import.
 *
 * Revision 1.5  2001/05/11 13:27:32  tng
 * Copyright update.
 *
 * Revision 1.4  2001/05/10 17:49:40  tng
 * Schema: SchemaValidator fixes
 *
 * Revision 1.3  2001/05/10 16:33:12  knoaman
 * Traverse Schema Part III + error messages.
 *
 * Revision 1.2  2001/05/03 20:34:41  tng
 * Schema: SchemaValidator update
 *
 * Revision 1.1  2001/04/19 17:43:14  knoaman
 * More schema implementation classes.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/XMLBuffer.hpp>
#include <xercesc/validators/schema/ComplexTypeInfo.hpp>
#include <xercesc/validators/schema/SchemaAttDefList.hpp>
#include <xercesc/validators/common/AllContentModel.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/common/DFAContentModel.hpp>
#include <xercesc/validators/common/MixedContentModel.hpp>
#include <xercesc/validators/common/SimpleContentModel.hpp>
#include <xercesc/validators/schema/XSDLocator.hpp>

#include <xercesc/internal/XTemplateSerializer.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  ComplexTypeInfo: Constructors and Destructor
// ---------------------------------------------------------------------------
ComplexTypeInfo::ComplexTypeInfo(MemoryManager* const manager)
    : fAnonymous(false)
    , fAbstract(false)
    , fAdoptContentSpec(true)
    , fAttWithTypeId(false)
    , fPreprocessed(false)
    , fDerivedBy(0)
    , fBlockSet(0)
    , fFinalSet(0)
    , fScopeDefined(Grammar::TOP_LEVEL_SCOPE)
    , fElementId(XMLElementDecl::fgInvalidElemId)
    , fContentType(SchemaElementDecl::Empty)
    , fTypeName(0)
    , fTypeLocalName(0)
    , fTypeUri(0)
    , fBaseDatatypeValidator(0)
    , fDatatypeValidator(0)
    , fBaseComplexTypeInfo(0)
    , fContentSpec(0)
    , fAttWildCard(0)
    , fAttDefs(0)
    , fAttList(0)
    , fElements(0)
    , fContentModel(0)
    , fFormattedModel(0)
    , fContentSpecOrgURI(0)
    , fUniqueURI(0)
    , fContentSpecOrgURISize(16)
    , fSpecNodesToDelete(0)
    , fLocator(0)
    , fMemoryManager(manager)
{

}


ComplexTypeInfo::~ComplexTypeInfo()
{
    fMemoryManager->deallocate(fTypeName); //delete [] fTypeName;
    fMemoryManager->deallocate(fTypeLocalName); //delete [] fTypeLocalName;
    fMemoryManager->deallocate(fTypeUri); //delete [] fTypeUri;

    if (fAdoptContentSpec) {
        delete fContentSpec;
    }

    delete fAttWildCard;
    delete fAttDefs;
    delete fAttList;
    delete fElements;
    delete fSpecNodesToDelete;
    delete fLocator;

    delete fContentModel;
    fMemoryManager->deallocate(fFormattedModel); //delete [] fFormattedModel;
    fMemoryManager->deallocate(fContentSpecOrgURI); //delete [] fContentSpecOrgURI;

}

// ---------------------------------------------------------------------------
//  ComplexTypeInfo: Setter methods
// ---------------------------------------------------------------------------
void ComplexTypeInfo::addAttDef(SchemaAttDef* const toAdd) {

    // Fault in the att list if required
    if (!fAttDefs)
        faultInAttDefList();

    // Tell this guy the element id of its parent (us)
    toAdd->setElemId(getElementId());

    fAttDefs->put((void*)(toAdd->getAttName()->getLocalPart()),
                          toAdd->getAttName()->getURI(), toAdd);
    // update and/or create fAttList
    if(!fAttList)
        ((ComplexTypeInfo*)this)->fAttList = new (fMemoryManager) SchemaAttDefList(fAttDefs,fMemoryManager);
    fAttList->addAttDef(toAdd);
}

void ComplexTypeInfo::setContentSpec(ContentSpecNode* const toAdopt) {

    if (fContentSpec && fAdoptContentSpec) {
        delete fContentSpec;
    }

    fContentSpec = toAdopt;

    //reset Content Model
    setContentModel(0);
}

void ComplexTypeInfo::setLocator(XSDLocator* const aLocator) {

    if (fLocator)
        delete fLocator;

    fLocator = aLocator;
}

// ---------------------------------------------------------------------------
//  ComplexTypeInfo: Getter methods
// ---------------------------------------------------------------------------
XMLAttDefList& ComplexTypeInfo::getAttDefList() const
{
    if (!fAttList)
    {
        // If the att def list is not made yet, then fault it in too
        if (!fAttDefs)
            faultInAttDefList();

        ((ComplexTypeInfo*)this)->fAttList = new (fMemoryManager) SchemaAttDefList(fAttDefs, fMemoryManager);
    }

    // Reset it before we return it
    fAttList->Reset();
    return *fAttList;
}

const XMLCh*
ComplexTypeInfo::getFormattedContentModel() const
{
    //
    //  If its not already built, then call the protected virtual method
    //  to allow the derived class to build it (since only it knows.)
    //  Otherwise, just return the previously formatted methods.
    //
    //  Since we are faulting this in, within a const getter, we have to
    //  cast off the const-ness.
    //
    if (!fFormattedModel)
        ((ComplexTypeInfo*)this)->fFormattedModel = formatContentModel();

    return fFormattedModel;
}

// ---------------------------------------------------------------------------
//  ComplexTypeInfo: Helper methods
// ---------------------------------------------------------------------------
XMLAttDef* ComplexTypeInfo::findAttr(const XMLCh* const qName
                                     , const unsigned int uriId
                                     , const XMLCh* const baseName
                                     , const XMLCh* const prefix
                                     , const XMLElementDecl::LookupOpts   options
                                     , bool&              wasAdded) const
{
    SchemaAttDef* retVal = 0;

    // If no att list faulted in yet, then it cannot exist
    if (fAttDefs)
        retVal = fAttDefs->get(baseName, uriId);

    // Fault it in if not found and ask to add it
    if (!retVal && (options == XMLElementDecl::AddIfNotFound))
    {
        // Fault in the list itself if not already
        if (!fAttDefs)
            faultInAttDefList();

        // And add a default attribute for this name
        retVal = new (fMemoryManager) SchemaAttDef
        (
            prefix
            , baseName
            , uriId
            , XMLAttDef::CData
            , XMLAttDef::Implied
            , fMemoryManager
        );
        retVal->setElemId(getElementId());
        fAttDefs->put((void*)retVal->getAttName()->getLocalPart(), uriId, retVal);

        // update and/or create fAttList
        if(!fAttList)
            ((ComplexTypeInfo*)this)->fAttList = new (fMemoryManager) SchemaAttDefList(fAttDefs,fMemoryManager);
        fAttList->addAttDef(retVal);
        wasAdded = true;
    }
    else
    {
        wasAdded = false;
    }
    return retVal;
}

bool ComplexTypeInfo::resetDefs() {

    // If the collection hasn't been faulted in, then no att defs
    if (!fAttDefs)
        return false;

    //
    //  Ok, run through them and clear the 'provided' flag on each of them.
    //  This lets the scanner use them to track which has been provided and
    //  which have not.
    //
    RefHash2KeysTableOfEnumerator<SchemaAttDef> enumDefs(fAttDefs, false, fMemoryManager);
    while (enumDefs.hasMoreElements())
        enumDefs.nextElement().setProvided(false);

    return true;
}


void ComplexTypeInfo::checkUniqueParticleAttribution (SchemaGrammar*    const pGrammar,
                                                      GrammarResolver*  const pGrammarResolver,
                                                      XMLStringPool*    const pStringPool,
                                                      XMLValidator*     const pValidator)
{
    if (fContentSpec) {
        ContentSpecNode* specNode = new (fMemoryManager) ContentSpecNode(*fContentSpec);
        XMLContentModel* cm = makeContentModel(true, specNode);

        if (cm) {
            cm->checkUniqueParticleAttribution(pGrammar, pGrammarResolver, pStringPool, pValidator, fContentSpecOrgURI);
            delete cm;
        }

        fSpecNodesToDelete->removeAllElements();
    }
}

// ---------------------------------------------------------------------------
//  ComplexTypeInfo: Private Helper methods
// ---------------------------------------------------------------------------
void ComplexTypeInfo::faultInAttDefList() const
{
    // Use a hash modulus of 29 and tell it owns its elements
    ((ComplexTypeInfo*)this)->fAttDefs =
        new (fMemoryManager) RefHash2KeysTableOf<SchemaAttDef>(29, true, fMemoryManager);
}

XMLCh* ComplexTypeInfo::formatContentModel() const
{
    XMLCh* newValue = 0;
    if (fContentType == SchemaElementDecl::Any)
    {
        newValue = XMLString::replicate(XMLUni::fgAnyString, fMemoryManager);
    }
    else if (fContentType == SchemaElementDecl::Empty)
    {
        newValue = XMLString::replicate(XMLUni::fgEmptyString, fMemoryManager);
    }
    else
    {
        //
        //  Use a temp XML buffer to format into. Content models could be
        //  pretty long, but very few will be longer than one K. The buffer
        //  will expand to handle the more pathological ones.
        //
        const ContentSpecNode* specNode = fContentSpec;

        if (specNode) {
            XMLBuffer bufFmt(1023, fMemoryManager);

            specNode->formatSpec(bufFmt);
            newValue = XMLString::replicate
            (
                bufFmt.getRawBuffer()
                , fMemoryManager
            );
        }
    }
    return newValue;
}

XMLContentModel* ComplexTypeInfo::makeContentModel(const bool checkUPA, ContentSpecNode* const specNode)
{
    if ((specNode || fContentSpec) && !fSpecNodesToDelete) {
        fSpecNodesToDelete = new (fMemoryManager) RefVectorOf<ContentSpecNode>(8, true, fMemoryManager);
    }

    // expand the content spec first
    ContentSpecNode* aSpecNode = specNode;
    XMLContentModel* retModel = 0;
    if (aSpecNode) {

        fContentSpecOrgURI = (unsigned int*) fMemoryManager->allocate
        (
            fContentSpecOrgURISize * sizeof(unsigned int)
        ); //new unsigned int[fContentSpecOrgURISize];
        aSpecNode = convertContentSpecTree(aSpecNode, checkUPA);
        retModel = buildContentModel(aSpecNode);
        fSpecNodesToDelete->addElement(aSpecNode);
    }
    else {
        // building content model for the complex type
        aSpecNode = new (fMemoryManager) ContentSpecNode(*fContentSpec);
        aSpecNode = convertContentSpecTree(aSpecNode, checkUPA);
        retModel = buildContentModel(aSpecNode);
        delete aSpecNode;
    }

    return retModel;
}

XMLContentModel* ComplexTypeInfo::buildContentModel(ContentSpecNode* const aSpecNode)
{
    XMLContentModel* cmRet = 0;
    if (fContentType == SchemaElementDecl::Simple) {
       // just return nothing
    }
    else if (fContentType == SchemaElementDecl::Mixed_Simple)
    {
        //
        //  Just create a mixel content model object. This type of
        //  content model is optimized for mixed content validation.
        //
        cmRet = new (fMemoryManager) MixedContentModel(false, aSpecNode, false, fMemoryManager);
    }
    else if (fContentType == SchemaElementDecl::Mixed_Complex) {

            cmRet = createChildModel(aSpecNode, true);
    }
    else if (fContentType == SchemaElementDecl::Children)
    {
        //
        //  This method will create an optimal model for the complexity
        //  of the element's defined model. If its simple, it will create
        //  a SimpleContentModel object. If its a simple list, it will
        //  create a SimpleListContentModel object. If its complex, it
        //  will create a DFAContentModel object.
        //
         cmRet = createChildModel(aSpecNode, false);
    }
     else
    {
        ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::CM_MustBeMixedOrChildren, fMemoryManager);
    }

    return cmRet;
}

// ---------------------------------------------------------------------------
//  SchemaElementDecl: Private helper methods
// ---------------------------------------------------------------------------
XMLContentModel* ComplexTypeInfo::createChildModel(ContentSpecNode* specNode, const bool isMixed)
{
    if(!specNode)
        ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::CM_UnknownCMSpecType, fMemoryManager);

    ContentSpecNode::NodeTypes specType = specNode->getType();
    //
    //  Do a sanity check that the node is does not have a PCDATA id. Since,
    //  if it was, it should have already gotten taken by the Mixed model.
    //
    if (specNode->getElement()) {
        if (specNode->getElement()->getURI() == XMLElementDecl::fgPCDataElemId)
            ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::CM_NoPCDATAHere, fMemoryManager);
    }

    //
    //  According to the type of node, we will create the correct type of
    //  content model.
    //
    if (((specType & 0x0f) == ContentSpecNode::Any) ||
       ((specType & 0x0f) == ContentSpecNode::Any_Other) ||
       ((specType & 0x0f) == ContentSpecNode::Any_NS)) {
       // let fall through to build a DFAContentModel
    }
    else if (isMixed)
    {
        if (specType == ContentSpecNode::All) {
            // All the nodes under an ALL must be additional ALL nodes and
            // ELEMENTs (or ELEMENTs under ZERO_OR_ONE nodes.)
            // We collapse the ELEMENTs into a single vector.
            return new (fMemoryManager) AllContentModel(specNode, true, fMemoryManager);
        }
        else if (specType == ContentSpecNode::ZeroOrOne) {
            // An ALL node can appear under a ZERO_OR_ONE node.
            if (specNode->getFirst()->getType() == ContentSpecNode::All) {
                return new (fMemoryManager) AllContentModel(specNode->getFirst(), true, fMemoryManager);
            }
        }

        // otherwise, let fall through to build a DFAContentModel
    }
     else if (specType == ContentSpecNode::Leaf)
    {
        // Create a simple content model
        return new (fMemoryManager) SimpleContentModel
        (
            false
            , specNode->getElement()
            , 0
            , ContentSpecNode::Leaf
            , fMemoryManager
        );
    }
     else if (((specType & 0x0f) == ContentSpecNode::Choice)
          ||  ((specType & 0x0f) == ContentSpecNode::Sequence))
    {
        //
        //  Lets see if both of the children are leafs. If so, then it has to
        //  be a simple content model
        //
        if ((specNode->getFirst()->getType() == ContentSpecNode::Leaf)
        &&  (specNode->getSecond())
        &&  (specNode->getSecond()->getType() == ContentSpecNode::Leaf))
        {
            return new (fMemoryManager) SimpleContentModel
            (
                false
                , specNode->getFirst()->getElement()
                , specNode->getSecond()->getElement()
                , specType
                , fMemoryManager
            );
        }
    }
     else if ((specType == ContentSpecNode::OneOrMore)
          ||  (specType == ContentSpecNode::ZeroOrMore)
          ||  (specType == ContentSpecNode::ZeroOrOne))
    {
        //
        //  Its a repetition, so see if its one child is a leaf. If so its a
        //  repetition of a single element, so we can do a simple content
        //  model for that.
        //
        if (specNode->getFirst()->getType() == ContentSpecNode::Leaf)
        {
            return new (fMemoryManager) SimpleContentModel
            (
                false
                , specNode->getFirst()->getElement()
                , 0
                , specType
                , fMemoryManager
            );
        }
        else if (specNode->getFirst()->getType() == ContentSpecNode::All)
            return new (fMemoryManager) AllContentModel(specNode->getFirst(), false, fMemoryManager);

    }
    else if (specType == ContentSpecNode::All)
        return new (fMemoryManager) AllContentModel(specNode, false, fMemoryManager);

    else
    {
        ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::CM_UnknownCMSpecType, fMemoryManager);
    }

    // Its not any simple type of content, so create a DFA based content model
    return new (fMemoryManager) DFAContentModel(false, specNode, isMixed, fMemoryManager);
}

ContentSpecNode*
ComplexTypeInfo::convertContentSpecTree(ContentSpecNode* const curNode,
                                        const bool checkUPA) {

    if (!curNode)
        return 0;

    const ContentSpecNode::NodeTypes curType = curNode->getType();

    // When checking Unique Particle Attribution, rename leaf elements
    if (checkUPA) {
        if (curNode->getElement()) {
            fContentSpecOrgURI[fUniqueURI] = curNode->getElement()->getURI();
            curNode->getElement()->setURI(fUniqueURI);
            fUniqueURI++;
        }
        if (fUniqueURI == fContentSpecOrgURISize)
            resizeContentSpecOrgURI();
    }

    // Get the spec type of the passed node
    int minOccurs = curNode->getMinOccurs();
    int maxOccurs = curNode->getMaxOccurs();
    ContentSpecNode* retNode = curNode;

    if ((curType & 0x0f) == ContentSpecNode::Any
        || (curType & 0x0f) == ContentSpecNode::Any_Other
        || (curType & 0x0f) == ContentSpecNode::Any_NS
        || curType == ContentSpecNode::Leaf)
    {
        retNode =  expandContentModel(curNode, minOccurs, maxOccurs);
    }
    else if (((curType & 0x0f) == ContentSpecNode::Choice)
        ||   (curType == ContentSpecNode::All)
        ||   ((curType & 0x0f) == ContentSpecNode::Sequence))
    {
        ContentSpecNode* childNode = curNode->getFirst();
        ContentSpecNode* leftNode = convertContentSpecTree(childNode, checkUPA);
        ContentSpecNode* rightNode = curNode->getSecond();

        if (!rightNode) {

            retNode = expandContentModel(leftNode, minOccurs, maxOccurs);
            curNode->setAdoptFirst(false);
            delete curNode;
            return retNode;
        }

        if (leftNode != childNode) {

            curNode->setAdoptFirst(false);
            curNode->setFirst(leftNode);
            curNode->setAdoptFirst(true);
        }

        childNode = rightNode;
        rightNode =  convertContentSpecTree(childNode, checkUPA);

        if (rightNode != childNode) {

            curNode->setAdoptSecond(false);
            curNode->setSecond(rightNode);
            curNode->setAdoptSecond(true);
        }

        retNode =  expandContentModel(curNode, minOccurs, maxOccurs);
    }

    return retNode;
}

ContentSpecNode* ComplexTypeInfo::expandContentModel(ContentSpecNode* const specNode,
                                                     const int minOccurs,
                                                     const int maxOccurs)
{
    if (!specNode) {
        return 0;
    }

    ContentSpecNode* saveNode = specNode;
    ContentSpecNode* retNode = specNode;

    if (minOccurs == 1 && maxOccurs == 1) {
    }
    else if (minOccurs == 0 && maxOccurs == 1) {

        retNode = new (fMemoryManager) ContentSpecNode
        (
            ContentSpecNode::ZeroOrOne
            , retNode
            , 0
            , true
            , true
            , fMemoryManager
        );
    }
    else if (minOccurs == 0 && maxOccurs == -1) {
        retNode = new (fMemoryManager) ContentSpecNode
        (
            ContentSpecNode::ZeroOrMore
            , retNode
            , 0
            , true
            , true
            , fMemoryManager
        );
    }
    else if (minOccurs == 1 && maxOccurs == -1) {
        retNode = new (fMemoryManager) ContentSpecNode
        (
            ContentSpecNode::OneOrMore
            , retNode
            , 0
            , true
            , true
            , fMemoryManager
        );
    }
    else if (maxOccurs == -1) {

        retNode = new (fMemoryManager) ContentSpecNode
        (
            ContentSpecNode::OneOrMore
            , retNode
            , 0
            , true
            , true
            , fMemoryManager
        );

        for (int i=0; i < (int)(minOccurs-1); i++) {
            retNode = new (fMemoryManager) ContentSpecNode
            (
                ContentSpecNode::Sequence
                , saveNode
                , retNode
                , false
                , true
                , fMemoryManager
            );
        }
    }
    else {

        if (minOccurs == 0) {

            ContentSpecNode* optional = new (fMemoryManager) ContentSpecNode
            (
                ContentSpecNode::ZeroOrOne
                , saveNode
                , 0
                , true
                , true
                , fMemoryManager
            );

            retNode = optional;

            for (int i=0; i < (int)(maxOccurs-minOccurs-1); i++) {
                retNode = new (fMemoryManager) ContentSpecNode
                (
                    ContentSpecNode::Sequence
                    , retNode
                    , optional
                    , true
                    , false
                    , fMemoryManager
                );
            }
        }
        else {

            if (minOccurs > 1) {

                retNode = new (fMemoryManager) ContentSpecNode
                (
                    ContentSpecNode::Sequence
                    , retNode
                    , saveNode
                    , true
                    , false
                    , fMemoryManager
                );

                for (int i=1; i < (int)(minOccurs-1); i++) {
                    retNode = new (fMemoryManager) ContentSpecNode
                    (
                        ContentSpecNode::Sequence
                        , retNode
                        , saveNode
                        , true
                        , false
                        , fMemoryManager
                    );
                }
            }

            int counter = maxOccurs-minOccurs;

            if (counter > 0) {

                ContentSpecNode* optional = new (fMemoryManager) ContentSpecNode
                (
                    ContentSpecNode::ZeroOrOne
                    , saveNode
                    , 0
                    , false
                    , true
                    , fMemoryManager
                );

                retNode = new (fMemoryManager) ContentSpecNode
                (
                    ContentSpecNode::Sequence
                    , retNode
                    , optional
                    , true
                    , true
                    , fMemoryManager
                );

                for (int j=1; j < counter; j++) {

                    retNode = new (fMemoryManager) ContentSpecNode
                    (
                        ContentSpecNode::Sequence
					    , retNode
                        , optional
                        , true
                        , false
                        , fMemoryManager
                    );
                }
            }
        }
    }

    return retNode;
}

void ComplexTypeInfo::resizeContentSpecOrgURI() {

    unsigned int newSize = fContentSpecOrgURISize * 2;
    unsigned int* newContentSpecOrgURI = (unsigned int*) fMemoryManager->allocate
    (
        newSize * sizeof(unsigned int)
    ); //new unsigned int[newSize];

    // Copy the existing values
    unsigned int index = 0;
    for (; index < fContentSpecOrgURISize; index++)
        newContentSpecOrgURI[index] = fContentSpecOrgURI[index];

    for (; index < newSize; index++)
        newContentSpecOrgURI[index] = 0;

    // Delete the old array and udpate our members
    fMemoryManager->deallocate(fContentSpecOrgURI); //delete [] fContentSpecOrgURI;
    fContentSpecOrgURI = newContentSpecOrgURI;
    fContentSpecOrgURISize = newSize;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(ComplexTypeInfo)

void ComplexTypeInfo::serialize(XSerializeEngine& serEng)
{
   
    if (serEng.isStoring())
    {    
        serEng<<fAnonymous;
        serEng<<fAbstract;
        serEng<<fAdoptContentSpec;
        serEng<<fAttWithTypeId;
        serEng<<fPreprocessed;
        serEng<<fDerivedBy;
        serEng<<fBlockSet;
        serEng<<fFinalSet;
        serEng<<fScopeDefined;
        serEng<<fElementId;
        serEng<<fContentType;

        serEng.writeString(fTypeName);
        serEng.writeString(fTypeLocalName);
        serEng.writeString(fTypeUri);

        DatatypeValidator::storeDV(serEng, fBaseDatatypeValidator);
        DatatypeValidator::storeDV(serEng, fDatatypeValidator);

        serEng<<fBaseComplexTypeInfo;
        serEng<<fContentSpec;
        serEng<<fAttWildCard;
        serEng<<fAttList;

        /***
         * 
         * Serialize RefVectorOf<SchemaElementDecl>*    fElements;
         * Serialize RefHash2KeysTableOf<SchemaAttDef>* fAttDefs;
         ***/
        XTemplateSerializer::storeObject(fElements, serEng);
        XTemplateSerializer::storeObject(fAttDefs, serEng);

         /***
          *   Don't serialize 
          *
          *   fContentModel;
          *   fFormattedModel;
          *   fLocator;
          *   fSpecNodesToDelete
          * 
          *   fContentSpecOrgURI:     start of the array
          *   fContentSpecOrgURISize: size of the array
          *   fUniqueURI:             the current last element in the array
          ***/
    }
    else
    {
        serEng>>fAnonymous;
        serEng>>fAbstract;
        serEng>>fAdoptContentSpec;
        serEng>>fAttWithTypeId;
        serEng>>fPreprocessed;
        serEng>>fDerivedBy;
        serEng>>fBlockSet;
        serEng>>fFinalSet;
        serEng>>fScopeDefined;
        serEng>>fElementId;
        serEng>>fContentType;

        serEng.readString(fTypeName);
        serEng.readString(fTypeLocalName);
        serEng.readString(fTypeUri);

        fBaseDatatypeValidator = DatatypeValidator::loadDV(serEng);
        fDatatypeValidator     = DatatypeValidator::loadDV(serEng);

        serEng>>fBaseComplexTypeInfo;
        serEng>>fContentSpec;
        serEng>>fAttWildCard;
        serEng>>fAttList;

        /***
         * 
         * Deserialize RefVectorOf<SchemaElementDecl>*    fElements;
         * Deserialize RefHash2KeysTableOf<SchemaAttDef>* fAttDefs;
         ***/
        XTemplateSerializer::loadObject(&fElements, 8, false, serEng);
        XTemplateSerializer::loadObject(&fAttDefs, 29, true, serEng);

         /***
          *   Don't deserialize 
          *
          *   fFormattedModel;
          *   fLocator;
          *   fSpecNodesToDelete
          * 
          *   fContentSpecOrgURI:     start of the array
          *   fContentSpecOrgURISize: size of the array
          *   fUniqueURI:             the current last element in the array
          ***/

         fFormattedModel = 0;
         fLocator = 0;
         fSpecNodesToDelete = 0;
         fContentSpecOrgURI = 0;
         fContentSpecOrgURISize = 0;
         fUniqueURI = 0;
    }
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file ComplexTypeInfo.cpp
  */


