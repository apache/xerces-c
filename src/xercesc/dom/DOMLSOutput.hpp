#ifndef DOMLSInput_HEADER_GUARD_
#define DOMLSInput_HEADER_GUARD_

/*
 * Copyright 2002,2004 The Apache Software Foundation.
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


#include <xercesc/util/XercesDefs.hpp>

XERCES_CPP_NAMESPACE_BEGIN


class BinInputStream;


/**
  * This interface represents an output destination for data.
  *
  * Xerces currently doesn't use <code>DOMLSOutput</code> objects
  *
  * @see XMLFormatTarget
  * @since DOM Level 3
  */
class CDOM_EXPORT DOMLSOutput
{
protected:
    // -----------------------------------------------------------------------
    //  Hidden constructors
    // -----------------------------------------------------------------------
    /** @name Hidden constructors */
    //@{    
    DOMLSOutput() {};
    //@}

private:
    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    /** @name Unimplemented constructors and operators */
    //@{
    DOMLSOutput(const DOMLSOutput &);
    DOMLSOutput & operator = (const DOMLSOutput &);
    //@}

public:
    // -----------------------------------------------------------------------
    //  All constructors are hidden, just the destructor is available
    // -----------------------------------------------------------------------
    /** @name Destructor */
    //@{
    /**
     * Destructor
     *
     */
    virtual ~DOMLSOutput() {};
    //@}

    // -----------------------------------------------------------------------
    //  Virtual DOMLSOutput interface
    // -----------------------------------------------------------------------
    /** @name Functions introduced in DOM Level 3 */
    //@{
    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    /**
     * An input source can be set to force the parser to assume a particular
     * encoding for the data that input source reprsents, via the setEncoding()
     * method. This method returns name of the encoding that is to be forced.
     * If the encoding has never been forced, it returns a null pointer.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @return The forced encoding, or null if none was supplied.
     * @see #setEncoding
     * @since DOM Level 3
     */
    virtual const XMLCh* getEncoding() const = 0;

    /**
     * Get the system identifier for this input source.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * <p>If the system ID is a URL, it will be fully resolved.</p>
     *
     * @return The system identifier.
     * @see #setSystemId
     * @since DOM Level 3
     */
    virtual const XMLCh* getSystemId() const = 0;

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    /**
     * Set the encoding which will be required for use with the XML text read
     * via a stream opened by this input source.
     *
     * <p>This is usually not set, allowing the encoding to be sensed in the
     * usual XML way. However, in some cases, the encoding in the file is known
     * to be incorrect because of intermediate transcoding, for instance
     * encapsulation within a MIME document.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @param encodingStr The name of the encoding to force.
     * @since DOM Level 3
     */
    virtual void setEncoding(const XMLCh* const encodingStr) = 0;

    /**
     * Set the system identifier for this input source.
     *
     * <p>The system id is always required. The public id may be used to map
     * to another system id, but the system id must always be present as a fall
     * back.</p>
     *
     * <p>If the system ID is a URL, it must be fully resolved.</p>
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @param systemId The system identifier as a string.
     * @see #getSystemId
     * @since DOM Level 3
     */
    virtual void setSystemId(const XMLCh* const systemId) = 0;
    //@}

    // -----------------------------------------------------------------------
    //  Non-standard Extension
    // -----------------------------------------------------------------------
    /** @name Non-standard Extension */
    //@{

    /**
     * Makes the byte stream for this input source.
     *
     * <p>The derived class must create and return a binary input stream of an
     * appropriate type for its kind of data source. The returned stream must
     * be dynamically allocated and becomes the parser's property.
     * </p>
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @see BinInputStream
     */
    virtual BinInputStream* makeStream() const = 0;

    /**
     * Called to indicate that this DOMLSOutput is no longer in use
     * and that the implementation may relinquish any resources associated with it.
     *
     * Access to a released object will lead to unexpected result.
     */
    virtual void              release() = 0;
    //@}
};


XERCES_CPP_NAMESPACE_END

#endif
