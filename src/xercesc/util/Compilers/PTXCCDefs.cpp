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
 * Revision 1.2  2004/09/08 13:56:32  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.1.1.1  2002/02/01 22:22:19  peiyongz
 * sane_include
 *
 * Revision 1.2  2000/04/04 21:07:39  abagchi
 * Fixed copyrights with initial checkin
 *
 */ 

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#include <xercesc/util/Compilers/PTXCCDefs.hpp>
#include <strings.h>


int stricmp(const char* const str1, const char* const  str2) 
{
    return strcasecmp(str1, str2);
}

int strnicmp(const char* const str1, const char* const  str2, const unsigned int count)
{
    if (count == 0)
        return 0;

    return strncasecmp( str1, str2, (size_t)count);
}

