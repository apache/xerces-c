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
      , DisplayErrorMessage                = 103
      , E_HighBounds                       = 104
      , F_LowBounds                        = 105
      , ExpectedCommentOrCDATA             = 106
      , ExpectedAttrName                   = 107
      , ExpectedNotationName               = 108
      , NoRepInMixed                       = 109
      , BadDefAttrDecl                     = 110
      , ExpectedDefAttrDecl                = 111
      , AttListSyntaxError                 = 112
      , ExpectedEqSign                     = 113
      , DupAttrName                        = 114
      , BadIdForXMLLangAttr                = 115
      , ExpectedElementName                = 116
      , MustStartWithXMLDecl               = 117
      , CommentsMustStartWith              = 118
      , InvalidDocumentStructure           = 119
      , ExpectedDeclString                 = 120
      , BadXMLVersion                      = 121
      , UnsupportedXMLVersion              = 122
      , UnterminatedXMLDecl                = 123
      , BadXMLEncoding                     = 124
      , BadStandalone                      = 125
      , UnterminatedComment                = 126
      , PINameExpected                     = 127
      , UnterminatedPI                     = 128
      , InvalidCharacter                   = 129
      , UnexpectedTextBeforeRoot           = 130
      , UnterminatedStartTag               = 131
      , ExpectedAttrValue                  = 132
      , UnterminatedEndTag                 = 133
      , ExpectedAttributeType              = 134
      , ExpectedEndOfTagX                  = 135
      , ExpectedMarkup                     = 136
      , NotValidAfterContent               = 137
      , ExpectedComment                    = 138
      , ExpectedCommentOrPI                = 139
      , ExpectedWhitespace                 = 140
      , NoRootElemInDOCTYPE                = 141
      , ExpectedQuotedString               = 142
      , ExpectedPublicId                   = 143
      , InvalidPublicIdChar                = 144
      , UnterminatedDOCTYPE                = 145
      , InvalidCharacterInIntSubset        = 146
      , ExpectedCDATA                      = 147
      , InvalidInitialNameChar             = 148
      , InvalidNameChar                    = 149
      , UnexpectedWhitespace               = 150
      , InvalidCharacterInAttrValue        = 151
      , ExpectedMarkupDecl                 = 152
      , TextDeclNotLegalHere               = 153
      , ConditionalSectInIntSubset         = 154
      , ExpectedPEName                     = 155
      , UnterminatedEntityDecl             = 156
      , InvalidCharacterRef                = 157
      , UnterminatedCharRef                = 158
      , ExpectedEntityRefName              = 159
      , EntityNotFound                     = 160
      , NoUnparsedEntityRefs               = 161
      , UnterminatedEntityRef              = 162
      , RecursiveEntity                    = 163
      , PartialMarkupInEntity              = 164
      , UnterminatedElementDecl            = 165
      , ExpectedContentSpecExpr            = 166
      , ExpectedAsterisk                   = 167
      , UnterminatedContentModel           = 168
      , ExpectedSystemId                   = 169
      , ExpectedSystemOrPublicId           = 170
      , UnterminatedNotationDecl           = 171
      , ExpectedSeqChoiceLeaf              = 172
      , ExpectedChoiceOrCloseParen         = 173
      , ExpectedSeqOrCloseParen            = 174
      , ExpectedEnumValue                  = 175
      , ExpectedEnumSepOrParen             = 176
      , UnterminatedEntityLiteral          = 177
      , MoreEndThanStartTags               = 178
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

