/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * Revision 1.2  2002/11/04 15:17:00  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:29  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/06/01 14:15:36  knoaman
 * Add a return value to satisfy compilers that complain about
 * no return value, although that code will not be executed.
 *
 * Revision 1.2  2001/05/11 13:26:43  tng
 * Copyright update.
 *
 * Revision 1.1  2001/03/02 19:22:47  knoaman
 * Schema: Regular expression handling part I
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/Op.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Op: Constructors and Destructors
// ---------------------------------------------------------------------------
Op::Op(const short type) : fOpType(type),
                           fNextOp(0) {

}

// ---------------------------------------------------------------------------
//  Op: Getter methods
// ---------------------------------------------------------------------------
int Op::getSize() const {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
    return 0; // for compilers that complain about no return value
}

XMLInt32 Op::getData() const {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
    return 0; // for compilers that complain about no return value
}

XMLInt32 Op::getData2() const {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
    return 0; // for compilers that complain about no return value
}

int Op::getRefNo() const {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
    return 0; // for compilers that complain about no return value
}

const Op* Op::elementAt(int index) const {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
    return 0; // for compilers that complain about no return value
}

const Op* Op::getChild() const {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
    return 0; // for compilers that complain about no return value
}

const Op* Op::getConditionFlow() const {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
    return 0; // for compilers that complain about no return value
}

const Op* Op::getYesFlow() const {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
    return 0; // for compilers that complain about no return value
}

const Op* Op::getNoFlow() const {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
    return 0; // for compilers that complain about no return value
}
	
const XMLCh* Op::getLiteral() const {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
    return 0; // for compilers that complain about no return value
}
	
const Token* Op::getToken() const {

	ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
    return 0; // for compilers that complain about no return value
}


// ---------------------------------------------------------------------------
//  CharOp: Constructors and Destuctors
// ---------------------------------------------------------------------------
CharOp::CharOp(const short type, const XMLInt32 charData)
    : Op(type)
      , fCharData(charData) {
}

// ---------------------------------------------------------------------------
//  CharOp: Getter methods
// ---------------------------------------------------------------------------
XMLInt32 CharOp::getData() const {

	return fCharData;
}

// ---------------------------------------------------------------------------
//  UnionOp: Constructors and Destuctors
// ---------------------------------------------------------------------------
UnionOp::UnionOp(const short type, const int size)
    : Op(type)
      , fBranches(new RefVectorOf<Op> (size, false)) {

}

// ---------------------------------------------------------------------------
//  UnionOp: Getter/Setter methods
// ---------------------------------------------------------------------------
int UnionOp::getSize() const {

	return fBranches->size();
}

const Op* UnionOp::elementAt(int index) const {

	return fBranches->elementAt(index);
}

void UnionOp::addElement(Op* const op) {

	fBranches->addElement(op);
}

// ---------------------------------------------------------------------------
//  ChildOp: Constructors and Destuctors
// ---------------------------------------------------------------------------
ChildOp::ChildOp(const short type)
    : Op(type)
      , fChild(0) {

}

// ---------------------------------------------------------------------------
//  ChildOp: Getter/Setter methods
// ---------------------------------------------------------------------------
const Op* ChildOp::getChild() const {

	return fChild;
}

void ChildOp::setChild(const Op* const child) {

	fChild = child;
}

// ---------------------------------------------------------------------------
//  ModifierOp: Constructors and Destuctors
// ---------------------------------------------------------------------------
ModifierOp::ModifierOp(const short type, const XMLInt32 v1, const XMLInt32 v2)
    : ChildOp(type)
      , fVal1(v1)
      , fVal2(v2) {

}

// ---------------------------------------------------------------------------
//  ModifierOp: Getter methods
// ---------------------------------------------------------------------------
XMLInt32 ModifierOp::getData() const {

	return fVal1;
}

XMLInt32 ModifierOp::getData2() const {

	return fVal2;
}

// ---------------------------------------------------------------------------
//  RangeOp: Constructors and Destuctors
// ---------------------------------------------------------------------------
RangeOp::RangeOp(const short type, const Token* const token)
    : Op (type)
      , fToken(token) {

}

// ---------------------------------------------------------------------------
//  RangeOp: Getter methods
// ---------------------------------------------------------------------------
const Token* RangeOp::getToken() const {

	return fToken;
}


// ---------------------------------------------------------------------------
//  StringOp: Constructors and Destuctors
// ---------------------------------------------------------------------------
StringOp::StringOp(const short type, const XMLCh* const literal)
    : Op (type)
      , fLiteral(XMLString::replicate(literal)) {

}

// ---------------------------------------------------------------------------
//  StringOp: Getter methods
// ---------------------------------------------------------------------------
const XMLCh* StringOp::getLiteral() const {

	return fLiteral;
}

// ---------------------------------------------------------------------------
//  ConditionOp: Constructors and Destuctors
// ---------------------------------------------------------------------------
ConditionOp::ConditionOp(const short type, const int refNo,
                         const Op* const condFlow, const Op* const yesFlow,
                         const Op* const noFlow)
    : Op (type)
      , fRefNo(refNo)
      , fConditionOp(condFlow)
      , fYesOp(yesFlow)
      , fNoOp(noFlow) {

}

// ---------------------------------------------------------------------------
//  ConditionOp: Getter methods
// ---------------------------------------------------------------------------
int ConditionOp::getRefNo() const {
	
	return fRefNo;
}

const Op* ConditionOp::getConditionFlow() const {

	return fConditionOp;
}

const Op* ConditionOp::getYesFlow() const {

	return fYesOp;
}

const Op* ConditionOp::getNoFlow() const {

	return fNoOp;
}

XERCES_CPP_NAMESPACE_END

/**
  * End file Op.cpp
  */
