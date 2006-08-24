 #
 # Copyright 2002,2004 The Apache Software Foundation.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #      http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #

# This file was created automatically by SWIG 1.3.29.
# Don't modify this file, modify the SWIG interface instead.
package XML::Xerces;
require Exporter;
require DynaLoader;
@ISA = qw(Exporter DynaLoader);
package XML::Xercesc;
bootstrap XML::Xerces;
package XML::Xerces;
@EXPORT = qw( );
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

package XML::Xerces; 
use vars qw($VERSION @EXPORT);

# ---------- BASE METHODS -------------

package XML::Xerces;

sub TIEHASH {
    my ($classname,$obj) = @_;
    return bless $obj, $classname;
}

sub CLEAR { }

sub FIRSTKEY { }

sub NEXTKEY { }

sub FETCH {
    my ($self,$field) = @_;
    my $member_func = "swig_${field}_get";
    $self->$member_func();
}

sub STORE {
    my ($self,$field,$newval) = @_;
    my $member_func = "swig_${field}_set";
    $self->$member_func($newval);
}

sub this {
    my $ptr = shift;
    return tied(%$ptr);
}


# ------- FUNCTION WRAPPERS --------

package XML::Xerces;


############# Class : XML::Xerces::XMLDeleter ##############

package XML::Xerces::XMLDeleter;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLDeleter($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLPlatformUtils ##############

package XML::Xerces::XMLPlatformUtils;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*fgFileMgr = *XML::Xercesc::XMLPlatformUtils_fgFileMgr;
*fgMutexMgr = *XML::Xercesc::XMLPlatformUtils_fgMutexMgr;
*fgAtomicOpMgr = *XML::Xercesc::XMLPlatformUtils_fgAtomicOpMgr;
*fgXMLChBigEndian = *XML::Xercesc::XMLPlatformUtils_fgXMLChBigEndian;
*Initialize = *XML::Xercesc::XMLPlatformUtils_Initialize;
*Terminate = *XML::Xercesc::XMLPlatformUtils_Terminate;
*panic = *XML::Xercesc::XMLPlatformUtils_panic;
*makeFileMgr = *XML::Xercesc::XMLPlatformUtils_makeFileMgr;
*makeMutexMgr = *XML::Xercesc::XMLPlatformUtils_makeMutexMgr;
*makeAtomicOpMgr = *XML::Xercesc::XMLPlatformUtils_makeAtomicOpMgr;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLPlatformUtils($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLURL ##############

package XML::Xerces::XMLURL;
use overload
    "!=" => sub { $_[0]->__ne__($_[1])},
    "==" => sub { $_[0]->__eq__($_[1])},
    "fallback" => 1;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*File = *XML::Xercesc::XMLURL_File;
*HTTP = *XML::Xercesc::XMLURL_HTTP;
*FTP = *XML::Xercesc::XMLURL_FTP;
*Protocols_Count = *XML::Xercesc::XMLURL_Protocols_Count;
*Unknown = *XML::Xercesc::XMLURL_Unknown;
*lookupByName = *XML::Xercesc::XMLURL_lookupByName;
*parse = *XML::Xercesc::XMLURL_parse;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_XMLURL(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLURL($self);
        delete $OWNER{$self};
    }
}

*__eq__ = *XML::Xercesc::XMLURL___eq__;
*__ne__ = *XML::Xercesc::XMLURL___ne__;
*getFragment = *XML::Xercesc::XMLURL_getFragment;
*getHost = *XML::Xercesc::XMLURL_getHost;
*getPassword = *XML::Xercesc::XMLURL_getPassword;
*getPath = *XML::Xercesc::XMLURL_getPath;
*getPortNum = *XML::Xercesc::XMLURL_getPortNum;
*getProtocol = *XML::Xercesc::XMLURL_getProtocol;
*getProtocolName = *XML::Xercesc::XMLURL_getProtocolName;
*getQuery = *XML::Xercesc::XMLURL_getQuery;
*getURLText = *XML::Xercesc::XMLURL_getURLText;
*getUser = *XML::Xercesc::XMLURL_getUser;
*getMemoryManager = *XML::Xercesc::XMLURL_getMemoryManager;
*setURL = *XML::Xercesc::XMLURL_setURL;
*isRelative = *XML::Xercesc::XMLURL_isRelative;
*hasInvalidChar = *XML::Xercesc::XMLURL_hasInvalidChar;
*makeNewStream = *XML::Xercesc::XMLURL_makeNewStream;
*makeRelativeTo = *XML::Xercesc::XMLURL_makeRelativeTo;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLUri ##############

package XML::Xerces::XMLUri;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLUri($self);
        delete $OWNER{$self};
    }
}

*getUriText = *XML::Xercesc::XMLUri_getUriText;
*getScheme = *XML::Xercesc::XMLUri_getScheme;
*getUserInfo = *XML::Xercesc::XMLUri_getUserInfo;
*getHost = *XML::Xercesc::XMLUri_getHost;
*getPort = *XML::Xercesc::XMLUri_getPort;
*getRegBasedAuthority = *XML::Xercesc::XMLUri_getRegBasedAuthority;
*getPath = *XML::Xercesc::XMLUri_getPath;
*getQueryString = *XML::Xercesc::XMLUri_getQueryString;
*getFragment = *XML::Xercesc::XMLUri_getFragment;
*setScheme = *XML::Xercesc::XMLUri_setScheme;
*setUserInfo = *XML::Xercesc::XMLUri_setUserInfo;
*setHost = *XML::Xercesc::XMLUri_setHost;
*setPort = *XML::Xercesc::XMLUri_setPort;
*setRegBasedAuthority = *XML::Xercesc::XMLUri_setRegBasedAuthority;
*setPath = *XML::Xercesc::XMLUri_setPath;
*setQueryString = *XML::Xercesc::XMLUri_setQueryString;
*setFragment = *XML::Xercesc::XMLUri_setFragment;
*isURIString = *XML::Xercesc::XMLUri_isURIString;
*isValidURI = *XML::Xercesc::XMLUri_isValidURI;
*normalizeURI = *XML::Xercesc::XMLUri_normalizeURI;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_XMLUri(@_);
    bless $self, $pkg if defined($self);
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLExcepts ##############

