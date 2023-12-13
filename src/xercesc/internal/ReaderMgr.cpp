/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
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
 * $Id$
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/BinMemInputStream.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/util/UnexpectedEOFException.hpp>
#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMLUri.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/URLInputSource.hpp>
#include <xercesc/framework/XMLBuffer.hpp>
#include <xercesc/framework/XMLDocumentHandler.hpp>
#include <xercesc/framework/XMLEntityDecl.hpp>
#include <xercesc/framework/XMLEntityHandler.hpp>
#include <xercesc/internal/EndOfEntityException.hpp>
#include <xercesc/internal/ReaderMgr.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/util/XMLResourceIdentifier.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  ReaderMgr::ReaderData: Constructors and Destructor
// ---------------------------------------------------------------------------
ReaderMgr::ReaderData::ReaderData( XMLReader* const       reader
                                   , XMLEntityDecl* const entity
                                   , const bool           adoptEntity) :

    fReader(reader)
    , fEntity(entity)
    , fEntityAdopted(adoptEntity)
{
}

ReaderMgr::ReaderData::~ReaderData()
{
    delete fReader;

    if (fEntityAdopted)
        delete fEntity;
}

// ---------------------------------------------------------------------------
//  ReaderMgr::ReaderData: Getter methods
// ---------------------------------------------------------------------------
XMLReader* ReaderMgr::ReaderData::getReader() const
{
  return fReader;
}

XMLEntityDecl* ReaderMgr::ReaderData::getEntity() const
{
  return fEntity;
}

bool ReaderMgr::ReaderData::getEntityAdopted() const
{
  return fEntityAdopted;
}

//
//  This method gives up the entity object ownership but leaves the fEntity
//  pointer intact.
//
XMLEntityDecl* ReaderMgr::ReaderData::releaseEntity()
{
  fEntityAdopted = false;
  return fEntity;
}

// ---------------------------------------------------------------------------
//  ReaderMgr: Constructors and Destructor
// ---------------------------------------------------------------------------
ReaderMgr::ReaderMgr(MemoryManager* const manager) :

    fCurReaderData(0)
    , fCurReader(0)
    , fEntityHandler(0)
    , fEntityStack(0)
    , fNextReaderNum(1)
    , fReaderStack(0)
    , fThrowEOE(false)
    , fXMLVersion(XMLReader::XMLV1_0)
    , fStandardUriConformant(false)
    , fMemoryManager(manager)
{
}

ReaderMgr::~ReaderMgr()
{
    //
    //  Clean up the reader stack and orphan entities container. Note that
    //  all adopted entities (potentially contained in fCurReaderData,
    //  fReaderStack, and fEntityStack) are deleted here.
    //
    delete fCurReaderData;
    delete fReaderStack;
    delete fEntityStack;
}


// ---------------------------------------------------------------------------
//  ReaderMgr: Getter methods
// ---------------------------------------------------------------------------
bool ReaderMgr::isEmpty() const
{
    return fReaderStack->empty();
}


// ---------------------------------------------------------------------------
//  ReaderMgr: Scanning APIs
// ---------------------------------------------------------------------------
XMLCh ReaderMgr::getNextChar()
{
    XMLCh chRet;
    if (fCurReader->getNextChar(chRet))
        return chRet;

    //
    //  Didn't get anything back so this reader is hosed. So lets move to
    //  the next reader on the stack. If this fails, it will be because
    //  its the end of the original file, and we just return zero.
    //
    //  If its the end of an entity and fThrowEOE is set, it will throw out
    //  of here. Otherwise, it will take us down to the next reader and
    //  we'll have more chars.
    //
    if (!popReader())
        return XMLCh(0);

    // Else try again and return the new character
    fCurReader->getNextChar(chRet);
    return chRet;
}


void ReaderMgr::getSpaces(XMLBuffer& toFill)
{
    // Reset the buffer before we start
    toFill.reset();

    //
    //  Get all the spaces from the current reader. If it returns true,
    //  it hit a non-space and we are done. Else we have to pop a reader
    //  and keep going.
    //
    while (!fCurReader->getSpaces(toFill))
    {
        // We wore that one out, so lets pop a reader and try again
        if (!popReader())
            break;
    }
}


