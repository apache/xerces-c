/*
 * Copyright 2001,2004 The Apache Software Foundation.
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
 * Revision 1.6  2004/09/08 13:56:59  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/10/01 16:32:42  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.4  2003/05/18 14:02:09  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.3  2003/05/15 18:59:34  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 14:47:41  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:51  peiyongz
 * sane_include
 *
 * Revision 1.1  2001/11/02 14:08:40  knoaman
 * Add support for identity constraints.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/validators/schema/identity/XPathMatcherStack.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XPathMatherStack: Constructors and Destructor
// ---------------------------------------------------------------------------
XPathMatcherStack::XPathMatcherStack(MemoryManager* const manager)
    : fMatchersCount(0)
    , fContextStack(new (manager) ValueStackOf<int>(8, manager))
    , fMatchers(0)
{
    try {
        fMatchers = new (manager) RefVectorOf<XPathMatcher>(8, true, manager);
    }
    catch(const OutOfMemoryException&)
    {
        throw;
    }
    catch(...) {

        delete fContextStack;
        throw;
    }
}

XPathMatcherStack::~XPathMatcherStack() {

    delete fContextStack;
    delete fMatchers;
}

// ---------------------------------------------------------------------------
//  XPathMatherStack: Clear methods
// ---------------------------------------------------------------------------
void XPathMatcherStack::clear() {

    fContextStack->removeAllElements();
    fMatchers->removeAllElements();
    fMatchersCount = 0;
}

XERCES_CPP_NAMESPACE_END

/**
  * End of file XPathMatcherStack.cpp
  */

