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

#include <xercesc/framework/psvi/XSAttributeGroupDefinition.hpp>
#include <xercesc/validators/schema/XercesAttGroupInfo.hpp>
#include <xercesc/framework/psvi/XSAttributeUse.hpp>
#include <xercesc/framework/psvi/XSWildcard.hpp>

XERCES_CPP_NAMESPACE_BEGIN

XSAttributeGroupDefinition::XSAttributeGroupDefinition(XercesAttGroupInfo* xercesAttGroupInfo,
                                                       XMLStringPool*  uriStringPool,
                                                       MemoryManager * const manager):
    fXercesAttGroupInfo(xercesAttGroupInfo),
    fXSAttributeUseList(0),
    fXSWildcard(0),
    XSObject(XSConstants::ATTRIBUTE_GROUP_DEFINITION, manager )
{
    unsigned int attCount = fXercesAttGroupInfo->attributeCount();
    if (fXercesAttGroupInfo->attributeCount())
    {
        fXSAttributeUseList = new (manager) RefVectorOf <XSAttributeUse> (attCount, true, manager);
        for (unsigned int i=0; i < attCount; i++) 
        {
            fXSAttributeUseList->addElement(new (manager) XSAttributeUse(fXercesAttGroupInfo->attributeAt(i), uriStringPool, manager));
        }
    }
    
    if (fXercesAttGroupInfo->getCompleteWildCard()) 
    {
        fXSWildcard = new (manager) XSWildcard(fXercesAttGroupInfo->getCompleteWildCard(), uriStringPool, manager);
    }
}

XSAttributeGroupDefinition::~XSAttributeGroupDefinition()
{
    if (fXSAttributeUseList)
    {
        delete fXSAttributeUseList;
    }
    if (fXSWildcard)
    {
        delete fXSWildcard;
    }
}

// XSAttributeGroupDefinition methods

/**
 * A set of [attribute uses]. 
 */
XSAttributeUseList *XSAttributeGroupDefinition::getAttributeUses()
{
    return fXSAttributeUseList;
}

/**
 * Optional. A [wildcard]. 
 */
XSWildcard *XSAttributeGroupDefinition::getAttributeWildcard()
{
    return fXSWildcard;
}

/**
 * Optional. An [annotation]. 
 */
XSAnnotation *XSAttributeGroupDefinition::getAnnotation()
{
    // REVISIT
    return 0;
}

XERCES_CPP_NAMESPACE_END


