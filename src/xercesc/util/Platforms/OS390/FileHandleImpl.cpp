/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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

#include <fstream.h>
#include <stdio.h>
#include <ctype.h>
#include <typeinfo>
#define _XOPEN_SOURCE_EXTENDED 1
#include <stdlib.h>
#include <xercesc/util/XMLUniDefs.hpp>
#include "FileHandleImpl.hpp"
#include <xercesc/framework/MemoryManager.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//Constructor:
FileHandleImpl::FileHandleImpl(FILE* open_handle, int o_type, bool r_type, int fileLrecl, MemoryManager* const manager): 
   Handle(open_handle), openType(o_type), recordType(r_type), lrecl(fileLrecl), fMemoryManager(manager) {

   stgBufferPtr = 0;
   nextByte = 0;

   if ((openType == _FHI_WRITE) &&
       (recordType == _FHI_TYPE_RECORD) &&
       (lrecl != 0))
   {
      //stgBufferPtr = new XMLByte [lrecl];
      stgBufferPtr = (XMLByte*) manager->allocate(lrecl * sizeof(XMLByte)); 
   }

// printf("FileHandleImpl constructor called\n");
// printf("stgBufferPtr is: x%8.8X\n", stgBufferPtr);
// printf("Handle is: x%8.8X\n", Handle);
// printf("openType is : %d\n",openType);
// printf("recordType is : %d\n",recordType);
// printf("lrecl is : %d\n",lrecl);

}

//Destructor:

FileHandleImpl::~FileHandleImpl() {

// printf("FileHandleImpl destructor called\n");
// printf("stgBufferPtr is: x%8.8X\n", stgBufferPtr);
// printf("Handle is: x%8.8X\n", Handle);
// printf("openType is : %d\n",openType);
// printf("recordType is : %d\n",recordType);

   if (stgBufferPtr != 0)
   {
// printf("stgBufferPtr is being freed at: x%8.8X\n", stgBufferPtr);
      //delete [] stgBufferPtr;
        fMemoryManager->deallocate(stgBufferPtr);
   }
}

XERCES_CPP_NAMESPACE_END
