/*
 * Copyright 2001,2004 The Apache Software Foundation.
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
 * Revision 1.9  2004/09/08 13:56:58  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2003/11/20 17:06:05  knoaman
 * PSVI: store name and namespace information
 *
 * Revision 1.7  2003/11/13 23:20:47  peiyongz
 * initSize
 *
 * Revision 1.6  2003/10/17 21:17:12  peiyongz
 * using XTemplateSerializer
 *
 * Revision 1.5  2003/10/14 15:22:28  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.4  2003/05/15 18:57:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/11/04 14:49:42  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/09/24 20:12:48  tng
 * Performance: use XMLString::equals instead of XMLString::compareString
 *
 * Revision 1.1.1.1  2002/02/01 22:22:50  peiyongz
 * sane_include
 *
 * Revision 1.4  2001/11/20 20:21:49  knoaman
 * Add check for attribute derviation ok when redefining attribute group by restriction.
 *
 * Revision 1.3  2001/11/20 15:57:04  knoaman
 * Add check for multiple attributes with type derived from ID.
 *
 * Revision 1.2  2001/08/09 15:23:17  knoaman
 * add support for <anyAttribute> declaration.
 *
 * Revision 1.1  2001/07/31 15:26:54  knoaman
 * Added support for <attributeGroup>.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/XercesAttGroupInfo.hpp>
#include <xercesc/util/QName.hpp>

#include <xercesc/internal/XTemplateSerializer.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XercesAttGroupInfo: Constructors and Destructor
// ---------------------------------------------------------------------------
XercesAttGroupInfo::XercesAttGroupInfo(MemoryManager* const manager)
    : fTypeWithId(false)
    , fNameId(0)
    , fNamespaceId(0)
    , fAttributes(0)
    , fAnyAttributes(0)
    , fCompleteWildCard(0)
    , fMemoryManager(manager)
{

}

XercesAttGroupInfo::XercesAttGroupInfo(unsigned int attGroupNameId,
                                       unsigned int attGroupNamespaceId,
                                       MemoryManager* const manager)
    : fTypeWithId(false)
    , fNameId(attGroupNameId)
    , fNamespaceId(attGroupNamespaceId)
    , fAttributes(0)
    , fAnyAttributes(0)
    , fCompleteWildCard(0)
    , fMemoryManager(manager)
{

}

XercesAttGroupInfo::~XercesAttGroupInfo()
{
    delete fAttributes;
    delete fAnyAttributes;
    delete fCompleteWildCard;
}

bool XercesAttGroupInfo::containsAttribute(const XMLCh* const name,
                                           const unsigned int uri) {

    if (fAttributes) {

        unsigned int attCount = fAttributes->size();

        if (attCount) {

            for (unsigned int i=0; i < attCount; i++) {

                QName* attName = fAttributes->elementAt(i)->getAttName();

                if (attName->getURI() == uri &&
                    XMLString::equals(attName->getLocalPart(),name)) {
                    return true;
                }
            }
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
//  XercesAttGroupInfo: Getter methods
// ---------------------------------------------------------------------------
const SchemaAttDef* XercesAttGroupInfo::getAttDef(const XMLCh* const baseName,
                                                  const int uriId) const {

    // If no list, then return a null
    if (!fAttributes)
        return 0;

    unsigned int attSize = fAttributes->size();

    for (unsigned int i=0; i<attSize; i++) {

        const SchemaAttDef* attDef = fAttributes->elementAt(i);
        QName* attName = attDef->getAttName();

        if (uriId == (int) attName->getURI() &&
			XMLString::equals(baseName, attName->getLocalPart())) {

            return attDef;
        }
    }

    return 0;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(XercesAttGroupInfo)

void XercesAttGroupInfo::serialize(XSerializeEngine& serEng)
{

    if (serEng.isStoring())
    {
        serEng<<fTypeWithId;
        serEng<<fNameId;
        serEng<<fNamespaceId;

        /***
         *
         * Serialize RefVectorOf<SchemaAttDef>* fAttributes;
         *
         ***/
        XTemplateSerializer::storeObject(fAttributes, serEng);

        /***
         *
         * Serialize RefVectorOf<SchemaAttDef>* fAnyAttributes;
         *
         ***/
        XTemplateSerializer::storeObject(fAnyAttributes, serEng);

        serEng<<fCompleteWildCard;
    }
    else
    {
        serEng>>fTypeWithId;
        serEng>>fNameId;
        serEng>>fNamespaceId;

        /***
         *
         * Deserialize RefVectorOf<SchemaAttDef>* fAttributes;
         *
         ***/
        XTemplateSerializer::loadObject(&fAttributes, 4, true, serEng);

        /***
         *
         * Deserialize RefVectorOf<SchemaAttDef>* fAnyAttributes;
         *
         ***/

        XTemplateSerializer::loadObject(&fAnyAttributes, 2, true, serEng);

        serEng>>fCompleteWildCard;
    }

}

XERCES_CPP_NAMESPACE_END

/**
  * End of file XercesAttGroupInfo.cpp
  */


