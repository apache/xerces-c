// This file is generated, don't edit it!!

#if !defined(ERRHEADER_XMLErrs)
#define ERRHEADER_XMLErrs

#include <xercesc/framework/XMLErrorReporter.hpp>

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
      , XMLException_Warning               = 7
      , W_HighBounds                       = 8
      , E_LowBounds                        = 9
      , FeatureUnsupported                 = 10
      , TopLevelNoNameComplexType          = 11
      , TopLevelNoNameAttribute            = 12
      , NoNameRefAttribute                 = 13
      , GlobalNoNameElement                = 14
      , NoNameRefElement                   = 15
      , NoNameRefGroup                     = 16
      , NoNameRefAttGroup                  = 17
      , AnonComplexTypeWithName            = 18
      , AnonSimpleTypeWithName             = 19
      , InvalidElementContent              = 20
      , UntypedElement                     = 21
      , SimpleTypeContentError             = 22
      , ExpectedSimpleTypeInList           = 23
      , ListUnionRestrictionError          = 24
      , SimpleTypeDerivationByListError    = 25
      , ExpectedSimpleTypeInRestriction    = 26
      , DuplicateFacet                     = 27
      , ExpectedSimpleTypeInUnion          = 28
      , EmptySimpleTypeContent             = 29
      , InvalidSimpleContent               = 30
      , UnspecifiedBase                    = 31
      , InvalidComplexContent              = 32
      , SchemaElementContentError          = 33
      , ContentError                       = 34
      , UnknownSimpleType                  = 35
      , UnknownComplexType                 = 36
      , UnresolvedPrefix                   = 37
      , RefElementNotFound                 = 38
      , TypeNotFound                       = 39
      , TopLevelAttributeNotFound          = 40
      , InvalidChildInComplexType          = 41
      , BaseTypeNotFound                   = 42
      , NoAttributeInSchema                = 43
      , DatatypeValidatorCreationError     = 44
      , InvalidChildFollowingSimpleContent   = 45
      , InvalidChildFollowingConplexContent   = 46
      , InvalidComplexTypeBlockValue       = 47
      , InvalidComplexTypeFinalValue       = 48
      , AttributeDefaultFixedValue         = 49
      , NotOptionalDefaultAttValue         = 50
      , LocalAttributeWithNameRef          = 51
      , GlobalAttributeWithNameRef         = 52
      , DuplicateAttribute                 = 53
      , AttributeWithTypeAndSimpleType     = 54
      , AttributeSimpleTypeNotFound        = 55
      , ElementWithFixedAndDefault         = 56
      , DeclarationWithNameRef             = 57
      , BadAttWithRef                      = 58
      , InvalidDeclarationName             = 59
      , GlobalElementWithRef               = 60
      , ElementWithTypeAndAnonType         = 61
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
      , ImportNamespaceDifference          = 88
      , ImportRootError                    = 89
      , DeclarationNoSchemaLocation        = 90
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
      , BadAttDerivation_8                 = 131
      , AllContentError                    = 132
      , RedefineNamespaceDifference        = 133
      , Redefine_InvalidSimpleType         = 134
      , Redefine_InvalidSimpleTypeBase     = 135
      , Redefine_InvalidComplexType        = 136
      , Redefine_InvalidComplexTypeBase    = 137
      , Redefine_InvalidGroupMinMax        = 138
      , Redefine_DeclarationNotFound       = 139
      , Redefine_GroupRefCount             = 140
      , Redefine_AttGroupRefCount          = 141
      , Notation_InvalidDecl               = 142
      , Notation_DeclNotFound              = 143
      , IC_DuplicateDecl                   = 144
      , IC_BadContent                      = 145
      , IC_KeyRefReferNotFound             = 146
      , IC_KeyRefCardinality               = 147
      , IC_XPathExprMissing                = 148
      , PD_EmptyBase                       = 149
      , AttUseCorrect                      = 150
      , AttDeclPropCorrect3                = 151
      , AttDeclPropCorrect5                = 152
      , AttGrpPropCorrect3                 = 153
      , InvalidTargetNSValue               = 154
      , DisplayErrorMessage                = 155
      , XMLException_Error                 = 156
      , InvalidRedefine                    = 157
      , InvalidNSReference                 = 158
      , NotAllContent                      = 159
      , InvalidAnnotationContent           = 160
      , InvalidFacetName                   = 161
      , InvalidXMLSchemaRoot               = 162
      , E_HighBounds                       = 163
      , F_LowBounds                        = 164
      , ExpectedCommentOrCDATA             = 165
      , ExpectedAttrName                   = 166
      , ExpectedNotationName               = 167
      , NoRepInMixed                       = 168
      , BadDefAttrDecl                     = 169
      , ExpectedDefAttrDecl                = 170
      , AttListSyntaxError                 = 171
      , ExpectedEqSign                     = 172
      , DupAttrName                        = 173
      , BadIdForXMLLangAttr                = 174
      , ExpectedElementName                = 175
      , MustStartWithXMLDecl               = 176
      , CommentsMustStartWith              = 177
      , InvalidDocumentStructure           = 178
      , ExpectedDeclString                 = 179
      , BadXMLVersion                      = 180
      , UnsupportedXMLVersion              = 181
      , UnterminatedXMLDecl                = 182
      , BadXMLEncoding                     = 183
      , BadStandalone                      = 184
      , UnterminatedComment                = 185
      , PINameExpected                     = 186
      , UnterminatedPI                     = 187
      , InvalidCharacter                   = 188
      , UnexpectedTextBeforeRoot           = 189
      , UnterminatedStartTag               = 190
      , ExpectedAttrValue                  = 191
      , UnterminatedEndTag                 = 192
      , ExpectedAttributeType              = 193
      , ExpectedEndOfTagX                  = 194
      , ExpectedMarkup                     = 195
      , NotValidAfterContent               = 196
      , ExpectedComment                    = 197
      , ExpectedCommentOrPI                = 198
      , ExpectedWhitespace                 = 199
      , NoRootElemInDOCTYPE                = 200
      , ExpectedQuotedString               = 201
      , ExpectedPublicId                   = 202
      , InvalidPublicIdChar                = 203
      , UnterminatedDOCTYPE                = 204
      , InvalidCharacterInIntSubset        = 205
      , ExpectedCDATA                      = 206
      , InvalidInitialNameChar             = 207
      , InvalidNameChar                    = 208
      , UnexpectedWhitespace               = 209
      , InvalidCharacterInAttrValue        = 210
      , ExpectedMarkupDecl                 = 211
      , TextDeclNotLegalHere               = 212
      , ConditionalSectInIntSubset         = 213
      , ExpectedPEName                     = 214
      , UnterminatedEntityDecl             = 215
      , InvalidCharacterRef                = 216
      , UnterminatedCharRef                = 217
      , ExpectedEntityRefName              = 218
      , EntityNotFound                     = 219
      , NoUnparsedEntityRefs               = 220
      , UnterminatedEntityRef              = 221
      , RecursiveEntity                    = 222
      , PartialMarkupInEntity              = 223
      , UnterminatedElementDecl            = 224
      , ExpectedContentSpecExpr            = 225
      , ExpectedAsterisk                   = 226
      , UnterminatedContentModel           = 227
      , ExpectedSystemId                   = 228
      , ExpectedSystemOrPublicId           = 229
      , UnterminatedNotationDecl           = 230
      , ExpectedSeqChoiceLeaf              = 231
      , ExpectedChoiceOrCloseParen         = 232
      , ExpectedSeqOrCloseParen            = 233
      , ExpectedEnumValue                  = 234
      , ExpectedEnumSepOrParen             = 235
      , UnterminatedEntityLiteral          = 236
      , MoreEndThanStartTags               = 237
      , ExpectedOpenParen                  = 238
      , AttrAlreadyUsedInSTag              = 239
      , BracketInAttrValue                 = 240
      , Expected2ndSurrogateChar           = 241
      , ExpectedEndOfConditional           = 242
      , ExpectedIncOrIgn                   = 243
      , ExpectedINCLUDEBracket             = 244
      , ExpectedTextDecl                   = 245
      , ExpectedXMLDecl                    = 246
      , UnexpectedEOE                      = 247
      , PEPropogated                       = 248
      , ExtraCloseSquare                   = 249
      , PERefInMarkupInIntSubset           = 250
      , EntityPropogated                   = 251
      , ExpectedNumericalCharRef           = 252
      , ExpectedOpenSquareBracket          = 253
      , BadSequenceInCharData              = 254
      , IllegalSequenceInComment           = 255
      , UnterminatedCDATASection           = 256
      , ExpectedNDATA                      = 257
      , NDATANotValidForPE                 = 258
      , HexRadixMustBeLowerCase            = 259
      , DeclStringRep                      = 260
      , DeclStringsInWrongOrder            = 261
      , NoExtRefsInAttValue                = 262
      , XMLDeclMustBeLowerCase             = 263
      , ExpectedEntityValue                = 264
      , BadDigitForRadix                   = 265
      , EndedWithTagsOnStack               = 266
      , AmbiguousContentModel              = 267
      , NestedCDATA                        = 268
      , UnknownPrefix                      = 269
      , PartialTagMarkupError              = 270
      , EmptyMainEntity                    = 271
      , CDATAOutsideOfContent              = 272
      , OnlyCharRefsAllowedHere            = 273
      , Unexpected2ndSurrogateChar         = 274
      , NoPIStartsWithXML                  = 275
      , XMLDeclMustBeFirst                 = 276
      , XMLVersionRequired                 = 277
      , StandaloneNotLegal                 = 278
      , EncodingRequired                   = 279
      , TooManyColonsInName                = 280
      , InvalidColonPos                    = 281
      , ColonNotLegalWithNS                = 282
      , SysException                       = 283
      , XMLException_Fatal                 = 284
      , UnexpectedEOF                      = 285
      , UnexpectedError                    = 286
      , BadSchemaLocation                  = 287
      , NoGrammarResolver                  = 288
      , SchemaScanFatalError               = 289
      , F_HighBounds                       = 290
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

