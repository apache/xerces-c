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
      , E_HighBounds                       = 167
      , F_LowBounds                        = 168
      , ExpectedCommentOrCDATA             = 169
      , ExpectedAttrName                   = 170
      , ExpectedNotationName               = 171
      , NoRepInMixed                       = 172
      , BadDefAttrDecl                     = 173
      , ExpectedDefAttrDecl                = 174
      , AttListSyntaxError                 = 175
      , ExpectedEqSign                     = 176
      , DupAttrName                        = 177
      , BadIdForXMLLangAttr                = 178
      , ExpectedElementName                = 179
      , MustStartWithXMLDecl               = 180
      , CommentsMustStartWith              = 181
      , InvalidDocumentStructure           = 182
      , ExpectedDeclString                 = 183
      , BadXMLVersion                      = 184
      , UnsupportedXMLVersion              = 185
      , UnterminatedXMLDecl                = 186
      , BadXMLEncoding                     = 187
      , BadStandalone                      = 188
      , UnterminatedComment                = 189
      , PINameExpected                     = 190
      , UnterminatedPI                     = 191
      , InvalidCharacter                   = 192
      , UnexpectedTextBeforeRoot           = 193
      , UnterminatedStartTag               = 194
      , ExpectedAttrValue                  = 195
      , UnterminatedEndTag                 = 196
      , ExpectedAttributeType              = 197
      , ExpectedEndOfTagX                  = 198
      , ExpectedMarkup                     = 199
      , NotValidAfterContent               = 200
      , ExpectedComment                    = 201
      , ExpectedCommentOrPI                = 202
      , ExpectedWhitespace                 = 203
      , NoRootElemInDOCTYPE                = 204
      , ExpectedQuotedString               = 205
      , ExpectedPublicId                   = 206
      , InvalidPublicIdChar                = 207
      , UnterminatedDOCTYPE                = 208
      , InvalidCharacterInIntSubset        = 209
      , ExpectedCDATA                      = 210
      , InvalidInitialNameChar             = 211
      , InvalidNameChar                    = 212
      , UnexpectedWhitespace               = 213
      , InvalidCharacterInAttrValue        = 214
      , ExpectedMarkupDecl                 = 215
      , TextDeclNotLegalHere               = 216
      , ConditionalSectInIntSubset         = 217
      , ExpectedPEName                     = 218
      , UnterminatedEntityDecl             = 219
      , InvalidCharacterRef                = 220
      , UnterminatedCharRef                = 221
      , ExpectedEntityRefName              = 222
      , EntityNotFound                     = 223
      , NoUnparsedEntityRefs               = 224
      , UnterminatedEntityRef              = 225
      , RecursiveEntity                    = 226
      , PartialMarkupInEntity              = 227
      , UnterminatedElementDecl            = 228
      , ExpectedContentSpecExpr            = 229
      , ExpectedAsterisk                   = 230
      , UnterminatedContentModel           = 231
      , ExpectedSystemId                   = 232
      , ExpectedSystemOrPublicId           = 233
      , UnterminatedNotationDecl           = 234
      , ExpectedSeqChoiceLeaf              = 235
      , ExpectedChoiceOrCloseParen         = 236
      , ExpectedSeqOrCloseParen            = 237
      , ExpectedEnumValue                  = 238
      , ExpectedEnumSepOrParen             = 239
      , UnterminatedEntityLiteral          = 240
      , MoreEndThanStartTags               = 241
      , ExpectedOpenParen                  = 242
      , AttrAlreadyUsedInSTag              = 243
      , BracketInAttrValue                 = 244
      , Expected2ndSurrogateChar           = 245
      , ExpectedEndOfConditional           = 246
      , ExpectedIncOrIgn                   = 247
      , ExpectedINCLUDEBracket             = 248
      , ExpectedTextDecl                   = 249
      , ExpectedXMLDecl                    = 250
      , UnexpectedEOE                      = 251
      , PEPropogated                       = 252
      , ExtraCloseSquare                   = 253
      , PERefInMarkupInIntSubset           = 254
      , EntityPropogated                   = 255
      , ExpectedNumericalCharRef           = 256
      , ExpectedOpenSquareBracket          = 257
      , BadSequenceInCharData              = 258
      , IllegalSequenceInComment           = 259
      , UnterminatedCDATASection           = 260
      , ExpectedNDATA                      = 261
      , NDATANotValidForPE                 = 262
      , HexRadixMustBeLowerCase            = 263
      , DeclStringRep                      = 264
      , DeclStringsInWrongOrder            = 265
      , NoExtRefsInAttValue                = 266
      , XMLDeclMustBeLowerCase             = 267
      , ExpectedEntityValue                = 268
      , BadDigitForRadix                   = 269
      , EndedWithTagsOnStack               = 270
      , AmbiguousContentModel              = 271
      , NestedCDATA                        = 272
      , UnknownPrefix                      = 273
      , PartialTagMarkupError              = 274
      , EmptyMainEntity                    = 275
      , CDATAOutsideOfContent              = 276
      , OnlyCharRefsAllowedHere            = 277
      , Unexpected2ndSurrogateChar         = 278
      , NoPIStartsWithXML                  = 279
      , XMLDeclMustBeFirst                 = 280
      , XMLVersionRequired                 = 281
      , StandaloneNotLegal                 = 282
      , EncodingRequired                   = 283
      , TooManyColonsInName                = 284
      , InvalidColonPos                    = 285
      , ColonNotLegalWithNS                = 286
      , SysException                       = 287
      , XMLException_Fatal                 = 288
      , UnexpectedEOF                      = 289
      , UnexpectedError                    = 290
      , BadSchemaLocation                  = 291
      , NoGrammarResolver                  = 292
      , SchemaScanFatalError               = 293
      , IllegalRefInStandalone             = 294
      , PEBetweenDecl                      = 295
      , NoEmptyStrNamespace                = 296
      , NoUseOfxmlnsAsPrefix               = 297
      , NoUseOfxmlnsURI                    = 298
      , PrefixXMLNotMatchXMLURI            = 299
      , XMLURINotMatchXMLPrefix            = 300
      , NoXMLNSAsElementPrefix             = 301
      , CT_SimpleTypeChildRequired         = 302
      , F_HighBounds                       = 303
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