void ReaderMgr::getUpToCharOrWS(XMLBuffer& toFill, const XMLCh toCheck)
{
    // Reset the target buffer before we start
    toFill.reset();

    //
    //  Ok, enter a loop where we ask the current reader to get chars until
    //  it meets the criteria. It returns false if it came back due to eating
    //  up all of its data. Else it returned because something matched, and
    //  we are done.
    //
    while (!fCurReader->getUpToCharOrWS(toFill, toCheck))
    {
        // We ate that one up, lets try to pop another. If not, break out
        if (!popReader())
            break;
    }
}


XMLCh ReaderMgr::peekNextChar()
{
    XMLCh chRet;
    if (fCurReader->peekNextChar(chRet))
        return chRet;

    //
    //  Didn't get anything back so this reader is hosed. So lets move to
    //  the next reader on the stack. If this fails, it will be because
    //  its the end of the original file, and we just return zero.
    //
    if (!popReader())
        return XMLCh(0);

    // Else peek again and return the character
    fCurReader->peekNextChar(chRet);
    return chRet;
}


bool ReaderMgr::skippedChar(const XMLCh toCheck)
{
    while (true)
    {
        // If we get it, then just return true now
        if (fCurReader->skippedChar(toCheck))
            return true;

        //
        //  Check to see if we hit end of input on this reader. If so, then
        //  lets pop and try again. Else, we failed. If we cannot pop another
        //  then we failed.
        //
        if (!fCurReader->getNoMoreFlag())
            break;

        if (!popReader())
            break;
    }
    return false;
}


bool ReaderMgr::skippedSpace()
{
    while (true)
    {
        // If we get it, then just return true now
        if (fCurReader->skippedSpace())
            return true;

        //
        //  Check to see if we hit end of input on this reader. If so, then
        //  lets pop and try again. Else, we failed. If we cannot pop another
        //  then we failed.
        //
        if (!fCurReader->getNoMoreFlag())
            break;

        if (!popReader())
            break;
    }
    return false;
}


bool ReaderMgr::skipIfQuote(XMLCh& chGotten)
{
    while (true)
    {
        // If we get it, then just return true now
        if (fCurReader->skipIfQuote(chGotten))
            return true;

        //
        //  Check to see if we hit end of input on this reader. If so, then
        //  lets pop and try again. Else, we failed. If we cannot pop another
        //  then we failed.
        //
        if (!fCurReader->getNoMoreFlag())
            break;

        if (!popReader())
            break;
    }
    return false;
}

void ReaderMgr::skipPastSpaces(bool& skippedSomething, bool inDecl /* = false */)
{
    // we rely on the fact that fCurReader->skipSpaces will NOT reset the flag to false, but only
    // set it to true if a space is found
    skippedSomething = false;
    //
    //  Skip all the spaces in the current reader. If it returned because
    //  it hit a non-space, break out. Else we have to pop another entity
    //  and keep going.
    //
    while (!fCurReader->skipSpaces(skippedSomething, inDecl))
    {
        // Try to pop another entity. If we can't then we are done
        if (!popReader())
            break;
    }
}

void ReaderMgr::skipPastSpaces()
{
    // we are not using it, so we don't care to initialize it
    bool tmpFlag;
    //
    //  Skip all the spaces in the current reader. If it returned because
    //  it hit a non-space, break out. Else we have to pop another entity
    //  and keep going.
    //
    while (!fCurReader->skipSpaces(tmpFlag, false))
    {
        // Try to pop another entity. If we can't then we are done
        if (!popReader())
            break;
    }
}

void ReaderMgr::skipQuotedString(const XMLCh quoteCh)
{
    XMLCh nextCh;
    // If we get an end of file char, then return
    while ((nextCh = getNextChar())!=0)
    {
        // If we get the quote char, then break out
        if (nextCh == quoteCh)
            break;
    }
}


XMLCh ReaderMgr::skipUntilIn(const XMLCh* const listToSkip)
{
    XMLCh nextCh;
    // If we get an end of file char, then return
    while ((nextCh = peekNextChar())!=0)
    {
        if (XMLString::indexOf(listToSkip, nextCh) != -1)
            break;

        // Its one of ours so eat it
        getNextChar();
    }
    return nextCh;
}


