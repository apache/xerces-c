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
 * Revision 1.6  2004/09/08 13:56:09  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/11/24 10:30:53  gareth
 * fixed typo in include
 *
 * Revision 1.4  2003/11/21 17:34:04  knoaman
 * PSVI update
 *
 * Revision 1.3  2003/11/14 22:47:53  neilg
 * fix bogus log message from previous commit...
 *
 * Revision 1.2  2003/11/14 22:33:30  neilg
 * Second phase of schema component model implementation.  
 * Implement XSModel, XSNamespaceItem, and the plumbing necessary
 * to connect them to the other components.
 * Thanks to David Cargill.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#include <xercesc/framework/psvi/XSParticle.hpp>
#include <xercesc/framework/psvi/XSElementDeclaration.hpp>
#include <xercesc/framework/psvi/XSModelGroup.hpp>
#include <xercesc/framework/psvi/XSWildcard.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XSParticle: Constructors and Destructor
// ---------------------------------------------------------------------------
XSParticle::XSParticle(TERM_TYPE            termType,
                       XSModel* const       xsModel,
                       XSObject* const      particleTerm,
                       int                  minOccurs,
                       int                  maxOccurs,
                       MemoryManager* const manager)
    : XSObject(XSConstants::PARTICLE, xsModel, manager)
    , fTermType(termType)
    , fMinOccurs(minOccurs)
    , fMaxOccurs(maxOccurs)
    , fTerm(particleTerm)
{
}

XSParticle::~XSParticle()
{
    if (fTerm && (fTermType == TERM_MODELGROUP))
        delete ((XSModelGroup*) fTerm);
}

// ---------------------------------------------------------------------------
//  XSParticle: methods
// ---------------------------------------------------------------------------
XSElementDeclaration *XSParticle::getElementTerm()
{
    if (fTermType == TERM_ELEMENT)
        return (XSElementDeclaration*) fTerm;

    return 0;
}

XSModelGroup *XSParticle::getModelGroupTerm()
{
    if (fTermType == TERM_MODELGROUP)
        return (XSModelGroup*) fTerm;

    return 0;
}

XSWildcard *XSParticle::getWildcardTerm()
{
    if (fTermType == TERM_WILDCARD)
        return (XSWildcard*) fTerm;

    return 0;
}

XERCES_CPP_NAMESPACE_END


