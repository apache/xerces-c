// This file is generated, don't edit it!!

#if !defined(ERRHEADER_XMLErrs)
#define ERRHEADER_XMLErrs

#include <framework/XMLErrorReporter.hpp>

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
      , W_HighBounds                       = 7
      , E_LowBounds                        = 8
      , FeatureUnsupported                 = 9
      , TopLevelNoNameComplexType          = 10
      , TopLevelNoNameAttribute            = 11
      , NoNameRefAttribute                 = 12
      , GlobalNoNameElement                = 13
      , NoNameRefElement                   = 14
      , NoNameRefGroup                     = 15
      , NoNameRefAttGroup                  = 16
      , AnonComplexTypeWithName            = 17
      , AnonSimpleTypeWithName             = 18
      , InvalidElementContent              = 19
      , UntypedElement                     = 20
      , SimpleTypeContentError             = 21
      , ExpectedSimpleTypeInList           = 22
      , ListUnionRestrictionError          = 23
      , SimpleTypeDerivationByListError    = 24
      , ExpectedSimpleTypeInRestriction    = 25
      , DuplicateFacet                     = 26
      , ExpectedSimpleTypeInUnion          = 27
      , EmptySimpleTypeContent             = 28
      , InvalidSimpleContent               = 29
      , UnspecifiedBase                    = 30
      , InvalidComplexContent              = 31
      , SchemaElementContentError          = 32
      , ContentError                       = 33
      , UnknownSimpleType                  = 34
      , UnknownComplexType                 = 35
      , UnresolvedPrefix                   = 36
      , RefElementNotFound                 = 37
      , TypeNotFound                       = 38
      , TopLevelAttributeNotFound          = 39
      , InvalidChildInComplexType          = 40
      , BaseTypeNotFound                   = 41
      , NoAttributeInSchema                = 42
      , DatatypeValidatorCreationError     = 43
      , InvalidChildFollowingSimpleContent   = 44
      , InvalidChildFollowingConplexContent   = 45
      , InvalidComplexTypeBlockValue       = 46
      , InvalidComplexTypeFinalValue       = 47
      , AttributeDefaultFixedValue         = 48
      , NotOptionalDefaultAttValue         = 49
      , LocalAttributeWithNameRef          = 50
      , GlobalAttributeWithNameRef         = 51
      , DuplicateAttribute                 = 52
      , AttributeWithTypeAndSimpleType     = 53
      , AttributeSimpleTypeNotFound        = 54
      , ElementWithFixedAndDefault         = 55
      , DeclarationWithNameRef             = 56
      , BadAttWithRef                      = 57
      , InvalidDeclarationName             = 58
      , GlobalElementWithRef               = 59
      , ElementWithTypeAndAnonType         = 60
      , NotSimpleOrMixedElement            = 61
      , DisallowedSimpleTypeExtension      = 62
      , InvalidSimpleContentBase           = 63
      , InvalidComplexTypeBase             = 64
      , InvalidChildInSimpleContent        = 65
      , InvalidChildInComplexContent       = 66
      , AnnotationError                    = 67
      , DisallowedBaseDerivation           = 68
      , SubstitutionRepeated               = 69
      , UnionRepeated                      = 70
      , ExtensionRepeated                  = 71
      , ListRepeated                       = 72
      , RestrictionRepeated                = 73
      , InvalidBlockValue                  = 74
      , InvalidFinalValue                  = 75
      , InvalidSubstitutionGroupElement    = 76
      , SubstitutionGroupTypeMismatch      = 77
      , DuplicateElementDeclaration        = 78
      , InvalidElementBlockValue           = 79
      , InvalidElementFinalValue           = 80
      , InvalidAttValue                    = 81
      , AttributeRefContentError           = 82
      , DuplicateRefAttribute              = 83
      , ForbiddenDerivationByRestriction   = 84
      , ForbiddenDerivationByExtension     = 85
      , BaseNotComplexType                 = 86
      , ImportNamespaceDifference          = 87
      , ImportRootError                    = 88
      , DeclarationNoSchemaLocation        = 89
      , IncludeNamespaceDifference         = 90
      , OnlyAnnotationExpected             = 91
      , InvalidAttributeContent            = 92
      , AttributeRequired                  = 93
      , AttributeDisallowed                = 94
      , InvalidMin2MaxOccurs               = 95
      , AnyAttributeContentError           = 96
      , NoNameGlobalElement                = 97
      , NoCircularDefinition               = 98
      , DuplicateGlobalType                = 99
      , DuplicateGlobalDeclaration         = 100
      , WS_CollapseExpected                = 101
      , Import_1_1                         = 102
      , Import_1_2                         = 103
      , ElemIDValueConstraint              = 104
      , NoNotationType                     = 105
      , EmptiableMixedContent              = 106
      , EmptyComplexRestrictionDerivation   = 107
      , MixedOrElementOnly                 = 108
      , InvalidContentRestriction          = 109
      , ForbiddenDerivation                = 110
      , AtomicItemType                     = 111
      , MemberTypeNoUnion                  = 112
      , GroupContentError                  = 113
      , AttGroupContentError               = 114
      , MinMaxOnGroupChild                 = 115
      , DeclarationNotFound                = 116
      , AllContentLimited                  = 117
      , BadMinMaxAllCT                     = 118
      , BadMinMaxAllElem                   = 119
      , NoCircularAttGroup                 = 120
      , DuplicateAttInDerivation           = 121
      , NotExpressibleWildCardIntersection   = 122
      , BadAttDerivation_1                 = 123
      , BadAttDerivation_2                 = 124
      , BadAttDerivation_3                 = 125
      , BadAttDerivation_4                 = 126
      , BadAttDerivation_5                 = 127
      , BadAttDerivation_6                 = 128
      , BadAttDerivation_7                 = 129
      , AllContentError                    = 130
      , RedefineNamespaceDifference        = 131
      , Redefine_InvalidSimpleType         = 132
      , Redefine_InvalidSimpleTypeBase     = 133
      , Redefine_InvalidComplexType        = 134
      , Redefine_InvalidComplexTypeBase    = 135
      , Redefine_InvalidGroupMinMax        = 136
      , Redefine_DeclarationNotFound       = 137
      , Redefine_GroupRefCount             = 138
      , Redefine_AttGroupRefCount          = 139
      , Notation_InvalidDecl               = 140
      , Notation_DeclNotFound              = 141
      , IC_DuplicateDecl                   = 142
      , IC_BadContent                      = 143
      , IC_KeyRefReferNotFound             = 144
      , IC_KeyRefCardinality               = 145
      , IC_XPathExprMissing                = 146
      , DisplayErrorMessage                = 147
      , E_HighBounds                       = 148
      , F_LowBounds                        = 149
      , ExpectedCommentOrCDATA             = 150
      , ExpectedAttrName                   = 151
      , ExpectedNotationName               = 152
      , NoRepInMixed                       = 153
      , BadDefAttrDecl                     = 154
      , ExpectedDefAttrDecl                = 155
      , AttListSyntaxError                 = 156
      , ExpectedEqSign                     = 157
      , DupAttrName                        = 158
      , BadIdForXMLLangAttr                = 159
      , ExpectedElementName                = 160
      , MustStartWithXMLDecl               = 161
      , CommentsMustStartWith              = 162
      , InvalidDocumentStructure           = 163
      , ExpectedDeclString                 = 164
      , BadXMLVersion                      = 165
      , UnsupportedXMLVersion              = 166
      , UnterminatedXMLDecl                = 167
      , BadXMLEncoding                     = 168
      , BadStandalone                      = 169
      , UnterminatedComment                = 170
      , PINameExpected                     = 171
      , UnterminatedPI                     = 172
      , InvalidCharacter                   = 173
      , UnexpectedTextBeforeRoot           = 174
      , UnterminatedStartTag               = 175
      , ExpectedAttrValue                  = 176
      , UnterminatedEndTag                 = 177
      , ExpectedAttributeType              = 178
      , ExpectedEndOfTagX                  = 179
      , ExpectedMarkup                     = 180
      , NotValidAfterContent               = 181
      , ExpectedComment                    = 182
      , ExpectedCommentOrPI                = 183
      , ExpectedWhitespace                 = 184
      , NoRootElemInDOCTYPE                = 185
      , ExpectedQuotedString               = 186
      , ExpectedPublicId                   = 187
      , InvalidPublicIdChar                = 188
      , UnterminatedDOCTYPE                = 189
      , InvalidCharacterInIntSubset        = 190
      , ExpectedCDATA                      = 191
      , InvalidInitialNameChar             = 192
      , InvalidNameChar                    = 193
      , UnexpectedWhitespace               = 194
      , InvalidCharacterInAttrValue        = 195
      , ExpectedMarkupDecl                 = 196
      , TextDeclNotLegalHere               = 197
      , ConditionalSectInIntSubset         = 198
      , ExpectedPEName                     = 199
      , UnterminatedEntityDecl             = 200
      , InvalidCharacterRef                = 201
      , UnterminatedCharRef                = 202
      , ExpectedEntityRefName              = 203
      , EntityNotFound                     = 204
      , NoUnparsedEntityRefs               = 205
      , UnterminatedEntityRef              = 206
      , RecursiveEntity                    = 207
      , PartialMarkupInEntity              = 208
      , UnterminatedElementDecl            = 209
      , ExpectedContentSpecExpr            = 210
      , ExpectedAsterisk                   = 211
      , UnterminatedContentModel           = 212
      , ExpectedSystemId                   = 213
      , ExpectedSystemOrPublicId           = 214
      , UnterminatedNotationDecl           = 215
      , ExpectedSeqChoiceLeaf              = 216
      , ExpectedChoiceOrCloseParen         = 217
      , ExpectedSeqOrCloseParen            = 218
      , ExpectedEnumValue                  = 219
      , ExpectedEnumSepOrParen             = 220
      , UnterminatedEntityLiteral          = 221
      , MoreEndThanStartTags               = 222
      , ExpectedOpenParen                  = 223
      , AttrAlreadyUsedInSTag              = 224
      , BracketInAttrValue                 = 225
      , Expected2ndSurrogateChar           = 226
      , ExpectedEndOfConditional           = 227
      , ExpectedIncOrIgn                   = 228
      , ExpectedINCLUDEBracket             = 229
      , ExpectedTextDecl                   = 230
      , ExpectedXMLDecl                    = 231
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
      , AmbiguousContentModel              = 252
      , NestedCDATA                        = 253
      , UnknownPrefix                      = 254
      , PartialTagMarkupError              = 255
      , EmptyMainEntity                    = 256
      , CDATAOutsideOfContent              = 257
      , OnlyCharRefsAllowedHere            = 258
      , Unexpected2ndSurrogateChar         = 259
      , NoPIStartsWithXML                  = 260
      , XMLDeclMustBeFirst                 = 261
      , XMLVersionRequired                 = 262
      , StandaloneNotLegal                 = 263
      , EncodingRequired                   = 264
      , TooManyColonsInName                = 265
      , InvalidColonPos                    = 266
      , ColonNotLegalWithNS                = 267
      , SysException                       = 268
      , XMLException                       = 269
      , UnexpectedEOF                      = 270
      , UnexpectedError                    = 271
      , BadSchemaLocation                  = 272
      , NoGrammarResolver                  = 273
      , SchemaScanFatalError               = 274
      , F_HighBounds                       = 275
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