package XML::Xerces::XMLExcepts;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*NoError = *XML::Xercesc::XMLExcepts_NoError;
*W_LowBounds = *XML::Xercesc::XMLExcepts_W_LowBounds;
*Scan_CouldNotOpenSource_Warning = *XML::Xercesc::XMLExcepts_Scan_CouldNotOpenSource_Warning;
*GC_ExistingGrammar = *XML::Xercesc::XMLExcepts_GC_ExistingGrammar;
*W_HighBounds = *XML::Xercesc::XMLExcepts_W_HighBounds;
*F_LowBounds = *XML::Xercesc::XMLExcepts_F_LowBounds;
*Array_BadIndex = *XML::Xercesc::XMLExcepts_Array_BadIndex;
*Array_BadNewSize = *XML::Xercesc::XMLExcepts_Array_BadNewSize;
*AttrList_BadIndex = *XML::Xercesc::XMLExcepts_AttrList_BadIndex;
*AttDef_BadAttType = *XML::Xercesc::XMLExcepts_AttDef_BadAttType;
*AttDef_BadDefAttType = *XML::Xercesc::XMLExcepts_AttDef_BadDefAttType;
*Bitset_BadIndex = *XML::Xercesc::XMLExcepts_Bitset_BadIndex;
*Bitset_NotEqualSize = *XML::Xercesc::XMLExcepts_Bitset_NotEqualSize;
*BufMgr_NoMoreBuffers = *XML::Xercesc::XMLExcepts_BufMgr_NoMoreBuffers;
*BufMgr_BufferNotInPool = *XML::Xercesc::XMLExcepts_BufMgr_BufferNotInPool;
*CPtr_PointerIsZero = *XML::Xercesc::XMLExcepts_CPtr_PointerIsZero;
*CM_BinOpHadUnaryType = *XML::Xercesc::XMLExcepts_CM_BinOpHadUnaryType;
*CM_MustBeMixedOrChildren = *XML::Xercesc::XMLExcepts_CM_MustBeMixedOrChildren;
*CM_NoPCDATAHere = *XML::Xercesc::XMLExcepts_CM_NoPCDATAHere;
*CM_NotValidForSpecType = *XML::Xercesc::XMLExcepts_CM_NotValidForSpecType;
*CM_UnaryOpHadBinType = *XML::Xercesc::XMLExcepts_CM_UnaryOpHadBinType;
*CM_UnknownCMType = *XML::Xercesc::XMLExcepts_CM_UnknownCMType;
*CM_UnknownCMSpecType = *XML::Xercesc::XMLExcepts_CM_UnknownCMSpecType;
*CM_NoParentCSN = *XML::Xercesc::XMLExcepts_CM_NoParentCSN;
*CM_NotValidSpecTypeForNode = *XML::Xercesc::XMLExcepts_CM_NotValidSpecTypeForNode;
*DTD_UnknownCreateReason = *XML::Xercesc::XMLExcepts_DTD_UnknownCreateReason;
*ElemStack_EmptyStack = *XML::Xercesc::XMLExcepts_ElemStack_EmptyStack;
*ElemStack_BadIndex = *XML::Xercesc::XMLExcepts_ElemStack_BadIndex;
*ElemStack_StackUnderflow = *XML::Xercesc::XMLExcepts_ElemStack_StackUnderflow;
*ElemStack_NoParentPushed = *XML::Xercesc::XMLExcepts_ElemStack_NoParentPushed;
*Enum_NoMoreElements = *XML::Xercesc::XMLExcepts_Enum_NoMoreElements;
*File_CouldNotOpenFile = *XML::Xercesc::XMLExcepts_File_CouldNotOpenFile;
*File_CouldNotGetCurPos = *XML::Xercesc::XMLExcepts_File_CouldNotGetCurPos;
*File_CouldNotCloseFile = *XML::Xercesc::XMLExcepts_File_CouldNotCloseFile;
*File_CouldNotSeekToEnd = *XML::Xercesc::XMLExcepts_File_CouldNotSeekToEnd;
*File_CouldNotSeekToPos = *XML::Xercesc::XMLExcepts_File_CouldNotSeekToPos;
*File_CouldNotDupHandle = *XML::Xercesc::XMLExcepts_File_CouldNotDupHandle;
*File_CouldNotReadFromFile = *XML::Xercesc::XMLExcepts_File_CouldNotReadFromFile;
*File_CouldNotWriteToFile = *XML::Xercesc::XMLExcepts_File_CouldNotWriteToFile;
*File_CouldNotResetFile = *XML::Xercesc::XMLExcepts_File_CouldNotResetFile;
*File_CouldNotGetSize = *XML::Xercesc::XMLExcepts_File_CouldNotGetSize;
*File_CouldNotGetBasePathName = *XML::Xercesc::XMLExcepts_File_CouldNotGetBasePathName;
*File_BasePathUnderflow = *XML::Xercesc::XMLExcepts_File_BasePathUnderflow;
*Gen_ParseInProgress = *XML::Xercesc::XMLExcepts_Gen_ParseInProgress;
*Gen_NoDTDValidator = *XML::Xercesc::XMLExcepts_Gen_NoDTDValidator;
*Gen_CouldNotOpenDTD = *XML::Xercesc::XMLExcepts_Gen_CouldNotOpenDTD;
*Gen_CouldNotOpenExtEntity = *XML::Xercesc::XMLExcepts_Gen_CouldNotOpenExtEntity;
*Gen_UnexpectedEOF = *XML::Xercesc::XMLExcepts_Gen_UnexpectedEOF;
*HshTbl_ZeroModulus = *XML::Xercesc::XMLExcepts_HshTbl_ZeroModulus;
*HshTbl_BadHashFromKey = *XML::Xercesc::XMLExcepts_HshTbl_BadHashFromKey;
*HshTbl_NoSuchKeyExists = *XML::Xercesc::XMLExcepts_HshTbl_NoSuchKeyExists;
*Mutex_CouldNotCreate = *XML::Xercesc::XMLExcepts_Mutex_CouldNotCreate;
*Mutex_CouldNotClose = *XML::Xercesc::XMLExcepts_Mutex_CouldNotClose;
*Mutex_CouldNotLock = *XML::Xercesc::XMLExcepts_Mutex_CouldNotLock;
*Mutex_CouldNotUnlock = *XML::Xercesc::XMLExcepts_Mutex_CouldNotUnlock;
*Mutex_CouldNotDestroy = *XML::Xercesc::XMLExcepts_Mutex_CouldNotDestroy;
*NetAcc_InternalError = *XML::Xercesc::XMLExcepts_NetAcc_InternalError;
*NetAcc_LengthError = *XML::Xercesc::XMLExcepts_NetAcc_LengthError;
*NetAcc_InitFailed = *XML::Xercesc::XMLExcepts_NetAcc_InitFailed;
*NetAcc_TargetResolution = *XML::Xercesc::XMLExcepts_NetAcc_TargetResolution;
*NetAcc_CreateSocket = *XML::Xercesc::XMLExcepts_NetAcc_CreateSocket;
*NetAcc_ConnSocket = *XML::Xercesc::XMLExcepts_NetAcc_ConnSocket;
*NetAcc_WriteSocket = *XML::Xercesc::XMLExcepts_NetAcc_WriteSocket;
*NetAcc_ReadSocket = *XML::Xercesc::XMLExcepts_NetAcc_ReadSocket;
*NetAcc_UnsupportedMethod = *XML::Xercesc::XMLExcepts_NetAcc_UnsupportedMethod;
*Pool_ElemAlreadyExists = *XML::Xercesc::XMLExcepts_Pool_ElemAlreadyExists;
*Pool_BadHashFromKey = *XML::Xercesc::XMLExcepts_Pool_BadHashFromKey;
*Pool_InvalidId = *XML::Xercesc::XMLExcepts_Pool_InvalidId;
*Pool_ZeroModulus = *XML::Xercesc::XMLExcepts_Pool_ZeroModulus;
*RdrMgr_ReaderIdNotFound = *XML::Xercesc::XMLExcepts_RdrMgr_ReaderIdNotFound;
*Reader_BadAutoEncoding = *XML::Xercesc::XMLExcepts_Reader_BadAutoEncoding;
*Reader_CouldNotDecodeFirstLine = *XML::Xercesc::XMLExcepts_Reader_CouldNotDecodeFirstLine;
*Reader_NelLsepinDecl = *XML::Xercesc::XMLExcepts_Reader_NelLsepinDecl;
*Reader_EOIInMultiSeq = *XML::Xercesc::XMLExcepts_Reader_EOIInMultiSeq;
*Reader_SrcOfsNotSupported = *XML::Xercesc::XMLExcepts_Reader_SrcOfsNotSupported;
*Reader_EncodingStrRequired = *XML::Xercesc::XMLExcepts_Reader_EncodingStrRequired;
*Scan_CouldNotOpenSource = *XML::Xercesc::XMLExcepts_Scan_CouldNotOpenSource;
*Scan_UnbalancedStartEnd = *XML::Xercesc::XMLExcepts_Scan_UnbalancedStartEnd;
*Scan_BadPScanToken = *XML::Xercesc::XMLExcepts_Scan_BadPScanToken;
*Stack_BadIndex = *XML::Xercesc::XMLExcepts_Stack_BadIndex;
*Stack_EmptyStack = *XML::Xercesc::XMLExcepts_Stack_EmptyStack;
*Str_ZeroSizedTargetBuf = *XML::Xercesc::XMLExcepts_Str_ZeroSizedTargetBuf;
*Str_UnknownRadix = *XML::Xercesc::XMLExcepts_Str_UnknownRadix;
*Str_TargetBufTooSmall = *XML::Xercesc::XMLExcepts_Str_TargetBufTooSmall;
*Str_StartIndexPastEnd = *XML::Xercesc::XMLExcepts_Str_StartIndexPastEnd;
*Str_ConvertOverflow = *XML::Xercesc::XMLExcepts_Str_ConvertOverflow;
*Strm_StdErrWriteFailure = *XML::Xercesc::XMLExcepts_Strm_StdErrWriteFailure;
*Strm_StdOutWriteFailure = *XML::Xercesc::XMLExcepts_Strm_StdOutWriteFailure;
*Strm_ConWriteFailure = *XML::Xercesc::XMLExcepts_Strm_ConWriteFailure;
*StrPool_IllegalId = *XML::Xercesc::XMLExcepts_StrPool_IllegalId;
*Trans_CouldNotCreateDefCvtr = *XML::Xercesc::XMLExcepts_Trans_CouldNotCreateDefCvtr;
*Trans_InvalidSizeReq = *XML::Xercesc::XMLExcepts_Trans_InvalidSizeReq;
*Trans_Unrepresentable = *XML::Xercesc::XMLExcepts_Trans_Unrepresentable;
*Trans_NotValidForEncoding = *XML::Xercesc::XMLExcepts_Trans_NotValidForEncoding;
*Trans_BadBlockSize = *XML::Xercesc::XMLExcepts_Trans_BadBlockSize;
*Trans_BadSrcSeq = *XML::Xercesc::XMLExcepts_Trans_BadSrcSeq;
*Trans_BadSrcCP = *XML::Xercesc::XMLExcepts_Trans_BadSrcCP;
*Trans_BadTrailingSurrogate = *XML::Xercesc::XMLExcepts_Trans_BadTrailingSurrogate;
*Trans_CantCreateCvtrFor = *XML::Xercesc::XMLExcepts_Trans_CantCreateCvtrFor;
*URL_MalformedURL = *XML::Xercesc::XMLExcepts_URL_MalformedURL;
*URL_UnsupportedProto = *XML::Xercesc::XMLExcepts_URL_UnsupportedProto;
*URL_UnsupportedProto1 = *XML::Xercesc::XMLExcepts_URL_UnsupportedProto1;
*URL_OnlyLocalHost = *XML::Xercesc::XMLExcepts_URL_OnlyLocalHost;
*URL_NoProtocolPresent = *XML::Xercesc::XMLExcepts_URL_NoProtocolPresent;
*URL_ExpectingTwoSlashes = *XML::Xercesc::XMLExcepts_URL_ExpectingTwoSlashes;
*URL_IncorrectEscapedCharRef = *XML::Xercesc::XMLExcepts_URL_IncorrectEscapedCharRef;
*URL_UnterminatedHostComponent = *XML::Xercesc::XMLExcepts_URL_UnterminatedHostComponent;
*URL_RelativeBaseURL = *XML::Xercesc::XMLExcepts_URL_RelativeBaseURL;
*URL_BaseUnderflow = *XML::Xercesc::XMLExcepts_URL_BaseUnderflow;
*URL_BadPortField = *XML::Xercesc::XMLExcepts_URL_BadPortField;
*UTF8_FormatError = *XML::Xercesc::XMLExcepts_UTF8_FormatError;
*UTF8_Invalid_2BytesSeq = *XML::Xercesc::XMLExcepts_UTF8_Invalid_2BytesSeq;
*UTF8_Invalid_3BytesSeq = *XML::Xercesc::XMLExcepts_UTF8_Invalid_3BytesSeq;
*UTF8_Irregular_3BytesSeq = *XML::Xercesc::XMLExcepts_UTF8_Irregular_3BytesSeq;
*UTF8_Invalid_4BytesSeq = *XML::Xercesc::XMLExcepts_UTF8_Invalid_4BytesSeq;
*UTF8_Exceede_BytesLimit = *XML::Xercesc::XMLExcepts_UTF8_Exceede_BytesLimit;
*Vector_BadIndex = *XML::Xercesc::XMLExcepts_Vector_BadIndex;
*Val_InvalidElemId = *XML::Xercesc::XMLExcepts_Val_InvalidElemId;
*Val_CantHaveIntSS = *XML::Xercesc::XMLExcepts_Val_CantHaveIntSS;
*XMLRec_UnknownEncoding = *XML::Xercesc::XMLExcepts_XMLRec_UnknownEncoding;
*Parser_Parse1 = *XML::Xercesc::XMLExcepts_Parser_Parse1;
*Parser_Parse2 = *XML::Xercesc::XMLExcepts_Parser_Parse2;
*Parser_Next1 = *XML::Xercesc::XMLExcepts_Parser_Next1;
*Parser_Next2 = *XML::Xercesc::XMLExcepts_Parser_Next2;
*Parser_Next3 = *XML::Xercesc::XMLExcepts_Parser_Next3;
*Parser_Next4 = *XML::Xercesc::XMLExcepts_Parser_Next4;
*Parser_Factor1 = *XML::Xercesc::XMLExcepts_Parser_Factor1;
*Parser_Factor2 = *XML::Xercesc::XMLExcepts_Parser_Factor2;
*Parser_Factor3 = *XML::Xercesc::XMLExcepts_Parser_Factor3;
*Parser_Factor4 = *XML::Xercesc::XMLExcepts_Parser_Factor4;
*Parser_Factor5 = *XML::Xercesc::XMLExcepts_Parser_Factor5;
*Parser_Factor6 = *XML::Xercesc::XMLExcepts_Parser_Factor6;
*Parser_Atom1 = *XML::Xercesc::XMLExcepts_Parser_Atom1;
*Parser_Atom2 = *XML::Xercesc::XMLExcepts_Parser_Atom2;
*Parser_Atom3 = *XML::Xercesc::XMLExcepts_Parser_Atom3;
*Parser_Atom4 = *XML::Xercesc::XMLExcepts_Parser_Atom4;
*Parser_Atom5 = *XML::Xercesc::XMLExcepts_Parser_Atom5;
*Parser_CC1 = *XML::Xercesc::XMLExcepts_Parser_CC1;
*Parser_CC2 = *XML::Xercesc::XMLExcepts_Parser_CC2;
*Parser_CC3 = *XML::Xercesc::XMLExcepts_Parser_CC3;
*Parser_CC4 = *XML::Xercesc::XMLExcepts_Parser_CC4;
*Parser_CC5 = *XML::Xercesc::XMLExcepts_Parser_CC5;
*Parser_CC6 = *XML::Xercesc::XMLExcepts_Parser_CC6;
*Parser_Ope1 = *XML::Xercesc::XMLExcepts_Parser_Ope1;
*Parser_Ope2 = *XML::Xercesc::XMLExcepts_Parser_Ope2;
*Parser_Ope3 = *XML::Xercesc::XMLExcepts_Parser_Ope3;
*Parser_Descape1 = *XML::Xercesc::XMLExcepts_Parser_Descape1;
*Parser_Descape2 = *XML::Xercesc::XMLExcepts_Parser_Descape2;
*Parser_Descape3 = *XML::Xercesc::XMLExcepts_Parser_Descape3;
*Parser_Descape4 = *XML::Xercesc::XMLExcepts_Parser_Descape4;
*Parser_Descape5 = *XML::Xercesc::XMLExcepts_Parser_Descape5;
*Parser_Process1 = *XML::Xercesc::XMLExcepts_Parser_Process1;
*Parser_Process2 = *XML::Xercesc::XMLExcepts_Parser_Process2;
*Parser_Quantifier1 = *XML::Xercesc::XMLExcepts_Parser_Quantifier1;
*Parser_Quantifier2 = *XML::Xercesc::XMLExcepts_Parser_Quantifier2;
*Parser_Quantifier3 = *XML::Xercesc::XMLExcepts_Parser_Quantifier3;
*Parser_Quantifier4 = *XML::Xercesc::XMLExcepts_Parser_Quantifier4;
*Parser_Quantifier5 = *XML::Xercesc::XMLExcepts_Parser_Quantifier5;
*Gen_NoSchemaValidator = *XML::Xercesc::XMLExcepts_Gen_NoSchemaValidator;
*XUTIL_UnCopyableNodeType = *XML::Xercesc::XMLExcepts_XUTIL_UnCopyableNodeType;
*SubGrpComparator_NGR = *XML::Xercesc::XMLExcepts_SubGrpComparator_NGR;
*FACET_Invalid_Len = *XML::Xercesc::XMLExcepts_FACET_Invalid_Len;
*FACET_Invalid_maxLen = *XML::Xercesc::XMLExcepts_FACET_Invalid_maxLen;
*FACET_Invalid_minLen = *XML::Xercesc::XMLExcepts_FACET_Invalid_minLen;
*FACET_NonNeg_Len = *XML::Xercesc::XMLExcepts_FACET_NonNeg_Len;
*FACET_NonNeg_maxLen = *XML::Xercesc::XMLExcepts_FACET_NonNeg_maxLen;
*FACET_NonNeg_minLen = *XML::Xercesc::XMLExcepts_FACET_NonNeg_minLen;
*FACET_Len_maxLen = *XML::Xercesc::XMLExcepts_FACET_Len_maxLen;
*FACET_Len_minLen = *XML::Xercesc::XMLExcepts_FACET_Len_minLen;
*FACET_maxLen_minLen = *XML::Xercesc::XMLExcepts_FACET_maxLen_minLen;
*FACET_bool_Pattern = *XML::Xercesc::XMLExcepts_FACET_bool_Pattern;
*FACET_Invalid_Tag = *XML::Xercesc::XMLExcepts_FACET_Invalid_Tag;
*FACET_Len_baseLen = *XML::Xercesc::XMLExcepts_FACET_Len_baseLen;
*FACET_minLen_baseminLen = *XML::Xercesc::XMLExcepts_FACET_minLen_baseminLen;
*FACET_minLen_basemaxLen = *XML::Xercesc::XMLExcepts_FACET_minLen_basemaxLen;
*FACET_maxLen_basemaxLen = *XML::Xercesc::XMLExcepts_FACET_maxLen_basemaxLen;
*FACET_maxLen_baseminLen = *XML::Xercesc::XMLExcepts_FACET_maxLen_baseminLen;
*FACET_Len_baseMinLen = *XML::Xercesc::XMLExcepts_FACET_Len_baseMinLen;
*FACET_Len_baseMaxLen = *XML::Xercesc::XMLExcepts_FACET_Len_baseMaxLen;
*FACET_minLen_baseLen = *XML::Xercesc::XMLExcepts_FACET_minLen_baseLen;
*FACET_maxLen_baseLen = *XML::Xercesc::XMLExcepts_FACET_maxLen_baseLen;
*FACET_enum_base = *XML::Xercesc::XMLExcepts_FACET_enum_base;
*FACET_Invalid_WS = *XML::Xercesc::XMLExcepts_FACET_Invalid_WS;
*FACET_WS_collapse = *XML::Xercesc::XMLExcepts_FACET_WS_collapse;
*FACET_WS_replace = *XML::Xercesc::XMLExcepts_FACET_WS_replace;
*FACET_Invalid_MaxIncl = *XML::Xercesc::XMLExcepts_FACET_Invalid_MaxIncl;
*FACET_Invalid_MaxExcl = *XML::Xercesc::XMLExcepts_FACET_Invalid_MaxExcl;
*FACET_Invalid_MinIncl = *XML::Xercesc::XMLExcepts_FACET_Invalid_MinIncl;
*FACET_Invalid_MinExcl = *XML::Xercesc::XMLExcepts_FACET_Invalid_MinExcl;
*FACET_Invalid_TotalDigit = *XML::Xercesc::XMLExcepts_FACET_Invalid_TotalDigit;
*FACET_Invalid_FractDigit = *XML::Xercesc::XMLExcepts_FACET_Invalid_FractDigit;
*FACET_PosInt_TotalDigit = *XML::Xercesc::XMLExcepts_FACET_PosInt_TotalDigit;
*FACET_NonNeg_FractDigit = *XML::Xercesc::XMLExcepts_FACET_NonNeg_FractDigit;
*FACET_max_Incl_Excl = *XML::Xercesc::XMLExcepts_FACET_max_Incl_Excl;
*FACET_min_Incl_Excl = *XML::Xercesc::XMLExcepts_FACET_min_Incl_Excl;
*FACET_maxExcl_minExcl = *XML::Xercesc::XMLExcepts_FACET_maxExcl_minExcl;
*FACET_maxExcl_minIncl = *XML::Xercesc::XMLExcepts_FACET_maxExcl_minIncl;
*FACET_maxIncl_minExcl = *XML::Xercesc::XMLExcepts_FACET_maxIncl_minExcl;
*FACET_maxIncl_minIncl = *XML::Xercesc::XMLExcepts_FACET_maxIncl_minIncl;
*FACET_TotDigit_FractDigit = *XML::Xercesc::XMLExcepts_FACET_TotDigit_FractDigit;
*FACET_maxIncl_base_maxExcl = *XML::Xercesc::XMLExcepts_FACET_maxIncl_base_maxExcl;
*FACET_maxIncl_base_maxIncl = *XML::Xercesc::XMLExcepts_FACET_maxIncl_base_maxIncl;
*FACET_maxIncl_base_minIncl = *XML::Xercesc::XMLExcepts_FACET_maxIncl_base_minIncl;
*FACET_maxIncl_base_minExcl = *XML::Xercesc::XMLExcepts_FACET_maxIncl_base_minExcl;
*FACET_maxExcl_base_maxExcl = *XML::Xercesc::XMLExcepts_FACET_maxExcl_base_maxExcl;
*FACET_maxExcl_base_maxIncl = *XML::Xercesc::XMLExcepts_FACET_maxExcl_base_maxIncl;
*FACET_maxExcl_base_minIncl = *XML::Xercesc::XMLExcepts_FACET_maxExcl_base_minIncl;
*FACET_maxExcl_base_minExcl = *XML::Xercesc::XMLExcepts_FACET_maxExcl_base_minExcl;
*FACET_minExcl_base_maxExcl = *XML::Xercesc::XMLExcepts_FACET_minExcl_base_maxExcl;
*FACET_minExcl_base_maxIncl = *XML::Xercesc::XMLExcepts_FACET_minExcl_base_maxIncl;
*FACET_minExcl_base_minIncl = *XML::Xercesc::XMLExcepts_FACET_minExcl_base_minIncl;
*FACET_minExcl_base_minExcl = *XML::Xercesc::XMLExcepts_FACET_minExcl_base_minExcl;
*FACET_minIncl_base_maxExcl = *XML::Xercesc::XMLExcepts_FACET_minIncl_base_maxExcl;
*FACET_minIncl_base_maxIncl = *XML::Xercesc::XMLExcepts_FACET_minIncl_base_maxIncl;
*FACET_minIncl_base_minIncl = *XML::Xercesc::XMLExcepts_FACET_minIncl_base_minIncl;
*FACET_minIncl_base_minExcl = *XML::Xercesc::XMLExcepts_FACET_minIncl_base_minExcl;
*FACET_maxIncl_notFromBase = *XML::Xercesc::XMLExcepts_FACET_maxIncl_notFromBase;
*FACET_maxExcl_notFromBase = *XML::Xercesc::XMLExcepts_FACET_maxExcl_notFromBase;
*FACET_minIncl_notFromBase = *XML::Xercesc::XMLExcepts_FACET_minIncl_notFromBase;
*FACET_minExcl_notFromBase = *XML::Xercesc::XMLExcepts_FACET_minExcl_notFromBase;
*FACET_totalDigit_base_totalDigit = *XML::Xercesc::XMLExcepts_FACET_totalDigit_base_totalDigit;
*FACET_fractDigit_base_totalDigit = *XML::Xercesc::XMLExcepts_FACET_fractDigit_base_totalDigit;
*FACET_fractDigit_base_fractDigit = *XML::Xercesc::XMLExcepts_FACET_fractDigit_base_fractDigit;
*FACET_maxIncl_base_fixed = *XML::Xercesc::XMLExcepts_FACET_maxIncl_base_fixed;
*FACET_maxExcl_base_fixed = *XML::Xercesc::XMLExcepts_FACET_maxExcl_base_fixed;
*FACET_minIncl_base_fixed = *XML::Xercesc::XMLExcepts_FACET_minIncl_base_fixed;
*FACET_minExcl_base_fixed = *XML::Xercesc::XMLExcepts_FACET_minExcl_base_fixed;
*FACET_totalDigit_base_fixed = *XML::Xercesc::XMLExcepts_FACET_totalDigit_base_fixed;
*FACET_fractDigit_base_fixed = *XML::Xercesc::XMLExcepts_FACET_fractDigit_base_fixed;
*FACET_maxLen_base_fixed = *XML::Xercesc::XMLExcepts_FACET_maxLen_base_fixed;
*FACET_minLen_base_fixed = *XML::Xercesc::XMLExcepts_FACET_minLen_base_fixed;
*FACET_len_base_fixed = *XML::Xercesc::XMLExcepts_FACET_len_base_fixed;
*FACET_whitespace_base_fixed = *XML::Xercesc::XMLExcepts_FACET_whitespace_base_fixed;
*FACET_internalError_fixed = *XML::Xercesc::XMLExcepts_FACET_internalError_fixed;
*FACET_List_Null_baseValidator = *XML::Xercesc::XMLExcepts_FACET_List_Null_baseValidator;
*FACET_Union_Null_memberTypeValidators = *XML::Xercesc::XMLExcepts_FACET_Union_Null_memberTypeValidators;
*FACET_Union_Null_baseValidator = *XML::Xercesc::XMLExcepts_FACET_Union_Null_baseValidator;
*FACET_Union_invalid_baseValidatorType = *XML::Xercesc::XMLExcepts_FACET_Union_invalid_baseValidatorType;
*VALUE_NotMatch_Pattern = *XML::Xercesc::XMLExcepts_VALUE_NotMatch_Pattern;
*VALUE_Not_Base64 = *XML::Xercesc::XMLExcepts_VALUE_Not_Base64;
*VALUE_Not_HexBin = *XML::Xercesc::XMLExcepts_VALUE_Not_HexBin;
*VALUE_GT_maxLen = *XML::Xercesc::XMLExcepts_VALUE_GT_maxLen;
*VALUE_LT_minLen = *XML::Xercesc::XMLExcepts_VALUE_LT_minLen;
*VALUE_NE_Len = *XML::Xercesc::XMLExcepts_VALUE_NE_Len;
*VALUE_NotIn_Enumeration = *XML::Xercesc::XMLExcepts_VALUE_NotIn_Enumeration;
*VALUE_exceed_totalDigit = *XML::Xercesc::XMLExcepts_VALUE_exceed_totalDigit;
*VALUE_exceed_fractDigit = *XML::Xercesc::XMLExcepts_VALUE_exceed_fractDigit;
*VALUE_exceed_maxIncl = *XML::Xercesc::XMLExcepts_VALUE_exceed_maxIncl;
*VALUE_exceed_maxExcl = *XML::Xercesc::XMLExcepts_VALUE_exceed_maxExcl;
*VALUE_exceed_minIncl = *XML::Xercesc::XMLExcepts_VALUE_exceed_minIncl;
*VALUE_exceed_minExcl = *XML::Xercesc::XMLExcepts_VALUE_exceed_minExcl;
*VALUE_WS_replaced = *XML::Xercesc::XMLExcepts_VALUE_WS_replaced;
*VALUE_WS_collapsed = *XML::Xercesc::XMLExcepts_VALUE_WS_collapsed;
*VALUE_Invalid_NCName = *XML::Xercesc::XMLExcepts_VALUE_Invalid_NCName;
*VALUE_Invalid_Name = *XML::Xercesc::XMLExcepts_VALUE_Invalid_Name;
*VALUE_ID_Not_Unique = *XML::Xercesc::XMLExcepts_VALUE_ID_Not_Unique;
*VALUE_ENTITY_Invalid = *XML::Xercesc::XMLExcepts_VALUE_ENTITY_Invalid;
*VALUE_QName_Invalid = *XML::Xercesc::XMLExcepts_VALUE_QName_Invalid;
*VALUE_NOTATION_Invalid = *XML::Xercesc::XMLExcepts_VALUE_NOTATION_Invalid;
*VALUE_no_match_memberType = *XML::Xercesc::XMLExcepts_VALUE_no_match_memberType;
*VALUE_URI_Malformed = *XML::Xercesc::XMLExcepts_VALUE_URI_Malformed;
*XMLNUM_emptyString = *XML::Xercesc::XMLExcepts_XMLNUM_emptyString;
*XMLNUM_WSString = *XML::Xercesc::XMLExcepts_XMLNUM_WSString;
*XMLNUM_2ManyDecPoint = *XML::Xercesc::XMLExcepts_XMLNUM_2ManyDecPoint;
*XMLNUM_Inv_chars = *XML::Xercesc::XMLExcepts_XMLNUM_Inv_chars;
*XMLNUM_null_ptr = *XML::Xercesc::XMLExcepts_XMLNUM_null_ptr;
*XMLNUM_URI_Component_Empty = *XML::Xercesc::XMLExcepts_XMLNUM_URI_Component_Empty;
*XMLNUM_URI_Component_for_GenURI_Only = *XML::Xercesc::XMLExcepts_XMLNUM_URI_Component_for_GenURI_Only;
*XMLNUM_URI_Component_Invalid_EscapeSequence = *XML::Xercesc::XMLExcepts_XMLNUM_URI_Component_Invalid_EscapeSequence;
*XMLNUM_URI_Component_Invalid_Char = *XML::Xercesc::XMLExcepts_XMLNUM_URI_Component_Invalid_Char;
*XMLNUM_URI_Component_Set_Null = *XML::Xercesc::XMLExcepts_XMLNUM_URI_Component_Set_Null;
*XMLNUM_URI_Component_Not_Conformant = *XML::Xercesc::XMLExcepts_XMLNUM_URI_Component_Not_Conformant;
*XMLNUM_URI_No_Scheme = *XML::Xercesc::XMLExcepts_XMLNUM_URI_No_Scheme;
*XMLNUM_URI_NullHost = *XML::Xercesc::XMLExcepts_XMLNUM_URI_NullHost;
*XMLNUM_URI_NullPath = *XML::Xercesc::XMLExcepts_XMLNUM_URI_NullPath;
*XMLNUM_URI_Component_inPath = *XML::Xercesc::XMLExcepts_XMLNUM_URI_Component_inPath;
*XMLNUM_URI_PortNo_Invalid = *XML::Xercesc::XMLExcepts_XMLNUM_URI_PortNo_Invalid;
*XMLNUM_DBL_FLT_maxNeg = *XML::Xercesc::XMLExcepts_XMLNUM_DBL_FLT_maxNeg;
*XMLNUM_DBL_FLT_maxPos = *XML::Xercesc::XMLExcepts_XMLNUM_DBL_FLT_maxPos;
*XMLNUM_DBL_FLT_minNegPos = *XML::Xercesc::XMLExcepts_XMLNUM_DBL_FLT_minNegPos;
*XMLNUM_DBL_FLT_InvalidType = *XML::Xercesc::XMLExcepts_XMLNUM_DBL_FLT_InvalidType;
*XMLNUM_DBL_FLT_No_Exponent = *XML::Xercesc::XMLExcepts_XMLNUM_DBL_FLT_No_Exponent;
*Regex_Result_Not_Set = *XML::Xercesc::XMLExcepts_Regex_Result_Not_Set;
*Regex_CompactRangesError = *XML::Xercesc::XMLExcepts_Regex_CompactRangesError;
*Regex_MergeRangesTypeMismatch = *XML::Xercesc::XMLExcepts_Regex_MergeRangesTypeMismatch;
*Regex_SubtractRangesError = *XML::Xercesc::XMLExcepts_Regex_SubtractRangesError;
*Regex_IntersectRangesError = *XML::Xercesc::XMLExcepts_Regex_IntersectRangesError;
*Regex_ComplementRangesInvalidArg = *XML::Xercesc::XMLExcepts_Regex_ComplementRangesInvalidArg;
*Regex_InvalidCategoryName = *XML::Xercesc::XMLExcepts_Regex_InvalidCategoryName;
*Regex_KeywordNotFound = *XML::Xercesc::XMLExcepts_Regex_KeywordNotFound;
*Regex_BadRefNo = *XML::Xercesc::XMLExcepts_Regex_BadRefNo;
*Regex_UnknownOption = *XML::Xercesc::XMLExcepts_Regex_UnknownOption;
*Regex_UnknownTokenType = *XML::Xercesc::XMLExcepts_Regex_UnknownTokenType;
*Regex_RangeTokenGetError = *XML::Xercesc::XMLExcepts_Regex_RangeTokenGetError;
*Regex_NotSupported = *XML::Xercesc::XMLExcepts_Regex_NotSupported;
*Regex_InvalidChildIndex = *XML::Xercesc::XMLExcepts_Regex_InvalidChildIndex;
*Regex_RepPatMatchesZeroString = *XML::Xercesc::XMLExcepts_Regex_RepPatMatchesZeroString;
*Regex_InvalidRepPattern = *XML::Xercesc::XMLExcepts_Regex_InvalidRepPattern;
*NEL_RepeatedCalls = *XML::Xercesc::XMLExcepts_NEL_RepeatedCalls;
*RethrowError = *XML::Xercesc::XMLExcepts_RethrowError;
*Out_Of_Memory = *XML::Xercesc::XMLExcepts_Out_Of_Memory;
*DV_InvalidOperation = *XML::Xercesc::XMLExcepts_DV_InvalidOperation;
*XPath_NoAttrSelector = *XML::Xercesc::XMLExcepts_XPath_NoAttrSelector;
*XPath_NoUnionAtStart = *XML::Xercesc::XMLExcepts_XPath_NoUnionAtStart;
*XPath_NoMultipleUnion = *XML::Xercesc::XMLExcepts_XPath_NoMultipleUnion;
*XPath_MissingAttr = *XML::Xercesc::XMLExcepts_XPath_MissingAttr;
*XPath_ExpectedToken1 = *XML::Xercesc::XMLExcepts_XPath_ExpectedToken1;
*XPath_PrefixNoURI = *XML::Xercesc::XMLExcepts_XPath_PrefixNoURI;
*XPath_NoDoubleColon = *XML::Xercesc::XMLExcepts_XPath_NoDoubleColon;
*XPath_ExpectedStep1 = *XML::Xercesc::XMLExcepts_XPath_ExpectedStep1;
*XPath_ExpectedStep2 = *XML::Xercesc::XMLExcepts_XPath_ExpectedStep2;
*XPath_ExpectedStep3 = *XML::Xercesc::XMLExcepts_XPath_ExpectedStep3;
*XPath_NoForwardSlash = *XML::Xercesc::XMLExcepts_XPath_NoForwardSlash;
*XPath_NoDoubleForwardSlash = *XML::Xercesc::XMLExcepts_XPath_NoDoubleForwardSlash;
*XPath_NoForwardSlashAtStart = *XML::Xercesc::XMLExcepts_XPath_NoForwardSlashAtStart;
*XPath_NoSelectionOfRoot = *XML::Xercesc::XMLExcepts_XPath_NoSelectionOfRoot;
*XPath_EmptyExpr = *XML::Xercesc::XMLExcepts_XPath_EmptyExpr;
*XPath_NoUnionAtEnd = *XML::Xercesc::XMLExcepts_XPath_NoUnionAtEnd;
*XPath_InvalidChar = *XML::Xercesc::XMLExcepts_XPath_InvalidChar;
*XPath_TokenNotSupported = *XML::Xercesc::XMLExcepts_XPath_TokenNotSupported;
*XPath_FindSolution = *XML::Xercesc::XMLExcepts_XPath_FindSolution;
*DateTime_Assert_Buffer_Fail = *XML::Xercesc::XMLExcepts_DateTime_Assert_Buffer_Fail;
*DateTime_dt_missingT = *XML::Xercesc::XMLExcepts_DateTime_dt_missingT;
*DateTime_gDay_invalid = *XML::Xercesc::XMLExcepts_DateTime_gDay_invalid;
*DateTime_gMth_invalid = *XML::Xercesc::XMLExcepts_DateTime_gMth_invalid;
*DateTime_gMthDay_invalid = *XML::Xercesc::XMLExcepts_DateTime_gMthDay_invalid;
*DateTime_dur_Start_dashP = *XML::Xercesc::XMLExcepts_DateTime_dur_Start_dashP;
*DateTime_dur_noP = *XML::Xercesc::XMLExcepts_DateTime_dur_noP;
*DateTime_dur_DashNotFirst = *XML::Xercesc::XMLExcepts_DateTime_dur_DashNotFirst;
*DateTime_dur_inv_b4T = *XML::Xercesc::XMLExcepts_DateTime_dur_inv_b4T;
*DateTime_dur_NoTimeAfterT = *XML::Xercesc::XMLExcepts_DateTime_dur_NoTimeAfterT;
*DateTime_dur_NoElementAtAll = *XML::Xercesc::XMLExcepts_DateTime_dur_NoElementAtAll;
*DateTime_dur_inv_seconds = *XML::Xercesc::XMLExcepts_DateTime_dur_inv_seconds;
*DateTime_date_incomplete = *XML::Xercesc::XMLExcepts_DateTime_date_incomplete;
*DateTime_date_invalid = *XML::Xercesc::XMLExcepts_DateTime_date_invalid;
*DateTime_time_incomplete = *XML::Xercesc::XMLExcepts_DateTime_time_incomplete;
*DateTime_time_invalid = *XML::Xercesc::XMLExcepts_DateTime_time_invalid;
*DateTime_ms_noDigit = *XML::Xercesc::XMLExcepts_DateTime_ms_noDigit;
*DateTime_ym_incomplete = *XML::Xercesc::XMLExcepts_DateTime_ym_incomplete;
*DateTime_ym_invalid = *XML::Xercesc::XMLExcepts_DateTime_ym_invalid;
*DateTime_year_tooShort = *XML::Xercesc::XMLExcepts_DateTime_year_tooShort;
*DateTime_year_leadingZero = *XML::Xercesc::XMLExcepts_DateTime_year_leadingZero;
*DateTime_ym_noMonth = *XML::Xercesc::XMLExcepts_DateTime_ym_noMonth;
*DateTime_tz_noUTCsign = *XML::Xercesc::XMLExcepts_DateTime_tz_noUTCsign;
*DateTime_tz_stuffAfterZ = *XML::Xercesc::XMLExcepts_DateTime_tz_stuffAfterZ;
*DateTime_tz_invalid = *XML::Xercesc::XMLExcepts_DateTime_tz_invalid;
*DateTime_year_zero = *XML::Xercesc::XMLExcepts_DateTime_year_zero;
*DateTime_mth_invalid = *XML::Xercesc::XMLExcepts_DateTime_mth_invalid;
*DateTime_day_invalid = *XML::Xercesc::XMLExcepts_DateTime_day_invalid;
*DateTime_hour_invalid = *XML::Xercesc::XMLExcepts_DateTime_hour_invalid;
*DateTime_min_invalid = *XML::Xercesc::XMLExcepts_DateTime_min_invalid;
*DateTime_second_invalid = *XML::Xercesc::XMLExcepts_DateTime_second_invalid;
*DateTime_tz_hh_invalid = *XML::Xercesc::XMLExcepts_DateTime_tz_hh_invalid;
*PD_EmptyBase = *XML::Xercesc::XMLExcepts_PD_EmptyBase;
*PD_NSCompat1 = *XML::Xercesc::XMLExcepts_PD_NSCompat1;
*PD_OccurRangeE = *XML::Xercesc::XMLExcepts_PD_OccurRangeE;
*PD_NameTypeOK1 = *XML::Xercesc::XMLExcepts_PD_NameTypeOK1;
*PD_NameTypeOK2 = *XML::Xercesc::XMLExcepts_PD_NameTypeOK2;
*PD_NameTypeOK3 = *XML::Xercesc::XMLExcepts_PD_NameTypeOK3;
*PD_NameTypeOK4 = *XML::Xercesc::XMLExcepts_PD_NameTypeOK4;
*PD_NameTypeOK5 = *XML::Xercesc::XMLExcepts_PD_NameTypeOK5;
*PD_NameTypeOK6 = *XML::Xercesc::XMLExcepts_PD_NameTypeOK6;
*PD_NameTypeOK7 = *XML::Xercesc::XMLExcepts_PD_NameTypeOK7;
*PD_RecurseAsIfGroup = *XML::Xercesc::XMLExcepts_PD_RecurseAsIfGroup;
*PD_Recurse1 = *XML::Xercesc::XMLExcepts_PD_Recurse1;
*PD_Recurse2 = *XML::Xercesc::XMLExcepts_PD_Recurse2;
*PD_ForbiddenRes1 = *XML::Xercesc::XMLExcepts_PD_ForbiddenRes1;
*PD_ForbiddenRes2 = *XML::Xercesc::XMLExcepts_PD_ForbiddenRes2;
*PD_ForbiddenRes3 = *XML::Xercesc::XMLExcepts_PD_ForbiddenRes3;
*PD_ForbiddenRes4 = *XML::Xercesc::XMLExcepts_PD_ForbiddenRes4;
*PD_NSSubset1 = *XML::Xercesc::XMLExcepts_PD_NSSubset1;
*PD_NSSubset2 = *XML::Xercesc::XMLExcepts_PD_NSSubset2;
*PD_NSRecurseCheckCardinality1 = *XML::Xercesc::XMLExcepts_PD_NSRecurseCheckCardinality1;
*PD_RecurseUnordered = *XML::Xercesc::XMLExcepts_PD_RecurseUnordered;
*PD_MapAndSum = *XML::Xercesc::XMLExcepts_PD_MapAndSum;
*PD_InvalidContentType = *XML::Xercesc::XMLExcepts_PD_InvalidContentType;
*NodeIDMap_GrowErr = *XML::Xercesc::XMLExcepts_NodeIDMap_GrowErr;
*XSer_ProtoType_Null_ClassName = *XML::Xercesc::XMLExcepts_XSer_ProtoType_Null_ClassName;
*XSer_ProtoType_NameLen_Dif = *XML::Xercesc::XMLExcepts_XSer_ProtoType_NameLen_Dif;
*XSer_ProtoType_Name_Dif = *XML::Xercesc::XMLExcepts_XSer_ProtoType_Name_Dif;
*XSer_InStream_Read_LT_Req = *XML::Xercesc::XMLExcepts_XSer_InStream_Read_LT_Req;
*XSer_InStream_Read_OverFlow = *XML::Xercesc::XMLExcepts_XSer_InStream_Read_OverFlow;
*XSer_Storing_Violation = *XML::Xercesc::XMLExcepts_XSer_Storing_Violation;
*XSer_StoreBuffer_Violation = *XML::Xercesc::XMLExcepts_XSer_StoreBuffer_Violation;
*XSer_LoadPool_UppBnd_Exceed = *XML::Xercesc::XMLExcepts_XSer_LoadPool_UppBnd_Exceed;
*XSer_LoadPool_NoTally_ObjCnt = *XML::Xercesc::XMLExcepts_XSer_LoadPool_NoTally_ObjCnt;
*XSer_Loading_Violation = *XML::Xercesc::XMLExcepts_XSer_Loading_Violation;
*XSer_LoadBuffer_Violation = *XML::Xercesc::XMLExcepts_XSer_LoadBuffer_Violation;
*XSer_Inv_ClassIndex = *XML::Xercesc::XMLExcepts_XSer_Inv_ClassIndex;
*XSer_Inv_FillBuffer_Size = *XML::Xercesc::XMLExcepts_XSer_Inv_FillBuffer_Size;
*XSer_Inv_checkFillBuffer_Size = *XML::Xercesc::XMLExcepts_XSer_Inv_checkFillBuffer_Size;
*XSer_Inv_checkFlushBuffer_Size = *XML::Xercesc::XMLExcepts_XSer_Inv_checkFlushBuffer_Size;
*XSer_Inv_Null_Pointer = *XML::Xercesc::XMLExcepts_XSer_Inv_Null_Pointer;
*XSer_Inv_Buffer_Len = *XML::Xercesc::XMLExcepts_XSer_Inv_Buffer_Len;
*XSer_CreateObject_Fail = *XML::Xercesc::XMLExcepts_XSer_CreateObject_Fail;
*XSer_ObjCount_UppBnd_Exceed = *XML::Xercesc::XMLExcepts_XSer_ObjCount_UppBnd_Exceed;
*XSer_GrammarPool_Locked = *XML::Xercesc::XMLExcepts_XSer_GrammarPool_Locked;
*XSer_GrammarPool_Empty = *XML::Xercesc::XMLExcepts_XSer_GrammarPool_Empty;
*XSer_GrammarPool_NotEmpty = *XML::Xercesc::XMLExcepts_XSer_GrammarPool_NotEmpty;
*XSer_StringPool_NotEmpty = *XML::Xercesc::XMLExcepts_XSer_StringPool_NotEmpty;
*XSer_Storer_NewerThan_Loader = *XML::Xercesc::XMLExcepts_XSer_Storer_NewerThan_Loader;
*VALUE_QName_Invalid2 = *XML::Xercesc::XMLExcepts_VALUE_QName_Invalid2;
*F_HighBounds = *XML::Xercesc::XMLExcepts_F_HighBounds;
*E_LowBounds = *XML::Xercesc::XMLExcepts_E_LowBounds;
*E_HighBounds = *XML::Xercesc::XMLExcepts_E_HighBounds;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLExcepts($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLException ##############

package XML::Xerces::XMLException;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLException($self);
        delete $OWNER{$self};
    }
}

*getType = *XML::Xercesc::XMLException_getType;
*getCode = *XML::Xercesc::XMLException_getCode;
*getMessage = *XML::Xercesc::XMLException_getMessage;
*getSrcFile = *XML::Xercesc::XMLException_getSrcFile;
*getSrcLine = *XML::Xercesc::XMLException_getSrcLine;
*getErrorType = *XML::Xercesc::XMLException_getErrorType;
*setPosition = *XML::Xercesc::XMLException_setPosition;
*reinitMsgMutex = *XML::Xercesc::XMLException_reinitMsgMutex;
*reinitMsgLoader = *XML::Xercesc::XMLException_reinitMsgLoader;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLUni ##############

