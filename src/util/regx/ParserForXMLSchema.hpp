/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
 * $Id$
 */

#if !defined(PARSERFORXMLSCHEMA_HPP)
#define PARSERFORXMLSCHEMA_HPP

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/regx/RegxParser.hpp>
#include <util/RuntimeException.hpp>

// ---------------------------------------------------------------------------
//  Forward Declaration
// ---------------------------------------------------------------------------
class Token;
class RangeToken;

class XMLUTIL_EXPORT ParserForXMLSchema : public RegxParser {
public:
    // -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
    ParserForXMLSchema();
    ~ParserForXMLSchema();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------

protected:
    // -----------------------------------------------------------------------
    //  Parsing/Processing methods
    // -----------------------------------------------------------------------
    XMLInt32    processCInCharacterClass(RangeToken* const tok,
                                         const XMLInt32 ch);
    Token*      processCaret();
    Token*      processDollar();
	Token*		processLook(const unsigned short tokType);
    Token*      processBacksolidus_A();
    Token*      processBacksolidus_Z();
    Token*      processBacksolidus_z();
    Token*      processBacksolidus_b();
    Token*      processBacksolidus_B();
    Token*      processBacksolidus_c();
    Token*      processBacksolidus_C();
    Token*      processBacksolidus_i();
    Token*      processBacksolidus_I();
    Token*      processBacksolidus_g();
    Token*      processBacksolidus_X();
    Token*      processBacksolidus_lt();
    Token*      processBacksolidus_gt();
    Token*      processStar(Token* const tok);
    Token*      processPlus(Token* const tok);
	Token*      processQuestion(Token* const tok);
	Token*      processParen();
	Token*      processParen2();
	Token*      processCondition();
	Token*      processModifiers();
	Token*      processIndependent();
	Token*      processBackReference();
	RangeToken* parseCharacterClass(const bool useNRange);
	RangeToken* parseSetOperations();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
	Token* getTokenForShorthand(const XMLInt32 ch);

    // -----------------------------------------------------------------------
    //  Helper methods
    // -----------------------------------------------------------------------
    bool checkQuestion(const int off);
	XMLInt32 decodeEscaped();

private:
	// -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    ParserForXMLSchema(const ParserForXMLSchema&);
    void operator=(const ParserForXMLSchema&);

    // -----------------------------------------------------------------------
    //  Private data members
	// -----------------------------------------------------------------------
};

// ---------------------------------------------------------------------------
//  ParserForXMLSchema: Helper methods
// ---------------------------------------------------------------------------
inline bool ParserForXMLSchema::checkQuestion(const int off) {

    return false;
}


// ---------------------------------------------------------------------------
//  ParserForXMLSchema: Parsing/Processing methods
// ---------------------------------------------------------------------------
inline Token* ParserForXMLSchema::processLook(const unsigned short tokType) {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processBacksolidus_A() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processBacksolidus_B() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processBacksolidus_b() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processBacksolidus_C() {

    processNext();
    return getTokenForShorthand(chLatin_C);
}

inline Token* ParserForXMLSchema::processBacksolidus_c() {

	processNext();
	return getTokenForShorthand(chLatin_c);
}

inline Token* ParserForXMLSchema::processBacksolidus_g() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processBacksolidus_gt() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processBacksolidus_I() {

	processNext();
	return getTokenForShorthand(chLatin_I);
}

inline Token* ParserForXMLSchema::processBacksolidus_i() {

	processNext();
	return getTokenForShorthand(chLatin_i);
}

inline Token* ParserForXMLSchema::processBacksolidus_lt() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processBacksolidus_X() {


	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processBacksolidus_Z() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processBacksolidus_z() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processBackReference() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processCondition() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processIndependent() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processModifiers() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline Token* ParserForXMLSchema::processParen2() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}

inline RangeToken* ParserForXMLSchema::parseSetOperations() {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
}
#endif

/**
  * End of file ParserForXMLSchema.hpp
  */
