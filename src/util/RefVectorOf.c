/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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

/**
 * $Log$
 * Revision 1.4  2000/07/31 19:18:25  jpolast
 * bug fix in removeAll() to zero out all the pointers.
 *
 * Revision 1.3  2000/03/02 19:54:45  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:03  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:04  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:13  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#if defined(XERCES_TMPLSINC)
#include <util/RefVectorOf.hpp>
#endif


// ---------------------------------------------------------------------------
//  RefVectorOf: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TElem> RefVectorOf<TElem>::
RefVectorOf(const unsigned int maxElems, const bool adoptElems) :

    fAdoptedElems(adoptElems)
    , fCurCount(0)
    , fMaxCount(maxElems)
    , fElemList(0)
{
    // Allocate and initialize the array
    fElemList = new TElem*[maxElems];
    for (unsigned int index = 0; index < maxElems; index++)
        fElemList[index] = 0;
}

template <class TElem> RefVectorOf<TElem>::~RefVectorOf()
{
    if (fAdoptedElems)
    {
        for (unsigned int index = 0; index < fCurCount; index++)
            delete fElemList[index];
    }
    delete [] fElemList;
}


// ---------------------------------------------------------------------------
//  RefVectorOf: Element management
// ---------------------------------------------------------------------------
template <class TElem> void RefVectorOf<TElem>::addElement(TElem* const toAdd)
{
    ensureExtraCapacity(1);
    fElemList[fCurCount] = toAdd;
    fCurCount++;
}


template <class TElem> void
RefVectorOf<TElem>::setElementAt(TElem* const toSet, const unsigned int setAt)
{
    if (setAt >= fCurCount)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex);

    if (fAdoptedElems)
        delete fElemList[setAt];
    fElemList[setAt] = toSet;
}

template <class TElem> void RefVectorOf<TElem>::
insertElementAt(TElem* const toInsert, const unsigned int insertAt)
{
    if (insertAt == fCurCount)
    {
        addElement(toInsert);
        return;
    }

    if (insertAt > fCurCount)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex);

    ensureExtraCapacity(1);

    // Make room for the newbie
    for (unsigned int index = fCurCount; index > insertAt; index--)
        fElemList[index] = fElemList[index-1];

    // And stick it in and bump the count
    fElemList[insertAt] = toInsert;
    fCurCount++;
}

template <class TElem> TElem* RefVectorOf<TElem>::
orphanElementAt(const unsigned int orphanAt)
{
    if (orphanAt >= fCurCount)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex);

    // Get the element we are going to orphan
    TElem* retVal = fElemList[orphanAt];

    // Optimize if its the last element
    if (orphanAt == fCurCount-1)
    {
        fElemList[orphanAt] = 0;
        fCurCount--;
        return retVal;
    }

    // Copy down every element above orphan point
    for (unsigned int index = orphanAt; index < fCurCount-1; index++)
        fElemList[index] = fElemList[index+1];

    // Keep unused elements zero for sanity's sake
    fElemList[fCurCount-1] = 0;

    // And bump down count
    fCurCount--;

    return retVal;
}

template <class TElem> void RefVectorOf<TElem>::removeAllElements()
{
    for (unsigned int index = 0; index < fCurCount; index++)
    {
        if (fAdoptedElems)
          delete fElemList[index];

        // Keep unused elements zero for sanity's sake
        fElemList[index] = 0;
    }
    fCurCount = 0;
}

template <class TElem> void RefVectorOf<TElem>::
removeElementAt(const unsigned int removeAt)
{
    if (removeAt >= fCurCount)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex);

    if (fAdoptedElems)
        delete fElemList[removeAt];

    // Optimize if its the last element
    if (removeAt == fCurCount-1)
    {
        fElemList[removeAt] = 0;
        fCurCount--;
        return;
    }

    // Copy down every element above remove point
    for (unsigned int index = removeAt; index < fCurCount-1; index++)
        fElemList[index] = fElemList[index+1];

    // Keep unused elements zero for sanity's sake
    fElemList[fCurCount-1] = 0;

    // And bump down count
    fCurCount--;
}

template <class TElem> void RefVectorOf<TElem>::removeLastElement()
{
    if (!fCurCount)
        return;
    fCurCount--;

    if (fAdoptedElems)
        delete fElemList[fCurCount];
}


// ---------------------------------------------------------------------------
//  RefVectorOf: Getter methods
// ---------------------------------------------------------------------------
template <class TElem> unsigned int RefVectorOf<TElem>::curCapacity() const
{
    return fMaxCount;
}

template <class TElem> const TElem* RefVectorOf<TElem>::
elementAt(const unsigned int getAt) const
{
    if (getAt >= fCurCount)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex);
    return fElemList[getAt];
}

template <class TElem> TElem*
RefVectorOf<TElem>::elementAt(const unsigned int getAt)
{
    if (getAt >= fCurCount)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex);
    return fElemList[getAt];
}

template <class TElem> unsigned int RefVectorOf<TElem>::size() const
{
    return fCurCount;
}


// ---------------------------------------------------------------------------
//  RefVectorOf: Miscellaneous
// ---------------------------------------------------------------------------
template <class TElem> void RefVectorOf<TElem>::
ensureExtraCapacity(const unsigned int length)
{
    unsigned int newMax = fCurCount + length;

    if (newMax < fMaxCount)
        return;

    // Avoid too many reallocations by providing a little more space
    if (newMax < fMaxCount + 32)
        newMax = fMaxCount + 32;

    // Allocate the new array and copy over the existing stuff
    TElem** newList = new TElem*[newMax];
    unsigned int index = 0;
    for (; index < fCurCount; index++)
        newList[index] = fElemList[index];

    // Zero out the rest of them
    for (; index < newMax; index++)
        newList[index] = 0;

    // Clean up the old array and update our members
    delete [] fElemList;
    fElemList = newList;
    fMaxCount = newMax;
}



// ---------------------------------------------------------------------------
//  RefVectorEnumerator: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TElem> RefVectorEnumerator<TElem>::
RefVectorEnumerator(        RefVectorOf<TElem>* const   toEnum
                    , const bool                        adopt) :
    fAdopted(adopt)
    , fCurIndex(0)
    , fToEnum(toEnum)
{
}

template <class TElem> RefVectorEnumerator<TElem>::~RefVectorEnumerator()
{
    if (fAdopted)
        delete fToEnum;
}


// ---------------------------------------------------------------------------
//  RefVectorEnumerator: Enum interface
// ---------------------------------------------------------------------------
template <class TElem> bool RefVectorEnumerator<TElem>::hasMoreElements() const
{
    if (fCurIndex >= fToEnum->size())
        return false;
    return true;
}

template <class TElem> TElem& RefVectorEnumerator<TElem>::nextElement()
{
    return *(fToEnum->elementAt(fCurIndex++));
}

template <class TElem> void RefVectorEnumerator<TElem>::Reset()
{
    fCurIndex = 0;
}
