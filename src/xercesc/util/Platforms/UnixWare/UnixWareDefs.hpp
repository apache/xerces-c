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
 * Revision 1.3  2004/09/08 13:56:43  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.2  2002/05/03 15:33:57  peiyongz
 * Bug#7261: Remove obsolete define in UnixWareDefs.hpp  - patch from
 * Martin Kalen.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:26  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/03/02 19:55:32  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/06 07:48:31  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  1999/12/17 23:31:29  rahulj
 * Fixed the typo in Little endian #define.
 *
 * Revision 1.1  1999/12/17 22:24:03  rahulj
 * Added missing UnixWare files to the repository.
 *
 * Created by Ron Record (rr@sco.com) based on SolarisDefs
 * 13-Nov-1999
 */


// ---------------------------------------------------------------------------
//  UnixWare runs in little endian mode
// ---------------------------------------------------------------------------
#define ENDIANMODE_LITTLE
typedef void* FileHandle;



