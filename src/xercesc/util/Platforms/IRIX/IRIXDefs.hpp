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
 * Revision 1.2  2004/09/08 13:56:39  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.1.1.1  2002/02/01 22:22:24  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/03/02 19:55:22  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/06 07:48:27  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  1999/12/14 23:53:31  rahulj
 * Removed the offending Ctrl-M's from the commit message
 * logs which was giving packaging problems.
 *
 * PR:
 * Obtained from:
 * Submitted by:
 * Reviewed by:
 *
 * Revision 1.1  1999/12/01 17:16:17  rahulj
 * Added support for IRIX 6.5.5 using SGI MIPSpro C++ 7.3 and 7.21 generating 32 bit objects. Changes submitted by Marc Stuessel
 *
 */


// ---------------------------------------------------------------------------
//  IRIX runs in big endian mode
// ---------------------------------------------------------------------------

#define ENDIANMODE_BIG
typedef void* FileHandle;

