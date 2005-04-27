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
 * Revision 1.6  2005/04/27 18:21:51  cargilld
 * Fix for problem on Solaris where open may return 0 as a valid FileHandle.  Check for -1 instead.
 *
 * Revision 1.5  2004/09/08 13:56:42  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2004/08/23 16:05:49  peiyongz
 * The missing FileHandle
 *
 * Revision 1.3  2004/08/19 21:24:12  peiyongz
 * Jira[1255], patch from Neil Sharman
 *
 * Revision 1.2  2003/11/17 16:18:01  peiyongz
 * Fix to  #4556
 *
 * Revision 1.1.1.1  2002/02/01 22:22:25  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/03/02 19:55:30  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/17 03:36:54  rahulj
 * Fixed a cut-paste typo in the comments.
 *
 * Revision 1.2  2000/02/06 07:48:30  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:06:30  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:32  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

 // --------------------------------------------------------------------------- 
 // Detect endian mode 
 // --------------------------------------------------------------------------- 
#include <sys/isa_defs.h> 

#ifdef _LITTLE_ENDIAN 
  #define ENDIANMODE_LITTLE 
#elif defined(_BIG_ENDIAN) 
  #define ENDIANMODE_BIG 
#else 
  #error : unknown byte order! 
#endif 
 
typedef int FileHandle;

#undef  XERCES_Invalid_File_Handle
#define XERCES_Invalid_File_Handle -1
 
#ifndef SOLARIS
#define SOLARIS
#endif

