#if (__OS400_TGTVRM__>=510)                               /* @01a */
    #pragma datamodel(P128)                               /* @01a */
#endif                                                    /* @01a */

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

/**
 * $Log$
 * Revision 1.4  2005/05/18 13:44:43  cargilld
 * OS400 updates from Jay Hansen.
 *
 * Revision 1.3  2004/09/23 21:44:13  cargilld
 * Fixes to build on OS400.  Thanks to Patrick Townsend and Jay Hansen.
 *
 * Revision 1.2  2004/09/08 13:56:41  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.1.1.1  2002/02/01 22:22:25  peiyongz
 * sane_include
 *
 * Revision 1.2  2000/03/02 21:10:03  abagchi
 * Replaced XML4CDefs with XercesDefs
 *
 * Revision 1.1  2000/02/10 17:58:07  abagchi
 * Initial checkin
 *
 */


// ---------------------------------------------------------------------------
//  AIX runs in big endian mode
// ---------------------------------------------------------------------------
#define ENDIANMODE_BIG


// ---------------------------------------------------------------------------
//  And define our file handle abstraction
// ---------------------------------------------------------------------------
typedef void* FileHandle;
#if (__OS400_TGTVRM__>=510)                                /* @01a */  
     #pragma datamodel(pop)                                /* @01a */ 
#endif                                                     /* @01a */

