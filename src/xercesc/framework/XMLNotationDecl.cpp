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

/**
 * $Log$
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
    , fName(0)
    , fPublicId(0)
    , fSystemId(0)
    , fBaseURI(0)
    , fNameSpaceId(0)
    , fMemoryManager(manager)
{
}

XMLNotationDecl::XMLNotationDecl( const XMLCh* const   notName
                                , const XMLCh* const   pubId
                                , const XMLCh* const   sysId
                                , const XMLCh* const   baseURI
                                , MemoryManager* const manager) :
    fId(0)
    , fName(0)
    , fPublicId(0)
    , fSystemId(0)
    , fBaseURI(0)
    , fNameSpaceId(0)
    , fMemoryManager(manager)
{
    try
    {
        fName = XMLString::replicate(notName, fMemoryManager);
        fPublicId = XMLString::replicate(pubId, fMemoryManager);
        fSystemId = XMLString::replicate(sysId, fMemoryManager);
        fBaseURI  = XMLString::replicate(baseURI, fMemoryManager);
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...)
    {
        cleanUp();
    }
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
        serEng.writeString(fName);
        serEng.writeString(fPublicId);
        serEng.writeString(fSystemId);
        serEng.writeString(fBaseURI);
    }
    else
    {
        serEng>>fId;
        serEng.readString(fName);
        serEng.readString(fPublicId);
        serEng.readString(fSystemId);
        serEng.readString(fBaseURI);
    }
}


XERCES_CPP_NAMESPACE_END
