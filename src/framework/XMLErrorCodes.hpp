// This file is generated, don't edit it!!

#if !defined(ERRHEADER_XMLErrs)
#define ERRHEADER_XMLErrs

#include <framework/XMLErrorReporter.hpp>

class XMLErrs
{
public :
    enum Codes
    {
        NoError                            = 0
      , W_LowBounds                        = 1
      , NotationAlreadyExists              = 2
      , AttListAlreadyExists               = 3
      , ContradictoryEncoding              = 4
      , UndeclaredElemInCM                 = 5
      , UndeclaredElemInAttList            = 6
      , W_HighBounds                       = 7
      , E_LowBounds                        = 8
      , ExpectedCommentOrCDATA             = 9
      , ExpectedAttrName                   = 10
      , ExpectedNotationName               = 11
      , NoRepInMixed                       = 12
      , BadDefAttrDecl                     = 13
      , ExpectedDefAttrDecl                = 14
      , AttListSyntaxError                 = 15
      , ExpectedEqSign                     = 16
      , DupAttrName                        = 17
      , BadIdForXMLLangAttr                = 18
      , ExpectedElementName                = 19
      , MustStartWithXMLDecl               = 20
      , CommentsMustStartWith              = 21
      , InvalidDocumentStructure           = 22
      , ExpectedDeclString                 = 23
      , BadXMLVersion                      = 24
      , UnsupportedXMLVersion              = 25
      , UnterminatedXMLDecl                = 26
      , BadXMLEncoding                     = 27
      , BadStandalone                      = 28
      , UnterminatedComment                = 29
      , PINameExpected                     = 30
      , UnterminatedPI                     = 31
      , InvalidCharacter                   = 32
      , UnexpectedTextBeforeRoot           = 33
      , UnterminatedStartTag               = 34
      , ExpectedAttrValue                  = 35
      , UnterminatedEndTag                 = 36
      , ExpectedAttributeType              = 37
      , ExpectedEndOfTagX                  = 38
      , ExpectedMarkup                     = 39
      , NotValidAfterContent               = 40
      , ExpectedComment                    = 41
      , ExpectedCommentOrPI                = 42
      , ExpectedWhitespace                 = 43
      , NoRootElemInDOCTYPE                = 44
      , ExpectedQuotedString               = 45
      , ExpectedPublicId                   = 46
      , InvalidPublicIdChar                = 47
      , UnterminatedDOCTYPE                = 48
      , InvalidCharacterInIntSubset        = 49
      , ExpectedCDATA                      = 50
      , InvalidInitialNameChar             = 51
      , InvalidNameChar                    = 52
      , UnexpectedWhitespace               = 53
      , InvalidCharacterInAttrValue        = 54
      , ExpectedMarkupDecl                 = 55
      , TextDeclNotLegalHere               = 56
      , ConditionalSectInIntSubset         = 57
      , ExpectedPEName                     = 58
      , UnterminatedEntityDecl             = 59
      , InvalidCharacterRef                = 60
      , UnterminatedCharRef                = 61
      , ExpectedEntityRefName              = 62
      , EntityNotFound                     = 63
      , NoUnparsedEntityRefs               = 64
      , UnterminatedEntityRef              = 65
      , RecursiveEntity                    = 66
      , PartialMarkupInEntity              = 67
      , UnterminatedElementDecl            = 68
      , ExpectedContentSpecExpr            = 69
      , ExpectedAsterisk                   = 70
      , UnterminatedContentModel           = 71
      , ExpectedSystemId                   = 72
      , ExpectedSystemOrPublicId           = 73
      , UnterminatedNotationDecl           = 74
      , ExpectedSeqChoiceLeaf              = 75
      , ExpectedChoiceOrCloseParen         = 76
      , ExpectedSeqOrCloseParen            = 77
      , ExpectedEnumValue                  = 78
      , ExpectedEnumSepOrParen             = 79
      , UnterminatedEntityLiteral          = 80
      , MoreEndThanStartTags               = 81
      , IllegalRefInStandalone             = 82
      , ExpectedOpenParen                  = 83
      , AttrAlreadyUsedInSTag              = 84
      , BracketInAttrValue                 = 85
      , Expected2ndSurrogateChar           = 86
      , ExpectedEndOfConditional           = 87
      , ExpectedIncOrIgn                   = 88
      , ExpectedINCLUDEBracket             = 89
      , ExpectedTextDecl                   = 90
      , ExpectedXMLDecl                    = 91
      , UnexpectedEOE                      = 92
      , PEPropogated                       = 93
      , ExtraCloseSquare                   = 94
      , PERefInMarkupInIntSubset           = 95
      , EntityPropogated                   = 96
      , ExpectedNumericalCharRef           = 97
      , ExpectedOpenSquareBracket          = 98
      , BadSequenceInCharData              = 99
      , IllegalSequenceInComment           = 100
      , UnterminatedCDATASection           = 101
      , ExpectedNDATA                      = 102
      , NDATANotValidForPE                 = 103
      , HexRadixMustBeLowerCase            = 104
      , DeclStringRep                      = 105
      , DeclStringsInWrongOrder            = 106
      , NoExtRefsInAttValue                = 107
      , XMLDeclMustBeLowerCase             = 108
      , ExpectedEntityValue                = 109
      , BadDigitForRadix                   = 110
      , EndedWithTagsOnStack               = 111
      , AmbiguousContentModel              = 112
      , NestedCDATA                        = 113
      , UnknownPrefix                      = 114
      , PartialTagMarkupError              = 115
      , EmptyMainEntity                    = 116
      , CDATAOutsideOfContent              = 117
      , OnlyCharRefsAllowedHere            = 118
      , Unexpected2ndSurrogateChar         = 119
      , NoPIStartsWithXML                  = 120
      , XMLDeclMustBeFirst                 = 121
      , XMLVersionRequired                 = 122
      , StandaloneNotLegal                 = 123
      , TooManyColonsInName                = 124
      , InvalidColonPos                    = 125
      , ColonNotLegalWithNS                = 126
      , SysException                       = 127
      , XMLException                       = 128
      , UnexpectedEOF                      = 129
      , E_HighBounds                       = 130
      , V_LowBounds                        = 131
      , V_HighBounds                       = 132
    };

    static bool isFatal(const XMLErrs::Codes toCheck)
    {
        return ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds));
    }

    static bool isWarning(const XMLErrs::Codes toCheck)
    {
        return ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds));
    }

    static bool isValid(const XMLErrs::Codes toCheck)
    {
        return ((toCheck >= V_LowBounds) && (toCheck <= V_HighBounds));
    }

    static XMLErrorReporter::ErrTypes errorType(const XMLErrs::Codes toCheck)
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

