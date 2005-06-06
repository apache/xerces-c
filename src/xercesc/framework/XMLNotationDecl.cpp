/*
 * Copyright 1999-2004 The Apache Software Foundation.
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

/**
 * $Log$
 * Revision 1.12  2004/10/28 20:10:50  peiyongz
 * Data member reshuffle and store/load fNameScopeId
 *
 * Revision 1.11  2004/09/08 13:55:59  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.10  2003/11/21 22:34:46  neilg
 * More schema component model implementation, thanks to David Cargill.
 * In particular, this cleans up and completes the XSModel, XSNamespaceItem,
 * XSAttributeDeclaration and XSAttributeGroup implementations.
 *
 * Revision 1.9  2003/11/06 15:30:06  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.8  2003/10/10 16:23:29  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.7  2003/10/01 16:32:38  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.6  2003/05/16 21:36:55  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.5  2003/05/15 18:26:07  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2003/04/21 20:46:01  knoaman
 * Use XMLString::release to prepare for configurable memory manager.
 *
 * Revision 1.3  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/08/22 19:27:41  tng
 * [Bug 11448] DomCount has problems with XHTML1.1 DTD.
 *
 * Revision 1.1.1.1  2002/02/01 22:21:52  peiyongz
 * sane_include
 *
 * Revision 1.2  2000/02/06 07:47:48  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:35  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:39  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/XMLNotationDecl.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLNotationDecl: Constructors and operators
// ---------------------------------------------------------------------------
XMLNotationDecl::XMLNotationDecl(MemoryManager* const manager) :

    fId(0)
    , fNameSpaceId(0)
    , fName(0)
    , fPublicId(0)
    , fSystemId(0)
    , fBaseURI(0)
    , fMemoryManager(manager)
{
}

typedef JanitorMemFunCall<XMLNotationDecl>  CleanupType;

XMLNotationDecl::XMLNotationDecl( const XMLCh* const   notName
                                , const XMLCh* const   pubId
                                , const XMLCh* const   sysId
                                , const XMLCh* const   baseURI
                                , MemoryManager* const manager) :
    fId(0)
    , fNameSpaceId(0)
    , fName(0)
    , fPublicId(0)
    , fSystemId(0)
    , fBaseURI(0)
    , fMemoryManager(manager)
{
    CleanupType cleanup(this, &XMLNotationDecl::cleanUp);

    try
    {
        fName = XMLString::replicate(notName, fMemoryManager);
        fPublicId = XMLString::replicate(pubId, fMemoryManager);
        fSystemId = XMLString::replicate(sysId, fMemoryManager);
        fBaseURI  = XMLString::replicate(baseURI, fMemoryManager);
    }
    catch(const OutOfMemoryException&)
    {
        cleanup.release();

        throw;
    }

    cleanup.release();
}

XMLNotationDecl::~XMLNotationDecl()
{
    cleanUp();
}


// -----------------------------------------------------------------------
//  Setter methods
// -----------------------------------------------------------------------
void XMLNotationDecl::setName(const XMLCh* const notName)
{
    // Clean up the current name stuff and replicate the passed name
    if (fName)
        fMemoryManager->deallocate(fName);

    fName = XMLString::replicate(notName, fMemoryManager);
}



// ---------------------------------------------------------------------------
//  XMLNotationDecl: Private helper methods
// ---------------------------------------------------------------------------
void XMLNotationDecl::cleanUp()
{
    fMemoryManager->deallocate(fName);
    fMemoryManager->deallocate(fPublicId);
    fMemoryManager->deallocate(fSystemId);
    fMemoryManager->deallocate(fBaseURI);
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(XMLNotationDecl)

void XMLNotationDecl::serialize(XSerializeEngine& serEng)
{
    if (serEng.isStoring())
    {
        serEng<<fId;
        serEng<<fNameSpaceId;
        serEng.writeString(fName);
        serEng.writeString(fPublicId);
        serEng.writeString(fSystemId);
        serEng.writeString(fBaseURI);
    }
    else
    {
        serEng>>fId;
        serEng>>fNameSpaceId;
        serEng.readString(fName);
        serEng.readString(fPublicId);
        serEng.readString(fSystemId);
        serEng.readString(fBaseURI);
    }
}


XERCES_CPP_NAMESPACE_END
