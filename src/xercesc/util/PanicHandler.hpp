/*
 * Copyright 2003,2004 The Apache Software Foundation.
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
 * $Log$
 * Revision 1.9  2005/04/05 18:36:00  cargilld
 * Change platform mutex code to do a panic instead of throwing an exception as the exception code uses mutexes and this can result in infinite recursion.
 *
 * Revision 1.8  2004/10/20 15:18:35  knoaman
 * Allow option of initializing static data in XMLPlatformUtils::Initialize
 *
 * Revision 1.7  2004/09/08 13:56:22  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.6  2003/12/24 17:12:21  cargilld
 * Memory management update.
 *
 * Revision 1.5  2003/12/24 15:24:13  cargilld
 * More updates to memory management so that the static memory manager.
 *
 * Revision 1.4  2003/05/22 18:15:16  neilg
 * The PanicHandler interface should not inherit from XMemory.
 * The reason for this is that the default implementation does not
 * allocate memory dynamically and if such an inheritance relation existed,
 * a user would have to be very careful about installing a memory
 * handler on their own PanicHandler before handing it to the
 * XMLPlatformUtils::Initialize() method, since otherwise
 * the (uninitialized) XMLPlatformUtils::fgMemoryManager would be used
 * upon construction of their PanicHandler implementation.
 *
 * Revision 1.3  2003/05/15 19:04:35  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2003/03/10 16:05:11  peiyongz
 * assignment operator
 *
 * Revision 1.1  2003/03/09 17:06:16  peiyongz
 * PanicHandler
 *
 * $Id$
 *
 */

#ifndef PANICHANDLER_HPP
#define PANICHANDLER_HPP

#include <xercesc/util/XMemory.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/**
  * Receive notification of panic.
  *
  * <p>This is the interface, through which the Xercesc reports
  *    a panic to the application. 
  * </p>
  *
  * <p>Application may implement this interface, instantiate an
  *    object of the derivative, and plug it to Xercesc in the
  *    invocation to XMLPlatformUtils::Initialize(), if it prefers 
  *    to handling panic itself rather than Xercesc doing it.
  * </p>
  *
  */

class XMLUTIL_EXPORT PanicHandler
{
public:

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
        , Panic_AllStaticInitErr
        , Panic_MutexErr
        , PanicReasons_Count
    };
    //@}

protected:

    /** @name hidden Constructors */
    //@{
    /** Default constructor */
    PanicHandler(){};

public:

    /** Destructor */
    virtual ~PanicHandler(){};   
    //@}

    /** @name The virtual panic handler interface */
    //@{
   /**
    * Receive notification of panic
    *
    * This method is called when an unrecoverable error has occurred in the Xerces library.  
    *
    * This method must not return normally, otherwise, the results are undefined. 
    * 
    * Ways of handling this call could include throwing an exception or exiting the process.
    *
    * Once this method has been called, the results of calling any other Xerces API, 
    * or using any existing Xerces objects are undefined.    
    *
    * @param reason The reason of panic
    *
    */
    virtual void panic(const PanicHandler::PanicReasons reason) = 0;
    //@}

    static const char* getPanicReasonString(const PanicHandler::PanicReasons reason);
    
private:

    /* Unimplemented Constructors and operators */
    /* Copy constructor */
    PanicHandler(const PanicHandler&);
    
    /** Assignment operator */
    PanicHandler& operator=(const PanicHandler&);
};

XERCES_CPP_NAMESPACE_END

#endif
