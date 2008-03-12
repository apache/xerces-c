 #
 # Licensed to the Apache Software Foundation (ASF) under one or more
 # contributor license agreements.  See the NOTICE file distributed with
 # this work for additional information regarding copyright ownership.
 # The ASF licenses this file to You under the Apache License, Version 2.0
 # (the "License"); you may not use this file except in compliance with
 # the License.  You may obtain a copy of the License at
 #
 #      http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #

# This file was created automatically by SWIG 1.3.30.
# Don't modify this file, modify the SWIG interface instead.
package XML::Xerces;
use base qw(Exporter);
use base qw(DynaLoader);
bootstrap XML::Xerces;
package XML::Xerces;
@EXPORT = qw();
package XML::Xerces;

use strict;
use Carp;
use vars qw(@EXPORT_OK $VERSION %REMEMBER);
@EXPORT_OK = qw(fatal_error error);
$VERSION = 300.061003;

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

sub fatal_error {
  my $error = shift;
  my $context = shift;
  error($error,$context, my $fatal = 1);
}

sub error {
  my $error = shift;
  my $context = shift;
  my $fatal = defined (shift) ? 1 : 0;
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
  if ($fatal) {
    croak($msg);
  } else {
    carp($msg);
  }
}

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

############# Class : XML::Xerces::PerlEntityResolver ##############
package XML::Xerces::PerlEntityResolver;
use vars qw(@ISA);
@ISA = qw(XML::Xerces::XMLEntityResolver XML::Xerces::EntityResolver);
sub new {
  my $class = shift;
  return bless {}, $class;
}

sub resolve_entity {
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

package XML::Xerces::XMLChVector;
# convert the XMLChVector to a perl list
sub to_list {
  my $self = shift;
  my @list;
  my $count = $self->size();
  if ($count) {
    for (my $i=0;$i<$count;$i++) {
      push(@list,$self->elementAt($i));
    }
  }
  return @list;
}

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
  my $count = $self->getAttDefCount();
  if ($count) {
    for (my $i=0;$i<$count;$i++) {
      push(@list,$self->getAttDef($i));
    }
  }
  return @list;
}

