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
 * Revision 1.3  2000/03/02 19:54:47  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:04  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:26  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:17  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#if defined(XERCES_TMPLSINC)
#include <util/ValueArrayOf.hpp>
#endif


// ---------------------------------------------------------------------------
//  ValueArrayOf: Contructors and Destructor
// ---------------------------------------------------------------------------
template <class TElem> ValueArrayOf<TElem>::
ValueArrayOf(const unsigned int size) :

    fSize(size)
    , fArray(0)
{
    fArray = new TElem[fSize];
}

template <class TElem> ValueArrayOf<TElem>::
ValueArrayOf(const TElem* values, const unsigned int size) :

    fSize(size)
    , fArray(0)
{
    fArray = new TElem[fSize];
    for (unsigned int index = 0; index < fSize; index++)
        fArray[index] = values[index];
}

template <class TElem> ValueArrayOf<TElem>::
ValueArrayOf(const ValueArrayOf<TElem>& source) :

    fSize(source.fSize)
    , fArray(0)
{
    fArray = new TElem[fSize];
    for (unsigned int index = 0; index < fSize; index++)
        fArray[index] = source.fArray[index];
}

template <class TElem> ValueArrayOf<TElem>::~ValueArrayOf()
{
    delete [] fArray;
}


// ---------------------------------------------------------------------------
//  ValueArrayOf: Public operators
// ---------------------------------------------------------------------------
template <class TElem> TElem& ValueArrayOf<TElem>::
operator[](const unsigned int index)
{
    if (index >= fSize)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Array_BadIndex);
    return fArray[index];
}

template <class TElem> const TElem& ValueArrayOf<TElem>::
operator[](const unsigned int index) const
{
    if (index >= fSize)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Array_BadIndex);
    return fArray[index];
}

template <class TElem> ValueArrayOf<TElem>& ValueArrayOf<TElem>::
operator=(const ValueArrayOf<TElem>& toAssign)
{
    if (this == &toAssign)
        return *this;

    // Reallocate if not the same size
    if (toAssign.fSize != fSize)
    {
        delete [] fArray;
        fSize = toAssign.fSize;
        fArray = new TElem[fSize];
    }

    // Copy over the source elements
    for (unsigned int index = 0; index < fSize; index++)
        fArray[index] = toAssign.fArray[index];

    return *this;
}

template <class TElem> bool ValueArrayOf<TElem>::
operator==(const ValueArrayOf<TElem>& toCompare) const
{
    if (this == &toCompare)
        return true;

    if (fSize != toCompare.fSize)
        return false;

    for (unsigned int index = 0; index < fSize; index++)
    {
        if (fArray[index] != toCompare.fArray[index])
            return false;
    }

    return true;
}

template <class TElem> bool ValueArrayOf<TElem>::
operator!=(const ValueArrayOf<TElem>& toCompare) const
{
    return !operator==(toCompare);
}


// ---------------------------------------------------------------------------
//  ValueArrayOf: Copy operations
// ---------------------------------------------------------------------------
template <class TElem> unsigned int ValueArrayOf<TElem>::
copyFrom(const ValueArrayOf<TElem>& srcArray)
{
    //
    //  Copy over as many of the source elements as will fit into
    //  this array.
    //
    const unsigned int count = fSize < srcArray.fSize ?
                                fSize : srcArray.fSize;

    for (unsigned int index = 0; index < count; index++)
        fArray[index] = srcArray.fArray[index];

    return count;
}


// ---------------------------------------------------------------------------
//  ValueArrayOf: Getter methods
// ---------------------------------------------------------------------------
template <class TElem> unsigned int ValueArrayOf<TElem>::
length() const
{
    return fSize;
}

template <class TElem> TElem* ValueArrayOf<TElem>::
rawData() const
{
    return fArray;
}


// ---------------------------------------------------------------------------
//  ValueArrayOf: Miscellaneous methods
// ---------------------------------------------------------------------------
template <class TElem> void ValueArrayOf<TElem>::
resize(const unsigned int newSize)
{
    if (newSize == fSize)
        return;

    if (newSize < fSize)
        ThrowXML(IllegalArgumentException, XMLExcepts::Array_BadNewSize);

    // Allocate the new array
    TElem* newArray = new TElem[newSize];

    // Copy the existing values
    unsigned int index = 0;
    for (; index < fSize; index++)
        newArray[index] = fArray[index];

    for (; index < newSize; index++)
        newArray[index] = TElem(0);

    // Delete the old array and udpate our members
    delete [] fArray;
    fArray = newArray;
    fSize = newSize;
}



// ---------------------------------------------------------------------------
//  ValueArrayEnumerator: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TElem> ValueArrayEnumerator<TElem>::
ValueArrayEnumerator(ValueArrayOf<TElem>* const toEnum, const bool adopt) :
    fAdopted(adopt)
    , fCurIndex(0)
    , fToEnum(toEnum)
{
}

template <class TElem> ValueArrayEnumerator<TElem>::~ValueArrayEnumerator()
{
    if (fAdopted)
        delete fToEnum;
}


// ---------------------------------------------------------------------------
//  ValueArrayEnumerator: Enum interface
// ---------------------------------------------------------------------------
template <class TElem> bool ValueArrayEnumerator<TElem>::hasMoreElements() const
{
    if (fCurIndex >= fToEnum->length())
        return false;
    return true;
}

template <class TElem> TElem& ValueArrayEnumerator<TElem>::nextElement()
{
    return (*fToEnum)[fCurIndex++];
}

template <class TElem> void ValueArrayEnumerator<TElem>::Reset()
{
    fCurIndex = 0;
}
