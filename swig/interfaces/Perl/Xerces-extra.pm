package XML::Xerces;

use strict;
use Carp;
use vars qw(@EXPORT_OK $VERSION %REMEMBER);
@EXPORT_OK = qw(error);
$VERSION = 260.2;

#
# Cleanup removes all objects being remembered by an object
#
sub cleanup {
  my $self = shift;
  return unless tied(%{$self});

# printf STDERR "Cleaning up: $self\n";
  if (exists $REMEMBER{tied(%{$self})}) {
      delete $REMEMBER{tied(%{$self})};
  }
# my $class = ref $self;
#   if (exists $class::OWNER{tied(%{$self})}) {
# printf STDERR "\tObject is owner: $self\n";
#   }
}

sub error {
  my $error = shift;
  my $context = shift;
  my $msg = "Error in eval: ";
  if (ref $error) {
    if ($error->isa('XML::Xerces::DOMException')) {
      $msg .= "Message: <"
        . $error->getMessage()
	. "> Code: "
        . $XML::Xerces::DOMException::CODES[$error->getCode];
    } else {
      $msg .= $error->getMessage();
    }
  } else {
    $msg .= $error;
  }
  $msg .= ", Context: $context"
    if defined $context;
  croak($msg);
}

package XML::Xerces::DOMException;
use vars qw(@CODES);
@CODES = qw(__NONEXISTENT__
	    INDEX_SIZE_ERR
	    DOMSTRING_SIZE_ERR
	    HIERARCHY_REQUEST_ERR
	    WRONG_DOCUMENT_ERR
	    INVALID_CHARACTER_ERR
	    NO_DATA_ALLOWED_ERR
	    NO_MODIFICATION_ALLOWED_ERR
	    NOT_FOUND_ERR
	    NOT_SUPPORTED_ERR
	    INUSE_ATTRIBUTE_ERR
	    INVALID_STATE_ERR
	    SYNTAX_ERR
	    INVALID_MODIFICATION_ERR
	    NAMESPACE_ERR
	    INVALID_ACCESS_ERR
	   );

