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
 * Revision 1.7  2003/10/10 16:25:40  peiyongz
 * Implementation of Serialization/Deserialization
 *
 * Revision 1.6  2003/05/18 14:02:07  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.5  2003/05/16 21:43:21  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.4  2003/02/06 13:51:55  gareth
 * fixed bug with multiple attributes being validated by the same union type.
 *
 * Revision 1.3  2003/01/29 19:47:16  gareth
 * added DOMTypeInfo and some PSVI methods
 *
 * Revision 1.2  2002/11/04 14:49:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:46  peiyongz
 * sane_include
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
#include <xercesc/validators/datatype/DatatypeValidator.hpp>
#include <xercesc/validators/datatype/UnionDatatypeValidator.hpp>
#include <xercesc/validators/schema/PSVIDefs.hpp>
XERCES_CPP_NAMESPACE_BEGIN

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
    SchemaAttDef(MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    SchemaAttDef
    (
          const XMLCh* const           prefix
        , const XMLCh* const           localPart
        , const int                    uriId
        , const XMLAttDef::AttTypes    type = CData
        , const XMLAttDef::DefAttTypes defType = Implied
        , MemoryManager* const         manager = XMLPlatformUtils::fgMemoryManager
    );
    SchemaAttDef
    (
          const XMLCh* const           prefix
        , const XMLCh* const           localPart
        , const int                    uriId
        , const XMLCh* const           attValue
        , const XMLAttDef::AttTypes    type
        , const XMLAttDef::DefAttTypes defType
        , const XMLCh* const           enumValues = 0
        , MemoryManager* const         manager = XMLPlatformUtils::fgMemoryManager
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
    virtual void reset();


    // ----------------------------------------------------------------------
    // Partial implementation of PSVI
    // The values these methods return are only accurate until the DOMAttr
    // is created that uses the values. After this a celan up method is called
    // and the SchemaAttDef may be used again.
    // note that some of this information has dependancies. For example,
    // if something is not valid then the information returned by the other 
    // calls may be meaningless
    // See http://www.w3.org/TR/xmlschema-1/ for detailed information
    // ----------------------------------------------------------------------

    /** 
     * The appropriate case among the following:
     * 1 If it was strictly assessed, then the appropriate case among the following:
     * 1.1 If it was valid as defined by Attribute Locally Valid (3.2.4), then valid;
     * 1.2 otherwise invalid.
     * 2 otherwise notKnown.
     */
    PSVIDefs::Validity getValidity() const;

    /**
     * The appropriate case among the following:
     * 1 If it was strictly assessed, then full;
     * 2 otherwise none.
     */
    PSVIDefs::Validation getValidationAttempted() const;

    /**
     * @return the complexity. Always simple for attrs
     */
    PSVIDefs::Complexity getTypeType() const;

    /**
     * The target namespace of the type definition.
     */
    const XMLCh* getTypeUri() const;

    /**
     * The {name} of the type definition, if it is not absent. 
     */
    const XMLCh* getTypeName() const;

    /**
     * true if the {name} of the type definition is absent, otherwise false.
     */
    bool getTypeAnonymous() const;

    /**
     * If this method returns true and validity is VALID then the next three 
     * produce accurate results
     * @return true if the element is validated using a union type
     */
    bool isTypeDefinitionUnion() const;

    /**
     * The {target namespace} of the actual member type definition.
     */
    const XMLCh* getMemberTypeUri() const;

    /**
     * @return true if the {name} of the actual member type definition is absent, otherwise false.
     */
    bool getMemberTypeAnonymous() const;

    /**
     * @return the {name} of the actual member type definition, if it is not absent. 
     */
    const XMLCh* getMemberTypeName() const;

    virtual const XMLCh* getDOMTypeInfoUri() const;
    virtual const XMLCh* getDOMTypeInfoName() const;
    
    

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
    void setAnyDatatypeValidator(DatatypeValidator* newDatatypeValidator);
    void setMembertypeValidator(const DatatypeValidator* newDatatypeValidator);
    void setNamespaceList(const ValueVectorOf<unsigned int>* const toSet);
    void resetNamespaceList();
    void setValidity(PSVIDefs::Validity valid);
    void setValidationAttempted(PSVIDefs::Validation validation);

    /***
     * Support for Serialization/De-serialization
     ***/
    DECL_XSERIALIZABLE(SchemaAttDef)

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
    //  fAnyDatatypeValidator
    //      Tempory storage for the DatatypeValidator used to validate an any
    //
    //  fMemberTypeValidator
    //      Tempory storage used when the validator being used is of union type.
    //      This stores the actual member validator used to validate.
    //
    //  fNamespaceList
    //      The list of namespace values for a wildcard attribute
    //
    //  fValidity
    //      After this attr has been validated this is its validity
    //
    //  fValidation
    //      The type of validation that happened to this attr
    //
    // -----------------------------------------------------------------------
    unsigned int                 fElemId;
    QName*                       fAttName;
    DatatypeValidator*           fDatatypeValidator;
    DatatypeValidator*           fAnyDatatypeValidator;
    const DatatypeValidator*     fMemberTypeValidator;
    ValueVectorOf<unsigned int>* fNamespaceList;
    PSVIDefs::Validity           fValidity;
    PSVIDefs::Validation         fValidation;
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

inline void SchemaAttDef::setValidity(PSVIDefs::Validity valid) {
    fValidity = valid;
}

inline void SchemaAttDef::setValidationAttempted(PSVIDefs::Validation validation) {
    fValidation = validation;
}


inline const XMLCh* SchemaAttDef::getTypeName() const {
    if(fAnyDatatypeValidator) 
        return fAnyDatatypeValidator->getTypeLocalName();
    else if(fDatatypeValidator)
        return fDatatypeValidator->getTypeLocalName();

    //its anySimpleType if we have not done validation on it
    if(getValidationAttempted() == PSVIDefs::NONE)
        return SchemaSymbols::fgDT_ANYSIMPLETYPE;


    return 0;
}

inline PSVIDefs::Complexity SchemaAttDef::getTypeType() const {
    return PSVIDefs::SIMPLE;
}

inline const XMLCh* SchemaAttDef::getTypeUri() const {
    if(fAnyDatatypeValidator) 
        return fAnyDatatypeValidator->getTypeUri();
    else if(fDatatypeValidator) 
        return fDatatypeValidator->getTypeUri();

    //its anySimpleType if we have not done validation on it
    if(getValidationAttempted() == PSVIDefs::NONE)
        return SchemaSymbols::fgURI_SCHEMAFORSCHEMA;

    return 0;
}


inline const XMLCh* SchemaAttDef::getMemberTypeName() const {
    if(fMemberTypeValidator) 
        return fMemberTypeValidator->getTypeLocalName();
    return 0;
}

inline const XMLCh* SchemaAttDef::getMemberTypeUri() const {
    if(fMemberTypeValidator) 
        return fMemberTypeValidator->getTypeUri();
    return 0;
}

inline PSVIDefs::Validity SchemaAttDef::getValidity() const {
    return fValidity;
}

inline PSVIDefs::Validation SchemaAttDef::getValidationAttempted() const {
    return fValidation;
}

inline const XMLCh* SchemaAttDef::getDOMTypeInfoName() const {
    if(fValidity != PSVIDefs::VALID)
        return SchemaSymbols::fgDT_ANYSIMPLETYPE;
    if(getTypeAnonymous() || getMemberTypeAnonymous())
        return 0;
    if(fMemberTypeValidator)
        return getMemberTypeName();

    return getTypeName();
}

inline const XMLCh* SchemaAttDef::getDOMTypeInfoUri() const {
    if(fValidity != PSVIDefs::VALID)
        return SchemaSymbols::fgURI_SCHEMAFORSCHEMA;
    if(getTypeAnonymous() || getMemberTypeAnonymous())
        return 0;
    if(fMemberTypeValidator)
        return getMemberTypeUri();
    return getTypeUri();
}

inline bool SchemaAttDef::getTypeAnonymous() const {
    if(fAnyDatatypeValidator) 
        return fAnyDatatypeValidator->getAnonymous();
    else if(fDatatypeValidator)
        return fDatatypeValidator->getAnonymous();

    return false;
}

inline bool SchemaAttDef::getMemberTypeAnonymous() const {
    if(fMemberTypeValidator) 
        return fMemberTypeValidator->getAnonymous();

    return false;
}

inline bool SchemaAttDef::isTypeDefinitionUnion() const {
   if(fAnyDatatypeValidator && fAnyDatatypeValidator->getType() == DatatypeValidator::Union ||
      fDatatypeValidator && fDatatypeValidator->getType() == DatatypeValidator::Union)
       return true;
    return false;
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

inline void SchemaAttDef::setAnyDatatypeValidator(DatatypeValidator* newDatatypeValidator)
{
    fAnyDatatypeValidator = newDatatypeValidator;
}

inline void SchemaAttDef::setMembertypeValidator(const DatatypeValidator* newDatatypeValidator)
{
    fMemberTypeValidator = newDatatypeValidator;
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
            fNamespaceList = new (getMemoryManager()) ValueVectorOf<unsigned int>(*toSet);
        }
    }
    else  {
        resetNamespaceList();
    }
}

inline void SchemaAttDef::reset() {
    if(fAnyDatatypeValidator && fAnyDatatypeValidator->getType() == DatatypeValidator::Union)
        ((UnionDatatypeValidator *)fAnyDatatypeValidator)->reset();
    else if(fDatatypeValidator && fDatatypeValidator->getType() == DatatypeValidator::Union)
        ((UnionDatatypeValidator *)fDatatypeValidator)->reset();
    fAnyDatatypeValidator = 0;
    fMemberTypeValidator = 0;
    fValidity = PSVIDefs::UNKNOWN;
    fValidation = PSVIDefs::NONE;    
}

XERCES_CPP_NAMESPACE_END

#endif
