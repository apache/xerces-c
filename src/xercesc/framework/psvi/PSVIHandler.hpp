/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
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
  * $Log$
  * Revision 1.11  2005/01/06 21:39:43  amassari
  * Removed warnings
  *
  * Revision 1.10  2004/09/23 21:22:15  peiyongz
  * Documentation
  *
  * Revision 1.9  2004/09/22 20:38:45  peiyongz
  * provide default implementation and documentation
  *
  * Revision 1.8  2004/09/21 16:09:37  peiyongz
  * Handle partial PSVIElement
  *
  * Revision 1.7  2004/09/08 13:56:07  peiyongz
  * Apache License Version 2.0
  *
  * Revision 1.6  2003/12/01 23:23:26  neilg
  * fix for bug 25118; thanks to Jeroen Witmond
  *
  * Revision 1.5  2003/11/28 05:13:45  neilg
  * remove passing of prefixes in PSVIHandler
  *
  * Revision 1.4  2003/11/25 14:20:21  neilg
  * clean up usage of const in PSVIHandler
  *
  * Revision 1.3  2003/11/17 18:12:00  neilg
  * PSVIAttributeList needs to be included by PSVIHandler.  Thanks to Pete Lloyd for the patch
  *
  * Revision 1.2  2003/09/22 15:03:06  neilg
  * clearly the local name of an element should be a string, not an XMLElementDecl...
  *
  * Revision 1.1  2003/09/16 14:33:36  neilg
  * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
  *
  */


#if !defined(PSVIHANDLER_HPP)
#define PSVIHANDLER_HPP

#include <xercesc/framework/psvi/PSVIElement.hpp>
#include <xercesc/framework/psvi/PSVIAttributeList.hpp>

XERCES_CPP_NAMESPACE_BEGIN


/**
  * This abstract class provides the interface for the scanner to return
  * PSVI information to the application.
  *
  */
class XMLPARSER_EXPORT PSVIHandler
{
public:
    // -----------------------------------------------------------------------
    //  Constructors are hidden, just the virtual destructor is exposed
    // -----------------------------------------------------------------------
    /** @name Destructor */
    //@{
    virtual ~PSVIHandler()
    {
    }
    //@}

    /** @name The PSVI handler interface */
    //@{
    /** Receive notification of the PSVI properties of an element.
      * The scanner will issue this call after the XMLDocumentHandler
      * endElement call.  Since the scanner will issue the psviAttributes
      * call immediately after reading the start tag of an element, all element
      * content will be effectively bracketed by these two calls.
      * @param  localName The name of the element whose end tag was just
      *                     parsed.
      * @param  uri       The namespace to which the element is bound
      * @param  elementInfo    Object containing the element's PSVI properties
      */
    virtual void handleElementPSVI
    (
        const   XMLCh* const            localName 
        , const XMLCh* const            uri
        ,       PSVIElement *           elementInfo
    ) = 0;

    /** 
      * Receive notification of partial PSVI properties of an element.
      * This callback is made right after the psviAttributes
      * call for non-empty element. 
      *
      *  The PSVIElement passed in has all fields properly set and it
      *  can be safely accessed the same way as the one passed in handleElementPSVI.
      *  However, fields listed below always have default values.
      *
      *  getValidity()	              PSVIItem::VALIDITY_NOTKNOWN
      *  getValidationAttemped()      PSVIItem::VALIDATION_NONE
      *  getMemberTypeDefinition()    0
      *  getSchemaNormalizedValue()   0
      *  getCanonicalRepresentation() 0
      *  getNotationDeclaration()     0
      *
      *
      * @param  localName    The name of the element upon which start tag 
      *                      these attributes were encountered.
      * @param  uri          The namespace to which the element is bound
      * @param  elementInfo  Object containing the element's partial PSVI properties
      */
    virtual void handlePartialElementPSVI
    (
          const XMLCh*        const localName 
        , const XMLCh*        const uri
        ,       PSVIElement *       elementInfo
    );

    /**
      * Enables PSVI information about attributes to be passed back to the
      * application.  This callback will be made on *all*
      * elements; on elements with no attributes, the final parameter will
      * be null.
      * @param  localName The name of the element upon which start tag 
      *          these attributes were encountered.
      * @param  uri       The namespace to which the element is bound
      * @param  psviAttributes   Object containing the attributes' PSVI properties
      *          with information to identify them.
      */
    virtual void handleAttributesPSVI
    (
        const   XMLCh* const            localName 
        , const XMLCh* const            uri
        ,       PSVIAttributeList *     psviAttributes
    ) = 0;


    //@}



protected :
    // -----------------------------------------------------------------------
    //  Hidden Constructors
    // -----------------------------------------------------------------------
    PSVIHandler()
    {
    }


private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    PSVIHandler(const PSVIHandler&);
    PSVIHandler& operator=(const PSVIHandler&);
};

inline void PSVIHandler::handlePartialElementPSVI(const XMLCh*        const /*localName*/
                                                , const XMLCh*        const /*uri*/
                                                ,       PSVIElement *       /*elementInfo*/
                                                 )
{
}

XERCES_CPP_NAMESPACE_END

#endif
