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
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.9  2003/10/01 16:32:42  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.8  2003/05/18 14:02:09  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.7  2003/05/15 18:59:34  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.6  2003/01/13 20:16:52  knoaman
 * [Bug 16024] SchemaSymbols.hpp conflicts C++ Builder 6 dir.h
 *
 * Revision 1.5  2003/01/13 16:30:19  knoaman
 * [Bug 14469] Validator doesn't enforce xsd:key.
 *
 * Revision 1.4  2002/11/04 14:47:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.3  2002/08/27 05:56:19  knoaman
 * Identity Constraint: handle case of recursive elements.
 *
 * Revision 1.2  2002/02/18 06:26:50  jberry
 * Quiet codewarrior compiler warnings
 *
 * Revision 1.1.1.1  2002/02/01 22:22:51  peiyongz
 * sane_include
 *
 * Revision 1.2  2001/11/15 17:10:19  knoaman
 * Particle derivation checking support.
 *
 * Revision 1.1  2001/11/02 14:08:40  knoaman
 * Add support for identity constraints.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/identity/XPathMatcher.hpp>
#include <xercesc/validators/schema/identity/XercesXPath.hpp>
#include <xercesc/validators/schema/SchemaElementDecl.hpp>
#include <xercesc/validators/schema/SchemaAttDef.hpp>
#include <xercesc/validators/schema/SchemaSymbols.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XPathMatcher: Constructors and Destructor
// ---------------------------------------------------------------------------
XPathMatcher::XPathMatcher( XercesXPath* const xpath
                          , MemoryManager* const manager)
    : fLocationPathSize(0)
    , fMatched(0)
    , fNoMatchDepth(0)
    , fCurrentStep(0)
    , fStepIndexes(0)
    , fLocationPaths(0)
    , fIdentityConstraint(0)
    , fMemoryManager(manager)
{
    try {
        init(xpath);
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...) {

        cleanUp();
        throw;
    }
}


XPathMatcher::XPathMatcher(XercesXPath* const xpath,
                           IdentityConstraint* const ic,
						   MemoryManager* const manager)
    : fLocationPathSize(0)
    , fMatched(0)
    , fNoMatchDepth(0)
    , fCurrentStep(0)
    , fStepIndexes(0)
    , fLocationPaths(0)
    , fIdentityConstraint(ic)
    , fMemoryManager(manager)
{
    try {
        init(xpath);
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...) {

        cleanUp();
        throw;
    }
}


XPathMatcher::~XPathMatcher()
{
    cleanUp();
}

// ---------------------------------------------------------------------------
//  XPathMatcher: Helper methods
// ---------------------------------------------------------------------------
void XPathMatcher::init(XercesXPath* const xpath) {

    if (xpath) {

        fLocationPaths = xpath->getLocationPaths();
        fLocationPathSize = (fLocationPaths ? fLocationPaths->size() : 0);

        if (fLocationPathSize) {

            fStepIndexes = new (fMemoryManager) RefVectorOf<ValueStackOf<int> >(fLocationPathSize, true, fMemoryManager);
            fCurrentStep = (int*) fMemoryManager->allocate
            (
                fLocationPathSize * sizeof(int)
            );//new int[fLocationPathSize];
            fNoMatchDepth = (int*) fMemoryManager->allocate
            (
                fLocationPathSize * sizeof(int)
            );//new int[fLocationPathSize];
            fMatched = (int*) fMemoryManager->allocate
            (
                fLocationPathSize * sizeof(int)
            );//new int[fLocationPathSize];

            for(unsigned int i=0; i < fLocationPathSize; i++) {
                fStepIndexes->addElement(new (fMemoryManager) ValueStackOf<int>(8, fMemoryManager));
            }
        }
    }
}


// ---------------------------------------------------------------------------
//  XPathMatcher: XMLDocumentHandler methods
// ---------------------------------------------------------------------------
void XPathMatcher::startDocumentFragment() {

    for(unsigned int i = 0; i < fLocationPathSize; i++) {

        fStepIndexes->elementAt(i)->removeAllElements();
        fCurrentStep[i] = 0;
        fNoMatchDepth[i] = 0;
        fMatched[i] = 0;
    }
}

