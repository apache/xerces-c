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
      , NoNameRefGroup                     = 15
      , NoNameRefAttGroup                  = 16
      , AnonComplexTypeWithName            = 17
      , AnonSimpleTypeWithName             = 18
      , InvalidElementContent              = 19
      , UntypedElement                     = 20
      , SimpleTypeContentError             = 21
      , ExpectedSimpleTypeInList           = 22
      , ListUnionRestrictionError          = 23
      , SimpleTypeDerivationByListError    = 24
      , ExpectedSimpleTypeInRestriction    = 25
      , DuplicateFacet                     = 26
      , ExpectedSimpleTypeInUnion          = 27
      , EmptySimpleTypeContent             = 28
      , InvalidSimpleContent               = 29
      , UnspecifiedBase                    = 30
      , InvalidComplexContent              = 31
      , SchemaElementContentError          = 32
      , ContentError                       = 33
      , UnknownSimpleType                  = 34
      , UnknownComplexType                 = 35
      , UnresolvedPrefix                   = 36
      , RefElementNotFound                 = 37
      , TypeNotFound                       = 38
      , TopLevelAttributeNotFound          = 39
      , InvalidChildInComplexType          = 40
      , BaseTypeNotFound                   = 41
      , NoAttributeInSchema                = 42
      , DatatypeValidatorCreationError     = 43
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
      , DeclarationWithNameRef             = 56
      , BadAttWithRef                      = 57
      , InvalidDeclarationName             = 58
      , GlobalElementWithRef               = 59
      , ElementWithTypeAndAnonType         = 60
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
      , InvalidAttValue                    = 81
      , AttributeRefContentError           = 82
      , DuplicateRefAttribute              = 83
      , ForbiddenDerivationByRestriction   = 84
      , ForbiddenDerivationByExtension     = 85
      , BaseNotComplexType                 = 86
      , ImportNoSchemaLocation             = 87
      , ImportNamespaceDifference          = 88
      , ImportRootError                    = 89
      , IncludeNoSchemaLocation            = 90
      , IncludeNamespaceDifference         = 91
      , OnlyAnnotationExpected             = 92
      , InvalidAttributeContent            = 93
      , AttributeRequired                  = 94
      , AttributeDisallowed                = 95
      , InvalidMin2MaxOccurs               = 96
      , AnyAttributeContentError           = 97
      , NoNameGlobalElement                = 98
      , NoCircularDefinition               = 99
      , DuplicateGlobalType                = 100
      , DuplicateGlobalDeclaration         = 101
      , WS_CollapseExpected                = 102
      , Import_1_1                         = 103
      , Import_1_2                         = 104
      , ElemIDValueConstraint              = 105
      , NoNotationType                     = 106
      , EmptiableMixedContent              = 107
      , EmptyComplexRestrictionDerivation   = 108
      , MixedOrElementOnly                 = 109
      , InvalidContentRestriction          = 110
      , ForbiddenDerivation                = 111
      , AtomicItemType                     = 112
      , MemberTypeNoUnion                  = 113
      , GroupContentError                  = 114
      , AttGroupContentError               = 115
      , MinMaxOnGroupChild                 = 116
      , DeclarationNotFound                = 117
      , AllContentLimited                  = 118
      , BadMinMaxAllCT                     = 119
      , BadMinMaxAllElem                   = 120
      , NoCircularAttGroup                 = 121
      , DuplicateAttInDerivation           = 122
      , NotExpressibleWildCardIntersection   = 123
      , BadAttDerivation_1                 = 124
      , BadAttDerivation_2                 = 125
      , BadAttDerivation_3                 = 126
      , BadAttDerivation_4                 = 127
      , BadAttDerivation_5                 = 128
      , BadAttDerivation_6                 = 129
      , BadAttDerivation_7                 = 130
      , AllContentError                    = 131
      , DisplayErrorMessage                = 132
      , E_HighBounds                       = 133
      , F_LowBounds                        = 134
      , ExpectedCommentOrCDATA             = 135
      , ExpectedAttrName                   = 136
      , ExpectedNotationName               = 137
      , NoRepInMixed                       = 138
      , BadDefAttrDecl                     = 139
      , ExpectedDefAttrDecl                = 140
      , AttListSyntaxError                 = 141
      , ExpectedEqSign                     = 142
      , DupAttrName                        = 143
      , BadIdForXMLLangAttr                = 144
      , ExpectedElementName                = 145
      , MustStartWithXMLDecl               = 146
      , CommentsMustStartWith              = 147
      , InvalidDocumentStructure           = 148
      , ExpectedDeclString                 = 149
      , BadXMLVersion                      = 150
      , UnsupportedXMLVersion              = 151
      , UnterminatedXMLDecl                = 152
      , BadXMLEncoding                     = 153
      , BadStandalone                      = 154
      , UnterminatedComment                = 155
      , PINameExpected                     = 156
      , UnterminatedPI                     = 157
      , InvalidCharacter                   = 158
      , UnexpectedTextBeforeRoot           = 159
      , UnterminatedStartTag               = 160
      , ExpectedAttrValue                  = 161
      , UnterminatedEndTag                 = 162
      , ExpectedAttributeType              = 163
      , ExpectedEndOfTagX                  = 164
      , ExpectedMarkup                     = 165
      , NotValidAfterContent               = 166
      , ExpectedComment                    = 167
      , ExpectedCommentOrPI                = 168
      , ExpectedWhitespace                 = 169
      , NoRootElemInDOCTYPE                = 170
      , ExpectedQuotedString               = 171
      , ExpectedPublicId                   = 172
      , InvalidPublicIdChar                = 173
      , UnterminatedDOCTYPE                = 174
      , InvalidCharacterInIntSubset        = 175
      , ExpectedCDATA                      = 176
      , InvalidInitialNameChar             = 177
      , InvalidNameChar                    = 178
      , UnexpectedWhitespace               = 179
      , InvalidCharacterInAttrValue        = 180
      , ExpectedMarkupDecl                 = 181
      , TextDeclNotLegalHere               = 182
      , ConditionalSectInIntSubset         = 183
      , ExpectedPEName                     = 184
      , UnterminatedEntityDecl             = 185
      , InvalidCharacterRef                = 186
      , UnterminatedCharRef                = 187
      , ExpectedEntityRefName              = 188
      , EntityNotFound                     = 189
      , NoUnparsedEntityRefs               = 190
      , UnterminatedEntityRef              = 191
      , RecursiveEntity                    = 192
      , PartialMarkupInEntity              = 193
      , UnterminatedElementDecl            = 194
      , ExpectedContentSpecExpr            = 195
      , ExpectedAsterisk                   = 196
      , UnterminatedContentModel           = 197
      , ExpectedSystemId                   = 198
      , ExpectedSystemOrPublicId           = 199
      , UnterminatedNotationDecl           = 200
      , ExpectedSeqChoiceLeaf              = 201
      , ExpectedChoiceOrCloseParen         = 202
      , ExpectedSeqOrCloseParen            = 203
      , ExpectedEnumValue                  = 204
      , ExpectedEnumSepOrParen             = 205
      , UnterminatedEntityLiteral          = 206
      , MoreEndThanStartTags               = 207
      , ExpectedOpenParen                  = 208
      , AttrAlreadyUsedInSTag              = 209
      , BracketInAttrValue                 = 210
      , Expected2ndSurrogateChar           = 211
      , ExpectedEndOfConditional           = 212
      , ExpectedIncOrIgn                   = 213
      , ExpectedINCLUDEBracket             = 214
      , ExpectedTextDecl                   = 215
      , ExpectedXMLDecl                    = 216
      , UnexpectedEOE                      = 217
      , PEPropogated                       = 218
      , ExtraCloseSquare                   = 219
      , PERefInMarkupInIntSubset           = 220
      , EntityPropogated                   = 221
      , ExpectedNumericalCharRef           = 222
      , ExpectedOpenSquareBracket          = 223
      , BadSequenceInCharData              = 224
      , IllegalSequenceInComment           = 225
      , UnterminatedCDATASection           = 226
      , ExpectedNDATA                      = 227
      , NDATANotValidForPE                 = 228
      , HexRadixMustBeLowerCase            = 229
      , DeclStringRep                      = 230
      , DeclStringsInWrongOrder            = 231
      , NoExtRefsInAttValue                = 232
      , XMLDeclMustBeLowerCase             = 233
      , ExpectedEntityValue                = 234
      , BadDigitForRadix                   = 235
      , EndedWithTagsOnStack               = 236
      , AmbiguousContentModel              = 237
      , NestedCDATA                        = 238
      , UnknownPrefix                      = 239
      , PartialTagMarkupError              = 240
      , EmptyMainEntity                    = 241
      , CDATAOutsideOfContent              = 242
      , OnlyCharRefsAllowedHere            = 243
      , Unexpected2ndSurrogateChar         = 244
      , NoPIStartsWithXML                  = 245
      , XMLDeclMustBeFirst                 = 246
      , XMLVersionRequired                 = 247
      , StandaloneNotLegal                 = 248
      , EncodingRequired                   = 249
      , TooManyColonsInName                = 250
      , InvalidColonPos                    = 251
      , ColonNotLegalWithNS                = 252
      , SysException                       = 253
      , XMLException                       = 254
      , UnexpectedEOF                      = 255
      , UnexpectedError                    = 256
      , BadSchemaLocation                  = 257
      , NoGrammarResolver                  = 258
      , SchemaScanFatalError               = 259
      , F_HighBounds                       = 260
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

