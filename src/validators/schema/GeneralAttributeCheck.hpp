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
 * $Id$
 */

#if !defined(GENERALATTRIBUTECHECK_HPP)
#define GENERALATTRIBUTECHECK_HPP


/** 
  * A general purpose class to check for valid values of attributes, as well
  * as check for proper association with corresponding schema elements.
  */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/RefVectorOf.hpp>
#include <util/RefHashTableOf.hpp>
#include <util/RefHash2KeysTableOf.hpp>
#include <dom/DOM_Element.hpp>
#include <validators/datatype/IDDatatypeValidator.hpp>

// ---------------------------------------------------------------------------
//  Forward declaration
// ---------------------------------------------------------------------------
class TraverseSchema;


class AttributeInfo {
public:
    // -----------------------------------------------------------------------
    //  Constructor/Destructor
    // -----------------------------------------------------------------------
    AttributeInfo(const XMLCh* const name, const short defaultOption,
                  const XMLCh* const defaultValue, const short dvIndex);
    ~AttributeInfo();

    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    short  getDefaultOption() const;
    short  getValidatorIndex() const;
    XMLCh* getName() const;
    XMLCh* getDefaultValue() const;


private:
    // -----------------------------------------------------------------------
    //  CleanUp methods
    // -----------------------------------------------------------------------
    void cleanUp();

    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    short  fDefaultOption;
    short  fValidatorIndex;
    XMLCh* fName;
    XMLCh* fDefaultValue;
};


// ---------------------------------------------------------------------------
//  local type declaration
// ---------------------------------------------------------------------------
typedef RefVectorOf<AttributeInfo> RefVectorOfAttributeInfo;


class VALIDATORS_EXPORT GeneralAttributeCheck
{
public:
    // -----------------------------------------------------------------------
    //  Constructor/Destructor
    // -----------------------------------------------------------------------
    GeneralAttributeCheck();
    ~GeneralAttributeCheck();

    // -----------------------------------------------------------------------
    //  Public Constants
    // -----------------------------------------------------------------------
    static const unsigned short GlobalContext;
    static const unsigned short LocalContext;

    // -----------------------------------------------------------------------
    //  Setter methods
    // -----------------------------------------------------------------------
    void setIDRefList(RefHashTableOf<XMLRefInfo>* const refList);

    // -----------------------------------------------------------------------
    //  Validation methods
    // -----------------------------------------------------------------------
    void checkAttributes(const DOM_Element& elem,
                         const unsigned short elemContext,
                         TraverseSchema* const schema);

    // -----------------------------------------------------------------------
    //  Notification that lazy data has been deleted
    // -----------------------------------------------------------------------
	static void reinitGeneralAttCheck();

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    GeneralAttributeCheck(const GeneralAttributeCheck&);
    void operator=(const GeneralAttributeCheck&);

    // -----------------------------------------------------------------------
    //  Setup methods
    // -----------------------------------------------------------------------
    void setUpAttributes();
    void setUpValidators();
    void mapElements();

    // -----------------------------------------------------------------------
    //  Validation methods
    // -----------------------------------------------------------------------
    void validate(const XMLCh* const attName, const XMLCh* const attValue,
                  const short dvIndex, TraverseSchema* const schema);

    // -----------------------------------------------------------------------
    //  Private Constants
    // -----------------------------------------------------------------------
    // attributes 
    enum {
        Att_Abstract_D,                // starts with 0
        Att_Attribute_FD_D,
        Att_Base_R,
        Att_Base_N,
        Att_Block_N,
        Att_Block1_N,
        Att_Block_D_D,
        Att_Default_N,
        Att_Element_FD_D,
        Att_Final_N,
        Att_Final1_N,
        Att_Final_D_D,
        Att_Fixed_N,
        Att_Fixed_D,
        Att_Form_N,
        Att_ID_N,
        Att_ItemType_N,
        Att_MaxOccurs_D,
        Att_MaxOccurs1_D,
        Att_Member_T_N,
        Att_MinOccurs_D,
        Att_MinOccurs1_D,
        Att_Mixed_D,
        Att_Mixed_N,
        Att_Name_R,
        Att_Namespace_D,
        Att_Namespace_N,
        Att_Nillable_D,
        Att_Process_C_D,
        Att_Public_R,
        Att_Ref_R,
        Att_Refer_R,
        Att_Schema_L_R,
        Att_Schema_L_N,
        Att_Source_N,
        Att_Substitution_G_N,
        Att_System_N,
        Att_Target_N_N,
        Att_Type_N,
        Att_Use_D,
        Att_Value_NNI_N,
        Att_Value_STR_N,
        Att_Value_WS_N,
        Att_Version_N,
        Att_XPath_R,
        Att_XPath1_R,

        Att_Count
    };

    // direct value compare
    enum {
        DT_Block = -1,
        DT_Block1 = -2,
        DT_Final = -3,
        DT_Final1 = -4,
        DT_Form = -5,
        DT_MaxOccurs = -6,
        DT_MaxOccurs1 = -7,
        DT_MemberTypes = -8,
        DT_MinOccurs1 = -9,
        DT_Namespace = -10,
        DT_ProcessContents = -11,
        DT_Public = -12,
        DT_Use = -13,
        DT_WhiteSpace = -14,
        DT_ID = -15
    };

    // datatype validators
    enum {
        DT_String,
        DT_Token,
        DT_AnyURI,
        DT_NonNegInt,
        DT_QName,
        DT_Boolean,

        // add XPath
        DT_Count
    };

    // element context prefixes
    enum {
        globalPrefix,
        localNamePrefix,
        localRefPrefix
    };

    // optional vs. required attribute 
    enum { 
        Att_Required,
        Att_Optional_Default,
        Att_Optional_NoDefault
    };

    // -----------------------------------------------------------------------
    //  Private data members
    // -----------------------------------------------------------------------
    static AttributeInfo**                                fAttributes;
    static DatatypeValidator**                            fValidators;
    static RefHash2KeysTableOf<RefVectorOfAttributeInfo>* fElementMap;
    IDDatatypeValidator                                   fIDValidator;
    RefHashTableOf<XMLRefInfo>*                           fIDRefList;
};


// ---------------------------------------------------------------------------
//  AttributeInfo: Getter methods
// ---------------------------------------------------------------------------
inline short AttributeInfo::getDefaultOption() const {

    return fDefaultOption;
}

inline short AttributeInfo::getValidatorIndex() const {

    return fValidatorIndex;
}

inline XMLCh* AttributeInfo::getName() const {

    return fName;
}

inline XMLCh* AttributeInfo::getDefaultValue() const {

    return fDefaultValue;
}

// ---------------------------------------------------------------------------
//  AttributeInfo: CleanUp methods
// ---------------------------------------------------------------------------
inline void AttributeInfo::cleanUp() {

    delete [] fName;
    delete [] fDefaultValue;
}

// ---------------------------------------------------------------------------
//  GeneralAttributeCheck: Setter methods
// ---------------------------------------------------------------------------
inline void
GeneralAttributeCheck::setIDRefList(RefHashTableOf<XMLRefInfo>* const refList) {

    fIDRefList = refList;
}

#endif

/**
  * End of file GeneralAttributeCheck.hpp
  */