XMLCh ReaderMgr::skipUntilInOrWS(const XMLCh* const listToSkip)
{
    XMLCh nextCh;
    // If we get an end of file char, then return
    while ((nextCh = peekNextChar())!=0)
    {
        if (fCurReader->isWhitespace(nextCh))
            break;

        if (XMLString::indexOf(listToSkip, nextCh) != -1)
            break;

        // Its one of ours, so eat it
        getNextChar();
    }
    return nextCh;
}



// ---------------------------------------------------------------------------
//  ReaderMgr: Control methods
// ---------------------------------------------------------------------------

//
//  If the reader stack is empty, then there is only the original main XML
//  entity left. If its empty, then we have no more input.
//
bool ReaderMgr::atEOF() const
{
    return fReaderStack->empty() && fCurReader->getNoMoreFlag();
}


//
//  This method is called in the case of errors to clean up the stack when
//  entities have been incorrectly left on the stack due to syntax errors.
//  It just cleans back the stack, and sends no entity events.
//
void ReaderMgr::cleanStackBackTo(const XMLSize_t readerNum)
{
    //
    //  Just start popping readers until we find the one with the indicated
    //  reader number.
    //
    while (true)
    {
        if (fCurReader->getReaderNum() == readerNum)
            break;

        if (fReaderStack->empty())
            ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::RdrMgr_ReaderIdNotFound, fMemoryManager);

        delete fCurReaderData;
        fCurReaderData = fReaderStack->pop();
        fCurReader = fCurReaderData->getReader ();
    }
}


XMLReader* ReaderMgr::createReader( const   InputSource&        src
                                    , const bool
                                    , const XMLReader::RefFrom  refFrom
                                    , const XMLReader::Types    type
                                    , const XMLReader::Sources  source
                                    , const bool                calcSrcOfs
                                    ,       XMLSize_t           lowWaterMark)
{
    //
    //  Ask the input source to create us an input stream. The particular
    //  type of input source will know what kind to create.
    //
    BinInputStream* newStream = src.makeStream();
    if (!newStream)
        return 0;

    Janitor<BinInputStream>   streamJanitor(newStream);

    //
    //  Create a new reader and return it. If the source has an encoding that
    //  it wants to force, then we call the constructor that does that.
    //  Otherwise, we just call the one that provides the provisional encoding
    //  to be possibly updated later by the encoding="" setting.
    //
    XMLReader* retVal = 0;

    // XMLReader ctor invokes refreshRawBuffer() which calls
    // newStream->readBytes().
    // This readBytes() may throw exception, which neither
    // refresRawBuffer(), nor XMLReader ctor catches.
    // We need to handle this exception to avoid leak on newStream.

    try {
		const XMLCh* encoding = src.getEncoding();
		if(encoding == 0)
			encoding = newStream->getEncoding();
        if (encoding)
        {
            retVal = new (fMemoryManager) XMLReader
                (
                src.getPublicId()
                , src.getSystemId()
                , newStream
                , encoding
                , refFrom
                , type
                , source
                , false
                , calcSrcOfs
                , lowWaterMark
                , fXMLVersion
                , fMemoryManager
                );
        }
        else
        {
            retVal = new (fMemoryManager) XMLReader
                (
                src.getPublicId()
                , src.getSystemId()
                , newStream
                , refFrom
                , type
                , source
                , false
                , calcSrcOfs
                , lowWaterMark
                , fXMLVersion
                , fMemoryManager
                );
        }
    }
    catch(const OutOfMemoryException&)
    {
        streamJanitor.release();

        throw;
    }

    assert(retVal);

    streamJanitor.release();

    // Set the next available reader number on this reader
    retVal->setReaderNum(fNextReaderNum++);
    return retVal;
}


