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
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.2  2003/11/06 15:30:04  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSWildcard.hpp>
#include <xercesc/validators/schema/SchemaAttDef.hpp>
#include <xercesc/util/ValueVectorOf.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/util/StringPool.hpp>

XERCES_CPP_NAMESPACE_BEGIN

XSWildcard::XSWildcard(SchemaAttDef*            attWildCard,
                       XMLStringPool*           uriStringPool,
                       MemoryManager * const    manager):
    fNsConstraintList(0),
    fURIStringPool(uriStringPool),
    XSObject(XSConstants::WILDCARD, manager )
{
    switch (attWildCard->getType())
    {
        case XMLAttDef::Any_Other:
            fConstraintType = NSCONSTRAINT_NOT;
            break;
        case XMLAttDef::Any_List:
            fConstraintType = NSCONSTRAINT_DERIVATION_LIST;
            break;
        default:
            //case XMLAttDef::Any_Any:
            // REVISIT: should I issue error if not Any_Any???
            fConstraintType = NSCONSTRAINT_ANY;
            break;
    }

    switch (attWildCard->getDefaultType())
    {
        case XMLAttDef::ProcessContents_Skip:
            fProcessContents = PC_SKIP;
            break;
        case XMLAttDef::ProcessContents_Lax:
            fProcessContents = PC_LAX;
            break;
        default:
            //case XMLAttDef::ProcessContents_Strict:
            // REVISIT: should I issue error if not ProcessContents_Strict???
            fProcessContents = PC_STRICT;
            break;
    }

    if (fConstraintType != NSCONSTRAINT_ANY ) {
        ValueVectorOf<unsigned int>* nsList = attWildCard->getNamespaceList();
        unsigned int nsListSize = nsList->size();
        if (nsListSize) {
            fNsConstraintList = new (manager) RefArrayVectorOf <XMLCh> (nsListSize, true, manager);              
            for (unsigned int i=0; i < nsListSize; i++)
            {
                XMLCh* nameSpace = (XMLCh*) fURIStringPool->getValueForId(nsList->elementAt(i));
                fNsConstraintList->addElement(nameSpace);
            }
        }
    }
}

XSWildcard::XSWildcard(ContentSpecNode*         elmWildCard,
                       XMLStringPool*           uriStringPool,
                       MemoryManager* const     manager):
    fNsConstraintList(0),
    fURIStringPool(uriStringPool),
    XSObject(XSConstants::WILDCARD, manager )
{
    // REVISIT:
    // this is the right approach but need to handle getType() of
    // choice_any... will need to step through a list to get all the names...
    // Khaled: will update getType to have choice_any
    switch (elmWildCard->getType())
    {
        case ContentSpecNode::Any_Other:
            fConstraintType = NSCONSTRAINT_NOT;
            fProcessContents = PC_STRICT;
            break;
        case ContentSpecNode::Any_NS:
            fConstraintType = NSCONSTRAINT_DERIVATION_LIST;
            fProcessContents = PC_STRICT;
            break;
        case ContentSpecNode::Any:
            fConstraintType = NSCONSTRAINT_ANY;
            fProcessContents = PC_STRICT;
            break;

        case ContentSpecNode::Any_Other_Lax:
            fConstraintType = NSCONSTRAINT_NOT;
            fProcessContents = PC_LAX;
            break;
        case ContentSpecNode::Any_NS_Lax:
            fConstraintType = NSCONSTRAINT_DERIVATION_LIST;
            fProcessContents = PC_LAX;
            break;
        case ContentSpecNode::Any_Lax:
            fConstraintType = NSCONSTRAINT_ANY;
            fProcessContents = PC_LAX;
            break;

        case ContentSpecNode::Any_Other_Skip:
            fConstraintType = NSCONSTRAINT_NOT;
            fProcessContents = PC_SKIP;
            break;
        case ContentSpecNode::Any_NS_Skip:
            fConstraintType = NSCONSTRAINT_DERIVATION_LIST;
            fProcessContents = PC_SKIP;
            break;
        case ContentSpecNode::Any_Skip:
            fConstraintType = NSCONSTRAINT_ANY;
            fProcessContents = PC_SKIP;
            break;

        default:
            // REVISIT: should I issue error???
            // need to set something.....
            fConstraintType = NSCONSTRAINT_ANY;
            fProcessContents = PC_STRICT;
            break;
    }

    if (fConstraintType != NSCONSTRAINT_ANY ) {
        fNsConstraintList = new (manager) RefArrayVectorOf <XMLCh> (1, true, manager); 
        XMLCh* nameSpace = (XMLCh*) fURIStringPool->getValueForId(elmWildCard->getElement()->getURI());
        fNsConstraintList->addElement(nameSpace);        
    }
}

// XSWildcard methods


/**
 * Namespace constraint: A constraint type: any, not, list. 
 */
XSWildcard::NAMESPACE_CONSTRAINT XSWildcard::getConstraintType() const
{
    return fConstraintType;
}

/**
 * Namespace constraint. For <code>constraintType</code> 
 * <code>NSCONSTRAINT_DERIVATION_LIST</code>, the list contains allowed namespaces. 
 * For <code>constraintType</code> <code>NSCONSTRAINT_NOT</code>, the 
 * list contains disallowed namespaces. 
 */
StringList *XSWildcard::getNsConstraintList()
{
    return fNsConstraintList;
}

/**
 * [process contents]: one of skip, lax or strict. Valid constants values 
 * are: <code>PC_SKIP, PC_LAX, PC_STRICT</code>. 
 */
XSWildcard::PROCESS_CONTENTS XSWildcard::getProcessContents() const
{
    return fProcessContents;
}

/**
 * Optional. An [annotation]. 
 */
XSAnnotation *XSWildcard::getAnnotation()
{
    // REVISIT
    return 0;
}

XERCES_CPP_NAMESPACE_END
