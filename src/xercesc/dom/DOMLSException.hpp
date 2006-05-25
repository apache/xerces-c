#ifndef DOMLSException_HEADER_GUARD_
#define DOMLSException_HEADER_GUARD_

/*
 * Copyright 2001-2002,2004 The Apache Software Foundation.
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
 */

#include <xercesc/dom/DOMException.hpp>

XERCES_CPP_NAMESPACE_BEGIN


/**
 * Parser or write operations may throw an LSException if the processing is stopped. 
 * The processing can be stopped due to a <code>DOMError</code> with a severity of 
 * DOMError::DOM_SEVERITY_FATAL_ERROR or a non recovered DOMError::DOM_SEVERITY_ERROR, 
 * or if <code>DOMErrorHandler::handleError()</code> returned <code>false</code>.
 * <p><b>Note</b>: As suggested in the definition of the constants in the <code>DOMError</code>
 * interface, a DOM implementation may choose to continue after a fatal error, but the 
 * resulting DOM tree is then implementation dependent.
 * <p>See also the <a href='http://www.w3.org/TR/2004/REC-DOM-Level-3-LS-20040407/DOM3-LS.html'>
 * Document Object Model (DOM) Level 3 Load and Save Specification</a>.
 * @since DOM Level 3
 */

class MemoryManager;

class CDOM_EXPORT DOMLSException : public DOMException {
public:
    // -----------------------------------------------------------------------
    //  Class Types
    // -----------------------------------------------------------------------
    /** @name Public Contants */
    //@{
    /**
     * ExceptionCode
     *
     * <p><code>PARSE_ERR:</code>
     * If an attempt was made to load a document, or an XML Fragment, using DOMLSParser 
     * and the processing has been stopped.</p>
     *
     * <p><code>SERIALIZE_ERR:</code>
     * If an attempt was made to serialize a Node using LSSerializer and the processing 
     * has been stopped.</p>
     *
     * @since DOM Level 3
     */
    enum LSExceptionCode {
         PARSE_ERR        = 81,
         SERIALIZE_ERR    = 82
        };
    //@}

    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    /** @name Constructors */
    //@{
    /**
      * Default constructor for DOMLSException.
      *
      */
    DOMLSException();

    /**
      * Constructor which takes an error code and a message.
      *
      * @param code           The error code which indicates the exception
      * @param message        The string containing the error message
      * @param memoryManager  The memory manager used to (de)allocate memory
      */
    DOMLSException(    LSExceptionCode              code
                     ,       short                  messageCode
                     ,       MemoryManager* const   memoryManager);

    /**
      * Copy constructor.
      *
      * @param other The object to be copied.
      */
    DOMLSException(const DOMLSException &other);

    //@}

    // -----------------------------------------------------------------------
    //  Destructors
    // -----------------------------------------------------------------------
    /** @name Destructor. */
    //@{
	 /**
	  * Destructor for DOMLSException.
	  *
	  */
    virtual ~DOMLSException();
    //@}

    // -----------------------------------------------------------------------
    //  Getter
    // -----------------------------------------------------------------------
    inline const XMLCh* getMessage()    const;

    // -----------------------------------------------------------------------
    //  Class Types
    // -----------------------------------------------------------------------
    /** @name Public variables */
    //@{
	 /**
	  * A code value, from the set defined by the LSExceptionCode enum,
      * indicating the type of error that occured.
      */
    LSExceptionCode   code;

	 /**
	  * A string value.  Applications may use this field to hold an error
      * message.  The field value is not set by the DOM implementation,
      * meaning that the string will be empty when an exception is first
      * thrown.
	  */
    const XMLCh *msg;
    //@}

protected:

    MemoryManager*  fMemoryManager;

private:

	 /**
	  * A boolean value.  
      *   If the message is provided by the applications, it is not adopted.
      *   If the message is resolved by the DOM implementation, it is owned.
	  */
    bool            fMsgOwned;

private:
    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------    
    DOMLSException & operator = (const DOMLSException &);
};

inline const XMLCh* DOMLSException::getMessage() const
{
    return msg;
}

XERCES_CPP_NAMESPACE_END

#endif