package XML::Xerces::XMLExcepts;
use vars qw(@CODES);
@CODES = qw(NoError
	    W_LowBounds
	    Scan_CouldNotOpenSource_Warning
	    GC_ExistingGrammar
	    W_HighBounds
	    F_LowBounds
	    Array_BadIndex
	    Array_BadNewSize
	    AttrList_BadIndex
	    AttDef_BadAttType
	    AttDef_BadDefAttType
	    Bitset_BadIndex
	    Bitset_NotEqualSize
	    BufMgr_NoMoreBuffers
	    BufMgr_BufferNotInPool
	    CPtr_PointerIsZero
	    CM_BinOpHadUnaryType
	    CM_MustBeMixedOrChildren
	    CM_NoPCDATAHere
	    CM_NotValidForSpecType
	    CM_UnaryOpHadBinType
	    CM_UnknownCMType
	    CM_UnknownCMSpecType
	    CM_NoParentCSN
	    CM_NotValidSpecTypeForNode
	    DTD_UnknownCreateReason
	    ElemStack_EmptyStack
	    ElemStack_BadIndex
	    ElemStack_StackUnderflow
	    ElemStack_NoParentPushed
	    Enum_NoMoreElements
	    File_CouldNotOpenFile
	    File_CouldNotGetCurPos
	    File_CouldNotCloseFile
	    File_CouldNotSeekToEnd
	    File_CouldNotSeekToPos
	    File_CouldNotDupHandle
	    File_CouldNotReadFromFile
	    File_CouldNotWriteToFile
	    File_CouldNotResetFile
	    File_CouldNotGetSize
	    File_CouldNotGetBasePathName
	    File_BasePathUnderflow
	    Gen_ParseInProgress
	    Gen_NoDTDValidator
	    Gen_CouldNotOpenDTD
	    Gen_CouldNotOpenExtEntity
	    Gen_UnexpectedEOF
	    HshTbl_ZeroModulus
	    HshTbl_BadHashFromKey
	    HshTbl_NoSuchKeyExists
	    Mutex_CouldNotCreate
	    Mutex_CouldNotClose
	    Mutex_CouldNotLock
	    Mutex_CouldNotUnlock
	    Mutex_CouldNotDestroy
	    NetAcc_InternalError
	    NetAcc_LengthError
	    NetAcc_InitFailed
	    NetAcc_TargetResolution
	    NetAcc_CreateSocket
	    NetAcc_ConnSocket
	    NetAcc_WriteSocket
	    NetAcc_ReadSocket
	    NetAcc_UnsupportedMethod
	    Pool_ElemAlreadyExists
	    Pool_BadHashFromKey
	    Pool_InvalidId
	    Pool_ZeroModulus
	    RdrMgr_ReaderIdNotFound
	    Reader_BadAutoEncoding
	    Reader_CouldNotDecodeFirstLine
	    Reader_NelLsepinDecl
	    Reader_EOIInMultiSeq
	    Reader_SrcOfsNotSupported
	    Reader_EncodingStrRequired
	    Scan_CouldNotOpenSource
	    Scan_UnbalancedStartEnd
	    Scan_BadPScanToken
	    Stack_BadIndex
	    Stack_EmptyStack
	    Str_ZeroSizedTargetBuf
	    Str_UnknownRadix
	    Str_TargetBufTooSmall
	    Str_StartIndexPastEnd
	    Str_ConvertOverflow
	    Strm_StdErrWriteFailure
	    Strm_StdOutWriteFailure
	    Strm_ConWriteFailure
	    StrPool_IllegalId
	    Trans_CouldNotCreateDefCvtr
	    Trans_InvalidSizeReq
	    Trans_Unrepresentable
	    Trans_NotValidForEncoding
	    Trans_BadBlockSize
	    Trans_BadSrcSeq
	    Trans_BadSrcCP
	    Trans_BadTrailingSurrogate
	    Trans_CantCreateCvtrFor
	    URL_MalformedURL
	    URL_UnsupportedProto
	    URL_UnsupportedProto1
	    URL_OnlyLocalHost
	    URL_NoProtocolPresent
	    URL_ExpectingTwoSlashes
	    URL_IncorrectEscapedCharRef
	    URL_UnterminatedHostComponent
	    URL_RelativeBaseURL
	    URL_BaseUnderflow
	    URL_BadPortField
	    UTF8_FormatError
	    UTF8_Invalid_2BytesSeq
	    UTF8_Invalid_3BytesSeq
	    UTF8_Irregular_3BytesSeq
	    UTF8_Invalid_4BytesSeq
	    UTF8_Exceede_BytesLimit
	    Vector_BadIndex
	    Val_InvalidElemId
	    Val_CantHaveIntSS
	    XMLRec_UnknownEncoding
	    Parser_Parse1
	    Parser_Parse2
	    Parser_Next1
	    Parser_Next2
	    Parser_Next3
	    Parser_Next4
	    Parser_Factor1
	    Parser_Factor2
	    Parser_Factor3
	    Parser_Factor4
	    Parser_Factor5
	    Parser_Factor6
	    Parser_Atom1
	    Parser_Atom2
	    Parser_Atom3
	    Parser_Atom4
	    Parser_Atom5
	    Parser_CC1
	    Parser_CC2
	    Parser_CC3
	    Parser_CC4
	    Parser_CC5
	    Parser_CC6
	    Parser_Ope1
	    Parser_Ope2
	    Parser_Ope3
	    Parser_Descape1
	    Parser_Descape2
	    Parser_Descape3
	    Parser_Descape4
	    Parser_Descape5
	    Parser_Process1
	    Parser_Process2
	    Parser_Quantifier1
	    Parser_Quantifier2
	    Parser_Quantifier3
	    Parser_Quantifier4
	    Parser_Quantifier5
	    Gen_NoSchemaValidator
	    XUTIL_UnCopyableNodeType
	    SubGrpComparator_NGR
	    FACET_Invalid_Len
	    FACET_Invalid_maxLen
	    FACET_Invalid_minLen
	    FACET_NonNeg_Len
	    FACET_NonNeg_maxLen
	    FACET_NonNeg_minLen
	    FACET_Len_maxLen
	    FACET_Len_minLen
	    FACET_maxLen_minLen
	    FACET_bool_Pattern
	    FACET_Invalid_Tag
	    FACET_Len_baseLen
	    FACET_minLen_baseminLen
	    FACET_minLen_basemaxLen
	    FACET_maxLen_basemaxLen
	    FACET_maxLen_baseminLen
	    FACET_Len_baseMinLen
	    FACET_Len_baseMaxLen
	    FACET_minLen_baseLen
	    FACET_maxLen_baseLen
	    FACET_enum_base
	    FACET_Invalid_WS
	    FACET_WS_collapse
	    FACET_WS_replace
	    FACET_Invalid_MaxIncl
	    FACET_Invalid_MaxExcl
	    FACET_Invalid_MinIncl
	    FACET_Invalid_MinExcl
	    FACET_Invalid_TotalDigit
	    FACET_Invalid_FractDigit
	    FACET_PosInt_TotalDigit
	    FACET_NonNeg_FractDigit
	    FACET_max_Incl_Excl
	    FACET_min_Incl_Excl
	    FACET_maxExcl_minExcl
	    FACET_maxExcl_minIncl
	    FACET_maxIncl_minExcl
	    FACET_maxIncl_minIncl
	    FACET_TotDigit_FractDigit
	    FACET_maxIncl_base_maxExcl
	    FACET_maxIncl_base_maxIncl
	    FACET_maxIncl_base_minIncl
	    FACET_maxIncl_base_minExcl
	    FACET_maxExcl_base_maxExcl
	    FACET_maxExcl_base_maxIncl
	    FACET_maxExcl_base_minIncl
	    FACET_maxExcl_base_minExcl
	    FACET_minExcl_base_maxExcl
	    FACET_minExcl_base_maxIncl
	    FACET_minExcl_base_minIncl
	    FACET_minExcl_base_minExcl
	    FACET_minIncl_base_maxExcl
	    FACET_minIncl_base_maxIncl
	    FACET_minIncl_base_minIncl
	    FACET_minIncl_base_minExcl
	    FACET_maxIncl_notFromBase
	    FACET_maxExcl_notFromBase
	    FACET_minIncl_notFromBase
	    FACET_minExcl_notFromBase
	    FACET_totalDigit_base_totalDigit
	    FACET_fractDigit_base_totalDigit
	    FACET_fractDigit_base_fractDigit
	    FACET_maxIncl_base_fixed
	    FACET_maxExcl_base_fixed
	    FACET_minIncl_base_fixed
	    FACET_minExcl_base_fixed
	    FACET_totalDigit_base_fixed
	    FACET_fractDigit_base_fixed
	    FACET_maxLen_base_fixed
	    FACET_minLen_base_fixed
	    FACET_len_base_fixed
	    FACET_whitespace_base_fixed
	    FACET_internalError_fixed
	    FACET_List_Null_baseValidator
	    FACET_Union_Null_memberTypeValidators
	    FACET_Union_Null_baseValidator
	    FACET_Union_invalid_baseValidatorType
	    VALUE_NotMatch_Pattern
	    VALUE_Not_Base64
	    VALUE_Not_HexBin
	    VALUE_GT_maxLen
	    VALUE_LT_minLen
	    VALUE_NE_Len
	    VALUE_NotIn_Enumeration
	    VALUE_exceed_totalDigit
	    VALUE_exceed_fractDigit
	    VALUE_exceed_maxIncl
	    VALUE_exceed_maxExcl
	    VALUE_exceed_minIncl
	    VALUE_exceed_minExcl
	    VALUE_WS_replaced
	    VALUE_WS_collapsed
	    VALUE_Invalid_NCName
	    VALUE_Invalid_Name
	    VALUE_ID_Not_Unique
	    VALUE_ENTITY_Invalid
	    VALUE_QName_Invalid
	    VALUE_NOTATION_Invalid
	    VALUE_no_match_memberType
	    VALUE_URI_Malformed
	    XMLNUM_emptyString
	    XMLNUM_WSString
	    XMLNUM_2ManyDecPoint
	    XMLNUM_Inv_chars
	    XMLNUM_null_ptr
	    XMLNUM_URI_Component_Empty
	    XMLNUM_URI_Component_for_GenURI_Only
	    XMLNUM_URI_Component_Invalid_EscapeSequence
	    XMLNUM_URI_Component_Invalid_Char
	    XMLNUM_URI_Component_Set_Null
	    XMLNUM_URI_Component_Not_Conformant
	    XMLNUM_URI_No_Scheme
	    XMLNUM_URI_NullHost
	    XMLNUM_URI_NullPath
	    XMLNUM_URI_Component_inPath
	    XMLNUM_URI_PortNo_Invalid
	    XMLNUM_DBL_FLT_maxNeg
	    XMLNUM_DBL_FLT_maxPos
	    XMLNUM_DBL_FLT_minNegPos
	    XMLNUM_DBL_FLT_InvalidType
	    XMLNUM_DBL_FLT_No_Exponent
	    Regex_Result_Not_Set
	    Regex_CompactRangesError
	    Regex_MergeRangesTypeMismatch
	    Regex_SubtractRangesError
	    Regex_IntersectRangesError
	    Regex_ComplementRangesInvalidArg
	    Regex_InvalidCategoryName
	    Regex_KeywordNotFound
	    Regex_BadRefNo
	    Regex_UnknownOption
	    Regex_UnknownTokenType
	    Regex_RangeTokenGetError
	    Regex_NotSupported
	    Regex_InvalidChildIndex
	    Regex_RepPatMatchesZeroString
	    Regex_InvalidRepPattern
	    NEL_RepeatedCalls
	    RethrowError
	    Out_Of_Memory
	    DV_InvalidOperation
	    XPath_NoAttrSelector
	    XPath_NoUnionAtStart
	    XPath_NoMultipleUnion
	    XPath_MissingAttr
	    XPath_ExpectedToken1
	    XPath_PrefixNoURI
	    XPath_NoDoubleColon
	    XPath_ExpectedStep1
	    XPath_ExpectedStep2
	    XPath_ExpectedStep3
	    XPath_NoForwardSlash
	    XPath_NoDoubleForwardSlash
	    XPath_NoForwardSlashAtStart
	    XPath_NoSelectionOfRoot
	    XPath_EmptyExpr
	    XPath_NoUnionAtEnd
	    XPath_InvalidChar
	    XPath_TokenNotSupported
	    XPath_FindSolution
	    DateTime_Assert_Buffer_Fail
	    DateTime_dt_missingT
	    DateTime_gDay_invalid
	    DateTime_gMth_invalid
	    DateTime_gMthDay_invalid
	    DateTime_dur_Start_dashP
	    DateTime_dur_noP
	    DateTime_dur_DashNotFirst
	    DateTime_dur_inv_b4T
	    DateTime_dur_NoTimeAfterT
	    DateTime_dur_NoElementAtAll
	    DateTime_dur_inv_seconds
	    DateTime_date_incomplete
	    DateTime_date_invalid
	    DateTime_time_incomplete
	    DateTime_time_invalid
	    DateTime_ms_noDigit
	    DateTime_ym_incomplete
	    DateTime_ym_invalid
	    DateTime_year_tooShort
	    DateTime_year_leadingZero
	    DateTime_ym_noMonth
	    DateTime_tz_noUTCsign
	    DateTime_tz_stuffAfterZ
	    DateTime_tz_invalid
	    DateTime_year_zero
	    DateTime_mth_invalid
	    DateTime_day_invalid
	    DateTime_hour_invalid
	    DateTime_min_invalid
	    DateTime_second_invalid
	    DateTime_tz_hh_invalid
	    PD_EmptyBase
	    PD_NSCompat1
	    PD_OccurRangeE
	    PD_NameTypeOK1
	    PD_NameTypeOK2
	    PD_NameTypeOK3
	    PD_NameTypeOK4
	    PD_NameTypeOK5
	    PD_NameTypeOK6
	    PD_NameTypeOK7
	    PD_RecurseAsIfGroup
	    PD_Recurse1
	    PD_Recurse2
	    PD_ForbiddenRes1
	    PD_ForbiddenRes2
	    PD_ForbiddenRes3
	    PD_ForbiddenRes4
	    PD_NSSubset1
	    PD_NSSubset2
	    PD_NSRecurseCheckCardinality1
	    PD_RecurseUnordered
	    PD_MapAndSum
	    PD_InvalidContentType
	    NodeIDMap_GrowErr
	    XSer_ProtoType_Null_ClassName
	    XSer_ProtoType_NameLen_Dif
	    XSer_ProtoType_Name_Dif
	    XSer_InStream_Read_LT_Req
	    XSer_InStream_Read_OverFlow
	    XSer_Storing_Violation
	    XSer_StoreBuffer_Violation
	    XSer_LoadPool_UppBnd_Exceed
	    XSer_LoadPool_NoTally_ObjCnt
	    XSer_Loading_Violation
	    XSer_LoadBuffer_Violation
	    XSer_Inv_ClassIndex
	    XSer_Inv_FillBuffer_Size
	    XSer_Inv_checkFillBuffer_Size
	    XSer_Inv_checkFlushBuffer_Size
	    XSer_Inv_Null_Pointer
	    XSer_Inv_Buffer_Len
	    XSer_CreateObject_Fail
	    XSer_ObjCount_UppBnd_Exceed
	    XSer_GrammarPool_Locked
	    XSer_GrammarPool_Empty
	    XSer_GrammarPool_NotEmpty
	    XSer_StringPool_NotEmpty
	    XSer_BinaryData_Version_NotSupported
	    F_HighBounds
	    E_LowBounds
	    E_HighBounds
	   );

