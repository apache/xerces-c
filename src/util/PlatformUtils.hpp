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
 * Revision 1.7  2000/02/16 18:29:50  abagchi
 * Shifted exception macro to the end of the file to make doc++ happy
 *
 * Revision 1.6  2000/02/15 00:58:28  abagchi
 * Added API docs
 *
 * Revision 1.5  2000/02/06 07:48:03  rahulj
 * Year 2K copyright swat.
 *
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


/**
  * Utilities that must be implemented in a platform-specific way.
  *
  * <p>This class contains functions that must be implemented in a platform-specific
  * manner. This is just an abstract class. The concrete implementations of these
  * functions are available in the per-platform files indide <code>src/util/Platforms</code>.</p>
  *
  */
class XMLUTIL_EXPORT XMLPlatformUtils
{
public :
    /** @name Public Enumerators */
    //@{
    /** Public types */

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
    //@}

    /** @name Public Static Data */
    //@{
    /** Public static data */
    /** This is the network access implementation.
      * This is provided by the per-platform driver, so each platform can choose what actual
      * implementation it wants to use.
      */
    static XMLNetAccessor*      fgNetAccessor;
    /**
      *  This is the transcoding service.
      *  This is provided by the per platform driver, so each platform can choose what implemenation
      *  it wants to use.
      */
    static XMLTransService*     fgTransService;
    //@}

    /** @name Initialization amd Panic methods */
    //@{
    /**
      * Initialization method.
      * This must be called first in any client code.
      */
    static void Initialize();

    /**
      * The panic mechanism.
      *
      * <p>If, during initialization, we cannot even get far enough
      * along to get transcoding up or get message loading working, we call
      * this.</p>
      *
      * <p>Each platform can implement it however they want. This method is
      * expected to display something meaningful and end the process. The
      * enum indicates why its being called, to allow the per-platform code
      * to display something more specific if desired.</p>
      *
      * @param reason The enumeration that defines the cause of the failure
      */
    static void panic
    (
        const   PanicReasons    reason
    );
    //@}

    /** @name File Methods */
    //@{
    /** Get the current file position
      *
      * @param theFile The file handle
      */
    static unsigned int curFilePos(FileHandle theFile);

    /**
      * Closes the file handle
      *
      * @param theFile The file handle
      */
    static void closeFile(FileHandle theFile);

    /**
      * Returns the file size
      *
      * @param theFile The file handle whose size you want
      * @return Returns the size of the file in bytes
      */
    static unsigned int fileSize(FileHandle theFile);

    /**
      * Opens the file
      *
      * @param fileName The string containing the name of the file
      * @return The file handle of the opened file
      */
    static FileHandle openFile(const char* const fileName);

    /**
      * Opens the file
      *
      * @param fileName The string containing the name of the file
      * @return The file handle of the opened file
      */
    static FileHandle openFile(const XMLCh* const fileName);

    /**
      * Opens the standard input as a file
      * @return The file handle of the standard input stream
      */
    static FileHandle openStdInHandle();

    /**
      * Reads the file buffer
      *
      * @param theFile The file handle that you want to read
      * @param toRead The number of byte to read from the current position
      * @param toFill The string buffer to fill
      * @return Returns the number of bytes read from the stream or file
      */
    static unsigned int readFileBuffer
    (
                FileHandle      theFile
        , const unsigned int    toRead
        ,       XMLByte* const  toFill
    );

    /**
      * Resets the file handle
      * @param theFile The file handle that you want to reset
      */
    static void resetFile(FileHandle theFile);
    //@}

    /** @name File System Methods */
    //@{
    /**
      * Gets the full path from a relative path
      * @param srcPath The path of the file for which you want the full path
      * @return Returns the fully qualified path of the file name including the file name
      */
    static XMLCh* getFullPath(const XMLCh* const srcPath);

    /**
      * Looks at a file name and tells if the path is specified relative to a directory, or absolute to the root
      * @param toCheck The file name which you want to check
      * @return Returns true if the filename appears to be relative
      */
    static bool isRelative(const XMLCh* const toCheck);

    /**
      * Utility to join two paths
      * @param basePath The string containing the base path
      * @param relativePath The string containing the relative path
      * @return Returns a string containing the 'woven' path
      */
    static XMLCh* weavePaths
    (
        const   XMLCh* const    basePath
        , const XMLCh* const    relativePath
    );
    //@}