package XML::Xerces::XMLUni;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*fgAnyString = *XML::Xercesc::XMLUni_fgAnyString;
*fgAttListString = *XML::Xercesc::XMLUni_fgAttListString;
*fgCommentString = *XML::Xercesc::XMLUni_fgCommentString;
*fgCDATAString = *XML::Xercesc::XMLUni_fgCDATAString;
*fgDefaultString = *XML::Xercesc::XMLUni_fgDefaultString;
*fgDocTypeString = *XML::Xercesc::XMLUni_fgDocTypeString;
*fgEBCDICEncodingString = *XML::Xercesc::XMLUni_fgEBCDICEncodingString;
*fgElemString = *XML::Xercesc::XMLUni_fgElemString;
*fgEmptyString = *XML::Xercesc::XMLUni_fgEmptyString;
*fgEncodingString = *XML::Xercesc::XMLUni_fgEncodingString;
*fgEntitString = *XML::Xercesc::XMLUni_fgEntitString;
*fgEntityString = *XML::Xercesc::XMLUni_fgEntityString;
*fgEntitiesString = *XML::Xercesc::XMLUni_fgEntitiesString;
*fgEnumerationString = *XML::Xercesc::XMLUni_fgEnumerationString;
*fgExceptDomain = *XML::Xercesc::XMLUni_fgExceptDomain;
*fgFixedString = *XML::Xercesc::XMLUni_fgFixedString;
*fgIBM037EncodingString = *XML::Xercesc::XMLUni_fgIBM037EncodingString;
*fgIBM037EncodingString2 = *XML::Xercesc::XMLUni_fgIBM037EncodingString2;
*fgIBM1047EncodingString = *XML::Xercesc::XMLUni_fgIBM1047EncodingString;
*fgIBM1047EncodingString2 = *XML::Xercesc::XMLUni_fgIBM1047EncodingString2;
*fgIBM1140EncodingString = *XML::Xercesc::XMLUni_fgIBM1140EncodingString;
*fgIBM1140EncodingString2 = *XML::Xercesc::XMLUni_fgIBM1140EncodingString2;
*fgIBM1140EncodingString3 = *XML::Xercesc::XMLUni_fgIBM1140EncodingString3;
*fgIBM1140EncodingString4 = *XML::Xercesc::XMLUni_fgIBM1140EncodingString4;
*fgIESString = *XML::Xercesc::XMLUni_fgIESString;
*fgIDString = *XML::Xercesc::XMLUni_fgIDString;
*fgIDRefString = *XML::Xercesc::XMLUni_fgIDRefString;
*fgIDRefsString = *XML::Xercesc::XMLUni_fgIDRefsString;
*fgImpliedString = *XML::Xercesc::XMLUni_fgImpliedString;
*fgIgnoreString = *XML::Xercesc::XMLUni_fgIgnoreString;
*fgIncludeString = *XML::Xercesc::XMLUni_fgIncludeString;
*fgISO88591EncodingString = *XML::Xercesc::XMLUni_fgISO88591EncodingString;
*fgISO88591EncodingString2 = *XML::Xercesc::XMLUni_fgISO88591EncodingString2;
*fgISO88591EncodingString3 = *XML::Xercesc::XMLUni_fgISO88591EncodingString3;
*fgISO88591EncodingString4 = *XML::Xercesc::XMLUni_fgISO88591EncodingString4;
*fgISO88591EncodingString5 = *XML::Xercesc::XMLUni_fgISO88591EncodingString5;
*fgISO88591EncodingString6 = *XML::Xercesc::XMLUni_fgISO88591EncodingString6;
*fgISO88591EncodingString7 = *XML::Xercesc::XMLUni_fgISO88591EncodingString7;
*fgISO88591EncodingString8 = *XML::Xercesc::XMLUni_fgISO88591EncodingString8;
*fgISO88591EncodingString9 = *XML::Xercesc::XMLUni_fgISO88591EncodingString9;
*fgISO88591EncodingString10 = *XML::Xercesc::XMLUni_fgISO88591EncodingString10;
*fgISO88591EncodingString11 = *XML::Xercesc::XMLUni_fgISO88591EncodingString11;
*fgISO88591EncodingString12 = *XML::Xercesc::XMLUni_fgISO88591EncodingString12;
*fgLocalHostString = *XML::Xercesc::XMLUni_fgLocalHostString;
*fgNoString = *XML::Xercesc::XMLUni_fgNoString;
*fgNotationString = *XML::Xercesc::XMLUni_fgNotationString;
*fgNDATAString = *XML::Xercesc::XMLUni_fgNDATAString;
*fgNmTokenString = *XML::Xercesc::XMLUni_fgNmTokenString;
*fgNmTokensString = *XML::Xercesc::XMLUni_fgNmTokensString;
*fgPCDATAString = *XML::Xercesc::XMLUni_fgPCDATAString;
*fgPIString = *XML::Xercesc::XMLUni_fgPIString;
*fgPubIDString = *XML::Xercesc::XMLUni_fgPubIDString;
*fgRefString = *XML::Xercesc::XMLUni_fgRefString;
*fgRequiredString = *XML::Xercesc::XMLUni_fgRequiredString;
*fgStandaloneString = *XML::Xercesc::XMLUni_fgStandaloneString;
*fgVersion1_0 = *XML::Xercesc::XMLUni_fgVersion1_0;
*fgVersion1_1 = *XML::Xercesc::XMLUni_fgVersion1_1;
*fgSysIDString = *XML::Xercesc::XMLUni_fgSysIDString;
*fgUnknownURIName = *XML::Xercesc::XMLUni_fgUnknownURIName;
*fgUCS4EncodingString = *XML::Xercesc::XMLUni_fgUCS4EncodingString;
*fgUCS4EncodingString2 = *XML::Xercesc::XMLUni_fgUCS4EncodingString2;
*fgUCS4EncodingString3 = *XML::Xercesc::XMLUni_fgUCS4EncodingString3;
*fgUCS4BEncodingString = *XML::Xercesc::XMLUni_fgUCS4BEncodingString;
*fgUCS4BEncodingString2 = *XML::Xercesc::XMLUni_fgUCS4BEncodingString2;
*fgUCS4LEncodingString = *XML::Xercesc::XMLUni_fgUCS4LEncodingString;
*fgUCS4LEncodingString2 = *XML::Xercesc::XMLUni_fgUCS4LEncodingString2;
*fgUSASCIIEncodingString = *XML::Xercesc::XMLUni_fgUSASCIIEncodingString;
*fgUSASCIIEncodingString2 = *XML::Xercesc::XMLUni_fgUSASCIIEncodingString2;
*fgUSASCIIEncodingString3 = *XML::Xercesc::XMLUni_fgUSASCIIEncodingString3;
*fgUSASCIIEncodingString4 = *XML::Xercesc::XMLUni_fgUSASCIIEncodingString4;
*fgUTF8EncodingString = *XML::Xercesc::XMLUni_fgUTF8EncodingString;
*fgUTF8EncodingString2 = *XML::Xercesc::XMLUni_fgUTF8EncodingString2;
*fgUTF16EncodingString = *XML::Xercesc::XMLUni_fgUTF16EncodingString;
*fgUTF16EncodingString2 = *XML::Xercesc::XMLUni_fgUTF16EncodingString2;
*fgUTF16EncodingString3 = *XML::Xercesc::XMLUni_fgUTF16EncodingString3;
*fgUTF16EncodingString4 = *XML::Xercesc::XMLUni_fgUTF16EncodingString4;
*fgUTF16EncodingString5 = *XML::Xercesc::XMLUni_fgUTF16EncodingString5;
*fgUTF16EncodingString6 = *XML::Xercesc::XMLUni_fgUTF16EncodingString6;
*fgUTF16EncodingString7 = *XML::Xercesc::XMLUni_fgUTF16EncodingString7;
*fgUTF16BEncodingString = *XML::Xercesc::XMLUni_fgUTF16BEncodingString;
*fgUTF16BEncodingString2 = *XML::Xercesc::XMLUni_fgUTF16BEncodingString2;
*fgUTF16LEncodingString = *XML::Xercesc::XMLUni_fgUTF16LEncodingString;
*fgUTF16LEncodingString2 = *XML::Xercesc::XMLUni_fgUTF16LEncodingString2;
*fgVersionString = *XML::Xercesc::XMLUni_fgVersionString;
*fgValidityDomain = *XML::Xercesc::XMLUni_fgValidityDomain;
*fgWin1252EncodingString = *XML::Xercesc::XMLUni_fgWin1252EncodingString;
*fgXMLChEncodingString = *XML::Xercesc::XMLUni_fgXMLChEncodingString;
*fgXMLDOMMsgDomain = *XML::Xercesc::XMLUni_fgXMLDOMMsgDomain;
*fgXMLString = *XML::Xercesc::XMLUni_fgXMLString;
*fgXMLStringSpace = *XML::Xercesc::XMLUni_fgXMLStringSpace;
*fgXMLStringHTab = *XML::Xercesc::XMLUni_fgXMLStringHTab;
*fgXMLStringCR = *XML::Xercesc::XMLUni_fgXMLStringCR;
*fgXMLStringLF = *XML::Xercesc::XMLUni_fgXMLStringLF;
*fgXMLStringSpaceU = *XML::Xercesc::XMLUni_fgXMLStringSpaceU;
*fgXMLStringHTabU = *XML::Xercesc::XMLUni_fgXMLStringHTabU;
*fgXMLStringCRU = *XML::Xercesc::XMLUni_fgXMLStringCRU;
*fgXMLStringLFU = *XML::Xercesc::XMLUni_fgXMLStringLFU;
*fgXMLDeclString = *XML::Xercesc::XMLUni_fgXMLDeclString;
*fgXMLDeclStringSpace = *XML::Xercesc::XMLUni_fgXMLDeclStringSpace;
*fgXMLDeclStringHTab = *XML::Xercesc::XMLUni_fgXMLDeclStringHTab;
*fgXMLDeclStringLF = *XML::Xercesc::XMLUni_fgXMLDeclStringLF;
*fgXMLDeclStringCR = *XML::Xercesc::XMLUni_fgXMLDeclStringCR;
*fgXMLDeclStringSpaceU = *XML::Xercesc::XMLUni_fgXMLDeclStringSpaceU;
*fgXMLDeclStringHTabU = *XML::Xercesc::XMLUni_fgXMLDeclStringHTabU;
*fgXMLDeclStringLFU = *XML::Xercesc::XMLUni_fgXMLDeclStringLFU;
*fgXMLDeclStringCRU = *XML::Xercesc::XMLUni_fgXMLDeclStringCRU;
*fgXMLNSString = *XML::Xercesc::XMLUni_fgXMLNSString;
*fgXMLNSColonString = *XML::Xercesc::XMLUni_fgXMLNSColonString;
*fgXMLNSURIName = *XML::Xercesc::XMLUni_fgXMLNSURIName;
*fgXMLErrDomain = *XML::Xercesc::XMLUni_fgXMLErrDomain;
*fgXMLURIName = *XML::Xercesc::XMLUni_fgXMLURIName;
*fgInfosetURIName = *XML::Xercesc::XMLUni_fgInfosetURIName;
*fgYesString = *XML::Xercesc::XMLUni_fgYesString;
*fgZeroLenString = *XML::Xercesc::XMLUni_fgZeroLenString;
*fgDTDEntityString = *XML::Xercesc::XMLUni_fgDTDEntityString;
*fgAmp = *XML::Xercesc::XMLUni_fgAmp;
*fgLT = *XML::Xercesc::XMLUni_fgLT;
*fgGT = *XML::Xercesc::XMLUni_fgGT;
*fgQuot = *XML::Xercesc::XMLUni_fgQuot;
*fgApos = *XML::Xercesc::XMLUni_fgApos;
*fgWFXMLScanner = *XML::Xercesc::XMLUni_fgWFXMLScanner;
*fgIGXMLScanner = *XML::Xercesc::XMLUni_fgIGXMLScanner;
*fgSGXMLScanner = *XML::Xercesc::XMLUni_fgSGXMLScanner;
*fgDGXMLScanner = *XML::Xercesc::XMLUni_fgDGXMLScanner;
*fgXSAXMLScanner = *XML::Xercesc::XMLUni_fgXSAXMLScanner;
*fgCDataStart = *XML::Xercesc::XMLUni_fgCDataStart;
*fgCDataEnd = *XML::Xercesc::XMLUni_fgCDataEnd;
*fgArrayIndexOutOfBoundsException_Name = *XML::Xercesc::XMLUni_fgArrayIndexOutOfBoundsException_Name;
*fgEmptyStackException_Name = *XML::Xercesc::XMLUni_fgEmptyStackException_Name;
*fgIllegalArgumentException_Name = *XML::Xercesc::XMLUni_fgIllegalArgumentException_Name;
*fgInvalidCastException_Name = *XML::Xercesc::XMLUni_fgInvalidCastException_Name;
*fgIOException_Name = *XML::Xercesc::XMLUni_fgIOException_Name;
*fgNoSuchElementException_Name = *XML::Xercesc::XMLUni_fgNoSuchElementException_Name;
*fgNullPointerException_Name = *XML::Xercesc::XMLUni_fgNullPointerException_Name;
*fgXMLPlatformUtilsException_Name = *XML::Xercesc::XMLUni_fgXMLPlatformUtilsException_Name;
*fgRuntimeException_Name = *XML::Xercesc::XMLUni_fgRuntimeException_Name;
*fgTranscodingException_Name = *XML::Xercesc::XMLUni_fgTranscodingException_Name;
*fgUnexpectedEOFException_Name = *XML::Xercesc::XMLUni_fgUnexpectedEOFException_Name;
*fgUnsupportedEncodingException_Name = *XML::Xercesc::XMLUni_fgUnsupportedEncodingException_Name;
*fgUTFDataFormatException_Name = *XML::Xercesc::XMLUni_fgUTFDataFormatException_Name;
*fgNetAccessorException_Name = *XML::Xercesc::XMLUni_fgNetAccessorException_Name;
*fgMalformedURLException_Name = *XML::Xercesc::XMLUni_fgMalformedURLException_Name;
*fgNumberFormatException_Name = *XML::Xercesc::XMLUni_fgNumberFormatException_Name;
*fgParseException_Name = *XML::Xercesc::XMLUni_fgParseException_Name;
*fgInvalidDatatypeFacetException_Name = *XML::Xercesc::XMLUni_fgInvalidDatatypeFacetException_Name;
*fgInvalidDatatypeValueException_Name = *XML::Xercesc::XMLUni_fgInvalidDatatypeValueException_Name;
*fgSchemaDateTimeException_Name = *XML::Xercesc::XMLUni_fgSchemaDateTimeException_Name;
*fgXPathException_Name = *XML::Xercesc::XMLUni_fgXPathException_Name;
*fgXSerializationException_Name = *XML::Xercesc::XMLUni_fgXSerializationException_Name;
*fgNegINFString = *XML::Xercesc::XMLUni_fgNegINFString;
*fgNegZeroString = *XML::Xercesc::XMLUni_fgNegZeroString;
*fgPosZeroString = *XML::Xercesc::XMLUni_fgPosZeroString;
*fgPosINFString = *XML::Xercesc::XMLUni_fgPosINFString;
*fgNaNString = *XML::Xercesc::XMLUni_fgNaNString;
*fgEString = *XML::Xercesc::XMLUni_fgEString;
*fgZeroString = *XML::Xercesc::XMLUni_fgZeroString;
*fgNullString = *XML::Xercesc::XMLUni_fgNullString;
*fgXercesDynamic = *XML::Xercesc::XMLUni_fgXercesDynamic;
*fgXercesSchema = *XML::Xercesc::XMLUni_fgXercesSchema;
*fgXercesSchemaFullChecking = *XML::Xercesc::XMLUni_fgXercesSchemaFullChecking;
*fgXercesIdentityConstraintChecking = *XML::Xercesc::XMLUni_fgXercesIdentityConstraintChecking;
*fgXercesSchemaExternalSchemaLocation = *XML::Xercesc::XMLUni_fgXercesSchemaExternalSchemaLocation;
*fgXercesSchemaExternalNoNameSpaceSchemaLocation = *XML::Xercesc::XMLUni_fgXercesSchemaExternalNoNameSpaceSchemaLocation;
*fgXercesSecurityManager = *XML::Xercesc::XMLUni_fgXercesSecurityManager;
*fgXercesLoadExternalDTD = *XML::Xercesc::XMLUni_fgXercesLoadExternalDTD;
*fgXercesContinueAfterFatalError = *XML::Xercesc::XMLUni_fgXercesContinueAfterFatalError;
*fgXercesValidationErrorAsFatal = *XML::Xercesc::XMLUni_fgXercesValidationErrorAsFatal;
*fgXercesUserAdoptsDOMDocument = *XML::Xercesc::XMLUni_fgXercesUserAdoptsDOMDocument;
*fgXercesCacheGrammarFromParse = *XML::Xercesc::XMLUni_fgXercesCacheGrammarFromParse;
*fgXercesUseCachedGrammarInParse = *XML::Xercesc::XMLUni_fgXercesUseCachedGrammarInParse;
*fgXercesScannerName = *XML::Xercesc::XMLUni_fgXercesScannerName;
*fgXercesParserUseDocumentFromImplementation = *XML::Xercesc::XMLUni_fgXercesParserUseDocumentFromImplementation;
*fgXercesCalculateSrcOfs = *XML::Xercesc::XMLUni_fgXercesCalculateSrcOfs;
*fgXercesStandardUriConformant = *XML::Xercesc::XMLUni_fgXercesStandardUriConformant;
*fgXercesDOMHasPSVIInfo = *XML::Xercesc::XMLUni_fgXercesDOMHasPSVIInfo;
*fgXercesGenerateSyntheticAnnotations = *XML::Xercesc::XMLUni_fgXercesGenerateSyntheticAnnotations;
*fgXercesValidateAnnotations = *XML::Xercesc::XMLUni_fgXercesValidateAnnotations;
*fgXercesIgnoreCachedDTD = *XML::Xercesc::XMLUni_fgXercesIgnoreCachedDTD;
*fgXercesIgnoreAnnotations = *XML::Xercesc::XMLUni_fgXercesIgnoreAnnotations;
*fgXercesDisableDefaultEntityResolution = *XML::Xercesc::XMLUni_fgXercesDisableDefaultEntityResolution;
*fgXercesSkipDTDValidation = *XML::Xercesc::XMLUni_fgXercesSkipDTDValidation;
*fgXercesEntityResolver = *XML::Xercesc::XMLUni_fgXercesEntityResolver;
*fgSAX2CoreValidation = *XML::Xercesc::XMLUni_fgSAX2CoreValidation;
*fgSAX2CoreNameSpaces = *XML::Xercesc::XMLUni_fgSAX2CoreNameSpaces;
*fgSAX2CoreNameSpacePrefixes = *XML::Xercesc::XMLUni_fgSAX2CoreNameSpacePrefixes;
*fgDOMCanonicalForm = *XML::Xercesc::XMLUni_fgDOMCanonicalForm;
*fgDOMCDATASections = *XML::Xercesc::XMLUni_fgDOMCDATASections;
*fgDOMComments = *XML::Xercesc::XMLUni_fgDOMComments;
*fgDOMCharsetOverridesXMLEncoding = *XML::Xercesc::XMLUni_fgDOMCharsetOverridesXMLEncoding;
*fgDOMCheckCharacterNormalization = *XML::Xercesc::XMLUni_fgDOMCheckCharacterNormalization;
*fgDOMDatatypeNormalization = *XML::Xercesc::XMLUni_fgDOMDatatypeNormalization;
*fgDOMDisallowDoctype = *XML::Xercesc::XMLUni_fgDOMDisallowDoctype;
*fgDOMElementContentWhitespace = *XML::Xercesc::XMLUni_fgDOMElementContentWhitespace;
*fgDOMErrorHandler = *XML::Xercesc::XMLUni_fgDOMErrorHandler;
*fgDOMEntities = *XML::Xercesc::XMLUni_fgDOMEntities;
*fgDOMIgnoreUnknownCharacterDenormalization = *XML::Xercesc::XMLUni_fgDOMIgnoreUnknownCharacterDenormalization;
*fgDOMInfoset = *XML::Xercesc::XMLUni_fgDOMInfoset;
*fgDOMNamespaces = *XML::Xercesc::XMLUni_fgDOMNamespaces;
*fgDOMNamespaceDeclarations = *XML::Xercesc::XMLUni_fgDOMNamespaceDeclarations;
*fgDOMNormalizeCharacters = *XML::Xercesc::XMLUni_fgDOMNormalizeCharacters;
*fgDOMResourceResolver = *XML::Xercesc::XMLUni_fgDOMResourceResolver;
*fgDOMSchemaLocation = *XML::Xercesc::XMLUni_fgDOMSchemaLocation;
*fgDOMSchemaType = *XML::Xercesc::XMLUni_fgDOMSchemaType;
*fgDOMSplitCDATASections = *XML::Xercesc::XMLUni_fgDOMSplitCDATASections;
*fgDOMSupportedMediatypesOnly = *XML::Xercesc::XMLUni_fgDOMSupportedMediatypesOnly;
*fgDOMValidate = *XML::Xercesc::XMLUni_fgDOMValidate;
*fgDOMValidateIfSchema = *XML::Xercesc::XMLUni_fgDOMValidateIfSchema;
*fgDOMWellFormed = *XML::Xercesc::XMLUni_fgDOMWellFormed;
*fgDOMXMLSchemaType = *XML::Xercesc::XMLUni_fgDOMXMLSchemaType;
*fgDOMDTDType = *XML::Xercesc::XMLUni_fgDOMDTDType;
*fgDOMWRTCanonicalForm = *XML::Xercesc::XMLUni_fgDOMWRTCanonicalForm;
*fgDOMWRTDiscardDefaultContent = *XML::Xercesc::XMLUni_fgDOMWRTDiscardDefaultContent;
*fgDOMWRTEntities = *XML::Xercesc::XMLUni_fgDOMWRTEntities;
*fgDOMWRTFormatPrettyPrint = *XML::Xercesc::XMLUni_fgDOMWRTFormatPrettyPrint;
*fgDOMWRTNormalizeCharacters = *XML::Xercesc::XMLUni_fgDOMWRTNormalizeCharacters;
*fgDOMWRTSplitCdataSections = *XML::Xercesc::XMLUni_fgDOMWRTSplitCdataSections;
*fgDOMWRTValidation = *XML::Xercesc::XMLUni_fgDOMWRTValidation;
*fgDOMWRTWhitespaceInElementContent = *XML::Xercesc::XMLUni_fgDOMWRTWhitespaceInElementContent;
*fgDOMWRTBOM = *XML::Xercesc::XMLUni_fgDOMWRTBOM;
*fgDOMXMLDeclaration = *XML::Xercesc::XMLUni_fgDOMXMLDeclaration;
*fgDOMWRTXercesPrettyPrint = *XML::Xercesc::XMLUni_fgDOMWRTXercesPrettyPrint;
*fgXercescInterfacePSVITypeInfo = *XML::Xercesc::XMLUni_fgXercescInterfacePSVITypeInfo;
*fgXercescInterfaceDOMDocumentTypeImpl = *XML::Xercesc::XMLUni_fgXercescInterfaceDOMDocumentTypeImpl;
*fgXercescInterfaceDOMMemoryManager = *XML::Xercesc::XMLUni_fgXercescInterfaceDOMMemoryManager;
*fgXercescDefaultLocale = *XML::Xercesc::XMLUni_fgXercescDefaultLocale;
*fgDefErrMsg = *XML::Xercesc::XMLUni_fgDefErrMsg;
*fgValueZero = *XML::Xercesc::XMLUni_fgValueZero;
*fgNegOne = *XML::Xercesc::XMLUni_fgNegOne;
*fgValueOne = *XML::Xercesc::XMLUni_fgValueOne;
*fgLongMaxInc = *XML::Xercesc::XMLUni_fgLongMaxInc;
*fgLongMinInc = *XML::Xercesc::XMLUni_fgLongMinInc;
*fgIntMaxInc = *XML::Xercesc::XMLUni_fgIntMaxInc;
*fgIntMinInc = *XML::Xercesc::XMLUni_fgIntMinInc;
*fgShortMaxInc = *XML::Xercesc::XMLUni_fgShortMaxInc;
*fgShortMinInc = *XML::Xercesc::XMLUni_fgShortMinInc;
*fgByteMaxInc = *XML::Xercesc::XMLUni_fgByteMaxInc;
*fgByteMinInc = *XML::Xercesc::XMLUni_fgByteMinInc;
*fgULongMaxInc = *XML::Xercesc::XMLUni_fgULongMaxInc;
*fgUIntMaxInc = *XML::Xercesc::XMLUni_fgUIntMaxInc;
*fgUShortMaxInc = *XML::Xercesc::XMLUni_fgUShortMaxInc;
*fgUByteMaxInc = *XML::Xercesc::XMLUni_fgUByteMaxInc;
*fgLangPattern = *XML::Xercesc::XMLUni_fgLangPattern;
*fgBooleanValueSpace = *XML::Xercesc::XMLUni_fgBooleanValueSpace;
*fgBooleanValueSpaceArraySize = *XML::Xercesc::XMLUni_fgBooleanValueSpaceArraySize;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLUni($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::QName ##############

package XML::Xerces::QName;
use overload
    "==" => sub { $_[0]->__eq__($_[1])},
    "fallback" => 1;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_QName(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_QName($self);
        delete $OWNER{$self};
    }
}

*getPrefix = *XML::Xercesc::QName_getPrefix;
*getLocalPart = *XML::Xercesc::QName_getLocalPart;
*getURI = *XML::Xercesc::QName_getURI;
*getRawName = *XML::Xercesc::QName_getRawName;
*getMemoryManager = *XML::Xercesc::QName_getMemoryManager;
*setName = *XML::Xercesc::QName_setName;
*setPrefix = *XML::Xercesc::QName_setPrefix;
*setLocalPart = *XML::Xercesc::QName_setLocalPart;
*setNPrefix = *XML::Xercesc::QName_setNPrefix;
*setNLocalPart = *XML::Xercesc::QName_setNLocalPart;
*setURI = *XML::Xercesc::QName_setURI;
*setValues = *XML::Xercesc::QName_setValues;
*__eq__ = *XML::Xercesc::QName___eq__;
*cleanUp = *XML::Xercesc::QName_cleanUp;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLElementDecl ##############

package XML::Xerces::XMLElementDecl;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*NoReason = *XML::Xercesc::XMLElementDecl_NoReason;
*Declared = *XML::Xercesc::XMLElementDecl_Declared;
*AttList = *XML::Xercesc::XMLElementDecl_AttList;
*InContentModel = *XML::Xercesc::XMLElementDecl_InContentModel;
*AsRootElem = *XML::Xercesc::XMLElementDecl_AsRootElem;
*JustFaultIn = *XML::Xercesc::XMLElementDecl_JustFaultIn;
*NoCharData = *XML::Xercesc::XMLElementDecl_NoCharData;
*SpacesOk = *XML::Xercesc::XMLElementDecl_SpacesOk;
*AllCharData = *XML::Xercesc::XMLElementDecl_AllCharData;
*fgInvalidElemId = *XML::Xercesc::XMLElementDecl_fgInvalidElemId;
*fgPCDataElemId = *XML::Xercesc::XMLElementDecl_fgPCDataElemId;
*fgPCDataElemName = *XML::Xercesc::XMLElementDecl_fgPCDataElemName;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLElementDecl($self);
        delete $OWNER{$self};
    }
}

*getAttDefList = *XML::Xercesc::XMLElementDecl_getAttDefList;
*getCharDataOpts = *XML::Xercesc::XMLElementDecl_getCharDataOpts;
*hasAttDefs = *XML::Xercesc::XMLElementDecl_hasAttDefs;
*getContentSpec = *XML::Xercesc::XMLElementDecl_getContentSpec;
*setContentSpec = *XML::Xercesc::XMLElementDecl_setContentSpec;
*getContentModel = *XML::Xercesc::XMLElementDecl_getContentModel;
*setContentModel = *XML::Xercesc::XMLElementDecl_setContentModel;
*getFormattedContentModel = *XML::Xercesc::XMLElementDecl_getFormattedContentModel;
*getBaseName = *XML::Xercesc::XMLElementDecl_getBaseName;
*getURI = *XML::Xercesc::XMLElementDecl_getURI;
*getElementName = *XML::Xercesc::XMLElementDecl_getElementName;
*getFullName = *XML::Xercesc::XMLElementDecl_getFullName;
*getCreateReason = *XML::Xercesc::XMLElementDecl_getCreateReason;
*getId = *XML::Xercesc::XMLElementDecl_getId;
*isDeclared = *XML::Xercesc::XMLElementDecl_isDeclared;
*isExternal = *XML::Xercesc::XMLElementDecl_isExternal;
*getMemoryManager = *XML::Xercesc::XMLElementDecl_getMemoryManager;
*setElementName = *XML::Xercesc::XMLElementDecl_setElementName;
*setCreateReason = *XML::Xercesc::XMLElementDecl_setCreateReason;
*setId = *XML::Xercesc::XMLElementDecl_setId;
*setExternalElemDeclaration = *XML::Xercesc::XMLElementDecl_setExternalElemDeclaration;
*Schema = *XML::Xercesc::XMLElementDecl_Schema;
*DTD = *XML::Xercesc::XMLElementDecl_DTD;
*UnKnown = *XML::Xercesc::XMLElementDecl_UnKnown;
*getObjectType = *XML::Xercesc::XMLElementDecl_getObjectType;
*storeElementDecl = *XML::Xercesc::XMLElementDecl_storeElementDecl;
*loadElementDecl = *XML::Xercesc::XMLElementDecl_loadElementDecl;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLEntityDecl ##############

package XML::Xerces::XMLEntityDecl;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLEntityDecl($self);
        delete $OWNER{$self};
    }
}