############# Class : XML::Xerces::PerlContentHandler ##############
package XML::Xerces::PerlContentHandler;
use vars qw(@ISA);
@ISA = qw();
sub new {
  my $class = shift;
  return bless {}, $class;
}

sub start_element {}
sub end_element {}
sub start_prefix_mapping {}
sub end_prefix_mapping {}
sub skipped_entity {}
sub start_document {}
sub end_document {}
sub reset_document {}
sub characters {}
sub processing_instruction {}
sub set_document_locator {}
sub ignorable_whitespace {}


############# Class : XML::Xerces::PerlDocumentHandler ##############
package XML::Xerces::PerlDocumentHandler;
use vars qw(@ISA);
@ISA = qw();
sub new {
  my $class = shift;
  return bless {}, $class;
}

sub start_element {}
sub end_element {}
sub start_document {}
sub end_document {}
sub reset_document {}
sub characters {}
sub processing_instruction {}
sub set_document_locator {}
sub ignorable_whitespace {}


############# Class : XML::Xerces::PerlEntityResolver ##############
package XML::Xerces::PerlEntityResolver;
use vars qw(@ISA);
@ISA = qw();
sub new {
  my $class = shift;
  return bless {}, $class;
}

sub resolve_entity {
  return undef;
}


############# Class : XML::Xerces::PerlNodeFilter ##############
package XML::Xerces::PerlNodeFilter;
use vars qw(@ISA);
@ISA = qw();
sub new {
  my $class = shift;
  return bless {}, $class;
}

