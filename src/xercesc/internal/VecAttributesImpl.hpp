/*
 * Copyright 1999-2001,2004 The Apache Software Foundation.
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
 * Revision 1.4  2004/09/08 13:56:13  peiyongz
 * Apache License Version 2.0
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
 * Revision 1.5  2001/05/11 13:26:16  tng
 * Copyright update.
 *
 * Revision 1.4  2001/03/21 21:56:04  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.3  2001/02/26 19:44:15  tng
 * Schema: add utility class QName, by Pei Yong Zhang.
 *
 * Revision 1.2  2000/08/09 22:11:17  jpolast
 * changes to allow const instances of the sax2
 * Attributes class.
 *
 * Revision 1.1  2000/08/02 18:09:14  jpolast
 * initial checkin: attributes vector needed for
 * Attributes class as defined by sax2 spec
 *
 *
 */


#if !defined(VECATTRIBUTESIMPL_HPP)
#define VECATTRIBUTESIMPL_HPP

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/framework/XMLAttr.hpp>
#include <xercesc/util/RefVectorOf.hpp>
#include <xercesc/internal/XMLScanner.hpp>
#include <xercesc/framework/XMLBuffer.hpp>

XERCES_CPP_NAMESPACE_BEGIN

class XMLPARSER_EXPORT VecAttributesImpl : public Attributes
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    VecAttributesImpl();
    ~VecAttributesImpl();


    // -----------------------------------------------------------------------
    //  Implementation of the attributes interface
    // -----------------------------------------------------------------------
    virtual unsigned int getLength() const ;

	virtual const XMLCh* getURI(const unsigned int index) const;
    virtual const XMLCh* getLocalName(const unsigned int index) const ;
    virtual const XMLCh* getQName(const unsigned int index) const ;
    virtual const XMLCh* getType(const unsigned int index) const ;
    virtual const XMLCh* getValue(const unsigned int index) const ;

	virtual int getIndex(const XMLCh* const uri, const XMLCh* const localPart ) const  ;
	virtual int getIndex(const XMLCh* const qName ) const  ;

	virtual const XMLCh* getType(const XMLCh* const uri, const XMLCh* const localPart ) const  ;
    virtual const XMLCh* getType(const XMLCh* const qName) const ;

    virtual const XMLCh* getValue(const XMLCh* const qName) const;
	virtual const XMLCh* getValue(const XMLCh* const uri, const XMLCh* const localPart ) const  ;


    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setVector
    (
        const   RefVectorOf<XMLAttr>* const srcVec
        , const unsigned int                count
		, const XMLScanner * const		scanner
        , const bool                        adopt = false
    );

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    VecAttributesImpl(const VecAttributesImpl&);
    VecAttributesImpl& operator=(const VecAttributesImpl&);


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
	//
	//	fScanner
	//		This is a pointer to the in use Scanner, so that we can resolve
	//		namespace URIs from UriIds
	//
	//	fURIBuffer
	//		A temporary buffer which is re-used when getting namespace URI's
    // -----------------------------------------------------------------------
    bool                        fAdopt;
    unsigned int                fCount;
    const RefVectorOf<XMLAttr>* fVector;
	const XMLScanner *		fScanner ;
	//XMLBuffer				    fURIBuffer ;
};

XERCES_CPP_NAMESPACE_END

#endif // ! VECATTRIBUTESIMPL_HPP
