/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
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
 * Revision 1.18  2005/04/05 15:12:36  cargilld
 * Implement support for disabling default entity resolution.
 *
 * Revision 1.17  2005/04/04 15:11:38  cargilld
 * Fix a problem where illegal qualified names were not reported as errors.  Also store the colon position when searching for a qualified name to avoid looking it up again.
 *
 * Revision 1.16  2005/03/09 16:07:11  amassari
 * Protected getSrcOffset to avoid crashing when parsing has finished; updated documentation
 *
 * Revision 1.15  2005/02/08 09:21:11  amassari
 * Removed warnings
 *
 * Revision 1.14  2004/09/08 13:56:13  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.13  2004/06/14 15:18:52  peiyongz
 * Consolidated End Of Line Handling
 *
 * Revision 1.12  2004/06/03 15:38:27  peiyongz
 * XML1.1:  The characters #x85 and #x2028 cannot be reliably recognized
 * and translated until an entity's encoding declaration (if present) has been
 * read. Therefore, it is a fatal error to use them within the XML declaration or
 * text declaration.
 *
 * Revision 1.11  2004/01/29 11:46:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.10  2003/08/14 02:56:41  knoaman
 * Code refactoring to improve performance of validation.
 *
 * Revision 1.9  2003/05/16 21:36:57  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.8  2003/05/15 18:26:29  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.7  2003/01/03 20:08:39  tng
 * New feature StandardUriConformant to force strict standard uri conformance.
 *
 * Revision 1.6  2002/12/20 22:09:56  tng
 * XML 1.1
 *
 * Revision 1.5  2002/12/03 15:31:19  knoaman
 * Enable/disable calculation of src offset.
 *
 * Revision 1.4  2002/11/04 14:58:18  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/08/22 19:28:11  tng
 * [Bug 11448] DomCount has problems with XHTML1.1 DTD.
 *
 * Revision 1.2  2002/05/27 18:42:14  tng
 * To get ready for 64 bit large file, use XMLSSize_t to represent line and column number.
 *
 * Revision 1.1.1.1  2002/02/01 22:21:58  peiyongz
 * sane_include
 *
 * Revision 1.13  2001/07/12 18:50:08  tng
 * Some performance modification regarding standalone check and xml decl check.
 *
 * Revision 1.12  2000/09/09 00:18:18  andyh
 * Reordered member variables in ThrowEOEJanitor.  Patch submitted
 * by Kirk Wylie.
 *
 * Revision 1.11  2000/07/08 00:17:13  andyh
 * Cleanup of yesterday's speedup changes.  Merged new bit into the
 * scanner character properties table.
 *
 * Revision 1.10  2000/07/07 01:08:44  andyh
 * Parser speed up in scan of XML content.
 *
 * Revision 1.9  2000/03/02 19:54:29  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.8  2000/02/24 20:18:07  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.7  2000/02/24 02:12:53  aruna1
 * ReaderMgr:;getReaderDepth() added
 *
 * Revision 1.6  2000/02/06 07:47:53  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.5  2000/01/25 01:04:21  roddey
 * Fixes a bogus error about ]]> in char data.
 *
 * Revision 1.4  2000/01/24 20:40:43  roddey
 * Exposed the APIs to get to the byte offset in the source XML buffer. This stuff
 * is not tested yet, but I wanted to get the API changes in now so that the API
 * can be stablized.
 *
 * Revision 1.3  2000/01/12 00:15:04  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.2  1999/12/15 19:48:03  roddey
 * Changed to use new split of transcoder interfaces into XML transcoders and
 * LCP transcoders, and implementation of intrinsic transcoders as pluggable
 * transcoders, and addition of Latin1 intrinsic support.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:13  twl
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

#if !defined(READERMGR_HPP)
#define READERMGR_HPP

#include <xercesc/internal/XMLReader.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/RefStackOf.hpp>
#include <xercesc/sax/Locator.hpp>
#include <xercesc/framework/XMLBuffer.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLEntityDecl;
class XMLEntityHandler;
class XMLDocumentHandler;
class XMLScanner;