sub acceptNode {
  return undef;
}


############# Class : XML::Xerces::PerlErrorHandler ##############
package XML::Xerces::PerlErrorHandler;
use Carp;
use vars qw(@ISA);
@ISA = qw();
sub new {
  my $class = shift;
  return bless {}, $class;
}

sub warning {
  my $system_id = $_[1]->getSystemId;
  my $line_num = $_[1]->getLineNumber;
  my $col_num = $_[1]->getColumnNumber;
  my $msg = $_[1]->getMessage;
  carp(<<EOT);
WARNING:
FILE:    $system_id
LINE:    $line_num
COLUMN:  $col_num
MESSAGE: $msg
EOT
}

sub error {
  my $system_id = $_[1]->getSystemId;
  my $line_num = $_[1]->getLineNumber;
  my $col_num = $_[1]->getColumnNumber;
  my $msg = $_[1]->getMessage;
  croak(<<EOT);
ERROR:
FILE:    $system_id
LINE:    $line_num
COLUMN:  $col_num
MESSAGE: $msg
EOT
}

sub fatal_error {
  my $system_id = $_[1]->getSystemId;
  my $line_num = $_[1]->getLineNumber;
  my $col_num = $_[1]->getColumnNumber;
  my $msg = $_[1]->getMessage;
  croak(<<EOT);
FATAL ERROR:
FILE:    $system_id
LINE:    $line_num
COLUMN:  $col_num
MESSAGE: $msg
EOT
}


