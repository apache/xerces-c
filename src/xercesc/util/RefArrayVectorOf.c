
// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#if defined(XERCES_TMPLSINC)
#include "RefArrayVectorOf.hpp"
#endif

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  RefArrayVectorOf: Constructor and Destructor
// ---------------------------------------------------------------------------
template <class TElem>
RefArrayVectorOf<TElem>::RefArrayVectorOf( const unsigned int maxElems
                                         , const bool adoptElems
                                         , MemoryManager* const manager)
    : BaseRefVectorOf<TElem>(maxElems, adoptElems, manager)
{
}


template <class TElem> RefArrayVectorOf<TElem>::~RefArrayVectorOf()
{
    if (fAdoptedElems)
    {
      for (unsigned int index = 0; index < fCurCount; index++)
            fMemoryManager->deallocate(fElemList[index]);//delete[] fElemList[index];
    }
    fMemoryManager->deallocate(fElemList);//delete [] fElemList;
}

template <class TElem> void
RefArrayVectorOf<TElem>::setElementAt(TElem* const toSet, const unsigned int setAt)
{
    if (setAt >= fCurCount)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex);

    if (fAdoptedElems)
        fMemoryManager->deallocate(fElemList[setAt]);

    fElemList[setAt] = toSet;
}

template <class TElem> void RefArrayVectorOf<TElem>::removeAllElements()
{
    for (unsigned int index = 0; index < fCurCount; index++)
    {
        if (fAdoptedElems)
          fMemoryManager->deallocate(fElemList[index]);

        // Keep unused elements zero for sanity's sake
        fElemList[index] = 0;
    }
    fCurCount = 0;
}

template <class TElem> void RefArrayVectorOf<TElem>::
removeElementAt(const unsigned int removeAt)
{
    if (removeAt >= fCurCount)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex);

    if (fAdoptedElems)
        fMemoryManager->deallocate(fElemList[removeAt]);

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

template <class TElem> void RefArrayVectorOf<TElem>::removeLastElement()
{
    if (!fCurCount)
        return;
    fCurCount--;

    if (fAdoptedElems)
        fMemoryManager->deallocate(fElemList[fCurCount]);
}

template <class TElem> void RefArrayVectorOf<TElem>::cleanup()
{
    if (fAdoptedElems)
    {
        for (unsigned int index = 0; index < fCurCount; index++)
            fMemoryManager->deallocate(fElemList[index]);
    }
    fMemoryManager->deallocate(fElemList);//delete [] fElemList;
}

XERCES_CPP_NAMESPACE_END
