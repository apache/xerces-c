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
 * Revision 1.1  1999/11/09 01:08:36  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:44:39  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(XMLPSCANTOKEN_HPP)
#define XMLPSCANTOKEN_HPP

class XMLScanner;

//
//  This simple class is used as a sanity check when the scanner is used to
//  do progressive parsing. It insures that things are not done out of
//  sequence and that sequences of scan calls are made correctly to the
//  right scanner instances.
//
class XMLPARSER_EXPORT XMLPScanToken
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XMLPScanToken();
    XMLPScanToken(const XMLPScanToken& toCopy);
    ~XMLPScanToken();


    // -----------------------------------------------------------------------
    //  Public operators
    // -----------------------------------------------------------------------
    XMLPScanToken& operator=(const XMLPScanToken& toCopy);


protected :
    // -----------------------------------------------------------------------
    //  XMLScanner is our friend, can you say friend? Sure...
    // -----------------------------------------------------------------------
    friend class XMLScanner;


    // -----------------------------------------------------------------------
    //  Hidden methods for use by XMLScanner
    // -----------------------------------------------------------------------
    void set
    (
        const   XMLUInt32   scannerId
        , const XMLUInt32   sequenceId
    );


private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fScannerId
    //      This field is set to the id of the scanner, to catch problems
    //      where a token is gotten from one scanner and passed to another.
    //      Each scanner is assigned an incrementing id.
    //
    //  fSequenceId
    //      In order to avoid problems such as calling scanNext() without
    //      a call to scanFirst() and such, this value is set when scanFirst()
    //      is called and matches this token to the current sequence id of
    //      the scanner.
    // -----------------------------------------------------------------------
    XMLUInt32   fScannerId;
    XMLUInt32   fSequenceId;
};


// ---------------------------------------------------------------------------
//  XMLPScanToken: Constructors and Operators
// ---------------------------------------------------------------------------
inline XMLPScanToken::XMLPScanToken() :

    fScannerId(0)
    , fSequenceId(0)
{
}

inline XMLPScanToken::XMLPScanToken(const XMLPScanToken& toCopy) :

    fScannerId(toCopy.fScannerId)
    , fSequenceId(toCopy.fSequenceId)
{
}

inline XMLPScanToken::~XMLPScanToken()
{
}


// ---------------------------------------------------------------------------
//  XMLPScanToken: Public operators
// ---------------------------------------------------------------------------
inline XMLPScanToken& XMLPScanToken::operator=(const XMLPScanToken& toCopy)
{
    if (this == &toCopy)
        return *this;

    fScannerId = toCopy.fScannerId;
    fSequenceId = toCopy.fSequenceId;

    return *this;
}


// ---------------------------------------------------------------------------
//  XMLPScanToken: Hidden methods
// ---------------------------------------------------------------------------
inline void XMLPScanToken::set( const   XMLUInt32   scannerId
                                , const XMLUInt32   sequenceId)
{
    fScannerId = scannerId;
    fSequenceId = sequenceId;
}


#endif
