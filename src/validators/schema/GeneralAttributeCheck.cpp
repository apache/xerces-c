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
 * Revision 1.8  2001/10/16 17:01:58  knoaman
 * Extra constraint checking.
 *
 * Revision 1.7  2001/10/15 19:29:26  knoaman
 * Add support for <notation> declaration.
 *
 * Revision 1.6  2001/09/18 14:41:56  knoaman
 * Add support for <annotation>.
 *
 * Revision 1.5  2001/08/27 20:14:42  knoaman
 * Validate attributes in <all>, <redefine>, <group> and <attributeGroup> declarations.
 * Misc. fixes.
 *
 * Revision 1.4  2001/06/06 13:09:02  knoaman
 * Use BooleanDatatypeValidator to validate values.
 *
 * Revision 1.3  2001/05/18 20:05:30  knoaman
 * Modified wording of error messages.
 *
 * Revision 1.2  2001/05/17 18:11:15  knoaman
 * More constraint and attribute checking.
 *
 * Revision 1.1  2001/05/15 21:59:31  knoaman
 * TraverseSchema: add attribute checking + some fixes + more error messages.
 * More attribute cheking to come.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <validators/schema/GeneralAttributeCheck.hpp>
#include <validators/schema/SchemaSymbols.hpp>
#include <util/XMLString.hpp>
#include <util/XMLUniDefs.hpp>
#include <util/Janitor.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#include <framework/XMLErrorCodes.hpp>
#include <validators/schema/TraverseSchema.hpp>
#include <util/PlatformUtils.hpp>
#include <util/XMLDeleterFor.hpp>
#include <validators/datatype/DatatypeValidatorFactory.hpp>

// ---------------------------------------------------------------------------
//  Local const data
// ---------------------------------------------------------------------------
const XMLCh fgValueZero[] =
{
    chDigit_0, chNull
};

const XMLCh fgValueOne[] =
{
    chDigit_1, chNull
};

const XMLCh fgUnbounded[] =
{
    chLatin_u, chLatin_n, chLatin_b, chLatin_o, chLatin_u, chLatin_n, chLatin_d,
    chLatin_e, chLatin_d, chNull
};

const XMLCh fgLocal[] =
{
    chLatin_l, chLatin_o, chLatin_c, chLatin_a, chLatin_l, chNull
};

const XMLCh fgGlobal[] =
{
    chLatin_g, chLatin_l, chLatin_o, chLatin_b, chLatin_a, chLatin_l, chNull
};


// ---------------------------------------------------------------------------
//  Static member data initialization
// ---------------------------------------------------------------------------
GeneralAttributeCheck* GeneralAttributeCheck::fInstance = 0;
const unsigned short   GeneralAttributeCheck::GlobalContext = 0;
const unsigned short   GeneralAttributeCheck::LocalContext = 1;


// ---------------------------------------------------------------------------
//  AttributeInfo: Constructors and Destructor
// ---------------------------------------------------------------------------
AttributeInfo::AttributeInfo(const XMLCh* const name,
                             const short defaultOption,
                             const XMLCh* const defaultValue,
                             const short dvIndex)
    : fDefaultOption(defaultOption)
    , fValidatorIndex(dvIndex)
    , fName(XMLString::replicate(name))
    , fDefaultValue(0)
{
    try {
        if (defaultValue) {
            fDefaultValue = XMLString::replicate(defaultValue);
        }    
    }
    catch(...) {
        cleanUp();
    }
}


AttributeInfo::~AttributeInfo()
{
    cleanUp();
}



// ---------------------------------------------------------------------------
//  GeneralAttributeCheck: Constructors and Destructor
// ---------------------------------------------------------------------------
GeneralAttributeCheck::GeneralAttributeCheck()
    : fAttributes(0)
    , fValidators(0)
    , fElementMap(0)
{
    try {
        mapElements();
    }
    catch(...) {

        cleanUp();
        throw;
    }
}

GeneralAttributeCheck::~GeneralAttributeCheck()
{
    cleanUp();
}


