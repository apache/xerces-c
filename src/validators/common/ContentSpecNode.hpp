/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
 * Revision 1.13  2001/08/22 16:04:07  tng
 * ContentSpecNode copy constructor should copy Min and Max as well.
 *
 * Revision 1.12  2001/08/21 18:47:42  peiyongz
 * AIX 4.2, xlC 3 rev.1 compilation error: dtor() declared with external linkage
 *                                 and called or defined before being declared as inline
 *
 * Revision 1.11  2001/08/21 16:06:11  tng
 * Schema: Unique Particle Attribution Constraint Checking.
 *
 * Revision 1.10  2001/08/20 13:18:58  tng
 * bug in ContentSpecNode copy constructor.
 *
 * Revision 1.9  2001/07/24 18:33:13  knoaman
 * Added support for <group> + extra constraint checking for complexType
 *
 * Revision 1.8  2001/07/09 15:22:36  knoaman
 * complete <any> declaration.
 *
 * Revision 1.7  2001/05/11 13:27:18  tng
 * Copyright update.
 *
 * Revision 1.6  2001/05/10 16:33:08  knoaman
 * Traverse Schema Part III + error messages.
 *
 * Revision 1.5  2001/05/03 20:34:39  tng
 * Schema: SchemaValidator update
 *
 * Revision 1.4  2001/04/19 18:17:29  tng
 * Schema: SchemaValidator update, and use QName in Content Model
 *
 * Revision 1.3  2001/03/21 21:56:26  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.2  2001/02/27 14:48:49  tng
 * Schema: Add CMAny and ContentLeafNameTypeVector, by Pei Yong Zhang
 *
 * Revision 1.1  2001/02/16 14:17:29  tng
 * Schema: Move the common Content Model files that are shared by DTD
 * and schema from 'DTD' folder to 'common' folder.  By Pei Yong Zhang.
 *
 * Revision 1.4  2000/03/02 19:55:38  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/24 20:16:48  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/09 21:42:37  abagchi
 * Copyright swat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:14  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:38  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(CONTENTSPECNODE_HPP)
#define CONTENTSPECNODE_HPP

#include <framework/XMLElementDecl.hpp>
#include <util/XercesDefs.hpp>

class XMLBuffer;
class Grammar;


class ContentSpecNode
{
public :
    // -----------------------------------------------------------------------
    //  Class specific types
    // -----------------------------------------------------------------------
    enum NodeTypes
    {
        Leaf = 0
        , ZeroOrOne
        , ZeroOrMore
        , OneOrMore
        , Choice
        , Sequence
        , Any
        , Any_Other
        , Any_NS = 8
        , All = 9
        , Any_Lax = 22
        , Any_Other_Lax = 23
        , Any_NS_Lax = 24
        , Any_Skip = 38
        , Any_Other_Skip = 39
        , Any_NS_Skip = 40

        , UnknownType = -1
    };


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    ContentSpecNode();
    ContentSpecNode(QName* const toAdopt);
    ContentSpecNode
    (
        const   NodeTypes               type
        ,       ContentSpecNode* const  firstToAdopt
        ,       ContentSpecNode* const  secondToAdopt
        , const bool                    adoptFirst = true
        , const bool                    adoptSecond = true
    );
    ContentSpecNode(const ContentSpecNode&);
	~ContentSpecNode();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    QName* getElement();
    const QName* getElement() const;
    ContentSpecNode* getFirst();
    const ContentSpecNode* getFirst() const;
    ContentSpecNode* getSecond();
    const ContentSpecNode* getSecond() const;
    NodeTypes getType() const;
    ContentSpecNode* orphanFirst();
    ContentSpecNode* orphanSecond();
    unsigned int getMinOccurs() const;
    unsigned int getMaxOccurs() const;
    bool isFirstAdopted() const;
    bool isSecondAdopted() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setElement(QName* const toAdopt);
    void setFirst(ContentSpecNode* const toAdopt);
    void setSecond(ContentSpecNode* const toAdopt);
    void setType(const NodeTypes type);
    void setMinOccurs(unsigned int min);
    void setMaxOccurs(unsigned int max);
    void setAdoptFirst(bool adoptFirst);
    void setAdoptSecond(bool adoptSecond);


