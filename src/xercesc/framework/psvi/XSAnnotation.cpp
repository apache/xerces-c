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
 * Revision 1.8  2003/12/19 07:18:56  neilg
 * remove a throw clause inserted during debugging (but should we really swallow this exception?)
 *
 * Revision 1.7  2003/12/15 19:04:55  neilg
 * fix segfault when a writeAnnotation() method was called
 *
 * Revision 1.6  2003/11/27 21:29:05  neilg
 * implement writeAnnotation; thanks to Dave Cargill
 *
 * Revision 1.5  2003/11/14 22:47:53  neilg
 * fix bogus log message from previous commit...
 *
 * Revision 1.4  2003/11/14 22:33:30  neilg
 * Second phase of schema component model implementation.  
 * Implement XSModel, XSNamespaceItem, and the plumbing necessary
 * to connect them to the other components.
 * Thanks to David Cargill.
 *
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

#include <xercesc/framework/MemBufInputSource.hpp>

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMElement.hpp>

XERCES_CPP_NAMESPACE_BEGIN

XSAnnotation::XSAnnotation(const XMLCh* const content,
                           MemoryManager * const manager):
    XSObject(XSConstants::ANNOTATION, 0, manager)
    , fContents(XMLString::replicate(content, manager))
    , fNext(0)
{
}

XSAnnotation::XSAnnotation(MemoryManager * const manager):
    XSObject(XSConstants::ANNOTATION, 0, manager)
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
void XSAnnotation::writeAnnotation(DOMNode* node, ANNOTATION_TARGET targetType)
{
    XercesDOMParser *parser = new (fMemoryManager) XercesDOMParser(0, fMemoryManager);
    parser->setDoNamespaces(true);
    parser->setValidationScheme(XercesDOMParser::Val_Never);
    
    DOMDocument* futureOwner = (targetType == W3C_DOM_ELEMENT) ?
        ((DOMElement*)node)->getOwnerDocument() :
        (DOMDocument*)node;

    MemBufInputSource* memBufIS = new (fMemoryManager) MemBufInputSource
    (
        (const XMLByte*)fContents
        , XMLString::stringLen(fContents)*sizeof(XMLCh)
        , ""
        , false
        , fMemoryManager
    );
    memBufIS->setEncoding(XMLUni::fgXMLChEncodingString);

    try
    {        
        parser->parse(*memBufIS);
    }
    catch (const XMLException&)
    {
        // REVISIT:  should we really eat this?
    }

    DOMNode* newElem = futureOwner->importNode((parser->getDocument())->getDocumentElement(), true);
    node->insertBefore(newElem, node->getFirstChild());

    delete parser;
    delete memBufIS;
}


void XSAnnotation::writeAnnotation(ContentHandler* handler)
{   
    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader(fMemoryManager);
    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
    parser->setFeature(XMLUni::fgSAX2CoreValidation, false);
    parser->setContentHandler(handler);
    
    MemBufInputSource* memBufIS = new (fMemoryManager) MemBufInputSource
    (
        (const XMLByte*)fContents
        , XMLString::stringLen(fContents)*sizeof(XMLCh)
        , ""
        , false
        , fMemoryManager
    );
    memBufIS->setEncoding(XMLUni::fgXMLChEncodingString);

    try
    {        
        parser->parse(*memBufIS);
    }
    catch (const XMLException&)
    {
    }

    delete parser;
    delete memBufIS;
}


void XSAnnotation::setNext(XSAnnotation* const nextAnnotation)
{
    if (fNext)
        fNext->setNext(nextAnnotation);
    else
        fNext = nextAnnotation;
}

XSAnnotation* XSAnnotation::getNext()
{
    return fNext;
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


