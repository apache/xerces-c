/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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


#if !defined(PLATFORMUTILS_HPP)
#define PLATFORMUTILS_HPP

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/XMLException.hpp>

class XMLMsgLoader;
class XMLNetAccessor;
class XMLTransService;


//
//  For internal use only
//
//  This class provides a simple abstract API via which lazily evaluated
//  data can be cleaned up.
//
class XMLUTIL_EXPORT XMLDeleter
{
public :
    virtual ~XMLDeleter();

protected :
    XMLDeleter();

private :
    XMLDeleter(const XMLDeleter&);
    void operator=(const XMLDeleter&);
};


/**
  * Utilities that must be implemented in a platform-specific way.
  *
  * This class contains methods that must be implemented in a platform
  * specific manner. The actual implementations of these methods are
  * available in the per-platform files indide <code>src/util/Platforms
  * </code>.
  */
class XMLUTIL_EXPORT XMLPlatformUtils
{
public :
    /** @name Public Types */
    //@{
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

    /** The network accessor
      *
      * This is provided by the per-platform driver, so each platform can
      * choose what actual implementation it wants to use. The object must
      * be dynamically allocated.
      *
      * <i>Note that you may optionally, if your platform driver does not
      * install a network accessor, set it manually from your client code
      * after calling Initialize(). This works because this object is
      * not required during initialization, and only comes into play during
      * actual XML parsing.</i>
      */
    static XMLNetAccessor*      fgNetAccessor;

    /** The transcoding service.
      *
      * This is provided by the per platform driver, so each platform can
      * choose what implemenation it wants to use. When the platform
      * independent initialization code needs to get a transcoding service
      * object, it will call <code>makeTransService()</code> to ask the
      * per-platform code to create one. Only one transcoding service
      * object is reqeusted per-process, so it is shared and synchronized
      * among parser instances within that process.
      */
    static XMLTransService*     fgTransService;

    //@}


    /** @name Initialization amd Panic methods */
    //@{

    /** Perform per-process parser initialization
      *
      * Initialization <b>must</b> be called first in any client code.
      */
    static void Initialize();

    /** Perform per-process parser termination
      *
      * The termination call is currently optional, to aid those dynamically
      * loading the parser to clean up before exit, or to avoid spurious
      * reports from leak detectors.
      */
    static void Terminate();

    /** The panic mechanism.
      *
      * If, during initialization, we cannot even get far enough along
      * to get transcoding up or get message loading working, we call
      * this method.</p>
      *
      * Each platform can implement it however they want. This method is
      * expected to display something meaningful and end the process. The
      * enum indicates why its being called, to allow the per-platform code
      * to display or log something more specific if desired.</p>
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
      * This must be implemented by the per-platform driver, which should
      * use local file services to deterine the current position within
      * the passed file.
      *
      * Since the file API provided here only reads, if the host platform
      * supports separate read/write positions, only the read position is
      * of any interest, and hence should be the one returned.
      *
      * @param theFile The file handle
      */
    static unsigned int curFilePos(FileHandle theFile);

    /** Closes the file handle
      *
      * This must be implemented by the per-platform driver, which should
      * use local file services to close the passed file handle, and to
      * destroy the passed file handle and any allocated data or system
      * resources it contains.
      *
      * @param theFile The file handle to close
      */
    static void closeFile(FileHandle theFile);

    /** Returns the file size
      *
      * This must be implemented by the per-platform driver, which should
      * use local file services to determine the current size of the file
      * represented by the passed handle.
      *
      * @param theFile The file handle whose size you want
      *
      * @return Returns the size of the file in bytes
      */
    static unsigned int fileSize(FileHandle theFile);

    /** Opens the file
      *
      * This must be implemented by the per-platform driver, which should
      * use local file services to open passed file. If it fails, a
      * null handle pointer should be returned.
      *
      * @param fileName The string containing the name of the file
      *
      * @return The file handle of the opened file
      */
    static FileHandle openFile(const char* const fileName);