XMLReader* ReaderMgr::createReader( const   XMLCh* const        sysId
                                    , const XMLCh* const        pubId
                                    , const bool                xmlDecl
                                    , const XMLReader::RefFrom  refFrom
                                    , const XMLReader::Types    type
                                    , const XMLReader::Sources  source
                                    ,       InputSource*&       srcToFill
                                    , const bool                calcSrcOfs
                                    ,       XMLSize_t           lowWaterMark
                                    , const bool                disableDefaultEntityResolution)
{
    //Normalize sysId
    XMLBuffer normalizedSysId(1023, fMemoryManager);
    if(sysId)
        XMLString::removeChar(sysId, 0xFFFF, normalizedSysId);
    const XMLCh* normalizedURI = normalizedSysId.getRawBuffer();

    // Create a buffer for expanding the system id
    XMLBuffer expSysId(1023, fMemoryManager);

    //
    //  Allow the entity handler to expand the system id if they choose
    //  to do so.
    //
    if (fEntityHandler)
    {
        if (!fEntityHandler->expandSystemId(normalizedURI, expSysId))
            expSysId.set(normalizedURI);
    }
     else
    {
        expSysId.set(normalizedURI);
    }

    // Call the entity resolver interface to get an input source
    srcToFill = 0;
    if (fEntityHandler)
    {
        LastExtEntityInfo lastInfo;
        getLastExtEntityInfo(lastInfo);
        XMLResourceIdentifier resourceIdentifier(XMLResourceIdentifier::ExternalEntity,
                            expSysId.getRawBuffer(), XMLUni::fgZeroLenString, pubId, lastInfo.systemId,
                            this);
        srcToFill = fEntityHandler->resolveEntity(&resourceIdentifier);
    }

    //
    //  If they didn't create a source via the entity resolver, then we
    //  have to create one on our own.
    //
    if (!srcToFill)
    {
        if (disableDefaultEntityResolution)
            return 0;

        LastExtEntityInfo lastInfo;
        getLastExtEntityInfo(lastInfo);

// Keep this #if 0 block as it was exposing a threading problem on AIX.
// Got rid of the problem by changing XMLURL to not throw malformedurl
// exceptions.
#if 0
        try
        {
            XMLURL urlTmp(lastInfo.systemId, expSysId.getRawBuffer(), fMemoryManager);
            if (urlTmp.isRelative())
            {
                ThrowXMLwithMemMgr
                (
                    MalformedURLException
                    , XMLExcepts::URL_NoProtocolPresent
                    , fMemoryManager
                );
            }
            else {
                if (fStandardUriConformant && urlTmp.hasInvalidChar())
                    ThrowXMLwithMemMgr(MalformedURLException, XMLExcepts::URL_MalformedURL, fMemoryManager);
                srcToFill = new (fMemoryManager) URLInputSource(urlTmp, fMemoryManager);
            }
        }

        catch(const MalformedURLException& e)
        {
            // Its not a URL, so lets assume its a local file name if non-standard uri is allowed
            if (!fStandardUriConformant)
                srcToFill = new (fMemoryManager) LocalFileInputSource
                (
                    lastInfo.systemId
                    , expSysId.getRawBuffer()
                    , fMemoryManager
                );
            else
                throw e;
        }
#else
        XMLURL urlTmp(fMemoryManager);
        if ((!urlTmp.setURL(lastInfo.systemId, expSysId.getRawBuffer(), urlTmp)) ||
            (urlTmp.isRelative()))
        {
            if (!fStandardUriConformant)
            {
                XMLBuffer resolvedSysId(1023, fMemoryManager);
                XMLUri::normalizeURI(expSysId.getRawBuffer(), resolvedSysId);

                srcToFill = new (fMemoryManager) LocalFileInputSource
                (
                    lastInfo.systemId
                    , resolvedSysId.getRawBuffer()
                    , fMemoryManager
                );
            }
            else
                ThrowXMLwithMemMgr(MalformedURLException, XMLExcepts::URL_MalformedURL, fMemoryManager);
        }
        else
        {
            if (fStandardUriConformant && urlTmp.hasInvalidChar())
                ThrowXMLwithMemMgr(MalformedURLException, XMLExcepts::URL_MalformedURL, fMemoryManager);
            srcToFill = new (fMemoryManager) URLInputSource(urlTmp, fMemoryManager);
        }
#endif
    }

    // Put a janitor on the input source
    Janitor<InputSource> janSrc(srcToFill);

    //
    //  Now call the other version with the input source that we have, and
    //  return the resulting reader.
    //
    XMLReader* retVal = createReader
    (
        *srcToFill
        , xmlDecl
        , refFrom
        , type
        , source
        , calcSrcOfs
        , lowWaterMark
    );

    // Either way, we can release the input source now
    janSrc.orphan();

    // If it failed for any reason, then return zero.
    if (!retVal)
        return 0;

    // Give this reader the next available reader number and return it
    retVal->setReaderNum(fNextReaderNum++);
    return retVal;
}


