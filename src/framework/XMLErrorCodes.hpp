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
      , DisplayErrorMessage                = 108
      , E_HighBounds                       = 109
      , F_LowBounds                        = 110
      , ExpectedCommentOrCDATA             = 111
      , ExpectedAttrName                   = 112
      , ExpectedNotationName               = 113
      , NoRepInMixed                       = 114
      , BadDefAttrDecl                     = 115
      , ExpectedDefAttrDecl                = 116
      , AttListSyntaxError                 = 117
      , ExpectedEqSign                     = 118
      , DupAttrName                        = 119
      , BadIdForXMLLangAttr                = 120
      , ExpectedElementName                = 121
      , MustStartWithXMLDecl               = 122
      , CommentsMustStartWith              = 123
      , InvalidDocumentStructure           = 124
      , ExpectedDeclString                 = 125
      , BadXMLVersion                      = 126
      , UnsupportedXMLVersion              = 127
      , UnterminatedXMLDecl                = 128
      , BadXMLEncoding                     = 129
      , BadStandalone                      = 130
      , UnterminatedComment                = 131
      , PINameExpected                     = 132
      , UnterminatedPI                     = 133
      , InvalidCharacter                   = 134
      , UnexpectedTextBeforeRoot           = 135
      , UnterminatedStartTag               = 136
      , ExpectedAttrValue                  = 137
      , UnterminatedEndTag                 = 138
      , ExpectedAttributeType              = 139
      , ExpectedEndOfTagX                  = 140
      , ExpectedMarkup                     = 141
      , NotValidAfterContent               = 142
      , ExpectedComment                    = 143
      , ExpectedCommentOrPI                = 144
      , ExpectedWhitespace                 = 145
      , NoRootElemInDOCTYPE                = 146
      , ExpectedQuotedString               = 147
      , ExpectedPublicId                   = 148
      , InvalidPublicIdChar                = 149
      , UnterminatedDOCTYPE                = 150
      , InvalidCharacterInIntSubset        = 151
      , ExpectedCDATA                      = 152
      , InvalidInitialNameChar             = 153
      , InvalidNameChar                    = 154
      , UnexpectedWhitespace               = 155
      , InvalidCharacterInAttrValue        = 156
      , ExpectedMarkupDecl                 = 157
      , TextDeclNotLegalHere               = 158
      , ConditionalSectInIntSubset         = 159
      , ExpectedPEName                     = 160
      , UnterminatedEntityDecl             = 161
      , InvalidCharacterRef                = 162
      , UnterminatedCharRef                = 163
      , ExpectedEntityRefName              = 164
      , EntityNotFound                     = 165
      , NoUnparsedEntityRefs               = 166
      , UnterminatedEntityRef              = 167
      , RecursiveEntity                    = 168
      , PartialMarkupInEntity              = 169
      , UnterminatedElementDecl            = 170
      , ExpectedContentSpecExpr            = 171
      , ExpectedAsterisk                   = 172
      , UnterminatedContentModel           = 173
      , ExpectedSystemId                   = 174
      , ExpectedSystemOrPublicId           = 175
      , UnterminatedNotationDecl           = 176
      , ExpectedSeqChoiceLeaf              = 177
      , ExpectedChoiceOrCloseParen         = 178
      , ExpectedSeqOrCloseParen            = 179
      , ExpectedEnumValue                  = 180
      , ExpectedEnumSepOrParen             = 181
      , UnterminatedEntityLiteral          = 182
      , MoreEndThanStartTags               = 183
      , ExpectedOpenParen                  = 184
      , AttrAlreadyUsedInSTag              = 185
      , BracketInAttrValue                 = 186
      , Expected2ndSurrogateChar           = 187
      , ExpectedEndOfConditional           = 188
      , ExpectedIncOrIgn                   = 189
      , ExpectedINCLUDEBracket             = 190
      , ExpectedTextDecl                   = 191
      , ExpectedXMLDecl                    = 192
      , UnexpectedEOE                      = 193
      , PEPropogated                       = 194
      , ExtraCloseSquare                   = 195
      , PERefInMarkupInIntSubset           = 196
      , EntityPropogated                   = 197
      , ExpectedNumericalCharRef           = 198
      , ExpectedOpenSquareBracket          = 199
      , BadSequenceInCharData              = 200
      , IllegalSequenceInComment           = 201
      , UnterminatedCDATASection           = 202
      , ExpectedNDATA                      = 203
      , NDATANotValidForPE                 = 204
      , HexRadixMustBeLowerCase            = 205
      , DeclStringRep                      = 206
      , DeclStringsInWrongOrder            = 207
      , NoExtRefsInAttValue                = 208
      , XMLDeclMustBeLowerCase             = 209
      , ExpectedEntityValue                = 210
      , BadDigitForRadix                   = 211
      , EndedWithTagsOnStack               = 212
      , AmbiguousContentModel              = 213
      , NestedCDATA                        = 214
      , UnknownPrefix                      = 215
      , PartialTagMarkupError              = 216
      , EmptyMainEntity                    = 217
      , CDATAOutsideOfContent              = 218
      , OnlyCharRefsAllowedHere            = 219
      , Unexpected2ndSurrogateChar         = 220
      , NoPIStartsWithXML                  = 221
      , XMLDeclMustBeFirst                 = 222
      , XMLVersionRequired                 = 223
      , StandaloneNotLegal                 = 224
      , EncodingRequired                   = 225
      , TooManyColonsInName                = 226
      , InvalidColonPos                    = 227
      , ColonNotLegalWithNS                = 228
      , SysException                       = 229
      , XMLException                       = 230
      , UnexpectedEOF                      = 231
      , UnexpectedError                    = 232
      , BadSchemaLocation                  = 233
      , NoGrammarResolver                  = 234
      , F_HighBounds                       = 235
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