    /** Opens a named file
      *
      * This must be implemented by the per-platform driver, which should
      * use local file services to open the passed file. If it fails, a
      * null handle pointer should be returned.
      *
      * @param fileName The string containing the name of the file
      *
      * @return The file handle of the opened file
      */
    static FileHandle openFile(const XMLCh* const fileName);

    /** Open a named file to write
      *
      * This must be implemented by the per-platform driver, which should
      * use local file services to open passed file. If it fails, a
      * null handle pointer should be returned.
      *
      * @param fileName The string containing the name of the file
      *
      * @return The file handle of the opened file
      */
    static FileHandle openFileToWrite(const char* const fileName);

    /** Open a named file to write
      *
      * This must be implemented by the per-platform driver, which should
      * use local file services to open the passed file. If it fails, a
      * null handle pointer should be returned.
      *
      * @param fileName The string containing the name of the file
      *
      * @return The file handle of the opened file
      */
    static FileHandle openFileToWrite(const XMLCh* const fileName);

    /** Opens the standard input as a file
      *
      * This must be implemented by the per-platform driver, which should
      * use local file services to open a handle to the standard input.
      * It should be a copy of the standard input handle, since it will
      * be closed later!
      *
      * @return The file handle of the standard input stream
      */
    static FileHandle openStdInHandle();

    /** Reads the file buffer
      *
      * This must be implemented by the per-platform driver, which should
      * use local file services to read up to 'toRead' bytes of data from
      * the passed file, and return those bytes in the 'toFill' buffer. It
      * is not an error not to read the requested number of bytes. When the
      * end of file is reached, zero should be returned.
      *
      * @param theFile The file handle to be read from.
      * @param toRead The maximum number of byte to read from the current
      * position
      * @param toFill The byte buffer to fill
      *
      * @return Returns the number of bytes read from the stream or file
      */
    static unsigned int readFileBuffer
    (
                FileHandle      theFile
        , const unsigned int    toRead
        ,       XMLByte* const  toFill
    );

    /** Writes the buffer to the file
      *
      * This must be implemented by the per-platform driver, which should
      * use local file services to write up to 'toWrite' bytes of data to
      * the passed file. Unless exception raised by local file services,
      * 'toWrite' bytes of data is to be written to the passed file.
      *
      * @param theFile The file handle to be written to.
      * @param toWrite The maximum number of byte to write from the current
      * position
      * @param toFlush The byte buffer to flush
      *
      * @return void
      */
    static void writeBufferToFile
    (
          FileHandle     const  theFile
        , long                  toWrite
        , const XMLByte* const  toFlush
    );

    /** Resets the file handle
      *
      * This must be implemented by the per-platform driver which will use
      * local file services to reset the file position to the start of the
      * the file.
      *
      * @param theFile The file handle that you want to reset
      */
    static void resetFile(FileHandle theFile);

    //@}


    /** @name File System Methods */
    //@{
    /** Gets the full path from a relative path
      *
      * This must be implemented by the per-platform driver. It should
      * complete a relative path using the 'current directory', or whatever
      * the local equivalent of a current directory is. If the passed
      * source path is actually fully qualified, then a straight copy of it
      * will be returned.
      *
      * @param srcPath The path of the file for which you want the full path
      *
      * @return Returns the fully qualified path of the file name including
      * the file name. This is dyanmically allocated and must be deleted
      * by the caller when its no longer needed!
      */
    static XMLCh* getFullPath(const XMLCh* const srcPath);

    /** Determines if a path is relative or absolute
      *
      * This must be implemented by the per-platform driver, which should
      * determine whether the passed path is relative or not. The concept
      * of relative and absolute might be... well relative on different
      * platforms. But, as long as the determination is made consistently
      * and in coordination with the weavePaths() method, it should work
      * for any platform.
      *
      * @param toCheck The file name which you want to check
      *
      * @return Returns true if the filename appears to be relative
      */
    static bool isRelative(const XMLCh* const toCheck);

