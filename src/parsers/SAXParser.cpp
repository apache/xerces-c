/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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
 * Revision 1.8  2000/04/05 18:56:17  roddey
 * Init the fDTDHandler member. Enable installation of DTDHandler
 * on SAX parser.
 *
 * Revision 1.7  2000/03/03 01:29:34  roddey
 * Added a scanReset()/parseReset() method to the scanner and
 * parsers, to allow for reset after early exit from a progressive parse.
 * Added calls to new Terminate() call to all of the samples. Improved
 * documentation in SAX and DOM parsers.
 *
 * Revision 1.6  2000/03/02 19:54:33  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.5  2000/02/17 03:54:26  rahulj
 * Added some new getters to query the parser state and
 * clarified the documentation.
 *
 * Revision 1.4  2000/02/06 07:47:56  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.3  2000/01/12 00:15:22  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.2  1999/12/15 19:57:48  roddey
 * Got rid of redundant 'const' on boolean return value. Some compilers choke
 * on this and its useless.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:50  twl
 * Initial checkin
 *
 * Revision 1.6  1999/11/08 20:44:53  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/IOException.hpp>
#include <sax/DocumentHandler.hpp>
#include <sax/DTDHandler.hpp>
#include <sax/ErrorHandler.hpp>
#include <sax/EntityResolver.hpp>
#include <sax/SAXParseException.hpp>
#include <internal/XMLScanner.hpp>
#include <parsers/SAXParser.hpp>
#include <validators/DTD/DTDValidator.hpp>


// ---------------------------------------------------------------------------
//  SAXParser: Constructors and Destructor
// ---------------------------------------------------------------------------
SAXParser::SAXParser(XMLValidator* const valToAdopt) :

    fDocHandler(0)
    , fDTDHandler(0)
    , fElemDepth(0)
    , fEntityResolver(0)
    , fErrorHandler(0)
    , fAdvDHCount(0)
    , fAdvDHList(0)
    , fAdvDHListSize(32)
    , fParseInProgress(false)
    , fScanner(0)
    , fValidator(valToAdopt)
{
    // Create the validator if one was not provided.
    if (!fValidator)
        fValidator = new DTDValidator;

    // Create our scanner and tell it what validator to use
    fScanner = new XMLScanner(fValidator);

    // Create the initial advanced handler list array and zero it out
    fAdvDHList = new XMLDocumentHandler*[fAdvDHListSize];
    memset(fAdvDHList, 0, sizeof(void*) * fAdvDHListSize);
}


SAXParser::~SAXParser() 
{
    delete [] fAdvDHList;
    delete fScanner;
    delete fValidator;
}



// ---------------------------------------------------------------------------
//  SAXParser: Advanced document handler list maintenance methods
// ---------------------------------------------------------------------------
void SAXParser::installAdvDocHandler(XMLDocumentHandler* const toInstall)
{
    // See if we need to expand and do so now if needed
    if (fAdvDHCount == fAdvDHListSize)
    {
        // Calc a new size and allocate the new temp buffer
        const unsigned int newSize = (unsigned int)(fAdvDHListSize * 1.5);
        XMLDocumentHandler** newList = new XMLDocumentHandler*[newSize];

        // Copy over the old data to the new list and zero out the rest
        memcpy(newList, fAdvDHList, sizeof(void*) * fAdvDHListSize);
        memset
        (
            &newList[fAdvDHListSize]
            , 0
            , sizeof(void*) * (newSize - fAdvDHListSize)
        );

        // And now clean up the old array and store the new stuff
        delete [] fAdvDHList;
        fAdvDHList = newList;
        fAdvDHListSize = newSize;
    }

    // Add this new guy into the empty slot
    fAdvDHList[fAdvDHCount++] = toInstall;

    //
    //  Install ourself as the document handler with the scanner. We might
    //  already be, but its not worth checking, just do it.
    //
    fScanner->setDocHandler(this);
}


bool SAXParser::removeAdvDocHandler(XMLDocumentHandler* const toRemove)
{
    // If our count is zero, can't be any installed
    if (!fAdvDHCount)
        return false;

    //
    //  Search the array until we find this handler. If we find a null entry
    //  first, we can stop there before the list is kept contiguous.
    //
    unsigned int index;
    for (index = 0; index < fAdvDHCount; index++)
    {
        //
        //  We found it. We have to keep the list contiguous, so we have to
        //  copy down any used elements after this one.
        //
        if (fAdvDHList[index] == toRemove)
        {
            //
            //  Optimize if only one entry (pretty common). Otherwise, we
            //  have to copy them down to compact them.
            //
            if (fAdvDHCount > 1)
            {
                index++;
                while (index < fAdvDHCount)
                    fAdvDHList[index - 1] = fAdvDHList[index];
            }

            // Bump down the count and zero out the last one
            fAdvDHCount--;
            fAdvDHList[fAdvDHCount] = 0;

            //
            //  If this leaves us with no advanced handlers and there is
            //  no SAX doc handler installed on us, then remove us from the
            //  scanner as the document handler.
            //
            if (!fAdvDHCount && !fDocHandler)
                fScanner->setDocHandler(0);

            return true;
        }
    }

    // Never found it
    return false;
}