*getDeclaredInIntSubset = *XML::Xercesc::XMLEntityDecl_getDeclaredInIntSubset;
*getIsParameter = *XML::Xercesc::XMLEntityDecl_getIsParameter;
*getIsSpecialChar = *XML::Xercesc::XMLEntityDecl_getIsSpecialChar;
*getId = *XML::Xercesc::XMLEntityDecl_getId;
*getName = *XML::Xercesc::XMLEntityDecl_getName;
*getNotationName = *XML::Xercesc::XMLEntityDecl_getNotationName;
*getPublicId = *XML::Xercesc::XMLEntityDecl_getPublicId;
*getSystemId = *XML::Xercesc::XMLEntityDecl_getSystemId;
*getBaseURI = *XML::Xercesc::XMLEntityDecl_getBaseURI;
*getValue = *XML::Xercesc::XMLEntityDecl_getValue;
*getValueLen = *XML::Xercesc::XMLEntityDecl_getValueLen;
*isExternal = *XML::Xercesc::XMLEntityDecl_isExternal;
*isUnparsed = *XML::Xercesc::XMLEntityDecl_isUnparsed;
*getMemoryManager = *XML::Xercesc::XMLEntityDecl_getMemoryManager;
*setName = *XML::Xercesc::XMLEntityDecl_setName;
*setNotationName = *XML::Xercesc::XMLEntityDecl_setNotationName;
*setPublicId = *XML::Xercesc::XMLEntityDecl_setPublicId;
*setSystemId = *XML::Xercesc::XMLEntityDecl_setSystemId;
*setBaseURI = *XML::Xercesc::XMLEntityDecl_setBaseURI;
*setValue = *XML::Xercesc::XMLEntityDecl_setValue;
*setId = *XML::Xercesc::XMLEntityDecl_setId;
*getKey = *XML::Xercesc::XMLEntityDecl_getKey;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLNotationDecl ##############

package XML::Xerces::XMLNotationDecl;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_XMLNotationDecl(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLNotationDecl($self);
        delete $OWNER{$self};
    }
}

*getId = *XML::Xercesc::XMLNotationDecl_getId;
*getName = *XML::Xercesc::XMLNotationDecl_getName;
*getPublicId = *XML::Xercesc::XMLNotationDecl_getPublicId;
*getSystemId = *XML::Xercesc::XMLNotationDecl_getSystemId;
*getBaseURI = *XML::Xercesc::XMLNotationDecl_getBaseURI;
*getNameSpaceId = *XML::Xercesc::XMLNotationDecl_getNameSpaceId;
*getMemoryManager = *XML::Xercesc::XMLNotationDecl_getMemoryManager;
*setId = *XML::Xercesc::XMLNotationDecl_setId;
*setName = *XML::Xercesc::XMLNotationDecl_setName;
*setPublicId = *XML::Xercesc::XMLNotationDecl_setPublicId;
*setSystemId = *XML::Xercesc::XMLNotationDecl_setSystemId;
*setBaseURI = *XML::Xercesc::XMLNotationDecl_setBaseURI;
*setNameSpaceId = *XML::Xercesc::XMLNotationDecl_setNameSpaceId;
*getKey = *XML::Xercesc::XMLNotationDecl_getKey;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLAttDefList ##############

package XML::Xerces::XMLAttDefList;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLAttDefList($self);
        delete $OWNER{$self};
    }
}

*isEmpty = *XML::Xercesc::XMLAttDefList_isEmpty;
*findAttDef = *XML::Xercesc::XMLAttDefList_findAttDef;
*getAttDefCount = *XML::Xercesc::XMLAttDefList_getAttDefCount;
*getAttDef = *XML::Xercesc::XMLAttDefList_getAttDef;
*getMemoryManager = *XML::Xercesc::XMLAttDefList_getMemoryManager;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLAttDef ##############

package XML::Xerces::XMLAttDef;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*CData = *XML::Xercesc::XMLAttDef_CData;
*ID = *XML::Xercesc::XMLAttDef_ID;
*IDRef = *XML::Xercesc::XMLAttDef_IDRef;
*IDRefs = *XML::Xercesc::XMLAttDef_IDRefs;
*Entity = *XML::Xercesc::XMLAttDef_Entity;
*Entities = *XML::Xercesc::XMLAttDef_Entities;
*NmToken = *XML::Xercesc::XMLAttDef_NmToken;
*NmTokens = *XML::Xercesc::XMLAttDef_NmTokens;
*Notation = *XML::Xercesc::XMLAttDef_Notation;
*Enumeration = *XML::Xercesc::XMLAttDef_Enumeration;
*Simple = *XML::Xercesc::XMLAttDef_Simple;
*Any_Any = *XML::Xercesc::XMLAttDef_Any_Any;
*Any_Other = *XML::Xercesc::XMLAttDef_Any_Other;
*Any_List = *XML::Xercesc::XMLAttDef_Any_List;
*AttTypes_Count = *XML::Xercesc::XMLAttDef_AttTypes_Count;
*AttTypes_Min = *XML::Xercesc::XMLAttDef_AttTypes_Min;
*AttTypes_Max = *XML::Xercesc::XMLAttDef_AttTypes_Max;
*AttTypes_Unknown = *XML::Xercesc::XMLAttDef_AttTypes_Unknown;
*Default = *XML::Xercesc::XMLAttDef_Default;
*Fixed = *XML::Xercesc::XMLAttDef_Fixed;
*Required = *XML::Xercesc::XMLAttDef_Required;
*Required_And_Fixed = *XML::Xercesc::XMLAttDef_Required_And_Fixed;
*Implied = *XML::Xercesc::XMLAttDef_Implied;
*ProcessContents_Skip = *XML::Xercesc::XMLAttDef_ProcessContents_Skip;
*ProcessContents_Lax = *XML::Xercesc::XMLAttDef_ProcessContents_Lax;
*ProcessContents_Strict = *XML::Xercesc::XMLAttDef_ProcessContents_Strict;
*Prohibited = *XML::Xercesc::XMLAttDef_Prohibited;
*DefAttTypes_Count = *XML::Xercesc::XMLAttDef_DefAttTypes_Count;
*DefAttTypes_Min = *XML::Xercesc::XMLAttDef_DefAttTypes_Min;
*DefAttTypes_Max = *XML::Xercesc::XMLAttDef_DefAttTypes_Max;
*DefAttTypes_Unknown = *XML::Xercesc::XMLAttDef_DefAttTypes_Unknown;
*NoReason = *XML::Xercesc::XMLAttDef_NoReason;
*JustFaultIn = *XML::Xercesc::XMLAttDef_JustFaultIn;
*fgInvalidAttrId = *XML::Xercesc::XMLAttDef_fgInvalidAttrId;
*getAttTypeString = *XML::Xercesc::XMLAttDef_getAttTypeString;
*getDefAttTypeString = *XML::Xercesc::XMLAttDef_getDefAttTypeString;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLAttDef($self);
        delete $OWNER{$self};
    }
}

*getFullName = *XML::Xercesc::XMLAttDef_getFullName;
*reset = *XML::Xercesc::XMLAttDef_reset;
*getDefaultType = *XML::Xercesc::XMLAttDef_getDefaultType;
*getEnumeration = *XML::Xercesc::XMLAttDef_getEnumeration;
*getId = *XML::Xercesc::XMLAttDef_getId;
*getType = *XML::Xercesc::XMLAttDef_getType;
*getValue = *XML::Xercesc::XMLAttDef_getValue;
*getCreateReason = *XML::Xercesc::XMLAttDef_getCreateReason;
*isExternal = *XML::Xercesc::XMLAttDef_isExternal;
*getMemoryManager = *XML::Xercesc::XMLAttDef_getMemoryManager;
*setDefaultType = *XML::Xercesc::XMLAttDef_setDefaultType;
*setId = *XML::Xercesc::XMLAttDef_setId;
*setType = *XML::Xercesc::XMLAttDef_setType;
*setValue = *XML::Xercesc::XMLAttDef_setValue;
*setEnumeration = *XML::Xercesc::XMLAttDef_setEnumeration;
*setCreateReason = *XML::Xercesc::XMLAttDef_setCreateReason;
*setExternalAttDeclaration = *XML::Xercesc::XMLAttDef_setExternalAttDeclaration;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLValidator ##############

package XML::Xerces::XMLValidator;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLValidator($self);
        delete $OWNER{$self};
    }
}

*getGrammar = *XML::Xercesc::XMLValidator_getGrammar;
*setGrammar = *XML::Xercesc::XMLValidator_setGrammar;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::Grammar ##############

package XML::Xerces::Grammar;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*DTDGrammarType = *XML::Xercesc::Grammar_DTDGrammarType;
*SchemaGrammarType = *XML::Xercesc::Grammar_SchemaGrammarType;
*UnKnown = *XML::Xercesc::Grammar_UnKnown;
*UNKNOWN_SCOPE = *XML::Xercesc::Grammar_UNKNOWN_SCOPE;
*TOP_LEVEL_SCOPE = *XML::Xercesc::Grammar_TOP_LEVEL_SCOPE;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_Grammar($self);
        delete $OWNER{$self};
    }
}

*getGrammarType = *XML::Xercesc::Grammar_getGrammarType;
*getTargetNamespace = *XML::Xercesc::Grammar_getTargetNamespace;
*getValidated = *XML::Xercesc::Grammar_getValidated;
*findOrAddElemDecl = *XML::Xercesc::Grammar_findOrAddElemDecl;
*getElemId = *XML::Xercesc::Grammar_getElemId;
*getElemDecl = *XML::Xercesc::Grammar_getElemDecl;
*getNotationDecl = *XML::Xercesc::Grammar_getNotationDecl;
*putElemDecl = *XML::Xercesc::Grammar_putElemDecl;
*putNotationDecl = *XML::Xercesc::Grammar_putNotationDecl;
*setValidated = *XML::Xercesc::Grammar_setValidated;
*reset = *XML::Xercesc::Grammar_reset;
*setGrammarDescription = *XML::Xercesc::Grammar_setGrammarDescription;
*getGrammarDescription = *XML::Xercesc::Grammar_getGrammarDescription;
*storeGrammar = *XML::Xercesc::Grammar_storeGrammar;
*loadGrammar = *XML::Xercesc::Grammar_loadGrammar;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLGrammarDescription ##############

package XML::Xerces::XMLGrammarDescription;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLGrammarDescription($self);
        delete $OWNER{$self};
    }
}

*getGrammarType = *XML::Xercesc::XMLGrammarDescription_getGrammarType;
*getGrammarKey = *XML::Xercesc::XMLGrammarDescription_getGrammarKey;
*getMemoryManager = *XML::Xercesc::XMLGrammarDescription_getMemoryManager;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLDTDDescription ##############

package XML::Xerces::XMLDTDDescription;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLGrammarDescription XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLDTDDescription($self);
        delete $OWNER{$self};
    }
}

*getGrammarType = *XML::Xercesc::XMLDTDDescription_getGrammarType;
*getRootName = *XML::Xercesc::XMLDTDDescription_getRootName;
*getSystemId = *XML::Xercesc::XMLDTDDescription_getSystemId;
*setRootName = *XML::Xercesc::XMLDTDDescription_setRootName;
*setSystemId = *XML::Xercesc::XMLDTDDescription_setSystemId;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLSchemaDescription ##############

package XML::Xerces::XMLSchemaDescription;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLGrammarDescription XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLSchemaDescription($self);
        delete $OWNER{$self};
    }
}

*getGrammarType = *XML::Xercesc::XMLSchemaDescription_getGrammarType;
*CONTEXT_INCLUDE = *XML::Xercesc::XMLSchemaDescription_CONTEXT_INCLUDE;
*CONTEXT_REDEFINE = *XML::Xercesc::XMLSchemaDescription_CONTEXT_REDEFINE;
*CONTEXT_IMPORT = *XML::Xercesc::XMLSchemaDescription_CONTEXT_IMPORT;
*CONTEXT_PREPARSE = *XML::Xercesc::XMLSchemaDescription_CONTEXT_PREPARSE;
*CONTEXT_INSTANCE = *XML::Xercesc::XMLSchemaDescription_CONTEXT_INSTANCE;
*CONTEXT_ELEMENT = *XML::Xercesc::XMLSchemaDescription_CONTEXT_ELEMENT;
*CONTEXT_ATTRIBUTE = *XML::Xercesc::XMLSchemaDescription_CONTEXT_ATTRIBUTE;
*CONTEXT_XSITYPE = *XML::Xercesc::XMLSchemaDescription_CONTEXT_XSITYPE;
*CONTEXT_UNKNOWN = *XML::Xercesc::XMLSchemaDescription_CONTEXT_UNKNOWN;
*getContextType = *XML::Xercesc::XMLSchemaDescription_getContextType;
*getTargetNamespace = *XML::Xercesc::XMLSchemaDescription_getTargetNamespace;
*getLocationHints = *XML::Xercesc::XMLSchemaDescription_getLocationHints;
*getTriggeringComponent = *XML::Xercesc::XMLSchemaDescription_getTriggeringComponent;
*getEnclosingElementName = *XML::Xercesc::XMLSchemaDescription_getEnclosingElementName;
*getAttributes = *XML::Xercesc::XMLSchemaDescription_getAttributes;
*setContextType = *XML::Xercesc::XMLSchemaDescription_setContextType;
*setTargetNamespace = *XML::Xercesc::XMLSchemaDescription_setTargetNamespace;
*setLocationHints = *XML::Xercesc::XMLSchemaDescription_setLocationHints;
*setTriggeringComponent = *XML::Xercesc::XMLSchemaDescription_setTriggeringComponent;
*setEnclosingElementName = *XML::Xercesc::XMLSchemaDescription_setEnclosingElementName;
*setAttributes = *XML::Xercesc::XMLSchemaDescription_setAttributes;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DTDElementDecl ##############

package XML::Xerces::DTDElementDecl;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLElementDecl XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*Empty = *XML::Xercesc::DTDElementDecl_Empty;
*Any = *XML::Xercesc::DTDElementDecl_Any;
*Mixed_Simple = *XML::Xercesc::DTDElementDecl_Mixed_Simple;
*Children = *XML::Xercesc::DTDElementDecl_Children;
*ModelTypes_Count = *XML::Xercesc::DTDElementDecl_ModelTypes_Count;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DTDElementDecl($self);
        delete $OWNER{$self};
    }
}

*getAttDefList = *XML::Xercesc::DTDElementDecl_getAttDefList;
*getCharDataOpts = *XML::Xercesc::DTDElementDecl_getCharDataOpts;
*hasAttDefs = *XML::Xercesc::DTDElementDecl_hasAttDefs;
*getContentSpec = *XML::Xercesc::DTDElementDecl_getContentSpec;
*setContentSpec = *XML::Xercesc::DTDElementDecl_setContentSpec;
*getContentModel = *XML::Xercesc::DTDElementDecl_getContentModel;
*setContentModel = *XML::Xercesc::DTDElementDecl_setContentModel;
*getFormattedContentModel = *XML::Xercesc::DTDElementDecl_getFormattedContentModel;
*getKey = *XML::Xercesc::DTDElementDecl_getKey;
*getAttDef = *XML::Xercesc::DTDElementDecl_getAttDef;
*getModelType = *XML::Xercesc::DTDElementDecl_getModelType;
*addAttDef = *XML::Xercesc::DTDElementDecl_addAttDef;
*setModelType = *XML::Xercesc::DTDElementDecl_setModelType;
*getObjectType = *XML::Xercesc::DTDElementDecl_getObjectType;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DTDElementDeclEnumerator ##############

package XML::Xerces::DTDElementDeclEnumerator;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_DTDElementDeclEnumerator(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DTDElementDeclEnumerator($self);
        delete $OWNER{$self};
    }
}

*hasMoreElements = *XML::Xercesc::DTDElementDeclEnumerator_hasMoreElements;
*nextElement = *XML::Xercesc::DTDElementDeclEnumerator_nextElement;
*Reset = *XML::Xercesc::DTDElementDeclEnumerator_Reset;
*size = *XML::Xercesc::DTDElementDeclEnumerator_size;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DTDEntityDecl ##############

package XML::Xerces::DTDEntityDecl;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLEntityDecl XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DTDEntityDecl($self);
        delete $OWNER{$self};
    }
}

*getDeclaredInIntSubset = *XML::Xercesc::DTDEntityDecl_getDeclaredInIntSubset;
*getIsParameter = *XML::Xercesc::DTDEntityDecl_getIsParameter;
*getIsSpecialChar = *XML::Xercesc::DTDEntityDecl_getIsSpecialChar;
*setDeclaredInIntSubset = *XML::Xercesc::DTDEntityDecl_setDeclaredInIntSubset;
*setIsParameter = *XML::Xercesc::DTDEntityDecl_setIsParameter;
*setIsSpecialChar = *XML::Xercesc::DTDEntityDecl_setIsSpecialChar;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DTDEntityDeclEnumerator ##############

package XML::Xerces::DTDEntityDeclEnumerator;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_DTDEntityDeclEnumerator(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DTDEntityDeclEnumerator($self);
        delete $OWNER{$self};
    }
}

*hasMoreElements = *XML::Xercesc::DTDEntityDeclEnumerator_hasMoreElements;
*nextElement = *XML::Xercesc::DTDEntityDeclEnumerator_nextElement;
*Reset = *XML::Xercesc::DTDEntityDeclEnumerator_Reset;
*size = *XML::Xercesc::DTDEntityDeclEnumerator_size;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DTDAttDefList ##############

package XML::Xerces::DTDAttDefList;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLAttDefList XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DTDAttDefList($self);
        delete $OWNER{$self};
    }
}

*isEmpty = *XML::Xercesc::DTDAttDefList_isEmpty;
*findAttDef = *XML::Xercesc::DTDAttDefList_findAttDef;
*getAttDefCount = *XML::Xercesc::DTDAttDefList_getAttDefCount;
*getAttDef = *XML::Xercesc::DTDAttDefList_getAttDef;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DTDAttDef ##############

package XML::Xerces::DTDAttDef;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLAttDef XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DTDAttDef($self);
        delete $OWNER{$self};
    }
}

*getFullName = *XML::Xercesc::DTDAttDef_getFullName;
*reset = *XML::Xercesc::DTDAttDef_reset;
*getElemId = *XML::Xercesc::DTDAttDef_getElemId;
*setElemId = *XML::Xercesc::DTDAttDef_setElemId;
*setName = *XML::Xercesc::DTDAttDef_setName;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DTDGrammar ##############

package XML::Xerces::DTDGrammar;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::Grammar XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_DTDGrammar(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DTDGrammar($self);
        delete $OWNER{$self};
    }
}

*getGrammarType = *XML::Xercesc::DTDGrammar_getGrammarType;
*getTargetNamespace = *XML::Xercesc::DTDGrammar_getTargetNamespace;
*findOrAddElemDecl = *XML::Xercesc::DTDGrammar_findOrAddElemDecl;
*getElemId = *XML::Xercesc::DTDGrammar_getElemId;
*getElemDecl = *XML::Xercesc::DTDGrammar_getElemDecl;
*getNotationDecl = *XML::Xercesc::DTDGrammar_getNotationDecl;
*getValidated = *XML::Xercesc::DTDGrammar_getValidated;
*putElemDecl = *XML::Xercesc::DTDGrammar_putElemDecl;
*putNotationDecl = *XML::Xercesc::DTDGrammar_putNotationDecl;
*setValidated = *XML::Xercesc::DTDGrammar_setValidated;
*reset = *XML::Xercesc::DTDGrammar_reset;
*getEntityDecl = *XML::Xercesc::DTDGrammar_getEntityDecl;
*getEntityDeclPool = *XML::Xercesc::DTDGrammar_getEntityDeclPool;
*getElemEnumerator = *XML::Xercesc::DTDGrammar_getElemEnumerator;
*getEntityEnumerator = *XML::Xercesc::DTDGrammar_getEntityEnumerator;
*getNotationEnumerator = *XML::Xercesc::DTDGrammar_getNotationEnumerator;
*setGrammarDescription = *XML::Xercesc::DTDGrammar_setGrammarDescription;
*getGrammarDescription = *XML::Xercesc::DTDGrammar_getGrammarDescription;
*putEntityDecl = *XML::Xercesc::DTDGrammar_putEntityDecl;
*reinitDfltEntities = *XML::Xercesc::DTDGrammar_reinitDfltEntities;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DTDValidator ##############

package XML::Xerces::DTDValidator;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLValidator XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_DTDValidator(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DTDValidator($self);
        delete $OWNER{$self};
    }
}

*getGrammar = *XML::Xercesc::DTDValidator_getGrammar;
*setGrammar = *XML::Xercesc::DTDValidator_setGrammar;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::SchemaElementDecl ##############

package XML::Xerces::SchemaElementDecl;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLElementDecl XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*Empty = *XML::Xercesc::SchemaElementDecl_Empty;
*Any = *XML::Xercesc::SchemaElementDecl_Any;
*Mixed_Simple = *XML::Xercesc::SchemaElementDecl_Mixed_Simple;
*Mixed_Complex = *XML::Xercesc::SchemaElementDecl_Mixed_Complex;
*Children = *XML::Xercesc::SchemaElementDecl_Children;
*Simple = *XML::Xercesc::SchemaElementDecl_Simple;
*ModelTypes_Count = *XML::Xercesc::SchemaElementDecl_ModelTypes_Count;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SchemaElementDecl($self);
        delete $OWNER{$self};
    }
}

*getAttDefList = *XML::Xercesc::SchemaElementDecl_getAttDefList;
*getCharDataOpts = *XML::Xercesc::SchemaElementDecl_getCharDataOpts;
*hasAttDefs = *XML::Xercesc::SchemaElementDecl_hasAttDefs;
*getContentSpec = *XML::Xercesc::SchemaElementDecl_getContentSpec;
*setContentSpec = *XML::Xercesc::SchemaElementDecl_setContentSpec;
*getContentModel = *XML::Xercesc::SchemaElementDecl_getContentModel;
*setContentModel = *XML::Xercesc::SchemaElementDecl_setContentModel;
*getFormattedContentModel = *XML::Xercesc::SchemaElementDecl_getFormattedContentModel;
*getAttDef = *XML::Xercesc::SchemaElementDecl_getAttDef;
*getAttWildCard = *XML::Xercesc::SchemaElementDecl_getAttWildCard;
*getModelType = *XML::Xercesc::SchemaElementDecl_getModelType;
*getPSVIScope = *XML::Xercesc::SchemaElementDecl_getPSVIScope;
*getDatatypeValidator = *XML::Xercesc::SchemaElementDecl_getDatatypeValidator;
*getEnclosingScope = *XML::Xercesc::SchemaElementDecl_getEnclosingScope;
*getFinalSet = *XML::Xercesc::SchemaElementDecl_getFinalSet;
*getBlockSet = *XML::Xercesc::SchemaElementDecl_getBlockSet;
*getMiscFlags = *XML::Xercesc::SchemaElementDecl_getMiscFlags;
*getDefaultValue = *XML::Xercesc::SchemaElementDecl_getDefaultValue;
*getComplexTypeInfo = *XML::Xercesc::SchemaElementDecl_getComplexTypeInfo;
*isGlobalDecl = *XML::Xercesc::SchemaElementDecl_isGlobalDecl;
*getSubstitutionGroupElem = *XML::Xercesc::SchemaElementDecl_getSubstitutionGroupElem;
*setModelType = *XML::Xercesc::SchemaElementDecl_setModelType;
*setPSVIScope = *XML::Xercesc::SchemaElementDecl_setPSVIScope;
*setDatatypeValidator = *XML::Xercesc::SchemaElementDecl_setDatatypeValidator;
*setEnclosingScope = *XML::Xercesc::SchemaElementDecl_setEnclosingScope;
*setFinalSet = *XML::Xercesc::SchemaElementDecl_setFinalSet;
*setBlockSet = *XML::Xercesc::SchemaElementDecl_setBlockSet;
*setMiscFlags = *XML::Xercesc::SchemaElementDecl_setMiscFlags;
*setDefaultValue = *XML::Xercesc::SchemaElementDecl_setDefaultValue;
*setComplexTypeInfo = *XML::Xercesc::SchemaElementDecl_setComplexTypeInfo;
*setAttWildCard = *XML::Xercesc::SchemaElementDecl_setAttWildCard;
*setSubstitutionGroupElem = *XML::Xercesc::SchemaElementDecl_setSubstitutionGroupElem;
*addIdentityConstraint = *XML::Xercesc::SchemaElementDecl_addIdentityConstraint;
*getIdentityConstraintCount = *XML::Xercesc::SchemaElementDecl_getIdentityConstraintCount;
*getIdentityConstraintAt = *XML::Xercesc::SchemaElementDecl_getIdentityConstraintAt;
*getObjectType = *XML::Xercesc::SchemaElementDecl_getObjectType;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::SchemaElementDeclEnumerator ##############

package XML::Xerces::SchemaElementDeclEnumerator;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SchemaElementDeclEnumerator($self);
        delete $OWNER{$self};
    }
}

sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_SchemaElementDeclEnumerator(@_);
    bless $self, $pkg if defined($self);
}

*hasMoreElements = *XML::Xercesc::SchemaElementDeclEnumerator_hasMoreElements;
*nextElement = *XML::Xercesc::SchemaElementDeclEnumerator_nextElement;
*Reset = *XML::Xercesc::SchemaElementDeclEnumerator_Reset;
*size = *XML::Xercesc::SchemaElementDeclEnumerator_size;
*resetKey = *XML::Xercesc::SchemaElementDeclEnumerator_resetKey;
*nextElementKey = *XML::Xercesc::SchemaElementDeclEnumerator_nextElementKey;
*hasMoreKeys = *XML::Xercesc::SchemaElementDeclEnumerator_hasMoreKeys;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::SchemaGrammar ##############

package XML::Xerces::SchemaGrammar;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::Grammar XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_SchemaGrammar(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SchemaGrammar($self);
        delete $OWNER{$self};
    }
}

*getGrammarType = *XML::Xercesc::SchemaGrammar_getGrammarType;
*getTargetNamespace = *XML::Xercesc::SchemaGrammar_getTargetNamespace;
*findOrAddElemDecl = *XML::Xercesc::SchemaGrammar_findOrAddElemDecl;
*getElemId = *XML::Xercesc::SchemaGrammar_getElemId;
*getElemDecl = *XML::Xercesc::SchemaGrammar_getElemDecl;
*getNotationDecl = *XML::Xercesc::SchemaGrammar_getNotationDecl;
*getValidated = *XML::Xercesc::SchemaGrammar_getValidated;
*putElemDecl = *XML::Xercesc::SchemaGrammar_putElemDecl;
*putNotationDecl = *XML::Xercesc::SchemaGrammar_putNotationDecl;
*setValidated = *XML::Xercesc::SchemaGrammar_setValidated;
*reset = *XML::Xercesc::SchemaGrammar_reset;
*getElemEnumerator = *XML::Xercesc::SchemaGrammar_getElemEnumerator;
*getNotationEnumerator = *XML::Xercesc::SchemaGrammar_getNotationEnumerator;
*getAttributeDeclRegistry = *XML::Xercesc::SchemaGrammar_getAttributeDeclRegistry;
*getComplexTypeRegistry = *XML::Xercesc::SchemaGrammar_getComplexTypeRegistry;
*getGroupInfoRegistry = *XML::Xercesc::SchemaGrammar_getGroupInfoRegistry;
*getAttGroupInfoRegistry = *XML::Xercesc::SchemaGrammar_getAttGroupInfoRegistry;
*getDatatypeRegistry = *XML::Xercesc::SchemaGrammar_getDatatypeRegistry;
*getNamespaceScope = *XML::Xercesc::SchemaGrammar_getNamespaceScope;
*getValidSubstitutionGroups = *XML::Xercesc::SchemaGrammar_getValidSubstitutionGroups;
*getValidationContext = *XML::Xercesc::SchemaGrammar_getValidationContext;
*setTargetNamespace = *XML::Xercesc::SchemaGrammar_setTargetNamespace;
*setAttributeDeclRegistry = *XML::Xercesc::SchemaGrammar_setAttributeDeclRegistry;
*setComplexTypeRegistry = *XML::Xercesc::SchemaGrammar_setComplexTypeRegistry;
*setGroupInfoRegistry = *XML::Xercesc::SchemaGrammar_setGroupInfoRegistry;
*setAttGroupInfoRegistry = *XML::Xercesc::SchemaGrammar_setAttGroupInfoRegistry;
*setNamespaceScope = *XML::Xercesc::SchemaGrammar_setNamespaceScope;
*setValidSubstitutionGroups = *XML::Xercesc::SchemaGrammar_setValidSubstitutionGroups;
*setGrammarDescription = *XML::Xercesc::SchemaGrammar_setGrammarDescription;
*getGrammarDescription = *XML::Xercesc::SchemaGrammar_getGrammarDescription;
*putGroupElemDecl = *XML::Xercesc::SchemaGrammar_putGroupElemDecl;
*putAnnotation = *XML::Xercesc::SchemaGrammar_putAnnotation;
*addAnnotation = *XML::Xercesc::SchemaGrammar_addAnnotation;
*getAnnotation = *XML::Xercesc::SchemaGrammar_getAnnotation;
*getAnnotations = *XML::Xercesc::SchemaGrammar_getAnnotations;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::SchemaValidator ##############