    /** @name Timing Methods */
    //@{
    /**
      * Gets the system time in milliseconds (for later comparison)
      * @return Returns the system time as an unsigned long
      */
    static unsigned long getCurrentMillis();
    //@}

    /** @name Mutex Methods */
    //@{
    /**
      * Closes a mutex handle
      * @param mtxHandle The mutex handle that you want to close
      */
    static void closeMutex(void* const mtxHandle);

    /**
      * Locks a mutex handle
      * @param mtxHandle The mutex handle that you want to lock
      */
    static void lockMutex(void* const mtxHandle);

    /**
      * Make a new mutex
      */
    static void* makeMutex();

    /**
      * Unlocks a mutex
      * @param mtxGandle The mutex handle that you want to unlock
      */
    static void unlockMutex(void* const mtxHandle);
    //@}

    /** @name External Message Support */
    //@{
    /**
      * Loads the message set from among the available domains
      *
      * @param msgDomain The message domain which you want to load
      */
    static XMLMsgLoader* loadMsgSet(const XMLCh* const msgDomain);
    //@}

    /** @name Miscellaneous synchronization methods */
    //@{
    /**
      * Conditionally updates or returns a single word variable atomically
      * The compareAndSwap subroutine performs an atomic operation which
      * compares the contents of a single word variable with a stored old value.
      * If the values are equal, a new value is stored in the single word
      * variable and TRUE is returned; otherwise, the old value is set to the
      * current value of the single word variable and FALSE is returned.
      *
      * The compareAndSwap subroutine is useful when a word value must be
      * updated only if it has not been changed since it was last read.
      *
      * Note: The word containing the single word variable must be aligned
      * on a full word boundary.
      *
      * @param toFill Specifies the address of the single word variable
      * @param newValue Specifies the new value to be conditionally assigned to the single word variable.
      * @param toCompare Specifies the address of the old value to be checked against (and conditionally updated with) the value of the single word variable.
      * @return Returns the new value assigned to the single word variable
      */
    static void* compareAndSwap
    (
                void**      toFill
        , const void* const newValue
        , const void* const toCompare
    );
    //@}

    /** @name Atomic Increment and Decrement */
    //@{

    /**
      * Increments a single word variable atomically.
      * The atomicIncrement subroutine increments one word in a single atomic
      * operation. This operation is useful when a counter variable is shared
      * between several threads or processes. When updating such a counter
      * variable, it is important to make sure that the fetch, update, and
      * store operations occur atomically (are not interruptible).
      *
      * @param location Specifies the address of the word variable to be incremented.
      *
      * @return The function return value is positive if the result of the operation
      * was positive. Zero if the result of the operation was zero. Negative
      * if the result of the operation was negative. Except for the zero
      * case, the value returned may differ from the actual result of the
      * operation - only the sign and zero/nonzero state is guaranteed to be
      * correct.
      */
    static int atomicIncrement(int& location);
    /**
      * Decrements a single word variable atomically.
      * The atomicDecrement subroutine increments one word in a single atomic
      * operation. This operation is useful when a counter variable is shared
      * between several threads or processes. When updating such a counter
      * variable, it is important to make sure that the fetch, update, and
      * store operations occur atomically (are not interruptible).
      *
      * @param location Specifies the address of the word variable to be decremented.
      *
      * @return The function return value is positive if the result of the operation
      * was positive. Zero if the result of the operation was zero. Negative
      * if the result of the operation was negative. Except for the zero
      * case, the value returned may differ from the actual result of the
      * operation - only the sign and zero/nonzero state is guaranteed to be
      * correct.
      */
    static int atomicDecrement(int& location);
    //@}

private :
    /** @name Private static methods */
    //@{
    /** Loads a message set from the available domains
      *
      * @param msgDomain The message domain containing the message to be loaded
      */
    static XMLMsgLoader* loadAMsgSet(const XMLCh* const msgDomain);

    /**
      * Creates a net accessor object
      */
    static XMLNetAccessor* makeNetAccessor();

    /**
      * Creates a Transoding service
      */
    static XMLTransService* makeTransService();

    /**
      * Does initialization for a particular platform
      * Here you put in code that you wish to execute before anything else is done in the application.
      */
    static void platformInit();
    //@}

    /** @name Private static data members */
    //@{
    /** This is used to avoid multiple inits if the client code calls us
      * more than once. They aren't supposed to, but some have trouble
      * keeping up if they are COM objects and such.
      */
    static bool     fgInitFlag;
    //@}
};

//
//  Generate an exception for platform utilities to throw when something
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

#endif
