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
      , AnonComplexTypeWithName            = 16
      , AnonSimpleTypeWithName             = 17
      , InvalidElementContent              = 18
      , InvalidAttributeName               = 19
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
      , InvalidComplexTypeName             = 44
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
      , ElementWithNameRef                 = 57
      , GroupWithNameRef                   = 58
      , BadAttWithRef                      = 59
      , InvalidElementName                 = 60
      , InvalidGroupName                   = 61
      , GlobalElementWithRef               = 62
      , ElementWithTypeAndAnonType         = 63
      , NotSimpleOrMixedElement            = 64
      , DisallowedSimpleTypeExtension      = 65
      , InvalidSimpleContentBase           = 66
      , InvalidComplexTypeBase             = 67
      , InvalidChildInSimpleContent        = 68
      , InvalidChildInComplexContent       = 69
      , AnnotationError                    = 70
      , DisallowedBaseDerivation           = 71
      , SubstitutionRepeated               = 72
      , UnionRepeated                      = 73
      , ExtensionRepeated                  = 74
      , ListRepeated                       = 75
      , RestrictionRepeated                = 76
      , InvalidBlockValue                  = 77
      , InvalidFinalValue                  = 78
      , InvalidSubstitutionGroupElement    = 79
      , SubstitutionGroupTypeMismatch      = 80
      , DuplicateElementDeclaration        = 81
      , InvalidElementBlockValue           = 82
      , InvalidElementFinalValue           = 83
      , InvalidAttValue                    = 84
      , AttributeRefContentError           = 85
      , DuplicateRefAttribute              = 86
      , ForbiddenDerivationByRestriction   = 87
      , ForbiddenDerivationByExtension     = 88
      , BaseNotComplexType                 = 89
      , ImportNoSchemaLocation             = 90
      , ImportNamespaceDifference          = 91
      , ImportRootError                    = 92
      , IncludeNoSchemaLocation            = 93
      , IncludeNamespaceDifference         = 94
      , OnlyAnnotationExpected             = 95
      , InvalidAttributeContent            = 96
      , AttributeRequired                  = 97
      , AttributeDisallowed                = 98
      , InvalidMin2MaxOccurs               = 99
      , AnyAttributeContentError           = 100
      , NoNameGlobalElement                = 101
      , NoCircularDefinition               = 102
      , DuplicateGlobalType                = 103
      , DuplicateGlobalGroup               = 104
      , WS_CollapseExpected                = 105
      , Import_1_1                         = 106
      , Import_1_2                         = 107
      , ElemIDValueConstraint              = 108
      , NoNotationType                     = 109
      , EmptiableMixedContent              = 110
      , EmptyComplexRestrictionDerivation   = 111
      , MixedOrElementOnly                 = 112
      , InvalidContentRestriction          = 113
      , ForbiddenDerivation                = 114
      , AtomicItemType                     = 115
      , MemberTypeNoUnion                  = 116
      , GroupContentError                  = 117
      , MinMaxOnGroupChild                 = 118
      , GroupNotFound                      = 119
      , AllContentLimited                  = 120
      , BadMinMaxAllCT                     = 121
      , BadMinMaxAllElem                   = 122
      , DisplayErrorMessage                = 123
      , E_HighBounds                       = 124
      , F_LowBounds                        = 125
      , ExpectedCommentOrCDATA             = 126
      , ExpectedAttrName                   = 127
      , ExpectedNotationName               = 128
      , NoRepInMixed                       = 129
      , BadDefAttrDecl                     = 130
      , ExpectedDefAttrDecl                = 131
      , AttListSyntaxError                 = 132
      , ExpectedEqSign                     = 133
      , DupAttrName                        = 134
      , BadIdForXMLLangAttr                = 135
      , ExpectedElementName                = 136
      , MustStartWithXMLDecl               = 137
      , CommentsMustStartWith              = 138
      , InvalidDocumentStructure           = 139
      , ExpectedDeclString                 = 140
      , BadXMLVersion                      = 141
      , UnsupportedXMLVersion              = 142
      , UnterminatedXMLDecl                = 143
      , BadXMLEncoding                     = 144
      , BadStandalone                      = 145
      , UnterminatedComment                = 146
      , PINameExpected                     = 147
      , UnterminatedPI                     = 148
      , InvalidCharacter                   = 149
      , UnexpectedTextBeforeRoot           = 150
      , UnterminatedStartTag               = 151
      , ExpectedAttrValue                  = 152
      , UnterminatedEndTag                 = 153
      , ExpectedAttributeType              = 154
      , ExpectedEndOfTagX                  = 155
      , ExpectedMarkup                     = 156
      , NotValidAfterContent               = 157
      , ExpectedComment                    = 158
      , ExpectedCommentOrPI                = 159
      , ExpectedWhitespace                 = 160
      , NoRootElemInDOCTYPE                = 161
      , ExpectedQuotedString               = 162
      , ExpectedPublicId                   = 163
      , InvalidPublicIdChar                = 164
      , UnterminatedDOCTYPE                = 165
      , InvalidCharacterInIntSubset        = 166
      , ExpectedCDATA                      = 167
      , InvalidInitialNameChar             = 168
      , InvalidNameChar                    = 169
      , UnexpectedWhitespace               = 170
      , InvalidCharacterInAttrValue        = 171
      , ExpectedMarkupDecl                 = 172
      , TextDeclNotLegalHere               = 173
      , ConditionalSectInIntSubset         = 174
      , ExpectedPEName                     = 175
      , UnterminatedEntityDecl             = 176
      , InvalidCharacterRef                = 177
      , UnterminatedCharRef                = 178
      , ExpectedEntityRefName              = 179
      , EntityNotFound                     = 180
      , NoUnparsedEntityRefs               = 181
      , UnterminatedEntityRef              = 182
      , RecursiveEntity                    = 183
      , PartialMarkupInEntity              = 184
      , UnterminatedElementDecl            = 185
      , ExpectedContentSpecExpr            = 186
      , ExpectedAsterisk                   = 187
      , UnterminatedContentModel           = 188
      , ExpectedSystemId                   = 189
      , ExpectedSystemOrPublicId           = 190
      , UnterminatedNotationDecl           = 191
      , ExpectedSeqChoiceLeaf              = 192
      , ExpectedChoiceOrCloseParen         = 193
      , ExpectedSeqOrCloseParen            = 194
      , ExpectedEnumValue                  = 195
      , ExpectedEnumSepOrParen             = 196
      , UnterminatedEntityLiteral          = 197
      , MoreEndThanStartTags               = 198
      , ExpectedOpenParen                  = 199
      , AttrAlreadyUsedInSTag              = 200
      , BracketInAttrValue                 = 201
      , Expected2ndSurrogateChar           = 202
      , ExpectedEndOfConditional           = 203
      , ExpectedIncOrIgn                   = 204
      , ExpectedINCLUDEBracket             = 205
      , ExpectedTextDecl                   = 206
      , ExpectedXMLDecl                    = 207
      , UnexpectedEOE                      = 208
      , PEPropogated                       = 209
      , ExtraCloseSquare                   = 210
      , PERefInMarkupInIntSubset           = 211
      , EntityPropogated                   = 212
      , ExpectedNumericalCharRef           = 213
      , ExpectedOpenSquareBracket          = 214
      , BadSequenceInCharData              = 215
      , IllegalSequenceInComment           = 216
      , UnterminatedCDATASection           = 217
      , ExpectedNDATA                      = 218
      , NDATANotValidForPE                 = 219
      , HexRadixMustBeLowerCase            = 220
      , DeclStringRep                      = 221
      , DeclStringsInWrongOrder            = 222
      , NoExtRefsInAttValue                = 223
      , XMLDeclMustBeLowerCase             = 224
      , ExpectedEntityValue                = 225
      , BadDigitForRadix                   = 226
      , EndedWithTagsOnStack               = 227
      , AmbiguousContentModel              = 228
      , NestedCDATA                        = 229
      , UnknownPrefix                      = 230
      , PartialTagMarkupError              = 231
      , EmptyMainEntity                    = 232
      , CDATAOutsideOfContent              = 233
      , OnlyCharRefsAllowedHere            = 234
      , Unexpected2ndSurrogateChar         = 235
      , NoPIStartsWithXML                  = 236
      , XMLDeclMustBeFirst                 = 237
      , XMLVersionRequired                 = 238
      , StandaloneNotLegal                 = 239
      , EncodingRequired                   = 240
      , TooManyColonsInName                = 241
      , InvalidColonPos                    = 242
      , ColonNotLegalWithNS                = 243
      , SysException                       = 244
      , XMLException                       = 245
      , UnexpectedEOF                      = 246
      , UnexpectedError                    = 247
      , BadSchemaLocation                  = 248
      , NoGrammarResolver                  = 249
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

