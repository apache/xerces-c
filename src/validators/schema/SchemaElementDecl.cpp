/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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
 * Revision 1.1  2001/02/27 18:48:22  tng
 * Schema: Add SchemaAttDef, SchemaElementDecl, SchemaAttDefList.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/XMLUni.hpp>
#include <framework/XMLBuffer.hpp>
#include <validators/common/DFAContentModel.hpp>
#include <validators/common/ContentSpecNode.hpp>
#include <validators/common/MixedContentModel.hpp>
#include <validators/common/SimpleContentModel.hpp>
#include <validators/schema/SchemaAttDefList.hpp>
#include <validators/schema/SchemaElementDecl.hpp>

// ---------------------------------------------------------------------------
//  SchemaElementDecl: Constructors and Destructor
// ---------------------------------------------------------------------------
SchemaElementDecl::SchemaElementDecl() :

    fAttDefs(0)
    , fAttList(0)
    , fContentSpec(0)
    , fModelType(Any)
    , fDatatypeValidator(0)
    , fEnclosingScope(-1)
{
}

SchemaElementDecl::SchemaElementDecl(const XMLCh* const                  prefix
                                   , const XMLCh* const                  localPart
                                   , const int                           uriId
                                   , const SchemaElementDecl::ModelTypes type
                                   , const int                           enclosingScope
                                   ) :
    fAttDefs(0)
    , fAttList(0)
    , fContentSpec(0)
    , fModelType(type)
    , fDatatypeValidator(0)
    , fEnclosingScope(enclosingScope)
{
    fElementName = new QName(prefix, localPart, uriId);
}

SchemaElementDecl::~SchemaElementDecl()
{
    delete fAttDefs;
    delete fAttList;
    delete fContentSpec;
    delete fElementName;
}


// ---------------------------------------------------------------------------
//  SchemaElementDecl: XMLElementDecl virtual interface implementation
// ---------------------------------------------------------------------------
XMLAttDef* SchemaElementDecl::findAttr(const XMLCh* const    qName
                                     , const unsigned int    uriId
                                     , const XMLCh* const    baseName
                                     , const XMLCh* const    prefix
                                     , const LookupOpts      options
                                     , bool&           wasAdded) const
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
        retVal->setElemId(getId());
        fAttDefs->put((void*)baseName, uriId, retVal);

        wasAdded = true;
    }
     else
    {
        wasAdded = false;
    }
    return retVal;
}


XMLAttDefList& SchemaElementDecl::getAttDefList() const
{
    if (!fAttList)
    {
        // If the att def list is not made yet, then fault it in too
        if (!fAttDefs)
            faultInAttDefList();

        ((SchemaElementDecl*)this)->fAttList = new SchemaAttDefList(fAttDefs);
    }

    // Reset it before we return it
    fAttList->Reset();
    return *fAttList;
}


const XMLCh* SchemaElementDecl::getBaseName() const
{
    return fElementName->getLocalPart();
}


XMLElementDecl::CharDataOpts SchemaElementDecl::getCharDataOpts() const
{
    XMLElementDecl::CharDataOpts retVal;
    switch(fModelType)
    {
        case Children :
            retVal = XMLElementDecl::SpacesOk;
            break;

        case Empty :
            retVal = XMLElementDecl::NoCharData;
            break;

        default :
            retVal = XMLElementDecl::AllCharData;
            break;
    }
    return retVal;
}


bool SchemaElementDecl::hasAttDefs() const
{
    // If the collection hasn't been faulted in, then no att defs
    if (!fAttDefs)
        return false;

    return !fAttDefs->isEmpty();
}