sub reset_errors {}

package XML::Xerces::XMLAttDefList;
#
# This class is both a list and a hash, so at the moment we
# enable users to choose how to access the information. Perhaps
# in the future we will use an order preserving hash like Tie::IxHash.
#

# convert the AttDefList to a perl list
sub to_list {
  my $self = shift;
  my @list;
  if ($self->hasMoreElements()) {
    while ($self->hasMoreElements()) {
      push(@list,$self->nextElement());
    }
  }
  return @list;
}

# convert the AttDefList to a perl hash
sub to_hash {
  my $self = shift;
  my %hash;
  if ($self->hasMoreElements()) {
    while ($self->hasMoreElements()) {
      my $attr = $self->nextElement();
      $hash{$attr->getFullName()} = $attr;
    }
  }
  return %hash;
}

package XML::Xerces::DOMNodeList;
# convert the NodeList to a perl list
sub to_list {
  my $self = shift;
  my @list;
  for (my $i=0;$i<$self->getLength();$i++) {
    push(@list,$self->item($i));
  }
  return @list;
}

package XML::Xerces::Attributes;
sub to_hash {
  my $self = shift;
  my %hash;
  for (my $i=0; $i < $self->getLength(); $i++) {
    my $qname = $self->getQName($i);
    $hash{$qname}->{localName} = $self->getLocalName($i);
    $hash{$qname}->{URI} = $self->getURI($i);
    $hash{$qname}->{value} = $self->getValue($i);
    $hash{$qname}->{type} = $self->getType($i);
  }
  return %hash;
}

