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
 * Revision 1.8  2004/02/16 10:11:28  amassari
 * Fixed warning with gcc 3.3
 *
 * Revision 1.7  2003/12/17 00:18:37  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.6  2003/05/18 14:02:06  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.5  2003/05/16 00:03:10  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.4  2003/05/15 18:42:54  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/11/04 15:17:00  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/03/18 19:29:53  knoaman
 * Change constant names to eliminate possible conflict with user defined ones.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:29  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/06/07 20:55:36  tng
 * Fix no newline at the end warning.  By Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/11 13:26:43  tng
 * Copyright update.
 *
 * Revision 1.1  2001/03/02 19:22:48  knoaman
 * Schema: Regular expression handling part I
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/Op.hpp>
#include <xercesc/util/regx/OpFactory.hpp>
#include <xercesc/util/PlatformUtils.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  OpFactory: Constructors and Destructor
// ---------------------------------------------------------------------------
OpFactory::OpFactory(MemoryManager* const manager) :
    fOpVector(0)
    , fMemoryManager(manager)
{
    fOpVector = new (fMemoryManager) RefVectorOf<Op>(16, true, fMemoryManager);
}

OpFactory::~OpFactory() {

	delete fOpVector;
	fOpVector = 0;
}

// ---------------------------------------------------------------------------
//  OpFactory - Factory methods
// ---------------------------------------------------------------------------
Op* OpFactory::createDotOp() {

	Op* tmpOp = new (fMemoryManager) Op(Op::O_DOT, fMemoryManager);
	fOpVector->addElement(tmpOp);
	return tmpOp;
}

CharOp* OpFactory::createCharOp(int data) {

	CharOp* tmpOp = new (fMemoryManager) CharOp(Op::O_CHAR, data, fMemoryManager);

	fOpVector->addElement(tmpOp);
	return tmpOp;
}

CharOp* OpFactory::createAnchorOp(int data) {

	CharOp* tmpOp = new (fMemoryManager) CharOp(Op::O_ANCHOR, data, fMemoryManager);

	fOpVector->addElement(tmpOp);
	return tmpOp;
}

CharOp* OpFactory::createCaptureOp(int number, const Op* const next) {

	CharOp* tmpOp = new (fMemoryManager) CharOp(Op::O_CAPTURE, number, fMemoryManager);

	tmpOp->setNextOp(next);
	fOpVector->addElement(tmpOp);
	return tmpOp;
}

UnionOp* OpFactory::createUnionOp(int size) {

	UnionOp* tmpOp = new (fMemoryManager) UnionOp(Op::O_UNION, size, fMemoryManager);

	fOpVector->addElement(tmpOp);
	return tmpOp;
}

ChildOp* OpFactory::createClosureOp(int id) {

	ModifierOp* tmpOp = new (fMemoryManager) ModifierOp(Op::O_CLOSURE, id, -1, fMemoryManager);

	fOpVector->addElement(tmpOp);
	return tmpOp;
}

ChildOp* OpFactory::createNonGreedyClosureOp() {

	ChildOp* tmpOp = new (fMemoryManager) ChildOp(Op::O_NONGREEDYCLOSURE, fMemoryManager);

	fOpVector->addElement(tmpOp);
	return tmpOp;
}

ChildOp* OpFactory::createQuestionOp(bool nonGreedy) {

	ChildOp* tmpOp = new (fMemoryManager)  ChildOp(nonGreedy ? Op::O_NONGREEDYQUESTION :
											 Op::O_QUESTION, fMemoryManager);

	fOpVector->addElement(tmpOp);
	return tmpOp;
}

RangeOp* OpFactory::createRangeOp(const Token* const token) {

	RangeOp* tmpOp = new (fMemoryManager)  RangeOp(Op::O_RANGE, token, fMemoryManager);
	
	fOpVector->addElement(tmpOp);
	return tmpOp;
}

ChildOp* OpFactory::createLookOp(const short type, const Op* const next,
						         const Op* const branch) {

	ChildOp* tmpOp = new (fMemoryManager) ChildOp(type, fMemoryManager);

	tmpOp->setNextOp(next);
	tmpOp->setChild(branch);
	fOpVector->addElement(tmpOp);
	return tmpOp;
}

CharOp* OpFactory::createBackReferenceOp(int refNo) {

	CharOp* tmpOp = new (fMemoryManager) CharOp(Op::O_BACKREFERENCE, refNo, fMemoryManager);

	fOpVector->addElement(tmpOp);
	return tmpOp;
}

StringOp* OpFactory::createStringOp(const XMLCh* const literal) {

	StringOp* tmpOp = new (fMemoryManager) StringOp(Op::O_STRING, literal, fMemoryManager);

	fOpVector->addElement(tmpOp);
	return tmpOp;
}

ChildOp* OpFactory::createIndependentOp(const Op* const next,
							            const Op* const branch) {

	ChildOp* tmpOp = new (fMemoryManager) ChildOp(Op::O_INDEPENDENT, fMemoryManager);

	tmpOp->setNextOp(next);
	tmpOp->setChild(branch);
	fOpVector->addElement(tmpOp);
	return tmpOp;
}

ModifierOp* OpFactory::createModifierOp(const Op* const next,
                                        const Op* const branch,
                                        const int add, const int mask) {

	ModifierOp* tmpOp = new (fMemoryManager) ModifierOp(Op::O_MODIFIER, add, mask, fMemoryManager);

	tmpOp->setNextOp(next);
	tmpOp->setChild(branch);
	return tmpOp;
}
	
ConditionOp* OpFactory::createConditionOp(const Op* const next, const int ref,
								          const Op* const conditionFlow,
								          const Op* const yesFlow,
								          const Op* const noFlow) {

	ConditionOp* tmpOp = new (fMemoryManager) ConditionOp(Op::O_CONDITION, ref, conditionFlow,
										 yesFlow, noFlow, fMemoryManager);

	tmpOp->setNextOp(next);
	return tmpOp;
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file OpFactory.cpp
  */
