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

#if !defined(OP_HPP)
#define OP_HPP

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XercesDefs.hpp>
#include <util/RefVectorOf.hpp>

// ---------------------------------------------------------------------------
//  Forward Declaration
// ---------------------------------------------------------------------------
class Token;


class XMLUTIL_EXPORT Op {
public:

	enum {
		DOT					= 0,
		CHAR				= 1,
		RANGE				= 3,
		NRANGE				= 4,
		ANCHOR				= 5,
		STRING				= 6,
		CLOSURE				= 7,
		NONGREEDYCLOSURE	= 8,
		QUESTION			= 9,
		NONGREEDYQUESTION	= 10,
		UNION				= 11,
		CAPTURE				= 15,
		BACKREFERENCE		= 16,
		LOOKAHEAD			= 20,
		NEGATIVELOOKAHEAD	= 21,
		LOOKBEHIND			= 22,
		NEGATIVELOOKBEHIND	= 23,
		INDEPENDENT			= 24,
		MODIFIER			= 25,
		CONDITION			= 26
	};

	// -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
	virtual ~Op() { }

	// -----------------------------------------------------------------------
	// Getter functions
	// -----------------------------------------------------------------------
	        short			getOpType() const;
	        const Op*		getNextOp() const;
	virtual XMLInt32		getData() const;
	virtual XMLInt32		getData2() const;
	virtual int				getSize() const;
	virtual int				getRefNo() const;
	virtual const Op*		getConditionFlow() const;
	virtual const Op*		getYesFlow() const;
	virtual const Op*		getNoFlow() const;
	virtual const Op*		elementAt(int index) const;
	virtual const Op*		getChild() const;
	virtual const Token*	getToken() const;
	virtual const XMLCh*	getLiteral() const;

	// -----------------------------------------------------------------------
	// Setter functions
	// -----------------------------------------------------------------------
	void setOpType(const short type);
	void setNextOp(const Op* const next);

protected:
	// -----------------------------------------------------------------------
    //  Protected Constructors
    // -----------------------------------------------------------------------
	Op(const short type);
	friend class OpFactory;

private:
	// -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    Op(const Op&);
    void operator=(const Op&);

	// -----------------------------------------------------------------------
    //  Private data members
    //
    //  fNextOp
    //      Points to the next operation in the chain
	//		
    //  fOpType
    //      Indicates the type of operation
    // -----------------------------------------------------------------------
	const Op*	fNextOp;
	short		fOpType;
};


class XMLUTIL_EXPORT CharOp: public Op {
public:
	// -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
	CharOp(const short type, const XMLInt32 charData);
	~CharOp() {}

	// -----------------------------------------------------------------------
	// Getter functions
	// -----------------------------------------------------------------------
	XMLInt32 getData() const;

private:
	// Private data members
	XMLInt32 fCharData;

};

class XMLUTIL_EXPORT UnionOp : public Op {
public:
	// -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
	UnionOp(const short type, const int size);
	~UnionOp() { delete fBranches; }

	// -----------------------------------------------------------------------
	// Getter functions
	// -----------------------------------------------------------------------
	int getSize() const;
	const Op* elementAt(int index) const;

	// -----------------------------------------------------------------------
	// Setter functions
	// -----------------------------------------------------------------------
	void addElement(Op* const op);

private:
	// Private Data memebers
	RefVectorOf<Op>* fBranches;
};


class XMLUTIL_EXPORT ChildOp: public Op {
public:
	// -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
	ChildOp(const short type);
	~ChildOp() {}

	// -----------------------------------------------------------------------
	// Getter functions
	// -----------------------------------------------------------------------
	const Op* getChild() const;

	// -----------------------------------------------------------------------
	// Setter functions
	// -----------------------------------------------------------------------
	void setChild(const Op* const child);

private:
	// Private data members
	const Op* fChild;
};

class XMLUTIL_EXPORT ModifierOp: public ChildOp {
public:
	// -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
	ModifierOp(const short type, const XMLInt32 v1, const XMLInt32 v2);
	~ModifierOp() {}

	// -----------------------------------------------------------------------
	// Getter functions
	// -----------------------------------------------------------------------
	XMLInt32 getData() const;
	XMLInt32 getData2() const;

private:
	// Private data members
	XMLInt32 fVal1;
	XMLInt32 fVal2;
};

class XMLUTIL_EXPORT RangeOp: public Op {
public:
	// -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
	RangeOp(const short type, const Token* const token);
	~RangeOp() {}

	// -----------------------------------------------------------------------
	// Getter functions
	// -----------------------------------------------------------------------
	const Token* getToken() const;

private:
	// Private data members
	const Token* fToken;
};