bool SchemaElementDecl::resetDefs()
{
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

void
SchemaElementDecl::setContentSpec(ContentSpecNode* toAdopt)
{
    delete fContentSpec;
    fContentSpec = toAdopt;
}

// ---------------------------------------------------------------------------
//  SchemaElementDecl: Getter methods
// ---------------------------------------------------------------------------
const SchemaAttDef* SchemaElementDecl::getAttDef(const XMLCh* const baseName, const int uriId) const
{
    // If no list, then return a null
    if (!fAttDefs)
        return 0;

    return fAttDefs->get(baseName, uriId);
}

SchemaAttDef* SchemaElementDecl::getAttDef(const XMLCh* const baseName, const int uriId)
{
    // If no list, then return a null
    if (!fAttDefs)
        return 0;

    return fAttDefs->get(baseName, uriId);
}


// ---------------------------------------------------------------------------
//  SchemaElementDecl: Implementation of the protected virtual interface
// ---------------------------------------------------------------------------
void SchemaElementDecl::addAttDef(SchemaAttDef* const toAdd)
{
    // Fault in the att list if required
    if (!fAttDefs)
            faultInAttDefList();

    // Tell this guy the element id of its parent (us)
    toAdd->setElemId(getId());

    fAttDefs->put((void*)(toAdd->getAttName()->getLocalPart()), toAdd->getAttName()->getURI(), toAdd);
}


// ---------------------------------------------------------------------------
//  SchemaElementDecl: Implementation of the protected virtual interface
// ---------------------------------------------------------------------------
XMLCh*
SchemaElementDecl::formatContentModel(const XMLValidator& validator) const
{
    XMLCh* newValue = 0;
    if (fModelType == Any)
    {
        newValue = XMLString::replicate(XMLUni::fgAnyString);
    }
     else if (fModelType == Empty)
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
        XMLBuffer bufFmt;
        fContentSpec->formatSpec(validator, bufFmt);
        newValue = XMLString::replicate(bufFmt.getRawBuffer());
    }
    return newValue;
}

XMLContentModel* SchemaElementDecl::makeContentModel() const
{
    XMLContentModel* cmRet = 0;
    if (fModelType == Simple) {
       // just return nothing
    }
     else if (fModelType == Mixed)
    {
        //
        //  Just create a mixel content model object. This type of
        //  content model is optimized for mixed content validation.
        //
        cmRet = new MixedContentModel(*this);
    }
     else if (fModelType == Children)
    {
        //
        //  This method will create an optimal model for the complexity
        //  of the element's defined model. If its simple, it will create
        //  a SimpleContentModel object. If its a simple list, it will
        //  create a SimpleListContentModel object. If its complex, it
        //  will create a DFAContentModel object.
        //
        cmRet = createChildModel();
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
XMLContentModel* SchemaElementDecl::createChildModel() const
{
    // Get the content spec node of the element
    const ContentSpecNode* specNode = getContentSpec();

    //
    //  Do a sanity check that the node is does not have a PCDATA id. Since,
    //  if it was, it should have already gotten taken by the Mixed model.
    //
    if (specNode->getElemId() == XMLElementDecl::fgPCDataElemId)
        ThrowXML(RuntimeException, XMLExcepts::CM_NoPCDATAHere);

    //
    //  According to the type of node, we will create the correct type of
    //  content model.
    //
    if ((specNode->getType() == ContentSpecNode.Any) ||
       (specNode->getType() == ContentSpecNode.Any_Other) ||
       (specNode->getType() == ContentSpecNode.Any_Local)) {
       // let fall through to build a DFAContentModel
    }
     else if (specNode->getType() == ContentSpecNode::Leaf)
    {
        // Create a simple content model
        return new SimpleContentModel
        (
            specNode->getElemId()
            , XMLElementDecl::fgInvalidElemId
            , ContentSpecNode::Leaf
        );
    }
     else if ((specNode->getType() == ContentSpecNode::Choice)
          ||  (specNode->getType() == ContentSpecNode::Sequence))
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
                specNode->getFirst()->getElemId()
                , specNode->getSecond()->getElemId()
                , specNode->getType()
            );
        }
    }
     else if ((specNode->getType() == ContentSpecNode::OneOrMore)
          ||  (specNode->getType() == ContentSpecNode::ZeroOrMore)
          ||  (specNode->getType() == ContentSpecNode::ZeroOrOne))
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
                specNode->getFirst()->getElemId()
                , XMLElementDecl::fgInvalidElemId
                , specNode->getType()
            );
        }
    }
     else
    {
        ThrowXML(RuntimeException, XMLExcepts::CM_UnknownCMSpecType);
    }

    // Its not any simple type of content, so create a DFA based content model
    return new DFAContentModel(*this);
}


void SchemaElementDecl::faultInAttDefList() const
{
    // Use a hash modulus of 29 and tell it owns its elements
    ((SchemaElementDecl*)this)->fAttDefs = new RefHash2KeysTableOf<SchemaAttDef>(29, true);
}
