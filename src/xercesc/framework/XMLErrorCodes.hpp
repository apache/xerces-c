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
      , PD_EmptyBase                       = 150
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
      , E_HighBounds                       = 164
      , F_LowBounds                        = 165
      , ExpectedCommentOrCDATA             = 166
      , ExpectedAttrName                   = 167
      , ExpectedNotationName               = 168
      , NoRepInMixed                       = 169
      , BadDefAttrDecl                     = 170
      , ExpectedDefAttrDecl                = 171
      , AttListSyntaxError                 = 172
      , ExpectedEqSign                     = 173
      , DupAttrName                        = 174
      , BadIdForXMLLangAttr                = 175
      , ExpectedElementName                = 176
      , MustStartWithXMLDecl               = 177
      , CommentsMustStartWith              = 178
      , InvalidDocumentStructure           = 179
      , ExpectedDeclString                 = 180
      , BadXMLVersion                      = 181
      , UnsupportedXMLVersion              = 182
      , UnterminatedXMLDecl                = 183
      , BadXMLEncoding                     = 184
      , BadStandalone                      = 185
      , UnterminatedComment                = 186
      , PINameExpected                     = 187
      , UnterminatedPI                     = 188
      , InvalidCharacter                   = 189
      , UnexpectedTextBeforeRoot           = 190
      , UnterminatedStartTag               = 191
      , ExpectedAttrValue                  = 192
      , UnterminatedEndTag                 = 193
      , ExpectedAttributeType              = 194
      , ExpectedEndOfTagX                  = 195
      , ExpectedMarkup                     = 196
      , NotValidAfterContent               = 197
      , ExpectedComment                    = 198
      , ExpectedCommentOrPI                = 199
      , ExpectedWhitespace                 = 200
      , NoRootElemInDOCTYPE                = 201
      , ExpectedQuotedString               = 202
      , ExpectedPublicId                   = 203
      , InvalidPublicIdChar                = 204
      , UnterminatedDOCTYPE                = 205
      , InvalidCharacterInIntSubset        = 206
      , ExpectedCDATA                      = 207
      , InvalidInitialNameChar             = 208
      , InvalidNameChar                    = 209
      , UnexpectedWhitespace               = 210
      , InvalidCharacterInAttrValue        = 211
      , ExpectedMarkupDecl                 = 212
      , TextDeclNotLegalHere               = 213
      , ConditionalSectInIntSubset         = 214
      , ExpectedPEName                     = 215
      , UnterminatedEntityDecl             = 216
      , InvalidCharacterRef                = 217
      , UnterminatedCharRef                = 218
      , ExpectedEntityRefName              = 219
      , EntityNotFound                     = 220
      , NoUnparsedEntityRefs               = 221
      , UnterminatedEntityRef              = 222
      , RecursiveEntity                    = 223
      , PartialMarkupInEntity              = 224
      , UnterminatedElementDecl            = 225
      , ExpectedContentSpecExpr            = 226
      , ExpectedAsterisk                   = 227
      , UnterminatedContentModel           = 228
      , ExpectedSystemId                   = 229
      , ExpectedSystemOrPublicId           = 230
      , UnterminatedNotationDecl           = 231
      , ExpectedSeqChoiceLeaf              = 232
      , ExpectedChoiceOrCloseParen         = 233
      , ExpectedSeqOrCloseParen            = 234
      , ExpectedEnumValue                  = 235
      , ExpectedEnumSepOrParen             = 236
      , UnterminatedEntityLiteral          = 237
      , MoreEndThanStartTags               = 238
      , ExpectedOpenParen                  = 239
      , AttrAlreadyUsedInSTag              = 240
      , BracketInAttrValue                 = 241
      , Expected2ndSurrogateChar           = 242
      , ExpectedEndOfConditional           = 243
      , ExpectedIncOrIgn                   = 244
      , ExpectedINCLUDEBracket             = 245
      , ExpectedTextDecl                   = 246
      , ExpectedXMLDecl                    = 247
      , UnexpectedEOE                      = 248
      , PEPropogated                       = 249
      , ExtraCloseSquare                   = 250
      , PERefInMarkupInIntSubset           = 251
      , EntityPropogated                   = 252
      , ExpectedNumericalCharRef           = 253
      , ExpectedOpenSquareBracket          = 254
      , BadSequenceInCharData              = 255
      , IllegalSequenceInComment           = 256
      , UnterminatedCDATASection           = 257
      , ExpectedNDATA                      = 258
      , NDATANotValidForPE                 = 259
      , HexRadixMustBeLowerCase            = 260
      , DeclStringRep                      = 261
      , DeclStringsInWrongOrder            = 262
      , NoExtRefsInAttValue                = 263
      , XMLDeclMustBeLowerCase             = 264
      , ExpectedEntityValue                = 265
      , BadDigitForRadix                   = 266
      , EndedWithTagsOnStack               = 267
      , AmbiguousContentModel              = 268
      , NestedCDATA                        = 269
      , UnknownPrefix                      = 270
      , PartialTagMarkupError              = 271
      , EmptyMainEntity                    = 272
      , CDATAOutsideOfContent              = 273
      , OnlyCharRefsAllowedHere            = 274
      , Unexpected2ndSurrogateChar         = 275
      , NoPIStartsWithXML                  = 276
      , XMLDeclMustBeFirst                 = 277
      , XMLVersionRequired                 = 278
      , StandaloneNotLegal                 = 279
      , EncodingRequired                   = 280
      , TooManyColonsInName                = 281
      , InvalidColonPos                    = 282
      , ColonNotLegalWithNS                = 283
      , SysException                       = 284
      , XMLException_Fatal                 = 285
      , UnexpectedEOF                      = 286
      , UnexpectedError                    = 287
      , BadSchemaLocation                  = 288
      , NoGrammarResolver                  = 289
      , SchemaScanFatalError               = 290
      , IllegalRefInStandalone             = 291
      , PEBetweenDecl                      = 292
      , NoEmptyStrNamespace                = 293
      , NoUseOfxmlnsAsPrefix               = 294
      , NoUseOfxmlnsURI                    = 295
      , PrefixXMLNotMatchXMLURI            = 296
      , XMLURINotMatchXMLPrefix            = 297
      , NoXMLNSAsElementPrefix             = 298
      , CT_SimpleTypeChildRequired         = 299
      , F_HighBounds                       = 300
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