XMLReader* ReaderMgr::createReader( const   XMLCh* const        baseURI
                                    , const XMLCh* const        sysId
                                    , const XMLCh* const        pubId
                                    , const bool                xmlDecl
                                    , const XMLReader::RefFrom  refFrom
                                    , const XMLReader::Types    type
                                    , const XMLReader::Sources  source
                                    ,       InputSource*&       srcToFill
                                    , const bool                calcSrcOfs
                                    ,       XMLSize_t           lowWaterMark
                                    , const bool                disableDefaultEntityResolution)
{
    //Normalize sysId
    XMLBuffer normalizedSysId(1023, fMemoryManager);
    XMLString::removeChar(sysId, 0xFFFF, normalizedSysId);
    const XMLCh* normalizedURI = normalizedSysId.getRawBuffer();

    // Create a buffer for expanding the system id
    XMLBuffer expSysId(1023, fMemoryManager);

    //
    //  Allow the entity handler to expand the system id if they choose
    //  to do so.
    //
    if (fEntityHandler)
    {
        if (!fEntityHandler->expandSystemId(normalizedURI, expSysId))
            expSysId.set(normalizedURI);
    }
     else
    {
        expSysId.set(normalizedURI);
    }

    // Call the entity resolver interface to get an input source
    srcToFill = 0;
    if (fEntityHandler)
    {
        XMLResourceIdentifier resourceIdentifier(XMLResourceIdentifier::ExternalEntity,
                            expSysId.getRawBuffer(), XMLUni::fgZeroLenString, pubId, baseURI,
                            this);
        srcToFill = fEntityHandler->resolveEntity(&resourceIdentifier);
    }

    //
    //  If they didn't create a source via the entity resolver, then we
    //  have to create one on our own.
    //
    if (!srcToFill)
    {
        if (disableDefaultEntityResolution)
            return 0;

        LastExtEntityInfo lastInfo;

        const XMLCh* baseuri=baseURI;
        if(!baseuri || !*baseuri)
        {
            getLastExtEntityInfo(lastInfo);
            baseuri = lastInfo.systemId;
        }

        XMLURL urlTmp(fMemoryManager);
        if ((!urlTmp.setURL(baseuri, expSysId.getRawBuffer(), urlTmp)) ||
            (urlTmp.isRelative()))
        {
            if (!fStandardUriConformant)
            {
                XMLBuffer resolvedSysId(1023, fMemoryManager);
                XMLUri::normalizeURI(expSysId.getRawBuffer(), resolvedSysId);

                srcToFill = new (fMemoryManager) LocalFileInputSource
                (
                    baseuri
                    , resolvedSysId.getRawBuffer()
                    , fMemoryManager
                );
            }
            else
                ThrowXMLwithMemMgr(MalformedURLException, XMLExcepts::URL_MalformedURL, fMemoryManager);
        }
        else
        {
            if (fStandardUriConformant && urlTmp.hasInvalidChar())
                ThrowXMLwithMemMgr(MalformedURLException, XMLExcepts::URL_MalformedURL, fMemoryManager);
            srcToFill = new (fMemoryManager) URLInputSource(urlTmp, fMemoryManager);
        }
    }

    // Put a janitor on the input source
    Janitor<InputSource> janSrc(srcToFill);

    //
    //  Now call the other version with the input source that we have, and
    //  return the resulting reader.
    //
    XMLReader* retVal = createReader
    (
        *srcToFill
        , xmlDecl
        , refFrom
        , type
        , source
        , calcSrcOfs
        , lowWaterMark
    );

    // Either way, we can release the input source now
    janSrc.orphan();

    // If it failed for any reason, then return zero.
    if (!retVal)
        return 0;

    // Give this reader the next available reader number and return it
    retVal->setReaderNum(fNextReaderNum++);
    return retVal;
}