// ---------------------------------------------------------------------------
//  SAXParser: Setter methods
// ---------------------------------------------------------------------------
void SAXParser::setDoNamespaces(const bool newState)
{
    fScanner->setDoNamespaces(newState);
}


void SAXParser::setDoValidation(const bool newState)
{
    fScanner->setDoValidation(newState);
}


void SAXParser::setExitOnFirstFatalError(const bool newState)
{
    fScanner->setExitOnFirstFatal(newState);
}



// ---------------------------------------------------------------------------
//  SAXParser: Getter methods
// ---------------------------------------------------------------------------
const XMLValidator& SAXParser::getValidator() const
{
    return *fValidator;
}


bool SAXParser::getDoNamespaces() const
{
    return fScanner->getDoNamespaces();
}

bool SAXParser::getDoValidation() const
{
    return fScanner->getDoValidation();
}

bool SAXParser::getExitOnFirstFatalError() const
{
    return fScanner->getExitOnFirstFatal();
}


// ---------------------------------------------------------------------------
//  SAXParser: Overrides of the SAX Parser interface
// ---------------------------------------------------------------------------
void SAXParser::parse(const InputSource& source, const bool reuseValidator)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(source, reuseValidator);
        fParseInProgress = false;
    }

    catch (...)
    {
        fParseInProgress = false;
        throw;
    }
}

void SAXParser::parse(const XMLCh* const systemId, const bool reuseValidator)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(systemId, reuseValidator);
        fParseInProgress = false;
    }

    catch (...)
    {
        fParseInProgress = false;
        throw;
    }
}

void SAXParser::parse(const char* const systemId, const bool reuseValidator)
{
    // Avoid multiple entrance
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    try
    {
        fParseInProgress = true;
        fScanner->scanDocument(systemId, reuseValidator);
        fParseInProgress = false;
    }

    catch (...)
    {
        fParseInProgress = false;
        throw;
    }
}

void SAXParser::setDocumentHandler(DocumentHandler* const handler)
{
    fDocHandler = handler;
    if (fDocHandler)
    {
        //
        //  Make sure we are set as the document handler with the scanner.
        //  We may already be (if advanced handlers are installed), but its
        //  not worthing checking, just do it.
        //
        fScanner->setDocHandler(this);
    }
     else
    {
        //
        //  If we don't have any advanced handlers either, then deinstall us
        //  from the scanner because we don't need document events anymore.
        //
        if (!fAdvDHCount)
            fScanner->setDocHandler(0);
    }
}


void SAXParser::setDTDHandler(DTDHandler* const handler)
{
    //
    //  <TBD>
    //  This gets tricky. We can only set this if its a DTD validator, but it
    //  might not be. We'll have to think about this one once there is more
    //  than a DTD validator available.
    //
    fDTDHandler = handler;
    if (fDTDHandler)
        ((DTDValidator*)fValidator)->setDocTypeHandler(this);
    else
        ((DTDValidator*)fValidator)->setDocTypeHandler(0);
}


void SAXParser::setErrorHandler(ErrorHandler* const handler)
{
    //
    //  Store the handler. Then either install or deinstall us as the
    //  error reporter on the scanner and validator.
    //
    fErrorHandler = handler;
    if (fErrorHandler)
    {
        fScanner->setErrorReporter(this);
        fValidator->setErrorReporter(this);
    }
     else
    {
        fScanner->setErrorReporter(0);
        fValidator->setErrorReporter(0);
    }
}


void SAXParser::setEntityResolver(EntityResolver* const resolver)
{
    fEntityResolver = resolver;
    if (fEntityResolver)
        fScanner->setEntityHandler(this);
    else
        fScanner->setEntityHandler(0);
}



// ---------------------------------------------------------------------------
//  SAXParser: Progressive parse methods
// ---------------------------------------------------------------------------
bool SAXParser::parseFirst( const   XMLCh* const    systemId
                            ,       XMLPScanToken&  toFill
                            , const bool            reuseValidator)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    return fScanner->scanFirst(systemId, toFill, reuseValidator);
}

bool SAXParser::parseFirst( const   char* const     systemId
                            ,       XMLPScanToken&  toFill
                            , const bool            reuseValidator)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    return fScanner->scanFirst(systemId, toFill, reuseValidator);
}

