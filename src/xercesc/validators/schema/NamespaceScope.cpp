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
 * Revision 1.5  2003/05/18 14:02:07  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.4  2003/05/16 21:43:21  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.3  2003/05/15 18:57:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 14:49:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:45  peiyongz
 * sane_include
 *
 * Revision 1.3  2001/07/31 15:26:54  knoaman
 * Added support for <attributeGroup>.
 *
 * Revision 1.2  2001/05/11 13:27:33  tng
 * Copyright update.
 *
 * Revision 1.1  2001/04/19 17:43:15  knoaman
 * More schema implementation classes.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <string.h>
#include <xercesc/util/EmptyStackException.hpp>
#include <xercesc/validators/schema/NamespaceScope.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  NamespaceScope: Constructors and Destructor
// ---------------------------------------------------------------------------
NamespaceScope::NamespaceScope(MemoryManager* const manager) :

    fEmptyNamespaceId(0)
    , fStackCapacity(8)
    , fStackTop(0)
    , fPrefixPool(109, manager)
    , fStack(0)
    , fMemoryManager(manager)
{
    // Do an initial allocation of the stack and zero it out
    fStack = (StackElem**) fMemoryManager->allocate
    (
        fStackCapacity * sizeof(StackElem*)
    );//new StackElem*[fStackCapacity];
    memset(fStack, 0, fStackCapacity * sizeof(StackElem*));
}

NamespaceScope::~NamespaceScope()
{
    //
    //  Start working from the bottom of the stack and clear it out as we
    //  go up. Once we hit an uninitialized one, we can break out.
    //
    for (unsigned int stackInd = 0; stackInd < fStackCapacity; stackInd++)
    {
        // If this entry has been set, then lets clean it up
        if (!fStack[stackInd])
            break;

        // Delete the row for this entry, then delete the row structure
        fMemoryManager->deallocate(fStack[stackInd]->fMap);//delete [] fStack[stackInd]->fMap;
        delete fStack[stackInd];
    }

    // Delete the stack array itself now
    fMemoryManager->deallocate(fStack);//delete [] fStack;
}


// ---------------------------------------------------------------------------
//  NamespaceScope: Stack access
// ---------------------------------------------------------------------------
unsigned int NamespaceScope::increaseDepth()
{
    // See if we need to expand the stack
    if (fStackTop == fStackCapacity)
        expandStack();

    // If this element has not been initialized yet, then initialize it
    if (!fStack[fStackTop])
    {
        fStack[fStackTop] = new (fMemoryManager) StackElem;
        fStack[fStackTop]->fMapCapacity = 0;
        fStack[fStackTop]->fMap = 0;
    }

    // Set up the new top row
    fStack[fStackTop]->fMapCount = 0;

    // Bump the top of stack
    fStackTop++;

    return fStackTop-1;
}

unsigned int NamespaceScope::decreaseDepth()
{
    if (!fStackTop)
        ThrowXML(EmptyStackException, XMLExcepts::ElemStack_StackUnderflow);

    fStackTop--;

    return fStackTop;
}


// ---------------------------------------------------------------------------
//  NamespaceScope: Prefix map methods
// ---------------------------------------------------------------------------
void NamespaceScope::addPrefix(const XMLCh* const prefixToAdd,
                               const unsigned int uriId) {

    if (!fStackTop)
        ThrowXML(EmptyStackException, XMLExcepts::ElemStack_EmptyStack);

    // Get a convenience pointer to the stack top row
    StackElem* curRow = fStack[fStackTop - 1];

    // Map the prefix to its unique id
    const unsigned int prefId = fPrefixPool.addOrFind(prefixToAdd);

    //
    //  Add a new element to the prefix map for this element. If its full,
    //  then expand it out.
    //
    if (curRow->fMapCount == curRow->fMapCapacity)
        expandMap(curRow);

    //
    //  And now add a new element for this prefix.
    //
    curRow->fMap[curRow->fMapCount].fPrefId = prefId;
    curRow->fMap[curRow->fMapCount].fURIId = uriId;

    // Bump the map count now
    curRow->fMapCount++;
}