XMLReader*
ReaderMgr::createIntEntReader(  const   XMLCh* const        sysId
                                , const XMLReader::RefFrom  refFrom
                                , const XMLReader::Types    type
                                , const XMLCh* const        dataBuf
                                , const XMLSize_t           dataLen
                                , const bool                copyBuf
                                , const bool                calcSrcOfs
                                ,       XMLSize_t           lowWaterMark)
{
    //
    //  This one is easy, we just create an input stream for the data and
    //  provide a few extra goodies.
    //
    //  NOTE: We use a special encoding string that will be recognized
    //  as a 'do nothing' transcoder for the already internalized XMLCh
    //  data that makes up an internal entity.
    //
    BinMemInputStream* newStream = new (fMemoryManager) BinMemInputStream
                                   (
                                     (const XMLByte*)dataBuf
                                     , dataLen * sizeof(XMLCh)
                                     , copyBuf ? BinMemInputStream::BufOpt_Copy
                                               : BinMemInputStream::BufOpt_Reference
                                     , fMemoryManager
                                   );
    if (!newStream)
        return 0;

    XMLReader* retVal = new (fMemoryManager) XMLReader
    (
        sysId
        , 0
        , newStream
        , XMLRecognizer::XERCES_XMLCH
        , refFrom
        , type
        , XMLReader::Source_Internal
        , false
        , calcSrcOfs
        , lowWaterMark
        , fXMLVersion
        , fMemoryManager
    );

    // If it failed for any reason, then return zero.
    if (!retVal) {
        delete newStream;
        return 0;
    }

    // Set the reader number to the next available number
    retVal->setReaderNum(fNextReaderNum++);
    return retVal;
}


const XMLCh* ReaderMgr::getCurrentEncodingStr() const
{
    const XMLEntityDecl*    theEntity;
    const XMLReader*        theReader = getLastExtEntity(theEntity);

    return theReader->getEncodingStr();
}


const XMLEntityDecl* ReaderMgr::getCurrentEntity() const
{
    return fCurReaderData? fCurReaderData->getEntity() : 0;
}


XMLEntityDecl* ReaderMgr::getCurrentEntity()
{
    return fCurReaderData? fCurReaderData->getEntity() : 0;
}


XMLSize_t ReaderMgr::getReaderDepth() const
{
    // If the stack doesn't exist, its obviously zero
    if (!fReaderStack)
        return 0;

    //
    //  The return is the stack size, plus one if there is a current
    //  reader. So if there is no current reader and none on the stack,
    //  its zero, else its some non-zero value.
    //
    XMLSize_t retVal = fReaderStack->size();
    if (fCurReader)
        retVal++;
    return retVal;
}

void ReaderMgr::getLastExtEntityInfo(LastExtEntityInfo& lastInfo) const
{
    //
    //  If the reader stack never got created or we've not managed to open any
    //  main entity yet, then we can't give this information.
    //
    if (!fReaderStack || !fCurReader)
    {
        lastInfo.systemId = XMLUni::fgZeroLenString;
        lastInfo.publicId = XMLUni::fgZeroLenString;
        lastInfo.lineNumber = 0;
        lastInfo.colNumber = 0;
        return;
    }

    // We have at least one entity so get the data
    const XMLEntityDecl*    theEntity;
    const XMLReader*        theReader = getLastExtEntity(theEntity);

    // Fill in the info structure with the reader we found
    lastInfo.systemId = theReader->getSystemId();
    lastInfo.publicId = theReader->getPublicId();
    lastInfo.lineNumber = theReader->getLineNumber();
    lastInfo.colNumber = theReader->getColumnNumber();
}


bool ReaderMgr::isScanningPERefOutOfLiteral() const
{
    // If the current reader is not for an entity, then definitely not
    if (!fCurReaderData || !fCurReaderData->getEntity())
        return false;

    //
    //  If this is a PE entity, and its not being expanded in a literal
    //  then its true.
    //
    if ((fCurReader->getType() == XMLReader::Type_PE)
    &&  (fCurReader->getRefFrom() == XMLReader::RefFrom_NonLiteral))
    {
        return true;
    }
    return false;
}

