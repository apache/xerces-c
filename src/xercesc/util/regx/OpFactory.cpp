/*
 * Copyright 2001,2004 The Apache Software Foundation.
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
 * Revision 1.9  2004/09/08 13:56:47  peiyongz
 * Apache License Version 2.0
 *
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

CharOp* OpFactory::createCharOp(XMLInt32 data) {

	CharOp* tmpOp = new (fMemoryManager) CharOp(Op::O_CHAR, data, fMemoryManager);

	fOpVector->addElement(tmpOp);
	return tmpOp;
}

CharOp* OpFactory::createAnchorOp(XMLInt32 data) {

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
