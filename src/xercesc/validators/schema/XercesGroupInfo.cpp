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
 * Revision 1.12  2004/09/08 13:56:58  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.11  2003/11/20 17:06:05  knoaman
 * PSVI: store name and namespace information
 *
 * Revision 1.10  2003/11/13 23:20:47  peiyongz
 * initSize
 *
 * Revision 1.9  2003/10/29 16:21:52  peiyongz
 * don't own the element list pointed to by fElements
 *
 * Revision 1.8  2003/10/17 21:17:12  peiyongz
 * using XTemplateSerializer
 *
 * Revision 1.7  2003/10/14 15:22:28  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.6  2003/05/18 14:02:08  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.5  2003/05/15 18:57:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2002/11/04 14:49:42  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/04/01 15:47:06  knoaman
 * Move Element Consistency checking (ref to global declarations) to SchemaValidator.
 *
 * Revision 1.2  2002/03/25 20:25:32  knoaman
 * Move particle derivation checking from TraverseSchema to SchemaValidator.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:50  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/08/24 20:36:37  knoaman
 * Add support for <redefine>.
 *
 * Revision 1.1  2001/07/24 18:33:46  knoaman
 * Added support for <group> + extra constraint checking for complexType
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/XercesGroupInfo.hpp>
#include <xercesc/validators/common/ContentSpecNode.hpp>
#include <xercesc/validators/schema/XSDLocator.hpp>

#include <xercesc/internal/XTemplateSerializer.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XercesGroupInfo: Constructors and Destructor
// ---------------------------------------------------------------------------
XercesGroupInfo::XercesGroupInfo(MemoryManager* const manager)
    : fCheckElementConsistency(true)
    , fScope(-1)
    , fNameId(0)
    , fNamespaceId(0)
    , fContentSpec(0)
    , fElements(0)
    , fBaseGroup(0)
    , fLocator(0)
{
    fElements = new (manager) RefVectorOf<SchemaElementDecl>(4, false, manager);
}

XercesGroupInfo::XercesGroupInfo(unsigned int groupNameId,
                                 unsigned int groupNamespaceId,
                                 MemoryManager* const manager)
    : fCheckElementConsistency(true)
    , fScope(-1)
    , fNameId(groupNameId)
    , fNamespaceId(groupNamespaceId)
    , fContentSpec(0)
    , fElements(0)
    , fBaseGroup(0)
    , fLocator(0)
{
    fElements = new (manager) RefVectorOf<SchemaElementDecl>(4, false, manager);
}


XercesGroupInfo::~XercesGroupInfo()
{
    delete fElements;
    delete fContentSpec;
    delete fLocator;
}


// ---------------------------------------------------------------------------
//  XercesGroupInfo: Constructors and Destructor
// ---------------------------------------------------------------------------
void XercesGroupInfo::setLocator(XSDLocator* const aLocator) {

    if (fLocator)
        delete fLocator;

    fLocator = aLocator;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(XercesGroupInfo)

void XercesGroupInfo::serialize(XSerializeEngine& serEng)
{
    if (serEng.isStoring())
    {   
        serEng<<fCheckElementConsistency;
        serEng<<fScope;
        serEng<<fNameId;
        serEng<<fNamespaceId;
        serEng<<fContentSpec;

        /***
         *
         * Serialize RefVectorOf<SchemaElementDecl>* fElements;
         *
         ***/

        XTemplateSerializer::storeObject(fElements, serEng);

        serEng<<fBaseGroup;

        //don't serialize     XSDLocator* fLocator;
    }
    else
    {
        serEng>>fCheckElementConsistency;
        serEng>>fScope;
        serEng>>fNameId;
        serEng>>fNamespaceId;
        serEng>>fContentSpec;

        /***
         * 
         * Deserialize RefVectorOf<SchemaElementDecl>*    fElements;
         *
         ***/
        XTemplateSerializer::loadObject(&fElements, 4, false, serEng);

        serEng>>fBaseGroup;

        //don't serialize     XSDLocator* fLocator;
        fLocator = 0;
    }

}

XERCES_CPP_NAMESPACE_END

/**
  * End of file XercesGroupInfo.cpp
  */


