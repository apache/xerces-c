/*
 * Copyright 1999-2002,2004 The Apache Software Foundation.
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
 * $Id$
 */

#include "DOM_DocumentFragment.hpp"
#include "DocumentFragmentImpl.hpp"

XERCES_CPP_NAMESPACE_BEGIN


DOM_DocumentFragment::DOM_DocumentFragment()
: DOM_Node(null)
{
};


DOM_DocumentFragment::DOM_DocumentFragment(const DOM_DocumentFragment & other)
: DOM_Node(other)
{
};


DOM_DocumentFragment::DOM_DocumentFragment(DocumentFragmentImpl *impl) :
        DOM_Node(impl)
{
};


DOM_DocumentFragment::~DOM_DocumentFragment()
{
};


DOM_DocumentFragment & DOM_DocumentFragment::operator = (const DOM_DocumentFragment & other)
{
     return (DOM_DocumentFragment &) DOM_Node::operator = (other);
};


DOM_DocumentFragment & DOM_DocumentFragment::operator = (const DOM_NullPtr *other)
{
     return (DOM_DocumentFragment &) DOM_Node::operator = (other);
};


XERCES_CPP_NAMESPACE_END

