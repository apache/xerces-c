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
      , ImportNamespaceDifference          = 87
      , ImportRootError                    = 88
      , DeclarationNoSchemaLocation        = 89
      , IncludeNamespaceDifference         = 90
      , OnlyAnnotationExpected             = 91
      , InvalidAttributeContent            = 92
      , AttributeRequired                  = 93
      , AttributeDisallowed                = 94
      , InvalidMin2MaxOccurs               = 95
      , AnyAttributeContentError           = 96
      , NoNameGlobalElement                = 97
      , NoCircularDefinition               = 98
      , DuplicateGlobalType                = 99
      , DuplicateGlobalDeclaration         = 100
      , WS_CollapseExpected                = 101
      , Import_1_1                         = 102
      , Import_1_2                         = 103
      , ElemIDValueConstraint              = 104
      , NoNotationType                     = 105
      , EmptiableMixedContent              = 106
      , EmptyComplexRestrictionDerivation   = 107
      , MixedOrElementOnly                 = 108
      , InvalidContentRestriction          = 109
      , ForbiddenDerivation                = 110
      , AtomicItemType                     = 111
      , MemberTypeNoUnion                  = 112
      , GroupContentError                  = 113
      , AttGroupContentError               = 114
      , MinMaxOnGroupChild                 = 115
      , DeclarationNotFound                = 116
      , AllContentLimited                  = 117
      , BadMinMaxAllCT                     = 118
      , BadMinMaxAllElem                   = 119
      , NoCircularAttGroup                 = 120
      , DuplicateAttInDerivation           = 121
      , NotExpressibleWildCardIntersection   = 122
      , BadAttDerivation_1                 = 123
      , BadAttDerivation_2                 = 124
      , BadAttDerivation_3                 = 125
      , BadAttDerivation_4                 = 126
      , BadAttDerivation_5                 = 127
      , BadAttDerivation_6                 = 128
      , BadAttDerivation_7                 = 129
      , BadAttDerivation_8                 = 130
      , AllContentError                    = 131
      , RedefineNamespaceDifference        = 132
      , Redefine_InvalidSimpleType         = 133
      , Redefine_InvalidSimpleTypeBase     = 134
      , Redefine_InvalidComplexType        = 135
      , Redefine_InvalidComplexTypeBase    = 136
      , Redefine_InvalidGroupMinMax        = 137
      , Redefine_DeclarationNotFound       = 138
      , Redefine_GroupRefCount             = 139
      , Redefine_AttGroupRefCount          = 140
      , Notation_InvalidDecl               = 141
      , Notation_DeclNotFound              = 142
      , IC_DuplicateDecl                   = 143
      , IC_BadContent                      = 144
      , IC_KeyRefReferNotFound             = 145
      , IC_KeyRefCardinality               = 146
      , IC_XPathExprMissing                = 147
      , PD_EmptyBase                       = 148
      , AttUseCorrect                      = 149
      , AttDeclPropCorrect3                = 150
      , AttDeclPropCorrect5                = 151
      , InvalidTargetNSValue               = 152
      , DisplayErrorMessage                = 153
      , E_HighBounds                       = 154
      , F_LowBounds                        = 155
      , ExpectedCommentOrCDATA             = 156
      , ExpectedAttrName                   = 157
      , ExpectedNotationName               = 158
      , NoRepInMixed                       = 159
      , BadDefAttrDecl                     = 160
      , ExpectedDefAttrDecl                = 161
      , AttListSyntaxError                 = 162
      , ExpectedEqSign                     = 163
      , DupAttrName                        = 164
      , BadIdForXMLLangAttr                = 165
      , ExpectedElementName                = 166
      , MustStartWithXMLDecl               = 167
      , CommentsMustStartWith              = 168
      , InvalidDocumentStructure           = 169
      , ExpectedDeclString                 = 170
      , BadXMLVersion                      = 171
      , UnsupportedXMLVersion              = 172
      , UnterminatedXMLDecl                = 173
      , BadXMLEncoding                     = 174
      , BadStandalone                      = 175
      , UnterminatedComment                = 176
      , PINameExpected                     = 177
      , UnterminatedPI                     = 178
      , InvalidCharacter                   = 179
      , UnexpectedTextBeforeRoot           = 180
      , UnterminatedStartTag               = 181
      , ExpectedAttrValue                  = 182
      , UnterminatedEndTag                 = 183
      , ExpectedAttributeType              = 184
      , ExpectedEndOfTagX                  = 185
      , ExpectedMarkup                     = 186
      , NotValidAfterContent               = 187
      , ExpectedComment                    = 188
      , ExpectedCommentOrPI                = 189
      , ExpectedWhitespace                 = 190
      , NoRootElemInDOCTYPE                = 191
      , ExpectedQuotedString               = 192
      , ExpectedPublicId                   = 193
      , InvalidPublicIdChar                = 194
      , UnterminatedDOCTYPE                = 195
      , InvalidCharacterInIntSubset        = 196
      , ExpectedCDATA                      = 197
      , InvalidInitialNameChar             = 198
      , InvalidNameChar                    = 199
      , UnexpectedWhitespace               = 200
      , InvalidCharacterInAttrValue        = 201
      , ExpectedMarkupDecl                 = 202
      , TextDeclNotLegalHere               = 203
      , ConditionalSectInIntSubset         = 204
      , ExpectedPEName                     = 205
      , UnterminatedEntityDecl             = 206
      , InvalidCharacterRef                = 207
      , UnterminatedCharRef                = 208
      , ExpectedEntityRefName              = 209
      , EntityNotFound                     = 210
      , NoUnparsedEntityRefs               = 211
      , UnterminatedEntityRef              = 212
      , RecursiveEntity                    = 213
      , PartialMarkupInEntity              = 214
      , UnterminatedElementDecl            = 215
      , ExpectedContentSpecExpr            = 216
      , ExpectedAsterisk                   = 217
      , UnterminatedContentModel           = 218
      , ExpectedSystemId                   = 219
      , ExpectedSystemOrPublicId           = 220
      , UnterminatedNotationDecl           = 221
      , ExpectedSeqChoiceLeaf              = 222
      , ExpectedChoiceOrCloseParen         = 223
      , ExpectedSeqOrCloseParen            = 224
      , ExpectedEnumValue                  = 225
      , ExpectedEnumSepOrParen             = 226
      , UnterminatedEntityLiteral          = 227
      , MoreEndThanStartTags               = 228
      , ExpectedOpenParen                  = 229
      , AttrAlreadyUsedInSTag              = 230
      , BracketInAttrValue                 = 231
      , Expected2ndSurrogateChar           = 232
      , ExpectedEndOfConditional           = 233
      , ExpectedIncOrIgn                   = 234
      , ExpectedINCLUDEBracket             = 235
      , ExpectedTextDecl                   = 236
      , ExpectedXMLDecl                    = 237
      , UnexpectedEOE                      = 238
      , PEPropogated                       = 239
      , ExtraCloseSquare                   = 240
      , PERefInMarkupInIntSubset           = 241
      , EntityPropogated                   = 242
      , ExpectedNumericalCharRef           = 243
      , ExpectedOpenSquareBracket          = 244
      , BadSequenceInCharData              = 245
      , IllegalSequenceInComment           = 246
      , UnterminatedCDATASection           = 247
      , ExpectedNDATA                      = 248
      , NDATANotValidForPE                 = 249
      , HexRadixMustBeLowerCase            = 250
      , DeclStringRep                      = 251
      , DeclStringsInWrongOrder            = 252
      , NoExtRefsInAttValue                = 253
      , XMLDeclMustBeLowerCase             = 254
      , ExpectedEntityValue                = 255
      , BadDigitForRadix                   = 256
      , EndedWithTagsOnStack               = 257
      , AmbiguousContentModel              = 258
      , NestedCDATA                        = 259
      , UnknownPrefix                      = 260
      , PartialTagMarkupError              = 261
      , EmptyMainEntity                    = 262
      , CDATAOutsideOfContent              = 263
      , OnlyCharRefsAllowedHere            = 264
      , Unexpected2ndSurrogateChar         = 265
      , NoPIStartsWithXML                  = 266
      , XMLDeclMustBeFirst                 = 267
      , XMLVersionRequired                 = 268
      , StandaloneNotLegal                 = 269
      , EncodingRequired                   = 270
      , TooManyColonsInName                = 271
      , InvalidColonPos                    = 272
      , ColonNotLegalWithNS                = 273
      , SysException                       = 274
      , XMLException                       = 275
      , UnexpectedEOF                      = 276
      , UnexpectedError                    = 277
      , BadSchemaLocation                  = 278
      , NoGrammarResolver                  = 279
      , SchemaScanFatalError               = 280
      , F_HighBounds                       = 281
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

