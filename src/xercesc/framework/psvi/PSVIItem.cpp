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
 * Revision 1.6  2004/09/08 13:56:07  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/11/28 21:18:31  knoaman
 * Make use of canonical representation in PSVIElement
 *
 * Revision 1.4  2003/11/25 16:14:28  neilg
 * move inlines into PSVIItem.hpp
 *
 * Revision 1.3  2003/11/21 22:34:45  neilg
 * More schema component model implementation, thanks to David Cargill.
 * In particular, this cleans up and completes the XSModel, XSNamespaceItem,
 * XSAttributeDeclaration and XSAttributeGroup implementations.
 *
 * Revision 1.2  2003/09/17 17:45:37  neilg
 * remove spurious inlines; hopefully this will make Solaris/AIX compilers happy.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/PSVIItem.hpp>

XERCES_CPP_NAMESPACE_BEGIN

PSVIItem::PSVIItem( MemoryManager* const manager ):  
        fMemoryManager(manager),
        fValidationContext(0),
        fNormalizedValue(0),
        fDefaultValue(0),
        fCanonicalValue(0),
        fValidityState(VALIDITY_NOTKNOWN),
        fAssessmentType(VALIDATION_FULL),
        fIsSpecified(false),
        fType(0),
        fMemberType(0)
{
}

void PSVIItem::reset(
            const XMLCh* const validationContext
            , const XMLCh* const normalizedValue
            , const VALIDITY_STATE validityState
            , const ASSESSMENT_TYPE assessmentType
        )
{
    // this is just a wrapper method; fValidationContext will
    // be valid as long as and no longer than the thing to which
    // validationContext points
    fValidationContext = validationContext;
    fNormalizedValue = normalizedValue;
    fValidityState = validityState;
    fAssessmentType = assessmentType;
}

void PSVIItem::setValidationAttempted(PSVIItem::ASSESSMENT_TYPE attemptType)
{
    fAssessmentType = attemptType;
}
 
void PSVIItem::setValidity(PSVIItem::VALIDITY_STATE validity)
{
    fValidityState = validity;
}

XERCES_CPP_NAMESPACE_END


