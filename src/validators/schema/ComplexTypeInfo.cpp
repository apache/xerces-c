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
#include <framework/XMLBuffer.hpp>
#include <validators/schema/ComplexTypeInfo.hpp>
#include <validators/schema/SchemaAttDefList.hpp>
#include <validators/common/AllContentModel.hpp>
#include <validators/common/ContentSpecNode.hpp>
#include <validators/common/DFAContentModel.hpp>
#include <validators/common/MixedContentModel.hpp>
#include <validators/common/SimpleContentModel.hpp>

// ---------------------------------------------------------------------------
//  ComplexTypeInfo: Constructors and Destructor
// ---------------------------------------------------------------------------
ComplexTypeInfo::ComplexTypeInfo()
    : fAbstract(false)
    , fAdoptContentSpec(true)
    , fDerivedBy(0)
    , fBlockSet(0)
    , fFinalSet(0)
    , fScopeDefined(Grammar::TOP_LEVEL_SCOPE)
    , fElementId(XMLElementDecl::fgInvalidElemId)
    , fTypeName(0)
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
{

}


ComplexTypeInfo::~ComplexTypeInfo()
{
    delete [] fTypeName;

    if (fAdoptContentSpec) {
        delete fContentSpec;
    }

    delete fAttWildCard;
    delete fAttDefs;
    delete fAttList;
    delete fElements;

    delete fContentModel;
    delete [] fFormattedModel;
    delete [] fContentSpecOrgURI;
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
}

void ComplexTypeInfo::setContentSpec(ContentSpecNode* const toAdopt) {

    if (fContentSpec && fAdoptContentSpec) {
        delete fContentSpec;
    }

    fContentSpec = toAdopt;
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

        ((ComplexTypeInfo*)this)->fAttList = new SchemaAttDefList(fAttDefs);
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
        retVal = new SchemaAttDef(prefix, baseName, uriId);
        retVal->setElemId(getElementId());
        fAttDefs->put((void*)baseName, uriId, retVal);

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
    RefHash2KeysTableOfEnumerator<SchemaAttDef> enumDefs(fAttDefs);
    while (enumDefs.hasMoreElements())
        enumDefs.nextElement().setProvided(false);

    return true;
}


void ComplexTypeInfo::checkUniqueParticleAttribution (GrammarResolver*  const pGrammarResolver,
                                                      XMLStringPool*    const pStringPool,
                                                      XMLValidator*     const pValidator)
{
    if (fContentSpec) {
        ContentSpecNode* specNode = new ContentSpecNode(*fContentSpec);
        XMLContentModel* cm = makeContentModel(true, specNode);

        if (cm) {
            cm->checkUniqueParticleAttribution(pGrammarResolver, pStringPool, pValidator, fContentSpecOrgURI);
            delete cm;
        }

        delete specNode;
    }
}

// ---------------------------------------------------------------------------
//  ComplexTypeInfo: Private Helper methods
// ---------------------------------------------------------------------------
void ComplexTypeInfo::faultInAttDefList() const
{
    // Use a hash modulus of 29 and tell it owns its elements
    ((ComplexTypeInfo*)this)->fAttDefs =
                    new RefHash2KeysTableOf<SchemaAttDef>(29, true);
}

XMLCh* ComplexTypeInfo::formatContentModel() const
{
    XMLCh* newValue = 0;
    if (fContentType == SchemaElementDecl::Any)
    {
        newValue = XMLString::replicate(XMLUni::fgAnyString);
    }
     else if (fContentType == SchemaElementDecl::Empty)
    {
        newValue = XMLString::replicate(XMLUni::fgEmptyString);
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
            XMLBuffer bufFmt;


            specNode->formatSpec(bufFmt);
            newValue = XMLString::replicate(bufFmt.getRawBuffer());
        }
    }
    return newValue;
}

XMLContentModel* ComplexTypeInfo::makeContentModel(const bool checkUPA, ContentSpecNode* specNode)
{
    // expand the content spec first
    fContentSpecOrgURI = new unsigned int[fContentSpecOrgURISize];
    if (specNode) {
        specNode = convertContentSpecTree(specNode, true, checkUPA);
    }
    else {
        specNode = convertContentSpecTree(fContentSpec, fAdoptContentSpec, checkUPA);
        if (specNode != fContentSpec) {
            fContentSpec = specNode;
            fAdoptContentSpec = true;
        }
    }


    XMLContentModel* cmRet = 0;
    if (fContentType == SchemaElementDecl::Simple) {
       // just return nothing
    }
     else if (fContentType == SchemaElementDecl::Mixed)
    {
        //
        //  Just create a mixel content model object. This type of
        //  content model is optimized for mixed content validation.
        //
        if(!specNode)
            ThrowXML(RuntimeException, XMLExcepts::CM_UnknownCMSpecType);

        if (specNode->getElement()->getURI() == XMLElementDecl::fgPCDataElemId) {
            cmRet = new MixedContentModel(false, specNode);
        }
        else {
            cmRet = createChildModel(specNode, true);
        }
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
         cmRet = createChildModel(specNode, false);
    }
     else
    {
        ThrowXML(RuntimeException, XMLExcepts::CM_MustBeMixedOrChildren);
    }
    return cmRet;
}



