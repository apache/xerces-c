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
      , LocalAttributeWithNameRef          = 50
      , GlobalAttributeWithNameRef         = 51
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
      , InvalidAttValue                    = 82
      , AttributeRefContentError           = 83
      , DuplicateRefAttribute              = 84
      , ForbiddenDerivationByRestriction   = 85
      , ForbiddenDerivationByExtension     = 86
      , BaseNotComplexType                 = 87
      , ImportNoSchemaLocation             = 88
      , ImportNamespaceDifference          = 89
      , ImportRootError                    = 90
      , IncludeNoSchemaLocation            = 91
      , IncludeNamespaceDifference         = 92
      , OnlyAnnotationExpected             = 93
      , InvalidAttributeContent            = 94
      , AttributeRequired                  = 95
      , AttributeDisallowed                = 96
      , InvalidMin2MaxOccurs               = 97
      , AnyAttributeContentError           = 98
      , DisplayErrorMessage                = 99
      , E_HighBounds                       = 100
      , F_LowBounds                        = 101
      , ExpectedCommentOrCDATA             = 102
      , ExpectedAttrName                   = 103
      , ExpectedNotationName               = 104
      , NoRepInMixed                       = 105
      , BadDefAttrDecl                     = 106
      , ExpectedDefAttrDecl                = 107
      , AttListSyntaxError                 = 108
      , ExpectedEqSign                     = 109
      , DupAttrName                        = 110
      , BadIdForXMLLangAttr                = 111
      , ExpectedElementName                = 112
      , MustStartWithXMLDecl               = 113
      , CommentsMustStartWith              = 114
      , InvalidDocumentStructure           = 115
      , ExpectedDeclString                 = 116
      , BadXMLVersion                      = 117
      , UnsupportedXMLVersion              = 118
      , UnterminatedXMLDecl                = 119
      , BadXMLEncoding                     = 120
      , BadStandalone                      = 121
      , UnterminatedComment                = 122
      , PINameExpected                     = 123
      , UnterminatedPI                     = 124
      , InvalidCharacter                   = 125
      , UnexpectedTextBeforeRoot           = 126
      , UnterminatedStartTag               = 127
      , ExpectedAttrValue                  = 128
      , UnterminatedEndTag                 = 129
      , ExpectedAttributeType              = 130
      , ExpectedEndOfTagX                  = 131
      , ExpectedMarkup                     = 132
      , NotValidAfterContent               = 133
      , ExpectedComment                    = 134
      , ExpectedCommentOrPI                = 135
      , ExpectedWhitespace                 = 136
      , NoRootElemInDOCTYPE                = 137
      , ExpectedQuotedString               = 138
      , ExpectedPublicId                   = 139
      , InvalidPublicIdChar                = 140
      , UnterminatedDOCTYPE                = 141
      , InvalidCharacterInIntSubset        = 142
      , ExpectedCDATA                      = 143
      , InvalidInitialNameChar             = 144
      , InvalidNameChar                    = 145
      , UnexpectedWhitespace               = 146
      , InvalidCharacterInAttrValue        = 147
      , ExpectedMarkupDecl                 = 148
      , TextDeclNotLegalHere               = 149
      , ConditionalSectInIntSubset         = 150
      , ExpectedPEName                     = 151
      , UnterminatedEntityDecl             = 152
      , InvalidCharacterRef                = 153
      , UnterminatedCharRef                = 154
      , ExpectedEntityRefName              = 155
      , EntityNotFound                     = 156
      , NoUnparsedEntityRefs               = 157
      , UnterminatedEntityRef              = 158
      , RecursiveEntity                    = 159
      , PartialMarkupInEntity              = 160
      , UnterminatedElementDecl            = 161
      , ExpectedContentSpecExpr            = 162
      , ExpectedAsterisk                   = 163
      , UnterminatedContentModel           = 164
      , ExpectedSystemId                   = 165
      , ExpectedSystemOrPublicId           = 166
      , UnterminatedNotationDecl           = 167
      , ExpectedSeqChoiceLeaf              = 168
      , ExpectedChoiceOrCloseParen         = 169
      , ExpectedSeqOrCloseParen            = 170
      , ExpectedEnumValue                  = 171
      , ExpectedEnumSepOrParen             = 172
      , UnterminatedEntityLiteral          = 173
      , MoreEndThanStartTags               = 174
      , IllegalRefInStandalone             = 175
      , ExpectedOpenParen                  = 176
      , AttrAlreadyUsedInSTag              = 177
      , BracketInAttrValue                 = 178
      , Expected2ndSurrogateChar           = 179
      , ExpectedEndOfConditional           = 180
      , ExpectedIncOrIgn                   = 181
      , ExpectedINCLUDEBracket             = 182
      , ExpectedTextDecl                   = 183
      , ExpectedXMLDecl                    = 184
      , UnexpectedEOE                      = 185
      , PEPropogated                       = 186
      , ExtraCloseSquare                   = 187
      , PERefInMarkupInIntSubset           = 188
      , EntityPropogated                   = 189
      , ExpectedNumericalCharRef           = 190
      , ExpectedOpenSquareBracket          = 191
      , BadSequenceInCharData              = 192
      , IllegalSequenceInComment           = 193
      , UnterminatedCDATASection           = 194
      , ExpectedNDATA                      = 195
      , NDATANotValidForPE                 = 196
      , HexRadixMustBeLowerCase            = 197
      , DeclStringRep                      = 198
      , DeclStringsInWrongOrder            = 199
      , NoExtRefsInAttValue                = 200
      , XMLDeclMustBeLowerCase             = 201
      , ExpectedEntityValue                = 202
      , BadDigitForRadix                   = 203
      , EndedWithTagsOnStack               = 204
      , AmbiguousContentModel              = 205
      , NestedCDATA                        = 206
      , UnknownPrefix                      = 207
      , PartialTagMarkupError              = 208
      , EmptyMainEntity                    = 209
      , CDATAOutsideOfContent              = 210
      , OnlyCharRefsAllowedHere            = 211
      , Unexpected2ndSurrogateChar         = 212
      , NoPIStartsWithXML                  = 213
      , XMLDeclMustBeFirst                 = 214
      , XMLVersionRequired                 = 215
      , StandaloneNotLegal                 = 216
      , TooManyColonsInName                = 217
      , InvalidColonPos                    = 218
      , ColonNotLegalWithNS                = 219
      , SysException                       = 220
      , XMLException                       = 221
      , UnexpectedEOF                      = 222
      , UnexpectedError                    = 223
      , BadSchemaLocation                  = 224
      , NoGrammarResolver                  = 225
      , F_HighBounds                       = 226
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

