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
 * Revision 1.4  2003/11/27 22:52:37  knoaman
 * PSVIElement implementation
 *
 * Revision 1.3  2003/11/21 22:34:45  neilg
 * More schema component model implementation, thanks to David Cargill.
 * In particular, this cleans up and completes the XSModel, XSNamespaceItem,
 * XSAttributeDeclaration and XSAttributeGroup implementations.
 *
 * Revision 1.2  2003/11/06 21:50:33  neilg
 * fix compilation errors under gcc 3.3.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/PSVIElement.hpp>
#include <xercesc/framework/psvi/XSElementDeclaration.hpp>
#include <xercesc/framework/psvi/XSComplexTypeDefinition.hpp>
#include <xercesc/framework/psvi/XSSimpleTypeDefinition.hpp>

XERCES_CPP_NAMESPACE_BEGIN

PSVIElement::PSVIElement(MemoryManager* const manager):  
        PSVIItem(manager),
        fElementDecl(0),
        fNotationDecl(0),
        fSchemaInfo(0)
{
}

XSTypeDefinition* PSVIElement::getTypeDefinition()
{
    return fType;
}

    /**
     * If and only if that type definition is a simple type definition
     * with {variety} union, or a complex type definition whose {content type}
     * is a simple type definition with {variety} union, 
     
     * then an item isomorphic
     * to that member of the union's {member type definitions} which actually
     * validated the element item's normalized value.
     * 
     * @return  a simple type declaration
     */
XSSimpleTypeDefinition* PSVIElement::getMemberTypeDefinition() 
{
    return fMemberType;
}

void PSVIElement::reset( const VALIDITY_STATE          validityState
                       , const ASSESSMENT_TYPE         assessmentType
                       , const XMLCh* const            validationContext
                       , bool                          isSpecified
                       , XSElementDeclaration* const   elemDecl
                       , XSTypeDefinition* const       typeDef
                       , XSSimpleTypeDefinition* const memberType
                       , XSModel* const                schemaInfo
                       , const XMLCh* const            defaultValue
                       , const XMLCh* const            normalizedValue
                       , const XMLCh* const            canonicalValue
                       , XSNotationDeclaration* const  notationDecl)
{
    fValidationContext = validationContext;
    fValidityState = validityState;
    fAssessmentType = assessmentType;
    fIsSpecified = isSpecified;
    fType = typeDef;
    fMemberType = memberType;
    fElementDecl = elemDecl;
    fNotationDecl = notationDecl;
    fSchemaInfo = schemaInfo;
    fDefaultValue = defaultValue;
    fNormalizedValue = normalizedValue;
    fCanonicalValue = canonicalValue;
}

XERCES_CPP_NAMESPACE_END