    /** Utility to join two paths
      *
      * This must be implemented by the per-platform driver, and should
      * weave the relative path part together with the base part and return
      * a new path that represents this combination.
      *
      * If the relative part turns out to be fully qualified, it will be
      * returned as is. If it is not, then it will be woven onto the
      * passed base path, by removing one path component for each leading
      * "../" (or whatever is the equivalent in the local system) in the
      * relative path.
      *
      * @param basePath The string containing the base path
      * @param relativePath The string containing the relative path
      *
      * @return Returns a string containing the 'woven' path. It should
      * be dynamically allocated and becomes the responsibility of the
      * caller to delete.
      */
    static XMLCh* weavePaths
    (
        const   XMLCh* const    basePath
        , const XMLCh* const    relativePath
    );
    //@}

    /** @name Timing Methods */
    //@{

    /** Gets the system time in milliseconds
      *
      * This must be implemented by the per-platform driver, which should
      * use local services to return the current value of a running
      * millisecond timer. Note that the value returned is only as accurate
      * as the millisecond time of the underyling host system.
      *
      * @return Returns the system time as an unsigned long
      */
    static unsigned long getCurrentMillis();
    //@}

    /** @name Mutex Methods */
    //@{

    /** Closes a mutex handle
      *
      * Each per-platform driver must implement this. Only it knows what
      * the actual content of the passed mutex handle is.
      *
      * @param mtxHandle The mutex handle that you want to close
      */
    static void closeMutex(void* const mtxHandle);

    /** Locks a mutex handle
      *
      * Each per-platform driver must implement this. Only it knows what
      * the actual content of the passed mutex handle is.
      *
      * @param mtxHandle The mutex handle that you want to lock
      */
    static void lockMutex(void* const mtxHandle);

    /** Make a new mutex
      *
      * Each per-platform driver must implement this. Only it knows what
      * the actual content of the passed mutex handle is. The returned
      * handle pointer will be eventually passed to closeMutex() which is
      * also implemented by the platform driver.
      */
    static void* makeMutex();

    /** Unlocks a mutex
      *
      * Each per-platform driver must implement this. Only it knows what
      * the actual content of the passed mutex handle is.
      *
      * Note that, since the underlying system synchronization services
      * are used, Xerces cannot guarantee that lock/unlock operaitons are
      * correctly enforced on a per-thread basis or that incorrect nesting
      * of lock/unlock operations will be caught.
      *
      * @param mtxHandle The mutex handle that you want to unlock
      */
    static void unlockMutex(void* const mtxHandle);

    //@}


    /** @name External Message Support */
    //@{

    /** Loads the message set from among the available domains
      *
      * The returned object must be dynamically allocated and the caller
      * becomes responsible for cleaning it up.
      *
      * @param msgDomain The message domain which you want to load
      */
    static XMLMsgLoader* loadMsgSet(const XMLCh* const msgDomain);

    //@}

    /** @name Miscellaneous synchronization methods */
    //@{

    /** Conditionally updates or returns a single word variable atomically
      *
      * This must be implemented by the per-platform driver. The
      * compareAndSwap subroutine performs an atomic operation which
      * compares the contents of a single word variable with a stored old
      * value. If the values are equal, a new value is stored in the single
      * word variable and TRUE is returned; otherwise, the old value is set
      * to the current value of the single word variable and FALSE is
      * returned.
      *
      * The compareAndSwap subroutine is useful when a word value must be
      * updated only if it has not been changed since it was last read.
      *
      * Note: The word containing the single word variable must be aligned
      * on a full word boundary.
      *
      * @param toFill Specifies the address of the single word variable
      * @param newValue Specifies the new value to be conditionally assigned
      * to the single word variable.
      * @param toCompare Specifies the address of the old value to be checked
      * against (and conditionally updated with) the value of the single word
      * variable.
      *
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

    /** Increments a single word variable atomically.
      *
      * This must be implemented by the per-platform driver. The
      * atomicIncrement subroutine increments one word in a single atomic
      * operation. This operation is useful when a counter variable is shared
      * between several threads or processes. When updating such a counter
      * variable, it is important to make sure that the fetch, update, and
      * store operations occur atomically (are not interruptible).
      *
      * @param location Specifies the address of the word variable to be
      * incremented.
      *
      * @return The function return value is positive if the result of the
      * operation was positive. Zero if the result of the operation was zero.
      * Negative if the result of the operation was negative. Except for the
      * zero case, the value returned may differ from the actual result of
      * the operation - only the sign and zero/nonzero state is guaranteed
      * to be correct.
      */
    static int atomicIncrement(int& location);

