#ifndef DOMWriterFilter_HEADER_GUARD_
#define DOMWriterFilter_HEADER_GUARD_

/*
 * Copyright 2002,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id$
 * $Log$
 * Revision 1.10  2004/09/08 13:55:39  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.9  2003/03/07 19:59:09  tng
 * [Bug 11692] Unimplement the hidden constructors and assignment operator to remove warnings from gcc.
 *
 * Revision 1.8  2002/11/04 15:09:25  tng
 * C++ Namespace Support.
 *
 * Revision 1.7  2002/08/22 15:04:57  tng
 * Remove unused parameter variables in inline functions.
 *
 * Revision 1.6  2002/06/06 20:53:07  tng
 * Documentation Fix: Update the API Documentation for DOM headers
 *
 * Revision 1.5  2002/06/04 14:24:04  peiyongz
 * Make DOMWriterFilter pure abstract class w/o implementing any method
 * and data
 *
 * Revision 1.4  2002/06/03 22:34:53  peiyongz
 * DOMWriterFilter: setter provided, and allows any SHOW setting
 *
 * Revision 1.3  2002/05/31 20:59:40  peiyongz
 * Add "introduced in DOM3"
 *
 * Revision 1.2  2002/05/30 16:25:33  tng
 * Fix doxygen warning message.
 *
 * Revision 1.1  2002/05/28 22:38:55  peiyongz
 * DOM3 Save Interface: DOMWriter/DOMWriterFilter
 *
 */

/**
 *
 * DOMWriterFilter.hpp: interface for the DOMWriterFilter class.
 *
 * DOMWriterFilter provide applications the ability to examine nodes
 * as they are being serialized.
 *
 * DOMWriterFilter lets the application decide what nodes should be
 * serialized or not.
 *
 * The DOMDocument, DOMDocumentType, DOMNotation, and DOMEntity nodes are not passed
 * to the filter.
 *
 * @since DOM Level 3
 */


#include <xercesc/dom/DOMNodeFilter.hpp>

XERCES_CPP_NAMESPACE_BEGIN


class CDOM_EXPORT DOMWriterFilter : public DOMNodeFilter {
protected:
    // -----------------------------------------------------------------------
    //  Hidden constructors
    // -----------------------------------------------------------------------
    /** @name Hidden constructors */
    //@{    
    DOMWriterFilter() {};
    //@}

private:
    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    /** @name Unimplemented constructors and operators */
    //@{
    DOMWriterFilter(const DOMWriterFilter &);
    DOMWriterFilter & operator = (const DOMWriterFilter &);
    //@}


public:
    // -----------------------------------------------------------------------
    //  All constructors are hidden, just the destructor is available
    // -----------------------------------------------------------------------
    /** @name Destructor */
    //@{
    /**
     * Destructor
     *
     */
    virtual ~DOMWriterFilter() {};
    //@}

    // -----------------------------------------------------------------------
    //  Virtual DOMWriterFilter interface
    // -----------------------------------------------------------------------
    /** @name Functions introduced in DOM Level 3 */
    //@{
	 /**
     * Interface from <code>DOMNodeFilter</code>,
     * to be implemented by implementation (derived class)
     */
    virtual short acceptNode(const DOMNode* node) const = 0;

    /**
     * Tells the DOMWriter what types of nodes to show to the filter.
     * See <code>DOMNodeFilter</code> for definition of the constants.
     * The constant SHOW_ATTRIBUTE is meaningless here, attribute nodes will
     * never be passed to a DOMWriterFilter.
     *
     *  <p><b>"Experimental - subject to change"</b></p>
     *
     * @return The constants of what types of nodes to show.
     * @see   setWhatToShow
     * @since DOM Level 3
     */
    virtual unsigned long getWhatToShow() const =0;

    /**
     * Set what types of nodes are to be presented.
     * See <code>DOMNodeFilter</code> for definition of the constants.
     *
     *  <p><b>"Experimental - subject to change"</b></p>
     *
     * @param toShow The constants of what types of nodes to show.
     * @see   getWhatToShow
     * @since DOM Level 3
     */
    virtual void          setWhatToShow(unsigned long toShow) =0;
    //@}

private:

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fWhatToShow
    //
    //      The whatToShow mask.
    //      Tells the DOMWriter what types of nodes to show to the filter.
    //      See NodeFilter for definition of the constants.
    //      The constants
    //      SHOW_ATTRIBUTE,
    //      SHOW_DOCUMENT,
    //      SHOW_DOCUMENT_TYPE,
    //      SHOW_NOTATION, and
    //      SHOW_DOCUMENT_FRAGMENT are meaningless here,
    //      Entity nodes are not passed to the filter.
    //
    //      Those nodes will never be passed to a DOMWriterFilter.
    //
    //   Derived class shall add this data member:
    //
    //   unsigned long fWhatToShow;
    // -----------------------------------------------------------------------

};

XERCES_CPP_NAMESPACE_END

#endif