// ---------------------------------------------------------------------------
//  GeneralAttributeCheck: Setup methods
// ---------------------------------------------------------------------------
void GeneralAttributeCheck::setUpAttributes() {

    fAttributes = new AttributeInfo*[Att_Count];

    fAttributes[Att_Abstract_D] = 
        new AttributeInfo(SchemaSymbols::fgATT_ABSTRACT, Att_Optional_Default,
                          SchemaSymbols::fgATTVAL_FALSE, DT_Boolean);

    fAttributes[Att_Attribute_FD_D] =
        new AttributeInfo(SchemaSymbols::fgATT_ATTRIBUTEFORMDEFAULT, Att_Optional_Default,
                          SchemaSymbols::fgATTVAL_UNQUALIFIED, DT_Form);

    fAttributes[Att_Base_R] = 
        new AttributeInfo(SchemaSymbols::fgATT_BASE, Att_Required,
                          0, DT_QName);

    fAttributes[Att_Base_N] = 
        new AttributeInfo(SchemaSymbols::fgATT_BASE, Att_Optional_NoDefault,
                          0, DT_QName);

    fAttributes[Att_Block_N] =
        new AttributeInfo(SchemaSymbols::fgATT_BLOCK, Att_Optional_NoDefault,
                          0, DT_Block);

    fAttributes[Att_Block1_N] =
        new AttributeInfo(SchemaSymbols::fgATT_BLOCK, Att_Optional_NoDefault,
                          0, DT_Block1);

    fAttributes[Att_Block_D_D] =
        new AttributeInfo(SchemaSymbols::fgATT_BLOCKDEFAULT, Att_Optional_Default,
                          XMLUni::fgZeroLenString, DT_Block);

    fAttributes[Att_Default_N] =
        new AttributeInfo(SchemaSymbols::fgATT_DEFAULT, Att_Optional_NoDefault,
                          0, DT_String);

    fAttributes[Att_Element_FD_D] =
        new AttributeInfo(SchemaSymbols::fgATT_ELEMENTFORMDEFAULT, Att_Optional_Default,
                          SchemaSymbols::fgATTVAL_UNQUALIFIED, DT_Form);

    fAttributes[Att_Final_N] =
        new AttributeInfo(SchemaSymbols::fgATT_FINAL, Att_Optional_NoDefault,
                          0, DT_Final);

    fAttributes[Att_Final1_N] =
        new AttributeInfo(SchemaSymbols::fgATT_FINAL, Att_Optional_NoDefault,
                          0, DT_Final1);
        
    fAttributes[Att_Final_D_D] =
        new AttributeInfo(SchemaSymbols::fgATT_FINALDEFAULT, Att_Optional_Default,
                          XMLUni::fgZeroLenString, DT_Final);

    fAttributes[Att_Fixed_N] =
        new AttributeInfo(SchemaSymbols::fgATT_FIXED, Att_Optional_NoDefault,
                          0, DT_String);

    fAttributes[Att_Fixed_D] =
        new AttributeInfo(SchemaSymbols::fgATT_FIXED, Att_Optional_Default,
                          SchemaSymbols::fgATTVAL_FALSE, DT_Boolean);

    fAttributes[Att_Form_N] =
        new AttributeInfo(SchemaSymbols::fgATT_FORM, Att_Optional_NoDefault,
                          0, DT_Form);

    fAttributes[Att_ID_N] =
        new AttributeInfo(SchemaSymbols::fgATT_ID, Att_Optional_NoDefault,
                          0, 0);

    fAttributes[Att_ItemType_N] =
        new AttributeInfo(SchemaSymbols::fgATT_ITEMTYPE, Att_Optional_NoDefault,
                          0, DT_QName);

    fAttributes[Att_MaxOccurs_D] =
        new AttributeInfo(SchemaSymbols::fgATT_MAXOCCURS, Att_Optional_Default,
                          fgValueOne, DT_MaxOccurs);

    fAttributes[Att_MaxOccurs1_D] =
        new AttributeInfo(SchemaSymbols::fgATT_MAXOCCURS, Att_Optional_Default,
                          fgValueOne, DT_MaxOccurs1);

    fAttributes[Att_Member_T_N] =
        new AttributeInfo(SchemaSymbols::fgATT_MEMBERTYPES, Att_Optional_NoDefault,
                          0, DT_MemberTypes);

    fAttributes[Att_MinOccurs_D] =
        new AttributeInfo(SchemaSymbols::fgATT_MINOCCURS, Att_Optional_Default,
                          fgValueOne, DT_NonNegInt);

    fAttributes[Att_MinOccurs1_D] =
        new AttributeInfo(SchemaSymbols::fgATT_MINOCCURS, Att_Optional_Default,
                          fgValueOne, DT_MinOccurs1);

    fAttributes[Att_Mixed_D] =
        new AttributeInfo(SchemaSymbols::fgATT_MIXED, Att_Optional_Default,
                          SchemaSymbols::fgATTVAL_FALSE, DT_Boolean);

    fAttributes[Att_Mixed_N] =
        new AttributeInfo(SchemaSymbols::fgATT_MIXED, Att_Optional_NoDefault,
                          0, DT_Boolean);

    fAttributes[Att_Name_R] =
        new AttributeInfo(SchemaSymbols::fgATT_NAME, Att_Required,
                          0, 0);

    fAttributes[Att_Namespace_D] =
        new AttributeInfo(SchemaSymbols::fgATT_NAMESPACE, Att_Optional_Default,
                          SchemaSymbols::fgATTVAL_TWOPOUNDANY, DT_Namespace);

    fAttributes[Att_Namespace_N] =
        new AttributeInfo(SchemaSymbols::fgATT_NAMESPACE, Att_Optional_NoDefault,
                          0, 0);

    fAttributes[Att_Nillable_D] =
        new AttributeInfo(SchemaSymbols::fgATT_NILLABLE, Att_Optional_Default,
                          SchemaSymbols::fgATTVAL_FALSE, DT_Boolean);

    fAttributes[Att_Process_C_D] =
        new AttributeInfo(SchemaSymbols::fgATT_PROCESSCONTENTS, Att_Optional_Default,
                          SchemaSymbols::fgATTVAL_STRICT, DT_ProcessContents);

    fAttributes[Att_Public_R] =
        new AttributeInfo(SchemaSymbols::fgATT_PUBLIC, Att_Required,
                          0, DT_Public);

    fAttributes[Att_Ref_R] =
        new AttributeInfo(SchemaSymbols::fgATT_REF, Att_Required,
                          0, DT_QName);

    fAttributes[Att_Refer_R] =
        new AttributeInfo(SchemaSymbols::fgATT_REFER, Att_Required,
                          0, DT_QName);

    fAttributes[Att_Schema_L_R] =
        new AttributeInfo(SchemaSymbols::fgATT_SCHEMALOCATION, Att_Required,
                          0, 0);

    fAttributes[Att_Schema_L_N] =
        new AttributeInfo(SchemaSymbols::fgATT_SCHEMALOCATION, Att_Optional_NoDefault,
                          0, 0);

    fAttributes[Att_Source_N] =
        new AttributeInfo(SchemaSymbols::fgATT_SOURCE, Att_Optional_NoDefault,
                          0, DT_AnyURI);

    fAttributes[Att_Substitution_G_N] =
        new AttributeInfo(SchemaSymbols::fgATT_SUBSTITUTIONGROUP, Att_Optional_NoDefault,
                          0, DT_QName);

    fAttributes[Att_System_N] =
        new AttributeInfo(SchemaSymbols::fgATT_SYSTEM, Att_Optional_NoDefault,
                          0, DT_AnyURI);
        
    fAttributes[Att_Target_N_N] =
        new AttributeInfo(SchemaSymbols::fgATT_TARGETNAMESPACE, Att_Optional_NoDefault,
                          0, 0);

    fAttributes[Att_Type_N] =
        new AttributeInfo(SchemaSymbols::fgATT_TYPE, Att_Optional_NoDefault,
                          0, DT_QName);

    fAttributes[Att_Use_D] =
        new AttributeInfo(SchemaSymbols::fgATT_USE, Att_Optional_Default,
                          SchemaSymbols::fgATTVAL_OPTIONAL, DT_Use);

    fAttributes[Att_Value_NNI_N] =
        new AttributeInfo(SchemaSymbols::fgATT_VALUE, Att_Optional_NoDefault,
                          0, DT_NonNegInt);

    fAttributes[Att_Value_STR_N] =
        new AttributeInfo(SchemaSymbols::fgATT_VALUE, Att_Optional_NoDefault,
                          0, 0);

    fAttributes[Att_Value_WS_N] =
        new AttributeInfo(SchemaSymbols::fgATT_VALUE, Att_Optional_NoDefault,
                          0, DT_WhiteSpace);

    fAttributes[Att_Version_N] =
        new AttributeInfo(SchemaSymbols::fgATT_VERSION, Att_Optional_NoDefault,
                          0, 0);
}

