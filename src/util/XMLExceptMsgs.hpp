// This file is generated, don't edit it!!

#if !defined(ERRHEADER_XMLExcepts)
#define ERRHEADER_XMLExcepts

class XMLExcepts
{
public :
    enum Codes
    {
        NoError                            = 0
      , E_LowBounds                        = 1
      , Array_BadIndex                     = 2
      , Array_BadNewSize                   = 3
      , AttrList_BadIndex                  = 4
      , AttDef_BadAttType                  = 5
      , AttDef_BadDefAttType               = 6
      , Bitset_BadIndex                    = 7
      , BufMgr_NoMoreBuffers               = 8
      , BufMgr_BufferNotInPool             = 9
      , CPtr_PointerIsZero                 = 10
      , CM_BinOpHadUnaryType               = 11
      , CM_MustBeMixedOrChildren           = 12
      , CM_NoPCDATAHere                    = 13
      , CM_NotValidForSpecType             = 14
      , CM_UnaryOpHadBinType               = 15
      , CM_UnknownCMType                   = 16
      , CM_UnknownCMSpecType               = 17
      , CM_NoParentCSN                     = 18
      , DTD_UnknownCreateReason            = 19
      , ElemStack_EmptyStack               = 20
      , ElemStack_BadIndex                 = 21
      , ElemStack_StackUnderflow           = 22
      , ElemStack_NoParentPushed           = 23
      , Enum_NoMoreElements                = 24
      , File_CouldNotOpenFile              = 25
      , File_CouldNotGetCurPos             = 26
      , File_CouldNotCloseFile             = 27
      , File_CouldNotSeekToEnd             = 28
      , File_CouldNotSeekToPos             = 29
      , File_CouldNotDupHandle             = 30
      , File_CouldNotReadFromFile          = 31
      , File_CouldNotResetFile             = 32
      , File_CouldNotGetSize               = 33
      , File_CouldNotGetBasePathName       = 34
      , File_BasePathUnderflow             = 35
      , Gen_ParseInProgress                = 36
      , Gen_NoDTDValidator                 = 37
      , Gen_CouldNotOpenDTD                = 38
      , Gen_CouldNotOpenExtEntity          = 39
      , Gen_UnexpectedEOF                  = 40
      , HshTbl_ZeroModulus                 = 41
      , HshTbl_BadHashFromKey              = 42
      , HshTbl_NoSuchKeyExists             = 43
      , Mutex_CouldNotCreate               = 44
      , Mutex_CouldNotClose                = 45
      , Mutex_CouldNotLock                 = 46
      , Mutex_CouldNotUnlock               = 47
      , Mutex_CouldNotDestroy              = 48
      , NetAcc_InternalError               = 49
      , Pool_ElemAlreadyExists             = 50
      , Pool_BadHashFromKey                = 51
      , Pool_InvalidId                     = 52
      , Pool_ZeroModulus                   = 53
      , RdrMgr_ReaderIdNotFound            = 54
      , Reader_BadAutoEncoding             = 55
      , Reader_CantCreateCvtrFor           = 56
      , Reader_CouldNotDecodeFirstLine     = 57
      , Reader_EOIInMultiSeq               = 58
      , Reader_SrcOfsNotSupported          = 59
      , Reader_EncodingStrRequired         = 60
      , Scan_CouldNotOpenSource            = 61
      , Scan_UnbalancedStartEnd            = 62
      , Scan_BadPScanToken                 = 63
      , Stack_BadIndex                     = 64
      , Stack_EmptyStack                   = 65
      , Str_ZeroSizedTargetBuf             = 66
      , Str_UnknownRadix                   = 67
      , Str_TargetBufTooSmall              = 68
      , Str_StartIndexPastEnd              = 69
      , Strm_StdErrWriteFailure            = 70
      , Strm_StdOutWriteFailure            = 71
      , Strm_ConWriteFailure               = 72
      , StrPool_IllegalId                  = 73
      , Trans_CouldNotCreateDefCvtr        = 74
      , Trans_InvalidSizeReq               = 75
      , Trans_Unrepresentable              = 76
      , Trans_NotValidForEncoding          = 77
      , Trans_BadBlockSize                 = 78
      , Trans_BadSrcSeq                    = 79
      , Trans_BadSrcCP                     = 80
      , Trans_BadTrailingSurrogate         = 81
      , URL_MalformedURL                   = 82
      , URL_UnsupportedProto               = 83
      , URL_UnsupportedProto1              = 84
      , URL_OnlyLocalHost                  = 85
      , URL_NoProtocolPresent              = 86
      , URL_ExpectingTwoSlashes            = 87
      , URL_IncorrectEscapedCharRef        = 88
      , URL_UnterminatedHostComponent      = 89
      , URL_RelativeBaseURL                = 90
      , URL_BaseUnderflow                  = 91
      , Vector_BadIndex                    = 92
      , Val_InvalidElemId                  = 93
      , Val_CantHaveIntSS                  = 94
      , XMLRec_UnknownEncoding             = 95
      , E_HighBounds                       = 96
      , W_LowBounds                        = 97
      , W_HighBounds                       = 98
      , V_LowBounds                        = 99
      , V_HighBounds                       = 100
    };

};
#endif

