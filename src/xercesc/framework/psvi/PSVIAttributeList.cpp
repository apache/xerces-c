/*
 * Copyright 2003,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Log$
 * Revision 1.8  2004/09/08 13:56:07  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.7  2004/02/15 19:37:16  amassari
 * Removed cause for warnings in VC 7.1
 *
 * Revision 1.6  2003/12/30 16:48:16  neilg
 * some indices in the PSVIAttributeList were 1 off
 *
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
unsigned int PSVIAttributeList::getLength() const
{
    return fAttrPos;
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
    for (unsigned int index=0; index < fAttrPos; index++) {
        if (XMLString::equals(attrName,fAttrNameList->elementAt(index))
                && XMLString::equals(attrNamespace,fAttrNSList->elementAt(index)))
            return fAttrList->elementAt(index);
    }
    return 0;
}

XERCES_CPP_NAMESPACE_END


