/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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
 * Revision 1.1  2002/05/21 19:57:17  tng
 * DOM Reorganization: Move old DOM interface files from src/xercesc/dom to src/xercesc/dom/deprecated
 *
 * Revision 1.3  2002/02/20 18:17:00  tng
 * [Bug 5977] Warnings on generating apiDocs.
 *
 * Revision 1.2  2002/02/04 19:57:15  tng
 * Remove the phrase "Experimental".
 *
 * Revision 1.1.1.1  2002/02/01 22:21:46  peiyongz
 * sane_include
 *
 * Revision 1.8  2000/07/28 01:33:31  aruna1
 * DOM Level 2 Range feature introduced
 *
 * Revision 1.7  2000/03/24 01:30:50  chchou
 * Fix bug #8 to support ignorable whitespace text nodes
 *
 * Revision 1.6  2000/03/02 19:53:57  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.5  2000/02/24 20:11:28  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.4  2000/02/06 07:47:31  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.3  2000/02/04 01:49:27  aruna1
 * TreeWalker and NodeIterator changes
 *
 * Revision 1.2  2000/01/05 01:16:08  andyh
 * DOM Level 2 core, namespace support added.
 *
 * Revision 1.1.1.1  1999/11/09 01:09:04  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:22  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#ifndef DOM_Text_HEADER_GUARD_
#define DOM_Text_HEADER_GUARD_

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOM_CharacterData.hpp>

class TextImpl;


/**
 * The <code>Text</code> interface represents the textual content (termed
 * character  data in XML) of an <code>Element</code> or <code>Attr</code>.
 * If there is no markup inside an element's content, the text is contained
 * in a single object implementing the <code>Text</code> interface that is
 * the only child of the element. If there is markup, it is parsed into a
 * list of elements and <code>Text</code> nodes that form the list of
 * children of the element.
 * <p>When a document is first made available via the DOM, there is  only one
 * <code>Text</code> node for each block of text. Users may create  adjacent
 * <code>Text</code> nodes that represent the  contents of a given element
 * without any intervening markup, but should be aware that there is no way
 * to represent the separations between these nodes in XML, so they
 * will not (in general) persist between DOM editing sessions. The
 * <code>normalize()</code> method on <code>Element</code> merges any such
 * adjacent <code>Text</code> objects into a single node for each block of
 * text; this is  recommended before employing operations that depend on a
 * particular document structure, such as navigation with
 * <code>XPointers.</code>
 */
class CDOM_EXPORT DOM_Text: public DOM_CharacterData {

    public:
    /** @name Constructors and assignment operator */
    //@{
    /**
      * Default constructor for DOM_Text.  The resulting object does not
      * refer to an actual Text node; it will compare == to 0, and is similar
      * to a null object reference variable in Java.  It may subsequently be
      * assigned to refer to an actual comment node.
      *
      */
    DOM_Text();

    /**
      * Copy constructor.  Creates a new <code>DOM_Text</code> that refers to the
      * same underlying node as the original.  See also DOM_Node::clone(),
      * which will copy the actual Text node, rather than just creating a new
      * reference to the original node.
      *
      * @param other The object to be copied.
      */
    DOM_Text(const DOM_Text &other);

    /**
      * Assignment operator.
      *
      * @param other The object to be copied.
      */
    DOM_Text & operator = (const DOM_Text &other);

    /**
      * Assignment operator.  This overloaded variant is provided for
      *   the sole purpose of setting a DOM_Node reference variable to
      *   zero.  Nulling out a reference variable in this way will decrement
      *   the reference count on the underlying Node object that the variable
      *   formerly referenced.  This effect is normally obtained when reference
      *   variable goes out of scope, but zeroing them can be useful for
      *   global instances, or for local instances that will remain in scope
      *   for an extended time,  when the storage belonging to the underlying
      *   node needs to be reclaimed.
      *
      * @param val  Only a value of 0, or null, is allowed.
      */
    DOM_Text & operator = (const DOM_NullPtr *val);

    //@}
    /** @name Destructor. */
    //@{
	 /**
	  * Destructor for DOM_Text. The object being destroyed is the reference
      * object, not the underlying Comment node itself.
	  *
	  */
    ~DOM_Text();

    //@}
    /** @name Functions to modify the Text node. */
    //@{

    /**
     * Breaks this node into two nodes at the specified
     * offset, keeping both in the tree as siblings.
     *
     * This node then only
     * contains all the content up to the <code>offset</code> point. And a new
     * node of the same nodeType, which is inserted as the next sibling of this
     * node, contains all the content at and after the <code>offset</code>
     * point. When the <code>offset</code> is equal to the lenght of this node,
     * the new node has no data.
     * @param offset The offset at which to split, starting from 0.
     * @return The new <code>Text</code> node.
     * @exception DOMException
     *   INDEX_SIZE_ERR: Raised if the specified offset is negative or greater
     *   than the number of characters in <code>data</code>.
     *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
     */
    DOM_Text splitText(unsigned int offset);

    //@}
    /** @name Non-standard (not defined by the DOM specification) functions. */
    //@{

    /**
     *
     * Return true if this node contains ignorable whitespaces only.
     * @return True if this node contains ignorable whitespaces only.
     */
    bool isIgnorableWhitespace();

    //@}

protected:
    DOM_Text(TextImpl *);

    friend class DOM_Document;
    friend class RangeImpl;



};

#endif


