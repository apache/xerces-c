/*
 * Copyright 1999-2004 The Apache Software Foundation.
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
 * Revision 1.6  2005/01/07 15:22:47  amassari
 * Removed warnings
 *
 * Revision 1.5  2004/11/29 16:20:35  cargilld
 * Remove XObjectComparator and XTemplateComparator.  These are debug files that aren't used.
 *
 * Revision 1.4  2004/09/08 13:56:14  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2003/10/29 16:16:08  peiyongz
 * GrammarPool' serialization/deserialization support
 *
 * Revision 1.2  2003/09/23 18:12:19  peiyongz
 * Macro re-organized: provide create/nocreate macros for abstract and
 * nonabstract classes
 *
 * Revision 1.1  2003/09/18 18:31:24  peiyongz
 * OSU: Object Serialization Utilities
 *
 *
 */

#if !defined(XSERIALIZABLE_HPP)
#define XSERIALIZABLE_HPP

#include <xercesc/internal/XSerializeEngine.hpp>
#include <xercesc/internal/XProtoType.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLUTIL_EXPORT XSerializable
{
public :

    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    virtual ~XSerializable() {} ;

    // -----------------------------------------------------------------------
    //  Serialization Interface
    // -----------------------------------------------------------------------   
    virtual bool        isSerializable()               const = 0;

    virtual void        serialize(XSerializeEngine& )        = 0;

    virtual XProtoType* getProtoType()                 const = 0;

protected:
    XSerializable() {}
    XSerializable(const XSerializable& ) {}             

private:
    // -----------------------------------------------------------------------
    //  Unimplemented assignment operator
    // -----------------------------------------------------------------------
	XSerializable& operator=(const XSerializable&);

};

inline void XSerializable::serialize(XSerializeEngine& )
{
}

/***
 * Macro to be included in XSerializable derivatives'
 * declaration's public section
 ***/
#define DECL_XSERIALIZABLE(class_name) \
public: \
\
DECL_XPROTOTYPE(class_name) \
\
virtual bool                    isSerializable()                  const ;  \
virtual XProtoType*             getProtoType()                    const;   \
virtual void                    serialize(XSerializeEngine&); \
\
inline friend XSerializeEngine& operator>>(XSerializeEngine& serEng  \
                                         , class_name*&      objPtr) \
{objPtr = (class_name*) serEng.read(XPROTOTYPE_CLASS(class_name));   \
 return serEng; \
};
	
/***
 * Macro to be included in the implementation file
 * of XSerializable derivatives' which is instantiable
 ***/
#define IMPL_XSERIALIZABLE_TOCREATE(class_name) \
IMPL_XPROTOTYPE_TOCREATE(class_name) \
IMPL_XSERIAL(class_name)

/***
 * Macro to be included in the implementation file
 * of XSerializable derivatives' which is UN-instantiable
 ***/
#define IMPL_XSERIALIZABLE_NOCREATE(class_name) \
IMPL_XPROTOTYPE_NOCREATE(class_name) \
IMPL_XSERIAL(class_name)

/***
 * Helper Macro 
 ***/
#define IMPL_XSERIAL(class_name) \
bool        class_name::isSerializable() const \
{return true; } \
XProtoType* class_name::getProtoType()   const \
{return XPROTOTYPE_CLASS(class_name); } 

#define IS_EQUIVALENT(lptr, rptr) \
    if (lptr == rptr)             \
        return true;              \
    if (( lptr && !rptr) || (!lptr &&  rptr))  \
        return false;

XERCES_CPP_NAMESPACE_END

#endif

