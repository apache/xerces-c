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
      , Notation_InvalidDecl               = 143
      , Notation_DeclNotFound              = 144
      , IC_DuplicateDecl                   = 145
      , IC_BadContent                      = 146
      , IC_KeyRefReferNotFound             = 147
      , IC_KeyRefCardinality               = 148
      , IC_XPathExprMissing                = 149
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
      , CircularSubsGroup                  = 163
      , SubsGroupMemberAbstract            = 164
      , ELTSchemaNS                        = 165
      , E_HighBounds                       = 166
      , F_LowBounds                        = 167
      , ExpectedCommentOrCDATA             = 168
      , ExpectedAttrName                   = 169
      , ExpectedNotationName               = 170
      , NoRepInMixed                       = 171
      , BadDefAttrDecl                     = 172
      , ExpectedDefAttrDecl                = 173
      , AttListSyntaxError                 = 174
      , ExpectedEqSign                     = 175
      , DupAttrName                        = 176
      , BadIdForXMLLangAttr                = 177
      , ExpectedElementName                = 178
      , MustStartWithXMLDecl               = 179
      , CommentsMustStartWith              = 180
      , InvalidDocumentStructure           = 181
      , ExpectedDeclString                 = 182
      , BadXMLVersion                      = 183
      , UnsupportedXMLVersion              = 184
      , UnterminatedXMLDecl                = 185
      , BadXMLEncoding                     = 186
      , BadStandalone                      = 187
      , UnterminatedComment                = 188
      , PINameExpected                     = 189
      , UnterminatedPI                     = 190
      , InvalidCharacter                   = 191
      , UnexpectedTextBeforeRoot           = 192
      , UnterminatedStartTag               = 193
      , ExpectedAttrValue                  = 194
      , UnterminatedEndTag                 = 195
      , ExpectedAttributeType              = 196
      , ExpectedEndOfTagX                  = 197
      , ExpectedMarkup                     = 198
      , NotValidAfterContent               = 199
      , ExpectedComment                    = 200
      , ExpectedCommentOrPI                = 201
      , ExpectedWhitespace                 = 202
      , NoRootElemInDOCTYPE                = 203
      , ExpectedQuotedString               = 204
      , ExpectedPublicId                   = 205
      , InvalidPublicIdChar                = 206
      , UnterminatedDOCTYPE                = 207
      , InvalidCharacterInIntSubset        = 208
      , ExpectedCDATA                      = 209
      , InvalidInitialNameChar             = 210
      , InvalidNameChar                    = 211
      , UnexpectedWhitespace               = 212
      , InvalidCharacterInAttrValue        = 213
      , ExpectedMarkupDecl                 = 214
      , TextDeclNotLegalHere               = 215
      , ConditionalSectInIntSubset         = 216
      , ExpectedPEName                     = 217
      , UnterminatedEntityDecl             = 218
      , InvalidCharacterRef                = 219
      , UnterminatedCharRef                = 220
      , ExpectedEntityRefName              = 221
      , EntityNotFound                     = 222
      , NoUnparsedEntityRefs               = 223
      , UnterminatedEntityRef              = 224
      , RecursiveEntity                    = 225
      , PartialMarkupInEntity              = 226
      , UnterminatedElementDecl            = 227
      , ExpectedContentSpecExpr            = 228
      , ExpectedAsterisk                   = 229
      , UnterminatedContentModel           = 230
      , ExpectedSystemId                   = 231
      , ExpectedSystemOrPublicId           = 232
      , UnterminatedNotationDecl           = 233
      , ExpectedSeqChoiceLeaf              = 234
      , ExpectedChoiceOrCloseParen         = 235
      , ExpectedSeqOrCloseParen            = 236
      , ExpectedEnumValue                  = 237
      , ExpectedEnumSepOrParen             = 238
      , UnterminatedEntityLiteral          = 239
      , MoreEndThanStartTags               = 240
      , ExpectedOpenParen                  = 241
      , AttrAlreadyUsedInSTag              = 242
      , BracketInAttrValue                 = 243
      , Expected2ndSurrogateChar           = 244
      , ExpectedEndOfConditional           = 245
      , ExpectedIncOrIgn                   = 246
      , ExpectedINCLUDEBracket             = 247
      , ExpectedTextDecl                   = 248
      , ExpectedXMLDecl                    = 249
      , UnexpectedEOE                      = 250
      , PEPropogated                       = 251
      , ExtraCloseSquare                   = 252
      , PERefInMarkupInIntSubset           = 253
      , EntityPropogated                   = 254
      , ExpectedNumericalCharRef           = 255
      , ExpectedOpenSquareBracket          = 256
      , BadSequenceInCharData              = 257
      , IllegalSequenceInComment           = 258
      , UnterminatedCDATASection           = 259
      , ExpectedNDATA                      = 260
      , NDATANotValidForPE                 = 261
      , HexRadixMustBeLowerCase            = 262
      , DeclStringRep                      = 263
      , DeclStringsInWrongOrder            = 264
      , NoExtRefsInAttValue                = 265
      , XMLDeclMustBeLowerCase             = 266
      , ExpectedEntityValue                = 267
      , BadDigitForRadix                   = 268
      , EndedWithTagsOnStack               = 269
      , AmbiguousContentModel              = 270
      , NestedCDATA                        = 271
      , UnknownPrefix                      = 272
      , PartialTagMarkupError              = 273
      , EmptyMainEntity                    = 274
      , CDATAOutsideOfContent              = 275
      , OnlyCharRefsAllowedHere            = 276
      , Unexpected2ndSurrogateChar         = 277
      , NoPIStartsWithXML                  = 278
      , XMLDeclMustBeFirst                 = 279
      , XMLVersionRequired                 = 280
      , StandaloneNotLegal                 = 281
      , EncodingRequired                   = 282
      , TooManyColonsInName                = 283
      , InvalidColonPos                    = 284
      , ColonNotLegalWithNS                = 285
      , SysException                       = 286
      , XMLException_Fatal                 = 287
      , UnexpectedEOF                      = 288
      , UnexpectedError                    = 289
      , BadSchemaLocation                  = 290
      , NoGrammarResolver                  = 291
      , SchemaScanFatalError               = 292
      , IllegalRefInStandalone             = 293
      , PEBetweenDecl                      = 294
      , NoEmptyStrNamespace                = 295
      , NoUseOfxmlnsAsPrefix               = 296
      , NoUseOfxmlnsURI                    = 297
      , PrefixXMLNotMatchXMLURI            = 298
      , XMLURINotMatchXMLPrefix            = 299
      , NoXMLNSAsElementPrefix             = 300
      , CT_SimpleTypeChildRequired         = 301
      , F_HighBounds                       = 302
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

