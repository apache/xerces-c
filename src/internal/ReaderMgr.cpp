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

/**
 * $Log$
 * Revision 1.8  2000/02/22 00:36:50  roddey
 * Added a new 'native XMLCh' transcoder to correctly handle
 * internal entities now that XMLCh isn't always UTF-16.
 *
 * Revision 1.7  2000/02/06 07:47:53  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.6  2000/01/25 01:04:21  roddey
 * Fixes a bogus error about ]]> in char data.
 *
 * Revision 1.5  2000/01/15 01:26:16  rahulj
 * Added support for HTTP to the parser using libWWW 5.2.8.
 * Renamed URL.[ch]pp to XMLURL.[ch]pp and like wise for the class name.
 * Only tested under NT 4.0 SP 5.
 * Removed URL.hpp from files where it was not used.
 *
 * Revision 1.4  2000/01/13 22:50:29  aruna1
 * For unix made additional check for relative url path
 *
 * Revision 1.3  2000/01/12 00:15:03  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.2  1999/12/18 00:20:00  roddey
 * More changes to support the new, completely orthagonal, support for
 * intrinsic encodings.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:11  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:56:54  droddey
 * If the main xml entity does not exist, we need to get the error handling for that
 * inside the main XMLScanner::scanDocument() try block so that it gets reported
 * in the normal way. We have to add a little extra safety code because, when this
 * happens, there is no reader on the reader stack to get position ino from.
 *
 * Revision 1.3  1999/11/08 20:44:43  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/BinMemInputStream.hpp>
#include <util/Janitor.hpp>
#include <util/PlatformUtils.hpp>
#include <util/RuntimeException.hpp>
#include <util/UnexpectedEOFException.hpp>
#include <util/XMLURL.hpp>
#include <util/XMLUni.hpp>
#include <sax/InputSource.hpp>
#include <framework/LocalFileInputSource.hpp>
#include <framework/URLInputSource.hpp>
#include <framework/XMLBuffer.hpp>
#include <framework/XMLDocumentHandler.hpp>
#include <framework/XMLEntityDecl.hpp>
#include <framework/XMLEntityHandler.hpp>
#include <internal/EndOfEntityException.hpp>
#include <internal/ReaderMgr.hpp>


// ---------------------------------------------------------------------------
//  ReaderMgr: Constructors and Destructor
// ---------------------------------------------------------------------------
ReaderMgr::ReaderMgr() :

    fCurEntity(0)
    , fCurReader(0)
    , fEntityHandler(0)
    , fEntityStack(0)
    , fNextReaderNum(1)
    , fReaderStack(0)
    , fThrowEOE(false)
{
}

ReaderMgr::~ReaderMgr()
{
    //
    //  Clean up the reader and entity stacks. Note that we don't own the
    //  entities, so we don't delete the current entity (and the entity stack
    //  does not own its elements either, so deleting it will not delete the
    //  entities it still references!)
    //
    delete fCurReader;
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

    while (true)
    {
        //
        //  Get all the spaces from the current reader. If it returns true,
        //  it hit a non-space and we are done. Else we have to pop a reader
        //  and keep going.
        //
        if (fCurReader->getSpaces(toFill))
            break;

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
    while (true)
    {
        if (fCurReader->getUpToCharOrWS(toFill, toCheck))
            break;

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


bool ReaderMgr::skipPastSpaces()
{
    bool skippedSomething = false;
    bool tmpFlag;
    while (true)
    {
        //
        //  Skip all the spaces in the current reader. If it returned because
        //  it hit a non-space, break out. Else we have to pop another entity
        //  and keep going.
        //
        if (fCurReader->skipSpaces(tmpFlag))
            break;

        if (tmpFlag)
            skippedSomething = true;

        // Try to pop another enitity. If we can't then we are done
        if (!popReader())
            break;
    }
    return (tmpFlag || skippedSomething);
}


void ReaderMgr::skipQuotedString(const XMLCh quoteCh)
{
    XMLCh nextCh;
    while (true)
    {
        nextCh = getNextChar();

        // If we get an end of file char, then return
        if (!nextCh)
            break;

        // If we get the quote char, then break out
        if (nextCh == quoteCh)
            break;
    }
}


XMLCh ReaderMgr::skipUntilIn(const XMLCh* const listToSkip)
{
    XMLCh nextCh;
    while (true)
    {
        nextCh = peekNextChar();

        if (!nextCh)
            break;

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
    while (true)
    {
        nextCh = peekNextChar();

        if (!nextCh)
            break;

        if (XMLReader::isWhitespace(nextCh))
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
void ReaderMgr::cleanStackBackTo(const unsigned int readerNum)
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
            ThrowXML(RuntimeException, XML4CExcepts::RdrMgr_ReaderIdNotFound);

        delete fCurReader;
        fCurReader = fReaderStack->pop();
        fCurEntity = fEntityStack->pop();
    }
}


XMLReader* ReaderMgr::createReader( const   InputSource&        src
                                    , const bool                xmlDecl
                                    , const XMLReader::RefFrom  refFrom
                                    , const XMLReader::Types    type
                                    , const XMLReader::Sources  source)
{
    //
    //  Ask the input source to create us an input stream. The particular
    //  type of input source will know what kind to create.
    //
    BinInputStream* newStream = src.makeStream();
    if (!newStream)
        return 0;

    //
    //  Create a new reader and return it. If the source has an encoding that
    //  it wants to force, then we call the constructor that does that.
    //  Otherwise, we just call the one that provides the provisional encoding
    //  to be possibly updated later by the encoding="" setting.
    //
    XMLReader* retVal = 0;
    if (src.getEncoding())
    {
        retVal = new XMLReader
        (
            src.getPublicId()
            , src.getSystemId()
            , newStream
            , src.getEncoding()
            , refFrom
            , type
            , source
        );
    }
     else
    {
        retVal = new XMLReader
        (
            src.getPublicId()
            , src.getSystemId()
            , newStream
            , refFrom
            , type
            , source
        );
    }

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
                                    ,       InputSource*&       srcToFill)
{
    // Create a buffer for expanding the system id
    XMLBuffer expSysId;

    //
    //  Allow the entity handler to expand the system id if they choose
    //  to do so.
    //
    if (fEntityHandler)
    {
        if (!fEntityHandler->expandSystemId(sysId, expSysId))
            expSysId.set(sysId);
    }
     else
    {
        expSysId.set(sysId);
    }

    // Call the entity resolver interface to get an input source
    srcToFill = 0;
    if (fEntityHandler)
    {
        srcToFill = fEntityHandler->resolveEntity
        (
            pubId
            , expSysId.getRawBuffer()
        );
    }

    //
    //  If they didn't create a source via the entity resolver, then we
    //  have to create one on our own.
    //
    if (!srcToFill)
    {
        LastExtEntityInfo lastInfo;
        getLastExtEntityInfo(lastInfo);

        try
        {
            XMLURL urlTmp(lastInfo.systemId, expSysId.getRawBuffer());
            if (urlTmp.isRelative())
                ThrowXML(MalformedURLException,
                         XML4CExcepts::URL_NoProtocolPresent);
            srcToFill = new URLInputSource(urlTmp);
        }

        catch(const MalformedURLException&)
        {
            // Its not a URL, so lets assume its a local file name.
            srcToFill = new LocalFileInputSource
            (
                lastInfo.systemId
                , expSysId.getRawBuffer()
            );
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
                                , const unsigned int        dataLen
                                , const bool                copyBuf)
{
    //
    //  This one is easy, we just create an input stream for the data and
    //  provide a few extra goodies.
    //
    //  NOTE: We use a special encoding string that will be recognized
    //  as a 'do nothing' transcoder for the already internalized XMLCh
    //  data that makes up an internal entity.
    //
    XMLReader* retVal = new XMLReader
    (
        sysId
        , 0
        , new BinMemInputStream
          (
            (const XMLByte*)dataBuf
            , dataLen * sizeof(XMLCh)
            , copyBuf ? BinMemInputStream::BufOpt_Copy
                        : BinMemInputStream::BufOpt_Reference
          )
        , XMLUni::fgXMLChEncodingString
        , refFrom
        , type
        , XMLReader::Source_Internal
    );

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
    return fCurEntity;
}


XMLEntityDecl* ReaderMgr::getCurrentEntity()
{
    return fCurEntity;
}


const XMLReader* ReaderMgr::getCurrentReader() const
{
    return fCurReader;
}


XMLReader* ReaderMgr::getCurrentReader()
{
    return fCurReader;
}


void ReaderMgr::getLastExtEntityInfo(LastExtEntityInfo& lastInfo) const
{
    //
    //  If the reader stack never got created, then we can't give this
    //  information.
    //
    if (!fReaderStack && !fCurReader)
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
    if (!fCurEntity)
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
    //
    //  First, if an entity was passed, we have to confirm that this entity
    //  is not already on the entity stack. If so, then this is a recursive
    //  entity expansion, so we issue an error and refuse to put the reader
    //  on the stack.
    //
    //  If there is no entity passed, then its not an entity being pushed, so
    //  nothing to do. If there is no entity stack yet, then of coures it
    //  cannot already be there.
    //
    if (entity && fEntityStack)
    {
        const unsigned int count = fEntityStack->size();
        const XMLCh* const theName = entity->getName();
        for (unsigned int index = 0; index < count; index++)
        {
            const XMLEntityDecl* curDecl = fEntityStack->elementAt(index);
            if (curDecl)
            {
                if (!XMLString::compareString(theName, curDecl->getName()))
                {
                    // Oops, already there so delete reader and return
                    delete reader;
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
        fReaderStack = new RefStackOf<XMLReader>(16, true);

    // And the entity stack, which does not own its elements
    if (!fEntityStack)
        fEntityStack = new RefStackOf<XMLEntityDecl>(16, false);

    //
    //  Push the current reader and entity onto their respective stacks.
    //  Note that the the current entity can be null if the current reader
    //  is not for an entity.
    //
    if (fCurReader)
    {
        fReaderStack->push(fCurReader);
        fEntityStack->push(fCurEntity);
    }

    //
    //  Make the passed reader and entity the current top of stack. The
    //  passed entity can (and often is) null.
    //
    fCurReader = reader;
    fCurEntity = entity;

    return true;
}


void ReaderMgr::reset()
{
    // Reset all of the flags
    fThrowEOE = false;

    // Delete the current reader and flush the reader stack
    delete fCurReader;
    fCurReader = 0;
    if (fReaderStack)
        fReaderStack->removeAllElements();

    //
    //  And do the same for the entity stack, but don't delete the current
    //  entity (if any) since we don't own them.
    //
    fCurEntity = 0;
    if (fEntityStack)
        fEntityStack->removeAllElements();
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
    const XMLEntityDecl* curEntity = fCurEntity;
    if (curEntity && !curEntity->isExternal())
    {
        unsigned int index = fReaderStack->size();
        if (index)
        {
            while (true)
            {
                // Move down to the previous element and get a pointer to it
                index--;
                curEntity = fEntityStack->elementAt(index);

                //
                //  If its null or its an external entity, then this reader
                //  is what we want, so break out with that one.
                //
                if (!curEntity)
                {
                    theReader = fReaderStack->elementAt(index);
                    break;
                }
                 else if (curEntity->isExternal())
                {
                    theReader = fReaderStack->elementAt(index);
                    break;
                }

                // We hit the end, so leave the main file reader as the one
                if (!index)
                    break;
            }
        }
    }

    itsEntity = curEntity;
    return theReader;
}


bool ReaderMgr::popReader()
{
    //
    //  We didn't get any more, so try to pop off a reader. If the reader
    //  stack is empty, then we are at the end, so return false.
    //
    if (fReaderStack->empty())
        return false;

    //
    //  Remember the current entity, before we pop off a new one. We might
    //  need this to throw the end of entity exception at the end.
    //
    XMLEntityDecl* prevEntity = fCurEntity;
    const bool prevReaderThrowAtEnd = fCurReader->getThrowAtEnd();
    const unsigned int readerNum = fCurReader->getReaderNum();

    //
    //  Delete the current reader and pop a new reader and entity off
    //  the stacks.
    //
    delete fCurReader;
    fCurReader = fReaderStack->pop();
    fCurEntity = fEntityStack->pop();

    //
    //  If there was a previous entity, and either the fThrowEOE flag is set
    //  or reader was marked as such, then throw an end of entity.
    //
    if (prevEntity && fThrowEOE || prevReaderThrowAtEnd)
        throw EndOfEntityException(prevEntity, readerNum);

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
        delete fCurReader;
        fCurReader = fReaderStack->pop();
        fCurEntity = fEntityStack->pop();
    }
    return true;
}
