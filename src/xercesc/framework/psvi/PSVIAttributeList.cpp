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
 * Revision 1.5  2003/12/20 06:19:38  neilg
 * store name/namespace of corresponding attribute in PSVIAttributeList; not all PSVIAttributes have XSAttributeDeclarations
 *
 * Revision 1.4  2003/12/15 17:23:48  cargilld
 * psvi updates; cleanup revisits and bug fixes
 *
 * Revision 1.3  2003/11/26 16:20:37  neilg
 * more complete implementation of PSVIAttributeList; remove some problematic const-ness
 *
 * Revision 1.2  2003/11/06 15:30:04  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/PSVIAttributeList.hpp>
#include <xercesc/framework/psvi/XSAttributeDeclaration.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_BEGIN

PSVIAttributeList::PSVIAttributeList( MemoryManager* const manager ):  
        fMemoryManager(manager)
        , fAttrPos(0)
{
    fAttrList= new (fMemoryManager) RefVectorOf<PSVIAttribute> (10, true, fMemoryManager);
    fAttrNameList= new (fMemoryManager) RefArrayVectorOf<XMLCh> (10, false, fMemoryManager);
    fAttrNSList= new (fMemoryManager) RefArrayVectorOf<XMLCh> (10, false, fMemoryManager);
}

/*
 * Get the number of attributes whose PSVI contributions
 * are contained in this list.
 */
const unsigned int PSVIAttributeList::getLength() const
{
    return fAttrPos+1;
}

/*
 * Get the PSVI contribution of attribute at position i
 * in this list.  Indeces start from 0.
 * @param index index from which the attribute PSVI contribution
 * is to come.  
 * @return PSVIAttribute containing the attributes PSVI contributions;
 * null is returned if the index is out of range.
 */
PSVIAttribute *PSVIAttributeList::getAttributePSVIAtIndex(const unsigned int index)
{
    if(index >= fAttrPos)
        return 0;
    return fAttrList->elementAt(index);
}

/*
 * Get local part of attribute name at position index in the list.
 * Indeces start from 0.
 * @param index index from which the attribute name 
 * is to come.  
 * @return local part of the attribute's name; null is returned if the index
 * is out of range.
 */
const XMLCh *PSVIAttributeList::getAttributeNameAtIndex(const unsigned int index)
{
    
    if(index >= fAttrPos)
        return 0;
    return fAttrNameList->elementAt(index);
}

/*
 * Get namespace of attribute at position index in the list.
 * Indeces start from 0.
 * @param index index from which the attribute namespace 
 * is to come.  
 * @return namespace of the attribute; 
 * null is returned if the index is out of range.
 */
const XMLCh *PSVIAttributeList::getAttributeNamespaceAtIndex(const unsigned int index)
{
    if(index >= fAttrPos)
        return 0;
    return fAttrNSList->elementAt(index);
}

/*
 * Get the PSVI contribution of attribute with given 
 * local name and namespace.
 * @param attrName  local part of the attribute's name
 * @param attrNamespace  namespace of the attribute
 * @return null if the attribute PSVI does not exist
 */
PSVIAttribute *PSVIAttributeList::getAttributePSVIByName(const XMLCh *attrName
                , const XMLCh * attrNamespace)
{
    for (unsigned int index=0; index <= fAttrPos; index++) {
        if (XMLString::equals(attrName,fAttrNameList->elementAt(index))
                && XMLString::equals(attrNamespace,fAttrNSList->elementAt(index)))
            return fAttrList->elementAt(index);
    }
    return 0;
}

XERCES_CPP_NAMESPACE_END


