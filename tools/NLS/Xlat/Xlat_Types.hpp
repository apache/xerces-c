/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Log$
 * Revision 1.4  2004/09/08 13:57:07  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2000/03/02 19:55:54  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:42  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:01:28  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:42:06  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Data types
//
//  ErrReturns
//      These are the values returned from main when an error occurs. It is
//      also used to throw errors out to the main for return.
//
//  OutFormts
//      The available output formats. This is mapped to from the /OutFmt=
//      command line parameter.
// ---------------------------------------------------------------------------
enum ErrReturns
{
    ErrReturn_Success               = 0
    , ErrReturn_BadParameters       = 1
    , ErrReturn_OutFileOpenFailed   = 4
    , ErrReturn_ParserInit          = 5
    , ErrReturn_ParseErr            = 6
    , ErrReturn_LocaleErr           = 7
    , ErrReturn_NoTranscoder        = 8
    , ErrReturn_SrcFmtError         = 9
    , ErrReturn_UnknownDomain       = 10
    , ErrReturn_Internal            = 9999
};

enum OutFormats
{
    OutFormat_Unknown
    , OutFormat_CppSrc
    , OutFormat_ResBundle
    , OutFormat_Win32RC
    , OutFormat_MsgCatalog
};
