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

/**
 * $Log$
 * Revision 1.2  2000/02/06 07:47:47  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:30  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:36  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <memory.h>
#include <string.h>
#include <util/RuntimeException.hpp>
#include <framework/XMLBuffer.hpp>
#include <framework/XMLBufferMgr.hpp>



// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
XMLBufferMgr::XMLBufferMgr() :

    fBufCount(32)
    , fBufList(0)
{
    // Allocate the buffer list and zero it out
    fBufList = new XMLBuffer*[fBufCount];
    for (unsigned int index = 0; index < fBufCount; index++)
        fBufList[index] = 0;
}

XMLBufferMgr::~XMLBufferMgr()
{
    // Delete any buffers that got allocated
    for (unsigned int index = 0; index < fBufCount; index++)
        delete fBufList[index];

    // And then the buffer list
    delete [] fBufList;
}


// ---------------------------------------------------------------------------
//  Buffer management
// ---------------------------------------------------------------------------
XMLBuffer& XMLBufferMgr::bidOnBuffer()
{
    //
    //  Look for a buffer that is not in use. If we hit a null entry, then
    //  we have to add one.
    //
    for (unsigned int index = 0; index < fBufCount; index++)
    {
        // No more buffers available, so create one and take it
        if (!fBufList[index])
        {
            fBufList[index] = new XMLBuffer;
            fBufList[index]->setInUse(true);
            return *fBufList[index];
        }

        //
        //  There's one here, so see if its use. If not, mark it, reset it,
        //  and take it
        //
        if (!fBufList[index]->getInUse())
        {
            fBufList[index]->reset();
            fBufList[index]->setInUse(true);
            return *(fBufList[index]);
        }
    }

    // We did not find one, so freak out
    ThrowXML(RuntimeException, XML4CExcepts::BufMgr_NoMoreBuffers);

    // NOTE: Dummy return to make some compilers happy. Never really gets called!
    return *fBufList[0];
}


void XMLBufferMgr::releaseBuffer(XMLBuffer& toRelease)
{
    // Look for this buffer in the list
    for (unsigned int index = 0; index < fBufCount; index++)
    {
        if (fBufList[index] == &toRelease)
        {
            // Unmark it
            toRelease.setInUse(false);
            return;
        }
    }

    // It was not a legal buffer
    ThrowXML(RuntimeException, XML4CExcepts::BufMgr_BufferNotInPool);
}
