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
      , NoNameGlobalElement                = 99
      , NoCircularDefinition               = 100
      , DuplicateGlobalType                = 101
      , WS_CollapseExpected                = 102
      , Import_1_1                         = 103
      , Import_1_2                         = 104
      , ElemIDValueConstraint              = 105
      , DisplayErrorMessage                = 106
      , E_HighBounds                       = 107
      , F_LowBounds                        = 108
      , ExpectedCommentOrCDATA             = 109
      , ExpectedAttrName                   = 110
      , ExpectedNotationName               = 111
      , NoRepInMixed                       = 112
      , BadDefAttrDecl                     = 113
      , ExpectedDefAttrDecl                = 114
      , AttListSyntaxError                 = 115
      , ExpectedEqSign                     = 116
      , DupAttrName                        = 117
      , BadIdForXMLLangAttr                = 118
      , ExpectedElementName                = 119
      , MustStartWithXMLDecl               = 120
      , CommentsMustStartWith              = 121
      , InvalidDocumentStructure           = 122
      , ExpectedDeclString                 = 123
      , BadXMLVersion                      = 124
      , UnsupportedXMLVersion              = 125
      , UnterminatedXMLDecl                = 126
      , BadXMLEncoding                     = 127
      , BadStandalone                      = 128
      , UnterminatedComment                = 129
      , PINameExpected                     = 130
      , UnterminatedPI                     = 131
      , InvalidCharacter                   = 132
      , UnexpectedTextBeforeRoot           = 133
      , UnterminatedStartTag               = 134
      , ExpectedAttrValue                  = 135
      , UnterminatedEndTag                 = 136
      , ExpectedAttributeType              = 137
      , ExpectedEndOfTagX                  = 138
      , ExpectedMarkup                     = 139
      , NotValidAfterContent               = 140
      , ExpectedComment                    = 141
      , ExpectedCommentOrPI                = 142
      , ExpectedWhitespace                 = 143
      , NoRootElemInDOCTYPE                = 144
      , ExpectedQuotedString               = 145
      , ExpectedPublicId                   = 146
      , InvalidPublicIdChar                = 147
      , UnterminatedDOCTYPE                = 148
      , InvalidCharacterInIntSubset        = 149
      , ExpectedCDATA                      = 150
      , InvalidInitialNameChar             = 151
      , InvalidNameChar                    = 152
      , UnexpectedWhitespace               = 153
      , InvalidCharacterInAttrValue        = 154
      , ExpectedMarkupDecl                 = 155
      , TextDeclNotLegalHere               = 156
      , ConditionalSectInIntSubset         = 157
      , ExpectedPEName                     = 158
      , UnterminatedEntityDecl             = 159
      , InvalidCharacterRef                = 160
      , UnterminatedCharRef                = 161
      , ExpectedEntityRefName              = 162
      , EntityNotFound                     = 163
      , NoUnparsedEntityRefs               = 164
      , UnterminatedEntityRef              = 165
      , RecursiveEntity                    = 166
      , PartialMarkupInEntity              = 167
      , UnterminatedElementDecl            = 168
      , ExpectedContentSpecExpr            = 169
      , ExpectedAsterisk                   = 170
      , UnterminatedContentModel           = 171
      , ExpectedSystemId                   = 172
      , ExpectedSystemOrPublicId           = 173
      , UnterminatedNotationDecl           = 174
      , ExpectedSeqChoiceLeaf              = 175
      , ExpectedChoiceOrCloseParen         = 176
      , ExpectedSeqOrCloseParen            = 177
      , ExpectedEnumValue                  = 178
      , ExpectedEnumSepOrParen             = 179
      , UnterminatedEntityLiteral          = 180
      , MoreEndThanStartTags               = 181
      , ExpectedOpenParen                  = 182
      , AttrAlreadyUsedInSTag              = 183
      , BracketInAttrValue                 = 184
      , Expected2ndSurrogateChar           = 185
      , ExpectedEndOfConditional           = 186
      , ExpectedIncOrIgn                   = 187
      , ExpectedINCLUDEBracket             = 188
      , ExpectedTextDecl                   = 189
      , ExpectedXMLDecl                    = 190
      , UnexpectedEOE                      = 191
      , PEPropogated                       = 192
      , ExtraCloseSquare                   = 193
      , PERefInMarkupInIntSubset           = 194
      , EntityPropogated                   = 195
      , ExpectedNumericalCharRef           = 196
      , ExpectedOpenSquareBracket          = 197
      , BadSequenceInCharData              = 198
      , IllegalSequenceInComment           = 199
      , UnterminatedCDATASection           = 200
      , ExpectedNDATA                      = 201
      , NDATANotValidForPE                 = 202
      , HexRadixMustBeLowerCase            = 203
      , DeclStringRep                      = 204
      , DeclStringsInWrongOrder            = 205
      , NoExtRefsInAttValue                = 206
      , XMLDeclMustBeLowerCase             = 207
      , ExpectedEntityValue                = 208
      , BadDigitForRadix                   = 209
      , EndedWithTagsOnStack               = 210
      , AmbiguousContentModel              = 211
      , NestedCDATA                        = 212
      , UnknownPrefix                      = 213
      , PartialTagMarkupError              = 214
      , EmptyMainEntity                    = 215
      , CDATAOutsideOfContent              = 216
      , OnlyCharRefsAllowedHere            = 217
      , Unexpected2ndSurrogateChar         = 218
      , NoPIStartsWithXML                  = 219
      , XMLDeclMustBeFirst                 = 220
      , XMLVersionRequired                 = 221
      , StandaloneNotLegal                 = 222
      , EncodingRequired                   = 223
      , TooManyColonsInName                = 224
      , InvalidColonPos                    = 225
      , ColonNotLegalWithNS                = 226
      , SysException                       = 227
      , XMLException                       = 228
      , UnexpectedEOF                      = 229
      , UnexpectedError                    = 230
      , BadSchemaLocation                  = 231
      , NoGrammarResolver                  = 232
      , F_HighBounds                       = 233
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

