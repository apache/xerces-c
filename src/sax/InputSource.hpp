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
 * Revision 1.9  2000/03/02 19:54:35  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.8  2000/02/24 20:12:55  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.7  2000/02/12 03:42:21  rahulj
 * Fixed DOC++ documentation formatting errors.
 *
 * Revision 1.6  2000/02/12 03:31:55  rahulj
 * Removed duplicate CVS Log entries.
 *
 * Revision 1.5  2000/02/12 01:27:19  aruna1
 * Documentation updated
 *
 * Revision 1.4  2000/02/09 02:15:28  abagchi
 * Documented destructor
 *
 * Revision 1.3  2000/02/06 07:47:58  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/01/12 00:15:39  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:46  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:01  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#ifndef INPUTSOURCE_HPP
#define INPUTSOURCE_HPP

#include <util/XercesDefs.hpp>

class BinInputStream;


/**
  * A single input source for an XML entity.
  *
  * <p>This class encapsulates information about an input source in a
  * single object, which may include a public identifier or a system
  * identifier</p>
  *
  * <p>There are two places that the application will deliver this input
  * source to the parser: as the argument to the Parser::parse method, or as
  * the return value of the EntityResolver::resolveEntity method.</p>
  *
  * <p>InputSource is never used directly, but is the base class for a number
  * of derived classes for particular types of input sources. Derivatives are
  * provided (in the internal/ directory) for URL input sources, memory buffer
  * input sources, and so on.</p>
  *
  * <p>When it is time to parse the input described by an input source, it
  * will be asked to create a binary stream for that source. That stream will
  * be used to input the data of the source. The derived class provides the
  * implementation of the makeStream() method, and provides a type of stream
  * of the correct type for the input source it represents.
  *
  * <p>An InputSource object belongs to the application: the parser never
  * modifies them in any way. They are always passed by const reference so
  * the parser will make a copy of any input sources that it must keep 
  * around beyond the call.</p>
  *
  * @see Parser#parse
  * @see EntityResolver#resolveEntity
  */
class SAX_EXPORT InputSource
{
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
    virtual ~InputSource();
    //@}


    // -----------------------------------------------------------------------
    /** @name Virtual input source interface */
    //@{
  /**
    * Makes the byte stream for this input source.
    *
    * <p>The derived class must create and return a binary input stream of an
    * appropriate type for its kind of data source. The returned stream must
    * be dynamically allocated and becomes the parser's property.
    * </p>
    *
    * @see BinInputStream
    */
    virtual BinInputStream* makeStream() const = 0;

    //@}


    // -----------------------------------------------------------------------
    /** @name Getter methods */
    //@{
  /**
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
    * Get the public identifier for this input source.
    *
    * @return The public identifier, or null if none was supplied.
    * @see #setPublicId
    */
    const XMLCh* getPublicId() const;


  /**
    * Get the system identifier for this input source.
    *
    * <p>If the system ID is a URL, it will be fully resolved.</p>
    *
    * @return The system identifier.
    * @see #setSystemId
    */
    const XMLCh* getSystemId() const;

    //@}


    // -----------------------------------------------------------------------
    /** @name Setter methods */
    //@{

  /**
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
    * Set the system identifier for this input source.
    *
    * <p>Set the system identifier for this input source.
    *
    * </p>The system id is always required. The public id may be used to map
    * to another system id, but the system id must always be present as a fall
    * back.
    *
    * <p>If the system ID is a URL, it must be fully resolved.</p>
    *
    * @param systemId The system identifier as a string.
    * @see #getSystemId
    * @see Locator#getSystemId
    * @see SAXParseException#getSystemId
    */
    void setSystemId(const XMLCh* const systemId);

    //@}


protected :
    // -----------------------------------------------------------------------
    //  Hidden constructors
    // -----------------------------------------------------------------------
    /** @name Constructors and Destructor */
    //@{
    /** Default constructor */
    InputSource();

    /** Constructor with a system identifier as XMLCh type.
      * @param systemId The system identifier (URI).
      */
    InputSource(const XMLCh* const systemId);

    /** Constructor with a system and public identifiers 
      * @param systemId The system identifier (URI).
      * @param publicId The public identifier as in the entity definition.
      */
    InputSource
    (
        const   XMLCh* const    systemId
        , const XMLCh* const    publicId
    );
    
    /** Constructor witha system identifier as string 
      * @param systemId The system identifier (URI).
      */
    InputSource(const char* const systemId);
    
    /** Constructor witha system and public identifiers. Both as string 
      * @param systemId The system identifier (URI).
      * @param publicId The public identifier as in the entity definition.
      */
    InputSource
    (
        const   char* const systemId
        , const char* const publicId
    );
    
    //@}


    


private:
    // -----------------------------------------------------------------------    
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------    
    InputSource(const InputSource&);
    void operator=(const InputSource&);


    // -----------------------------------------------------------------------    
    //  Private data members
    //
    //  fEncoding
    //      This is the encoding to use. Usually this is null, which means
    //      to use the information found in the file itself. But, if set,
    //      this encoding will be used without question.
    //
    //  fPublicId
    //      This is the optional public id for the input source. It can be
    //      null if none is desired.
    //
    //  fSystemId
    //      This is the system id for the input source. This is what is
    //      actually used to open the source.
    // -----------------------------------------------------------------------    
    XMLCh*  fEncoding;
    XMLCh*  fPublicId;
    XMLCh*  fSystemId;
};


// ---------------------------------------------------------------------------
//  InputSource: Getter methods
// ---------------------------------------------------------------------------
inline const XMLCh* InputSource::getEncoding() const 
{
    return fEncoding;
}

inline const XMLCh* InputSource::getPublicId() const 
{
    return fPublicId;
}

inline const XMLCh* InputSource::getSystemId() const 
{
    return fSystemId;
}

#endif
