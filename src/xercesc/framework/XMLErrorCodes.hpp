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
      , E_HighBounds                       = 159
      , F_LowBounds                        = 160
      , ExpectedCommentOrCDATA             = 161
      , ExpectedAttrName                   = 162
      , ExpectedNotationName               = 163
      , NoRepInMixed                       = 164
      , BadDefAttrDecl                     = 165
      , ExpectedDefAttrDecl                = 166
      , AttListSyntaxError                 = 167
      , ExpectedEqSign                     = 168
      , DupAttrName                        = 169
      , BadIdForXMLLangAttr                = 170
      , ExpectedElementName                = 171
      , MustStartWithXMLDecl               = 172
      , CommentsMustStartWith              = 173
      , InvalidDocumentStructure           = 174
      , ExpectedDeclString                 = 175
      , BadXMLVersion                      = 176
      , UnsupportedXMLVersion              = 177
      , UnterminatedXMLDecl                = 178
      , BadXMLEncoding                     = 179
      , BadStandalone                      = 180
      , UnterminatedComment                = 181
      , PINameExpected                     = 182
      , UnterminatedPI                     = 183
      , InvalidCharacter                   = 184
      , UnexpectedTextBeforeRoot           = 185
      , UnterminatedStartTag               = 186
      , ExpectedAttrValue                  = 187
      , UnterminatedEndTag                 = 188
      , ExpectedAttributeType              = 189
      , ExpectedEndOfTagX                  = 190
      , ExpectedMarkup                     = 191
      , NotValidAfterContent               = 192
      , ExpectedComment                    = 193
      , ExpectedCommentOrPI                = 194
      , ExpectedWhitespace                 = 195
      , NoRootElemInDOCTYPE                = 196
      , ExpectedQuotedString               = 197
      , ExpectedPublicId                   = 198
      , InvalidPublicIdChar                = 199
      , UnterminatedDOCTYPE                = 200
      , InvalidCharacterInIntSubset        = 201
      , ExpectedCDATA                      = 202
      , InvalidInitialNameChar             = 203
      , InvalidNameChar                    = 204
      , UnexpectedWhitespace               = 205
      , InvalidCharacterInAttrValue        = 206
      , ExpectedMarkupDecl                 = 207
      , TextDeclNotLegalHere               = 208
      , ConditionalSectInIntSubset         = 209
      , ExpectedPEName                     = 210
      , UnterminatedEntityDecl             = 211
      , InvalidCharacterRef                = 212
      , UnterminatedCharRef                = 213
      , ExpectedEntityRefName              = 214
      , EntityNotFound                     = 215
      , NoUnparsedEntityRefs               = 216
      , UnterminatedEntityRef              = 217
      , RecursiveEntity                    = 218
      , PartialMarkupInEntity              = 219
      , UnterminatedElementDecl            = 220
      , ExpectedContentSpecExpr            = 221
      , ExpectedAsterisk                   = 222
      , UnterminatedContentModel           = 223
      , ExpectedSystemId                   = 224
      , ExpectedSystemOrPublicId           = 225
      , UnterminatedNotationDecl           = 226
      , ExpectedSeqChoiceLeaf              = 227
      , ExpectedChoiceOrCloseParen         = 228
      , ExpectedSeqOrCloseParen            = 229
      , ExpectedEnumValue                  = 230
      , ExpectedEnumSepOrParen             = 231
      , UnterminatedEntityLiteral          = 232
      , MoreEndThanStartTags               = 233
      , ExpectedOpenParen                  = 234
      , AttrAlreadyUsedInSTag              = 235
      , BracketInAttrValue                 = 236
      , Expected2ndSurrogateChar           = 237
      , ExpectedEndOfConditional           = 238
      , ExpectedIncOrIgn                   = 239
      , ExpectedINCLUDEBracket             = 240
      , ExpectedTextDecl                   = 241
      , ExpectedXMLDecl                    = 242
      , UnexpectedEOE                      = 243
      , PEPropogated                       = 244
      , ExtraCloseSquare                   = 245
      , PERefInMarkupInIntSubset           = 246
      , EntityPropogated                   = 247
      , ExpectedNumericalCharRef           = 248
      , ExpectedOpenSquareBracket          = 249
      , BadSequenceInCharData              = 250
      , IllegalSequenceInComment           = 251
      , UnterminatedCDATASection           = 252
      , ExpectedNDATA                      = 253
      , NDATANotValidForPE                 = 254
      , HexRadixMustBeLowerCase            = 255
      , DeclStringRep                      = 256
      , DeclStringsInWrongOrder            = 257
      , NoExtRefsInAttValue                = 258
      , XMLDeclMustBeLowerCase             = 259
      , ExpectedEntityValue                = 260
      , BadDigitForRadix                   = 261
      , EndedWithTagsOnStack               = 262
      , AmbiguousContentModel              = 263
      , NestedCDATA                        = 264
      , UnknownPrefix                      = 265
      , PartialTagMarkupError              = 266
      , EmptyMainEntity                    = 267
      , CDATAOutsideOfContent              = 268
      , OnlyCharRefsAllowedHere            = 269
      , Unexpected2ndSurrogateChar         = 270
      , NoPIStartsWithXML                  = 271
      , XMLDeclMustBeFirst                 = 272
      , XMLVersionRequired                 = 273
      , StandaloneNotLegal                 = 274
      , EncodingRequired                   = 275
      , TooManyColonsInName                = 276
      , InvalidColonPos                    = 277
      , ColonNotLegalWithNS                = 278
      , SysException                       = 279
      , XMLException_Fatal                 = 280
      , UnexpectedEOF                      = 281
      , UnexpectedError                    = 282
      , BadSchemaLocation                  = 283
      , NoGrammarResolver                  = 284
      , SchemaScanFatalError               = 285
      , F_HighBounds                       = 286
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