# convert the AttDefList to a perl hash
sub to_hash {
  my $self = shift;
  my %hash;
  my $count = $self->getAttDefCount();
  if ($count) {
    for (my $i=0;$i<$count;$i++) {
      my $attr = $self->getAttDef($i);
      $hash{$attr->getFullName()} = $attr;
    }
  }
  return %hash;
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
  XML::Xerces::fatal_error(__PACKAGE__ . ": Must set catalog before calling initialize")
      unless defined $CATALOG;

  my $DOM = XML::Xerces::XercesDOMParser->new();
  my $ERROR_HANDLER = XML::Xerces::PerlErrorHandler->new();
  $DOM->setErrorHandler($ERROR_HANDLER);

  # we parse the example XML Catalog
  eval{$DOM->parse($CATALOG)};
  XML::Xerces::fatal_error($@, __PACKAGE__ . ": Couldn't parse catalog: $CATALOG")
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

  #
  # FIXME - this should be creating and throwing an exception
  #
  XML::Xerces::fatal_error(__PACKAGE__ . ": Must call initialize before using the resolver")
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
*fgUCS4EncodingString4 = *XML::Xercesc::XMLUni_fgUCS4EncodingString4;
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
*fgXMLXIncludeException_Name = *XML::Xercesc::XMLUni_fgXMLXIncludeException_Name;
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
*fgXercesHandleMultipleImports = *XML::Xercesc::XMLUni_fgXercesHandleMultipleImports;
*fgXercesDoXInclude = *XML::Xercesc::XMLUni_fgXercesDoXInclude;
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
*fgXercescInterfaceDOMDocumentImpl = *XML::Xercesc::XMLUni_fgXercescInterfaceDOMDocumentImpl;
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


############# Class : XML::Xerces::SchemaSymbols ##############

package XML::Xerces::SchemaSymbols;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*fgURI_XSI = *XML::Xercesc::SchemaSymbols_fgURI_XSI;
*fgURI_SCHEMAFORSCHEMA = *XML::Xercesc::SchemaSymbols_fgURI_SCHEMAFORSCHEMA;
*fgXSI_SCHEMALOCACTION = *XML::Xercesc::SchemaSymbols_fgXSI_SCHEMALOCACTION;
*fgXSI_NONAMESPACESCHEMALOCACTION = *XML::Xercesc::SchemaSymbols_fgXSI_NONAMESPACESCHEMALOCACTION;
*fgXSI_TYPE = *XML::Xercesc::SchemaSymbols_fgXSI_TYPE;
*fgELT_ALL = *XML::Xercesc::SchemaSymbols_fgELT_ALL;
*fgELT_ANNOTATION = *XML::Xercesc::SchemaSymbols_fgELT_ANNOTATION;
*fgELT_ANY = *XML::Xercesc::SchemaSymbols_fgELT_ANY;
*fgELT_WILDCARD = *XML::Xercesc::SchemaSymbols_fgELT_WILDCARD;
*fgELT_ANYATTRIBUTE = *XML::Xercesc::SchemaSymbols_fgELT_ANYATTRIBUTE;
*fgELT_APPINFO = *XML::Xercesc::SchemaSymbols_fgELT_APPINFO;
*fgELT_ATTRIBUTE = *XML::Xercesc::SchemaSymbols_fgELT_ATTRIBUTE;
*fgELT_ATTRIBUTEGROUP = *XML::Xercesc::SchemaSymbols_fgELT_ATTRIBUTEGROUP;
*fgELT_CHOICE = *XML::Xercesc::SchemaSymbols_fgELT_CHOICE;
*fgELT_COMPLEXTYPE = *XML::Xercesc::SchemaSymbols_fgELT_COMPLEXTYPE;
*fgELT_CONTENT = *XML::Xercesc::SchemaSymbols_fgELT_CONTENT;
*fgELT_DOCUMENTATION = *XML::Xercesc::SchemaSymbols_fgELT_DOCUMENTATION;
*fgELT_DURATION = *XML::Xercesc::SchemaSymbols_fgELT_DURATION;
*fgELT_ELEMENT = *XML::Xercesc::SchemaSymbols_fgELT_ELEMENT;
*fgELT_ENCODING = *XML::Xercesc::SchemaSymbols_fgELT_ENCODING;
*fgELT_ENUMERATION = *XML::Xercesc::SchemaSymbols_fgELT_ENUMERATION;
*fgELT_FIELD = *XML::Xercesc::SchemaSymbols_fgELT_FIELD;
*fgELT_WHITESPACE = *XML::Xercesc::SchemaSymbols_fgELT_WHITESPACE;
*fgELT_GROUP = *XML::Xercesc::SchemaSymbols_fgELT_GROUP;
*fgELT_IMPORT = *XML::Xercesc::SchemaSymbols_fgELT_IMPORT;
*fgELT_INCLUDE = *XML::Xercesc::SchemaSymbols_fgELT_INCLUDE;
*fgELT_REDEFINE = *XML::Xercesc::SchemaSymbols_fgELT_REDEFINE;
*fgELT_KEY = *XML::Xercesc::SchemaSymbols_fgELT_KEY;
*fgELT_KEYREF = *XML::Xercesc::SchemaSymbols_fgELT_KEYREF;
*fgELT_LENGTH = *XML::Xercesc::SchemaSymbols_fgELT_LENGTH;
*fgELT_MAXEXCLUSIVE = *XML::Xercesc::SchemaSymbols_fgELT_MAXEXCLUSIVE;
*fgELT_MAXINCLUSIVE = *XML::Xercesc::SchemaSymbols_fgELT_MAXINCLUSIVE;
*fgELT_MAXLENGTH = *XML::Xercesc::SchemaSymbols_fgELT_MAXLENGTH;
*fgELT_MINEXCLUSIVE = *XML::Xercesc::SchemaSymbols_fgELT_MINEXCLUSIVE;
*fgELT_MININCLUSIVE = *XML::Xercesc::SchemaSymbols_fgELT_MININCLUSIVE;
*fgELT_MINLENGTH = *XML::Xercesc::SchemaSymbols_fgELT_MINLENGTH;
*fgELT_NOTATION = *XML::Xercesc::SchemaSymbols_fgELT_NOTATION;
*fgELT_PATTERN = *XML::Xercesc::SchemaSymbols_fgELT_PATTERN;
*fgELT_PERIOD = *XML::Xercesc::SchemaSymbols_fgELT_PERIOD;
*fgELT_TOTALDIGITS = *XML::Xercesc::SchemaSymbols_fgELT_TOTALDIGITS;
*fgELT_FRACTIONDIGITS = *XML::Xercesc::SchemaSymbols_fgELT_FRACTIONDIGITS;
*fgELT_SCHEMA = *XML::Xercesc::SchemaSymbols_fgELT_SCHEMA;
*fgELT_SELECTOR = *XML::Xercesc::SchemaSymbols_fgELT_SELECTOR;
*fgELT_SEQUENCE = *XML::Xercesc::SchemaSymbols_fgELT_SEQUENCE;
*fgELT_SIMPLETYPE = *XML::Xercesc::SchemaSymbols_fgELT_SIMPLETYPE;
*fgELT_UNION = *XML::Xercesc::SchemaSymbols_fgELT_UNION;
*fgELT_LIST = *XML::Xercesc::SchemaSymbols_fgELT_LIST;
*fgELT_UNIQUE = *XML::Xercesc::SchemaSymbols_fgELT_UNIQUE;
*fgELT_COMPLEXCONTENT = *XML::Xercesc::SchemaSymbols_fgELT_COMPLEXCONTENT;
*fgELT_SIMPLECONTENT = *XML::Xercesc::SchemaSymbols_fgELT_SIMPLECONTENT;
*fgELT_RESTRICTION = *XML::Xercesc::SchemaSymbols_fgELT_RESTRICTION;
*fgELT_EXTENSION = *XML::Xercesc::SchemaSymbols_fgELT_EXTENSION;
*fgATT_ABSTRACT = *XML::Xercesc::SchemaSymbols_fgATT_ABSTRACT;
*fgATT_ATTRIBUTEFORMDEFAULT = *XML::Xercesc::SchemaSymbols_fgATT_ATTRIBUTEFORMDEFAULT;
*fgATT_BASE = *XML::Xercesc::SchemaSymbols_fgATT_BASE;
*fgATT_ITEMTYPE = *XML::Xercesc::SchemaSymbols_fgATT_ITEMTYPE;
*fgATT_MEMBERTYPES = *XML::Xercesc::SchemaSymbols_fgATT_MEMBERTYPES;
*fgATT_BLOCK = *XML::Xercesc::SchemaSymbols_fgATT_BLOCK;
*fgATT_BLOCKDEFAULT = *XML::Xercesc::SchemaSymbols_fgATT_BLOCKDEFAULT;
*fgATT_DEFAULT = *XML::Xercesc::SchemaSymbols_fgATT_DEFAULT;
*fgATT_ELEMENTFORMDEFAULT = *XML::Xercesc::SchemaSymbols_fgATT_ELEMENTFORMDEFAULT;
*fgATT_SUBSTITUTIONGROUP = *XML::Xercesc::SchemaSymbols_fgATT_SUBSTITUTIONGROUP;
*fgATT_FINAL = *XML::Xercesc::SchemaSymbols_fgATT_FINAL;
*fgATT_FINALDEFAULT = *XML::Xercesc::SchemaSymbols_fgATT_FINALDEFAULT;
*fgATT_FIXED = *XML::Xercesc::SchemaSymbols_fgATT_FIXED;
*fgATT_FORM = *XML::Xercesc::SchemaSymbols_fgATT_FORM;
*fgATT_ID = *XML::Xercesc::SchemaSymbols_fgATT_ID;
*fgATT_MAXOCCURS = *XML::Xercesc::SchemaSymbols_fgATT_MAXOCCURS;
*fgATT_MINOCCURS = *XML::Xercesc::SchemaSymbols_fgATT_MINOCCURS;
*fgATT_NAME = *XML::Xercesc::SchemaSymbols_fgATT_NAME;
*fgATT_NAMESPACE = *XML::Xercesc::SchemaSymbols_fgATT_NAMESPACE;
*fgATT_NILL = *XML::Xercesc::SchemaSymbols_fgATT_NILL;
*fgATT_NILLABLE = *XML::Xercesc::SchemaSymbols_fgATT_NILLABLE;
*fgATT_PROCESSCONTENTS = *XML::Xercesc::SchemaSymbols_fgATT_PROCESSCONTENTS;
*fgATT_REF = *XML::Xercesc::SchemaSymbols_fgATT_REF;
*fgATT_REFER = *XML::Xercesc::SchemaSymbols_fgATT_REFER;
*fgATT_SCHEMALOCATION = *XML::Xercesc::SchemaSymbols_fgATT_SCHEMALOCATION;
*fgATT_SOURCE = *XML::Xercesc::SchemaSymbols_fgATT_SOURCE;
*fgATT_SYSTEM = *XML::Xercesc::SchemaSymbols_fgATT_SYSTEM;
*fgATT_PUBLIC = *XML::Xercesc::SchemaSymbols_fgATT_PUBLIC;
*fgATT_TARGETNAMESPACE = *XML::Xercesc::SchemaSymbols_fgATT_TARGETNAMESPACE;
*fgATT_TYPE = *XML::Xercesc::SchemaSymbols_fgATT_TYPE;
*fgATT_USE = *XML::Xercesc::SchemaSymbols_fgATT_USE;
*fgATT_VALUE = *XML::Xercesc::SchemaSymbols_fgATT_VALUE;
*fgATT_MIXED = *XML::Xercesc::SchemaSymbols_fgATT_MIXED;
*fgATT_VERSION = *XML::Xercesc::SchemaSymbols_fgATT_VERSION;
*fgATT_XPATH = *XML::Xercesc::SchemaSymbols_fgATT_XPATH;
*fgATTVAL_TWOPOUNDANY = *XML::Xercesc::SchemaSymbols_fgATTVAL_TWOPOUNDANY;
*fgATTVAL_TWOPOUNDLOCAL = *XML::Xercesc::SchemaSymbols_fgATTVAL_TWOPOUNDLOCAL;
*fgATTVAL_TWOPOUNDOTHER = *XML::Xercesc::SchemaSymbols_fgATTVAL_TWOPOUNDOTHER;
*fgATTVAL_TWOPOUNDTRAGETNAMESPACE = *XML::Xercesc::SchemaSymbols_fgATTVAL_TWOPOUNDTRAGETNAMESPACE;
*fgATTVAL_POUNDALL = *XML::Xercesc::SchemaSymbols_fgATTVAL_POUNDALL;
*fgATTVAL_BASE64 = *XML::Xercesc::SchemaSymbols_fgATTVAL_BASE64;
*fgATTVAL_BOOLEAN = *XML::Xercesc::SchemaSymbols_fgATTVAL_BOOLEAN;
*fgATTVAL_DEFAULT = *XML::Xercesc::SchemaSymbols_fgATTVAL_DEFAULT;
*fgATTVAL_ELEMENTONLY = *XML::Xercesc::SchemaSymbols_fgATTVAL_ELEMENTONLY;
*fgATTVAL_EMPTY = *XML::Xercesc::SchemaSymbols_fgATTVAL_EMPTY;
*fgATTVAL_EXTENSION = *XML::Xercesc::SchemaSymbols_fgATTVAL_EXTENSION;
*fgATTVAL_FALSE = *XML::Xercesc::SchemaSymbols_fgATTVAL_FALSE;
*fgATTVAL_FIXED = *XML::Xercesc::SchemaSymbols_fgATTVAL_FIXED;
*fgATTVAL_HEX = *XML::Xercesc::SchemaSymbols_fgATTVAL_HEX;
*fgATTVAL_ID = *XML::Xercesc::SchemaSymbols_fgATTVAL_ID;
*fgATTVAL_LAX = *XML::Xercesc::SchemaSymbols_fgATTVAL_LAX;
*fgATTVAL_MAXLENGTH = *XML::Xercesc::SchemaSymbols_fgATTVAL_MAXLENGTH;
*fgATTVAL_MINLENGTH = *XML::Xercesc::SchemaSymbols_fgATTVAL_MINLENGTH;
*fgATTVAL_MIXED = *XML::Xercesc::SchemaSymbols_fgATTVAL_MIXED;
*fgATTVAL_NCNAME = *XML::Xercesc::SchemaSymbols_fgATTVAL_NCNAME;
*fgATTVAL_OPTIONAL = *XML::Xercesc::SchemaSymbols_fgATTVAL_OPTIONAL;
*fgATTVAL_PROHIBITED = *XML::Xercesc::SchemaSymbols_fgATTVAL_PROHIBITED;
*fgATTVAL_QNAME = *XML::Xercesc::SchemaSymbols_fgATTVAL_QNAME;
*fgATTVAL_QUALIFIED = *XML::Xercesc::SchemaSymbols_fgATTVAL_QUALIFIED;
*fgATTVAL_REQUIRED = *XML::Xercesc::SchemaSymbols_fgATTVAL_REQUIRED;
*fgATTVAL_RESTRICTION = *XML::Xercesc::SchemaSymbols_fgATTVAL_RESTRICTION;
*fgATTVAL_SKIP = *XML::Xercesc::SchemaSymbols_fgATTVAL_SKIP;
*fgATTVAL_STRICT = *XML::Xercesc::SchemaSymbols_fgATTVAL_STRICT;
*fgATTVAL_STRING = *XML::Xercesc::SchemaSymbols_fgATTVAL_STRING;
*fgATTVAL_TEXTONLY = *XML::Xercesc::SchemaSymbols_fgATTVAL_TEXTONLY;
*fgATTVAL_TIMEDURATION = *XML::Xercesc::SchemaSymbols_fgATTVAL_TIMEDURATION;
*fgATTVAL_TRUE = *XML::Xercesc::SchemaSymbols_fgATTVAL_TRUE;
*fgATTVAL_UNQUALIFIED = *XML::Xercesc::SchemaSymbols_fgATTVAL_UNQUALIFIED;
*fgATTVAL_URI = *XML::Xercesc::SchemaSymbols_fgATTVAL_URI;
*fgATTVAL_URIREFERENCE = *XML::Xercesc::SchemaSymbols_fgATTVAL_URIREFERENCE;
*fgATTVAL_SUBSTITUTIONGROUP = *XML::Xercesc::SchemaSymbols_fgATTVAL_SUBSTITUTIONGROUP;
*fgATTVAL_SUBSTITUTION = *XML::Xercesc::SchemaSymbols_fgATTVAL_SUBSTITUTION;
*fgATTVAL_ANYTYPE = *XML::Xercesc::SchemaSymbols_fgATTVAL_ANYTYPE;
*fgWS_PRESERVE = *XML::Xercesc::SchemaSymbols_fgWS_PRESERVE;
*fgWS_COLLAPSE = *XML::Xercesc::SchemaSymbols_fgWS_COLLAPSE;
*fgWS_REPLACE = *XML::Xercesc::SchemaSymbols_fgWS_REPLACE;
*fgDT_STRING = *XML::Xercesc::SchemaSymbols_fgDT_STRING;
*fgDT_TOKEN = *XML::Xercesc::SchemaSymbols_fgDT_TOKEN;
*fgDT_LANGUAGE = *XML::Xercesc::SchemaSymbols_fgDT_LANGUAGE;
*fgDT_NAME = *XML::Xercesc::SchemaSymbols_fgDT_NAME;
*fgDT_NCNAME = *XML::Xercesc::SchemaSymbols_fgDT_NCNAME;
*fgDT_INTEGER = *XML::Xercesc::SchemaSymbols_fgDT_INTEGER;
*fgDT_DECIMAL = *XML::Xercesc::SchemaSymbols_fgDT_DECIMAL;
*fgDT_BOOLEAN = *XML::Xercesc::SchemaSymbols_fgDT_BOOLEAN;
*fgDT_NONPOSITIVEINTEGER = *XML::Xercesc::SchemaSymbols_fgDT_NONPOSITIVEINTEGER;
*fgDT_NEGATIVEINTEGER = *XML::Xercesc::SchemaSymbols_fgDT_NEGATIVEINTEGER;
*fgDT_LONG = *XML::Xercesc::SchemaSymbols_fgDT_LONG;
*fgDT_INT = *XML::Xercesc::SchemaSymbols_fgDT_INT;
*fgDT_SHORT = *XML::Xercesc::SchemaSymbols_fgDT_SHORT;
*fgDT_BYTE = *XML::Xercesc::SchemaSymbols_fgDT_BYTE;
*fgDT_NONNEGATIVEINTEGER = *XML::Xercesc::SchemaSymbols_fgDT_NONNEGATIVEINTEGER;
*fgDT_ULONG = *XML::Xercesc::SchemaSymbols_fgDT_ULONG;
*fgDT_UINT = *XML::Xercesc::SchemaSymbols_fgDT_UINT;
*fgDT_USHORT = *XML::Xercesc::SchemaSymbols_fgDT_USHORT;
*fgDT_UBYTE = *XML::Xercesc::SchemaSymbols_fgDT_UBYTE;
*fgDT_POSITIVEINTEGER = *XML::Xercesc::SchemaSymbols_fgDT_POSITIVEINTEGER;
*fgDT_DATETIME = *XML::Xercesc::SchemaSymbols_fgDT_DATETIME;
*fgDT_DATE = *XML::Xercesc::SchemaSymbols_fgDT_DATE;
*fgDT_TIME = *XML::Xercesc::SchemaSymbols_fgDT_TIME;
*fgDT_DURATION = *XML::Xercesc::SchemaSymbols_fgDT_DURATION;
*fgDT_DAY = *XML::Xercesc::SchemaSymbols_fgDT_DAY;
*fgDT_MONTH = *XML::Xercesc::SchemaSymbols_fgDT_MONTH;
*fgDT_MONTHDAY = *XML::Xercesc::SchemaSymbols_fgDT_MONTHDAY;
*fgDT_YEAR = *XML::Xercesc::SchemaSymbols_fgDT_YEAR;
*fgDT_YEARMONTH = *XML::Xercesc::SchemaSymbols_fgDT_YEARMONTH;
*fgDT_BASE64BINARY = *XML::Xercesc::SchemaSymbols_fgDT_BASE64BINARY;
*fgDT_HEXBINARY = *XML::Xercesc::SchemaSymbols_fgDT_HEXBINARY;
*fgDT_FLOAT = *XML::Xercesc::SchemaSymbols_fgDT_FLOAT;
*fgDT_DOUBLE = *XML::Xercesc::SchemaSymbols_fgDT_DOUBLE;
*fgDT_URIREFERENCE = *XML::Xercesc::SchemaSymbols_fgDT_URIREFERENCE;
*fgDT_ANYURI = *XML::Xercesc::SchemaSymbols_fgDT_ANYURI;
*fgDT_QNAME = *XML::Xercesc::SchemaSymbols_fgDT_QNAME;
*fgDT_NORMALIZEDSTRING = *XML::Xercesc::SchemaSymbols_fgDT_NORMALIZEDSTRING;
*fgDT_ANYSIMPLETYPE = *XML::Xercesc::SchemaSymbols_fgDT_ANYSIMPLETYPE;
*fgRegEx_XOption = *XML::Xercesc::SchemaSymbols_fgRegEx_XOption;
*fgRedefIdentifier = *XML::Xercesc::SchemaSymbols_fgRedefIdentifier;
*fgINT_MIN_VALUE = *XML::Xercesc::SchemaSymbols_fgINT_MIN_VALUE;
*fgINT_MAX_VALUE = *XML::Xercesc::SchemaSymbols_fgINT_MAX_VALUE;
*XSD_EMPTYSET = *XML::Xercesc::SchemaSymbols_XSD_EMPTYSET;
*XSD_SUBSTITUTION = *XML::Xercesc::SchemaSymbols_XSD_SUBSTITUTION;
*XSD_EXTENSION = *XML::Xercesc::SchemaSymbols_XSD_EXTENSION;
*XSD_RESTRICTION = *XML::Xercesc::SchemaSymbols_XSD_RESTRICTION;
*XSD_LIST = *XML::Xercesc::SchemaSymbols_XSD_LIST;
*XSD_UNION = *XML::Xercesc::SchemaSymbols_XSD_UNION;
*XSD_ENUMERATION = *XML::Xercesc::SchemaSymbols_XSD_ENUMERATION;
*XSD_CHOICE = *XML::Xercesc::SchemaSymbols_XSD_CHOICE;
*XSD_SEQUENCE = *XML::Xercesc::SchemaSymbols_XSD_SEQUENCE;
*XSD_ALL = *XML::Xercesc::SchemaSymbols_XSD_ALL;
*XSD_UNBOUNDED = *XML::Xercesc::SchemaSymbols_XSD_UNBOUNDED;
*XSD_NILLABLE = *XML::Xercesc::SchemaSymbols_XSD_NILLABLE;
*XSD_ABSTRACT = *XML::Xercesc::SchemaSymbols_XSD_ABSTRACT;
*XSD_FIXED = *XML::Xercesc::SchemaSymbols_XSD_FIXED;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SchemaSymbols($self);
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


############# Class : XML::Xerces::PSVIUni ##############

package XML::Xerces::PSVIUni;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*fgPsvColon = *XML::Xercesc::PSVIUni_fgPsvColon;
*fgAllDeclarationsProcessed = *XML::Xercesc::PSVIUni_fgAllDeclarationsProcessed;
*fgAttribute = *XML::Xercesc::PSVIUni_fgAttribute;
*fgAttributes = *XML::Xercesc::PSVIUni_fgAttributes;
*fgAttributeType = *XML::Xercesc::PSVIUni_fgAttributeType;
*fgBaseURI = *XML::Xercesc::PSVIUni_fgBaseURI;
*fgCharacter = *XML::Xercesc::PSVIUni_fgCharacter;
*fgCharacterEncodingScheme = *XML::Xercesc::PSVIUni_fgCharacterEncodingScheme;
*fgChildren = *XML::Xercesc::PSVIUni_fgChildren;
*fgComment = *XML::Xercesc::PSVIUni_fgComment;
*fgContent = *XML::Xercesc::PSVIUni_fgContent;
*fgDocument = *XML::Xercesc::PSVIUni_fgDocument;
*fgDocTypeDeclaration = *XML::Xercesc::PSVIUni_fgDocTypeDeclaration;
*fgDocumentElement = *XML::Xercesc::PSVIUni_fgDocumentElement;
*fgElement = *XML::Xercesc::PSVIUni_fgElement;
*fgInScopeNamespaces = *XML::Xercesc::PSVIUni_fgInScopeNamespaces;
*fgLocalName = *XML::Xercesc::PSVIUni_fgLocalName;
*fgNamespace = *XML::Xercesc::PSVIUni_fgNamespace;
*fgNamespaceAttributes = *XML::Xercesc::PSVIUni_fgNamespaceAttributes;
*fgNamespaceName = *XML::Xercesc::PSVIUni_fgNamespaceName;
*fgNormalizedValue = *XML::Xercesc::PSVIUni_fgNormalizedValue;
*fgNotations = *XML::Xercesc::PSVIUni_fgNotations;
*fgPrefix = *XML::Xercesc::PSVIUni_fgPrefix;
*fgProcessingInstruction = *XML::Xercesc::PSVIUni_fgProcessingInstruction;
*fgReferences = *XML::Xercesc::PSVIUni_fgReferences;
*fgSpecified = *XML::Xercesc::PSVIUni_fgSpecified;
*fgStandalone = *XML::Xercesc::PSVIUni_fgStandalone;
*fgTarget = *XML::Xercesc::PSVIUni_fgTarget;
*fgText = *XML::Xercesc::PSVIUni_fgText;
*fgTextContent = *XML::Xercesc::PSVIUni_fgTextContent;
*fgUnparsedEntities = *XML::Xercesc::PSVIUni_fgUnparsedEntities;
*fgVersion = *XML::Xercesc::PSVIUni_fgVersion;
*fgAbstract = *XML::Xercesc::PSVIUni_fgAbstract;
*fgAnnotation = *XML::Xercesc::PSVIUni_fgAnnotation;
*fgAnnotations = *XML::Xercesc::PSVIUni_fgAnnotations;
*fgApplicationInformation = *XML::Xercesc::PSVIUni_fgApplicationInformation;
*fgAttributeDeclaration = *XML::Xercesc::PSVIUni_fgAttributeDeclaration;
*fgAttributeGroupDefinition = *XML::Xercesc::PSVIUni_fgAttributeGroupDefinition;
*fgAttributeUse = *XML::Xercesc::PSVIUni_fgAttributeUse;
*fgAttributeUses = *XML::Xercesc::PSVIUni_fgAttributeUses;
*fgAttributeWildcard = *XML::Xercesc::PSVIUni_fgAttributeWildcard;
*fgBaseTypeDefinition = *XML::Xercesc::PSVIUni_fgBaseTypeDefinition;
*fgCanonicalRepresentation = *XML::Xercesc::PSVIUni_fgCanonicalRepresentation;
*fgComplexTypeDefinition = *XML::Xercesc::PSVIUni_fgComplexTypeDefinition;
*fgCompositor = *XML::Xercesc::PSVIUni_fgCompositor;
*fgContentType = *XML::Xercesc::PSVIUni_fgContentType;
*fgDeclaration = *XML::Xercesc::PSVIUni_fgDeclaration;
*fgDerivationMethod = *XML::Xercesc::PSVIUni_fgDerivationMethod;
*fgDisallowedSubstitutions = *XML::Xercesc::PSVIUni_fgDisallowedSubstitutions;
*fgPsvDocument = *XML::Xercesc::PSVIUni_fgPsvDocument;
*fgDocumentLocation = *XML::Xercesc::PSVIUni_fgDocumentLocation;
*fgElementDeclaration = *XML::Xercesc::PSVIUni_fgElementDeclaration;
*fgFacets = *XML::Xercesc::PSVIUni_fgFacets;
*fgFacetFixed = *XML::Xercesc::PSVIUni_fgFacetFixed;
*fgFields = *XML::Xercesc::PSVIUni_fgFields;
*fgFinal = *XML::Xercesc::PSVIUni_fgFinal;
*fgFundamentalFacets = *XML::Xercesc::PSVIUni_fgFundamentalFacets;
*fgIdentityConstraintCategory = *XML::Xercesc::PSVIUni_fgIdentityConstraintCategory;
*fgIdentityConstraintDefinition = *XML::Xercesc::PSVIUni_fgIdentityConstraintDefinition;
*fgIdentityConstraintDefinitions = *XML::Xercesc::PSVIUni_fgIdentityConstraintDefinitions;
*fgIdentityConstraintTable = *XML::Xercesc::PSVIUni_fgIdentityConstraintTable;
*fgIdIdrefTable = *XML::Xercesc::PSVIUni_fgIdIdrefTable;
*fgItemTypeDefinition = *XML::Xercesc::PSVIUni_fgItemTypeDefinition;
*fgMaxOccurs = *XML::Xercesc::PSVIUni_fgMaxOccurs;
*fgMemberTypeDefinition = *XML::Xercesc::PSVIUni_fgMemberTypeDefinition;
*fgMemberTypeDefinitions = *XML::Xercesc::PSVIUni_fgMemberTypeDefinitions;
*fgMinOccurs = *XML::Xercesc::PSVIUni_fgMinOccurs;
*fgModelGroup = *XML::Xercesc::PSVIUni_fgModelGroup;
*fgModelGroupDefinition = *XML::Xercesc::PSVIUni_fgModelGroupDefinition;
*fgName = *XML::Xercesc::PSVIUni_fgName;
*fgNamespaceConstraint = *XML::Xercesc::PSVIUni_fgNamespaceConstraint;
*fgNamespaces = *XML::Xercesc::PSVIUni_fgNamespaces;
*fgNamespaceSchemaInformation = *XML::Xercesc::PSVIUni_fgNamespaceSchemaInformation;
*fgNil = *XML::Xercesc::PSVIUni_fgNil;
*fgNillable = *XML::Xercesc::PSVIUni_fgNillable;
*fgNotation = *XML::Xercesc::PSVIUni_fgNotation;
*fgNotationDeclaration = *XML::Xercesc::PSVIUni_fgNotationDeclaration;
*fgParticle = *XML::Xercesc::PSVIUni_fgParticle;
*fgParticles = *XML::Xercesc::PSVIUni_fgParticles;
*fgPrimitiveTypeDefinition = *XML::Xercesc::PSVIUni_fgPrimitiveTypeDefinition;
*fgProcessContents = *XML::Xercesc::PSVIUni_fgProcessContents;
*fgProhibitedSubstitutions = *XML::Xercesc::PSVIUni_fgProhibitedSubstitutions;
*fgPublicIdentifier = *XML::Xercesc::PSVIUni_fgPublicIdentifier;
*fgReferencedKey = *XML::Xercesc::PSVIUni_fgReferencedKey;
*fgRequired = *XML::Xercesc::PSVIUni_fgRequired;
*fgSchemaAnnotations = *XML::Xercesc::PSVIUni_fgSchemaAnnotations;
*fgSchemaComponents = *XML::Xercesc::PSVIUni_fgSchemaComponents;
*fgSchemaDefault = *XML::Xercesc::PSVIUni_fgSchemaDefault;
*fgSchemaDocument = *XML::Xercesc::PSVIUni_fgSchemaDocument;
*fgSchemaDocuments = *XML::Xercesc::PSVIUni_fgSchemaDocuments;
*fgSchemaErrorCode = *XML::Xercesc::PSVIUni_fgSchemaErrorCode;
*fgSchemaInformation = *XML::Xercesc::PSVIUni_fgSchemaInformation;
*fgSchemaNamespace = *XML::Xercesc::PSVIUni_fgSchemaNamespace;
*fgSchemaNormalizedValue = *XML::Xercesc::PSVIUni_fgSchemaNormalizedValue;
*fgSchemaSpecified = *XML::Xercesc::PSVIUni_fgSchemaSpecified;
*fgScope = *XML::Xercesc::PSVIUni_fgScope;
*fgSelector = *XML::Xercesc::PSVIUni_fgSelector;
*fgSimpleTypeDefinition = *XML::Xercesc::PSVIUni_fgSimpleTypeDefinition;
*fgSubstitutionGroupAffiliation = *XML::Xercesc::PSVIUni_fgSubstitutionGroupAffiliation;
*fgSubstitutionGroupExclusions = *XML::Xercesc::PSVIUni_fgSubstitutionGroupExclusions;
*fgSystemIdentifier = *XML::Xercesc::PSVIUni_fgSystemIdentifier;
*fgTargetNamespace = *XML::Xercesc::PSVIUni_fgTargetNamespace;
*fgTerm = *XML::Xercesc::PSVIUni_fgTerm;
*fgTypeDefinition = *XML::Xercesc::PSVIUni_fgTypeDefinition;
*fgUserInformation = *XML::Xercesc::PSVIUni_fgUserInformation;
*fgValidationAttempted = *XML::Xercesc::PSVIUni_fgValidationAttempted;
*fgValidationContext = *XML::Xercesc::PSVIUni_fgValidationContext;
*fgValidity = *XML::Xercesc::PSVIUni_fgValidity;
*fgValue = *XML::Xercesc::PSVIUni_fgValue;
*fgValueConstraint = *XML::Xercesc::PSVIUni_fgValueConstraint;
*fgVariety = *XML::Xercesc::PSVIUni_fgVariety;
*fgWildcard = *XML::Xercesc::PSVIUni_fgWildcard;
*fgXpath = *XML::Xercesc::PSVIUni_fgXpath;
*fgAll = *XML::Xercesc::PSVIUni_fgAll;
*fgAny = *XML::Xercesc::PSVIUni_fgAny;
*fgAppinfo = *XML::Xercesc::PSVIUni_fgAppinfo;
*fgAtomic = *XML::Xercesc::PSVIUni_fgAtomic;
*fgChoice = *XML::Xercesc::PSVIUni_fgChoice;
*fgDefault = *XML::Xercesc::PSVIUni_fgDefault;
*fgDocumentation = *XML::Xercesc::PSVIUni_fgDocumentation;
*fgElementOnly = *XML::Xercesc::PSVIUni_fgElementOnly;
*fgEmpty = *XML::Xercesc::PSVIUni_fgEmpty;
*fgExtension = *XML::Xercesc::PSVIUni_fgExtension;
*fgFalse = *XML::Xercesc::PSVIUni_fgFalse;
*fgFull = *XML::Xercesc::PSVIUni_fgFull;
*fgGlobal = *XML::Xercesc::PSVIUni_fgGlobal;
*fgInfoset = *XML::Xercesc::PSVIUni_fgInfoset;
*fgInvalid = *XML::Xercesc::PSVIUni_fgInvalid;
*fgKey = *XML::Xercesc::PSVIUni_fgKey;
*fgKeyref = *XML::Xercesc::PSVIUni_fgKeyref;
*fgLax = *XML::Xercesc::PSVIUni_fgLax;
*fgList = *XML::Xercesc::PSVIUni_fgList;
*fgLocal = *XML::Xercesc::PSVIUni_fgLocal;
*fgMixed = *XML::Xercesc::PSVIUni_fgMixed;
*fgNone = *XML::Xercesc::PSVIUni_fgNone;
*fgNotKnown = *XML::Xercesc::PSVIUni_fgNotKnown;
*fgNsNamespace = *XML::Xercesc::PSVIUni_fgNsNamespace;
*fgOnePointZero = *XML::Xercesc::PSVIUni_fgOnePointZero;
*fgPartial = *XML::Xercesc::PSVIUni_fgPartial;
*fgRestrict = *XML::Xercesc::PSVIUni_fgRestrict;
*fgRestriction = *XML::Xercesc::PSVIUni_fgRestriction;
*fgSchema = *XML::Xercesc::PSVIUni_fgSchema;
*fgSequence = *XML::Xercesc::PSVIUni_fgSequence;
*fgSimple = *XML::Xercesc::PSVIUni_fgSimple;
*fgSkip = *XML::Xercesc::PSVIUni_fgSkip;
*fgStrict = *XML::Xercesc::PSVIUni_fgStrict;
*fgSubstitution = *XML::Xercesc::PSVIUni_fgSubstitution;
*fgTotal = *XML::Xercesc::PSVIUni_fgTotal;
*fgTrue = *XML::Xercesc::PSVIUni_fgTrue;
*fgUnbounded = *XML::Xercesc::PSVIUni_fgUnbounded;
*fgUnion = *XML::Xercesc::PSVIUni_fgUnion;
*fgUnique = *XML::Xercesc::PSVIUni_fgUnique;
*fgUnknown = *XML::Xercesc::PSVIUni_fgUnknown;
*fgValid = *XML::Xercesc::PSVIUni_fgValid;
*fgVCFixed = *XML::Xercesc::PSVIUni_fgVCFixed;
*fgXMLChNull = *XML::Xercesc::PSVIUni_fgXMLChNull;
*fgAg = *XML::Xercesc::PSVIUni_fgAg;
*fgAnnot = *XML::Xercesc::PSVIUni_fgAnnot;
*fgAttr = *XML::Xercesc::PSVIUni_fgAttr;
*fgAu = *XML::Xercesc::PSVIUni_fgAu;
*fgElt = *XML::Xercesc::PSVIUni_fgElt;
*fgIdc = *XML::Xercesc::PSVIUni_fgIdc;
*fgMg = *XML::Xercesc::PSVIUni_fgMg;
*fgNot = *XML::Xercesc::PSVIUni_fgNot;
*fgType = *XML::Xercesc::PSVIUni_fgType;
*fgBounded = *XML::Xercesc::PSVIUni_fgBounded;
*fgCardinality = *XML::Xercesc::PSVIUni_fgCardinality;
*fgEnumeration = *XML::Xercesc::PSVIUni_fgEnumeration;
*fgFractionDigits = *XML::Xercesc::PSVIUni_fgFractionDigits;
*fgLength = *XML::Xercesc::PSVIUni_fgLength;
*fgMaxExclusive = *XML::Xercesc::PSVIUni_fgMaxExclusive;
*fgMaxInclusive = *XML::Xercesc::PSVIUni_fgMaxInclusive;
*fgMaxLength = *XML::Xercesc::PSVIUni_fgMaxLength;
*fgMinExclusive = *XML::Xercesc::PSVIUni_fgMinExclusive;
*fgMinInclusive = *XML::Xercesc::PSVIUni_fgMinInclusive;
*fgMinLength = *XML::Xercesc::PSVIUni_fgMinLength;
*fgNumeric = *XML::Xercesc::PSVIUni_fgNumeric;
*fgOrdered = *XML::Xercesc::PSVIUni_fgOrdered;
*fgPattern = *XML::Xercesc::PSVIUni_fgPattern;
*fgTotalDigits = *XML::Xercesc::PSVIUni_fgTotalDigits;
*fgWhiteSpace = *XML::Xercesc::PSVIUni_fgWhiteSpace;
*fgNamespaceInfoset = *XML::Xercesc::PSVIUni_fgNamespaceInfoset;
*fgXsi = *XML::Xercesc::PSVIUni_fgXsi;
*fgNamespaceInstance = *XML::Xercesc::PSVIUni_fgNamespaceInstance;
*fgPsv = *XML::Xercesc::PSVIUni_fgPsv;
*fgNamespacePsvi = *XML::Xercesc::PSVIUni_fgNamespacePsvi;
*fgXml = *XML::Xercesc::PSVIUni_fgXml;
*fgNamespaceXmlSchema = *XML::Xercesc::PSVIUni_fgNamespaceXmlSchema;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_PSVIUni(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PSVIUni($self);
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
*Initialize = *XML::Xercesc::XMLPlatformUtils_Initialize;
*Terminate = *XML::Xercesc::XMLPlatformUtils_Terminate;
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
*HTTPS = *XML::Xercesc::XMLURL_HTTPS;
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


############# Class : XML::Xerces::SecurityManager ##############

package XML::Xerces::SecurityManager;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*ENTITY_EXPANSION_LIMIT = *XML::Xercesc::SecurityManager_ENTITY_EXPANSION_LIMIT;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_SecurityManager(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_SecurityManager($self);
        delete $OWNER{$self};
    }
}

*setEntityExpansionLimit = *XML::Xercesc::SecurityManager_setEntityExpansionLimit;
*getEntityExpansionLimit = *XML::Xercesc::SecurityManager_getEntityExpansionLimit;
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

sub getAttDefList {
    my $result = XML::Xercesc::XMLElementDecl_getAttDefList (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
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

*requiresNamespaces = *XML::Xercesc::XMLValidator_requiresNamespaces;
*getGrammar = *XML::Xercesc::XMLValidator_getGrammar;
*setGrammar = *XML::Xercesc::XMLValidator_setGrammar;
*handlesDTD = *XML::Xercesc::XMLValidator_handlesDTD;
*handlesSchema = *XML::Xercesc::XMLValidator_handlesSchema;
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
*setGrammarDescription = *XML::Xercesc::Grammar_setGrammarDescription;
*getGrammarDescription = *XML::Xercesc::Grammar_getGrammarDescription;
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


############# Class : XML::Xerces::BaseXMLChVector ##############

package XML::Xerces::BaseXMLChVector;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_BaseXMLChVector(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_BaseXMLChVector($self);
        delete $OWNER{$self};
    }
}

*addElement = *XML::Xercesc::BaseXMLChVector_addElement;
*setElementAt = *XML::Xercesc::BaseXMLChVector_setElementAt;
*insertElementAt = *XML::Xercesc::BaseXMLChVector_insertElementAt;
*orphanElementAt = *XML::Xercesc::BaseXMLChVector_orphanElementAt;
*removeAllElements = *XML::Xercesc::BaseXMLChVector_removeAllElements;
*removeElementAt = *XML::Xercesc::BaseXMLChVector_removeElementAt;
*removeLastElement = *XML::Xercesc::BaseXMLChVector_removeLastElement;
*containsElement = *XML::Xercesc::BaseXMLChVector_containsElement;
*cleanup = *XML::Xercesc::BaseXMLChVector_cleanup;
*reinitialize = *XML::Xercesc::BaseXMLChVector_reinitialize;
*curCapacity = *XML::Xercesc::BaseXMLChVector_curCapacity;
*elementAt = *XML::Xercesc::BaseXMLChVector_elementAt;
*size = *XML::Xercesc::BaseXMLChVector_size;
*getMemoryManager = *XML::Xercesc::BaseXMLChVector_getMemoryManager;
*ensureExtraCapacity = *XML::Xercesc::BaseXMLChVector_ensureExtraCapacity;
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


############# Class : XML::Xerces::XMLChVector ##############

package XML::Xerces::XMLChVector;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::BaseXMLChVector XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_XMLChVector(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLChVector($self);
        delete $OWNER{$self};
    }
}

*setElementAt = *XML::Xercesc::XMLChVector_setElementAt;
*removeAllElements = *XML::Xercesc::XMLChVector_removeAllElements;
*removeElementAt = *XML::Xercesc::XMLChVector_removeElementAt;
*removeLastElement = *XML::Xercesc::XMLChVector_removeLastElement;
*cleanup = *XML::Xercesc::XMLChVector_cleanup;
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
sub getLocationHints {
    my $result = XML::Xercesc::XMLSchemaDescription_getLocationHints (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
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

sub getAttDefList {
    my $result = XML::Xercesc::XMLElementDecl_getAttDefList (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
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
*getValidated = *XML::Xercesc::DTDGrammar_getValidated;
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

*requiresNamespaces = *XML::Xercesc::DTDValidator_requiresNamespaces;
*getGrammar = *XML::Xercesc::DTDValidator_getGrammar;
*setGrammar = *XML::Xercesc::DTDValidator_setGrammar;
*handlesDTD = *XML::Xercesc::DTDValidator_handlesDTD;
*handlesSchema = *XML::Xercesc::DTDValidator_handlesSchema;
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
*ElementOnlyEmpty = *XML::Xercesc::SchemaElementDecl_ElementOnlyEmpty;
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

sub getAttDefList {
    my $result = XML::Xercesc::XMLElementDecl_getAttDefList (@_);
    unless (defined$result) {
      return () if wantarray;
      return undef;#if *not* wantarray
    }
    return $result->to_list() if wantarray;
    return $result;#if *not* wantarray
}
  
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
*getValidated = *XML::Xercesc::SchemaGrammar_getValidated;
*getElemEnumerator = *XML::Xercesc::SchemaGrammar_getElemEnumerator;
*getNotationEnumerator = *XML::Xercesc::SchemaGrammar_getNotationEnumerator;
*getAttributeDeclRegistry = *XML::Xercesc::SchemaGrammar_getAttributeDeclRegistry;
*getComplexTypeRegistry = *XML::Xercesc::SchemaGrammar_getComplexTypeRegistry;
*getGroupInfoRegistry = *XML::Xercesc::SchemaGrammar_getGroupInfoRegistry;
*getAttGroupInfoRegistry = *XML::Xercesc::SchemaGrammar_getAttGroupInfoRegistry;
*getDatatypeRegistry = *XML::Xercesc::SchemaGrammar_getDatatypeRegistry;
*getValidSubstitutionGroups = *XML::Xercesc::SchemaGrammar_getValidSubstitutionGroups;
*getValidationContext = *XML::Xercesc::SchemaGrammar_getValidationContext;
*setTargetNamespace = *XML::Xercesc::SchemaGrammar_setTargetNamespace;
*setAttributeDeclRegistry = *XML::Xercesc::SchemaGrammar_setAttributeDeclRegistry;
*setComplexTypeRegistry = *XML::Xercesc::SchemaGrammar_setComplexTypeRegistry;
*setGroupInfoRegistry = *XML::Xercesc::SchemaGrammar_setGroupInfoRegistry;
*setAttGroupInfoRegistry = *XML::Xercesc::SchemaGrammar_setAttGroupInfoRegistry;
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

*requiresNamespaces = *XML::Xercesc::SchemaValidator_requiresNamespaces;
*getGrammar = *XML::Xercesc::SchemaValidator_getGrammar;
*setGrammar = *XML::Xercesc::SchemaValidator_setGrammar;
*handlesDTD = *XML::Xercesc::SchemaValidator_handlesDTD;
*handlesSchema = *XML::Xercesc::SchemaValidator_handlesSchema;
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
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_SAXParseException(@_);
    bless $self, $pkg if defined($self);
}

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


############# Class : XML::Xerces::BaseXMLAttrVector ##############

package XML::Xerces::BaseXMLAttrVector;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_BaseXMLAttrVector(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_BaseXMLAttrVector($self);
        delete $OWNER{$self};
    }
}

*addElement = *XML::Xercesc::BaseXMLAttrVector_addElement;
*setElementAt = *XML::Xercesc::BaseXMLAttrVector_setElementAt;
*insertElementAt = *XML::Xercesc::BaseXMLAttrVector_insertElementAt;
*orphanElementAt = *XML::Xercesc::BaseXMLAttrVector_orphanElementAt;
*removeAllElements = *XML::Xercesc::BaseXMLAttrVector_removeAllElements;
*removeElementAt = *XML::Xercesc::BaseXMLAttrVector_removeElementAt;
*removeLastElement = *XML::Xercesc::BaseXMLAttrVector_removeLastElement;
*containsElement = *XML::Xercesc::BaseXMLAttrVector_containsElement;
*cleanup = *XML::Xercesc::BaseXMLAttrVector_cleanup;
*reinitialize = *XML::Xercesc::BaseXMLAttrVector_reinitialize;
*curCapacity = *XML::Xercesc::BaseXMLAttrVector_curCapacity;
*elementAt = *XML::Xercesc::BaseXMLAttrVector_elementAt;
*size = *XML::Xercesc::BaseXMLAttrVector_size;
*getMemoryManager = *XML::Xercesc::BaseXMLAttrVector_getMemoryManager;
*ensureExtraCapacity = *XML::Xercesc::BaseXMLAttrVector_ensureExtraCapacity;
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


############# Class : XML::Xerces::XMLAttrVector ##############

package XML::Xerces::XMLAttrVector;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::BaseXMLAttrVector XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_XMLAttrVector(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLAttrVector($self);
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


############# Class : XML::Xerces::XMLAttr ##############

package XML::Xerces::XMLAttr;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_XMLAttr(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLAttr($self);
        delete $OWNER{$self};
    }
}

*getAttName = *XML::Xercesc::XMLAttr_getAttName;
*getName = *XML::Xercesc::XMLAttr_getName;
*getQName = *XML::Xercesc::XMLAttr_getQName;
*getSpecified = *XML::Xercesc::XMLAttr_getSpecified;
*getType = *XML::Xercesc::XMLAttr_getType;
*getValue = *XML::Xercesc::XMLAttr_getValue;
*getURIId = *XML::Xercesc::XMLAttr_getURIId;
*set = *XML::Xercesc::XMLAttr_set;
*setName = *XML::Xercesc::XMLAttr_setName;
*setSpecified = *XML::Xercesc::XMLAttr_setSpecified;
*setType = *XML::Xercesc::XMLAttr_setType;
*setValue = *XML::Xercesc::XMLAttr_setValue;
*setURIId = *XML::Xercesc::XMLAttr_setURIId;
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


############# Class : XML::Xerces::XMLDocumentHandler ##############

package XML::Xerces::XMLDocumentHandler;
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
        XML::Xercesc::delete_XMLDocumentHandler($self);
        delete $OWNER{$self};
    }
}

*docCharacters = *XML::Xercesc::XMLDocumentHandler_docCharacters;
*docComment = *XML::Xercesc::XMLDocumentHandler_docComment;
*docPI = *XML::Xercesc::XMLDocumentHandler_docPI;
*endDocument = *XML::Xercesc::XMLDocumentHandler_endDocument;
*endElement = *XML::Xercesc::XMLDocumentHandler_endElement;
*endEntityReference = *XML::Xercesc::XMLDocumentHandler_endEntityReference;
*ignorableWhitespace = *XML::Xercesc::XMLDocumentHandler_ignorableWhitespace;
*resetDocument = *XML::Xercesc::XMLDocumentHandler_resetDocument;
*startDocument = *XML::Xercesc::XMLDocumentHandler_startDocument;
*startElement = *XML::Xercesc::XMLDocumentHandler_startElement;
*startEntityReference = *XML::Xercesc::XMLDocumentHandler_startEntityReference;
*XMLDecl = *XML::Xercesc::XMLDocumentHandler_XMLDecl;
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


############# Class : XML::Xerces::XMLResourceIdentifier ##############

package XML::Xerces::XMLResourceIdentifier;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*SchemaGrammar = *XML::Xercesc::XMLResourceIdentifier_SchemaGrammar;
*SchemaImport = *XML::Xercesc::XMLResourceIdentifier_SchemaImport;
*SchemaInclude = *XML::Xercesc::XMLResourceIdentifier_SchemaInclude;
*SchemaRedefine = *XML::Xercesc::XMLResourceIdentifier_SchemaRedefine;
*ExternalEntity = *XML::Xercesc::XMLResourceIdentifier_ExternalEntity;
*UnKnown = *XML::Xercesc::XMLResourceIdentifier_UnKnown;
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_XMLResourceIdentifier(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_XMLResourceIdentifier($self);
        delete $OWNER{$self};
    }
}