package XML::Xerces::AttributeList;
sub to_hash {
  my $self = shift;
  my %hash;
  for (my $i=0;$i<$self->getLength();$i++) {
    $hash{$self->getName($i)} = $self->getValue($i)
  }
  return %hash;
}

package XML::Xerces::DOMNamedNodeMap;
# convert the NamedNodeMap to a perl hash
sub to_hash {
  my $self = shift;
  my @list;
  for (my $i=0;$i<$self->getLength();$i++) {
    my $node = $self->item($i);
    push(@list, $node->to_hash());
  }
  return @list;
}

package XML::Xerces::DOMNode;
sub to_hash {
  my $self = shift;
  return ($self->getNodeName,$self->getNodeValue);
}

sub quote_content {
  my ($self,$node_value) = @_;

  $node_value =~ s/&/&amp;/g;
  $node_value =~ s/</&lt;/g;
  $node_value =~ s/>/&gt;/g;
  $node_value =~ s/\"/&quot;/g;
  $node_value =~ s/\'/&apos;/g;

  return $node_value;
}

package XML::Xerces::DOMEntity;
sub to_hash {
  my $self = shift;
  if ($self->hasChildNodes) {
    return ($self->getNodeName(),
            $self->getFirstChild->getNodeValue());
  } else {
    return ($self->getNodeName(), '');
  }
}

package XML::Xerces::DOMText;
sub serialize {
  return $_[0]->quote_content($_[0]->getNodeValue);
}

package XML::Xerces::DOMProcessingInstruction;
sub serialize {
  my $output .= '<?' . $_[0]->getNodeName;
  if (length(my $str = $_[0]->getNodeValue)) {
    $output .= " $str"; 
  }
  $output .= '?>';
  return $output;
}

package XML::Xerces::DOMDocument;
sub serialize {
  my $output;
  my $indent = 2;
  for(my $child = $_[0]->getFirstChild() ;
     defined $child ;
     $child = $child->getNextSibling())
  {
    $output .= $child->serialize($indent);
  }
  return "$output\n";
}

package XML::Xerces::DOMElement;
sub serialize {
  my ($self,$indent) = @_;
  $indent ||= 0;
  my $output;
  ELEMENT: {
    my $node_name = $self->getNodeName;
    $output .= "<$node_name";

    my $attributes = $self->getAttributes;
    my $attribute_count = $attributes->getLength;

    for(my $ix = 0 ; $ix < $attribute_count ; ++$ix) {
      my $attribute = $attributes->item($ix);
      $output .= ' ' . $attribute->getNodeName . '="' . $self->quote_content($attribute->getNodeValue) . '"';
    }

    my $child = $self->getFirstChild();
    if (!defined $child) {
      $output .= '/>';
      last ELEMENT;
    }

    $output .= '>';
    while (defined $child) {
      $output .= $child->serialize($indent+2);
      $child = $child->getNextSibling();
    }
    $output .= "</$node_name>";
  }
  return $output;
}

package XML::Xerces::DOMEntityReference;
sub serialize {
  my ($self) = @_;
  my $output;
  for(my $child = $self->getFirstChild() ;
     defined $child;
     $child = $child->getNextSibling())
  {
    $output .= $child->serialize();
  }
  return $output;
}

package XML::Xerces::DOMCDATASection;
sub serialize {
  return '<![CDATA[' . $_[0]->getNodeValue . ']]>';
}

package XML::Xerces::DOMComment;
sub serialize {
  return '<!--' . $_[0]->getNodeValue . "-->\n";
}

