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
 * Revision 1.6  2004/09/08 13:56:50  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/10/10 16:24:51  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.4  2003/05/16 21:43:19  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.3  2003/05/15 18:54:50  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 14:50:40  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:43  peiyongz
 * sane_include
 *
 * Revision 1.3  2000/03/02 19:55:39  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/09 21:42:37  abagchi
 * Copyright swatswat
 *
 * Revision 1.1.1.1  1999/11/09 01:03:24  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:39  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/XMLElementDecl.hpp>
#include <xercesc/validators/DTD/DTDAttDef.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  DTDAttDef: Constructors and Destructor
// ---------------------------------------------------------------------------
DTDAttDef::DTDAttDef(MemoryManager* const manager) :

    XMLAttDef(XMLAttDef::CData, XMLAttDef::Implied, manager)
   , fElemId(XMLElementDecl::fgInvalidElemId)
    , fName(0)
{
}

DTDAttDef::DTDAttDef(const  XMLCh* const           attName
                    , const XMLAttDef::AttTypes    type
                    , const XMLAttDef::DefAttTypes defType
                    , MemoryManager* const         manager) :
    XMLAttDef(type, defType, manager)
    , fElemId(XMLElementDecl::fgInvalidElemId)
    , fName(0)
{
    fName = XMLString::replicate(attName, getMemoryManager());
}

DTDAttDef::DTDAttDef( const   XMLCh* const         attName
                    , const XMLCh* const           attValue
                    , const XMLAttDef::AttTypes    type
                    , const XMLAttDef::DefAttTypes defType
                    , const XMLCh* const           enumValues
                    , MemoryManager* const         manager) :

    XMLAttDef(attValue, type, defType, enumValues, manager)
    , fElemId(XMLElementDecl::fgInvalidElemId)
    , fName(0)
{
    fName = XMLString::replicate(attName, getMemoryManager());
}

DTDAttDef::~DTDAttDef()
{
    getMemoryManager()->deallocate(fName); //delete [] fName;
}


// ---------------------------------------------------------------------------
//  DTDAttDef: Setter methods
// ---------------------------------------------------------------------------
void DTDAttDef::setName(const XMLCh* const newName)
{
    getMemoryManager()->deallocate(fName); //delete [] fName;
    fName = XMLString::replicate(newName, getMemoryManager());
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(DTDAttDef)

void DTDAttDef::serialize(XSerializeEngine& serEng)
{

    XMLAttDef::serialize(serEng);

    if (serEng.isStoring())
    {
        serEng<<fElemId;
        serEng.writeString(fName);
    }
    else
    {
        serEng>>fElemId;
        serEng.readString(fName);
    }
}


XERCES_CPP_NAMESPACE_END
