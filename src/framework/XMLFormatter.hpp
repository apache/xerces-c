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
 * Revision 1.4  2000/04/06 23:50:38  roddey
 * Now the low level formatter handles doing char refs for
 * unrepresentable chars (in addition to the replacement char style
 * already done.)
 *
 * Revision 1.3  2000/04/06 19:09:21  roddey
 * Some more improvements to output formatting. Now it will correctly
 * handle doing the 'replacement char' style of dealing with chars
 * that are unrepresentable.
 *
 * Revision 1.2  2000/04/05 00:20:16  roddey
 * More updates for the low level formatted output support
 *
 * Revision 1.1  2000/03/28 19:43:17  roddey
 * Fixes for signed/unsigned warnings. New work for two way transcoding
 * stuff.
 *
 */

#if !defined(XMLFORMATTER_HPP)
#define XMLFORMATTER_HPP

#include <util/XercesDefs.hpp>

class XMLFormatTarget;
class XMLTranscoder;

/**
 *  This class provides the basic formatting capabilities that are required
 *  to turn the Unicode based XML data from the parsers into a form that can
 *  be used on non-Unicode based systems, i.e. into local or generic text
 *  encodings.
 *
 *  A number of flags are provided to control whether various optional
 *  formatting operations are performed.
 */
class XMLPARSER_EXPORT XMLFormatter
{
public:
    // -----------------------------------------------------------------------
    //  Class types
    // -----------------------------------------------------------------------
    enum EscapeFlags
    {
        NoEscapes
        , StdEscapes
        , AttrEscapes
        , CharEscapes

        // Special values, don't use directly
        , EscapeFlags_Count
        , DefaultEscape     = 999
    };

    enum UnRepFlags
    {
        UnRep_Fail
        , UnRep_CharRef
        , UnRep_Replace

        , DefaultUnRep      = 999
    };


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    XMLFormatter
    (
        const   XMLCh* const            outEncoding
        ,       XMLFormatTarget* const  target
        , const EscapeFlags             escapeFlags = NoEscapes
        , const UnRepFlags              unrepFlags = UnRep_Fail
    );

    XMLFormatter
    (
        const   char* const             outEncoding
        ,       XMLFormatTarget* const  target
        , const EscapeFlags             escapeFlags = NoEscapes
        , const UnRepFlags              unrepFlags = UnRep_Fail
    );

    ~XMLFormatter();


    // -----------------------------------------------------------------------
    //  Formatting methods
    // -----------------------------------------------------------------------
    void formatBuf
    (
        const   XMLCh* const    toFormat
        , const unsigned int    count
        , const EscapeFlags     escapeFlags = DefaultEscape
        , const UnRepFlags      unrepFlags = DefaultUnRep
    );

    XMLFormatter& operator<<
    (
        const   XMLCh* const    toFormat
    );

    XMLFormatter& operator<<
    (
        const   XMLCh           toFormat
    );


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    const XMLCh* getEncodingName() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setEscapeFlags
    (
        const   EscapeFlags     newFlags
    );

    void setUnRepFlags
    (
        const   UnRepFlags      newFlags
    );

    XMLFormatter& operator<<
    (
        const   EscapeFlags     newFlags
    );

    XMLFormatter& operator<<
    (
        const   UnRepFlags      newFlags
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XMLFormatter();
    XMLFormatter(const XMLFormatter&);
    void operator=(const XMLFormatter&);


    // -----------------------------------------------------------------------
    //  Private class constants
    // -----------------------------------------------------------------------
    enum Constants
    {
        kTmpBufSize     = 8192
    };


    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    const XMLByte* getAposRef();
    const XMLByte* getAmpRef();
    const XMLByte* getGTRef();
    const XMLByte* getLTRef();
    const XMLByte* getQuoteRef();

    void specialFormat
    (
        const   XMLCh* const    toFormat
        , const unsigned int    count
        , const EscapeFlags     escapeFlags
    );


    // -----------------------------------------------------------------------
    //  Private, non-virtual methods
    //
    //  fEscapeFlags
    //      The escape flags we were told to use in formatting. These are
    //      defaults set in the ctor, which can be overridden on a particular
    //      call.
    //
    //  fOutEncoding
    //      This the name of the output encoding. Saved mainly for meaningful
    //      error messages.
    //
    //  fRef
    //  fTarget
    //      This is the target object for the formatting operation.
    //
    //  fUnRepFlags
    //      The unrepresentable flags that indicate how to react when a
    //      character cannot be represented in the target encoding.
    //
    //  fXCoder
    //      This the transcoder that we will use. It is created using the
    //      encoding name we were told to use.
    //
    //  fAposRef
    //  fAmpRef
    //  fGTRef
    //  fLTRef
    //  fQuoteRef
    //      These are character refs for the standard char refs, in the
    //      output encoding. They are faulted in as required, by transcoding
    //      them from fixed Unicode versions.
    // -----------------------------------------------------------------------
    EscapeFlags                 fEscapeFlags;
    XMLCh*                      fOutEncoding;
    XMLFormatTarget*            fTarget;
    UnRepFlags                  fUnRepFlags;
    XMLTranscoder*              fXCoder;
    XMLByte                     fTmpBuf[kTmpBufSize + 1];

    XMLByte*                    fAposRef;
    XMLByte*                    fAmpRef;
    XMLByte*                    fGTRef;
    XMLByte*                    fLTRef;
    XMLByte*                    fQuoteRef;
};


class XMLPARSER_EXPORT XMLFormatTarget
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    virtual ~XMLFormatTarget() {}


    // -----------------------------------------------------------------------
    //  Virtual interface
    // -----------------------------------------------------------------------
    virtual void writeChars
    (
        const   XMLByte* const  toWrite
    ) = 0;


protected :
    // -----------------------------------------------------------------------
    //  Hidden constructors and operators
    // -----------------------------------------------------------------------
    XMLFormatTarget() {}
    XMLFormatTarget(const XMLFormatTarget&) {}
    void operator=(const XMLFormatTarget&) {}
};


// ---------------------------------------------------------------------------
//  XMLFormatter: Getter methods
// ---------------------------------------------------------------------------
inline const XMLCh* XMLFormatter::getEncodingName() const
{
    return fOutEncoding;
}


// ---------------------------------------------------------------------------
//  XMLFormatter: Setter methods
// ---------------------------------------------------------------------------
inline void XMLFormatter::setEscapeFlags(const EscapeFlags newFlags)
{
    fEscapeFlags = newFlags;
}

inline void XMLFormatter::setUnRepFlags(const UnRepFlags newFlags)
{
    fUnRepFlags = newFlags;
}


inline XMLFormatter& XMLFormatter::operator<<(const EscapeFlags newFlags)
{
    fEscapeFlags = newFlags;
    return *this;
}

inline XMLFormatter& XMLFormatter::operator<<(const UnRepFlags newFlags)
{
    fUnRepFlags = newFlags;
    return *this;
}


#endif