    /** Decrements a single word variable atomically.
      *
      * This must be implemented by the per-platform driver. The
      * atomicDecrement subroutine increments one word in a single atomic
      * operation. This operation is useful when a counter variable is shared
      * between several threads or processes. When updating such a counter
      * variable, it is important to make sure that the fetch, update, and
      * store operations occur atomically (are not interruptible).
      *
      * @param location Specifies the address of the word variable to be
      * decremented.
      *
      * @return The function return value is positive if the result of the
      * operation was positive. Zero if the result of the operation was zero.
      * Negative if the result of the operation was negative. Except for the
      * zero case, the value returned may differ from the actual result of the
      * operation - only the sign and zero/nonzero state is guaranteed to be
      * correct.
      */
    static int atomicDecrement(int& location);

    //@}

    /** @name NEL Character Handling  */
    //@{
	/**
      * This function enables the recognition of NEL char as whitespace chars
      * which is disabled by default.
      * It is only called once per process. Once it is set, any subsequent calls
      * will result in exception being thrown.
      *
      * Note: Turning this option on will make the parser non complicant.
      */
    static void recognizeNEL(bool state);

    /**
      * Return the value of fgNEL flag.
      */
    static bool isNELRecognized();
    //@}

    /** @name Strict IANA Encoding Checking */
    //@{
	/**
      * This function enables/disables strict IANA encoding names checking.
      *
      * The strict checking is disabled by default.
      *
      * @param state If true, a strict IANA encoding name check is performed,
      *              otherwise, no checking.
      *
      */
    static void strictIANAEncoding(const bool state);

    /**
      * Returns whether a strict IANA encoding name check is enabled or
      * disabled.
      */
    static bool isStrictIANAEncoding();
    //@}


private :
    /** @name Private static methods */
    //@{

    /** Loads a message set from the available domains
      *
      * @param msgDomain The message domain containing the message to be
      * loaded
      */
    static XMLMsgLoader* loadAMsgSet(const XMLCh* const msgDomain);

    /** Creates a net accessor object.
      *
      * Each per-platform driver must implement this method. However,
      * having a Net Accessor is optional and this method can return a
      * null pointer if remote access via HTTP and FTP URLs is not required.
      *
      * @return An object derived from XMLNetAccessor. It must be dynamically
      *         allocated, since it will be deleted later.
      */
    static XMLNetAccessor* makeNetAccessor();

    /** Creates a Transoding service
      *
      * Each per-platform driver must implement this method and return some
      * derivative of the XMLTransService class. This object serves as the
      * transcoder factory for this process. The object must be dynamically
      * allocated and the caller is responsible for cleaning it up.
      *
      * @return A dynamically allocated object of some class derived from
      *         the XMLTransService class.
      */
    static XMLTransService* makeTransService();

    /** Does initialization for a particular platform
      *
      * Each per-platform driver must implement this to do any low level
      * system initialization required. It <b>cannot</b> use any XML
      * parser or utilities services!
      */
    static void platformInit();

    /** Does termination for a particular platform
      *
      * Each per-platform driver must implement this to do any low level
      * system resource cleanup required. It <b>cannot</b> use any XML
      * parser or utilities services!
      */
    static void platformTerm();

    //@}
};


MakeXMLException(XMLPlatformUtilsException, XMLUTIL_EXPORT)



// ---------------------------------------------------------------------------
//  XMLDeleter: Public Destructor
// ---------------------------------------------------------------------------
inline XMLDeleter::~XMLDeleter()
{
}

// ---------------------------------------------------------------------------
//  XMLDeleter: Hidden constructors and operators
// ---------------------------------------------------------------------------
inline XMLDeleter::XMLDeleter()
{
}

#endif
