#ifndef IDOM_Attr_HEADER_GUARD_
#define IDOM_Attr_HEADER_GUARD_

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
 * Revision 1.1  2001/04/03 00:14:27  andyh
 * Initial revision
 *
 * Revision 1.7  2000/03/11 02:58:38  chchou
 * Fix bug # 18, remove set method of readonly attributes
 *
 * Revision 1.6  2000/03/02 19:53:52  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.5  2000/02/24 20:11:27  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.4  2000/02/17 17:47:24  andyh
 * Update Doc++ API comments
 * NameSpace update to track W3C
 * Changes were made by Chih Hsiang Chou
 *
 * Revision 1.3  2000/02/06 07:47:27  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/01/05 01:16:06  andyh
 * DOM Level 2 core, namespace support added.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:48  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:44:13  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#include <util/XercesDefs.hpp>
#include "IDOM_Node.hpp"

class IDOM_Element;


/**
* The <code>IDOM_Attr</code> class refers to an attribute of an XML element.
*
* Typically the allowable values for the 
* attribute are defined in a documenttype definition.
* <p><code>IDOM_Attr</code> objects inherit the <code>DOM_Node</code>  interface, but 
* since attributes are not actually child nodes of the elements they are associated with, the 
* DOM does not consider them part of the document  tree.  Thus, the 
* <code>DOM_Node</code> attributes <code>parentNode</code>,  
* <code>previousSibling</code>, and <code>nextSibling</code> have a  null 
* value for <code>IDOM_Attr</code> objects. The DOM takes the  view that 
* attributes are properties of elements rather than having a  separate 
* identity from the elements they are associated with;  this should make it 
* more efficient to implement such features as default attributes associated 
* with all elements of a  given type.  Furthermore, attribute nodes 
* may not be immediate children of a <code>DocumentFragment</code>. However, 
* they can be associated with <code>Element</code> nodes contained within a 
* <code>DocumentFragment</code>. In short, users of the DOM 
* need to be aware that  <code>Attr</code> nodes have some things in  common 
* with other objects inheriting the <code>Node</code> interface, but they 
* also are quite distinct.
*  
*/
class CDOM_EXPORT IDOM_Attr: public IDOM_Node {
    
protected:
    IDOM_Attr() {};
    IDOM_Attr(const IDOM_Attr &other) {};
    IDOM_Attr & operator = (const IDOM_Attr &other) {return *this;};
    

public:
  /** @name Destructor */
  //@{
	    
  /**
    * Destructor.  The object being destroyed is a reference to the Attribute
    * "node", not the underlying attribute itself.
    *
    */
    virtual ~IDOM_Attr() {};
	//@}
    
  /** @name Getter functions */
  //@{
    /**
    * Returns the name of this attribute. 
    */
    virtual const XMLCh *       getName() const = 0;

    /**
    *
    * Returns true if the attribute received its value explicitly in the
    * XML document, or if a value was assigned programatically with
    * the setValue function.  Returns false if the attribute value 
    * came from the default value declared in the document's DTD.
    */
    virtual bool            getSpecified() const = 0;

    /**
	* Returns the value of the attribute.
	*
    * The value of the attribute is returned as a string. 
    * Character and general entity references are replaced with their values.
    */
    virtual const XMLCh *       getValue() const = 0;
    
	//@}
  /** @name Setter functions */
  //@{
    /**
	* Sets the value of the attribute.  A text node with the unparsed contents
    * of the string will be created.
	*
    * @param value The value of the DOM attribute to be set
    */
    virtual void            setValue(const XMLCh *value) = 0;
	//@}

    /** @name Functions introduced in DOM Level 2. */
    //@{
    /**
     * The <code>DOM_Element</code> node this attribute is attached to or
     * <code>null</code> if this attribute is not in use.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     */
    virtual IDOM_Element     *getOwnerElement() const = 0;
    //@}

};

#endif


