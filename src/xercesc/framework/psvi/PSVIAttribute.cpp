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

/*
 * $Log$
 * Revision 1.10  2004/09/08 13:56:07  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.9  2004/01/29 11:46:30  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.8  2003/12/30 05:58:56  neilg
 * allow schema normalized values to be associated with a PSVIAttribute after it is reset
 *
 * Revision 1.7  2003/12/02 17:37:03  neilg
 * fix compilation problem
 *
 * Revision 1.6  2003/12/02 17:31:42  neilg
 * since there are certain things, such as schemaLocation attributes, that have a datatype and which we nonetheless do not validate, make canonical-value production dependent on validity being valid
 *
 * Revision 1.5  2003/11/28 22:41:04  neilg
 * fix compilation error
 *
 * Revision 1.4  2003/11/28 20:20:54  neilg
 * make use of canonical representation in PSVIAttribute implementation
 *
 * Revision 1.3  2003/11/27 06:10:32  neilg
 * PSVIAttribute implementation
 *
 * Revision 1.2  2003/11/06 21:50:33  neilg
 * fix compilation errors under gcc 3.3.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/PSVIAttribute.hpp>

XERCES_CPP_NAMESPACE_BEGIN

PSVIAttribute::PSVIAttribute( MemoryManager* const manager ):  
        PSVIItem(manager)
        , fAttributeDecl(0)
        , fDV(0)
{
}

void PSVIAttribute::reset(
            const XMLCh * const         valContext
            , PSVIItem::VALIDITY_STATE  state
            , PSVIItem::ASSESSMENT_TYPE assessmentType
            , XSSimpleTypeDefinition *  validatingType
            , XSSimpleTypeDefinition *  memberType
            , const XMLCh * const       defaultValue
            , const bool                isSpecified
            , XSAttributeDeclaration *  attrDecl
            , DatatypeValidator *dv
        )
{
    fValidationContext = valContext;
    fValidityState = state;
    fAssessmentType = assessmentType;
    fType = validatingType;
    fMemberType = memberType;
    fDefaultValue = defaultValue;
    fIsSpecified = isSpecified;
    fMemoryManager->deallocate((void *)fCanonicalValue);
    fCanonicalValue = 0;
    fNormalizedValue = 0;
    fAttributeDecl = attrDecl;
    fDV = dv;
}

void PSVIAttribute::setValue(const XMLCh * const       normalizedValue)
{ 
    if(normalizedValue)
    {
        fNormalizedValue = normalizedValue;
        if(fDV && fValidityState == PSVIItem::VALIDITY_VALID)
            fCanonicalValue = (XMLCh *)fDV->getCanonicalRepresentation(normalizedValue, fMemoryManager);
    }
}

XERCES_CPP_NAMESPACE_END


