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
      , AttributeRequired                  = 96
      , AttributeDisallowed                = 97
      , DisplayErrorMessage                = 98
      , E_HighBounds                       = 99
      , F_LowBounds                        = 100
      , ExpectedCommentOrCDATA             = 101
      , ExpectedAttrName                   = 102
      , ExpectedNotationName               = 103
      , NoRepInMixed                       = 104
      , BadDefAttrDecl                     = 105
      , ExpectedDefAttrDecl                = 106
      , AttListSyntaxError                 = 107
      , ExpectedEqSign                     = 108
      , DupAttrName                        = 109
      , BadIdForXMLLangAttr                = 110
      , ExpectedElementName                = 111
      , MustStartWithXMLDecl               = 112
      , CommentsMustStartWith              = 113
      , InvalidDocumentStructure           = 114
      , ExpectedDeclString                 = 115
      , BadXMLVersion                      = 116
      , UnsupportedXMLVersion              = 117
      , UnterminatedXMLDecl                = 118
      , BadXMLEncoding                     = 119
      , BadStandalone                      = 120
      , UnterminatedComment                = 121
      , PINameExpected                     = 122
      , UnterminatedPI                     = 123
      , InvalidCharacter                   = 124
      , UnexpectedTextBeforeRoot           = 125
      , UnterminatedStartTag               = 126
      , ExpectedAttrValue                  = 127
      , UnterminatedEndTag                 = 128
      , ExpectedAttributeType              = 129
      , ExpectedEndOfTagX                  = 130
      , ExpectedMarkup                     = 131
      , NotValidAfterContent               = 132
      , ExpectedComment                    = 133
      , ExpectedCommentOrPI                = 134
      , ExpectedWhitespace                 = 135
      , NoRootElemInDOCTYPE                = 136
      , ExpectedQuotedString               = 137
      , ExpectedPublicId                   = 138
      , InvalidPublicIdChar                = 139
      , UnterminatedDOCTYPE                = 140
      , InvalidCharacterInIntSubset        = 141
      , ExpectedCDATA                      = 142
      , InvalidInitialNameChar             = 143
      , InvalidNameChar                    = 144
      , UnexpectedWhitespace               = 145
      , InvalidCharacterInAttrValue        = 146
      , ExpectedMarkupDecl                 = 147
      , TextDeclNotLegalHere               = 148
      , ConditionalSectInIntSubset         = 149
      , ExpectedPEName                     = 150
      , UnterminatedEntityDecl             = 151
      , InvalidCharacterRef                = 152
      , UnterminatedCharRef                = 153
      , ExpectedEntityRefName              = 154
      , EntityNotFound                     = 155
      , NoUnparsedEntityRefs               = 156
      , UnterminatedEntityRef              = 157
      , RecursiveEntity                    = 158
      , PartialMarkupInEntity              = 159
      , UnterminatedElementDecl            = 160
      , ExpectedContentSpecExpr            = 161
      , ExpectedAsterisk                   = 162
      , UnterminatedContentModel           = 163
      , ExpectedSystemId                   = 164
      , ExpectedSystemOrPublicId           = 165
      , UnterminatedNotationDecl           = 166
      , ExpectedSeqChoiceLeaf              = 167
      , ExpectedChoiceOrCloseParen         = 168
      , ExpectedSeqOrCloseParen            = 169
      , ExpectedEnumValue                  = 170
      , ExpectedEnumSepOrParen             = 171
      , UnterminatedEntityLiteral          = 172
      , MoreEndThanStartTags               = 173
      , IllegalRefInStandalone             = 174
      , ExpectedOpenParen                  = 175
      , AttrAlreadyUsedInSTag              = 176
      , BracketInAttrValue                 = 177
      , Expected2ndSurrogateChar           = 178
      , ExpectedEndOfConditional           = 179
      , ExpectedIncOrIgn                   = 180
      , ExpectedINCLUDEBracket             = 181
      , ExpectedTextDecl                   = 182
      , ExpectedXMLDecl                    = 183
      , UnexpectedEOE                      = 184
      , PEPropogated                       = 185
      , ExtraCloseSquare                   = 186
      , PERefInMarkupInIntSubset           = 187
      , EntityPropogated                   = 188
      , ExpectedNumericalCharRef           = 189
      , ExpectedOpenSquareBracket          = 190
      , BadSequenceInCharData              = 191
      , IllegalSequenceInComment           = 192
      , UnterminatedCDATASection           = 193
      , ExpectedNDATA                      = 194
      , NDATANotValidForPE                 = 195
      , HexRadixMustBeLowerCase            = 196
      , DeclStringRep                      = 197
      , DeclStringsInWrongOrder            = 198
      , NoExtRefsInAttValue                = 199
      , XMLDeclMustBeLowerCase             = 200
      , ExpectedEntityValue                = 201
      , BadDigitForRadix                   = 202
      , EndedWithTagsOnStack               = 203
      , AmbiguousContentModel              = 204
      , NestedCDATA                        = 205
      , UnknownPrefix                      = 206
      , PartialTagMarkupError              = 207
      , EmptyMainEntity                    = 208
      , CDATAOutsideOfContent              = 209
      , OnlyCharRefsAllowedHere            = 210
      , Unexpected2ndSurrogateChar         = 211
      , NoPIStartsWithXML                  = 212
      , XMLDeclMustBeFirst                 = 213
      , XMLVersionRequired                 = 214
      , StandaloneNotLegal                 = 215
      , TooManyColonsInName                = 216
      , InvalidColonPos                    = 217
      , ColonNotLegalWithNS                = 218
      , SysException                       = 219
      , XMLException                       = 220
      , UnexpectedEOF                      = 221
      , UnexpectedError                    = 222
      , BadSchemaLocation                  = 223
      , NoGrammarResolver                  = 224
      , F_HighBounds                       = 225
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