void XPathMatcher::startElement(const XMLElementDecl& elemDecl,
                                const unsigned int urlId,
                                const XMLCh* const elemPrefix,
								const RefVectorOf<XMLAttr>& attrList,
                                const unsigned int attrCount) {

    for (int i = 0; i < (int) fLocationPathSize; i++) {

        // push context
        int startStep = fCurrentStep[i];
        fStepIndexes->elementAt(i)->push(startStep);

        // try next xpath, if not matching
        if ((fMatched[i] & XP_MATCHED_D) == XP_MATCHED || fNoMatchDepth[i] > 0) {
            fNoMatchDepth[i]++;
            continue;
        }

        if((fMatched[i] & XP_MATCHED_D) == XP_MATCHED_D) {
            fMatched[i] = XP_MATCHED_DP;
        }

        // consume self::node() steps
        XercesLocationPath* locPath = fLocationPaths->elementAt(i);
        int stepSize = locPath->getStepSize();

        while (fCurrentStep[i] < stepSize &&
               locPath->getStep(fCurrentStep[i])->getAxisType() == XercesStep::SELF) {
            fCurrentStep[i]++;
        }

        if (fCurrentStep[i] == stepSize) {

            fMatched[i] = XP_MATCHED;
            continue;
        }

        // now if the current step is a descendant step, we let the next
        // step do its thing; if it fails, we reset ourselves
        // to look at this step for next time we're called.
        // so first consume all descendants:
        int descendantStep = fCurrentStep[i];

        while (fCurrentStep[i] < stepSize &&
               locPath->getStep(fCurrentStep[i])->getAxisType() == XercesStep::DESCENDANT) {
            fCurrentStep[i]++;
        }

        bool sawDescendant = fCurrentStep[i] > descendantStep;
        if (fCurrentStep[i] == stepSize) {

            fNoMatchDepth[i]++;
            continue;
        }

        // match child::... step, if haven't consumed any self::node()
        if ((fCurrentStep[i] == startStep || fCurrentStep[i] > descendantStep) &&
            locPath->getStep(fCurrentStep[i])->getAxisType() == XercesStep::CHILD) {

            XercesStep* step = locPath->getStep(fCurrentStep[i]);
            XercesNodeTest* nodeTest = step->getNodeTest();

            if (nodeTest->getType() == XercesNodeTest::QNAME) {

                QName elemQName(elemPrefix, elemDecl.getElementName()->getLocalPart(), urlId, fMemoryManager);

//                if (!(*(nodeTest->getName()) == *(elemDecl.getElementName()))) {
                if (!(*(nodeTest->getName()) == elemQName)) {

                    if(fCurrentStep[i] > descendantStep) {
                        fCurrentStep[i] = descendantStep;
                        continue;
                    }

                    fNoMatchDepth[i]++;
                    continue;
                }
            }

            fCurrentStep[i]++;
        }

        if (fCurrentStep[i] == stepSize) {

            if (sawDescendant) {

                fCurrentStep[i] = descendantStep;
                fMatched[i] = XP_MATCHED_D;
            }
            else {
                fMatched[i] = XP_MATCHED;
            }

            continue;
        }

        // match attribute::... step
        if (fCurrentStep[i] < stepSize &&
            locPath->getStep(fCurrentStep[i])->getAxisType() == XercesStep::ATTRIBUTE) {

            if (attrCount) {

                XercesNodeTest* nodeTest = locPath->getStep(fCurrentStep[i])->getNodeTest();

                for (unsigned int attrIndex = 0; attrIndex < attrCount; attrIndex++) {

                    const XMLAttr* curDef = attrList.elementAt(attrIndex);

                    if (nodeTest->getType() != XercesNodeTest::QNAME ||
                        (*(nodeTest->getName()) == *(curDef->getAttName()))) {

                        fCurrentStep[i]++;

                        if (fCurrentStep[i] == stepSize) {

                            fMatched[i] = XP_MATCHED_A;
                            int j=0;

                            for(; j<i && ((fMatched[j] & XP_MATCHED) != XP_MATCHED); j++) ;

                            if(j == i) {

                                SchemaAttDef* attDef = ((SchemaElementDecl&) elemDecl).getAttDef(curDef->getName(), curDef->getURIId());
                                DatatypeValidator* dv = (attDef) ? attDef->getDatatypeValidator() : 0;
                                matched(curDef->getValue(), dv, false);
                            }
                        }
                        break;
                    }
                }
            }

            if ((fMatched[i] & XP_MATCHED) != XP_MATCHED) {

                if(fCurrentStep[i] > descendantStep) {

                    fCurrentStep[i] = descendantStep;
                    continue;
                }

                fNoMatchDepth[i]++;
            }
        }
    }
}

void XPathMatcher::endElement(const XMLElementDecl& elemDecl,
                              const XMLCh* const elemContent) {

    for(int i = 0; i < (int) fLocationPathSize; i++) {

        // go back a step
        fCurrentStep[i] = fStepIndexes->elementAt(i)->pop();

        // don't do anything, if not matching
        if (fNoMatchDepth[i] > 0) {
            fNoMatchDepth[i]--;
        }
        // signal match, if appropriate
        else {

            int j=0;
            for(; j<i && ((fMatched[j] & XP_MATCHED) != XP_MATCHED); j++) ;

            if (j < i || (fMatched[j] == 0)
                || ((fMatched[j] & XP_MATCHED_A) == XP_MATCHED_A))
				continue;

            DatatypeValidator* dv = ((SchemaElementDecl*) &elemDecl)->getDatatypeValidator();
            bool isNillable = (((SchemaElementDecl *) &elemDecl)->getMiscFlags() & SchemaSymbols::XSD_NILLABLE) != 0;

            matched(elemContent, dv, isNillable);
            fMatched[i] = 0;
        }
    }
}


// ---------------------------------------------------------------------------
//  XPathMatcher: Match methods
// ---------------------------------------------------------------------------
int XPathMatcher::isMatched() {

    // xpath has been matched if any one of the members of the union have matched.
    for (int i=0; i < (int) fLocationPathSize; i++) {
        if (((fMatched[i] & XP_MATCHED) == XP_MATCHED)
            && ((fMatched[i] & XP_MATCHED_DP) != XP_MATCHED_DP))
            return fMatched[i];
    }

    return 0;
}

void XPathMatcher::matched(const XMLCh* const content,
                           DatatypeValidator* const dv,
                           const bool isNil) {
    return;
}


// ---------------------------------------------------------------------------
//  XPathMatcher: Match methods
// ---------------------------------------------------------------------------
int XPathMatcher::getInitialDepth() const
{
    ThrowXML(RuntimeException, XMLExcepts::Regex_NotSupported);
    return 0; // to make some compilers happy
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file XPathMatcher.cpp
  */

