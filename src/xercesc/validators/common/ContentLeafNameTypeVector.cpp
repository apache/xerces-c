/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.1  2002/02/01 22:22:38  peiyongz
 * Initial revision
 *
 * Revision 1.3  2001/05/11 13:27:17  tng
 * Copyright update.
 *
 * Revision 1.2  2001/04/19 18:17:28  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.1  2001/02/27 14:48:49  tng
 * Schema: Add CMAny and ContentLeafNameTypeVector, by Pei Yong Zhang
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/validators/common/ContentLeafNameTypeVector.hpp>

// ---------------------------------------------------------------------------
//  ContentLeafNameTypeVector: Constructors and Destructor
// ---------------------------------------------------------------------------
ContentLeafNameTypeVector::ContentLeafNameTypeVector()
: fLeafNames(0)
, fLeafTypes(0)
, fLeafCount(0)
{
}

ContentLeafNameTypeVector::ContentLeafNameTypeVector
       (    QName** const                      names
          , ContentSpecNode::NodeTypes* const  types
          , const unsigned int                       count
       )
: fLeafNames(0)
, fLeafTypes(0)
, fLeafCount(0)
{
    setValues(names, types, count);
}

/***
copy ctor
***/
ContentLeafNameTypeVector::ContentLeafNameTypeVector
      (const ContentLeafNameTypeVector& toCopy)
	  :fLeafNames(0)
	  ,fLeafTypes(0)
	  ,fLeafCount(0)
{
    fLeafCount=toCopy.getLeafCount();
    init(fLeafCount);

    for (unsigned int i=0; i<this->fLeafCount; i++)
    {
        fLeafNames[i] = toCopy.getLeafNameAt(i);
        fLeafTypes[i] = toCopy.getLeafTypeAt(i);
    }
}

ContentLeafNameTypeVector::~ContentLeafNameTypeVector()
{
    cleanUp();
}

// ---------------------------------------------------------------------------
//  ContentSpecType: Setter methods
// ---------------------------------------------------------------------------
void ContentLeafNameTypeVector::setValues
    (
         QName** const                      names
       , ContentSpecNode::NodeTypes* const  types
       , const unsigned int                 count
    )
{
    cleanUp();
    init(count);

    for (unsigned int i=0; i<count; i++)
    {
        fLeafNames[i] = names[i];
        fLeafTypes[i] = types[i];
    }
}

// ---------------------------------------------------------------------------
//  ContentLeafNameTypeVector: Getter methods
// ---------------------------------------------------------------------------
QName* ContentLeafNameTypeVector::getLeafNameAt(const unsigned int pos) const
{
    if (pos >= fLeafCount)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex);

    return fLeafNames[pos];
}

const ContentSpecNode::NodeTypes ContentLeafNameTypeVector::getLeafTypeAt
       (const unsigned int pos) const
{
    if (pos >= fLeafCount)
        ThrowXML(ArrayIndexOutOfBoundsException, XMLExcepts::Vector_BadIndex);

	return fLeafTypes[pos];
}

const unsigned int ContentLeafNameTypeVector::getLeafCount() const
{
	return fLeafCount;
}