// ---------------------------------------------------------------------------
//  This class is used by the scanner. The scanner must deal with expansion
//  of entities, some of which are totally different files (external parsed
//  entities.) It does so by pushing readers onto a stack. The top reader is
//  the one it wants to read out of, but that one must be popped when it is
//  empty. To keep that logic from being all over the place, the scanner
//  talks to the reader manager, which handles the stack and popping off
//  used up readers.
// ---------------------------------------------------------------------------
class XMLPARSER_EXPORT ReaderMgr :   public XMemory 
                                   , public Locator
{
public :
    // -----------------------------------------------------------------------
    //  Class specific types
    // -----------------------------------------------------------------------
    struct LastExtEntityInfo : public XMemory
    {
        const   XMLCh*          systemId;
        const   XMLCh*          publicId;
                XMLSSize_t      lineNumber;
                XMLSSize_t      colNumber;
    };


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    ReaderMgr(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    ~ReaderMgr();


    // -----------------------------------------------------------------------
    //  Convenience scanning methods
    //
    //  This are all convenience methods that work in terms of the core
    //  character spooling methods.
    // -----------------------------------------------------------------------
    bool atEOF() const;
    bool getName(XMLBuffer& toFill);
    bool getQName(XMLBuffer& toFill, int* colonPosition);
    bool getNameToken(XMLBuffer& toFill);
    XMLCh getNextChar();
    bool getNextCharIfNot(const XMLCh chNotToGet, XMLCh& chGotten);
    void movePlainContentChars(XMLBuffer &dest);
    void getSpaces(XMLBuffer& toFill);
    void getUpToCharOrWS(XMLBuffer& toFill, const XMLCh toCheck);
    bool isEmpty() const;
    bool lookingAtChar(const XMLCh toCheck);
    bool lookingAtSpace();
    XMLCh peekNextChar();
    bool skipIfQuote(XMLCh& chGotten);
    void skipPastChar(const XMLCh toSkip);
    bool skipPastSpaces(bool inDecl = false);
    void skipToChar(const XMLCh toSkipTo);
    bool skippedChar(const XMLCh toSkip);
    bool skippedSpace();
    bool skippedString(const XMLCh* const toSkip);
    void skipQuotedString(const XMLCh quoteCh);
    XMLCh skipUntilIn(const XMLCh* const listToSkip);
    XMLCh skipUntilInOrWS(const XMLCh* const listToSkip);
    bool peekString(const XMLCh* const toPeek);


    // -----------------------------------------------------------------------
    //  Control methods
    // -----------------------------------------------------------------------
    void cleanStackBackTo(const unsigned int readerNum);
    XMLReader* createReader
    (
        const   InputSource&        src
        , const bool                xmlDecl
        , const XMLReader::RefFrom  refFrom
        , const XMLReader::Types    type
        , const XMLReader::Sources  source
        , const bool                calcSrsOfs = true
    );
    XMLReader* createReader
    (
        const   XMLCh* const        sysId
        , const XMLCh* const        pubId
        , const bool                xmlDecl
        , const XMLReader::RefFrom  refFrom
        , const XMLReader::Types    type
        , const XMLReader::Sources  source
        ,       InputSource*&       srcToFill
        , const bool                calcSrcOfs = true
        , const bool                disableDefaultEntityResolution = false
    );
    XMLReader* createReader
    (
        const   XMLCh* const        baseURI
        , const XMLCh* const        sysId
        , const XMLCh* const        pubId
        , const bool                xmlDecl
        , const XMLReader::RefFrom  refFrom
        , const XMLReader::Types    type
        , const XMLReader::Sources  source
        ,       InputSource*&       srcToFill
        , const bool                calcSrcOfs = true
        , const bool                disableDefaultEntityResolution = false
    );
    XMLReader* createIntEntReader
    (
        const   XMLCh* const        sysId
        , const XMLReader::RefFrom  refFrom
        , const XMLReader::Types    type
        , const XMLCh* const        dataBuf
        , const unsigned int        dataLen
        , const bool                copyBuf
        , const bool                calcSrcOfs = true
    );
    bool isScanningPERefOutOfLiteral() const;
    bool pushReader
    (
                XMLReader* const        reader
        ,       XMLEntityDecl* const    entity
    );
    void reset();


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* getCurrentEncodingStr() const;
    const XMLEntityDecl* getCurrentEntity() const;
    XMLEntityDecl* getCurrentEntity();
    const XMLReader* getCurrentReader() const;
    XMLReader* getCurrentReader();
    unsigned int getCurrentReaderNum() const;
    unsigned int getReaderDepth() const;
    void getLastExtEntityInfo(LastExtEntityInfo& lastInfo) const;
    unsigned int getSrcOffset() const;
    bool getThrowEOE() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setEntityHandler(XMLEntityHandler* const newHandler);
    void setThrowEOE(const bool newValue);
    void setXMLVersion(const XMLReader::XMLVersion version);
    void setStandardUriConformant(const bool newValue);

    // -----------------------------------------------------------------------
    //  Implement the SAX Locator interface
    // -----------------------------------------------------------------------
    virtual const XMLCh* getPublicId() const;
    virtual const XMLCh* getSystemId() const;
    virtual XMLSSize_t getLineNumber() const;
    virtual XMLSSize_t getColumnNumber() const;


private :
    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    const XMLReader* getLastExtEntity(const XMLEntityDecl*& itsEntity) const;
    bool popReader();

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    ReaderMgr(const ReaderMgr&);
    ReaderMgr& operator=(const ReaderMgr&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fCurEntity
    //      This is the current top of stack entity. We pull it off the stack
    //      and store it here for efficiency.
    //
    //  fCurReader
    //      This is the current top of stack reader. We pull it off the
    //      stack and store it here for efficiency.
    //
    //  fEntityHandler
    //      This is the installed entity handler. Its installed via the
    //      scanner but he passes it on to us since we need it the most, in
    //      process of creating external entity readers.
    //
    //  fEntityStack
    //      We need to keep up with which of the pushed readers are pushed
    //      entity values that are being spooled. This is done to avoid the
    //      problem of recursive definitions. This stack consists of refs to
    //      EntityDecl objects for the pushed entities.
    //
    //  fNextReaderNum
    //      This is the reader serial number value. Each new reader that is
    //      created from this reader is given a successive number. This lets
    //      us catch things like partial markup errors and such.
    //
    //  fReaderStack
    //      This is the stack of reader references. We own all the readers
    //      and destroy them when they are used up.
    //
    //  fThrowEOE
    //      This flag controls whether we throw an exception when we hit an
    //      end of entity. The scanner doesn't really need to know about ends
    //      of entities in the int/ext subsets, so it will turn this flag off
    //      until it gets into the content usually.
    //
    //  fXMLVersion
    //      Enum to indicate if each Reader should be created as XML 1.1 or
    //      XML 1.0 conformant
    //
    //  fStandardUriConformant
    //      This flag controls whether we force conformant URI
    // -----------------------------------------------------------------------
    XMLEntityDecl*              fCurEntity;
    XMLReader*                  fCurReader;
    XMLEntityHandler*           fEntityHandler;
    RefStackOf<XMLEntityDecl>*  fEntityStack;
    unsigned int                fNextReaderNum;
    RefStackOf<XMLReader>*      fReaderStack;
    bool                        fThrowEOE;
    XMLReader::XMLVersion       fXMLVersion;
    bool                        fStandardUriConformant;
    MemoryManager*              fMemoryManager;
};



// ---------------------------------------------------------------------------
//  ReaderMgr: Inlined methods
//
//  NOTE: We cannot put these in alphabetical and type order as we usually
//  do because some of the compilers we have to support are too stupid to
//  understand out of order inlines!
// ---------------------------------------------------------------------------
inline unsigned int ReaderMgr::getCurrentReaderNum() const
{
    return fCurReader->getReaderNum();
}

inline const XMLReader* ReaderMgr::getCurrentReader() const
{
    return fCurReader;
}

inline XMLReader* ReaderMgr::getCurrentReader()
{
    return fCurReader;
}

inline bool ReaderMgr::getName(XMLBuffer& toFill)
{
    toFill.reset();
    return fCurReader->getName(toFill, false);
}

inline bool ReaderMgr::getQName(XMLBuffer& toFill, int *colonPosition)
{
    toFill.reset();
    return fCurReader->getQName(toFill, colonPosition);
}

inline bool ReaderMgr::getNameToken(XMLBuffer& toFill)
{
    toFill.reset();
    return fCurReader->getName(toFill, true);
}

inline bool ReaderMgr::getNextCharIfNot(const XMLCh chNotToGet, XMLCh& chGotten)
{
    return fCurReader->getNextCharIfNot(chNotToGet, chGotten);
}

inline void ReaderMgr::movePlainContentChars(XMLBuffer &dest)
{
    fCurReader->movePlainContentChars(dest);
}

inline bool ReaderMgr::getThrowEOE() const
{
    return fThrowEOE;
}

inline unsigned int ReaderMgr::getSrcOffset() const
{
    return fCurReader? fCurReader->getSrcOffset() : 0;
}

inline bool ReaderMgr::lookingAtChar(const XMLCh chToCheck)
{
    return (chToCheck == peekNextChar());
}

inline bool ReaderMgr::lookingAtSpace()
{
    XMLCh c = peekNextChar();
    return fCurReader->isWhitespace(c);
}

inline void ReaderMgr::setThrowEOE(const bool newValue)
{
    fThrowEOE = newValue;
}

inline void ReaderMgr::setStandardUriConformant(const bool newValue)
{
    fStandardUriConformant = newValue;
}

inline bool ReaderMgr::skippedString(const XMLCh* const toSkip)
{
    return fCurReader->skippedString(toSkip);
}

inline void ReaderMgr::skipToChar(const XMLCh toSkipTo)
{
	XMLCh nextCh = 0;
    do
    {
        // Get chars until we find the one to skip
        nextCh = getNextChar();
	} 
    // Break out at end of input or the char to skip
	while((nextCh != toSkipTo) && nextCh!=0);
}

inline void ReaderMgr::skipPastChar(const XMLCh toSkipPast)
{
	XMLCh nextCh = 0;
    do
    {
        // Get chars until we find the one to skip
        nextCh = getNextChar();
	} 
	while((nextCh != toSkipPast) && nextCh!=0);
}

inline bool ReaderMgr::peekString(const XMLCh* const toPeek)
{
    return fCurReader->peekString(toPeek);
}

inline void ReaderMgr::setEntityHandler(XMLEntityHandler* const newHandler)
{
    fEntityHandler = newHandler;
}

inline void ReaderMgr::setXMLVersion(const XMLReader::XMLVersion version)
{
    fXMLVersion = version;
    fCurReader->setXMLVersion(version);
}

//
//  This is a simple class to temporarily change the 'throw at end of entity'
//  flag of the reader manager. There are some places where we need to
//  turn this on and off on a scoped basis.
//
class XMLPARSER_EXPORT ThrowEOEJanitor
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and destructor
    // -----------------------------------------------------------------------
    ThrowEOEJanitor(ReaderMgr* mgrTarget, const bool newValue) :

        fOld(mgrTarget->getThrowEOE())
        , fMgr(mgrTarget)
    {
        mgrTarget->setThrowEOE(newValue);
    }

    ~ThrowEOEJanitor()
    {
        fMgr->setThrowEOE(fOld);
    };

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------    
    ThrowEOEJanitor(const ThrowEOEJanitor&);
    ThrowEOEJanitor& operator=(const ThrowEOEJanitor&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fOld
    //      The previous value of the flag, which we replaced during ctor,
    //      and will replace during dtor.
    //
    //  fMgr
    //      A pointer to the reader manager we are going to set/reset the
    //      flag on.
    // -----------------------------------------------------------------------
    bool        fOld;
    ReaderMgr*  fMgr;
};

XERCES_CPP_NAMESPACE_END

#endif
