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
 * Revision 1.4  2004/10/20 15:18:49  knoaman
 * Allow option of initializing static data in XMLPlatformUtils::Initialize
 *
 * Revision 1.3  2004/09/08 13:56:47  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.2  2002/11/04 15:17:00  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:29  peiyongz
 * sane_include
 *
 * Revision 1.4  2001/05/11 13:26:45  tng
 * Copyright update.
 *
 * Revision 1.3  2001/05/03 18:17:33  knoaman
 * Some design changes:
 * o Changed the TokenFactory from a single static instance, to a
 *    normal class. Each RegularExpression object will have its own
 *    instance of TokenFactory, and that instance will be passed to
 *    other classes that need to use a TokenFactory to create Token
 *    objects (with the exception of RangeTokenMap).
 * o Added a new class RangeTokenMap to map a the different ranges
 *    in a given category to a specific RangeFactory object. In the old
 *    design RangeFactory had dual functionality (act as a Map, and as
 *    a factory for creating RangeToken(s)). The RangeTokenMap will
 *    have its own copy of the TokenFactory. There will be only one
 *    instance of the RangeTokenMap class, and that instance will be
 *    lazily deleted when XPlatformUtils::Terminate is called.
 *
 * Revision 1.2  2001/03/22 13:23:32  knoaman
 * Minor modifications to eliminate compiler warnings.
 *
 * Revision 1.1  2001/03/02 19:26:44  knoaman
 * Schema: Regular expression handling part II
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/RangeFactory.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  RangeFactory: Constructors and Destructor
// ---------------------------------------------------------------------------
RangeFactory::RangeFactory() :
   fRangesCreated(false)
 , fKeywordsInitialized(false)
{

}

RangeFactory::~RangeFactory() {

}

XERCES_CPP_NAMESPACE_END

/**
  * End of file RangeFactory.cpp
  */
