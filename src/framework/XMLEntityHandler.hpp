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
  * Revision 1.2  1999/12/15 19:46:28  roddey
  * Got rid of redundant 'const' on bool return value. Some compilers choke on this
  * and its not useful anyway.
  *
  * Revision 1.1.1.1  1999/11/09 01:08:33  twl
  * Initial checkin
  *
  * Revision 1.2  1999/11/08 20:44:38  rahul
  * Swat for adding in Product name and CVS comment log variable.
  *
  */


#if !defined(XMLENTITYHANDLER_HPP)
#define XMLENTITYHANDLER_HPP

#include <util/XML4CDefs.hpp>

class InputSource;
class XMLBuffer;


//
//  This abstract class is a callback mechanism for the scanner. By creating
//  a derivative of this class and plugging into the scanner, the scanner
//  will call back on the object's methods to entity events.
//
class XMLPARSER_EXPORT XMLEntityHandler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors are hidden, only the virtual destructor is exposed
    // -----------------------------------------------------------------------
    virtual ~XMLEntityHandler()
    {
    }


    // -----------------------------------------------------------------------
    //  The virtual entity handler interface
    // -----------------------------------------------------------------------
    /** @name The pure virtual methods in this interface. */
    //@{

    /**
      * This method get called after the scanner has finished reading from
      * the given input source while processing external entity references.
      *
      * @param inputSource The input source for the entity
      */
    virtual void endInputSource(const InputSource& inputSource) = 0;

    /**
      * This method allows the passes the scanned systemId to the entity
      * handler, thereby giving it a chance to provide any customized
      * handling like resolving relative path names. The scanner first
      * calls this method before calling <code>resolveEntity</code>.
      *
      * @param systemId The system id extracted by the scanner from the
      *                 input source.
      * @param toFill The buffer in which the fully expanded system id needs
      *               to be stored.
      */
    virtual bool expandSystemId
    (
        const   XMLCh* const    systemId
        ,       XMLBuffer&      toFill
    ) = 0;

    /**
      * This method allows the entity handler to reset itself, so that
      * it can be used again.
      */
    virtual void resetEntities() = 0;

    /**
      * This method allows the entity handler to provide customized
      * application specific entity resolution. This method is defined
      * by SAX 1.0 API.
      *
      * @param systemId The system id of the external entity reference. 
      * @param publicId The public id of the external entity reference.
      */
    virtual InputSource* resolveEntity
    (
        const   XMLCh* const    publicId
        , const XMLCh* const    systemId
    ) = 0;

    /**
      * This method will be called before the scanner starts reading
      * from an input source while processing external entity references.
      *
      * @param inputSource The external input source.
      */
    virtual void startInputSource(const InputSource& inputSource) = 0;
    //@}


protected :
    // -----------------------------------------------------------------------
    //  Hidden Constructors
    // -----------------------------------------------------------------------
    XMLEntityHandler()
    {
    }


private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and destructor
    // -----------------------------------------------------------------------
    XMLEntityHandler(const XMLEntityHandler&);
    void operator=(const XMLEntityHandler&);
};

#endif
