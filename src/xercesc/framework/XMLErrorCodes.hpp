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
      , E_HighBounds                       = 165
      , F_LowBounds                        = 166
      , ExpectedCommentOrCDATA             = 167
      , ExpectedAttrName                   = 168
      , ExpectedNotationName               = 169
      , NoRepInMixed                       = 170
      , BadDefAttrDecl                     = 171
      , ExpectedDefAttrDecl                = 172
      , AttListSyntaxError                 = 173
      , ExpectedEqSign                     = 174
      , DupAttrName                        = 175
      , BadIdForXMLLangAttr                = 176
      , ExpectedElementName                = 177
      , MustStartWithXMLDecl               = 178
      , CommentsMustStartWith              = 179
      , InvalidDocumentStructure           = 180
      , ExpectedDeclString                 = 181
      , BadXMLVersion                      = 182
      , UnsupportedXMLVersion              = 183
      , UnterminatedXMLDecl                = 184
      , BadXMLEncoding                     = 185
      , BadStandalone                      = 186
      , UnterminatedComment                = 187
      , PINameExpected                     = 188
      , UnterminatedPI                     = 189
      , InvalidCharacter                   = 190
      , UnexpectedTextBeforeRoot           = 191
      , UnterminatedStartTag               = 192
      , ExpectedAttrValue                  = 193
      , UnterminatedEndTag                 = 194
      , ExpectedAttributeType              = 195
      , ExpectedEndOfTagX                  = 196
      , ExpectedMarkup                     = 197
      , NotValidAfterContent               = 198
      , ExpectedComment                    = 199
      , ExpectedCommentOrPI                = 200
      , ExpectedWhitespace                 = 201
      , NoRootElemInDOCTYPE                = 202
      , ExpectedQuotedString               = 203
      , ExpectedPublicId                   = 204
      , InvalidPublicIdChar                = 205
      , UnterminatedDOCTYPE                = 206
      , InvalidCharacterInIntSubset        = 207
      , ExpectedCDATA                      = 208
      , InvalidInitialNameChar             = 209
      , InvalidNameChar                    = 210
      , UnexpectedWhitespace               = 211
      , InvalidCharacterInAttrValue        = 212
      , ExpectedMarkupDecl                 = 213
      , TextDeclNotLegalHere               = 214
      , ConditionalSectInIntSubset         = 215
      , ExpectedPEName                     = 216
      , UnterminatedEntityDecl             = 217
      , InvalidCharacterRef                = 218
      , UnterminatedCharRef                = 219
      , ExpectedEntityRefName              = 220
      , EntityNotFound                     = 221
      , NoUnparsedEntityRefs               = 222
      , UnterminatedEntityRef              = 223
      , RecursiveEntity                    = 224
      , PartialMarkupInEntity              = 225
      , UnterminatedElementDecl            = 226
      , ExpectedContentSpecExpr            = 227
      , ExpectedAsterisk                   = 228
      , UnterminatedContentModel           = 229
      , ExpectedSystemId                   = 230
      , ExpectedSystemOrPublicId           = 231
      , UnterminatedNotationDecl           = 232
      , ExpectedSeqChoiceLeaf              = 233
      , ExpectedChoiceOrCloseParen         = 234
      , ExpectedSeqOrCloseParen            = 235
      , ExpectedEnumValue                  = 236
      , ExpectedEnumSepOrParen             = 237
      , UnterminatedEntityLiteral          = 238
      , MoreEndThanStartTags               = 239
      , ExpectedOpenParen                  = 240
      , AttrAlreadyUsedInSTag              = 241
      , BracketInAttrValue                 = 242
      , Expected2ndSurrogateChar           = 243
      , ExpectedEndOfConditional           = 244
      , ExpectedIncOrIgn                   = 245
      , ExpectedINCLUDEBracket             = 246
      , ExpectedTextDecl                   = 247
      , ExpectedXMLDecl                    = 248
      , UnexpectedEOE                      = 249
      , PEPropogated                       = 250
      , ExtraCloseSquare                   = 251
      , PERefInMarkupInIntSubset           = 252
      , EntityPropogated                   = 253
      , ExpectedNumericalCharRef           = 254
      , ExpectedOpenSquareBracket          = 255
      , BadSequenceInCharData              = 256
      , IllegalSequenceInComment           = 257
      , UnterminatedCDATASection           = 258
      , ExpectedNDATA                      = 259
      , NDATANotValidForPE                 = 260
      , HexRadixMustBeLowerCase            = 261
      , DeclStringRep                      = 262
      , DeclStringsInWrongOrder            = 263
      , NoExtRefsInAttValue                = 264
      , XMLDeclMustBeLowerCase             = 265
      , ExpectedEntityValue                = 266
      , BadDigitForRadix                   = 267
      , EndedWithTagsOnStack               = 268
      , AmbiguousContentModel              = 269
      , NestedCDATA                        = 270
      , UnknownPrefix                      = 271
      , PartialTagMarkupError              = 272
      , EmptyMainEntity                    = 273
      , CDATAOutsideOfContent              = 274
      , OnlyCharRefsAllowedHere            = 275
      , Unexpected2ndSurrogateChar         = 276
      , NoPIStartsWithXML                  = 277
      , XMLDeclMustBeFirst                 = 278
      , XMLVersionRequired                 = 279
      , StandaloneNotLegal                 = 280
      , EncodingRequired                   = 281
      , TooManyColonsInName                = 282
      , InvalidColonPos                    = 283
      , ColonNotLegalWithNS                = 284
      , SysException                       = 285
      , XMLException_Fatal                 = 286
      , UnexpectedEOF                      = 287
      , UnexpectedError                    = 288
      , BadSchemaLocation                  = 289
      , NoGrammarResolver                  = 290
      , SchemaScanFatalError               = 291
      , IllegalRefInStandalone             = 292
      , PEBetweenDecl                      = 293
      , NoEmptyStrNamespace                = 294
      , NoUseOfxmlnsAsPrefix               = 295
      , NoUseOfxmlnsURI                    = 296
      , PrefixXMLNotMatchXMLURI            = 297
      , XMLURINotMatchXMLPrefix            = 298
      , NoXMLNSAsElementPrefix             = 299
      , CT_SimpleTypeChildRequired         = 300
      , F_HighBounds                       = 301
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

