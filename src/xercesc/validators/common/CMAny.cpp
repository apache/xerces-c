/*
 * Copyright 1999-2001,2004 The Apache Software Foundation.
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
 * Revision 1.5  2004/09/08 13:56:51  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/12/17 00:18:38  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.3  2003/05/15 18:48:27  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 14:54:58  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:37  peiyongz
 * sane_include
 *
 * Revision 1.6  2001/08/08 13:23:27  knoaman
 * Fix condition check.
 *
 * Revision 1.5  2001/07/09 15:22:35  knoaman
 * complete <any> declaration.
 *
 * Revision 1.4  2001/06/07 20:58:38  tng
 * Fix no newline at the end warning.  By Pei Yong Zhang.
 *
 * Revision 1.3  2001/05/11 13:27:14  tng
 * Copyright update.
 *
 * Revision 1.2  2001/05/03 21:02:27  tng
 * Schema: Add SubstitutionGroupComparator and update exception messages.  By Pei Yong Zhang.
 *
 * Revision 1.1  2001/02/27 14:48:45  tng
 * Schema: Add CMAny and ContentLeafNameTypeVector, by Pei Yong Zhang
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/validators/common/CMStateSet.hpp>
#include <xercesc/validators/common/CMAny.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  CMUnaryOp: Constructors and Destructor
// ---------------------------------------------------------------------------
CMAny::CMAny( const ContentSpecNode::NodeTypes type
            , const unsigned int               URI
            , const unsigned int               position
            ,       MemoryManager* const       manager) :
       CMNode(type, manager)
     , fURI(URI)
     , fPosition(position)
{
    if ((type & 0x0f) != ContentSpecNode::Any
    &&  (type & 0x0f) != ContentSpecNode::Any_Other
    &&  (type & 0x0f) != ContentSpecNode::Any_NS)
    {
		ThrowXMLwithMemMgr1(RuntimeException,
		          XMLExcepts::CM_NotValidSpecTypeForNode,
				  "CMAny", manager);
    }

}

CMAny::~CMAny()
{
}

// ---------------------------------------------------------------------------
//  Getter methods
// ---------------------------------------------------------------------------
unsigned int CMAny::getURI() const
{
	return fURI;
}

unsigned int CMAny::getPosition() const
{
    return fPosition;
}

// ---------------------------------------------------------------------------
//  Setter methods
// ---------------------------------------------------------------------------
void CMAny::setPosition(const unsigned int newPosition)
{
    fPosition = newPosition;
}

// ---------------------------------------------------------------------------
//  Implementation of public CMNode virtual interface
// ---------------------------------------------------------------------------
bool CMAny::isNullable() const
{
    // Leaf nodes are never nullable unless its an epsilon node
    return (fPosition == -1);
}

// ---------------------------------------------------------------------------
//  Implementation of protected CMNode virtual interface
// ---------------------------------------------------------------------------
void CMAny::calcFirstPos(CMStateSet& toSet) const
{
    // If we are an epsilon node, then the first pos is an empty set
    if (fPosition == -1)
        toSet.zeroBits();
    else
    // Otherwise, its just the one bit of our position
        toSet.setBit(fPosition);

	return;
}

void CMAny::calcLastPos(CMStateSet& toSet) const
{
    // If we are an epsilon node, then the last pos is an empty set
    if (fPosition == -1)
        toSet.zeroBits();
    // Otherwise, its just the one bit of our position
    else
        toSet.setBit(fPosition);

	return;
}

XERCES_CPP_NAMESPACE_END
