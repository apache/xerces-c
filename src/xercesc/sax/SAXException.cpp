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
 * $Log$
 * Revision 1.3  2003/08/13 15:43:24  knoaman
 * Use memory manager when creating SAX exceptions.
 *
 * Revision 1.2  2002/11/04 14:56:26  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:08  peiyongz
 * sane_include
 *
 * Revision 1.1  2000/08/09 22:06:04  jpolast
 * more functionality to SAXException and its children.
 * msgs are now functional for SAXNotSupportedEx and
 * SAXNotRecognizedEx
 *
 *
 */

#include "SAXException.hpp"

XERCES_CPP_NAMESPACE_BEGIN


// SAXNotSupportedException Constructors
SAXNotSupportedException::SAXNotSupportedException(MemoryManager* const manager)
	: SAXException(manager)
{
}

SAXNotSupportedException::SAXNotSupportedException(const XMLCh* const msg,
                                                   MemoryManager* const manager)
	: SAXException(msg, manager)
{
}

SAXNotSupportedException::SAXNotSupportedException(const char* const msg,
                                                   MemoryManager* const manager)
	: SAXException(msg, manager)
{
}

SAXNotSupportedException::SAXNotSupportedException(const SAXException& toCopy)
  : SAXException(toCopy)
{
}

// SAXNotRecognizedException Constructors
SAXNotRecognizedException::SAXNotRecognizedException(MemoryManager* const manager)
	: SAXException(manager)
{
}

SAXNotRecognizedException::SAXNotRecognizedException(const XMLCh* const msg,
                                                     MemoryManager* const manager)
	: SAXException(msg, manager)
{
}

SAXNotRecognizedException::SAXNotRecognizedException(const char* const msg,
                                                     MemoryManager* const manager)
	: SAXException(msg, manager)
{
}

SAXNotRecognizedException::SAXNotRecognizedException(const SAXException& toCopy)
  : SAXException(toCopy)
{
}

XERCES_CPP_NAMESPACE_END

