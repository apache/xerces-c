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

/**
 * $Log$
 * Revision 1.12  2005/01/07 15:12:10  amassari
 * Removed warnings
 *
 * Revision 1.11  2004/11/11 01:31:54  peiyongz
 * Avoid unnecessary expansion -- patch from Christian
 *
 * Revision 1.10  2004/09/08 13:56:23  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.9  2003/12/17 00:18:35  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.8  2003/11/21 15:44:12  amassari
 * insertElementAt was not checking if there was room for the new element (bug#24714)
 *
 * Revision 1.7  2003/05/29 13:26:44  knoaman
 * Fix memory leak when using deprecated dom.
 *
 * Revision 1.6  2003/05/20 21:06:30  knoaman
 * Set values to 0.
 *
 * Revision 1.5  2003/05/16 21:37:00  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.4  2003/05/16 06:01:52  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2003/05/15 19:07:46  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 15:22:05  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:13  peiyongz
 * sane_include
 *
 * Revision 1.5  2002/01/10 17:44:49  knoaman
 * Fix for bug 5786.
 *
 * Revision 1.4  2001/08/09 15:24:37  knoaman
 * add support for <anyAttribute> declaration.
 *
 * Revision 1.3  2000/03/02 19:54:47  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:05  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:31  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:18  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#if defined(XERCES_TMPLSINC)
#include <xercesc/util/ValueVectorOf.hpp>
#endif
#include <string.h>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  ValueVectorOf: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TElem>
ValueVectorOf<TElem>::ValueVectorOf(const unsigned int maxElems,
                                    MemoryManager* const manager,
                                    const bool toCallDestructor) :

    fCallDestructor(toCallDestructor)
    , fCurCount(0)
    , fMaxCount(maxElems)
    , fElemList(0)
    , fMemoryManager(manager)
{
    fElemList = (TElem*) fMemoryManager->allocate
    (
        fMaxCount * sizeof(TElem)
    ); //new TElem[fMaxCount];

    memset(fElemList, 0, fMaxCount * sizeof(TElem));
}

template <class TElem>
ValueVectorOf<TElem>::ValueVectorOf(const ValueVectorOf<TElem>& toCopy) :
    XMemory(toCopy)
    , fCallDestructor(toCopy.fCallDestructor)
    , fCurCount(toCopy.fCurCount)
    , fMaxCount(toCopy.fMaxCount)
    , fElemList(0)
    , fMemoryManager(toCopy.fMemoryManager)
{
    fElemList = (TElem*) fMemoryManager->allocate
    (
        fMaxCount * sizeof(TElem)
    ); //new TElem[fMaxCount];

    memset(fElemList, 0, fMaxCount * sizeof(TElem));
    for (unsigned int index = 0; index < fCurCount; index++)
        fElemList[index] = toCopy.fElemList[index];
}

template <class TElem> ValueVectorOf<TElem>::~ValueVectorOf()
{
    if (fCallDestructor) {
        for (int index= fMaxCount - 1; index >= 0; index--)
            fElemList[index].~TElem();
    }
    fMemoryManager->deallocate(fElemList); //delete [] fElemList;
}



// ---------------------------------------------------------------------------
//  ValueVectorOf: Operators
// ---------------------------------------------------------------------------
template <class TElem> ValueVectorOf<TElem>&
ValueVectorOf<TElem>::operator=(const ValueVectorOf<TElem>& toAssign)
{
    if (this == &toAssign)
        return *this;

    // Reallocate if required
    if (fMaxCount < toAssign.fCurCount)
    {
        fMemoryManager->deallocate(fElemList); //delete [] fElemList;
        fElemList = (TElem*) fMemoryManager->allocate
        (
            toAssign.fMaxCount * sizeof(TElem)
        ); //new TElem[toAssign.fMaxCount];
        fMaxCount = toAssign.fMaxCount;
    }

    fCurCount = toAssign.fCurCount;
    for (unsigned int index = 0; index < fCurCount; index++)
        fElemList[index] = toAssign.fElemList[index];

    return *this;
}


// ---------------------------------------------------------------------------
//  ValueVectorOf: Element management
// ---------------------------------------------------------------------------
template <class TElem> void ValueVectorOf<TElem>::addElement(const TElem& toAdd)
{
    ensureExtraCapacity(1);
    fElemList[fCurCount] = toAdd;
    fCurCount++;
}

template <class TElem> void ValueVectorOf<TElem>::
setElementAt(const TElem& toSet, const unsigned int setAt)
{
    if (setAt >= fCurCount)
        ThrowXMLwithMemMgr(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex, fMemoryManager);
    fElemList[setAt] = toSet;
}

template <class TElem> void ValueVectorOf<TElem>::
insertElementAt(const TElem& toInsert, const unsigned int insertAt)
{
    if (insertAt == fCurCount)
    {
        addElement(toInsert);
        return;
    }

    if (insertAt > fCurCount)
        ThrowXMLwithMemMgr(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex, fMemoryManager);

    // Make room for the newbie
    ensureExtraCapacity(1);
    for (unsigned int index = fCurCount; index > insertAt; index--)
        fElemList[index] = fElemList[index-1];

    // And stick it in and bump the count
    fElemList[insertAt] = toInsert;
    fCurCount++;
}

template <class TElem> void ValueVectorOf<TElem>::
removeElementAt(const unsigned int removeAt)
{
    if (removeAt >= fCurCount)
        ThrowXMLwithMemMgr(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex, fMemoryManager);

    if (removeAt == fCurCount-1)
    {
        fCurCount--;
        return;
    }

    // Copy down every element above remove point
    for (unsigned int index = removeAt; index < fCurCount-1; index++)
        fElemList[index] = fElemList[index+1];

    // And bump down count
    fCurCount--;
}

template <class TElem> void ValueVectorOf<TElem>::removeAllElements()
{
    fCurCount = 0;
}

template <class TElem>
bool ValueVectorOf<TElem>::containsElement(const TElem& toCheck,
                                           const unsigned int startIndex) {

    for (unsigned int i = startIndex; i < fCurCount; i++) {
        if (fElemList[i] == toCheck) {
            return true;
        }
    }

    return false;
}


// ---------------------------------------------------------------------------
//  ValueVectorOf: Getter methods
// ---------------------------------------------------------------------------
template <class TElem> const TElem& ValueVectorOf<TElem>::
elementAt(const unsigned int getAt) const
{
    if (getAt >= fCurCount)
        ThrowXMLwithMemMgr(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex, fMemoryManager);
    return fElemList[getAt];
}

template <class TElem> TElem& ValueVectorOf<TElem>::
elementAt(const unsigned int getAt)
{
    if (getAt >= fCurCount)
        ThrowXMLwithMemMgr(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex, fMemoryManager);
    return fElemList[getAt];
}

template <class TElem> unsigned int ValueVectorOf<TElem>::curCapacity() const
{
    return fMaxCount;
}

template <class TElem> unsigned int ValueVectorOf<TElem>::size() const
{
    return fCurCount;
}

template <class TElem>
MemoryManager* ValueVectorOf<TElem>::getMemoryManager() const
{
    return fMemoryManager;
}

// ---------------------------------------------------------------------------
//  ValueVectorOf: Miscellaneous
// ---------------------------------------------------------------------------
template <class TElem> void ValueVectorOf<TElem>::
ensureExtraCapacity(const unsigned int length)
{
    unsigned int newMax = fCurCount + length;

    if (newMax <= fMaxCount)
        return;

    // Avoid too many reallocations by expanding by a percentage
    unsigned int minNewMax = (unsigned int)((double)fCurCount * 1.25);
    if (newMax < minNewMax)
        newMax = minNewMax;

    TElem* newList = (TElem*) fMemoryManager->allocate
    (
        newMax * sizeof(TElem)
    ); //new TElem[newMax];
    for (unsigned int index = 0; index < fCurCount; index++)
        newList[index] = fElemList[index];

    fMemoryManager->deallocate(fElemList); //delete [] fElemList;
    fElemList = newList;
    fMaxCount = newMax;
}

template <class TElem> const TElem* ValueVectorOf<TElem>::rawData() const
{
    return fElemList;
}



// ---------------------------------------------------------------------------
//  ValueVectorEnumerator: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TElem> ValueVectorEnumerator<TElem>::
ValueVectorEnumerator(       ValueVectorOf<TElem>* const toEnum
                     , const bool                        adopt) :
    fAdopted(adopt)
    , fCurIndex(0)
    , fToEnum(toEnum)
{
}

template <class TElem> ValueVectorEnumerator<TElem>::~ValueVectorEnumerator()
{
    if (fAdopted)
        delete fToEnum;
}


// ---------------------------------------------------------------------------
//  ValueVectorEnumerator: Enum interface
// ---------------------------------------------------------------------------
template <class TElem> bool
ValueVectorEnumerator<TElem>::hasMoreElements() const
{
    if (fCurIndex >= fToEnum->size())
        return false;
    return true;
}

template <class TElem> TElem& ValueVectorEnumerator<TElem>::nextElement()
{
    return fToEnum->elementAt(fCurIndex++);
}

template <class TElem> void ValueVectorEnumerator<TElem>::Reset()
{
    fCurIndex = 0;
}

XERCES_CPP_NAMESPACE_END
