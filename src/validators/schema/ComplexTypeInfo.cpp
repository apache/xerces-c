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
#include <validators/schema/ComplexTypeInfo.hpp>
#include <validators/schema/SchemaAttDefList.hpp>
#include <validators/common/ContentSpecNode.hpp>

// ---------------------------------------------------------------------------
//  ComplexTypeInfo: Constructors and Destructor
// ---------------------------------------------------------------------------
ComplexTypeInfo::ComplexTypeInfo()
    : fAbstract(false)
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
    , fAttDefs(0)
    , fAttList(0)
{

}


ComplexTypeInfo::~ComplexTypeInfo()
{
    delete [] fTypeName;
    delete fContentSpec;
    delete fAttDefs;
    delete fAttList;
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

    if (fContentSpec) {
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

// ---------------------------------------------------------------------------
//  ComplexTypeInfo: Helper methods
// ---------------------------------------------------------------------------
void ComplexTypeInfo::faultInAttDefList() const
{
    // Use a hash modulus of 29 and tell it owns its elements
    ((ComplexTypeInfo*)this)->fAttDefs =
                    new RefHash2KeysTableOf<SchemaAttDef>(29, true);
}


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


/**
  * End of file ComplexTypeInfo.cpp
  */


