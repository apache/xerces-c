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
      , E_HighBounds                       = 157
      , F_LowBounds                        = 158
      , ExpectedCommentOrCDATA             = 159
      , ExpectedAttrName                   = 160
      , ExpectedNotationName               = 161
      , NoRepInMixed                       = 162
      , BadDefAttrDecl                     = 163
      , ExpectedDefAttrDecl                = 164
      , AttListSyntaxError                 = 165
      , ExpectedEqSign                     = 166
      , DupAttrName                        = 167
      , BadIdForXMLLangAttr                = 168
      , ExpectedElementName                = 169
      , MustStartWithXMLDecl               = 170
      , CommentsMustStartWith              = 171
      , InvalidDocumentStructure           = 172
      , ExpectedDeclString                 = 173
      , BadXMLVersion                      = 174
      , UnsupportedXMLVersion              = 175
      , UnterminatedXMLDecl                = 176
      , BadXMLEncoding                     = 177
      , BadStandalone                      = 178
      , UnterminatedComment                = 179
      , PINameExpected                     = 180
      , UnterminatedPI                     = 181
      , InvalidCharacter                   = 182
      , UnexpectedTextBeforeRoot           = 183
      , UnterminatedStartTag               = 184
      , ExpectedAttrValue                  = 185
      , UnterminatedEndTag                 = 186
      , ExpectedAttributeType              = 187
      , ExpectedEndOfTagX                  = 188
      , ExpectedMarkup                     = 189
      , NotValidAfterContent               = 190
      , ExpectedComment                    = 191
      , ExpectedCommentOrPI                = 192
      , ExpectedWhitespace                 = 193
      , NoRootElemInDOCTYPE                = 194
      , ExpectedQuotedString               = 195
      , ExpectedPublicId                   = 196
      , InvalidPublicIdChar                = 197
      , UnterminatedDOCTYPE                = 198
      , InvalidCharacterInIntSubset        = 199
      , ExpectedCDATA                      = 200
      , InvalidInitialNameChar             = 201
      , InvalidNameChar                    = 202
      , UnexpectedWhitespace               = 203
      , InvalidCharacterInAttrValue        = 204
      , ExpectedMarkupDecl                 = 205
      , TextDeclNotLegalHere               = 206
      , ConditionalSectInIntSubset         = 207
      , ExpectedPEName                     = 208
      , UnterminatedEntityDecl             = 209
      , InvalidCharacterRef                = 210
      , UnterminatedCharRef                = 211
      , ExpectedEntityRefName              = 212
      , EntityNotFound                     = 213
      , NoUnparsedEntityRefs               = 214
      , UnterminatedEntityRef              = 215
      , RecursiveEntity                    = 216
      , PartialMarkupInEntity              = 217
      , UnterminatedElementDecl            = 218
      , ExpectedContentSpecExpr            = 219
      , ExpectedAsterisk                   = 220
      , UnterminatedContentModel           = 221
      , ExpectedSystemId                   = 222
      , ExpectedSystemOrPublicId           = 223
      , UnterminatedNotationDecl           = 224
      , ExpectedSeqChoiceLeaf              = 225
      , ExpectedChoiceOrCloseParen         = 226
      , ExpectedSeqOrCloseParen            = 227
      , ExpectedEnumValue                  = 228
      , ExpectedEnumSepOrParen             = 229
      , UnterminatedEntityLiteral          = 230
      , MoreEndThanStartTags               = 231
      , ExpectedOpenParen                  = 232
      , AttrAlreadyUsedInSTag              = 233
      , BracketInAttrValue                 = 234
      , Expected2ndSurrogateChar           = 235
      , ExpectedEndOfConditional           = 236
      , ExpectedIncOrIgn                   = 237
      , ExpectedINCLUDEBracket             = 238
      , ExpectedTextDecl                   = 239
      , ExpectedXMLDecl                    = 240
      , UnexpectedEOE                      = 241
      , PEPropogated                       = 242
      , ExtraCloseSquare                   = 243
      , PERefInMarkupInIntSubset           = 244
      , EntityPropogated                   = 245
      , ExpectedNumericalCharRef           = 246
      , ExpectedOpenSquareBracket          = 247
      , BadSequenceInCharData              = 248
      , IllegalSequenceInComment           = 249
      , UnterminatedCDATASection           = 250
      , ExpectedNDATA                      = 251
      , NDATANotValidForPE                 = 252
      , HexRadixMustBeLowerCase            = 253
      , DeclStringRep                      = 254
      , DeclStringsInWrongOrder            = 255
      , NoExtRefsInAttValue                = 256
      , XMLDeclMustBeLowerCase             = 257
      , ExpectedEntityValue                = 258
      , BadDigitForRadix                   = 259
      , EndedWithTagsOnStack               = 260
      , AmbiguousContentModel              = 261
      , NestedCDATA                        = 262
      , UnknownPrefix                      = 263
      , PartialTagMarkupError              = 264
      , EmptyMainEntity                    = 265
      , CDATAOutsideOfContent              = 266
      , OnlyCharRefsAllowedHere            = 267
      , Unexpected2ndSurrogateChar         = 268
      , NoPIStartsWithXML                  = 269
      , XMLDeclMustBeFirst                 = 270
      , XMLVersionRequired                 = 271
      , StandaloneNotLegal                 = 272
      , EncodingRequired                   = 273
      , TooManyColonsInName                = 274
      , InvalidColonPos                    = 275
      , ColonNotLegalWithNS                = 276
      , SysException                       = 277
      , XMLException_Fatal                 = 278
      , UnexpectedEOF                      = 279
      , UnexpectedError                    = 280
      , BadSchemaLocation                  = 281
      , NoGrammarResolver                  = 282
      , SchemaScanFatalError               = 283
      , F_HighBounds                       = 284
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

