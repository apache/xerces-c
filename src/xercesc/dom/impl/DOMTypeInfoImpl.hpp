/*
 * Copyright 2003,2004 The Apache Software Foundation.
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

//
//  This file is part of the internal implementation of the C++ XML DOM.
//  It should NOT be included or used directly by application programs.
//
//  Applications should include the file <xercesc/dom/DOM.hpp> for the entire
//  DOM API, or xercesc/dom/DOM*.hpp for individual DOM classes, where the class
//  name is substituded for the *.
//


#if !defined(DOMTYPEINFOIMPL_HPP)
#define DOMTYPEINFOIMPL_HPP

//------------------------------------------------------------------------------------
//  Includes
//------------------------------------------------------------------------------------
#include <xercesc/dom/DOMTypeInfo.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class DOMDocumentImpl;

class CDOM_EXPORT DOMTypeInfoImpl : public DOMTypeInfo
{
public:

    //-----------------------------------------------------------------------------------
    //  Constructor
    //-----------------------------------------------------------------------------------
    DOMTypeInfoImpl(const XMLCh* name, const XMLCh* namespaceURI, DOMDocumentImpl *impl);
    DOMTypeInfoImpl();

    //@{
    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    /**
     * Returns The name of a type declared for the associated <code>DOMElement</code> 
     * or <code>DOMAttr</code>, or null if undeclared.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @return The name of a type declared for the associated <code>DOMElement</code> 
     * or <code>DOMAttribute</code>, or null if undeclared.
     * @since DOM level 3
     */
    virtual const XMLCh* getName() const;

    /**
     * The namespace of the type declared for the associated <code>DOMElement</code> 
     * or <code>DOMAttr</code> or null if the <code>DOMElement</code> does not have 
     * declaration or if no namespace information is available.
     *
     * <p><b>"Experimental - subject to change"</b></p>
     *
     * @return The namespace of the type declared for the associated <code>DOMElement</code> 
     * or <code>DOMAttr</code> or null if the <code>DOMElement</code> does not have 
     * declaration or if no namespace information is available.
     * @since DOM level 3
     */
    virtual const XMLCh* getNamespace() const;
    //@}
  
  private:
    const XMLCh* name;
    const XMLCh* namespaceURI;
    
    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DOMTypeInfoImpl (const DOMTypeInfoImpl&);
    DOMTypeInfoImpl & operator = (const DOMTypeInfoImpl &);
};

XERCES_CPP_NAMESPACE_END

#endif

/**
 * End of file DOMTypeInfo.hpp
 */