    // -----------------------------------------------------------------------
    //  Miscellaneous
    // -----------------------------------------------------------------------
    void formatSpec (XMLBuffer&      bufToFill)   const;


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------



    // -----------------------------------------------------------------------
    //  Private Data Members
    //
    //  fElement
    //      If the type is Leaf, then this is the qName of the element. If the URI
    //      is fgPCDataElemId, then its a PCData node.
    //
    //  fFirst
    //  fSecond
    //      The optional first and second nodes. The fType field indicates
    //      which of these are valid. The validaty constraints are:
    //
    //          Leaf = Neither valid
    //          ZeroOrOne, ZeroOrMore = First
    //          Choice, Sequence = First and Second
    //          Any* = Neither valid
    //
    //  fType
    //      The type of node. This controls how many of the child node fields
    //      are used.
    //
    //  fAdoptFirst
    //      Indicate if this ContentSpecNode adopts the fFirst, and is responsible
    //      for deleting it.
    //
    //  fAdoptSecond
    //      Indicate if this ContentSpecNode adopts the fSecond, and is responsible
    //      for deleting it.
    //
    //  fMinOccurs
    //      Indicate the minimum times that this node can occur
    //
    //  fMaxOccurs
    //      Indicate the maximum times that this node can occur
    //      -1 (Unbounded), default (1)
    // -----------------------------------------------------------------------
    QName*              fElement;
    ContentSpecNode*    fFirst;
    ContentSpecNode*    fSecond;
    NodeTypes           fType;
    bool                fAdoptFirst;
    bool                fAdoptSecond;
    unsigned int        fMinOccurs;
    unsigned int        fMaxOccurs;
};

// ---------------------------------------------------------------------------
//  ContentSpecNode: Constructors and Destructor
// ---------------------------------------------------------------------------
inline ContentSpecNode::ContentSpecNode() :

    fElement(0)
    , fFirst(0)
    , fSecond(0)
    , fType(ContentSpecNode::Leaf)
    , fAdoptFirst(true)
    , fAdoptSecond(true)
    , fMinOccurs(1)
    , fMaxOccurs(1)
{
    fElement = new QName (XMLUni::fgZeroLenString, XMLUni::fgZeroLenString, XMLElementDecl::fgInvalidElemId);
}

inline
ContentSpecNode::ContentSpecNode(QName* const element) :

    fElement(0)
    , fFirst(0)
    , fSecond(0)
    , fType(ContentSpecNode::Leaf)
    , fAdoptFirst(true)
    , fAdoptSecond(true)
    , fMinOccurs(1)
    , fMaxOccurs(1)
{
    if (!element)
        fElement = new QName (XMLUni::fgZeroLenString, XMLUni::fgZeroLenString, XMLElementDecl::fgInvalidElemId);
    else
        fElement = new QName(element);
}

inline
ContentSpecNode::ContentSpecNode(const  NodeTypes               type
                                ,       ContentSpecNode* const  firstAdopt
                                ,       ContentSpecNode* const  secondAdopt
                                , const bool                    adoptFirst
                                , const bool                    adoptSecond) :

    fElement(0)
    , fFirst(firstAdopt)
    , fSecond(secondAdopt)
    , fType(type)
    , fAdoptFirst(adoptFirst)
    , fAdoptSecond(adoptSecond)
    , fMinOccurs(1)
    , fMaxOccurs(1)
{
    fElement = new QName (XMLUni::fgZeroLenString, XMLUni::fgZeroLenString, XMLElementDecl::fgInvalidElemId);
}

