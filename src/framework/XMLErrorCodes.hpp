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
      , ImportNoSchemaLocation             = 87
      , ImportNamespaceDifference          = 88
      , ImportRootError                    = 89
      , IncludeNoSchemaLocation            = 90
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
      , DisplayErrorMessage                = 122
      , E_HighBounds                       = 123
      , F_LowBounds                        = 124
      , ExpectedCommentOrCDATA             = 125
      , ExpectedAttrName                   = 126
      , ExpectedNotationName               = 127
      , NoRepInMixed                       = 128
      , BadDefAttrDecl                     = 129
      , ExpectedDefAttrDecl                = 130
      , AttListSyntaxError                 = 131
      , ExpectedEqSign                     = 132
      , DupAttrName                        = 133
      , BadIdForXMLLangAttr                = 134
      , ExpectedElementName                = 135
      , MustStartWithXMLDecl               = 136
      , CommentsMustStartWith              = 137
      , InvalidDocumentStructure           = 138
      , ExpectedDeclString                 = 139
      , BadXMLVersion                      = 140
      , UnsupportedXMLVersion              = 141
      , UnterminatedXMLDecl                = 142
      , BadXMLEncoding                     = 143
      , BadStandalone                      = 144
      , UnterminatedComment                = 145
      , PINameExpected                     = 146
      , UnterminatedPI                     = 147
      , InvalidCharacter                   = 148
      , UnexpectedTextBeforeRoot           = 149
      , UnterminatedStartTag               = 150
      , ExpectedAttrValue                  = 151
      , UnterminatedEndTag                 = 152
      , ExpectedAttributeType              = 153
      , ExpectedEndOfTagX                  = 154
      , ExpectedMarkup                     = 155
      , NotValidAfterContent               = 156
      , ExpectedComment                    = 157
      , ExpectedCommentOrPI                = 158
      , ExpectedWhitespace                 = 159
      , NoRootElemInDOCTYPE                = 160
      , ExpectedQuotedString               = 161
      , ExpectedPublicId                   = 162
      , InvalidPublicIdChar                = 163
      , UnterminatedDOCTYPE                = 164
      , InvalidCharacterInIntSubset        = 165
      , ExpectedCDATA                      = 166
      , InvalidInitialNameChar             = 167
      , InvalidNameChar                    = 168
      , UnexpectedWhitespace               = 169
      , InvalidCharacterInAttrValue        = 170
      , ExpectedMarkupDecl                 = 171
      , TextDeclNotLegalHere               = 172
      , ConditionalSectInIntSubset         = 173
      , ExpectedPEName                     = 174
      , UnterminatedEntityDecl             = 175
      , InvalidCharacterRef                = 176
      , UnterminatedCharRef                = 177
      , ExpectedEntityRefName              = 178
      , EntityNotFound                     = 179
      , NoUnparsedEntityRefs               = 180
      , UnterminatedEntityRef              = 181
      , RecursiveEntity                    = 182
      , PartialMarkupInEntity              = 183
      , UnterminatedElementDecl            = 184
      , ExpectedContentSpecExpr            = 185
      , ExpectedAsterisk                   = 186
      , UnterminatedContentModel           = 187
      , ExpectedSystemId                   = 188
      , ExpectedSystemOrPublicId           = 189
      , UnterminatedNotationDecl           = 190
      , ExpectedSeqChoiceLeaf              = 191
      , ExpectedChoiceOrCloseParen         = 192
      , ExpectedSeqOrCloseParen            = 193
      , ExpectedEnumValue                  = 194
      , ExpectedEnumSepOrParen             = 195
      , UnterminatedEntityLiteral          = 196
      , MoreEndThanStartTags               = 197
      , ExpectedOpenParen                  = 198
      , AttrAlreadyUsedInSTag              = 199
      , BracketInAttrValue                 = 200
      , Expected2ndSurrogateChar           = 201
      , ExpectedEndOfConditional           = 202
      , ExpectedIncOrIgn                   = 203
      , ExpectedINCLUDEBracket             = 204
      , ExpectedTextDecl                   = 205
      , ExpectedXMLDecl                    = 206
      , UnexpectedEOE                      = 207
      , PEPropogated                       = 208
      , ExtraCloseSquare                   = 209
      , PERefInMarkupInIntSubset           = 210
      , EntityPropogated                   = 211
      , ExpectedNumericalCharRef           = 212
      , ExpectedOpenSquareBracket          = 213
      , BadSequenceInCharData              = 214
      , IllegalSequenceInComment           = 215
      , UnterminatedCDATASection           = 216
      , ExpectedNDATA                      = 217
      , NDATANotValidForPE                 = 218
      , HexRadixMustBeLowerCase            = 219
      , DeclStringRep                      = 220
      , DeclStringsInWrongOrder            = 221
      , NoExtRefsInAttValue                = 222
      , XMLDeclMustBeLowerCase             = 223
      , ExpectedEntityValue                = 224
      , BadDigitForRadix                   = 225
      , EndedWithTagsOnStack               = 226
      , AmbiguousContentModel              = 227
      , NestedCDATA                        = 228
      , UnknownPrefix                      = 229
      , PartialTagMarkupError              = 230
      , EmptyMainEntity                    = 231
      , CDATAOutsideOfContent              = 232
      , OnlyCharRefsAllowedHere            = 233
      , Unexpected2ndSurrogateChar         = 234
      , NoPIStartsWithXML                  = 235
      , XMLDeclMustBeFirst                 = 236
      , XMLVersionRequired                 = 237
      , StandaloneNotLegal                 = 238
      , EncodingRequired                   = 239
      , TooManyColonsInName                = 240
      , InvalidColonPos                    = 241
      , ColonNotLegalWithNS                = 242
      , SysException                       = 243
      , XMLException                       = 244
      , UnexpectedEOF                      = 245
      , UnexpectedError                    = 246
      , BadSchemaLocation                  = 247
      , NoGrammarResolver                  = 248
      , SchemaScanFatalError               = 249
      , F_HighBounds                       = 250
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

