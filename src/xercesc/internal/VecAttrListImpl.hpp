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
 * Revision 1.5  2004/09/08 13:56:13  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/05/15 18:26:29  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2003/03/07 18:08:58  tng
 * Return a reference instead of void for operator=
 *
 * Revision 1.2  2002/11/04 14:58:18  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:21:58  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/02/24 20:18:07  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.3  2000/02/06 07:47:53  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  1999/12/15 19:49:37  roddey
 * Added second getValue() method which takes a short name for the attribute
 * to get the value for. Just a convenience method.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:19  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:45  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(VECATTRLISTIMPL_HPP)
#define VECATTRLISTIMPL_HPP

#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/framework/XMLAttr.hpp>
#include <xercesc/util/RefVectorOf.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLPARSER_EXPORT VecAttrListImpl : public XMemory, public AttributeList
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    VecAttrListImpl();
    ~VecAttrListImpl();


    // -----------------------------------------------------------------------
    //  Implementation of the attribute list interface
    // -----------------------------------------------------------------------
    virtual unsigned int getLength() const;
    virtual const XMLCh* getName(const unsigned int index) const;
    virtual const XMLCh* getType(const unsigned int index) const;
    virtual const XMLCh* getValue(const unsigned int index) const;
    virtual const XMLCh* getType(const XMLCh* const name) const;
    virtual const XMLCh* getValue(const XMLCh* const name) const;
    virtual const XMLCh* getValue(const char* const name) const;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setVector
    (
        const   RefVectorOf<XMLAttr>* const srcVec
        , const unsigned int                count
        , const bool                        adopt = false
    );


private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    VecAttrListImpl(const VecAttrListImpl&);
    VecAttrListImpl& operator=(const VecAttrListImpl&);


    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fAdopt
    //      Indicates whether the passed vector is to be adopted or not. If
    //      so, we destroy it when we are destroyed (and when a new vector is
    //      set!)
    //
    //  fCount
    //      The count of elements in the vector that should be considered
    //      valid. This is an optimization to allow vector elements to be
    //      reused over and over but a different count of them be valid for
    //      each use.
    //
    //  fVector
    //      The vector that provides the backing for the list.
    // -----------------------------------------------------------------------
    bool                        fAdopt;
    unsigned int                fCount;
    const RefVectorOf<XMLAttr>* fVector;
};

XERCES_CPP_NAMESPACE_END

#endif
