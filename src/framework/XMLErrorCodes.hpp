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
      , DisplayErrorMessage                = 142
      , E_HighBounds                       = 143
      , F_LowBounds                        = 144
      , ExpectedCommentOrCDATA             = 145
      , ExpectedAttrName                   = 146
      , ExpectedNotationName               = 147
      , NoRepInMixed                       = 148
      , BadDefAttrDecl                     = 149
      , ExpectedDefAttrDecl                = 150
      , AttListSyntaxError                 = 151
      , ExpectedEqSign                     = 152
      , DupAttrName                        = 153
      , BadIdForXMLLangAttr                = 154
      , ExpectedElementName                = 155
      , MustStartWithXMLDecl               = 156
      , CommentsMustStartWith              = 157
      , InvalidDocumentStructure           = 158
      , ExpectedDeclString                 = 159
      , BadXMLVersion                      = 160
      , UnsupportedXMLVersion              = 161
      , UnterminatedXMLDecl                = 162
      , BadXMLEncoding                     = 163
      , BadStandalone                      = 164
      , UnterminatedComment                = 165
      , PINameExpected                     = 166
      , UnterminatedPI                     = 167
      , InvalidCharacter                   = 168
      , UnexpectedTextBeforeRoot           = 169
      , UnterminatedStartTag               = 170
      , ExpectedAttrValue                  = 171
      , UnterminatedEndTag                 = 172
      , ExpectedAttributeType              = 173
      , ExpectedEndOfTagX                  = 174
      , ExpectedMarkup                     = 175
      , NotValidAfterContent               = 176
      , ExpectedComment                    = 177
      , ExpectedCommentOrPI                = 178
      , ExpectedWhitespace                 = 179
      , NoRootElemInDOCTYPE                = 180
      , ExpectedQuotedString               = 181
      , ExpectedPublicId                   = 182
      , InvalidPublicIdChar                = 183
      , UnterminatedDOCTYPE                = 184
      , InvalidCharacterInIntSubset        = 185
      , ExpectedCDATA                      = 186
      , InvalidInitialNameChar             = 187
      , InvalidNameChar                    = 188
      , UnexpectedWhitespace               = 189
      , InvalidCharacterInAttrValue        = 190
      , ExpectedMarkupDecl                 = 191
      , TextDeclNotLegalHere               = 192
      , ConditionalSectInIntSubset         = 193
      , ExpectedPEName                     = 194
      , UnterminatedEntityDecl             = 195
      , InvalidCharacterRef                = 196
      , UnterminatedCharRef                = 197
      , ExpectedEntityRefName              = 198
      , EntityNotFound                     = 199
      , NoUnparsedEntityRefs               = 200
      , UnterminatedEntityRef              = 201
      , RecursiveEntity                    = 202
      , PartialMarkupInEntity              = 203
      , UnterminatedElementDecl            = 204
      , ExpectedContentSpecExpr            = 205
      , ExpectedAsterisk                   = 206
      , UnterminatedContentModel           = 207
      , ExpectedSystemId                   = 208
      , ExpectedSystemOrPublicId           = 209
      , UnterminatedNotationDecl           = 210
      , ExpectedSeqChoiceLeaf              = 211
      , ExpectedChoiceOrCloseParen         = 212
      , ExpectedSeqOrCloseParen            = 213
      , ExpectedEnumValue                  = 214
      , ExpectedEnumSepOrParen             = 215
      , UnterminatedEntityLiteral          = 216
      , MoreEndThanStartTags               = 217
      , ExpectedOpenParen                  = 218
      , AttrAlreadyUsedInSTag              = 219
      , BracketInAttrValue                 = 220
      , Expected2ndSurrogateChar           = 221
      , ExpectedEndOfConditional           = 222
      , ExpectedIncOrIgn                   = 223
      , ExpectedINCLUDEBracket             = 224
      , ExpectedTextDecl                   = 225
      , ExpectedXMLDecl                    = 226
      , UnexpectedEOE                      = 227
      , PEPropogated                       = 228
      , ExtraCloseSquare                   = 229
      , PERefInMarkupInIntSubset           = 230
      , EntityPropogated                   = 231
      , ExpectedNumericalCharRef           = 232
      , ExpectedOpenSquareBracket          = 233
      , BadSequenceInCharData              = 234
      , IllegalSequenceInComment           = 235
      , UnterminatedCDATASection           = 236
      , ExpectedNDATA                      = 237
      , NDATANotValidForPE                 = 238
      , HexRadixMustBeLowerCase            = 239
      , DeclStringRep                      = 240
      , DeclStringsInWrongOrder            = 241
      , NoExtRefsInAttValue                = 242
      , XMLDeclMustBeLowerCase             = 243
      , ExpectedEntityValue                = 244
      , BadDigitForRadix                   = 245
      , EndedWithTagsOnStack               = 246
      , AmbiguousContentModel              = 247
      , NestedCDATA                        = 248
      , UnknownPrefix                      = 249
      , PartialTagMarkupError              = 250
      , EmptyMainEntity                    = 251
      , CDATAOutsideOfContent              = 252
      , OnlyCharRefsAllowedHere            = 253
      , Unexpected2ndSurrogateChar         = 254
      , NoPIStartsWithXML                  = 255
      , XMLDeclMustBeFirst                 = 256
      , XMLVersionRequired                 = 257
      , StandaloneNotLegal                 = 258
      , EncodingRequired                   = 259
      , TooManyColonsInName                = 260
      , InvalidColonPos                    = 261
      , ColonNotLegalWithNS                = 262
      , SysException                       = 263
      , XMLException                       = 264
      , UnexpectedEOF                      = 265
      , UnexpectedError                    = 266
      , BadSchemaLocation                  = 267
      , NoGrammarResolver                  = 268
      , SchemaScanFatalError               = 269
      , F_HighBounds                       = 270
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