// ---------------------------------------------------------------------------
//  SchemaElementDecl: Private helper methods
// ---------------------------------------------------------------------------
XMLContentModel* ComplexTypeInfo::createChildModel(ContentSpecNode* specNode, const bool isMixed)
{
    if(!specNode)
        ThrowXML(RuntimeException, XMLExcepts::CM_UnknownCMSpecType);

    ContentSpecNode::NodeTypes specType = specNode->getType();
    //
    //  Do a sanity check that the node is does not have a PCDATA id. Since,
    //  if it was, it should have already gotten taken by the Mixed model.
    //
    if (specNode->getElement()->getURI() == XMLElementDecl::fgPCDataElemId)
        ThrowXML(RuntimeException, XMLExcepts::CM_NoPCDATAHere);

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
        ContentSpecNode* rightNode = specNode->getSecond();
        ContentSpecNode::NodeTypes rightType = rightNode->getType();        

        if (rightType == ContentSpecNode::All) {
            // All the nodes under an ALL must be additional ALL nodes and
            // ELEMENTs (or ELEMENTs under ZERO_OR_ONE nodes.)
            // We collapse the ELEMENTs into a single vector.
            return new AllContentModel(rightNode, true);
        }
        else if (rightType == ContentSpecNode::ZeroOrOne) {
            // An ALL node can appear under a ZERO_OR_ONE node.
            if (rightNode->getFirst()->getType() == ContentSpecNode::All) {
                return new AllContentModel(rightNode->getFirst(), true);
            }
        }
        // otherwise, let fall through to build a DFAContentModel
    }
     else if (specType == ContentSpecNode::Leaf)
    {
        // Create a simple content model
        return new SimpleContentModel
        (
            false
            , specNode->getElement()
            , 0
            , ContentSpecNode::Leaf
        );
    }
     else if ((specType == ContentSpecNode::Choice)
          ||  (specType == ContentSpecNode::Sequence))
    {
        //
        //  Lets see if both of the children are leafs. If so, then it has to
        //  be a simple content model
        //
        if ((specNode->getFirst()->getType() == ContentSpecNode::Leaf)
        &&  (specNode->getSecond()->getType() == ContentSpecNode::Leaf))
        {
            return new SimpleContentModel
            (
                false
                , specNode->getFirst()->getElement()
                , specNode->getSecond()->getElement()
                , specType
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
            return new SimpleContentModel
            (
                false
                , specNode->getFirst()->getElement()
                , 0
                , specType
            );
        }
        else if (specNode->getFirst()->getType() == ContentSpecNode::All)
            return new AllContentModel(specNode->getFirst(), false);

    }
    else if (specType == ContentSpecNode::All)
        return new AllContentModel(specNode, false);

    else
    {
        ThrowXML(RuntimeException, XMLExcepts::CM_UnknownCMSpecType);
    }

    // Its not any simple type of content, so create a DFA based content model
    return new DFAContentModel(false, specNode, isMixed);
}

ContentSpecNode* ComplexTypeInfo::convertContentSpecTree(ContentSpecNode* const curNode, const bool toAdoptSpecNode, const bool checkUPA) {

    if (!curNode)
        return 0;

    // When checking Unique Particle Attribution, rename leaf elements
    if (checkUPA) {
        fContentSpecOrgURI[fUniqueURI] = curNode->getElement()->getURI();
        curNode->getElement()->setURI(fUniqueURI);
        fUniqueURI++;
        if (fUniqueURI == fContentSpecOrgURISize)
            resizeContentSpecOrgURI();
    }

    // Get the spec type of the passed node
    int minOccurs = curNode->getMinOccurs();
    int maxOccurs = curNode->getMaxOccurs();
    const ContentSpecNode::NodeTypes curType = curNode->getType();
    ContentSpecNode* retNode = curNode;

    if ((curType & 0x0f) == ContentSpecNode::Any
        || (curType & 0x0f) == ContentSpecNode::Any_Other
        || (curType & 0x0f) == ContentSpecNode::Any_NS
        || curType == ContentSpecNode::Leaf)
    {
        retNode =  expandContentModel(curNode, minOccurs, maxOccurs, toAdoptSpecNode);
    }
    else if ((curType == ContentSpecNode::Choice)
        ||   (curType == ContentSpecNode::All)
        ||   (curType == ContentSpecNode::Sequence))
    {
        bool toAdoptLeft = curNode->isFirstAdopted();
        ContentSpecNode* leftNode = convertContentSpecTree(curNode->getFirst(), toAdoptLeft, checkUPA);
        ContentSpecNode* rightNode = curNode->getSecond();
       
        if (leftNode != curNode->getFirst()) {

            curNode->setAdoptFirst(false);
            curNode->setFirst(leftNode);
            toAdoptLeft = true;
        }

        if (!rightNode) {

            retNode = expandContentModel(leftNode, minOccurs, maxOccurs, toAdoptLeft);
            
            if (toAdoptSpecNode) {

                curNode->setAdoptFirst(false);				
                delete curNode;
            }

            return retNode;
        }

        curNode->setAdoptFirst(toAdoptLeft);

        rightNode =  convertContentSpecTree(curNode->getSecond(), curNode->isSecondAdopted(), checkUPA);
        if (rightNode != curNode->getSecond()) {

            curNode->setAdoptSecond(false);
            curNode->setSecond(rightNode);
            curNode->setAdoptSecond(true);
        }

        retNode =  expandContentModel(curNode, minOccurs, maxOccurs, toAdoptSpecNode);
    }

    return retNode;
}