class XMLUTIL_EXPORT StringOp: public Op {
public:
	// -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
	StringOp(const short type, const XMLCh* const literal);
	~StringOp() { delete[] fLiteral;}

	// -----------------------------------------------------------------------
	// Getter functions
	// -----------------------------------------------------------------------
	const XMLCh* getLiteral() const;

private:
	// Private data members
	XMLCh* fLiteral;
};

class XMLUTIL_EXPORT ConditionOp: public Op {
public:
	// -----------------------------------------------------------------------
    //  Public Constructors and Destructor
    // -----------------------------------------------------------------------
	ConditionOp(const short type, const int refNo,
				const Op* const condFlow, const Op* const yesFlow,
				const Op* const noFlow);
	~ConditionOp() {}

	// -----------------------------------------------------------------------
	// Getter functions
	// -----------------------------------------------------------------------
	int			getRefNo() const;
	const Op*	getConditionFlow() const;
	const Op*	getYesFlow() const;
	const Op*	getNoFlow() const;
	
private:
	// Private data members
	int fRefNo;
	const Op* fConditionOp;
	const Op* fYesOp;
	const Op* fNoOp;
};

// ---------------------------------------------------------------------------
//  Op: getter methods
// ---------------------------------------------------------------------------
inline short Op::getOpType() const {

	return fOpType;
}

inline const Op* Op::getNextOp() const {

	return fNextOp;
}

inline int Op::getSize() const {

	//REVISIT do we throw an exception
	return 0;
}

inline XMLInt32	Op::getData() const {

    //REVISIT do we throw an exception
	return 0;
}

inline XMLInt32	Op::getData2() const {

    //REVISIT do we throw an exception
	return 0;
}

inline int	Op::getRefNo() const {

    //REVISIT do we throw an exception
	return 0;
}

inline const Op* Op::elementAt(int index) const {

    //REVISIT do we throw an exception
	return 0;
}

inline const Op* Op::getChild() const {

    //REVISIT do we throw an exception
	return 0;
}

inline const Op* Op::getConditionFlow() const {

    //REVISIT do we throw an exception
	return 0;
}

inline const Op* Op::getYesFlow() const {

    //REVISIT do we throw an exception
	return 0;
}

inline const Op* Op::getNoFlow() const {

    //REVISIT do we throw an exception
	return 0;
}
	
inline const XMLCh* Op::getLiteral() const {

    //REVISIT do we throw an exception
	return 0;
}
	
inline const Token* Op::getToken() const {

    //REVISIT do we throw an exception
	return 0;
}

// ---------------------------------------------------------------------------
//  Op: setter methods
// ---------------------------------------------------------------------------
inline void Op::setOpType(const short type) {

	fOpType = type;
}

inline void Op::setNextOp(const Op* const nextOp) {
	
	fNextOp = nextOp;
}

// ---------------------------------------------------------------------------
//  CharOp: getter/setter methods
// ---------------------------------------------------------------------------
inline XMLInt32 CharOp::getData() const {

	return fCharData;
}

// ---------------------------------------------------------------------------
//  UnionOp: getter/setter methods
// ---------------------------------------------------------------------------
inline int UnionOp::getSize() const {

	return fBranches->size();
}

inline const Op* UnionOp::elementAt(int index) const {

	return fBranches->elementAt(index);
}

inline void UnionOp::addElement(Op* const op) {

	fBranches->addElement(op);
}

// ---------------------------------------------------------------------------
//  ChildOp: getter/setter methods
// ---------------------------------------------------------------------------
inline const Op* ChildOp::getChild() const {

	return fChild;
}

inline void ChildOp::setChild(const Op* const child) {

	fChild = child;
}

// ---------------------------------------------------------------------------
//  ModifierOp: getter methods
// ---------------------------------------------------------------------------
inline XMLInt32 ModifierOp::getData() const {

	return fVal1;
}

inline XMLInt32 ModifierOp::getData2() const {

	return fVal2;
}

// ---------------------------------------------------------------------------
//  RangeOp: getter methods
// ---------------------------------------------------------------------------
inline const Token* RangeOp::getToken() const {

	return fToken;
}

// ---------------------------------------------------------------------------
//  StringOp: getter methods
// ---------------------------------------------------------------------------
inline const XMLCh* StringOp::getLiteral() const {

	return fLiteral;
}

// ---------------------------------------------------------------------------
//  ConditionOp: getter methods
// ---------------------------------------------------------------------------
inline int ConditionOp::getRefNo() const {
	
	return fRefNo;
}

inline const Op* ConditionOp::getConditionFlow() const {

	return fConditionOp;
}

inline const Op* ConditionOp::getYesFlow() const {

	return fYesOp;
}

inline const Op* ConditionOp::getNoFlow() const {

	return fNoOp;
}

#endif

/**
  * End of file Op.hpp
  */
