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
 * Revision 1.3  2003/11/11 22:48:13  knoaman
 * Serialization of XSAnnotation.
 *
 * Revision 1.2  2003/11/06 19:28:11  knoaman
 * PSVI support for annotations.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSAnnotation.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_BEGIN

XSAnnotation::XSAnnotation(const XMLCh* const content,
                           MemoryManager * const manager):
    XSObject(XSConstants::ANNOTATION, manager)
    , fContents(XMLString::replicate(content, manager))
    , fNext(0)
{
}

XSAnnotation::XSAnnotation(MemoryManager * const manager):
    XSObject(XSConstants::ANNOTATION, manager)
    , fContents(0)
    , fNext(0)
{
}

XSAnnotation::~XSAnnotation()
{
    fMemoryManager->deallocate(fContents);

    if (fNext)
        delete fNext;
}

// XSAnnotation methods

/**
 *  Write contents of the annotation to the specified object. If the 
 * specified <code>target</code> object is a DOM in-scope namespace 
 * declarations for <code>annotation</code> element are added as 
 * attribute nodes of the serialized <code>annotation</code>, otherwise 
 * the corresponding events for all in-scope namespace declaration are 
 * sent via the specified document handler. 
 * @param target  A target pointer to the annotation target object, i.e. 
 *   <code>DOMDocument</code>, 
 *   <code>DOMElement</code>, 
 *   <code>ContentHandler</code>. 
 * @param targetType  A target type. 
 * @return If the <code>target</code> is a recognized type that is supported by 
 *   this implementation, and the operation succeeds, return true; otherwise return false. 
 */
bool XSAnnotation::writeAnnotation(void *target, 
                               ANNOTATION_TARGET targetType)
{
    // REVISIT
    return false;
}

void XSAnnotation::setNext(XSAnnotation* const nextAnnotation)
{
    if (fNext)
        fNext->setNext(nextAnnotation);
    else
        fNext = nextAnnotation;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(XSAnnotation)

void XSAnnotation::serialize(XSerializeEngine& serEng)
{
    /***
     * Since we are pretty sure that fIdMap and fHashTable is 
     * not shared by any other object, therefore there is no owned/referenced
     * issue. Thus we can serialize the raw data only, rather than serializing 
     * both fIdMap and fHashTable.
     *
     * And we can rebuild the fIdMap and fHashTable out of the raw data during
     * deserialization.
     *
    ***/
    if (serEng.isStoring())
    {
        serEng.writeString(fContents);
        serEng<<fNext;
    }
    else
    {
        serEng.readString(fContents);
        serEng>>fNext;
    }
}

XERCES_CPP_NAMESPACE_END