package XML::Xerces::SchemaValidator;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLValidator XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_SchemaValidator(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SchemaValidator($self);
        delete $OWNER{$self};
    }
}

*getGrammar = *XML::Xercesc::SchemaValidator_getGrammar;
*setGrammar = *XML::Xercesc::SchemaValidator_setGrammar;
*normalizeWhiteSpace = *XML::Xercesc::SchemaValidator_normalizeWhiteSpace;
*setGrammarResolver = *XML::Xercesc::SchemaValidator_setGrammarResolver;
*setXsiType = *XML::Xercesc::SchemaValidator_setXsiType;
*setNillable = *XML::Xercesc::SchemaValidator_setNillable;
*setExitOnFirstFatal = *XML::Xercesc::SchemaValidator_setExitOnFirstFatal;
*setDatatypeBuffer = *XML::Xercesc::SchemaValidator_setDatatypeBuffer;
*clearDatatypeBuffer = *XML::Xercesc::SchemaValidator_clearDatatypeBuffer;
*getCurrentTypeInfo = *XML::Xercesc::SchemaValidator_getCurrentTypeInfo;
*getCurrentDatatypeValidator = *XML::Xercesc::SchemaValidator_getCurrentDatatypeValidator;
*getMostRecentAttrValidator = *XML::Xercesc::SchemaValidator_getMostRecentAttrValidator;
*getErrorOccurred = *XML::Xercesc::SchemaValidator_getErrorOccurred;
*getIsElemSpecified = *XML::Xercesc::SchemaValidator_getIsElemSpecified;
*getNormalizedValue = *XML::Xercesc::SchemaValidator_getNormalizedValue;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::SchemaAttDefList ##############

package XML::Xerces::SchemaAttDefList;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLAttDefList XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SchemaAttDefList($self);
        delete $OWNER{$self};
    }
}

*isEmpty = *XML::Xercesc::SchemaAttDefList_isEmpty;
*findAttDef = *XML::Xercesc::SchemaAttDefList_findAttDef;
*findAttDefLocalPart = *XML::Xercesc::SchemaAttDefList_findAttDefLocalPart;
*getAttDefCount = *XML::Xercesc::SchemaAttDefList_getAttDefCount;
*getAttDef = *XML::Xercesc::SchemaAttDefList_getAttDef;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::SchemaAttDef ##############

package XML::Xerces::SchemaAttDef;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::XMLAttDef XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SchemaAttDef($self);
        delete $OWNER{$self};
    }
}

*getFullName = *XML::Xercesc::SchemaAttDef_getFullName;
*reset = *XML::Xercesc::SchemaAttDef_reset;
*getElemId = *XML::Xercesc::SchemaAttDef_getElemId;
*getAttName = *XML::Xercesc::SchemaAttDef_getAttName;
*getDatatypeValidator = *XML::Xercesc::SchemaAttDef_getDatatypeValidator;
*getNamespaceList = *XML::Xercesc::SchemaAttDef_getNamespaceList;
*getBaseAttDecl = *XML::Xercesc::SchemaAttDef_getBaseAttDecl;
*getPSVIScope = *XML::Xercesc::SchemaAttDef_getPSVIScope;
*setElemId = *XML::Xercesc::SchemaAttDef_setElemId;
*setAttName = *XML::Xercesc::SchemaAttDef_setAttName;
*setDatatypeValidator = *XML::Xercesc::SchemaAttDef_setDatatypeValidator;
*setBaseAttDecl = *XML::Xercesc::SchemaAttDef_setBaseAttDecl;
*setPSVIScope = *XML::Xercesc::SchemaAttDef_setPSVIScope;
*setNamespaceList = *XML::Xercesc::SchemaAttDef_setNamespaceList;
*resetNamespaceList = *XML::Xercesc::SchemaAttDef_resetNamespaceList;
*setEnclosingCT = *XML::Xercesc::SchemaAttDef_setEnclosingCT;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::InputSource ##############

package XML::Xerces::InputSource;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_InputSource($self);
        delete $OWNER{$self};
    }
}

*makeStream = *XML::Xercesc::InputSource_makeStream;
*getEncoding = *XML::Xercesc::InputSource_getEncoding;
*getPublicId = *XML::Xercesc::InputSource_getPublicId;
*getSystemId = *XML::Xercesc::InputSource_getSystemId;
*getIssueFatalErrorIfNotFound = *XML::Xercesc::InputSource_getIssueFatalErrorIfNotFound;
*getMemoryManager = *XML::Xercesc::InputSource_getMemoryManager;
*setEncoding = *XML::Xercesc::InputSource_setEncoding;
*setPublicId = *XML::Xercesc::InputSource_setPublicId;
*setSystemId = *XML::Xercesc::InputSource_setSystemId;
*setIssueFatalErrorIfNotFound = *XML::Xercesc::InputSource_setIssueFatalErrorIfNotFound;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::LocalFileInputSource ##############

package XML::Xerces::LocalFileInputSource;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::InputSource XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_LocalFileInputSource(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_LocalFileInputSource($self);
        delete $OWNER{$self};
    }
}

*makeStream = *XML::Xercesc::LocalFileInputSource_makeStream;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::StdInInputSource ##############

package XML::Xerces::StdInInputSource;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::InputSource XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_StdInInputSource(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_StdInInputSource($self);
        delete $OWNER{$self};
    }
}

*makeStream = *XML::Xercesc::StdInInputSource_makeStream;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::URLInputSource ##############

package XML::Xerces::URLInputSource;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::InputSource XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_URLInputSource(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_URLInputSource($self);
        delete $OWNER{$self};
    }
}

*makeStream = *XML::Xercesc::URLInputSource_makeStream;
*urlSrc = *XML::Xercesc::URLInputSource_urlSrc;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::MemBufInputSource ##############

package XML::Xerces::MemBufInputSource;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::InputSource XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
  my $pkg = shift;
  # SYSTEM ID is *optional*
  if (scalar @_ == 1) {
    push(@_,'FAKE_SYSTEM_ID');
  }

  my $self = XML::Xercesc::new_MemBufInputSource(@_);
    
  bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_MemBufInputSource($self);
        delete $OWNER{$self};
    }
}

*makeStream = *XML::Xercesc::MemBufInputSource_makeStream;
*setCopyBufToStream = *XML::Xercesc::MemBufInputSource_setCopyBufToStream;
*resetMemBufInputSource = *XML::Xercesc::MemBufInputSource_resetMemBufInputSource;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMException ##############

package XML::Xerces::DOMException;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_DOMException(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMException($self);
        delete $OWNER{$self};
    }
}

*INDEX_SIZE_ERR = *XML::Xercesc::DOMException_INDEX_SIZE_ERR;
*DOMSTRING_SIZE_ERR = *XML::Xercesc::DOMException_DOMSTRING_SIZE_ERR;
*HIERARCHY_REQUEST_ERR = *XML::Xercesc::DOMException_HIERARCHY_REQUEST_ERR;
*WRONG_DOCUMENT_ERR = *XML::Xercesc::DOMException_WRONG_DOCUMENT_ERR;
*INVALID_CHARACTER_ERR = *XML::Xercesc::DOMException_INVALID_CHARACTER_ERR;
*NO_DATA_ALLOWED_ERR = *XML::Xercesc::DOMException_NO_DATA_ALLOWED_ERR;
*NO_MODIFICATION_ALLOWED_ERR = *XML::Xercesc::DOMException_NO_MODIFICATION_ALLOWED_ERR;
*NOT_FOUND_ERR = *XML::Xercesc::DOMException_NOT_FOUND_ERR;
*NOT_SUPPORTED_ERR = *XML::Xercesc::DOMException_NOT_SUPPORTED_ERR;
*INUSE_ATTRIBUTE_ERR = *XML::Xercesc::DOMException_INUSE_ATTRIBUTE_ERR;
*INVALID_STATE_ERR = *XML::Xercesc::DOMException_INVALID_STATE_ERR;
*SYNTAX_ERR = *XML::Xercesc::DOMException_SYNTAX_ERR;
*INVALID_MODIFICATION_ERR = *XML::Xercesc::DOMException_INVALID_MODIFICATION_ERR;
*NAMESPACE_ERR = *XML::Xercesc::DOMException_NAMESPACE_ERR;
*INVALID_ACCESS_ERR = *XML::Xercesc::DOMException_INVALID_ACCESS_ERR;
*VALIDATION_ERR = *XML::Xercesc::DOMException_VALIDATION_ERR;
*TYPE_MISMATCH_ERR = *XML::Xercesc::DOMException_TYPE_MISMATCH_ERR;
*getMessage = *XML::Xercesc::DOMException_getMessage;
*swig_code_get = *XML::Xercesc::DOMException_code_get;
*swig_code_set = *XML::Xercesc::DOMException_code_set;
*swig_msg_get = *XML::Xercesc::DOMException_msg_get;
*swig_msg_set = *XML::Xercesc::DOMException_msg_set;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMNode ##############

package XML::Xerces::DOMNode;
use overload
    "!=" => sub { $_[0]->__ne__($_[1])},
    "==" => sub { $_[0]->__eq__($_[1])},
    "fallback" => 1;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMNode($self);
        delete $OWNER{$self};
    }
}

*ELEMENT_NODE = *XML::Xercesc::DOMNode_ELEMENT_NODE;
*ATTRIBUTE_NODE = *XML::Xercesc::DOMNode_ATTRIBUTE_NODE;
*TEXT_NODE = *XML::Xercesc::DOMNode_TEXT_NODE;
*CDATA_SECTION_NODE = *XML::Xercesc::DOMNode_CDATA_SECTION_NODE;
*ENTITY_REFERENCE_NODE = *XML::Xercesc::DOMNode_ENTITY_REFERENCE_NODE;
*ENTITY_NODE = *XML::Xercesc::DOMNode_ENTITY_NODE;
*PROCESSING_INSTRUCTION_NODE = *XML::Xercesc::DOMNode_PROCESSING_INSTRUCTION_NODE;
*COMMENT_NODE = *XML::Xercesc::DOMNode_COMMENT_NODE;
*DOCUMENT_NODE = *XML::Xercesc::DOMNode_DOCUMENT_NODE;
*DOCUMENT_TYPE_NODE = *XML::Xercesc::DOMNode_DOCUMENT_TYPE_NODE;
*DOCUMENT_FRAGMENT_NODE = *XML::Xercesc::DOMNode_DOCUMENT_FRAGMENT_NODE;
*NOTATION_NODE = *XML::Xercesc::DOMNode_NOTATION_NODE;
*DOCUMENT_POSITION_DISCONNECTED = *XML::Xercesc::DOMNode_DOCUMENT_POSITION_DISCONNECTED;
*DOCUMENT_POSITION_PRECEDING = *XML::Xercesc::DOMNode_DOCUMENT_POSITION_PRECEDING;
*DOCUMENT_POSITION_FOLLOWING = *XML::Xercesc::DOMNode_DOCUMENT_POSITION_FOLLOWING;
*DOCUMENT_POSITION_CONTAINS = *XML::Xercesc::DOMNode_DOCUMENT_POSITION_CONTAINS;
*DOCUMENT_POSITION_CONTAINED_BY = *XML::Xercesc::DOMNode_DOCUMENT_POSITION_CONTAINED_BY;
*DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC = *XML::Xercesc::DOMNode_DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC;
*getNodeName = *XML::Xercesc::DOMNode_getNodeName;
*getNodeValue = *XML::Xercesc::DOMNode_getNodeValue;
*getNodeType = *XML::Xercesc::DOMNode_getNodeType;
*getParentNode = *XML::Xercesc::DOMNode_getParentNode;
sub getChildNodes {
    my $result = XML::Xercesc::DOMNode_getChildNodes (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
*getFirstChild = *XML::Xercesc::DOMNode_getFirstChild;
*getLastChild = *XML::Xercesc::DOMNode_getLastChild;
*getPreviousSibling = *XML::Xercesc::DOMNode_getPreviousSibling;
*getNextSibling = *XML::Xercesc::DOMNode_getNextSibling;
sub getAttributes {
    my $result = XML::Xercesc::DOMNode_getAttributes (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_hash() if wantarray;
    return $result;#if *not* wantarray
}
  
*getOwnerDocument = *XML::Xercesc::DOMNode_getOwnerDocument;
*cloneNode = *XML::Xercesc::DOMNode_cloneNode;
*insertBefore = *XML::Xercesc::DOMNode_insertBefore;
*replaceChild = *XML::Xercesc::DOMNode_replaceChild;
*removeChild = *XML::Xercesc::DOMNode_removeChild;
*appendChild = *XML::Xercesc::DOMNode_appendChild;
*hasChildNodes = *XML::Xercesc::DOMNode_hasChildNodes;
*setNodeValue = *XML::Xercesc::DOMNode_setNodeValue;
*normalize = *XML::Xercesc::DOMNode_normalize;
*isSupported = *XML::Xercesc::DOMNode_isSupported;
*getNamespaceURI = *XML::Xercesc::DOMNode_getNamespaceURI;
*getLocalName = *XML::Xercesc::DOMNode_getLocalName;
*setPrefix = *XML::Xercesc::DOMNode_setPrefix;
*hasAttributes = *XML::Xercesc::DOMNode_hasAttributes;
*isSameNode = *XML::Xercesc::DOMNode_isSameNode;
*isEqualNode = *XML::Xercesc::DOMNode_isEqualNode;
*setUserData = *XML::Xercesc::DOMNode_setUserData;
*getUserData = *XML::Xercesc::DOMNode_getUserData;
*getBaseURI = *XML::Xercesc::DOMNode_getBaseURI;
*compareDocumentPosition = *XML::Xercesc::DOMNode_compareDocumentPosition;
*getTextContent = *XML::Xercesc::DOMNode_getTextContent;
*setTextContent = *XML::Xercesc::DOMNode_setTextContent;
*lookupPrefix = *XML::Xercesc::DOMNode_lookupPrefix;
*isDefaultNamespace = *XML::Xercesc::DOMNode_isDefaultNamespace;
*lookupNamespaceURI = *XML::Xercesc::DOMNode_lookupNamespaceURI;
*getFeature = *XML::Xercesc::DOMNode_getFeature;
*release = *XML::Xercesc::DOMNode_release;
*__eq__ = *XML::Xercesc::DOMNode___eq__;
*__ne__ = *XML::Xercesc::DOMNode___ne__;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMAttr ##############

package XML::Xerces::DOMAttr;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMAttr($self);
        delete $OWNER{$self};
    }
}

*getName = *XML::Xercesc::DOMAttr_getName;
*getSpecified = *XML::Xercesc::DOMAttr_getSpecified;
*getValue = *XML::Xercesc::DOMAttr_getValue;
*setValue = *XML::Xercesc::DOMAttr_setValue;
*getOwnerElement = *XML::Xercesc::DOMAttr_getOwnerElement;
*isId = *XML::Xercesc::DOMAttr_isId;
*getSchemaTypeInfo = *XML::Xercesc::DOMAttr_getSchemaTypeInfo;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMElement ##############

package XML::Xerces::DOMElement;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMElement($self);
        delete $OWNER{$self};
    }
}

*getTagName = *XML::Xercesc::DOMElement_getTagName;
*getAttribute = *XML::Xercesc::DOMElement_getAttribute;
*getAttributeNode = *XML::Xercesc::DOMElement_getAttributeNode;
sub getElementsByTagName {
    my $result = XML::Xercesc::DOMElement_getElementsByTagName (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
*setAttribute = *XML::Xercesc::DOMElement_setAttribute;
*setAttributeNode = *XML::Xercesc::DOMElement_setAttributeNode;
*removeAttributeNode = *XML::Xercesc::DOMElement_removeAttributeNode;
*removeAttribute = *XML::Xercesc::DOMElement_removeAttribute;
*getAttributeNS = *XML::Xercesc::DOMElement_getAttributeNS;
*setAttributeNS = *XML::Xercesc::DOMElement_setAttributeNS;
*removeAttributeNS = *XML::Xercesc::DOMElement_removeAttributeNS;
*getAttributeNodeNS = *XML::Xercesc::DOMElement_getAttributeNodeNS;
*setAttributeNodeNS = *XML::Xercesc::DOMElement_setAttributeNodeNS;
sub getElementsByTagNameNS {
    my $result = XML::Xercesc::DOMElement_getElementsByTagNameNS (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
*hasAttribute = *XML::Xercesc::DOMElement_hasAttribute;
*hasAttributeNS = *XML::Xercesc::DOMElement_hasAttributeNS;
*setIdAttribute = *XML::Xercesc::DOMElement_setIdAttribute;
*setIdAttributeNS = *XML::Xercesc::DOMElement_setIdAttributeNS;
*setIdAttributeNode = *XML::Xercesc::DOMElement_setIdAttributeNode;
*getSchemaTypeInfo = *XML::Xercesc::DOMElement_getSchemaTypeInfo;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMEntity ##############

package XML::Xerces::DOMEntity;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMEntity($self);
        delete $OWNER{$self};
    }
}

*getPublicId = *XML::Xercesc::DOMEntity_getPublicId;
*getSystemId = *XML::Xercesc::DOMEntity_getSystemId;
*getNotationName = *XML::Xercesc::DOMEntity_getNotationName;
*getInputEncoding = *XML::Xercesc::DOMEntity_getInputEncoding;
*getXmlEncoding = *XML::Xercesc::DOMEntity_getXmlEncoding;
*getXmlVersion = *XML::Xercesc::DOMEntity_getXmlVersion;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMDocumentType ##############

package XML::Xerces::DOMDocumentType;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMDocumentType($self);
        delete $OWNER{$self};
    }
}

*getName = *XML::Xercesc::DOMDocumentType_getName;
sub getEntities {
    my $result = XML::Xercesc::DOMDocumentType_getEntities (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_hash() if wantarray;
    return $result;#if *not* wantarray
}
  
sub getNotations {
    my $result = XML::Xercesc::DOMDocumentType_getNotations (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_hash() if wantarray;
    return $result;#if *not* wantarray
}
  
*getPublicId = *XML::Xercesc::DOMDocumentType_getPublicId;
*getSystemId = *XML::Xercesc::DOMDocumentType_getSystemId;
*getInternalSubset = *XML::Xercesc::DOMDocumentType_getInternalSubset;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMCharacterData ##############

package XML::Xerces::DOMCharacterData;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMCharacterData($self);
        delete $OWNER{$self};
    }
}

*getData = *XML::Xercesc::DOMCharacterData_getData;
*getLength = *XML::Xercesc::DOMCharacterData_getLength;
*substringData = *XML::Xercesc::DOMCharacterData_substringData;
*appendData = *XML::Xercesc::DOMCharacterData_appendData;
*insertData = *XML::Xercesc::DOMCharacterData_insertData;
*deleteData = *XML::Xercesc::DOMCharacterData_deleteData;
*replaceData = *XML::Xercesc::DOMCharacterData_replaceData;
*setData = *XML::Xercesc::DOMCharacterData_setData;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMComment ##############

package XML::Xerces::DOMComment;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMCharacterData XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMComment($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMText ##############

package XML::Xerces::DOMText;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMCharacterData XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMText($self);
        delete $OWNER{$self};
    }
}

*splitText = *XML::Xercesc::DOMText_splitText;
*getIsElementContentWhitespace = *XML::Xercesc::DOMText_getIsElementContentWhitespace;
*getWholeText = *XML::Xercesc::DOMText_getWholeText;
*replaceWholeText = *XML::Xercesc::DOMText_replaceWholeText;
*isIgnorableWhitespace = *XML::Xercesc::DOMText_isIgnorableWhitespace;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMCDATASection ##############

package XML::Xerces::DOMCDATASection;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMText XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMCDATASection($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMNodeList ##############

package XML::Xerces::DOMNodeList;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMNodeList($self);
        delete $OWNER{$self};
    }
}

*item = *XML::Xercesc::DOMNodeList_item;
*getLength = *XML::Xercesc::DOMNodeList_getLength;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMNamedNodeMap ##############

package XML::Xerces::DOMNamedNodeMap;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMNamedNodeMap($self);
        delete $OWNER{$self};
    }
}

*setNamedItem = *XML::Xercesc::DOMNamedNodeMap_setNamedItem;
*item = *XML::Xercesc::DOMNamedNodeMap_item;
*getNamedItem = *XML::Xercesc::DOMNamedNodeMap_getNamedItem;
*getLength = *XML::Xercesc::DOMNamedNodeMap_getLength;
*removeNamedItem = *XML::Xercesc::DOMNamedNodeMap_removeNamedItem;
*getNamedItemNS = *XML::Xercesc::DOMNamedNodeMap_getNamedItemNS;
*setNamedItemNS = *XML::Xercesc::DOMNamedNodeMap_setNamedItemNS;
*removeNamedItemNS = *XML::Xercesc::DOMNamedNodeMap_removeNamedItemNS;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMDocumentFragment ##############

package XML::Xerces::DOMDocumentFragment;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMDocumentFragment($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMEntityReference ##############

package XML::Xerces::DOMEntityReference;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMEntityReference($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMNotation ##############

package XML::Xerces::DOMNotation;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMNotation($self);
        delete $OWNER{$self};
    }
}

*getPublicId = *XML::Xercesc::DOMNotation_getPublicId;
*getSystemId = *XML::Xercesc::DOMNotation_getSystemId;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMProcessingInstruction ##############

package XML::Xerces::DOMProcessingInstruction;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMProcessingInstruction($self);
        delete $OWNER{$self};
    }
}

*getTarget = *XML::Xercesc::DOMProcessingInstruction_getTarget;
*getData = *XML::Xercesc::DOMProcessingInstruction_getData;
*setData = *XML::Xercesc::DOMProcessingInstruction_setData;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMDocumentRange ##############

package XML::Xerces::DOMDocumentRange;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMDocumentRange($self);
        delete $OWNER{$self};
    }
}

*createRange = *XML::Xercesc::DOMDocumentRange_createRange;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMDocumentTraversal ##############

package XML::Xerces::DOMDocumentTraversal;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMDocumentTraversal($self);
        delete $OWNER{$self};
    }
}

sub createNodeIterator {
    my ($self,$root,$what,$filter,$expand) = @_;
    my $callback = XML::Xerces::PerlNodeFilterCallbackHandler->new($filter);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__NODE_FILTER} = $callback;

    my @args = ($self,$root,$what,$callback,$expand);
    if ($args[0]->isa('XML::Xerces::DOMDocument')) {

    }
    return XML::Xercesc::DOMDocumentTraversal_createNodeIterator(@args);
}

sub createTreeWalker {
    my ($self,$root,$what,$filter,$expand) = @_;
    my $callback = XML::Xerces::PerlNodeFilterCallbackHandler->new($filter);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__NODE_FILTER} = $callback;

    my @args = ($self,$root,$what,$callback,$expand);
    if ($args[0]->isa('XML::Xerces::DOMDocument')) {

    }
    return XML::Xercesc::DOMDocumentTraversal_createTreeWalker(@args);
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMNodeIterator ##############

package XML::Xerces::DOMNodeIterator;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMNodeIterator($self);
        delete $OWNER{$self};
    }
}

*getRoot = *XML::Xercesc::DOMNodeIterator_getRoot;
*getWhatToShow = *XML::Xercesc::DOMNodeIterator_getWhatToShow;
*getFilter = *XML::Xercesc::DOMNodeIterator_getFilter;
*getExpandEntityReferences = *XML::Xercesc::DOMNodeIterator_getExpandEntityReferences;
*nextNode = *XML::Xercesc::DOMNodeIterator_nextNode;
*previousNode = *XML::Xercesc::DOMNodeIterator_previousNode;
*detach = *XML::Xercesc::DOMNodeIterator_detach;
*release = *XML::Xercesc::DOMNodeIterator_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMNodeFilter ##############

package XML::Xerces::DOMNodeFilter;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMNodeFilter($self);
        delete $OWNER{$self};
    }
}

*FILTER_ACCEPT = *XML::Xercesc::DOMNodeFilter_FILTER_ACCEPT;
*FILTER_REJECT = *XML::Xercesc::DOMNodeFilter_FILTER_REJECT;
*FILTER_SKIP = *XML::Xercesc::DOMNodeFilter_FILTER_SKIP;
*SHOW_ALL = *XML::Xercesc::DOMNodeFilter_SHOW_ALL;
*SHOW_ELEMENT = *XML::Xercesc::DOMNodeFilter_SHOW_ELEMENT;
*SHOW_ATTRIBUTE = *XML::Xercesc::DOMNodeFilter_SHOW_ATTRIBUTE;
*SHOW_TEXT = *XML::Xercesc::DOMNodeFilter_SHOW_TEXT;
*SHOW_CDATA_SECTION = *XML::Xercesc::DOMNodeFilter_SHOW_CDATA_SECTION;
*SHOW_ENTITY_REFERENCE = *XML::Xercesc::DOMNodeFilter_SHOW_ENTITY_REFERENCE;
*SHOW_ENTITY = *XML::Xercesc::DOMNodeFilter_SHOW_ENTITY;
*SHOW_PROCESSING_INSTRUCTION = *XML::Xercesc::DOMNodeFilter_SHOW_PROCESSING_INSTRUCTION;
*SHOW_COMMENT = *XML::Xercesc::DOMNodeFilter_SHOW_COMMENT;
*SHOW_DOCUMENT = *XML::Xercesc::DOMNodeFilter_SHOW_DOCUMENT;
*SHOW_DOCUMENT_TYPE = *XML::Xercesc::DOMNodeFilter_SHOW_DOCUMENT_TYPE;
*SHOW_DOCUMENT_FRAGMENT = *XML::Xercesc::DOMNodeFilter_SHOW_DOCUMENT_FRAGMENT;
*SHOW_NOTATION = *XML::Xercesc::DOMNodeFilter_SHOW_NOTATION;
*acceptNode = *XML::Xercesc::DOMNodeFilter_acceptNode;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMRange ##############

package XML::Xerces::DOMRange;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMRange($self);
        delete $OWNER{$self};
    }
}

*START_TO_START = *XML::Xercesc::DOMRange_START_TO_START;
*START_TO_END = *XML::Xercesc::DOMRange_START_TO_END;
*END_TO_END = *XML::Xercesc::DOMRange_END_TO_END;
*END_TO_START = *XML::Xercesc::DOMRange_END_TO_START;
*getStartContainer = *XML::Xercesc::DOMRange_getStartContainer;
*getStartOffset = *XML::Xercesc::DOMRange_getStartOffset;
*getEndContainer = *XML::Xercesc::DOMRange_getEndContainer;
*getEndOffset = *XML::Xercesc::DOMRange_getEndOffset;
*getCollapsed = *XML::Xercesc::DOMRange_getCollapsed;
*getCommonAncestorContainer = *XML::Xercesc::DOMRange_getCommonAncestorContainer;
*setStart = *XML::Xercesc::DOMRange_setStart;
*setEnd = *XML::Xercesc::DOMRange_setEnd;
*setStartBefore = *XML::Xercesc::DOMRange_setStartBefore;
*setStartAfter = *XML::Xercesc::DOMRange_setStartAfter;
*setEndBefore = *XML::Xercesc::DOMRange_setEndBefore;
*setEndAfter = *XML::Xercesc::DOMRange_setEndAfter;
*collapse = *XML::Xercesc::DOMRange_collapse;
*selectNode = *XML::Xercesc::DOMRange_selectNode;
*selectNodeContents = *XML::Xercesc::DOMRange_selectNodeContents;
*compareBoundaryPoints = *XML::Xercesc::DOMRange_compareBoundaryPoints;
*deleteContents = *XML::Xercesc::DOMRange_deleteContents;
*extractContents = *XML::Xercesc::DOMRange_extractContents;
*cloneContents = *XML::Xercesc::DOMRange_cloneContents;
*insertNode = *XML::Xercesc::DOMRange_insertNode;
*surroundContents = *XML::Xercesc::DOMRange_surroundContents;
*cloneRange = *XML::Xercesc::DOMRange_cloneRange;
*toString = *XML::Xercesc::DOMRange_toString;
*detach = *XML::Xercesc::DOMRange_detach;
*release = *XML::Xercesc::DOMRange_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMRangeException ##############

