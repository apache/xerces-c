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


#ifndef WRAPPER4INPUTSOURCE_HPP
#define WRAPPER4INPUTSOURCE_HPP

#include <xercesc/dom/DOMInputSource.hpp>

class InputSource;


/**
  * Wrap a SAX InputSource object to a DOM InputSource.
  */
class XMLPARSER_EXPORT Wrapper4InputSource: public DOMInputSource
{
public:
    /** @name Constructors and Destructor */
    //@{

  /**
    * Constructor
    */
    Wrapper4InputSource(InputSource* const inputSource);

  /**
    * Destructor
    *
    */
    virtual ~Wrapper4InputSource();
    //@}


    // -----------------------------------------------------------------------
    /** @name Virtual input source interface */
    //@{
  /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Makes the byte stream for this input source.
    *
    * <p>The derived class must create and return a binary input stream of an
    * appropriate type for its kind of data source. The returned stream must
    * be dynamically allocated and becomes the parser's property.
    * </p>
    *
    * @see BinInputStream
    */
    BinInputStream* makeStream() const;

    //@}

    // -----------------------------------------------------------------------
    /** @name Getter methods */
    //@{
  /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * An input source can be set to force the parser to assume a particular
    * encoding for the data that input source reprsents, via the setEncoding()
    * method. This method returns name of the encoding that is to be forced.
    * If the encoding has never been forced, it returns a null pointer.
    *
    * @return The forced encoding, or null if none was supplied.
    * @see #setEncoding
    */
    const XMLCh* getEncoding() const;


  /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Get the public identifier for this input source.
    *
    * @return The public identifier, or null if none was supplied.
    * @see #setPublicId
    */
    const XMLCh* getPublicId() const;


  /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Get the system identifier for this input source.
    *
    * <p>If the system ID is a URL, it will be fully resolved.</p>
    *
    * @return The system identifier.
    * @see #setSystemId
    */
    const XMLCh* getSystemId() const;

   /**
     * Get the base URI to be used for resolving relative URIs to absolute
     * URIs. If the baseURI is itself a relative URI, the behavior is
     * implementation dependent.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @return The base URI.
     * @see #setBaseURI
     * @since DOM Level 3
     */
    const XMLCh* getBaseURI() const;

 /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Get the flag that indicates if the parser should issue fatal error if this input source
    * is not found.
    *
    * @return True if the parser should issue fatal error if this input source is not found.
    *         False if the parser issue warning message instead.
    * @see #setIssueFatalErrorIfNotFound
    */
    const bool getIssueFatalErrorIfNotFound() const;

    //@}


    // -----------------------------------------------------------------------
    /** @name Setter methods */
    //@{

  /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Set the encoding which will be required for use with the XML text read
    * via a stream opened by this input source.
    *
    * <p>This is usually not set, allowing the encoding to be sensed in the
    * usual XML way. However, in some cases, the encoding in the file is known
    * to be incorrect because of intermediate transcoding, for instance
    * encapsulation within a MIME document.
    *
    * @param encodingStr The name of the encoding to force.
    */
    void setEncoding(const XMLCh* const encodingStr);


  /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Set the public identifier for this input source.
    *
    * <p>The public identifier is always optional: if the application writer
    * includes one, it will be provided as part of the location information.</p>
    *
    * @param publicId The public identifier as a string.
    * @see Locator#getPublicId
    * @see SAXParseException#getPublicId
    * @see #getPublicId
    */
    void setPublicId(const XMLCh* const publicId);

  /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Set the system identifier for this input source.
    *
    * <p>The system id is always required. The public id may be used to map
    * to another system id, but the system id must always be present as a fall
    * back.</p>
    *
    * <p>If the system ID is a URL, it must be fully resolved.</p>
    *
    * @param systemId The system identifier as a string.
    * @see #getSystemId
    * @see Locator#getSystemId
    * @see SAXParseException#getSystemId
    */
    void setSystemId(const XMLCh* const systemId);

   /**
     * Set the base URI to be used for resolving relative URIs to absolute
     * URIs. If the baseURI is itself a relative URI, the behavior is
     * implementation dependent.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @param baseURI The base URI.
     * @see #getBaseURI
     * @since DOM Level 3
     */
    void setBaseURI(const XMLCh* const baseURI);

  /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Indicates if the parser should issue fatal error if this input source
    * is not found.  If set to false, the parser issue warning message instead.
    *
    * @param  flag True if the parser should issue fatal error if this input source is not found.
    *               If set to false, the parser issue warning message instead.  (Default: true)
    *
    * @see #getIssueFatalErrorIfNotFound
    */
    void setIssueFatalErrorIfNotFound(const bool flag);

    /**
     * Called to indicate that this DOMInputSource is no longer in use
     * and that the implementation may relinquish any resources associated with it.
     *
     * Access to a released object will lead to unexpected result.
     */
    void              release();

  /**
    * <p><b>"Experimental - subject to change"</b></p>
    *
    * Indicates if the parser should own the input source.
    * If set to true, the parser will delete the input source.
    *
    * @param flag if the parser should delete the input source or not.
    *             (Default: true)
    *
    */
    void setAdoptInputSource(const bool flag);

    //@}


private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    Wrapper4InputSource(const Wrapper4InputSource&);
    void operator=(const Wrapper4InputSource&);

    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    bool         fAdoptInputSource;
    InputSource* fInputSource;
};


// ---------------------------------------------------------------------------
//  Wrapper4InputSource: Getter methods
// ---------------------------------------------------------------------------
inline const XMLCh* Wrapper4InputSource::getBaseURI() const
{
    return 0; // REVISIT - should we return an empty string?
}

// ---------------------------------------------------------------------------
//  Wrapper4InputSource: Setter methods
// ---------------------------------------------------------------------------
inline void Wrapper4InputSource::setAdoptInputSource(const bool flag)
{
    fAdoptInputSource = flag;
}

inline void Wrapper4InputSource::setBaseURI(const XMLCh* const baseURI)
{
}

#endif
