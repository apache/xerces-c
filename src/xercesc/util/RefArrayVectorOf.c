
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
template <class TElem> RefArrayVectorOf<TElem>::RefArrayVectorOf(const unsigned int maxElems, const bool adoptElems) : BaseRefVectorOf<TElem>(maxElems, adoptElems)
{
}


template <class TElem> RefArrayVectorOf<TElem>::~RefArrayVectorOf()
{
    if (fAdoptedElems)
    {
      for (unsigned int index = 0; index < fCurCount; index++)
        delete[] fElemList[index];
    }
    delete [] fElemList;
}


XERCES_CPP_NAMESPACE_END