*getResourceIdentifierType = *XML::Xercesc::XMLResourceIdentifier_getResourceIdentifierType;
*getPublicId = *XML::Xercesc::XMLResourceIdentifier_getPublicId;
*getSystemId = *XML::Xercesc::XMLResourceIdentifier_getSystemId;
*getSchemaLocation = *XML::Xercesc::XMLResourceIdentifier_getSchemaLocation;
*getBaseURI = *XML::Xercesc::XMLResourceIdentifier_getBaseURI;
*getNameSpace = *XML::Xercesc::XMLResourceIdentifier_getNameSpace;
*getLocator = *XML::Xercesc::XMLResourceIdentifier_getLocator;
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


############# Class : XML::Xerces::XMLEntityResolver ##############

package XML::Xerces::XMLEntityResolver;
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
        XML::Xercesc::delete_XMLEntityResolver($self);
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


############# Class : XML::Xerces::PSVIItem ##############

package XML::Xerces::PSVIItem;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
*VALIDITY_NOTKNOWN = *XML::Xercesc::PSVIItem_VALIDITY_NOTKNOWN;
*VALIDITY_INVALID = *XML::Xercesc::PSVIItem_VALIDITY_INVALID;
*VALIDITY_VALID = *XML::Xercesc::PSVIItem_VALIDITY_VALID;
*VALIDATION_NONE = *XML::Xercesc::PSVIItem_VALIDATION_NONE;
*VALIDATION_PARTIAL = *XML::Xercesc::PSVIItem_VALIDATION_PARTIAL;
*VALIDATION_FULL = *XML::Xercesc::PSVIItem_VALIDATION_FULL;
sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PSVIItem($self);
        delete $OWNER{$self};
    }
}