bool ReaderMgr::pushReader(         XMLReader* const        reader
                            ,       XMLEntityDecl* const    entity)
{
    return pushReaderAdoptEntity(reader, entity, false);
}

bool ReaderMgr::pushReaderAdoptEntity(     XMLReader* const        reader
                                       ,   XMLEntityDecl* const    entity
                                       ,   const bool              adoptEntity)
{
    //
    //  First, if an entity was passed, we have to confirm that this entity
    //  is not already on the reader stack. If so, then this is a recursive
    //  entity expansion, so we issue an error and refuse to put the reader
    //  on the stack.
    //
    //  If there is no entity passed, then its not an entity being pushed, so
    //  nothing to do. If there is no entity stack yet, then of coures it
    //  cannot already be there.
    //
    if (entity && fReaderStack)
    {
        // @@ Strangely, we don't check the entity at the top of the stack
        //    (fCurReaderData). Is it a bug?
        //
        const XMLSize_t count = fReaderStack->size();
        const XMLCh* const theName = entity->getName();
        for (XMLSize_t index = 0; index < count; index++)
        {
            const XMLEntityDecl* curDecl =
              fReaderStack->elementAt(index)->getEntity();

            if (curDecl)
            {
                if (XMLString::equals(theName, curDecl->getName()))
                {
                    // Oops, already there so delete reader and entity and
                    // return.
                    //
                    delete reader;

                    if (adoptEntity)
                        delete entity;

                    return false;
                }
            }
        }
    }

    //
    //  Fault in the reader stack. Give it an initial capacity of 16, and
    //  tell it it does own its elements.
    //
    if (!fReaderStack)
        fReaderStack = new (fMemoryManager) RefStackOf<ReaderData>(16, true, fMemoryManager);

    //
    //  Push the current reader and entity onto the stack. Note that
    //  the current entity can be null if the current reader is not for
    //  an entity.
    //
    if (fCurReaderData)
        fReaderStack->push(fCurReaderData);

    //
    //  Make the passed reader and entity the current top of stack. The
    //  passed entity can (and often is) null.
    //
    fCurReaderData = new (fMemoryManager) ReaderData(reader, entity, adoptEntity);
    fCurReader = reader;

    return true;
}

void ReaderMgr::reset()
{
    // Reset all of the flags
    fThrowEOE = false;

    // Delete the current reader and flush the reader stack
    delete fCurReaderData;
    fCurReaderData = 0;
    fCurReader = 0;
    if (fReaderStack)
        fReaderStack->removeAllElements();
}


// ---------------------------------------------------------------------------
//  ReaderMgr: Implement the SAX Locator interface
// ---------------------------------------------------------------------------
const XMLCh* ReaderMgr::getPublicId() const
{
    if (!fReaderStack && !fCurReader)
        return XMLUni::fgZeroLenString;

    const XMLEntityDecl* theEntity;
    return getLastExtEntity(theEntity)->getPublicId();
}

const XMLCh* ReaderMgr::getSystemId() const
{
    if (!fReaderStack && !fCurReader)
        return XMLUni::fgZeroLenString;

    const XMLEntityDecl* theEntity;
    return getLastExtEntity(theEntity)->getSystemId();
}

XMLFileLoc ReaderMgr::getColumnNumber() const
{
    if (!fReaderStack && !fCurReader)
        return 0;

    const XMLEntityDecl* theEntity;
    return getLastExtEntity(theEntity)->getColumnNumber();
}

XMLFileLoc ReaderMgr::getLineNumber() const
{
    if (!fReaderStack && !fCurReader)
        return 0;

    const XMLEntityDecl* theEntity;
    return getLastExtEntity(theEntity)->getLineNumber();
}



