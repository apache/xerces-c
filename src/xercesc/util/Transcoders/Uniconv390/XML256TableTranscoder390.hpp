/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2004 The Apache Software Foundation.  All rights
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
 * Revision 1.2  2004/04/22 22:46:46  neilg
 * not all 390 processors support the new transcoding instructions; this patch makes Xerces work there as well.  Thanks to Steve Dulin
 *
 * Revision 1.1  2004/02/06 15:02:11  cargilld
 * Intrinsic transcoding support for 390.
 *
 */

#ifndef XML256TABLETRANSCODER390_HPP
#define XML256TABLETRANSCODER390_HPP

#include <xercesc/util/TransService.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
//  This class implements the functionality of a common type of transcoder
//  for an 8 bit, single byte encoding based on a set of 'to' and 'from'
//  translation tables. Actual derived classes are trivial and just have to
//  provide us with pointers to their tables and we do all the work.
//
class XMLUTIL_EXPORT XML256TableTranscoder390 : public XMLTranscoder
{
public :
    // -----------------------------------------------------------------------
    //  Public constructors and destructor
    // -----------------------------------------------------------------------
    virtual ~XML256TableTranscoder390();


    // -----------------------------------------------------------------------
    //  The virtual transcoding interface
    // -----------------------------------------------------------------------
    virtual unsigned int transcodeFrom
    (
        const   XMLByte* const          srcData
        , const unsigned int            srcCount
        ,       XMLCh* const            toFill
        , const unsigned int            maxChars
        ,       unsigned int&           bytesEaten
        ,       unsigned char* const    charSizes
    );

    virtual unsigned int transcodeTo
    (
        const   XMLCh* const    srcData
        , const unsigned int    srcCount
        ,       XMLByte* const  toFill
        , const unsigned int    maxBytes
        ,       unsigned int&   charsEaten
        , const UnRepOpts       options
    );

    virtual bool canTranscodeTo
    (
        const   unsigned int    toCheck
    )   const;


protected :
    // -----------------------------------------------------------------------
    //  Hidden constructors
    // -----------------------------------------------------------------------
    XML256TableTranscoder390
    (
        const   XMLCh* const                        encodingName
        , const unsigned int                        blockSize
        , const XMLCh* const                        fromTable
        , const XMLTransService::TransRec* const    toTable
        , const unsigned int                        toTableSize
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );


    // -----------------------------------------------------------------------
    //  Protected helper methods
    // -----------------------------------------------------------------------
    XMLByte xlatOneTo
    (
        const   XMLCh       toXlat
    )   const;


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XML256TableTranscoder390();
    XML256TableTranscoder390(const XML256TableTranscoder390&);
    XML256TableTranscoder390& operator=(const XML256TableTranscoder390&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fFromTable
    //      This is the 'from' table that we were given during construction.
    //      It is a 256 entry table of XMLCh chars. Each entry is the
    //      Unicode code point for the external encoding point of that value.
    //      So fFromTable[N] is the Unicode translation of code point N of
    //      the source encoding.
    //
    //      We don't own this table, we just refer to it. It is assumed that
    //      the table is static, for performance reasons.
    //
    //  fToSize
    //      The 'to' table is variable sized. This indicates how many records
    //      are in it.
    //
    //  fToTable
    //      This is a variable sized table of TransRec structures. It must
    //      be sorted by the intCh field, i.e. the XMLCh field. It is searched
    //      binarily to find the record for a particular Unicode char. Then
    //      that record's extch field is the translation record.
    //
    //      We don't own this table, we just refer to it. It is assumed that
    //      the table is static, for performance reasons.
    //
    //      NOTE: There may be dups of the extCh field, since there might be
    //      multiple Unicode code points which map to the same external code
    //      point. Normally this won't happen, since the parser assumes that
    //      internalization is normalized, but we have to be prepared to do
    //      the right thing if some client code gives us non-normalized data
    //      itself.
    // -----------------------------------------------------------------------
    const XMLCh*                        fFromTable;
    unsigned int                        fToSize;
    const XMLTransService::TransRec*    fToTable;
};

XERCES_CPP_NAMESPACE_END

#endif