*getValidationContext = *XML::Xercesc::PSVIItem_getValidationContext;
*getValidity = *XML::Xercesc::PSVIItem_getValidity;
*getValidationAttempted = *XML::Xercesc::PSVIItem_getValidationAttempted;
*getSchemaNormalizedValue = *XML::Xercesc::PSVIItem_getSchemaNormalizedValue;
*getTypeDefinition = *XML::Xercesc::PSVIItem_getTypeDefinition;
*getMemberTypeDefinition = *XML::Xercesc::PSVIItem_getMemberTypeDefinition;
*getSchemaDefault = *XML::Xercesc::PSVIItem_getSchemaDefault;
*getIsSchemaSpecified = *XML::Xercesc::PSVIItem_getIsSchemaSpecified;
*getCanonicalRepresentation = *XML::Xercesc::PSVIItem_getCanonicalRepresentation;
*getActualValue = *XML::Xercesc::PSVIItem_getActualValue;
*setValidationAttempted = *XML::Xercesc::PSVIItem_setValidationAttempted;
*setValidity = *XML::Xercesc::PSVIItem_setValidity;
*reset = *XML::Xercesc::PSVIItem_reset;
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


############# Class : XML::Xerces::PSVIElement ##############

package XML::Xerces::PSVIElement;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::PSVIItem XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_PSVIElement(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PSVIElement($self);
        delete $OWNER{$self};
    }
}

