// This file is generated, don't edit it!!

#if !defined(ERRHEADER_XMLValid)
#define ERRHEADER_XMLValid

#include <framework/XMLErrorReporter.hpp>

class XMLValid
{
public :
    enum Codes
    {
        NoError                            = 0
      , V_LowBounds                        = 1
      , ElementNotDefined                  = 2
      , AttNotDefined                      = 3
      , NotationNotDeclared                = 4
      , RootElemNotLikeDocType             = 5
      , RequiredAttrNotProvided            = 6
      , ElementNotValidForContent          = 7
      , BadIDAttrDefType                   = 8
      , InvalidEmptyAttValue               = 9
      , ElementAlreadyExists               = 10
      , MultipleIdAttrs                    = 11
      , ReusedIDValue                      = 12
      , IDNotDeclared                      = 13
      , UnknownNotRefAttr                  = 14
      , UndeclaredElemInDocType            = 15
      , EmptyNotValidForContent            = 16
      , AttNotDefinedForElement            = 17
      , BadEntityRefAttr                   = 18
      , UnknownEntityRefAttr               = 19
      , NotEnoughElemsForCM                = 20
      , NoCharDataInCM                     = 21
      , DoesNotMatchEnumList               = 22
      , AttrValNotName                     = 23
      , NoMultipleValues                   = 24
      , NotSameAsFixedValue                = 25
      , RepElemInMixed                     = 26
      , NotBoolean                         = 27
      , NotDecimal                         = 28
      , FacetsInconsistent                 = 29
      , IllegalFacetValue                  = 30
      , IllegalDecimalFacet                = 31
      , UnknownFacet                       = 32
      , InvalidEnumValue                   = 33
      , OutOfBounds                        = 34
      , NotAnEnumValue                     = 35
      , NotInteger                         = 36
      , IllegalIntegerFacet                = 37
      , NotReal                            = 38
      , IllegalRealFacet                   = 39
      , ScaleLargerThanPrecision           = 40
      , PrecisionExceeded                  = 41
      , ScaleExceeded                      = 42
      , NotFloat                           = 43
      , V_HighBounds                       = 44
      , W_LowBounds                        = 45
      , W_HighBounds                       = 46
      , E_LowBounds                        = 47
      , E_HighBounds                       = 48
    };

    static bool isFatal(const XMLValid::Codes toCheck)
    {
        return ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds));
    }

    static bool isWarning(const XMLValid::Codes toCheck)
    {
        return ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds));
    }

    static bool isValid(const XMLValid::Codes toCheck)
    {
        return ((toCheck >= V_LowBounds) && (toCheck <= V_HighBounds));
    }

    static XMLErrorReporter::ErrTypes errorType(const XMLValid::Codes toCheck)
    {
       if ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds))
           return XMLErrorReporter::ErrType_Warning;
       else if ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds))
            return XMLErrorReporter::ErrType_Fatal;
       else if ((toCheck >= V_LowBounds) && (toCheck <= V_HighBounds))
            return XMLErrorReporter::ErrType_Invalid;
       return XMLErrorReporter::ErrTypes_Unknown;
    }
};
#endif