bool SAXParser::parseFirst( const   InputSource&    source
                            ,       XMLPScanToken&  toFill
                            , const bool            reuseValidator)
{
    //
    //  Avoid multiple entrance. We cannot enter here while a regular parse
    //  is in progress.
    //
    if (fParseInProgress)
        ThrowXML(IOException, XMLExcepts::Gen_ParseInProgress);

    return fScanner->scanFirst(source, toFill, reuseValidator);
}

bool SAXParser::parseNext(XMLPScanToken& token)
{
    return fScanner->scanNext(token);
}

void SAXParser::parseReset(XMLPScanToken& token)
{
    // Reset the scanner
    fScanner->scanReset(token);
}


// ---------------------------------------------------------------------------
//  SAXParser: Overrides of the XMLDocumentHandler interface
// ---------------------------------------------------------------------------
void SAXParser::docCharacters(  const   XMLCh* const    chars
                                , const unsigned int    length
                                , const bool            cdataSection)
{
    // SAX don't want to know about chars before content
    if (!fElemDepth)
        return;

    // Just map to the SAX document handler
    if (fDocHandler)
        fDocHandler->characters(chars, length);

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->docCharacters(chars, length, cdataSection);
}


void SAXParser::docComment(const XMLCh* const commentText)
{
    // SAX don't want to know about comments before content
    if (!fElemDepth)
        return;

    //
    //  SAX has no way to report this. But, if there are any installed
    //  advanced handlers, then lets call them with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->docComment(commentText);
}


void SAXParser::XMLDecl(const   XMLCh* const
                        , const XMLCh* const
                        , const XMLCh* const
                        , const XMLCh* const)
{
    // SAX has no way to report this event
}


void SAXParser::docPI(  const   XMLCh* const    target
                        , const XMLCh* const    data)
{
    // SAX don't want to know about PIs before content
    if (!fElemDepth)
        return;

    // Just map to the SAX document handler
    if (fDocHandler)
        fDocHandler->processingInstruction(target, data);

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->docPI(target, data);
}


void SAXParser::endDocument()
{
    if (fDocHandler)
        fDocHandler->endDocument();

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->endDocument();
}


void SAXParser::endElement( const   XMLElementDecl& elemDecl
                            , const unsigned int    uriId
                            , const bool            isRoot)
{
    // Just map to the SAX document handler
    if (fDocHandler)
        fDocHandler->endElement(elemDecl.getFullName());

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->endElement(elemDecl, uriId, isRoot);

    //
    //  Dump the element depth down again. Don't let it underflow in case
    //  of malformed XML.
    //
    if (fElemDepth)
        fElemDepth--;
}


void SAXParser::endEntityReference(const XMLEntityDecl& entityDecl)
{
    //
    //  SAX has no way to report this event. But, if there are any installed
    //  advanced handlers, then lets call them with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->endEntityReference(entityDecl);
}


void SAXParser::ignorableWhitespace(const   XMLCh* const    chars
                                    , const unsigned int    length
                                    , const bool            cdataSection)
{
    // SAX don't want to know about space before content
    if (!fElemDepth)
        return;

    // Just map to the SAX document handler
    if (fDocHandler)
        fDocHandler->ignorableWhitespace(chars, length);

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->ignorableWhitespace(chars, length, cdataSection);
}


void SAXParser::resetDocument()
{
    // Just map to the SAX document handler
    if (fDocHandler)
        fDocHandler->resetDocument();

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->resetDocument();

    // Make sure our element depth flag gets set back to zero
    fElemDepth = 0;
}


void SAXParser::startDocument()
{
    // Just map to the SAX document handler
    if (fDocHandler)
    {
        fDocHandler->setDocumentLocator(fScanner->getLocator());
        fDocHandler->startDocument();
    }

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->startDocument();
}


void SAXParser::
startElement(   const   XMLElementDecl&         elemDecl
                , const unsigned int            elemURLId
                , const XMLCh* const            elemPrefix
                , const RefVectorOf<XMLAttr>&   attrList
                , const unsigned int            attrCount
                , const bool                    isEmpty
                , const bool                    isRoot)
{
    // Bump the element depth counter if not empty
    if (!isEmpty)
        fElemDepth++;

    if (fDocHandler)
    {
        fAttrList.setVector(&attrList, attrCount);
        fDocHandler->startElement(elemDecl.getFullName(), fAttrList);

        // If its empty, send the end tag event now
        if (isEmpty)
            fDocHandler->endElement(elemDecl.getFullName());
    }

    //
    //  If there are any installed advanced handlers, then lets call them
    //  with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
    {
        fAdvDHList[index]->startElement
        (
            elemDecl
            , elemURLId
            , elemPrefix
            , attrList
            , attrCount
            , isEmpty
            , isRoot
        );
    }
}


