// This file is generated, don't edit it!!

#if !defined(XERCESC_INCLUDE_GUARD_ERRHEADER_XMLErrs)
#define XERCESC_INCLUDE_GUARD_ERRHEADER_XMLErrs

#include <xercesc/framework/XMLErrorReporter.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOMError.hpp>

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
      , XIncludeResourceErrorWarning       = 8
      , XIncludeCannotOpenFile             = 9
      , XIncludeInternalTextIncError       = 10
      , XIncludeIncludeFailedResourceError   = 11
      , W_HighBounds                       = 12
      , E_LowBounds                        = 13
      , FeatureUnsupported                 = 14
      , TopLevelNoNameComplexType          = 15
      , TopLevelNoNameAttribute            = 16
      , NoNameRefAttribute                 = 17
      , GlobalNoNameElement                = 18
      , NoNameRefElement                   = 19
      , NoNameRefGroup                     = 20
      , NoNameRefAttGroup                  = 21
      , AnonComplexTypeWithName            = 22
      , AnonSimpleTypeWithName             = 23
      , InvalidElementContent              = 24
      , UntypedElement                     = 25
      , SimpleTypeContentError             = 26
      , ExpectedSimpleTypeInList           = 27
      , ListUnionRestrictionError          = 28
      , SimpleTypeDerivationByListError    = 29
      , ExpectedSimpleTypeInRestriction    = 30
      , DuplicateFacet                     = 31
      , ExpectedSimpleTypeInUnion          = 32
      , EmptySimpleTypeContent             = 33
      , InvalidSimpleContent               = 34
      , UnspecifiedBase                    = 35
      , InvalidComplexContent              = 36
      , SchemaElementContentError          = 37
      , ContentError                       = 38
      , UnknownSimpleType                  = 39
      , UnknownComplexType                 = 40
      , UnresolvedPrefix                   = 41
      , RefElementNotFound                 = 42
      , TypeNotFound                       = 43
      , TopLevelAttributeNotFound          = 44
      , InvalidChildInComplexType          = 45
      , BaseTypeNotFound                   = 46
      , NoAttributeInSchema                = 47
      , DatatypeValidatorCreationError     = 48
      , InvalidChildFollowingSimpleContent   = 49
      , InvalidChildFollowingConplexContent   = 50
      , InvalidComplexTypeBlockValue       = 51
      , InvalidComplexTypeFinalValue       = 52
      , AttributeDefaultFixedValue         = 53
      , NotOptionalDefaultAttValue         = 54
      , LocalAttributeWithNameRef          = 55
      , GlobalAttributeWithNameRef         = 56
      , DuplicateAttribute                 = 57
      , AttributeWithTypeAndSimpleType     = 58
      , AttributeSimpleTypeNotFound        = 59
      , ElementWithFixedAndDefault         = 60
      , DeclarationWithNameRef             = 61
      , BadAttWithRef                      = 62
      , InvalidDeclarationName             = 63
      , GlobalElementWithRef               = 64
      , ElementWithTypeAndAnonType         = 65
      , NotSimpleOrMixedElement            = 66
      , DisallowedSimpleTypeExtension      = 67
      , InvalidSimpleContentBase           = 68
      , InvalidComplexTypeBase             = 69
      , InvalidChildInSimpleContent        = 70
      , InvalidChildInComplexContent       = 71
      , AnnotationError                    = 72
      , DisallowedBaseDerivation           = 73
      , SubstitutionRepeated               = 74
      , UnionRepeated                      = 75
      , ExtensionRepeated                  = 76
      , ListRepeated                       = 77
      , RestrictionRepeated                = 78
      , InvalidBlockValue                  = 79
      , InvalidFinalValue                  = 80
      , InvalidSubstitutionGroupElement    = 81
      , SubstitutionGroupTypeMismatch      = 82
      , DuplicateElementDeclaration        = 83
      , InvalidElementBlockValue           = 84
      , InvalidElementFinalValue           = 85
      , InvalidAttValue                    = 86
      , AttributeRefContentError           = 87
      , DuplicateRefAttribute              = 88
      , ForbiddenDerivationByRestriction   = 89
      , ForbiddenDerivationByExtension     = 90
      , BaseNotComplexType                 = 91
      , ImportNamespaceDifference          = 92
      , ImportRootError                    = 93
      , DeclarationNoSchemaLocation        = 94
      , IncludeNamespaceDifference         = 95
      , OnlyAnnotationExpected             = 96
      , InvalidAttributeContent            = 97
      , AttributeRequired                  = 98
      , AttributeDisallowed                = 99
      , InvalidMin2MaxOccurs               = 100
      , AnyAttributeContentError           = 101
      , NoNameGlobalElement                = 102
      , NoCircularDefinition               = 103
      , DuplicateGlobalType                = 104
      , DuplicateGlobalDeclaration         = 105
      , WS_CollapseExpected                = 106
      , Import_1_1                         = 107
      , Import_1_2                         = 108
      , ElemIDValueConstraint              = 109
      , NoNotationType                     = 110
      , EmptiableMixedContent              = 111
      , EmptyComplexRestrictionDerivation   = 112
      , MixedOrElementOnly                 = 113
      , InvalidContentRestriction          = 114
      , ForbiddenDerivation                = 115
      , AtomicItemType                     = 116
      , MemberTypeNoUnion                  = 117
      , GroupContentError                  = 118
      , AttGroupContentError               = 119
      , MinMaxOnGroupChild                 = 120
      , DeclarationNotFound                = 121
      , AllContentLimited                  = 122
      , BadMinMaxAllCT                     = 123
      , BadMinMaxAllElem                   = 124
      , NoCircularAttGroup                 = 125
      , DuplicateAttInDerivation           = 126
      , NotExpressibleWildCardIntersection   = 127
      , BadAttDerivation_1                 = 128
      , BadAttDerivation_2                 = 129
      , BadAttDerivation_3                 = 130
      , BadAttDerivation_4                 = 131
      , BadAttDerivation_5                 = 132
      , BadAttDerivation_6                 = 133
      , BadAttDerivation_7                 = 134
      , BadAttDerivation_8                 = 135
      , BadAttDerivation_9                 = 136
      , AllContentError                    = 137
      , RedefineNamespaceDifference        = 138
      , Redefine_InvalidSimpleType         = 139
      , Redefine_InvalidSimpleTypeBase     = 140
      , Redefine_InvalidComplexType        = 141
      , Redefine_InvalidComplexTypeBase    = 142
      , Redefine_InvalidGroupMinMax        = 143
      , Redefine_DeclarationNotFound       = 144
      , Redefine_GroupRefCount             = 145
      , Redefine_AttGroupRefCount          = 146
      , Redefine_InvalidChild              = 147
      , Notation_InvalidDecl               = 148
      , Notation_DeclNotFound              = 149
      , IC_DuplicateDecl                   = 150
      , IC_BadContent                      = 151
      , IC_KeyRefReferNotFound             = 152
      , IC_KeyRefCardinality               = 153
      , IC_XPathExprMissing                = 154
      , AttUseCorrect                      = 155
      , AttDeclPropCorrect3                = 156
      , AttDeclPropCorrect5                = 157
      , AttGrpPropCorrect3                 = 158
      , InvalidTargetNSValue               = 159
      , DisplayErrorMessage                = 160
      , XMLException_Error                 = 161
      , InvalidRedefine                    = 162
      , InvalidNSReference                 = 163
      , NotAllContent                      = 164
      , InvalidAnnotationContent           = 165
      , InvalidFacetName                   = 166
      , InvalidXMLSchemaRoot               = 167
      , CircularSubsGroup                  = 168
      , SubsGroupMemberAbstract            = 169
      , ELTSchemaNS                        = 170
      , InvalidAttTNS                      = 171
      , NSDeclInvalid                      = 172
      , DOMLevel1Node                      = 173
      , E_HighBounds                       = 174
      , F_LowBounds                        = 175
      , EntityExpansionLimitExceeded       = 176
      , ExpectedCommentOrCDATA             = 177
      , ExpectedAttrName                   = 178
      , ExpectedNotationName               = 179
      , NoRepInMixed                       = 180
      , BadDefAttrDecl                     = 181
      , ExpectedDefAttrDecl                = 182
      , AttListSyntaxError                 = 183
      , ExpectedEqSign                     = 184
      , DupAttrName                        = 185
      , BadIdForXMLLangAttr                = 186
      , ExpectedElementName                = 187
      , MustStartWithXMLDecl               = 188
      , CommentsMustStartWith              = 189
      , InvalidDocumentStructure           = 190
      , ExpectedDeclString                 = 191
      , BadXMLVersion                      = 192
      , UnsupportedXMLVersion              = 193
      , UnterminatedXMLDecl                = 194
      , BadXMLEncoding                     = 195
      , BadStandalone                      = 196
      , UnterminatedComment                = 197
      , PINameExpected                     = 198
      , UnterminatedPI                     = 199
      , InvalidCharacter                   = 200
      , UnexpectedTextBeforeRoot           = 201
      , UnterminatedStartTag               = 202
      , ExpectedAttrValue                  = 203
      , UnterminatedEndTag                 = 204
      , ExpectedAttributeType              = 205
      , ExpectedEndOfTagX                  = 206
      , ExpectedMarkup                     = 207
      , NotValidAfterContent               = 208
      , ExpectedComment                    = 209
      , ExpectedCommentOrPI                = 210
      , ExpectedWhitespace                 = 211
      , NoRootElemInDOCTYPE                = 212
      , ExpectedQuotedString               = 213
      , ExpectedPublicId                   = 214
      , InvalidPublicIdChar                = 215
      , UnterminatedDOCTYPE                = 216
      , InvalidCharacterInIntSubset        = 217
      , ExpectedCDATA                      = 218
      , InvalidInitialNameChar             = 219
      , InvalidNameChar                    = 220
      , UnexpectedWhitespace               = 221
      , InvalidCharacterInAttrValue        = 222
      , ExpectedMarkupDecl                 = 223
      , TextDeclNotLegalHere               = 224
      , ConditionalSectInIntSubset         = 225
      , ExpectedPEName                     = 226
      , UnterminatedEntityDecl             = 227
      , InvalidCharacterRef                = 228
      , UnterminatedCharRef                = 229
      , ExpectedEntityRefName              = 230
      , EntityNotFound                     = 231
      , NoUnparsedEntityRefs               = 232
      , UnterminatedEntityRef              = 233
      , RecursiveEntity                    = 234
      , PartialMarkupInEntity              = 235
      , UnterminatedElementDecl            = 236
      , ExpectedContentSpecExpr            = 237
      , ExpectedAsterisk                   = 238
      , UnterminatedContentModel           = 239
      , ExpectedSystemId                   = 240
      , ExpectedSystemOrPublicId           = 241
      , UnterminatedNotationDecl           = 242
      , ExpectedSeqChoiceLeaf              = 243
      , ExpectedChoiceOrCloseParen         = 244
      , ExpectedSeqOrCloseParen            = 245
      , ExpectedEnumValue                  = 246
      , ExpectedEnumSepOrParen             = 247
      , UnterminatedEntityLiteral          = 248
      , MoreEndThanStartTags               = 249
      , ExpectedOpenParen                  = 250
      , AttrAlreadyUsedInSTag              = 251
      , BracketInAttrValue                 = 252
      , Expected2ndSurrogateChar           = 253
      , ExpectedEndOfConditional           = 254
      , ExpectedIncOrIgn                   = 255
      , ExpectedINCLUDEBracket             = 256
      , ExpectedTextDecl                   = 257
      , ExpectedXMLDecl                    = 258
      , UnexpectedEOE                      = 259
      , PEPropogated                       = 260
      , ExtraCloseSquare                   = 261
      , PERefInMarkupInIntSubset           = 262
      , EntityPropogated                   = 263
      , ExpectedNumericalCharRef           = 264
      , ExpectedOpenSquareBracket          = 265
      , BadSequenceInCharData              = 266
      , IllegalSequenceInComment           = 267
      , UnterminatedCDATASection           = 268
      , ExpectedNDATA                      = 269
      , NDATANotValidForPE                 = 270
      , HexRadixMustBeLowerCase            = 271
      , DeclStringRep                      = 272
      , DeclStringsInWrongOrder            = 273
      , NoExtRefsInAttValue                = 274
      , XMLDeclMustBeLowerCase             = 275
      , ExpectedEntityValue                = 276
      , BadDigitForRadix                   = 277
      , EndedWithTagsOnStack               = 278
      , AmbiguousContentModel              = 279
      , NestedCDATA                        = 280
      , UnknownPrefix                      = 281
      , PartialTagMarkupError              = 282
      , EmptyMainEntity                    = 283
      , CDATAOutsideOfContent              = 284
      , OnlyCharRefsAllowedHere            = 285
      , Unexpected2ndSurrogateChar         = 286
      , NoPIStartsWithXML                  = 287
      , XMLDeclMustBeFirst                 = 288
      , XMLVersionRequired                 = 289
      , StandaloneNotLegal                 = 290
      , EncodingRequired                   = 291
      , TooManyColonsInName                = 292
      , InvalidColonPos                    = 293
      , ColonNotLegalWithNS                = 294
      , SysException                       = 295
      , XMLException_Fatal                 = 296
      , UnexpectedEOF                      = 297
      , UnexpectedError                    = 298
      , BadSchemaLocation                  = 299
      , NoGrammarResolver                  = 300
      , SchemaScanFatalError               = 301
      , IllegalRefInStandalone             = 302
      , PEBetweenDecl                      = 303
      , NoEmptyStrNamespace                = 304
      , NoUseOfxmlnsAsPrefix               = 305
      , NoUseOfxmlnsURI                    = 306
      , PrefixXMLNotMatchXMLURI            = 307
      , XMLURINotMatchXMLPrefix            = 308
      , NoXMLNSAsElementPrefix             = 309
      , CT_SimpleTypeChildRequired         = 310
      , InvalidRootElemInDOCTYPE           = 311
      , InvalidElementName                 = 312
      , InvalidAttrName                    = 313
      , InvalidEntityRefName               = 314
      , DuplicateDocTypeDecl               = 315
      , XIncludeOrphanFallback             = 316
      , XIncludeNoHref                     = 317
      , XIncludeXPointerNotSupported       = 318
      , XIncludeInvalidParseVal            = 319
      , XIncludeMultipleFallbackElems      = 320
      , XIncludeIncludeFailedNoFallback    = 321
      , XIncludeCircularInclusionLoop      = 322
      , XIncludeCircularInclusionDocIncludesSelf   = 323
      , F_HighBounds                       = 324
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
    static DOMError::ErrorSeverity  DOMErrorType(const XMLErrs::Codes toCheck)
    {
       if ((toCheck >= W_LowBounds) && (toCheck <= W_HighBounds))
           return DOMError::DOM_SEVERITY_WARNING;
       else if ((toCheck >= F_LowBounds) && (toCheck <= F_HighBounds))
            return DOMError::DOM_SEVERITY_FATAL_ERROR;
       else return DOMError::DOM_SEVERITY_ERROR;
    }

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLErrs();
};

XERCES_CPP_NAMESPACE_END

#endif

