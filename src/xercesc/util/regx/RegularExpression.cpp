/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2003 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.10  2003/05/16 06:01:57  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.9  2003/05/16 00:03:10  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.8  2003/05/15 18:42:54  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.7  2003/05/12 10:08:22  gareth
 * The correct file this time.
 *
 * Revision 1.5  2002/12/18 13:01:02  gareth
 * New functionality - tokenize and replace. Fixed REVISIT for case insensitive match. Patch by Jennifer Schachter.
 *
 * Revision 1.4  2002/11/04 15:17:00  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/10/15 18:56:02  knoaman
 * [Bug 13604] while loop never terminates.
 *
 * Revision 1.2  2002/03/18 19:29:53  knoaman
 * Change constant names to eliminate possible conflict with user defined ones.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:30  peiyongz
 * sane_include
 *
 * Revision 1.6  2002/01/02 20:09:11  knoaman
 * Fix for regular expression patterns that begin with ".".
 *
 * Revision 1.5  2001/10/09 12:20:25  tng
 * Leak fix: Need to delete fMatch if adopted.
 *
 * Revision 1.4  2001/05/11 21:50:58  knoaman
 * Schema updates and fixes.
 *
 * Revision 1.3  2001/05/11 13:26:46  tng
 * Copyright update.
 *
 * Revision 1.2  2001/05/03 18:17:42  knoaman
 * Some design changes:
 * o Changed the TokenFactory from a single static instance, to a
 *    normal class. Each RegularExpression object will have its own
 *    instance of TokenFactory, and that instance will be passed to
 *    other classes that need to use a TokenFactory to create Token
 *    objects (with the exception of RangeTokenMap).
 * o Added a new class RangeTokenMap to map a the different ranges
 *    in a given category to a specific RangeFactory object. In the old
 *    design RangeFactory had dual functionality (act as a Map, and as
 *    a factory for creating RangeToken(s)). The RangeTokenMap will
 *    have its own copy of the TokenFactory. There will be only one
 *    instance of the RangeTokenMap class, and that instance will be
 *    lazily deleted when XPlatformUtils::Terminate is called.
 *
 * Revision 1.1  2001/03/02 19:22:52  knoaman
 * Schema: Regular expression handling part I
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/RegularExpression.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/regx/RegxUtil.hpp>
#include <xercesc/util/regx/Match.hpp>
#include <xercesc/util/regx/RangeToken.hpp>
#include <xercesc/util/regx/RegxDefs.hpp>
#include <xercesc/util/regx/XMLUniCharacter.hpp>
#include <xercesc/util/regx/ParserForXMLSchema.hpp>
#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/ParseException.hpp>
#include <xercesc/util/IllegalArgumentException.hpp>
#include <xercesc/framework/XMLBuffer.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Static member data initialization
// ---------------------------------------------------------------------------
const unsigned int RegularExpression::MARK_PARENS = 1;
const unsigned int RegularExpression::IGNORE_CASE = 2;
const unsigned int RegularExpression::SINGLE_LINE = 4;
const unsigned int RegularExpression::MULTIPLE_LINE = 8;
const unsigned int RegularExpression::EXTENDED_COMMENT = 16;
const unsigned int RegularExpression::USE_UNICODE_CATEGORY = 32;
const unsigned int RegularExpression::UNICODE_WORD_BOUNDARY = 64;
const unsigned int RegularExpression::PROHIBIT_HEAD_CHARACTER_OPTIMIZATION = 128;
const unsigned int RegularExpression::PROHIBIT_FIXED_STRING_OPTIMIZATION = 256;
const unsigned int RegularExpression::XMLSCHEMA_MODE = 512;
const unsigned int RegularExpression::SPECIAL_COMMA = 1024;
const unsigned short RegularExpression::WT_IGNORE = 0;
const unsigned short RegularExpression::WT_LETTER = 1;
const unsigned short RegularExpression::WT_OTHER = 2;
RangeToken*          RegularExpression::fWordRange = 0;

// ---------------------------------------------------------------------------
//  RegularExpression::Context: Constructors and Destructor
// ---------------------------------------------------------------------------
RegularExpression::Context::Context(MemoryManager* const manager) :
    fInUse(false)
	, fAdoptMatch(false)
    , fStart(0)
	, fLimit(0)
	, fLength(0)
	, fSize(0)
	, fOffsets(0)
	, fMatch(0)
	, fString(0)
    , fMemoryManager(manager)
{
}

RegularExpression::Context::~Context()
{
	fMemoryManager->deallocate(fOffsets);//delete [] fOffsets;
    fMemoryManager->deallocate(fString);//delete [] fString;

	if (fAdoptMatch)
		delete fMatch;
}

// ---------------------------------------------------------------------------
//  RegularExpression::Context: Public methods
// ---------------------------------------------------------------------------
void RegularExpression::Context::reset(const XMLCh* const string
                                       , const int start, const int limit
                                       , const int noClosures)
{
    fMemoryManager->deallocate(fString);//delete [] fString;
    fString = XMLString::replicate(string, fMemoryManager);
	fStart = start;
	fLimit = limit;
	fLength = fLimit - fStart;
	fInUse = true;
	if (fAdoptMatch)
		delete fMatch;
	fMatch = 0;

	if (fOffsets == 0 || fSize != noClosures) {

		if (fOffsets)
            fMemoryManager->deallocate(fOffsets);//delete [] fOffsets;
		fOffsets = (int*) fMemoryManager->allocate(noClosures * sizeof(int));//new int[noClosures];
	}

	fSize = noClosures;

	for (int i = 0; i< fSize; i++)
		fOffsets[i] = -1;
}