ContentSpecNode* ComplexTypeInfo::expandContentModel(ContentSpecNode* const specNode,
                                                     const int minOccurs,
                                                     const int maxOccurs,
                                                     const bool toAdoptSpecNode)
{
    if (!specNode) {
        return 0;
    }

    ContentSpecNode* saveNode = specNode;
    ContentSpecNode* retNode = specNode;

    if (minOccurs == 1 && maxOccurs == 1) {
    }
    else if (minOccurs == 0 && maxOccurs == 1) {

        retNode = new ContentSpecNode(ContentSpecNode::ZeroOrOne,
                                      retNode, 0, toAdoptSpecNode);
    }
    else if (minOccurs == 0 && maxOccurs == -1) {
        retNode = new ContentSpecNode(ContentSpecNode::ZeroOrMore,
                                      retNode, 0, toAdoptSpecNode);
    }
    else if (minOccurs == 1 && maxOccurs == -1) {
        retNode = new ContentSpecNode(ContentSpecNode::OneOrMore,
                                      retNode, 0, toAdoptSpecNode);
    }
    else if (maxOccurs == -1) {

        retNode = new ContentSpecNode(ContentSpecNode::OneOrMore,
                                      retNode, 0, toAdoptSpecNode);

        for (int i=0; i < (int)(minOccurs-1); i++) {
            retNode = new ContentSpecNode(ContentSpecNode::Sequence,
                                          saveNode, retNode, false);
        }
    }
    else {

        if (minOccurs == 0) {

            ContentSpecNode* optional =
                new ContentSpecNode(ContentSpecNode::ZeroOrOne, saveNode, 0, toAdoptSpecNode);

            retNode = optional;

            for (int i=0; i < (int)(maxOccurs-minOccurs-1); i++) {
                retNode = new ContentSpecNode(ContentSpecNode::Sequence,
                                              retNode, optional, true, false);
            }
        }
        else {

            bool isRetAdopted = toAdoptSpecNode;

            if (minOccurs > 1) {

                retNode = new ContentSpecNode(ContentSpecNode::Sequence,
                                              retNode, saveNode, toAdoptSpecNode, false);

                for (int i=1; i < (int)(minOccurs-1); i++) {
                    retNode = new ContentSpecNode(ContentSpecNode::Sequence,
                                                  retNode, saveNode, true, false);
                }

                isRetAdopted = true;
            }

            int counter = maxOccurs-minOccurs;

            if (counter > 0) {

                ContentSpecNode* optional = new ContentSpecNode(ContentSpecNode::ZeroOrOne, saveNode, 0, false);

                retNode = new ContentSpecNode(ContentSpecNode::Sequence,
					                          retNode, optional, isRetAdopted, true);

                for (int j=1; j < counter; j++) {

                    retNode = new ContentSpecNode(ContentSpecNode::Sequence,
					                              retNode, optional, true, false);
                }
            }
        }
    }

    return retNode;
}

void ComplexTypeInfo::resizeContentSpecOrgURI() {

    unsigned int newSize = fContentSpecOrgURISize * 2;
    unsigned int* newContentSpecOrgURI = new unsigned int[newSize];

    // Copy the existing values
    unsigned int index = 0;
    for (; index < fContentSpecOrgURISize; index++)
        newContentSpecOrgURI[index] = fContentSpecOrgURI[index];

    for (; index < newSize; index++)
        newContentSpecOrgURI[index] = 0;

    // Delete the old array and udpate our members
    delete [] fContentSpecOrgURI;
    fContentSpecOrgURI = newContentSpecOrgURI;
    fContentSpecOrgURISize = newSize;
}


/**
  * End of file ComplexTypeInfo.cpp
  */


