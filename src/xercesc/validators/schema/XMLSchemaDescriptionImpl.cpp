/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights
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
 * Revision 1.3  2003/07/31 17:14:27  peiyongz
 * Grammar embed grammar description
 *
 * Revision 1.2  2003/06/20 22:19:12  peiyongz
 * Stateless Grammar Pool :: Part I
 *
 * Revision 1.1  2003/06/20 18:40:28  peiyongz
 * Stateless Grammar Pool :: Part I
 *
 * $Id$
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/XMLSchemaDescriptionImpl.hpp>
#include <xercesc/util/QName.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLSchemaDescriptionImpl: constructor and destructor
// ---------------------------------------------------------------------------
XMLSchemaDescriptionImpl::XMLSchemaDescriptionImpl(const XMLCh* const   targetNamespace
                                                 , MemoryManager* const memMgr)
:XMLSchemaDescription(memMgr)
,fContextType(CONTEXT_UNKNOWN)
,fNamespace(0)
,fLocationHints(0)
,fTriggeringComponent(0)
,fEnclosingElementName(0)
,fAttributes(0) 
{
    if (targetNamespace)
        fNamespace = XMLString::replicate(targetNamespace, memMgr);


    fLocationHints =  new (memMgr) RefArrayVectorOf<XMLCh>(4, true, memMgr);

    /***
        fAttributes
    ***/
}

XMLSchemaDescriptionImpl::~XMLSchemaDescriptionImpl()
{
    if (fNamespace)
        XMLGrammarDescription::getMemoryManager()->deallocate((void*)fNamespace);

    if (fLocationHints)
        delete fLocationHints;

    if (fTriggeringComponent)
        delete fTriggeringComponent;

    if (fEnclosingElementName)
        delete fEnclosingElementName;

}

const XMLCh* XMLSchemaDescriptionImpl::getGrammarKey() const
{ 
    return getTargetNamespace(); 
}
    
XMLSchemaDescription::ContextType XMLSchemaDescriptionImpl::getContextType() const
{
    return fContextType; 
}

const XMLCh* XMLSchemaDescriptionImpl::getTargetNamespace() const
{ 
    return fNamespace; 
}

RefArrayVectorOf<XMLCh>* XMLSchemaDescriptionImpl::getLocationHints() const
{ 
    return fLocationHints; 
}

const QName* XMLSchemaDescriptionImpl::getTriggeringComponent() const                      
{ 
    return fTriggeringComponent; 
}

const QName* XMLSchemaDescriptionImpl::getEnclosingElementName() const
{ 
    return fEnclosingElementName; 
}

const XMLAttDef* XMLSchemaDescriptionImpl::getAttributes() const                      
{ 
    return fAttributes; 
}
         
void XMLSchemaDescriptionImpl::setContextType(ContextType type)
{ 
    fContextType = type; 
}

void XMLSchemaDescriptionImpl::setTargetNamespace(const XMLCh* const newNamespace)
{  
    if (fNamespace)
        XMLGrammarDescription::getMemoryManager()->deallocate((void*)fNamespace);
                                
    fNamespace = XMLString::replicate(newNamespace, XMLGrammarDescription::getMemoryManager()); 
}

void XMLSchemaDescriptionImpl::setLocationHints(const XMLCh* const hint)
{    
    fLocationHints->addElement(XMLString::replicate(hint, XMLGrammarDescription::getMemoryManager())); 
}

void XMLSchemaDescriptionImpl::setTriggeringComponent(QName* const trigComponent)
{ 
    if ( fTriggeringComponent)
        delete fTriggeringComponent;
                                
    fTriggeringComponent = new QName(*trigComponent); 

}

void XMLSchemaDescriptionImpl::setEnclosingElementName(QName* const encElement)
{ 
    if (fEnclosingElementName)
        delete fEnclosingElementName;
                                 
    fEnclosingElementName = new QName(*encElement); 

}

void XMLSchemaDescriptionImpl::setAttributes(XMLAttDef* const attDefs)
{ 
    // XMLAttDef is part of the grammar that this description refers to
    // so we reference to it instead of adopting/owning/cloning.
    fAttributes = attDefs; 
}

XERCES_CPP_NAMESPACE_END
