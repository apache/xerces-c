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


#if !defined(CONTENTLEAFNAMETYPEVECTOR_HPP)
#define CONTENTLEAFNAMETYPEVECTOR_HPP

#include <util/XercesDefs.hpp>
#include <util/ValueVectorOf.hpp>
#include <validators/common/ContentSpecNode.hpp>

class ContentLeafNameTypeVector
{
public :
    // -----------------------------------------------------------------------
    //  Class specific types
    // -----------------------------------------------------------------------


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    ContentLeafNameTypeVector();
    ContentLeafNameTypeVector
    (
        QName** const                      qName
      , ContentSpecNode::NodeTypes* const  types
      , const unsigned int                       count
    );

    ~ContentLeafNameTypeVector();

    ContentLeafNameTypeVector(const ContentLeafNameTypeVector&);

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    QName* getLeafNameAt(const unsigned int pos) const;

    const ContentSpecNode::NodeTypes getLeafTypeAt(const unsigned int pos) const;
    const unsigned int getLeafCount() const;

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setValues
    (
        QName** const                      qName
      , ContentSpecNode::NodeTypes* const  types
      , const unsigned int                       count
    );

    // -----------------------------------------------------------------------
    //  Miscellaneous
    // -----------------------------------------------------------------------

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    void operator=(const ContentLeafNameTypeVector&);

    // -----------------------------------------------------------------------
    //  helper methods
    // -----------------------------------------------------------------------
    void cleanUp();
    void init(const unsigned int);

    // -----------------------------------------------------------------------
    //  Private Data Members
    //
    // -----------------------------------------------------------------------
    QName**                       fLeafNames;
    ContentSpecNode::NodeTypes   *fLeafTypes;
    unsigned int                  fLeafCount;
};

inline void ContentLeafNameTypeVector::cleanUp()
{
	delete [] fLeafNames;
	delete [] fLeafTypes;
}

inline void ContentLeafNameTypeVector::init(const unsigned int size)
{
	fLeafNames = new QName*[size];
	fLeafTypes = new ContentSpecNode::NodeTypes [size];
}

#endif
