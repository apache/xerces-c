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
      , DisplayErrorMessage                = 102
      , E_HighBounds                       = 103
      , F_LowBounds                        = 104
      , ExpectedCommentOrCDATA             = 105
      , ExpectedAttrName                   = 106
      , ExpectedNotationName               = 107
      , NoRepInMixed                       = 108
      , BadDefAttrDecl                     = 109
      , ExpectedDefAttrDecl                = 110
      , AttListSyntaxError                 = 111
      , ExpectedEqSign                     = 112
      , DupAttrName                        = 113
      , BadIdForXMLLangAttr                = 114
      , ExpectedElementName                = 115
      , MustStartWithXMLDecl               = 116
      , CommentsMustStartWith              = 117
      , InvalidDocumentStructure           = 118
      , ExpectedDeclString                 = 119
      , BadXMLVersion                      = 120
      , UnsupportedXMLVersion              = 121
      , UnterminatedXMLDecl                = 122
      , BadXMLEncoding                     = 123
      , BadStandalone                      = 124
      , UnterminatedComment                = 125
      , PINameExpected                     = 126
      , UnterminatedPI                     = 127
      , InvalidCharacter                   = 128
      , UnexpectedTextBeforeRoot           = 129
      , UnterminatedStartTag               = 130
      , ExpectedAttrValue                  = 131
      , UnterminatedEndTag                 = 132
      , ExpectedAttributeType              = 133
      , ExpectedEndOfTagX                  = 134
      , ExpectedMarkup                     = 135
      , NotValidAfterContent               = 136
      , ExpectedComment                    = 137
      , ExpectedCommentOrPI                = 138
      , ExpectedWhitespace                 = 139
      , NoRootElemInDOCTYPE                = 140
      , ExpectedQuotedString               = 141
      , ExpectedPublicId                   = 142
      , InvalidPublicIdChar                = 143
      , UnterminatedDOCTYPE                = 144
      , InvalidCharacterInIntSubset        = 145
      , ExpectedCDATA                      = 146
      , InvalidInitialNameChar             = 147
      , InvalidNameChar                    = 148
      , UnexpectedWhitespace               = 149
      , InvalidCharacterInAttrValue        = 150
      , ExpectedMarkupDecl                 = 151
      , TextDeclNotLegalHere               = 152
      , ConditionalSectInIntSubset         = 153
      , ExpectedPEName                     = 154
      , UnterminatedEntityDecl             = 155
      , InvalidCharacterRef                = 156
      , UnterminatedCharRef                = 157
      , ExpectedEntityRefName              = 158
      , EntityNotFound                     = 159
      , NoUnparsedEntityRefs               = 160
      , UnterminatedEntityRef              = 161
      , RecursiveEntity                    = 162
      , PartialMarkupInEntity              = 163
      , UnterminatedElementDecl            = 164
      , ExpectedContentSpecExpr            = 165
      , ExpectedAsterisk                   = 166
      , UnterminatedContentModel           = 167
      , ExpectedSystemId                   = 168
      , ExpectedSystemOrPublicId           = 169
      , UnterminatedNotationDecl           = 170
      , ExpectedSeqChoiceLeaf              = 171
      , ExpectedChoiceOrCloseParen         = 172
      , ExpectedSeqOrCloseParen            = 173
      , ExpectedEnumValue                  = 174
      , ExpectedEnumSepOrParen             = 175
      , UnterminatedEntityLiteral          = 176
      , MoreEndThanStartTags               = 177
      , IllegalRefInStandalone             = 178
      , ExpectedOpenParen                  = 179
      , AttrAlreadyUsedInSTag              = 180
      , BracketInAttrValue                 = 181
      , Expected2ndSurrogateChar           = 182
      , ExpectedEndOfConditional           = 183
      , ExpectedIncOrIgn                   = 184
      , ExpectedINCLUDEBracket             = 185
      , ExpectedTextDecl                   = 186
      , ExpectedXMLDecl                    = 187
      , UnexpectedEOE                      = 188
      , PEPropogated                       = 189
      , ExtraCloseSquare                   = 190
      , PERefInMarkupInIntSubset           = 191
      , EntityPropogated                   = 192
      , ExpectedNumericalCharRef           = 193
      , ExpectedOpenSquareBracket          = 194
      , BadSequenceInCharData              = 195
      , IllegalSequenceInComment           = 196
      , UnterminatedCDATASection           = 197
      , ExpectedNDATA                      = 198
      , NDATANotValidForPE                 = 199
      , HexRadixMustBeLowerCase            = 200
      , DeclStringRep                      = 201
      , DeclStringsInWrongOrder            = 202
      , NoExtRefsInAttValue                = 203
      , XMLDeclMustBeLowerCase             = 204
      , ExpectedEntityValue                = 205
      , BadDigitForRadix                   = 206
      , EndedWithTagsOnStack               = 207
      , AmbiguousContentModel              = 208
      , NestedCDATA                        = 209
      , UnknownPrefix                      = 210
      , PartialTagMarkupError              = 211
      , EmptyMainEntity                    = 212
      , CDATAOutsideOfContent              = 213
      , OnlyCharRefsAllowedHere            = 214
      , Unexpected2ndSurrogateChar         = 215
      , NoPIStartsWithXML                  = 216
      , XMLDeclMustBeFirst                 = 217
      , XMLVersionRequired                 = 218
      , StandaloneNotLegal                 = 219
      , TooManyColonsInName                = 220
      , InvalidColonPos                    = 221
      , ColonNotLegalWithNS                = 222
      , SysException                       = 223
      , XMLException                       = 224
      , UnexpectedEOF                      = 225
      , UnexpectedError                    = 226
      , BadSchemaLocation                  = 227
      , NoGrammarResolver                  = 228
      , F_HighBounds                       = 229
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