inline
ContentSpecNode::ContentSpecNode(const ContentSpecNode& toCopy)
{
    const QName* tempElement = toCopy.getElement();
    if (tempElement)
        fElement = new QName(tempElement);
    else
        fElement = new QName (XMLUni::fgZeroLenString, XMLUni::fgZeroLenString, XMLElementDecl::fgInvalidElemId);

    const ContentSpecNode *tmp = toCopy.getFirst();
    if (tmp)
        fFirst = new ContentSpecNode(*tmp);
    else
        fFirst = 0;

    tmp = toCopy.getSecond();
    if (tmp)
        fSecond = new ContentSpecNode(*tmp);
    else
        fSecond = 0;

    fType = toCopy.getType();
    fAdoptFirst = true;
    fAdoptSecond = true;
    fMinOccurs = toCopy.getMinOccurs();
    fMaxOccurs = toCopy.getMaxOccurs();
}

inline ContentSpecNode::~ContentSpecNode()
{
    // Delete our children, which cause recursive cleanup
    if (fAdoptFirst) {
		delete fFirst;
    }

    if (fAdoptSecond) {
		delete fSecond;
    }

    delete fElement;
}

// ---------------------------------------------------------------------------
//  ContentSpecNode: Getter methods
// ---------------------------------------------------------------------------
inline QName* ContentSpecNode::getElement()
{
    return fElement;
}

inline const QName* ContentSpecNode::getElement() const
{
    return fElement;
}

inline ContentSpecNode* ContentSpecNode::getFirst()
{
    return fFirst;
}

inline const ContentSpecNode* ContentSpecNode::getFirst() const
{
    return fFirst;
}

inline ContentSpecNode* ContentSpecNode::getSecond()
{
    return fSecond;
}

inline const ContentSpecNode* ContentSpecNode::getSecond() const
{
    return fSecond;
}

inline ContentSpecNode::NodeTypes ContentSpecNode::getType() const
{
    return fType;
}

inline ContentSpecNode* ContentSpecNode::orphanFirst()
{
    ContentSpecNode* retNode = fFirst;
    fFirst = 0;
    return retNode;
}

inline ContentSpecNode* ContentSpecNode::orphanSecond()
{
    ContentSpecNode* retNode = fSecond;
    fSecond = 0;
    return retNode;
}

inline unsigned int ContentSpecNode::getMinOccurs() const
{
    return fMinOccurs;
}

inline unsigned int ContentSpecNode::getMaxOccurs() const
{
    return fMaxOccurs;
}

inline bool ContentSpecNode::isFirstAdopted() const
{
    return fAdoptFirst;
}

inline bool ContentSpecNode::isSecondAdopted() const
{
    return fAdoptSecond;
}


// ---------------------------------------------------------------------------
//  ContentSpecType: Setter methods
// ---------------------------------------------------------------------------
inline void ContentSpecNode::setElement(QName* const element)
{
    delete fElement;
    fElement = new QName(element);
}

inline void ContentSpecNode::setFirst(ContentSpecNode* const toAdopt)
{
    if (fAdoptFirst)
        delete fFirst;
    fFirst = toAdopt;
}

inline void ContentSpecNode::setSecond(ContentSpecNode* const toAdopt)
{
    if (fAdoptSecond)
        delete fSecond;
    fSecond = toAdopt;
}

inline void ContentSpecNode::setType(const NodeTypes type)
{
    fType = type;
}

inline void ContentSpecNode::setMinOccurs(unsigned int min)
{
    fMinOccurs = min;
}

inline void ContentSpecNode::setMaxOccurs(unsigned int max)
{
    fMaxOccurs = max;
}

inline void ContentSpecNode::setAdoptFirst(bool newState)
{
    fAdoptFirst = newState;
}

inline void ContentSpecNode::setAdoptSecond(bool newState)
{
    fAdoptSecond = newState;
}

#endif
