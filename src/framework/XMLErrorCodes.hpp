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
      , GenericError                       = 42
      , DatatypeValidatorCreationError     = 43
      , InvalidComplexTypeName             = 44
      , InvalidChildFollowingSimpleContent   = 45
      , InvalidChildFollowingConplexContent   = 46
      , InvalidComplexTypeBlockValue       = 47
      , InvalidComplexTypeFinalValue       = 48
      , AttributeDefaultFixedValue         = 49
      , NotOptionalDefaultAttValue         = 50
      , AttributeWithNameRef               = 51
      , DuplicateAttribute                 = 52
      , AttributeWithTypeAndSimpleType     = 53
      , AttributeSimpleTypeNotFound        = 54
      , ElementWithFixedAndDefault         = 55
      , ElementWithNameRef                 = 56
      , BadAttWithRef                      = 57
      , InvalidElementName                 = 58
      , GlobalElementWithRef               = 59
      , ElementWithTypeAndAnonType         = 60
      , DatatypeValidationFailure          = 61
      , NotSimpleOrMixedElement            = 62
      , DisallowedSimpleTypeExtension      = 63
      , InvalidSimpleContentBase           = 64
      , InvalidComplexTypeBase             = 65
      , InvalidChildInSimpleContent        = 66
      , InvalidChildInComplexContent       = 67
      , AnnotationError                    = 68
      , DisallowedBaseDerivation           = 69
      , SubstitutionRepeated               = 70
      , UnionRepeated                      = 71
      , ExtensionRepeated                  = 72
      , ListRepeated                       = 73
      , RestrictionRepeated                = 74
      , InvalidBlockValue                  = 75
      , InvalidFinalValue                  = 76
      , InvalidSubstitutionGroupElement    = 77
      , SubstitutionGroupTypeMismatch      = 78
      , DuplicateElementDeclaration        = 79
      , InvalidElementBlockValue           = 80
      , InvalidElementFinalValue           = 81
      , AttributeRefContentError           = 82
      , DuplicateRefAttribute              = 83
      , ForbiddenDerivationByRestriction   = 84
      , ForbiddenDerivationByExtension     = 85
      , BaseNotComplexType                 = 86
      , DisplayErrorMessage                = 87
      , E_HighBounds                       = 88
      , F_LowBounds                        = 89
      , ExpectedCommentOrCDATA             = 90
      , ExpectedAttrName                   = 91
      , ExpectedNotationName               = 92
      , NoRepInMixed                       = 93
      , BadDefAttrDecl                     = 94
      , ExpectedDefAttrDecl                = 95
      , AttListSyntaxError                 = 96
      , ExpectedEqSign                     = 97
      , DupAttrName                        = 98
      , BadIdForXMLLangAttr                = 99
      , ExpectedElementName                = 100
      , MustStartWithXMLDecl               = 101
      , CommentsMustStartWith              = 102
      , InvalidDocumentStructure           = 103
      , ExpectedDeclString                 = 104
      , BadXMLVersion                      = 105
      , UnsupportedXMLVersion              = 106
      , UnterminatedXMLDecl                = 107
      , BadXMLEncoding                     = 108
      , BadStandalone                      = 109
      , UnterminatedComment                = 110
      , PINameExpected                     = 111
      , UnterminatedPI                     = 112
      , InvalidCharacter                   = 113
      , UnexpectedTextBeforeRoot           = 114
      , UnterminatedStartTag               = 115
      , ExpectedAttrValue                  = 116
      , UnterminatedEndTag                 = 117
      , ExpectedAttributeType              = 118
      , ExpectedEndOfTagX                  = 119
      , ExpectedMarkup                     = 120
      , NotValidAfterContent               = 121
      , ExpectedComment                    = 122
      , ExpectedCommentOrPI                = 123
      , ExpectedWhitespace                 = 124
      , NoRootElemInDOCTYPE                = 125
      , ExpectedQuotedString               = 126
      , ExpectedPublicId                   = 127
      , InvalidPublicIdChar                = 128
      , UnterminatedDOCTYPE                = 129
      , InvalidCharacterInIntSubset        = 130
      , ExpectedCDATA                      = 131
      , InvalidInitialNameChar             = 132
      , InvalidNameChar                    = 133
      , UnexpectedWhitespace               = 134
      , InvalidCharacterInAttrValue        = 135
      , ExpectedMarkupDecl                 = 136
      , TextDeclNotLegalHere               = 137
      , ConditionalSectInIntSubset         = 138
      , ExpectedPEName                     = 139
      , UnterminatedEntityDecl             = 140
      , InvalidCharacterRef                = 141
      , UnterminatedCharRef                = 142
      , ExpectedEntityRefName              = 143
      , EntityNotFound                     = 144
      , NoUnparsedEntityRefs               = 145
      , UnterminatedEntityRef              = 146
      , RecursiveEntity                    = 147
      , PartialMarkupInEntity              = 148
      , UnterminatedElementDecl            = 149
      , ExpectedContentSpecExpr            = 150
      , ExpectedAsterisk                   = 151
      , UnterminatedContentModel           = 152
      , ExpectedSystemId                   = 153
      , ExpectedSystemOrPublicId           = 154
      , UnterminatedNotationDecl           = 155
      , ExpectedSeqChoiceLeaf              = 156
      , ExpectedChoiceOrCloseParen         = 157
      , ExpectedSeqOrCloseParen            = 158
      , ExpectedEnumValue                  = 159
      , ExpectedEnumSepOrParen             = 160
      , UnterminatedEntityLiteral          = 161
      , MoreEndThanStartTags               = 162
      , IllegalRefInStandalone             = 163
      , ExpectedOpenParen                  = 164
      , AttrAlreadyUsedInSTag              = 165
      , BracketInAttrValue                 = 166
      , Expected2ndSurrogateChar           = 167
      , ExpectedEndOfConditional           = 168
      , ExpectedIncOrIgn                   = 169
      , ExpectedINCLUDEBracket             = 170
      , ExpectedTextDecl                   = 171
      , ExpectedXMLDecl                    = 172
      , UnexpectedEOE                      = 173
      , PEPropogated                       = 174
      , ExtraCloseSquare                   = 175
      , PERefInMarkupInIntSubset           = 176
      , EntityPropogated                   = 177
      , ExpectedNumericalCharRef           = 178
      , ExpectedOpenSquareBracket          = 179
      , BadSequenceInCharData              = 180
      , IllegalSequenceInComment           = 181
      , UnterminatedCDATASection           = 182
      , ExpectedNDATA                      = 183
      , NDATANotValidForPE                 = 184
      , HexRadixMustBeLowerCase            = 185
      , DeclStringRep                      = 186
      , DeclStringsInWrongOrder            = 187
      , NoExtRefsInAttValue                = 188
      , XMLDeclMustBeLowerCase             = 189
      , ExpectedEntityValue                = 190
      , BadDigitForRadix                   = 191
      , EndedWithTagsOnStack               = 192
      , AmbiguousContentModel              = 193
      , NestedCDATA                        = 194
      , UnknownPrefix                      = 195
      , PartialTagMarkupError              = 196
      , EmptyMainEntity                    = 197
      , CDATAOutsideOfContent              = 198
      , OnlyCharRefsAllowedHere            = 199
      , Unexpected2ndSurrogateChar         = 200
      , NoPIStartsWithXML                  = 201
      , XMLDeclMustBeFirst                 = 202
      , XMLVersionRequired                 = 203
      , StandaloneNotLegal                 = 204
      , TooManyColonsInName                = 205
      , InvalidColonPos                    = 206
      , ColonNotLegalWithNS                = 207
      , SysException                       = 208
      , XMLException                       = 209
      , UnexpectedEOF                      = 210
      , UnexpectedError                    = 211
      , BadSchemaLocation                  = 212
      , NoGrammarResolver                  = 213
      , F_HighBounds                       = 214
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

