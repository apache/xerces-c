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
      , NotAllContent                      = 159
      , E_HighBounds                       = 160
      , F_LowBounds                        = 161
      , ExpectedCommentOrCDATA             = 162
      , ExpectedAttrName                   = 163
      , ExpectedNotationName               = 164
      , NoRepInMixed                       = 165
      , BadDefAttrDecl                     = 166
      , ExpectedDefAttrDecl                = 167
      , AttListSyntaxError                 = 168
      , ExpectedEqSign                     = 169
      , DupAttrName                        = 170
      , BadIdForXMLLangAttr                = 171
      , ExpectedElementName                = 172
      , MustStartWithXMLDecl               = 173
      , CommentsMustStartWith              = 174
      , InvalidDocumentStructure           = 175
      , ExpectedDeclString                 = 176
      , BadXMLVersion                      = 177
      , UnsupportedXMLVersion              = 178
      , UnterminatedXMLDecl                = 179
      , BadXMLEncoding                     = 180
      , BadStandalone                      = 181
      , UnterminatedComment                = 182
      , PINameExpected                     = 183
      , UnterminatedPI                     = 184
      , InvalidCharacter                   = 185
      , UnexpectedTextBeforeRoot           = 186
      , UnterminatedStartTag               = 187
      , ExpectedAttrValue                  = 188
      , UnterminatedEndTag                 = 189
      , ExpectedAttributeType              = 190
      , ExpectedEndOfTagX                  = 191
      , ExpectedMarkup                     = 192
      , NotValidAfterContent               = 193
      , ExpectedComment                    = 194
      , ExpectedCommentOrPI                = 195
      , ExpectedWhitespace                 = 196
      , NoRootElemInDOCTYPE                = 197
      , ExpectedQuotedString               = 198
      , ExpectedPublicId                   = 199
      , InvalidPublicIdChar                = 200
      , UnterminatedDOCTYPE                = 201
      , InvalidCharacterInIntSubset        = 202
      , ExpectedCDATA                      = 203
      , InvalidInitialNameChar             = 204
      , InvalidNameChar                    = 205
      , UnexpectedWhitespace               = 206
      , InvalidCharacterInAttrValue        = 207
      , ExpectedMarkupDecl                 = 208
      , TextDeclNotLegalHere               = 209
      , ConditionalSectInIntSubset         = 210
      , ExpectedPEName                     = 211
      , UnterminatedEntityDecl             = 212
      , InvalidCharacterRef                = 213
      , UnterminatedCharRef                = 214
      , ExpectedEntityRefName              = 215
      , EntityNotFound                     = 216
      , NoUnparsedEntityRefs               = 217
      , UnterminatedEntityRef              = 218
      , RecursiveEntity                    = 219
      , PartialMarkupInEntity              = 220
      , UnterminatedElementDecl            = 221
      , ExpectedContentSpecExpr            = 222
      , ExpectedAsterisk                   = 223
      , UnterminatedContentModel           = 224
      , ExpectedSystemId                   = 225
      , ExpectedSystemOrPublicId           = 226
      , UnterminatedNotationDecl           = 227
      , ExpectedSeqChoiceLeaf              = 228
      , ExpectedChoiceOrCloseParen         = 229
      , ExpectedSeqOrCloseParen            = 230
      , ExpectedEnumValue                  = 231
      , ExpectedEnumSepOrParen             = 232
      , UnterminatedEntityLiteral          = 233
      , MoreEndThanStartTags               = 234
      , ExpectedOpenParen                  = 235
      , AttrAlreadyUsedInSTag              = 236
      , BracketInAttrValue                 = 237
      , Expected2ndSurrogateChar           = 238
      , ExpectedEndOfConditional           = 239
      , ExpectedIncOrIgn                   = 240
      , ExpectedINCLUDEBracket             = 241
      , ExpectedTextDecl                   = 242
      , ExpectedXMLDecl                    = 243
      , UnexpectedEOE                      = 244
      , PEPropogated                       = 245
      , ExtraCloseSquare                   = 246
      , PERefInMarkupInIntSubset           = 247
      , EntityPropogated                   = 248
      , ExpectedNumericalCharRef           = 249
      , ExpectedOpenSquareBracket          = 250
      , BadSequenceInCharData              = 251
      , IllegalSequenceInComment           = 252
      , UnterminatedCDATASection           = 253
      , ExpectedNDATA                      = 254
      , NDATANotValidForPE                 = 255
      , HexRadixMustBeLowerCase            = 256
      , DeclStringRep                      = 257
      , DeclStringsInWrongOrder            = 258
      , NoExtRefsInAttValue                = 259
      , XMLDeclMustBeLowerCase             = 260
      , ExpectedEntityValue                = 261
      , BadDigitForRadix                   = 262
      , EndedWithTagsOnStack               = 263
      , AmbiguousContentModel              = 264
      , NestedCDATA                        = 265
      , UnknownPrefix                      = 266
      , PartialTagMarkupError              = 267
      , EmptyMainEntity                    = 268
      , CDATAOutsideOfContent              = 269
      , OnlyCharRefsAllowedHere            = 270
      , Unexpected2ndSurrogateChar         = 271
      , NoPIStartsWithXML                  = 272
      , XMLDeclMustBeFirst                 = 273
      , XMLVersionRequired                 = 274
      , StandaloneNotLegal                 = 275
      , EncodingRequired                   = 276
      , TooManyColonsInName                = 277
      , InvalidColonPos                    = 278
      , ColonNotLegalWithNS                = 279
      , SysException                       = 280
      , XMLException_Fatal                 = 281
      , UnexpectedEOF                      = 282
      , UnexpectedError                    = 283
      , BadSchemaLocation                  = 284
      , NoGrammarResolver                  = 285
      , SchemaScanFatalError               = 286
      , F_HighBounds                       = 287
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

