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
 * Revision 1.4  2004/09/08 13:56:40  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2002/11/19 18:19:41  tng
 * [Bug 13487] Linux runs on many non-i386 platforms.
 *
 * Revision 1.2  2002/10/29 18:58:14  tng
 * Support for Linux/390 which is big endian
 *
 * Revision 1.1.1.1  2002/02/01 22:22:24  peiyongz
 * sane_include
 *
 * Revision 1.3  2000/03/02 19:55:24  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:28  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:00  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:30  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Detect endian mode
// ---------------------------------------------------------------------------
#include <endian.h>
#ifdef __BYTE_ORDER
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        #define ENDIANMODE_LITTLE
    #else
        #if __BYTE_ORDER == __BIG_ENDIAN
            #define ENDIANMODE_BIG
        #else
            #error: unknown byte order!
        #endif
    #endif
#endif /* __BYTE_ORDER */

typedef void* FileHandle;

#ifndef LINUX
#define LINUX
#endif
