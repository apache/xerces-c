/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
 * Revision 1.2  1999/12/15 19:41:28  roddey
 * Support for the new transcoder system, where even intrinsic encodings are
 * done via the same transcoder abstraction as external ones.
 *
 * Revision 1.1.1.1  1999/11/09 01:04:53  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:11  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XMLMsgLoader.hpp>
#include <util/Mutexes.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLString.hpp>
#include <util/TransService.hpp>
#include <util/XMLUni.hpp>


// ---------------------------------------------------------------------------
//  Local data members
//
//  gMsgMutex
//      This is a mutex that will be used to synchronize access to the single
//      static message loader for exception messages.
// ---------------------------------------------------------------------------
static XMLMutex*    gMsgMutex = 0;


// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Static Data Members
// ---------------------------------------------------------------------------
const char*         XMLPlatformUtils::fgLibLocation = 0;
bool                XMLPlatformUtils::fgInitFlag = false;
XMLNetAccessor*     XMLPlatformUtils::fgNetAccessor = 0;
XMLTransService*    XMLPlatformUtils::fgTransService = 0;



// ---------------------------------------------------------------------------
//  XMLPlatformUtils: Platform independent init methods
// ---------------------------------------------------------------------------
void XMLPlatformUtils::Initialize()
{
    //
    //  Make sure we haven't already been initialized. Note that this is not
    //  thread safe and is not intended for that. Its more for those COM
    //  like processes that cannot keep up with whether they have initialized
    //  us yet or not.
    //
    if (fgInitFlag)
        return;
    fgInitFlag = true;

    // Create the message sync mutex
    gMsgMutex = new XMLMutex;

    //
    //  Call the platform init method, which is implemented in each of the
    //  per-platform implementation cpp files. This one does the very low
    //  level per-platform setup. It cannot use any XML util services at all,
    //  i.e. only native services.
    //
    //  One important thing it does is to set the fgLibLocation string, which
    //  can be used by any subsequent init code that needs to find stuff
    //  relative to the lib/DLL.
    //
    platformInit();

    //
    //  Ask the per-platform code to make the desired transcoding service for
    //  us to use. This call cannot throw any exceptions or do anything that
    //  cause any transcoding to happen. It should create the service and
    //  return it or zero if it cannot.
    //
    //  This one also cannot use any utility services. It can only create a
    //  transcoding service object and return it.
    //
    //  If we cannot make one, then we call panic to end the process.
    //
    fgTransService = makeTransService();
    if (!fgTransService)
        panic(Panic_NoTransService);

    //
    //  Try to create a default transcoder. This is the one that will be
    //  used internally by the XMLString class. If we cannot create one, then
    //  call the panic method.
    //
    XMLLCPTranscoder* defXCode = XMLPlatformUtils::fgTransService->makeNewLCPTranscoder();
    if (!defXCode)
        panic(Panic_NoDefTranscoder);
    XMLString::initString(defXCode);

    //
    //  Now lets ask the per-platform code to give us an instance of the type
    //  of network access implementation he wants to use.
    //
    fgNetAccessor = makeNetAccessor();
    if (!fgNetAccessor)
    {
        // <TBD> For now its returning a null, but later this will be required
    }
}


// ---------------------------------------------------------------------------
//  Platform independent Msg support methods
// ---------------------------------------------------------------------------
XMLMsgLoader* XMLPlatformUtils::loadMsgSet(const XMLCh* const msgDomain)
{
    //
    //  Ask the platform support to load up the correct type of message
    //  loader for the indicated message set. We don't check here whether it
    //  works or not. That's their decision.
    //
    return loadAMsgSet(msgDomain);
}