// ---------------------------------------------------------------------------
//  ReaderMgr: Private helper methods
// ---------------------------------------------------------------------------
const XMLReader*
ReaderMgr::getLastExtEntity(const XMLEntityDecl*& itsEntity) const
{
    //
    //  Scan down the reader stack until we find a reader for an entity that
    //  is external. First check that there is anything in the stack at all,
    //  in which case the current reader is the main file and that's the one
    //  that we want.
    //
    const XMLReader* theReader = fCurReader;

    //
    //  If there is a current entity and it is not an external entity, then
    //  search the stack; else, keep the reader that we've got since its
    //  either an external entity reader or the main file reader.
    //
    const XMLEntityDecl* curEntity =
        fCurReaderData? fCurReaderData->getEntity() : 0;

    if (curEntity && !curEntity->isExternal())
    {
        XMLSize_t index = fReaderStack->size();
        if (index)
        {
            while (true)
            {
                // Move down to the previous element and get a pointer to it
                index--;
                curEntity = fReaderStack->elementAt(index)->getEntity();

                //
                //  If its null or its an external entity, then this reader
                //  is what we want, so break out with that one.
                //
                if (!curEntity)
                {
                    theReader = fReaderStack->elementAt(index)->getReader ();
                    break;
                }
                 else if (curEntity->isExternal())
                {
                    theReader = fReaderStack->elementAt(index)->getReader ();
                    break;
                }

                // We hit the end, so leave the main file reader as the one
                if (!index)
                    break;
            }
        }
    }

    // @@ It feels like we may end up with theReader being from the top of
    //    the stack (fCurReader) and itsEntity being from the bottom of the
    //    stack (if there are no null or external entities on the stack).
    //    Is it a bug?
    //
    itsEntity = curEntity;
    return theReader;
}


bool ReaderMgr::popReader()
{
    //
    //  We didn't get any more, so try to pop off a reader. If the reader
    //  stack is empty, then we are at the end, so return false.
    //
    //  @@ It feels like we never pop the reader pushed to the stack first
    //     (think of fReaderStack empty but fCurReader not null). Is it a
    //     bug?
    //
    if (fReaderStack->empty())
        return false;

    //
    //  Remember the current reader, before we pop off a new one. We might
    //  need this to throw the end of entity exception at the end.
    //
    ReaderData* prevReaderData = fCurReaderData;
    const bool prevReaderThrowAtEnd = fCurReader->getThrowAtEnd();
    const XMLSize_t readerNum = fCurReader->getReaderNum();

    //
    //  Pop a new reader and entity off the stack.
    //
    fCurReaderData = fReaderStack->pop();
    fCurReader = fCurReaderData->getReader();

    //
    //  If there was a previous entity, and either the fThrowEOE flag is set
    //  or reader was marked as such, then throw an end of entity. Otherwise,
    //  delete the previous reader data.
    //
    if (prevReaderData->getEntity() && (fThrowEOE || prevReaderThrowAtEnd))
    {
        //
        // If the entity is adopted, then move it to fEntityStack so that
        // its life-time is prolonged to the life-time of this reader
        // manager. Also delete the previous reader data before throwing
        // EndOfEntityException.
        //
        XMLEntityDecl* entity;

        if (prevReaderData->getEntityAdopted())
        {
            if (!fEntityStack)
                fEntityStack = new (fMemoryManager) RefStackOf<XMLEntityDecl>(16, true, fMemoryManager);

            entity = prevReaderData->releaseEntity();
            fEntityStack->push(entity);
        }
        else
            entity = prevReaderData->getEntity();

        delete prevReaderData;

        throw EndOfEntityException(entity, readerNum);
    }
    else
        delete prevReaderData;

    while (true)
    {
        //
        //  They don't want us to throw, so lets just return with a new
        //  reader. Here we have to do a loop because we might have multiple
        //  readers on these stack that are empty (i.e. the last char in them
        //  was the ';' at the end of the entity ref that caused the next
        //  entity to be pushed.
        //
        //  So we loop until we find a non-empty reader, or hit the main
        //  file entity. If we find one with some chars available, then break
        //  out and take that one.
        //
        if (fCurReader->charsLeftInBuffer())
            break;

        fCurReader->refreshCharBuffer();
        if (fCurReader->charsLeftInBuffer())
            break;

        //
        //  The current one is hosed. So, if the reader stack is empty we
        //  are dead meat and can give up now.
        //
        if (fReaderStack->empty())
            return false;

        // Else pop again and try it one more time
        delete fCurReaderData;
        fCurReaderData = fReaderStack->pop();
        fCurReader = fCurReaderData->getReader();
    }
    return true;
}

XERCES_CPP_NAMESPACE_END
