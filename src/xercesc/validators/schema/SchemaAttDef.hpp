/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.1  2002/02/01 22:22:46  peiyongz
 * Initial revision
 *
 * Revision 1.6  2001/08/10 12:34:25  knoaman
 * Fix compilation error.
 *
 * Revision 1.5  2001/08/09 15:23:16  knoaman
 * add support for <anyAttribute> declaration.
 *
 * Revision 1.4  2001/07/31 15:26:54  knoaman
 * Added support for <attributeGroup>.
 *
 * Revision 1.3  2001/05/11 17:17:41  tng
 * Schema: DatatypeValidator fixes.  By Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/11 13:27:34  tng
 * Copyright update.
 *
 * Revision 1.1  2001/02/27 18:48:22  tng
 * Schema: Add SchemaAttDef, SchemaElementDecl, SchemaAttDefList.
 *
 */
#if !defined(SCHEMAATTDEF_HPP)
#define SCHEMAATTDEF_HPP

#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/XMLAttDef.hpp>
#include <xercesc/util/ValueVectorOf.hpp>

class DatatypeValidator;
class QName;

//
//  This class is a derivative of the core XMLAttDef class. This class adds
//  any Schema specific data members and provides Schema specific implementations
//  of any underlying attribute def virtual methods.
//
class VALIDATORS_EXPORT SchemaAttDef : public XMLAttDef
{
public :

    // -----------------------------------------------------------------------
    //  Constructors and Destructors
    // -----------------------------------------------------------------------
    SchemaAttDef();
    SchemaAttDef
    (
          const XMLCh* const                     prefix
        , const XMLCh* const                     localPart
        , const int                              uriId
        , const XMLAttDef::AttTypes              type = CData
        , const XMLAttDef::DefAttTypes           defType = Implied
    );
    SchemaAttDef
    (
          const XMLCh* const                     prefix
        , const XMLCh* const                     localPart
        , const int                              uriId
        , const XMLCh* const                     attValue
        , const XMLAttDef::AttTypes              type
        , const XMLAttDef::DefAttTypes           defType
        , const XMLCh* const                     enumValues = 0
    );
    SchemaAttDef
    (
          const SchemaAttDef*                   other
    );
    virtual ~SchemaAttDef();

    // -----------------------------------------------------------------------
    //  Implementation of the XMLAttDef interface
    // -----------------------------------------------------------------------
    virtual const XMLCh* getFullName() const;


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    unsigned int getElemId() const;
    QName* getAttName() const;
    DatatypeValidator* getDatatypeValidator() const;
    ValueVectorOf<unsigned int>* getNamespaceList() const;

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setElemId(const unsigned int newId);
    void setAttName
    (
        const XMLCh* const        prefix
       ,const XMLCh* const        localPart
       ,const int                 uriId = -1
    );
    void setDatatypeValidator(DatatypeValidator* newDatatypeValidator);
    void setNamespaceList(const ValueVectorOf<unsigned int>* const toSet);
    void resetNamespaceList();


private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fElemId
    //      This is the id of the element (the id is into the element decl
    //      pool) of the element this attribute def said it belonged to.
    //      This is used later to link back to the element, mostly for
    //      validation purposes.
    //
    //  fAttName
    //      This is the name of the attribute.
    //
    //  fDatatypeValidator
    //      The DatatypeValidator used to validate this attribute type.
    //
    //  fNamespaceList
    //      The list of namespace values for a wildcard attribute
    //
    // -----------------------------------------------------------------------
    unsigned int                 fElemId;
    QName*                       fAttName;
    DatatypeValidator*           fDatatypeValidator;
    ValueVectorOf<unsigned int>* fNamespaceList;
};


// ---------------------------------------------------------------------------
//  SchemaAttDef: Getter methods
// ---------------------------------------------------------------------------
inline unsigned int SchemaAttDef::getElemId() const
{
    return fElemId;
}


inline QName* SchemaAttDef::getAttName() const
{
    return fAttName;
}

inline DatatypeValidator* SchemaAttDef::getDatatypeValidator() const
{
    return fDatatypeValidator;
}

inline ValueVectorOf<unsigned int>*
SchemaAttDef::getNamespaceList() const {
    return fNamespaceList;
}


// ---------------------------------------------------------------------------
//  SchemaAttDef: Setter methods
// ---------------------------------------------------------------------------
inline void SchemaAttDef::setElemId(const unsigned int newId)
{
    fElemId = newId;
}

inline void SchemaAttDef::setDatatypeValidator(DatatypeValidator* newDatatypeValidator)
{
    fDatatypeValidator = newDatatypeValidator;
}

inline void SchemaAttDef::resetNamespaceList() {

    if (fNamespaceList && fNamespaceList->size()) {
        fNamespaceList->removeAllElements();
    }
}

inline void SchemaAttDef::setNamespaceList(const ValueVectorOf<unsigned int>* const toSet) {

    if (toSet && toSet->size()) {

        if (fNamespaceList) {
            *fNamespaceList = *toSet;
        }
        else {
            fNamespaceList = new ValueVectorOf<unsigned int>(*toSet);
        }
    }
    else  {
        resetNamespaceList();
    }
}

#endif