*getElementDeclaration = *XML::Xercesc::PSVIElement_getElementDeclaration;
*getNotationDeclaration = *XML::Xercesc::PSVIElement_getNotationDeclaration;
*getSchemaInformation = *XML::Xercesc::PSVIElement_getSchemaInformation;
*getTypeDefinition = *XML::Xercesc::PSVIElement_getTypeDefinition;
*getMemberTypeDefinition = *XML::Xercesc::PSVIElement_getMemberTypeDefinition;
*reset = *XML::Xercesc::PSVIElement_reset;
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


############# Class : XML::Xerces::PSVIAttribute ##############

package XML::Xerces::PSVIAttribute;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces::PSVIItem XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_PSVIAttribute(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PSVIAttribute($self);
        delete $OWNER{$self};
    }
}

*getAttributeDeclaration = *XML::Xercesc::PSVIAttribute_getAttributeDeclaration;
*getTypeDefinition = *XML::Xercesc::PSVIAttribute_getTypeDefinition;
*getMemberTypeDefinition = *XML::Xercesc::PSVIAttribute_getMemberTypeDefinition;
*reset = *XML::Xercesc::PSVIAttribute_reset;
*setValue = *XML::Xercesc::PSVIAttribute_setValue;
*updateValidity = *XML::Xercesc::PSVIAttribute_updateValidity;
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


############# Class : XML::Xerces::PSVIAttributeList ##############

package XML::Xerces::PSVIAttributeList;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( XML::Xerces );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = XML::Xercesc::new_PSVIAttributeList(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        XML::Xercesc::delete_PSVIAttributeList($self);
        delete $OWNER{$self};
    }
}

*getLength = *XML::Xercesc::PSVIAttributeList_getLength;
*getAttributePSVIAtIndex = *XML::Xercesc::PSVIAttributeList_getAttributePSVIAtIndex;
*getAttributeNameAtIndex = *XML::Xercesc::PSVIAttributeList_getAttributeNameAtIndex;
*getAttributeNamespaceAtIndex = *XML::Xercesc::PSVIAttributeList_getAttributeNamespaceAtIndex;
*getAttributePSVIByName = *XML::Xercesc::PSVIAttributeList_getAttributePSVIByName;
*getPSVIAttributeToFill = *XML::Xercesc::PSVIAttributeList_getPSVIAttributeToFill;
*reset = *XML::Xercesc::PSVIAttributeList_reset;
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
*warning = *XML::Xercesc::PerlErrorCallbackHandler_warning;
*error = *XML::Xercesc::PerlErrorCallbackHandler_error;
*fatalError = *XML::Xercesc::PerlErrorCallbackHandler_fatalError;
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
@ISA = qw( XML::Xerces::EntityResolver XML::Xerces::XMLEntityResolver XML::Xerces::PerlCallbackHandler XML::Xerces );
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


# ------- VARIABLE STUBS --------

