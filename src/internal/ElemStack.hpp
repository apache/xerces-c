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
 * Revision 1.3  2000/02/24 20:18:07  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/06 07:47:52  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:06  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:42  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(ELEMSTACK_HPP)
#define ELEMSTACK_HPP

#include <util/XML4CDefs.hpp>
#include <util/StringPool.hpp>

#if defined(XML4C_DEBUG)
class TextOutputStream;
class XMLValidator;
#endif

class XMLElementDecl;

//
//  During the scan of content, we have to keep up with the nesting of
//  elements (for validation and wellformedness purposes) and we have to
//  have places to remember namespace (prefix to URI) mappings.
//
//  We only have to keep a stack of the current path down through the tree
//  that we are currently scanning, and keep track of any children of any
//  elements along that path.
//
//  So, this data structure is a stack, which represents the current path
//  through the tree that we've worked our way down to. For each node in
//  the stack, there is an array of element ids that represent the ids of
//  the child elements scanned so far. Upon exit from that element, its
//  array of child elements is validated.
//
//  Since we have the actual XMLElementDecl in the stack nodes, when its time
//  to validate, we just extract the content model from that element decl
//  and validate. All the required data falls easily to hand. Note that we
//  actually have some derivative of XMLElementDecl, which is specific to
//  the validator used, but the abstract API is sufficient for the needs of
//  the scanner.
//
//  Since the namespace support also requires the storage of information on
//  a nested element basis, this structure also holds the namespace info. For
//  each level, the prefixes defined at that level (and the namespaces that
//  they map to) are stored.
//
class XMLPARSER_EXPORT ElemStack
{
public :
    // -----------------------------------------------------------------------
    //  Class specific data types
    //
    //  These really should be private, but some of the compilers we have to
    //  support are too dumb to deal with that.
    //
    //  PrefMapElem
    //      fURIId is the id of the URI from the validator's URI map. The
    //      fPrefId is the id of the prefix from our own prefix pool. The
    //      namespace stack consists of these elements.
    //
    //  StackElem
    //      fThisElement is the basic element decl for the current element.
    //      The fRowCapacity is how large fChildIds has grown so far.
    //      fChildCount is how many of them are valid right now.
    //
    //      The fMapCapacity is how large fMap has grown so far. fMapCount
    //      is how many of them are valid right now.
    //
    //      Note that we store the reader number we were in when we found the
    //      start tag. We'll use this at the end tag to test for unbalanced
    //      markup in entities.
    //
    //  MapModes
    //      When a prefix is mapped to a namespace id, it matters whether the
    //      QName being mapped is an attribute or name. Attributes are not
    //      affected by an sibling xmlns attributes, whereas elements are
    //      affected by its own xmlns attributes.
    // -----------------------------------------------------------------------
    struct PrefMapElem
    {
        unsigned int        fPrefId;
        unsigned int        fURIId;
    };

    struct StackElem
    {
        XMLElementDecl*     fThisElement;
        unsigned int        fReaderNum;

        unsigned int        fChildCapacity;
        unsigned int        fChildCount;
        unsigned int*       fChildIds;

        PrefMapElem*        fMap;
        unsigned int        fMapCapacity;
        unsigned int        fMapCount;
    };

    enum MapModes
    {
        Mode_Attribute
        , Mode_Element
    };


    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    ElemStack();
    ~ElemStack();


    // -----------------------------------------------------------------------
    //  Stack access
    // -----------------------------------------------------------------------
    unsigned int addLevel();
    unsigned int addLevel(XMLElementDecl* const toSet, const unsigned int readerNum);
    XMLElementDecl& elemAt(const unsigned int index);
    const XMLElementDecl& elemAt(const unsigned int index) const;
    const StackElem* popTop();


    // -----------------------------------------------------------------------
    //  Stack top access
    // -----------------------------------------------------------------------
    unsigned int addChild(const unsigned int childId, const bool toParent);
    const StackElem* topElement() const;
    void setElement(XMLElementDecl* const toSet, const unsigned int readerNum);


    // -----------------------------------------------------------------------
    //  Prefix map methods
    // -----------------------------------------------------------------------
    void addPrefix
    (
        const   XMLCh* const    prefixToAdd
        , const unsigned int    uriId
    );
    unsigned int mapPrefixToURI
    (
        const   XMLCh* const    prefixToMap
        , const MapModes        mode
        ,       bool&           unknown
    )   const;


    // -----------------------------------------------------------------------
    //  Miscellaneous methods
    // -----------------------------------------------------------------------
    bool isEmpty() const;
    void reset
    (
        const   unsigned int    emptyId
        , const unsigned int    globalId
        , const unsigned int    unknownId
        , const unsigned int    xmlId
        , const unsigned int    xmlNSId
    );


    // -----------------------------------------------------------------------
    //  Debug only stuff
    // -----------------------------------------------------------------------
    #if defined(XML4C_DEBUG)
    void dumpStacks
    (
                TextOutputStream&   target
        , const XMLValidator&       srcPools
    );
    #endif

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    ElemStack(const ElemStack&);
    void operator=(const ElemStack&);


    // -----------------------------------------------------------------------
    //  Private helper methods
    // -----------------------------------------------------------------------
    void expandMap(StackElem* const toExpand);
    void expandStack();


    // -----------------------------------------------------------------------
    //  Data members
    //
    //  fEmptyNamespaceId
    //      This is the special URI id for the "" namespace, which is magic
    //      because of the xmlns="" operation.
    //
    //  fGlobalNamespaceId
    //  fGlobalPoolId
    //      This is a special URI id that is returned when the namespace
    //      prefix is "" and no one has explicitly mapped that prefix to an
    //      explicit URI (or when they explicitly clear any such mapping,
    //      which they can also do.) And also its prefix pool id, which is
    //      stored here for fast access.
    //
    //  fPrefixPool
    //      This is the prefix pool where prefixes are hashed and given unique
    //      ids. These ids are used to track prefixes in the element stack.
    //
    //  fStack
    //  fStackCapacity
    //  fStackTop
    //      This the stack array. Its an array of pointers to StackElem
    //      structures. The capacity is the current high water mark of the
    //      stack. The top is the current top of stack (i.e. the part of it
    //      being used.)
    //
    //  fUnknownNamespaceId
    //      This is the URI id for the special URI that is assigned to any
    //      prefix which has not been mapped. This lets us keep going after
    //      issuing the error.
    //
    //  fXMLNamespaceId
    //  fXMLPoolId
    //  fXMLNSNamespaceId
    //  fXMLNSPoolId
    //      These are the URI ids for the special URIs that are assigned to
    //      the 'xml' and 'xmlns' namespaces. And also its prefix pool id,
    //      which is stored here for fast access.
    // -----------------------------------------------------------------------
    unsigned int    fEmptyNamespaceId;
    unsigned int    fGlobalNamespaceId;
    unsigned int    fGlobalPoolId;
    XMLStringPool   fPrefixPool;
    StackElem**     fStack;
    unsigned int    fStackCapacity;
    unsigned int    fStackTop;
    unsigned int    fUnknownNamespaceId;
    unsigned int    fXMLNamespaceId;
    unsigned int    fXMLPoolId;
    unsigned int    fXMLNSNamespaceId;
    unsigned int    fXMLNSPoolId;
};


// ---------------------------------------------------------------------------
//  ElemStack: Miscellaneous methods
// ---------------------------------------------------------------------------
inline bool ElemStack::isEmpty() const
{
    return (fStackTop == 0);
}

#endif
