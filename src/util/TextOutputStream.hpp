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
 * Revision 1.1  1999/11/09 01:05:14  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:45:15  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(OUTPUTSTREAM_HPP)
#define OUTPUTSTREAM_HPP

#include <util/XML4CDefs.hpp>

class StreamJanitor;

class XMLUTIL_EXPORT TextOutputStream
{
public :
    // -----------------------------------------------------------------------
    //  Public types
    // -----------------------------------------------------------------------
    enum Radices
    {
        bin         = 2
        , decimal   = 10
        , hex       = 16
    };

    enum SpecialValues
    {
        EndLine
    };


    // -----------------------------------------------------------------------
    //  Public virtual destructor
    // -----------------------------------------------------------------------
    virtual ~TextOutputStream();


    // -----------------------------------------------------------------------
    //  Formatting operators
    // -----------------------------------------------------------------------
    TextOutputStream& operator<<
    (
        const   XMLCh* const        toWrite
    );

    TextOutputStream& operator<<
    (
        const   XMLCh               toWrite
    );

    TextOutputStream& operator<<
    (
        const   char* const         toWrite
    );

    TextOutputStream& operator<<
    (
        const   char                toWrite
    );

    TextOutputStream& operator<<
    (
        const   unsigned int        toWrite
    );

    TextOutputStream& operator<<
    (
        const   long                toWrite
    );

    TextOutputStream& operator<<
    (
        const   unsigned long       toWrite
    );

    TextOutputStream& operator<<
    (
        const   double&             toWrite
    );

    TextOutputStream& operator<<
    (
        const   Radices             newRadix
    );

    TextOutputStream& operator<<
    (
        const   SpecialValues       specialVal
    );


    // -----------------------------------------------------------------------
    //  Virtual interface to be provided by the derived class
    // -----------------------------------------------------------------------
    virtual void flush() = 0;
    virtual void write(const XMLCh* const toWrite) = 0;
    virtual void write(const char* const toWrite) = 0;


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    Radices getRadix() const
    {
        return fRadix;
    }


protected :
    // -----------------------------------------------------------------------
    //  Declare our friends
    // -----------------------------------------------------------------------
    friend class StreamJanitor;


    // -----------------------------------------------------------------------
    //  Hidden Constructors for derived classes only
    // -----------------------------------------------------------------------
    TextOutputStream();


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    TextOutputStream(const TextOutputStream&);
    void operator=(const TextOutputStream&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fRadix
    //      The current radix to use when formatting values.
    // -----------------------------------------------------------------------
    Radices     fRadix;
};


class XMLUTIL_EXPORT StreamJanitor
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    StreamJanitor(TextOutputStream* const toSanitize);
    ~StreamJanitor();

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    StreamJanitor(const StreamJanitor&);
    void operator=(const StreamJanitor&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fRadix
    //      The saved radix that will be replaced.
    //
    //  fStream
    //      This is the stream that we are sanitizing.
    // -----------------------------------------------------------------------
    TextOutputStream::Radices   fRadix;
    TextOutputStream*           fStream;
};


// ---------------------------------------------------------------------------
//  Convenenience defines for some of the special characters
// ---------------------------------------------------------------------------
#define EndLn           TextOutputStream::EndLine
#define BinRadix        TextOutputStream::bin
#define DecimalRadix    TextOutputStream::decimal
#define HexRadix        TextOutputStream::hex

#endif
