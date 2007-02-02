#ifndef DOMXPathException_HEADER_GUARD_
#define DOMXPathException_HEADER_GUARD_

/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <xercesc/dom/DOMException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/**
 * @since DOM Level 3
 */
class CDOM_EXPORT DOMXPathException : public DOMException
{
public:
    // -----------------------------------------------------------------------
    //  Constructors
    // -----------------------------------------------------------------------
    /** @name Constructors */
    //@{
    /**
      * Default constructor for DOMXPathException.
      *
      */
    DOMXPathException();

    /**
      * Constructor which takes an error code and a message.
      *
      * @param code The error code which indicates the exception
      * @param message The string containing the error message
      */
    DOMXPathException( short                 code
                     , short                 messageCode = 0
                     , MemoryManager* const  memoryManager = XMLPlatformUtils::fgMemoryManager
                     );

    /**
      * Copy constructor.
      *
      * @param other The object to be copied.
      */
    DOMXPathException(const DOMXPathException  &other);

    //@}

    // -----------------------------------------------------------------------
    //  Destructors
    // -----------------------------------------------------------------------
    /** @name Destructor. */
    //@{
	 /**
	  * Destructor for DOMXPathException.
	  *
	  */
    virtual ~DOMXPathException();
    //@}

public:

    //@{
    /**
     * ExceptionCode
     * INVALID_EXPRESSION_ERR If the expression has a syntax error or otherwise
     * is not a legal expression according to the rules of the specific 
     * <code>DOMXPathEvaluator</code> or contains specialized extension functions
     * or variables not supported by this implementation.
     * TYPE_ERR If the expression cannot be converted to return the specified type.
     */
	enum ExceptionCode {
		INVALID_EXPRESSION_ERR = 51,
		TYPE_ERR = 52
	};
    //@}

private:
    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DOMXPathException& operator = (const DOMXPathException&);
};

XERCES_CPP_NAMESPACE_END

#endif