package XML::Xerces::DOMRangeException;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMException XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*BAD_BOUNDARYPOINTS_ERR = *XML::Xercesc::DOMRangeException_BAD_BOUNDARYPOINTS_ERR;
*INVALID_NODE_TYPE_ERR = *XML::Xercesc::DOMRangeException_INVALID_NODE_TYPE_ERR;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_DOMRangeException(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMRangeException($self);
        delete $OWNER{$self};
    }
}

*swig_code_get = *XML::Xercesc::DOMRangeException_code_get;
*swig_code_set = *XML::Xercesc::DOMRangeException_code_set;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMTreeWalker ##############

package XML::Xerces::DOMTreeWalker;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMTreeWalker($self);
        delete $OWNER{$self};
    }
}

*getRoot = *XML::Xercesc::DOMTreeWalker_getRoot;
*getWhatToShow = *XML::Xercesc::DOMTreeWalker_getWhatToShow;
*getFilter = *XML::Xercesc::DOMTreeWalker_getFilter;
*getExpandEntityReferences = *XML::Xercesc::DOMTreeWalker_getExpandEntityReferences;
*getCurrentNode = *XML::Xercesc::DOMTreeWalker_getCurrentNode;
*parentNode = *XML::Xercesc::DOMTreeWalker_parentNode;
*firstChild = *XML::Xercesc::DOMTreeWalker_firstChild;
*lastChild = *XML::Xercesc::DOMTreeWalker_lastChild;
*previousSibling = *XML::Xercesc::DOMTreeWalker_previousSibling;
*nextSibling = *XML::Xercesc::DOMTreeWalker_nextSibling;
*previousNode = *XML::Xercesc::DOMTreeWalker_previousNode;
*nextNode = *XML::Xercesc::DOMTreeWalker_nextNode;
*setCurrentNode = *XML::Xercesc::DOMTreeWalker_setCurrentNode;
*release = *XML::Xercesc::DOMTreeWalker_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMConfiguration ##############

package XML::Xerces::DOMConfiguration;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*setParameter = *XML::Xercesc::DOMConfiguration_setParameter;
*getParameter = *XML::Xercesc::DOMConfiguration_getParameter;
*canSetParameter = *XML::Xercesc::DOMConfiguration_canSetParameter;
*getParameterNames = *XML::Xercesc::DOMConfiguration_getParameterNames;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMConfiguration($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMImplementationLS ##############

package XML::Xerces::DOMImplementationLS;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMImplementationLS($self);
        delete $OWNER{$self};
    }
}

*MODE_SYNCHRONOUS = *XML::Xercesc::DOMImplementationLS_MODE_SYNCHRONOUS;
*MODE_ASYNCHRONOUS = *XML::Xercesc::DOMImplementationLS_MODE_ASYNCHRONOUS;
*createLSParser = *XML::Xercesc::DOMImplementationLS_createLSParser;
*createLSSerializer = *XML::Xercesc::DOMImplementationLS_createLSSerializer;
*createLSInput = *XML::Xercesc::DOMImplementationLS_createLSInput;
*createLSOutput = *XML::Xercesc::DOMImplementationLS_createLSOutput;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMImplementation ##############

package XML::Xerces::DOMImplementation;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMImplementationLS XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMImplementation($self);
        delete $OWNER{$self};
    }
}

*hasFeature = *XML::Xercesc::DOMImplementation_hasFeature;
*createDocumentType = *XML::Xercesc::DOMImplementation_createDocumentType;
*getFeature = *XML::Xercesc::DOMImplementation_getFeature;
sub createDocument {
    if (UNIVERSAL::isa($_[3],"XML::Xerces::DOMDocumentType")) {
        $_[3]->DISOWN();
    }

    my $return = XML::Xercesc::DOMImplementation_createDocument(@_);
    
    return $return;
}

*getImplementation = *XML::Xercesc::DOMImplementation_getImplementation;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMImplementationSource ##############

package XML::Xerces::DOMImplementationSource;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMImplementationSource($self);
        delete $OWNER{$self};
    }
}

*getDOMImplementation = *XML::Xercesc::DOMImplementationSource_getDOMImplementation;
*getDOMImplementationList = *XML::Xercesc::DOMImplementationSource_getDOMImplementationList;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMImplementationRegistry ##############

package XML::Xerces::DOMImplementationRegistry;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*getDOMImplementation = *XML::Xercesc::DOMImplementationRegistry_getDOMImplementation;
*getDOMImplementationList = *XML::Xercesc::DOMImplementationRegistry_getDOMImplementationList;
*addSource = *XML::Xercesc::DOMImplementationRegistry_addSource;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMImplementationRegistry($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMErrorHandler ##############

package XML::Xerces::DOMErrorHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMErrorHandler($self);
        delete $OWNER{$self};
    }
}

*handleError = *XML::Xercesc::DOMErrorHandler_handleError;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMDocument ##############

package XML::Xerces::DOMDocument;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMDocumentRange XML::Xerces::DOMDocumentTraversal XML::Xerces::DOMNode XML::Xerces );
%OWNER = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;

    if ($self->can('cleanup')) {
        $_[0]->cleanup();
    }

    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMDocument($self);
        delete $OWNER{$self};
    }
}

*createElement = *XML::Xercesc::DOMDocument_createElement;
*createDocumentFragment = *XML::Xercesc::DOMDocument_createDocumentFragment;
*createTextNode = *XML::Xercesc::DOMDocument_createTextNode;
*createComment = *XML::Xercesc::DOMDocument_createComment;
*createCDATASection = *XML::Xercesc::DOMDocument_createCDATASection;
*createProcessingInstruction = *XML::Xercesc::DOMDocument_createProcessingInstruction;
*createAttribute = *XML::Xercesc::DOMDocument_createAttribute;
*createEntityReference = *XML::Xercesc::DOMDocument_createEntityReference;
*getDoctype = *XML::Xercesc::DOMDocument_getDoctype;
*getImplementation = *XML::Xercesc::DOMDocument_getImplementation;
*getDocumentElement = *XML::Xercesc::DOMDocument_getDocumentElement;
sub getElementsByTagName {
    my $result = XML::Xercesc::DOMDocument_getElementsByTagName (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
*importNode = *XML::Xercesc::DOMDocument_importNode;
*createAttributeNS = *XML::Xercesc::DOMDocument_createAttributeNS;
sub getElementsByTagNameNS {
    my $result = XML::Xercesc::DOMDocument_getElementsByTagNameNS (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
*getElementById = *XML::Xercesc::DOMDocument_getElementById;
*getInputEncoding = *XML::Xercesc::DOMDocument_getInputEncoding;
*getXmlEncoding = *XML::Xercesc::DOMDocument_getXmlEncoding;
*getXmlStandalone = *XML::Xercesc::DOMDocument_getXmlStandalone;
*setXmlStandalone = *XML::Xercesc::DOMDocument_setXmlStandalone;
*getXmlVersion = *XML::Xercesc::DOMDocument_getXmlVersion;
*setXmlVersion = *XML::Xercesc::DOMDocument_setXmlVersion;
*getDocumentURI = *XML::Xercesc::DOMDocument_getDocumentURI;
*setDocumentURI = *XML::Xercesc::DOMDocument_setDocumentURI;
*getStrictErrorChecking = *XML::Xercesc::DOMDocument_getStrictErrorChecking;
*setStrictErrorChecking = *XML::Xercesc::DOMDocument_setStrictErrorChecking;
*renameNode = *XML::Xercesc::DOMDocument_renameNode;
*adoptNode = *XML::Xercesc::DOMDocument_adoptNode;
*normalizeDocument = *XML::Xercesc::DOMDocument_normalizeDocument;
*getDOMConfig = *XML::Xercesc::DOMDocument_getDOMConfig;
*createEntity = *XML::Xercesc::DOMDocument_createEntity;
*createDocumentType = *XML::Xercesc::DOMDocument_createDocumentType;
*createNotation = *XML::Xercesc::DOMDocument_createNotation;
*createElementNS = *XML::Xercesc::DOMDocument_createElementNS;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLocator ##############

package XML::Xerces::DOMLocator;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLocator($self);
        delete $OWNER{$self};
    }
}

*getLineNumber = *XML::Xercesc::DOMLocator_getLineNumber;
*getColumnNumber = *XML::Xercesc::DOMLocator_getColumnNumber;
*getByteOffset = *XML::Xercesc::DOMLocator_getByteOffset;
*getUtf16Offset = *XML::Xercesc::DOMLocator_getUtf16Offset;
*getRelatedNode = *XML::Xercesc::DOMLocator_getRelatedNode;
*getURI = *XML::Xercesc::DOMLocator_getURI;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSResourceResolver ##############

package XML::Xerces::DOMLSResourceResolver;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSResourceResolver($self);
        delete $OWNER{$self};
    }
}

*resolveResource = *XML::Xercesc::DOMLSResourceResolver_resolveResource;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSInput ##############

package XML::Xerces::DOMLSInput;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSInput($self);
        delete $OWNER{$self};
    }
}

*getStringData = *XML::Xercesc::DOMLSInput_getStringData;
*getByteStream = *XML::Xercesc::DOMLSInput_getByteStream;
*getEncoding = *XML::Xercesc::DOMLSInput_getEncoding;
*getPublicId = *XML::Xercesc::DOMLSInput_getPublicId;
*getSystemId = *XML::Xercesc::DOMLSInput_getSystemId;
*getBaseURI = *XML::Xercesc::DOMLSInput_getBaseURI;
*setStringData = *XML::Xercesc::DOMLSInput_setStringData;
*setByteStream = *XML::Xercesc::DOMLSInput_setByteStream;
*setEncoding = *XML::Xercesc::DOMLSInput_setEncoding;
*setPublicId = *XML::Xercesc::DOMLSInput_setPublicId;
*setSystemId = *XML::Xercesc::DOMLSInput_setSystemId;
*setBaseURI = *XML::Xercesc::DOMLSInput_setBaseURI;
*setIssueFatalErrorIfNotFound = *XML::Xercesc::DOMLSInput_setIssueFatalErrorIfNotFound;
*getIssueFatalErrorIfNotFound = *XML::Xercesc::DOMLSInput_getIssueFatalErrorIfNotFound;
*release = *XML::Xercesc::DOMLSInput_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::Wrapper4InputSource ##############

package XML::Xerces::Wrapper4InputSource;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMLSInput XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    # we never handle the memory
    # so we tell Perl to forget about the InputSource
    $_[0]->DISOWN();

    my $self = XML::Xercesc::new_Wrapper4InputSource(@_);
    
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_Wrapper4InputSource($self);
        delete $OWNER{$self};
    }
}

*getStringData = *XML::Xercesc::Wrapper4InputSource_getStringData;
*getByteStream = *XML::Xercesc::Wrapper4InputSource_getByteStream;
*getEncoding = *XML::Xercesc::Wrapper4InputSource_getEncoding;
*getPublicId = *XML::Xercesc::Wrapper4InputSource_getPublicId;
*getSystemId = *XML::Xercesc::Wrapper4InputSource_getSystemId;
*getBaseURI = *XML::Xercesc::Wrapper4InputSource_getBaseURI;
*getIssueFatalErrorIfNotFound = *XML::Xercesc::Wrapper4InputSource_getIssueFatalErrorIfNotFound;
*setStringData = *XML::Xercesc::Wrapper4InputSource_setStringData;
*setByteStream = *XML::Xercesc::Wrapper4InputSource_setByteStream;
*setEncoding = *XML::Xercesc::Wrapper4InputSource_setEncoding;
*setPublicId = *XML::Xercesc::Wrapper4InputSource_setPublicId;
*setSystemId = *XML::Xercesc::Wrapper4InputSource_setSystemId;
*setBaseURI = *XML::Xercesc::Wrapper4InputSource_setBaseURI;
*setIssueFatalErrorIfNotFound = *XML::Xercesc::Wrapper4InputSource_setIssueFatalErrorIfNotFound;
*release = *XML::Xercesc::Wrapper4InputSource_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::Wrapper4DOMLSInput ##############

package XML::Xerces::Wrapper4DOMLSInput;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::InputSource XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_Wrapper4DOMLSInput(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_Wrapper4DOMLSInput($self);
        delete $OWNER{$self};
    }
}

*makeStream = *XML::Xercesc::Wrapper4DOMLSInput_makeStream;
*getEncoding = *XML::Xercesc::Wrapper4DOMLSInput_getEncoding;
*getPublicId = *XML::Xercesc::Wrapper4DOMLSInput_getPublicId;
*getSystemId = *XML::Xercesc::Wrapper4DOMLSInput_getSystemId;
*getIssueFatalErrorIfNotFound = *XML::Xercesc::Wrapper4DOMLSInput_getIssueFatalErrorIfNotFound;
*setEncoding = *XML::Xercesc::Wrapper4DOMLSInput_setEncoding;
*setPublicId = *XML::Xercesc::Wrapper4DOMLSInput_setPublicId;
*setSystemId = *XML::Xercesc::Wrapper4DOMLSInput_setSystemId;
*setIssueFatalErrorIfNotFound = *XML::Xercesc::Wrapper4DOMLSInput_setIssueFatalErrorIfNotFound;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSParser ##############

package XML::Xerces::DOMLSParser;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSParser($self);
        delete $OWNER{$self};
    }
}

*ACTION_APPEND_AS_CHILDREN = *XML::Xercesc::DOMLSParser_ACTION_APPEND_AS_CHILDREN;
*ACTION_REPLACE_CHILDREN = *XML::Xercesc::DOMLSParser_ACTION_REPLACE_CHILDREN;
*ACTION_INSERT_BEFORE = *XML::Xercesc::DOMLSParser_ACTION_INSERT_BEFORE;
*ACTION_INSERT_AFTER = *XML::Xercesc::DOMLSParser_ACTION_INSERT_AFTER;
*ACTION_REPLACE = *XML::Xercesc::DOMLSParser_ACTION_REPLACE;
*getDomConfig = *XML::Xercesc::DOMLSParser_getDomConfig;
*getAsync = *XML::Xercesc::DOMLSParser_getAsync;
*getBusy = *XML::Xercesc::DOMLSParser_getBusy;
*setFilter = *XML::Xercesc::DOMLSParser_setFilter;
*parse = *XML::Xercesc::DOMLSParser_parse;
*parseURI = *XML::Xercesc::DOMLSParser_parseURI;
*parseWithContext = *XML::Xercesc::DOMLSParser_parseWithContext;
*abort = *XML::Xercesc::DOMLSParser_abort;
*release = *XML::Xercesc::DOMLSParser_release;
*resetDocumentPool = *XML::Xercesc::DOMLSParser_resetDocumentPool;
*loadGrammar = *XML::Xercesc::DOMLSParser_loadGrammar;
*getGrammar = *XML::Xercesc::DOMLSParser_getGrammar;
*getRootGrammar = *XML::Xercesc::DOMLSParser_getRootGrammar;
*getURIText = *XML::Xercesc::DOMLSParser_getURIText;
*resetCachedGrammarPool = *XML::Xercesc::DOMLSParser_resetCachedGrammarPool;
*getSrcOffset = *XML::Xercesc::DOMLSParser_getSrcOffset;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSOutput ##############

package XML::Xerces::DOMLSOutput;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSOutput($self);
        delete $OWNER{$self};
    }
}

*getByteStream = *XML::Xercesc::DOMLSOutput_getByteStream;
*getEncoding = *XML::Xercesc::DOMLSOutput_getEncoding;
*getSystemId = *XML::Xercesc::DOMLSOutput_getSystemId;
*setByteStream = *XML::Xercesc::DOMLSOutput_setByteStream;
*setEncoding = *XML::Xercesc::DOMLSOutput_setEncoding;
*setSystemId = *XML::Xercesc::DOMLSOutput_setSystemId;
*release = *XML::Xercesc::DOMLSOutput_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::StdOutFormatTarget ##############

package XML::Xerces::StdOutFormatTarget;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_StdOutFormatTarget(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_StdOutFormatTarget($self);
        delete $OWNER{$self};
    }
}

*writeChars = *XML::Xercesc::StdOutFormatTarget_writeChars;
*flush = *XML::Xercesc::StdOutFormatTarget_flush;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::LocalFileFormatTarget ##############

package XML::Xerces::LocalFileFormatTarget;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_LocalFileFormatTarget(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_LocalFileFormatTarget($self);
        delete $OWNER{$self};
    }
}

*writeChars = *XML::Xercesc::LocalFileFormatTarget_writeChars;
*flush = *XML::Xercesc::LocalFileFormatTarget_flush;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::MemBufFormatTarget ##############

package XML::Xerces::MemBufFormatTarget;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_MemBufFormatTarget(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_MemBufFormatTarget($self);
        delete $OWNER{$self};
    }
}

*writeChars = *XML::Xercesc::MemBufFormatTarget_writeChars;
*getRawBuffer = *XML::Xercesc::MemBufFormatTarget_getRawBuffer;
*getLen = *XML::Xercesc::MemBufFormatTarget_getLen;
*reset = *XML::Xercesc::MemBufFormatTarget_reset;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSSerializer ##############

package XML::Xerces::DOMLSSerializer;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSSerializer($self);
        delete $OWNER{$self};
    }
}

*getDomConfig = *XML::Xercesc::DOMLSSerializer_getDomConfig;
*setNewLine = *XML::Xercesc::DOMLSSerializer_setNewLine;
*setFilter = *XML::Xercesc::DOMLSSerializer_setFilter;
*getNewLine = *XML::Xercesc::DOMLSSerializer_getNewLine;
*write = *XML::Xercesc::DOMLSSerializer_write;
*writeToURI = *XML::Xercesc::DOMLSSerializer_writeToURI;
*writeToString = *XML::Xercesc::DOMLSSerializer_writeToString;
*release = *XML::Xercesc::DOMLSSerializer_release;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DOMLSSerializerFilter ##############

package XML::Xerces::DOMLSSerializerFilter;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNodeFilter XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DOMLSSerializerFilter($self);
        delete $OWNER{$self};
    }
}

*acceptNode = *XML::Xercesc::DOMLSSerializerFilter_acceptNode;
*getWhatToShow = *XML::Xercesc::DOMLSSerializerFilter_getWhatToShow;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::SAXException ##############

package XML::Xerces::SAXException;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SAXException($self);
        delete $OWNER{$self};
    }
}

*getMessage = *XML::Xercesc::SAXException_getMessage;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::SAXNotSupportedException ##############

package XML::Xerces::SAXNotSupportedException;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::SAXException XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_SAXNotSupportedException(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SAXNotSupportedException($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::SAXNotRecognizedException ##############

package XML::Xerces::SAXNotRecognizedException;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::SAXException XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_SAXNotRecognizedException(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SAXNotRecognizedException($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::SAXParseException ##############

package XML::Xerces::SAXParseException;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::SAXException XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SAXParseException($self);
        delete $OWNER{$self};
    }
}

*getColumnNumber = *XML::Xercesc::SAXParseException_getColumnNumber;
*getLineNumber = *XML::Xercesc::SAXParseException_getLineNumber;
*getPublicId = *XML::Xercesc::SAXParseException_getPublicId;
*getSystemId = *XML::Xercesc::SAXParseException_getSystemId;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::ErrorHandler ##############

package XML::Xerces::ErrorHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_ErrorHandler($self);
        delete $OWNER{$self};
    }
}

*warning = *XML::Xercesc::ErrorHandler_warning;
*error = *XML::Xercesc::ErrorHandler_error;
*fatalError = *XML::Xercesc::ErrorHandler_fatalError;
*resetErrors = *XML::Xercesc::ErrorHandler_resetErrors;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DTDHandler ##############

package XML::Xerces::DTDHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DTDHandler($self);
        delete $OWNER{$self};
    }
}

*unparsedEntityDecl = *XML::Xercesc::DTDHandler_unparsedEntityDecl;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DocumentHandler ##############

package XML::Xerces::DocumentHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DocumentHandler($self);
        delete $OWNER{$self};
    }
}

*characters = *XML::Xercesc::DocumentHandler_characters;
*processingInstruction = *XML::Xercesc::DocumentHandler_processingInstruction;
*setDocumentLocator = *XML::Xercesc::DocumentHandler_setDocumentLocator;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::EntityResolver ##############

package XML::Xerces::EntityResolver;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_EntityResolver($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::AttributeList ##############

package XML::Xerces::AttributeList;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_AttributeList($self);
        delete $OWNER{$self};
    }
}

*getLength = *XML::Xercesc::AttributeList_getLength;
*getName = *XML::Xercesc::AttributeList_getName;
*getType = *XML::Xercesc::AttributeList_getType;
*getValue = *XML::Xercesc::AttributeList_getValue;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::HandlerBase ##############

package XML::Xerces::HandlerBase;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::EntityResolver XML::Xerces::DTDHandler XML::Xerces::DocumentHandler XML::Xerces::ErrorHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*characters = *XML::Xercesc::HandlerBase_characters;
*processingInstruction = *XML::Xercesc::HandlerBase_processingInstruction;
*setDocumentLocator = *XML::Xercesc::HandlerBase_setDocumentLocator;
*error = *XML::Xercesc::HandlerBase_error;
*fatalError = *XML::Xercesc::HandlerBase_fatalError;
*warning = *XML::Xercesc::HandlerBase_warning;
*resetErrors = *XML::Xercesc::HandlerBase_resetErrors;
*unparsedEntityDecl = *XML::Xercesc::HandlerBase_unparsedEntityDecl;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_HandlerBase(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_HandlerBase($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::Locator ##############

package XML::Xerces::Locator;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_Locator($self);
        delete $OWNER{$self};
    }
}

*getPublicId = *XML::Xercesc::Locator_getPublicId;
*getSystemId = *XML::Xercesc::Locator_getSystemId;
*getLineNumber = *XML::Xercesc::Locator_getLineNumber;
*getColumnNumber = *XML::Xercesc::Locator_getColumnNumber;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::Attributes ##############

package XML::Xerces::Attributes;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_Attributes($self);
        delete $OWNER{$self};
    }
}

*getLength = *XML::Xercesc::Attributes_getLength;
*getURI = *XML::Xercesc::Attributes_getURI;
*getLocalName = *XML::Xercesc::Attributes_getLocalName;
*getQName = *XML::Xercesc::Attributes_getQName;
*getIndex = *XML::Xercesc::Attributes_getIndex;
*getType = *XML::Xercesc::Attributes_getType;
*getValue = *XML::Xercesc::Attributes_getValue;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::ContentHandler ##############

package XML::Xerces::ContentHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_ContentHandler($self);
        delete $OWNER{$self};
    }
}

*characters = *XML::Xercesc::ContentHandler_characters;
*processingInstruction = *XML::Xercesc::ContentHandler_processingInstruction;
*setDocumentLocator = *XML::Xercesc::ContentHandler_setDocumentLocator;
*startPrefixMapping = *XML::Xercesc::ContentHandler_startPrefixMapping;
*endPrefixMapping = *XML::Xercesc::ContentHandler_endPrefixMapping;
*skippedEntity = *XML::Xercesc::ContentHandler_skippedEntity;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::LexicalHandler ##############

package XML::Xerces::LexicalHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_LexicalHandler($self);
        delete $OWNER{$self};
    }
}

*comment = *XML::Xercesc::LexicalHandler_comment;
*endCDATA = *XML::Xercesc::LexicalHandler_endCDATA;
*endDTD = *XML::Xercesc::LexicalHandler_endDTD;
*endEntity = *XML::Xercesc::LexicalHandler_endEntity;
*startCDATA = *XML::Xercesc::LexicalHandler_startCDATA;
*startDTD = *XML::Xercesc::LexicalHandler_startDTD;
*startEntity = *XML::Xercesc::LexicalHandler_startEntity;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DeclHandler ##############

package XML::Xerces::DeclHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DeclHandler($self);
        delete $OWNER{$self};
    }
}

*attributeDecl = *XML::Xercesc::DeclHandler_attributeDecl;
*internalEntityDecl = *XML::Xercesc::DeclHandler_internalEntityDecl;
*externalEntityDecl = *XML::Xercesc::DeclHandler_externalEntityDecl;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::DefaultHandler ##############

package XML::Xerces::DefaultHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::EntityResolver XML::Xerces::DTDHandler XML::Xerces::ContentHandler XML::Xerces::ErrorHandler XML::Xerces::LexicalHandler XML::Xerces::DeclHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*characters = *XML::Xercesc::DefaultHandler_characters;
*processingInstruction = *XML::Xercesc::DefaultHandler_processingInstruction;
*setDocumentLocator = *XML::Xercesc::DefaultHandler_setDocumentLocator;
*startPrefixMapping = *XML::Xercesc::DefaultHandler_startPrefixMapping;
*endPrefixMapping = *XML::Xercesc::DefaultHandler_endPrefixMapping;
*skippedEntity = *XML::Xercesc::DefaultHandler_skippedEntity;
*error = *XML::Xercesc::DefaultHandler_error;
*fatalError = *XML::Xercesc::DefaultHandler_fatalError;
*warning = *XML::Xercesc::DefaultHandler_warning;
*resetErrors = *XML::Xercesc::DefaultHandler_resetErrors;
*unparsedEntityDecl = *XML::Xercesc::DefaultHandler_unparsedEntityDecl;
*comment = *XML::Xercesc::DefaultHandler_comment;
*endCDATA = *XML::Xercesc::DefaultHandler_endCDATA;
*endDTD = *XML::Xercesc::DefaultHandler_endDTD;
*endEntity = *XML::Xercesc::DefaultHandler_endEntity;
*startCDATA = *XML::Xercesc::DefaultHandler_startCDATA;
*startDTD = *XML::Xercesc::DefaultHandler_startDTD;
*startEntity = *XML::Xercesc::DefaultHandler_startEntity;
*attributeDecl = *XML::Xercesc::DefaultHandler_attributeDecl;
*internalEntityDecl = *XML::Xercesc::DefaultHandler_internalEntityDecl;
*externalEntityDecl = *XML::Xercesc::DefaultHandler_externalEntityDecl;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_DefaultHandler(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_DefaultHandler($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLPScanToken ##############

package XML::Xerces::XMLPScanToken;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_XMLPScanToken(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLPScanToken($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::PSVIHandler ##############

package XML::Xerces::PSVIHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PSVIHandler($self);
        delete $OWNER{$self};
    }
}

*handleElementPSVI = *XML::Xercesc::PSVIHandler_handleElementPSVI;
*handlePartialElementPSVI = *XML::Xercesc::PSVIHandler_handlePartialElementPSVI;
*handleAttributesPSVI = *XML::Xercesc::PSVIHandler_handleAttributesPSVI;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::Parser ##############

package XML::Xerces::Parser;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_Parser($self);
        delete $OWNER{$self};
    }
}

sub setEntityResolver {
  my ($self,$handler) = @_;
  my $callback = XML::Xerces::PerlEntityResolverHandler->new($handler);
  $XML::Xerces::REMEMBER{tied(% {$self})}->{__ENTITY_RESOLVER} = $callback;

  return XML::Xercesc::Parser_setEntityResolver($self,$callback);
}

*setDTDHandler = *XML::Xercesc::Parser_setDTDHandler;
*setDocumentHandler = *XML::Xercesc::Parser_setDocumentHandler;
sub setErrorHandler {
  my ($self,$handler) = @_;
  my $callback = XML::Xerces::PerlErrorCallbackHandler->new($handler);
  $XML::Xerces::REMEMBER{tied(% {$self})}->{__ERROR_HANDLER} = $callback;

  return XML::Xercesc::Parser_setErrorHandler($self,$callback);
}

*parse = *XML::Xercesc::Parser_parse;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::AbstractDOMParser ##############

