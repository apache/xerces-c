
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
    if (BaseRefVectorOf<TElem>::fAdoptedElems)
    {
      for (unsigned int index = 0; index < BaseRefVectorOf<TElem>::fCurCount; index++)
            BaseRefVectorOf<TElem>::fMemoryManager->deallocate(BaseRefVectorOf<TElem>::fElemList[index]);//delete[] fElemList[index];
    }
    BaseRefVectorOf<TElem>::fMemoryManager->deallocate(BaseRefVectorOf<TElem>::fElemList);//delete [] fElemList;
}

template <class TElem> void
RefArrayVectorOf<TElem>::setElementAt(TElem* const toSet, const unsigned int setAt)
{
    if (setAt >= BaseRefVectorOf<TElem>::fCurCount)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex);

    if (BaseRefVectorOf<TElem>::fAdoptedElems)
        BaseRefVectorOf<TElem>::fMemoryManager->deallocate(BaseRefVectorOf<TElem>::fElemList[setAt]);

    BaseRefVectorOf<TElem>::fElemList[setAt] = toSet;
}

template <class TElem> void RefArrayVectorOf<TElem>::removeAllElements()
{
    for (unsigned int index = 0; index < BaseRefVectorOf<TElem>::fCurCount; index++)
    {
        if (BaseRefVectorOf<TElem>::fAdoptedElems)
          BaseRefVectorOf<TElem>::fMemoryManager->deallocate(BaseRefVectorOf<TElem>::fElemList[index]);

        // Keep unused elements zero for sanity's sake
        BaseRefVectorOf<TElem>::fElemList[index] = 0;
    }
    BaseRefVectorOf<TElem>::fCurCount = 0;
}

template <class TElem> void RefArrayVectorOf<TElem>::
removeElementAt(const unsigned int removeAt)
{
    if (removeAt >= BaseRefVectorOf<TElem>::fCurCount)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex);

    if (BaseRefVectorOf<TElem>::fAdoptedElems)
        BaseRefVectorOf<TElem>::fMemoryManager->deallocate(BaseRefVectorOf<TElem>::fElemList[removeAt]);

    // Optimize if its the last element
    if (removeAt == BaseRefVectorOf<TElem>::fCurCount-1)
    {
        BaseRefVectorOf<TElem>::fElemList[removeAt] = 0;
        BaseRefVectorOf<TElem>::fCurCount--;
        return;
    }

    // Copy down every element above remove point
    for (unsigned int index = removeAt; index < BaseRefVectorOf<TElem>::fCurCount-1; index++)
        BaseRefVectorOf<TElem>::fElemList[index] = BaseRefVectorOf<TElem>::fElemList[index+1];

    // Keep unused elements zero for sanity's sake
    BaseRefVectorOf<TElem>::fElemList[BaseRefVectorOf<TElem>::fCurCount-1] = 0;

    // And bump down count
    BaseRefVectorOf<TElem>::fCurCount--;
}

template <class TElem> void RefArrayVectorOf<TElem>::removeLastElement()
{
    if (!BaseRefVectorOf<TElem>::fCurCount)
        return;
    BaseRefVectorOf<TElem>::fCurCount--;

    if (BaseRefVectorOf<TElem>::fAdoptedElems)
        BaseRefVectorOf<TElem>::fMemoryManager->deallocate(BaseRefVectorOf<TElem>::fElemList[BaseRefVectorOf<TElem>::fCurCount]);
}

template <class TElem> void RefArrayVectorOf<TElem>::cleanup()
{
    if (BaseRefVectorOf<TElem>::fAdoptedElems)
    {
        for (unsigned int index = 0; index < BaseRefVectorOf<TElem>::fCurCount; index++)
            BaseRefVectorOf<TElem>::fMemoryManager->deallocate(BaseRefVectorOf<TElem>::fElemList[index]);
    }
    BaseRefVectorOf<TElem>::fMemoryManager->deallocate(BaseRefVectorOf<TElem>::fElemList);//delete [] fElemList;
}

XERCES_CPP_NAMESPACE_END
