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
      , FeatureUnsupported                 = 9
      , TopLevelNoNameComplexType          = 10
      , TopLevelNoNameAttribute            = 11
      , NoNameRefAttribute                 = 12
      , GlobalNoNameElement                = 13
      , NoNameRefElement                   = 14
      , AnonComplexTypeWithName            = 15
      , AnonSimpleTypeWithName             = 16
      , InvalidElementContent              = 17
      , InvalidAttributeName               = 18
      , UntypedElement                     = 19
      , SimpleTypeContentError             = 20
      , ExpectedSimpleTypeInList           = 21
      , ListUnionRestrictionError          = 22
      , SimpleTypeDerivationByListError    = 23
      , ExpectedSimpleTypeInRestriction    = 24
      , DuplicateFacet                     = 25
      , ExpectedSimpleTypeInUnion          = 26
      , EmptySimpleTypeContent             = 27
      , InvalidSimpleContent               = 28
      , UnspecifiedBase                    = 29
      , InvalidComplexContent              = 30
      , SchemaElementContentError          = 31
      , ContentError                       = 32
      , UnknownSimpleType                  = 33
      , UnknownComplexType                 = 34
      , UnresolvedPrefix                   = 35
      , RefElementNotFound                 = 36
      , TypeNotFound                       = 37
      , TopLevelAttributeNotFound          = 38
      , InvalidChildInComplexType          = 39
      , BaseTypeNotFound                   = 40
      , NoAttributeInSchema                = 41
      , DatatypeValidatorCreationError     = 42
      , InvalidComplexTypeName             = 43
      , InvalidChildFollowingSimpleContent   = 44
      , InvalidChildFollowingConplexContent   = 45
      , InvalidComplexTypeBlockValue       = 46
      , InvalidComplexTypeFinalValue       = 47
      , AttributeDefaultFixedValue         = 48
      , NotOptionalDefaultAttValue         = 49
      , AttributeWithNameRef               = 50
      , DuplicateAttribute                 = 51
      , AttributeWithTypeAndSimpleType     = 52
      , AttributeSimpleTypeNotFound        = 53
      , ElementWithFixedAndDefault         = 54
      , ElementWithNameRef                 = 55
      , BadAttWithRef                      = 56
      , InvalidElementName                 = 57
      , GlobalElementWithRef               = 58
      , ElementWithTypeAndAnonType         = 59
      , DatatypeValidationFailure          = 60
      , NotSimpleOrMixedElement            = 61
      , DisallowedSimpleTypeExtension      = 62
      , InvalidSimpleContentBase           = 63
      , InvalidComplexTypeBase             = 64
      , InvalidChildInSimpleContent        = 65
      , InvalidChildInComplexContent       = 66
      , AnnotationError                    = 67
      , DisallowedBaseDerivation           = 68
      , SubstitutionRepeated               = 69
      , UnionRepeated                      = 70
      , ExtensionRepeated                  = 71
      , ListRepeated                       = 72
      , RestrictionRepeated                = 73
      , InvalidBlockValue                  = 74
      , InvalidFinalValue                  = 75
      , InvalidSubstitutionGroupElement    = 76
      , SubstitutionGroupTypeMismatch      = 77
      , DuplicateElementDeclaration        = 78
      , InvalidElementBlockValue           = 79
      , InvalidElementFinalValue           = 80
      , AttributeRefContentError           = 81
      , DuplicateRefAttribute              = 82
      , ForbiddenDerivationByRestriction   = 83
      , ForbiddenDerivationByExtension     = 84
      , BaseNotComplexType                 = 85
      , DisplayErrorMessage                = 86
      , E_HighBounds                       = 87
      , F_LowBounds                        = 88
      , ExpectedCommentOrCDATA             = 89
      , ExpectedAttrName                   = 90
      , ExpectedNotationName               = 91
      , NoRepInMixed                       = 92
      , BadDefAttrDecl                     = 93
      , ExpectedDefAttrDecl                = 94
      , AttListSyntaxError                 = 95
      , ExpectedEqSign                     = 96
      , DupAttrName                        = 97
      , BadIdForXMLLangAttr                = 98
      , ExpectedElementName                = 99
      , MustStartWithXMLDecl               = 100
      , CommentsMustStartWith              = 101
      , InvalidDocumentStructure           = 102
      , ExpectedDeclString                 = 103
      , BadXMLVersion                      = 104
      , UnsupportedXMLVersion              = 105
      , UnterminatedXMLDecl                = 106
      , BadXMLEncoding                     = 107
      , BadStandalone                      = 108
      , UnterminatedComment                = 109
      , PINameExpected                     = 110
      , UnterminatedPI                     = 111
      , InvalidCharacter                   = 112
      , UnexpectedTextBeforeRoot           = 113
      , UnterminatedStartTag               = 114
      , ExpectedAttrValue                  = 115
      , UnterminatedEndTag                 = 116
      , ExpectedAttributeType              = 117
      , ExpectedEndOfTagX                  = 118
      , ExpectedMarkup                     = 119
      , NotValidAfterContent               = 120
      , ExpectedComment                    = 121
      , ExpectedCommentOrPI                = 122
      , ExpectedWhitespace                 = 123
      , NoRootElemInDOCTYPE                = 124
      , ExpectedQuotedString               = 125
      , ExpectedPublicId                   = 126
      , InvalidPublicIdChar                = 127
      , UnterminatedDOCTYPE                = 128
      , InvalidCharacterInIntSubset        = 129
      , ExpectedCDATA                      = 130
      , InvalidInitialNameChar             = 131
      , InvalidNameChar                    = 132
      , UnexpectedWhitespace               = 133
      , InvalidCharacterInAttrValue        = 134
      , ExpectedMarkupDecl                 = 135
      , TextDeclNotLegalHere               = 136
      , ConditionalSectInIntSubset         = 137
      , ExpectedPEName                     = 138
      , UnterminatedEntityDecl             = 139
      , InvalidCharacterRef                = 140
      , UnterminatedCharRef                = 141
      , ExpectedEntityRefName              = 142
      , EntityNotFound                     = 143
      , NoUnparsedEntityRefs               = 144
      , UnterminatedEntityRef              = 145
      , RecursiveEntity                    = 146
      , PartialMarkupInEntity              = 147
      , UnterminatedElementDecl            = 148
      , ExpectedContentSpecExpr            = 149
      , ExpectedAsterisk                   = 150
      , UnterminatedContentModel           = 151
      , ExpectedSystemId                   = 152
      , ExpectedSystemOrPublicId           = 153
      , UnterminatedNotationDecl           = 154
      , ExpectedSeqChoiceLeaf              = 155
      , ExpectedChoiceOrCloseParen         = 156
      , ExpectedSeqOrCloseParen            = 157
      , ExpectedEnumValue                  = 158
      , ExpectedEnumSepOrParen             = 159
      , UnterminatedEntityLiteral          = 160
      , MoreEndThanStartTags               = 161
      , IllegalRefInStandalone             = 162
      , ExpectedOpenParen                  = 163
      , AttrAlreadyUsedInSTag              = 164
      , BracketInAttrValue                 = 165
      , Expected2ndSurrogateChar           = 166
      , ExpectedEndOfConditional           = 167
      , ExpectedIncOrIgn                   = 168
      , ExpectedINCLUDEBracket             = 169
      , ExpectedTextDecl                   = 170
      , ExpectedXMLDecl                    = 171
      , UnexpectedEOE                      = 172
      , PEPropogated                       = 173
      , ExtraCloseSquare                   = 174
      , PERefInMarkupInIntSubset           = 175
      , EntityPropogated                   = 176
      , ExpectedNumericalCharRef           = 177
      , ExpectedOpenSquareBracket          = 178
      , BadSequenceInCharData              = 179
      , IllegalSequenceInComment           = 180
      , UnterminatedCDATASection           = 181
      , ExpectedNDATA                      = 182
      , NDATANotValidForPE                 = 183
      , HexRadixMustBeLowerCase            = 184
      , DeclStringRep                      = 185
      , DeclStringsInWrongOrder            = 186
      , NoExtRefsInAttValue                = 187
      , XMLDeclMustBeLowerCase             = 188
      , ExpectedEntityValue                = 189
      , BadDigitForRadix                   = 190
      , EndedWithTagsOnStack               = 191
      , AmbiguousContentModel              = 192
      , NestedCDATA                        = 193
      , UnknownPrefix                      = 194
      , PartialTagMarkupError              = 195
      , EmptyMainEntity                    = 196
      , CDATAOutsideOfContent              = 197
      , OnlyCharRefsAllowedHere            = 198
      , Unexpected2ndSurrogateChar         = 199
      , NoPIStartsWithXML                  = 200
      , XMLDeclMustBeFirst                 = 201
      , XMLVersionRequired                 = 202
      , StandaloneNotLegal                 = 203
      , TooManyColonsInName                = 204
      , InvalidColonPos                    = 205
      , ColonNotLegalWithNS                = 206
      , SysException                       = 207
      , XMLException                       = 208
      , UnexpectedEOF                      = 209
      , UnexpectedError                    = 210
      , BadSchemaLocation                  = 211
      , NoGrammarResolver                  = 212
      , F_HighBounds                       = 213
    };

    static bool isFatal(const XMLErrs::Codes toCheck)
    {
        return ((toCheck >= F_LowBounds) && (toCheck <= F_HighBounds));
    }

    static bool isWarning(const XMLErrs::Codes toCheck)
    {
        return ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds));
    }

    static bool isError(const XMLErrs::Codes toCheck)
    {
        return ((toCheck >= E_LowBounds) && (toCheck <= E_HighBounds));
    }

    static XMLErrorReporter::ErrTypes errorType(const XMLErrs::Codes toCheck)
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

