// This file is generated, don't edit it!!

#if !defined(ERRHEADER_XMLErrs)
#define ERRHEADER_XMLErrs

#include <xercesc/framework/XMLErrorReporter.hpp>
#include <xercesc/util/XercesDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN

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
      , BadAttDerivation_9                 = 132
      , AllContentError                    = 133
      , RedefineNamespaceDifference        = 134
      , Redefine_InvalidSimpleType         = 135
      , Redefine_InvalidSimpleTypeBase     = 136
      , Redefine_InvalidComplexType        = 137
      , Redefine_InvalidComplexTypeBase    = 138
      , Redefine_InvalidGroupMinMax        = 139
      , Redefine_DeclarationNotFound       = 140
      , Redefine_GroupRefCount             = 141
      , Redefine_AttGroupRefCount          = 142
      , Redefine_InvalidChild              = 143
      , Notation_InvalidDecl               = 144
      , Notation_DeclNotFound              = 145
      , IC_DuplicateDecl                   = 146
      , IC_BadContent                      = 147
      , IC_KeyRefReferNotFound             = 148
      , IC_KeyRefCardinality               = 149
      , IC_XPathExprMissing                = 150
      , AttUseCorrect                      = 151
      , AttDeclPropCorrect3                = 152
      , AttDeclPropCorrect5                = 153
      , AttGrpPropCorrect3                 = 154
      , InvalidTargetNSValue               = 155
      , DisplayErrorMessage                = 156
      , XMLException_Error                 = 157
      , InvalidRedefine                    = 158
      , InvalidNSReference                 = 159
      , NotAllContent                      = 160
      , InvalidAnnotationContent           = 161
      , InvalidFacetName                   = 162
      , InvalidXMLSchemaRoot               = 163
      , CircularSubsGroup                  = 164
      , SubsGroupMemberAbstract            = 165
      , ELTSchemaNS                        = 166
      , InvalidAttTNS                      = 167
      , E_HighBounds                       = 168
      , F_LowBounds                        = 169
      , EntityExpansionLimitExceeded       = 170
      , ExpectedCommentOrCDATA             = 171
      , ExpectedAttrName                   = 172
      , ExpectedNotationName               = 173
      , NoRepInMixed                       = 174
      , BadDefAttrDecl                     = 175
      , ExpectedDefAttrDecl                = 176
      , AttListSyntaxError                 = 177
      , ExpectedEqSign                     = 178
      , DupAttrName                        = 179
      , BadIdForXMLLangAttr                = 180
      , ExpectedElementName                = 181
      , MustStartWithXMLDecl               = 182
      , CommentsMustStartWith              = 183
      , InvalidDocumentStructure           = 184
      , ExpectedDeclString                 = 185
      , BadXMLVersion                      = 186
      , UnsupportedXMLVersion              = 187
      , UnterminatedXMLDecl                = 188
      , BadXMLEncoding                     = 189
      , BadStandalone                      = 190
      , UnterminatedComment                = 191
      , PINameExpected                     = 192
      , UnterminatedPI                     = 193
      , InvalidCharacter                   = 194
      , UnexpectedTextBeforeRoot           = 195
      , UnterminatedStartTag               = 196
      , ExpectedAttrValue                  = 197
      , UnterminatedEndTag                 = 198
      , ExpectedAttributeType              = 199
      , ExpectedEndOfTagX                  = 200
      , ExpectedMarkup                     = 201
      , NotValidAfterContent               = 202
      , ExpectedComment                    = 203
      , ExpectedCommentOrPI                = 204
      , ExpectedWhitespace                 = 205
      , NoRootElemInDOCTYPE                = 206
      , ExpectedQuotedString               = 207
      , ExpectedPublicId                   = 208
      , InvalidPublicIdChar                = 209
      , UnterminatedDOCTYPE                = 210
      , InvalidCharacterInIntSubset        = 211
      , ExpectedCDATA                      = 212
      , InvalidInitialNameChar             = 213
      , InvalidNameChar                    = 214
      , UnexpectedWhitespace               = 215
      , InvalidCharacterInAttrValue        = 216
      , ExpectedMarkupDecl                 = 217
      , TextDeclNotLegalHere               = 218
      , ConditionalSectInIntSubset         = 219
      , ExpectedPEName                     = 220
      , UnterminatedEntityDecl             = 221
      , InvalidCharacterRef                = 222
      , UnterminatedCharRef                = 223
      , ExpectedEntityRefName              = 224
      , EntityNotFound                     = 225
      , NoUnparsedEntityRefs               = 226
      , UnterminatedEntityRef              = 227
      , RecursiveEntity                    = 228
      , PartialMarkupInEntity              = 229
      , UnterminatedElementDecl            = 230
      , ExpectedContentSpecExpr            = 231
      , ExpectedAsterisk                   = 232
      , UnterminatedContentModel           = 233
      , ExpectedSystemId                   = 234
      , ExpectedSystemOrPublicId           = 235
      , UnterminatedNotationDecl           = 236
      , ExpectedSeqChoiceLeaf              = 237
      , ExpectedChoiceOrCloseParen         = 238
      , ExpectedSeqOrCloseParen            = 239
      , ExpectedEnumValue                  = 240
      , ExpectedEnumSepOrParen             = 241
      , UnterminatedEntityLiteral          = 242
      , MoreEndThanStartTags               = 243
      , ExpectedOpenParen                  = 244
      , AttrAlreadyUsedInSTag              = 245
      , BracketInAttrValue                 = 246
      , Expected2ndSurrogateChar           = 247
      , ExpectedEndOfConditional           = 248
      , ExpectedIncOrIgn                   = 249
      , ExpectedINCLUDEBracket             = 250
      , ExpectedTextDecl                   = 251
      , ExpectedXMLDecl                    = 252
      , UnexpectedEOE                      = 253
      , PEPropogated                       = 254
      , ExtraCloseSquare                   = 255
      , PERefInMarkupInIntSubset           = 256
      , EntityPropogated                   = 257
      , ExpectedNumericalCharRef           = 258
      , ExpectedOpenSquareBracket          = 259
      , BadSequenceInCharData              = 260
      , IllegalSequenceInComment           = 261
      , UnterminatedCDATASection           = 262
      , ExpectedNDATA                      = 263
      , NDATANotValidForPE                 = 264
      , HexRadixMustBeLowerCase            = 265
      , DeclStringRep                      = 266
      , DeclStringsInWrongOrder            = 267
      , NoExtRefsInAttValue                = 268
      , XMLDeclMustBeLowerCase             = 269
      , ExpectedEntityValue                = 270
      , BadDigitForRadix                   = 271
      , EndedWithTagsOnStack               = 272
      , AmbiguousContentModel              = 273
      , NestedCDATA                        = 274
      , UnknownPrefix                      = 275
      , PartialTagMarkupError              = 276
      , EmptyMainEntity                    = 277
      , CDATAOutsideOfContent              = 278
      , OnlyCharRefsAllowedHere            = 279
      , Unexpected2ndSurrogateChar         = 280
      , NoPIStartsWithXML                  = 281
      , XMLDeclMustBeFirst                 = 282
      , XMLVersionRequired                 = 283
      , StandaloneNotLegal                 = 284
      , EncodingRequired                   = 285
      , TooManyColonsInName                = 286
      , InvalidColonPos                    = 287
      , ColonNotLegalWithNS                = 288
      , SysException                       = 289
      , XMLException_Fatal                 = 290
      , UnexpectedEOF                      = 291
      , UnexpectedError                    = 292
      , BadSchemaLocation                  = 293
      , NoGrammarResolver                  = 294
      , SchemaScanFatalError               = 295
      , IllegalRefInStandalone             = 296
      , PEBetweenDecl                      = 297
      , NoEmptyStrNamespace                = 298
      , NoUseOfxmlnsAsPrefix               = 299
      , NoUseOfxmlnsURI                    = 300
      , PrefixXMLNotMatchXMLURI            = 301
      , XMLURINotMatchXMLPrefix            = 302
      , NoXMLNSAsElementPrefix             = 303
      , CT_SimpleTypeChildRequired         = 304
      , F_HighBounds                       = 305
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

XERCES_CPP_NAMESPACE_END

#endif

