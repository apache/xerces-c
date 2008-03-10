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
      , AttributeRequired                  = 84
      , AttributeDisallowed                = 85
      , InvalidMin2MaxOccurs               = 86
      , AnyAttributeContentError           = 87
      , NoNameGlobalElement                = 88
      , NoCircularDefinition               = 89
      , DuplicateGlobalType                = 90
      , DuplicateGlobalDeclaration         = 91
      , WS_CollapseExpected                = 92
      , Import_1_1                         = 93
      , Import_1_2                         = 94
      , ElemIDValueConstraint              = 95
      , NoNotationType                     = 96
      , EmptiableMixedContent              = 97
      , EmptyComplexRestrictionDerivation   = 98
      , MixedOrElementOnly                 = 99
      , InvalidContentRestriction          = 100
      , ForbiddenDerivation                = 101
      , AtomicItemType                     = 102
      , GroupContentError                  = 103
      , AttGroupContentError               = 104
      , MinMaxOnGroupChild                 = 105
      , DeclarationNotFound                = 106
      , AllContentLimited                  = 107
      , BadMinMaxAllCT                     = 108
      , BadMinMaxAllElem                   = 109
      , DuplicateAttInDerivation           = 110
      , NotExpressibleWildCardIntersection   = 111
      , BadAttDerivation_1                 = 112
      , BadAttDerivation_2                 = 113
      , BadAttDerivation_3                 = 114
      , BadAttDerivation_4                 = 115
      , BadAttDerivation_5                 = 116
      , BadAttDerivation_6                 = 117
      , BadAttDerivation_7                 = 118
      , BadAttDerivation_8                 = 119
      , BadAttDerivation_9                 = 120
      , AllContentError                    = 121
      , RedefineNamespaceDifference        = 122
      , Redefine_InvalidSimpleType         = 123
      , Redefine_InvalidSimpleTypeBase     = 124
      , Redefine_InvalidComplexType        = 125
      , Redefine_InvalidComplexTypeBase    = 126
      , Redefine_InvalidGroupMinMax        = 127
      , Redefine_DeclarationNotFound       = 128
      , Redefine_GroupRefCount             = 129
      , Redefine_AttGroupRefCount          = 130
      , Redefine_InvalidChild              = 131
      , Notation_DeclNotFound              = 132
      , IC_DuplicateDecl                   = 133
      , IC_BadContent                      = 134
      , IC_KeyRefReferNotFound             = 135
      , IC_KeyRefCardinality               = 136
      , IC_XPathExprMissing                = 137
      , AttUseCorrect                      = 138
      , AttDeclPropCorrect3                = 139
      , AttDeclPropCorrect5                = 140
      , AttGrpPropCorrect3                 = 141
      , InvalidTargetNSValue               = 142
      , XMLException_Error                 = 143
      , InvalidRedefine                    = 144
      , InvalidNSReference                 = 145
      , NotAllContent                      = 146
      , InvalidAnnotationContent           = 147
      , InvalidFacetName                   = 148
      , InvalidXMLSchemaRoot               = 149
      , CircularSubsGroup                  = 150
      , ELTSchemaNS                        = 151
      , InvalidAttTNS                      = 152
      , NSDeclInvalid                      = 153
      , DOMLevel1Node                      = 154
      , E_HighBounds                       = 155
      , F_LowBounds                        = 156
      , EntityExpansionLimitExceeded       = 157
      , ExpectedCommentOrCDATA             = 158
      , ExpectedAttrName                   = 159
      , ExpectedNotationName               = 160
      , NoRepInMixed                       = 161
      , ExpectedDefAttrDecl                = 162
      , ExpectedEqSign                     = 163
      , ExpectedElementName                = 164
      , CommentsMustStartWith              = 165
      , InvalidDocumentStructure           = 166
      , ExpectedDeclString                 = 167
      , BadXMLVersion                      = 168
      , UnsupportedXMLVersion              = 169
      , UnterminatedXMLDecl                = 170
      , BadXMLEncoding                     = 171
      , BadStandalone                      = 172
      , UnterminatedComment                = 173
      , PINameExpected                     = 174
      , UnterminatedPI                     = 175
      , InvalidCharacter                   = 176
      , UnterminatedStartTag               = 177
      , ExpectedAttrValue                  = 178
      , UnterminatedEndTag                 = 179
      , ExpectedAttributeType              = 180
      , ExpectedEndOfTagX                  = 181
      , ExpectedMarkup                     = 182
      , NotValidAfterContent               = 183
      , ExpectedComment                    = 184
      , ExpectedCommentOrPI                = 185
      , ExpectedWhitespace                 = 186
      , NoRootElemInDOCTYPE                = 187
      , ExpectedQuotedString               = 188
      , ExpectedPublicId                   = 189
      , InvalidPublicIdChar                = 190
      , UnterminatedDOCTYPE                = 191
      , InvalidCharacterInIntSubset        = 192
      , UnexpectedWhitespace               = 193
      , InvalidCharacterInAttrValue        = 194
      , ExpectedMarkupDecl                 = 195
      , TextDeclNotLegalHere               = 196
      , ConditionalSectInIntSubset         = 197
      , ExpectedPEName                     = 198
      , UnterminatedEntityDecl             = 199
      , InvalidCharacterRef                = 200
      , UnterminatedCharRef                = 201
      , ExpectedEntityRefName              = 202
      , EntityNotFound                     = 203
      , NoUnparsedEntityRefs               = 204
      , UnterminatedEntityRef              = 205
      , RecursiveEntity                    = 206
      , PartialMarkupInEntity              = 207
      , UnterminatedElementDecl            = 208
      , ExpectedContentSpecExpr            = 209
      , ExpectedAsterisk                   = 210
      , UnterminatedContentModel           = 211
      , ExpectedSystemOrPublicId           = 212
      , UnterminatedNotationDecl           = 213
      , ExpectedSeqChoiceLeaf              = 214
      , ExpectedChoiceOrCloseParen         = 215
      , ExpectedSeqOrCloseParen            = 216
      , ExpectedEnumValue                  = 217
      , ExpectedEnumSepOrParen             = 218
      , UnterminatedEntityLiteral          = 219
      , MoreEndThanStartTags               = 220
      , ExpectedOpenParen                  = 221
      , AttrAlreadyUsedInSTag              = 222
      , BracketInAttrValue                 = 223
      , Expected2ndSurrogateChar           = 224
      , ExpectedEndOfConditional           = 225
      , ExpectedIncOrIgn                   = 226
      , ExpectedINCLUDEBracket             = 227
      , UnexpectedEOE                      = 228
      , PEPropogated                       = 229
      , ExtraCloseSquare                   = 230
      , PERefInMarkupInIntSubset           = 231
      , EntityPropogated                   = 232
      , ExpectedNumericalCharRef           = 233
      , ExpectedOpenSquareBracket          = 234
      , BadSequenceInCharData              = 235
      , IllegalSequenceInComment           = 236
      , UnterminatedCDATASection           = 237
      , ExpectedNDATA                      = 238
      , NDATANotValidForPE                 = 239
      , HexRadixMustBeLowerCase            = 240
      , DeclStringRep                      = 241
      , DeclStringsInWrongOrder            = 242
      , NoExtRefsInAttValue                = 243
      , XMLDeclMustBeLowerCase             = 244
      , ExpectedEntityValue                = 245
      , BadDigitForRadix                   = 246
      , EndedWithTagsOnStack               = 247
      , NestedCDATA                        = 248
      , UnknownPrefix                      = 249
      , PartialTagMarkupError              = 250
      , EmptyMainEntity                    = 251
      , CDATAOutsideOfContent              = 252
      , Unexpected2ndSurrogateChar         = 253
      , NoPIStartsWithXML                  = 254
      , XMLDeclMustBeFirst                 = 255
      , XMLVersionRequired                 = 256
      , StandaloneNotLegal                 = 257
      , EncodingRequired                   = 258
      , ColonNotLegalWithNS                = 259
      , XMLException_Fatal                 = 260
      , BadSchemaLocation                  = 261
      , SchemaScanFatalError               = 262
      , IllegalRefInStandalone             = 263
      , PEBetweenDecl                      = 264
      , NoEmptyStrNamespace                = 265
      , NoUseOfxmlnsAsPrefix               = 266
      , NoUseOfxmlnsURI                    = 267
      , PrefixXMLNotMatchXMLURI            = 268
      , XMLURINotMatchXMLPrefix            = 269
      , NoXMLNSAsElementPrefix             = 270
      , CT_SimpleTypeChildRequired         = 271
      , InvalidRootElemInDOCTYPE           = 272
      , InvalidElementName                 = 273
      , InvalidAttrName                    = 274
      , InvalidEntityRefName               = 275
      , DuplicateDocTypeDecl               = 276
      , XIncludeOrphanFallback             = 277
      , XIncludeNoHref                     = 278
      , XIncludeXPointerNotSupported       = 279
      , XIncludeInvalidParseVal            = 280
      , XIncludeMultipleFallbackElems      = 281
      , XIncludeIncludeFailedNoFallback    = 282
      , XIncludeCircularInclusionLoop      = 283
      , XIncludeCircularInclusionDocIncludesSelf   = 284
      , XIncludeDisallowedChild            = 285
      , XIncludeConflictingNotation        = 286
      , XIncludeConflictingEntity          = 287
      , F_HighBounds                       = 288
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

