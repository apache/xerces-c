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
      , AttGrpPropCorrect3                 = 152
      , InvalidTargetNSValue               = 153
      , DisplayErrorMessage                = 154
      , E_HighBounds                       = 155
      , F_LowBounds                        = 156
      , ExpectedCommentOrCDATA             = 157
      , ExpectedAttrName                   = 158
      , ExpectedNotationName               = 159
      , NoRepInMixed                       = 160
      , BadDefAttrDecl                     = 161
      , ExpectedDefAttrDecl                = 162
      , AttListSyntaxError                 = 163
      , ExpectedEqSign                     = 164
      , DupAttrName                        = 165
      , BadIdForXMLLangAttr                = 166
      , ExpectedElementName                = 167
      , MustStartWithXMLDecl               = 168
      , CommentsMustStartWith              = 169
      , InvalidDocumentStructure           = 170
      , ExpectedDeclString                 = 171
      , BadXMLVersion                      = 172
      , UnsupportedXMLVersion              = 173
      , UnterminatedXMLDecl                = 174
      , BadXMLEncoding                     = 175
      , BadStandalone                      = 176
      , UnterminatedComment                = 177
      , PINameExpected                     = 178
      , UnterminatedPI                     = 179
      , InvalidCharacter                   = 180
      , UnexpectedTextBeforeRoot           = 181
      , UnterminatedStartTag               = 182
      , ExpectedAttrValue                  = 183
      , UnterminatedEndTag                 = 184
      , ExpectedAttributeType              = 185
      , ExpectedEndOfTagX                  = 186
      , ExpectedMarkup                     = 187
      , NotValidAfterContent               = 188
      , ExpectedComment                    = 189
      , ExpectedCommentOrPI                = 190
      , ExpectedWhitespace                 = 191
      , NoRootElemInDOCTYPE                = 192
      , ExpectedQuotedString               = 193
      , ExpectedPublicId                   = 194
      , InvalidPublicIdChar                = 195
      , UnterminatedDOCTYPE                = 196
      , InvalidCharacterInIntSubset        = 197
      , ExpectedCDATA                      = 198
      , InvalidInitialNameChar             = 199
      , InvalidNameChar                    = 200
      , UnexpectedWhitespace               = 201
      , InvalidCharacterInAttrValue        = 202
      , ExpectedMarkupDecl                 = 203
      , TextDeclNotLegalHere               = 204
      , ConditionalSectInIntSubset         = 205
      , ExpectedPEName                     = 206
      , UnterminatedEntityDecl             = 207
      , InvalidCharacterRef                = 208
      , UnterminatedCharRef                = 209
      , ExpectedEntityRefName              = 210
      , EntityNotFound                     = 211
      , NoUnparsedEntityRefs               = 212
      , UnterminatedEntityRef              = 213
      , RecursiveEntity                    = 214
      , PartialMarkupInEntity              = 215
      , UnterminatedElementDecl            = 216
      , ExpectedContentSpecExpr            = 217
      , ExpectedAsterisk                   = 218
      , UnterminatedContentModel           = 219
      , ExpectedSystemId                   = 220
      , ExpectedSystemOrPublicId           = 221
      , UnterminatedNotationDecl           = 222
      , ExpectedSeqChoiceLeaf              = 223
      , ExpectedChoiceOrCloseParen         = 224
      , ExpectedSeqOrCloseParen            = 225
      , ExpectedEnumValue                  = 226
      , ExpectedEnumSepOrParen             = 227
      , UnterminatedEntityLiteral          = 228
      , MoreEndThanStartTags               = 229
      , ExpectedOpenParen                  = 230
      , AttrAlreadyUsedInSTag              = 231
      , BracketInAttrValue                 = 232
      , Expected2ndSurrogateChar           = 233
      , ExpectedEndOfConditional           = 234
      , ExpectedIncOrIgn                   = 235
      , ExpectedINCLUDEBracket             = 236
      , ExpectedTextDecl                   = 237
      , ExpectedXMLDecl                    = 238
      , UnexpectedEOE                      = 239
      , PEPropogated                       = 240
      , ExtraCloseSquare                   = 241
      , PERefInMarkupInIntSubset           = 242
      , EntityPropogated                   = 243
      , ExpectedNumericalCharRef           = 244
      , ExpectedOpenSquareBracket          = 245
      , BadSequenceInCharData              = 246
      , IllegalSequenceInComment           = 247
      , UnterminatedCDATASection           = 248
      , ExpectedNDATA                      = 249
      , NDATANotValidForPE                 = 250
      , HexRadixMustBeLowerCase            = 251
      , DeclStringRep                      = 252
      , DeclStringsInWrongOrder            = 253
      , NoExtRefsInAttValue                = 254
      , XMLDeclMustBeLowerCase             = 255
      , ExpectedEntityValue                = 256
      , BadDigitForRadix                   = 257
      , EndedWithTagsOnStack               = 258
      , AmbiguousContentModel              = 259
      , NestedCDATA                        = 260
      , UnknownPrefix                      = 261
      , PartialTagMarkupError              = 262
      , EmptyMainEntity                    = 263
      , CDATAOutsideOfContent              = 264
      , OnlyCharRefsAllowedHere            = 265
      , Unexpected2ndSurrogateChar         = 266
      , NoPIStartsWithXML                  = 267
      , XMLDeclMustBeFirst                 = 268
      , XMLVersionRequired                 = 269
      , StandaloneNotLegal                 = 270
      , EncodingRequired                   = 271
      , TooManyColonsInName                = 272
      , InvalidColonPos                    = 273
      , ColonNotLegalWithNS                = 274
      , SysException                       = 275
      , XMLException                       = 276
      , UnexpectedEOF                      = 277
      , UnexpectedError                    = 278
      , BadSchemaLocation                  = 279
      , NoGrammarResolver                  = 280
      , SchemaScanFatalError               = 281
      , F_HighBounds                       = 282
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

