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
      , NetAcc_InitFailed                  = 50
      , NetAcc_TargetResolution            = 51
      , NetAcc_CreateSocket                = 52
      , NetAcc_ConnSocket                  = 53
      , NetAcc_WriteSocket                 = 54
      , NetAcc_ReadSocket                  = 55
      , Pool_ElemAlreadyExists             = 56
      , Pool_BadHashFromKey                = 57
      , Pool_InvalidId                     = 58
      , Pool_ZeroModulus                   = 59
      , RdrMgr_ReaderIdNotFound            = 60
      , Reader_BadAutoEncoding             = 61
      , Reader_CantCreateCvtrFor           = 62
      , Reader_CouldNotDecodeFirstLine     = 63
      , Reader_EOIInMultiSeq               = 64
      , Reader_SrcOfsNotSupported          = 65
      , Reader_EncodingStrRequired         = 66
      , Scan_CouldNotOpenSource            = 67
      , Scan_UnbalancedStartEnd            = 68
      , Scan_BadPScanToken                 = 69
      , Stack_BadIndex                     = 70
      , Stack_EmptyStack                   = 71
      , Str_ZeroSizedTargetBuf             = 72
      , Str_UnknownRadix                   = 73
      , Str_TargetBufTooSmall              = 74
      , Str_StartIndexPastEnd              = 75
      , Str_ConvertOverflow                = 76
      , Strm_StdErrWriteFailure            = 77
      , Strm_StdOutWriteFailure            = 78
      , Strm_ConWriteFailure               = 79
      , StrPool_IllegalId                  = 80
      , Trans_CouldNotCreateDefCvtr        = 81
      , Trans_InvalidSizeReq               = 82
      , Trans_Unrepresentable              = 83
      , Trans_NotValidForEncoding          = 84
      , Trans_BadBlockSize                 = 85
      , Trans_BadSrcSeq                    = 86
      , Trans_BadSrcCP                     = 87
      , Trans_BadTrailingSurrogate         = 88
      , URL_MalformedURL                   = 89
      , URL_UnsupportedProto               = 90
      , URL_UnsupportedProto1              = 91
      , URL_OnlyLocalHost                  = 92
      , URL_NoProtocolPresent              = 93
      , URL_ExpectingTwoSlashes            = 94
      , URL_IncorrectEscapedCharRef        = 95
      , URL_UnterminatedHostComponent      = 96
      , URL_RelativeBaseURL                = 97
      , URL_BaseUnderflow                  = 98
      , URL_BadPortField                   = 99
      , Vector_BadIndex                    = 100
      , Val_InvalidElemId                  = 101
      , Val_CantHaveIntSS                  = 102
      , XMLRec_UnknownEncoding             = 103
      , E_HighBounds                       = 104
      , W_LowBounds                        = 105
      , W_HighBounds                       = 106
      , V_LowBounds                        = 107
      , V_HighBounds                       = 108
    };

};
#endif