package XML::Xerces;

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
*XMLUni_fgUCS4EncodingString4 = *XML::Xercesc::XMLUni_fgUCS4EncodingString4;
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
*XMLUni_fgXMLXIncludeException_Name = *XML::Xercesc::XMLUni_fgXMLXIncludeException_Name;
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
*XMLUni_fgXercesHandleMultipleImports = *XML::Xercesc::XMLUni_fgXercesHandleMultipleImports;
*XMLUni_fgXercesDoXInclude = *XML::Xercesc::XMLUni_fgXercesDoXInclude;
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
*XMLUni_fgXercescInterfaceDOMDocumentImpl = *XML::Xercesc::XMLUni_fgXercescInterfaceDOMDocumentImpl;
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
*SchemaSymbols_fgURI_XSI = *XML::Xercesc::SchemaSymbols_fgURI_XSI;
*SchemaSymbols_fgURI_SCHEMAFORSCHEMA = *XML::Xercesc::SchemaSymbols_fgURI_SCHEMAFORSCHEMA;
*SchemaSymbols_fgXSI_SCHEMALOCACTION = *XML::Xercesc::SchemaSymbols_fgXSI_SCHEMALOCACTION;
*SchemaSymbols_fgXSI_NONAMESPACESCHEMALOCACTION = *XML::Xercesc::SchemaSymbols_fgXSI_NONAMESPACESCHEMALOCACTION;
*SchemaSymbols_fgXSI_TYPE = *XML::Xercesc::SchemaSymbols_fgXSI_TYPE;
*SchemaSymbols_fgELT_ALL = *XML::Xercesc::SchemaSymbols_fgELT_ALL;
*SchemaSymbols_fgELT_ANNOTATION = *XML::Xercesc::SchemaSymbols_fgELT_ANNOTATION;
*SchemaSymbols_fgELT_ANY = *XML::Xercesc::SchemaSymbols_fgELT_ANY;
*SchemaSymbols_fgELT_WILDCARD = *XML::Xercesc::SchemaSymbols_fgELT_WILDCARD;
*SchemaSymbols_fgELT_ANYATTRIBUTE = *XML::Xercesc::SchemaSymbols_fgELT_ANYATTRIBUTE;
*SchemaSymbols_fgELT_APPINFO = *XML::Xercesc::SchemaSymbols_fgELT_APPINFO;
*SchemaSymbols_fgELT_ATTRIBUTE = *XML::Xercesc::SchemaSymbols_fgELT_ATTRIBUTE;
*SchemaSymbols_fgELT_ATTRIBUTEGROUP = *XML::Xercesc::SchemaSymbols_fgELT_ATTRIBUTEGROUP;
*SchemaSymbols_fgELT_CHOICE = *XML::Xercesc::SchemaSymbols_fgELT_CHOICE;
*SchemaSymbols_fgELT_COMPLEXTYPE = *XML::Xercesc::SchemaSymbols_fgELT_COMPLEXTYPE;
*SchemaSymbols_fgELT_CONTENT = *XML::Xercesc::SchemaSymbols_fgELT_CONTENT;
*SchemaSymbols_fgELT_DOCUMENTATION = *XML::Xercesc::SchemaSymbols_fgELT_DOCUMENTATION;
*SchemaSymbols_fgELT_DURATION = *XML::Xercesc::SchemaSymbols_fgELT_DURATION;
*SchemaSymbols_fgELT_ELEMENT = *XML::Xercesc::SchemaSymbols_fgELT_ELEMENT;
*SchemaSymbols_fgELT_ENCODING = *XML::Xercesc::SchemaSymbols_fgELT_ENCODING;
*SchemaSymbols_fgELT_ENUMERATION = *XML::Xercesc::SchemaSymbols_fgELT_ENUMERATION;
*SchemaSymbols_fgELT_FIELD = *XML::Xercesc::SchemaSymbols_fgELT_FIELD;
*SchemaSymbols_fgELT_WHITESPACE = *XML::Xercesc::SchemaSymbols_fgELT_WHITESPACE;
*SchemaSymbols_fgELT_GROUP = *XML::Xercesc::SchemaSymbols_fgELT_GROUP;
*SchemaSymbols_fgELT_IMPORT = *XML::Xercesc::SchemaSymbols_fgELT_IMPORT;
*SchemaSymbols_fgELT_INCLUDE = *XML::Xercesc::SchemaSymbols_fgELT_INCLUDE;
*SchemaSymbols_fgELT_REDEFINE = *XML::Xercesc::SchemaSymbols_fgELT_REDEFINE;
*SchemaSymbols_fgELT_KEY = *XML::Xercesc::SchemaSymbols_fgELT_KEY;
*SchemaSymbols_fgELT_KEYREF = *XML::Xercesc::SchemaSymbols_fgELT_KEYREF;
*SchemaSymbols_fgELT_LENGTH = *XML::Xercesc::SchemaSymbols_fgELT_LENGTH;
*SchemaSymbols_fgELT_MAXEXCLUSIVE = *XML::Xercesc::SchemaSymbols_fgELT_MAXEXCLUSIVE;
*SchemaSymbols_fgELT_MAXINCLUSIVE = *XML::Xercesc::SchemaSymbols_fgELT_MAXINCLUSIVE;
*SchemaSymbols_fgELT_MAXLENGTH = *XML::Xercesc::SchemaSymbols_fgELT_MAXLENGTH;
*SchemaSymbols_fgELT_MINEXCLUSIVE = *XML::Xercesc::SchemaSymbols_fgELT_MINEXCLUSIVE;
*SchemaSymbols_fgELT_MININCLUSIVE = *XML::Xercesc::SchemaSymbols_fgELT_MININCLUSIVE;
*SchemaSymbols_fgELT_MINLENGTH = *XML::Xercesc::SchemaSymbols_fgELT_MINLENGTH;
*SchemaSymbols_fgELT_NOTATION = *XML::Xercesc::SchemaSymbols_fgELT_NOTATION;
*SchemaSymbols_fgELT_PATTERN = *XML::Xercesc::SchemaSymbols_fgELT_PATTERN;
*SchemaSymbols_fgELT_PERIOD = *XML::Xercesc::SchemaSymbols_fgELT_PERIOD;
*SchemaSymbols_fgELT_TOTALDIGITS = *XML::Xercesc::SchemaSymbols_fgELT_TOTALDIGITS;
*SchemaSymbols_fgELT_FRACTIONDIGITS = *XML::Xercesc::SchemaSymbols_fgELT_FRACTIONDIGITS;
*SchemaSymbols_fgELT_SCHEMA = *XML::Xercesc::SchemaSymbols_fgELT_SCHEMA;
*SchemaSymbols_fgELT_SELECTOR = *XML::Xercesc::SchemaSymbols_fgELT_SELECTOR;
*SchemaSymbols_fgELT_SEQUENCE = *XML::Xercesc::SchemaSymbols_fgELT_SEQUENCE;
*SchemaSymbols_fgELT_SIMPLETYPE = *XML::Xercesc::SchemaSymbols_fgELT_SIMPLETYPE;
*SchemaSymbols_fgELT_UNION = *XML::Xercesc::SchemaSymbols_fgELT_UNION;
*SchemaSymbols_fgELT_LIST = *XML::Xercesc::SchemaSymbols_fgELT_LIST;
*SchemaSymbols_fgELT_UNIQUE = *XML::Xercesc::SchemaSymbols_fgELT_UNIQUE;
*SchemaSymbols_fgELT_COMPLEXCONTENT = *XML::Xercesc::SchemaSymbols_fgELT_COMPLEXCONTENT;
*SchemaSymbols_fgELT_SIMPLECONTENT = *XML::Xercesc::SchemaSymbols_fgELT_SIMPLECONTENT;
*SchemaSymbols_fgELT_RESTRICTION = *XML::Xercesc::SchemaSymbols_fgELT_RESTRICTION;
*SchemaSymbols_fgELT_EXTENSION = *XML::Xercesc::SchemaSymbols_fgELT_EXTENSION;
*SchemaSymbols_fgATT_ABSTRACT = *XML::Xercesc::SchemaSymbols_fgATT_ABSTRACT;
*SchemaSymbols_fgATT_ATTRIBUTEFORMDEFAULT = *XML::Xercesc::SchemaSymbols_fgATT_ATTRIBUTEFORMDEFAULT;
*SchemaSymbols_fgATT_BASE = *XML::Xercesc::SchemaSymbols_fgATT_BASE;
*SchemaSymbols_fgATT_ITEMTYPE = *XML::Xercesc::SchemaSymbols_fgATT_ITEMTYPE;
*SchemaSymbols_fgATT_MEMBERTYPES = *XML::Xercesc::SchemaSymbols_fgATT_MEMBERTYPES;
*SchemaSymbols_fgATT_BLOCK = *XML::Xercesc::SchemaSymbols_fgATT_BLOCK;
*SchemaSymbols_fgATT_BLOCKDEFAULT = *XML::Xercesc::SchemaSymbols_fgATT_BLOCKDEFAULT;
*SchemaSymbols_fgATT_DEFAULT = *XML::Xercesc::SchemaSymbols_fgATT_DEFAULT;
*SchemaSymbols_fgATT_ELEMENTFORMDEFAULT = *XML::Xercesc::SchemaSymbols_fgATT_ELEMENTFORMDEFAULT;
*SchemaSymbols_fgATT_SUBSTITUTIONGROUP = *XML::Xercesc::SchemaSymbols_fgATT_SUBSTITUTIONGROUP;
*SchemaSymbols_fgATT_FINAL = *XML::Xercesc::SchemaSymbols_fgATT_FINAL;
*SchemaSymbols_fgATT_FINALDEFAULT = *XML::Xercesc::SchemaSymbols_fgATT_FINALDEFAULT;
*SchemaSymbols_fgATT_FIXED = *XML::Xercesc::SchemaSymbols_fgATT_FIXED;
*SchemaSymbols_fgATT_FORM = *XML::Xercesc::SchemaSymbols_fgATT_FORM;
*SchemaSymbols_fgATT_ID = *XML::Xercesc::SchemaSymbols_fgATT_ID;
*SchemaSymbols_fgATT_MAXOCCURS = *XML::Xercesc::SchemaSymbols_fgATT_MAXOCCURS;
*SchemaSymbols_fgATT_MINOCCURS = *XML::Xercesc::SchemaSymbols_fgATT_MINOCCURS;
*SchemaSymbols_fgATT_NAME = *XML::Xercesc::SchemaSymbols_fgATT_NAME;
*SchemaSymbols_fgATT_NAMESPACE = *XML::Xercesc::SchemaSymbols_fgATT_NAMESPACE;
*SchemaSymbols_fgATT_NILL = *XML::Xercesc::SchemaSymbols_fgATT_NILL;
*SchemaSymbols_fgATT_NILLABLE = *XML::Xercesc::SchemaSymbols_fgATT_NILLABLE;
*SchemaSymbols_fgATT_PROCESSCONTENTS = *XML::Xercesc::SchemaSymbols_fgATT_PROCESSCONTENTS;
*SchemaSymbols_fgATT_REF = *XML::Xercesc::SchemaSymbols_fgATT_REF;
*SchemaSymbols_fgATT_REFER = *XML::Xercesc::SchemaSymbols_fgATT_REFER;
*SchemaSymbols_fgATT_SCHEMALOCATION = *XML::Xercesc::SchemaSymbols_fgATT_SCHEMALOCATION;
*SchemaSymbols_fgATT_SOURCE = *XML::Xercesc::SchemaSymbols_fgATT_SOURCE;
*SchemaSymbols_fgATT_SYSTEM = *XML::Xercesc::SchemaSymbols_fgATT_SYSTEM;
*SchemaSymbols_fgATT_PUBLIC = *XML::Xercesc::SchemaSymbols_fgATT_PUBLIC;
*SchemaSymbols_fgATT_TARGETNAMESPACE = *XML::Xercesc::SchemaSymbols_fgATT_TARGETNAMESPACE;
*SchemaSymbols_fgATT_TYPE = *XML::Xercesc::SchemaSymbols_fgATT_TYPE;
*SchemaSymbols_fgATT_USE = *XML::Xercesc::SchemaSymbols_fgATT_USE;
*SchemaSymbols_fgATT_VALUE = *XML::Xercesc::SchemaSymbols_fgATT_VALUE;
*SchemaSymbols_fgATT_MIXED = *XML::Xercesc::SchemaSymbols_fgATT_MIXED;
*SchemaSymbols_fgATT_VERSION = *XML::Xercesc::SchemaSymbols_fgATT_VERSION;
*SchemaSymbols_fgATT_XPATH = *XML::Xercesc::SchemaSymbols_fgATT_XPATH;
*SchemaSymbols_fgATTVAL_TWOPOUNDANY = *XML::Xercesc::SchemaSymbols_fgATTVAL_TWOPOUNDANY;
*SchemaSymbols_fgATTVAL_TWOPOUNDLOCAL = *XML::Xercesc::SchemaSymbols_fgATTVAL_TWOPOUNDLOCAL;
*SchemaSymbols_fgATTVAL_TWOPOUNDOTHER = *XML::Xercesc::SchemaSymbols_fgATTVAL_TWOPOUNDOTHER;
*SchemaSymbols_fgATTVAL_TWOPOUNDTRAGETNAMESPACE = *XML::Xercesc::SchemaSymbols_fgATTVAL_TWOPOUNDTRAGETNAMESPACE;
*SchemaSymbols_fgATTVAL_POUNDALL = *XML::Xercesc::SchemaSymbols_fgATTVAL_POUNDALL;
*SchemaSymbols_fgATTVAL_BASE64 = *XML::Xercesc::SchemaSymbols_fgATTVAL_BASE64;
*SchemaSymbols_fgATTVAL_BOOLEAN = *XML::Xercesc::SchemaSymbols_fgATTVAL_BOOLEAN;
*SchemaSymbols_fgATTVAL_DEFAULT = *XML::Xercesc::SchemaSymbols_fgATTVAL_DEFAULT;
*SchemaSymbols_fgATTVAL_ELEMENTONLY = *XML::Xercesc::SchemaSymbols_fgATTVAL_ELEMENTONLY;
*SchemaSymbols_fgATTVAL_EMPTY = *XML::Xercesc::SchemaSymbols_fgATTVAL_EMPTY;
*SchemaSymbols_fgATTVAL_EXTENSION = *XML::Xercesc::SchemaSymbols_fgATTVAL_EXTENSION;
*SchemaSymbols_fgATTVAL_FALSE = *XML::Xercesc::SchemaSymbols_fgATTVAL_FALSE;
*SchemaSymbols_fgATTVAL_FIXED = *XML::Xercesc::SchemaSymbols_fgATTVAL_FIXED;
*SchemaSymbols_fgATTVAL_HEX = *XML::Xercesc::SchemaSymbols_fgATTVAL_HEX;
*SchemaSymbols_fgATTVAL_ID = *XML::Xercesc::SchemaSymbols_fgATTVAL_ID;
*SchemaSymbols_fgATTVAL_LAX = *XML::Xercesc::SchemaSymbols_fgATTVAL_LAX;
*SchemaSymbols_fgATTVAL_MAXLENGTH = *XML::Xercesc::SchemaSymbols_fgATTVAL_MAXLENGTH;
*SchemaSymbols_fgATTVAL_MINLENGTH = *XML::Xercesc::SchemaSymbols_fgATTVAL_MINLENGTH;
*SchemaSymbols_fgATTVAL_MIXED = *XML::Xercesc::SchemaSymbols_fgATTVAL_MIXED;
*SchemaSymbols_fgATTVAL_NCNAME = *XML::Xercesc::SchemaSymbols_fgATTVAL_NCNAME;
*SchemaSymbols_fgATTVAL_OPTIONAL = *XML::Xercesc::SchemaSymbols_fgATTVAL_OPTIONAL;
*SchemaSymbols_fgATTVAL_PROHIBITED = *XML::Xercesc::SchemaSymbols_fgATTVAL_PROHIBITED;
*SchemaSymbols_fgATTVAL_QNAME = *XML::Xercesc::SchemaSymbols_fgATTVAL_QNAME;
*SchemaSymbols_fgATTVAL_QUALIFIED = *XML::Xercesc::SchemaSymbols_fgATTVAL_QUALIFIED;
*SchemaSymbols_fgATTVAL_REQUIRED = *XML::Xercesc::SchemaSymbols_fgATTVAL_REQUIRED;
*SchemaSymbols_fgATTVAL_RESTRICTION = *XML::Xercesc::SchemaSymbols_fgATTVAL_RESTRICTION;
*SchemaSymbols_fgATTVAL_SKIP = *XML::Xercesc::SchemaSymbols_fgATTVAL_SKIP;
*SchemaSymbols_fgATTVAL_STRICT = *XML::Xercesc::SchemaSymbols_fgATTVAL_STRICT;
*SchemaSymbols_fgATTVAL_STRING = *XML::Xercesc::SchemaSymbols_fgATTVAL_STRING;
*SchemaSymbols_fgATTVAL_TEXTONLY = *XML::Xercesc::SchemaSymbols_fgATTVAL_TEXTONLY;
*SchemaSymbols_fgATTVAL_TIMEDURATION = *XML::Xercesc::SchemaSymbols_fgATTVAL_TIMEDURATION;
*SchemaSymbols_fgATTVAL_TRUE = *XML::Xercesc::SchemaSymbols_fgATTVAL_TRUE;
*SchemaSymbols_fgATTVAL_UNQUALIFIED = *XML::Xercesc::SchemaSymbols_fgATTVAL_UNQUALIFIED;
*SchemaSymbols_fgATTVAL_URI = *XML::Xercesc::SchemaSymbols_fgATTVAL_URI;
*SchemaSymbols_fgATTVAL_URIREFERENCE = *XML::Xercesc::SchemaSymbols_fgATTVAL_URIREFERENCE;
*SchemaSymbols_fgATTVAL_SUBSTITUTIONGROUP = *XML::Xercesc::SchemaSymbols_fgATTVAL_SUBSTITUTIONGROUP;
*SchemaSymbols_fgATTVAL_SUBSTITUTION = *XML::Xercesc::SchemaSymbols_fgATTVAL_SUBSTITUTION;
*SchemaSymbols_fgATTVAL_ANYTYPE = *XML::Xercesc::SchemaSymbols_fgATTVAL_ANYTYPE;
*SchemaSymbols_fgWS_PRESERVE = *XML::Xercesc::SchemaSymbols_fgWS_PRESERVE;
*SchemaSymbols_fgWS_COLLAPSE = *XML::Xercesc::SchemaSymbols_fgWS_COLLAPSE;
*SchemaSymbols_fgWS_REPLACE = *XML::Xercesc::SchemaSymbols_fgWS_REPLACE;
*SchemaSymbols_fgDT_STRING = *XML::Xercesc::SchemaSymbols_fgDT_STRING;
*SchemaSymbols_fgDT_TOKEN = *XML::Xercesc::SchemaSymbols_fgDT_TOKEN;
*SchemaSymbols_fgDT_LANGUAGE = *XML::Xercesc::SchemaSymbols_fgDT_LANGUAGE;
*SchemaSymbols_fgDT_NAME = *XML::Xercesc::SchemaSymbols_fgDT_NAME;
*SchemaSymbols_fgDT_NCNAME = *XML::Xercesc::SchemaSymbols_fgDT_NCNAME;
*SchemaSymbols_fgDT_INTEGER = *XML::Xercesc::SchemaSymbols_fgDT_INTEGER;
*SchemaSymbols_fgDT_DECIMAL = *XML::Xercesc::SchemaSymbols_fgDT_DECIMAL;
*SchemaSymbols_fgDT_BOOLEAN = *XML::Xercesc::SchemaSymbols_fgDT_BOOLEAN;
*SchemaSymbols_fgDT_NONPOSITIVEINTEGER = *XML::Xercesc::SchemaSymbols_fgDT_NONPOSITIVEINTEGER;
*SchemaSymbols_fgDT_NEGATIVEINTEGER = *XML::Xercesc::SchemaSymbols_fgDT_NEGATIVEINTEGER;
*SchemaSymbols_fgDT_LONG = *XML::Xercesc::SchemaSymbols_fgDT_LONG;
*SchemaSymbols_fgDT_INT = *XML::Xercesc::SchemaSymbols_fgDT_INT;
*SchemaSymbols_fgDT_SHORT = *XML::Xercesc::SchemaSymbols_fgDT_SHORT;
*SchemaSymbols_fgDT_BYTE = *XML::Xercesc::SchemaSymbols_fgDT_BYTE;
*SchemaSymbols_fgDT_NONNEGATIVEINTEGER = *XML::Xercesc::SchemaSymbols_fgDT_NONNEGATIVEINTEGER;
*SchemaSymbols_fgDT_ULONG = *XML::Xercesc::SchemaSymbols_fgDT_ULONG;
*SchemaSymbols_fgDT_UINT = *XML::Xercesc::SchemaSymbols_fgDT_UINT;
*SchemaSymbols_fgDT_USHORT = *XML::Xercesc::SchemaSymbols_fgDT_USHORT;
*SchemaSymbols_fgDT_UBYTE = *XML::Xercesc::SchemaSymbols_fgDT_UBYTE;
*SchemaSymbols_fgDT_POSITIVEINTEGER = *XML::Xercesc::SchemaSymbols_fgDT_POSITIVEINTEGER;
*SchemaSymbols_fgDT_DATETIME = *XML::Xercesc::SchemaSymbols_fgDT_DATETIME;
*SchemaSymbols_fgDT_DATE = *XML::Xercesc::SchemaSymbols_fgDT_DATE;
*SchemaSymbols_fgDT_TIME = *XML::Xercesc::SchemaSymbols_fgDT_TIME;
*SchemaSymbols_fgDT_DURATION = *XML::Xercesc::SchemaSymbols_fgDT_DURATION;
*SchemaSymbols_fgDT_DAY = *XML::Xercesc::SchemaSymbols_fgDT_DAY;
*SchemaSymbols_fgDT_MONTH = *XML::Xercesc::SchemaSymbols_fgDT_MONTH;
*SchemaSymbols_fgDT_MONTHDAY = *XML::Xercesc::SchemaSymbols_fgDT_MONTHDAY;
*SchemaSymbols_fgDT_YEAR = *XML::Xercesc::SchemaSymbols_fgDT_YEAR;
*SchemaSymbols_fgDT_YEARMONTH = *XML::Xercesc::SchemaSymbols_fgDT_YEARMONTH;
*SchemaSymbols_fgDT_BASE64BINARY = *XML::Xercesc::SchemaSymbols_fgDT_BASE64BINARY;
*SchemaSymbols_fgDT_HEXBINARY = *XML::Xercesc::SchemaSymbols_fgDT_HEXBINARY;
*SchemaSymbols_fgDT_FLOAT = *XML::Xercesc::SchemaSymbols_fgDT_FLOAT;
*SchemaSymbols_fgDT_DOUBLE = *XML::Xercesc::SchemaSymbols_fgDT_DOUBLE;
*SchemaSymbols_fgDT_URIREFERENCE = *XML::Xercesc::SchemaSymbols_fgDT_URIREFERENCE;
*SchemaSymbols_fgDT_ANYURI = *XML::Xercesc::SchemaSymbols_fgDT_ANYURI;
*SchemaSymbols_fgDT_QNAME = *XML::Xercesc::SchemaSymbols_fgDT_QNAME;
*SchemaSymbols_fgDT_NORMALIZEDSTRING = *XML::Xercesc::SchemaSymbols_fgDT_NORMALIZEDSTRING;
*SchemaSymbols_fgDT_ANYSIMPLETYPE = *XML::Xercesc::SchemaSymbols_fgDT_ANYSIMPLETYPE;
*SchemaSymbols_fgRegEx_XOption = *XML::Xercesc::SchemaSymbols_fgRegEx_XOption;
*SchemaSymbols_fgRedefIdentifier = *XML::Xercesc::SchemaSymbols_fgRedefIdentifier;
*SchemaSymbols_fgINT_MIN_VALUE = *XML::Xercesc::SchemaSymbols_fgINT_MIN_VALUE;
*SchemaSymbols_fgINT_MAX_VALUE = *XML::Xercesc::SchemaSymbols_fgINT_MAX_VALUE;
*PSVIUni_fgPsvColon = *XML::Xercesc::PSVIUni_fgPsvColon;
*PSVIUni_fgAllDeclarationsProcessed = *XML::Xercesc::PSVIUni_fgAllDeclarationsProcessed;
*PSVIUni_fgAttribute = *XML::Xercesc::PSVIUni_fgAttribute;
*PSVIUni_fgAttributes = *XML::Xercesc::PSVIUni_fgAttributes;
*PSVIUni_fgAttributeType = *XML::Xercesc::PSVIUni_fgAttributeType;
*PSVIUni_fgBaseURI = *XML::Xercesc::PSVIUni_fgBaseURI;
*PSVIUni_fgCharacter = *XML::Xercesc::PSVIUni_fgCharacter;
*PSVIUni_fgCharacterEncodingScheme = *XML::Xercesc::PSVIUni_fgCharacterEncodingScheme;
*PSVIUni_fgChildren = *XML::Xercesc::PSVIUni_fgChildren;
*PSVIUni_fgComment = *XML::Xercesc::PSVIUni_fgComment;
*PSVIUni_fgContent = *XML::Xercesc::PSVIUni_fgContent;
*PSVIUni_fgDocument = *XML::Xercesc::PSVIUni_fgDocument;
*PSVIUni_fgDocTypeDeclaration = *XML::Xercesc::PSVIUni_fgDocTypeDeclaration;
*PSVIUni_fgDocumentElement = *XML::Xercesc::PSVIUni_fgDocumentElement;
*PSVIUni_fgElement = *XML::Xercesc::PSVIUni_fgElement;
*PSVIUni_fgInScopeNamespaces = *XML::Xercesc::PSVIUni_fgInScopeNamespaces;
*PSVIUni_fgLocalName = *XML::Xercesc::PSVIUni_fgLocalName;
*PSVIUni_fgNamespace = *XML::Xercesc::PSVIUni_fgNamespace;
*PSVIUni_fgNamespaceAttributes = *XML::Xercesc::PSVIUni_fgNamespaceAttributes;
*PSVIUni_fgNamespaceName = *XML::Xercesc::PSVIUni_fgNamespaceName;
*PSVIUni_fgNormalizedValue = *XML::Xercesc::PSVIUni_fgNormalizedValue;
*PSVIUni_fgNotations = *XML::Xercesc::PSVIUni_fgNotations;
*PSVIUni_fgPrefix = *XML::Xercesc::PSVIUni_fgPrefix;
*PSVIUni_fgProcessingInstruction = *XML::Xercesc::PSVIUni_fgProcessingInstruction;
*PSVIUni_fgReferences = *XML::Xercesc::PSVIUni_fgReferences;
*PSVIUni_fgSpecified = *XML::Xercesc::PSVIUni_fgSpecified;
*PSVIUni_fgStandalone = *XML::Xercesc::PSVIUni_fgStandalone;
*PSVIUni_fgTarget = *XML::Xercesc::PSVIUni_fgTarget;
*PSVIUni_fgText = *XML::Xercesc::PSVIUni_fgText;
*PSVIUni_fgTextContent = *XML::Xercesc::PSVIUni_fgTextContent;
*PSVIUni_fgUnparsedEntities = *XML::Xercesc::PSVIUni_fgUnparsedEntities;
*PSVIUni_fgVersion = *XML::Xercesc::PSVIUni_fgVersion;
*PSVIUni_fgAbstract = *XML::Xercesc::PSVIUni_fgAbstract;
*PSVIUni_fgAnnotation = *XML::Xercesc::PSVIUni_fgAnnotation;
*PSVIUni_fgAnnotations = *XML::Xercesc::PSVIUni_fgAnnotations;
*PSVIUni_fgApplicationInformation = *XML::Xercesc::PSVIUni_fgApplicationInformation;
*PSVIUni_fgAttributeDeclaration = *XML::Xercesc::PSVIUni_fgAttributeDeclaration;
*PSVIUni_fgAttributeGroupDefinition = *XML::Xercesc::PSVIUni_fgAttributeGroupDefinition;
*PSVIUni_fgAttributeUse = *XML::Xercesc::PSVIUni_fgAttributeUse;
*PSVIUni_fgAttributeUses = *XML::Xercesc::PSVIUni_fgAttributeUses;
*PSVIUni_fgAttributeWildcard = *XML::Xercesc::PSVIUni_fgAttributeWildcard;
*PSVIUni_fgBaseTypeDefinition = *XML::Xercesc::PSVIUni_fgBaseTypeDefinition;
*PSVIUni_fgCanonicalRepresentation = *XML::Xercesc::PSVIUni_fgCanonicalRepresentation;
*PSVIUni_fgComplexTypeDefinition = *XML::Xercesc::PSVIUni_fgComplexTypeDefinition;
*PSVIUni_fgCompositor = *XML::Xercesc::PSVIUni_fgCompositor;
*PSVIUni_fgContentType = *XML::Xercesc::PSVIUni_fgContentType;
*PSVIUni_fgDeclaration = *XML::Xercesc::PSVIUni_fgDeclaration;
*PSVIUni_fgDerivationMethod = *XML::Xercesc::PSVIUni_fgDerivationMethod;
*PSVIUni_fgDisallowedSubstitutions = *XML::Xercesc::PSVIUni_fgDisallowedSubstitutions;
*PSVIUni_fgPsvDocument = *XML::Xercesc::PSVIUni_fgPsvDocument;
*PSVIUni_fgDocumentLocation = *XML::Xercesc::PSVIUni_fgDocumentLocation;
*PSVIUni_fgElementDeclaration = *XML::Xercesc::PSVIUni_fgElementDeclaration;
*PSVIUni_fgFacets = *XML::Xercesc::PSVIUni_fgFacets;
*PSVIUni_fgFacetFixed = *XML::Xercesc::PSVIUni_fgFacetFixed;
*PSVIUni_fgFields = *XML::Xercesc::PSVIUni_fgFields;
*PSVIUni_fgFinal = *XML::Xercesc::PSVIUni_fgFinal;
*PSVIUni_fgFundamentalFacets = *XML::Xercesc::PSVIUni_fgFundamentalFacets;
*PSVIUni_fgIdentityConstraintCategory = *XML::Xercesc::PSVIUni_fgIdentityConstraintCategory;
*PSVIUni_fgIdentityConstraintDefinition = *XML::Xercesc::PSVIUni_fgIdentityConstraintDefinition;
*PSVIUni_fgIdentityConstraintDefinitions = *XML::Xercesc::PSVIUni_fgIdentityConstraintDefinitions;
*PSVIUni_fgIdentityConstraintTable = *XML::Xercesc::PSVIUni_fgIdentityConstraintTable;
*PSVIUni_fgIdIdrefTable = *XML::Xercesc::PSVIUni_fgIdIdrefTable;
*PSVIUni_fgItemTypeDefinition = *XML::Xercesc::PSVIUni_fgItemTypeDefinition;
*PSVIUni_fgMaxOccurs = *XML::Xercesc::PSVIUni_fgMaxOccurs;
*PSVIUni_fgMemberTypeDefinition = *XML::Xercesc::PSVIUni_fgMemberTypeDefinition;
*PSVIUni_fgMemberTypeDefinitions = *XML::Xercesc::PSVIUni_fgMemberTypeDefinitions;
*PSVIUni_fgMinOccurs = *XML::Xercesc::PSVIUni_fgMinOccurs;
*PSVIUni_fgModelGroup = *XML::Xercesc::PSVIUni_fgModelGroup;
*PSVIUni_fgModelGroupDefinition = *XML::Xercesc::PSVIUni_fgModelGroupDefinition;
*PSVIUni_fgName = *XML::Xercesc::PSVIUni_fgName;
*PSVIUni_fgNamespaceConstraint = *XML::Xercesc::PSVIUni_fgNamespaceConstraint;
*PSVIUni_fgNamespaces = *XML::Xercesc::PSVIUni_fgNamespaces;
*PSVIUni_fgNamespaceSchemaInformation = *XML::Xercesc::PSVIUni_fgNamespaceSchemaInformation;
*PSVIUni_fgNil = *XML::Xercesc::PSVIUni_fgNil;
*PSVIUni_fgNillable = *XML::Xercesc::PSVIUni_fgNillable;
*PSVIUni_fgNotation = *XML::Xercesc::PSVIUni_fgNotation;
*PSVIUni_fgNotationDeclaration = *XML::Xercesc::PSVIUni_fgNotationDeclaration;
*PSVIUni_fgParticle = *XML::Xercesc::PSVIUni_fgParticle;
*PSVIUni_fgParticles = *XML::Xercesc::PSVIUni_fgParticles;
*PSVIUni_fgPrimitiveTypeDefinition = *XML::Xercesc::PSVIUni_fgPrimitiveTypeDefinition;
*PSVIUni_fgProcessContents = *XML::Xercesc::PSVIUni_fgProcessContents;
*PSVIUni_fgProhibitedSubstitutions = *XML::Xercesc::PSVIUni_fgProhibitedSubstitutions;
*PSVIUni_fgPublicIdentifier = *XML::Xercesc::PSVIUni_fgPublicIdentifier;
*PSVIUni_fgReferencedKey = *XML::Xercesc::PSVIUni_fgReferencedKey;
*PSVIUni_fgRequired = *XML::Xercesc::PSVIUni_fgRequired;
*PSVIUni_fgSchemaAnnotations = *XML::Xercesc::PSVIUni_fgSchemaAnnotations;
*PSVIUni_fgSchemaComponents = *XML::Xercesc::PSVIUni_fgSchemaComponents;
*PSVIUni_fgSchemaDefault = *XML::Xercesc::PSVIUni_fgSchemaDefault;
*PSVIUni_fgSchemaDocument = *XML::Xercesc::PSVIUni_fgSchemaDocument;
*PSVIUni_fgSchemaDocuments = *XML::Xercesc::PSVIUni_fgSchemaDocuments;
*PSVIUni_fgSchemaErrorCode = *XML::Xercesc::PSVIUni_fgSchemaErrorCode;
*PSVIUni_fgSchemaInformation = *XML::Xercesc::PSVIUni_fgSchemaInformation;
*PSVIUni_fgSchemaNamespace = *XML::Xercesc::PSVIUni_fgSchemaNamespace;
*PSVIUni_fgSchemaNormalizedValue = *XML::Xercesc::PSVIUni_fgSchemaNormalizedValue;
*PSVIUni_fgSchemaSpecified = *XML::Xercesc::PSVIUni_fgSchemaSpecified;
*PSVIUni_fgScope = *XML::Xercesc::PSVIUni_fgScope;
*PSVIUni_fgSelector = *XML::Xercesc::PSVIUni_fgSelector;
*PSVIUni_fgSimpleTypeDefinition = *XML::Xercesc::PSVIUni_fgSimpleTypeDefinition;
*PSVIUni_fgSubstitutionGroupAffiliation = *XML::Xercesc::PSVIUni_fgSubstitutionGroupAffiliation;
*PSVIUni_fgSubstitutionGroupExclusions = *XML::Xercesc::PSVIUni_fgSubstitutionGroupExclusions;
*PSVIUni_fgSystemIdentifier = *XML::Xercesc::PSVIUni_fgSystemIdentifier;
*PSVIUni_fgTargetNamespace = *XML::Xercesc::PSVIUni_fgTargetNamespace;
*PSVIUni_fgTerm = *XML::Xercesc::PSVIUni_fgTerm;
*PSVIUni_fgTypeDefinition = *XML::Xercesc::PSVIUni_fgTypeDefinition;
*PSVIUni_fgUserInformation = *XML::Xercesc::PSVIUni_fgUserInformation;
*PSVIUni_fgValidationAttempted = *XML::Xercesc::PSVIUni_fgValidationAttempted;
*PSVIUni_fgValidationContext = *XML::Xercesc::PSVIUni_fgValidationContext;
*PSVIUni_fgValidity = *XML::Xercesc::PSVIUni_fgValidity;
*PSVIUni_fgValue = *XML::Xercesc::PSVIUni_fgValue;
*PSVIUni_fgValueConstraint = *XML::Xercesc::PSVIUni_fgValueConstraint;
*PSVIUni_fgVariety = *XML::Xercesc::PSVIUni_fgVariety;
*PSVIUni_fgWildcard = *XML::Xercesc::PSVIUni_fgWildcard;
*PSVIUni_fgXpath = *XML::Xercesc::PSVIUni_fgXpath;
*PSVIUni_fgAll = *XML::Xercesc::PSVIUni_fgAll;
*PSVIUni_fgAny = *XML::Xercesc::PSVIUni_fgAny;
*PSVIUni_fgAppinfo = *XML::Xercesc::PSVIUni_fgAppinfo;
*PSVIUni_fgAtomic = *XML::Xercesc::PSVIUni_fgAtomic;
*PSVIUni_fgChoice = *XML::Xercesc::PSVIUni_fgChoice;
*PSVIUni_fgDefault = *XML::Xercesc::PSVIUni_fgDefault;
*PSVIUni_fgDocumentation = *XML::Xercesc::PSVIUni_fgDocumentation;
*PSVIUni_fgElementOnly = *XML::Xercesc::PSVIUni_fgElementOnly;
*PSVIUni_fgEmpty = *XML::Xercesc::PSVIUni_fgEmpty;
*PSVIUni_fgExtension = *XML::Xercesc::PSVIUni_fgExtension;
*PSVIUni_fgFalse = *XML::Xercesc::PSVIUni_fgFalse;
*PSVIUni_fgFull = *XML::Xercesc::PSVIUni_fgFull;
*PSVIUni_fgGlobal = *XML::Xercesc::PSVIUni_fgGlobal;
*PSVIUni_fgInfoset = *XML::Xercesc::PSVIUni_fgInfoset;
*PSVIUni_fgInvalid = *XML::Xercesc::PSVIUni_fgInvalid;
*PSVIUni_fgKey = *XML::Xercesc::PSVIUni_fgKey;
*PSVIUni_fgKeyref = *XML::Xercesc::PSVIUni_fgKeyref;
*PSVIUni_fgLax = *XML::Xercesc::PSVIUni_fgLax;
*PSVIUni_fgList = *XML::Xercesc::PSVIUni_fgList;
*PSVIUni_fgLocal = *XML::Xercesc::PSVIUni_fgLocal;
*PSVIUni_fgMixed = *XML::Xercesc::PSVIUni_fgMixed;
*PSVIUni_fgNone = *XML::Xercesc::PSVIUni_fgNone;
*PSVIUni_fgNotKnown = *XML::Xercesc::PSVIUni_fgNotKnown;
*PSVIUni_fgNsNamespace = *XML::Xercesc::PSVIUni_fgNsNamespace;
*PSVIUni_fgOnePointZero = *XML::Xercesc::PSVIUni_fgOnePointZero;
*PSVIUni_fgPartial = *XML::Xercesc::PSVIUni_fgPartial;
*PSVIUni_fgRestrict = *XML::Xercesc::PSVIUni_fgRestrict;
*PSVIUni_fgRestriction = *XML::Xercesc::PSVIUni_fgRestriction;
*PSVIUni_fgSchema = *XML::Xercesc::PSVIUni_fgSchema;
*PSVIUni_fgSequence = *XML::Xercesc::PSVIUni_fgSequence;
*PSVIUni_fgSimple = *XML::Xercesc::PSVIUni_fgSimple;
*PSVIUni_fgSkip = *XML::Xercesc::PSVIUni_fgSkip;
*PSVIUni_fgStrict = *XML::Xercesc::PSVIUni_fgStrict;
*PSVIUni_fgSubstitution = *XML::Xercesc::PSVIUni_fgSubstitution;
*PSVIUni_fgTotal = *XML::Xercesc::PSVIUni_fgTotal;
*PSVIUni_fgTrue = *XML::Xercesc::PSVIUni_fgTrue;
*PSVIUni_fgUnbounded = *XML::Xercesc::PSVIUni_fgUnbounded;
*PSVIUni_fgUnion = *XML::Xercesc::PSVIUni_fgUnion;
*PSVIUni_fgUnique = *XML::Xercesc::PSVIUni_fgUnique;
*PSVIUni_fgUnknown = *XML::Xercesc::PSVIUni_fgUnknown;
*PSVIUni_fgValid = *XML::Xercesc::PSVIUni_fgValid;
*PSVIUni_fgVCFixed = *XML::Xercesc::PSVIUni_fgVCFixed;
*PSVIUni_fgXMLChNull = *XML::Xercesc::PSVIUni_fgXMLChNull;
*PSVIUni_fgAg = *XML::Xercesc::PSVIUni_fgAg;
*PSVIUni_fgAnnot = *XML::Xercesc::PSVIUni_fgAnnot;
*PSVIUni_fgAttr = *XML::Xercesc::PSVIUni_fgAttr;
*PSVIUni_fgAu = *XML::Xercesc::PSVIUni_fgAu;
*PSVIUni_fgElt = *XML::Xercesc::PSVIUni_fgElt;
*PSVIUni_fgIdc = *XML::Xercesc::PSVIUni_fgIdc;
*PSVIUni_fgMg = *XML::Xercesc::PSVIUni_fgMg;
*PSVIUni_fgNot = *XML::Xercesc::PSVIUni_fgNot;
*PSVIUni_fgType = *XML::Xercesc::PSVIUni_fgType;
*PSVIUni_fgBounded = *XML::Xercesc::PSVIUni_fgBounded;
*PSVIUni_fgCardinality = *XML::Xercesc::PSVIUni_fgCardinality;
*PSVIUni_fgEnumeration = *XML::Xercesc::PSVIUni_fgEnumeration;
*PSVIUni_fgFractionDigits = *XML::Xercesc::PSVIUni_fgFractionDigits;
*PSVIUni_fgLength = *XML::Xercesc::PSVIUni_fgLength;
*PSVIUni_fgMaxExclusive = *XML::Xercesc::PSVIUni_fgMaxExclusive;
*PSVIUni_fgMaxInclusive = *XML::Xercesc::PSVIUni_fgMaxInclusive;
*PSVIUni_fgMaxLength = *XML::Xercesc::PSVIUni_fgMaxLength;
*PSVIUni_fgMinExclusive = *XML::Xercesc::PSVIUni_fgMinExclusive;
*PSVIUni_fgMinInclusive = *XML::Xercesc::PSVIUni_fgMinInclusive;
*PSVIUni_fgMinLength = *XML::Xercesc::PSVIUni_fgMinLength;
*PSVIUni_fgNumeric = *XML::Xercesc::PSVIUni_fgNumeric;
*PSVIUni_fgOrdered = *XML::Xercesc::PSVIUni_fgOrdered;
*PSVIUni_fgPattern = *XML::Xercesc::PSVIUni_fgPattern;
*PSVIUni_fgTotalDigits = *XML::Xercesc::PSVIUni_fgTotalDigits;
*PSVIUni_fgWhiteSpace = *XML::Xercesc::PSVIUni_fgWhiteSpace;
*PSVIUni_fgNamespaceInfoset = *XML::Xercesc::PSVIUni_fgNamespaceInfoset;
*PSVIUni_fgXsi = *XML::Xercesc::PSVIUni_fgXsi;
*PSVIUni_fgNamespaceInstance = *XML::Xercesc::PSVIUni_fgNamespaceInstance;
*PSVIUni_fgPsv = *XML::Xercesc::PSVIUni_fgPsv;
*PSVIUni_fgNamespacePsvi = *XML::Xercesc::PSVIUni_fgNamespacePsvi;
*PSVIUni_fgXml = *XML::Xercesc::PSVIUni_fgXml;
*PSVIUni_fgNamespaceXmlSchema = *XML::Xercesc::PSVIUni_fgNamespaceXmlSchema;
*XMLElementDecl_fgInvalidElemId = *XML::Xercesc::XMLElementDecl_fgInvalidElemId;
*XMLElementDecl_fgPCDataElemId = *XML::Xercesc::XMLElementDecl_fgPCDataElemId;
*XMLElementDecl_fgPCDataElemName = *XML::Xercesc::XMLElementDecl_fgPCDataElemName;
*XMLAttDef_fgInvalidAttrId = *XML::Xercesc::XMLAttDef_fgInvalidAttrId;
*PERLCALLBACKHANDLER_BASE_TYPE = *XML::Xercesc::PERLCALLBACKHANDLER_BASE_TYPE;
*PERLCALLBACKHANDLER_ERROR_TYPE = *XML::Xercesc::PERLCALLBACKHANDLER_ERROR_TYPE;
*PERLCALLBACKHANDLER_ENTITY_TYPE = *XML::Xercesc::PERLCALLBACKHANDLER_ENTITY_TYPE;
*PERLCALLBACKHANDLER_NODE_TYPE = *XML::Xercesc::PERLCALLBACKHANDLER_NODE_TYPE;
*PERLCALLBACKHANDLER_SAX_TYPE = *XML::Xercesc::PERLCALLBACKHANDLER_SAX_TYPE;
*PERLCALLBACKHANDLER_DOMERROR_TYPE = *XML::Xercesc::PERLCALLBACKHANDLER_DOMERROR_TYPE;
1;
