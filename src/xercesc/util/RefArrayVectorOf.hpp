#if !defined(REFARRAYVECTOROF_HPP)
#define REFARRAYVECTOROF_HPP

#include <xercesc/util/BaseRefVectorOf.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/** 
 * Class with implementation for vectors of pointers to arrays  - implements from 
 * the Abstract class Vector
 */ 
template <class TElem> class RefArrayVectorOf : public BaseRefVectorOf<TElem> 
{
public :
    // -----------------------------------------------------------------------
    //  Constructor
    // -----------------------------------------------------------------------
    RefArrayVectorOf(const unsigned int maxElems, const bool adoptElems = true);

    // -----------------------------------------------------------------------
    //  Destructor
    // -----------------------------------------------------------------------
    ~RefArrayVectorOf();

};

XERCES_CPP_NAMESPACE_END

#if !defined(XERCES_TMPLSINC)
#include <xercesc/util/RefArrayVectorOf.c>
#endif

#endif