package XML::Xerces::AbstractDOMParser;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::PSVIHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*Val_Never = *XML::Xercesc::AbstractDOMParser_Val_Never;
*Val_Always = *XML::Xercesc::AbstractDOMParser_Val_Always;
*Val_Auto = *XML::Xercesc::AbstractDOMParser_Val_Auto;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_AbstractDOMParser($self);
        delete $OWNER{$self};
    }
}

*reset = *XML::Xercesc::AbstractDOMParser_reset;
*adoptDocument = *XML::Xercesc::AbstractDOMParser_adoptDocument;
# hold a reference to the parser internally, so that the
# document can exist after the parser has gone out of scope
sub getDocument {
  my ($self) = @_;
  my $result = XML::Xercesc::AbstractDOMParser_getDocument($self);

  # the DOMDocument destructor will automatically delete this entry if we
  # store it using the underlying tied() object
  $XML::Xerces::REMEMBER{tied(% {$result})}->{__PARSER} = $self;
  return $result;
}

*getValidator = *XML::Xercesc::AbstractDOMParser_getValidator;
*getValidationScheme = *XML::Xercesc::AbstractDOMParser_getValidationScheme;
*getDoSchema = *XML::Xercesc::AbstractDOMParser_getDoSchema;
*getValidationSchemaFullChecking = *XML::Xercesc::AbstractDOMParser_getValidationSchemaFullChecking;
*getIdentityConstraintChecking = *XML::Xercesc::AbstractDOMParser_getIdentityConstraintChecking;
*getErrorCount = *XML::Xercesc::AbstractDOMParser_getErrorCount;
*getDoNamespaces = *XML::Xercesc::AbstractDOMParser_getDoNamespaces;
*getExitOnFirstFatalError = *XML::Xercesc::AbstractDOMParser_getExitOnFirstFatalError;
*getValidationConstraintFatal = *XML::Xercesc::AbstractDOMParser_getValidationConstraintFatal;
*getCreateEntityReferenceNodes = *XML::Xercesc::AbstractDOMParser_getCreateEntityReferenceNodes;
*getIncludeIgnorableWhitespace = *XML::Xercesc::AbstractDOMParser_getIncludeIgnorableWhitespace;
*getExternalSchemaLocation = *XML::Xercesc::AbstractDOMParser_getExternalSchemaLocation;
*getExternalNoNamespaceSchemaLocation = *XML::Xercesc::AbstractDOMParser_getExternalNoNamespaceSchemaLocation;
*getSecurityManager = *XML::Xercesc::AbstractDOMParser_getSecurityManager;
*getLoadExternalDTD = *XML::Xercesc::AbstractDOMParser_getLoadExternalDTD;
*getCreateCommentNodes = *XML::Xercesc::AbstractDOMParser_getCreateCommentNodes;
*getCalculateSrcOfs = *XML::Xercesc::AbstractDOMParser_getCalculateSrcOfs;
*getStandardUriConformant = *XML::Xercesc::AbstractDOMParser_getStandardUriConformant;
*getPSVIHandler = *XML::Xercesc::AbstractDOMParser_getPSVIHandler;
*getCreateSchemaInfo = *XML::Xercesc::AbstractDOMParser_getCreateSchemaInfo;
*getGenerateSyntheticAnnotations = *XML::Xercesc::AbstractDOMParser_getGenerateSyntheticAnnotations;
*getValidateAnnotations = *XML::Xercesc::AbstractDOMParser_getValidateAnnotations;
*getIgnoreAnnotations = *XML::Xercesc::AbstractDOMParser_getIgnoreAnnotations;
*getDisableDefaultEntityResolution = *XML::Xercesc::AbstractDOMParser_getDisableDefaultEntityResolution;
*getSkipDTDValidation = *XML::Xercesc::AbstractDOMParser_getSkipDTDValidation;
*setGenerateSyntheticAnnotations = *XML::Xercesc::AbstractDOMParser_setGenerateSyntheticAnnotations;
*setValidateAnnotations = *XML::Xercesc::AbstractDOMParser_setValidateAnnotations;
*setDoNamespaces = *XML::Xercesc::AbstractDOMParser_setDoNamespaces;
*setExitOnFirstFatalError = *XML::Xercesc::AbstractDOMParser_setExitOnFirstFatalError;
*setValidationConstraintFatal = *XML::Xercesc::AbstractDOMParser_setValidationConstraintFatal;
*setCreateEntityReferenceNodes = *XML::Xercesc::AbstractDOMParser_setCreateEntityReferenceNodes;
*setIncludeIgnorableWhitespace = *XML::Xercesc::AbstractDOMParser_setIncludeIgnorableWhitespace;
*setValidationScheme = *XML::Xercesc::AbstractDOMParser_setValidationScheme;
*setDoSchema = *XML::Xercesc::AbstractDOMParser_setDoSchema;
*setValidationSchemaFullChecking = *XML::Xercesc::AbstractDOMParser_setValidationSchemaFullChecking;
*setIdentityConstraintChecking = *XML::Xercesc::AbstractDOMParser_setIdentityConstraintChecking;
*setExternalSchemaLocation = *XML::Xercesc::AbstractDOMParser_setExternalSchemaLocation;
*setExternalNoNamespaceSchemaLocation = *XML::Xercesc::AbstractDOMParser_setExternalNoNamespaceSchemaLocation;
*setSecurityManager = *XML::Xercesc::AbstractDOMParser_setSecurityManager;
*setLoadExternalDTD = *XML::Xercesc::AbstractDOMParser_setLoadExternalDTD;
*setCreateCommentNodes = *XML::Xercesc::AbstractDOMParser_setCreateCommentNodes;
*setCalculateSrcOfs = *XML::Xercesc::AbstractDOMParser_setCalculateSrcOfs;
*setStandardUriConformant = *XML::Xercesc::AbstractDOMParser_setStandardUriConformant;
*useScanner = *XML::Xercesc::AbstractDOMParser_useScanner;
*useImplementation = *XML::Xercesc::AbstractDOMParser_useImplementation;
*setPSVIHandler = *XML::Xercesc::AbstractDOMParser_setPSVIHandler;
*setCreateSchemaInfo = *XML::Xercesc::AbstractDOMParser_setCreateSchemaInfo;
*setIgnoreAnnotations = *XML::Xercesc::AbstractDOMParser_setIgnoreAnnotations;
*setDisableDefaultEntityResolution = *XML::Xercesc::AbstractDOMParser_setDisableDefaultEntityResolution;
*setSkipDTDValidation = *XML::Xercesc::AbstractDOMParser_setSkipDTDValidation;
*parse = *XML::Xercesc::AbstractDOMParser_parse;
*parseFirst = *XML::Xercesc::AbstractDOMParser_parseFirst;
*parseNext = *XML::Xercesc::AbstractDOMParser_parseNext;
*parseReset = *XML::Xercesc::AbstractDOMParser_parseReset;
*handleElementPSVI = *XML::Xercesc::AbstractDOMParser_handleElementPSVI;
*handlePartialElementPSVI = *XML::Xercesc::AbstractDOMParser_handlePartialElementPSVI;
*handleAttributesPSVI = *XML::Xercesc::AbstractDOMParser_handleAttributesPSVI;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XercesDOMParser ##############

package XML::Xerces::XercesDOMParser;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::AbstractDOMParser XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_XercesDOMParser(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XercesDOMParser($self);
        delete $OWNER{$self};
    }
}

*getErrorHandler = *XML::Xercesc::XercesDOMParser_getErrorHandler;
*getEntityResolver = *XML::Xercesc::XercesDOMParser_getEntityResolver;
*getXMLEntityResolver = *XML::Xercesc::XercesDOMParser_getXMLEntityResolver;
*isCachingGrammarFromParse = *XML::Xercesc::XercesDOMParser_isCachingGrammarFromParse;
*isUsingCachedGrammarInParse = *XML::Xercesc::XercesDOMParser_isUsingCachedGrammarInParse;
*getGrammar = *XML::Xercesc::XercesDOMParser_getGrammar;
*getRootGrammar = *XML::Xercesc::XercesDOMParser_getRootGrammar;
*getURIText = *XML::Xercesc::XercesDOMParser_getURIText;
*getSrcOffset = *XML::Xercesc::XercesDOMParser_getSrcOffset;
*getIgnoreCachedDTD = *XML::Xercesc::XercesDOMParser_getIgnoreCachedDTD;
sub setErrorHandler {
  my ($self,$handler) = @_;
  my $callback = XML::Xerces::PerlErrorCallbackHandler->new($handler);
  $XML::Xerces::REMEMBER{tied(% {$self})}->{__ERROR_HANDLER} = $callback;

  return XML::Xercesc::XercesDOMParser_setErrorHandler($self,$callback);
}

sub setEntityResolver {
  my ($self,$handler) = @_;
  my $callback = XML::Xerces::PerlEntityResolverHandler->new($handler);
  $XML::Xerces::REMEMBER{tied(% {$self})}->{__ENTITY_RESOLVER} = $callback;

  return XML::Xercesc::XercesDOMParser_setEntityResolver($self,$callback);
}

*setXMLEntityResolver = *XML::Xercesc::XercesDOMParser_setXMLEntityResolver;
*cacheGrammarFromParse = *XML::Xercesc::XercesDOMParser_cacheGrammarFromParse;
*useCachedGrammarInParse = *XML::Xercesc::XercesDOMParser_useCachedGrammarInParse;
*setIgnoreCachedDTD = *XML::Xercesc::XercesDOMParser_setIgnoreCachedDTD;
*resetDocumentPool = *XML::Xercesc::XercesDOMParser_resetDocumentPool;
*error = *XML::Xercesc::XercesDOMParser_error;
*resetErrors = *XML::Xercesc::XercesDOMParser_resetErrors;
*loadGrammar = *XML::Xercesc::XercesDOMParser_loadGrammar;
*resetCachedGrammarPool = *XML::Xercesc::XercesDOMParser_resetCachedGrammarPool;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::SAXParser ##############

package XML::Xerces::SAXParser;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::Parser XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*Val_Never = *XML::Xercesc::SAXParser_Val_Never;
*Val_Always = *XML::Xercesc::SAXParser_Val_Always;
*Val_Auto = *XML::Xercesc::SAXParser_Val_Auto;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_SAXParser(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SAXParser($self);
        delete $OWNER{$self};
    }
}

*getDocumentHandler = *XML::Xercesc::SAXParser_getDocumentHandler;
*getEntityResolver = *XML::Xercesc::SAXParser_getEntityResolver;
*getXMLEntityResolver = *XML::Xercesc::SAXParser_getXMLEntityResolver;
*getErrorHandler = *XML::Xercesc::SAXParser_getErrorHandler;
*getPSVIHandler = *XML::Xercesc::SAXParser_getPSVIHandler;
*getValidator = *XML::Xercesc::SAXParser_getValidator;
*getValidationScheme = *XML::Xercesc::SAXParser_getValidationScheme;
*getDoSchema = *XML::Xercesc::SAXParser_getDoSchema;
*getValidationSchemaFullChecking = *XML::Xercesc::SAXParser_getValidationSchemaFullChecking;
*getIdentityConstraintChecking = *XML::Xercesc::SAXParser_getIdentityConstraintChecking;
*getErrorCount = *XML::Xercesc::SAXParser_getErrorCount;
*getDoNamespaces = *XML::Xercesc::SAXParser_getDoNamespaces;
*getExitOnFirstFatalError = *XML::Xercesc::SAXParser_getExitOnFirstFatalError;
*getValidationConstraintFatal = *XML::Xercesc::SAXParser_getValidationConstraintFatal;
*getExternalSchemaLocation = *XML::Xercesc::SAXParser_getExternalSchemaLocation;
*getExternalNoNamespaceSchemaLocation = *XML::Xercesc::SAXParser_getExternalNoNamespaceSchemaLocation;
*getSecurityManager = *XML::Xercesc::SAXParser_getSecurityManager;
*getLoadExternalDTD = *XML::Xercesc::SAXParser_getLoadExternalDTD;
*isCachingGrammarFromParse = *XML::Xercesc::SAXParser_isCachingGrammarFromParse;
*isUsingCachedGrammarInParse = *XML::Xercesc::SAXParser_isUsingCachedGrammarInParse;
*getCalculateSrcOfs = *XML::Xercesc::SAXParser_getCalculateSrcOfs;
*getStandardUriConformant = *XML::Xercesc::SAXParser_getStandardUriConformant;
*getGrammar = *XML::Xercesc::SAXParser_getGrammar;
*getRootGrammar = *XML::Xercesc::SAXParser_getRootGrammar;
*getURIText = *XML::Xercesc::SAXParser_getURIText;
*getSrcOffset = *XML::Xercesc::SAXParser_getSrcOffset;
*getGenerateSyntheticAnnotations = *XML::Xercesc::SAXParser_getGenerateSyntheticAnnotations;
*getValidateAnnotations = *XML::Xercesc::SAXParser_getValidateAnnotations;
*getIgnoreCachedDTD = *XML::Xercesc::SAXParser_getIgnoreCachedDTD;
*getIgnoreAnnotations = *XML::Xercesc::SAXParser_getIgnoreAnnotations;
*getDisableDefaultEntityResolution = *XML::Xercesc::SAXParser_getDisableDefaultEntityResolution;
*getSkipDTDValidation = *XML::Xercesc::SAXParser_getSkipDTDValidation;
*setGenerateSyntheticAnnotations = *XML::Xercesc::SAXParser_setGenerateSyntheticAnnotations;
*setValidateAnnotations = *XML::Xercesc::SAXParser_setValidateAnnotations;
*setDoNamespaces = *XML::Xercesc::SAXParser_setDoNamespaces;
*setValidationScheme = *XML::Xercesc::SAXParser_setValidationScheme;
*setDoSchema = *XML::Xercesc::SAXParser_setDoSchema;
*setValidationSchemaFullChecking = *XML::Xercesc::SAXParser_setValidationSchemaFullChecking;
*setIdentityConstraintChecking = *XML::Xercesc::SAXParser_setIdentityConstraintChecking;
*setExitOnFirstFatalError = *XML::Xercesc::SAXParser_setExitOnFirstFatalError;
*setValidationConstraintFatal = *XML::Xercesc::SAXParser_setValidationConstraintFatal;
*setExternalSchemaLocation = *XML::Xercesc::SAXParser_setExternalSchemaLocation;
*setExternalNoNamespaceSchemaLocation = *XML::Xercesc::SAXParser_setExternalNoNamespaceSchemaLocation;
*setSecurityManager = *XML::Xercesc::SAXParser_setSecurityManager;
*setLoadExternalDTD = *XML::Xercesc::SAXParser_setLoadExternalDTD;
*cacheGrammarFromParse = *XML::Xercesc::SAXParser_cacheGrammarFromParse;
*useCachedGrammarInParse = *XML::Xercesc::SAXParser_useCachedGrammarInParse;
*setCalculateSrcOfs = *XML::Xercesc::SAXParser_setCalculateSrcOfs;
*setStandardUriConformant = *XML::Xercesc::SAXParser_setStandardUriConformant;
*useScanner = *XML::Xercesc::SAXParser_useScanner;
*setInputBufferSize = *XML::Xercesc::SAXParser_setInputBufferSize;
*setIgnoreCachedDTD = *XML::Xercesc::SAXParser_setIgnoreCachedDTD;
*setIgnoreAnnotations = *XML::Xercesc::SAXParser_setIgnoreAnnotations;
*setDisableDefaultEntityResolution = *XML::Xercesc::SAXParser_setDisableDefaultEntityResolution;
*setSkipDTDValidation = *XML::Xercesc::SAXParser_setSkipDTDValidation;
*installAdvDocHandler = *XML::Xercesc::SAXParser_installAdvDocHandler;
*removeAdvDocHandler = *XML::Xercesc::SAXParser_removeAdvDocHandler;
*parseFirst = *XML::Xercesc::SAXParser_parseFirst;
*parseNext = *XML::Xercesc::SAXParser_parseNext;
*parseReset = *XML::Xercesc::SAXParser_parseReset;
*loadGrammar = *XML::Xercesc::SAXParser_loadGrammar;
*resetCachedGrammarPool = *XML::Xercesc::SAXParser_resetCachedGrammarPool;
*parse = *XML::Xercesc::SAXParser_parse;
sub setDocumentHandler {
    my ($self,$handler) = @_;
    my $callback = XML::Xerces::PerlDocumentCallbackHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__DOCUMENT_HANDLER} = $callback;

    my @args = ($self,$callback);
    return XML::Xercesc::SAXParser_setDocumentHandler(@args);
}

*setDTDHandler = *XML::Xercesc::SAXParser_setDTDHandler;
sub setErrorHandler {
  my ($self,$handler) = @_;
  my $callback = XML::Xerces::PerlErrorCallbackHandler->new($handler);
  $XML::Xerces::REMEMBER{tied(% {$self})}->{__ERROR_HANDLER} = $callback;

  return XML::Xercesc::SAXParser_setErrorHandler($self,$callback);
}

*setPSVIHandler = *XML::Xercesc::SAXParser_setPSVIHandler;
sub setEntityResolver {
  my ($self,$handler) = @_;
  my $callback = XML::Xerces::PerlEntityResolverHandler->new($handler);
  $XML::Xerces::REMEMBER{tied(% {$self})}->{__ENTITY_RESOLVER} = $callback;

  return XML::Xercesc::SAXParser_setEntityResolver($self,$callback);
}

*setXMLEntityResolver = *XML::Xercesc::SAXParser_setXMLEntityResolver;
*error = *XML::Xercesc::SAXParser_error;
*resetErrors = *XML::Xercesc::SAXParser_resetErrors;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::SAX2XMLReader ##############

package XML::Xerces::SAX2XMLReader;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*Val_Never = *XML::Xercesc::SAX2XMLReader_Val_Never;
*Val_Always = *XML::Xercesc::SAX2XMLReader_Val_Always;
*Val_Auto = *XML::Xercesc::SAX2XMLReader_Val_Auto;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SAX2XMLReader($self);
        delete $OWNER{$self};
    }
}

*getContentHandler = *XML::Xercesc::SAX2XMLReader_getContentHandler;
*getDTDHandler = *XML::Xercesc::SAX2XMLReader_getDTDHandler;
*getFeature = *XML::Xercesc::SAX2XMLReader_getFeature;
*getProperty = *XML::Xercesc::SAX2XMLReader_getProperty;
sub setContentHandler {
    my ($self,$handler) = @_;
    my $callback = XML::Xerces::PerlContentCallbackHandler->new($handler);
    $XML::Xerces::REMEMBER{tied(% {$self})}->{__CONTENT_HANDLER} = $callback;

    my @args = ($self,$callback);
    return XML::Xercesc::SAX2XMLReader_setContentHandler(@args);
}

*setDTDHandler = *XML::Xercesc::SAX2XMLReader_setDTDHandler;
sub setEntityResolver {
  my ($self,$handler) = @_;
  my $callback = XML::Xerces::PerlEntityResolverHandler->new($handler);
  $XML::Xerces::REMEMBER{tied(% {$self})}->{__ENTITY_RESOLVER} = $callback;

  return XML::Xercesc::SAX2XMLReader_setEntityResolver($self,$callback);
}

sub setErrorHandler {
  my ($self,$handler) = @_;
  my $callback = XML::Xerces::PerlErrorCallbackHandler->new($handler);
  $XML::Xerces::REMEMBER{tied(% {$self})}->{__ERROR_HANDLER} = $callback;

  return XML::Xercesc::SAX2XMLReader_setErrorHandler($self,$callback);
}

*setFeature = *XML::Xercesc::SAX2XMLReader_setFeature;
*setProperty = *XML::Xercesc::SAX2XMLReader_setProperty;
*parse = *XML::Xercesc::SAX2XMLReader_parse;
*getDeclarationHandler = *XML::Xercesc::SAX2XMLReader_getDeclarationHandler;
*getLexicalHandler = *XML::Xercesc::SAX2XMLReader_getLexicalHandler;
*setDeclarationHandler = *XML::Xercesc::SAX2XMLReader_setDeclarationHandler;
*setLexicalHandler = *XML::Xercesc::SAX2XMLReader_setLexicalHandler;
*getValidator = *XML::Xercesc::SAX2XMLReader_getValidator;
*getErrorCount = *XML::Xercesc::SAX2XMLReader_getErrorCount;
*getExitOnFirstFatalError = *XML::Xercesc::SAX2XMLReader_getExitOnFirstFatalError;
*getValidationConstraintFatal = *XML::Xercesc::SAX2XMLReader_getValidationConstraintFatal;
*getGrammar = *XML::Xercesc::SAX2XMLReader_getGrammar;
*getRootGrammar = *XML::Xercesc::SAX2XMLReader_getRootGrammar;
*getURIText = *XML::Xercesc::SAX2XMLReader_getURIText;
*getSrcOffset = *XML::Xercesc::SAX2XMLReader_getSrcOffset;
*setValidator = *XML::Xercesc::SAX2XMLReader_setValidator;
*setExitOnFirstFatalError = *XML::Xercesc::SAX2XMLReader_setExitOnFirstFatalError;
*setValidationConstraintFatal = *XML::Xercesc::SAX2XMLReader_setValidationConstraintFatal;
*parseFirst = *XML::Xercesc::SAX2XMLReader_parseFirst;
*parseNext = *XML::Xercesc::SAX2XMLReader_parseNext;
*parseReset = *XML::Xercesc::SAX2XMLReader_parseReset;
*loadGrammar = *XML::Xercesc::SAX2XMLReader_loadGrammar;
*resetCachedGrammarPool = *XML::Xercesc::SAX2XMLReader_resetCachedGrammarPool;
*setInputBufferSize = *XML::Xercesc::SAX2XMLReader_setInputBufferSize;
*installAdvDocHandler = *XML::Xercesc::SAX2XMLReader_installAdvDocHandler;
*removeAdvDocHandler = *XML::Xercesc::SAX2XMLReader_removeAdvDocHandler;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::XMLReaderFactory ##############

package XML::Xerces::XMLReaderFactory;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
*createXMLReader = *XML::Xercesc::XMLReaderFactory_createXMLReader;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::PerlCallbackHandler ##############

package XML::Xerces::PerlCallbackHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_PerlCallbackHandler(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PerlCallbackHandler($self);
        delete $OWNER{$self};
    }
}

*type = *XML::Xercesc::PerlCallbackHandler_type;
*set_callback_obj = *XML::Xercesc::PerlCallbackHandler_set_callback_obj;
*XMLString2Perl = *XML::Xercesc::PerlCallbackHandler_XMLString2Perl;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::PerlErrorCallbackHandler ##############

package XML::Xerces::PerlErrorCallbackHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::ErrorHandler XML::Xerces::PerlCallbackHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_PerlErrorCallbackHandler(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PerlErrorCallbackHandler($self);
        delete $OWNER{$self};
    }
}

*type = *XML::Xercesc::PerlErrorCallbackHandler_type;
*resetErrors = *XML::Xercesc::PerlErrorCallbackHandler_resetErrors;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::PerlEntityResolverHandler ##############

package XML::Xerces::PerlEntityResolverHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::EntityResolver XML::Xerces::PerlCallbackHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_PerlEntityResolverHandler(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PerlEntityResolverHandler($self);
        delete $OWNER{$self};
    }
}

*type = *XML::Xercesc::PerlEntityResolverHandler_type;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::PerlNodeFilterCallbackHandler ##############

package XML::Xerces::PerlNodeFilterCallbackHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DOMNodeFilter XML::Xerces::PerlCallbackHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_PerlNodeFilterCallbackHandler(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PerlNodeFilterCallbackHandler($self);
        delete $OWNER{$self};
    }
}

*type = *XML::Xercesc::PerlNodeFilterCallbackHandler_type;
*acceptNode = *XML::Xercesc::PerlNodeFilterCallbackHandler_acceptNode;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::PerlDocumentCallbackHandler ##############

package XML::Xerces::PerlDocumentCallbackHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::DocumentHandler XML::Xerces::PerlCallbackHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_PerlDocumentCallbackHandler(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PerlDocumentCallbackHandler($self);
        delete $OWNER{$self};
    }
}

*type = *XML::Xercesc::PerlDocumentCallbackHandler_type;
*characters = *XML::Xercesc::PerlDocumentCallbackHandler_characters;
*processingInstruction = *XML::Xercesc::PerlDocumentCallbackHandler_processingInstruction;
*setDocumentLocator = *XML::Xercesc::PerlDocumentCallbackHandler_setDocumentLocator;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


############# Class : XML::Xerces::PerlContentCallbackHandler ##############

package XML::Xerces::PerlContentCallbackHandler;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::ContentHandler XML::Xerces::PerlCallbackHandler XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_PerlContentCallbackHandler(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PerlContentCallbackHandler($self);
        delete $OWNER{$self};
    }
}

*type = *XML::Xercesc::PerlContentCallbackHandler_type;
*characters = *XML::Xercesc::PerlContentCallbackHandler_characters;
*processingInstruction = *XML::Xercesc::PerlContentCallbackHandler_processingInstruction;
*setDocumentLocator = *XML::Xercesc::PerlContentCallbackHandler_setDocumentLocator;
*startPrefixMapping = *XML::Xercesc::PerlContentCallbackHandler_startPrefixMapping;
*endPrefixMapping = *XML::Xercesc::PerlContentCallbackHandler_endPrefixMapping;
*skippedEntity = *XML::Xercesc::PerlContentCallbackHandler_skippedEntity;
sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


# ------- VARIABLE STUBS --------

package XML::Xerces;

