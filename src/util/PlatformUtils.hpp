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
 * Revision 1.4  2000/01/25 21:34:23  roddey
 * Added two more 'panic' values, which were requested by folks who needed
 * to report a couple more panic situations.
 *
 * Revision 1.3  2000/01/19 00:56:59  roddey
 * Changes to get rid of dependence on old utils standard streams and to
 * get rid of the fgLibLocation stuff.
 *
 * Revision 1.2  2000/01/12 00:16:22  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.1.1.1  1999/11/09 01:04:55  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:45:11  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(PLATFORMUTILS_HPP)
#define PLATFORMUTILS_HPP

#include <util/XML4CDefs.hpp>
#include <util/XMLException.hpp>
#include <util/XMLUni.hpp>

class XMLMsgLoader;
class XMLNetAccessor;
class XMLTransService;


//
//  Generate an exception for platform utitilities to throw when something
//  goes awry.
//
const XMLCh gXMLPlatformUtilsException_Name[] =
{
        chLatin_X, chLatin_M, chLatin_L, chLatin_P, chLatin_l, chLatin_a
    ,   chLatin_t, chLatin_f, chLatin_o, chLatin_r, chLatin_m, chLatin_E
    ,   chLatin_x, chLatin_c, chLatin_e, chLatin_p, chLatin_t, chLatin_i
    ,   chLatin_o, chLatin_n, chNull
};
MakeXML4CException(XMLPlatformUtilsException, XMLUTIL_EXPORT)


class XMLUTIL_EXPORT XMLPlatformUtils
{
public :
    // -----------------------------------------------------------------------
    //  Public types
    // -----------------------------------------------------------------------
    enum PanicReasons
    {
        Panic_NoTransService
        , Panic_NoDefTranscoder
        , Panic_CantFindLib
        , Panic_UnknownMsgDomain
        , Panic_CantLoadMsgDomain
        , Panic_SynchronizationErr
        , Panic_SystemInit

        , PanicReasons_Count
    };


    // -----------------------------------------------------------------------
    //  Public, static data
    //
    //  fgNetAccessor
    //      This is the network access implementation. This is provided by
    //      the per-platform driver, so each platform can choose what actual
    //      implementation it wants to use.
    //
    //  fgTransService
    //      This is the transcoding service. This is provided by the per
    //      platform driver, so each platform can choose what implemenation
    //      it wants to use.
    // -----------------------------------------------------------------------
    static XMLNetAccessor*      fgNetAccessor;
    static XMLTransService*     fgTransService;


    // -----------------------------------------------------------------------
    //  Initialization method. This must be called first in any client code.
    // -----------------------------------------------------------------------
    static void Initialize();


    // -----------------------------------------------------------------------
    //  The panic mechanism. If, during init, we cannot even get far enough
    //  along to get transcoding up or get message loading working, we call
    //  this.
    //
    //  Each platform can implement it however they want. This method is
    //  expected to display something meaningful and end the process. The
    //  enum indicates why its being called, to allow the per-platform code
    //  to display something more specific if desired.
    // -----------------------------------------------------------------------
    static void panic
    (
        const   PanicReasons    reason
    );


    // -----------------------------------------------------------------------
    //  File methods
    // -----------------------------------------------------------------------
    static unsigned int curFilePos(FileHandle theFile);
    static void closeFile(FileHandle theFile);
    static unsigned int fileSize(FileHandle theFile);
    static FileHandle openFile(const char* const fileName);
    static FileHandle openFile(const XMLCh* const fileName);
    static FileHandle openStdInHandle();
    static unsigned int readFileBuffer
    (
                FileHandle      theFile
        , const unsigned int    toRead
        ,       XMLByte* const  toFill
    );
    static void resetFile(FileHandle theFile);


    // -----------------------------------------------------------------------
    //  File system methods
    // -----------------------------------------------------------------------
    static XMLCh* getFullPath(const XMLCh* const srcPath);
    static bool isRelative(const XMLCh* const toCheck);
    static XMLCh* weavePaths
    (
        const   XMLCh* const    basePath
        , const XMLCh* const    relativePath
    );


    // -----------------------------------------------------------------------
    //  Timing methods
    // -----------------------------------------------------------------------
    static unsigned long getCurrentMillis();


    // -----------------------------------------------------------------------
    //  Mutex methods
    // -----------------------------------------------------------------------
    static void closeMutex(void* const mtxHandle);
    static void lockMutex(void* const mtxHandle);
    static void* makeMutex();
    static void unlockMutex(void* const mtxHandle);


    // -----------------------------------------------------------------------
    //  External message support
    // -----------------------------------------------------------------------
    static XMLMsgLoader* loadMsgSet(const XMLCh* const msgDomain);


    // -----------------------------------------------------------------------
    //  Miscellaneous synchronization methods
    // -----------------------------------------------------------------------
    static void* compareAndSwap
    (
                void**      toFill
        , const void* const newValue
        , const void* const toCompare
    );


    // -----------------------------------------------------------------------
    //  Atomic Increment and Decrement
    //
    //  The function return value is positive if the result of the operation
    //  was positive. Zero if the result of the operation was zero. Negative
    //  if the result of the operation was negative. Except for the zero
    //  case, the value returned may differ from the actual result of the
    //  operation - only the sign and zero/nonzero state is guaranteed to be
    //  correct.
    // -----------------------------------------------------------------------
    static int atomicIncrement(int& location);
    static int atomicDecrement(int& location);


private :
    // -----------------------------------------------------------------------
    //  Private static methods. These are provided by the per-platform
    //  implementation files.
    // -----------------------------------------------------------------------
    static XMLMsgLoader* loadAMsgSet(const XMLCh* const msgDomain);
    static XMLNetAccessor* makeNetAccessor();
    static XMLTransService* makeTransService();
    static void platformInit();


    // -----------------------------------------------------------------------
    //  Private static data members
    //
    //  fgInitFlag
    //      This is used to avoid multiple inits if the client code calls us
    //      more than once. They aren't supposed to, but some have trouble
    //      keeping up if they are COM objects and such.
    // -----------------------------------------------------------------------
    static bool     fgInitFlag;
};

#endif