void GeneralAttributeCheck::setUpValidators() {

    fValidators = new DatatypeValidator*[DT_Count];

    for (int i=0; i< DT_Count; i++) {
        fValidators[i] = 0;
    }

    DatatypeValidatorFactory dvFactory;

    dvFactory.expandRegistryToFullSchemaSet();
    fValidators[DT_NonNegInt] = 
        dvFactory.getDatatypeValidator(SchemaSymbols::fgDT_NONNEGATIVEINTEGER);

    fValidators[DT_Boolean] = 
        dvFactory.getDatatypeValidator(SchemaSymbols::fgDT_BOOLEAN);

    // TO DO - add remaining valdiators
}

void GeneralAttributeCheck::mapElements() {

    if (fElementMap) {
        return;
    }

    setUpAttributes();
    setUpValidators();

    RefVectorOf<AttributeInfo>* attList = 0;
    int prefixContext = globalPrefix;

    fElementMap = new RefHash2KeysTableOf<RefVectorOfAttributeInfo>(25);

    // element "attribute" - global
    attList = new RefVectorOf<AttributeInfo>(5, false);
    attList->addElement(fAttributes[Att_Default_N]);
    attList->addElement(fAttributes[Att_Fixed_N]);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Name_R]);
    attList->addElement(fAttributes[Att_Type_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ATTRIBUTE, prefixContext, attList);

    // element "element" - global
    attList = new RefVectorOf<AttributeInfo>(10, false);
    attList->addElement(fAttributes[Att_Abstract_D]);
    attList->addElement(fAttributes[Att_Block_N]);
    attList->addElement(fAttributes[Att_Default_N]);
    attList->addElement(fAttributes[Att_Final_N]);
    attList->addElement(fAttributes[Att_Fixed_N]);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Name_R]);
    attList->addElement(fAttributes[Att_Nillable_D]);
    attList->addElement(fAttributes[Att_Substitution_G_N]);
    attList->addElement(fAttributes[Att_Type_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ELEMENT, prefixContext, attList);

    // element "complexType" - global
    attList = new RefVectorOf<AttributeInfo>(6, false);
    attList->addElement(fAttributes[Att_Abstract_D]);
    attList->addElement(fAttributes[Att_Block1_N]);
    attList->addElement(fAttributes[Att_Final_N]);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Mixed_D]);
    attList->addElement(fAttributes[Att_Name_R]);
    fElementMap->put((void*) SchemaSymbols::fgELT_COMPLEXTYPE, prefixContext, attList);

    // element "simpleType" - global
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_Final1_N]);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Name_R]);
    fElementMap->put((void*) SchemaSymbols::fgELT_SIMPLETYPE, prefixContext, attList);

    // element "schema" - global
    attList = new RefVectorOf<AttributeInfo>(7, false);
    attList->addElement(fAttributes[Att_Attribute_FD_D]);
    attList->addElement(fAttributes[Att_Block_D_D]);
    attList->addElement(fAttributes[Att_Element_FD_D]);
    attList->addElement(fAttributes[Att_Final_D_D]);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Target_N_N]);
    attList->addElement(fAttributes[Att_Version_N]);
    // xml:lang = language ???
    fElementMap->put((void*) SchemaSymbols::fgELT_SCHEMA, prefixContext, attList);

    // element "include" - global
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Schema_L_R]);
    fElementMap->put((void*) SchemaSymbols::fgELT_INCLUDE, prefixContext, attList);

    // element "import" - global
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Namespace_N]);
    attList->addElement(fAttributes[Att_Schema_L_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_IMPORT, prefixContext, attList);

    // for element "redefine" - global (same as include)
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Schema_L_R]);
    fElementMap->put((void*) SchemaSymbols::fgELT_REDEFINE, prefixContext, attList);


    // element "attributeGroup" - global
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Name_R]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ATTRIBUTEGROUP, prefixContext, attList);

    // element "group" - global
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Name_R]);
    fElementMap->put((void*) SchemaSymbols::fgELT_GROUP, prefixContext, attList);

    // element "annotation" - global
    attList = new RefVectorOf<AttributeInfo>(1, false);
    attList->addElement(fAttributes[Att_ID_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ANNOTATION, prefixContext, attList);

    // element "notation" - global
    attList = new RefVectorOf<AttributeInfo>(4, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Name_R]);
    attList->addElement(fAttributes[Att_Public_R]);
    attList->addElement(fAttributes[Att_System_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_NOTATION, prefixContext, attList);

    // element "attribute" - local ref
    prefixContext = localRefPrefix;
    attList = new RefVectorOf<AttributeInfo>(5, false);
    attList->addElement(fAttributes[Att_Default_N]);
    attList->addElement(fAttributes[Att_Fixed_N]);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Ref_R]);
    attList->addElement(fAttributes[Att_Use_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ATTRIBUTE, prefixContext, attList);

    // element "element" - local ref
    attList = new RefVectorOf<AttributeInfo>(4, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_MaxOccurs_D]);
    attList->addElement(fAttributes[Att_MinOccurs_D]);
    attList->addElement(fAttributes[Att_Ref_R]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ELEMENT, prefixContext, attList);

    // element "attributeGroup" - local ref
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Ref_R]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ATTRIBUTEGROUP, prefixContext, attList);

    // element "group" - local ref
    attList = new RefVectorOf<AttributeInfo>(4, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_MaxOccurs_D]);
    attList->addElement(fAttributes[Att_MinOccurs_D]);
    attList->addElement(fAttributes[Att_Ref_R]);
    fElementMap->put((void*) SchemaSymbols::fgELT_GROUP, prefixContext, attList);

    // element "attribute" - local name
    prefixContext = localNamePrefix;
    attList = new RefVectorOf<AttributeInfo>(7, false);
    attList->addElement(fAttributes[Att_Default_N]);
    attList->addElement(fAttributes[Att_Fixed_N]);
    attList->addElement(fAttributes[Att_Form_N]);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Name_R]);
    attList->addElement(fAttributes[Att_Type_N]);
    attList->addElement(fAttributes[Att_Use_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ATTRIBUTE, prefixContext, attList);

    // for element "element" - local name
    attList = new RefVectorOf<AttributeInfo>(10, false);
    attList->addElement(fAttributes[Att_Block_N]);
    attList->addElement(fAttributes[Att_Default_N]);
    attList->addElement(fAttributes[Att_Fixed_N]);
    attList->addElement(fAttributes[Att_Form_N]);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_MaxOccurs_D]);
    attList->addElement(fAttributes[Att_MinOccurs_D]);
    attList->addElement(fAttributes[Att_Name_R]);
    attList->addElement(fAttributes[Att_Nillable_D]);
    attList->addElement(fAttributes[Att_Type_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ELEMENT, prefixContext, attList);

    // element "complexType" - local name
    prefixContext = localNamePrefix;
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Mixed_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_COMPLEXTYPE, prefixContext, attList);

    // element "simpleContent" - local name
    attList = new RefVectorOf<AttributeInfo>(1, false);
    attList->addElement(fAttributes[Att_ID_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_SIMPLECONTENT, prefixContext, attList);

    // element "restriction" - local name
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_Base_N]);
    attList->addElement(fAttributes[Att_ID_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_RESTRICTION, prefixContext, attList);

    // element "extension" - local name
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_Base_R]);
    attList->addElement(fAttributes[Att_ID_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_EXTENSION, prefixContext, attList);

    // element "anyAttribute" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Namespace_D]);
    attList->addElement(fAttributes[Att_Process_C_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ANYATTRIBUTE, prefixContext, attList);

    // element "complexContent" - local name
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Mixed_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_COMPLEXCONTENT, prefixContext, attList);

    // element "choice" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_MaxOccurs_D]);
    attList->addElement(fAttributes[Att_MinOccurs_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_CHOICE, prefixContext, attList);

    // element "sequence" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_MaxOccurs_D]);
    attList->addElement(fAttributes[Att_MinOccurs_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_SEQUENCE, prefixContext, attList);

    // for element "any" - local name
    attList = new RefVectorOf<AttributeInfo>(5, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_MaxOccurs_D]);
    attList->addElement(fAttributes[Att_MinOccurs_D]);
    attList->addElement(fAttributes[Att_Namespace_D]);
    attList->addElement(fAttributes[Att_Process_C_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ANY, prefixContext, attList);

    // element "simpleType" - local name
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_Final1_N]);
    attList->addElement(fAttributes[Att_ID_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_SIMPLETYPE, prefixContext, attList);

    // element "list" - local name
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_ItemType_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_LIST, prefixContext, attList);

    // element "union" - local name
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Member_T_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_UNION, prefixContext, attList);

    // element "length" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Value_NNI_N]);
    attList->addElement(fAttributes[Att_Fixed_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_LENGTH, prefixContext, attList);

    // element "minLength" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Value_NNI_N]);
    attList->addElement(fAttributes[Att_Fixed_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_MINLENGTH, prefixContext, attList);

    // element "maxLength" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Value_NNI_N]);
    attList->addElement(fAttributes[Att_Fixed_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_MAXLENGTH, prefixContext, attList);

    // element "totalDigits" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Value_NNI_N]);
    attList->addElement(fAttributes[Att_Fixed_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_TOTALDIGITS, prefixContext, attList);

    // element "fractionDigits" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Value_NNI_N]);
    attList->addElement(fAttributes[Att_Fixed_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_FRACTIONDIGITS, prefixContext, attList);

    // element "pattern" - local name
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Value_STR_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_PATTERN, prefixContext, attList);

    // element "enumeration" - local name
    attList = new RefVectorOf<AttributeInfo>(2, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Value_STR_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ENUMERATION, prefixContext, attList);

    // element "whiteSpace" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Value_WS_N]);
    attList->addElement(fAttributes[Att_Fixed_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_WHITESPACE, prefixContext, attList);

    // element "maxInclusive" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Value_STR_N]);
    attList->addElement(fAttributes[Att_Fixed_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_MAXINCLUSIVE, prefixContext, attList);

    // element "maxExclusive" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Value_STR_N]);
    attList->addElement(fAttributes[Att_Fixed_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_MAXEXCLUSIVE, prefixContext, attList);

    // for element "minInclusive" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Value_STR_N]);
    attList->addElement(fAttributes[Att_Fixed_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_MININCLUSIVE, prefixContext, attList);

    // for element "minExclusive" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_Value_STR_N]);
    attList->addElement(fAttributes[Att_Fixed_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_MINEXCLUSIVE, prefixContext, attList);

    // element "all" - local name
    attList = new RefVectorOf<AttributeInfo>(3, false);
    attList->addElement(fAttributes[Att_ID_N]);
    attList->addElement(fAttributes[Att_MaxOccurs1_D]);
    attList->addElement(fAttributes[Att_MinOccurs1_D]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ALL, prefixContext, attList);

    // element "annotation" - local name
    attList = new RefVectorOf<AttributeInfo>(1, false);
    attList->addElement(fAttributes[Att_ID_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_ANNOTATION, prefixContext, attList);

    // element "appinfo" - local name
    attList = new RefVectorOf<AttributeInfo>(1, false);
    attList->addElement(fAttributes[Att_Source_N]);
    fElementMap->put((void*) SchemaSymbols::fgELT_APPINFO, prefixContext, attList);

    // element "documentation" - local name
    attList = new RefVectorOf<AttributeInfo>(1, false);
    attList->addElement(fAttributes[Att_Source_N]);
    // xml:lang = language ???
    fElementMap->put((void*) SchemaSymbols::fgELT_DOCUMENTATION, prefixContext, attList);

    // element "unique" - local name

    // element "keyref" - local name

    // element "selector" - local name

    // element "field" - local name

}

// ---------------------------------------------------------------------------
//  GeneralAttributeCheck: CleanUp methods
// ---------------------------------------------------------------------------
void GeneralAttributeCheck::cleanUp() {

    for (unsigned int index = 0; index < Att_Count; index++) {
            delete fAttributes[index];
    }

    delete [] fAttributes;
    delete [] fValidators;
    delete fElementMap;
}

// ---------------------------------------------------------------------------
//  GeneralAttributeCheck: Instance methods
// ---------------------------------------------------------------------------
GeneralAttributeCheck* GeneralAttributeCheck::instance() {

    if (!fInstance) {

        fInstance = new GeneralAttributeCheck();
        XMLPlatformUtils::registerLazyData
        (
            new XMLDeleterFor<GeneralAttributeCheck>(fInstance)
        );
    }

    return fInstance;
}

// ---------------------------------------------------------------------------
//  GeneralAttributeCheck: Validation methods
// ---------------------------------------------------------------------------
void
GeneralAttributeCheck::checkAttributes(const DOM_Element& elem,
                                       const unsigned short elemContext,
                                       TraverseSchema* const schema) {

    if (elem == 0) {
        return;
    }

    DOMString                   name = elem.getLocalName();
    int                         prefixContext = globalPrefix;
    unsigned int                nameLen = name.length();
    XMLCh*                      elemName = 0;
    const XMLCh*                contextStr = fgGlobal;
    RefVectorOf<AttributeInfo>* elemAttrs = 0;

    if (nameLen) {
        elemName = new XMLCh[nameLen + 1];
        XMLString::copyNString(elemName, name.rawBuffer(), nameLen);
        elemName[nameLen] = chNull;
    }

    ArrayJanitor<XMLCh> janName(elemName);

    if (elemContext == LocalContext) {

        contextStr = fgLocal;

        if (elem.getAttribute(SchemaSymbols::fgATT_REF) == 0) {
            prefixContext = localNamePrefix;
        }
        else {
            prefixContext = localRefPrefix;
        }
    }

    elemAttrs = fElementMap->get(elemName, prefixContext);

    if (!elemAttrs) {

        // Try ref, some local declaration can have only a ref
        if (prefixContext == localNamePrefix) {
            elemAttrs = fElementMap->get(elemName, localRefPrefix);

            if (!elemAttrs) {
                return;
            }

            prefixContext = localRefPrefix;
        }
        else {        
            // We should report an error
            return;
        }
    }

    unsigned int           size = elemAttrs->size();
    RefHashTableOf<XMLCh>  attNameList(5);
    XMLBuffer              aBuffer(128);

    for (unsigned int i=0; i< size; i++) {

        AttributeInfo* attInfo = elemAttrs->elementAt(i);

        if (attInfo) {

            XMLCh* attName = attInfo->getName();
            DOMString attValue = elem.getAttribute(attName);
            unsigned int attValueLen = attValue.length();

            attNameList.put((void*) attName, 0);

            if (attValueLen > 0) {

                aBuffer.set(attValue.rawBuffer(), attValueLen);
                validate(attName, aBuffer.getRawBuffer(),
                         attInfo->getValidatorIndex(), schema);
            }
            else {
                if (attInfo->getDefaultOption() == Att_Required) {
                    schema->reportSchemaError(XMLUni::fgXMLErrDomain, 
                        XMLErrs::AttributeRequired, attName, contextStr, elemName);
                }
            }
        }
    }

    // ------------------------------------------------------------------
    // Check for disallowed attributes
    // ------------------------------------------------------------------
    DOM_NamedNodeMap eltAttrs = elem.getAttributes();
    int attrCount = eltAttrs.getLength();

    for (int j = 0; j < attrCount; j++) {

        DOM_Node  attribute = eltAttrs.item(j);

        if (attribute.isNull()) {
            break;
        }

        // Bypass attributes that start with xml 
        DOMString attName = attribute.getNodeName();
        aBuffer.set(attName.rawBuffer(), attName.length());
        XMLCh* tmpName = aBuffer.getRawBuffer();

        if ((*tmpName == chLatin_X || *tmpName == chLatin_x)
           && (*(tmpName+1) == chLatin_M || *(tmpName+1) == chLatin_m)
           && (*(tmpName+2) == chLatin_L || *(tmpName+2) == chLatin_l)) {
            continue;
        }

        attName = attribute.getLocalName();
        aBuffer.set(attName.rawBuffer(), attName.length());

        if (!attNameList.containsKey(aBuffer.getRawBuffer())) {
            schema->reportSchemaError(XMLUni::fgXMLErrDomain,
                XMLErrs::AttributeDisallowed, aBuffer.getRawBuffer(), contextStr, elemName);
        }
    }
}


void GeneralAttributeCheck::validate(const XMLCh* const attName,
                                     const XMLCh* const attValue,
                                     const short dvIndex,
                                     TraverseSchema* const schema)
{
    bool isInvalid = false;
    DatatypeValidator* dv = 0;

    switch (dvIndex) {
    case DT_Form:
        if (XMLString::compareString(attValue, SchemaSymbols::fgATTVAL_QUALIFIED) != 0
            && XMLString::compareString(attValue, SchemaSymbols::fgATTVAL_UNQUALIFIED) != 0) {
            isInvalid = true;
        }
        break;
    case DT_MaxOccurs:
            // maxOccurs = (nonNegativeInteger | unbounded)
        if (XMLString::compareString(attValue, fgUnbounded) != 0) {
            dv = fValidators[DT_NonNegInt];
        }
        break;
    case DT_MaxOccurs1:
        if (XMLString::compareString(attValue, fgValueOne) != 0) {
            isInvalid = true;
        }
        break;
    case DT_MinOccurs1:
        if (XMLString::compareString(attValue, fgValueZero) != 0 
            && XMLString::compareString(attValue, fgValueOne) != 0) {
            isInvalid = true;
        }
        break;
    case DT_ProcessContents:
        if (XMLString::compareString(attValue, SchemaSymbols::fgATTVAL_SKIP) != 0
            && XMLString::compareString(attValue, SchemaSymbols::fgATTVAL_LAX) != 0
            && XMLString::compareString(attValue, SchemaSymbols::fgATTVAL_STRICT) != 0) {
            isInvalid = true;
        }
        break;
    case DT_Use:
        if (XMLString::compareString(attValue, SchemaSymbols::fgATTVAL_OPTIONAL) != 0
            && XMLString::compareString(attValue, SchemaSymbols::fgATTVAL_PROHIBITED) != 0
            && XMLString::compareString(attValue, SchemaSymbols::fgATTVAL_REQUIRED) != 0) {
            isInvalid = true;
        }
        break;
    case DT_WhiteSpace:
        if (XMLString::compareString(attValue, SchemaSymbols::fgWS_PRESERVE) != 0
            && XMLString::compareString(attValue, SchemaSymbols::fgWS_REPLACE) != 0
            && XMLString::compareString(attValue, SchemaSymbols::fgWS_COLLAPSE) != 0) {
            isInvalid = true;
        }
        break;
    case DT_Boolean:
        dv = fValidators[DT_Boolean];
        break;
    case DT_NonNegInt:
        dv = fValidators[DT_NonNegInt];
        break;
    }

    if (dv) {
        try {
            dv->validate(attValue);
        }
        catch(...) {
            isInvalid = true;
        }
    }

    if (isInvalid) {
        schema->reportSchemaError(XMLUni::fgXMLErrDomain, XMLErrs::InvalidAttValue,
                                  attValue, attName);
    }
}

/**
  * End of file GeneralAttributeCheck.cpp
  */


