
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


XERCES_CPP_NAMESPACE_END
