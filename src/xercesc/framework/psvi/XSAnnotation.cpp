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
 * Revision 1.11  2004/09/28 02:14:13  cargilld
 * Add support for validating annotations.
 *
 * Revision 1.10  2004/09/08 13:56:07  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.9  2004/04/02 16:51:05  peiyongz
 * Better error report support
 *
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

XSAnnotation::XSAnnotation(const XMLCh*          const content,
                                 MemoryManager * const manager)
:XSObject(XSConstants::ANNOTATION, 0, manager)
,fContents(XMLString::replicate(content, manager))
,fNext(0)
,fSystemId(0)
,fLine(0)
,fCol(0)
{
}

XSAnnotation::XSAnnotation(MemoryManager * const manager)
:XSObject(XSConstants::ANNOTATION, 0, manager)
,fContents(0)
,fNext(0)
,fSystemId(0)
,fLine(0)
,fCol(0)
{
}

XSAnnotation::~XSAnnotation()
{
    fMemoryManager->deallocate(fContents);

    if (fNext)
        delete fNext;

    fMemoryManager->deallocate(fSystemId);
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
    memBufIS->setCopyBufToStream(false);

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
    memBufIS->setCopyBufToStream(false);

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

void XSAnnotation::setSystemId(const XMLCh* const systemId)
{
    if (fSystemId)
    {
        fMemoryManager->deallocate(fSystemId);
        fSystemId = 0;
    }

    if (systemId)
        fSystemId = XMLString::replicate(systemId, fMemoryManager);
    
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(XSAnnotation)

void XSAnnotation::serialize(XSerializeEngine& serEng)
{

    if (serEng.isStoring())
    {
        serEng.writeString(fContents);
        serEng<<fNext;
        serEng.writeString(fSystemId);
        serEng<<fLine;
        serEng<<fCol;
    }
    else
    {
        serEng.readString(fContents);
        serEng>>fNext;
        serEng.readString(fSystemId);
        serEng>>fLine;
        serEng>>fCol;
    }
}

XERCES_CPP_NAMESPACE_END