bool RegularExpression::Context::nextCh(XMLInt32& ch, int& offset,
										const short direction)
{

	ch = fString[offset];

	if (RegxUtil::isHighSurrogate(ch)) {
		if ((offset + 1 < fLimit) && (direction > 0) &&
			RegxUtil::isLowSurrogate(fString[offset+1])) {

				ch = RegxUtil::composeFromSurrogate(ch, fString[++offset]);
		}
		else
			return false;
	}
	else if (RegxUtil::isLowSurrogate(ch)) {
		if ((offset - 1 >= 0) && (direction <= 0) &&
			RegxUtil::isHighSurrogate(fString[offset-1])) {

				ch = RegxUtil::composeFromSurrogate(fString[--offset], ch);
		}
		else
			return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
//  RegularExpression: Constructors and Destructors
// ---------------------------------------------------------------------------
RegularExpression::RegularExpression(const char* const pattern,
                                     MemoryManager* const manager)
	:fHasBackReferences(false),
	 fFixedStringOnly(false),
	 fNoGroups(0),
	 fMinLength(0),
	 fNoClosures(0),
	 fOptions(0),
	 fContext(0),
	 fBMPattern(0),
	 fPattern(0),
	 fFixedString(0),
	 fOperations(0),
	 fTokenTree(0),
	 fFirstChar(0),
     fOpFactory(manager),
     fTokenFactory(0),
     fMemoryManager(manager)
{
	try {

		XMLCh* tmpBuf = XMLString::transcode(pattern, fMemoryManager);
        ArrayJanitor<XMLCh> janBuf(tmpBuf, fMemoryManager);
		setPattern(tmpBuf);
	}
	catch (...) {

		cleanUp();
		throw;
	}
}

RegularExpression::RegularExpression(const char* const pattern,
									 const char* const options,
                                     MemoryManager* const manager)
	:fHasBackReferences(false),
	 fFixedStringOnly(false),
	 fNoGroups(0),
	 fMinLength(0),
	 fNoClosures(0),
	 fOptions(0),
	 fContext(0),
	 fBMPattern(0),
	 fPattern(0),
	 fFixedString(0),
	 fOperations(0),
	 fTokenTree(0),
	 fFirstChar(0),
     fOpFactory(manager),
     fTokenFactory(0),
     fMemoryManager(manager)
{
	try {

		XMLCh* tmpBuf = XMLString::transcode(pattern, fMemoryManager);
		ArrayJanitor<XMLCh> janBuf(tmpBuf, fMemoryManager);
		XMLCh* tmpOptions = XMLString::transcode(options, fMemoryManager);
		ArrayJanitor<XMLCh> janOps(tmpOptions, fMemoryManager);
		setPattern(tmpBuf, tmpOptions);
	}
	catch (...) {

		cleanUp();
		throw;
	}
}


RegularExpression::RegularExpression(const XMLCh* const pattern,
                                     MemoryManager* const manager)
	:fHasBackReferences(false),
	 fFixedStringOnly(false),
	 fNoGroups(0),
	 fMinLength(0),
	 fNoClosures(0),
	 fOptions(0),
	 fContext(0),
	 fBMPattern(0),
	 fPattern(0),
	 fFixedString(0),
	 fOperations(0),
	 fTokenTree(0),
	 fFirstChar(0),
     fOpFactory(manager),
     fTokenFactory(0),
     fMemoryManager(manager)
{
	try {

		setPattern(pattern);
	}
	catch (...) {

		cleanUp();
		throw;
	}
}

RegularExpression::RegularExpression(const XMLCh* const pattern,
									 const XMLCh* const options,
                                     MemoryManager* const manager)
	:fHasBackReferences(false),
	 fFixedStringOnly(false),
	 fNoGroups(0),
	 fMinLength(0),
	 fNoClosures(0),
	 fOptions(0),
	 fContext(0),
	 fBMPattern(0),
	 fPattern(0),
	 fFixedString(0),
	 fOperations(0),
	 fTokenTree(0),
	 fFirstChar(0),
     fOpFactory(manager),
     fTokenFactory(0),
     fMemoryManager(manager)
{
	try {

		setPattern(pattern, options);
	}
	catch (...) {

		cleanUp();
		throw;
	}
}

RegularExpression::~RegularExpression() {

	cleanUp();
}

// ---------------------------------------------------------------------------
//  RegularExpression: Setter methods
// ---------------------------------------------------------------------------
void RegularExpression::setPattern(const XMLCh* const pattern,
								   const XMLCh* const options) {

    fTokenFactory = new (fMemoryManager) TokenFactory(fMemoryManager);
	fOptions = parseOptions(options);
	fPattern = XMLString::replicate(pattern, fMemoryManager);

	RegxParser* regxParser = isSet(fOptions, XMLSCHEMA_MODE)
		? new (fMemoryManager) ParserForXMLSchema(fMemoryManager) 
        : new (fMemoryManager) RegxParser(fMemoryManager);

    if (regxParser) {
        regxParser->setTokenFactory(fTokenFactory);
    }

	Janitor<RegxParser> janRegxParser(regxParser);
	fTokenTree = regxParser->parse(fPattern, fOptions);
	fNoGroups = regxParser->getNoParen();
	fHasBackReferences = regxParser->hasBackReferences();
}

// ---------------------------------------------------------------------------
//  RegularExpression: Matching methods
// ---------------------------------------------------------------------------
bool RegularExpression::matches(const char* const expression) {

    XMLCh* tmpBuf = XMLString::transcode(expression, fMemoryManager);
    ArrayJanitor<XMLCh> janBuf(tmpBuf, fMemoryManager);
	return matches(tmpBuf, 0, XMLString::stringLen(tmpBuf), 0);
}

bool RegularExpression::matches(const char* const expression,
								const int start, const int end) {

	XMLCh* tmpBuf = XMLString::transcode(expression, fMemoryManager);
    ArrayJanitor<XMLCh> janBuf(tmpBuf, fMemoryManager);
	return matches(tmpBuf, start, end, 0);
}

bool RegularExpression::matches(const char* const expression,
								Match* const match)				{

	XMLCh* tmpBuf = XMLString::transcode(expression, fMemoryManager);
    ArrayJanitor<XMLCh> janBuf(tmpBuf, fMemoryManager);
	return matches(tmpBuf, 0, XMLString::stringLen(tmpBuf), match);
}

bool RegularExpression::matches(const char* const expression, const int start,
                                const int end, Match* const pMatch)				{

	XMLCh* tmpBuf = XMLString::transcode(expression, fMemoryManager);
    ArrayJanitor<XMLCh> janBuf(tmpBuf, fMemoryManager);
	return matches(tmpBuf, start, end, pMatch);
}


// ---------------------------------------------------------------------------
//  RegularExpression: Matching methods - Wide char version
// ---------------------------------------------------------------------------
bool RegularExpression::matches(const XMLCh* const expression) {

	return matches(expression, 0, XMLString::stringLen(expression), 0);
}

bool RegularExpression::matches(const XMLCh* const expression,
								const int start, const int end) {

	return matches(expression, start, end, 0);
}

bool RegularExpression::matches(const XMLCh* const expression,
								Match* const match)				{

	return matches(expression, 0, XMLString::stringLen(expression), match);
}

bool RegularExpression::matches(const XMLCh* const expression, const int start,
                                const int end, Match* const pMatch)				{
		
	if (fOperations == 0)
		prepare();

	Context* context = 0;
	Context* tmpContext = 0;
	int		 strLength = XMLString::stringLen(expression);

	{
		XMLMutexLock lockInit(&fMutex);

		if (fContext == 0)
			fContext = new (fMemoryManager) Context(fMemoryManager);

		if (fContext->fInUse) {
			context = new (fMemoryManager) Context(fMemoryManager);
			tmpContext = context;
		}
		else {
			context = fContext;
		}

		context->reset(expression, start, end, fNoClosures);
	}

	Janitor<Context> janContext(tmpContext);

	bool adoptMatch = false;
	Match* lMatch = pMatch;

	if (lMatch != 0) {
		lMatch->setNoGroups(fNoGroups);
	}
	else if (fHasBackReferences) {

		lMatch = new (fMemoryManager) Match(fMemoryManager);
		lMatch->setNoGroups(fNoGroups);
		adoptMatch = true;
	}

	if (context->fAdoptMatch)
		delete context->fMatch;
    context->fMatch = lMatch;
	context->fAdoptMatch = adoptMatch;

	if (isSet(fOptions, XMLSCHEMA_MODE)) {

		int matchEnd = match(context, fOperations, context->fStart, 1);

		if (matchEnd == context->fLimit) {

			if (context->fMatch != 0) {

				context->fMatch->setStartPos(0, context->fStart);
				context->fMatch->setEndPos(0, matchEnd);
			}

			context->fInUse = false;
			return true;
		}

		return false;
	}

	/*
	 *	If the pattern has only fixed string, use Boyer-Moore
	 */
	if (fFixedStringOnly) {

		int ret = fBMPattern->matches(expression, context->fStart,
			                          context->fLimit);
		if (ret >= 0) {

			if (context->fMatch != 0) {
				context->fMatch->setStartPos(0, ret);
				context->fMatch->setEndPos(0, ret + strLength);
			}

			context->fInUse = false;
			return true;
		}
		context->fInUse = false;
		return false;
	}

	/*
	 *	If the pattern contains a fixed string, we check with Boyer-Moore
	 *	whether the text contains the fixed string or not. If not found
	 *	return false
	 */
	if (fFixedString != 0) {

		int ret = fBMPattern->matches(expression, context->fStart,
                                      context->fLimit);

		if (ret < 0) { // No match

			context->fInUse = false;
			return false;
		}
	}

	int limit = context->fLimit - fMinLength;
	int matchStart;
	int matchEnd = -1;

	/*
	 *	Check whether the expression start with ".*"
	 */
	if (fOperations != 0 && fOperations->getOpType() == Op::O_CLOSURE
        && fOperations->getChild()->getOpType() == Op::O_DOT) {

		if (isSet(fOptions, SINGLE_LINE)) {
			matchStart = context->fStart;
			matchEnd = match(context, fOperations, matchStart, 1);
		}
		else {
			bool previousIsEOL = true;

			for (matchStart=context->fStart; matchStart<=limit; matchStart++) {

				XMLCh ch = expression[matchStart];
				if (RegxUtil::isEOLChar(ch)) {
					previousIsEOL = true;
				}
				else {

					if (previousIsEOL) {
						if (0 <= (matchEnd = match(context, fOperations,
                                                   matchStart, 1)))
                            break;
					}

					previousIsEOL = false;
				}
			}
		}
	}
	else {
        /*
         *	Optimization against the first char
         */
		if (fFirstChar != 0) {
			bool ignoreCase = isSet(fOptions, IGNORE_CASE);
			RangeToken* range = fFirstChar;

			if (ignoreCase)
				range = fFirstChar->getCaseInsensitiveToken(fTokenFactory);

			for (matchStart=context->fStart; matchStart<=limit; matchStart++) {

                XMLInt32 ch;

				if (!context->nextCh(ch, matchStart, 1))
					break;

				if (!range->match(ch)) {

					if (!ignoreCase)
						continue;

					// Perform case insensitive match
					// REVISIT
					continue;
				}

				if (0 <= (matchEnd = match(context,fOperations,matchStart,1)))
					break;
            }
		}
		else {

            /*
             *	Straightforward matching
             */
			for (matchStart=context->fStart; matchStart<=limit; matchStart++) {

				if (0 <= (matchEnd = match(context,fOperations,matchStart,1)))
					break;
			}
		}
	}

	if (matchEnd >= 0) {

		if (context->fMatch != 0) {

			context->fMatch->setStartPos(0, matchStart);
			context->fMatch->setEndPos(0, matchEnd);
		}

		context->fInUse = false;
		return true;
	}

	context->fInUse = false;
	return false;
}

// ---------------------------------------------------------------------------
//  RegularExpression: Tokenize methods
// ---------------------------------------------------------------------------
RefArrayVectorOf<XMLCh>* RegularExpression::tokenize(const char* const expression) {

  XMLCh* tmpBuf = XMLString::transcode(expression);
  ArrayJanitor<XMLCh> janBuf(tmpBuf);
  return tokenize(tmpBuf, 0, XMLString::stringLen(tmpBuf));
}

RefArrayVectorOf<XMLCh>* RegularExpression::tokenize(const char* const expression,
								const int start, const int end) {

  XMLCh* tmpBuf = XMLString::transcode(expression);
  ArrayJanitor<XMLCh> janBuf(tmpBuf);
  return tokenize(tmpBuf, start, end);
}



// ---------------------------------------------------------------------------
//  RegularExpression: Tokenize methods - Wide char version
// ---------------------------------------------------------------------------
RefArrayVectorOf<XMLCh>* RegularExpression::tokenize(const XMLCh* const expression) {
  return tokenize(expression, 0, XMLString::stringLen(expression), 0);
}

RefArrayVectorOf<XMLCh>* RegularExpression::tokenize(const XMLCh* const expression,
								                                     const int start, const int end)
{
  return tokenize(expression, start, end, 0);
}

RefArrayVectorOf<XMLCh>* RegularExpression::tokenize(const XMLCh* const expression, 
                                                     const int start, const int end,
                                                     RefVectorOf<Match> *subEx){
  
  if (fOperations == 0)
	  prepare();

  RefArrayVectorOf<XMLCh>* tokenStack = new (fMemoryManager) RefArrayVectorOf<XMLCh>(16, true);

  Context* context = 0;
  Context* tmpContext = 0;

  int		 strLength = XMLString::stringLen(expression);
 
  {
 	   XMLMutexLock lockInit(&fMutex);

 	   if (fContext == 0)
 	     fContext = new (fMemoryManager) Context(fMemoryManager);

 	   if (fContext->fInUse) {
 	     context = new (fMemoryManager) Context(fMemoryManager);
 	     tmpContext = context;
 	   }
 	   else {
 	     context = fContext;
 	   }

 	   context->reset(expression, start, end, fNoClosures);
  }

  Janitor<Context> janContext(tmpContext);

  Match* lMatch = 0;
  bool adoptMatch = false;

  if (subEx || fHasBackReferences) {
    lMatch = new (fMemoryManager) Match(fMemoryManager);
    adoptMatch = true;
    lMatch->setNoGroups(fNoGroups);
  }

  if (context->fAdoptMatch)
 	  delete context->fMatch;
  
  context->fMatch = lMatch;
  context->fAdoptMatch = adoptMatch;

  int tokStart = start;
  int matchStart = start;

  for (; matchStart <= end; matchStart++) { 
  
 	  int matchEnd = match(context, fOperations, matchStart, 1);
  
 	  if (matchEnd != -1) {

 	    if (context->fMatch != 0) {
 	      context->fMatch->setStartPos(0, context->fStart);
 	      context->fMatch->setEndPos(0, matchEnd);
 	    }

      if (subEx){
        subEx->addElement(lMatch);
        lMatch = new (fMemoryManager) Match(*(context->fMatch));
        adoptMatch = true;
        
        context->fAdoptMatch = adoptMatch;
        context->fMatch = lMatch;
      }
  
 	    context->fInUse = false;

      XMLCh* token;
      if (tokStart == matchStart){
  
        if (tokStart == strLength){
          tokStart--;
          break;  
        }

        token = (XMLCh*) fMemoryManager->allocate(sizeof(XMLCh));//new XMLCh[1];
        token[0] = chNull;

        // When you tokenize using zero string, will return each
        // token in the string. Since the zero string will also 
        // match the start/end characters, resulting in empty 
        // tokens, we ignore them and do not add them to the stack. 
        if (!XMLString::equals(fPattern, &chNull)) 
          tokenStack->addElement(token); 
        else
            fMemoryManager->deallocate(token);//delete[] token;

      } else {
        token = (XMLCh*) fMemoryManager->allocate
        (
            (matchStart + 1 - tokStart) * sizeof(XMLCh)
        );//new XMLCh[matchStart + 1 - tokStart];
        XMLString::subString(token, expression, tokStart, matchStart);
        tokenStack->addElement(token);
      } 

      tokStart = matchEnd;

      //decrement matchStart as will increment it at the top of the loop
      if (matchStart < matchEnd - 1) 
        matchStart = matchEnd - 1; 	    
    }
  }
 
  XMLCh* token;
 
  if (matchStart == tokStart + 1){
    token = (XMLCh*) fMemoryManager->allocate(sizeof(XMLCh));//new XMLCh[1];
    token[0] = chNull;
  
  } else {
    token = (XMLCh*) fMemoryManager->allocate
    (
        (strLength + 1 - tokStart) * sizeof(XMLCh)
    );//new XMLCh[strLength + 1 - tokStart];
    XMLString::subString(token, expression, tokStart, strLength);
  }  

  if (!XMLString::equals(fPattern, &chNull)) 
    tokenStack->addElement(token);
  else
    fMemoryManager->deallocate(token);//delete[] token;

  return tokenStack;

}


// -----------------------------------------------------------------------
//  RegularExpression: Replace methods
// -----------------------------------------------------------------------
XMLCh* RegularExpression::replace(const char* const matchString, 
                                  const char* const replaceString){

	XMLCh* tmpBuf = XMLString::transcode(matchString);
    ArrayJanitor<XMLCh> janBuf(tmpBuf);
	XMLCh* tmpBuf2 = XMLString::transcode(replaceString);
    ArrayJanitor<XMLCh> janBuf2(tmpBuf2);

	return replace(tmpBuf, tmpBuf2, 0, XMLString::stringLen(tmpBuf));
}

XMLCh* RegularExpression::replace(const char* const matchString, 
                                  const char* const replaceString,
                                  const int start, const int end){

 	XMLCh* tmpBuf = XMLString::transcode(matchString);
    ArrayJanitor<XMLCh> janBuf(tmpBuf);
 	XMLCh* tmpBuf2 = XMLString::transcode(replaceString);
    ArrayJanitor<XMLCh> janBuf2(tmpBuf2);
  
  return replace(tmpBuf, tmpBuf2, start, end);
}


// ---------------------------------------------------------------------------
//  RegularExpression: Replace methods - Wide char version
// ---------------------------------------------------------------------------
XMLCh* RegularExpression::replace(const XMLCh* const matchString, 
                                  const XMLCh* const replaceString){

  return replace(matchString, replaceString, 0, 
                 XMLString::stringLen(matchString));
}

XMLCh* RegularExpression::replace(const XMLCh* const matchString,  
                                  const XMLCh* const replaceString,
                                  const int start, const int end)
{
  //check if matches zero length string - throw error if so
  if (matches(XMLUni::fgZeroLenString)){
		ThrowXML(RuntimeException, XMLExcepts::Regex_RepPatMatchesZeroString);
  }
      
  RefVectorOf<Match> *subEx = new (fMemoryManager) RefVectorOf<Match>(10, true);
	  Janitor<RefVectorOf<Match> > janSubEx(subEx);

  //Call to tokenize with Match vector so that we keep track of the locations
  //of the subExpression within each of the matches
  RefArrayVectorOf<XMLCh>* tokenStack = tokenize(matchString, start, end, subEx);
	  Janitor<RefArrayVectorOf<XMLCh> > janTokStack(tokenStack);
    
  XMLBuffer result;
  
  int numSubEx = 0;
  
  if (subEx && subEx->size() > 0)
    numSubEx = subEx->elementAt(0)->getNoGroups() - 1;
  
  int tokStackSize = tokenStack->size();
  const XMLCh* curRepString = XMLString::replicate(replaceString);
    
  for (int i = 0; i < tokStackSize; i++){
      
    result.append(tokenStack->elementAt(i));
  
    if (i != tokStackSize - 1) {
       
      //if there are subExpressions, then determine the string we want to 
      //substitute in.
        if (numSubEx != 0) {
            delete[] (XMLCh*)curRepString;
            curRepString = subInExp(replaceString, matchString, subEx->elementAt(i));     
        }
      result.append(curRepString);
    }
  }  
    
  delete[] (XMLCh*)curRepString;
  return XMLString::replicate(result.getRawBuffer()); 
    
}

// ---------------------------------------------------------------------------
//  RegularExpression: Helpers methods
// ---------------------------------------------------------------------------
int RegularExpression::getOptionValue(const XMLCh ch) {

	int ret = 0;

	switch (ch) {

		case chLatin_i:
			ret = IGNORE_CASE;
			break;
		case chLatin_m:
			ret = MULTIPLE_LINE;
			break;
		case chLatin_s:
			ret = SINGLE_LINE;
			break;
		case chLatin_x:
			ret = EXTENDED_COMMENT;
			break;
		case chLatin_u:
			ret = USE_UNICODE_CATEGORY;
			break;
		case chLatin_w:
			ret = UNICODE_WORD_BOUNDARY;
			break;
		case chLatin_F:
			ret = PROHIBIT_FIXED_STRING_OPTIMIZATION;
			break;
		case chLatin_H:
			ret = PROHIBIT_HEAD_CHARACTER_OPTIMIZATION;
			break;
		case chLatin_X:
			ret = XMLSCHEMA_MODE;
			break;
		case chComma:
			ret = SPECIAL_COMMA;
			break;
		default:
			break;
	}

	return ret;
}


int RegularExpression::match(Context* const context, const Op* const operations
							 , int offset, const short direction)
{
	const Op* tmpOp = operations;
	bool ignoreCase = isSet(fOptions, IGNORE_CASE);

	while (true) {

		if (tmpOp == 0)
			break;

		if (offset > context->fLimit || offset < context->fStart)
			return -1;

		switch(tmpOp->getOpType()) {
		case Op::O_CHAR:
			if (!matchChar(context, tmpOp->getData(), offset, direction,
						   ignoreCase))
				return -1;
			tmpOp = tmpOp->getNextOp();
			break;
		case Op::O_DOT:
			if (!matchDot(context, offset, direction))
				return -1;
			tmpOp = tmpOp->getNextOp();
			break;
		case Op::O_RANGE:
		case Op::O_NRANGE:
			if (!matchRange(context, tmpOp, offset, direction, ignoreCase))
				return -1;
			tmpOp = tmpOp->getNextOp();
			break;
		case Op::O_ANCHOR:
			if (!matchAnchor(context, tmpOp->getData(), offset))
				return -1;
			tmpOp = tmpOp->getNextOp();
			break;
		case Op::O_BACKREFERENCE:
			if (!matchBackReference(context, tmpOp->getData(), offset,
									direction, ignoreCase))
				return -1;
			tmpOp = tmpOp->getNextOp();
			break;
		case Op::O_STRING:
			if (!matchString(context, tmpOp->getLiteral(), offset, direction,
							 ignoreCase))
				return -1;
			tmpOp = tmpOp->getNextOp();
			break;
		case Op::O_CLOSURE:
			{
				XMLInt32 id = tmpOp->getData();
				if (id >= 0) {
					int prevOffset = context->fOffsets[id];
					if (prevOffset < 0 || prevOffset != offset) {
						context->fOffsets[id] = offset;
					}
					else {

						context->fOffsets[id] = -1;
						tmpOp = tmpOp->getNextOp();
						break;
					}
				}

				int ret = match(context, tmpOp->getChild(), offset, direction);
				if (id >= 0) {
					context->fOffsets[id] = -1;
				}

				if (ret >= 0)
					return ret;

				tmpOp = tmpOp->getNextOp();
			}
			break;
		case Op::O_QUESTION:
			{
				int ret = match(context, tmpOp->getChild(), offset, direction);
				if (ret >= 0)
					return ret;
				tmpOp = tmpOp->getNextOp();
			}
			break;
		case Op::O_NONGREEDYCLOSURE:
		case Op::O_NONGREEDYQUESTION:
			{
				int ret = match(context,tmpOp->getNextOp(),offset,direction);
				if (ret >= 0)
					return ret;
				tmpOp = tmpOp->getChild();
			}
			break;
		case Op::O_UNION:
			{
				return matchUnion(context, tmpOp, offset, direction);
			}
		case Op::O_CAPTURE:
			if (context->fMatch != 0 && tmpOp->getData() != 0)
				return matchCapture(context, tmpOp, offset, direction);
			tmpOp = tmpOp->getNextOp();
			break;
		case Op::O_LOOKAHEAD:
			if (0 > match(context, tmpOp->getChild(), offset, 1))
				return -1;
			tmpOp = tmpOp->getNextOp();
			break;
		case Op::O_NEGATIVELOOKAHEAD:
			if (0 <= match(context, tmpOp->getChild(), offset, 1))
				return -1;
			tmpOp = tmpOp->getNextOp();
			break;
		case Op::O_LOOKBEHIND:
			if (0 > match(context, tmpOp->getChild(), offset, -1))
				return - 1;
			tmpOp = tmpOp->getNextOp();
			break;
		case Op::O_NEGATIVELOOKBEHIND:
			if (0 <= match(context, tmpOp->getChild(), offset, -1))
				return -1;
			tmpOp = tmpOp->getNextOp();
			break;
		case Op::O_INDEPENDENT:
        case Op::O_MODIFIER:
			{
				int ret = (tmpOp->getOpType() == Op::O_INDEPENDENT)
					   ? match(context, tmpOp->getChild(), offset, direction)
                       : matchModifier(context, tmpOp, offset, direction);
                if (ret < 0)
                    return ret;
				offset = ret;
				tmpOp = tmpOp->getNextOp();
			}
			break;
		case Op::O_CONDITION:
			if (tmpOp->getRefNo() >= fNoGroups)
				return -1;
			if (matchCondition(context, tmpOp, offset, direction))
				tmpOp = tmpOp->getYesFlow();
			else
				if (tmpOp->getNoFlow() != 0)
                    tmpOp = tmpOp->getNoFlow();
                else
                    tmpOp = tmpOp->getNextOp();
			break;
		}
	}
	
	return offset;
}
bool RegularExpression::matchChar(Context* const context,
								  const XMLInt32 ch, int& offset,
								  const short direction, const bool ignoreCase)
{
	int tmpOffset = direction > 0 ? offset : offset - 1;

	if (tmpOffset >= context->fLimit || tmpOffset < 0)
		return false;

	XMLInt32 strCh = 0;
	
	if (!context->nextCh(strCh, tmpOffset, direction))
		return false;

	bool match = ignoreCase ? matchIgnoreCase(ch, strCh)
		                    : (ch == strCh);
	if (!match)
		return false;

	offset = (direction > 0) ? ++tmpOffset : tmpOffset;

	return true;
}

bool RegularExpression::matchDot(Context* const context, int& offset,
								 const short direction)
{
	int tmpOffset = direction > 0 ? offset : offset - 1;

	if (tmpOffset >= context->fLimit || tmpOffset < 0)
		return false;

	XMLInt32 strCh = 0;
	
	if (!context->nextCh(strCh, tmpOffset, direction))
		return false;

	if (!isSet(fOptions, SINGLE_LINE)) {

		if (direction > 0 && RegxUtil::isEOLChar(strCh))
			return false;

		if (direction <= 0 && !RegxUtil::isEOLChar(strCh) )
			return false;
	}

    offset = (direction > 0) ? ++tmpOffset : tmpOffset;
	return true;
}

bool RegularExpression::matchRange(Context* const context, const Op* const op,
								   int& offset, const short direction,
								   const bool ignoreCase)
{
	int tmpOffset = direction > 0 ? offset : offset - 1;

	if (tmpOffset >= context->fLimit || tmpOffset < 0)
		return false;

	XMLInt32 strCh = 0;
	
	if (!context->nextCh(strCh, tmpOffset, direction))
		return false;

	RangeToken* tok = (RangeToken *) op->getToken();
	bool match = false;

	if (ignoreCase) {

		//REVISIT we should match ignoring case, but for now
		//we will do a normal match
		//tok = tok->getCaseInsensitiveToken();
		//if (!token->match(strCh)) {

		//	if (strCh > 0x10000)
		//		return -1;
			// Do case insensitive matching - uppercase match
			// or lowercase match
		//}
		match = tok->match(strCh);
	}
	else
		match = tok->match(strCh);

	if (!match)
		return false;

	offset = (direction > 0) ? ++tmpOffset : tmpOffset;

	return true;
}

bool RegularExpression::matchAnchor(Context* const context, const XMLInt32 ch,
									const int offset)
{
	switch ((XMLCh) ch) {
	case chLatin_A:
		if (offset != context->fStart)
			return false;
		break;
	case chLatin_B:
		if (context->fLength == 0)
			break;
		{
			int after = getWordType(context->fString, context->fStart,
									context->fLimit, offset);
			if (after == WT_IGNORE
				|| after == getPreviousWordType(context->fString,
												context->fStart,
												context->fLimit, offset))
				break;
		}
		return false;
	case chLatin_b:
		if (context->fLength == 0)
			return false;
		{
			int after = getWordType(context->fString, context->fStart,
									context->fLimit, offset);
			if (after == WT_IGNORE
				|| after == getPreviousWordType(context->fString,
												context->fStart
												, context->fLimit, offset))
				return false;
		}
		break;
	case chLatin_Z:
	case chDollarSign:
		if ( (XMLCh) ch == chDollarSign && isSet(fOptions, MULTIPLE_LINE)) {
			if (!(offset == context->fLimit || (offset < context->fLimit
				&& RegxUtil::isEOLChar(context->fString[offset]))))
				return false;
		}
		else {

			if (!(offset == context->fLimit
				|| (offset+1 == context->fLimit
				    && RegxUtil::isEOLChar(context->fString[offset]))
				|| (offset+2 == context->fLimit
				    && context->fString[offset] == chCR
					&& context->fString[offset+1] == chLF)))
				return false;
		}
		break;
	case chLatin_z:
		if (offset != context->fLimit)
			return false;
		break;
	case chAt:
	case chCaret:
		if ( (XMLCh) ch == chCaret && !isSet(fOptions, MULTIPLE_LINE)) {

			if (offset != context->fStart)
				return false;
		}
		else {

			if (!(offset == context->fStart || (offset > context->fStart
				      && RegxUtil::isEOLChar(context->fString[offset-1]))))
				return false;
		}
		break;
	case chOpenAngle:
		if (context->fLength == 0 || offset == context->fLimit)
			return false;

		if (getWordType(context->fString, context->fStart, context->fLimit,
						offset) != WT_LETTER
			|| getPreviousWordType(context->fString, context->fStart,
								   context->fLimit, offset) != WT_OTHER)
			return false;
		break;
	case chCloseAngle:
		if (context->fLength == 0 || offset == context->fStart)
			return false;

		if (getWordType(context->fString, context->fStart, context->fLimit,
						offset) != WT_OTHER
			|| getPreviousWordType(context->fString, context->fStart,
								   context->fLimit, offset) != WT_LETTER)
			return false;
		break;
	}

	return true;
}

bool RegularExpression::matchBackReference(Context* const context,
										   const XMLInt32 refNo, int& offset,
										   const short direction,
										   const bool ignoreCase)
{
	if (refNo <=0 || refNo >= fNoGroups)
		ThrowXML(IllegalArgumentException, XMLExcepts::Regex_BadRefNo);

	if (context->fMatch->getStartPos(refNo) < 0
		|| context->fMatch->getEndPos(refNo) < 0)
		return false;

	int start = context->fMatch->getStartPos(refNo);
	int length = context->fMatch->getEndPos(refNo) - start;
	int tmpOffset = (direction > 0) ? offset : offset - length;

	if (context->fLimit - tmpOffset < length)
		return false;

	bool match = ignoreCase
					? XMLString::regionIMatches(context->fString,tmpOffset,
												context->fString,start,length)
					: XMLString::regionMatches(context->fString, tmpOffset,
											   context->fString, start,length);

	if (!match)
		return false;

	offset = (direction > 0) ? offset + length : offset - length;
	return true;
}

bool RegularExpression::matchString(Context* const context,
									const XMLCh* const literal, int& offset,
									const short direction, const bool ignoreCase)
{
	int length = XMLString::stringLen(literal);
	int tmpOffset = (direction > 0) ? offset : offset - length;

	if (context->fLimit - tmpOffset < length)
		return false;

	bool match = ignoreCase
					? XMLString::regionIMatches(context->fString, tmpOffset,
												literal, 0, length)
					: XMLString::regionMatches(context->fString, tmpOffset,
											   literal, 0, length);

	if (match) {
	    offset = direction > 0 ? offset + length : offset - length;
    }

	return match;
}

int RegularExpression::matchCapture(Context* const context, const Op* const op,
                                    int offset, const short direction)
{
	// No check is made for nullness of fMatch as the function is only called if
	// fMatch is not null.
	XMLInt32 index = op->getData();
	int save = (index > 0) ? context->fMatch->getStartPos(index)
                           : context->fMatch->getEndPos(-index);

	if (index > 0) {

		context->fMatch->setStartPos(index, offset);
		int ret = match(context, op->getNextOp(), offset, direction);
		if (ret < 0)
			context->fMatch->setStartPos(index, save);
		return ret;
	}
	
	context->fMatch->setEndPos(-index, offset);
	int ret = match(context, op->getNextOp(), offset, direction);
	if (ret < 0)
		context->fMatch->setEndPos(-index, save);
	return ret;
}

bool RegularExpression::matchCondition(Context* const context,
                                              const Op* const op, int offset,
                                              const short direction)
{

	int refNo = op->getRefNo();
	if ( refNo > 0)
		return (context->fMatch->getStartPos(refNo) >= 0
                && context->fMatch->getEndPos(refNo) >= 0);

	return (0 <= match(context, op->getConditionFlow(), offset, direction));
}

int RegularExpression::parseOptions(const XMLCh* const options)
{

	if (options == 0)
		return 0;

	int opts = 0;
	int length = XMLString::stringLen(options);

	for (int i=0; i < length; i++) {
	
		int v = getOptionValue(options[i]);

		if (v == 0)
			ThrowXML1(ParseException, XMLExcepts::Regex_UnknownOption, options);

		opts |= v;
	}

	return opts;
}

void RegularExpression::compile(const Token* const token) {

	if (fOperations != 0)
		return;

	fNoClosures = 0;
	fOperations = compile(token, 0, false);
}

Op* RegularExpression::compile(const Token* const token, Op* const next,
							   const bool reverse) {

	Op* ret = 0;

	const unsigned short tokenType = token->getTokenType();

	switch(tokenType) {
	case Token::T_DOT:
	case Token::T_CHAR:
	case Token::T_ANCHOR:
	case Token::T_RANGE:
	case Token::T_NRANGE:
	case Token::T_STRING:
	case Token::T_BACKREFERENCE:
	case Token::T_EMPTY:
		ret = compileSingle(token, next, tokenType);
		break;
	case Token::T_CONCAT:
		ret = compileConcat(token, next, reverse);
		break;
	case Token::T_UNION:
		ret = compileUnion(token, next, reverse);
		break;
	case Token::T_CLOSURE:
	case Token::T_NONGREEDYCLOSURE:
		ret = compileClosure(token, next, reverse, tokenType);
		break;
	case Token::T_PAREN:
		ret = compileParenthesis(token, next, reverse);
		break;
	case Token::T_LOOKAHEAD:
	case Token::T_NEGATIVELOOKAHEAD:
		ret = compileLook(token, next, false, tokenType);
		break;
	case Token::T_LOOKBEHIND:
	case Token::T_NEGATIVELOOKBEHIND:
		ret = compileLook(token, next, true, tokenType);
		break;
	case Token::T_INDEPENDENT:
	case Token::T_MODIFIERGROUP:
		ret = compileLook(token, next, reverse, tokenType);
		break;
	case Token::T_CONDITION:
		ret = compileCondition(token, next, reverse);
		break;
	default:
		ThrowXML(RuntimeException, XMLExcepts::Regex_UnknownTokenType);
		break; // this line to be deleted
	}

	return ret;
}

/*
 * Helper for Replace. This method prepares the replacement string by substituting
 * in actual values for parenthesized sub expressions. 
 *
 * An error will be thrown if:
 *  1) repString references an undefined subExpression
 *  2) there is an unescaped chDollar which is not followed by a digit
 *
 */
const XMLCh* RegularExpression::subInExp(const XMLCh* const repString, 
                                         const XMLCh* const origString, 
                                         const Match* subEx){

  int numSubExp = subEx->getNoGroups() - 1;

  if (numSubExp == 0)
    return XMLString::replicate(repString);
  
  bool notEscaped = true;                 
  
  XMLBuffer newString;                   
  
  XMLCh indexStr[2]; //holds the string rep of a 

  indexStr[1] = chNull;
  int index = -1;

  for (const XMLCh* ptr = repString; *ptr != chNull; ptr++){

    if ((*ptr == chDollarSign) && notEscaped) {
      
      ptr++;
      
      //check that after the $ is a digit 
      if (!XMLString::isDigit(*ptr)){
       
        //invalid replace string - $ must be followed by a digit
				ThrowXML(RuntimeException, XMLExcepts::Regex_InvalidRepPattern);
      }
        
      indexStr[0] = *ptr;                     //get the digit 
      index = XMLString::parseInt(indexStr);  //convert it to an int

      //now check that the index is legal
      if (index > numSubExp){
				ThrowXML(RuntimeException, XMLExcepts::Regex_InvalidRepPattern);
      }
        
      int start = subEx->getStartPos(index);
      int end = subEx->getEndPos(index);

      //now copy the substring into the new string
      for (int i=start; i<end; i++){
        newString.append(origString[i]);
      }
          
    } else {
 
      //if you have a slash and then a character that's not a $ or /, 
      //then it's an invalid replace string  
      if (!notEscaped && (*ptr != chDollarSign && *ptr != chBackSlash)){
				ThrowXML(RuntimeException, XMLExcepts::Regex_InvalidRepPattern);
      }
      
      if (*ptr == chBackSlash){
        notEscaped = false;
        continue;
        
      }else   
        notEscaped = true;  

      newString.append(*ptr);
    }
  }

  return XMLString::replicate(newString.getRawBuffer());
       
}


/*
 * Prepares for matching. This method is called just before starting matching
 */
void RegularExpression::prepare() {

	XMLMutexLock lockInit(&fMutex);

	compile(fTokenTree);

	fMinLength = fTokenTree->getMinLength();
	fFirstChar = 0;

	if (!isSet(fOptions, PROHIBIT_HEAD_CHARACTER_OPTIMIZATION) &&
		!isSet(fOptions, XMLSCHEMA_MODE))							{

		RangeToken* rangeTok = fTokenFactory->createRange();
		int result = fTokenTree->analyzeFirstCharacter(rangeTok, fOptions, fTokenFactory);

		if (result == Token::FC_TERMINAL) {

			rangeTok->compactRanges();
			fFirstChar = rangeTok;
		}
	}

	if (fOperations != 0 && fOperations->getNextOp() == 0 &&
		(fOperations->getOpType() == Op::O_STRING ||
		 fOperations->getOpType() == Op::O_CHAR) )			 {

		fFixedStringOnly = true;

		if (fOperations->getOpType() == Op::O_STRING) {
			fMemoryManager->deallocate(fFixedString);//delete [] fFixedString;
			fFixedString = XMLString::replicate(fOperations->getLiteral(), fMemoryManager);
		}
		else{
			
			XMLInt32 ch = fOperations->getData();

			if ( ch >= 0x10000) { // add as constant
				fMemoryManager->deallocate(fFixedString);//delete [] fFixedString;
				fFixedString = RegxUtil::decomposeToSurrogates(ch, fMemoryManager);
			}
			else {

				XMLCh* dummyStr = (XMLCh*) fMemoryManager->allocate(2 * sizeof(XMLCh));//new XMLCh[2];
				dummyStr[0] = (XMLCh) fOperations->getData();
				dummyStr[1] = chNull;
				fMemoryManager->deallocate(fFixedString);//delete [] fFixedString;
				fFixedString = dummyStr;
			}
		}

		fBMPattern = new (fMemoryManager) BMPattern(fFixedString, 256,
								  isSet(fOptions, IGNORE_CASE));
	}
	else if (!isSet(fOptions, XMLSCHEMA_MODE) &&
			 !isSet(fOptions, PROHIBIT_FIXED_STRING_OPTIMIZATION)) {

		int fixedOpts = 0;
		Token* tok = fTokenTree->findFixedString(fOptions, fixedOpts);

		fMemoryManager->deallocate(fFixedString);//delete [] fFixedString;

		fFixedString = (tok == 0) ? 0
			: XMLString::replicate(tok->getString(), fMemoryManager);

		if (fFixedString != 0 && XMLString::stringLen(fFixedString) < 2) {

			fMemoryManager->deallocate(fFixedString);//delete [] fFixedString;
			fFixedString = 0;
		}
		
		if (fFixedString != 0) {

			fBMPattern = new (fMemoryManager) BMPattern(fFixedString, 256,
									   isSet(fixedOpts, IGNORE_CASE));
		}
	}
}

unsigned short RegularExpression::getCharType(const XMLCh ch) {

    if (!isSet(fOptions, UNICODE_WORD_BOUNDARY)) {

		if (isSet(fOptions, USE_UNICODE_CATEGORY)) {

			if (fWordRange == 0) {

				fWordRange = fTokenFactory->getRange(fgUniIsWord);
				if (fWordRange == 0)
					ThrowXML1(RuntimeException, XMLExcepts::Regex_RangeTokenGetError, fgUniIsWord);
			}

			return fWordRange->match(ch) ? WT_LETTER : WT_OTHER;
		}

		return RegxUtil::isWordChar(ch);
    }

	switch (XMLUniCharacter::getType(ch)) {
	case XMLUniCharacter::UPPERCASE_LETTER:
	case XMLUniCharacter::LOWERCASE_LETTER:
	case XMLUniCharacter::TITLECASE_LETTER:
	case XMLUniCharacter::MODIFIER_LETTER:
	case XMLUniCharacter::OTHER_LETTER:
	case XMLUniCharacter::LETTER_NUMBER:
	case XMLUniCharacter::DECIMAL_DIGIT_NUMBER:
	case XMLUniCharacter::OTHER_NUMBER:
	case XMLUniCharacter::COMBINING_SPACING_MARK:
		return WT_LETTER;
	case XMLUniCharacter::FORMAT:
	case XMLUniCharacter::NON_SPACING_MARK:
	case XMLUniCharacter::ENCLOSING_MARK:
		return WT_IGNORE;
	case XMLUniCharacter::CONTROL:
		switch (ch) {
		case chHTab:
		case chLF:
		case chVTab:
		case chFF:
		case chCR:
			return WT_OTHER;
		default:
			return WT_IGNORE;
		}
	}

    return WT_OTHER;
}


XERCES_CPP_NAMESPACE_END

/**
  *	End of file RegularExpression.cpp
  */

