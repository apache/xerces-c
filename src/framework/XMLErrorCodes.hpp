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
      , AnonComplexTypeWithName            = 15
      , AnonSimpleTypeWithName             = 16
      , InvalidElementContent              = 17
      , InvalidAttributeName               = 18
      , UntypedElement                     = 19
      , SimpleTypeContentError             = 20
      , ExpectedSimpleTypeInList           = 21
      , ListUnionRestrictionError          = 22
      , SimpleTypeDerivationByListError    = 23
      , ExpectedSimpleTypeInRestriction    = 24
      , DuplicateFacet                     = 25
      , ExpectedSimpleTypeInUnion          = 26
      , EmptySimpleTypeContent             = 27
      , InvalidSimpleContent               = 28
      , UnspecifiedBase                    = 29
      , InvalidComplexContent              = 30
      , SchemaElementContentError          = 31
      , ContentError                       = 32
      , UnknownSimpleType                  = 33
      , UnknownComplexType                 = 34
      , UnresolvedPrefix                   = 35
      , RefElementNotFound                 = 36
      , TypeNotFound                       = 37
      , TopLevelAttributeNotFound          = 38
      , InvalidChildInComplexType          = 39
      , BaseTypeNotFound                   = 40
      , NoAttributeInSchema                = 41
      , DatatypeValidatorCreationError     = 42
      , InvalidComplexTypeName             = 43
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
      , ElementWithNameRef                 = 56
      , BadAttWithRef                      = 57
      , InvalidElementName                 = 58
      , GlobalElementWithRef               = 59
      , ElementWithTypeAndAnonType         = 60
      , DatatypeValidationFailure          = 61
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
      , ImportNoSchemaLocation             = 88
      , ImportNamespaceDifference          = 89
      , ImportRootError                    = 90
      , IncludeNoSchemaLocation            = 91
      , IncludeNamespaceDifference         = 92
      , OnlyAnnotationExpected             = 93
      , InvalidAttributeContent            = 94
      , AttributeRequired                  = 95
      , AttributeDisallowed                = 96
      , InvalidMin2MaxOccurs               = 97
      , AnyAttributeContentError           = 98
      , NoNameGlobalElement                = 99
      , NoCircularDefinition               = 100
      , DuplicateGlobalType                = 101
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
      , DisplayErrorMessage                = 114
      , E_HighBounds                       = 115
      , F_LowBounds                        = 116
      , ExpectedCommentOrCDATA             = 117
      , ExpectedAttrName                   = 118
      , ExpectedNotationName               = 119
      , NoRepInMixed                       = 120
      , BadDefAttrDecl                     = 121
      , ExpectedDefAttrDecl                = 122
      , AttListSyntaxError                 = 123
      , ExpectedEqSign                     = 124
      , DupAttrName                        = 125
      , BadIdForXMLLangAttr                = 126
      , ExpectedElementName                = 127
      , MustStartWithXMLDecl               = 128
      , CommentsMustStartWith              = 129
      , InvalidDocumentStructure           = 130
      , ExpectedDeclString                 = 131
      , BadXMLVersion                      = 132
      , UnsupportedXMLVersion              = 133
      , UnterminatedXMLDecl                = 134
      , BadXMLEncoding                     = 135
      , BadStandalone                      = 136
      , UnterminatedComment                = 137
      , PINameExpected                     = 138
      , UnterminatedPI                     = 139
      , InvalidCharacter                   = 140
      , UnexpectedTextBeforeRoot           = 141
      , UnterminatedStartTag               = 142
      , ExpectedAttrValue                  = 143
      , UnterminatedEndTag                 = 144
      , ExpectedAttributeType              = 145
      , ExpectedEndOfTagX                  = 146
      , ExpectedMarkup                     = 147
      , NotValidAfterContent               = 148
      , ExpectedComment                    = 149
      , ExpectedCommentOrPI                = 150
      , ExpectedWhitespace                 = 151
      , NoRootElemInDOCTYPE                = 152
      , ExpectedQuotedString               = 153
      , ExpectedPublicId                   = 154
      , InvalidPublicIdChar                = 155
      , UnterminatedDOCTYPE                = 156
      , InvalidCharacterInIntSubset        = 157
      , ExpectedCDATA                      = 158
      , InvalidInitialNameChar             = 159
      , InvalidNameChar                    = 160
      , UnexpectedWhitespace               = 161
      , InvalidCharacterInAttrValue        = 162
      , ExpectedMarkupDecl                 = 163
      , TextDeclNotLegalHere               = 164
      , ConditionalSectInIntSubset         = 165
      , ExpectedPEName                     = 166
      , UnterminatedEntityDecl             = 167
      , InvalidCharacterRef                = 168
      , UnterminatedCharRef                = 169
      , ExpectedEntityRefName              = 170
      , EntityNotFound                     = 171
      , NoUnparsedEntityRefs               = 172
      , UnterminatedEntityRef              = 173
      , RecursiveEntity                    = 174
      , PartialMarkupInEntity              = 175
      , UnterminatedElementDecl            = 176
      , ExpectedContentSpecExpr            = 177
      , ExpectedAsterisk                   = 178
      , UnterminatedContentModel           = 179
      , ExpectedSystemId                   = 180
      , ExpectedSystemOrPublicId           = 181
      , UnterminatedNotationDecl           = 182
      , ExpectedSeqChoiceLeaf              = 183
      , ExpectedChoiceOrCloseParen         = 184
      , ExpectedSeqOrCloseParen            = 185
      , ExpectedEnumValue                  = 186
      , ExpectedEnumSepOrParen             = 187
      , UnterminatedEntityLiteral          = 188
      , MoreEndThanStartTags               = 189
      , ExpectedOpenParen                  = 190
      , AttrAlreadyUsedInSTag              = 191
      , BracketInAttrValue                 = 192
      , Expected2ndSurrogateChar           = 193
      , ExpectedEndOfConditional           = 194
      , ExpectedIncOrIgn                   = 195
      , ExpectedINCLUDEBracket             = 196
      , ExpectedTextDecl                   = 197
      , ExpectedXMLDecl                    = 198
      , UnexpectedEOE                      = 199
      , PEPropogated                       = 200
      , ExtraCloseSquare                   = 201
      , PERefInMarkupInIntSubset           = 202
      , EntityPropogated                   = 203
      , ExpectedNumericalCharRef           = 204
      , ExpectedOpenSquareBracket          = 205
      , BadSequenceInCharData              = 206
      , IllegalSequenceInComment           = 207
      , UnterminatedCDATASection           = 208
      , ExpectedNDATA                      = 209
      , NDATANotValidForPE                 = 210
      , HexRadixMustBeLowerCase            = 211
      , DeclStringRep                      = 212
      , DeclStringsInWrongOrder            = 213
      , NoExtRefsInAttValue                = 214
      , XMLDeclMustBeLowerCase             = 215
      , ExpectedEntityValue                = 216
      , BadDigitForRadix                   = 217
      , EndedWithTagsOnStack               = 218
      , AmbiguousContentModel              = 219
      , NestedCDATA                        = 220
      , UnknownPrefix                      = 221
      , PartialTagMarkupError              = 222
      , EmptyMainEntity                    = 223
      , CDATAOutsideOfContent              = 224
      , OnlyCharRefsAllowedHere            = 225
      , Unexpected2ndSurrogateChar         = 226
      , NoPIStartsWithXML                  = 227
      , XMLDeclMustBeFirst                 = 228
      , XMLVersionRequired                 = 229
      , StandaloneNotLegal                 = 230
      , EncodingRequired                   = 231
      , TooManyColonsInName                = 232
      , InvalidColonPos                    = 233
      , ColonNotLegalWithNS                = 234
      , SysException                       = 235
      , XMLException                       = 236
      , UnexpectedEOF                      = 237
      , UnexpectedError                    = 238
      , BadSchemaLocation                  = 239
      , NoGrammarResolver                  = 240
      , F_HighBounds                       = 241
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

