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
      , DisplayErrorMessage                = 140
      , E_HighBounds                       = 141
      , F_LowBounds                        = 142
      , ExpectedCommentOrCDATA             = 143
      , ExpectedAttrName                   = 144
      , ExpectedNotationName               = 145
      , NoRepInMixed                       = 146
      , BadDefAttrDecl                     = 147
      , ExpectedDefAttrDecl                = 148
      , AttListSyntaxError                 = 149
      , ExpectedEqSign                     = 150
      , DupAttrName                        = 151
      , BadIdForXMLLangAttr                = 152
      , ExpectedElementName                = 153
      , MustStartWithXMLDecl               = 154
      , CommentsMustStartWith              = 155
      , InvalidDocumentStructure           = 156
      , ExpectedDeclString                 = 157
      , BadXMLVersion                      = 158
      , UnsupportedXMLVersion              = 159
      , UnterminatedXMLDecl                = 160
      , BadXMLEncoding                     = 161
      , BadStandalone                      = 162
      , UnterminatedComment                = 163
      , PINameExpected                     = 164
      , UnterminatedPI                     = 165
      , InvalidCharacter                   = 166
      , UnexpectedTextBeforeRoot           = 167
      , UnterminatedStartTag               = 168
      , ExpectedAttrValue                  = 169
      , UnterminatedEndTag                 = 170
      , ExpectedAttributeType              = 171
      , ExpectedEndOfTagX                  = 172
      , ExpectedMarkup                     = 173
      , NotValidAfterContent               = 174
      , ExpectedComment                    = 175
      , ExpectedCommentOrPI                = 176
      , ExpectedWhitespace                 = 177
      , NoRootElemInDOCTYPE                = 178
      , ExpectedQuotedString               = 179
      , ExpectedPublicId                   = 180
      , InvalidPublicIdChar                = 181
      , UnterminatedDOCTYPE                = 182
      , InvalidCharacterInIntSubset        = 183
      , ExpectedCDATA                      = 184
      , InvalidInitialNameChar             = 185
      , InvalidNameChar                    = 186
      , UnexpectedWhitespace               = 187
      , InvalidCharacterInAttrValue        = 188
      , ExpectedMarkupDecl                 = 189
      , TextDeclNotLegalHere               = 190
      , ConditionalSectInIntSubset         = 191
      , ExpectedPEName                     = 192
      , UnterminatedEntityDecl             = 193
      , InvalidCharacterRef                = 194
      , UnterminatedCharRef                = 195
      , ExpectedEntityRefName              = 196
      , EntityNotFound                     = 197
      , NoUnparsedEntityRefs               = 198
      , UnterminatedEntityRef              = 199
      , RecursiveEntity                    = 200
      , PartialMarkupInEntity              = 201
      , UnterminatedElementDecl            = 202
      , ExpectedContentSpecExpr            = 203
      , ExpectedAsterisk                   = 204
      , UnterminatedContentModel           = 205
      , ExpectedSystemId                   = 206
      , ExpectedSystemOrPublicId           = 207
      , UnterminatedNotationDecl           = 208
      , ExpectedSeqChoiceLeaf              = 209
      , ExpectedChoiceOrCloseParen         = 210
      , ExpectedSeqOrCloseParen            = 211
      , ExpectedEnumValue                  = 212
      , ExpectedEnumSepOrParen             = 213
      , UnterminatedEntityLiteral          = 214
      , MoreEndThanStartTags               = 215
      , ExpectedOpenParen                  = 216
      , AttrAlreadyUsedInSTag              = 217
      , BracketInAttrValue                 = 218
      , Expected2ndSurrogateChar           = 219
      , ExpectedEndOfConditional           = 220
      , ExpectedIncOrIgn                   = 221
      , ExpectedINCLUDEBracket             = 222
      , ExpectedTextDecl                   = 223
      , ExpectedXMLDecl                    = 224
      , UnexpectedEOE                      = 225
      , PEPropogated                       = 226
      , ExtraCloseSquare                   = 227
      , PERefInMarkupInIntSubset           = 228
      , EntityPropogated                   = 229
      , ExpectedNumericalCharRef           = 230
      , ExpectedOpenSquareBracket          = 231
      , BadSequenceInCharData              = 232
      , IllegalSequenceInComment           = 233
      , UnterminatedCDATASection           = 234
      , ExpectedNDATA                      = 235
      , NDATANotValidForPE                 = 236
      , HexRadixMustBeLowerCase            = 237
      , DeclStringRep                      = 238
      , DeclStringsInWrongOrder            = 239
      , NoExtRefsInAttValue                = 240
      , XMLDeclMustBeLowerCase             = 241
      , ExpectedEntityValue                = 242
      , BadDigitForRadix                   = 243
      , EndedWithTagsOnStack               = 244
      , AmbiguousContentModel              = 245
      , NestedCDATA                        = 246
      , UnknownPrefix                      = 247
      , PartialTagMarkupError              = 248
      , EmptyMainEntity                    = 249
      , CDATAOutsideOfContent              = 250
      , OnlyCharRefsAllowedHere            = 251
      , Unexpected2ndSurrogateChar         = 252
      , NoPIStartsWithXML                  = 253
      , XMLDeclMustBeFirst                 = 254
      , XMLVersionRequired                 = 255
      , StandaloneNotLegal                 = 256
      , EncodingRequired                   = 257
      , TooManyColonsInName                = 258
      , InvalidColonPos                    = 259
      , ColonNotLegalWithNS                = 260
      , SysException                       = 261
      , XMLException                       = 262
      , UnexpectedEOF                      = 263
      , UnexpectedError                    = 264
      , BadSchemaLocation                  = 265
      , NoGrammarResolver                  = 266
      , SchemaScanFatalError               = 267
      , F_HighBounds                       = 268
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

