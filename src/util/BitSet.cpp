/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
 * Revision 1.1  1999/11/09 01:04:09  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:45:05  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XML4CDefs.hpp>
#include <util/IllegalArgumentException.hpp>
#include <util/ArrayIndexOutOfBoundsException.hpp>
#include <util/BitSet.hpp>


// ---------------------------------------------------------------------------
//  Local const data
//
//  kBitsPerUnit
//      The number of bits in each of our allocation units, which is a 32
//      bit value in this case.
//
//  kGrowBy
//      The minimum allocation units to grow the buffer by.
// ---------------------------------------------------------------------------
const unsigned int  kBitsPerUnit    = 32;
const unsigned int  kGrowBy         = 1;



// ---------------------------------------------------------------------------
//  BitSet: Constructors and Destructor
// ---------------------------------------------------------------------------
BitSet::BitSet(const unsigned int size) :

    fBits(0)
    , fUnitLen(0)
{
    ensureCapacity(size);
}

BitSet::BitSet(const BitSet& toCopy) :

    fBits(0)
    , fUnitLen(toCopy.fUnitLen)
{
    fBits = new unsigned long[fUnitLen];
    for (unsigned int i = 0; i < fUnitLen; i++)
        fBits[i] = toCopy.fBits[i];
}

BitSet::~BitSet()
{
    delete [] fBits;
}


// ---------------------------------------------------------------------------
//  BitSet: Equality methods
// ---------------------------------------------------------------------------
bool BitSet::equals(const BitSet& other) const
{
    if (this == &other)
        return true;

    if (fUnitLen != other.fUnitLen)
        return false;

    for (unsigned int i = 0; i < fUnitLen; i++)
    {
        if (fBits[i] != other.fBits[i])
            return false;
    }
    return true;
}


// ---------------------------------------------------------------------------
//  BitSet: Getter methods
// ---------------------------------------------------------------------------
bool BitSet::get(const unsigned int index) const
{
    const unsigned int unitOfBit = (index / kBitsPerUnit);
    const unsigned int bitWithinUnit = index % kBitsPerUnit;

    //
    //  If the index is beyond our size, don't actually expand. Just return
    //  false, which is what the state would be if we did expand it.
    //
    bool retVal = false;
    if (unitOfBit <= fUnitLen)
    {
        if (fBits[unitOfBit] & (1 << bitWithinUnit))
            retVal = true;
    }
    return retVal;
}


unsigned int BitSet::size() const
{
    return fUnitLen * kBitsPerUnit;
}



// ---------------------------------------------------------------------------
//  BitSet: Setter methods
// ---------------------------------------------------------------------------
bool BitSet::allAreCleared() const
{
    for (unsigned int index = 0; index < fUnitLen; index++)
    {
        if (fBits[index])
            return false;
    }
    return true;
}

bool BitSet::allAreSet() const
{
    for (unsigned int index = 0; index < fUnitLen; index++)
    {
        if (fBits[index] != 0xFFFFFFFF)
            return false;
    }
    return true;
}

void BitSet::clearAll()
{
    // Just zero out all the units
    for (unsigned int index = 0; index < fUnitLen; index++)
        fBits[index] = 0;
}

void BitSet::clear(const unsigned int index)
{
    ensureCapacity(index+1);

    const int unitOfBit = (index / kBitsPerUnit);
    const int bitWithinUnit = index % kBitsPerUnit;

    fBits[unitOfBit] &= ~(1UL << bitWithinUnit);
}


void BitSet::set(const unsigned int index)
{
    ensureCapacity(index+1);

    const int unitOfBit = (index / kBitsPerUnit);
    const int bitWithinUnit = index % kBitsPerUnit;

    fBits[unitOfBit] |= (1UL << bitWithinUnit);
}



// ---------------------------------------------------------------------------
//  BitSet: Bitwise logical methods
// ---------------------------------------------------------------------------
void BitSet::andWith(const BitSet& other)
{
    if (fUnitLen < other.fUnitLen)
        ensureCapacity(other.fUnitLen * kBitsPerUnit);

    for (unsigned int index = 0; index < other.fUnitLen; index++)
        fBits[index] &= other.fBits[index];
}


void BitSet::orWith(const BitSet& other)
{
    if (fUnitLen < other.fUnitLen)
        ensureCapacity(other.fUnitLen * kBitsPerUnit);

    for (unsigned int index = 0; index < other.fUnitLen; index++)
        fBits[index] |= other.fBits[index];
}


void BitSet::xorWith(const BitSet& other)
{
    if (fUnitLen < other.fUnitLen)
        ensureCapacity(other.fUnitLen * kBitsPerUnit);

    for (unsigned int index = 0; index < other.fUnitLen; index++)
        fBits[index] ^= other.fBits[index];
}



// ---------------------------------------------------------------------------
//  BitSet: Miscellaneous methods
// ---------------------------------------------------------------------------
unsigned int BitSet::hash(const unsigned int hashModulus) const
{
    const unsigned char* pBytes = (const unsigned char*)fBits;
    const int unsigned len = fUnitLen * sizeof(unsigned long);

    unsigned int  hashVal = 0;
    for (unsigned int index = 0; index < len; index++)
    {
        hashVal <<= 1;
        hashVal ^= *pBytes;
    }
    return hashVal % hashModulus;
}



// ---------------------------------------------------------------------------
//  BitSet: Private methods
// ---------------------------------------------------------------------------
void BitSet::ensureCapacity(const unsigned int size)
{
    // Calculate the units required to hold the passed bit count.
    unsigned int unitsNeeded = size / kBitsPerUnit;
    if (size % kBitsPerUnit)
        unitsNeeded++;

    // If its more than we have, then reallocate
    if (unitsNeeded > fUnitLen)
    {
        // Regrow the unit length by at least the expansion unit
        if (unitsNeeded < (fUnitLen + kGrowBy))
            unitsNeeded = fUnitLen + kGrowBy;

        // Allocate the array, copy the old stuff, and zero the new stuff
        unsigned long* newBits = new unsigned long[unitsNeeded];

        unsigned int index;
        for (index = 0; index < fUnitLen; index++)
            newBits[index] = fBits[index];

        for (; index < unitsNeeded; index++)
            newBits[index] = 0;

        delete [] fBits;
        fBits = newBits;
        fUnitLen = unitsNeeded;
    }
}
