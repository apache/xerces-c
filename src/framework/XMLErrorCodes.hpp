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
      , InvalidElementNillableValue        = 82
      , InvalidElementAbstractValue        = 83
      , AttributeRefContentError           = 84
      , DuplicateRefAttribute              = 85
      , ForbiddenDerivationByRestriction   = 86
      , ForbiddenDerivationByExtension     = 87
      , BaseNotComplexType                 = 88
      , ImportNoSchemaLocation             = 89
      , ImportNamespaceDifference          = 90
      , ImportRootError                    = 91
      , IncludeNoSchemaLocation            = 92
      , IncludeNamespaceDifference         = 93
      , OnlyAnnotationExpected             = 94
      , InvalidAttributeContent            = 95
      , DisplayErrorMessage                = 96
      , E_HighBounds                       = 97
      , F_LowBounds                        = 98
      , ExpectedCommentOrCDATA             = 99
      , ExpectedAttrName                   = 100
      , ExpectedNotationName               = 101
      , NoRepInMixed                       = 102
      , BadDefAttrDecl                     = 103
      , ExpectedDefAttrDecl                = 104
      , AttListSyntaxError                 = 105
      , ExpectedEqSign                     = 106
      , DupAttrName                        = 107
      , BadIdForXMLLangAttr                = 108
      , ExpectedElementName                = 109
      , MustStartWithXMLDecl               = 110
      , CommentsMustStartWith              = 111
      , InvalidDocumentStructure           = 112
      , ExpectedDeclString                 = 113
      , BadXMLVersion                      = 114
      , UnsupportedXMLVersion              = 115
      , UnterminatedXMLDecl                = 116
      , BadXMLEncoding                     = 117
      , BadStandalone                      = 118
      , UnterminatedComment                = 119
      , PINameExpected                     = 120
      , UnterminatedPI                     = 121
      , InvalidCharacter                   = 122
      , UnexpectedTextBeforeRoot           = 123
      , UnterminatedStartTag               = 124
      , ExpectedAttrValue                  = 125
      , UnterminatedEndTag                 = 126
      , ExpectedAttributeType              = 127
      , ExpectedEndOfTagX                  = 128
      , ExpectedMarkup                     = 129
      , NotValidAfterContent               = 130
      , ExpectedComment                    = 131
      , ExpectedCommentOrPI                = 132
      , ExpectedWhitespace                 = 133
      , NoRootElemInDOCTYPE                = 134
      , ExpectedQuotedString               = 135
      , ExpectedPublicId                   = 136
      , InvalidPublicIdChar                = 137
      , UnterminatedDOCTYPE                = 138
      , InvalidCharacterInIntSubset        = 139
      , ExpectedCDATA                      = 140
      , InvalidInitialNameChar             = 141
      , InvalidNameChar                    = 142
      , UnexpectedWhitespace               = 143
      , InvalidCharacterInAttrValue        = 144
      , ExpectedMarkupDecl                 = 145
      , TextDeclNotLegalHere               = 146
      , ConditionalSectInIntSubset         = 147
      , ExpectedPEName                     = 148
      , UnterminatedEntityDecl             = 149
      , InvalidCharacterRef                = 150
      , UnterminatedCharRef                = 151
      , ExpectedEntityRefName              = 152
      , EntityNotFound                     = 153
      , NoUnparsedEntityRefs               = 154
      , UnterminatedEntityRef              = 155
      , RecursiveEntity                    = 156
      , PartialMarkupInEntity              = 157
      , UnterminatedElementDecl            = 158
      , ExpectedContentSpecExpr            = 159
      , ExpectedAsterisk                   = 160
      , UnterminatedContentModel           = 161
      , ExpectedSystemId                   = 162
      , ExpectedSystemOrPublicId           = 163
      , UnterminatedNotationDecl           = 164
      , ExpectedSeqChoiceLeaf              = 165
      , ExpectedChoiceOrCloseParen         = 166
      , ExpectedSeqOrCloseParen            = 167
      , ExpectedEnumValue                  = 168
      , ExpectedEnumSepOrParen             = 169
      , UnterminatedEntityLiteral          = 170
      , MoreEndThanStartTags               = 171
      , IllegalRefInStandalone             = 172
      , ExpectedOpenParen                  = 173
      , AttrAlreadyUsedInSTag              = 174
      , BracketInAttrValue                 = 175
      , Expected2ndSurrogateChar           = 176
      , ExpectedEndOfConditional           = 177
      , ExpectedIncOrIgn                   = 178
      , ExpectedINCLUDEBracket             = 179
      , ExpectedTextDecl                   = 180
      , ExpectedXMLDecl                    = 181
      , UnexpectedEOE                      = 182
      , PEPropogated                       = 183
      , ExtraCloseSquare                   = 184
      , PERefInMarkupInIntSubset           = 185
      , EntityPropogated                   = 186
      , ExpectedNumericalCharRef           = 187
      , ExpectedOpenSquareBracket          = 188
      , BadSequenceInCharData              = 189
      , IllegalSequenceInComment           = 190
      , UnterminatedCDATASection           = 191
      , ExpectedNDATA                      = 192
      , NDATANotValidForPE                 = 193
      , HexRadixMustBeLowerCase            = 194
      , DeclStringRep                      = 195
      , DeclStringsInWrongOrder            = 196
      , NoExtRefsInAttValue                = 197
      , XMLDeclMustBeLowerCase             = 198
      , ExpectedEntityValue                = 199
      , BadDigitForRadix                   = 200
      , EndedWithTagsOnStack               = 201
      , AmbiguousContentModel              = 202
      , NestedCDATA                        = 203
      , UnknownPrefix                      = 204
      , PartialTagMarkupError              = 205
      , EmptyMainEntity                    = 206
      , CDATAOutsideOfContent              = 207
      , OnlyCharRefsAllowedHere            = 208
      , Unexpected2ndSurrogateChar         = 209
      , NoPIStartsWithXML                  = 210
      , XMLDeclMustBeFirst                 = 211
      , XMLVersionRequired                 = 212
      , StandaloneNotLegal                 = 213
      , TooManyColonsInName                = 214
      , InvalidColonPos                    = 215
      , ColonNotLegalWithNS                = 216
      , SysException                       = 217
      , XMLException                       = 218
      , UnexpectedEOF                      = 219
      , UnexpectedError                    = 220
      , BadSchemaLocation                  = 221
      , NoGrammarResolver                  = 222
      , F_HighBounds                       = 223
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

