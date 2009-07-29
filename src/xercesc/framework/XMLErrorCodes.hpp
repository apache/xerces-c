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
      , XIncludeIncludeFailedResourceError   = 10
      , W_HighBounds                       = 11
      , E_LowBounds                        = 12
      , FeatureUnsupported                 = 13
      , TopLevelNoNameComplexType          = 14
      , TopLevelNoNameAttribute            = 15
      , NoNameRefAttribute                 = 16
      , NoNameRefElement                   = 17
      , NoNameRefGroup                     = 18
      , NoNameRefAttGroup                  = 19
      , AnonComplexTypeWithName            = 20
      , AnonSimpleTypeWithName             = 21
      , InvalidElementContent              = 22
      , SimpleTypeContentError             = 23
      , ExpectedSimpleTypeInList           = 24
      , ListUnionRestrictionError          = 25
      , SimpleTypeDerivationByListError    = 26
      , ExpectedSimpleTypeInRestriction    = 27
      , DuplicateFacet                     = 28
      , ExpectedSimpleTypeInUnion          = 29
      , EmptySimpleTypeContent             = 30
      , InvalidSimpleContent               = 31
      , UnspecifiedBase                    = 32
      , InvalidComplexContent              = 33
      , SchemaElementContentError          = 34
      , ContentError                       = 35
      , UnknownSimpleType                  = 36
      , UnknownComplexType                 = 37
      , UnresolvedPrefix                   = 38
      , RefElementNotFound                 = 39
      , TypeNotFound                       = 40
      , TopLevelAttributeNotFound          = 41
      , InvalidChildInComplexType          = 42
      , BaseTypeNotFound                   = 43
      , DatatypeValidatorCreationError     = 44
      , InvalidChildFollowingSimpleContent   = 45
      , InvalidChildFollowingConplexContent   = 46
      , AttributeDefaultFixedValue         = 47
      , NotOptionalDefaultAttValue         = 48
      , DuplicateAttribute                 = 49
      , AttributeWithTypeAndSimpleType     = 50
      , AttributeSimpleTypeNotFound        = 51
      , ElementWithFixedAndDefault         = 52
      , InvalidDeclarationName             = 53
      , ElementWithTypeAndAnonType         = 54
      , NotSimpleOrMixedElement            = 55
      , DisallowedSimpleTypeExtension      = 56
      , InvalidSimpleContentBase           = 57
      , InvalidComplexTypeBase             = 58
      , InvalidChildInSimpleContent        = 59
      , InvalidChildInComplexContent       = 60
      , AnnotationError                    = 61
      , DisallowedBaseDerivation           = 62
      , SubstitutionRepeated               = 63
      , UnionRepeated                      = 64
      , ExtensionRepeated                  = 65
      , ListRepeated                       = 66
      , RestrictionRepeated                = 67
      , InvalidBlockValue                  = 68
      , InvalidFinalValue                  = 69
      , InvalidSubstitutionGroupElement    = 70
      , SubstitutionGroupTypeMismatch      = 71
      , DuplicateElementDeclaration        = 72
      , InvalidAttValue                    = 73
      , AttributeRefContentError           = 74
      , DuplicateRefAttribute              = 75
      , ForbiddenDerivationByRestriction   = 76
      , ForbiddenDerivationByExtension     = 77
      , BaseNotComplexType                 = 78
      , ImportNamespaceDifference          = 79
      , DeclarationNoSchemaLocation        = 80
      , IncludeNamespaceDifference         = 81
      , OnlyAnnotationExpected             = 82
      , InvalidAttributeContent            = 83
      , AttributeRequiredGlobal            = 84
      , AttributeRequiredLocal             = 85
      , AttributeDisallowedGlobal          = 86
      , AttributeDisallowedLocal           = 87
      , InvalidMin2MaxOccurs               = 88
      , AnyAttributeContentError           = 89
      , NoNameGlobalElement                = 90
      , NoCircularDefinition               = 91
      , DuplicateGlobalType                = 92
      , DuplicateGlobalDeclaration         = 93
      , WS_CollapseExpected                = 94
      , Import_1_1                         = 95
      , Import_1_2                         = 96
      , ElemIDValueConstraint              = 97
      , NoNotationType                     = 98
      , EmptiableMixedContent              = 99
      , EmptyComplexRestrictionDerivation   = 100
      , MixedOrElementOnly                 = 101
      , InvalidContentRestriction          = 102
      , ForbiddenDerivation                = 103
      , AtomicItemType                     = 104
      , GroupContentError                  = 105
      , AttGroupContentError               = 106
      , MinMaxOnGroupChild                 = 107
      , DeclarationNotFound                = 108
      , AllContentLimited                  = 109
      , BadMinMaxAllCT                     = 110
      , BadMinMaxAllElem                   = 111
      , DuplicateAttInDerivation           = 112
      , NotExpressibleWildCardIntersection   = 113
      , BadAttDerivation_1                 = 114
      , BadAttDerivation_2                 = 115
      , BadAttDerivation_3                 = 116
      , BadAttDerivation_4                 = 117
      , BadAttDerivation_5                 = 118
      , BadAttDerivation_6                 = 119
      , BadAttDerivation_7                 = 120
      , BadAttDerivation_8                 = 121
      , BadAttDerivation_9                 = 122
      , AllContentError                    = 123
      , RedefineNamespaceDifference        = 124
      , Redefine_InvalidSimpleType         = 125
      , Redefine_InvalidSimpleTypeBase     = 126
      , Redefine_InvalidComplexType        = 127
      , Redefine_InvalidComplexTypeBase    = 128
      , Redefine_InvalidGroupMinMax        = 129
      , Redefine_DeclarationNotFound       = 130
      , Redefine_GroupRefCount             = 131
      , Redefine_AttGroupRefCount          = 132
      , Redefine_InvalidChild              = 133
      , Notation_DeclNotFound              = 134
      , IC_DuplicateDecl                   = 135
      , IC_BadContent                      = 136
      , IC_KeyRefReferNotFound             = 137
      , IC_KeyRefCardinality               = 138
      , IC_XPathExprMissing                = 139
      , AttUseCorrect                      = 140
      , AttDeclPropCorrect3                = 141
      , AttDeclPropCorrect5                = 142
      , AttGrpPropCorrect3                 = 143
      , InvalidTargetNSValue               = 144
      , XMLException_Error                 = 145
      , InvalidRedefine                    = 146
      , InvalidNSReference                 = 147
      , NotAllContent                      = 148
      , InvalidAnnotationContent           = 149
      , InvalidFacetName                   = 150
      , InvalidXMLSchemaRoot               = 151
      , CircularSubsGroup                  = 152
      , ELTSchemaNS                        = 153
      , InvalidAttTNS                      = 154
      , NSDeclInvalid                      = 155
      , DOMLevel1Node                      = 156
      , DuplicateAnyAttribute              = 157
      , AnyAttributeBeforeAttribute        = 158
      , E_HighBounds                       = 159
      , F_LowBounds                        = 160
      , EntityExpansionLimitExceeded       = 161
      , ExpectedCommentOrCDATA             = 162
      , ExpectedAttrName                   = 163
      , ExpectedNotationName               = 164
      , NoRepInMixed                       = 165
      , ExpectedDefAttrDecl                = 166
      , ExpectedEqSign                     = 167
      , ExpectedElementName                = 168
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
      , UnterminatedStartTag               = 181
      , ExpectedAttrValue                  = 182
      , UnterminatedEndTag                 = 183
      , ExpectedAttributeType              = 184
      , ExpectedEndOfTagX                  = 185
      , ExpectedMarkup                     = 186
      , NotValidAfterContent               = 187
      , ExpectedComment                    = 188
      , ExpectedCommentOrPI                = 189
      , ExpectedWhitespace                 = 190
      , NoRootElemInDOCTYPE                = 191
      , ExpectedQuotedString               = 192
      , ExpectedPublicId                   = 193
      , InvalidPublicIdChar                = 194
      , UnterminatedDOCTYPE                = 195
      , InvalidCharacterInIntSubset        = 196
      , UnexpectedWhitespace               = 197
      , InvalidCharacterInAttrValue        = 198
      , ExpectedMarkupDecl                 = 199
      , TextDeclNotLegalHere               = 200
      , ConditionalSectInIntSubset         = 201
      , ExpectedPEName                     = 202
      , UnterminatedEntityDecl             = 203
      , InvalidCharacterRef                = 204
      , UnterminatedCharRef                = 205
      , ExpectedEntityRefName              = 206
      , EntityNotFound                     = 207
      , NoUnparsedEntityRefs               = 208
      , UnterminatedEntityRef              = 209
      , RecursiveEntity                    = 210
      , PartialMarkupInEntity              = 211
      , UnterminatedElementDecl            = 212
      , ExpectedContentSpecExpr            = 213
      , ExpectedAsterisk                   = 214
      , UnterminatedContentModel           = 215
      , ExpectedSystemOrPublicId           = 216
      , UnterminatedNotationDecl           = 217
      , ExpectedSeqChoiceLeaf              = 218
      , ExpectedChoiceOrCloseParen         = 219
      , ExpectedSeqOrCloseParen            = 220
      , ExpectedEnumValue                  = 221
      , ExpectedEnumSepOrParen             = 222
      , UnterminatedEntityLiteral          = 223
      , MoreEndThanStartTags               = 224
      , ExpectedOpenParen                  = 225
      , AttrAlreadyUsedInSTag              = 226
      , BracketInAttrValue                 = 227
      , Expected2ndSurrogateChar           = 228
      , ExpectedEndOfConditional           = 229
      , ExpectedIncOrIgn                   = 230
      , ExpectedINCLUDEBracket             = 231
      , UnexpectedEOE                      = 232
      , PEPropogated                       = 233
      , ExtraCloseSquare                   = 234
      , PERefInMarkupInIntSubset           = 235
      , EntityPropogated                   = 236
      , ExpectedNumericalCharRef           = 237
      , ExpectedOpenSquareBracket          = 238
      , BadSequenceInCharData              = 239
      , IllegalSequenceInComment           = 240
      , UnterminatedCDATASection           = 241
      , ExpectedNDATA                      = 242
      , NDATANotValidForPE                 = 243
      , HexRadixMustBeLowerCase            = 244
      , DeclStringRep                      = 245
      , DeclStringsInWrongOrder            = 246
      , NoExtRefsInAttValue                = 247
      , XMLDeclMustBeLowerCase             = 248
      , ExpectedEntityValue                = 249
      , BadDigitForRadix                   = 250
      , EndedWithTagsOnStack               = 251
      , NestedCDATA                        = 252
      , UnknownPrefix                      = 253
      , PartialTagMarkupError              = 254
      , EmptyMainEntity                    = 255
      , CDATAOutsideOfContent              = 256
      , Unexpected2ndSurrogateChar         = 257
      , NoPIStartsWithXML                  = 258
      , XMLDeclMustBeFirst                 = 259
      , XMLVersionRequired                 = 260
      , StandaloneNotLegal                 = 261
      , EncodingRequired                   = 262
      , ColonNotLegalWithNS                = 263
      , XMLException_Fatal                 = 264
      , BadSchemaLocation                  = 265
      , SchemaScanFatalError               = 266
      , IllegalRefInStandalone             = 267
      , PEBetweenDecl                      = 268
      , NoEmptyStrNamespace                = 269
      , NoUseOfxmlnsAsPrefix               = 270
      , NoUseOfxmlnsURI                    = 271
      , PrefixXMLNotMatchXMLURI            = 272
      , XMLURINotMatchXMLPrefix            = 273
      , NoXMLNSAsElementPrefix             = 274
      , CT_SimpleTypeChildRequired         = 275
      , InvalidRootElemInDOCTYPE           = 276
      , InvalidElementName                 = 277
      , InvalidAttrName                    = 278
      , InvalidEntityRefName               = 279
      , DuplicateDocTypeDecl               = 280
      , XIncludeOrphanFallback             = 281
      , XIncludeNoHref                     = 282
      , XIncludeXPointerNotSupported       = 283
      , XIncludeInvalidParseVal            = 284
      , XIncludeMultipleFallbackElems      = 285
      , XIncludeIncludeFailedNoFallback    = 286
      , XIncludeCircularInclusionLoop      = 287
      , XIncludeCircularInclusionDocIncludesSelf   = 288
      , XIncludeDisallowedChild            = 289
      , XIncludeConflictingNotation        = 290
      , XIncludeConflictingEntity          = 291
      , F_HighBounds                       = 292
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

