/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id$
 */

#include <xercesc/util/XercesDefs.hpp>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// These functions are needed because MacOS doesn't define them
//	(these routines are defined in CW 8 by extras.h, but there is no MachO
//	library for extras).

#if __MACH__
// Compare lexigraphically two strings
int stricmp(const char *s1, const char *s2)
{
    char c1, c2;
    while (1)
    {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (c1 == 0) return 0;
    }
}

// Compare lexigraphically two strings up to a max length
int strnicmp(const char *s1, const char *s2, const unsigned int n)
{
    int i;
    char c1, c2;
    for (i=0; i<n; i++)
    {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (!c1) return 0;
    }
    return 0;
}
#endif


#if defined(_WIN32) || defined(WIN32)
int mbswcslen(const char * s, const unsigned int n)
{
	int     result;
	char *  source;
	int count = -1;
	size_t  source_len;
	
	source_len = strlen(s);

    source      = (char *)s;
    
    for (count = 0; count < n; count++)
    {
    	if (*source)
    	{
        	result = mbtowc(0, source, source_len);
        	if (result > 0)
        	{
        		source += result;
        		source_len -= result;
        	}
        	else
        		return((size_t)-1);								/*- mm 011102 -*/
        }
        else
        	break;
    }

	return(count);
}

int wcsmbslen(const wchar_t * pwcs, const unsigned int n)
{
	int     count = 0;
	int     result;
	wchar_t * source;
	char    temp[3];
	
	if (!pwcs)
		return (0);
	
	source = (wchar_t*)pwcs;
	
	while(count <= n)
	{
		if (!*source)
		{
			break;
		}
		else
		{
			result = wctomb(temp, *source++);
			if ((count + result) <= n)
			{
				count += result;
			}
			else
				break;
		}
	}
			
	return(count);
}
#endif




