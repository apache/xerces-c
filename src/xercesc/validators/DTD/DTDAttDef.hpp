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
 * Revision 1.10  2004/09/20 14:47:13  amassari
 * Mark some methods as deprecated
 *
 * Revision 1.9  2004/09/08 13:56:50  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2004/01/29 11:52:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.7  2003/12/17 00:18:40  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.6  2003/10/10 16:24:51  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.5  2003/05/16 21:43:19  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.4  2003/05/15 18:54:50  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2003/01/29 19:46:40  gareth
 * added DOMTypeInfo API
 *
 * Revision 1.2  2002/11/04 14:50:40  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:43  peiyongz
 * sane_include
 *
 * Revision 1.3  2000/02/24 20:16:48  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.2  2000/02/09 21:42:37  abagchi
 * Copyright swat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:26  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:39  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(DTDATTDEF_HPP)
#define DTDATTDEF_HPP

#include <xercesc/framework/XMLAttDef.hpp>

XERCES_CPP_NAMESPACE_BEGIN

//
//  This class is a derivative of the core XMLAttDef class. This class adds
//  any DTD specific data members and provides DTD specific implementations
//  of any underlying attribute def virtual methods.
//
//  In the DTD we don't do namespaces, so the attribute names are just the
//  QName literally from the DTD. This is what we return as the full name,
//  which is what is used to key these in any name keyed collections.
//
class VALIDATORS_EXPORT DTDAttDef : public XMLAttDef
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructors
    // -----------------------------------------------------------------------
    DTDAttDef(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    DTDAttDef
    (
        const   XMLCh* const           attName
        , const XMLAttDef::AttTypes    type = CData
        , const XMLAttDef::DefAttTypes defType = Implied
        , MemoryManager* const         manager = XMLPlatformUtils::fgMemoryManager
    );
    DTDAttDef
    (
        const   XMLCh* const           attName
        , const XMLCh* const           attValue
        , const XMLAttDef::AttTypes    type
        , const XMLAttDef::DefAttTypes defType
        , const XMLCh* const           enumValues = 0
        , MemoryManager* const         manager = XMLPlatformUtils::fgMemoryManager
    );
    ~DTDAttDef();


    // -----------------------------------------------------------------------
    //  Implementation of the XMLAttDef interface
    // -----------------------------------------------------------------------
    virtual const XMLCh* getFullName() const;

    //does nothing currently
    virtual void reset() {};

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    unsigned int getElemId() const;

    /**
     * @deprecated
    **/
    virtual const XMLCh* getDOMTypeInfoName() const;

    /**
     * @deprecated
    **/
    virtual const XMLCh* getDOMTypeInfoUri() const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setElemId(const unsigned int newId);
    void setName(const XMLCh* const newName);

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(DTDAttDef)

private :
    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DTDAttDef(const DTDAttDef &);
    DTDAttDef& operator = (const  DTDAttDef&);

    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fElemId
    //      This is the id of the element (the id is into the element decl
    //      pool) of the element this attribute def said it belonged to.
    //      This is used later to link back to the element, mostly for
    //      validation purposes.
    //
    //  fName
    //      This is the name of the attribute. Since we don't do namespaces
    //      in the DTD, its just the fully qualified name.
    // -----------------------------------------------------------------------
    unsigned int    fElemId;
    XMLCh*          fName;
};


// ---------------------------------------------------------------------------
//  DTDAttDef: Implementation of the XMLAttDef interface
// ---------------------------------------------------------------------------
inline const XMLCh* DTDAttDef::getFullName() const
{
    return fName;
}


// ---------------------------------------------------------------------------
//  DTDAttDef: Getter methods
// ---------------------------------------------------------------------------
inline unsigned int DTDAttDef::getElemId() const
{
    return fElemId;
}

inline const XMLCh* DTDAttDef::getDOMTypeInfoName() const 
{
    return getAttTypeString(getType(), getMemoryManager());
}

inline const XMLCh* DTDAttDef::getDOMTypeInfoUri() const 
{
    return 0;
}

// ---------------------------------------------------------------------------
//  DTDAttDef: Setter methods
// ---------------------------------------------------------------------------
inline void DTDAttDef::setElemId(const unsigned int newId)
{
    fElemId = newId;
}


XERCES_CPP_NAMESPACE_END

#endif
