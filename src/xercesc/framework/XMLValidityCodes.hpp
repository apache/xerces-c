// This file is generated, don't edit it!!

#if !defined(ERRHEADER_XMLValid)
#define ERRHEADER_XMLValid

#include <xercesc/framework/XMLErrorReporter.hpp>

class XMLValid
{
public :
    enum Codes
    {
        NoError                            = 0
      , E_LowBounds                        = 1
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
      , NoValidatorFor                     = 27
      , IncorrectDatatype                  = 28
      , NotADatatype                       = 29
      , TextOnlyContentWithType            = 30
      , FeatureUnsupported                 = 31
      , NestedOnlyInElemOnly               = 32
      , EltRefOnlyInMixedElemOnly          = 33
      , OnlyInEltContent                   = 34
      , OrderIsAll                         = 35
      , DatatypeWithType                   = 36
      , DatatypeQualUnsupported            = 37
      , GroupContentRestricted             = 38
      , UnknownBaseDatatype                = 39
      , OneOfTypeRefArchRef                = 40
      , NoContentForRef                    = 41
      , IncorrectDefaultType               = 42
      , IllegalAttContent                  = 43
      , ValueNotInteger                    = 44
      , DatatypeError                      = 45
      , SchemaError                        = 46
      , TypeAlreadySet                     = 47
      , ProhibitedAttributePresent         = 48
      , IllegalXMLSpace                    = 49
      , NotBoolean                         = 50
      , NotDecimal                         = 51
      , FacetsInconsistent                 = 52
      , IllegalFacetValue                  = 53
      , IllegalDecimalFacet                = 54
      , UnknownFacet                       = 55
      , InvalidEnumValue                   = 56
      , OutOfBounds                        = 57
      , NotAnEnumValue                     = 58
      , NotInteger                         = 59
      , IllegalIntegerFacet                = 60
      , NotReal                            = 61
      , IllegalRealFacet                   = 62
      , ScaleLargerThanPrecision           = 63
      , PrecisionExceeded                  = 64
      , ScaleExceeded                      = 65
      , NotFloat                           = 66
      , SchemaRootError                    = 67
      , WrongTargetNamespace               = 68
      , SimpleTypeHasChild                 = 69
      , NoDatatypeValidatorForSimpleType   = 70
      , GrammarNotFound                    = 71
      , DisplayErrorMessage                = 72
      , NillNotAllowed                     = 73
      , NilAttrNotEmpty                    = 74
      , FixedDifferentFromActual           = 75
      , NoDatatypeValidatorForAttribute    = 76
      , GenericError                       = 77
      , ElementNotQualified                = 78
      , ElementNotUnQualified              = 79
      , IllegalRefInStandalone             = 80
      , NoDefAttForStandalone              = 81
      , NoAttNormForStandalone             = 82
      , NoWSForStandalone                  = 83
      , VC_EntityNotFound                  = 84
      , PartialMarkupInPE                  = 85
      , DatatypeValidationFailure          = 86
      , UniqueParticleAttributionFail      = 87
      , NoAbstractInXsiType                = 88
      , NoDirectUseAbstractElement         = 89
      , NoUseAbstractType                  = 90
      , BadXsiType                         = 91
      , NonDerivedXsiType                  = 92
      , NoSubforBlock                      = 93
      , AttributeNotQualified              = 94
      , AttributeNotUnQualified            = 95
      , IC_FieldMultipleMatch              = 96
      , IC_UnknownField                    = 97
      , IC_AbsentKeyValue                  = 98
      , IC_UniqueNotEnoughValues           = 99
      , IC_KeyNotEnoughValues              = 100
      , IC_KeyRefNotEnoughValues           = 101
      , IC_KeyMatchesNillable              = 102
      , IC_DuplicateUnique                 = 103
      , IC_DuplicateKey                    = 104
      , IC_KeyRefOutOfScope                = 105
      , IC_KeyNotFound                     = 106
      , E_HighBounds                       = 107
      , W_LowBounds                        = 108
      , W_HighBounds                       = 109
      , F_LowBounds                        = 110
      , F_HighBounds                       = 111
    };

    static bool isFatal(const XMLValid::Codes toCheck)
    {
        return ((toCheck >= F_LowBounds) && (toCheck <= F_HighBounds));
    }

    static bool isWarning(const XMLValid::Codes toCheck)
    {
        return ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds));
    }

    static bool isError(const XMLValid::Codes toCheck)
    {
        return ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds));
    }

    static XMLErrorReporter::ErrTypes errorType(const XMLValid::Codes toCheck)
    {
       if ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds))
           return XMLErrorReporter::ErrType_Warning;
       else if ((toCheck >= F_LowBounds) && (toCheck <= F_HighBounds))
            return XMLErrorReporter::ErrType_Fatal;
       else if ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds))
            return XMLErrorReporter::ErrType_Error;
       return XMLErrorReporter::ErrTypes_Unknown;
    }
};
#endif