package XML::Xerces::DOMDocumentType;
sub serialize {
  my $output;
  $output .= '<!DOCTYPE ' . $_[0]->getNodeName;

  my $id;
  if ($id = $_[0]->getPublicId) {
    $output .= qq[ PUBLIC "$id"];
    if ($id = $_[0]->getSystemId) {
      $output .= qq[ "$id"];
    }
  } elsif ($id = $_[0]->getSystemId) {
    $output .= qq[ SYSTEM "$id"];
  }

  if ($id = $_[0]->getInternalSubset) {
    $output .= " [$id]";
  }

  $output .= ">\n";
  return $output;
}

package XML::Xerces::DOMEntity;
sub serialize {
  my $output;
  $output .= '<!ENTITY ' . $_[0]->getNodeName;

  my $id;
  if ($id = $_[0]->getPublicId) { $output .= qq[ PUBLIC "$id"]; }
  if ($id = $_[0]->getSystemId) { $output .= qq[ SYSTEM "$id"]; }
  if ($id = $_[0]->getNotationName) { $output .= qq[ NDATA "$id"]; }

  $output .= '>';
  return $output;
}

package XML::Xerces::DOMException;
sub getMessage {
  return shift->{msg};
}

sub getCode {
  return shift->{code};
}

# in previous versions we needed to define this method
# but it is now obsolete
package XML::Xerces::DOMElement;
sub get_text {
  my $self = shift;
  warn "XML::Xerces::DOMElement::get_text is depricated, use getTextContent instead";
  return $self->getTextContent(@_);
}

package XML::Xerces::XMLCatalogResolver;
use strict;
use Carp;
use vars qw($VERSION
	    @ISA
	    @EXPORT
	    @EXPORT_OK
	   );
require Exporter;

@ISA = qw(Exporter XML::Xerces::PerlEntityResolver);
@EXPORT_OK = qw();

sub new {
  my $pkg = shift;
  my $catalog = shift;
  my $self = bless {}, $pkg;
  $self->catalog($catalog);
  $self->initialize()
    if defined $catalog;
  return $self;
}

sub initialize {
  my $self = shift;
  my $CATALOG = $self->catalog();
  XML::Xerces::error (__PACKAGE__ . ": Must set catalog before calling initialize")
      unless defined $CATALOG;

  my $DOM = XML::Xerces::XercesDOMParser->new();
  my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
  $DOM->setErrorHandler($ERROR_HANDLER);

  # we parse the example XML Catalog
  eval{$DOM->parse($CATALOG)};
  XML::Xerces::error ($@, __PACKAGE__ . ": Couldn't parse catalog: $CATALOG")
      if $@;

  # now retrieve the mappings and store them
  my $doc = $DOM->getDocument();
  my @maps = $doc->getElementsByTagName('Map');
  my %maps = map {($_->getAttribute('PublicId'),
		   $_->getAttribute('HRef'))} @maps;
  $self->maps(\%maps);
  my @remaps = $doc->getElementsByTagName('Remap');
  my %remaps = map {($_->getAttribute('SystemId'),
		     $_->getAttribute('HRef'))} @remaps;
  $self->remaps(\%remaps);
}

sub catalog {
  my $self = shift;
  if (@_) {
    $self->{__CATALOG} = shift;
  }
  return $self->{__CATALOG};
}

sub maps {
  my $self = shift;
  if (@_) {
    $self->{__MAPS} = shift;
  }
  return $self->{__MAPS};
}

sub remaps {
  my $self = shift;
  if (@_) {
    $self->{__REMAPS} = shift;
  }
  return $self->{__REMAPS};
}

sub resolve_entity {
  my ($self,$pub,$sys) = @_;
  # print STDERR "Got PUBLIC: $pub\n";
  # print STDERR "Got SYSTEM: $sys\n";

  XML::Xerces::error (__PACKAGE__ . ": Must call initialize before using the resolver")
      unless defined $self->maps or defined $self->remaps;

  # now check which one we were asked for
  my $href;
  if ($pub) {
    $href = $self->maps->{$pub};
  }
  if ((not defined $href) and $sys) {
    $href = $self->remaps->{$sys};
  }
  if (not defined $href) {
    croak("could not resolve PUBLIC id:[$pub] or SYSTEM id: [$sys] using catalog: ["
	  . $self->catalog . "]");
  }

  my $is = eval {XML::Xerces::LocalFileInputSource->new($href)};
  error($@,"Couldn't create input source for $href")
      if $@;

  # tell Perl *not* to manage the memory for the InputSource
  $is->DISOWN();
  return $is;
}

