/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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
 * $Log$
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
#include <iostream.h>

XERCES_CPP_NAMESPACE_BEGIN

StdOutFormatTarget::StdOutFormatTarget()
{}

StdOutFormatTarget::~StdOutFormatTarget()
{}

void StdOutFormatTarget::flush()
{
    cout.flush();
}

void StdOutFormatTarget::writeChars(const XMLByte* const  toWrite
                                  , const unsigned int    count
                                  , XMLFormatter* const   formatter)
{
        // Surprisingly, Solaris was the only platform on which
        // required the char* cast to print out the string correctly.
        // Without the cast, it was printing the pointer value in hex.
        // Quite annoying, considering every other platform printed
        // the string with the explicit cast to char* below.
    cout.write((char *) toWrite, (int) count);
    cout.flush();

}

XERCES_CPP_NAMESPACE_END