unsigned int
NamespaceScope::getNamespaceForPrefix(const XMLCh* const prefixToMap,
                                      const int depthLevel) const {

    //
    //  Map the prefix to its unique id, from the prefix string pool. If its
    //  not a valid prefix, then its a failure.
    //
    unsigned int prefixId = fPrefixPool.getId(prefixToMap);

    if (!prefixId){
        return fEmptyNamespaceId;
    }

    //
    //  Start at the stack top and work backwards until we come to some
    //  element that mapped this prefix.
    //
    int startAt = depthLevel;
    for (int index = startAt; index >= 0; index--)
    {
        // Get a convenience pointer to the current element
        StackElem* curRow = fStack[index];

        // If no prefixes mapped at this level, then go the next one
        if (!curRow->fMapCount)
            continue;

        // Search the map at this level for the passed prefix
        for (unsigned int mapIndex = 0; mapIndex < curRow->fMapCount; mapIndex++)
        {
            if (curRow->fMap[mapIndex].fPrefId == prefixId)
                return curRow->fMap[mapIndex].fURIId;
        }
    }

    return fEmptyNamespaceId;
}


// ---------------------------------------------------------------------------
//  NamespaceScope: Miscellaneous methods
// ---------------------------------------------------------------------------
void NamespaceScope::reset(const unsigned int emptyId)
{
    // Flush the prefix pool and put back in the standard prefixes
    fPrefixPool.flushAll();

    // Reset the stack top to clear the stack
    fStackTop = 0;

    // And store the new special URI ids
    fEmptyNamespaceId = emptyId;
}


// ---------------------------------------------------------------------------
//  Namespace: Private helpers
// ---------------------------------------------------------------------------
void NamespaceScope::expandMap(StackElem* const toExpand)
{
    // For convenience get the old map size
    const unsigned int oldCap = toExpand->fMapCapacity;

    //
    //  Expand the capacity by 25%, or initialize it to 16 if its currently
    //  empty. Then allocate a new temp buffer.
    //
    const unsigned int newCapacity = oldCap ?
                                     (unsigned int)(oldCap * 1.25) : 16;
    PrefMapElem* newMap = (PrefMapElem*) fMemoryManager->allocate
    (
        newCapacity * sizeof(PrefMapElem)
    );//new PrefMapElem[newCapacity];

    //
    //  Copy over the old stuff. We DON'T have to zero out the new stuff
    //  since this is a by value map and the current map index controls what
    //  is relevant.
    //
    memcpy(newMap, toExpand->fMap, oldCap * sizeof(PrefMapElem));

    // Delete the old map and store the new stuff
    fMemoryManager->deallocate(toExpand->fMap);//delete [] toExpand->fMap;
    toExpand->fMap = newMap;
    toExpand->fMapCapacity = newCapacity;
}

void NamespaceScope::expandStack()
{
    // Expand the capacity by 25% and allocate a new buffer
    const unsigned int newCapacity = (unsigned int)(fStackCapacity * 1.25);
    StackElem** newStack = (StackElem**) fMemoryManager->allocate
    (
        newCapacity * sizeof(StackElem*)
    );//new StackElem*[newCapacity];

    // Copy over the old stuff
    memcpy(newStack, fStack, fStackCapacity * sizeof(StackElem*));

    //
    //  And zero out the new stuff. Though we use a stack top, we reuse old
    //  stack contents so we need to know if elements have been initially
    //  allocated or not as we push new stuff onto the stack.
    //
    memset
    (
        &newStack[fStackCapacity]
        , 0
        , (newCapacity - fStackCapacity) * sizeof(StackElem*)
    );

    // Delete the old array and update our members
    fMemoryManager->deallocate(fStack);//delete [] fStack;
    fStack = newStack;
    fStackCapacity = newCapacity;
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file NamespaceScope.cpp
  */

