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
 * Revision 1.5  2004/09/08 13:56:47  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/12/17 00:18:37  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
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
 * Revision 1.2  2001/05/11 13:26:41  tng
 * Copyright update.
 *
 * Revision 1.1  2001/03/02 19:22:37  knoaman
 * Schema: Regular expression handling part I
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/ConcatToken.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Token: Constructors and Destructors
// ---------------------------------------------------------------------------
ConcatToken::ConcatToken(Token* const tok1, Token* const tok2, MemoryManager* const manager)
    : Token(Token::T_CONCAT, manager)
    , fChild1(tok1)
    , fChild2(tok2)
{

}


ConcatToken::~ConcatToken() {

}

XERCES_CPP_NAMESPACE_END

/**
  *	End of file ConcatToken.cpp
  */