void SAXParser::startEntityReference(const XMLEntityDecl& entityDecl)
{
    //
    //  SAX has no way to report this. But, If there are any installed
    //  advanced handlers, then lets call them with this info.
    //
    for (unsigned int index = 0; index < fAdvDHCount; index++)
        fAdvDHList[index]->startEntityReference(entityDecl);
}



// ---------------------------------------------------------------------------
//  SAXParser: Overrides of the DocTypeHandler interface
// ---------------------------------------------------------------------------
void SAXParser::attDef( const   DTDElementDecl& elemDecl
                        , const DTDAttDef&      attDef
                        , const bool            ignoring)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::doctypeComment(const XMLCh* const)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::doctypeDecl(const   DTDElementDecl& elemDecl
                            , const XMLCh* const    publicId
                            , const XMLCh* const    systemId
                            , const bool            hasIntSubset)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::doctypePI(  const   XMLCh* const
                            , const XMLCh* const)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::doctypeWhitespace(  const   XMLCh* const    chars
                                    , const unsigned int    length)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::elementDecl(const DTDElementDecl&, const bool)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::endAttList(const DTDElementDecl&)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::endIntSubset()
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::endExtSubset()
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::entityDecl( const   DTDEntityDecl&  entityDecl
                            , const bool            isPEDecl
                            , const bool            isIgnored)
{
    //
    //  If we have a DTD handler, and this entity is not ignored, and
    //  its an unparsed entity, then send this one.
    //
    if (fDTDHandler && !isIgnored)
    {
        if (entityDecl.isUnparsed())
        {
            fDTDHandler->unparsedEntityDecl
            (
                entityDecl.getName()
                , entityDecl.getPublicId()
                , entityDecl.getSystemId()
                , entityDecl.getNotationName()
            );
        }
    }
}


void SAXParser::resetDocType()
{
    // Just map to the DTD handler
    if (fDTDHandler)
        fDTDHandler->resetDocType();
}


void SAXParser::notationDecl(   const   XMLNotationDecl&    notDecl
                                , const bool                isIgnored)
{
    if (fDTDHandler && !isIgnored)
    {
        fDTDHandler->notationDecl
        (
            notDecl.getName()
            , notDecl.getPublicId()
            , notDecl.getSystemId()
        );
    }
}


void SAXParser::startAttList(const DTDElementDecl&)
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::startIntSubset()
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::startExtSubset()
{
    // Unused by SAX DTDHandler interface at this time
}


void SAXParser::TextDecl(   const  XMLCh* const
                            , const XMLCh* const)
{
    // Unused by SAX DTDHandler interface at this time
}


// ---------------------------------------------------------------------------
//  SAXParser: Overrides of the XMLErrorReporter interface
// ---------------------------------------------------------------------------
void SAXParser::resetErrors()
{
}


void SAXParser::error(  const   unsigned int                code
                        , const XMLCh* const                msgDomain
                        , const XMLErrorReporter::ErrTypes  errType
                        , const XMLCh* const                errorText
                        , const XMLCh* const                systemId
                        , const XMLCh* const                publicId
                        , const unsigned int                lineNum
                        , const unsigned int                colNum)
{
    SAXParseException toThrow = SAXParseException
    (
        errorText
        , publicId
        , systemId
        , lineNum
        , colNum
    );

    if (!fErrorHandler)
    {
        if (errType == XMLErrorReporter::ErrType_Fatal)
            throw toThrow;
        else
            return;
    }

    if (errType == XMLErrorReporter::ErrType_Warning)
        fErrorHandler->warning(toThrow);
    else if (errType == XMLErrorReporter::ErrType_Fatal)
        fErrorHandler->fatalError(toThrow);
    else
        fErrorHandler->error(toThrow);
}



// ---------------------------------------------------------------------------
//  SAXParser: Handlers for the XMLEntityHandler interface
// ---------------------------------------------------------------------------
void SAXParser::endInputSource(const InputSource&)
{
}

bool SAXParser::expandSystemId(const XMLCh* const, XMLBuffer&)
{
    return false;
}


void SAXParser::resetEntities()
{
    // Nothing to do for this one
}


InputSource*
SAXParser::resolveEntity(   const   XMLCh* const    publicId
                            , const XMLCh* const    systemId)
{
    // Just map to the SAX entity resolver handler
    if (fEntityResolver)
        return fEntityResolver->resolveEntity(publicId, systemId);
    return 0;
}


void SAXParser::startInputSource(const InputSource&)
{
    // Nothing to do for this one
}
