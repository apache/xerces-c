
/*
 * (C) Copyright IBM Corp. 1999  All rights reserved.
 *
 * US Government Users Restricted Rights Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *
 * The program is provided "as is" without any warranty express or
 * implied, including the warranty of non-infringement and the implied
 * warranties of merchantibility and fitness for a particular purpose.
 * IBM will not be liable for any damages suffered by you as a result
 * of using the Program. In no event will IBM be liable for any
 * special, indirect or consequential damages or lost profits even if
 * IBM has been advised of the possibility of their occurrence. IBM
 * will not be liable for any third party claims against you.
 */

/**
 * @version Revision: 35 1.3 src/com/ibm/xml/util/CSetDefs.cpp, xml4c2Utils, xml4c2 
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------

#include <sys/types.h>
#include <ctype.h>
 int
strcasecmp (const char *string1,const char * string2)
 {
  char *s1, *s2;
  int result;

  s1 = (char *)string1;
  s2 = (char *)string2;

  while ((result = tolower (*s1) - tolower (*s2)) == 0)
    {
      if (*s1++ == '\0')
        return 0;
      s2++;
    }
  return (result);
}
int
strncasecmp (const char *string1,const char *string2,size_t count)
{
  register char *s1, *s2;
  register int r;
  register unsigned int rcount;
  rcount = (unsigned int) count; 
  if (rcount > 0)
    {
      s1 = (char *)string1;
      s2 = (char *)string2;
      do
	{
	  if ((r = tolower (*s1) - tolower (*s2)) != 0)
	    return r;
	  if (*s1++ == '\0')
	    break;
	  s2++;
	}
      while (--rcount != 0);
    }
  return (0);
}
/* des not appear as though the following is needed */
#ifndef __OS400__
int stricmp(const char* str1, const char*  str2)
{
 return strcasecmp(str1, str2);
}

int strnicmp(const char* str1, const char* str2, const unsigned int count)
{
	if (count == 0)
		return 0;

	return strncasecmp( str1, str2, (size_t)count);
}
#endif
