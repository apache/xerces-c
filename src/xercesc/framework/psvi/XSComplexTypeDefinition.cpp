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
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSComplexTypeDefinition.hpp>

XERCES_CPP_NAMESPACE_BEGIN

XSComplexTypeDefinition::XSComplexTypeDefinition( MemoryManager* const manager ):  
        XSTypeDefinition(COMPLEX_TYPE, manager)
{
}

// XSComplexTypeDefinition methods

/**
 * [derivation method]: either <code>DERIVATION_EXTENSION</code>, 
 * <code>DERIVATION_RESTRICTION</code>, or <code>DERIVATION_NONE</code> 
 * (see <code>XSObject</code>). 
 */
XSConstants::DERIVATION_TYPE XSComplexTypeDefinition::getDerivationMethod() const
{
    // REVISIT
    return XSConstants::DERIVATION_NONE;
}

/**
 * [abstract]: a boolean. Complex types for which <code>abstract</code> is 
 * true must not be used as the type definition for the validation of 
 * element information items. 
 */
bool XSComplexTypeDefinition::getAbstract() const
{
    // REVISIT
    return false;
}

/**
 *  A set of attribute uses. 
 */
XSAttributeUseList *XSComplexTypeDefinition::getAttributeUses()
{
    // REVISIT
    return 0;
}

/**
 * Optional.An attribute wildcard. 
 */
XSWildcard *XSComplexTypeDefinition::getAttributeWildcard()
{
    // REVISIT
    return 0;
}

/**
 * [content type]: one of empty (<code>CONTENTTYPE_EMPTY</code>), a simple 
 * type definition (<code>CONTENTTYPE_SIMPLE</code>), mixed (
 * <code>CONTENTTYPE_MIXED</code>), or element-only (
 * <code>CONTENTTYPE_ELEMENT</code>). 
 */
XSComplexTypeDefinition::CONTENT_TYPE XSComplexTypeDefinition::getContentType() const
{
    // REVISIT
    return CONTENTTYPE_EMPTY;
}

/**
 * A simple type definition corresponding to simple content model, 
 * otherwise <code>null</code> 
 */
XSSimpleTypeDefinition *XSComplexTypeDefinition::getSimpleType()
{
    // REVISIT
    return 0;
}

/**
 * A particle for mixed or element-only content model, otherwise 
 * <code>null</code> 
 */
XSParticle *XSComplexTypeDefinition::getParticle()
{
    // REVISIT
    return 0;
}

/**
 * [prohibited substitutions]: a subset of {extension, restriction}
 * @param restriction  Extention or restriction constants (see 
 *   <code>XSObject</code>). 
 * @return True if toTest is a prohibited substitution, otherwise 
 *   false.
 */
bool XSComplexTypeDefinition::isProhibitedSubstitution(short toTest)
{
    // REVISIT
    return false;
}

/**
 *  [prohibited substitutions]: A subset of {extension, restriction} or 
 * <code>DERIVATION_NONE</code> represented as a bit flag (see 
 * <code>XSObject</code>). 
 */
short XSComplexTypeDefinition::getProhibitedSubstitutions()
{
    // REVISIT
    return 0;
}

/**
 * A set of [annotations]. 
 */
XSAnnotationList *XSComplexTypeDefinition::getAnnotations()
{
    // REVISIT
    return 0;
}

XERCES_CPP_NAMESPACE_END