*DEBUG_UTF8_OUT = *XML::Xercesc::DEBUG_UTF8_OUT;
*DEBUG_UTF8_IN = *XML::Xercesc::DEBUG_UTF8_IN;
*XMLPlatformUtils_fgFileMgr = *XML::Xercesc::XMLPlatformUtils_fgFileMgr;
*XMLPlatformUtils_fgMutexMgr = *XML::Xercesc::XMLPlatformUtils_fgMutexMgr;
*XMLPlatformUtils_fgAtomicOpMgr = *XML::Xercesc::XMLPlatformUtils_fgAtomicOpMgr;
*XMLPlatformUtils_fgXMLChBigEndian = *XML::Xercesc::XMLPlatformUtils_fgXMLChBigEndian;
*XMLUni_fgAnyString = *XML::Xercesc::XMLUni_fgAnyString;
*XMLUni_fgAttListString = *XML::Xercesc::XMLUni_fgAttListString;
*XMLUni_fgCommentString = *XML::Xercesc::XMLUni_fgCommentString;
*XMLUni_fgCDATAString = *XML::Xercesc::XMLUni_fgCDATAString;
*XMLUni_fgDefaultString = *XML::Xercesc::XMLUni_fgDefaultString;
*XMLUni_fgDocTypeString = *XML::Xercesc::XMLUni_fgDocTypeString;
*XMLUni_fgEBCDICEncodingString = *XML::Xercesc::XMLUni_fgEBCDICEncodingString;
*XMLUni_fgElemString = *XML::Xercesc::XMLUni_fgElemString;
*XMLUni_fgEmptyString = *XML::Xercesc::XMLUni_fgEmptyString;
*XMLUni_fgEncodingString = *XML::Xercesc::XMLUni_fgEncodingString;
*XMLUni_fgEntitString = *XML::Xercesc::XMLUni_fgEntitString;
*XMLUni_fgEntityString = *XML::Xercesc::XMLUni_fgEntityString;
*XMLUni_fgEntitiesString = *XML::Xercesc::XMLUni_fgEntitiesString;
*XMLUni_fgEnumerationString = *XML::Xercesc::XMLUni_fgEnumerationString;
*XMLUni_fgExceptDomain = *XML::Xercesc::XMLUni_fgExceptDomain;
*XMLUni_fgFixedString = *XML::Xercesc::XMLUni_fgFixedString;
*XMLUni_fgIBM037EncodingString = *XML::Xercesc::XMLUni_fgIBM037EncodingString;
*XMLUni_fgIBM037EncodingString2 = *XML::Xercesc::XMLUni_fgIBM037EncodingString2;
*XMLUni_fgIBM1047EncodingString = *XML::Xercesc::XMLUni_fgIBM1047EncodingString;
*XMLUni_fgIBM1047EncodingString2 = *XML::Xercesc::XMLUni_fgIBM1047EncodingString2;
*XMLUni_fgIBM1140EncodingString = *XML::Xercesc::XMLUni_fgIBM1140EncodingString;
*XMLUni_fgIBM1140EncodingString2 = *XML::Xercesc::XMLUni_fgIBM1140EncodingString2;
*XMLUni_fgIBM1140EncodingString3 = *XML::Xercesc::XMLUni_fgIBM1140EncodingString3;
*XMLUni_fgIBM1140EncodingString4 = *XML::Xercesc::XMLUni_fgIBM1140EncodingString4;
*XMLUni_fgIESString = *XML::Xercesc::XMLUni_fgIESString;
*XMLUni_fgIDString = *XML::Xercesc::XMLUni_fgIDString;
*XMLUni_fgIDRefString = *XML::Xercesc::XMLUni_fgIDRefString;
*XMLUni_fgIDRefsString = *XML::Xercesc::XMLUni_fgIDRefsString;
*XMLUni_fgImpliedString = *XML::Xercesc::XMLUni_fgImpliedString;
*XMLUni_fgIgnoreString = *XML::Xercesc::XMLUni_fgIgnoreString;
*XMLUni_fgIncludeString = *XML::Xercesc::XMLUni_fgIncludeString;
*XMLUni_fgISO88591EncodingString = *XML::Xercesc::XMLUni_fgISO88591EncodingString;
*XMLUni_fgISO88591EncodingString2 = *XML::Xercesc::XMLUni_fgISO88591EncodingString2;
*XMLUni_fgISO88591EncodingString3 = *XML::Xercesc::XMLUni_fgISO88591EncodingString3;
*XMLUni_fgISO88591EncodingString4 = *XML::Xercesc::XMLUni_fgISO88591EncodingString4;
*XMLUni_fgISO88591EncodingString5 = *XML::Xercesc::XMLUni_fgISO88591EncodingString5;
*XMLUni_fgISO88591EncodingString6 = *XML::Xercesc::XMLUni_fgISO88591EncodingString6;
*XMLUni_fgISO88591EncodingString7 = *XML::Xercesc::XMLUni_fgISO88591EncodingString7;
*XMLUni_fgISO88591EncodingString8 = *XML::Xercesc::XMLUni_fgISO88591EncodingString8;
*XMLUni_fgISO88591EncodingString9 = *XML::Xercesc::XMLUni_fgISO88591EncodingString9;
*XMLUni_fgISO88591EncodingString10 = *XML::Xercesc::XMLUni_fgISO88591EncodingString10;
*XMLUni_fgISO88591EncodingString11 = *XML::Xercesc::XMLUni_fgISO88591EncodingString11;
*XMLUni_fgISO88591EncodingString12 = *XML::Xercesc::XMLUni_fgISO88591EncodingString12;
*XMLUni_fgLocalHostString = *XML::Xercesc::XMLUni_fgLocalHostString;
*XMLUni_fgNoString = *XML::Xercesc::XMLUni_fgNoString;
*XMLUni_fgNotationString = *XML::Xercesc::XMLUni_fgNotationString;
*XMLUni_fgNDATAString = *XML::Xercesc::XMLUni_fgNDATAString;
*XMLUni_fgNmTokenString = *XML::Xercesc::XMLUni_fgNmTokenString;
*XMLUni_fgNmTokensString = *XML::Xercesc::XMLUni_fgNmTokensString;
*XMLUni_fgPCDATAString = *XML::Xercesc::XMLUni_fgPCDATAString;
*XMLUni_fgPIString = *XML::Xercesc::XMLUni_fgPIString;
*XMLUni_fgPubIDString = *XML::Xercesc::XMLUni_fgPubIDString;
*XMLUni_fgRefString = *XML::Xercesc::XMLUni_fgRefString;
*XMLUni_fgRequiredString = *XML::Xercesc::XMLUni_fgRequiredString;
*XMLUni_fgStandaloneString = *XML::Xercesc::XMLUni_fgStandaloneString;
*XMLUni_fgVersion1_0 = *XML::Xercesc::XMLUni_fgVersion1_0;
*XMLUni_fgVersion1_1 = *XML::Xercesc::XMLUni_fgVersion1_1;
*XMLUni_fgSysIDString = *XML::Xercesc::XMLUni_fgSysIDString;
*XMLUni_fgUnknownURIName = *XML::Xercesc::XMLUni_fgUnknownURIName;
*XMLUni_fgUCS4EncodingString = *XML::Xercesc::XMLUni_fgUCS4EncodingString;
*XMLUni_fgUCS4EncodingString2 = *XML::Xercesc::XMLUni_fgUCS4EncodingString2;
*XMLUni_fgUCS4EncodingString3 = *XML::Xercesc::XMLUni_fgUCS4EncodingString3;
*XMLUni_fgUCS4BEncodingString = *XML::Xercesc::XMLUni_fgUCS4BEncodingString;
*XMLUni_fgUCS4BEncodingString2 = *XML::Xercesc::XMLUni_fgUCS4BEncodingString2;
*XMLUni_fgUCS4LEncodingString = *XML::Xercesc::XMLUni_fgUCS4LEncodingString;
*XMLUni_fgUCS4LEncodingString2 = *XML::Xercesc::XMLUni_fgUCS4LEncodingString2;
*XMLUni_fgUSASCIIEncodingString = *XML::Xercesc::XMLUni_fgUSASCIIEncodingString;
*XMLUni_fgUSASCIIEncodingString2 = *XML::Xercesc::XMLUni_fgUSASCIIEncodingString2;
*XMLUni_fgUSASCIIEncodingString3 = *XML::Xercesc::XMLUni_fgUSASCIIEncodingString3;
*XMLUni_fgUSASCIIEncodingString4 = *XML::Xercesc::XMLUni_fgUSASCIIEncodingString4;
*XMLUni_fgUTF8EncodingString = *XML::Xercesc::XMLUni_fgUTF8EncodingString;
*XMLUni_fgUTF8EncodingString2 = *XML::Xercesc::XMLUni_fgUTF8EncodingString2;
*XMLUni_fgUTF16EncodingString = *XML::Xercesc::XMLUni_fgUTF16EncodingString;
*XMLUni_fgUTF16EncodingString2 = *XML::Xercesc::XMLUni_fgUTF16EncodingString2;
*XMLUni_fgUTF16EncodingString3 = *XML::Xercesc::XMLUni_fgUTF16EncodingString3;
*XMLUni_fgUTF16EncodingString4 = *XML::Xercesc::XMLUni_fgUTF16EncodingString4;
*XMLUni_fgUTF16EncodingString5 = *XML::Xercesc::XMLUni_fgUTF16EncodingString5;
*XMLUni_fgUTF16EncodingString6 = *XML::Xercesc::XMLUni_fgUTF16EncodingString6;
*XMLUni_fgUTF16EncodingString7 = *XML::Xercesc::XMLUni_fgUTF16EncodingString7;
*XMLUni_fgUTF16BEncodingString = *XML::Xercesc::XMLUni_fgUTF16BEncodingString;
*XMLUni_fgUTF16BEncodingString2 = *XML::Xercesc::XMLUni_fgUTF16BEncodingString2;
*XMLUni_fgUTF16LEncodingString = *XML::Xercesc::XMLUni_fgUTF16LEncodingString;
*XMLUni_fgUTF16LEncodingString2 = *XML::Xercesc::XMLUni_fgUTF16LEncodingString2;
*XMLUni_fgVersionString = *XML::Xercesc::XMLUni_fgVersionString;
*XMLUni_fgValidityDomain = *XML::Xercesc::XMLUni_fgValidityDomain;
*XMLUni_fgWin1252EncodingString = *XML::Xercesc::XMLUni_fgWin1252EncodingString;
*XMLUni_fgXMLChEncodingString = *XML::Xercesc::XMLUni_fgXMLChEncodingString;
*XMLUni_fgXMLDOMMsgDomain = *XML::Xercesc::XMLUni_fgXMLDOMMsgDomain;
*XMLUni_fgXMLString = *XML::Xercesc::XMLUni_fgXMLString;
*XMLUni_fgXMLStringSpace = *XML::Xercesc::XMLUni_fgXMLStringSpace;
*XMLUni_fgXMLStringHTab = *XML::Xercesc::XMLUni_fgXMLStringHTab;
*XMLUni_fgXMLStringCR = *XML::Xercesc::XMLUni_fgXMLStringCR;
*XMLUni_fgXMLStringLF = *XML::Xercesc::XMLUni_fgXMLStringLF;
*XMLUni_fgXMLStringSpaceU = *XML::Xercesc::XMLUni_fgXMLStringSpaceU;
*XMLUni_fgXMLStringHTabU = *XML::Xercesc::XMLUni_fgXMLStringHTabU;
*XMLUni_fgXMLStringCRU = *XML::Xercesc::XMLUni_fgXMLStringCRU;
*XMLUni_fgXMLStringLFU = *XML::Xercesc::XMLUni_fgXMLStringLFU;
*XMLUni_fgXMLDeclString = *XML::Xercesc::XMLUni_fgXMLDeclString;
*XMLUni_fgXMLDeclStringSpace = *XML::Xercesc::XMLUni_fgXMLDeclStringSpace;
*XMLUni_fgXMLDeclStringHTab = *XML::Xercesc::XMLUni_fgXMLDeclStringHTab;
*XMLUni_fgXMLDeclStringLF = *XML::Xercesc::XMLUni_fgXMLDeclStringLF;
*XMLUni_fgXMLDeclStringCR = *XML::Xercesc::XMLUni_fgXMLDeclStringCR;
*XMLUni_fgXMLDeclStringSpaceU = *XML::Xercesc::XMLUni_fgXMLDeclStringSpaceU;
*XMLUni_fgXMLDeclStringHTabU = *XML::Xercesc::XMLUni_fgXMLDeclStringHTabU;
*XMLUni_fgXMLDeclStringLFU = *XML::Xercesc::XMLUni_fgXMLDeclStringLFU;
*XMLUni_fgXMLDeclStringCRU = *XML::Xercesc::XMLUni_fgXMLDeclStringCRU;
*XMLUni_fgXMLNSString = *XML::Xercesc::XMLUni_fgXMLNSString;
*XMLUni_fgXMLNSColonString = *XML::Xercesc::XMLUni_fgXMLNSColonString;
*XMLUni_fgXMLNSURIName = *XML::Xercesc::XMLUni_fgXMLNSURIName;
*XMLUni_fgXMLErrDomain = *XML::Xercesc::XMLUni_fgXMLErrDomain;
*XMLUni_fgXMLURIName = *XML::Xercesc::XMLUni_fgXMLURIName;
*XMLUni_fgInfosetURIName = *XML::Xercesc::XMLUni_fgInfosetURIName;
*XMLUni_fgYesString = *XML::Xercesc::XMLUni_fgYesString;
*XMLUni_fgZeroLenString = *XML::Xercesc::XMLUni_fgZeroLenString;
*XMLUni_fgDTDEntityString = *XML::Xercesc::XMLUni_fgDTDEntityString;
*XMLUni_fgAmp = *XML::Xercesc::XMLUni_fgAmp;
*XMLUni_fgLT = *XML::Xercesc::XMLUni_fgLT;
*XMLUni_fgGT = *XML::Xercesc::XMLUni_fgGT;
*XMLUni_fgQuot = *XML::Xercesc::XMLUni_fgQuot;
*XMLUni_fgApos = *XML::Xercesc::XMLUni_fgApos;
*XMLUni_fgWFXMLScanner = *XML::Xercesc::XMLUni_fgWFXMLScanner;
*XMLUni_fgIGXMLScanner = *XML::Xercesc::XMLUni_fgIGXMLScanner;
*XMLUni_fgSGXMLScanner = *XML::Xercesc::XMLUni_fgSGXMLScanner;
*XMLUni_fgDGXMLScanner = *XML::Xercesc::XMLUni_fgDGXMLScanner;
*XMLUni_fgXSAXMLScanner = *XML::Xercesc::XMLUni_fgXSAXMLScanner;
*XMLUni_fgCDataStart = *XML::Xercesc::XMLUni_fgCDataStart;
*XMLUni_fgCDataEnd = *XML::Xercesc::XMLUni_fgCDataEnd;
*XMLUni_fgArrayIndexOutOfBoundsException_Name = *XML::Xercesc::XMLUni_fgArrayIndexOutOfBoundsException_Name;
*XMLUni_fgEmptyStackException_Name = *XML::Xercesc::XMLUni_fgEmptyStackException_Name;
*XMLUni_fgIllegalArgumentException_Name = *XML::Xercesc::XMLUni_fgIllegalArgumentException_Name;
*XMLUni_fgInvalidCastException_Name = *XML::Xercesc::XMLUni_fgInvalidCastException_Name;
*XMLUni_fgIOException_Name = *XML::Xercesc::XMLUni_fgIOException_Name;
*XMLUni_fgNoSuchElementException_Name = *XML::Xercesc::XMLUni_fgNoSuchElementException_Name;
*XMLUni_fgNullPointerException_Name = *XML::Xercesc::XMLUni_fgNullPointerException_Name;
*XMLUni_fgXMLPlatformUtilsException_Name = *XML::Xercesc::XMLUni_fgXMLPlatformUtilsException_Name;
*XMLUni_fgRuntimeException_Name = *XML::Xercesc::XMLUni_fgRuntimeException_Name;
*XMLUni_fgTranscodingException_Name = *XML::Xercesc::XMLUni_fgTranscodingException_Name;
*XMLUni_fgUnexpectedEOFException_Name = *XML::Xercesc::XMLUni_fgUnexpectedEOFException_Name;
*XMLUni_fgUnsupportedEncodingException_Name = *XML::Xercesc::XMLUni_fgUnsupportedEncodingException_Name;
*XMLUni_fgUTFDataFormatException_Name = *XML::Xercesc::XMLUni_fgUTFDataFormatException_Name;
*XMLUni_fgNetAccessorException_Name = *XML::Xercesc::XMLUni_fgNetAccessorException_Name;
*XMLUni_fgMalformedURLException_Name = *XML::Xercesc::XMLUni_fgMalformedURLException_Name;
*XMLUni_fgNumberFormatException_Name = *XML::Xercesc::XMLUni_fgNumberFormatException_Name;
*XMLUni_fgParseException_Name = *XML::Xercesc::XMLUni_fgParseException_Name;
*XMLUni_fgInvalidDatatypeFacetException_Name = *XML::Xercesc::XMLUni_fgInvalidDatatypeFacetException_Name;
*XMLUni_fgInvalidDatatypeValueException_Name = *XML::Xercesc::XMLUni_fgInvalidDatatypeValueException_Name;
*XMLUni_fgSchemaDateTimeException_Name = *XML::Xercesc::XMLUni_fgSchemaDateTimeException_Name;
*XMLUni_fgXPathException_Name = *XML::Xercesc::XMLUni_fgXPathException_Name;
*XMLUni_fgXSerializationException_Name = *XML::Xercesc::XMLUni_fgXSerializationException_Name;
*XMLUni_fgNegINFString = *XML::Xercesc::XMLUni_fgNegINFString;
*XMLUni_fgNegZeroString = *XML::Xercesc::XMLUni_fgNegZeroString;
*XMLUni_fgPosZeroString = *XML::Xercesc::XMLUni_fgPosZeroString;
*XMLUni_fgPosINFString = *XML::Xercesc::XMLUni_fgPosINFString;
*XMLUni_fgNaNString = *XML::Xercesc::XMLUni_fgNaNString;
*XMLUni_fgEString = *XML::Xercesc::XMLUni_fgEString;
*XMLUni_fgZeroString = *XML::Xercesc::XMLUni_fgZeroString;
*XMLUni_fgNullString = *XML::Xercesc::XMLUni_fgNullString;
*XMLUni_fgXercesDynamic = *XML::Xercesc::XMLUni_fgXercesDynamic;
*XMLUni_fgXercesSchema = *XML::Xercesc::XMLUni_fgXercesSchema;
*XMLUni_fgXercesSchemaFullChecking = *XML::Xercesc::XMLUni_fgXercesSchemaFullChecking;
*XMLUni_fgXercesIdentityConstraintChecking = *XML::Xercesc::XMLUni_fgXercesIdentityConstraintChecking;
*XMLUni_fgXercesSchemaExternalSchemaLocation = *XML::Xercesc::XMLUni_fgXercesSchemaExternalSchemaLocation;
*XMLUni_fgXercesSchemaExternalNoNameSpaceSchemaLocation = *XML::Xercesc::XMLUni_fgXercesSchemaExternalNoNameSpaceSchemaLocation;
*XMLUni_fgXercesSecurityManager = *XML::Xercesc::XMLUni_fgXercesSecurityManager;
*XMLUni_fgXercesLoadExternalDTD = *XML::Xercesc::XMLUni_fgXercesLoadExternalDTD;
*XMLUni_fgXercesContinueAfterFatalError = *XML::Xercesc::XMLUni_fgXercesContinueAfterFatalError;
*XMLUni_fgXercesValidationErrorAsFatal = *XML::Xercesc::XMLUni_fgXercesValidationErrorAsFatal;
*XMLUni_fgXercesUserAdoptsDOMDocument = *XML::Xercesc::XMLUni_fgXercesUserAdoptsDOMDocument;
*XMLUni_fgXercesCacheGrammarFromParse = *XML::Xercesc::XMLUni_fgXercesCacheGrammarFromParse;
*XMLUni_fgXercesUseCachedGrammarInParse = *XML::Xercesc::XMLUni_fgXercesUseCachedGrammarInParse;
*XMLUni_fgXercesScannerName = *XML::Xercesc::XMLUni_fgXercesScannerName;
*XMLUni_fgXercesParserUseDocumentFromImplementation = *XML::Xercesc::XMLUni_fgXercesParserUseDocumentFromImplementation;
*XMLUni_fgXercesCalculateSrcOfs = *XML::Xercesc::XMLUni_fgXercesCalculateSrcOfs;
*XMLUni_fgXercesStandardUriConformant = *XML::Xercesc::XMLUni_fgXercesStandardUriConformant;
*XMLUni_fgXercesDOMHasPSVIInfo = *XML::Xercesc::XMLUni_fgXercesDOMHasPSVIInfo;
*XMLUni_fgXercesGenerateSyntheticAnnotations = *XML::Xercesc::XMLUni_fgXercesGenerateSyntheticAnnotations;
*XMLUni_fgXercesValidateAnnotations = *XML::Xercesc::XMLUni_fgXercesValidateAnnotations;
*XMLUni_fgXercesIgnoreCachedDTD = *XML::Xercesc::XMLUni_fgXercesIgnoreCachedDTD;
*XMLUni_fgXercesIgnoreAnnotations = *XML::Xercesc::XMLUni_fgXercesIgnoreAnnotations;
*XMLUni_fgXercesDisableDefaultEntityResolution = *XML::Xercesc::XMLUni_fgXercesDisableDefaultEntityResolution;
*XMLUni_fgXercesSkipDTDValidation = *XML::Xercesc::XMLUni_fgXercesSkipDTDValidation;
*XMLUni_fgXercesEntityResolver = *XML::Xercesc::XMLUni_fgXercesEntityResolver;
*XMLUni_fgSAX2CoreValidation = *XML::Xercesc::XMLUni_fgSAX2CoreValidation;
*XMLUni_fgSAX2CoreNameSpaces = *XML::Xercesc::XMLUni_fgSAX2CoreNameSpaces;
*XMLUni_fgSAX2CoreNameSpacePrefixes = *XML::Xercesc::XMLUni_fgSAX2CoreNameSpacePrefixes;
*XMLUni_fgDOMCanonicalForm = *XML::Xercesc::XMLUni_fgDOMCanonicalForm;
*XMLUni_fgDOMCDATASections = *XML::Xercesc::XMLUni_fgDOMCDATASections;
*XMLUni_fgDOMComments = *XML::Xercesc::XMLUni_fgDOMComments;
*XMLUni_fgDOMCharsetOverridesXMLEncoding = *XML::Xercesc::XMLUni_fgDOMCharsetOverridesXMLEncoding;
*XMLUni_fgDOMCheckCharacterNormalization = *XML::Xercesc::XMLUni_fgDOMCheckCharacterNormalization;
*XMLUni_fgDOMDatatypeNormalization = *XML::Xercesc::XMLUni_fgDOMDatatypeNormalization;
*XMLUni_fgDOMDisallowDoctype = *XML::Xercesc::XMLUni_fgDOMDisallowDoctype;
*XMLUni_fgDOMElementContentWhitespace = *XML::Xercesc::XMLUni_fgDOMElementContentWhitespace;
*XMLUni_fgDOMErrorHandler = *XML::Xercesc::XMLUni_fgDOMErrorHandler;
*XMLUni_fgDOMEntities = *XML::Xercesc::XMLUni_fgDOMEntities;
*XMLUni_fgDOMIgnoreUnknownCharacterDenormalization = *XML::Xercesc::XMLUni_fgDOMIgnoreUnknownCharacterDenormalization;
*XMLUni_fgDOMInfoset = *XML::Xercesc::XMLUni_fgDOMInfoset;
*XMLUni_fgDOMNamespaces = *XML::Xercesc::XMLUni_fgDOMNamespaces;
*XMLUni_fgDOMNamespaceDeclarations = *XML::Xercesc::XMLUni_fgDOMNamespaceDeclarations;
*XMLUni_fgDOMNormalizeCharacters = *XML::Xercesc::XMLUni_fgDOMNormalizeCharacters;
*XMLUni_fgDOMResourceResolver = *XML::Xercesc::XMLUni_fgDOMResourceResolver;
*XMLUni_fgDOMSchemaLocation = *XML::Xercesc::XMLUni_fgDOMSchemaLocation;
*XMLUni_fgDOMSchemaType = *XML::Xercesc::XMLUni_fgDOMSchemaType;
*XMLUni_fgDOMSplitCDATASections = *XML::Xercesc::XMLUni_fgDOMSplitCDATASections;
*XMLUni_fgDOMSupportedMediatypesOnly = *XML::Xercesc::XMLUni_fgDOMSupportedMediatypesOnly;
*XMLUni_fgDOMValidate = *XML::Xercesc::XMLUni_fgDOMValidate;
*XMLUni_fgDOMValidateIfSchema = *XML::Xercesc::XMLUni_fgDOMValidateIfSchema;
*XMLUni_fgDOMWellFormed = *XML::Xercesc::XMLUni_fgDOMWellFormed;
*XMLUni_fgDOMXMLSchemaType = *XML::Xercesc::XMLUni_fgDOMXMLSchemaType;
*XMLUni_fgDOMDTDType = *XML::Xercesc::XMLUni_fgDOMDTDType;
*XMLUni_fgDOMWRTCanonicalForm = *XML::Xercesc::XMLUni_fgDOMWRTCanonicalForm;
*XMLUni_fgDOMWRTDiscardDefaultContent = *XML::Xercesc::XMLUni_fgDOMWRTDiscardDefaultContent;
*XMLUni_fgDOMWRTEntities = *XML::Xercesc::XMLUni_fgDOMWRTEntities;
*XMLUni_fgDOMWRTFormatPrettyPrint = *XML::Xercesc::XMLUni_fgDOMWRTFormatPrettyPrint;
*XMLUni_fgDOMWRTNormalizeCharacters = *XML::Xercesc::XMLUni_fgDOMWRTNormalizeCharacters;
*XMLUni_fgDOMWRTSplitCdataSections = *XML::Xercesc::XMLUni_fgDOMWRTSplitCdataSections;
*XMLUni_fgDOMWRTValidation = *XML::Xercesc::XMLUni_fgDOMWRTValidation;
*XMLUni_fgDOMWRTWhitespaceInElementContent = *XML::Xercesc::XMLUni_fgDOMWRTWhitespaceInElementContent;
*XMLUni_fgDOMWRTBOM = *XML::Xercesc::XMLUni_fgDOMWRTBOM;
*XMLUni_fgDOMXMLDeclaration = *XML::Xercesc::XMLUni_fgDOMXMLDeclaration;
*XMLUni_fgDOMWRTXercesPrettyPrint = *XML::Xercesc::XMLUni_fgDOMWRTXercesPrettyPrint;
*XMLUni_fgXercescInterfacePSVITypeInfo = *XML::Xercesc::XMLUni_fgXercescInterfacePSVITypeInfo;
*XMLUni_fgXercescInterfaceDOMDocumentTypeImpl = *XML::Xercesc::XMLUni_fgXercescInterfaceDOMDocumentTypeImpl;
*XMLUni_fgXercescInterfaceDOMMemoryManager = *XML::Xercesc::XMLUni_fgXercescInterfaceDOMMemoryManager;
*XMLUni_fgXercescDefaultLocale = *XML::Xercesc::XMLUni_fgXercescDefaultLocale;
*XMLUni_fgDefErrMsg = *XML::Xercesc::XMLUni_fgDefErrMsg;
*XMLUni_fgValueZero = *XML::Xercesc::XMLUni_fgValueZero;
*XMLUni_fgNegOne = *XML::Xercesc::XMLUni_fgNegOne;
*XMLUni_fgValueOne = *XML::Xercesc::XMLUni_fgValueOne;
*XMLUni_fgLongMaxInc = *XML::Xercesc::XMLUni_fgLongMaxInc;
*XMLUni_fgLongMinInc = *XML::Xercesc::XMLUni_fgLongMinInc;
*XMLUni_fgIntMaxInc = *XML::Xercesc::XMLUni_fgIntMaxInc;
*XMLUni_fgIntMinInc = *XML::Xercesc::XMLUni_fgIntMinInc;
*XMLUni_fgShortMaxInc = *XML::Xercesc::XMLUni_fgShortMaxInc;
*XMLUni_fgShortMinInc = *XML::Xercesc::XMLUni_fgShortMinInc;
*XMLUni_fgByteMaxInc = *XML::Xercesc::XMLUni_fgByteMaxInc;
*XMLUni_fgByteMinInc = *XML::Xercesc::XMLUni_fgByteMinInc;
*XMLUni_fgULongMaxInc = *XML::Xercesc::XMLUni_fgULongMaxInc;
*XMLUni_fgUIntMaxInc = *XML::Xercesc::XMLUni_fgUIntMaxInc;
*XMLUni_fgUShortMaxInc = *XML::Xercesc::XMLUni_fgUShortMaxInc;
*XMLUni_fgUByteMaxInc = *XML::Xercesc::XMLUni_fgUByteMaxInc;
*XMLUni_fgLangPattern = *XML::Xercesc::XMLUni_fgLangPattern;
*XMLUni_fgBooleanValueSpace = *XML::Xercesc::XMLUni_fgBooleanValueSpace;
*XMLUni_fgBooleanValueSpaceArraySize = *XML::Xercesc::XMLUni_fgBooleanValueSpaceArraySize;
*XMLElementDecl_fgInvalidElemId = *XML::Xercesc::XMLElementDecl_fgInvalidElemId;
*XMLElementDecl_fgPCDataElemId = *XML::Xercesc::XMLElementDecl_fgPCDataElemId;
*XMLElementDecl_fgPCDataElemName = *XML::Xercesc::XMLElementDecl_fgPCDataElemName;
*XMLAttDef_fgInvalidAttrId = *XML::Xercesc::XMLAttDef_fgInvalidAttrId;
*PERLCALLBACKHANDLER_BASE_TYPE = *XML::Xercesc::PERLCALLBACKHANDLER_BASE_TYPE;
*PERLCALLBACKHANDLER_ERROR_TYPE = *XML::Xercesc::PERLCALLBACKHANDLER_ERROR_TYPE;
*PERLCALLBACKHANDLER_ENTITY_TYPE = *XML::Xercesc::PERLCALLBACKHANDLER_ENTITY_TYPE;
*PERLCALLBACKHANDLER_NODE_TYPE = *XML::Xercesc::PERLCALLBACKHANDLER_NODE_TYPE;
*PERLCALLBACKHANDLER_CONTENT_TYPE = *XML::Xercesc::PERLCALLBACKHANDLER_CONTENT_TYPE;
*PERLCALLBACKHANDLER_DOCUMENT_TYPE = *XML::Xercesc::PERLCALLBACKHANDLER_DOCUMENT_TYPE;
1;
