/*
 * Copyright 1999-2002,2004 The Apache Software Foundation.
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
 * $Id$
 * $Log$
 * Revision 1.10  2004/09/08 13:55:58  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.9  2004/02/09 23:01:43  peiyongz
 * Bug#20684: patch from carbe771@student.liu.se (C-J Berg)
 *
 * Revision 1.8  2004/01/29 11:46:29  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.7  2003/05/29 11:18:37  gareth
 * Added macros in so we can determine whether to do things like iostream as opposed to iostream.h and whether to use std:: or not.
 *
 * Revision 1.6  2003/01/24 20:20:22  tng
 * Add method flush to XMLFormatTarget
 *
 * Revision 1.5  2003/01/09 18:58:29  tng
 * [Bug 15427] DOMWriter dose not flush the output stream.
 *
 * Revision 1.4  2002/11/22 14:25:59  tng
 * Got a number of compilation erros for those non-ANSI C++ compliant compiler like xlC v3.
 * Since the previous fix is just for fixing a "warning", I think it doesn't worth to break users who
 * are not ANSI C++ ready yet.
 *
 * Revision 1.3  2002/11/21 15:45:34  gareth
 * gcc 3.2 now issues a warning for use of iostream.h. Removed the .h and prefixed cout with std::.
 *
 * Revision 1.2  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.1  2002/05/28 22:40:46  peiyongz
 * DOM3 Save Interface: DOMWriter/DOMWriterFilter
 *
 */

#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <stdio.h>

XERCES_CPP_NAMESPACE_BEGIN

StdOutFormatTarget::StdOutFormatTarget()
{}

StdOutFormatTarget::~StdOutFormatTarget()
{}

void StdOutFormatTarget::flush()
{
    fflush(stdout);
}

void StdOutFormatTarget::writeChars(const XMLByte* const  toWrite
                                  , const unsigned int    count
                                  , XMLFormatter* const)
{
    // Surprisingly, Solaris was the only platform on which
    // required the char* cast to print out the string correctly.
    // Without the cast, it was printing the pointer value in hex.
    // Quite annoying, considering every other platform printed
    // the string with the explicit cast to char* below.
    fwrite(toWrite, sizeof(XMLByte), (size_t)count, stdout);
    fflush(stdout);
}

XERCES_CPP_NAMESPACE_END

