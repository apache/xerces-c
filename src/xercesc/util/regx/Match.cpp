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
 * Revision 1.6  2005/01/07 15:12:11  amassari
 * Removed warnings
 *
 * Revision 1.5  2004/09/08 13:56:47  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/05/16 00:03:10  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/12/18 13:01:02  gareth
 * New functionality - tokenize and replace. Fixed REVISIT for case insensitive match. Patch by Jennifer Schachter.
 *
 * Revision 1.2  2002/11/04 15:17:00  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:29  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/05/11 13:26:42  tng
 * Copyright update.
 *
 * Revision 1.1  2001/03/02 19:22:40  knoaman
 * Schema: Regular expression handling part I
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/Match.hpp>
#include <xercesc/framework/MemoryManager.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Match: Constructors and Destructors
// ---------------------------------------------------------------------------
Match::Match(MemoryManager* const manager) :
    fNoGroups(0)
    , fPositionsSize(0)
    , fStartPositions(0)
    , fEndPositions(0)
    , fMemoryManager(manager)
{

}

Match::Match(const Match& toCopy) :
    XMemory(toCopy) 
    , fNoGroups(0)
    , fPositionsSize(0)
    , fStartPositions(0)
    , fEndPositions(0)
    , fMemoryManager(0)
{
  initialize(toCopy);
}

Match& Match::operator=(const Match& toAssign) {
  
  initialize(toAssign);
  return *this;
}


Match::~Match() {

	cleanUp();
}

// ---------------------------------------------------------------------------
//  Match: Setter Methods
// ---------------------------------------------------------------------------
void Match::setNoGroups(const int n) {

	if (fNoGroups <= 0 || fPositionsSize < n) {

		cleanUp();
		fPositionsSize = n;
		fStartPositions = (int*) fMemoryManager->allocate(n * sizeof(int));//new int[n];
		fEndPositions = (int*) fMemoryManager->allocate(n * sizeof(int));//new int[n];
	}

	fNoGroups = n;

	for (int i=0; i< fPositionsSize; i++) {

		fStartPositions[i] = -1;
		fEndPositions[i] = -1;
	}
}

// ---------------------------------------------------------------------------
//  Match: private helpers methods
// ---------------------------------------------------------------------------
void Match::initialize(const Match &toCopy){

  //do not copy over value of fPositionSize as it is irrelevant to the 
  //state of the Match

  fMemoryManager = toCopy.fMemoryManager;
  int toCopySize = toCopy.getNoGroups();
  setNoGroups(toCopySize);

  for (int i=0; i<toCopySize; i++){
    setStartPos(i, toCopy.getStartPos(i));
    setEndPos(i, toCopy.getEndPos(i));
  }           

}

void Match::cleanUp() {

	fMemoryManager->deallocate(fStartPositions);//delete [] fStartPositions;
	fMemoryManager->deallocate(fEndPositions);//delete [] fEndPositions;

	fStartPositions = 0;
	fEndPositions = 0;
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file Match.cpp
  */
