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
      , E_HighBounds                       = 162
      , F_LowBounds                        = 163
      , ExpectedCommentOrCDATA             = 164
      , ExpectedAttrName                   = 165
      , ExpectedNotationName               = 166
      , NoRepInMixed                       = 167
      , BadDefAttrDecl                     = 168
      , ExpectedDefAttrDecl                = 169
      , AttListSyntaxError                 = 170
      , ExpectedEqSign                     = 171
      , DupAttrName                        = 172
      , BadIdForXMLLangAttr                = 173
      , ExpectedElementName                = 174
      , MustStartWithXMLDecl               = 175
      , CommentsMustStartWith              = 176
      , InvalidDocumentStructure           = 177
      , ExpectedDeclString                 = 178
      , BadXMLVersion                      = 179
      , UnsupportedXMLVersion              = 180
      , UnterminatedXMLDecl                = 181
      , BadXMLEncoding                     = 182
      , BadStandalone                      = 183
      , UnterminatedComment                = 184
      , PINameExpected                     = 185
      , UnterminatedPI                     = 186
      , InvalidCharacter                   = 187
      , UnexpectedTextBeforeRoot           = 188
      , UnterminatedStartTag               = 189
      , ExpectedAttrValue                  = 190
      , UnterminatedEndTag                 = 191
      , ExpectedAttributeType              = 192
      , ExpectedEndOfTagX                  = 193
      , ExpectedMarkup                     = 194
      , NotValidAfterContent               = 195
      , ExpectedComment                    = 196
      , ExpectedCommentOrPI                = 197
      , ExpectedWhitespace                 = 198
      , NoRootElemInDOCTYPE                = 199
      , ExpectedQuotedString               = 200
      , ExpectedPublicId                   = 201
      , InvalidPublicIdChar                = 202
      , UnterminatedDOCTYPE                = 203
      , InvalidCharacterInIntSubset        = 204
      , ExpectedCDATA                      = 205
      , InvalidInitialNameChar             = 206
      , InvalidNameChar                    = 207
      , UnexpectedWhitespace               = 208
      , InvalidCharacterInAttrValue        = 209
      , ExpectedMarkupDecl                 = 210
      , TextDeclNotLegalHere               = 211
      , ConditionalSectInIntSubset         = 212
      , ExpectedPEName                     = 213
      , UnterminatedEntityDecl             = 214
      , InvalidCharacterRef                = 215
      , UnterminatedCharRef                = 216
      , ExpectedEntityRefName              = 217
      , EntityNotFound                     = 218
      , NoUnparsedEntityRefs               = 219
      , UnterminatedEntityRef              = 220
      , RecursiveEntity                    = 221
      , PartialMarkupInEntity              = 222
      , UnterminatedElementDecl            = 223
      , ExpectedContentSpecExpr            = 224
      , ExpectedAsterisk                   = 225
      , UnterminatedContentModel           = 226
      , ExpectedSystemId                   = 227
      , ExpectedSystemOrPublicId           = 228
      , UnterminatedNotationDecl           = 229
      , ExpectedSeqChoiceLeaf              = 230
      , ExpectedChoiceOrCloseParen         = 231
      , ExpectedSeqOrCloseParen            = 232
      , ExpectedEnumValue                  = 233
      , ExpectedEnumSepOrParen             = 234
      , UnterminatedEntityLiteral          = 235
      , MoreEndThanStartTags               = 236
      , ExpectedOpenParen                  = 237
      , AttrAlreadyUsedInSTag              = 238
      , BracketInAttrValue                 = 239
      , Expected2ndSurrogateChar           = 240
      , ExpectedEndOfConditional           = 241
      , ExpectedIncOrIgn                   = 242
      , ExpectedINCLUDEBracket             = 243
      , ExpectedTextDecl                   = 244
      , ExpectedXMLDecl                    = 245
      , UnexpectedEOE                      = 246
      , PEPropogated                       = 247
      , ExtraCloseSquare                   = 248
      , PERefInMarkupInIntSubset           = 249
      , EntityPropogated                   = 250
      , ExpectedNumericalCharRef           = 251
      , ExpectedOpenSquareBracket          = 252
      , BadSequenceInCharData              = 253
      , IllegalSequenceInComment           = 254
      , UnterminatedCDATASection           = 255
      , ExpectedNDATA                      = 256
      , NDATANotValidForPE                 = 257
      , HexRadixMustBeLowerCase            = 258
      , DeclStringRep                      = 259
      , DeclStringsInWrongOrder            = 260
      , NoExtRefsInAttValue                = 261
      , XMLDeclMustBeLowerCase             = 262
      , ExpectedEntityValue                = 263
      , BadDigitForRadix                   = 264
      , EndedWithTagsOnStack               = 265
      , AmbiguousContentModel              = 266
      , NestedCDATA                        = 267
      , UnknownPrefix                      = 268
      , PartialTagMarkupError              = 269
      , EmptyMainEntity                    = 270
      , CDATAOutsideOfContent              = 271
      , OnlyCharRefsAllowedHere            = 272
      , Unexpected2ndSurrogateChar         = 273
      , NoPIStartsWithXML                  = 274
      , XMLDeclMustBeFirst                 = 275
      , XMLVersionRequired                 = 276
      , StandaloneNotLegal                 = 277
      , EncodingRequired                   = 278
      , TooManyColonsInName                = 279
      , InvalidColonPos                    = 280
      , ColonNotLegalWithNS                = 281
      , SysException                       = 282
      , XMLException_Fatal                 = 283
      , UnexpectedEOF                      = 284
      , UnexpectedError                    = 285
      , BadSchemaLocation                  = 286
      , NoGrammarResolver                  = 287
      , SchemaScanFatalError               = 288
      , F_HighBounds                       = 289
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

