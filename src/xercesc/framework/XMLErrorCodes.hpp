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
      , ExpectedCommentOrCDATA             = 170
      , ExpectedAttrName                   = 171
      , ExpectedNotationName               = 172
      , NoRepInMixed                       = 173
      , BadDefAttrDecl                     = 174
      , ExpectedDefAttrDecl                = 175
      , AttListSyntaxError                 = 176
      , ExpectedEqSign                     = 177
      , DupAttrName                        = 178
      , BadIdForXMLLangAttr                = 179
      , ExpectedElementName                = 180
      , MustStartWithXMLDecl               = 181
      , CommentsMustStartWith              = 182
      , InvalidDocumentStructure           = 183
      , ExpectedDeclString                 = 184
      , BadXMLVersion                      = 185
      , UnsupportedXMLVersion              = 186
      , UnterminatedXMLDecl                = 187
      , BadXMLEncoding                     = 188
      , BadStandalone                      = 189
      , UnterminatedComment                = 190
      , PINameExpected                     = 191
      , UnterminatedPI                     = 192
      , InvalidCharacter                   = 193
      , UnexpectedTextBeforeRoot           = 194
      , UnterminatedStartTag               = 195
      , ExpectedAttrValue                  = 196
      , UnterminatedEndTag                 = 197
      , ExpectedAttributeType              = 198
      , ExpectedEndOfTagX                  = 199
      , ExpectedMarkup                     = 200
      , NotValidAfterContent               = 201
      , ExpectedComment                    = 202
      , ExpectedCommentOrPI                = 203
      , ExpectedWhitespace                 = 204
      , NoRootElemInDOCTYPE                = 205
      , ExpectedQuotedString               = 206
      , ExpectedPublicId                   = 207
      , InvalidPublicIdChar                = 208
      , UnterminatedDOCTYPE                = 209
      , InvalidCharacterInIntSubset        = 210
      , ExpectedCDATA                      = 211
      , InvalidInitialNameChar             = 212
      , InvalidNameChar                    = 213
      , UnexpectedWhitespace               = 214
      , InvalidCharacterInAttrValue        = 215
      , ExpectedMarkupDecl                 = 216
      , TextDeclNotLegalHere               = 217
      , ConditionalSectInIntSubset         = 218
      , ExpectedPEName                     = 219
      , UnterminatedEntityDecl             = 220
      , InvalidCharacterRef                = 221
      , UnterminatedCharRef                = 222
      , ExpectedEntityRefName              = 223
      , EntityNotFound                     = 224
      , NoUnparsedEntityRefs               = 225
      , UnterminatedEntityRef              = 226
      , RecursiveEntity                    = 227
      , PartialMarkupInEntity              = 228
      , UnterminatedElementDecl            = 229
      , ExpectedContentSpecExpr            = 230
      , ExpectedAsterisk                   = 231
      , UnterminatedContentModel           = 232
      , ExpectedSystemId                   = 233
      , ExpectedSystemOrPublicId           = 234
      , UnterminatedNotationDecl           = 235
      , ExpectedSeqChoiceLeaf              = 236
      , ExpectedChoiceOrCloseParen         = 237
      , ExpectedSeqOrCloseParen            = 238
      , ExpectedEnumValue                  = 239
      , ExpectedEnumSepOrParen             = 240
      , UnterminatedEntityLiteral          = 241
      , MoreEndThanStartTags               = 242
      , ExpectedOpenParen                  = 243
      , AttrAlreadyUsedInSTag              = 244
      , BracketInAttrValue                 = 245
      , Expected2ndSurrogateChar           = 246
      , ExpectedEndOfConditional           = 247
      , ExpectedIncOrIgn                   = 248
      , ExpectedINCLUDEBracket             = 249
      , ExpectedTextDecl                   = 250
      , ExpectedXMLDecl                    = 251
      , UnexpectedEOE                      = 252
      , PEPropogated                       = 253
      , ExtraCloseSquare                   = 254
      , PERefInMarkupInIntSubset           = 255
      , EntityPropogated                   = 256
      , ExpectedNumericalCharRef           = 257
      , ExpectedOpenSquareBracket          = 258
      , BadSequenceInCharData              = 259
      , IllegalSequenceInComment           = 260
      , UnterminatedCDATASection           = 261
      , ExpectedNDATA                      = 262
      , NDATANotValidForPE                 = 263
      , HexRadixMustBeLowerCase            = 264
      , DeclStringRep                      = 265
      , DeclStringsInWrongOrder            = 266
      , NoExtRefsInAttValue                = 267
      , XMLDeclMustBeLowerCase             = 268
      , ExpectedEntityValue                = 269
      , BadDigitForRadix                   = 270
      , EndedWithTagsOnStack               = 271
      , AmbiguousContentModel              = 272
      , NestedCDATA                        = 273
      , UnknownPrefix                      = 274
      , PartialTagMarkupError              = 275
      , EmptyMainEntity                    = 276
      , CDATAOutsideOfContent              = 277
      , OnlyCharRefsAllowedHere            = 278
      , Unexpected2ndSurrogateChar         = 279
      , NoPIStartsWithXML                  = 280
      , XMLDeclMustBeFirst                 = 281
      , XMLVersionRequired                 = 282
      , StandaloneNotLegal                 = 283
      , EncodingRequired                   = 284
      , TooManyColonsInName                = 285
      , InvalidColonPos                    = 286
      , ColonNotLegalWithNS                = 287
      , SysException                       = 288
      , XMLException_Fatal                 = 289
      , UnexpectedEOF                      = 290
      , UnexpectedError                    = 291
      , BadSchemaLocation                  = 292
      , NoGrammarResolver                  = 293
      , SchemaScanFatalError               = 294
      , IllegalRefInStandalone             = 295
      , PEBetweenDecl                      = 296
      , NoEmptyStrNamespace                = 297
      , NoUseOfxmlnsAsPrefix               = 298
      , NoUseOfxmlnsURI                    = 299
      , PrefixXMLNotMatchXMLURI            = 300
      , XMLURINotMatchXMLPrefix            = 301
      , NoXMLNSAsElementPrefix             = 302
      , CT_SimpleTypeChildRequired         = 303
      , F_HighBounds                       = 304
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

